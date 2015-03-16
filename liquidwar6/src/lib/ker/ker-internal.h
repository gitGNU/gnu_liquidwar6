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

#ifndef LIQUIDWAR6KER_INTERNAL_H
#define LIQUIDWAR6KER_INTERNAL_H

#define _LW6KER_NB_STRAIGHT_DIRS 16
#define _LW6KER_STRAIGHT_DIR_UP 0x01;
#define _LW6KER_STRAIGHT_DIR_RIGHT 0x02;
#define _LW6KER_STRAIGHT_DIR_DOWN 0x04;
#define _LW6KER_STRAIGHT_DIR_LEFT 0x08;

#define _LW6KER_ACT_LIMIT 100
#define _LW6KER_CHARGE_LIMIT 1000000

#define _LW6KER_BERZERK_ROUNDS_TO_KILL 2

typedef int32_t _lw6ker_tables_move_offset_t[LW6KER_NB_DIRS];
typedef int32_t _lw6ker_tables_move_dir_tries_t[LW6MAP_MAX_NB_DIR_TRIES];
typedef _lw6ker_tables_move_dir_tries_t _lw6ker_tables_move_dir_t[LW6KER_NB_PARITIES][LW6KER_NB_DIRS];
typedef int32_t _lw6ker_tables_straight_dirs_t[LW6KER_NB_PARITIES][_LW6KER_NB_STRAIGHT_DIRS];

extern _lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_X_OFFSET;
extern _lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_Y_OFFSET;
extern _lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_Z_OFFSET;
extern _lw6ker_tables_move_dir_t _LW6KER_TABLES_MOVE_DIR;
extern _lw6ker_tables_straight_dirs_t _LW6KER_TABLES_STRAIGHT_DIRS;

typedef struct _lw6ker_place_struct_s
{
  int32_t act_incr:12;
  int32_t pad:4;		// free for later use
  int32_t health_correction:16;
}
_lw6ker_place_struct_t;

typedef struct _lw6ker_zone_struct_s
{
  lw6sys_xyz_t pos;
  int32_t size;
  int32_t link[LW6KER_NB_DIRS];
  int16_t one_way_dir_odd;
  int16_t one_way_dir_even;
}
_lw6ker_zone_struct_t;

typedef struct _lw6ker_slot_struct_s
{
  int32_t zone_id;
}
_lw6ker_slot_struct_t;

/*
 * To some extent, this structure is very similar to lw6map_level_t.
 * Let's explain why it exists separately:
 * - this structure is not only used to store the information, but
 *   also to present it in a form which is adapted to (fast) treatments.
 *   This means it can be redundant, akward, whereas lw6map_level_t is meant
 *   to just convey informations. _lw6ker_map_struct_t is used when the
 *   game is running, to inform the core algorithm of tha map structure,
 *   whereas lw6map_level_t is used to intialize _lw6ker_map_struct_t, to send
 *   the maps structure to other players over the network, to initialize
 *   the graphics artifacts which will be used for rendering, and so on.
 * - having a loading/serialization (that is lw6map_level_t) separated from
 *   algorithmic wizardry related to shortest path insanities might be
 *   what could prevent LW6 from being as bloated as LW5.
 */
typedef struct _lw6ker_map_struct_s
{
  lw6sys_whd_t shape;
  int32_t nb_places;
  int32_t nb_zones;
  int32_t nb_slots;
  int32_t room_for_armies;
  int32_t max_zone_size;
  _lw6ker_place_struct_t *places;
  _lw6ker_zone_struct_t *zones;
  _lw6ker_slot_struct_t *slots;
}
_lw6ker_map_struct_t;

/*
 * Game struct contains all the data which never change during a game.
 * If it was feasible in C, we would change it to "read-only" when game
 * starts.
 */
typedef struct _lw6ker_game_struct_s
{
  /*
   * First fields id, level, rules are also present in public structure,
   * don't change them!
   */
  u_int32_t id;
  const lw6map_level_t *level;
  lw6map_rules_t rules;
  /*
   * now is the private part
   */
  _lw6ker_map_struct_t map_struct;
}
_lw6ker_game_struct_t;

/*
 * Completes _lw6ker_zone_struct_t with per-team information.
 */
typedef struct _lw6ker_zone_state_s
{
  int32_t potential:24;
  int32_t direction_to_cursor:8;
  lw6sys_xyz_t closest_cursor_pos;
}
_lw6ker_zone_state_t;

typedef struct _lw6ker_slot_state_s
{
  int32_t fighter_id;
}
_lw6ker_slot_state_t;

typedef struct _lw6ker_node_s
{
  u_int64_t node_id;
  int enabled;
  int last_command_round;
}
_lw6ker_node_t;

typedef struct _lw6ker_node_array_s
{
  int32_t nb_nodes;
  _lw6ker_node_t nodes[LW6MAP_MAX_NB_NODES];
}
_lw6ker_node_array_t;

typedef struct _lw6ker_cursor_array_s
{
  int32_t nb_cursors;
  lw6ker_cursor_t cursors[LW6MAP_MAX_NB_CURSORS];
}
_lw6ker_cursor_array_t;

typedef struct _lw6ker_team_s
{
  int active;
  int has_been_active;
  int respawn_round;
  int offline;
  const _lw6ker_map_struct_t *map_struct;
  _lw6ker_zone_state_t *gradient;
  int32_t cursor_ref_pot;
  int32_t last_spread_dir;
  int32_t charge;
  int32_t weapon_id;
  int32_t weapon_first_round;
  int32_t weapon_last_round;
}
_lw6ker_team_t;

typedef struct _lw6ker_armies_s
{
  const _lw6ker_map_struct_t *map_struct;
  /*
   * The maximum number or fighters in the armies, in fact
   * it's more a less the amount of free space in the map, minus
   * some standard percentage to avoid bloating a map completely
   * (playing with 100% space filled doesn't really make sense).
   */
  int32_t max_fighters;
  /*
   * The number of active fighters, that is the ones that are
   * actually doing things on the map, moving, fighting. Other
   * ones are dormant fighters which will activate when a
   * network player connects for instance.
   */
  int32_t active_fighters;
  /*
   * This is highly redundant for one could get this information
   * by simply reading the fighters themselves, however this is
   * inefficient, and having the exact count is usefull at every
   * game refreh, to display the information on the screen.
   */
  int32_t fighters_per_team[LW6MAP_MAX_NB_TEAMS];
  int32_t frags[LW6MAP_MAX_NB_TEAMS];
  /*
   * The actual data, a pointer to all the fighters in the map.
   * It's more convenient to access them this way, instead of
   * having to read a w*h array, which can be very big and 90%
   * empty. This optimization is not usefull on small and/or
   * crowded maps, but experience shows that
   * a) only small maps can be crowded anyway
   * b) small maps are fast, only big maps require optimization.
   */
  lw6ker_fighter_t *fighters;
}
_lw6ker_armies_t;

typedef struct _lw6ker_map_state_s
{
  const _lw6ker_map_struct_t *map_struct;
  lw6sys_whd_t shape;		// redundant but convenient
  int32_t shape_surface;
  _lw6ker_armies_t armies;
  int32_t max_nb_teams;
  _lw6ker_team_t teams[LW6MAP_MAX_NB_TEAMS];
  _lw6ker_cursor_array_t cursor_array;
  int32_t nb_slots;		// redundant but convenient
  _lw6ker_slot_state_t *slots;
}
_lw6ker_map_state_t;
typedef struct _lw6ker_history_s
{
  int nb_entries;
  int nb_fighters[LW6KER_HISTORY_SIZE][LW6MAP_MAX_NB_TEAMS];
} _lw6ker_history_t;

/*
 * Game state contains all the data which is verstatile, stuff that
 * changes, this includes "where fighters are" of course but also
 * some parameters which might change in-game (such as an option
 * which can be changeable, an alliance between teams, and so on...)
 */
typedef struct _lw6ker_game_state_s
{
  /*
   * First fields id and game_struct are also present in public structure,
   * don't change them!
   */
  u_int32_t id;
  const _lw6ker_game_struct_t *game_struct;
  /*
   * Now is the private part
   */
  _lw6ker_map_state_t map_state;
  _lw6ker_node_array_t node_array;
  _lw6ker_history_t global_history;
  _lw6ker_history_t latest_history;
  volatile u_int32_t moves;
  volatile u_int32_t spreads;
  volatile u_int32_t rounds;
  volatile u_int32_t total_rounds;
  int max_reached_teams;
  int over;
  /*
   * The following parameter should not be used in checksums,
   * in fact it's used to calculate and dump automatically
   * checksums on a regular basis, but it should not change
   * the checksum itself. Set to 0 to disable feature.
   */
  int checksum_log_interval;
}
_lw6ker_game_state_t;

typedef struct _lw6ker_move_context_s
{
  int i;
  int j;
  int parity;
  int32_t team_mask;
  _lw6ker_map_state_t *map_state;
  const _lw6ker_map_struct_t *map_struct;
  lw6map_rules_t rules;
  _lw6ker_armies_t *armies;
  int32_t active_fighters;
  lw6ker_fighter_t *fighter;
  const _lw6ker_place_struct_t *place_struct;
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
  int32_t fighter_attack[LW6MAP_MAX_NB_TEAMS][LW6MAP_MAX_NB_TEAMS];
  int32_t fighter_side_attack[LW6MAP_MAX_NB_TEAMS][LW6MAP_MAX_NB_TEAMS];
  int32_t fighter_defense[LW6MAP_MAX_NB_TEAMS];
  int32_t fighter_side_defense[LW6MAP_MAX_NB_TEAMS];
  int32_t fighter_regenerate[LW6MAP_MAX_NB_TEAMS];
  int32_t per_team_nb_move_tries[LW6MAP_MAX_NB_TEAMS];
  int32_t per_team_nb_attack_tries[LW6MAP_MAX_NB_TEAMS];
  int32_t per_team_nb_defense_tries[LW6MAP_MAX_NB_TEAMS];
  int32_t per_team_fast[LW6MAP_MAX_NB_TEAMS];
  int32_t per_team_weapon_id[LW6MAP_MAX_NB_TEAMS];
  lw6sys_whd_t shape;
  int32_t *move_dir_table;
  int32_t enemy_id;
  int32_t enemy_color;
} _lw6ker_move_context_t;

/* ker-armies.c */
extern int _lw6ker_armies_init (lw6sys_context_t * sys_context,
				_lw6ker_armies_t * armies, const _lw6ker_map_struct_t * map_struct, const lw6map_rules_t * options);
extern void _lw6ker_armies_clear (lw6sys_context_t * sys_context, _lw6ker_armies_t * armies);
extern int _lw6ker_armies_sync (lw6sys_context_t * sys_context, _lw6ker_armies_t * dst, const _lw6ker_armies_t * src);
extern void _lw6ker_armies_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_armies_t * armies, u_int32_t * checksum);
extern int32_t _lw6ker_armies_add_fighter (lw6sys_context_t * sys_context, _lw6ker_armies_t * armies, lw6ker_fighter_t fighter);
extern int _lw6ker_armies_remove_fighter (lw6sys_context_t * sys_context, _lw6ker_armies_t * armies);

/* ker-capture.c */
extern char *_lw6ker_capture_str (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);

/* ker-checksumlog.c */
extern void _lw6ker_game_state_checksum_log_set_interval (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, int checksum_log_interval);
extern void _lw6ker_game_state_checksum_log_if_needed (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);

/* ker-cursor.c */
extern void _lw6ker_cursor_init (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor, char letter);
extern void _lw6ker_cursor_update_checksum (lw6sys_context_t * sys_context, const lw6ker_cursor_t * cursor, u_int32_t * checksum);
extern int _lw6ker_cursor_check_node_id (lw6sys_context_t * sys_context, const lw6ker_cursor_t * cursor, u_int64_t node_id);
extern int _lw6ker_cursor_get_start_xy (lw6sys_context_t * sys_context,
					int32_t * x, int32_t * y,
					int team_color, int position_mode, int random_seed, const lw6sys_whd_t * shape, const lw6map_rules_t * rules);
extern void _lw6ker_cursor_update_apply_pos (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor, const _lw6ker_map_struct_t * map_struct);
extern int _lw6ker_cursor_enable (lw6sys_context_t * sys_context,
				  lw6ker_cursor_t * cursor, u_int64_t node_id, u_int16_t cursor_id, int team_color, int32_t x, int32_t y);
extern int _lw6ker_cursor_disable (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor);
extern int _lw6ker_cursor_update (lw6sys_context_t * sys_context,
				  lw6ker_cursor_t * cursor, int32_t x,
				  int32_t y, int fire, int fire2, int32_t pot_offset, const lw6sys_whd_t * shape, const lw6map_rules_t * rules);
extern int _lw6ker_cursor_sanity_check (lw6sys_context_t * sys_context,
					const lw6ker_cursor_t * cursor, const lw6sys_whd_t * shape, const lw6map_rules_t * rules);

/* ker-cursorarray.c */
extern void _lw6ker_cursor_array_reset (lw6sys_context_t * sys_context, _lw6ker_cursor_array_t * cursor_array);
extern void _lw6ker_cursor_array_init (lw6sys_context_t * sys_context, _lw6ker_cursor_array_t * cursor_array);
extern void _lw6ker_cursor_array_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_cursor_array_t * cursor_array, u_int32_t * checksum);
extern lw6ker_cursor_t *_lw6ker_cursor_array_find_free (lw6sys_context_t * sys_context, _lw6ker_cursor_array_t * cursor_array);
extern int _lw6ker_cursor_array_is_color_owned_by (lw6sys_context_t *
						   sys_context, const _lw6ker_cursor_array_t * cursor_array, u_int64_t node_id, int team_color);
extern lw6ker_cursor_t *_lw6ker_cursor_array_get_rw (lw6sys_context_t * sys_context, _lw6ker_cursor_array_t * cursor_array, u_int16_t cursor_id);
extern const lw6ker_cursor_t *_lw6ker_cursor_array_get_ro (lw6sys_context_t * sys_context, const _lw6ker_cursor_array_t * cursor_array, u_int16_t cursor_id);
extern int _lw6ker_cursor_array_enable (lw6sys_context_t * sys_context,
					_lw6ker_cursor_array_t * cursor_array, u_int64_t node_id, u_int16_t cursor_id, int team_color, int32_t x, int32_t y);
extern int _lw6ker_cursor_array_disable (lw6sys_context_t * sys_context, _lw6ker_cursor_array_t * cursor_array, u_int64_t node_id, u_int16_t cursor_id);
extern int _lw6ker_cursor_array_update (lw6sys_context_t * sys_context,
					_lw6ker_cursor_array_t * cursor_array,
					u_int64_t node_id,
					u_int16_t cursor_id, int32_t x,
					int32_t y, int fire, int fire2, int32_t pot_offset, const lw6sys_whd_t * shape, const lw6map_rules_t * rules);
extern int _lw6ker_cursor_array_sanity_check (lw6sys_context_t * sys_context,
					      const _lw6ker_cursor_array_t * cursor_array, const lw6sys_whd_t * shape, const lw6map_rules_t * rules);


/* ker-fighter.c */
extern void _lw6ker_fighter_clear (lw6sys_context_t * sys_context, lw6ker_fighter_t * fighter);
extern void _lw6ker_fighter_update_checksum (lw6sys_context_t * sys_context, const lw6ker_fighter_t * fighter, u_int32_t * checksum);
extern void _lw6ker_fighter_print_debug (lw6sys_context_t * sys_context, lw6ker_fighter_t * fighter);
extern void _lw6ker_fighter_move (lw6sys_context_t * sys_context,
				  lw6ker_fighter_t * fighter, int32_t fighter_id, int32_t x, int32_t y, int32_t z, _lw6ker_map_state_t * map_state);
extern void _lw6ker_fighter_attack (lw6sys_context_t * sys_context,
				    lw6ker_fighter_t * fighter, int32_t x,
				    int32_t y, int32_t z, _lw6ker_map_state_t * map_state, int32_t fighter_attack, int32_t fighter_new_health);
extern void _lw6ker_fighter_defend (lw6sys_context_t * sys_context,
				    lw6ker_fighter_t * fighter, int32_t x, int32_t y, int32_t z, _lw6ker_map_state_t * map_state, int32_t fighter_defense);
extern void _lw6ker_fighter_regenerate (lw6sys_context_t * sys_context, lw6ker_fighter_t * fighter, int32_t fighter_regenerate);

/* ker-gamestate.c */
extern void _lw6ker_game_state_set_id (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state);
extern _lw6ker_game_state_t *_lw6ker_game_state_new (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress);
extern void _lw6ker_game_state_free (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state);
extern void _lw6ker_game_state_point_to (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, const _lw6ker_game_struct_t * game_struct);
extern int _lw6ker_game_state_memory_footprint (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state);
extern char *_lw6ker_game_state_repr (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int _lw6ker_game_state_can_sync (lw6sys_context_t * sys_context, _lw6ker_game_state_t * dst, const _lw6ker_game_state_t * src);
extern int _lw6ker_game_state_sync (lw6sys_context_t * sys_context, _lw6ker_game_state_t * dst, const _lw6ker_game_state_t * src);
extern _lw6ker_game_state_t *_lw6ker_game_state_dup (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, lw6sys_progress_t * progress);
extern void _lw6ker_game_state_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int32_t * checksum);
extern u_int32_t _lw6ker_game_state_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int _lw6ker_game_state_register_node (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id);
extern int _lw6ker_game_state_unregister_node (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id);
extern int _lw6ker_game_state_node_exists (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int64_t node_id);
extern int _lw6ker_game_state_get_node_info (lw6sys_context_t * sys_context,
					     const _lw6ker_game_state_t * game_state, u_int16_t node_id, u_int32_t * last_command_round);
extern int _lw6ker_game_state_add_cursor (lw6sys_context_t * sys_context,
					  _lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id, int team_color);
extern int _lw6ker_game_state_remove_cursor (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id);
extern int _lw6ker_game_state_cursor_exists (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int16_t cursor_id);
extern int _lw6ker_game_state_get_cursor (lw6sys_context_t * sys_context,
					  const _lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, u_int16_t cursor_id);
extern void _lw6ker_game_state_get_cursor_by_index (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, int i);
extern int _lw6ker_game_state_set_cursor (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor);
extern int _lw6ker_game_state_add_team_internal (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, int team_color, int position_mode);
extern int _lw6ker_game_state_add_team (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id, int team_color);
extern int _lw6ker_game_state_remove_team_internal (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, int team_color);
extern int _lw6ker_game_state_remove_team (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id, int team_color);
extern int _lw6ker_game_state_team_exists (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int team_color);
extern int _lw6ker_game_state_get_team_info (lw6sys_context_t * sys_context,
					     const _lw6ker_game_state_t * game_state, int team_color, int32_t * nb_cursors, int32_t * nb_fighters);
extern int _lw6ker_game_state_get_nb_teams (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern void _lw6ker_game_state_do_spread (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int32_t team_mask);
extern void _lw6ker_game_state_do_move (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int32_t team_mask);
extern void _lw6ker_game_state_finish_round (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state);
extern void _lw6ker_game_state_do_round (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state);
extern u_int32_t _lw6ker_game_state_get_moves (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern u_int32_t _lw6ker_game_state_get_spreads (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern u_int32_t _lw6ker_game_state_get_rounds (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern u_int32_t _lw6ker_game_state_get_total_rounds (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int _lw6ker_game_state_is_over (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int _lw6ker_game_state_did_cursor_win (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int16_t cursor_id);
extern int _lw6ker_game_state_get_winner (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int excluded_team);
extern int _lw6ker_game_state_get_looser (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int excluded_team);
extern int32_t _lw6ker_game_state_get_nb_active_fighters (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int32_t _lw6ker_game_state_get_time_elapsed (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int32_t _lw6ker_game_state_get_time_left (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int32_t _lw6ker_game_state_get_global_history (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int i, int team_id);
extern int32_t _lw6ker_game_state_get_latest_history (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int i, int team_id);
extern int32_t _lw6ker_game_state_get_global_history_max (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int32_t _lw6ker_game_state_get_latest_history_max (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);

/* ker-gamestruct.c */
extern void _lw6ker_game_struct_set_id (lw6sys_context_t * sys_context, _lw6ker_game_struct_t * game_struct);
extern _lw6ker_game_struct_t *_lw6ker_game_struct_new (lw6sys_context_t * sys_context, const lw6map_level_t * level, lw6sys_progress_t * progress);
extern void _lw6ker_game_struct_free (lw6sys_context_t * sys_context, _lw6ker_game_struct_t * game_struct);
extern void _lw6ker_game_struct_point_to (lw6sys_context_t * sys_context, _lw6ker_game_struct_t * game_struct, const lw6map_level_t * level);
extern int _lw6ker_game_struct_memory_footprint (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct);
extern char *_lw6ker_game_struct_repr (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct);
extern _lw6ker_game_struct_t *_lw6ker_game_struct_dup (lw6sys_context_t *
						       sys_context, const _lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress);
extern void _lw6ker_game_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct, u_int32_t * checksum);
extern u_int32_t _lw6ker_game_struct_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct);
extern int _lw6ker_game_state_get_nb_colors (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int _lw6ker_game_state_get_nb_cursors (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern int _lw6ker_game_state_get_nb_nodes (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);

/* ker-hexa.c */
extern char *_lw6ker_game_struct_to_hexa (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct);
extern _lw6ker_game_struct_t *_lw6ker_game_struct_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const lw6map_level_t * level);
extern char *_lw6ker_game_state_to_hexa (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state);
extern _lw6ker_game_state_t *_lw6ker_game_state_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const _lw6ker_game_struct_t * game_struct);

/* ker-history.c */
extern void _lw6ker_history_set (lw6sys_context_t * sys_context, _lw6ker_history_t * history, _lw6ker_armies_t * armies, int i);
extern void _lw6ker_history_add (lw6sys_context_t * sys_context, _lw6ker_history_t * history, _lw6ker_armies_t * armies);
extern int32_t _lw6ker_history_get (lw6sys_context_t * sys_context, const _lw6ker_history_t * history, int i, int team_id);
extern int32_t _lw6ker_history_get_max (lw6sys_context_t * sys_context, const _lw6ker_history_t * history);
extern void _lw6ker_history_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_history_t * history, u_int32_t * checksum);

/* ker-mapstate.c */
extern int _lw6ker_map_state_init (lw6sys_context_t * sys_context,
				   _lw6ker_map_state_t * map_state,
				   const _lw6ker_map_struct_t * map_struct, const lw6map_rules_t * rules, lw6sys_progress_t * progress);
extern void _lw6ker_map_state_clear (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state);
extern int _lw6ker_map_state_sync (lw6sys_context_t * sys_context, _lw6ker_map_state_t * dst, const _lw6ker_map_state_t * src);
extern void _lw6ker_map_state_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state, u_int32_t * checksum);
extern int _lw6ker_map_state_get_nb_teams (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state);
extern void _lw6ker_map_state_frag (lw6sys_context_t * sys_context,
				    _lw6ker_map_state_t * map_state, int team_color, int frags_mode, int frags_to_distribute, int frags_fade_out);
static inline int
_lw6ker_map_state_slot_index (const _lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z)
{
  return (map_state->shape_surface * z + map_state->shape.w * y + x);
}

extern int _lw6ker_map_state_get_free_team_color (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state);
extern int32_t _lw6ker_map_state_populate_team (lw6sys_context_t *
						sys_context,
						_lw6ker_map_state_t *
						map_state, int32_t team_color, int32_t nb_fighters, lw6sys_xyz_t desired_center, const lw6map_rules_t * rules);
extern int _lw6ker_map_state_redistribute_team (lw6sys_context_t *
						sys_context,
						_lw6ker_map_state_t *
						map_state, int32_t dst_team_color, int32_t src_team_color, int32_t nb_fighters, const lw6map_rules_t * rules);
extern int _lw6ker_map_state_cancel_team (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t team_color);
extern int _lw6ker_map_state_remove_fighter (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t fighter_id);
extern int _lw6ker_map_state_remove_fighters (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t nb_fighters);
extern int _lw6ker_map_state_remove_team_fighters (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int32_t team_color, int32_t nb_fighters);
static inline void
_lw6ker_map_state_set_fighter_id (_lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z, int32_t fighter_id)
{
  map_state->slots[_lw6ker_map_state_slot_index (map_state, x, y, z)].fighter_id = fighter_id;
};

static inline int32_t
_lw6ker_map_state_get_fighter_id (const _lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z)
{
  return (map_state->slots[_lw6ker_map_state_slot_index (map_state, x, y, z)].fighter_id);
};

/*
 * Carefull with this one, there must really be a fighter
 * or it segfaults directly.
 */
static inline lw6ker_fighter_t *
_lw6ker_map_state_get_fighter_rw_unsafe (_lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z)
{
  return (&(map_state->armies.fighters[map_state->slots[_lw6ker_map_state_slot_index (map_state, x, y, z)].fighter_id]));
};

static inline const lw6ker_fighter_t *
_lw6ker_map_state_get_fighter_ro_unsafe (const _lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z)
{
  return (&(map_state->armies.fighters[map_state->slots[_lw6ker_map_state_slot_index (map_state, x, y, z)].fighter_id]));
};

extern int _lw6ker_map_state_sanity_check (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state);
extern void _lw6ker_map_state_spread_gradient (lw6sys_context_t * sys_context,
					       _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int32_t nb_spreads, u_int32_t team_mask);
extern void _lw6ker_map_state_move_fighters (lw6sys_context_t * sys_context,
					     _lw6ker_map_state_t * map_state,
					     int round, int parity, const lw6map_rules_t * rules, int32_t nb_moves, u_int32_t team_mask);
extern void _lw6ker_map_state_apply_cursors (lw6sys_context_t * sys_context,
					     _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, u_int32_t team_mask);
extern void _lw6ker_map_state_process_fire (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round);
extern void _lw6ker_map_state_charge (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round);
extern int _lw6ker_map_state_is_this_weapon_active (lw6sys_context_t *
						    sys_context, const _lw6ker_map_state_t * map_state, int round, int weapon_id, int team_color);
extern int _lw6ker_map_state_get_weapon_per1000_left (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state, int round, int team_color);

/* ker-mapstruct.c */
extern int _lw6ker_map_struct_init (lw6sys_context_t * sys_context,
				    _lw6ker_map_struct_t * map_struct, const lw6map_level_t * level, lw6sys_progress_t * progress);
extern void _lw6ker_map_struct_clear (lw6sys_context_t * sys_context, _lw6ker_map_struct_t * map_struct);
extern float _lw6ker_map_struct_get_compression (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct);
extern void _lw6ker_map_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct, u_int32_t * checksum);
extern int _lw6ker_map_struct_lazy_compare (lw6sys_context_t * sys_context,
					    const _lw6ker_map_struct_t * map_struct_a, const _lw6ker_map_struct_t * map_struct_b);
static inline int
_lw6ker_map_struct_slot_index (const _lw6ker_map_struct_t * map_struct, int32_t x, int32_t y, int32_t z)
{
  return (map_struct->nb_places * z + map_struct->shape.w * y + x);
}

static inline void
_lw6ker_map_struct_set_zone_id (_lw6ker_map_struct_t * map_struct, int32_t x, int32_t y, int32_t z, int32_t zone_id)
{
  map_struct->slots[_lw6ker_map_struct_slot_index (map_struct, x, y, z)].zone_id = zone_id;
};

static inline int32_t
_lw6ker_map_struct_get_zone_id (const _lw6ker_map_struct_t * map_struct, int32_t x, int32_t y, int32_t z)
{
  return (map_struct->slots[_lw6ker_map_struct_slot_index (map_struct, x, y, z)].zone_id);
};

extern void _lw6ker_map_struct_find_free_slot_near (lw6sys_context_t *
						    sys_context, const _lw6ker_map_struct_t * map_struct, lw6sys_xyz_t * there, lw6sys_xyz_t here);
extern int _lw6ker_map_struct_sanity_check (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct);


/* ker-move.c */
extern int _lw6ker_move_is_slot_free (const _lw6ker_map_struct_t * map_struct, const _lw6ker_map_state_t * map_state, int32_t x, int32_t y, int32_t z);
extern int _lw6ker_move_is_enemy_there (const _lw6ker_map_struct_t *
					map_struct,
					const _lw6ker_map_state_t * map_state,
					int32_t team_color, int32_t x, int32_t y, int32_t z, int32_t * enemy_id, int32_t * enemy_color);
extern int _lw6ker_move_is_ally_there (const _lw6ker_map_struct_t *
				       map_struct, const _lw6ker_map_state_t * map_state, int32_t team_color, int32_t x, int32_t y, int32_t z);
extern int32_t _lw6ker_move_find_straight_dir (int from_x, int from_y, lw6sys_xyz_t to, int parity);
extern int32_t _lw6ker_move_find_best_dir (const _lw6ker_map_state_t * map_state, const lw6ker_fighter_t * fighter, int parity);
extern void _lw6ker_move_goto_with_dir_xy (const lw6map_rules_t * rules,
					   const lw6sys_whd_t * shape, int32_t * dst_x, int32_t * dst_y, int32_t src_x, int32_t src_y, int32_t move_dir);
extern void _lw6ker_move_goto_with_dir_z (const lw6map_rules_t * rules, const lw6sys_whd_t * shape, int32_t * dst_z, int32_t src_z, int32_t move_dir);
extern void _lw6ker_move_goto_with_dir (const lw6map_rules_t * rules,
					const lw6sys_whd_t * shape,
					int32_t * dst_x, int32_t * dst_y, int32_t * dst_z, int32_t src_x, int32_t src_y, int32_t src_z, int32_t move_dir);
extern void _lw6ker_move_adjust_health (lw6ker_fighter_t * fighter, int32_t health_correction);
extern void _lw6ker_move_update_fighters_universal (lw6sys_context_t * sys_context, _lw6ker_move_context_t * context);
extern int _lw6ker_move_get_best_next_pos (lw6sys_context_t * sys_context,
					   const _lw6ker_game_state_t * game_state, lw6sys_xyz_t * next_pos, lw6sys_xyz_t * current_pos, int team_color);


/* ker-placestruct.c */
extern void _lw6ker_place_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_place_struct_t * place_struct, u_int32_t * checksum);
static inline int
_lw6ker_map_struct_place_index (const _lw6ker_map_struct_t * map_struct, int32_t x, int32_t y)
{
  return (map_struct->shape.w * y + x);
}

/* ker-score.c */
extern int _lw6ker_score_sort_quantity_callback (lw6ker_score_t * score_a, lw6ker_score_t * score_b);
extern int _lw6ker_score_sort_quantity_callback_desc (lw6ker_score_t * score_a, lw6ker_score_t * score_b);
extern int _lw6ker_score_sort_frags_callback (lw6ker_score_t * score_a, lw6ker_score_t * score_b);
extern int _lw6ker_score_sort_frags_callback_desc (lw6ker_score_t * score_a, lw6ker_score_t * score_b);

/* ker-scorearray.c */
extern int _lw6ker_score_array_update (lw6sys_context_t * sys_context, lw6ker_score_array_t * score_array, const _lw6ker_game_state_t * game_state);

/* ker-node.c */
extern void _lw6ker_node_reset (lw6sys_context_t * sys_context, _lw6ker_node_t * node);
extern void _lw6ker_node_init (lw6sys_context_t * sys_context, _lw6ker_node_t * node);
extern void _lw6ker_node_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_node_t * node, u_int32_t * checksum);
extern int _lw6ker_node_enable (lw6sys_context_t * sys_context, _lw6ker_node_t * node, u_int64_t node_id);
extern int _lw6ker_node_disable (lw6sys_context_t * sys_context, _lw6ker_node_t * node);
extern int _lw6ker_node_sanity_check (lw6sys_context_t * sys_context, const _lw6ker_node_t * node, const lw6map_rules_t * rules);

/* ker-nodearray.c */
extern void _lw6ker_node_array_reset (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array);
extern void _lw6ker_node_array_init (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array);
extern void _lw6ker_node_array_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_node_array_t * node_array, u_int32_t * checksum);
extern _lw6ker_node_t *_lw6ker_node_array_find_free (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array);
extern _lw6ker_node_t *_lw6ker_node_array_get_rw (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array, u_int64_t node_id);
extern const _lw6ker_node_t *_lw6ker_node_array_get_ro (lw6sys_context_t * sys_context, const _lw6ker_node_array_t * node_array, u_int64_t node_id);
extern int _lw6ker_node_array_enable (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array, u_int64_t node_id);
extern int _lw6ker_node_array_disable (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array, u_int64_t node_id);
extern int _lw6ker_node_array_sanity_check (lw6sys_context_t * sys_context, const _lw6ker_node_array_t * node_array, const lw6map_rules_t * rules);


/* ker-slotstruct.c */
extern void _lw6ker_slot_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_slot_struct_t * slot_struct, u_int32_t * checksum);

/* ker-team.c */
extern int _lw6ker_team_init (lw6sys_context_t * sys_context, _lw6ker_team_t * team, const _lw6ker_map_struct_t * map_struct, const lw6map_rules_t * rules);
extern void _lw6ker_team_clear (lw6sys_context_t * sys_context, _lw6ker_team_t * team);
extern int _lw6ker_team_sync (lw6sys_context_t * sys_context, _lw6ker_team_t * dst, const _lw6ker_team_t * src);
extern void _lw6ker_team_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_team_t * team, u_int32_t * checksum);
extern void _lw6ker_team_activate (lw6sys_context_t * sys_context, _lw6ker_team_t * team, lw6sys_xyz_t pos);
extern void _lw6ker_team_unactivate (lw6sys_context_t * sys_context, _lw6ker_team_t * team);
extern void _lw6ker_team_normalize_pot (lw6sys_context_t * sys_context, _lw6ker_team_t * team, const lw6map_rules_t * rules);
extern int _lw6ker_team_get_charge_per1000 (lw6sys_context_t * sys_context, const _lw6ker_team_t * team);
extern void _lw6ker_team_reset_charge (lw6sys_context_t * sys_context, _lw6ker_team_t * team);
extern int _lw6ker_team_is_this_weapon_active (lw6sys_context_t * sys_context, const _lw6ker_team_t * team, int round, int weapon_id);
extern int _lw6ker_team_get_weapon_per1000_left (lw6sys_context_t * sys_context, const _lw6ker_team_t * team, int round);

/* ker-slotstate.c */
extern void _lw6ker_slot_state_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_slot_state_t * slot_state, u_int32_t * checksum);

/* ker-slotstruct.c */
extern void _lw6ker_slot_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_slot_struct_t * slot_struct, u_int32_t * checksum);

/* ker-spread.c */
extern int32_t _lw6ker_spread_next_dir (int32_t dir);
extern void _lw6ker_spread_do_all_dirs_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones);
extern void _lw6ker_spread_do_all_dirs_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones);
extern void _lw6ker_spread_do_half_dirs_a_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones);
extern void _lw6ker_spread_do_half_dirs_b_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones);
extern void _lw6ker_spread_do_half_dirs_a_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones);
extern void _lw6ker_spread_do_half_dirs_b_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones);
extern void _lw6ker_spread_do_one_dir_incr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir);
extern void _lw6ker_spread_do_one_dir_incr_z (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir);
extern void _lw6ker_spread_do_one_dir_decr_xy (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir);
extern void _lw6ker_spread_do_one_dir_decr_z (_lw6ker_zone_struct_t * zone_structs, _lw6ker_zone_state_t * zone_states, int nb_zones, int dir);

extern void _lw6ker_spread_update_gradient (lw6sys_context_t * sys_context, _lw6ker_team_t * team, int skip_vertical, int spread_mode);

/* ker-weapon.c */
extern void _lw6ker_weapon_unset_by_weapon_id (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state, int weapon_id);
extern void _lw6ker_weapon_unset_all (lw6sys_context_t * sys_context, _lw6ker_map_state_t * map_state);
extern int _lw6ker_weapon_find_team_by_weapon_id (lw6sys_context_t * sys_context, const _lw6ker_map_state_t * map_state, int round, int weapon_id);
extern int _lw6ker_weapon_get_latest_weapon (lw6sys_context_t * sys_context,
					     const _lw6ker_map_state_t * map_state, int round, int *team_color, int *weapon_id, int *per1000_left);
extern int _lw6ker_weapon_fire (lw6sys_context_t * sys_context,
				_lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire2 (lw6sys_context_t * sys_context,
				 _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_berzerk (lw6sys_context_t * sys_context,
					_lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_invincible (lw6sys_context_t * sys_context,
					   _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_escape (lw6sys_context_t * sys_context,
				       _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_turbo (lw6sys_context_t * sys_context,
				      _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_teleport (lw6sys_context_t * sys_context,
					 _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_scatter (lw6sys_context_t * sys_context,
					_lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_fix (lw6sys_context_t * sys_context,
				    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_mix (lw6sys_context_t * sys_context,
				    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_control (lw6sys_context_t * sys_context,
					_lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_permutation (lw6sys_context_t * sys_context,
					    _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_steal (lw6sys_context_t * sys_context,
				      _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_atomic (lw6sys_context_t * sys_context,
				       _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_reverse (lw6sys_context_t * sys_context,
					_lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_crazy (lw6sys_context_t * sys_context,
				      _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_rewind (lw6sys_context_t * sys_context,
				       _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_attract (lw6sys_context_t * sys_context,
					_lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_shrink (lw6sys_context_t * sys_context,
				       _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_kamikaze (lw6sys_context_t * sys_context,
					 _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_disappear (lw6sys_context_t * sys_context,
					  _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);
extern int _lw6ker_weapon_fire_plague (lw6sys_context_t * sys_context,
				       _lw6ker_map_state_t * map_state, const lw6map_rules_t * rules, int round, int team_color, int charge_percent);

/* ker-zonestate.c */
extern void _lw6ker_zone_state_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_zone_state_t * zone_state, u_int32_t * checksum);

/* ker-zonestruct.c */
extern void _lw6ker_zone_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_zone_struct_t * zone_struct, u_int32_t * checksum);

#endif // LIQUIDWAR6KER_INTERNAL_H
