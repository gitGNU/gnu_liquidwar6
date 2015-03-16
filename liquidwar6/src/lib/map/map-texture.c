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
 * lw6map_texture_from_body
 *
 * @sys_context: global system context
 * @texture: texture to load (out param)
 * @body: body to pick data from
 * @color: colors to use
 *
 * Will create a default bicolor texture from the body data,
 * this is in case we don't want to use the texture or there is
 * none. Result is not beautifull but might be very comfortable
 * to play.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6map_texture_from_body (lw6sys_context_t * sys_context, lw6map_texture_t * texture, const lw6map_body_t * body, const lw6map_color_couple_t * color)
{
  int ret = 0;
  int x, y;

  lw6map_texture_clear (sys_context, texture);

  texture->w = body->shape.w;
  texture->h = body->shape.h;
  texture->data = (lw6sys_color_8_t *) LW6SYS_MALLOC (sys_context, texture->w * texture->h * sizeof (lw6sys_color_8_t));

  if (texture->data)
    {
      for (y = 0; y < texture->h; ++y)
	{
	  for (x = 0; x < texture->w; ++x)
	    {
	      lw6map_texture_set (texture, x, y, lw6map_body_get (body, x, y, 0) > 0 ? color->bg : color->fg);
	    }
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to construct texture from body"));
      texture->w = 0;
      texture->h = 0;
    }

  return ret;
}

/**
 * lw6map_texture_clear
 *
 * @sys_context: global system context
 * @texture: data to clear
 *
 * Clears a texture object, expects it to be in a consitent
 * state, either filled with real data of zeroed.
 *
 * Return value: none.
 */
void
lw6map_texture_clear (lw6sys_context_t * sys_context, lw6map_texture_t * texture)
{
  if (texture->data)
    {
      LW6SYS_FREE (sys_context, texture->data);
    }

  memset (texture, 0, sizeof (lw6map_texture_t));
}

/**
 * lw6map_texture_coord_from_body
 *
 * @sys_context: global system context
 * @level: map to work on
 * @texture_x: texture x coordinate (out param)
 * @texture_y: texture y coordinate (out param)
 * @body_x: body x coordinate (in param)
 * @body_y: body y coordinate (in param)
 *
 * Translates from body coordinate space to texture
 * coordinate space.
 *
 * Return value: 1 on success, 0 if failure.
 */
int
lw6map_texture_coord_from_body (lw6sys_context_t * sys_context, const lw6map_level_t * level, int *texture_x, int *texture_y, int body_x, int body_y)
{
  int ret = 0;

  if (level->body.shape.w > 0 && level->body.shape.h > 0)
    {
      (*texture_x) = (body_x * level->texture.w) / level->body.shape.w;
      (*texture_y) = (body_y * level->texture.h) / level->body.shape.h;
      (*texture_x) = lw6sys_imax (0, lw6sys_imin (level->texture.w - 1, *texture_x));
      (*texture_y) = lw6sys_imax (0, lw6sys_imin (level->texture.h - 1, *texture_y));
      ret = 1;
    }
  else
    {
      (*texture_x) = 0;
      (*texture_y) = 0;
    }

  return ret;
}

/**
 * lw6map_texture_get_with_body_coord
 *
 * @sys_context: global system context
 * @level: map to use
 * @body_x: x coordinate in body space
 * @body_y: y coordinate in body space
 *
 * Get the color of a given point in the texture, using the
 * body coordinate space.
 *
 * Return value: RGBA 8-bit color.
 */
lw6sys_color_8_t
lw6map_texture_get_with_body_coord (lw6sys_context_t * sys_context, const lw6map_level_t * level, int body_x, int body_y)
{
  lw6sys_color_8_t ret = LW6SYS_COLOR_8_BLACK;
  int texture_x;
  int texture_y;

  if (lw6map_texture_coord_from_body (sys_context, level, &texture_x, &texture_y, body_x, body_y))
    {
      ret = lw6map_texture_get (&level->texture, texture_x, texture_y);
    }

  return ret;
}

/**
 * lw6map_texture_has_alpha
 *
 * @sys_context: global system context
 * @texture: texture object to test
 *
 * Finds out if the texture is fully opaque or not. If it has
 * an alpha layer (typically, PNG file) but this one is filled
 * at 100% everywhere, then it will consider opaque. This is
 * a slow function but the result is cached in the has_alpha
 * member, so as the function is called at map loading, use
 * the cached value instead.
 *
 * Return value: 1 if has used alpha layer, 0 if opaque.
 */
int
lw6map_texture_has_alpha (lw6sys_context_t * sys_context, lw6map_texture_t * texture)
{
  int ret = 0;
  int x = 0;
  int y = 0;
  lw6sys_color_8_t color;

  if (texture->data)
    {
      for (y = 0; y < texture->h; ++y)
	{
	  for (x = 0; x < texture->w; ++x)
	    {
	      color = lw6map_texture_get (texture, x, y);
	      if (color.a < 255)
		{
		  ret = 1;
		}
	    }
	}
      texture->has_alpha = ret;
    }

  return ret;
}
