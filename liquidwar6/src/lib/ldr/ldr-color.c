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
#endif

#include "ldr.h"
#include "ldr-internal.h"

#include <math.h>

#define GLOBAL_BEGIN 0.0f
#define GLOBAL_END 1.0f
#define DARK_BEGIN 0.0f
#define DARK_END 0.25f
#define LIGHT_BEGIN 0.75f
#define LIGHT_END 1.0f
#define DARKER_COEF 0.66f
#define LIGHTER_COEF 0.66f
#define MIN_STEP 5
#define MAX_STEP_NB 50
#define SWAP_REGULAR_GOOFY_MAJ 1.5f
#define SWAP_BASE_ALTERNATE_MAJ 2.0f

typedef struct color_info_s
{
  lw6sys_color_8_t color_8;
  lw6sys_color_hsv_t color_hsv;
} color_info_t;

static int
_compare_color_info (const void *p1, const void *p2)
{
  int ret = 0;

  color_info_t *c1 = (color_info_t *) p1;
  color_info_t *c2 = (color_info_t *) p2;

  ret = (c1->color_hsv.v > c2->color_hsv.v) - (c1->color_hsv.v < c2->color_hsv.v);

  return ret;
}

static void
_average_color (lw6sys_context_t * sys_context, color_info_t * ret, int size, color_info_t * color_info, float begin, float end)
{
  int i1, i2, n, i;
  lw6sys_color_8_t *buf = NULL;

  ret->color_8 = LW6SYS_COLOR_8_BLACK;

  i2 = lw6sys_imax (0, lw6sys_imin (size - 1, end * size));
  i1 = lw6sys_imax (0, lw6sys_imin (i2, begin * size));
  n = i2 - i1;

  if (n > 0)
    {
      buf = (lw6sys_color_8_t *) LW6SYS_MALLOC (sys_context, n * sizeof (lw6sys_color_8_t));
      if (buf)
	{
	  for (i = 0; i < n; ++i)
	    {
	      buf[i] = color_info[i1 + i].color_8;
	    }
	  ret->color_8 = lw6sys_color_average (sys_context, n, buf);
	  LW6SYS_FREE (sys_context, buf);
	}
    }

  lw6sys_color_rgb_to_hsv (sys_context, &ret->color_hsv, ret->color_8);
}

static void
_set_to_invert (lw6sys_context_t * sys_context, color_info_t * dst, color_info_t * src)
{
  dst->color_hsv = src->color_hsv;
  lw6sys_color_hsv_invert (sys_context, &(dst->color_hsv), 1, 0, 0);
  dst->color_8 = lw6sys_color_hsv_to_rgb (sys_context, &(dst->color_hsv));
}

int
_lw6ldr_guess_colors (lw6sys_context_t * sys_context, lw6map_level_t * level, lw6sys_progress_t * progress)
{
  int ret = 0;
  int x, y, i_bg = 0, i_fg = 0;
  int ok_bg = 0;
  int ok_fg = 0;
  int size;
  int step;
  color_info_t *bg = NULL;
  color_info_t *fg = NULL;
  color_info_t global_bg;
  color_info_t dark_bg;
  color_info_t light_bg;
  color_info_t global_fg;
  color_info_t dark_fg;
  color_info_t light_fg;
  color_info_t base_bg;
  color_info_t base_fg;
  color_info_t alternate_bg;
  color_info_t alternate_fg;
  color_info_t tmp;
  float distance_regular;
  float distance_goofy;
  float distance_base;
  float distance_alternate;

  step = lw6sys_imax (MIN_STEP, (int) sqrt ((((float) level->texture.w) * ((float) level->texture.h)) / (MAX_STEP_NB * MAX_STEP_NB)));
  size = ((level->texture.w + 1) * (level->texture.h + 1)) / (step * step);
  if (size > 0)
    {
      bg = (color_info_t *) LW6SYS_MALLOC (sys_context, sizeof (color_info_t) * size);
      if (bg)
	{
	  fg = (color_info_t *) LW6SYS_MALLOC (sys_context, sizeof (color_info_t) * size);
	  if (fg)
	    {
	      for (y = 0; y < level->texture.h && i_bg < size && i_fg < size; y += step)
		{
		  lw6sys_progress_update (sys_context, progress, 0, level->texture.h, y);
		  for (x = 0; x < level->texture.w && i_bg < size && i_fg < size; x += step)
		    {
		      if (lw6map_body_get_with_texture_coord (sys_context, level, x, y, 0) > 0)
			{
			  bg[i_bg].color_8 = lw6map_texture_get (&level->texture, x, y);
			  lw6sys_color_rgb_to_hsv (sys_context, &(bg[i_bg].color_hsv), bg[i_bg].color_8);
			  lw6sys_color_8_solid (&(bg[i_bg].color_8));
			  i_bg++;
			}
		      else
			{
			  fg[i_fg].color_8 = lw6map_texture_get (&level->texture, x, y);
			  lw6sys_color_rgb_to_hsv (sys_context, &(fg[i_fg].color_hsv), fg[i_fg].color_8);
			  lw6sys_color_8_solid (&(fg[i_fg].color_8));
			  i_fg++;
			}
		    }
		}

	      if (i_bg > 0)
		{
		  qsort (bg, i_bg, sizeof (color_info_t), _compare_color_info);
		  _average_color (sys_context, &global_bg, i_bg, bg, GLOBAL_BEGIN, GLOBAL_END);
		  _average_color (sys_context, &dark_bg, i_bg, bg, DARK_BEGIN, DARK_END);
		  _average_color (sys_context, &light_bg, i_bg, bg, LIGHT_BEGIN, LIGHT_END);
		  ok_bg = 1;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("no background color found, unable to guess color"));
		}
	      if (i_fg > 0)
		{
		  qsort (fg, i_fg, sizeof (color_info_t), _compare_color_info);
		  _average_color (sys_context, &global_fg, i_fg, fg, GLOBAL_BEGIN, GLOBAL_END);
		  _average_color (sys_context, &dark_fg, i_fg, fg, DARK_BEGIN, DARK_END);
		  _average_color (sys_context, &light_fg, i_fg, fg, LIGHT_BEGIN, LIGHT_END);
		  ok_fg = 1;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("no foreground color found, unable to guess color"));
		}

	      if (ok_fg && !ok_bg)
		{
		  _set_to_invert (sys_context, &global_bg, &global_fg);
		  _set_to_invert (sys_context, &dark_bg, &dark_fg);
		  _set_to_invert (sys_context, &light_bg, &light_fg);
		  ok_bg = 1;
		}

	      if (ok_bg && !ok_fg)
		{
		  _set_to_invert (sys_context, &global_fg, &global_bg);
		  _set_to_invert (sys_context, &dark_fg, &dark_bg);
		  _set_to_invert (sys_context, &light_fg, &light_bg);
		  ok_fg = 1;
		}

	      if (ok_fg && ok_bg)
		{
		  ret = 1;
		  distance_regular = lw6sys_imin (lw6sys_color_distance
						  (sys_context, light_bg.color_8, light_fg.color_8), lw6sys_color_distance (sys_context, dark_bg.color_8,
															    dark_fg.color_8));
		  distance_goofy =
		    lw6sys_imin (lw6sys_color_distance (sys_context, light_bg.color_8, dark_fg.color_8),
				 lw6sys_color_distance (sys_context, dark_bg.color_8, light_fg.color_8));
		  base_bg = dark_bg;
		  alternate_bg = light_bg;
		  if (distance_regular < distance_goofy * SWAP_REGULAR_GOOFY_MAJ)
		    {
		      base_fg = dark_fg;
		      alternate_fg = light_fg;
		    }
		  else
		    {
		      base_fg = light_fg;
		      alternate_fg = dark_fg;
		    }
		  if (global_bg.color_hsv.v > global_fg.color_hsv.v)
		    {
		      if (base_bg.color_hsv.v < base_fg.color_hsv.v)
			{
			  tmp = base_bg;
			  base_bg = base_fg;
			  base_fg = tmp;
			}
		      if (alternate_bg.color_hsv.v < alternate_fg.color_hsv.v)
			{
			  tmp = alternate_bg;
			  alternate_bg = alternate_fg;
			  alternate_fg = tmp;
			}
		    }
		  else
		    {
		      if (base_bg.color_hsv.v > base_fg.color_hsv.v)
			{
			  tmp = base_bg;
			  base_bg = base_fg;
			  base_fg = tmp;
			}
		      if (alternate_bg.color_hsv.v > alternate_fg.color_hsv.v)
			{
			  tmp = alternate_bg;
			  alternate_bg = alternate_fg;
			  alternate_fg = tmp;
			}
		    }

		  distance_base = lw6sys_color_distance (sys_context, base_bg.color_8, base_fg.color_8);
		  distance_alternate = lw6sys_color_distance (sys_context, alternate_bg.color_8, alternate_fg.color_8);
		  if (distance_base > distance_alternate * SWAP_BASE_ALTERNATE_MAJ)
		    {
		      tmp = base_bg;
		      base_bg = alternate_bg;
		      alternate_bg = tmp;
		      tmp = base_fg;
		      base_fg = alternate_fg;
		      alternate_fg = tmp;
		    }

		  if (alternate_bg.color_hsv.v > alternate_fg.color_hsv.v)
		    {
		      alternate_bg.color_hsv.v = 1.0f - (LIGHTER_COEF * (1.0f - alternate_bg.color_hsv.v));
		      alternate_fg.color_hsv.v = DARKER_COEF * alternate_fg.color_hsv.v;
		    }
		  else
		    {
		      alternate_bg.color_hsv.v = DARKER_COEF * alternate_bg.color_hsv.v;
		      alternate_fg.color_hsv.v = 1.0f - (LIGHTER_COEF * (1.0f - alternate_fg.color_hsv.v));
		    }
		  alternate_bg.color_8 = lw6sys_color_hsv_to_rgb (sys_context, &alternate_bg.color_hsv);
		  alternate_fg.color_8 = lw6sys_color_hsv_to_rgb (sys_context, &alternate_fg.color_hsv);

		  level->texture.guessed_color_base.bg = base_bg.color_8;
		  level->texture.guessed_color_base.fg = base_fg.color_8;
		  level->texture.guessed_color_alternate.bg = alternate_bg.color_8;
		  level->texture.guessed_color_alternate.fg = alternate_fg.color_8;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess colors"));
		}
	      LW6SYS_FREE (sys_context, fg);
	    }
	  LW6SYS_FREE (sys_context, bg);
	}
    }

  return ret;
}

void
_lw6ldr_apply_guessed_colors (lw6sys_context_t * sys_context, lw6map_level_t * level)
{
  level->param.style.color_set.color_base = level->texture.guessed_color_base;
  level->param.style.color_set.color_alternate = level->texture.guessed_color_alternate;
}

/**
 * lw6ldr_auto_colors
 *
 * @style: the style structure to process.
 * @hints: additionnal hints to know what to set automatically
 *
 * Deduces all colors from background color, if needed. The function
 * will check color_auto parameters and replace all other colors
 * by base and alternate colors if needed. Note that the background
 * color itself is not changed by this function. Background can only
 * be guessed from texture.
 *
 * Return value: none.
 */
void
lw6ldr_auto_colors (lw6sys_context_t * sys_context, lw6map_style_t * style, const lw6ldr_hints_t * hints)
{
  if (hints->background_color_auto)
    {
      style->color_set.background_color_root = style->color_set.color_base;
      style->color_set.background_color_stuff = style->color_set.color_alternate;
    }

  if (hints->hud_color_auto)
    {
      style->color_set.hud_color_frame = style->color_set.color_base;
      style->color_set.hud_color_text = style->color_set.color_alternate;
    }

  if (hints->menu_color_auto)
    {
      style->color_set.menu_color_default = style->color_set.color_alternate;
      style->color_set.menu_color_selected = style->color_set.color_alternate;
      lw6map_color_invert (sys_context, &style->color_set.menu_color_selected);
      style->color_set.menu_color_disabled = style->color_set.color_base;
    }

  if (hints->view_color_auto)
    {
      style->color_set.view_color_cursor = style->color_set.color_alternate;
      style->color_set.view_color_map = style->color_set.color_base;
    }

  if (hints->system_color_auto)
    {
      style->color_set.system_color = style->color_set.color_alternate;
    }
}
