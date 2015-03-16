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

extern int
_mod_caca_display (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, int mask,
		   const lw6gui_look_t * look, const lw6map_level_t * level,
		   const lw6ker_game_struct_t * game_struct,
		   const lw6ker_game_state_t * game_state,
		   lw6pil_local_cursors_t * local_cursors,
		   lw6gui_menu_t * menu, float progress, float fps, float mps,
		   const char **log_list, int capture, int gfx_debug, int debug_team_id, int debug_layer_id)
{
  int ret = 1;
  int wc, hc;
  static int sp_free = 0;

  hc = caca_get_canvas_height (caca_context->canvas);
  wc = caca_get_canvas_width (caca_context->canvas);
  caca_set_color_ansi (caca_context->canvas, CACA_WHITE, CACA_DEFAULT);

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display mask=%d"), mask);
  if (mask & LW6GUI_DISPLAY_SPLASH)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=splash"));
      _mod_caca_plasma_anim (sys_context, caca_context);
      sp_free = 1;
    }
  else
    {
      if (sp_free == 1)
	{
	  _mod_caca_splash_free (sys_context, caca_context, NULL, NULL, NULL, 0);
	  sp_free = 0;
	}
      caca_set_color_ansi (caca_context->canvas, CACA_DEFAULT, CACA_DEFAULT);
      caca_fill_box (caca_context->canvas, 0, 0, wc, hc, ' ');
      if ((mask & LW6GUI_DISPLAY_MAP) && game_state)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=map"));
	  _mod_caca_display_map (sys_context, caca_context, look, game_state, game_struct, local_cursors);

	}
      if ((mask & LW6GUI_DISPLAY_FIGHTERS) && game_state)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=fighters"));

	}
      if ((mask & LW6GUI_DISPLAY_CURSORS) && game_state)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=cursor"));
	  // Why not ?
	}
      if ((mask & LW6GUI_DISPLAY_HUD) && game_state)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=hud"));
	  _mod_caca_display_hud (sys_context, caca_context, look, game_state, game_struct);
	}
      if ((mask & LW6GUI_DISPLAY_SCORE) && game_state)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=score"));
	  _mod_caca_display_score (sys_context, caca_context, look, game_state, game_struct);
	}
      if ((mask & LW6GUI_DISPLAY_PROGRESS))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=progress"));
	}
      if ((mask & LW6GUI_DISPLAY_MENU) && menu)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display step=menu"));
	  _mod_caca_display_menu (sys_context, caca_context, look, menu);
	}
    }

  caca_refresh_display (caca_context->display);

  return ret;
}
