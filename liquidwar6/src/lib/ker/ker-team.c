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

int
_lw6ker_team_init (lw6sys_context_t * sys_context, _lw6ker_team_t * team, const _lw6ker_map_struct_t * map_struct, const lw6map_rules_t * rules)
{
  int ret = 0;
  int i;

  team->active = 0;
  team->respawn_round = 0;
  team->offline = 0;
  team->map_struct = map_struct;
  team->gradient = (_lw6ker_zone_state_t *) LW6SYS_CALLOC (sys_context, map_struct->nb_zones * sizeof (_lw6ker_zone_state_t));
  team->cursor_ref_pot = rules->cursor_pot_init;
  team->last_spread_dir = LW6KER_DIR_NNE;
  // team->charge is set to 0 because of CALLOC
  team->weapon_id = LW6MAP_WEAPON_NONE;	// -1
  // team->weapon_first_round set to 0 because of CALLOC
  // team->weapon_last_round set to 0 because of CALLOC

  if (team->gradient)
    {
      for (i = 0; i < map_struct->nb_zones; ++i)
	{
	  /*
	   * Set direction to "uncalculated"
	   */
	  team->gradient[i].direction_to_cursor = -1;
	  /*
	   * Set closest cursor to the center of the square
	   */
	  team->gradient[i].closest_cursor_pos.x = map_struct->zones[i].pos.x + map_struct->zones[i].size / 2;
	  team->gradient[i].closest_cursor_pos.y = map_struct->zones[i].pos.y + map_struct->zones[i].size / 2;
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate gradient"));
    }

  return ret;
}

void
_lw6ker_team_clear (lw6sys_context_t * sys_context, _lw6ker_team_t * team)
{
  if (team->gradient)
    {
      LW6SYS_FREE (sys_context, team->gradient);
    }

  memset (team, 0, sizeof (_lw6ker_team_t));
}

int
_lw6ker_team_sync (lw6sys_context_t * sys_context, _lw6ker_team_t * dst, const _lw6ker_team_t * src)
{
  int ret = 0;

  if (dst && src && _lw6ker_map_struct_lazy_compare (sys_context, dst->map_struct, src->map_struct))
    {
      dst->active = src->active;
      dst->has_been_active = src->has_been_active;
      dst->respawn_round = src->respawn_round;
      dst->offline = src->offline;
      memcpy (dst->gradient, src->gradient, src->map_struct->nb_zones * sizeof (_lw6ker_zone_state_t));
      dst->cursor_ref_pot = src->cursor_ref_pot;
      dst->last_spread_dir = src->last_spread_dir;
      dst->charge = src->charge;
      dst->weapon_id = src->weapon_id;
      dst->weapon_first_round = src->weapon_first_round;
      dst->weapon_last_round = src->weapon_last_round;
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("team_copy only works if dst and src point to the same map_struct"));
    }

  return ret;
}

void
_lw6ker_team_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_team_t * team, u_int32_t * checksum)
{
  int i;

  lw6sys_checksum_update_int32 (sys_context, checksum, team->active);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->has_been_active);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->respawn_round);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->offline);
  // map_struct checksumed elsewhere
  for (i = 0; i < team->map_struct->nb_zones; ++i)
    {
      _lw6ker_zone_state_update_checksum (sys_context, &(team->gradient[i]), checksum);
    }
  lw6sys_checksum_update_int32 (sys_context, checksum, team->cursor_ref_pot);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->last_spread_dir);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->charge);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->weapon_id);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->weapon_first_round);
  lw6sys_checksum_update_int32 (sys_context, checksum, team->weapon_last_round);
}

void
_lw6ker_team_activate (lw6sys_context_t * sys_context, _lw6ker_team_t * team, lw6sys_xyz_t pos)
{
  team->active = 1;
  team->has_been_active = 1;
  team->respawn_round = 0;
  team->offline = 0;
  team->charge = 0;
}

void
_lw6ker_team_unactivate (lw6sys_context_t * sys_context, _lw6ker_team_t * team)
{
  team->active = 0;
  // carefull, do not touch "has_been_active" !!!
  team->respawn_round = 0;
  team->offline = 0;
  team->charge = 0;
}

void
_lw6ker_team_normalize_pot (lw6sys_context_t * sys_context, _lw6ker_team_t * team, const lw6map_rules_t * rules)
{
  int32_t i;
  int32_t n;
  u_int32_t min_pot = rules->max_cursor_pot;
  u_int32_t max_pot = 0;
  u_int32_t delta;
  _lw6ker_zone_state_t *zone_states = team->gradient;

  n = team->map_struct->nb_zones;
  for (i = 0; i < n; ++i)
    {
      min_pot = lw6sys_imin (min_pot, zone_states[i].potential);
      max_pot = lw6sys_imax (max_pot, zone_states[i].potential);
    }

  /*
   * Theorically, we could use delta=min_pot, but in the
   * case of an extremely big map, for which even after
   * long spreading, there are still pot=0 points, we
   * fallback on max_pot/2, that is we divide by 2
   * the cursor potential.
   */
  delta = lw6sys_imax (min_pot, max_pot / 2);

  for (i = 0; i < n; ++i)
    {
      zone_states[i].potential -= delta;
      if (zone_states[i].potential <= 0 || zone_states[i].potential > rules->max_cursor_pot)
	{
	  zone_states[i].potential = rules->cursor_pot_init;
	}
      zone_states[i].direction_to_cursor = -1;
    }

  team->cursor_ref_pot = max_pot - delta;
  if (team->cursor_ref_pot <= 0 || team->cursor_ref_pot > max_pot)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistent cursor_ref_pot=%d (max_pot=%d, delta=%d)"), team->cursor_ref_pot, max_pot, delta);
      team->cursor_ref_pot = rules->cursor_pot_init;
    }
}

int
_lw6ker_team_get_charge_per1000 (lw6sys_context_t * sys_context, const _lw6ker_team_t * team)
{
  int ret = 0;

  if (team->active)
    {
      ret = (team->charge * 1000) / _LW6KER_CHARGE_LIMIT;
    }

  return ret;
}

void
_lw6ker_team_reset_charge (lw6sys_context_t * sys_context, _lw6ker_team_t * team)
{
  team->charge = 0;
}

int
_lw6ker_team_is_this_weapon_active (lw6sys_context_t * sys_context, const _lw6ker_team_t * team, int round, int weapon_id)
{
  return (team->weapon_id == weapon_id && team->weapon_first_round <= round && team->weapon_last_round >= round);
}

int
_lw6ker_team_get_weapon_per1000_left (lw6sys_context_t * sys_context, const _lw6ker_team_t * team, int round)
{
  int ret = 0;
  int rounds_done = 0;
  int rounds_left = 0;
  int duration = 0;

  rounds_done = round - team->weapon_first_round;
  rounds_left = team->weapon_last_round - round;
  if (rounds_done >= 0 && rounds_left >= 0 && team->weapon_id >= LW6MAP_MIN_WEAPON_ID && team->weapon_id <= LW6MAP_MAX_WEAPON_ID)
    {
      duration = lw6sys_imax (1, rounds_done + rounds_left);
      ret = lw6sys_imax (1, lw6sys_imin (1000, rounds_left * 1000 / duration));
    }

  return ret;
}
