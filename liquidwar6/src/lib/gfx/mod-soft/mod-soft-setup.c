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

#include <time.h>

#include "../gfx.h"
#include "mod-soft-internal.h"

/*
 * Low-level SDL initialisation.
 */
_mod_soft_context_t *
_mod_soft_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6gui_video_mode_t * video_mode, lw6gui_resize_callback_func_t resize_callback)
{
  _mod_soft_context_t *soft_context = NULL;
  int sdl_ok = 1;
  int ttf_ok = 1;
  SDL_version version;

  soft_context = (_mod_soft_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_soft_context_t));
#ifndef LW6_ALLINONE
  if (soft_context)
    {
      soft_context->shared_sdl_handle = lw6dyn_dlopen_shared (sys_context, argc, argv, "gfx", "sdl");
      if (soft_context->shared_sdl_handle == NULL)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to load shared SDL code"));
	  _mod_soft_quit (sys_context, soft_context);
	  soft_context = NULL;
	}
    }
#endif // LW6_ALLINONE

  if (soft_context)
    {
      _lw6gfx_sdl_bind_funcs (sys_context, &(soft_context->sdl_context.funcs), soft_context->shared_sdl_handle);

      if (_mod_soft_path_init (sys_context, &(soft_context->path), argc, argv)
	  && _lw6gfx_sdl_path_init (sys_context, &(soft_context->sdl_context), argc, argv))
	{
	  memset (&version, 0, sizeof (SDL_version));
	  SDL_VERSION (&version);
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL header version when compiled %u.%u.%u"), version.major, version.minor, version.patch);
	  version = *SDL_Linked_Version ();
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL linked version now at runtime %u.%u.%u"), version.major, version.minor, version.patch);

	  if (lw6sys_sdl_register (sys_context))
	    {
	      sdl_ok = !SDL_Init (SDL_INIT_EVENTTHREAD);
	    }

	  if (!SDL_WasInit (SDL_INIT_EVENTTHREAD))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to start SDL event thread, events treated in main thread with poll() functions"));
	    }
	  sdl_ok = sdl_ok && (SDL_WasInit (SDL_INIT_VIDEO) || !SDL_InitSubSystem (SDL_INIT_VIDEO));
	  if (sdl_ok)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL Init"));
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("SDL init error: \"%s\""), SDL_GetError ());
	      _mod_soft_quit (sys_context, soft_context);
	      soft_context = NULL;
	    }

	  if (soft_context)
	    {
	      /*
	       * Icon must be set before video mode is set
	       */
	      //_mod_soft_icon_set (soft_context);
	      SDL_EnableUNICODE (1);
	    }

	  if (soft_context)
	    {
	      ttf_ok = (TTF_Init () != -1);
	      if (ttf_ok)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL_ttf Init"));
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("SDL_ttf init error: \"%s\""), TTF_GetError ());
		  _mod_soft_quit (sys_context, soft_context);
		  soft_context = NULL;
		}
	    }

	  if (soft_context)
	    {
	      if (soft_context && sdl_ok && ttf_ok)
		{
		  lw6gui_input_init (sys_context, &(soft_context->sdl_context.input));
		  //_mod_soft_show_mouse (&(soft_context->utils_context), 0, 1);
		  _mod_soft_set_resize_callback (sys_context, soft_context, resize_callback);
		  if (_mod_soft_load_consts (sys_context, soft_context) && _lw6gfx_sdl_load_consts (sys_context, &(soft_context->sdl_context)))
		    {
		      if (_mod_soft_set_video_mode (sys_context, soft_context, video_mode))
			{
			  _lw6gfx_sdl_timer_update (sys_context, &(soft_context->sdl_context));
			  // todo
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("unable to set video mode"));
			  _mod_soft_quit (sys_context, soft_context);
			  soft_context = NULL;
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("unable to load consts"));
		      _mod_soft_quit (sys_context, soft_context);
		      soft_context = NULL;
		    }
		}
	    }
	}
      else
	{
	  LW6SYS_FREE (sys_context, soft_context);
	  soft_context = NULL;
	}
    }

  return soft_context;
}

/*
 * Ends-up all SDL stuff.
 */
void
_mod_soft_quit (lw6sys_context_t * sys_context, _mod_soft_context_t * soft_context)
{
  float quit_sleep = 0.0f;

  /*
   * Keep this value locally since it can disappear
   * when freeing stuff.
   */
  quit_sleep = soft_context->sdl_context.const_data.quit_sleep;

  lw6gui_input_quit (sys_context, &(soft_context->sdl_context.input));

  // todo...

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL_ttf Quit"));
  TTF_Quit ();

  SDL_QuitSubSystem (SDL_INIT_VIDEO);

  if (lw6sys_sdl_unregister (sys_context))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL Quit"));
      SDL_Quit ();
    }

  _lw6gfx_sdl_unload_consts (sys_context, &(soft_context->sdl_context));
  _mod_soft_unload_consts (sys_context, soft_context);

  _lw6gfx_sdl_path_quit (sys_context, &(soft_context->sdl_context));
  _mod_soft_path_quit (sys_context, &(soft_context->path));

  _lw6gfx_sdl_unbind_funcs (sys_context, &(soft_context->sdl_context.funcs));

#ifndef LW6_ALLINONE
  if (soft_context->shared_sdl_handle)
    {
      lw6dyn_dlclose_shared (sys_context, soft_context->shared_sdl_handle);
      soft_context->shared_sdl_handle = NULL;
    }
#endif // LW6_ALLINONE

  LW6SYS_FREE (sys_context, soft_context);

  /*
   * For some reason, I suspect some segfaults occur when
   * "dlclosing" mod-soft just after SDL_Quit. Might be a handler
   * or callback called afterwards, whatever. So I prefer
   * "wasting" a little time when closing, one never knows,
   * it might better things.
   */
  lw6sys_sleep (sys_context, quit_sleep);
}
