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

char *
_lw6pil_seed_command_generate (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq)
{
  char *ret = NULL;

  if (seq > _lw6pil_pilot_get_last_commit_seq (sys_context, pilot))
    {
      ret =
	lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) server_id, LW6PIL_COMMAND_TEXT_SEED);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("calling seed with inconsistent seqs, seq=%"
		       LW6SYS_PRINTF_LL
		       "d is too low because last_commit_seq=%" LW6SYS_PRINTF_LL "d"), (long long) seq,
		  (long long) _lw6pil_pilot_get_last_commit_seq (sys_context, pilot));
    }

  return ret;
}

/**
 * lw6pil_seed_command_generate
 *
 * @sys_context: global system context
 * @pilot: the pilot to transform as a SEED.
 * @server_id: ID of server issuing the command
 * @seq: seq at which the dump should be generated
 *
 * Creates the SEED command for a given pilot, that is, a command that
 * contains macro informations about the game state such as current seq.
 * It should be followed by a dump.
 *
 * Return value: newly allocated string
 */
char *
lw6pil_seed_command_generate (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq)
{
  return _lw6pil_seed_command_generate (sys_context, (_lw6pil_pilot_t *) pilot, server_id, seq);
}
