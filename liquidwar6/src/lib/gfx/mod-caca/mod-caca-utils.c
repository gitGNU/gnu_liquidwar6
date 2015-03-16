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

#include "mod-caca-internal.h"

static int
_score_font (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, int player_id, lw6ker_score_t * score)
{
  int wc, hc, i;
  caca_canvas_t *cv;
  caca_font_t *f;
  caca_dither_t *d;
  char score_str[32];
  char const *const *fonts;
  uint8_t *buf;
  uint32_t *buffer;

  hc = caca_get_canvas_height (caca_context->canvas);
  wc = caca_get_canvas_width (caca_context->canvas);

  buffer = malloc (sizeof (*buffer) * 4 * (hc / 5));
  if (buffer == NULL)
    return 0;
  memset (buffer, CACA_TRANSPARENT, sizeof (*buffer) * 4 * (hc / 5));

  for (i = 0; i < 4 * (hc / 5); ++i)
    buffer[i] = lw6sys_color_8_to_ibgra (caca_context->const_data.team_color[score->team_color]);

  d = caca_create_dither (32, 4, (hc / 5), 4 * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x0);
  if (player_id < 5)
    caca_dither_bitmap (caca_context->canvas, (wc / 2) - 4, (hc / 5) * player_id, 4, (hc / 5), d, buffer);
  else
    caca_dither_bitmap (caca_context->canvas, wc - 4, (hc / 5) * (player_id - 5), 4, (hc / 5), d, buffer);

  caca_free_dither (d);

  cv = caca_create_canvas (wc / 2, hc / 5);
  if (cv == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("Can't create tmp canvas"));
      return (0);
    }

  caca_set_color_ansi (cv, CACA_WHITE, CACA_BLACK);

  memset (score_str, 0, 32);
  snprintf (score_str, 31, "%d %d %%", player_id, score->fighters_percent);
  caca_put_str (cv, 0, 0, score_str);

  fonts = caca_get_font_list ();
  if (fonts[0] == NULL || fonts[1] == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("libcaca was compiled without any fonts"));
      return (0);
    }
  f = caca_load_font (fonts[1], 0);
  if (f == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("could not load font : '%s'"), fonts[0]);
      return (0);
    }

  buf = malloc (4 * hc * wc);
  if (buf == NULL)
    return 0;
  memset (buf, 0, 4 * hc * wc);

  caca_render_canvas (cv, f, buf, wc, hc, 4 * wc);

  d = caca_create_dither (32, wc, hc, 4 * wc, 0xff00, 0xff0000, 0xff000000, 0xff);

  if (player_id < 5)
    caca_dither_bitmap (caca_context->canvas, 0, (hc / 5) * player_id, wc, 22, d, buf);
  else
    caca_dither_bitmap (caca_context->canvas, (wc / 2), (hc / 5) * (player_id - 5), wc, 22, d, buf);

  caca_refresh_display (caca_context->display);

  free (buf);
  caca_free_dither (d);
  caca_free_font (f);
  caca_free_canvas (cv);

  return (1);
}

int
_score_text (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, int player_id, lw6ker_score_t * score)
{
  // int wc, hc, i;
  int i;
  char score_str[32];
  uint32_t *buffer;
  caca_dither_t *dither;

  // hc = caca_get_canvas_height (caca_context->canvas);
  // wc = caca_get_canvas_width (caca_context->canvas);

  buffer = malloc (sizeof (*buffer) * 3);
  if (buffer == NULL)
    return 0;
  memset (buffer, CACA_TRANSPARENT, sizeof (*buffer) * 3);

  memset (score_str, 0, 32);
  snprintf (score_str, 12, " P%2d: %d %% ", player_id, score->fighters_percent);

  for (i = 0; i < 3; ++i)
    buffer[i] = lw6sys_color_8_to_ibgra (caca_context->const_data.team_color[score->team_color]);

  dither = caca_create_dither (32, 3, 1, 4 * 3, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x0);
  caca_dither_bitmap (caca_context->canvas, player_id * 15, 0, 3, 1, dither, buffer);

  caca_set_color_ansi (caca_context->canvas, CACA_BLACK, CACA_WHITE);
  caca_put_str (caca_context->canvas, player_id * 15 + 3, 0, score_str);

  caca_free_dither (dither);
  free (buffer);

  return (1);
}

extern int
_mod_caca_display_hud (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context,
		       const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, const lw6ker_game_struct_t * game_struct)
{
  int ret = 1;

  int i;

  lw6ker_score_array_update (sys_context, &caca_context->score_array, game_state);
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    if (caca_context->score_array.scores[i].has_been_active)
      _score_text (sys_context, caca_context, i, &caca_context->score_array.scores[i]);
  caca_refresh_display (caca_context->display);
  return ret;
}

extern int
_mod_caca_display_score (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context,
			 const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, const lw6ker_game_struct_t * game_struct)
{
  int ret = 1, i;

  lw6ker_score_array_update (sys_context, &caca_context->score_array, game_state);
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    if (caca_context->score_array.scores[i].has_been_active)
      _score_font (sys_context, caca_context, i, &caca_context->score_array.scores[i]);
  caca_refresh_display (caca_context->display);
  return ret;
}
