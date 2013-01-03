/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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
_mod_caca_display (_mod_caca_context_t * caca_context, int mask,
		   lw6gui_look_t * look, lw6map_level_t * level,
		   lw6ker_game_struct_t * game_struct,
		   lw6ker_game_state_t * game_state,
		   lw6pil_local_cursors_t * local_cursors,
		   lw6gui_menu_t * menu, float progress, float fps, float mps,
		   char **log_list, int capture, int gfx_debug,
		   int debug_team_id, int debug_layer_id)
{
  int ret = 1;

  // todo: display real stuff (menu, view, hud, ...)

  caca_set_color_ansi (caca_context->canvas, CACA_BLACK, CACA_WHITE);
  caca_put_str (caca_context->canvas, 0, 0, __FUNCTION__);

  caca_refresh_display (caca_context->display);

  return ret;
}
