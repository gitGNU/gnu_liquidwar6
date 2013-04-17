/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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
mod_caca_score_font(_mod_caca_context_t * caca_context, int player_id, lw6ker_score_t *score)
{
  int wc, hc;
  caca_canvas_t *cv;
  caca_font_t *f;
  caca_dither_t *d;
  char score_str[32];
  char const * const * fonts;
  uint8_t *buf;

  int color_map[10] = {
    CACA_BLUE,
    CACA_GREEN,
    CACA_CYAN,
    CACA_RED,
    CACA_MAGENTA,
    CACA_LIGHTGRAY,
    CACA_YELLOW,
    CACA_LIGHTBLUE,
    CACA_LIGHTGREEN,
    CACA_LIGHTRED
  };

  cv = caca_create_canvas(32, 1);
  if(cv == NULL)
    {
      lw6sys_log (LW6SYS_LOG_ERROR, _x_ ("Can't create tmp canvas"));
      return (0);
    }

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("team_color: %d"), score->team_color);
  caca_set_color_ansi(cv, color_map[score->team_color], CACA_BLACK);

  hc = caca_get_canvas_height(caca_context->canvas);
  wc = caca_get_canvas_width(caca_context->canvas);

  memset(score_str, 0, 32);
  /* snprintf(score_str, 31, "%d %d %%", player_id + 1, score->fighters_percent); */
  caca_put_str(cv, 0, 0, score_str);

  fonts = caca_get_font_list();
  if(fonts[0] == NULL || fonts[1] == NULL)
    {
      lw6sys_log (LW6SYS_LOG_ERROR, _x_ ("libcaca was compiled without any fonts"));
      return (0);
    }
  f = caca_load_font(fonts[1], 0);
  if(f == NULL)
    {
      lw6sys_log (LW6SYS_LOG_ERROR, _x_ ("could not load font : '%s'"), fonts[0]);
      return (0);
    }

  buf = malloc(4 * wc * hc);
  if(buf == NULL)
    return (0);
  caca_render_canvas(cv, f, buf, wc, hc, 4 * wc);

  d = caca_create_dither(32, wc, hc, 4 * wc, 0xff00, 0xff0000, 0xff000000, 0xff);

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("score:playerId %d"), player_id);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("score:height %d"), (hc - caca_get_font_height(f)) / 10 * player_id);

  if (player_id < 5)
    {
      caca_dither_bitmap(caca_context->canvas,
			 /* (wc / 2 - (strlen(score_str) * caca_get_font_width(f))) / 2, */
			 0,
                         hc / 5 * player_id,
			 wc,
			 22,
			 d,
			 buf);
    }
  else
    {
      caca_dither_bitmap(caca_context->canvas,
			 /* wc / 2 + (wc / 2 - (strlen(score_str) * caca_get_font_width(f))) / 2, */
			 wc / 2,
                         hc / 5 * (player_id - 5),
			 wc,
			 22,
			 d,
			 buf);
    }

  caca_refresh_display(caca_context->display);

  caca_free_dither(d);
  caca_free_canvas(cv);
  free(buf);
  caca_free_font(f);
  return (1);
}

static int
mod_caca_score_text(_mod_caca_context_t * caca_context, int player_id, lw6ker_score_t *score)
{
  int wc, hc;
  char score_str[32];

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("team_color: %d"), score->team_color);

  hc = caca_get_canvas_height(caca_context->canvas);
  wc = caca_get_canvas_width(caca_context->canvas);

  memset(score_str, 0, 32);
  snprintf(score_str, 31, "P%d: %d %%", player_id + 1, score->fighters_percent);

  caca_set_color_argb(caca_context->canvas, CACA_BLACK,
		      lw6sys_color_8_to_iargb (caca_context->const_data.team_color[score->team_color]));
  caca_put_str(caca_context->canvas, player_id * 10, 0, score_str);
  caca_refresh_display(caca_context->display);

  return (1);
}

extern int
_mod_caca_display_hud(_mod_caca_context_t * caca_context, lw6gui_look_t * look,
                      lw6ker_game_state_t * game_state, lw6ker_game_struct_t * game_struct)
{
  int ret = 1;

  int i;

  lw6ker_score_array_update(&caca_context->score_array, game_state);
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    if (caca_context->score_array.scores[i].has_been_active)
      mod_caca_score_text(caca_context, i, &caca_context->score_array.scores[i]);
  return ret;
}

extern int
_mod_caca_display_score(_mod_caca_context_t * caca_context, lw6gui_look_t * look,
                        lw6ker_game_state_t * game_state, lw6ker_game_struct_t * game_struct)
{
  int ret = 1;
  int i;

  lw6ker_score_array_update(&caca_context->score_array, game_state);
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    if (caca_context->score_array.scores[i].has_been_active)
      mod_caca_score_font(caca_context, i, &caca_context->score_array.scores[i]);
  return ret;
}
