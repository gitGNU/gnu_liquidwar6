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

#include <string.h>

#include "map.h"

/**
 * lw6map_coords_fix_xy
 *
 * @rules: set of rules to use
 * @shape: shape of the map
 * @x: x coord (in/out param)
 * @y: y coord (in/out param)
 *
 * Fixes the x and y values so that it's always inside the map. This
 * function will read the rules and if there's some polarity enable,
 * will do the right thing, for instance, a fighter too much on the
 * right might reapper on the left side of the map.
 *
 * Return value: none.
 */
void
lw6map_coords_fix_xy (const lw6map_rules_t * rules, const lw6sys_whd_t * shape, int *x, int *y)
{
  if ((*x) < 0 || (*y) < 0 || (*x) >= shape->w || (*y) >= shape->h)
    {
      /*
       * small optimization: we only declare variables now, this
       * way when the coords are within map boundaries, code
       * remains minimalist.
       */
      int w;
      int h;
      int x_polarity;
      int y_polarity;
      int tmp_x;
      int tmp_y;

      w = shape->w;
      h = shape->h;
      x_polarity = rules->x_polarity;
      y_polarity = rules->y_polarity;
      tmp_x = *x;
      tmp_y = *y;

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
	      tmp_y = h - 1 - tmp_y;
	    }
	  while (tmp_x < 0)
	    {
	      tmp_x += w;
	      tmp_y = h - 1 - tmp_y;
	    }
	}
      if (y_polarity < 0)
	{
	  while (tmp_y >= h)
	    {
	      tmp_x = w - 1 - tmp_x;
	      tmp_y -= h;
	    }
	  while (tmp_y < 0)
	    {
	      tmp_x = w - 1 - tmp_x;
	      tmp_y += h;
	    }
	}

      /*
       * Last, we check for absolute limits, this will also
       * handle the 0 polarity case.
       */
      tmp_x = lw6sys_imax (0, lw6sys_imin (tmp_x, w - 1));
      tmp_y = lw6sys_imax (0, lw6sys_imin (tmp_y, h - 1));

      // done, affect values
      (*x) = tmp_x;
      (*y) = tmp_y;
    }
}

/**
 * lw6map_coords_fix_z
 *
 * @rules: set of rules to use
 * @shape: shape of the map
 * @z: z coord (in/out param)
 *
 * Fixes the z value so that it's always inside the map. This
 * function will read the rules and if there's some polarity enable,
 * will do the right thing, for instance, a fighter too low it
 * might reapper on top.
 *
 * Return value: none.
 */
void
lw6map_coords_fix_z (const lw6map_rules_t * rules, const lw6sys_whd_t * shape, int *z)
{

  if ((*z) < 0 || (*z) >= shape->d)
    {
      int d;
      int z_polarity;
      int tmp_z;

      /*
       * Note: because shape->d is unsigned, affecting shape-> to d
       * does have its importance, the only alternative would be
       * to cast shape->d to a signed int.
       */
      d = shape->d;
      z_polarity = rules->z_polarity;
      tmp_z = *z;

      if (tmp_z >= d)
	{
	  if (z_polarity == 0)
	    {
	      tmp_z = d - 1;
	    }
	  else
	    {
	      tmp_z = 0;
	    }
	}

      if (tmp_z < 0)
	{
	  if (z_polarity == 0)
	    {
	      tmp_z = 0;
	    }
	  else
	    {
	      tmp_z = d - 1;
	    }
	}
      (*z) = tmp_z;
    }
}
