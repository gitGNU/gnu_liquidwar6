/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "gui.h"

/**
 * lw6gui_button_register_down
 *
 * @dst_x: the x coord to return
 * @dst_y: the y coord to return
 * @dst_x0: the x coord of point 0 in destination coord system
 * @dst_y0: the y coord of point 0 in destination coord system
 * @dst_w: the width of the area in destination coord system
 * @dst_h: the width of the area in destination coord system
 * @src_x: the x coord in source coord system
 * @src_y: the y coord in source coord system
 * @src_x0: the x coord of point 0 in source coord system
 * @src_y0: the y coord of point 0 in source coord system
 * @src_w: the width of the area in source coord system
 * @src_h: the width of the area in source coord system
 *
 * Registers a "down" (press) event on a button.
 *
 * Return value: none.
 */
int
lw6gui_coord_calc_xy (float *dst_x, float *dst_y, float dst_x0, float dst_y0,
		      float dst_w, float dst_h, float src_x, float src_y,
		      float src_x0, float src_y0, float src_w, float src_h)
{
  int ret = 0;

  if (dst_w > 0.0f && dst_h > 0.0f && src_w > 0.0f && src_h > 0.0f)
    {
      if (dst_x)
	{
	  (*dst_x) = dst_x0 + (src_x - src_x0) * (dst_w / src_w);
	}
      if (dst_y)
	{
	  (*dst_y) = dst_y0 + (src_y - src_y0) * (dst_h / src_h);
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("inconsistent coord data dst_w=%f dst_h=%f src_w=%f src_h=%f"),
		  dst_w, dst_h, src_w, src_h);
    }

  return ret;
}
