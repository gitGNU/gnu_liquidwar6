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

#include "pil.h"
#include "pil-internal.h"

char *
_lw6pil_dump_pilot_to_command (_lw6pil_pilot_t * pilot, int64_t timestamp,
			       u_int64_t server_id)
{
  char *ret = NULL;
  char *level_hexa = NULL;
  char *game_struct_hexa = NULL;
  char *game_state_hexa = NULL;
  int64_t seq = 0LL;

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
	  game_state_hexa = ret =
	    lw6ker_game_state_to_hexa (pilot->reference.game_state);

	  lw6sys_mutex_unlock (pilot->reference.global_mutex);
	}
    }

  if (level_hexa && game_struct_hexa && game_state_hexa)
    {
      seq = _lw6pil_pilot_get_next_seq (pilot, timestamp);
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
 * lw6pil_dump_pilot_to_command
 *
 * @pilot: the pilot to transform as a DUMP.
 *
 * Creates the DUMP command for a given pilot, that is, a command that
 * describes the whole data and state.
 *
 * Return value: newly allocated string
 */
char *
lw6pil_dump_pilot_to_command (lw6pil_pilot_t * pilot, int64_t timestamp,
			      u_int64_t server_id)
{
  return _lw6pil_dump_pilot_to_command ((_lw6pil_pilot_t *) pilot, timestamp,
					server_id);
}

int
_lw6pil_dump_command_to_pilot (_lw6pil_pilot_t * pilot,
			       _lw6pil_pilot_t ** new_pilot,
			       lw6ker_game_state_t ** new_game_state,
			       lw6ker_game_struct_t ** new_game_struct,
			       lw6map_level_t ** new_level)
{
  int ret = 0;

  // todo

  return ret;
}

/**
 * lw6pil_dump_command_to_pilot
 *
 * Interprets a DUMP command. A new pilot will be returned, along with game state, game struct and
 * level. Old objects won't be deleted, but you could (should) get rid of them at they are useless now.
 * 
 * describes the whole data and state.
 *
 * Return value: newly allocated string
 */
int
lw6pil_dump_command_to_pilot (lw6pil_pilot_t * pilot,
			      lw6pil_pilot_t ** new_pilot,
			      lw6ker_game_state_t ** new_game_state,
			      lw6ker_game_struct_t ** new_game_struct,
			      lw6map_level_t ** new_level)
{
  return _lw6pil_dump_command_to_pilot ((_lw6pil_pilot_t *) pilot,
					(_lw6pil_pilot_t **) new_pilot,
					new_game_state, new_game_struct,
					new_level);
}
