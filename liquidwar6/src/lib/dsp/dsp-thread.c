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

#include "dsp-internal.h"

#define WEIGHT_INSTANT_FPS 9.0f
#define WEIGHT_AVERAGE_FPS 1.0f
#define MPS_REFRESH_PER_SEC 2

void static
_init (lw6sys_context_t * sys_context, _lw6dsp_data_t * data)
{
  int ok = 0;
  lw6gui_fullscreen_modes_t fullscreen_modes;

  data->video_mode_requested = data->param.video_mode;

  data->gfx_backend = lw6gfx_create_backend (sys_context, data->argc, data->argv, data->gfx_backend_name);
  if (data->gfx_backend)
    {
      lw6gfx_get_fullscreen_modes (sys_context, data->gfx_backend, &data->fullscreen_modes);
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("dsp init requested %dx%d fullscreen=%d mode"),
		  data->video_mode_requested.width, data->video_mode_requested.height, data->video_mode_requested.fullscreen);
      if (lw6gfx_init (sys_context, data->gfx_backend, &(data->video_mode_requested), data->resize_callback))
	{
	  /*
	   * We call it here too for it seems to work better once init
	   * has been called...
	   */
	  lw6gfx_get_fullscreen_modes (sys_context, data->gfx_backend, &data->fullscreen_modes);
	  if (lw6gfx_get_video_mode (sys_context, data->gfx_backend, &(data->video_mode_obtained)))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("dsp init obtained %dx%d fullscreen=%d mode"),
			  data->video_mode_obtained.width, data->video_mode_obtained.height, data->video_mode_obtained.fullscreen);
	      ok = 1;
	    }
	  else
	    {
	      lw6gfx_quit (sys_context, data->gfx_backend);
	    }
	}
      if (!ok)
	{
	  if (data->video_mode_obtained.width <= 0 || data->video_mode_obtained.height <= 0)
	    {
	      if (lw6gfx_get_fullscreen_modes (sys_context, data->gfx_backend, &fullscreen_modes))
		{
		  data->video_mode_requested.width = fullscreen_modes.standard.width;
		  data->video_mode_requested.height = fullscreen_modes.standard.height;
		}
	    }

	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("couldn't set up mode, trying fallback %dx%d fullscreen=%d"),
		      data->video_mode_requested.width, data->video_mode_requested.height, data->video_mode_requested.fullscreen);
	  if (lw6gfx_init (sys_context, data->gfx_backend, &(data->video_mode_requested), data->resize_callback))
	    {
	      if (lw6gfx_get_video_mode (sys_context, data->gfx_backend, &(data->video_mode_obtained)))
		{
		  ok = 1;
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_
			      ("dsp init obtained %dx%d fullscreen=%d mode"),
			      data->video_mode_obtained.width, data->video_mode_obtained.height, data->video_mode_obtained.fullscreen);
		}
	      else
		{
		  lw6gfx_quit (sys_context, data->gfx_backend);
		}
	    }
	}
      if (!ok)
	{
	  lw6gfx_destroy_backend (sys_context, data->gfx_backend);
	  data->gfx_backend = NULL;
	}
    }

  if (!ok)
    {
      data->failed = 1;
    }
  data->started = 1;
}

static int
_poll (lw6sys_context_t * sys_context, _lw6dsp_data_t * data)
{
  int ret = 1;
  char **history = NULL;
  int64_t timestamp;
  lw6ker_game_state_t *game_state = NULL;
  lw6pil_local_cursors_t *local_cursors = NULL;
  float progress = 0.0f;
  int mask = 0;

  timestamp = lw6sys_get_timestamp (sys_context);
  data->input = lw6gfx_pump_events (sys_context, data->gfx_backend);
  lw6gui_input_update_repeat (sys_context, data->input, &(data->param.misc.repeat_settings), timestamp);
  mask = data->param.misc.mask;
  if (data->slow_fps)
    {
      mask |= LW6GUI_DISPLAY_FPS;
    }
  if (data->slow_mps)
    {
      mask |= LW6GUI_DISPLAY_MPS;
    }

  if (data->param.pilot)
    {
      if (!(data->param.game_state))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("pilot defined but not game_state"));
	  ret = 0;
	}
      if (!lw6pil_pilot_can_sync (sys_context, data->param.game_state, data->param.pilot))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_state state can't be synced from pilot"));
	  ret = 0;
	}
    }

  if (data->param.game_state)
    {
      if (!(data->param.game_struct))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_state defined but not game_struct"));
	  ret = 0;
	}
      if (data->param.game_state->game_struct != data->param.game_struct)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("game_state->game_struct=%p but game_struct=%p, they should be the same"),
		      data->param.game_state->game_struct, data->param.game_struct);
	  ret = 0;
	}
    }

  if (data->param.game_struct)
    {
      if (!(data->param.level))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_struct defined but not level"));
	  ret = 0;
	}
      if (data->param.game_struct->level != data->param.level)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("game_struct->level=%p but level=%p, they should be the same"), data->param.game_struct->level, data->param.level);
	  ret = 0;
	}
    }

  if (data->param.game_state)
    {
      game_state = data->param.game_state;
    }

  local_cursors = &(data->local_cursors);
  if (data->param.pilot)
    {
      switch (data->param.misc.dirty_read)
	{
	case LW6PIL_DIRTY_READ_NEVER:
	case LW6PIL_DIRTY_READ_SYNC_ONLY:
	  lw6pil_pilot_sync_from_draft (sys_context, game_state, data->param.pilot, data->param.misc.dirty_read);
	  /*
	   * Here we don't sync completely correctly, the local_cursors
	   * might contain inconsistent data, but at least it won't
	   * be modified *while* we are reading it.
	   */
	  data->local_cursors = (*lw6pil_pilot_get_local_cursors (sys_context, data->param.pilot));
	  break;
	case LW6PIL_DIRTY_READ_ALWAYS:
	  game_state = lw6pil_pilot_dirty_read (sys_context, data->param.pilot);
	  local_cursors = lw6pil_pilot_get_local_cursors (sys_context, data->param.pilot);
	  break;
	default:
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value %d for dirty_read"), data->param.misc.dirty_read);
	  break;
	}
    }

  if (game_state)
    {
      data->rounds = lw6ker_game_state_get_rounds (sys_context, game_state);
    }

  if (!(data->param.look))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("look is NULL"));
      ret = 0;
    }
  if (data->param.misc.progress)
    {
      progress = (*(data->param.misc.progress));
    }
  if (ret)
    {
      history = lw6sys_history_get (sys_context, data->param.misc.log_timeout);
      if (history)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display dsp->gfx begin"));
	  if (lw6gfx_display (sys_context, data->gfx_backend,
			      mask,
			      data->param.look,
			      data->param.level,
			      data->param.game_struct,
			      game_state,
			      local_cursors,
			      data->param.menu,
			      progress,
			      data->average_fps, data->mps,
			      (const char **) history,
			      data->param.misc.capture, data->param.misc.gfx_debug, data->param.misc.debug_team_id, data->param.misc.debug_layer_id))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("display dsp->gfx end"));
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display dsp->gfx problem"));
	      ret = 0;
	    }
	  lw6sys_history_free (sys_context, history);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't get log history"));
	  ret = 0;
	}
    }

  return ret;
}

static int
_loop (lw6sys_context_t * sys_context, _lw6dsp_data_t * data)
{
  int ret = 0;

  if (!lw6gui_video_mode_is_same (sys_context, &(data->video_mode_requested), &(data->param.video_mode)))
    {
      data->video_mode_requested = data->param.video_mode;
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("dsp update request %dx%d fullscreen=%d mode"),
		  data->video_mode_requested.width, data->video_mode_requested.height, data->video_mode_requested.fullscreen);
      if (lw6gfx_set_video_mode (sys_context, data->gfx_backend, &(data->video_mode_requested)))
	{
	  if (lw6gfx_get_video_mode (sys_context, data->gfx_backend, &(data->video_mode_obtained)))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_
			  ("dsp update obtained %dx%d fullscreen=%d mode"),
			  data->video_mode_obtained.width, data->video_mode_obtained.height, data->video_mode_obtained.fullscreen);
	    }
	}
    }

  ret = _poll (sys_context, data);

  return ret;
}

void
_lw6dsp_thread_func (lw6sys_context_t * sys_context, _lw6dsp_data_t * data)
{
  int frames_increment = 1;	// mustn't be 0
  int frames_counter = 0;
  int64_t nb_ticks = 0;
  int64_t last_display_ticks = 0;
  int64_t last_rounds_ticks = 0;
  int64_t ticks = 0;
  int64_t delta_ticks = 0;
  int64_t cpu_ticks = 0;
  int rounds;
  int delta_rounds = 0;
  int last_display_rounds = 0;
  int do_skip = 0;
  int delay_skip = 1;
  int delay_cpu = 0;
  int delay_real = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display thread"));

  LW6SYS_MUTEX_LOCK (sys_context, data->render_mutex);

#ifdef LW6_MAC_OS_X
  data->macosx_vthread_handler = _lw6dsp_macosx_thread_init (sys_context);
#endif // LW6_MAC_OS_X

  if (!data->gfx_backend)
    {
      _init (sys_context, data);
    }

  LW6SYS_MUTEX_UNLOCK (sys_context, data->render_mutex);

  if (data->gfx_backend)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display loop"));
      while (data->run)
	{
	  ticks = lw6sys_get_timestamp (sys_context);

	  LW6SYS_MUTEX_LOCK (sys_context, data->render_mutex);

	  if (data->param.misc.target_fps > 0)
	    {
	      frames_increment = LW6SYS_TICKS_PER_SEC / data->param.misc.target_fps;
	      delay_skip = LW6SYS_TICKS_PER_SEC / (2 * data->param.misc.target_fps);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't handle target_fps of %d"), data->param.misc.target_fps);
	    }
	  frames_increment = lw6sys_imax (frames_increment, 1);
	  delay_skip = lw6sys_imax (delay_skip, 1);

	  nb_ticks = ticks - data->start_ticks;

	  if (frames_counter < nb_ticks)
	    {
	      /*
	       * The following is more efficient than a plain +=
	       * it will avoid going too fast after a slowdown
	       */
	      frames_counter = lw6sys_imax (frames_counter + frames_increment, nb_ticks - frames_increment);
	      data->nb_frames++;

	      delta_ticks = ticks - last_display_ticks;
	      if (delta_ticks < 0)
		{
		  last_display_ticks = ticks;
		  data->slow_fps = 0;
		}
	      if (delta_ticks > 0)
		{
		  data->instant_fps = ((float) LW6SYS_TICKS_PER_SEC) / delta_ticks;
		  data->average_fps =
		    (WEIGHT_INSTANT_FPS * delta_ticks * data->instant_fps +
		     WEIGHT_AVERAGE_FPS * ((float) LW6SYS_TICKS_PER_SEC) *
		     data->average_fps) / (WEIGHT_INSTANT_FPS * delta_ticks + WEIGHT_AVERAGE_FPS * ((float) LW6SYS_TICKS_PER_SEC));
		  last_display_ticks = ticks;
		  data->slow_fps = data->average_fps < (data->param.misc.target_fps / 2);
		}

	      if (data->param.game_state && data->param.game_struct)
		{
		  delta_ticks = ticks - last_rounds_ticks;
		  if (delta_ticks < 0)
		    {
		      last_rounds_ticks = ticks;
		      data->slow_mps = 0;
		    }
		  if (delta_ticks > LW6SYS_TICKS_PER_SEC / MPS_REFRESH_PER_SEC)
		    {
		      rounds = data->rounds;
		      delta_rounds = rounds - last_display_rounds;
		      if (delta_rounds > 0)
			{
			  data->mps = ((float) (delta_rounds * LW6SYS_TICKS_PER_SEC * data->param.game_struct->rules.moves_per_round)) / delta_ticks;
			  last_display_rounds = rounds;
			  data->slow_mps =
			    (data->rounds >
			     data->param.game_struct->rules.rounds_per_sec)
			    && (data->mps < ((data->param.game_struct->rules.rounds_per_sec * data->param.game_struct->rules.moves_per_round) / 2));
			}
		      last_rounds_ticks = ticks;
		    }
		}
	      else
		{
		  data->slow_mps = 0;
		}

	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("frame %d at ticks %" LW6SYS_PRINTF_LL "d"), data->nb_frames, (long long) ticks);
	      _loop (sys_context, data);
	      do_skip = 0;
	    }
	  else
	    {
	      /*
	       * It's important not to sleep now but only
	       * after the mutex has been released so we
	       * just set a flag for later usage.
	       */
	      do_skip = 1;
	    }
	  cpu_ticks = lw6sys_get_timestamp (sys_context) - ticks;
	  delay_cpu = 0;
	  if (!do_skip)
	    {
	      data->last_frame_rendering_time = cpu_ticks;
	      if (data->param.misc.gfx_cpu_usage > 0.0f && data->param.misc.gfx_cpu_usage < 1.0f)
		{
		  delay_cpu = ((float) cpu_ticks) * (1.0f - data->param.misc.gfx_cpu_usage);
		}
	    }

	  LW6SYS_MUTEX_UNLOCK (sys_context, data->render_mutex);

	  if (!do_skip)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("frame %d rendering took %d ms"), (int) data->nb_frames, (int) cpu_ticks);
	    }
	  if (do_skip || (delay_cpu > 0))
	    {
	      delay_real = lw6sys_imin (LW6SYS_TICKS_PER_SEC, lw6sys_imax (1, lw6sys_imax (do_skip ? delay_skip : 0, delay_cpu)));
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sleeping %d ms (delay_skip=%d, delay_cpu=%d)"), delay_real, delay_skip, delay_cpu);
	      lw6sys_delay (sys_context, delay_real);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display thread can't initialise graphics"));
    }
}

void
_lw6dsp_thread_join (lw6sys_context_t * sys_context, _lw6dsp_data_t * data)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display join"));

  /*
   * We unset the gfx_backend now, it's very important to do it here
   * and not in the main thread, for in some cases (Mac OS X) for instance
   * some libraries such as SDL require all graphics functions to be in
   * the same thread.
   */
  if (data->gfx_backend)
    {
      lw6gfx_quit (sys_context, data->gfx_backend);
      lw6gfx_destroy_backend (sys_context, data->gfx_backend);
      data->gfx_backend = NULL;
    }

#ifdef LW6_MAC_OS_X
  _lw6dsp_macosx_thread_quit (sys_context, data->macosx_vthread_handler);
#endif // LW6_MAC_OS_X
}
