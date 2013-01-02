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

int
caca_color_for_fighters(int team_color)
{
  int color = 0xFFFFFF;

  switch (team_color) {
    case 0:
      color = 0xAA0000; // red
      break;
    case 1:
      color = 0x3146FF; // blue
      break;
    case 2:
      color = 0xAA5500; // yellow
      break;
    case 3:
      color = 0x009000; // green
      break;
    case 4:
      color = 0x0E697B; // cyan
      break;
    case 5:
      color = 0xAA00AA; // magenta
      break;
    case 6:
      color = 0x5555FF; // bright blue
      break;
    case 7:
      color = 0xFF5555; // bright red
      break;
    case 8:
      color = 0x55FFFF; //bright cyan
      break;
    case 9:
      color = 0x55FF55; // bright green
      break;
    default:
      break;
  }
  return color;
}

extern int
_mod_caca_display_map(_mod_caca_context_t * caca_context, lw6gui_look_t * look,
		  lw6ker_game_state_t * game_state, lw6ker_game_struct_t * game_struct)
{
  int fighter_id;
  lw6ker_fighter_t *fighter;
  int team_color;
  int ret = 1, width, height, y, x;
  /* lw6map_rules_t *rules = NULL; */
  lw6sys_whd_t shape = {0, 0, 0};
  caca_dither_t *dither;
  uint32_t *buffer;

  lw6ker_game_state_get_shape (game_state, &shape);
  width = shape.w;
  height = shape.h;
  buffer = malloc(sizeof(*buffer) * (shape.w * shape.h));
  if (buffer == NULL)
    return 0;

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      if (lw6ker_game_struct_is_fg(game_struct, x, y, 0))
	buffer[width * y + x] = 0x2a2a2a;
      else if ((lw6ker_game_struct_is_bg(game_struct, x, y, 0)))
	buffer[width * y + x] = 0xFFFFFF;
      else
      {
	fighter_id = -1;
	fighter_id = lw6ker_game_state_get_fighter_id (game_state, x, y, 0);
	if (fighter_id >= 0)
	{
	  fighter = lw6ker_game_state_get_fighter_by_id (game_state, fighter_id);
	  team_color = fighter->team_color;
	  if (team_color >= 0)
	  {
	    buffer[width * y + x] = caca_color_for_fighters(team_color);
	  }
	}
      }
    }
  }

  dither = caca_create_dither(32, width, height, 4 * width,
			      0x00ff0000, 0x0000ff00, 0x000000ff, 0x0);
  caca_dither_bitmap(caca_context->canvas, 0, 0, caca_get_canvas_width(caca_context->canvas),
		     caca_get_canvas_height(caca_context->canvas), dither, buffer);
  caca_free_dither(dither);
  free(buffer);
  return ret;
}
