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

/**
 * lw6pil_dump_zero
 *
 * @sys_context: global system context
 * @dump: object to initialize
 *
 * Fills a dump object with zero, regardless of what was there before.
 *
 * Return value: none.
 */
void
lw6pil_dump_zero (lw6sys_context_t * sys_context, lw6pil_dump_t * dump)
{
  if (dump)
    {
      memset (dump, 0, sizeof (lw6pil_dump_t));
    }
}

/**
 * lw6pil_dump_clear
 *
 * @sys_context: global system context
 * @dump: object to clear
 *
 * Clears a dump object, that is, frees all existing object if they are here,
 * and sets pointers to NULL.
 *
 * Return value: none.
 */
void
lw6pil_dump_clear (lw6sys_context_t * sys_context, lw6pil_dump_t * dump)
{
  if (dump->level)
    {
      lw6map_free (sys_context, dump->level);
      (dump->level) = NULL;
    }

  if (dump->game_struct)
    {
      lw6ker_game_struct_free (sys_context, dump->game_struct);
      (dump->game_struct) = NULL;
    }

  if (dump->game_state)
    {
      lw6ker_game_state_free (sys_context, dump->game_state);
      (dump->game_state) = NULL;
    }

  if (dump->pilot)
    {
      lw6pil_pilot_free (sys_context, dump->pilot);
      (dump->pilot) = NULL;
    }
}

/**
 * lw6pil_dump_exists
 *
 * @sys_context: global system context
 * @dump: object to test
 *
 * Tests wether there's actually a dump in the structure, or if it's empty.
 *
 * Return value: 1 if there's a dump, 0 if all fields set to NULL.
 */
int
lw6pil_dump_exists (lw6sys_context_t * sys_context, const lw6pil_dump_t * dump)
{
  return (dump && dump->level && dump->game_struct && dump->game_state && dump->pilot);
}

char *
_lw6pil_dump_command_generate (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq)
{
  char *ret = NULL;
  char *level_hexa = NULL;
  char *game_struct_hexa = NULL;
  char *game_state_hexa = NULL;
  lw6ker_game_state_t *game_state = NULL;
  int round = 0;

  if (seq > _lw6pil_pilot_get_last_commit_seq (sys_context, pilot))
    {
      /*
       * Before we can generate a dump, we want to make sure
       * the dump really corresponds to the right seq, so
       * we basically wait until reference is up-to-date.
       */
      while (_lw6pil_pilot_get_reference_current_seq (sys_context, pilot) < _lw6pil_pilot_get_last_commit_seq (sys_context, pilot))
	{
	  lw6sys_idle (sys_context);
	}

      level_hexa = lw6map_to_hexa (sys_context, pilot->reference.game_state->game_struct->level);
      if (level_hexa)
	{
	  game_struct_hexa = lw6ker_game_struct_to_hexa (sys_context, pilot->reference.game_state->game_struct);
	  if (game_struct_hexa)
	    {
	      LW6SYS_MUTEX_LOCK (sys_context, pilot->reference.global_mutex);

	      /*
	       * Duplicating it, indeed, we need to possibly compute a
	       * few rounds ahead to make sure the round/seq of the
	       * dump itself and of the command do match. Chicken and
	       * egg problem here, some messages could maybe be lost,
	       * usually the delta is low (only one round) but still
	       * we need it else it breaks all the "commit" and
	       * atomicity logic.
	       */
	      game_state = lw6ker_game_state_dup (sys_context, pilot->reference.game_state, NULL);

	      LW6SYS_MUTEX_UNLOCK (sys_context, pilot->reference.global_mutex);

	      if (game_state)
		{
		  round = _lw6pil_pilot_seq2round (sys_context, pilot, seq);

		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_ ("dump pilot to command initial round=%d final round=%d"), lw6ker_game_state_get_rounds (sys_context, game_state), round);
		  while (lw6ker_game_state_get_rounds (sys_context, game_state) < round)
		    {
		      lw6ker_game_state_do_round (sys_context, game_state);
		    }
		  game_state_hexa = lw6ker_game_state_to_hexa (sys_context, game_state);
		  lw6ker_game_state_free (sys_context, game_state);
		}
	    }
	}

      if (level_hexa && game_struct_hexa && game_state_hexa)
	{
	  ret =
	    lw6sys_new_sprintf (sys_context,
				"%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
				"x %s %s %s %s", (long long) seq,
				(long long) server_id, LW6PIL_COMMAND_TEXT_DUMP, level_hexa, game_struct_hexa, game_state_hexa);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("calling dump with inconsistent seqs, seq=%"
		       LW6SYS_PRINTF_LL
		       "d is too low because last_commit_seq=%" LW6SYS_PRINTF_LL "d"), (long long) seq,
		  (long long) _lw6pil_pilot_get_last_commit_seq (sys_context, pilot));
    }

  if (level_hexa)
    {
      LW6SYS_FREE (sys_context, level_hexa);
    }
  if (game_struct_hexa)
    {
      LW6SYS_FREE (sys_context, game_struct_hexa);
    }
  if (game_state_hexa)
    {
      LW6SYS_FREE (sys_context, game_state_hexa);
    }

  return ret;
}

/**
 * lw6pil_dump_command_generate
 *
 * @sys_context: global system context
 * @pilot: the pilot to transform as a DUMP.
 * @server_id: ID of server issuing the command
 * @seq: seq at which the dump should be generated
 *
 * Creates the DUMP command for a given pilot, that is, a command that
 * describes the whole data and state. One must give a sequence number
 * since the dump would otherwise typically be *always* late by at
 * least one round (or seq). We give the seq as we would for any message
 * generated, to make it fit well within the standard message queue.
 *
 * Return value: newly allocated string
 */
char *
lw6pil_dump_command_generate (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq)
{
  return _lw6pil_dump_command_generate (sys_context, (_lw6pil_pilot_t *) pilot, server_id, seq);
}

/**
 * lw6pil_dump_command_execute
 *
 * Interprets a DUMP command. A new pilot will be returned, along with game state, game struct and
 * level. Old objects won't be deleted, but you could (should) get rid of them at they are useless now.
 *
 * @sys_context: global system context
 * @dump: will contain the dump data, pilot and game state, struct, and level
 * @timestamp: current timestamp
 * @command: the command to execute
 * @progress: progress object to show the advancement of process
 *
 * Return value: newly allocated string
 */
int
lw6pil_dump_command_execute (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp, lw6pil_command_t * command, lw6sys_progress_t * progress)
{
  int ret = 0;
  char *level_repr = NULL;
  char *game_struct_repr = NULL;
  char *game_state_repr = NULL;
  char *pilot_repr = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("dump command to pilot, this can take some time..."));
  if (dump)
    {
      lw6pil_dump_clear (sys_context, dump);

      (dump->level) = lw6map_from_hexa (sys_context, command->args.dump.level_hexa);
      if (dump->level)
	{
	  (dump->game_struct) = lw6ker_game_struct_from_hexa (sys_context, command->args.dump.game_struct_hexa, dump->level);
	  if (dump->game_struct)
	    {
	      (dump->game_state) = lw6ker_game_state_from_hexa (sys_context, command->args.dump.game_state_hexa, dump->game_struct);
	      if (dump->game_state)
		{
		  (dump->pilot) = lw6pil_pilot_new (sys_context, dump->game_state, command->seq, timestamp, progress);
		  if (dump->pilot)
		    {
		      level_repr = lw6map_repr (sys_context, dump->level);
		      game_struct_repr = lw6ker_game_struct_repr (sys_context, dump->game_struct);
		      game_state_repr = lw6ker_game_state_repr (sys_context, dump->game_state);
		      pilot_repr = lw6pil_pilot_repr (sys_context, dump->pilot);
		      if (level_repr && game_struct_repr && game_state_repr && pilot_repr)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				      _x_
				      ("dump command to pilot OK level=\"%s\" game_struct=\"%s\" game_state=\"%s\" pilot=\"%s\""),
				      level_repr, game_struct_repr, game_state_repr, pilot_repr);
			}
		      else
			{
			  // repr problem, we still display some "OK done" log
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("dump command to pilot OK, but enable to generate repr"));
			}
		      ret = 1;
		    }
		}
	    }
	}

      if (!ret)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't read dump command"));
	  lw6pil_dump_clear (sys_context, dump);
	}
    }

  if (level_repr)
    {
      LW6SYS_FREE (sys_context, level_repr);
    }

  if (game_struct_repr)
    {
      LW6SYS_FREE (sys_context, game_struct_repr);
    }

  if (game_state_repr)
    {
      LW6SYS_FREE (sys_context, game_state_repr);
    }

  if (pilot_repr)
    {
      LW6SYS_FREE (sys_context, pilot_repr);
    }

  return ret;
}
