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

#include <string.h>

#include "map.h"

int
lw6map_texture_from_body (lw6map_texture_t * texture, lw6map_body_t * body,
			  lw6map_color_couple_t * color)
{
  int ret = 0;
  int x, y;

  lw6map_texture_clear (texture);

  texture->w = body->shape.w;
  texture->h = body->shape.h;
  texture->data =
    (lw6sys_color_8_t *) LW6SYS_MALLOC (texture->w *
					texture->h *
					sizeof (lw6sys_color_8_t));

  if (texture->data)
    {
      for (y = 0; y < texture->h; ++y)
	{
	  for (x = 0; x < texture->w; ++x)
	    {
	      lw6map_texture_set (texture, x, y,
				  lw6map_body_get (body, x,
						   y, 0) >
				  0 ? color->bg : color->fg);
	    }
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to construct texture from body"));
      texture->w = 0;
      texture->h = 0;
    }

  return ret;
}

/*
 * Clears a texture.
 */
void
lw6map_texture_clear (lw6map_texture_t * texture)
{
  if (texture->data)
    {
      LW6SYS_FREE (texture->data);
    }

  memset (texture, 0, sizeof (lw6map_texture_t));
}

int
lw6map_texture_coord_from_body (lw6map_level_t * level, int *texture_x,
				int *texture_y, int body_x, int body_y)
{
  int ret = 0;

  if (level->body.shape.w > 0 && level->body.shape.h > 0)
    {
      (*texture_x) = (body_x * level->texture.w) / level->body.shape.w;
      (*texture_y) = (body_y * level->texture.h) / level->body.shape.h;
      (*texture_x) =
	lw6sys_max (0, lw6sys_min (level->texture.w - 1, *texture_x));
      (*texture_y) =
	lw6sys_max (0, lw6sys_min (level->texture.h - 1, *texture_y));
      ret = 1;
    }
  else
    {
      (*texture_x) = 0;
      (*texture_y) = 0;
    }

  return ret;
}

lw6sys_color_8_t
lw6map_texture_get_with_body_coord (lw6map_level_t * level, int body_x,
				    int body_y)
{
  lw6sys_color_8_t ret = LW6SYS_COLOR_8_BLACK;
  int texture_x;
  int texture_y;

  if (lw6map_texture_coord_from_body
      (level, &texture_x, &texture_y, body_x, body_y))
    {
      ret = lw6map_texture_get (&level->texture, texture_x, texture_y);
    }

  return ret;
}

int
lw6map_texture_has_alpha (lw6map_texture_t * texture)
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
