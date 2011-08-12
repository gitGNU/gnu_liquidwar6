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

void
_lw6ker_weapon_unset_by_weapon_id (_lw6ker_map_state_t * map_state,
				   int weapon_id)
{
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].weapon_id == weapon_id)
	{
	  map_state->teams[i].weapon_id = _LW6KER_WEAPON_NONE;
	  map_state->teams[i].weapon_first_round = 0;
	  map_state->teams[i].weapon_last_round = 0;
	}
    }
}

int
_lw6ker_weapon_find_team_by_weapon_id (_lw6ker_map_state_t * map_state,
				       int round, int weapon_id)
{
  int i;
  int ret = LW6MAP_TEAM_COLOR_INVALID;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].weapon_id == weapon_id
	  && map_state->teams[i].weapon_first_round <= round
	  && map_state->teams[i].weapon_last_round >= round)
	{
	  ret = i;
	}
    }

  return ret;
}

int
_lw6ker_weapon_get_latest_weapon (_lw6ker_map_state_t * map_state,
				  int round, int *team_color, int *weapon_id,
				  int *per1000_left)
{
  int i;
  int found = LW6MAP_TEAM_COLOR_INVALID;
  int ret = 0;
  int latest_round = 0;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].weapon_id >= LW6MAP_MIN_WEAPON_ID
	  && map_state->teams[i].weapon_id <= LW6MAP_MAX_WEAPON_ID
	  && map_state->teams[i].weapon_first_round <= round
	  && map_state->teams[i].weapon_last_round >= round)
	{
	  if (map_state->teams[i].weapon_first_round > latest_round)
	    {
	      latest_round = map_state->teams[i].weapon_first_round;
	      found = i;
	      ret = 1;
	    }
	}
    }

  if (team_color)
    {
      (*team_color) = found;
    }

  if (weapon_id)
    {
      if (found != LW6MAP_TEAM_COLOR_INVALID)
	{
	  (*weapon_id) = map_state->teams[found].weapon_id;
	}
      else
	{
	  (*weapon_id) = _LW6KER_WEAPON_NONE;
	}
    }

  if (per1000_left)
    {
      if (found != LW6MAP_TEAM_COLOR_INVALID)
	{
	  (*per1000_left) =
	    _lw6ker_team_get_weapon_per1000_left (&(map_state->teams[found]),
						  round);
	}
      else
	{
	  (*per1000_left) = 0;
	}
    }

  return ret;
}

int
_lw6ker_weapon_fire (_lw6ker_map_state_t * map_state,
		     lw6map_rules_t * rules, int round, int team_color,
		     int charge_percent)
{
  int weapon_id = -1;
  int ret = 0;

  switch (rules->team_profile_weapon_mode[team_color])
    {
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_NONE:
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("weapons disabled by config, doing nothing"));
      break;
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_BY_ID:
      weapon_id = rules->team_profile_weapon_id[team_color];
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("weapon %d for color %d (config)"),
		  weapon_id, team_color);
      break;
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_RANDOM:
      weapon_id = lw6sys_random (LW6MAP_MAX_WEAPON_ID + 1);
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("random weapon %d"), weapon_id);
      break;
    }

  if (weapon_id >= LW6MAP_MIN_WEAPON_ID && weapon_id <= LW6MAP_MAX_WEAPON_ID)
    {
      switch (weapon_id)
	{
	case _LW6KER_WEAPON_BEZERK:
	  ret =
	    _lw6ker_weapon_fire_bezerk (map_state, rules, round, team_color,
					charge_percent);
	  break;
	case _LW6KER_WEAPON_INVINCIBLE:
	  ret =
	    _lw6ker_weapon_fire_invincible (map_state, rules, round,
					    team_color, charge_percent);
	  break;
	case _LW6KER_WEAPON_ESCAPE:
	  ret =
	    _lw6ker_weapon_fire_escape (map_state, rules, round, team_color,
					charge_percent);
	  break;
	case _LW6KER_WEAPON_TURBO:
	  ret =
	    _lw6ker_weapon_fire_turbo (map_state, rules, round, team_color,
				       charge_percent);
	  break;
	case _LW6KER_WEAPON_TELEPORT:
	  ret =
	    _lw6ker_weapon_fire_teleport (map_state, rules, round, team_color,
					  charge_percent);
	  break;
	case _LW6KER_WEAPON_SCATTER:
	  ret =
	    _lw6ker_weapon_fire_scatter (map_state, rules, round, team_color,
					 charge_percent);
	  break;
	case _LW6KER_WEAPON_FIX:
	  ret = _lw6ker_weapon_fire_fix (map_state, rules, round, team_color,
					 charge_percent);
	  break;
	case _LW6KER_WEAPON_MIX:
	  ret = _lw6ker_weapon_fire_mix (map_state, rules, round, team_color,
					 charge_percent);
	  break;
	case _LW6KER_WEAPON_CONTROL:
	  ret =
	    _lw6ker_weapon_fire_control (map_state, rules, round, team_color,
					 charge_percent);
	  break;
	case _LW6KER_WEAPON_PERMUTATION:
	  ret =
	    _lw6ker_weapon_fire_permutation (map_state, rules, round,
					     team_color, charge_percent);
	  break;
	}
      if (ret)
	{
	  map_state->teams[team_color].weapon_id = weapon_id;
	  map_state->teams[team_color].weapon_first_round = round;
	  map_state->teams[team_color].weapon_last_round =
	    round + rules->rounds_per_sec * rules->weapon_duration;
	}
    }

  return ret;
}

int
_lw6ker_weapon_fire_bezerk (_lw6ker_map_state_t * map_state,
			    lw6map_rules_t * rules, int round, int team_color,
			    int charge_percent)
{
  int ret = 1;

  return ret;
}

int
_lw6ker_weapon_fire_invincible (_lw6ker_map_state_t * map_state,
				lw6map_rules_t * rules, int round,
				int team_color, int charge_percent)
{
  int ret = 1;

  return ret;
}

int
_lw6ker_weapon_fire_escape (_lw6ker_map_state_t * map_state,
			    lw6map_rules_t * rules, int round, int team_color,
			    int charge_percent)
{
  int ret = 1;

  return ret;
}

int
_lw6ker_weapon_fire_turbo (_lw6ker_map_state_t * map_state,
			   lw6map_rules_t * rules, int round, int team_color,
			   int charge_percent)
{
  int ret = 1;

  return ret;
}

int
_lw6ker_weapon_fire_teleport (_lw6ker_map_state_t * map_state,
			      lw6map_rules_t * rules, int round,
			      int team_color, int charge_percent)
{
  int ret = 1;

  return ret;
}

int
_lw6ker_weapon_fire_scatter (_lw6ker_map_state_t * map_state,
			     lw6map_rules_t * rules, int round,
			     int team_color, int charge_percent)
{
  int ret = 1;

  return ret;
}

int
_lw6ker_weapon_fire_fix (_lw6ker_map_state_t * map_state,
			 lw6map_rules_t * rules, int round, int team_color,
			 int charge_percent)
{
  int ret = 1;

  _lw6ker_weapon_unset_by_weapon_id (map_state, _LW6KER_WEAPON_FIX);

  return ret;
}

int
_lw6ker_weapon_fire_mix (_lw6ker_map_state_t * map_state,
			 lw6map_rules_t * rules, int round, int team_color,
			 int charge_percent)
{
  int ret = 1;

  return ret;
}

int
_lw6ker_weapon_fire_control (_lw6ker_map_state_t * map_state,
			     lw6map_rules_t * rules, int round,
			     int team_color, int charge_percent)
{
  int ret = 1;

  _lw6ker_weapon_unset_by_weapon_id (map_state, _LW6KER_WEAPON_CONTROL);

  return ret;
}

int
_lw6ker_weapon_fire_permutation (_lw6ker_map_state_t * map_state,
				 lw6map_rules_t * rules, int round,
				 int team_color, int charge_percent)
{
  int ret = 1;

  return ret;
}
