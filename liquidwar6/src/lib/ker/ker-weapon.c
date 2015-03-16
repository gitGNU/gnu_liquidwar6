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

#define _INSTANT_DURATION_DIV1 10
#define _INSTANT_DURATION_DIV2 2

void
_lw6ker_weapon_unset_by_weapon_id (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int weapon_id)
{
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].weapon_id == weapon_id)
	{
	  map_state->teams[i].weapon_id = LW6MAP_WEAPON_NONE;
	  map_state->teams[i].weapon_first_round = 0;
	  map_state->teams[i].weapon_last_round = 0;
	}
    }
}

void
_lw6ker_weapon_unset_all (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state)
{
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      map_state->teams[i].weapon_id = LW6MAP_WEAPON_NONE;
      map_state->teams[i].weapon_first_round = 0;
      map_state->teams[i].weapon_last_round = 0;
    }
}

int
_lw6ker_weapon_find_team_by_weapon_id (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state, int round, int weapon_id)
{
  int i;
  int ret = LW6MAP_TEAM_COLOR_INVALID;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].weapon_id == weapon_id && map_state->teams[i].weapon_first_round <= round && map_state->teams[i].weapon_last_round >= round)
	{
	  ret = i;
	}
    }

  return ret;
}

int
_lw6ker_weapon_get_latest_weapon (lw6sys_context_t * sys_context,
				  const _lw6ker_map_state_t * map_state, int round, int *team_color, int *weapon_id, int *per1000_left)
{
  int i;
  int found = LW6MAP_TEAM_COLOR_INVALID;
  int ret = 0;
  int latest_round = 0;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].active &&
	  map_state->teams[i].weapon_id >= LW6MAP_MIN_WEAPON_ID
	  && map_state->teams[i].weapon_id <= LW6MAP_MAX_WEAPON_ID
	  && map_state->teams[i].weapon_first_round <= round && map_state->teams[i].weapon_last_round >= round)
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
	  (*weapon_id) = LW6MAP_WEAPON_NONE;
	}
    }

  if (per1000_left)
    {
      if (found != LW6MAP_TEAM_COLOR_INVALID)
	{
	  (*per1000_left) = _lw6ker_team_get_weapon_per1000_left (sys_context, &(map_state->teams[found]), round);
	}
      else
	{
	  (*per1000_left) = 0;
	}
    }

  return ret;
}

static int
_do_fire (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state,
	  const lw6map_rules_t * rules, int round, int team_color, int charge_percent, int weapon_id)
{
  int ret = 0;

  if (lw6map_weapon_is_valid (weapon_id) && lw6map_exp_is_weapon_allowed (sys_context, rules, weapon_id))
    {
      switch (weapon_id)
	{
	case LW6MAP_WEAPON_BERZERK:
	  ret = _lw6ker_weapon_fire_berzerk (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_INVINCIBLE:
	  ret = _lw6ker_weapon_fire_invincible (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_ESCAPE:
	  ret = _lw6ker_weapon_fire_escape (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_TURBO:
	  ret = _lw6ker_weapon_fire_turbo (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_TELEPORT:
	  ret = _lw6ker_weapon_fire_teleport (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_SCATTER:
	  ret = _lw6ker_weapon_fire_scatter (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_FIX:
	  ret = _lw6ker_weapon_fire_fix (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_MIX:
	  ret = _lw6ker_weapon_fire_mix (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_CONTROL:
	  ret = _lw6ker_weapon_fire_control (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_PERMUTATION:
	  ret = _lw6ker_weapon_fire_permutation (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_STEAL:
	  ret = _lw6ker_weapon_fire_steal (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_ATOMIC:
	  ret = _lw6ker_weapon_fire_atomic (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_REVERSE:
	  ret = _lw6ker_weapon_fire_reverse (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_CRAZY:
	  ret = _lw6ker_weapon_fire_crazy (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_REWIND:
	  ret = _lw6ker_weapon_fire_rewind (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_ATTRACT:
	  ret = _lw6ker_weapon_fire_attract (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_SHRINK:
	  ret = _lw6ker_weapon_fire_shrink (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_KAMIKAZE:
	  ret = _lw6ker_weapon_fire_kamikaze (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_DISAPPEAR:
	  ret = _lw6ker_weapon_fire_disappear (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	case LW6MAP_WEAPON_PLAGUE:
	  ret = _lw6ker_weapon_fire_plague (sys_context, map_state, rules, round, team_color, charge_percent);
	  break;
	}
    }

  return ret;
}

static void
_register_weapon_duration (lw6sys_context_t * sys_context,
			   _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent, int weapon_id)
{
  map_state->teams[team_color].weapon_id = weapon_id;
  map_state->teams[team_color].weapon_first_round = round;
  map_state->teams[team_color].weapon_last_round = round + lw6ker_percent (rules->rounds_per_sec * rules->weapon_duration, charge_percent);
}

static void
_register_weapon_instant (lw6sys_context_t * sys_context,
			  _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent, int weapon_id)
{
  map_state->teams[team_color].weapon_id = weapon_id;
  map_state->teams[team_color].weapon_first_round = round;
  map_state->teams[team_color].weapon_last_round =
    round + lw6sys_imin ((rules->rounds_per_sec * rules->weapon_duration) / _INSTANT_DURATION_DIV1, rules->rounds_per_sec / _INSTANT_DURATION_DIV2);
}

int
_lw6ker_weapon_fire (lw6sys_context_t * sys_context,
		     _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int weapon_id = -1;
  int ret = 0;

  switch (rules->team_profile_weapon_mode[team_color])
    {
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_NONE:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("weapons disabled by config, doing nothing"));
      break;
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_BY_ID:
      weapon_id = rules->team_profile_weapon_id[team_color];
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("weapon %d for color %d (config)"), weapon_id, team_color);
      break;
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_RANDOM:
      weapon_id = lw6sys_random (sys_context, LW6MAP_MAX_WEAPON_ID + 1);
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("random weapon %d"), weapon_id);
      break;
    }

  ret = _do_fire (sys_context, map_state, rules, round, team_color, charge_percent, weapon_id);

  return ret;
}

int
_lw6ker_weapon_fire2 (lw6sys_context_t * sys_context,
		      _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int weapon_id = -1;
  int ret = 0;

  switch (rules->team_profile_weapon_mode[team_color])
    {
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_NONE:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("weapons disabled by config, doing nothing"));
      break;
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_BY_ID:
      weapon_id = rules->team_profile_weapon_alternate_id[team_color];
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("alternate weapon %d for color %d (config)"), weapon_id, team_color);
      break;
    case LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_RANDOM:
      weapon_id = lw6sys_random (sys_context, LW6MAP_MAX_WEAPON_ID + 1);
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("random alternate weapon %d"), weapon_id);
      break;
    }

  ret = _do_fire (sys_context, map_state, rules, round, team_color, charge_percent, weapon_id);

  return ret;
}

int
_lw6ker_weapon_fire_berzerk (lw6sys_context_t * sys_context,
			     _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 1;

  /*
   * BERZERK weapon is implemented right in the move/action function,
   * when in this mode, the attack table is modified, nothing to do here.
   */
  _register_weapon_duration (sys_context, map_state, rules, round, team_color, charge_percent, LW6MAP_WEAPON_BERZERK);

  return ret;
}

int
_lw6ker_weapon_fire_invincible (lw6sys_context_t * sys_context,
				_lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 1;

  /*
   * INVINCIBLE weapon is implemented right in the move/action function,
   * when in this mode, the attack table is modified, nothing to do here.
   */
  _register_weapon_duration (sys_context, map_state, rules, round, team_color, charge_percent, LW6MAP_WEAPON_INVINCIBLE);

  return ret;
}

int
_lw6ker_weapon_fire_escape (lw6sys_context_t * sys_context,
			    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 1;
  int i = 0, x = 0, y = 0, z = 0;
  int pot = 0;
  int worst_pot = 0;
  int zone_id = 0;
  int slot_id = 0;
  int old_slot_id = 0;
  int new_slot_id = 0;
  lw6sys_whd_t shape = { 0, 0, 0 };
  lw6ker_fighter_t *fighter = NULL;
  int found = 0;
  lw6sys_xyz_t new_pos = { 0, 0, 0 };

  shape = map_state->map_struct->shape;
  for (i = 0; i < map_state->armies.active_fighters; ++i)
    {
      fighter = &(map_state->armies.fighters[i]);
      if (fighter->team_color == team_color)
	{
	  zone_id = _lw6ker_map_struct_get_zone_id (map_state->map_struct, fighter->pos.x, fighter->pos.y, fighter->pos.z);
	  worst_pot = map_state->teams[team_color].gradient[zone_id].potential;
	  found = 0;
	  for (z = 0; z < shape.d; ++z)
	    {
	      for (y = 0; y < shape.h; ++y)
		{
		  for (x = 0; x < shape.w; ++x)
		    {
		      slot_id = _lw6ker_map_struct_slot_index (map_state->map_struct, x, y, z);
		      zone_id = _lw6ker_map_struct_get_zone_id (map_state->map_struct, x, y, z);
		      if (slot_id >= 0 && zone_id >= 0)
			{
			  if (map_state->slots[slot_id].fighter_id < 0)
			    {
			      pot = map_state->teams[team_color].gradient[zone_id].potential;
			      if (pot < worst_pot)
				{
				  worst_pot = pot;
				  found = 1;
				  new_pos.x = x;
				  new_pos.y = y;
				  new_pos.z = z;
				}
			    }
			}
		    }
		}
	    }
	  if (found)
	    {
	      old_slot_id = _lw6ker_map_struct_slot_index (map_state->map_struct, fighter->pos.x, fighter->pos.y, fighter->pos.z);
	      new_slot_id = _lw6ker_map_struct_slot_index (map_state->map_struct, new_pos.x, new_pos.y, new_pos.z);
	      map_state->slots[new_slot_id].fighter_id = i;
	      map_state->slots[old_slot_id].fighter_id = -1;
	      fighter->pos = new_pos;
	    }
	}
    }

  _register_weapon_instant (sys_context, map_state, rules, round, team_color, charge_percent, LW6MAP_WEAPON_ESCAPE);

  return ret;
}

int
_lw6ker_weapon_fire_turbo (lw6sys_context_t * sys_context,
			   _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 1;

  /*
   * TURBO weapon is implemented right in the move/action function,
   * when in this mode, the move increment is modified, nothing to do here.
   */
  _register_weapon_duration (sys_context, map_state, rules, round, team_color, charge_percent, LW6MAP_WEAPON_TURBO);

  return ret;
}

int
_lw6ker_weapon_fire_teleport (lw6sys_context_t * sys_context,
			      _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_scatter (lw6sys_context_t * sys_context,
			     _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_fix (lw6sys_context_t * sys_context,
			 _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  /*
   * FIX is an exclusive weapon, as such, when someone is using
   * it, no one else can. So we disable it for everyone before triggering
   * it for a given team.
   */
  _lw6ker_weapon_unset_by_weapon_id (sys_context, map_state, LW6MAP_WEAPON_FIX);

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_mix (lw6sys_context_t * sys_context,
			 _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_control (lw6sys_context_t * sys_context,
			     _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  /*
   * CONTROL is an exclusive weapon, as such, when someone is using
   * it, no one else can. So we disable it for everyone before triggering
   * it for a given team.
   */
  _lw6ker_weapon_unset_by_weapon_id (sys_context, map_state, LW6MAP_WEAPON_CONTROL);

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_permutation (lw6sys_context_t * sys_context,
				 _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_steal (lw6sys_context_t * sys_context,
			   _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_atomic (lw6sys_context_t * sys_context,
			    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_reverse (lw6sys_context_t * sys_context,
			     _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_crazy (lw6sys_context_t * sys_context,
			   _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_rewind (lw6sys_context_t * sys_context,
			    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_attract (lw6sys_context_t * sys_context,
			     _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_shrink (lw6sys_context_t * sys_context,
			    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_kamikaze (lw6sys_context_t * sys_context,
			      _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_disappear (lw6sys_context_t * sys_context,
			       _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}

int
_lw6ker_weapon_fire_plague (lw6sys_context_t * sys_context,
			    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent)
{
  int ret = 0;

  // todo...

  return ret;
}
