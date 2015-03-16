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

#include "../../gfx.h"
#include "gl1-utils.h"

/*
 * A bunch of code here has been ripped from SDL examples,
 * mostly testgl.c
 */

/*
 * Returns the scale to use in a given direction.
 */
float
mod_gl1_utils_texture_scale (lw6sys_context_t * sys_context, int size)
{
  return ((float) size) / ((float) lw6gui_power_of_two_ge (sys_context, size));
}

/*
 * Transforms an SDL bitmap into a proper GL texture.
 */
static GLuint
_surface2texture_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int x, int y, int w, int h, int mipmap)
{
  GLuint texture = 0;

  SDL_Surface *image;
  SDL_Rect dst_area;
  SDL_Rect src_area;
  Uint32 saved_flags;
  Uint8 saved_alpha;

  if (w > utils_context->caps.max_texture_size || h > utils_context->caps.max_texture_size)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("texture is too big (%dx%d) for OpenGL driver (accepts %dx%d)"),
		  w, h, utils_context->caps.max_texture_size, utils_context->caps.max_texture_size);
    }

  w = lw6gui_power_of_two_ge (sys_context, w);
  h = lw6gui_power_of_two_ge (sys_context, h);

  image = mod_gl1_utils_create_surface (sys_context, utils_context, w, h);

  if (image == NULL)
    {
      return 0;
    }

  /* Save the alpha blending attributes */
  saved_flags = surface->flags & (SDL_SRCALPHA | SDL_RLEACCELOK);
  saved_alpha = surface->format->alpha;
  if ((saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA)
    {
      SDL_SetAlpha (surface, 0, 0);
    }

  /* Copy the surface into the GL texture image */
  src_area.x = x;
  src_area.y = y;
  src_area.w = surface->w - x;
  src_area.h = surface->h - y;
  dst_area.x = 0;
  dst_area.y = 0;
  dst_area.w = surface->w;
  dst_area.h = surface->h;
  SDL_BlitSurface (surface, &src_area, image, &dst_area);

  /* Restore the alpha blending attributes */
  if ((saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA)
    {
      SDL_SetAlpha (surface, saved_flags, saved_alpha);
    }

  /* Create an OpenGL texture for the image */
  glGenTextures (1, &texture);
  if (texture)
    {
      glBindTexture (GL_TEXTURE_2D, texture);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      if (mipmap)
	{
	  gluBuild2DMipmaps (GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	}
      else
	{
	  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("glGenTextures failed"));
    }

  if (texture && glIsTexture (texture))
    {
      utils_context->texture_counter.new_counter++;
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new texture %d (%dx%d mipmap=%d)"), (int) texture, w, h, mipmap);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to generate texture from surface x,y=%d,%d w,h=%d,%d"), y, y, w, h);
    }

  mod_gl1_utils_delete_surface (sys_context, utils_context, image);	/* No longer needed */

  return texture;
}

/*
 * Transforms an SDL bitmap into a proper GL texture.
 */
static GLuint
_surface2texture_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int w, int h, int mipmap)
{
  return _surface2texture_xywh (sys_context, utils_context, surface, 0, 0, w, h, mipmap);
}

/*
 * Transforms an SDL bitmap into a proper GL texture.
 */
static GLuint
_surface2texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int mipmap)
{
  int w, h;

  /* Use the surface width and height expanded to powers of 2 */
  w = lw6gui_power_of_two_ge (sys_context, surface->w);
  h = lw6gui_power_of_two_ge (sys_context, surface->h);

  return _surface2texture_wh (sys_context, utils_context, surface, w, h, mipmap);
}

/*
 * Converts a surface to a texture. Public wrapper function.
 */
GLuint
mod_gl1_utils_surface2texture_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int x, int y, int w, int h,
				    int mipmap)
{
  GLuint texture;

  texture = _surface2texture_xywh (sys_context, utils_context, surface, x, y, w, h, mipmap);

  return (int) texture;
}

/*
 * Converts a surface to a texture. Public wrapper function.
 */
GLuint
mod_gl1_utils_surface2texture_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int w, int h, int mipmap)
{
  GLuint texture;

  texture = _surface2texture_wh (sys_context, utils_context, surface, w, h, mipmap);

  return (int) texture;
}

/*
 * Converts a surface to a texture. Public wrapper function.
 */
GLuint
mod_gl1_utils_surface2texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int mipmap)
{
  GLuint texture;

  texture = _surface2texture (sys_context, utils_context, surface, mipmap);

  return (int) texture;
}

/*
 * Transforms an SDL bitmap into a proper GL texture.
 */
static GLuint
_map2texture_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int x, int y, int w, int h)
{
  GLuint texture = 0;
  SDL_Surface *image;

  if (w > utils_context->caps.max_texture_size || h > utils_context->caps.max_texture_size)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map is too big (%dx%d) to be texturized by OpenGL driver"), w, h);
    }

  w = lw6gui_power_of_two_ge (sys_context, w);
  h = lw6gui_power_of_two_ge (sys_context, h);

  image = mod_gl1_utils_map2surface_xywh (sys_context, utils_context, level, x, y, w, h);

  if (image)
    {
      /* Create an OpenGL texture for the image */
      glGenTextures (1, &texture);
      if (texture)
	{
	  glBindTexture (GL_TEXTURE_2D, texture);
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("glGenTextures failed"));
	}

      mod_gl1_utils_delete_surface (sys_context, utils_context, image);	/* No longer needed */
    }

  if (texture && glIsTexture (texture))
    {
      utils_context->texture_counter.new_counter++;
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new map texture %d (%dx%d)"), (int) texture, w, h);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to generate texture from map"));
    }

  return texture;
}

static GLuint
_map2texture_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int w, int h)
{
  return _map2texture_xywh (sys_context, utils_context, level, 0, 0, w, h);
}

static GLuint
_map2texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level)
{
  int w, h;

  /* Use the map width and height expanded to powers of 2 */
  w = lw6gui_power_of_two_ge (sys_context, level->texture.w);
  h = lw6gui_power_of_two_ge (sys_context, level->texture.h);

  return _map2texture_wh (sys_context, utils_context, level, w, h);
}

GLuint
mod_gl1_utils_map2texture_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int x, int y, int w,
				int h)
{
  GLuint texture;

  texture = _map2texture_xywh (sys_context, utils_context, level, x, y, w, h);

  return (int) texture;
}

GLuint
mod_gl1_utils_map2texture_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int w, int h)
{
  GLuint texture;

  texture = _map2texture_wh (sys_context, utils_context, level, w, h);

  return (int) texture;
}

GLuint
mod_gl1_utils_map2texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level)
{
  GLuint texture;

  texture = _map2texture (sys_context, utils_context, level);

  return (int) texture;
}

/*
 * Deletes a texture.
 * Note that calling this directly from just after a glEnd
 * when you're drawing textured stuff might cause ugly unexpected
 * results. Not enough time to find out how OpenGL works internally,
 * my workarround is to create textures, use them, and when glFinish
 * has been called, then we delete all textures at once. In short:
 * use mod_gl1_utils_schedule_delete_texture instead.
 */
void
mod_gl1_utils_delete_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, GLuint texture)
{
  if (texture)
    {
      utils_context->texture_counter.delete_counter++;
      glDeleteTextures (1, (GLuint *) & texture);
      if (glIsTexture ((GLuint) texture))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("delete texture %d"), (int) texture);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trying to delete texture %d, but glIsTexture returns false"), (int) texture);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to delete NULL texture"));
    }
}

/*
 * Schedules a texture to be deleted by mod_gl1_utils_flush_delete_texture
 */
void
mod_gl1_utils_schedule_delete_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, GLuint texture)
{
  GLuint *texture_ptr = NULL;

  texture_ptr = LW6SYS_MALLOC (sys_context, sizeof (GLuint));
  if (texture_ptr)
    {
      *texture_ptr = texture;
      lw6sys_lifo_push (sys_context, &(utils_context->texture_data.to_delete), (void *) texture_ptr);
    }
}

/*
 * Deletes all textures scheduled for delete.
 */
void
mod_gl1_utils_delete_scheduled_textures (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  GLuint *texture_ptr = NULL;

  if (utils_context->texture_data.to_delete != NULL)
    {
      while (!lw6sys_list_is_empty (sys_context, utils_context->texture_data.to_delete))
	{
	  texture_ptr = (GLuint *) lw6sys_lifo_pop (sys_context, &(utils_context->texture_data.to_delete));
	  if (texture_ptr != NULL)
	    {
	      mod_gl1_utils_delete_texture (sys_context, utils_context, *texture_ptr);
	      LW6SYS_FREE (sys_context, texture_ptr);
	    }
	}
    }
}

void
mod_gl1_utils_texture_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, GLuint texture, SDL_Surface * surface)
{
  if (texture && surface)
    {
      /*
       * Note that texture and surface need be the same size, else KABOUM!
       * Usually, one should create the bitmap and the texture with
       * _surface2texture then use this function afterwards on the same
       * objects. Safer. Typical use is displaying armies/fighters.
       */
      glBindTexture (GL_TEXTURE_2D, texture);
      /*
         glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
       */
      if (glIsTexture (texture))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("glTexSubImage2D on %d (%dx%d)"), texture, surface->w, surface->h);
	  glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, surface->w, surface->h, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("%d isn't a texture, can't call glTexSubImage2D"), texture);
	}
    }
}
