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

#include <string.h>

#include "ker.h"
#include "ker-internal.h"

#if LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER
#define _CALL_NEXT_DIR(A) _lw6ker_spread_next_dir((A))
#else
#define _CALL_NEXT_DIR(A) _next_dir((A))
#endif

static inline int32_t
_next_dir (int32_t dir)
{
  int ret;
  /*
   * Arbitrary hard-coded order for directions. (diags = 1 4 7 10)
   * 0 5 1
   * 3 8 4
   * 12
   * 6 11 7
   * 9 2 10
   * 13
   */
  static int corres[LW6KER_NB_DIRS] =
    { 5 /* 0->5 */ , 3 /* 1->3 */ , 10 /* 2->10 */ , 8 /* 3->8 */ ,
    12 /* 4->12 */ , 1 /* 5->1 */ , 11 /* 6->11 */ , 9 /* 7->9 */ ,
    4 /* 8->4 */ , 2 /* 9->2 */ ,
    13 /* 10->13 */ , 7 /* 11->7 */ , 6 /* 12->6 */ , 0	/* 13->0 */
  };

  ret = corres[dir];

  return ret;
}

int32_t
_lw6ker_spread_next_dir (int32_t dir)
{
  return _next_dir (dir);
}

/*
 * Highest potential = closest to the cursor
 */
void
_lw6ker_spread_update_gradient (_lw6ker_team_t * team, int skip_vertical)
{
  int32_t i;
  int32_t n;
  int32_t zone_id;
  int32_t new_potential;
  _lw6ker_zone_struct_t *zone_structs;
  _lw6ker_zone_state_t *zone_states;
  int32_t dir;

  n = team->map_struct->nb_zones;
  zone_structs = team->map_struct->zones;
  zone_states = team->gradient;

  team->last_spread_dir = _CALL_NEXT_DIR (team->last_spread_dir);
  if (skip_vertical)
    {
      while (team->last_spread_dir == LW6KER_DIR_UP
	     || team->last_spread_dir == LW6KER_DIR_DOWN)
	{
	  team->last_spread_dir = _CALL_NEXT_DIR (team->last_spread_dir);
	}
    }
  dir = team->last_spread_dir;

  switch (dir)
    {
    case LW6KER_DIR_ENE:
    case LW6KER_DIR_ESE:
    case LW6KER_DIR_SE:
    case LW6KER_DIR_SSE:
    case LW6KER_DIR_SSW:
    case LW6KER_DIR_SW:
      for (i = 0; i < n; ++i)
	{
	  if (((zone_id = zone_structs[i].link[dir]) >= 0)
	      && (zone_states[zone_id].potential
		  < (new_potential =
		     zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
      break;
    case LW6KER_DIR_DOWN:
      for (i = 0; i < n; ++i)
	{
	  if (((zone_id = zone_structs[i].link[dir]) >= 0)
	      && (zone_states[zone_id].potential
		  < (new_potential = zone_states[i].potential - 1)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
      break;
    case LW6KER_DIR_WSW:
    case LW6KER_DIR_WNW:
    case LW6KER_DIR_NW:
    case LW6KER_DIR_NNW:
    case LW6KER_DIR_NNE:
    case LW6KER_DIR_NE:
      for (i = n - 1; i >= 0; --i)
	{
	  if (((zone_id = zone_structs[i].link[dir]) >= 0)
	      && (zone_states[zone_id].potential
		  < (new_potential =
		     zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
      break;
    case LW6KER_DIR_UP:
      for (i = n - 1; i >= 0; --i)
	{
	  if (((zone_id = zone_structs[i].link[dir]) >= 0)
	      && (zone_states[zone_id].potential
		  < (new_potential = zone_states[i].potential - 1)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
      break;
    default:
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to spread gradient, incorrect dir=%d"), dir);
      break;
    }
}
