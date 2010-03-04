/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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
init (_lw6dsp_data_t * data)
{
  int ok = 0;
  lw6gui_fullscreen_modes_t fullscreen_modes;

  data->video_mode_requested = data->param.video_mode;

  data->gfx_backend =
    lw6gfx_create_backend (data->argc, data->argv, data->gfx_backend_name);
  if (data->gfx_backend)
    {
      lw6gfx_get_fullscreen_modes (data->gfx_backend,
				   &data->fullscreen_modes);
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _("dsp init requested %dx%d fullscreen=%d mode"),
		  data->video_mode_requested.width,
		  data->video_mode_requested.height,
		  data->video_mode_requested.fullscreen);
      if (lw6gfx_init
	  (data->gfx_backend, &(data->video_mode_requested),
	   data->resize_callback))
	{
	  /*
	   * We call it here too for it seems to work better once init
	   * has been called...
	   */
	  lw6gfx_get_fullscreen_modes (data->gfx_backend,
				       &data->fullscreen_modes);
	  if (lw6gfx_get_video_mode
	      (data->gfx_backend, &(data->video_mode_obtained)))
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("dsp init obtained %dx%d fullscreen=%d mode"),
			  data->video_mode_obtained.width,
			  data->video_mode_obtained.height,
			  data->video_mode_obtained.fullscreen);
	      ok = 1;
	    }
	  else
	    {
	      lw6gfx_quit (data->gfx_backend);
	    }
	}
      if (!ok)
	{
	  if (data->video_mode_obtained.width <= 0
	      || data->video_mode_obtained.height <= 0)
	    {
	      if (lw6gfx_get_fullscreen_modes
		  (data->gfx_backend, &fullscreen_modes))
		{
		  data->video_mode_requested.width =
		    fullscreen_modes.standard.width;
		  data->video_mode_requested.height =
		    fullscreen_modes.standard.height;
		}
	    }

	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("couldn't set up mode, trying fallback %dx%d fullscreen=%d, trying fallback %dx%d fullscreen=%d"),
		      data->video_mode_requested.width,
		      data->video_mode_requested.height,
		      data->video_mode_requested.fullscreen);
	  if (lw6gfx_init
	      (data->gfx_backend, &(data->video_mode_requested),
	       data->resize_callback))
	    {
	      if (lw6gfx_get_video_mode
		  (data->gfx_backend, &(data->video_mode_obtained)))
		{
		  ok = 1;
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _
			      ("dsp init obtained %dx%d fullscreen=%d mode"),
			      data->video_mode_obtained.width,
			      data->video_mode_obtained.height,
			      data->video_mode_obtained.fullscreen);
		}
	      else
		{
		  lw6gfx_quit (data->gfx_backend);
		}
	    }
	}
      if (!ok)
	{
	  lw6gfx_destroy_backend (data->gfx_backend);
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
poll (_lw6dsp_data_t * data)
{
  int ret = 1;
  char **history = NULL;
  int64_t timestamp;
  lw6ker_game_state_t *game_state = NULL;
  lw6pil_local_cursors_t *local_cursors = NULL;
  float progress = 0.0f;

  timestamp = lw6sys_get_timestamp ();
  data->input = lw6gfx_pump_events (data->gfx_backend);
  lw6gui_input_update_repeat (data->input,
			      &(data->param.misc.repeat_settings), timestamp);

  /*
   * We make a "local" copy of the quit flag for the other thread might not
   * be sure that input is here, so querying data->quit is safer
   */
  data->quit = data->quit || data->input->quit;

  if (data->param.pilot)
    {
      if (!(data->param.game_state))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("pilot defined but not game_state"));
	  ret = 0;
	}
      if (!lw6pil_pilot_can_sync (data->param.game_state, data->param.pilot))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("game_state state can't be synced from pilot"));
	  ret = 0;
	}
    }

  if (data->param.game_state)
    {
      if (!(data->param.game_struct))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("game_state defined but not game_struct"));
	  ret = 0;
	}
      if (data->param.game_state->game_struct != data->param.game_struct)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("game_state->game_struct=%p but game_struct=%p, they should be the same"),
		      data->param.game_state->game_struct,
		      data->param.game_struct);
	  ret = 0;
	}
    }

  if (data->param.game_struct)
    {
      if (!(data->param.level))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("game_struct defined but not level"));
	  ret = 0;
	}
      if (data->param.game_struct->level != data->param.level)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("game_struct->level=%p but level=%p, they should be the same"),
		      data->param.game_struct->level, data->param.level);
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
	  lw6pil_pilot_sync_from_draft (game_state, data->param.pilot,
					data->param.misc.dirty_read);
	  /*
	   * Here we don't sync completely correctly, the local_cursors
	   * might contain inconsistent data, but at least it won't
	   * be modified *while* we are reading it.
	   */
	  data->local_cursors = data->param.pilot->local_cursors;
	  break;
	case LW6PIL_DIRTY_READ_ALWAYS:
	  game_state = lw6pil_pilot_dirty_read (data->param.pilot);
	  local_cursors = &(data->param.pilot->local_cursors);
	  break;
	default:
	  lw6sys_log (LW6SYS_LOG_WARNING, _("bad value %d for dirty_read"),
		      data->param.misc.dirty_read);
	  break;
	}
    }

  if (game_state)
    {
      data->rounds = lw6ker_game_state_get_rounds (game_state);
    }

  if (!(data->param.look))
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("look is NULL"));
      ret = 0;
    }
  if (data->param.misc.progress)
    {
      progress = (*(data->param.misc.progress));
    }
  if (ret)
    {
      history = lw6sys_history_get (data->param.misc.log_timeout);
      if (history)
	{
	  if (lw6gfx_display (data->gfx_backend,
			      data->param.misc.mask,
			      data->param.look,
			      data->param.level,
			      data->param.game_struct,
			      game_state,
			      local_cursors,
			      data->param.menu,
			      progress,
			      data->average_fps, data->mps, history,
			      data->param.misc.capture,
			      data->param.misc.gfx_debug,
			      data->param.misc.debug_team_id,
			      data->param.misc.debug_layer_id))
	    {
	      // ok
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING, _("display thread problem"));
	      ret = 0;
	    }
	  lw6sys_history_free (history);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("couldn't get log history"));
	  ret = 0;
	}
    }

  return ret;
}

static int
loop (_lw6dsp_data_t * data)
{
  int ret = 0;

  if (!lw6gui_video_mode_is_same
      (&(data->video_mode_requested), &(data->param.video_mode)))
    {
      data->video_mode_requested = data->param.video_mode;
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _("dsp update request %dx%d fullscreen=%d mode"),
		  data->video_mode_requested.width,
		  data->video_mode_requested.height,
		  data->video_mode_requested.fullscreen);
      if (lw6gfx_set_video_mode (data->gfx_backend,
				 &(data->video_mode_requested)))
	{
	  if (lw6gfx_get_video_mode (data->gfx_backend,
				     &(data->video_mode_obtained)))
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _
			  ("dsp update obtained %dx%d fullscreen=%d mode"),
			  data->video_mode_obtained.width,
			  data->video_mode_obtained.height,
			  data->video_mode_obtained.fullscreen);
	    }
	}
    }

  ret = poll (data);

  return ret;
}

void
_lw6dsp_thread_func (_lw6dsp_data_t * data)
{
  int frames_increment = 1;	// mustn't be 0
  int frames_counter = 0;
  int64_t nb_ticks = 0;
  int64_t last_display_ticks = 0;
  int64_t last_rounds_ticks = 0;
  int64_t delta_ticks = 0;
  int64_t cpu_ticks = 0;
  int rounds;
  int delta_rounds = 0;
  int last_display_rounds = 0;
  int do_skip = 0;
  int delay_skip = 1;
  int delay_cpu = 0;
  int delay_real = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("display thread"));

  lw6sys_mutex_lock (data->render_mutex);

#ifdef LW6_MAC_OS_X
  data->macosx_handler = _lw6dsp_macosx_thread_init ();
#endif

  if (!data->gfx_backend)
    {
      init (data);
    }

  lw6sys_mutex_unlock (data->render_mutex);

  if (data->gfx_backend)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("display loop"));
      while (data->run)
	{
	  lw6sys_mutex_lock (data->render_mutex);

	  data->ticks = lw6sys_get_timestamp ();

	  if (data->param.misc.target_fps > 0)
	    {
	      frames_increment =
		LW6SYS_TICKS_PER_SEC / data->param.misc.target_fps;
	      delay_skip =
		LW6SYS_TICKS_PER_SEC / (2 * data->param.misc.target_fps);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _("can't handle target_fps of %d"),
			  data->param.misc.target_fps);
	    }
	  frames_increment = lw6sys_max (frames_increment, 1);
	  delay_skip = lw6sys_max (delay_skip, 1);

	  nb_ticks = data->ticks - data->start_ticks;

	  if (frames_counter < nb_ticks)
	    {
	      /*
	       * The following is more efficient than a plain += it will avoid going
	       * too fast after a slowdown
	       */
	      frames_counter =
		lw6sys_max (frames_counter + frames_increment,
			    nb_ticks - frames_increment);
	      data->nb_frames++;

	      delta_ticks = data->ticks - last_display_ticks;
	      if (delta_ticks > 0)
		{
		  data->instant_fps =
		    ((float) LW6SYS_TICKS_PER_SEC) / delta_ticks;
		  data->average_fps =
		    (WEIGHT_INSTANT_FPS * delta_ticks * data->instant_fps +
		     WEIGHT_AVERAGE_FPS * ((float) LW6SYS_TICKS_PER_SEC) *
		     data->average_fps) / (WEIGHT_INSTANT_FPS * delta_ticks +
					   WEIGHT_AVERAGE_FPS *
					   ((float) LW6SYS_TICKS_PER_SEC));
		  last_display_ticks = data->ticks;
		}
	      if (data->param.game_state && data->param.game_struct)
		{
		  delta_ticks = data->ticks - last_rounds_ticks;
		  if (delta_ticks >
		      LW6SYS_TICKS_PER_SEC / MPS_REFRESH_PER_SEC)
		    {
		      rounds = data->rounds;
		      delta_rounds = rounds - last_display_rounds;
		      if (delta_rounds > 0)
			{
			  data->mps =
			    ((float)
			     (delta_rounds * LW6SYS_TICKS_PER_SEC *
			      data->param.game_struct->
			      rules.moves_per_round)) / delta_ticks;
			  last_display_rounds = rounds;
			}
		      last_rounds_ticks = data->ticks;
		    }
		}

	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("frame %d at ticks %" LW6SYS_PRINTF_LL "d"),
			  data->nb_frames, data->ticks);
	      loop (data);
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
	  cpu_ticks = lw6sys_get_timestamp () - data->ticks;
	  delay_cpu = 0;
	  if (!do_skip)
	    {
	      data->last_frame_rendering_time = cpu_ticks;
	      if (data->param.misc.gfx_cpu_usage > 0.0f
		  && data->param.misc.gfx_cpu_usage < 1.0f)
		{
		  delay_cpu =
		    ((float) cpu_ticks) * (1.0f -
					   data->param.misc.gfx_cpu_usage);
		}
	    }

	  lw6sys_mutex_unlock (data->render_mutex);

	  if (!do_skip)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("frame %d rendering took %d ms"),
			  (int) data->nb_frames, (int) cpu_ticks);
	    }
	  if (do_skip || (delay_cpu > 0))
	    {
	      delay_real =
		lw6sys_min (LW6SYS_TICKS_PER_SEC,
			    lw6sys_max (1,
					lw6sys_max (do_skip ? delay_skip : 0,
						    delay_cpu)));
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("sleeping %d ms (delay_skip=%d, delay_cpu=%d)"),
			  delay_real, delay_skip, delay_cpu);
	      lw6sys_delay (delay_real);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("display thread can't initialise graphics"));
    }
}

void
_lw6dsp_thread_join (_lw6dsp_data_t * data)
{
  lw6sys_log (LW6SYS_LOG_DEBUG, _("display join"));

  /*
   * We unset the gfx_backend now, it's very important to do it here
   * and not in the main thread, for in some cases (Mac OS X) for instance
   * some libraries such as SDL require all graphics functions to be in
   * the same thread.
   */
  if (data->gfx_backend)
    {
      lw6gfx_quit (data->gfx_backend);
      lw6gfx_destroy_backend (data->gfx_backend);
      data->gfx_backend = NULL;
    }

#ifdef LW6_MAC_OS_X
  _lw6dsp_macosx_thread_quit (data->macosx_handler);
#endif
}
