/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include <time.h>

#include "../gfx.h"
#include "mod-gles2-internal.h"

/*
 * Low-level SDL initialisation.
 */
_mod_gles2_context_t *
_mod_gles2_init (int argc, const char *argv[],
		 lw6gui_video_mode_t * video_mode,
		 lw6gui_resize_callback_func_t resize_callback)
{
  _mod_gles2_context_t *gl_context = NULL;
  int sdl_ok = 1;
  SDL_version version;

  gl_context =
    (_mod_gles2_context_t *) LW6SYS_CALLOC (sizeof (_mod_gles2_context_t));
  if (gl_context)
    {
      if (_mod_gles2_path_init (&(gl_context->path), argc, argv))
	{
	  memset (&version, 0, sizeof (SDL_version));
	  SDL_VERSION (&version);
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("SDL header version when compiled %u.%u.%u"),
		      version.major, version.minor, version.patch);
	  version = *SDL_Linked_Version ();
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("SDL linked version now at runtime %u.%u.%u"),
		      version.major, version.minor, version.patch);

	  if (lw6sys_sdl_register ())
	    {
	      sdl_ok = !SDL_Init (SDL_INIT_EVENTTHREAD);
	    }

	  if (!SDL_WasInit (SDL_INIT_EVENTTHREAD))
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("unable to start SDL event thread, events treated in main thread with poll() functions"));
	    }
	  sdl_ok = sdl_ok && (SDL_WasInit (SDL_INIT_VIDEO)
			      || !SDL_InitSubSystem (SDL_INIT_VIDEO));
	  if (sdl_ok)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("SDL Init"));
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_ERROR, _("SDL init error: \"%s\""),
			  SDL_GetError ());
	      _mod_gles2_quit (gl_context);
	      gl_context = NULL;
	    }

	  if (gl_context)
	    {
	      // todo
	    }
	}
      else
	{
	  LW6SYS_FREE (gl_context);
	  gl_context = NULL;
	}
    }

  return gl_context;
}

/*
 * Ends-up all SDL stuff.
 */
void
_mod_gles2_quit (_mod_gles2_context_t * gl_context)
{
  // todo...

  SDL_QuitSubSystem (SDL_INIT_VIDEO);

  if (lw6sys_sdl_unregister ())
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("SDL Quit"));
      SDL_Quit ();
    }

  _mod_gles2_path_quit (&(gl_context->path));

  LW6SYS_FREE (gl_context);
}
