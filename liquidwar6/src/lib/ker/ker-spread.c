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

#if LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER
#define _CALL_NEXT_DIR(A) _lw6ker_spread_next_dir((A))
#define _CALL_DO_ALL_DIRS_INCR_XY(A,B,C) _lw6ker_spread_do_all_dirs_incr_xy((A),(B),(C))
#define _CALL_DO_ALL_DIRS_DECR_XY(A,B,C) _lw6ker_spread_do_all_dirs_decr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_A_INCR_XY(A,B,C) _lw6ker_spread_do_half_dirs_a_incr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_B_INCR_XY(A,B,C) _lw6ker_spread_do_half_dirs_b_incr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_A_DECR_XY(A,B,C) _lw6ker_spread_do_half_dirs_a_decr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_B_DECR_XY(A,B,C) _lw6ker_spread_do_half_dirs_b_decr_xy((A),(B),(C))
#define _CALL_DO_ONE_DIR_INCR_XY(A,B,C,D) _lw6ker_spread_do_one_dir_incr_xy((A),(B),(C),(D))
#define _CALL_DO_ONE_DIR_INCR_Z(A,B,C,D) _lw6ker_spread_do_one_dir_incr_z((A),(B),(C),(D))
#define _CALL_DO_ONE_DIR_DECR_XY(A,B,C,D) _lw6ker_spread_do_one_dir_decr_xy((A),(B),(C),(D))
#define _CALL_DO_ONE_DIR_DECR_Z(A,B,C,D) _lw6ker_spread_do_one_dir_decr_z((A),(B),(C),(D))
#else // LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER
#define _CALL_NEXT_DIR(A) _next_dir((A))
#define _CALL_DO_ALL_DIRS_INCR_XY(A,B,C) _do_all_dirs_incr_xy((A),(B),(C))
#define _CALL_DO_ALL_DIRS_DECR_XY(A,B,C) _do_all_dirs_decr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_A_INCR_XY(A,B,C) _do_half_dirs_a_incr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_B_INCR_XY(A,B,C) _do_half_dirs_b_incr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_A_DECR_XY(A,B,C) _do_half_dirs_a_decr_xy((A),(B),(C))
#define _CALL_DO_HALF_DIRS_B_DECR_XY(A,B,C) _do_half_dirs_b_decr_xy((A),(B),(C))
#define _CALL_DO_ONE_DIR_INCR_XY(A,B,C,D) _do_one_dir_incr_xy((A),(B),(C),(D))
#define _CALL_DO_ONE_DIR_INCR_Z(A,B,C,D) _do_one_dir_incr_z((A),(B),(C),(D))
#define _CALL_DO_ONE_DIR_DECR_XY(A,B,C,D) _do_one_dir_decr_xy((A),(B),(C),(D))
#define _CALL_DO_ONE_DIR_DECR_Z(A,B,C,D) _do_one_dir_decr_z((A),(B),(C),(D))
#endif // LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER

#define _ALL_DIRS_NB 6
#define _HALF_DIRS_A_NB 3
#define _HALF_DIRS_B_NB 3

static inline int32_t
_next_dir (int32_t dir)
{
  int ret;
  /*
   * Arbitrary hard-coded order for directions. (diags = 1 4 7 10)
   * For incr spread, use 0 to 5 (ENE to SW) and 12 (DOWN)
   * For decr spread, use 6 to 11 (WSW to NE) and 13 (UP)
   * 0 6
   * 3 9
   * 1 7
   * 12
   * 10 4
   * 8 2
   * 11 5
   * 13
   */
  static int corres[LW6KER_NB_DIRS] = { 6 /* 0->6 */ , 7 /* 1->7 */ , 11 /* 2->11 */ , 9 /* 3->9 */ ,
    8 /* 4->8 */ , 13 /* 5->13 */ , 3 /* 6->3 */ , 12 /* 7->12 */ ,
    2 /* 8->2 */ , 1 /* 9->1 */ ,
    4 /* 10->4 */ , 5 /* 11->5 */ , 10 /* 12->10 */ , 0	/* 13->0 */
  };

  ret = corres[dir];

  return ret;
}

int32_t
_lw6ker_spread_next_dir (int32_t dir)
{
  return _next_dir (dir);
}

static inline void
_do_all_dirs_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  int32_t i, j;
  int32_t zone_id;
  int32_t new_potential;
  static int dirs[_ALL_DIRS_NB] = { LW6KER_DIR_ENE, LW6KER_DIR_ESE, LW6KER_DIR_SE, LW6KER_DIR_SSE,
    LW6KER_DIR_SSW, LW6KER_DIR_SW
  };

  for (i = 0; i < nb_zones; ++i)
    {
      for (j = 0; j < _ALL_DIRS_NB; ++j)
	{
	  if (((zone_id = zone_structs[i].link[dirs[j]]) >= 0)
	      && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
    }
}

void
_lw6ker_spread_do_all_dirs_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  return _do_all_dirs_incr_xy (zone_structs, zone_states, nb_zones);
}

static inline void
_do_all_dirs_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  int32_t i, j;
  int32_t zone_id;
  int32_t new_potential;
  static int dirs[_ALL_DIRS_NB] = { LW6KER_DIR_WSW, LW6KER_DIR_WNW, LW6KER_DIR_NW, LW6KER_DIR_NNW,
    LW6KER_DIR_NNE, LW6KER_DIR_NE
  };

  for (i = nb_zones - 1; i >= 0; --i)
    {
      for (j = 0; j < _ALL_DIRS_NB; ++j)
	{
	  if (((zone_id = zone_structs[i].link[dirs[j]]) >= 0)
	      && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
    }
}

void
_lw6ker_spread_do_all_dirs_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  return _do_all_dirs_decr_xy (zone_structs, zone_states, nb_zones);
}

static inline void
_do_half_dirs_a_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  int32_t i, j;
  int32_t zone_id;
  int32_t new_potential;
  static int dirs[_HALF_DIRS_A_NB] = { LW6KER_DIR_ENE, LW6KER_DIR_ESE, LW6KER_DIR_SE
  };

  for (i = 0; i < nb_zones; ++i)
    {
      for (j = 0; j < _HALF_DIRS_A_NB; ++j)
	{
	  if (((zone_id = zone_structs[i].link[dirs[j]]) >= 0)
	      && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
    }
}

void
_lw6ker_spread_do_half_dirs_a_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  return _do_half_dirs_a_incr_xy (zone_structs, zone_states, nb_zones);
}

static inline void
_do_half_dirs_b_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  int32_t i, j;
  int32_t zone_id;
  int32_t new_potential;
  static int dirs[_HALF_DIRS_B_NB] = { LW6KER_DIR_SSE,
    LW6KER_DIR_SSW, LW6KER_DIR_SW
  };

  for (i = 0; i < nb_zones; ++i)
    {
      for (j = 0; j < _HALF_DIRS_B_NB; ++j)
	{
	  if (((zone_id = zone_structs[i].link[dirs[j]]) >= 0)
	      && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
    }
}

void
_lw6ker_spread_do_half_dirs_b_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  return _do_half_dirs_b_incr_xy (zone_structs, zone_states, nb_zones);
}

static inline void
_do_half_dirs_a_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  int32_t i, j;
  int32_t zone_id;
  int32_t new_potential;
  static int dirs[_HALF_DIRS_A_NB] = { LW6KER_DIR_WSW, LW6KER_DIR_WNW, LW6KER_DIR_NW
  };

  for (i = nb_zones - 1; i >= 0; --i)
    {
      for (j = 0; j < _HALF_DIRS_A_NB; ++j)
	{
	  if (((zone_id = zone_structs[i].link[dirs[j]]) >= 0)
	      && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
    }
}

void
_lw6ker_spread_do_half_dirs_a_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  return _do_half_dirs_a_decr_xy (zone_structs, zone_states, nb_zones);
}

static inline void
_do_half_dirs_b_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  int32_t i, j;
  int32_t zone_id;
  int32_t new_potential;
  static int dirs[_HALF_DIRS_B_NB] = { LW6KER_DIR_NNW,
    LW6KER_DIR_NNE, LW6KER_DIR_NE
  };

  for (i = nb_zones - 1; i >= 0; --i)
    {
      for (j = 0; j < _HALF_DIRS_B_NB; ++j)
	{
	  if (((zone_id = zone_structs[i].link[dirs[j]]) >= 0)
	      && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	    {
	      zone_states[zone_id].potential = new_potential;
	      zone_states[zone_id].direction_to_cursor = -1;
	    }
	}
    }
}

void
_lw6ker_spread_do_half_dirs_b_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones)
{
  return _do_half_dirs_b_decr_xy (zone_structs, zone_states, nb_zones);
}

static inline void
_do_one_dir_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  int32_t i;
  int32_t zone_id;
  int32_t new_potential;

  for (i = 0; i < nb_zones; ++i)
    {
      if (((zone_id = zone_structs[i].link[dir]) >= 0) && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	{
	  zone_states[zone_id].potential = new_potential;
	  zone_states[zone_id].direction_to_cursor = -1;
	}
    }
}

void
_lw6ker_spread_do_one_dir_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  return _do_one_dir_incr_xy (zone_structs, zone_states, nb_zones, dir);
}

static inline void
_do_one_dir_incr_z (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  int32_t i;
  int32_t zone_id;
  int32_t new_potential;

  for (i = 0; i < nb_zones; ++i)
    {
      if (((zone_id = zone_structs[i].link[dir]) >= 0) && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - 1)))
	{
	  zone_states[zone_id].potential = new_potential;
	  zone_states[zone_id].direction_to_cursor = -1;
	}
    }
}

void
_lw6ker_spread_do_one_dir_incr_z (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  return _do_one_dir_incr_z (zone_structs, zone_states, nb_zones, dir);
}

static inline void
_do_one_dir_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  int32_t i;
  int32_t zone_id;
  int32_t new_potential;

  for (i = nb_zones - 1; i >= 0; --i)
    {
      if (((zone_id = zone_structs[i].link[dir]) >= 0) && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - zone_structs[i].size)))
	{
	  zone_states[zone_id].potential = new_potential;
	  zone_states[zone_id].direction_to_cursor = -1;
	}
    }
}

void
_lw6ker_spread_do_one_dir_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  return _do_one_dir_decr_xy (zone_structs, zone_states, nb_zones, dir);
}

static inline void
_do_one_dir_decr_z (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  int32_t i;
  int32_t zone_id;
  int32_t new_potential;

  for (i = nb_zones - 1; i >= 0; --i)
    {
      if (((zone_id = zone_structs[i].link[dir]) >= 0) && (zone_states[zone_id].potential < (new_potential = zone_states[i].potential - 1)))
	{
	  zone_states[zone_id].potential = new_potential;
	  zone_states[zone_id].direction_to_cursor = -1;
	}
    }
}

void
_lw6ker_spread_do_one_dir_decr_z (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir)
{
  return _do_one_dir_decr_z (zone_structs, zone_states, nb_zones, dir);
}

/*
 * Highest potential = closest to the cursor
 */
void
_lw6ker_spread_update_gradient (lw6sys_context_t * sys_context, _lw6ker_team_t * team, int skip_vertical, int spread_mode)
{
  int32_t nb_zones;
  _lw6ker_zone_struct_t *zone_structs;
  _lw6ker_zone_state_t *zone_states;
  int32_t dir;

  nb_zones = team->map_struct->nb_zones;
  zone_structs = team->map_struct->zones;
  zone_states = team->gradient;

  team->last_spread_dir = _CALL_NEXT_DIR (team->last_spread_dir);
  if (skip_vertical)
    {
      while (team->last_spread_dir == LW6KER_DIR_UP || team->last_spread_dir == LW6KER_DIR_DOWN)
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
      switch (spread_mode)
	{
	case LW6MAP_RULES_SPREAD_MODE_ONE:
	  _CALL_DO_ONE_DIR_INCR_XY (zone_structs, zone_states, nb_zones, dir);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_HALF:
	  _CALL_DO_HALF_DIRS_A_INCR_XY (zone_structs, zone_states, nb_zones);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_ALL:
	  _CALL_DO_ALL_DIRS_INCR_XY (zone_structs, zone_states, nb_zones);
	  break;
	default:
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad spread_mode=%d"), spread_mode);
	  break;
	}
      break;
    case LW6KER_DIR_SSE:
    case LW6KER_DIR_SSW:
    case LW6KER_DIR_SW:
      switch (spread_mode)
	{
	case LW6MAP_RULES_SPREAD_MODE_ONE:
	  _CALL_DO_ONE_DIR_INCR_XY (zone_structs, zone_states, nb_zones, dir);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_HALF:
	  _CALL_DO_HALF_DIRS_B_INCR_XY (zone_structs, zone_states, nb_zones);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_ALL:
	  _CALL_DO_ALL_DIRS_INCR_XY (zone_structs, zone_states, nb_zones);
	  break;
	default:
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad spread_mode=%d"), spread_mode);
	  break;
	}
      break;
    case LW6KER_DIR_DOWN:
      _CALL_DO_ONE_DIR_INCR_Z (zone_structs, zone_states, nb_zones, dir);
      break;
    case LW6KER_DIR_WSW:
    case LW6KER_DIR_WNW:
    case LW6KER_DIR_NW:
      switch (spread_mode)
	{
	case LW6MAP_RULES_SPREAD_MODE_ONE:
	  _CALL_DO_ONE_DIR_DECR_XY (zone_structs, zone_states, nb_zones, dir);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_HALF:
	  _CALL_DO_HALF_DIRS_A_DECR_XY (zone_structs, zone_states, nb_zones);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_ALL:
	  _CALL_DO_ALL_DIRS_DECR_XY (zone_structs, zone_states, nb_zones);
	  break;
	default:
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad spread_mode=%d"), spread_mode);
	  break;
	}
      break;
    case LW6KER_DIR_NNW:
    case LW6KER_DIR_NNE:
    case LW6KER_DIR_NE:
      switch (spread_mode)
	{
	case LW6MAP_RULES_SPREAD_MODE_ONE:
	  _CALL_DO_ONE_DIR_DECR_XY (zone_structs, zone_states, nb_zones, dir);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_HALF:
	  _CALL_DO_HALF_DIRS_B_DECR_XY (zone_structs, zone_states, nb_zones);
	  break;
	case LW6MAP_RULES_SPREAD_MODE_ALL:
	  _CALL_DO_ALL_DIRS_DECR_XY (zone_structs, zone_states, nb_zones);
	  break;
	default:
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad spread_mode=%d"), spread_mode);
	  break;
	}
      break;
    case LW6KER_DIR_UP:
      _CALL_DO_ONE_DIR_DECR_Z (zone_structs, zone_states, nb_zones, dir);
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to spread gradient, incorrect dir=%d"), dir);
      break;
    }
}
