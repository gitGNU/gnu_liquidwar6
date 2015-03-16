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

#include "sys.h"
#include "sys-internal.h"

/**
 * lw6sys_shape_check_min_max_whd
 *
 * @sys_context: global system context
 * @shape: the dimensions to control
 * @min: the minimum shape allowed
 * @max: the maximum shape allowed
 *
 * Will check wether the given shape respects some basic constraints,
 * being not to small and not too big.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6sys_shape_check_min_max_whd (lw6sys_context_t * sys_context, const lw6sys_whd_t * shape, const lw6sys_whd_t * min, const lw6sys_whd_t * max)
{
  int ret = 0;

  ret = (shape->w >= min->w && shape->w <= max->w && shape->h >= min->h && shape->h <= max->h && shape->d >= min->d && shape->d <= max->d);

  return ret;
}

/**
 * lw6sys_shape_check_pos
 *
 * @sys_context: global system context
 * @shape: the boundary box
 * @pos: the position
 *
 * Checks wether position is within the given boundary box.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6sys_shape_check_pos (lw6sys_context_t * sys_context, const lw6sys_whd_t * shape, const lw6sys_xyz_t * pos)
{
  int ret = 0;

  ret = (pos->x >= 0 && pos->x < shape->w && pos->y >= 0 && pos->y < shape->h && pos->z >= 0 && (pos->z == 0 || pos->z < shape->d));

  return ret;
}

/**
 * lw6sys_shape_is_same
 *
 * @sys_context: global system context
 * @shape_a: the first shape to compare
 * @shape_b: the other shape to compare
 *
 * Compares two shapes.
 *
 * Return value: 1 if same, 0 if not.
 */
int
lw6sys_shape_is_same (lw6sys_context_t * sys_context, const lw6sys_whd_t * shape_a, const lw6sys_whd_t * shape_b)
{
  int ret = 0;

  ret = (shape_a->w == shape_b->w) && (shape_a->h == shape_b->h) && (shape_a->d == shape_b->d);

  return ret;
}

/**
 * lw6sys_shape_is_same_xy
 *
 * @sys_context: global system context
 * @shape_a: the first shape to compare
 * @shape_b: the other shape to compare
 *
 * Compares two shapes, but ignores the z (d) parameter.
 *
 * Return value: 1 if same_xy, 0 if not.
 */
int
lw6sys_shape_is_same_xy (lw6sys_context_t * sys_context, const lw6sys_whd_t * shape_a, const lw6sys_whd_t * shape_b)
{
  int ret = 0;

  ret = (shape_a->w == shape_b->w) && (shape_a->h == shape_b->h);

  return ret;
}

/**
 * lw6sys_shape_volume_whd
 *
 * @sys_context: global system context
 * @shape: the shape to query
 *
 * Gives the volume (w * h * d) for a given shape.
 *
 * Return value: the volume.
 */
int
lw6sys_shape_volume_whd (lw6sys_context_t * sys_context, const lw6sys_whd_t * shape)
{
  return (shape->w * shape->h * shape->d);
}

/**
 * lw6sys_shape_surface_wh
 *
 * @sys_context: global system context
 * @shape: the shape to query
 *
 * Gives the surface (w * h) for a given shape.
 *
 * Return value: the surface.
 */
int
lw6sys_shape_surface_wh (lw6sys_context_t * sys_context, const lw6sys_whd_t * shape)
{
  return (shape->w * shape->h);
}
