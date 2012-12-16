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
#endif // HAVE_CONFIG_H

#include "pil.h"
#include "pil-internal.h"

/**
 * lw6pil_dump_zero
 *
 * @dump: object to initialize
 *
 * Fills a dump object with zero, regardless of what was there before.
 *
 * Return value: none.
 */
void
lw6pil_dump_zero (lw6pil_dump_t * dump)
{
  if (dump)
    {
      memset (dump, 0, sizeof (lw6pil_dump_t));
    }
}

/**
 * lw6pil_dump_clear
 *
 * @dump: object to clear
 *
 * Clears a dump object, that is, frees all existing object if they are here,
 * and sets pointers to NULL.
 *
 * Return value: none.
 */
void
lw6pil_dump_clear (lw6pil_dump_t * dump)
{
  if (dump->level)
    {
      lw6map_free (dump->level);
      (dump->level) = NULL;
    }

  if (dump->game_struct)
    {
      lw6ker_game_struct_free (dump->game_struct);
      (dump->game_struct) = NULL;
    }

  if (dump->game_state)
    {
      lw6ker_game_state_free (dump->game_state);
      (dump->game_state) = NULL;
    }

  if (dump->pilot)
    {
      lw6pil_pilot_free (dump->pilot);
      (dump->pilot) = NULL;
    }
}

/**
 * lw6pil_dump_exists
 *
 * @dump: object to test
 *
 * Tests wether there's actually a dump in the structure, or if it's empty.
 *
 * Return value: 1 if there's a dump, 0 if all fields set to NULL.
 */
int
lw6pil_dump_exists (const lw6pil_dump_t * dump)
{
  return (dump && dump->level && dump->game_struct && dump->game_state
	  && dump->pilot);
}

char *
_lw6pil_dump_command_generate (_lw6pil_pilot_t * pilot, u_int64_t server_id)
{
  char *ret = NULL;
  char *level_hexa = NULL;
  char *game_struct_hexa = NULL;
  char *game_state_hexa = NULL;
  int64_t seq = 0LL;

  /*
   * Before we can generate a dump, we want to make sure
   * the dump really corresponds to the right seq, so
   * we basically wait until reference is up-to-date.
   */
  while (_lw6pil_pilot_get_reference_current_seq (pilot) <
	 _lw6pil_pilot_get_last_commit_seq (pilot))
    {
      lw6sys_idle ();
    }

  level_hexa =
    lw6map_to_hexa (pilot->reference.game_state->game_struct->level);
  if (level_hexa)
    {
      game_struct_hexa =
	lw6ker_game_struct_to_hexa (pilot->reference.game_state->game_struct);
      if (game_struct_hexa)
	{
	  lw6sys_mutex_lock (pilot->reference.global_mutex);

	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("dump pilot to command round=%d"),
		      lw6ker_game_state_get_rounds (pilot->
						    reference.game_state));
	  game_state_hexa =
	    lw6ker_game_state_to_hexa (pilot->reference.game_state);

	  lw6sys_mutex_unlock (pilot->reference.global_mutex);
	}
    }

  if (level_hexa && game_struct_hexa && game_state_hexa)
    {
      seq = _lw6pil_pilot_get_last_commit_seq (pilot);
      ret =
	lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s %s %s %s", (long long) seq,
			    (long long) server_id, LW6PIL_COMMAND_TEXT_DUMP,
			    level_hexa, game_struct_hexa, game_state_hexa);
    }

  if (level_hexa)
    {
      LW6SYS_FREE (level_hexa);
    }
  if (game_struct_hexa)
    {
      LW6SYS_FREE (game_struct_hexa);
    }
  if (game_state_hexa)
    {
      LW6SYS_FREE (game_state_hexa);
    }

  return ret;
}

/**
 * lw6pil_dump_command_generate
 *
 * @pilot: the pilot to transform as a DUMP.
 * @server_id: ID of server issuing the command
 *
 * Creates the DUMP command for a given pilot, that is, a command that
 * describes the whole data and state.
 *
 * Return value: newly allocated string
 */
char *
lw6pil_dump_command_generate (lw6pil_pilot_t * pilot, u_int64_t server_id)
{
  return _lw6pil_dump_command_generate ((_lw6pil_pilot_t *) pilot, server_id);
}

/**
 * lw6pil_dump_command_execute
 *
 * Interprets a DUMP command. A new pilot will be returned, along with game state, game struct and
 * level. Old objects won't be deleted, but you could (should) get rid of them at they are useless now.
 * 
 * @dump: will contain the dump data, pilot and game state, struct, and level
 * @timestamp: current timestamp
 * @command: the command to execute
 * @progress: progress object to show the advancement of process
 *
 * Return value: newly allocated string
 */
int
lw6pil_dump_command_execute (lw6pil_dump_t * dump,
			     int64_t timestamp,
			     lw6pil_command_t * command,
			     lw6sys_progress_t * progress)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_ ("dump command to pilot, this can take some time..."));
  if (dump)
    {
      lw6pil_dump_clear (dump);

      (dump->level) = lw6map_from_hexa (command->args.dump.level_hexa);
      if (dump->level)
	{
	  (dump->game_struct) =
	    lw6ker_game_struct_from_hexa (command->args.dump.game_struct_hexa,
					  dump->level);
	  if (dump->game_struct)
	    {
	      (dump->game_state) =
		lw6ker_game_state_from_hexa (command->args.
					     dump.game_state_hexa,
					     dump->game_struct);
	      if (dump->game_state)
		{
		  (dump->pilot) =
		    lw6pil_pilot_new (dump->game_state, command->seq,
				      timestamp, progress);
		  if (dump->pilot)
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _x_ ("dump command to pilot OK"));
		      ret = 1;
		    }
		}
	    }
	}

      if (!ret)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("couldn't read dump command"));
	  lw6pil_dump_clear (dump);
	}
    }

  return ret;
}
