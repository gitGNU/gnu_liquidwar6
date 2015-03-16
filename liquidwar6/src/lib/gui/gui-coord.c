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

#include "gui.h"

/**
 * lw6gui_coord_calc_xy
 *
 * @sys_context: global system context
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
 * Return value: 1 if OK, 0 if error (unable to calculate).
 */
int
lw6gui_coord_calc_xy (lw6sys_context_t * sys_context, float *dst_x, float *dst_y, float dst_x0, float dst_y0,
		      float dst_w, float dst_h, float src_x, float src_y, float src_x0, float src_y0, float src_w, float src_h)
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
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistent coord data dst_w=%f dst_h=%f src_w=%f src_h=%f"), dst_w, dst_h, src_w, src_h);
    }

  return ret;
}

/**
 * lw6gui_coords_fix_xy_float
 *
 * @sys_context: global system context
 * @x: x coord (in/out param)
 * @y: y coord (in/out param)
 * @x_flip: flip on x (out param, -1 or +1)
 * @y_flip: flip on y (out param, -1 or +1)
 * @w: width
 * @h: height
 * @x_polarity: x polarity (-1, 0 or 1)
 * @y_polarity: y polarity (-1, 0 or 1)
 *
 * Same as @lw6map_fix_coords except it operates on floats.
 * Usefull for cursor and other rendering operations. Additionnally,
 * will keep track of inversions, that is to say if map is flip
 * in one or another way. Be carefull, the flip values are -1 or 1
 * so that it's easy to multiply an offset by it, for instance, but
 * this means testing if flip is not 0 will always return true, you must
 * test if flip is stritly positive or negative.
 *
 * Return value: none
 */
void
lw6gui_coords_fix_xy_float (lw6sys_context_t * sys_context, float *x, float *y, int *x_flip, int *y_flip, float w, float h, int x_polarity, int y_polarity)
{
  /*
   * The algorithm is copied/pasted from map code, only here it
   * operates with floats, not integers.
   */
  (*x_flip) = 1;
  (*y_flip) = 1;

  if ((*x) < 0.0f || (*y) < 0.0f || (*x) > w || (*y) > h)
    {
      if (x_polarity > 0)
	{
	  while ((*x) > w)
	    {
	      (*x) -= w;
	    }
	  while ((*x) < 0)
	    {
	      (*x) += w;
	    }
	}
      if (y_polarity > 0)
	{
	  while ((*y) > h)
	    {
	      (*y) -= h;
	    }
	  while ((*y) < 0)
	    {
	      (*y) += h;
	    }
	}
      if (x_polarity < 0)
	{
	  while ((*x) > w)
	    {
	      (*x) -= w;
	      (*y) = h - 1 - (*y);
	      (*y_flip) = -(*y_flip);
	    }
	  while ((*x) < 0)
	    {
	      (*x) += w;
	      (*y) = h - 1 - (*y);
	      (*y_flip) = -(*y_flip);
	    }
	}
      if (y_polarity < 0)
	{
	  while ((*y) > h)
	    {
	      (*x) = w - 1 - (*x);
	      (*y) -= h;
	      (*x_flip) = -(*x_flip);
	    }
	  while ((*y) < 0)
	    {
	      (*x) = w - 1 - (*x);
	      (*y) += h;
	      (*x_flip) = -(*x_flip);
	    }
	}

      /*
       * Last, we check for absolute limits, this will also
       * handle the 0 polarit(*y) case.
       */
      if ((*x) < 0.0f)
	{
	  (*x) = 0.0f;
	}
      if ((*x) > w)
	{
	  (*x) = w;
	}
      if ((*y) < 0.0f)
	{
	  (*y) = 0.0f;
	}
      if ((*y) > h)
	{
	  (*y) = h;
	}
    }
}
