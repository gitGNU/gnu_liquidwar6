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

#include "gfx.h"

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("gfx backend function \"%s\" is not defined"), func_name);
}

/**
 * lw6gfx_init
 *
 * @sys_context: global system context
 * @backend: the graphical backend to use
 * @video_mode: the video mode to use at start up
 * @resize_callback: a callback function which will be called at each resize event
 *
 * Sets up the graphical backend for good, initializing a video mode and allocating
 * ressources. This call can typically fail if there's no device available, if
 * the user doesn't have enough rights to access the hardware, and so on.
 *
 * Return value: 1 on success, 0 if not
 */
int
lw6gfx_init (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_video_mode_t * video_mode, lw6gui_resize_callback_func_t resize_callback)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->init)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  backend->gfx_context = backend->init (sys_context, backend->argc, backend->argv, video_mode, resize_callback);
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->gfx_context ? 1 : 0;
}

/**
 * lw6gfx_quit
 *
 * @sys_context: global system context
 * @backend: the backend to quit
 *
 * Uninitializes the backend, that is, exits the graphical mode. All threads that
 * use graphics must be closed when this is called.
 *
 * Return value: none.
 */
void
lw6gfx_quit (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->quit)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  /*
	   * It's important to check that backend is not NULL for
	   * quit can *really* be called several times on the same backend
	   */
	  if (backend->gfx_context)
	    {
	      backend->quit (sys_context, backend->gfx_context);
	      backend->gfx_context = NULL;
	    }
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6gfx_repr
 *
 * @sys_context: global system context
 * @backend: the backend to represent
 *
 * Returns a readable version of the backend object.
 *
 * Return value: a newly allocated pointer.
 */
char *
lw6gfx_repr (lw6sys_context_t * sys_context, const lw6gfx_backend_t * backend)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  ret = backend->repr (sys_context, backend->gfx_context, backend->id);
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6gfx_set_video_mode
 *
 * @sys_context: global system context
 * @backend: the backend to use
 * @video_mode: the new video mode
 *
 * This function changes the video mode. Note that the first time you set up
 * the graphical environment you must call @lw6gfx_init but to change the current
 * mode, use this function. It should reload backend data automatically if needed
 * (textures for instance). Note that before giving up and failing this function
 * will try alternate video modes, and you're not garanteed to have the right mode
 * after the call, even if it returns true. To check this, use @lw6gfx_get_video_mode.
 *
 * Return value: 1 on success, 0 on failure;
 */
int
lw6gfx_set_video_mode (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->set_video_mode)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  ret = backend->set_video_mode (sys_context, backend->gfx_context, video_mode);
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6gfx_get_video_mode
 *
 * @sys_context: global system context
 * @backend: the backend to use
 * @video_mode: the current video mode (will be overwritten, out parameter)
 *
 * This function returns the current video mode.
 *
 * Return value: 1 on success, 0 on failure;
 */
int
lw6gfx_get_video_mode (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->get_video_mode)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  ret = backend->get_video_mode (sys_context, backend->gfx_context, video_mode);
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6gfx_get_fullscreen_modes
 *
 * @sys_context: global system context
 * @backend: the backend to use
 * @fullscreen_modes: the available fullscreen modes (will be overwritten, out parameter)
 *
 * This function returns the current video mode.
 *
 * Return value: 1 on success, 0 on failure;
 */
int
lw6gfx_get_fullscreen_modes (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_fullscreen_modes_t * fullscreen_modes)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->get_fullscreen_modes)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  ret = backend->get_fullscreen_modes (sys_context, backend->gfx_context, fullscreen_modes);
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6gfx_pump_events
 *
 * @sys_context: global system context
 * @backend: the backend to use
 *
 * This function "pumps" events, that is gets pending events, puts them in queues,
 * maintains internal states up to date. You really must call this very often or
 * no input will be processed at all.
 *
 * Return value: a pointer on the internal input state, musn't be freed.
 */
lw6gui_input_t *
lw6gfx_pump_events (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend)
{
  lw6gui_input_t *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->pump_events)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  ret = backend->pump_events (sys_context, backend->gfx_context);
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return ret;

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6gfx_display
 *
 * @sys_context: global system context
 * @backend: the graphical backend to use
 * @look: the look, the skin, contains display options
 * @mask: display flag, tells what to display
 * @level: the level to display
 * @game_struct: the game_struct associated with the level
 * @game_state: the game_state associated with the level
 * @local_cursors: the cursor to center the focus on
 * @menu: the menu to display
 * @progress: the value of the progress indicator
 * @fps: the number of frames per second to display
 * @mps: the number of moves per second to display
 * @log_list: log messages to display
 * @capture: wether to enable capture mode or not
 * @gfx_debug: wether to enable gfx debugging tools
 * @debug_team_id: for debug display, team to display informations about
 * @debug_layer_id: for debug display, layer to display
 *
 * This is the major drawing function, the one that encapsulates all others. As
 * the program uses a separate thread to display things, we just pass this function
 * many parameters, and let it do its job alone. So many parameters might sometimes
 * be useless. It also allows the graphics backend decide wether menus and hud
 * and background should interact. Or not.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6gfx_display (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, int mask,
		const lw6gui_look_t * look, const lw6map_level_t * level,
		const lw6ker_game_struct_t * game_struct,
		const lw6ker_game_state_t * game_state,
		lw6pil_local_cursors_t * local_cursors, lw6gui_menu_t * menu,
		float progress, float fps, float mps, const char **log_list, int capture, int gfx_debug, int debug_team_id, int debug_layer_id)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->display)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, backend->call_lock))
	{
	  ret =
	    backend->display (sys_context, backend->gfx_context, mask, look, level,
			      game_struct, game_state, local_cursors, menu, progress, fps, mps, log_list, capture, gfx_debug, debug_team_id, debug_layer_id);
	  LW6SYS_MUTEX_UNLOCK (sys_context, backend->call_lock);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
