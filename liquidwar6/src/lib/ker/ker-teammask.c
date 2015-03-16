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

#include <string.h>

#include "ker.h"
#include "ker-internal.h"

#define TEAM_MASK_ALL 0x3F;

/**
 * lw6ker_team_mask_get
 *
 * @sys_context: global system context
 * @even: even team mask (out param)
 * @odd: odd team mask (out param)
 * @round: round concerned
 *
 * Returns a default team mask for a given round, even and odd will contain
 * ready to use masks (for spread and move functions for instance).
 *
 * Return value: none.
 */
void
lw6ker_team_mask_get (lw6sys_context_t * sys_context, u_int32_t * even, u_int32_t * odd, int32_t round)
{
  u_int32_t checksum;

  checksum = lw6sys_checksum_int32 (sys_context, round);

  (*even) =
    ((checksum & 0x01) ? 0x001 : 0x002) | ((checksum & 0x02) ? 0x004 : 0x008)
    | ((checksum & 0x08) ? 0x010 : 0x020) | ((checksum & 0x10) ? 0x040 : 0x080) | ((checksum & 0x20) ? 0x100 : 0x200);
  (*odd) = (*even) ^ LW6KER_TEAM_MASK_ALL;
}

/**
 * lw6ker_team_mask_best
 *
 * @sys_context: global system context
 * @even: even team mask (out param)
 * @odd: odd team mask (out param)
 * @game_state: the game_state concerned
 *
 * Returns an optimal team mask for a given round, even and odd will contain
 * ready to use masks (for spread and move functions for instance). The difference
 * with the default team mask is that this one will test for which teams are present
 * and try and manage to find an equilibrated set of odd/even teams.
 *
 * Return value: none.
 */
void
lw6ker_team_mask_best (lw6sys_context_t * sys_context, u_int32_t * even, u_int32_t * odd, lw6ker_game_state_t * game_state)
{
  int i = 0, j = 0, k = 0, l = 0;
  int delta = LW6MAP_MAX_NB_TEAMS;
  int seed = 0;

  seed = lw6ker_game_state_get_rounds (sys_context, game_state);
  while (delta > 1 && l < LW6MAP_MAX_NB_TEAMS)
    {
      lw6ker_team_mask_get (sys_context, even, odd, seed);
      j = k = 0;
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  if (lw6ker_game_state_team_exists (sys_context, game_state, i))
	    {
	      if (lw6ker_team_mask_is_concerned (sys_context, i, *even))
		{
		  k++;
		}
	      j++;
	    }
	}
      seed = lw6sys_checksum_int32 (sys_context, seed);
      delta = abs (j - k * 2);
      l++;
    }

  if (l >= LW6MAP_MAX_NB_TEAMS)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("team_max_best with l=%d, consider optimizing"), l);
    }
}

/**
 * lw6ker_team_mask_is_concerned
 *
 * @sys_context: global system context
 * @team_color: color index
 * @team_mask: team mask
 *
 * Tells wether a given team is concerned by a team mask.
 *
 * Return value: 1 if concerned, 0 if not.
 */
int
lw6ker_team_mask_is_concerned (lw6sys_context_t * sys_context, int team_color, u_int32_t team_mask)
{
  static int matrix[LW6MAP_MAX_NB_TEAMS] = { LW6KER_TEAM_MASK_RED,
    LW6KER_TEAM_MASK_GREEN,
    LW6KER_TEAM_MASK_BLUE,
    LW6KER_TEAM_MASK_YELLOW,
    LW6KER_TEAM_MASK_CYAN,
    LW6KER_TEAM_MASK_MAGENTA,
    LW6KER_TEAM_MASK_ORANGE,
    LW6KER_TEAM_MASK_LIGHTBLUE,
    LW6KER_TEAM_MASK_PURPLE,
    LW6KER_TEAM_MASK_PINK
  };
  int ret = 0;

  if (team_color >= 0 && team_color < LW6MAP_MAX_NB_TEAMS)
    {
      ret = matrix[team_color] & team_mask;
    }

  return ret;
}

/**
 * lw6ker_team_mask_color2mask
 *
 * @team_color: color index
 *
 * Gives the mask corresponding to a given color.
 *
 * Return value: bitwise mask.
 */
int
lw6ker_team_mask_color2mask (lw6sys_context_t * sys_context, int team_color)
{
  int ret = 0;

  switch (team_color)
    {
    case LW6MAP_TEAM_COLOR_RED:
      ret = LW6KER_TEAM_MASK_RED;
      break;
    case LW6MAP_TEAM_COLOR_GREEN:
      ret = LW6KER_TEAM_MASK_GREEN;
      break;
    case LW6MAP_TEAM_COLOR_BLUE:
      ret = LW6KER_TEAM_MASK_BLUE;
      break;
    case LW6MAP_TEAM_COLOR_YELLOW:
      ret = LW6KER_TEAM_MASK_YELLOW;
      break;
    case LW6MAP_TEAM_COLOR_CYAN:
      ret = LW6KER_TEAM_MASK_CYAN;
      break;
    case LW6MAP_TEAM_COLOR_MAGENTA:
      ret = LW6KER_TEAM_MASK_MAGENTA;
      break;
    case LW6MAP_TEAM_COLOR_ORANGE:
      ret = LW6KER_TEAM_MASK_ORANGE;
      break;
    case LW6MAP_TEAM_COLOR_LIGHTBLUE:
      ret = LW6KER_TEAM_MASK_LIGHTBLUE;
      break;
    case LW6MAP_TEAM_COLOR_PURPLE:
      ret = LW6KER_TEAM_MASK_PURPLE;
      break;
    case LW6MAP_TEAM_COLOR_PINK:
      ret = LW6KER_TEAM_MASK_PINK;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid team index %d"), team_color);
    }

  return ret;
}
