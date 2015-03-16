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

#include "mod-gles2-internal.h"

/*
 * Initialize display.
 */
int
_mod_gles2_set_video_mode (lw6sys_context_t * sys_context, _mod_gles2_context_t * gles2_context, lw6gui_video_mode_t * video_mode)
{
  /* Information about the current video settings. */
  const SDL_VideoInfo *info = NULL;
  /* Color depth in bits of our window. */
  int bpp = 0;
  /* Flags we will pass into SDL_SetVideoMode. */
  int flags = 0;
  int width = 0;
  int height = 0;
  int width_test = 0;
  int height_test = 0;
  int fullscreen;
  lw6gui_video_mode_t ratio_mode = { 0, 0, 0 };
  lw6gui_video_mode_t target_mode = { 0, 0, 0 };
  lw6gui_fullscreen_modes_t fullscreen_modes;
  int ok = 1;
  SDL_Surface *video_surface = NULL;
  int warp_x = 0;
  int warp_y = 0;

  width = video_mode->width;
  height = video_mode->height;
  fullscreen = video_mode->fullscreen;

  lw6sys_sleep (sys_context, gles2_context->sdl_context.const_data.mode_sleep);

  /* Let's get some video information. */
  info = SDL_GetVideoInfo ();

  if (info)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		  _x_ ("SDL VideoInfo hw_available=%d wm_available=%d video_mem=%dkb"), info->hw_available, info->wm_available, info->video_mem);
    }
  else
    {
      /* This should probably never happen. */
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("SDL GetVideoInfo failed: \"%s\""), SDL_GetError ());
      fflush (stderr);
      ok = 0;
    }

  if (ok)
    {
      bpp = info->vfmt->BitsPerPixel;

      /*
       * Now, we want to setup our requested
       * window attributes for our OpenGL window.
       * We want *at least* 5 bits of red, green
       * and blue. We also want at least a 16-bit
       * depth buffer.
       *
       * The last thing we do is request a double
       * buffered window. '1' turns on double
       * buffering, '0' turns it off.
       *
       * Note that we do not use SDL_DOUBLEBUF in
       * the flags to SDL_SetVideoMode. That does
       * not affect the GL attribute state, only
       * the standard 2D blitting setup.
       */
      switch (bpp)
	{
	case 8:
	  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 3);
	  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 3);
	  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 2);
	  break;
	case 15:
	case 16:
	  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 5);
	  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 5);
	  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 5);
	  break;
	default:
	  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
	  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
	  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
	  break;
	}
      SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
      SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
      flags = SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE);

      if (width <= 0 || height <= 0)
	{
	  if (_mod_gles2_get_fullscreen_modes (sys_context, gles2_context, &fullscreen_modes))
	    {
	      width = fullscreen_modes.standard.width;
	      height = fullscreen_modes.standard.height;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mode %dx%d selected automatically"), width, height);
	    }
	}

      if (fullscreen)
	{
	  if (_mod_gles2_get_fullscreen_modes (sys_context, gles2_context, &fullscreen_modes))
	    {
	      ratio_mode = fullscreen_modes.high;
	      target_mode.width = width;
	      target_mode.height = height;
	      if (lw6gui_video_mode_sync_ratio (sys_context, &target_mode, &ratio_mode))
		{
		  width_test = target_mode.width;
		  height_test = target_mode.height;
		  _mod_gles2_find_closest_resolution (sys_context, gles2_context, &width_test, &height_test, width_test, height_test);
		  if (width_test != width || height_test != height)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("trying mode %dx%d which seems to fit best than %dx%d"), width_test, height_test, width, height);
		      width = width_test;
		      height = height_test;
		    }
		}
	    }
	}

      /*
       * Set the video mode
       */
      if (SDL_SetVideoMode (width, height, bpp, flags) == 0)
	{
	  /*
	   * This could happen for a variety of reasons,
	   * including DISPLAY not being set, the specified
	   * resolution not being available, etc.
	   */
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR,
		      _("SDL SetVideoMode %dx%d bpp=%d fullscreen=%d failed: \"%s\""), width, height, bpp, fullscreen, SDL_GetError ());
	}
      else
	{
	  video_surface = SDL_GetVideoSurface ();
	  if (video_surface)
	    {
	      if (video_surface->w != width || video_surface->h != height)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("video surface is %dx%d and not the requested %dx%d"), video_surface->w, video_surface->h, width, height);
		}
	      width = video_surface->w;
	      height = video_surface->h;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get video surface"));
	    }

	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL SetVideoMode %dx%d bpp=%d fullscreen=%d"), width, height, bpp, fullscreen);
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("%d BPP"), SDL_GetVideoSurface ()->format->BitsPerPixel);
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("OpenGL vendor \"%s\""), glGetString (GL_VENDOR));
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("OpenGL renderer \"%s\""), glGetString (GL_RENDERER));
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("OpenGL version \"%s\""), glGetString (GL_VERSION));
	}

      if (ok)
	{
	  //_mod_gles2_show_mouse (gles2_context, 0, 1);
	  SDL_PumpEvents ();
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL events pumped"));

	  gles2_context->sdl_context.video_mode.width = width;
	  gles2_context->sdl_context.video_mode.height = height;
	  gles2_context->sdl_context.video_mode.fullscreen = fullscreen;
	  gles2_context->caps.bpp = bpp;
	  gles2_context->caps.max_texture_size = 0;
	  _mod_gles2_sync_viewport (sys_context, gles2_context);
	  glGetIntegerv (GL_MAX_TEXTURE_SIZE, &(gles2_context->caps.max_texture_size));
	  if (!gles2_context->caps.max_texture_size)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get MAX_TEXTURE_SIZE"));
	      gles2_context->caps.max_texture_size = 512;
	    }
	}

      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		  _x_ ("set caption to \"%s\" - \"%s\""), lw6sys_build_get_package_string (), lw6sys_build_get_package_tarname ());
      SDL_WM_SetCaption (lw6sys_build_get_package_string (), lw6sys_build_get_package_tarname ());

      if (fullscreen)
	{
	  warp_x = gles2_context->sdl_context.const_data.warp_x * (float) width;
	  warp_y = gles2_context->sdl_context.const_data.warp_y * (float) height;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("warp mouse to %d,%d"), warp_x, warp_y);
	  SDL_WarpMouse (warp_x, warp_y);
	}

      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("OpenGL MAX_TEXTURE_SIZE = %d"), gles2_context->caps.max_texture_size);

      if (gles2_context->caps.max_texture_size < _MOD_GLES2_REQUIRED_TEXTURE_SIZE)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("OpenGL driver only handles %dx%d textures when %dx%d textures are needed"),
		      gles2_context->caps.max_texture_size,
		      gles2_context->caps.max_texture_size, _MOD_GLES2_REQUIRED_TEXTURE_SIZE, _MOD_GLES2_REQUIRED_TEXTURE_SIZE);
	}

      /*
       * Call this function to be sure we're truely in some known mode
       */
      //_mod_gles2_set_render_mode_2d (gles2_context);

      _mod_gles2_call_resize_callback (sys_context, gles2_context);
      //_mod_gles2_timer_set_bitmap_refresh (gles2_context);
      //_mod_gles2_smoothers_reset_drawable (gles2_context);

      lw6sys_sleep (sys_context, gles2_context->sdl_context.const_data.mode_sleep);
    }

  return ok;
}


/*
 * Called whenever window resize is asked for.
 */
int
_mod_gles2_resize_video_mode (lw6sys_context_t * sys_context, _mod_gles2_context_t * gles2_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  if (video_mode->width != gles2_context->sdl_context.video_mode.width || video_mode->height != gles2_context->sdl_context.video_mode.height)
    {
      int flags = 0;

      flags = SDL_OPENGL | (video_mode->fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE);

      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request screen %dx%d fullscreen=%d"), video_mode->width, video_mode->height, video_mode->fullscreen);
      if (SDL_SetVideoMode (video_mode->width, video_mode->height, gles2_context->caps.bpp, flags) != 0)
	{
	  gles2_context->sdl_context.video_mode = *video_mode;
	  _mod_gles2_sync_viewport (sys_context, gles2_context);

	  _mod_gles2_call_resize_callback (sys_context, gles2_context);

	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("screen set to %dx%d fullscreen=%d"), video_mode->width, video_mode->height, video_mode->fullscreen);

	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("unable to resize screen %dx%d fullscreen=%d"), video_mode->width, video_mode->height, video_mode->fullscreen);
	}
    }
  else
    {
      /*
       * Nothing to do...
       */

      ret = 1;
    }

  //_mod_gles2_timer_set_bitmap_refresh (gles2_context);
  //_mod_gles2_smoothers_reset_drawable (gles2_context);

  return ret;
}

int
_mod_gles2_get_video_mode (lw6sys_context_t * sys_context, _mod_gles2_context_t * gles2_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  (*video_mode) = gles2_context->sdl_context.video_mode;

  ret = 1;

  return ret;
}

/*
 * Update viewport
 */
int
_mod_gles2_sync_viewport (lw6sys_context_t * sys_context, _mod_gles2_context_t * gles2_context)
{
  int ret = 1;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("glViewport %dx%d"),
	      gles2_context->sdl_context.video_mode.width, gles2_context->sdl_context.video_mode.height);
  glViewport (0, 0, gles2_context->sdl_context.video_mode.width, gles2_context->sdl_context.video_mode.height);
  _mod_gles2_viewport_drawable_max (sys_context, gles2_context);

  return ret;
}

/*
 * Force mode.
 */
int
_mod_gles2_sync_mode (lw6sys_context_t * sys_context, _mod_gles2_context_t * gles2_context, int force)
{
  int ret = 0;
  lw6gui_video_mode_t video_mode;
  int flags = 0;

  _mod_gles2_get_video_mode (sys_context, gles2_context, &video_mode);
  flags = SDL_OPENGL | (video_mode.fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE);

  if (force)
    {
      if (SDL_SetVideoMode (video_mode.width, video_mode.height, gles2_context->caps.bpp, flags) == 0)
	{
	  _mod_gles2_sync_viewport (sys_context, gles2_context);
	  //_mod_gles2_timer_set_bitmap_refresh (gles2_context);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("unable to sync screen %dx%d fullscreen=%d"), video_mode.width, video_mode.height, video_mode.fullscreen);
	}
    }
  else
    {
      _mod_gles2_sync_viewport (sys_context, gles2_context);
      ret = 1;
    }

  return ret;
}

int
_mod_gles2_set_resize_callback (lw6sys_context_t * sys_context, _mod_gles2_context_t * gles2_context, lw6gui_resize_callback_func_t resize_callback)
{
  int ret = 0;

  gles2_context->sdl_context.resize_callback = resize_callback;
  ret = 1;

  return ret;
}

void
_mod_gles2_call_resize_callback (lw6sys_context_t * sys_context, _mod_gles2_context_t * gles2_context)
{
  if (gles2_context->sdl_context.resize_callback)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("calling resize callback"));
      gles2_context->sdl_context.resize_callback (sys_context, &(gles2_context->sdl_context.video_mode));
    }
}
