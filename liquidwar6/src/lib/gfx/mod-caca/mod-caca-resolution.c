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

#include "mod-caca-internal.h"

int
_mod_caca_get_fullscreen_modes (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, lw6gui_fullscreen_modes_t * modes)
{
  int ret = 0;

  modes->low.width = LW6GUI_DEFAULT_LOW_WIDTH;
  modes->low.height = LW6GUI_DEFAULT_LOW_HEIGHT;
  modes->low.fullscreen = 1;
  modes->standard.width = LW6GUI_DEFAULT_STANDARD_WIDTH;
  modes->standard.height = LW6GUI_DEFAULT_STANDARD_HEIGHT;
  modes->standard.fullscreen = 1;
  modes->high.width = LW6GUI_DEFAULT_HIGH_WIDTH;
  modes->high.height = LW6GUI_DEFAULT_HIGH_HEIGHT;
  modes->high.fullscreen = 1;

  // todo: get available modes (if it makes sense)

  modes->low.width = lw6sys_imin (modes->low.width, modes->high.width / 2);
  modes->low.height = lw6sys_imin (modes->low.height, modes->high.height / 2);

  modes->standard.width = (modes->low.width + modes->high.width) / 2;
  modes->standard.height = (modes->low.height + modes->high.height) / 2;

  lw6gui_video_mode_sync_ratio (sys_context, &(modes->standard), &(modes->high));

  _mod_caca_find_closest_resolution (sys_context, caca_context, &(modes->standard.width), &(modes->standard.height), modes->standard.width,
				     modes->standard.height);

  /*
   * Liquid War 6 does have a (retrospectively) quite awfull way
   * of dealing with return codes. 0 means error, and 1 means OK.
   * This allows stuff like if (foo()) { bar();} but is against
   * all established C/POSIX standards. The hell with if, function
   * must return 1, period.
   */
  ret = 1;

  return ret;
}

void
_mod_caca_find_closest_resolution (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, int *closest_width, int *closest_height,
				   int wished_width, int wished_height)
{
  (*closest_width) = wished_width;
  (*closest_height) = wished_height;

  // todo...
}
