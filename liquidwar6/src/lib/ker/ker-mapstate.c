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
#include <math.h>

#include "ker.h"
#include "ker-internal.h"

int
_lw6ker_map_state_init (lw6sys_context_t * sys_context,
			_lw6ker_map_state_t * map_state, const _lw6ker_map_struct_t * map_struct, const lw6map_rules_t * rules, lw6sys_progress_t * progress)
{
  int ret = 0;
  int32_t i;

  lw6sys_progress_begin (sys_context, progress);

  map_state->map_struct = map_struct;
  map_state->shape = map_struct->shape;
  map_state->shape_surface = map_struct->nb_places;

  ret = 1;

  ret = ret && _lw6ker_armies_init (sys_context, &(map_state->armies), map_struct, rules);

  lw6sys_progress_update (sys_context, progress, 0, 3, 1);

  map_state->max_nb_teams = rules->max_nb_teams;
  for (i = 0; i < map_state->max_nb_teams; ++i)
    {
      ret = ret && _lw6ker_team_init (sys_context, &(map_state->teams[i]), map_struct, rules);
    }

  _lw6ker_cursor_array_init (sys_context, &(map_state->cursor_array));

  lw6sys_progress_update (sys_context, progress, 0, 3, 2);

  map_state->nb_slots = map_struct->shape.w * map_struct->shape.h * map_struct->shape.d;
  map_state->slots = (_lw6ker_slot_state_t *) LW6SYS_CALLOC (sys_context, map_state->nb_slots * sizeof (_lw6ker_slot_state_t));
  if (map_state->slots)
    {
      for (i = 0; i < map_state->nb_slots; ++i)
	{
	  map_state->slots[i].fighter_id = -1;
	}
    }
  else
    {
      ret = 0;

      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate SLOT_STATE array"));
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

void
_lw6ker_map_state_clear (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state)
{
  int32_t i;

  if (map_state->slots)
    {
      LW6SYS_FREE (sys_context, map_state->slots);
    }
  for (i = 0; i < map_state->max_nb_teams; ++i)
    {
      _lw6ker_team_clear (sys_context, &(map_state->teams[i]));
    }
  _lw6ker_armies_clear (sys_context, &(map_state->armies));
  memset (map_state, 0, sizeof (_lw6ker_map_state_t));
}

int
_lw6ker_map_state_sync (lw6sys_context_t * sys_context, _lw6ker_map_state_t * dst, const _lw6ker_map_state_t * src)
{
  int ret = 0;

  if (dst && src && _lw6ker_map_struct_lazy_compare (sys_context, dst->map_struct, src->map_struct))
    {
      int i;

      dst->shape = src->shape;
      dst->shape_surface = src->shape_surface;
      ret = _lw6ker_armies_sync (sys_context, &(dst->armies), &(src->armies));
      dst->max_nb_teams = src->max_nb_teams;
      for (i = 0; i < src->max_nb_teams; ++i)
	{
	  ret = ret && _lw6ker_team_sync (sys_context, &(dst->teams[i]), &(src->teams[i]));
	}
      memcpy (&(dst->cursor_array), &(src->cursor_array), sizeof (_lw6ker_cursor_array_t));
      dst->nb_slots = src->nb_slots;
      memcpy (dst->slots, src->slots, src->map_struct->nb_slots * sizeof (_lw6ker_slot_state_t));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map_state_copy only works if dst and src point to the same map_struct"));
    }

  return ret;
}

void
_lw6ker_map_state_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state, u_int32_t * checksum)
{
  int i;
  /*
   * No need to compute map_struct checksum again, it's done
   * within game_state_update_checksum which itself calls
   * game_struct_update_checksum.
   */
  lw6sys_checksum_update_whd (sys_context, checksum, &(map_state->shape));
  lw6sys_checksum_update_int32 (sys_context, checksum, map_state->shape_surface);
  _lw6ker_armies_update_checksum (sys_context, &(map_state->armies), checksum);
  lw6sys_checksum_update_int32 (sys_context, checksum, map_state->max_nb_teams);
  for (i = 0; i < map_state->max_nb_teams; ++i)
    {
      _lw6ker_team_update_checksum (sys_context, &(map_state->teams[i]), checksum);
    }
  _lw6ker_cursor_array_update_checksum (sys_context, &(map_state->cursor_array), checksum);
  lw6sys_checksum_update_int32 (sys_context, checksum, map_state->nb_slots);
  for (i = 0; i < map_state->nb_slots; ++i)
    {
      _lw6ker_slot_state_update_checksum (sys_context, &(map_state->slots[i]), checksum);
    }
}

int
_lw6ker_map_state_get_free_team_color (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state)
{
  int32_t i;
  int32_t ret = LW6MAP_TEAM_COLOR_INVALID;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS && ret < 0; ++i)
    {
      if (!map_state->teams[i].active)
	{
	  ret = i;
	}
    }

  return ret;
}

int32_t
_lw6ker_map_state_populate_team (lw6sys_context_t * sys_context,
				 _lw6ker_map_state_t * map_state,
				 int32_t team_color, int32_t nb_fighters, lw6sys_xyz_t desired_center, const lw6map_rules_t * rules)
{
  lw6sys_xyz_t real_center;
  int32_t angle, radius;
  int32_t max_radius, max_angle;
  int32_t x, y, z;
  int32_t nb_fighters_added = 0;
  int32_t new_fighter_id;
  lw6ker_fighter_t new_fighter;
  lw6sys_whd_t shape;

  shape = map_state->shape;
  _lw6ker_fighter_clear (sys_context, &new_fighter);
  _lw6ker_map_struct_find_free_slot_near (sys_context, map_state->map_struct, &real_center, desired_center);
  max_radius = 2 * (map_state->map_struct->shape.w + map_state->map_struct->shape.h);	// +, not *
  for (radius = 1; radius < max_radius && nb_fighters_added < nb_fighters; ++radius)
    {
      max_angle = radius * M_PI * 4;
      for (angle = 0; angle < max_angle && nb_fighters_added < nb_fighters; ++angle)
	{
	  x = real_center.x + (lw6ker_cos ((angle * LW6KER_TRIGO_2PI) / max_angle) * radius) / (LW6KER_TRIGO_RADIUS * 2);
	  y = real_center.y - (lw6ker_sin ((angle * LW6KER_TRIGO_2PI) / max_angle) * radius) / (LW6KER_TRIGO_RADIUS * 2);

	  lw6map_coords_fix_xy (rules, &shape, &x, &y);

	  for (z = 0; z < map_state->shape.d; ++z)
	    {
	      if (_lw6ker_map_struct_get_zone_id (map_state->map_struct, x, y, z) >= 0)
		{
		  if (_lw6ker_map_state_get_fighter_id (map_state, x, y, z) < 0)
		    {
		      new_fighter.team_color = team_color;
		      new_fighter.pos.x = x;
		      new_fighter.pos.y = y;
		      new_fighter.pos.z = z;
		      new_fighter.health = LW6MAP_MAX_FIGHTER_HEALTH;
		      new_fighter.last_direction = 0;
		      new_fighter_id = _lw6ker_armies_add_fighter (sys_context, &(map_state->armies), new_fighter);
		      if (new_fighter_id >= 0)
			{
			  nb_fighters_added++;
			  _lw6ker_map_state_set_fighter_id (map_state, x, y, z, new_fighter_id);
			}
		    }
		}
	    }
	}
    }

  _lw6ker_team_activate (sys_context, &(map_state->teams[team_color]), real_center);

  return nb_fighters_added;
}

int
_lw6ker_map_state_redistribute_team (lw6sys_context_t * sys_context,
				     _lw6ker_map_state_t * map_state,
				     int32_t dst_team_color, int32_t src_team_color, int32_t nb_fighters, const lw6map_rules_t * rules)
{
  int ret = 0;
  int32_t i, j;
  int32_t nb_fighters_redistributed = 0;

  if (nb_fighters <= map_state->armies.fighters_per_team[src_team_color])
    {
      while (nb_fighters_redistributed < nb_fighters)
	{
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS && nb_fighters_redistributed < nb_fighters; ++i)
	    {
	      for (j = i; j < map_state->armies.active_fighters && nb_fighters_redistributed < nb_fighters; j += LW6MAP_MAX_NB_TEAMS)
		{
		  if (map_state->armies.fighters[j].team_color == src_team_color)
		    {
		      map_state->armies.fighters[j].team_color = dst_team_color;
		      (map_state->armies.fighters_per_team[src_team_color])--;
		      (map_state->armies.fighters_per_team[dst_team_color])++;
		      map_state->armies.fighters[j].health = LW6MAP_MAX_FIGHTER_HEALTH;
		      nb_fighters_redistributed++;
		    }
		}
	    }
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("can't redistribute %d fighters from team %d which has only %d"),
		  nb_fighters, src_team_color, map_state->armies.fighters_per_team[src_team_color]);
    }

  return ret;
}

int
_lw6ker_map_state_cancel_team (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t team_color)
{
  int ret = 0;

  if (map_state->teams[team_color].active)
    {
      if (map_state->armies.fighters_per_team[team_color] == 0)
	{
	  _lw6ker_team_unactivate (sys_context, &(map_state->teams[team_color]));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("trying to cancel non-zeroed team %d, still has %d fighters"), team_color, map_state->armies.fighters_per_team[team_color]);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to cancel inactive team %d"), team_color);
    }

  return ret;
}

int
_lw6ker_map_state_remove_fighter (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t fighter_id)
{
  int ret = 0;
  int32_t last_fighter_id;

  last_fighter_id = map_state->armies.active_fighters - 1;
  if (fighter_id >= 0 && fighter_id <= last_fighter_id)
    {
      lw6ker_fighter_t fighter;
      lw6ker_fighter_t last_fighter;

      fighter = map_state->armies.fighters[fighter_id];
      last_fighter = map_state->armies.fighters[last_fighter_id];

      if (fighter_id < last_fighter_id)
	{
	  _lw6ker_map_state_set_fighter_id (map_state, last_fighter.pos.x, last_fighter.pos.y, last_fighter.pos.z, fighter_id);
	  /*
	   * It's important to really *exchange* the fighters, for
	   * deletion in the armies struct will read the last fighter's
	   * team_color for instance, to maintain the list of active
	   * fighters in each team. So affecting the remaining one
	   * is not enough.
	   */
	  map_state->armies.fighters[fighter_id] = last_fighter;
	  map_state->armies.fighters[last_fighter_id] = fighter;
	}

      _lw6ker_map_state_set_fighter_id (map_state, fighter.pos.x, fighter.pos.y, fighter.pos.z, -1);
      _lw6ker_armies_remove_fighter (sys_context, &(map_state->armies));

      ret = 1;
    }

  return ret;
}

int
_lw6ker_map_state_remove_fighters (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t nb_fighters)
{
  int32_t i, j;
  int ret = 0;
  int32_t nb_fighters_removed = 0;
  int32_t fighters_to_remove_per_team[LW6MAP_MAX_NB_TEAMS];
  int32_t fighters_to_remove_total = 0;
  int64_t this_team_fighters = 0;
  int64_t active_fighters = 0;
  lw6ker_fighter_t *fighter = NULL;
  int team_color;

  memset (fighters_to_remove_per_team, 0, sizeof (fighters_to_remove_per_team));
  if (map_state->armies.active_fighters > 0)
    {
      active_fighters = map_state->armies.active_fighters;
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  this_team_fighters = map_state->armies.fighters_per_team[i];
	  fighters_to_remove_per_team[i] = (nb_fighters * this_team_fighters) / active_fighters;
	  fighters_to_remove_total += fighters_to_remove_per_team[i];
	}
    }
  if (fighters_to_remove_total <= nb_fighters)
    {
      for (i = nb_fighters; fighters_to_remove_total < nb_fighters; ++i)
	{
	  /*
	   * i is just used as a pseudo-random value to cycle
	   * amoung teams
	   */
	  j = i % LW6MAP_MAX_NB_TEAMS;
	  if (map_state->armies.fighters_per_team[j] > fighters_to_remove_per_team[j] + 1)
	    {
	      fighters_to_remove_per_team[j]++;
	      fighters_to_remove_total++;
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("strange, fighters_to_remove_total=%d but nb_fighters=%d"), fighters_to_remove_total, nb_fighters);
    }

  if (nb_fighters <= map_state->armies.active_fighters)
    {
      while (nb_fighters_removed < nb_fighters)
	{
	  /*
	   * To remove fighters, we used to simply pseudo-randomly
	   * pass the current map, trying not to process
	   * low-numbered fighters systematically to avoid
	   * disadvantaging "first" teams, and whenever we
	   * encounter a fighter, we deleted it.
	   * Now it has evolved a bit, we check the fighter belongs
	   * to a team that still has some fighters to give, hence
	   * giving all teams reasonnable chances to survive.
	   * This should solve https://savannah.gnu.org/bugs/?28030
	   */
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS && nb_fighters_removed < nb_fighters; ++i)
	    {
	      for (j = i; j < map_state->armies.active_fighters && nb_fighters_removed < nb_fighters; j += LW6MAP_MAX_NB_TEAMS)
		{
		  fighter = &(map_state->armies.fighters[j]);
		  team_color = fighter->team_color;
		  if (fighters_to_remove_per_team[team_color] > 0)
		    {
		      _lw6ker_map_state_remove_fighter (sys_context, map_state, j);
		      fighters_to_remove_per_team[team_color]--;
		      fighters_to_remove_total--;
		      nb_fighters_removed++;
		    }
		}
	    }
	}
      if (fighters_to_remove_total != 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("fighters_to_remove_total=%d and should be 0"), fighters_to_remove_total);
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't remove %d fighters, map only has %d"), nb_fighters, map_state->armies.active_fighters);
    }

  return ret;
}

int
_lw6ker_map_state_remove_team_fighters (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t team_color, int32_t nb_fighters)
{
  int ret = 0;
  int32_t nb_fighters_removed = 0;
  int32_t i, j;

  nb_fighters = lw6sys_imin (nb_fighters, map_state->armies.fighters_per_team[team_color]);

  while (nb_fighters_removed < nb_fighters)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS && nb_fighters_removed < nb_fighters; ++i)
	{
	  for (j = i; j < map_state->armies.active_fighters && nb_fighters_removed < nb_fighters; j += LW6MAP_MAX_NB_TEAMS)
	    {
	      if (map_state->armies.fighters[j].team_color == team_color)
		{
		  _lw6ker_map_state_remove_fighter (sys_context, map_state, j);
		  nb_fighters_removed++;
		}
	    }
	}
    }

  ret = 1;

  return ret;
}

int
_lw6ker_map_state_sanity_check (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state)
{
  int ret = 1;
  int32_t i;
  int32_t real_fighters_per_team[LW6MAP_MAX_NB_TEAMS];
  int32_t fighter_id;
  lw6ker_fighter_t fighter;

  if (map_state->armies.active_fighters > map_state->armies.max_fighters)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("active_fighters (%d) > max_fighters (%d)"), map_state->armies.active_fighters, map_state->armies.max_fighters);
      ret = 0;
    }
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      real_fighters_per_team[i] = 0;
    }
  for (i = 0; i < map_state->armies.active_fighters; ++i)
    {
      fighter = map_state->armies.fighters[i];
      if (fighter.team_color >= LW6MAP_MAX_NB_TEAMS)	// <0 check useless, unsigned
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("fighter.team_color out of range (%d) for fighter %d"), fighter.team_color, i);
	  ret = 0;
	}
      (real_fighters_per_team[fighter.team_color])++;
      fighter_id = _lw6ker_map_state_get_fighter_id (map_state, fighter.pos.x, fighter.pos.y, fighter.pos.z);
      if (i != fighter_id)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("fighter %d in armies array pretends to be at layer=%d,x=%d,y=%d, but in fact there is fighter %d there from slots point of view"),
		      i, fighter.pos.x, fighter.pos.y, fighter.pos.z, fighter_id);
	  ret = 0;
	}
    }
  for (i = 0; i < map_state->nb_slots; ++i)
    {
      if (map_state->slots[i].fighter_id < -1 || map_state->slots[i].fighter_id >= map_state->armies.max_fighters)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("fighter_id=%d at slot %d is invalid (max_fighters=%d)"), map_state->slots[i].fighter_id, i, map_state->armies.max_fighters);
	  ret = 0;
	}
    }
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->armies.fighters_per_team[i] > 0 && !map_state->teams[i].active)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("team %d pretends to have %d fighters but is inactive"), i, map_state->armies.fighters_per_team[i]);
	  ret = 0;
	}
      if (map_state->armies.fighters_per_team[i] != real_fighters_per_team[i])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("team %d pretends to have %d fighters but counting them one founds %d"),
		      i, map_state->armies.fighters_per_team[i], real_fighters_per_team[i]);
	  ret = 0;
	}
    }

  return ret;
}

/*
 * Applies the cursors before spreading the gradient.
 */
void
_lw6ker_map_state_spread_gradient (lw6sys_context_t * sys_context,
				   _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int32_t nb_spreads, u_int32_t team_mask)
{
  int i;
  int teams_concerned[LW6MAP_MAX_NB_TEAMS];
  int n = 0;

  _lw6ker_map_state_apply_cursors (sys_context, map_state, rules, team_mask);
  /*
   * We first construct a list of the concerned teams, this is to
   * (hopefully) optimize the following OpenMP pragma, so that it's
   * fed with threads that do somethings only...
   */
  for (i = 0; i < map_state->max_nb_teams; ++i)
    {
      if (map_state->teams[i].active)
	{
	  if (lw6ker_team_mask_is_concerned (sys_context, i, team_mask))
	    {
	      teams_concerned[n] = i;
	      n++;
	    }
	}
    }
  /*
   * Now gradients can be calculated independently, OpenMP should
   * really help us here, as it's a time consuming operation
   */
#ifdef LW6_OPENMP
#pragma omp parallel for
#endif // LW6_OPENMP
  for (i = 0; i < n; ++i)
    {
      /*
       * Now it's important to define j *now* and only *now*
       * inside the above for loop, since when running parrallel,
       * every thread needs its own copy of j.
       */
      int j;

      for (j = 0; j < nb_spreads; ++j)
	{
	  _lw6ker_spread_update_gradient (sys_context, &(map_state->teams[teams_concerned[i]]), map_state->shape.d == 1, rules->spread_mode);
	}
    }
}

int
_lw6ker_map_state_get_nb_teams (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state)
{
  int ret = 0;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].active)
	{
	  ret++;
	}
    }

  return ret;
}

void
_lw6ker_map_state_move_fighters (lw6sys_context_t * sys_context,
				 _lw6ker_map_state_t * map_state, int round, int parity, const lw6map_rules_t * rules, int32_t nb_moves, u_int32_t team_mask)
{
  _lw6ker_move_context_t context;
  int move_i = 0;
  int i, j;

  memset (&context, 0, sizeof (_lw6ker_move_context_t));

  context.parity = parity;
  context.team_mask = team_mask;
  context.map_state = map_state;
  context.map_struct = map_state->map_struct;
  context.rules = *rules;
  context.armies = &(map_state->armies);
  context.active_fighters = context.armies->active_fighters;

  if (context.rules.use_team_profiles)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	    {
	      context.fighter_attack[i][j] =
		lw6ker_percent (lw6ker_percent (rules->fighter_attack, rules->team_profile_aggressive[i]), rules->team_profile_vulnerable[j]);
	    }
	}
    }
  else
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	    {
	      context.fighter_attack[i][j] = rules->fighter_attack;
	      context.fighter_side_attack[i][j] = lw6ker_percent (context.fighter_attack[i][j], rules->side_attack_factor);
	    }
	}
    }

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      context.fighter_defense[i] = rules->fighter_defense;
      context.fighter_side_defense[i] = lw6sys_imax (rules->fighter_regenerate, lw6ker_percent (rules->fighter_defense, rules->side_defense_factor));
      context.fighter_regenerate[i] = rules->fighter_regenerate;
    }
  context.shape = map_state->shape;

  /*
   * Here we initialize a per-team array with nb_..._tries values so
   * that we don't calculate it each time.
   */
  if (context.rules.use_team_profiles)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  context.per_team_nb_move_tries[i] = context.rules.nb_move_tries + context.rules.team_profile_mobile[i];
	  context.per_team_nb_attack_tries[i] = context.rules.nb_attack_tries + context.rules.team_profile_mobile[i];
	  context.per_team_nb_defense_tries[i] = context.rules.nb_defense_tries + context.rules.team_profile_mobile[i];

	  context.per_team_nb_move_tries[i] = lw6sys_imax (context.per_team_nb_move_tries[i], LW6MAP_RULES_MIN_NB_MOVE_TRIES);
	  context.per_team_nb_move_tries[i] = lw6sys_imin (context.per_team_nb_move_tries[i], LW6MAP_RULES_MAX_NB_MOVE_TRIES);
	  context.per_team_nb_attack_tries[i] = lw6sys_imax (context.per_team_nb_attack_tries[i], LW6MAP_RULES_MIN_NB_ATTACK_TRIES);
	  context.per_team_nb_attack_tries[i] = lw6sys_imin (context.per_team_nb_attack_tries[i], LW6MAP_RULES_MAX_NB_ATTACK_TRIES);
	  context.per_team_nb_defense_tries[i] = lw6sys_imax (context.per_team_nb_defense_tries[i], LW6MAP_RULES_MIN_NB_DEFENSE_TRIES);
	  context.per_team_nb_defense_tries[i] = lw6sys_imin (context.per_team_nb_defense_tries[i], LW6MAP_RULES_MAX_NB_DEFENSE_TRIES);

	  context.per_team_fast[i] = context.rules.team_profile_fast[i];

	  if (map_state->teams[i].weapon_first_round <= round
	      && map_state->teams[i].weapon_last_round >= round
	      && map_state->teams[i].weapon_id >= LW6MAP_MIN_WEAPON_ID && map_state->teams[i].weapon_id <= LW6MAP_MAX_WEAPON_ID)
	    {
	      context.per_team_weapon_id[i] = map_state->teams[i].weapon_id;
	    }
	  else
	    {
	      context.per_team_weapon_id[i] = LW6MAP_WEAPON_NONE;
	    }
	}
      /*
       * Post-processing attack values due to weapons and such
       */
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  if (context.per_team_weapon_id[i] == LW6MAP_WEAPON_BERZERK)
	    {
	      for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
		{
		  context.fighter_attack[i][j] *= context.rules.weapon_tune_berzerk_power;
		}
	    }
	  if (context.per_team_weapon_id[i] == LW6MAP_WEAPON_INVINCIBLE)
	    {
	      for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
		{
		  context.fighter_attack[j][i] = 0;
		}
	    }
	  if (context.per_team_weapon_id[i] == LW6MAP_WEAPON_TURBO)
	    {
	      context.per_team_fast[i] *= context.rules.weapon_tune_turbo_power;
	    }
	}
      /*
       * Now we check for min & max, this is not the definitive test
       * but it will enforce some theorical limits.
       */
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  context.fighter_defense[i] =
	    lw6sys_imax (LW6MAP_RULES_MIN_FIGHTER_DEFENSE, lw6sys_imin (context.fighter_defense[i], LW6MAP_RULES_MAX_FIGHTER_DEFENSE));
	  context.fighter_side_defense[i] = lw6sys_imin (context.fighter_side_defense[i], LW6MAP_RULES_MAX_FIGHTER_DEFENSE);
	  context.fighter_regenerate[i] =
	    lw6sys_imax (LW6MAP_RULES_MIN_FIGHTER_REGENERATE, lw6sys_imin (context.fighter_regenerate[i], LW6MAP_RULES_MAX_FIGHTER_REGENERATE));
	  for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	    {
	      context.fighter_attack[i][j] =
		lw6sys_imax (LW6MAP_RULES_MIN_FIGHTER_ATTACK, lw6sys_imin (context.fighter_attack[i][j], LW6MAP_RULES_MAX_FIGHTER_ATTACK));
	      context.fighter_side_attack[i][j] = lw6sys_imin (context.fighter_side_attack[i][j], LW6MAP_RULES_MAX_FIGHTER_ATTACK);
	    }
	}
      /*
       * OK, now we correct all values depending on speed, indeed
       * if we don't do this, faster fighters attack stronger, which
       * is theorically OK, they do act faster in any way, but in
       * pratice it's just plain wrong, we don't want the moving
       * speed to change the strength.
       */
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  context.per_team_fast[i] = lw6sys_imax (1, context.per_team_fast[i]);

	  context.fighter_defense[i] *= LW6MAP_RULES_DEFAULT_TEAM_PROFILE_FAST;
	  context.fighter_defense[i] /= context.per_team_fast[i];
	  context.fighter_side_defense[i] *= LW6MAP_RULES_DEFAULT_TEAM_PROFILE_FAST;
	  context.fighter_side_defense[i] /= context.per_team_fast[i];
	  context.fighter_regenerate[i] *= LW6MAP_RULES_DEFAULT_TEAM_PROFILE_FAST;
	  context.fighter_regenerate[i] /= context.per_team_fast[i];
	  for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	    {
	      context.fighter_attack[i][j] *= LW6MAP_RULES_DEFAULT_TEAM_PROFILE_FAST;
	      context.fighter_attack[i][j] /= context.per_team_fast[i];
	      context.fighter_side_attack[i][j] *= LW6MAP_RULES_DEFAULT_TEAM_PROFILE_FAST;
	      context.fighter_side_attack[i][j] /= context.per_team_fast[i];
	    }
	}
      /*
       * Now we check for min & max, a 2nd time, after all the calcs we did, since
       * speed/fast has an influence, this won't enfore some theorical
       * limits, but globally, the job will be done in the sense that
       * we won't have overflow errors.
       */
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  context.fighter_defense[i] =
	    lw6sys_imax (LW6MAP_RULES_MIN_FIGHTER_DEFENSE, lw6sys_imin (context.fighter_defense[i], LW6MAP_RULES_MAX_FIGHTER_DEFENSE));
	  context.fighter_side_defense[i] = lw6sys_imin (context.fighter_side_defense[i], LW6MAP_RULES_MAX_FIGHTER_DEFENSE);
	  context.fighter_regenerate[i] =
	    lw6sys_imax (LW6MAP_RULES_MIN_FIGHTER_REGENERATE, lw6sys_imin (context.fighter_regenerate[i], LW6MAP_RULES_MAX_FIGHTER_REGENERATE));
	  for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	    {
	      lw6sys_imax (LW6MAP_RULES_MIN_FIGHTER_ATTACK,
			   context.fighter_attack[i][j] = lw6sys_imin (context.fighter_attack[i][j], LW6MAP_RULES_MAX_FIGHTER_ATTACK));
	      context.fighter_side_attack[i][j] = lw6sys_imin (context.fighter_side_attack[i][j], LW6MAP_RULES_MAX_FIGHTER_ATTACK);
	    }
	}
    }
  else
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  context.per_team_nb_move_tries[i] = context.rules.nb_move_tries;
	  context.per_team_nb_attack_tries[i] = context.rules.nb_attack_tries;
	  context.per_team_nb_defense_tries[i] = context.rules.nb_defense_tries;
	  context.per_team_fast[i] = LW6MAP_RULES_DEFAULT_TEAM_PROFILE_FAST;
	  context.per_team_weapon_id[i] = LW6MAP_WEAPON_NONE;
	}
    }

  for (move_i = 0; move_i < nb_moves; ++move_i)
    {
      _lw6ker_move_update_fighters_universal (sys_context, &context);
    }
}

void
_lw6ker_map_state_apply_cursors (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, u_int32_t team_mask)
{
  int32_t i, team_color;
  int32_t zone_id = -1;
  int32_t z;
  int32_t max_pot[LW6MAP_MAX_NB_TEAMS];
  int32_t round_delta[LW6MAP_MAX_NB_TEAMS];

  /*
   * Wizardry to determine a new cursor_ref_pot. In most cases
   * it will end with max_pot being the old cursor_ref_pot+1
   * and delta being 1. But it's there for special cases, mostly
   * at game startup and when one fiddles arround with cursors
   * pot_offset fields..
   */
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (lw6ker_team_mask_is_concerned (sys_context, i, team_mask))
	{
	  max_pot[i] = map_state->teams[i].cursor_ref_pot;
	  round_delta[i] = rules->round_delta;
	}
    }
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      if (map_state->cursor_array.cursors[i].enabled)
	{
	  team_color = map_state->cursor_array.cursors[i].team_color;
	  if (lw6ker_team_mask_is_concerned (sys_context, team_color, team_mask))
	    {
	      {
		_lw6ker_cursor_update_apply_pos (sys_context, &(map_state->cursor_array.cursors[i]), map_state->map_struct);
		for (z = 0, zone_id = -1; z < map_state->shape.d && zone_id < 0; ++z)
		  {
		    zone_id =
		      _lw6ker_map_struct_get_zone_id (map_state->map_struct,
						      map_state->cursor_array.cursors[i].apply_pos.x, map_state->cursor_array.cursors[i].apply_pos.y, z);
		  }
		if (zone_id >= 0)
		  {
		    max_pot[team_color] = lw6sys_imax (max_pot[team_color], map_state->teams[team_color].gradient[zone_id].potential);
		    round_delta[team_color] =
		      lw6sys_imax (round_delta[team_color],
				   map_state->teams[team_color].cursor_ref_pot +
				   map_state->cursor_array.cursors[i].pot_offset - map_state->teams[team_color].gradient[zone_id].potential);
		    round_delta[team_color] = lw6sys_imin (round_delta[team_color], rules->max_round_delta);
		  }
	      }
	    }
	}
    }
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (lw6ker_team_mask_is_concerned (sys_context, i, team_mask))
	{
	  /*
	   * Following line is essential, it ensures that cursor potentials
	   * will increase at each game turn. A consequence is that one
	   * shouldn't call apply_cursors twice per game round. Indeed,
	   * calling apply_cursors changes the map state (cursor_ref_pot)
	   * so calling it twice isn't the same as calling it only once.
	   */
	  map_state->teams[i].cursor_ref_pot = max_pot[i] + round_delta[i];

	  if (map_state->teams[i].cursor_ref_pot + rules->max_cursor_pot_offset > rules->max_cursor_pot)
	    {
	      _lw6ker_team_normalize_pot (sys_context, &(map_state->teams[i]), rules);
	    }
	}
    }

  /*
   * We actually apply the cursors
   */
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      if (map_state->cursor_array.cursors[i].enabled)
	{
	  team_color = map_state->cursor_array.cursors[i].team_color;
	  if (lw6ker_team_mask_is_concerned (sys_context, team_color, team_mask))
	    {
	      for (z = 0; z < map_state->shape.d; ++z)
		{
		  zone_id =
		    _lw6ker_map_struct_get_zone_id (map_state->map_struct,
						    map_state->cursor_array.cursors[i].apply_pos.x, map_state->cursor_array.cursors[i].apply_pos.y, z);
		  if (zone_id >= 0)
		    {
		      /*
		       * Beware of multiple cursors, we check again if changing
		       * the value is needed.
		       */
		      if (map_state->teams[team_color].gradient[zone_id].potential <
			  map_state->teams[team_color].cursor_ref_pot + map_state->cursor_array.cursors[i].pot_offset)
			{
			  map_state->teams[team_color].gradient[zone_id].potential =
			    map_state->teams[team_color].cursor_ref_pot + map_state->cursor_array.cursors[i].pot_offset;
			  map_state->teams[team_color].gradient[zone_id].closest_cursor_pos = map_state->cursor_array.cursors[i].apply_pos;
			}
		    }
		}
	    }
	}
    }
}

void
_lw6ker_map_state_process_fire (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round)
{
  int32_t i, team_color;
  int charge_percent;

  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      if (map_state->cursor_array.cursors[i].enabled && (map_state->cursor_array.cursors[i].fire || map_state->cursor_array.cursors[i].fire2))
	{
	  team_color = map_state->cursor_array.cursors[i].team_color;
	  charge_percent = _lw6ker_team_get_charge_per1000 (sys_context, &(map_state->teams[team_color])) / 10;
	  if (charge_percent >= 100)
	    {
	      if (map_state->cursor_array.cursors[i].fire)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("primary fire team_color=%d charge_percent=%d"), team_color, charge_percent);
		  if (_lw6ker_weapon_fire (sys_context, map_state, rules, round, team_color, charge_percent))
		    {
		      _lw6ker_team_reset_charge (sys_context, &(map_state->teams[team_color]));
		    }
		  else if (map_state->cursor_array.cursors[i].fire2)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				  _x_ ("secondary fire team_color=%d charge_percent=%d (primary failed)"), team_color, charge_percent);
		      if (_lw6ker_weapon_fire2 (sys_context, map_state, rules, round, team_color, charge_percent))
			{
			  _lw6ker_team_reset_charge (sys_context, &(map_state->teams[team_color]));
			}
		    }
		}
	      else if (map_state->cursor_array.cursors[i].fire2)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("secondary fire team_color=%d charge_percent=%d"), team_color, charge_percent);
		  if (_lw6ker_weapon_fire2 (sys_context, map_state, rules, round, team_color, charge_percent))
		    {
		      _lw6ker_team_reset_charge (sys_context, &(map_state->teams[team_color]));
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't fire not enough charge for color %d"), team_color);
	    }
	}
      map_state->cursor_array.cursors[i].fire = 0;
    }
}

void
_lw6ker_map_state_frag (lw6sys_context_t * sys_context,
			_lw6ker_map_state_t * map_state, int team_color, int frags_mode, int frags_to_distribute, int frags_fade_out)
{
  int active_fighters = 0;
  int nb_loosers = 0;
  int nb_winners = 0;
  int i = 0, j = 0;
  _lw6ker_armies_t *armies = &(map_state->armies);
  int frags_total = 0;
  int delta_frags[LW6MAP_MAX_NB_TEAMS];

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      delta_frags[i] = 0;
    }

  if (frags_mode == LW6MAP_RULES_FRAGS_MODE_BALANCED || frags_mode == LW6MAP_RULES_FRAGS_MODE_PROPORTIONAL)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  if (map_state->teams[i].active)
	    {
	      armies->frags[i] = lw6ker_percent (armies->frags[i], frags_fade_out);
	    }
	}
    }

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].active && (!map_state->teams[i].offline))
	{
	  if (armies->fighters_per_team[i] <= 0)
	    {
	      nb_loosers++;
	      map_state->teams[i].offline = 1;
	    }
	  else
	    {
	      nb_winners++;
	    }
	}
    }

  if (frags_mode == LW6MAP_RULES_FRAGS_MODE_ONE_NEGATIVE_POINT)
    {
      // old school simple moe
      delta_frags[team_color] = -1;
    }

  if (frags_mode == LW6MAP_RULES_FRAGS_MODE_ONE_FOR_WINNERS_ALL_FOR_LOSER)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  if (map_state->teams[i].active)
	    {
	      if (armies->fighters_per_team[i] <= 0)
		{
		  delta_frags[i] = -lw6sys_imax (1, nb_winners);
		}
	      else
		{
		  delta_frags[i] = 1;
		}
	    }
	}

    }

  active_fighters = armies->active_fighters;
  if (frags_mode == LW6MAP_RULES_FRAGS_MODE_BALANCED || frags_mode == LW6MAP_RULES_FRAGS_MODE_PROPORTIONAL)
    {
      if (active_fighters > 0)
	{
	  if (nb_loosers > 0)
	    {
	      if (nb_winners > 0)
		{
		  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
		    {
		      if (map_state->teams[i].active)
			{
			  if (armies->fighters_per_team[i] <= 0)
			    {
			      if (frags_mode == LW6MAP_RULES_FRAGS_MODE_BALANCED)
				{
				  delta_frags[i] -= lw6sys_imax (1, frags_to_distribute / nb_loosers);
				}
			    }
			  else
			    {
			      if (frags_mode == LW6MAP_RULES_FRAGS_MODE_BALANCED || frags_mode == LW6MAP_RULES_FRAGS_MODE_PROPORTIONAL)
				delta_frags[i] += lw6sys_imax (1, (frags_to_distribute * armies->fighters_per_team[i]) / armies->active_fighters);
			    }
			}
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't calculate frags when there are no winners"));
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't calculate frags when there are no loosers"));
	    }
	}
    }

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].active)
	{
	  frags_total += delta_frags[i];
	}
    }

  if (frags_mode == LW6MAP_RULES_FRAGS_MODE_ONE_FOR_WINNERS_ALL_FOR_LOSER || frags_mode == LW6MAP_RULES_FRAGS_MODE_BALANCED)
    {
      while (frags_total < 0)
	{
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS && frags_total < 0; ++i)
	    {
	      j = (i + team_color) % LW6MAP_MAX_NB_TEAMS;
	      if (map_state->teams[j].active)
		{
		  if (delta_frags[j] >= 0)
		    {
		      delta_frags[j]++;
		      frags_total++;
		    }
		}
	    }
	}

      while (frags_total > 0)
	{
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS && frags_total > 0; ++i)
	    {
	      j = (i + team_color) % LW6MAP_MAX_NB_TEAMS;
	      if (map_state->teams[j].active)
		{
		  if (delta_frags[j] <= 0)
		    {
		      delta_frags[j]--;
		      frags_total--;
		    }
		}
	    }
	}
    }


  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      if (map_state->teams[i].active)
	{
	  armies->frags[i] += delta_frags[i];
	}
    }
}

void
_lw6ker_map_state_charge (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round)
{
  int32_t team_color;
  int charge_incr = 0;
  int charge_percent = 0;
  int charge_max = 0;
  int nb_teams;

  if (rules->use_team_profiles)
    {
      nb_teams = _lw6ker_map_state_get_nb_teams (sys_context, map_state);
      charge_max = lw6ker_percent (_LW6KER_CHARGE_LIMIT, rules->weapon_charge_max);

      if (rules->weapon_charge_delay > 0 && rules->rounds_per_sec > 0)
	{
	  if (nb_teams > 0)
	    {
	      for (team_color = 0; team_color < LW6MAP_MAX_NB_TEAMS; ++team_color)
		{
		  if (map_state->teams[team_color].active
		      && rules->team_profile_weapon_mode[team_color] !=
		      LW6MAP_RULES_TEAM_PROFILE_WEAPON_MODE_NONE &&
		      _lw6ker_map_state_get_weapon_per1000_left (sys_context, map_state, round, team_color) <= 0 && !map_state->teams[team_color].offline)
		    {
		      charge_incr = _LW6KER_CHARGE_LIMIT / (rules->weapon_charge_delay * rules->rounds_per_sec);
		      if (map_state->armies.active_fighters > 0)
			{
			  charge_percent = (map_state->armies.fighters_per_team[team_color] * nb_teams * 100) / map_state->armies.active_fighters;
			}
		      else
			{
			  charge_percent = 100;
			}
		      charge_percent = (charge_percent + 100) / 2;
		      charge_incr = lw6ker_percent (charge_incr, charge_percent);
		      map_state->teams[team_color].charge += charge_incr;
		      map_state->teams[team_color].charge = lw6sys_imin (map_state->teams[team_color].charge, charge_max);
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new charge for team %d is %d"), team_color, map_state->teams[team_color].charge);
		    }
		}
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("bad rules weapon_charge_delay=%d rounds_per_sec=%d"), rules->weapon_charge_delay, rules->rounds_per_sec);
	}
    }
}

int
_lw6ker_map_state_is_this_weapon_active (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state, int round, int weapon_id, int team_color)
{
  return _lw6ker_team_is_this_weapon_active (sys_context, &(map_state->teams[team_color]), round, weapon_id);
}

int
_lw6ker_map_state_get_weapon_per1000_left (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state, int round, int team_color)
{
  return _lw6ker_team_get_weapon_per1000_left (sys_context, &(map_state->teams[team_color]), round);
}
