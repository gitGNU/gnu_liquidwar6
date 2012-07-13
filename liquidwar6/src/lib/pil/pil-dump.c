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
_lw6pil_dump_pilot_to_command (_lw6pil_pilot_t * pilot)
{
  char *ret = NULL;

  // todo

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
lw6pil_dump_pilot_to_command (lw6pil_pilot_t * pilot)
{
  return _lw6pil_dump_pilot_to_command ((_lw6pil_pilot_t *) pilot);
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
