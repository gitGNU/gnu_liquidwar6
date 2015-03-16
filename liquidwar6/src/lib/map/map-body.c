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

#include "map.h"

/**
 * lw6map_body_builtin_custom
 *
 * @sys_context: global system context
 * @body: the body to initialize
 * @w: the width
 * @h: the height
 * @d: the depth
 * @noise_percent: the noise level to fill meta layers with
 * @rules: the map rules
 *
 * Sets up a default body structure.
 *
 * Return value: none
 */
void
lw6map_body_builtin_custom (lw6sys_context_t * sys_context, lw6map_body_t * body, int w, int h, int d, int noise_percent, const lw6map_rules_t * rules)
{
  int layer;
  lw6sys_whd_t shape_min = { LW6MAP_MIN_BODY_WIDTH, LW6MAP_MIN_BODY_HEIGHT, LW6MAP_MIN_BODY_DEPTH };
  lw6sys_whd_t shape_max = { LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_BODY_DEPTH };

  lw6map_body_clear (sys_context, body);
  body->shape.w = w;
  body->shape.h = h;
  body->shape.d = d;
  if (!lw6sys_shape_check_min_max_whd (sys_context, &body->shape, &shape_min, &shape_max))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("incorrect default map size %dx%dx%d"), body->shape.w, body->shape.h, body->shape.d);
    }
  for (layer = 0; layer < body->shape.d; ++layer)
    {
      lw6map_layer_builtin_custom (sys_context, &(body->layers[layer]), w, h);
    }
  lw6map_body_check_and_fix_holes (sys_context, body, rules);

  lw6map_meta_layer_builtin_custom (sys_context, &(body->glue), w, h, 1, noise_percent, 0);
  lw6map_meta_layer_builtin_custom (sys_context, &(body->boost), w, h, 1, noise_percent, 1);
  lw6map_meta_layer_builtin_custom (sys_context, &(body->danger), w, h, 1, noise_percent, 2);
  lw6map_meta_layer_builtin_custom (sys_context, &(body->medicine), w, h, 1, noise_percent, 3);
  lw6map_meta_layer_builtin_custom (sys_context, &(body->one_way_north), w, h, 0, noise_percent, 4);
  lw6map_meta_layer_builtin_custom (sys_context, &(body->one_way_east), w, h, 0, noise_percent, 5);
  lw6map_meta_layer_builtin_custom (sys_context, &(body->one_way_south), w, h, 0, noise_percent, 6);
  lw6map_meta_layer_builtin_custom (sys_context, &(body->one_way_west), w, h, 0, noise_percent, 7);
  lw6map_body_fix_checksum (sys_context, body);
}

/**
 * lw6map_body_clear
 *
 * @sys_context: global system context
 * @body: the structure to clear
 *
 * Clears a body structure.
 *
 * Return value: none.
 */
void
lw6map_body_clear (lw6sys_context_t * sys_context, lw6map_body_t * body)
{
  int layer;

  for (layer = 0; layer < LW6MAP_MAX_BODY_DEPTH; ++layer)
    {
      lw6map_layer_clear (sys_context, &(body->layers[layer]));
    }
  lw6map_meta_layer_clear (sys_context, &(body->glue));
  lw6map_meta_layer_clear (sys_context, &(body->boost));
  lw6map_meta_layer_clear (sys_context, &(body->danger));
  lw6map_meta_layer_clear (sys_context, &(body->medicine));
  lw6map_meta_layer_clear (sys_context, &(body->one_way_north));
  lw6map_meta_layer_clear (sys_context, &(body->one_way_east));
  lw6map_meta_layer_clear (sys_context, &(body->one_way_south));
  lw6map_meta_layer_clear (sys_context, &(body->one_way_west));

  memset (body, 0, sizeof (lw6map_body_t));
}

static void
_update_checksum (lw6sys_context_t * sys_context, lw6map_body_t * body, u_int32_t * checksum)
{
  int x, y, z;
  u_int8_t b;

  for (z = 0; z < body->shape.d; ++z)
    {
      for (y = 0; y < body->shape.h; ++y)
	{
	  for (x = 0; x < body->shape.w; ++x)
	    {
	      b = lw6map_body_get (body, x, y, z);
	      lw6sys_checksum_update_int32 (sys_context, checksum, b);
	    }
	}
    }
}

/**
 * lw6map_body_fix_checksum
 *
 * @sys_context: global system context
 * @body: the structure to update
 *
 * Updates (calculates) the checksum of a map body structure.
 *
 * Return value: none.
 */
void
lw6map_body_fix_checksum (lw6sys_context_t * sys_context, lw6map_body_t * body)
{
  u_int32_t checksum = 0;

  _update_checksum (sys_context, body, &checksum);
  body->checksum = checksum;
}

static int
_find_first_free_point (lw6sys_context_t * sys_context, lw6map_body_t * body, int *found_x, int *found_y, int *found_z)
{
  int ret = 0;
  int x, y, z;

  *found_x = 0;
  *found_y = 0;
  *found_z = 0;
  for (y = 0; y < body->shape.h && !ret; ++y)
    {
      for (x = 0; x < body->shape.w && !ret; ++x)
	{
	  for (z = 0; z < body->shape.d; ++z)
	    {
	      if (lw6map_body_get (body, x, y, z) > 0)
		{
		  ret = 1;
		  *found_x = x;
		  *found_y = y;
		  *found_z = z;
		}
	    }
	}
    }

  return ret;
}

/*
 * Updates a body point if needed, returns true if value was changed
 */
static int
_update_if_needed (lw6sys_context_t * sys_context, lw6map_body_t * dst, lw6map_body_t * src, int x, int y, int z)
{
  int ret = 0;

  if (lw6map_body_get (dst, x, y, z) != lw6map_body_get (src, x, y, z))
    {
      lw6map_body_set (dst, x, y, z, lw6map_body_get (src, x, y, z));
      ret = 1;
    }

  return ret;
}

/**
 * lw6map_body_check_and_fix_holes
 *
 * @sys_context: global system context
 * @body: the structure to update
 * @rules: the game rules
 *
 * This (fundamental) function ensures that all playable areas in a map
 * are connected. If isolated zones are found out, then they are marked
 * as walls and not used any more.
 *
 * Return value: none.
 */
int
lw6map_body_check_and_fix_holes (lw6sys_context_t * sys_context, lw6map_body_t * body, const lw6map_rules_t * rules)
{
  int ret = 0;
  lw6map_body_t fixed_body;
  int layer;
  int x;
  int y;
  int z;
  int tx;
  int ty;
  int tz;

  memset (&fixed_body, 0, sizeof (lw6map_body_t));
  fixed_body.shape = body->shape;
  ret = 1;
  for (layer = 0; layer < fixed_body.shape.d; ++layer)
    {
      fixed_body.layers[layer].shape = fixed_body.shape;
      fixed_body.layers[layer].data = (unsigned char *) LW6SYS_CALLOC (sys_context, body->shape.w * body->shape.h * sizeof (unsigned char));
      if (!fixed_body.layers[layer].data)
	{
	  ret = 0;
	}
    }

  if (ret)
    {
      int found;

      /*
       * We first set one point, which will spread all over.
       */
      ret = _find_first_free_point (sys_context, body, &x, &y, &z) && _update_if_needed (sys_context, &fixed_body, body, x, y, z);
      found = ret ? 1 : 0;

      while (found)
	{
	  found = 0;

	  for (y = 0; y < body->shape.h; ++y)
	    {
	      for (x = 0; x < body->shape.w; ++x)
		{
		  for (z = 0; z < body->shape.d; ++z)
		    {
		      if (lw6map_body_get (&fixed_body, x, y, z) > 0)
			{
			  tx = x + 1;
			  ty = y;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tx = x + 1;
			  ty = y + 1;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tx = x;
			  ty = y + 1;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tx = x - 1;
			  ty = y + 1;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tz = z + 1;
			  lw6map_coords_fix_z (rules, &body->shape, &tz);
			  found += _update_if_needed (sys_context, &fixed_body, body, x, y, tz);
			}
		    }
		}
	    }

	  for (y = body->shape.h - 1; y >= 0; --y)
	    {
	      for (x = body->shape.w - 1; x >= 0; --x)
		{
		  for (z = 0; z < body->shape.d; ++z)
		    {
		      if (lw6map_body_get (&fixed_body, x, y, z) > 0)
			{
			  tx = x - 1;
			  ty = y;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tx = x - 1;
			  ty = y - 1;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tx = x;
			  ty = y - 1;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tx = x + 1;
			  ty = y - 1;
			  lw6map_coords_fix_xy (rules, &body->shape, &tx, &ty);
			  found += _update_if_needed (sys_context, &fixed_body, body, tx, ty, z);
			  tz = z - 1;
			  lw6map_coords_fix_z (rules, &body->shape, &tz);
			  found += _update_if_needed (sys_context, &fixed_body, body, x, y, tz);
			}
		    }


		}
	    }
	}
    }

  if (ret)
    {
      for (layer = 0; layer < fixed_body.shape.d; ++layer)
	{
	  LW6SYS_FREE (sys_context, body->layers[layer].data);
	  body->layers[layer].data = fixed_body.layers[layer].data;
	}
    }
  else
    {
      for (layer = 0; layer < fixed_body.shape.d; ++layer)
	{
	  if (fixed_body.layers[layer].data)
	    {
	      LW6SYS_FREE (sys_context, fixed_body.layers[layer].data);
	    }
	}
    }

  return ret;
}

/**
 * lw6map_body_coord_from_texture
 *
 * @sys_context: global system context
 * @level: the level to work on
 * @body_x: the body (logical) x coord
 * @body_y: the body (logical) y coord
 * @texture_x: the texture x coord
 * @texture_y: the texture y coord
 *
 * Gets body (logical) coords from texture position.
 *
 * Return value: 1 on success, 0 on failure (out of bounds)
 */
int
lw6map_body_coord_from_texture (lw6sys_context_t * sys_context, const lw6map_level_t * level, int *body_x, int *body_y, int texture_x, int texture_y)
{
  int ret = 0;

  if (level->texture.w > 0 && level->texture.h > 0)
    {
      (*body_x) = (texture_x * level->body.shape.w) / level->texture.w;
      (*body_y) = (texture_y * level->body.shape.h) / level->texture.h;
      (*body_x) = lw6sys_imax (0, lw6sys_imin (level->body.shape.w - 1, *body_x));
      (*body_y) = lw6sys_imax (0, lw6sys_imin (level->body.shape.h - 1, *body_y));
      ret = 1;
    }
  else
    {
      (*body_x) = 0;
      (*body_y) = 0;
    }

  return ret;
}

/**
 * lw6map_body_get_with_texture_coord
 *
 * @sys_context: global system context
 * @level: the level to work on
 * @texture_x: the texture x coord
 * @texture_y: the texture y coord
 * @z: the z position (depth related)
 *
 * Tells wether a given map position is free or not, but using
 * texture coords.
 *
 * Return value: 1 if position is playable, 0 if not (wall)
 */
u_int8_t
lw6map_body_get_with_texture_coord (lw6sys_context_t * sys_context, const lw6map_level_t * level, int texture_x, int texture_y, int z)
{
  unsigned char ret = 0;
  int body_x;
  int body_y;

  if (lw6map_body_coord_from_texture (sys_context, level, &body_x, &body_y, texture_x, texture_y))
    {
      ret = lw6map_body_get (&level->body, body_x, body_y, z);
    }

  return ret;
}
