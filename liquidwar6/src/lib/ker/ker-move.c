/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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
#include <math.h>

#include "ker.h"
#include "ker-internal.h"

#if LW6_GPROF || LW6_INSTRUMENT || LW6_PROFILER
#define _CALL_IS_SLOT_FREE(A,B,C,D,E) _lw6ker_move_is_slot_free((A),(B),(C),(D),(E))
#define _CALL_IS_ENEMY_THERE(A,B,C,D,E,F) _lw6ker_move_is_enemy_there((A),(B),(C),(D),(E),(F))
#define _CALL_IS_ALLY_THERE(A,B,C,D,E,F) _lw6ker_move_is_ally_there((A),(B),(C),(D),(E),(F))
#define _CALL_FIND_STRAIGHT_DIR(A,B,C,D) _lw6ker_move_find_straight_dir((A),(B),(C),(D))
#define _CALL_FIND_BEST_DIR(A,B,C) _lw6ker_move_find_best_dir((A),(B),(C))
#define _CALL_GOTO_WITH_DIR_XY(A,B,C,D,E,F,G) _lw6ker_move_goto_with_dir_xy((A),(B),(C),(D),(E),(F),(G))
#define _CALL_GOTO_WITH_DIR_Z(A,B,C,D,E) _lw6ker_move_goto_with_dir_z((A),(B),(C),(D),(E))
#define _CALL_GOTO_WITH_DIR(A,B,C,D,E,F,G,H,I) _lw6ker_move_goto_with_dir((A),(B),(C),(D),(E),(F),(G),(H),(I))
#define _CALL_ADJUST_HEALTH(A,B) _lw6ker_move_adjust_health((A),(B))
#else
#define _CALL_IS_SLOT_FREE(A,B,C,D,E) _is_slot_free((A),(B),(C),(D),(E))
#define _CALL_IS_ENEMY_THERE(A,B,C,D,E,F) _is_enemy_there((A),(B),(C),(D),(E),(F))
#define _CALL_IS_ALLY_THERE(A,B,C,D,E,F) _is_ally_there((A),(B),(C),(D),(E),(F))
#define _CALL_FIND_STRAIGHT_DIR(A,B,C,D) _find_straight_dir((A),(B),(C),(D))
#define _CALL_FIND_BEST_DIR(A,B,C) _find_best_dir((A),(B),(C))
#define _CALL_GOTO_WITH_DIR_XY(A,B,C,D,E,F,G) _goto_with_dir_xy((A),(B),(C),(D),(E),(F),(G))
#define _CALL_GOTO_WITH_DIR_Z(A,B,C,D,E) _goto_with_dir_z((A),(B),(C),(D),(E))
#define _CALL_GOTO_WITH_DIR(A,B,C,D,E,F,G,H,I) _goto_with_dir((A),(B),(C),(D),(E),(F),(G),(H),(I))
#define _CALL_ADJUST_HEALTH(A,B) _adjust_health((A),(B))
#endif

static inline int
_is_slot_free (lw6ker_map_struct_t * map_struct,
	       lw6ker_map_state_t * map_state, int32_t x, int32_t y,
	       int32_t z)
{
  /*
   * It's important to test the presence of a fighter before testing
   * if the zone is free (as of "struct free") because fighters are
   * more rare that free spaces, so the test will return false faster
   * and do not even call get_zone_id.
   */
  return (lw6ker_map_state_get_fighter_id (map_state, x, y, z) < 0 &&
	  lw6ker_map_struct_get_zone_id (map_struct, x, y, z) >= 0);
}

int
_lw6ker_move_is_slot_free (lw6ker_map_struct_t * map_struct,
			   lw6ker_map_state_t * map_state, int32_t x,
			   int32_t y, int32_t z)
{
  return _is_slot_free (map_struct, map_state, x, y, z);
}

static inline int
_is_enemy_there (lw6ker_map_struct_t * map_struct,
		 lw6ker_map_state_t * map_state, int32_t team_color,
		 int32_t x, int32_t y, int32_t z)
{
  int ret = 0;

#ifdef LW6_PARANOID
  if (lw6ker_map_struct_get_zone_id (map_struct, x, y, z) >= 0)
    {
#endif // LW6_PARANOID
      int32_t enemy_id;

      enemy_id = lw6ker_map_state_get_fighter_id (map_state, x, y, z);
      if (enemy_id >= 0)
	{
	  ret =
	    (map_state->armies.fighters[enemy_id].team_color != team_color);
	}
#ifdef LW6_PARANOID
    }
#endif // LW6_PARANOID

  return ret;
}

int
_lw6ker_move_is_enemy_there (lw6ker_map_struct_t * map_struct,
			     lw6ker_map_state_t * map_state,
			     int32_t team_color, int32_t x, int32_t y,
			     int32_t z)
{
  return _is_enemy_there (map_struct, map_state, team_color, x, y, z);
}

static inline int
_is_ally_there (lw6ker_map_struct_t * map_struct,
		lw6ker_map_state_t * map_state, int32_t team_color, int32_t x,
		int32_t y, int32_t z)
{
  int ret = 0;

#ifdef LW6_PARANOID
  if (lw6ker_map_struct_get_zone_id (map_struct, x, y, z) >= 0)
    {
#endif // LW6_PARANOID

      int32_t ally_id;

      ally_id = lw6ker_map_state_get_fighter_id (map_state, x, y, z);
      if (ally_id >= 0)
	{
	  ret =
	    (map_state->armies.fighters[ally_id].team_color == team_color);
	}
#ifdef LW6_PARANOID
    }
#endif // LW6_PARANOID

  return ret;
}

int
_lw6ker_move_is_ally_there (lw6ker_map_struct_t * map_struct,
			    lw6ker_map_state_t * map_state,
			    int32_t team_color, int32_t x, int32_t y,
			    int32_t z)
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
_lw6ker_move_find_straight_dir (int from_x, int from_y, lw6sys_xyz_t to,
				int parity)
{
  return _find_straight_dir (from_x, from_y, to, parity);
}

/*
 * Higher potential = closer to the cursor
 */
static inline int32_t
_find_best_dir (lw6ker_map_state_t * map_state, lw6ker_fighter_t * fighter,
		int parity)
{
  int32_t ret = fighter->last_direction;
  int32_t zone_id =
    lw6ker_map_struct_get_zone_id (map_state->map_struct, fighter->pos.x,
				   fighter->pos.y, fighter->pos.z);
  lw6ker_zone_state_t *zone_states =
    map_state->teams[fighter->team_color].gradient;

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
	  lw6ker_zone_struct_t *zone_structs = map_state->map_struct->zones;
	  int32_t neighbour_zone_id;
	  lw6ker_zone_struct_t *fighter_zone_struct;
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
			  if (zone_states[neighbour_zone_id].potential >
			      best_potential)
			    {
			      best_potential =
				zone_states[neighbour_zone_id].potential;
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
			  if (zone_states[neighbour_zone_id].potential >
			      best_potential)
			    {
			      best_potential =
				zone_states[neighbour_zone_id].potential;
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
	      ret =
		_CALL_FIND_STRAIGHT_DIR (fighter->pos.x, fighter->pos.y,
					 zone_states
					 [zone_id].closest_cursor_pos,
					 parity);
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
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("fighter with incorrect zone_id=%d (pos=%d,%d,%d)"),
		  zone_id, (int) fighter->pos.x, (int) fighter->pos.y,
		  (int) fighter->pos.z);
    }

  return ret;
}

int32_t
_lw6ker_move_find_best_dir (lw6ker_map_state_t * map_state,
			    lw6ker_fighter_t * fighter, int parity)
{
  return _find_best_dir (map_state, fighter, parity);
}

static inline void
_goto_with_dir_xy (lw6map_rules_t * rules, lw6sys_whd_t * shape,
		   int32_t * dst_x, int32_t * dst_y, int32_t src_x,
		   int32_t src_y, int32_t move_dir)
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
_lw6ker_move_goto_with_dir_xy (lw6map_rules_t * rules,
			       lw6sys_whd_t * shape, int32_t * dst_x,
			       int32_t * dst_y, int32_t src_x,
			       int32_t src_y, int32_t move_dir)
{
  _goto_with_dir_xy (rules, shape, dst_x, dst_y, src_x, src_y, move_dir);
}

static inline void
_goto_with_dir_z (lw6map_rules_t * rules, lw6sys_whd_t * shape,
		  int32_t * dst_z, int32_t src_z, int32_t move_dir)
{
  (*dst_z) = src_z + _LW6KER_TABLES_MOVE_Z_OFFSET[move_dir];
  /*
   * next line is need to stay within map boundaries,
   * it will also handle polarity information
   */
  lw6map_coords_fix_z (rules, shape, dst_z);
}

void
_lw6ker_move_goto_with_dir_z (lw6map_rules_t * rules, lw6sys_whd_t * shape,
			      int32_t * dst_z, int32_t src_z,
			      int32_t move_dir)
{
  _goto_with_dir_z (rules, shape, dst_z, src_z, move_dir);
}

static inline void
_goto_with_dir (lw6map_rules_t * rules, lw6sys_whd_t * shape,
		int32_t * dst_x, int32_t * dst_y, int32_t * dst_z,
		int32_t src_x, int32_t src_y, int32_t src_z, int32_t move_dir)
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
_lw6ker_move_goto_with_dir (lw6map_rules_t * rules, lw6sys_whd_t * shape,
			    int32_t * dst_x, int32_t * dst_y,
			    int32_t * dst_z, int32_t src_x, int32_t src_y,
			    int32_t src_z, int32_t move_dir)
{
  _goto_with_dir (rules, shape,
		  dst_x, dst_y, dst_z, src_x, src_y, src_z, move_dir);
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
_lw6ker_move_adjust_health (lw6ker_fighter_t * fighter,
			    int32_t health_correction)
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
_lw6ker_move_update_fighters_universal (_lw6ker_move_context_t * context)
{
  _lw6ker_move_context_t lc;

  lc = *context;

  lc.loop_init = lc.parity ? lc.active_fighters - 1 : 0;
  lc.loop_step = lc.parity ? -1 : 1;
  for (lc.i = lc.loop_init; lc.i >= 0 && lc.i < lc.active_fighters;
       lc.i += lc.loop_step)
    {
      lc.fighter = &(lc.armies->fighters[lc.i]);
      lc.fighter_team_color = lc.fighter->team_color;
      if (lw6ker_team_mask_is_concerned (lc.fighter_team_color, lc.team_mask))
	{
	  lc.place_struct =
	    &lc.map_struct->places[lw6ker_map_struct_place_index
				   (lc.map_struct, lc.fighter->pos.x,
				    lc.fighter->pos.y)];
	  lc.fighter->act_counter += lc.place_struct->act_incr;
	  while (lc.fighter->act_counter >= LW6KER_ACT_LIMIT)
	    {
	      lc.fighter->act_counter -= LW6KER_ACT_LIMIT;
	      lc.done_with_fighter = 0;

	      lc.x = lc.fighter_x = lc.fighter->pos.x;
	      lc.y = lc.fighter_y = lc.fighter->pos.y;
	      lc.z = lc.fighter_z = lc.fighter->pos.z;

	      _CALL_ADJUST_HEALTH (lc.fighter,
				   lc.place_struct->health_correction);

	      lc.best_dir =
		_CALL_FIND_BEST_DIR (lc.map_state, lc.fighter, lc.parity);
	      lc.move_dir_table =
		&(_LW6KER_TABLES_MOVE_DIR[lc.parity][lc.best_dir][0]);
	      // we assume best_dir is equal to something valid now

	      if (lc.shape.d <= 1)
		{
		  for (lc.j = 0; lc.j < lc.rules.nb_move_tries; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR_XY (&(lc.rules), &lc.shape,
					      &lc.x, &lc.y, lc.fighter_x,
					      lc.fighter_y, lc.test_dir);
		      if (_CALL_IS_SLOT_FREE
			  (lc.map_struct, lc.map_state, lc.x, lc.y, 0))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_move (lc.fighter, lc.i, lc.x, lc.y,
						0, lc.map_state);
			  _lw6ker_fighter_regenerate (lc.fighter,
						      lc.rules.
						      fighter_regenerate);
			  break;
			}
		    }
		}
	      else
		{
		  for (lc.j = 0; lc.j < lc.rules.nb_move_tries; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR (&(lc.rules), &lc.shape, &lc.x,
					   &lc.y, &lc.z, lc.fighter_x,
					   lc.fighter_y, lc.fighter_z,
					   lc.test_dir);
		      if (_CALL_IS_SLOT_FREE
			  (lc.map_struct, lc.map_state, lc.x, lc.y, lc.z))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_move (lc.fighter, lc.i, lc.x, lc.y,
						lc.z, lc.map_state);
			  _lw6ker_fighter_regenerate (lc.fighter,
						      lc.rules.
						      fighter_regenerate);
			  break;
			}
		      if (lc.j + 1 == lc.rules.vertical_move)
			{
			  _CALL_GOTO_WITH_DIR_Z (&(lc.rules), &lc.shape,
						 &lc.z, lc.fighter_z,
						 LW6KER_DIR_UP);
			  if (_CALL_IS_SLOT_FREE
			      (lc.map_struct, lc.map_state, lc.fighter_x,
			       lc.fighter_y, lc.z))
			    {
			      lc.done_with_fighter = 1;
			      _lw6ker_fighter_move (lc.fighter, lc.i,
						    lc.fighter_x,
						    lc.fighter_y, lc.z,
						    lc.map_state);
			      _lw6ker_fighter_regenerate (lc.fighter,
							  lc.rules.
							  fighter_regenerate);
			      break;
			    }
			  _CALL_GOTO_WITH_DIR_Z (&(lc.rules), &lc.shape,
						 &lc.z, lc.fighter_z,
						 LW6KER_DIR_DOWN);
			  if (_CALL_IS_SLOT_FREE
			      (lc.map_struct, lc.map_state, lc.fighter_x,
			       lc.fighter_y, lc.z))
			    {
			      lc.done_with_fighter = 1;
			      _lw6ker_fighter_move (lc.fighter, lc.i,
						    lc.fighter_x,
						    lc.fighter_y, lc.z,
						    lc.map_state);
			      _lw6ker_fighter_regenerate (lc.fighter,
							  lc.rules.
							  fighter_regenerate);
			      break;
			    }
			}
		    }
		}

	      if (!lc.done_with_fighter)
		{
		  for (lc.j = 0; lc.j < lc.rules.nb_attack_tries; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR_XY (&lc.rules, &lc.shape, &lc.x,
					      &lc.y, lc.fighter_x,
					      lc.fighter_y, lc.test_dir);
		      if (_CALL_IS_ENEMY_THERE
			  (lc.map_struct, lc.map_state, lc.fighter_team_color,
			   lc.x, lc.y, lc.z))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_attack (lc.fighter,
						  lc.x, lc.y, lc.z,
						  lc.map_state,
						  lc.j ==
						  0 ? lc.rules.
						  fighter_attack : lc.
						  fighter_side_attack,
						  lc.rules.
						  fighter_new_health);
			  break;
			}
		    }
		}

	      if (!lc.done_with_fighter)
		{
		  for (lc.j = 0; lc.j < lc.rules.nb_defense_tries; ++lc.j)
		    {
		      lc.test_dir = lc.move_dir_table[lc.j];
		      _CALL_GOTO_WITH_DIR_XY (&lc.rules, &lc.shape, &lc.x,
					      &lc.y, lc.fighter_x,
					      lc.fighter_y, lc.test_dir);
		      if (_CALL_IS_ALLY_THERE
			  (lc.map_struct, lc.map_state, lc.fighter_team_color,
			   lc.x, lc.y, lc.z))
			{
			  lc.done_with_fighter = 1;
			  _lw6ker_fighter_defend (lc.fighter,
						  lc.x, lc.y, lc.z,
						  lc.map_state,
						  lc.j ==
						  0 ? lc.rules.
						  fighter_defense : lc.
						  fighter_side_defense);
			  break;
			}
		    }
		}
	      if (!lc.done_with_fighter)
		{
		  _lw6ker_fighter_regenerate (lc.fighter,
					      lc.rules.fighter_regenerate);
		}
	    }
	}
    }
}
