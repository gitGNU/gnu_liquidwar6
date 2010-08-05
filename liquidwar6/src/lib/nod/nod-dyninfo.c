/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "nod.h"
#include "nod-internal.h"

void
_lw6nod_dyn_info_reset (lw6nod_dyn_info_t * dyn_info)
{
  if (dyn_info->level)
    {
      LW6SYS_FREE (dyn_info->level);
      dyn_info->level = lw6sys_str_copy ("");
    }
  dyn_info->required = 0;
  dyn_info->limit = 0;
  dyn_info->colors = 0;
  dyn_info->nodes = 0;
  dyn_info->cursors = 0;
  if (dyn_info->game_screenshot_data)
    {
      LW6SYS_FREE (dyn_info->game_screenshot_data);
      dyn_info->game_screenshot_data = NULL;
    }
}

int
_lw6nod_dyn_info_update (lw6nod_dyn_info_t * dyn_info, char *level,
			 int required,
			 int limit,
			 int colors,
			 int nodes,
			 int cursors,
			 int game_screenshot_size, void *game_screenshot_data)
{
  int ret = 0;

  if (dyn_info->level)
    {
      LW6SYS_FREE (dyn_info->level);
      dyn_info->level = NULL;
    }
  if (level)
    {
      dyn_info->level = lw6sys_str_copy (level);
    }
  dyn_info->required = required;
  dyn_info->limit = limit;
  dyn_info->colors = colors;
  dyn_info->nodes = nodes;
  dyn_info->cursors = cursors;
  dyn_info->game_screenshot_size = game_screenshot_size;
  if (dyn_info->game_screenshot_data)
    {
      LW6SYS_FREE (dyn_info->game_screenshot_data);
      dyn_info->game_screenshot_data = NULL;
    }
  if (game_screenshot_size > 0)
    {
      dyn_info->game_screenshot_data = LW6SYS_MALLOC (game_screenshot_size);
      if (dyn_info->game_screenshot_data)
	{
	  memcpy (dyn_info->game_screenshot_data, game_screenshot_data,
		  game_screenshot_size);
	}
    }

  ret = (dyn_info->level && dyn_info->game_screenshot_data);

  return ret;
}

/**
 * lw6nod_dyn_info_free
 *
 * @dyn_info: the dyn info struct to free
 *
 * Frees a dyn info object, to be used after a
 * call to @lw6nod_info_dup_dyn for instance.
 *
 * Return value: none
 */
void
lw6nod_dyn_info_free (lw6nod_dyn_info_t * dyn_info)
{
  _lw6nod_dyn_info_reset (dyn_info);
  if (dyn_info->level)
    {
      LW6SYS_FREE (dyn_info->level);
    }
  LW6SYS_FREE (dyn_info);
}
