/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
  Contact author        : ufoot@ufoot.org
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <errno.h>
#include <math.h>

#include "mod-caca-internal.h"

enum action
{ PREPARE, INIT, UPDATE, RENDER, FREE };

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define XSIZ 256
#define YSIZ 256

#define TABLEX (XSIZ * 2)
#define TABLEY (YSIZ * 2)
static uint8_t table[TABLEX * TABLEY];

static int frame = 0;

static void _do_plasma (lw6sys_context_t *, uint8_t *, double, double, double, double, double, double);

static void
_plasma (lw6sys_context_t * sys_context, enum action action, caca_canvas_t * cv)
{
  static caca_dither_t *dither;
  static uint8_t *screen;
  static uint32_t red[256], green[256], blue[256], alpha[256];
  static double r[3], R[6];

  int i, x, y;

  switch (action)
    {
    case PREPARE:
      /* Fill various tables */
      for (i = 0; i < 256; i++)
	red[i] = green[i] = blue[i] = alpha[i] = 0;

      for (i = 0; i < 3; i++)
	r[i] = (double) (caca_rand (1, 1000)) / 60000 * M_PI;

      for (i = 0; i < 6; i++)
	R[i] = (double) (caca_rand (1, 1000)) / 10000;

      for (y = 0; y < TABLEY; y++)
	for (x = 0; x < TABLEX; x++)
	  {
	    double tmp = (((double) ((x - (TABLEX / 2)) * (x - (TABLEX / 2))
				     + (y - (TABLEX / 2)) * (y - (TABLEX / 2)))) * (M_PI / (TABLEX * TABLEX + TABLEY * TABLEY)));

	    table[x + y * TABLEX] = (1.0 + sin (12.0 * sqrt (tmp))) * 256 / 6;
	  }
      break;

    case INIT:
      screen = malloc (XSIZ * YSIZ * sizeof (uint8_t));
      dither = caca_create_dither (8, XSIZ, YSIZ, XSIZ, 0, 0, 0, 0);
      break;

    case UPDATE:
      for (i = 0; i < 256; i++)
	{
	  double z = ((double) i) / 256 * 6 * M_PI;

	  red[i] = (1.0 + sin (z + r[1] * frame)) / 2 * 0xfff;
	  blue[i] = (1.0 + cos (z + r[0] * (frame + 100))) / 2 * 0xfff;
	  green[i] = (1.0 + cos (z + r[2] * (frame + 200))) / 2 * 0xfff;
	}

      /* Set the palette */
      caca_set_dither_palette (dither, red, green, blue, alpha);

      _do_plasma (sys_context, screen,
		  (1.0 + sin (((double) frame) * R[0])) / 2,
		  (1.0 + sin (((double) frame) * R[1])) / 2,
		  (1.0 + sin (((double) frame) * R[2])) / 2,
		  (1.0 + sin (((double) frame) * R[3])) / 2, (1.0 + sin (((double) frame) * R[4])) / 2, (1.0 + sin (((double) frame) * R[5])) / 2);
      break;

    case RENDER:
      caca_dither_bitmap (cv, 0, 0, caca_get_canvas_width (cv), caca_get_canvas_height (cv), dither, screen);
      break;

    case FREE:
      free (screen);
      caca_free_dither (dither);
      break;
    }
}

static void
_do_plasma (lw6sys_context_t * sys_context, uint8_t * pixels, double x_1, double y_1, double x_2, double y_2, double x_3, double y_3)
{
  unsigned int X1 = x_1 * (TABLEX / 2),
    Y1 = y_1 * (TABLEY / 2), X2 = x_2 * (TABLEX / 2), Y2 = y_2 * (TABLEY / 2), X3 = x_3 * (TABLEX / 2), Y3 = y_3 * (TABLEY / 2);
  unsigned int y;
  uint8_t *t1 = table + X1 + Y1 * TABLEX, *t2 = table + X2 + Y2 * TABLEX, *t3 = table + X3 + Y3 * TABLEX;

  for (y = 0; y < YSIZ; y++)
    {
      uint8_t *tmp = pixels + y * YSIZ;
      unsigned int ty = y * TABLEX, tmax = ty + XSIZ;
      for (; ty < tmax; ty++, tmp++)
	tmp[0] = t1[ty] + t2[ty] + t3[ty];
    }
}

static void
_increment_frame (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, caca_font_t * fo, caca_dither_t * di, uint8_t * buff)
{
  int wc, hc;
  static caca_font_t *f = NULL;
  static caca_dither_t *d = NULL;
  static uint8_t *buf = NULL;

  if (fo != NULL)
    f = fo;
  if (di != NULL)
    d = di;
  if (buff != NULL)
    buf = buff;
  hc = caca_get_canvas_height (caca_context->canvas);
  wc = caca_get_canvas_width (caca_context->canvas);
  _plasma (sys_context, UPDATE, caca_context->canvas);
  _plasma (sys_context, RENDER, caca_context->canvas);

  caca_set_color_ansi (caca_context->canvas, CACA_WHITE, CACA_BLACK);
  caca_dither_bitmap (caca_context->canvas, (wc - (10 * caca_get_font_width (f))) / 2, (hc - caca_get_font_height (f)) / 2, wc, hc, d, buf);
  caca_put_str (caca_context->canvas, wc - 8, 0, "mod-caca");
  caca_put_str (caca_context->canvas, 0, 0, "PFA Epitech 2012-2013");
  caca_put_str (caca_context->canvas, (wc - 41) / 2, hc - 1, "by france_a, clavel_r, lemonn_k, vougie_c");
  caca_refresh_display (caca_context->display);
  frame++;
}

void
_mod_caca_splash_free (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, caca_font_t * fo, caca_dither_t * di, uint8_t * buff, int init)
{
  static caca_font_t *f = NULL;
  static caca_dither_t *d = NULL;
  static uint8_t *buf = NULL;

  if (init == 1)
    {
      f = fo;
      d = di;
      buf = buff;
    }
  if (init == 0)
    {
      _plasma (sys_context, FREE, caca_context->canvas);
      free (buf);
      caca_free_dither (d);
      caca_free_font (f);
    }
}

void
_mod_caca_plasma_anim (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context)
{
  if (frame != 0)
    {
      _increment_frame (sys_context, caca_context, NULL, NULL, NULL);
      return;
    }
  int wc, hc;
  caca_canvas_t *cv = NULL;
  caca_font_t *f = NULL;
  caca_dither_t *d = NULL;
  uint8_t *buf = NULL;
  char const *const *fonts;

  cv = caca_create_canvas (10, 1);
  if (cv == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("Can't create tmp canvas"));
      return;
    }
  hc = caca_get_canvas_height (caca_context->canvas);
  wc = caca_get_canvas_width (caca_context->canvas);
  _plasma (sys_context, PREPARE, caca_context->canvas);
  _plasma (sys_context, INIT, caca_context->canvas);

  caca_set_color_ansi (cv, CACA_BLACK, CACA_TRANSPARENT);
  caca_put_str (cv, 0, 0, "LiquidWar6");
  fonts = caca_get_font_list ();
  if (fonts[0] == NULL || fonts[1] == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("libcaca was compiled without any fonts"));
      return;
    }
  f = caca_load_font (fonts[1], 0);
  if (f == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("could not load font : '%s'"), fonts[0]);
      return;
    }
  buf = malloc (4 * wc * hc);
  if (buf == NULL)
    return;
  caca_render_canvas (cv, f, buf, wc, hc, 4 * wc);
  d = caca_create_dither (32, wc, hc, 4 * wc, 0xff00, 0xff0000, 0xff000000, 0xff);
  _increment_frame (sys_context, caca_context, f, d, buf);
  _mod_caca_splash_free (sys_context, caca_context, f, d, buf, 1);
  caca_free_canvas (cv);
}
