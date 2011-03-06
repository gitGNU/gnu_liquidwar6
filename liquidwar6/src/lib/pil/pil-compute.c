 /*
    Liquid War 6 is a unique multiplayer wargame.
    Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "pil.h"
#include "pil-internal.h"

void
_lw6pil_compute_thread_func (lw6pil_worker_t * worker)
{
  lw6pil_command_t *command = NULL;
  lw6sys_list_t *commands = NULL;
  u_int32_t team_mask_even = 0;
  u_int32_t team_mask_odd = 0;
  _lw6pil_spread_data_t spread_data;
  void *spread_thread;

  memset (&spread_data, 0, sizeof (_lw6pil_spread_data_t));
  while (worker->run)
    {
      commands = NULL;
      command = NULL;

      lw6sys_spinlock_lock (worker->commands_spinlock);
      if (worker->commands && !lw6sys_list_is_empty (worker->commands))
	{
	  commands = worker->commands;
	  worker->commands =
	    lw6sys_list_new ((lw6sys_free_func_t) lw6pil_command_free);
	  if (!worker->commands)
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("worker->commands is NULL"));
	    }
	}
      lw6sys_spinlock_unlock (worker->commands_spinlock);

      if (commands)
	{
	  lw6sys_sort (&commands, _lw6pil_command_sort_callback);
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("worker global compute begin %d"),
		      worker->current_round);
	  lw6sys_mutex_lock (worker->global_mutex);
	  while (worker->run && commands
		 && (command = lw6sys_list_pop_front (&commands)) != NULL)
	    {
	      command->round =
		lw6sys_max (command->round, worker->current_round);
	      while (worker->run && worker->current_round < command->round)
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_ ("worker compute begin %d"),
			      worker->current_round);
		  lw6sys_mutex_lock (worker->compute_mutex);
		  lw6ker_team_mask_best (&team_mask_even,
					 &team_mask_odd, worker->game_state);
		  worker->target_round = command->round;
		  if (worker->game_state->game_struct->rules.spread_thread)
		    {
		      // even compute
		      spread_data.game_state = worker->game_state;
		      spread_data.team_mask = team_mask_odd;
		      spread_thread =
			lw6sys_thread_create ((lw6sys_thread_callback_func_t)
					      _lw6pil_spread_thread_func,
					      NULL, (void *) &spread_data);
		      lw6ker_game_state_do_move (worker->game_state,
						 team_mask_even);
		      if (spread_thread)
			{
			  lw6sys_thread_join (spread_thread);
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_ ("unable to spawn spread_thread"));
			  _lw6pil_spread_thread_func (&spread_data);
			}

		      // odd compute
		      spread_data.game_state = worker->game_state;
		      spread_data.team_mask = team_mask_even;
		      spread_thread =
			lw6sys_thread_create ((lw6sys_thread_callback_func_t)
					      _lw6pil_spread_thread_func,
					      NULL, (void *) &spread_data);
		      lw6ker_game_state_do_move (worker->game_state,
						 team_mask_odd);
		      if (spread_thread)
			{
			  lw6sys_thread_join (spread_thread);
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_ ("unable to spawn spread_thread"));
			  _lw6pil_spread_thread_func (&spread_data);
			}

		      lw6ker_game_state_finish_round (worker->game_state);
		    }
		  else
		    {
		      lw6ker_game_state_do_round (worker->game_state);
		    }
		  worker->current_round++;
		  if (lw6ker_game_state_is_over (worker->game_state))
		    {
		      worker->over = 1;
		    }
		  else
		    {
		      if (worker->current_round !=
			  lw6ker_game_state_get_rounds (worker->game_state))
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("possible race condition, worker and game_state do not report the same round number (%d vs %d)"),
				      worker->current_round,
				      lw6ker_game_state_get_rounds
				      (worker->game_state));
			}
		    }
		  worker->computed_rounds++;
		  lw6sys_mutex_unlock (worker->compute_mutex);
		  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("worker compute end %d"),
			      worker->current_round);
		}
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("worker execute begin %d"),
			  worker->current_round);
	      lw6sys_mutex_lock (worker->compute_mutex);
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("worker execute command %s"),
			  command->text);
	      lw6pil_command_execute (worker->game_state, command);
	      lw6sys_mutex_unlock (worker->compute_mutex);
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("worker execute end %d"),
			  worker->current_round);
	      lw6pil_command_free (command);
	    }
	  lw6sys_mutex_unlock (worker->global_mutex);
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("worker global compute end %d"),
		      worker->current_round);
	  if (commands)
	    {
	      // commands could be NULL if stopped by worker->run==0
	      lw6sys_list_free (commands);
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("worker idle"));
	  lw6sys_idle ();
	}
    }
}

void
_lw6pil_compute_thread_join (lw6pil_worker_t * worker)
{
  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("worker compute join"));
}
