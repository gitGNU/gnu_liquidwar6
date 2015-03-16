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

#define _FILE_MASK "%08u.bmp"

SDL_Surface *
mod_gl1_utils_capture2surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  SDL_Surface *surface_tmp = NULL;
  SDL_Surface *surface_ret = NULL;
  u_int32_t *pixels_tmp = NULL;
  u_int32_t *pixels_ret = NULL;
  int x, y, w, h;
  int offset_tmp, offset_ret;

  w = utils_context->sdl_context.video_mode.width;
  h = utils_context->sdl_context.video_mode.height;
  surface_tmp = mod_gl1_utils_create_surface (sys_context, utils_context, w, h);
  if (surface_tmp)
    {
      pixels_tmp = (u_int32_t *) surface_tmp->pixels;
      surface_ret = mod_gl1_utils_create_surface (sys_context, utils_context, w, h);
      if (surface_ret)
	{
	  pixels_ret = (u_int32_t *) surface_ret->pixels;
	  glFinish ();
	  glReadPixels (0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels_tmp);
	  /*
	   * Now surface_tmp contains an upside-down image (OpenGL
	   * and SDL conventions aren't the same) so we need to invert it
	   */
	  for (y = 0; y < h; ++y)
	    {
	      offset_tmp = y * w;
	      offset_ret = (h - y - 1) * w;
	      for (x = 0; x < w; ++x)
		{
		  pixels_ret[offset_ret + x] = pixels_tmp[offset_tmp + x];
		}
	    }
	}
      mod_gl1_utils_delete_surface (sys_context, utils_context, surface_tmp);
    }

  return surface_ret;
}

int
mod_gl1_utils_capture2disk (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  int ret = 0;
  SDL_Surface *surface = NULL;
  char *file = NULL;
  char *full_path = NULL;

  surface = mod_gl1_utils_capture2surface (sys_context, utils_context);
  if (surface)
    {
      if (!lw6sys_dir_exists (sys_context, utils_context->path.capture_dir))
	{
	  lw6sys_create_dir (sys_context, utils_context->path.capture_dir);
	}
      if (lw6sys_dir_exists (sys_context, utils_context->path.capture_dir))
	{
	  file = lw6sys_new_sprintf (sys_context, _FILE_MASK, utils_context->counter.nb_frames);
	  if (file)
	    {
	      full_path = lw6sys_path_concat (sys_context, utils_context->path.capture_dir, file);
	      if (full_path)
		{
		  if (SDL_SaveBMP (surface, full_path) == 0)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("wrote capture \"%s\""), full_path);
		      ret = 1;
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to write capture \"%s\" to disk"), full_path);
		    }
		  LW6SYS_FREE (sys_context, full_path);
		}
	      LW6SYS_FREE (sys_context, file);
	    }
	}
      mod_gl1_utils_delete_surface (sys_context, utils_context, surface);
    }

  return ret;
}
