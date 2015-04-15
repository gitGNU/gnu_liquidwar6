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

#include "pil.h"
#include "pil-internal.h"

void
_lw6pil_compute_thread_func (lw6sys_context_t * sys_context, lw6pil_worker_t * worker)
{
  lw6pil_command_t *command = NULL;
  lw6sys_list_t *commands = NULL;
  u_int32_t team_mask_even = 0;
  u_int32_t team_mask_odd = 0;
  _lw6pil_spread_data_t spread_data;
  lw6sys_thread_handler_t *spread_thread;
  int64_t timestamp = 0LL;
  lw6pil_pilot_t *old_worker_dump_pilot_ptr = NULL;
  u_int32_t old_worker_dump_pilot_id = 0;
  int yield_limit = 0;
  int yield_period = 1;

  memset (&spread_data, 0, sizeof (_lw6pil_spread_data_t));

  LW6SYS_MUTEX_LOCK (sys_context, worker->global_mutex);
  yield_limit = lw6sys_imax (1, (worker->game_state->game_struct->rules.rounds_per_sec * _LW6PIL_YIELD_LIMIT_MSEC) / 1000);
  yield_period = lw6sys_imax (1, (worker->game_state->game_struct->rules.rounds_per_sec * _LW6PIL_YIELD_PERIOD_MSEC) / 1000);
  LW6SYS_MUTEX_UNLOCK (sys_context, worker->global_mutex);

  while (worker->run)
    {
      commands = NULL;
      command = NULL;
      if (worker->verified)
	{
	  timestamp = lw6sys_get_timestamp (sys_context);
	}

      commands = lw6sys_list_r_transfer_to (sys_context, worker->commands);

      if (commands)
	{
	  lw6sys_sort (sys_context, &commands, _lw6pil_command_sort_callback, NULL);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker global compute begin %d"), worker->current_round);
	  LW6SYS_MUTEX_LOCK (sys_context, worker->global_mutex);
	  while (worker->run && commands && (command = lw6sys_list_pop_front (sys_context, &commands)) != NULL)
	    {
	      command->round = lw6sys_imax (command->round, worker->current_round);
	      while (worker->run && worker->current_round < command->round)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker compute begin %d"), worker->current_round);
		  LW6SYS_MUTEX_LOCK (sys_context, worker->compute_mutex);
		  lw6ker_team_mask_best (sys_context, &team_mask_even, &team_mask_odd, worker->game_state);
		  worker->target_round = command->round;
		  if (worker->game_state->game_struct->rules.spread_thread)
		    {
		      // even compute
		      spread_data.game_state = worker->game_state;
		      spread_data.team_mask = team_mask_odd;
		      spread_thread =
			lw6sys_thread_create (sys_context, (lw6sys_thread_callback_func_t) _lw6pil_spread_thread_func, NULL, (void *) &spread_data);
		      lw6ker_game_state_do_move (sys_context, worker->game_state, team_mask_even);
		      if (spread_thread)
			{
			  lw6sys_thread_join (sys_context, spread_thread);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to spawn spread_thread"));
			  _lw6pil_spread_thread_func (sys_context, &spread_data);
			}

		      // odd compute
		      spread_data.game_state = worker->game_state;
		      spread_data.team_mask = team_mask_even;
		      spread_thread =
			lw6sys_thread_create (sys_context, (lw6sys_thread_callback_func_t) _lw6pil_spread_thread_func, NULL, (void *) &spread_data);
		      lw6ker_game_state_do_move (sys_context, worker->game_state, team_mask_odd);
		      if (spread_thread)
			{
			  lw6sys_thread_join (sys_context, spread_thread);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to spawn spread_thread"));
			  _lw6pil_spread_thread_func (sys_context, &spread_data);
			}

		      lw6ker_game_state_finish_round (sys_context, worker->game_state);
		    }
		  else
		    {
		      lw6ker_game_state_do_round (sys_context, worker->game_state);
		    }
		  worker->current_round++;
		  if (lw6ker_game_state_is_over (sys_context, worker->game_state))
		    {
		      worker->over = 1;
		    }
		  else
		    {
		      if (worker->current_round != lw6ker_game_state_get_rounds (sys_context, worker->game_state))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				      _x_
				      ("possible race condition, worker and game_state do not report the same round number (%d vs %d)"),
				      worker->current_round, lw6ker_game_state_get_rounds (sys_context, worker->game_state));
			}
		    }
		  worker->computed_rounds++;
		  LW6SYS_MUTEX_UNLOCK (sys_context, worker->compute_mutex);
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker compute end %d"), worker->current_round);

		  /*
		   * https://savannah.gnu.org/bugs/index.php?41113
		   * One needs to stop calculating like mad if one is way behind schedule
		   * else control thread never gets its hands on this. In real games
		   * this rarely happens if the computer is correctly calibrated
		   * but during the bench we're actually targetting an infinite
		   * (or, for that matter, it's pretty much the same, as great as possible)
		   * number of rounds, so with lots of fast cores, the unlock/lock is done
		   * too fast for the control thread to hope and get the lock.
		   * Going idle() does fix the problem, there's no garantee (after all,
		   * main thread could never be fast enough to catch up) but in practice
		   * it does work.
		   */
		  if ((worker->target_round - worker->current_round > yield_limit) && (!(worker->computed_rounds % yield_period)))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_
				  ("at round %d and targetting %d, yielding a timeslice every %d rounds"),
				  worker->current_round, worker->target_round, yield_period);
		      lw6sys_idle (sys_context);
		    }
		}
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker execute begin %d"), worker->current_round);
	      LW6SYS_MUTEX_LOCK (sys_context, worker->compute_mutex);
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker execute command %s"), command->text);
	      if (worker->verified)
		{
		  lw6pil_command_execute (sys_context, &(worker->dump), timestamp, worker->game_state, command);
		  if (lw6pil_dump_exists (sys_context, &(worker->dump)) &&
		      ((old_worker_dump_pilot_ptr != worker->dump.pilot) || (old_worker_dump_pilot_id != worker->dump.pilot->id)))
		    {
		      if (commands)
			{
			  lw6sys_list_map (sys_context, commands, _lw6pil_compute_pump_command_callback, worker->dump.pilot);
			}
		    }
		  /*
		   * It's important to keep a track of the previous dump,
		   * to avoid putting messages twice in it. In practice it should
		   * be rare. Note that we srt these old_worker values *after*
		   * pumping the data if needed, this way if there's already a
		   * dump from previous compute, data will be appended the right way.
		   */
		  old_worker_dump_pilot_ptr = worker->dump.pilot;
		  if (old_worker_dump_pilot_ptr)
		    {
		      old_worker_dump_pilot_id = old_worker_dump_pilot_ptr->id;
		    }
		}
	      else
		{
		  /*
		   * Not in verified mode, we ignore all that dump stuff, if there's a
		   * dump we ignore it, the cost of processing it is too expensive
		   * compared to the gain it would introduce, we'll treat it when
		   * it's verified, meanwhile, keep working on old map.
		   */
		  lw6pil_command_execute (sys_context, NULL, 0L, worker->game_state, command);
		}
	      LW6SYS_MUTEX_UNLOCK (sys_context, worker->compute_mutex);
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker execute end %d"), worker->current_round);
	      lw6pil_command_free (sys_context, command);
	    }
	  LW6SYS_MUTEX_UNLOCK (sys_context, worker->global_mutex);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker global compute end %d"), worker->current_round);
	  if (commands)
	    {
	      // commands could be NULL if stopped by worker->run==0
	      lw6sys_list_free (sys_context, commands);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker idle"));
	  lw6sys_idle (sys_context);
	}
    }
}

void
_lw6pil_compute_thread_join (lw6sys_context_t * sys_context, lw6pil_worker_t * worker)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("worker compute join"));
}

void
_lw6pil_compute_pump_command_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  _lw6pil_pilot_t *pilot = (_lw6pil_pilot_t *) func_data;
  lw6pil_command_t *command = (lw6pil_command_t *) data;

  if (pilot && command && command->text)
    {
      /*
       * Log this because this is probably very error prone,
       * recopy of pending messages is a tricky stuff, and does not
       * happen often, so we log it.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("copying command \"%s\" into other pilot (dump)"), command->text);
      _lw6pil_pilot_send_command (sys_context, pilot, command->text, 1);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad parameters for pump command callback"));
    }
}
