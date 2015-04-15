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

#include "dsp-internal.h"

/*
 * This is used to stamp displays as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

/**
 * lw6dsp_create_backend
 *
 * @sys_context: global system context
 * @argc: argc as passed to @main
 * @argv: argv as passed to @main
 * @gfx_backend_name: the id/name of the gfx backend to use
 *
 * Creates a dsp_backend object. The created object won't be displaying
 * things until @lw6dsp_init is called. No thread is created,
 * but the graphics backend is loaded into memory. If video mode is
 * not available, it will appear later, when trying to start displaying
 * things, this function only allocates memory and checks code is
 * available in case of a dynamically loaded gfx backend.
 *
 * Return value: a newly allocated object.
 */
lw6dsp_backend_t *
lw6dsp_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *gfx_backend_name)
{
  lw6dsp_backend_t *ret = NULL;
  _lw6dsp_data_t *data = NULL;
  int ok = 0;

  ret = (lw6dsp_backend_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6dsp_backend_t));
  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}
      data = _lw6dsp_data_new (sys_context);
      if (data)
	{
	  ret->data = (void *) data;
	  data->argc = argc;
	  data->argv = argv;
	  data->gfx_backend_name = lw6sys_str_copy (sys_context, gfx_backend_name);
	  if (data->gfx_backend_name)
	    {
	      if (data->render_mutex)
		{
		  lw6dsp_param_zero (sys_context, &(data->param));
		  if ((ret->input = lw6gui_input_new (sys_context)) != NULL)
		    {
		      ok = 1;
		    }
		}
	    }
	}

      if (!ok)
	{
	  if (data)
	    {
	      _lw6dsp_data_free (sys_context, data);
	    }
	  data = NULL;
	  if (ret->input)
	    {
	      lw6gui_input_free (sys_context, ret->input);
	    }
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6dsp_destroy_backend
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend object to free
 *
 * Frees all ressources used by a dsp_backend object. Note that you must
 * call this on a inactive 'stopped' dsp_backend object.
 *
 * Return value: none.
 */
void
lw6dsp_destroy_backend (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend)
{
  if (dsp_backend)
    {
      if (!(dsp_backend->thread))
	{
	  if (dsp_backend->data)
	    {
	      _lw6dsp_data_free (sys_context, dsp_backend->data);
	      dsp_backend->data = NULL;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't destroy backend with NULL data"));
	    }
	  if (dsp_backend->input)
	    {
	      lw6gui_input_free (sys_context, dsp_backend->input);
	      dsp_backend->input = NULL;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free running dsp_backend"));
	}
      LW6SYS_FREE (sys_context, dsp_backend);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL backend"));
    }
}

/**
 * lw6dsp_repr
 *
 * @sys_context: global system context
 * @dsp_backend: the object to represent
 *
 * Gives a short human-readable description of the object.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6dsp_repr (lw6sys_context_t * sys_context, const lw6dsp_backend_t * dsp_backend)
{
  char *ret = NULL;
  _lw6dsp_data_t *data = NULL;
  char *gfx_backend_repr = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data && data->gfx_backend)
    {
      gfx_backend_repr = lw6gfx_repr (sys_context, data->gfx_backend);
      if (gfx_backend_repr)
	{
	  ret = lw6sys_new_sprintf (sys_context, _x_ ("%u using %s"), dsp_backend->id, gfx_backend_repr);
	  LW6SYS_FREE (sys_context, gfx_backend_repr);
	}
    }
  else
    {
      ret = lw6sys_new_sprintf (sys_context, _x_ ("%u with no gfx"), dsp_backend->id);
    }

  return ret;
}

/**
 * lw6dsp_init
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to start
 * @param: parameters to pass to the display funcs
 * @resize_callback: a function which will be called when there's a resize event
 *
 * Starts a dsp_backend object, that is, fire a separate thread
 * and start rendering. This will set up a video mode, so it's
 * very likely to fail if for some reason the video context isn't
 * right, for instance if you try to set up graphical stuff but
 * only have console access.
 *
 * Return value: 1 if success, 0 if error.
 */
int
lw6dsp_init (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, const lw6dsp_param_t * param, lw6gui_resize_callback_func_t resize_callback)
{
  int ret = 0;
  int thread_ok = 0;
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;
  if (lw6dsp_update (sys_context, dsp_backend, param))
    {
      data->run = 1;
      data->start_ticks = lw6sys_get_timestamp (sys_context);
      data->resize_callback = resize_callback;
      lw6gui_input_reset (sys_context, dsp_backend->input);
      if (lw6sys_vthread_is_running (sys_context))
	{
	  thread_ok =
	    lw6sys_vthread_create (sys_context,
				   (lw6sys_thread_callback_func_t)
				   _lw6dsp_thread_func, (lw6sys_thread_callback_func_t) _lw6dsp_thread_join, dsp_backend->data);
	}
      else
	{
	  dsp_backend->thread =
	    lw6sys_thread_create (sys_context, (lw6sys_thread_callback_func_t) _lw6dsp_thread_func, (lw6sys_thread_callback_func_t) _lw6dsp_thread_join,
				  dsp_backend->data);
	  thread_ok = (dsp_backend->thread != NULL);
	}
      if (thread_ok)
	{
#ifdef LW6_MAC_OS_X
	  data->macosx_thread_handler = _lw6dsp_macosx_thread_init (sys_context);
#endif // LW6_MAC_OS_X
	  while (!data->started)
	    {
	      lw6sys_idle (sys_context);
	    }
	  if (!data->failed)
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("stopping dsp_backend thread"));
	      data->run = 0;
	      if (lw6sys_vthread_is_running (sys_context))
		{
		  lw6sys_vthread_join (sys_context);
		}
	      else
		{
		  lw6sys_thread_join (sys_context, dsp_backend->thread);
		  dsp_backend->thread = NULL;
		}
	    }
	}
    }

  if (!ret)
    {
      data->run = 0;
      data->started = 0;
      data->failed = 0;
      _lw6dsp_param_clear (sys_context, &(data->param));
    }

  return ret;
}

/**
 * lw6dsp_quit
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to stop
 *
 * Stops a dsp_backend, that is, cancel rendering and unset the
 * video mode, hardware shouldn't be used any more after this
 * call.
 *
 * Return value: none.
 */
void
lw6dsp_quit (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend)
{
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;
  /*
   * It's important to check that data->run is not 0 because
   * quit can *really* be called several times on the same backend
   */
  if (data->run)
    {
      data->run = 0;
      data->started = 0;
      data->failed = 0;
#ifdef LW6_MAC_OS_X
      if (data->macosx_thread_handler)
	{
	  _lw6dsp_macosx_thread_quit (sys_context, data->macosx_thread_handler);
	}
#endif // LW6_MAC_OS_X
      if (lw6sys_vthread_is_running (sys_context))
	{
	  lw6sys_vthread_join (sys_context);
	}
      else
	{
	  lw6sys_thread_join (sys_context, dsp_backend->thread);
	}
      lw6gui_input_quit (sys_context, dsp_backend->input);
      _lw6dsp_param_clear (sys_context, &(data->param));
      dsp_backend->thread = NULL;
    }
}

/**
 * lw6dsp_update
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to update
 * @param: parameters to pass to the dsp_backend funcs
 *
 * Passes a new set of parameters to the display function.
 * This is in fact the only way to pass informations to the
 * dsp_backend object once it's been started. This function will
 * acquire a mutex, copy parameters, then give control back
 * to the main thread while display keeps on going with new
 * parameters in the background. It will get input informations.
 * You really must call it often otherwise the screen won't get
 * updated, or, at least, it will always display the same informations.
 * It should be reasonnable to call this 10 or 20 times per second,
 * the display itself can be faster, run at 60 or 100 fps to show
 * smooth animation (eye candy).
 *
 * Return value: 1 if success, 0 if error.
 */
int
lw6dsp_update (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, const lw6dsp_param_t * param)
{
  int ret = 1;
  _lw6dsp_data_t *data = NULL;
  int diff = 0;
  int need_sync = 0;
  int i = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("dsp update begin"));

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data)
    {
      /*
       * We don't go idle when nb_frames==0, because we
       * need to actually update the param to bootstrap
       */
      if (data->nb_frames > 0)
	{
	  /*
	   * No need to do anything until next frame is drawn
	   */
	  while (data->nb_frames <= data->nb_frames_at_last_update && i < _LW6DSP_IDLE_LIMIT)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("calling display_update too often, no frame drawn between two calls"));
	      /*
	       * We go idle for a very short time, this will just avoid the
	       * caller to get stuck in 100% CPU greedy loops.
	       */
	      lw6sys_idle (sys_context);
	      ++i;
	    }
	}

      /*
       * Note that it's really important to update data every time the
       * function is called for caller can rely on the fact that if
       * a NULL pointer has been passed for level, game_struct, game_state
       * of pilot, then the parent object can be freed.
       */
      data->nb_frames_at_last_update = data->nb_frames;
      diff = _lw6dsp_param_diff (sys_context, data, param);
      if (data->input)
	{
	  need_sync = lw6gui_input_need_sync (sys_context, data->input);
	}
      if (diff || need_sync)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("dsp param update diff=%d sync=%d"), diff, need_sync);

	  LW6SYS_MUTEX_LOCK (sys_context, data->render_mutex);

	  if (diff & _LW6DSP_PARAM_DIFF_MISC)
	    {
	      data->param.misc = param->misc;
	    }

	  if (diff & _LW6DSP_PARAM_DIFF_VIDEO_MODE)
	    {
	      data->param.video_mode = param->video_mode;
	    }

	  if (diff & _LW6DSP_PARAM_DIFF_LOOK)
	    {
	      if (data->param.look)
		{
		  lw6gui_look_free (sys_context, data->param.look);
		}
	      data->param.look = lw6gui_look_dup (sys_context, param->look);
	    }

	  if (diff & _LW6DSP_PARAM_DIFF_MENU)
	    {
	      if (data->param.menu)
		{
		  if (param->menu)
		    {
		      if (data->last_menu_id == param->menu->id)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("synchronising menu"));
			  lw6gui_menu_sync (sys_context, data->param.menu, param->menu);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new menu is totally different, duping it"));
			  lw6gui_menu_free (sys_context, data->param.menu);
			  data->param.menu = lw6gui_menu_dup (sys_context, param->menu);
			  data->last_menu_id = param->menu->id;
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new menu is NULL"));
		      lw6gui_menu_free (sys_context, data->param.menu);
		      data->param.menu = NULL;
		      data->last_menu_id = 0;
		    }
		}
	      else
		{
		  if (param->menu)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new menu and no previous one, duping it"));
		      data->param.menu = lw6gui_menu_dup (sys_context, param->menu);
		      data->last_menu_id = param->menu->id;
		    }
		}
	    }

	  if (diff & _LW6DSP_PARAM_DIFF_LEVEL)
	    {
	      data->param.level = param->level;
	    }

	  if (diff & _LW6DSP_PARAM_DIFF_GAME_STRUCT)
	    {
	      data->param.game_struct = param->game_struct;
	    }

	  if (diff & _LW6DSP_PARAM_DIFF_GAME_STATE)
	    {
	      data->slow_mps = 0;
	      data->param.game_state = param->game_state;
	    }

	  if (diff & _LW6DSP_PARAM_DIFF_PILOT)
	    {
	      data->param.pilot = param->pilot;
	    }
	  if (dsp_backend->input != NULL && data->input != NULL)
	    {
	      lw6gui_input_sync (sys_context, dsp_backend->input, data->input);
	    }

	  LW6SYS_MUTEX_UNLOCK (sys_context, data->render_mutex);
	}
    }

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("dsp update end"));

  return ret;
}

/**
 * lw6dsp_get_nb_frames
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to query
 *
 * Returns the number of frames displayed since the display was started.
 *
 * Return value: the number of frames displayed.
 */
int
lw6dsp_get_nb_frames (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend)
{
  int ret = 0;
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data)
    {
      /*
       * We don't lock/unlock a mutex, it's assumed an atomic type
       * affectation shouldn't be affected by concurrency.
       */
      ret = data->nb_frames;
    }

  return ret;
}

/**
 * lw6dsp_get_last_frame_rendering_time
 *
 * @dsp_backend: the dsp_backend to query
 *
 * Returns the rendering time of the last frame. Gives clues about performance.
 *
 * Return value: the number of milliseconds it took to draw screen
 */
int
lw6dsp_get_last_frame_rendering_time (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend)
{
  int ret = 0;
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data)
    {
      /*
       * We don't lock/unlock a mutex, it's assumed an atomic type
       * affectation shouldn't be affected by concurrency.
       */
      ret = data->last_frame_rendering_time;
    }

  return ret;
}

/**
 * lw6dsp_get_instant_fps
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to query
 *
 * Returns the current frames per sec display rate. This is the instant value,
 * it changes very often even if display seems smooth.
 *
 * Return value: the current instant display rate.
 */
int
lw6dsp_get_instant_fps (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend)
{
  int ret = 0;
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data)
    {
      /*
       * We don't lock/unlock a mutex, it's assumed an atomic type
       * affectation shouldn't be affected by concurrency.
       */
      ret = (int) data->instant_fps;
    }

  return ret;
}

/**
 * lw6dsp_get_average_fps
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to query
 *
 * Returns the current frames per sec display rate. This is not absolutely
 * accurate but fits for displaying info to the player, it's an average.
 *
 * Return value: the current averaged display rate.
 */
int
lw6dsp_get_average_fps (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend)
{
  int ret = 0;
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data)
    {
      /*
       * We don't lock/unlock a mutex, it's assumed an atomic type
       * affectation shouldn't be affected by concurrency.
       */
      ret = (int) data->average_fps;
    }

  return ret;
}

/**
 * lw6dsp_get_video_mode
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to query
 * @video_mode: a structure which will contain the results
 *
 * Returns the current video mode, the one obtained by the driver.
 * This function is also a way to know wether display is running correcly
 * or not, by testing its return value.
 *
 * Return value: 1 if ok, 0 if failure (mode not set)
 */
int
lw6dsp_get_video_mode (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data)
    {
      /*
       * Here we can't do a dirty read as with atomic values such as fps,
       * so we reserve the mutex
       */
      LW6SYS_MUTEX_LOCK (sys_context, data->render_mutex);
      if (data->run && data->started)
	{
	  (*video_mode) = data->video_mode_obtained;
	  ret = 1;
	}
      LW6SYS_MUTEX_UNLOCK (sys_context, data->render_mutex);
    }

  return ret;
}

/**
 * lw6dsp_get_fullscreen_modes
 *
 * @sys_context: global system context
 * @dsp_backend: the dsp_backend to query
 * @fullscreen_modes: a structure which will contain the results
 *
 * Returns the current available fullscreen modes. Note that this one
 * will only work if display is started, unlike @lw6gfx_get_fullscreen_modes which
 * is used internally. The reason is that in this dsp module context, we need
 * the thread to be launched, and the thread does start/stop display on its own.
 *
 * Return value: 1 if ok, 0 if failure (mode not set)
 */
int
lw6dsp_get_fullscreen_modes (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, lw6gui_fullscreen_modes_t * fullscreen_modes)
{
  int ret = 0;
  _lw6dsp_data_t *data = NULL;

  data = (_lw6dsp_data_t *) dsp_backend->data;

  if (data)
    {
      /*
       * Here we can't do a dirty read as with atomic values such as fps,
       * so we reserve the mutex
       */
      LW6SYS_MUTEX_LOCK (sys_context, data->render_mutex);
      if (data->run && data->started)
	{
	  (*fullscreen_modes) = data->fullscreen_modes;
	  ret = 1;
	}
      LW6SYS_MUTEX_UNLOCK (sys_context, data->render_mutex);
    }

  return ret;
}
