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

#include "../../gfx.h"
#include "gl-utils.h"

mod_gl_utils_bitmap_t *
mod_gl_utils_cursor_create_fg_bg (mod_gl_utils_context_t * utils_context,
				  lw6map_level_t * level)
{
  mod_gl_utils_bitmap_t *bitmap = NULL;
  SDL_Surface *surface = NULL;
  int x, y;

  surface =
    mod_gl_utils_create_surface (utils_context, LW6MAP_CURSOR_TEXTURE_SIZE,
				 LW6MAP_CURSOR_TEXTURE_SIZE);
  if (surface)
    {
      for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE; ++y)
	{
	  for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE; ++x)
	    {
	      mod_gl_utils_putpixel (surface, x, y,
				     lw6sys_color_8_to_i
				     (lw6map_cursor_texture_layer_get
				      (&(level->cursor_texture.fg_bg_layer),
				       x, y)));
	    }
	}
      bitmap = mod_gl_utils_surface2bitmap (utils_context, surface);
      if (!bitmap)
	{
	  mod_gl_utils_delete_surface (utils_context, surface);
	  surface = NULL;
	}
    }

  return bitmap;
}

mod_gl_utils_bitmap_t
  * mod_gl_utils_cursor_create_color (mod_gl_utils_context_t * utils_context,
				      lw6map_level_t * level,
				      lw6ker_cursor_t * cursor)
{
  mod_gl_utils_bitmap_t *bitmap = NULL;
  SDL_Surface *surface = NULL;
  int x, y;

  surface =
    mod_gl_utils_create_surface (utils_context, LW6MAP_CURSOR_TEXTURE_SIZE,
				 LW6MAP_CURSOR_TEXTURE_SIZE);
  if (surface)
    {
      for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE; ++y)
	{
	  for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE; ++x)
	    {
	      mod_gl_utils_putpixel (surface, x, y,
				     lw6sys_color_8_to_i
				     (lw6map_cursor_texture_layer_get
				      (&(level->cursor_texture.color_layer),
				       x, y)));
	    }
	}
      bitmap = mod_gl_utils_surface2bitmap (utils_context, surface);
      if (!bitmap)
	{
	  mod_gl_utils_delete_surface (utils_context, surface);
	  surface = NULL;
	}
    }

  return bitmap;
}
