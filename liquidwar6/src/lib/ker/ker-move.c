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

#if LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER
#define _CALL_IS_SLOT_FREE(A,B,C,D,E) _lw6ker_move_is_slot_free((A),(B),(C),(D),(E))
#define _CALL_IS_ENEMY_THERE(A,B,C,D,E,F,G,H) _lw6ker_move_is_enemy_there((A),(B),(C),(D),(E),(F),(G),(H))
#define _CALL_IS_ALLY_THERE(A,B,C,D,E,F) _lw6ker_move_is_ally_there((A),(B),(C),(D),(E),(F))
#define _CALL_FIND_STRAIGHT_DIR(A,B,C,D) _lw6ker_move_find_straight_dir((A),(B),(C),(D))
#define _CALL_FIND_BEST_DIR(A,B,C) _lw6ker_move_find_best_dir((A),(B),(C))
#define _CALL_GOTO_WITH_DIR_XY(A,B,C,D,E,F,G) _lw6ker_move_goto_with_dir_xy((A),(B),(C),(D),(E),(F),(G))
#define _CALL_GOTO_WITH_DIR_Z(A,B,C,D,E) _lw6ker_move_goto_with_dir_z((A),(B),(C),(D),(E))
#define _CALL_GOTO_WITH_DIR(A,B,C,D,E,F,G,H,I) _lw6ker_move_goto_with_dir((A),(B),(C),(D),(E),(F),(G),(H),(I))
#define _CALL_ADJUST_HEALTH(A,B) _lw6ker_move_adjust_health((A),(B))
#else // LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER
#define _CALL_IS_SLOT_FREE(A,B,C,D,E) _is_slot_free((A),(B),(C),(D),(E))
#define _CALL_IS_ENEMY_THERE(A,B,C,D,E,F,G,H) _is_enemy_there((A),(B),(C),(D),(E),(F),(G),(H))
#define _CALL_IS_ALLY_THERE(A,B,C,D,E,F) _is_ally_there((A),(B),(C),(D),(E),(F))
#define _CALL_FIND_STRAIGHT_DIR(A,B,C,D) _find_straight_dir((A),(B),(C),(D))
#define _CALL_FIND_BEST_DIR(A,B,C) _find_best_dir((A),(B),(C))
#define _CALL_GOTO_WITH_DIR_XY(A,B,C,D,E,F,G) _goto_with_dir_xy((A),(B),(C),(D),(E),(F),(G))
#define _CALL_GOTO_WITH_DIR_Z(A,B,C,D,E) _goto_with_dir_z((A),(B),(C),(D),(E))
#define _CALL_GOTO_WITH_DIR(A,B,C,D,E,F,G,H,I) _goto_with_dir((A),(B),(C),(D),(E),(F),(G),(H),(I))
#define _CALL_ADJUST_HEALTH(A,B) _adjust_health((A),(B))
#endif // LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER

static inline int
_is_slot_free (const _lw6ker_map_struct_t * map_struct, const _lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z)
{
  /*
   * It's important to test the presence of a fighter before testing
   * if the zone is free (as of "struct free") because fighters are
   * more rare that free spaces, so the test will return false faster
   * and do not even call get_zone_id.
   */
  return (_lw6ker_map_state_get_fighter_id (map_state, x, y, z) < 0 && _lw6ker_map_struct_get_zone_id (map_struct, x, y, z) >= 0);
}

int
_lw6ker_move_is_slot_free (const _lw6ker_map_struct_t * map_struct, const _lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z)
{
  return _is_slot_free (map_struct, map_state, x, y, z);
}

static inline int
_is_enemy_there (const _lw6ker_map_struct_t * map_struct,
		 const _lw6ker_map_state_t * map_state, int32_t team_color, int32_t x, int32_t y, int32_t z, int32_t * enemy_id, int32_t * enemy_color)
{
  int ret = 0;

#ifdef LW6_PARANOID
  if (_lw6ker_map_struct_get_zone_id (map_struct, x, y, z) >= 0)
    {
#endif // LW6_PARANOID
      *enemy_id = _lw6ker_map_state_get_fighter_id (map_state, x, y, z);
      if (*enemy_id >= 0)
	{
	  (*enemy_color) = map_state->armies.fighters[*enemy_id].team_color;
	  ret = (*enemy_color != team_color);
	}
#ifdef LW6_PARANOID
    }
#endif // LW6_PARANOID

  return ret;
}

int
_lw6ker_move_is_enemy_there (const _lw6ker_map_struct_t * map_struct,
			     const _lw6ker_map_state_t * map_state,
			     int32_t team_color, int32_t x, int32_t y, int32_t z, int32_t * enemy_id, int32_t * enemy_color)
{
  return _is_enemy_there (map_struct, map_state, team_color, x, y, z, enemy_id, enemy_color);
}

static inline int
_is_ally_there (const _lw6ker_map_struct_t * map_struct, const _lw6ker_map_state_t * map_state, int32_t team_color, int32_t x, int32_t y, int32_t z)
{
  int ret = 0;

#ifdef LW6_PARANOID
  if (_lw6ker_map_struct_get_zone_id (map_struct, x, y, z) >= 0)
    {
#endif // LW6_PARANOID

      int32_t ally_id;

      ally_id = _lw6ker_map_state_get_fighter_id (map_state, x, y, z);
      if (ally_id >= 0)
	{
	  ret = (map_state->armies.fighters[ally_id].team_color == team_color);
	}
#ifdef LW6_PARANOID
    }
#endif // LW6_PARANOID

  return ret;
}

int
_lw6ker_move_is_ally_there (const _lw6ker_map_struct_t * map_struct,
			    const _lw6ker_map_state_t * map_state, int32_t team_color, int32_t x, int32_t y, int32_t z)
{
  return _is_ally_there (map_struct, map_state, team_color, x, y, z);
}

static inline int32_t
_find_straight_dir (int from_x, int from_y, lw6sys_xyz_t to, int parity)
{
  int32_t ret;
  int32_t straight_dir = 0;

  if (to.y < from_y)
    {
      straight_dir = straight_dir | _LW6KER_STRAIGHT_DIR_UP;
    }
  if (to.x > from_x)
    {
      straight_dir = straight_dir | _LW6KER_STRAIGHT_DIR_RIGHT;
    }
  if (to.y > from_y)
    {
      straight_dir = straight_dir | _LW6KER_STRAIGHT_DIR_DOWN;
    }
  if (to.x < from_x)
    {
      straight_dir = straight_dir | _LW6KER_STRAIGHT_DIR_LEFT;
    }

  ret = _LW6KER_TABLES_STRAIGHT_DIRS[parity][straight_dir];

  return ret;
}

int32_t
_lw6ker_move_find_straight_dir (int from_x, int from_y, lw6sys_xyz_t to, int parity)
{
  return _find_straight_dir (from_x, from_y, to, parity);
}

/*
 * Higher potential = closer to the cursor
 */
static inline int32_t
_find_best_dir (const _lw6ker_map_state_t * map_state, const lw6ker_fighter_t * fighter, int parity)
{
  int32_t ret = fighter->last_direction;
  int32_t zone_id = _lw6ker_map_struct_get_zone_id (map_state->map_struct, fighter->pos.x,
						    fighter->pos.y, fighter->pos.z);
  _lw6ker_zone_state_t *zone_states = map_state->teams[fighter->team_color].gradient;

  if (zone_id >= 0)
    {
      if (zone_states[zone_id].direction_to_cursor >= 0)
	{
	  /*
	   * OK, it was cached...
	   */
	  ret = zone_states[zone_id].direction_to_cursor;
	}
      else
	{
	  /*
	   * Nothing in cache, we calculate it
	   */
	  int32_t i;
	  _lw6ker_zone_struct_t *zone_structs = map_state->map_struct->zones;
	  int32_t neighbour_zone_id;
	  _lw6ker_zone_struct_t *fighter_zone_struct;
	  fighter_zone_struct = &(zone_structs[zone_id]);
	  int32_t best_potential = zone_states[zone_id].potential;

	  ret = -1;

	  if (parity)
	    {
	      if (fighter_zone_struct->one_way_dir_odd >= 0)
		{
		  ret = fighter_zone_struct->one_way_dir_odd;
		}
	      else
		{
		  for (i = 0; i < LW6KER_NB_DIRS; ++i)
		    {
		      neighbour_zone_id = fighter_zone_struct->link[i];
		      if (neighbour_zone_id >= 0)
			{
			  if (zone_states[neighbour_zone_id].potential > best_potential)
			    {
			      best_potential = zone_states[neighbour_zone_id].potential;
			      ret = i;
			    }
			}
		    }
		}
	    }
	  else
	    {
	      if (fighter_zone_struct->one_way_dir_even >= 0)
		{
		  ret = fighter_zone_struct->one_way_dir_even;
		}
	      else
		{
		  for (i = LW6KER_NB_DIRS - 1; i >= 0; --i)
		    {
		      neighbour_zone_id = fighter_zone_struct->link[i];
		      if (neighbour_zone_id >= 0)
			{
			  if (zone_states[neighbour_zone_id].potential > best_potential)
			    {
			      best_potential = zone_states[neighbour_zone_id].potential;
			      ret = i;
			    }
			}
		    }
		}
	    }

	  if (ret < 0)
	    {
	      /*
	       * Now, in most cases, if we can't find an adjacent square
	       * which is closer, it just means we are in the square
	       * where the cursor is. So we go straight to it.
	       * Fortunately this information is cached in
	       * closest_cursor_pos.
	       */
	      ret = _CALL_FIND_STRAIGHT_DIR (fighter->pos.x, fighter->pos.y, zone_states[zone_id].closest_cursor_pos, parity);
	    }

	  if (ret < 0)
	    {
	      /*
	       * this can happen, for _find_straight_dir is very likely
	       * to return -1
	       */
	      ret = fighter->last_direction;
	    }

	  // cache for later use
	  zone_states[zone_id].direction_to_cursor = ret;
	}
    }

  return ret;
}

int32_t
_lw6ker_move_find_best_dir (const _lw6ker_map_state_t * map_state, const lw6ker_fighter_t * fighter, int parity)
{
  return _find_best_dir (map_state, fighter, parity);
}

static inline void
_goto_with_dir_xy (const lw6map_rules_t * rules, const lw6sys_whd_t * shape, int32_t * dst_x, int32_t * dst_y, int32_t src_x, int32_t src_y, int32_t move_dir)
{
  (*dst_x) = src_x + _LW6KER_TABLES_MOVE_X_OFFSET[move_dir];
  (*dst_y) = src_y + _LW6KER_TABLES_MOVE_Y_OFFSET[move_dir];

  /*
   * next line is need to stay within map boundaries,
   * it will also handle polarity information
   */
  lw6map_coords_fix_xy (rules, shape, dst_x, dst_y);
}

void
_lw6ker_move_goto_with_dir_xy (const lw6map_rules_t * rules,
			       const lw6sys_whd_t * shape, int32_t * dst_x, int32_t * dst_y, int32_t src_x, int32_t src_y, int32_t move_dir)
{
  _goto_with_dir_xy (rules, shape, dst_x, dst_y, src_x, src_y, move_dir);
}

static inline void
_goto_with_dir_z (const lw6map_rules_t * rules, const lw6sys_whd_t * shape, int32_t * dst_z, int32_t src_z, int32_t move_dir)
{
  (*dst_z) = src_z + _LW6KER_TABLES_MOVE_Z_OFFSET[move_dir];
  /*
   * next line is need to stay within map boundaries,
   * it will also handle polarity information
   */
  lw6map_coords_fix_z (rules, shape, dst_z);
}

void
_lw6ker_move_goto_with_dir_z (const lw6map_rules_t * rules, const lw6sys_whd_t * shape, int32_t * dst_z, int32_t src_z, int32_t move_dir)
{
  _goto_with_dir_z (rules, shape, dst_z, src_z, move_dir);
}

static inline void
_goto_with_dir (const lw6map_rules_t * rules, const lw6sys_whd_t * shape,
		int32_t * dst_x, int32_t * dst_y, int32_t * dst_z, int32_t src_x, int32_t src_y, int32_t src_z, int32_t move_dir)
{
  (*dst_x) = src_x + _LW6KER_TABLES_MOVE_X_OFFSET[move_dir];
  (*dst_y) = src_y + _LW6KER_TABLES_MOVE_Y_OFFSET[move_dir];
  (*dst_z) = src_z + _LW6KER_TABLES_MOVE_Z_OFFSET[move_dir];

  /*
   * next line is need to stay within map boundaries,
   * it will also handle polarity information
   */
  lw6map_coords_fix_xy (rules, shape, dst_x, dst_y);
  lw6map_coords_fix_z (rules, shape, dst_z);
}

void
_lw6ker_move_goto_with_dir (const lw6map_rules_t * rules,
			    const lw6sys_whd_t * shape, int32_t * dst_x,
			    int32_t * dst_y, int32_t * dst_z, int32_t src_x, int32_t src_y, int32_t src_z, int32_t move_dir)
{
  _goto_with_dir (rules, shape, dst_x, dst_y, dst_z, src_x, src_y, src_z, move_dir);
}

static inline void
_adjust_health (lw6ker_fighter_t * fighter, int32_t health_correction)
{
  fighter->health += health_correction;
  if (fighter->health > LW6MAP_MAX_FIGHTER_HEALTH)
    {
      fighter->health = LW6MAP_MAX_FIGHTER_HEALTH;
    }
  else if (fighter->health < 0)
    {
      fighter->health = 0;
    }
}

void
_lw6ker_move_adjust_health (lw6ker_fighter_t * fighter, int32_t health_correction)
{
  _adjust_health (fighter, health_correction);
}

/*
 * Probably the most time greedy function of the algorithm, and
 * one of the hardest one to optimize. It simply moves all the
 * fighters on the battlefield, once the gradient has been calculated.
 *
 * It's the direct equivalent of move_fighters in src/fighter.c
 * in Liquid War 5.
 */
void
_lw6ker_move_update_fighters_universal (lw6sys_context_t * sys_context, _lw6ker_move_context_t * context)
{
  _lw6ker_move_context_t lc;

  lc = *context;

  lc.loop_init = lc.parity ? lc.active_fighters - 1 : 0;
  lc.loop_step = lc.parity ? -1 : 1;
  for (lc.i = lc.loop_init; lc.i >= 0 && lc.i < lc.active_fighters; lc.i += lc.loop_step)
    {
      lc.fighter = &(lc.armies->fighters[lc.i]);
      lc.fighter_team_color = lc.fighter->team_color;
      if (lw6ker_team_mask_is_concerned (sys_context, lc.fighter_team_color, lc.team_mask))
	{
	  lc.place_struct = &lc.map_struct->places[_lw6ker_map_struct_place_index (lc.map_struct, lc.fighter->pos.x, lc.fighter->pos.y)];
	  /*
	   * We increment the act_counter. Depending on game rules, we might
	   * decide to change the default value (which can already depend
	   * on the map boost/glue settings) according to the specific
	   * team speed (profile_fast parameter).
	   */
	  if (lc.rules.use_team_profiles)
	    {
	      lc.fighter->act_counter += lw6ker_percent (lc.place_struct->act_incr, lc.per_team_fast[lc.fighter_team_color]);
	    }
	  else
	    {
	      lc.fighter->act_counter += lc.place_struct->act_incr;
	    }
	  if (lc.per_team_weapon_id[lc.fighter_team_color] == LW6MAP_WEAPON_INVINCIBLE)
	    {
	      lc.fighter->health = LW6MAP_MAX_FIGHTER_HEALTH;
	    }
	  while (lc.fighter->act_counter >= _LW6KER_ACT_LIMIT)
	    {
	      lc.fighter->act_counter -= _LW6KER_ACT_LIMIT;
	      lc.done_with_fighter = 0;

	      lc.x = lc.fighter_x = lc.fighter->pos.x;
	      lc.y = lc.fighter_y = lc.fighter->pos.y;
	      lc.z = lc.fighter_z = lc.fighter->pos.z;

	      _CALL_ADJUST_HEALTH (lc.fighter, lc.place_struct->health_correction);

	      lc.best_dir = _CALL_FIND_BEST_DIR (lc.map_state, lc.fighter, lc.parity);
	      lc.move_dir_table = &(_LW6KER_TABLES_MOVE_DIR[lc.parity][lc.best_dir][0]);
	      // we assume best_dir is equal to something valid now

	      if (lc.shape.d <= 1)
		{
		  for (lc.j = 0; lc.j < lc.per_team_nb_move_tries[lc.fighter_team_color]; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR_XY (&(lc.rules), &lc.shape, &lc.x, &lc.y, lc.fighter_x, lc.fighter_y, lc.test_dir);
		      if (_CALL_IS_SLOT_FREE (lc.map_struct, lc.map_state, lc.x, lc.y, lc.z))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_move (sys_context, lc.fighter, lc.i, lc.x, lc.y, lc.z, lc.map_state);
			  _lw6ker_fighter_regenerate (sys_context, lc.fighter, lc.fighter_regenerate[lc.fighter_team_color]);
			  break;
			}
		    }
		}
	      else
		{
		  for (lc.j = 0; lc.j < lc.per_team_nb_move_tries[lc.fighter_team_color]; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR (&(lc.rules), &lc.shape, &lc.x, &lc.y, &lc.z, lc.fighter_x, lc.fighter_y, lc.fighter_z, lc.test_dir);
		      if (_CALL_IS_SLOT_FREE (lc.map_struct, lc.map_state, lc.x, lc.y, lc.z))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_move (sys_context, lc.fighter, lc.i, lc.x, lc.y, lc.z, lc.map_state);
			  _lw6ker_fighter_regenerate (sys_context, lc.fighter, lc.fighter_regenerate[lc.fighter_team_color]);
			  break;
			}
		      if (lc.j + 1 == lc.rules.vertical_move)
			{
			  _CALL_GOTO_WITH_DIR_Z (&(lc.rules), &lc.shape, &lc.z, lc.fighter_z, LW6KER_DIR_UP);
			  if (_CALL_IS_SLOT_FREE (lc.map_struct, lc.map_state, lc.fighter_x, lc.fighter_y, lc.z))
			    {
			      lc.done_with_fighter = 1;
			      _lw6ker_fighter_move (sys_context, lc.fighter, lc.i, lc.fighter_x, lc.fighter_y, lc.z, lc.map_state);
			      _lw6ker_fighter_regenerate (sys_context, lc.fighter, lc.fighter_regenerate[lc.fighter_team_color]);
			      break;
			    }
			  _CALL_GOTO_WITH_DIR_Z (&(lc.rules), &lc.shape, &lc.z, lc.fighter_z, LW6KER_DIR_DOWN);
			  if (_CALL_IS_SLOT_FREE (lc.map_struct, lc.map_state, lc.fighter_x, lc.fighter_y, lc.z))
			    {
			      lc.done_with_fighter = 1;
			      _lw6ker_fighter_move (sys_context, lc.fighter, lc.i, lc.fighter_x, lc.fighter_y, lc.z, lc.map_state);
			      _lw6ker_fighter_regenerate (sys_context, lc.fighter, lc.fighter_regenerate[lc.fighter_team_color]);
			      break;
			    }
			}
		    }
		}

	      if (!lc.done_with_fighter)
		{
		  for (lc.j = 0; lc.j < lc.per_team_nb_attack_tries[lc.fighter_team_color]; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR_XY (&lc.rules, &lc.shape, &lc.x, &lc.y, lc.fighter_x, lc.fighter_y, lc.test_dir);
		      if (_CALL_IS_ENEMY_THERE (lc.map_struct, lc.map_state, lc.fighter_team_color, lc.x, lc.y, lc.z, &lc.enemy_id, &lc.enemy_color))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_attack (sys_context, lc.fighter,
						  lc.x, lc.y, lc.z,
						  lc.map_state,
						  lc.j ==
						  0 ? lc.fighter_attack
						  [lc.fighter_team_color]
						  [lc.enemy_color] :
						  lc.fighter_side_attack[lc.fighter_team_color][lc.enemy_color], lc.rules.fighter_new_health);
			  break;
			}
		    }
		}

	      if (!lc.done_with_fighter)
		{
		  for (lc.j = 0; lc.j < lc.per_team_nb_defense_tries[lc.fighter_team_color]; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR_XY (&lc.rules, &lc.shape, &lc.x, &lc.y, lc.fighter_x, lc.fighter_y, lc.test_dir);
		      if (_CALL_IS_ALLY_THERE (lc.map_struct, lc.map_state, lc.fighter_team_color, lc.x, lc.y, lc.z))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_defend (sys_context, lc.fighter,
						  lc.x, lc.y, lc.z,
						  lc.map_state,
						  lc.j == 0 ? lc.fighter_defense[lc.fighter_team_color] : lc.fighter_side_defense[lc.fighter_team_color]);
			  break;
			}
		    }
		}
	      if (!lc.done_with_fighter)
		{
		  _lw6ker_fighter_regenerate (sys_context, lc.fighter, lc.fighter_regenerate[lc.fighter_team_color]);
		}
	    }
	}
    }
}

/*
 * slow function, only for external use
 */
int
_lw6ker_move_get_best_next_pos (lw6sys_context_t * sys_context,
				const _lw6ker_game_state_t * game_state, lw6sys_xyz_t * next_pos, lw6sys_xyz_t * current_pos, int team_color)
{
  const _lw6ker_map_state_t *map_state = &(game_state->map_state);
  const _lw6ker_map_struct_t *map_struct = map_state->map_struct;
  const lw6map_rules_t *rules = &(game_state->game_struct->rules);
  const lw6sys_whd_t *shape = &(map_struct->shape);
  int32_t zone_id;
  const _lw6ker_zone_state_t *zone_states = map_state->teams[team_color].gradient;
  int best_direction = -1;
  lw6sys_xyz_t start_pos;
  lw6sys_xyz_t target;
  lw6sys_xyz_t test;
  int32_t test_x, test_y, test_z;
  int test_dir;
  int ret = 0;
  int parity = lw6sys_checksum_int32 (sys_context, game_state->rounds + team_color) % LW6KER_NB_PARITIES;
  int try;
  int i;

  start_pos = (*current_pos);
  zone_id = _lw6ker_map_struct_get_zone_id (map_struct, start_pos.x, start_pos.y, start_pos.z);
  if (zone_id < 0)
    {
      _lw6ker_map_struct_find_free_slot_near (sys_context, map_struct, &start_pos, *current_pos);
      zone_id = _lw6ker_map_struct_get_zone_id (map_struct, start_pos.x, start_pos.y, start_pos.z);
    }
  (*next_pos) = start_pos;
  if (zone_id >= 0)
    {
      _lw6ker_zone_struct_t *zone_structs = map_state->map_struct->zones;
      int32_t neighbour_zone_id;
      _lw6ker_zone_struct_t *fighter_zone_struct;
      fighter_zone_struct = &(zone_structs[zone_id]);
      int32_t best_potential = zone_states[zone_id].potential;

      if (parity)
	{
	  for (i = 0; i < LW6KER_NB_DIRS; ++i)
	    {
	      neighbour_zone_id = fighter_zone_struct->link[i];
	      if (neighbour_zone_id >= 0)
		{
		  if (zone_states[neighbour_zone_id].potential > best_potential)
		    {
		      best_potential = zone_states[neighbour_zone_id].potential;
		      best_direction = i;
		    }
		}
	    }
	}
      else
	{
	  for (i = LW6KER_NB_DIRS - 1; i >= 0; --i)
	    {
	      neighbour_zone_id = fighter_zone_struct->link[i];
	      if (neighbour_zone_id >= 0)
		{
		  if (zone_states[neighbour_zone_id].potential > best_potential)
		    {
		      best_potential = zone_states[neighbour_zone_id].potential;
		      best_direction = i;
		    }
		}
	    }
	}
      if (best_direction >= 0)
	{
	  for (try = 0; try < LW6MAP_MAX_NB_DIR_TRIES && !ret; ++try)
	    {
	      test = (*current_pos);
	      test_dir = _LW6KER_TABLES_MOVE_DIR[parity][best_direction][try];
	      _CALL_GOTO_WITH_DIR (rules, shape, &test_x, &test_y, &test_z, start_pos.x, start_pos.y, start_pos.z, test_dir);
	      test.x = test_x;
	      test.y = test_y;
	      test.z = test_z;
	      if (_lw6ker_map_struct_get_zone_id (map_struct, test.x, test.y, test.z) >= 0)
		{
		  (*next_pos) = test;
		  ret = 1;
		}
	    }
	}
      else
	{
	  test = start_pos;
	  target = zone_states[zone_id].closest_cursor_pos;
	  if (target.x > start_pos.x)
	    {
	      test.x++;
	    }
	  if (target.x < start_pos.x)
	    {
	      test.x--;
	    }
	  if (target.y > start_pos.y)
	    {
	      test.y++;
	    }
	  if (target.y < start_pos.y)
	    {
	      test.y--;
	    }
	  if (target.z > start_pos.z)
	    {
	      test.z++;
	    }
	  if (target.z < start_pos.z)
	    {
	      test.z--;
	    }
	  if (_lw6ker_map_struct_get_zone_id (map_struct, test.x, test.y, test.z) >= 0)
	    {
	      (*next_pos = test);
	      ret = 1;
	    }
	}
    }
  if (next_pos->x == current_pos->x && next_pos->y == current_pos->y && next_pos->z == current_pos->z)
    {
      ret = 0;
    }

  return ret;
}

/**
 * lw6ker_move_get_best_next_pos
 *
 * @game_state: the game_state to work on
 * @next_pos: the next position (out param)
 * @current_pos: the current position
 * @team_color: the team color
 *
 * Tries to find the best move given a position and a team. Note that
 * this function does not check for the presence of another fighter,
 * it will only check walls and can even (sometimes) fail when there's
 * a path. The reason is that it uses the game_state at a given
 * round and does not recalculate gradient while a real fighter
 * has an ever-changing gradient. Whatsoever, this can be
 * used to move cursors like they were fighters, it's not perfect
 * but gives a good illusion.
 *
 * Return value: 1 if best place found, 0 if not.
 */
int
lw6ker_move_get_best_next_pos (lw6sys_context_t * sys_context,
			       const lw6ker_game_state_t * game_state, lw6sys_xyz_t * next_pos, lw6sys_xyz_t * current_pos, int team_color)
{
  return _lw6ker_move_get_best_next_pos (sys_context, (const _lw6ker_game_state_t *) game_state, next_pos, current_pos, team_color);
}
