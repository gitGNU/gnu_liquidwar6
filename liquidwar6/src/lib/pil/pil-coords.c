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

#include "pil.h"
#include "pil-internal.h"

/**
 * lw6pil_coords_fix
 *
 * @sys_context: global system context
 * @rules: the set of rules to use (defines polarity)
 * @shape: the shape of the map (logical part)
 * @x: the x coord to fix
 * @y: the y coord to fix
 * @z: the z coord to fix
 *
 * Similar to @lw6map_coords_fix but using floats, this function
 * can be used to check cursor position boundaries. Any float pointer
 * can be NULL.
 *
 * Return value: none.
 */
void
lw6pil_coords_fix (lw6sys_context_t * sys_context, lw6map_rules_t * rules, lw6sys_whd_t * shape, float *x, float *y, float *z)
{
  float w;
  float h;
  int x_polarity;
  int y_polarity;
  float tmp_x = 0.0f;
  float tmp_y = 0.0f;

  w = shape->w;
  h = shape->h;
  x_polarity = rules->x_polarity;
  y_polarity = rules->y_polarity;
  if (x)
    {
      tmp_x = *x;
    }
  if (y)
    {
      tmp_y = *y;
    }

  if (x_polarity > 0)
    {
      while (tmp_x >= w)
	{
	  tmp_x -= w;
	}
      while (tmp_x < 0)
	{
	  tmp_x += w;
	}
    }
  if (y_polarity > 0)
    {
      while (tmp_y >= h)
	{
	  tmp_y -= h;
	}
      while (tmp_y < 0)
	{
	  tmp_y += h;
	}
    }
  if (x_polarity < 0)
    {
      while (tmp_x >= w)
	{
	  tmp_x -= w;
	  tmp_y = h - tmp_y;
	}
      while (tmp_x < 0)
	{
	  tmp_x += w;
	  tmp_y = h - tmp_y;
	}
    }
  if (y_polarity < 0)
    {
      while (tmp_y >= h)
	{
	  tmp_x = w - tmp_x;
	  tmp_y -= h;
	}
      while (tmp_y < 0)
	{
	  tmp_x = w - tmp_x;
	  tmp_y += h;
	}
    }

  /*
   * Last, we check for absolute limits, this will also
   * handle the 0 polarity case.
   */
  if (tmp_x < 0.0f)
    {
      tmp_x = 0.0f;
    }
  if (tmp_x >= w)
    {
      tmp_x = w;
    }
  if (tmp_y < 0.0f)
    {
      tmp_y = 0.0f;
    }
  if (tmp_y >= h)
    {
      tmp_y = h;
    }

  // done, affect values
  if (x)
    {
      (*x) = tmp_x;
    }
  if (y)
    {
      (*y) = tmp_y;
    }

  if (z)
    {
      if ((*z) >= (float) shape->d)
	{
	  if (rules->z_polarity == 0)
	    {
	      (*z) = (float) (shape->d - 1);
	    }
	  else
	    {
	      (*z) = 0.0f;
	    }
	}

      if ((*z) < 0.0f)
	{
	  if (rules->z_polarity == 0)
	    {
	      (*z) = 0.0f;
	    }
	  else
	    {
	      (*z) = (float) (shape->d - 1);
	    }
	}
    }
}

/**
 * lw6pil_coords_fix_x10
 *
 * @sys_context: global system context
 * @rules: the set of rules to use (defines polarity)
 * @shape: the shape of the map (logical part)
 * @x: the x coord to fix
 * @y: the y coord to fix
 * @z: the z coord to fix
 *
 * Similar to @lw6pil_coords_fix but does use a wider range, say 10 times
 * the actual size of the map, this is not to contain the cursor within
 * the map but just to avoid overflow errors.
 *
 * Return value: none.
 */
void
lw6pil_coords_fix_x10 (lw6sys_context_t * sys_context, lw6map_rules_t * rules, lw6sys_whd_t * shape, float *x, float *y, float *z)
{
  lw6sys_whd_t shape_x10;

  shape_x10 = (*shape);

  shape_x10.w *= 10;
  shape_x10.h *= 10;

  lw6pil_coords_fix (sys_context, rules, &shape_x10, x, y, z);
}
