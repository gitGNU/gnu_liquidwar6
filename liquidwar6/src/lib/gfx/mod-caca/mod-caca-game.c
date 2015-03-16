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

int
_mod_caca_display_map (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context,
		       const lw6gui_look_t * look,
		       const lw6ker_game_state_t * game_state, const lw6ker_game_struct_t * game_struct, lw6pil_local_cursors_t * local_cursors)
{
  int fighter_id;
  const lw6ker_fighter_t *fighter;
  int team_color;
  int ret = 1, width, height, y, x;
  lw6sys_whd_t shape = { 0, 0, 0 };
  caca_dither_t *dither;
  uint32_t *buffer;
  lw6pil_local_cursor_t *cursor = NULL;

  if (local_cursors)
    {
      cursor = &local_cursors->cursors[local_cursors->main_i];
    }
  lw6ker_game_state_get_shape (sys_context, game_state, &shape);
  width = shape.w;
  height = shape.h;
  buffer = malloc (sizeof (*buffer) * (shape.w * shape.h));
  if (buffer == NULL)
    return 0;
  memset (buffer, lw6sys_color_8_to_ibgra (caca_context->const_data.bg_color), sizeof (*buffer) * (shape.w * shape.h));

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("map [%d|%d]"), width, height);
  if (cursor)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("cursor [%d|%d]"), cursor->x, cursor->y);
    }
  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
	{
	  fighter_id = -1;
	  fighter_id = lw6ker_game_state_get_fighter_id (sys_context, game_state, x, y, 0);
	  if (fighter_id >= 0)
	    {
	      fighter = lw6ker_game_state_get_fighter_ro_by_id (sys_context, game_state, fighter_id);
	      team_color = fighter->team_color;
	      if (team_color >= 0 && team_color < 10)
		{
		  buffer[width * y + x] = lw6sys_color_8_to_ibgra (caca_context->const_data.team_color[team_color]);
		}
	    }
	  else
	    {
	      if (lw6ker_game_struct_is_bg (sys_context, game_struct, x, y, 0))
		buffer[width * y + x] = lw6sys_color_8_to_ibgra (caca_context->const_data.bg_color);
	      else
		buffer[width * y + x] = lw6sys_color_8_to_ibgra (caca_context->const_data.fg_color);
	    }
	}
    }
  if (cursor && cursor->y < height && cursor->x < width)
    buffer[width * cursor->y + cursor->x] = 0xff0000;

  dither = caca_create_dither (32, width, height, 4 * width, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x0);
  caca_dither_bitmap (caca_context->canvas, 0, 0, caca_get_canvas_width (caca_context->canvas), caca_get_canvas_height (caca_context->canvas), dither, buffer);
  caca_free_dither (dither);
  free (buffer);
  return ret;
}
