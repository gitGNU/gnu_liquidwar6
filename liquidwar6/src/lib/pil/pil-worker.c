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

int
_lw6pil_worker_init (lw6sys_context_t * sys_context, lw6pil_worker_t * worker, lw6ker_game_state_t * game_state, int verified, lw6sys_progress_t * progress)
{
  int ret = 0;

  memset (worker, 0, sizeof (lw6pil_worker_t));
  worker->run = 1;
  worker->verified = verified;
  worker->target_round = lw6ker_game_state_get_rounds (sys_context, game_state);
  worker->current_round = worker->target_round;
  worker->computed_rounds = 0;
  worker->game_state = lw6ker_game_state_dup (sys_context, game_state, progress);
  if (worker->game_state)
    {
      worker->commands = lw6sys_list_r_new (sys_context, (lw6sys_free_func_t) lw6pil_command_free);
      if (worker->commands)
	{
	  worker->compute_mutex = LW6SYS_MUTEX_CREATE (sys_context);
	  if (worker->compute_mutex)
	    {
	      worker->global_mutex = LW6SYS_MUTEX_CREATE (sys_context);
	      if (worker->global_mutex)
		{
		  worker->compute_thread =
		    lw6sys_thread_create (sys_context, (lw6sys_thread_callback_func_t)
					  _lw6pil_compute_thread_func, (lw6sys_thread_callback_func_t) _lw6pil_compute_thread_join, (void *) worker);
		  if (worker->compute_thread)
		    {
		      ret = 1;
		    }
		}
	    }
	}
    }

  if (!ret)
    {
      _lw6pil_worker_quit (sys_context, worker);
    }

  return ret;
}

int
_lw6pil_worker_quit (lw6sys_context_t * sys_context, lw6pil_worker_t * worker)
{
  int ret = 0;

  worker->run = 0;
  if (worker->compute_thread)
    {
      lw6sys_thread_join (sys_context, worker->compute_thread);
    }
  if (worker->compute_mutex)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, worker->compute_mutex);
    }
  if (worker->global_mutex)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, worker->global_mutex);
    }
  if (worker->commands)
    {
      lw6sys_list_r_free (sys_context, worker->commands);
    }
  if (worker->game_state)
    {
      lw6ker_game_state_free (sys_context, worker->game_state);
    }
  lw6pil_dump_clear (sys_context, &(worker->dump));
  memset (worker, 0, sizeof (lw6pil_worker_t));

  ret = 1;

  return ret;
}
