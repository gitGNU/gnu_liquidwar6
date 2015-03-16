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

#include "ker.h"
#include "ker-internal.h"

void
_lw6ker_game_state_checksum_log_set_interval (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, int checksum_log_interval)
{
  if (checksum_log_interval > 0)
    {
      game_state->checksum_log_interval = checksum_log_interval;
    }
}

/**
 * lw6ker_game_state_checksum_log_set_interval
 *
 * @sys_context: global system context
 * @game_state: the game_state to track
 * @checksum_log_interval: dump interval, if 0, feature is disabled
 *
 * Debugging function used to set automatically an interval at which engine
 * will log a checksum automatically. This is typically to track down where
 * and when there starts to be a difference between two game_states that have
 * evolved separately.
 *
 * Return value: none
 */
void
lw6ker_game_state_checksum_log_set_interval (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int checksum_log_interval)
{
  _lw6ker_game_state_checksum_log_set_interval (sys_context, (_lw6ker_game_state_t *) game_state, checksum_log_interval);
}

void
_lw6ker_game_state_checksum_log_if_needed (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  u_int32_t checksum = 0;
  char *repr = NULL;

  if (game_state->checksum_log_interval > 0)
    {
      if (!(_lw6ker_game_state_get_rounds (sys_context, game_state) % game_state->checksum_log_interval))
	{
	  checksum = _lw6ker_game_state_checksum (sys_context, game_state);
	  repr = _lw6ker_game_state_repr (sys_context, game_state);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("game_state \"%s\" has checksum %x"), repr, checksum);
	      LW6SYS_FREE (sys_context, repr);
	    }
	}
    }
}
