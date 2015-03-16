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

#include "../../gfx.h"
#include "gl1-utils.h"

/*
 * Basic wrapper to handle errors and use our defaults settings.
 */
SDL_Surface *
mod_gl1_utils_create_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context, int w, int h)
{
  SDL_Surface *image;

  image = SDL_CreateRGBSurface (SDL_SWSURFACE, w, h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN	/* OpenGL RGBA masks */
				0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
#else
				0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
#endif
    );

  if (image != NULL)
    {
      context->surface_counter.new_counter++;
      SDL_SetAlpha (image, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create image"));
    }

  return image;
}

/*
 * Basic wrapper to handle errors and use our defaults settings.
 */
void
mod_gl1_utils_delete_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context, SDL_Surface * surface)
{
  if (surface != NULL)
    {
      context->surface_counter.delete_counter++;
      SDL_FreeSurface (surface);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL surface"));
    }
}

void
mod_gl1_utils_clear_surface_with_color (lw6sys_context_t * sys_context, SDL_Surface * surface, SDL_Color color)
{
  SDL_FillRect (surface, NULL, SDL_MapRGB (surface->format, color.r, color.g, color.b));
}

void
mod_gl1_utils_clear_surface (lw6sys_context_t * sys_context, SDL_Surface * surface)
{
  memset (surface->pixels, 0, surface->pitch * surface->h);
}

static SDL_Surface *
_map2surface_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int x, int y, int w, int h)
{
  SDL_Surface *ret;
  int i, j;
  lw6sys_color_8_t color_8;

  ret = mod_gl1_utils_create_surface (sys_context, utils_context, w, h);

  if (ret)
    {
      /*
       * Blit map to image
       */
      memset (ret->pixels, 0, ret->pitch * ret->h);
      for (i = 0; i < level->texture.w; ++i)
	{
	  for (j = 0; j < level->texture.h; ++j)
	    {
	      color_8 = lw6map_texture_get (&level->texture, i, j);
	      mod_gl1_utils_putpixel (sys_context, ret, i + x, j + y, lw6sys_color_8_to_irgba (color_8));
	    }
	}
    }

  return ret;
}

static SDL_Surface *
_map2surface_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int w, int h)
{
  return _map2surface_xywh (sys_context, utils_context, level, 0, 0, w, h);
}

static SDL_Surface *
_map2surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level)
{
  /*
   * No need to use the closest power of 2 here, it does not
   * make sense for a surface (but it surely does for a texture).
   */
  return _map2surface_wh (sys_context, utils_context, level, level->texture.w, level->texture.h);
}

SDL_Surface *
mod_gl1_utils_map2surface_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int x, int y, int w,
				int h)
{
  SDL_Surface *surface;

  surface = _map2surface_xywh (sys_context, utils_context, level, x, y, w, h);

  return surface;
}

SDL_Surface *
mod_gl1_utils_map2surface_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int w, int h)
{
  SDL_Surface *surface;

  surface = _map2surface_wh (sys_context, utils_context, level, w, h);

  return surface;
}

SDL_Surface *
mod_gl1_utils_map2surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level)
{
  SDL_Surface *surface;

  surface = _map2surface (sys_context, utils_context, level);

  return surface;
}
