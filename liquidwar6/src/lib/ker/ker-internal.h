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

#ifndef LIQUIDWAR6KER_INTERNAL_H
#define LIQUIDWAR6KER_INTERNAL_H

#define _LW6KER_NB_STRAIGHT_DIRS 16
#define _LW6KER_STRAIGHT_DIR_UP 0x01;
#define _LW6KER_STRAIGHT_DIR_RIGHT 0x02;
#define _LW6KER_STRAIGHT_DIR_DOWN 0x04;
#define _LW6KER_STRAIGHT_DIR_LEFT 0x08;

typedef int32_t _lw6ker_tables_move_offset_t[LW6KER_NB_DIRS];
typedef int32_t _lw6ker_tables_move_dir_tries_t[LW6MAP_MAX_NB_DIR_TRIES];
typedef _lw6ker_tables_move_dir_tries_t
  _lw6ker_tables_move_dir_t[LW6KER_NB_PARITIES][LW6KER_NB_DIRS];
typedef int32_t
  _lw6ker_tables_straight_dirs_t[LW6KER_NB_PARITIES]
  [_LW6KER_NB_STRAIGHT_DIRS];

extern _lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_X_OFFSET;
extern _lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_Y_OFFSET;
extern _lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_Z_OFFSET;
extern _lw6ker_tables_move_dir_t _LW6KER_TABLES_MOVE_DIR;
extern _lw6ker_tables_straight_dirs_t _LW6KER_TABLES_STRAIGHT_DIRS;

typedef struct _lw6ker_move_context_s
{
  int i;
  int j;
  int parity;
  int32_t team_mask;
  lw6ker_map_state_t *map_state;
  lw6ker_map_struct_t *map_struct;
  lw6map_rules_t rules;
  lw6ker_armies_t *armies;
  int32_t active_fighters;
  lw6ker_fighter_t *fighter;
  lw6ker_place_struct_t *place_struct;
  int32_t x;
  int32_t y;
  int32_t z;
  int32_t best_dir;
  int32_t test_dir;
  int32_t fighter_team_color;
  int32_t fighter_x;
  int32_t fighter_y;
  int32_t fighter_z;
  int32_t loop_init;
  int32_t loop_step;
  int done_with_fighter;
  int32_t fighter_side_attack;
  int32_t fighter_side_defense;
  lw6sys_whd_t shape;
  int32_t *move_dir_table;
} _lw6ker_move_context_t;

/*
 * In armies.c
 */
extern int _lw6ker_armies_init (lw6ker_armies_t * armies,
				lw6ker_map_struct_t * map_struct,
				lw6map_rules_t * options);
extern void _lw6ker_armies_clear (lw6ker_armies_t * armies);
extern int _lw6ker_armies_sync (lw6ker_armies_t * dst, lw6ker_armies_t * src);
extern void _lw6ker_armies_update_checksum (lw6ker_armies_t * armies,
					    u_int32_t * checksum);

/*
 * In cursor.c
 */
extern void _lw6ker_cursor_reset (lw6ker_cursor_t * cursor);
extern void _lw6ker_cursor_init (lw6ker_cursor_t * cursor, char letter);
extern void _lw6ker_cursor_update_checksum (lw6ker_cursor_t * cursor,
					    u_int32_t * checksum);
extern int _lw6ker_cursor_check_node_id (lw6ker_cursor_t * cursor,
					 u_int64_t node_id);
extern int _lw6ker_cursor_get_start_xy (int32_t * x, int32_t * y,
					int team_color, int position_mode,
					int random_seed, lw6sys_whd_t * shape,
					lw6map_rules_t * rules);
extern void _lw6ker_cursor_update_apply_pos (lw6ker_cursor_t * cursor,
					     lw6ker_map_struct_t *
					     map_struct);

/*
 * In cursorarray.c
 */
extern void _lw6ker_cursor_array_reset (lw6ker_cursor_array_t * cursor_array);
extern void _lw6ker_cursor_array_init (lw6ker_cursor_array_t * cursor_array);
extern void _lw6ker_cursor_array_update_checksum (lw6ker_cursor_array_t *
						  cursor_array,
						  u_int32_t * checksum);
extern lw6ker_cursor_t *_lw6ker_cursor_array_find_free (lw6ker_cursor_array_t
							* cursor_array);
extern int _lw6ker_cursor_array_is_color_owned_by (lw6ker_cursor_array_t *
						   cursor_array,
						   u_int64_t node_id,
						   int team_color);

/*
 * In fighter.c
 */
extern void _lw6ker_fighter_clear (lw6ker_fighter_t * fighter);
extern void _lw6ker_fighter_update_checksum (lw6ker_fighter_t * fighter,
					     u_int32_t * checksum);
extern void _lw6ker_fighter_print_debug (lw6ker_fighter_t * fighter);
extern void _lw6ker_fighter_move (lw6ker_fighter_t * fighter,
				  int32_t fighter_id,
				  int32_t x,
				  int32_t y, int32_t z,
				  lw6ker_map_state_t * map_state);
extern void _lw6ker_fighter_attack (lw6ker_fighter_t * fighter,
				    int32_t x, int32_t y,
				    int32_t z, lw6ker_map_state_t * map_state,
				    int32_t fighter_attack,
				    int32_t fighter_new_health);
extern void _lw6ker_fighter_defend (lw6ker_fighter_t * fighter,
				    int32_t x, int32_t y,
				    int32_t z, lw6ker_map_state_t * map_state,
				    int32_t fighter_defense);
extern void _lw6ker_fighter_regenerate (lw6ker_fighter_t * fighter,
					int32_t fighter_regenerate);

/*
 * In gamestate.c
 */
extern void _lw6ker_game_state_update_checksum (lw6ker_game_state_t *
						game_state,
						u_int32_t * checksum);
extern int _lw6ker_game_state_add_team (lw6ker_game_state_t * game_state,
					int team_color, int position_mode);
extern int _lw6ker_game_state_remove_team (lw6ker_game_state_t * game_state,
					   int team_color);

/*
 * In gamestruct.c
 */
extern void _lw6ker_game_struct_update_checksum (lw6ker_game_struct_t *
						 game_struct,
						 u_int32_t * checksum);

/*
 * In history.c
 */
extern void _lw6ker_history_set (lw6ker_history_t * history,
				 lw6ker_armies_t * armies, int i);
extern void _lw6ker_history_add (lw6ker_history_t * history,
				 lw6ker_armies_t * armies);
extern int32_t _lw6ker_history_get (lw6ker_history_t * history,
				    int i, int team_id);
extern int32_t _lw6ker_history_get_max (lw6ker_history_t * history);
extern void _lw6ker_history_update_checksum (lw6ker_history_t * history,
					     u_int32_t * checksum);

/*
 * in mapstate.c
 */
extern int _lw6ker_map_state_init (lw6ker_map_state_t * map_state,
				   lw6ker_map_struct_t * map_struct,
				   lw6map_rules_t * options,
				   lw6sys_progress_t * progress);
extern void _lw6ker_map_state_clear (lw6ker_map_state_t * map_state);
extern int _lw6ker_map_state_sync (lw6ker_map_state_t * dst,
				   lw6ker_map_state_t * src);
extern void _lw6ker_map_state_update_checksum (lw6ker_map_state_t * map_state,
					       u_int32_t * checksum);
extern int _lw6ker_map_state_get_nb_teams (lw6ker_map_state_t * map_state);
extern void _lw6ker_map_state_frag (lw6ker_map_state_t * map_state,
				    int team_color, int frags_mode,
				    int frags_to_distribute,
				    int frags_fade_out);

/*
 * In mapstruct.c
 */
extern int _lw6ker_map_struct_init (lw6ker_map_struct_t * map_struct,
				    lw6map_level_t * level,
				    lw6sys_progress_t * progress);
extern void _lw6ker_map_struct_clear (lw6ker_map_struct_t * map_struct);
extern float _lw6ker_map_struct_get_compression (lw6ker_map_struct_t *
						 map_struct);
extern void _lw6ker_map_struct_update_checksum (lw6ker_map_struct_t *
						map_struct,
						u_int32_t * checksum);
extern int _lw6ker_map_struct_lazy_compare (lw6ker_map_struct_t *
					    map_struct_a,
					    lw6ker_map_struct_t *
					    map_struct_b);

/*
 * in move.c
 */
extern int _lw6ker_move_is_slot_free (lw6ker_map_struct_t * map_struct,
				      lw6ker_map_state_t * map_state,
				      int32_t x, int32_t y, int32_t z);
extern int _lw6ker_move_is_enemy_there (lw6ker_map_struct_t * map_struct,
					lw6ker_map_state_t * map_state,
					int32_t team_color, int32_t x,
					int32_t y, int32_t z);
extern int _lw6ker_move_is_ally_there (lw6ker_map_struct_t * map_struct,
				       lw6ker_map_state_t * map_state,
				       int32_t team_color, int32_t x,
				       int32_t y, int32_t z);
extern int32_t _lw6ker_move_find_straight_dir (int from_x, int from_y,
					       lw6sys_xyz_t to, int parity);
extern int32_t _lw6ker_move_find_best_dir (lw6ker_map_state_t * map_state,
					   lw6ker_fighter_t * fighter,
					   int parity);
extern void _lw6ker_move_goto_with_dir_xy (lw6map_rules_t * rules,
					   lw6sys_whd_t * shape,
					   int32_t * dst_x, int32_t * dst_y,
					   int32_t src_x, int32_t src_y,
					   int32_t move_dir);
extern void _lw6ker_move_goto_with_dir_z (lw6map_rules_t * rules,
					  lw6sys_whd_t * shape,
					  int32_t * dst_z, int32_t src_z,
					  int32_t move_dir);
extern void _lw6ker_move_goto_with_dir (lw6map_rules_t * rules,
					lw6sys_whd_t * shape, int32_t * dst_x,
					int32_t * dst_y, int32_t * dst_z,
					int32_t src_x, int32_t src_y,
					int32_t src_z, int32_t move_dir);
extern void _lw6ker_move_adjust_health (lw6ker_fighter_t * fighter,
					int32_t health_correction);
extern void _lw6ker_move_update_fighters_universal (_lw6ker_move_context_t *
						    context);

/* ker-placestruct.c */
extern void _lw6ker_place_struct_update_checksum (lw6ker_place_struct_t *
						  place_struct,
						  u_int32_t * checksum);

/*
 * in score.c
 */
extern int _lw6ker_score_sort_quantity_callback (lw6ker_score_t * score_a,
						 lw6ker_score_t * score_b);
extern int _lw6ker_score_sort_quantity_callback_desc (lw6ker_score_t *
						      score_a,
						      lw6ker_score_t *
						      score_b);
extern int _lw6ker_score_sort_frags_callback (lw6ker_score_t * score_a,
					      lw6ker_score_t * score_b);
extern int _lw6ker_score_sort_frags_callback_desc (lw6ker_score_t * score_a,
						   lw6ker_score_t * score_b);

/*
 * In node.c
 */
extern void _lw6ker_node_reset (lw6ker_node_t * node);
extern void _lw6ker_node_init (lw6ker_node_t * node);
extern void _lw6ker_node_update_checksum (lw6ker_node_t * node,
					  u_int32_t * checksum);

/*
 * In nodearray.c
 */
extern void _lw6ker_node_array_reset (lw6ker_node_array_t * node_array);
extern void _lw6ker_node_array_init (lw6ker_node_array_t * node_array);
extern void _lw6ker_node_array_update_checksum (lw6ker_node_array_t *
						node_array,
						u_int32_t * checksum);
extern lw6ker_node_t *_lw6ker_node_array_find_free (lw6ker_node_array_t
						    * node_array);

/*
 * In slotstruct.c
 */
extern void _lw6ker_slot_struct_update_checksum (lw6ker_slot_struct_t *
						 slot_struct,
						 u_int32_t * checksum);

/*
 * In team.c
 */
extern int _lw6ker_team_init (lw6ker_team_t * team,
			      lw6ker_map_struct_t * map_struct,
			      lw6map_rules_t * options);
extern void _lw6ker_team_clear (lw6ker_team_t * team);
extern int _lw6ker_team_sync (lw6ker_team_t * dst, lw6ker_team_t * src);
extern void _lw6ker_team_update_checksum (lw6ker_team_t * team,
					  u_int32_t * checksum);

/*
 * In slotstate.c
 */
extern void _lw6ker_slot_state_update_checksum (lw6ker_slot_state_t *
						slot_state,
						u_int32_t * checksum);

/*
 * In slotstruct.c
 */
extern void _lw6ker_slot_struct_update_checksum (lw6ker_slot_struct_t *
						 slot_struct,
						 u_int32_t * checksum);

/*
 * In spread.c
 */
extern int32_t _lw6ker_spread_next_dir (int32_t dir);
extern void _lw6ker_spread_update_gradient (lw6ker_team_t * team,
					    int skip_vertical);

/*
 * In zonestate.c
 */
extern void _lw6ker_zone_state_update_checksum (lw6ker_zone_state_t *
						zone_state,
						u_int32_t * checksum);

/*
 * In zonestruct.c
 */
extern void _lw6ker_zone_struct_update_checksum (lw6ker_zone_struct_t *
						 zone_struct,
						 u_int32_t * checksum);

#endif
