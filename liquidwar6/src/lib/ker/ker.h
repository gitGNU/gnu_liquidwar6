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

#ifndef LIQUIDWAR6KER_H
#define LIQUIDWAR6KER_H

#include "../sys/sys.h"
#include "../map/map.h"

#define LW6KER_NB_DIRS 14

#define LW6KER_DIR_NNE 0
#define LW6KER_DIR_NE 1
#define LW6KER_DIR_ENE 2
#define LW6KER_DIR_ESE 3
#define LW6KER_DIR_SE 4
#define LW6KER_DIR_SSE 5
#define LW6KER_DIR_SSW 6
#define LW6KER_DIR_SW 7
#define LW6KER_DIR_WSW 8
#define LW6KER_DIR_WNW 9
#define LW6KER_DIR_NW 10
#define LW6KER_DIR_NNW 11
#define LW6KER_DIR_UP 12
#define LW6KER_DIR_DOWN 13

#define LW6KER_NB_PARITIES 2

#define LW6KER_PARITY_EVEN 0
#define LW6KER_PARITY_ODD 1

#define LW6KER_TEAM_MASK_RED 0x001
#define LW6KER_TEAM_MASK_GREEN 0x002
#define LW6KER_TEAM_MASK_BLUE 0x004
#define LW6KER_TEAM_MASK_YELLOW 0x08
#define LW6KER_TEAM_MASK_CYAN 0x10
#define LW6KER_TEAM_MASK_MAGENTA 0x20
#define LW6KER_TEAM_MASK_ORANGE 0x40
#define LW6KER_TEAM_MASK_LIGHTBLUE 0x80
#define LW6KER_TEAM_MASK_PURPLE 0x100
#define LW6KER_TEAM_MASK_PINK 0x200
#define LW6KER_TEAM_MASK_ALL 0x3ff

#define LW6KER_HISTORY_SIZE 512

#define LW6KER_ACT_LIMIT 100

#include "ker-trigo.h"

typedef struct lw6ker_zone_struct_s
{
  lw6sys_xyz_t pos;
  u_int32_t size;
  int32_t link[LW6KER_NB_DIRS];
  int16_t one_way_dir_odd;
  int16_t one_way_dir_even;
}
lw6ker_zone_struct_t;

typedef struct lw6ker_slot_struct_s
{
  int32_t zone_id;
}
lw6ker_slot_struct_t;

typedef struct lw6ker_place_struct_s
{
  int32_t act_incr:12;
  int32_t pad:4;		// free for later use
  int32_t health_correction:16;
}
lw6ker_place_struct_t;

/*
 * To some extent, this structure is very similar to lw6map_level_t.
 * Let's explain why it exists separately: 
 * - this structure is not only used to store the information, but
 *   also to present it in a form which is adapted to (fast) treatments.
 *   This means it can be redundant, akward, whereas lw6map_level_t is meant
 *   to just convey informations. lw6ker_map_struct_t is used when the
 *   game is running, to inform the core algorithm of tha map structure,
 *   whereas lw6map_level_t is used to intialize lw6ker_map_struct_t, to send
 *   the maps structure to other players over the network, to initialize
 *   the graphics artifacts which will be used for rendering, and so on.
 * - having a loading/serialization (that is lw6map_level_t) separated from
 *   algorithmic wizardry related to shortest path insanities might be
 *   what could prevent LW6 from being as bloated as LW5.
 */
typedef struct lw6ker_map_struct_s
{
  lw6sys_whd_t shape;
  int32_t nb_places;
  int32_t nb_zones;
  int32_t nb_slots;
  int32_t nb_usable_slots;
  int32_t room_for_armies;
  int32_t max_zone_size;
  lw6ker_place_struct_t *places;
  lw6ker_zone_struct_t *zones;
  lw6ker_slot_struct_t *slots;
}
lw6ker_map_struct_t;

/*
 * Game struct contains all the data which never change during a game.
 * If it was feasible in C, we would change it to "read-only" when game
 * starts.
 */
typedef struct lw6ker_game_struct_s
{
  u_int32_t id;
  lw6map_level_t *level;
  lw6ker_map_struct_t map_struct;
  lw6map_rules_t rules;
}
lw6ker_game_struct_t;

/*
 * Completes lw6ker_zone_struct_t with per-team information.
 */
typedef struct lw6ker_zone_state_s
{
  int32_t potential:24;
  int32_t direction_to_cursor:8;
  lw6sys_xyz_t closest_cursor_pos;
}
lw6ker_zone_state_t;

typedef struct lw6ker_slot_state_s
{
  int32_t fighter_id;
}
lw6ker_slot_state_t;

typedef struct lw6ker_fighter_s
{
  u_int32_t team_color:8;
  int32_t last_direction:8;
  int32_t health:16;
  int32_t act_counter:16;
  int32_t pad:16;		// free for later use
  lw6sys_xyz_t pos;
}
lw6ker_fighter_t;

typedef struct lw6ker_cursor_s
{
  u_int64_t server_id;
  u_int16_t cursor_id;
  char letter;
  int enabled;
  int team_color;
  lw6sys_xyz_t pos;
  lw6sys_xyz_t apply_pos;
  int32_t pot_offset;
}
lw6ker_cursor_t;

typedef struct lw6ker_cursor_array_s
{
  int32_t nb_cursors;
  lw6ker_cursor_t cursors[LW6MAP_MAX_NB_CURSORS];
}
lw6ker_cursor_array_t;

typedef struct lw6ker_score_s
{
  int team_color;
  int fighters_percent;
  int fighters_absolute;
  float fighters_ratio;		// one of the rare float in lw6ker, only for eye candy 
  int frags;
}
lw6ker_score_t;

typedef struct lw6ker_score_array_s
{
  int active_fighters;
  int nb_scores;
  lw6ker_score_t scores[LW6MAP_NB_TEAM_COLORS];
}
lw6ker_score_array_t;

typedef struct lw6ker_history_s
{
  int nb_entries;
  int nb_fighters[LW6KER_HISTORY_SIZE][LW6MAP_MAX_NB_TEAMS];
} lw6ker_history_t;

typedef struct lw6ker_server_s
{
  u_int64_t server_id;
  int enabled;
  u_int32_t last_command_round;
}
lw6ker_server_t;

typedef struct lw6ker_server_array_s
{
  int32_t nb_servers;
  lw6ker_server_t servers[LW6MAP_MAX_NB_SERVERS];
}
lw6ker_server_array_t;

typedef struct lw6ker_team_s
{
  int active;
  lw6ker_map_struct_t *map_struct;
  lw6ker_zone_state_t *gradient;
  int32_t cursor_ref_pot;
  int32_t last_spread_dir;
}
lw6ker_team_t;

typedef struct lw6ker_armies_s
{
  lw6ker_map_struct_t *map_struct;
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
lw6ker_armies_t;

typedef struct lw6ker_map_state_s
{
  lw6ker_map_struct_t *map_struct;
  lw6sys_whd_t shape;		// redundant but convenient
  int32_t shape_surface;
  lw6ker_armies_t armies;
  int32_t max_nb_teams;
  lw6ker_team_t teams[LW6MAP_MAX_NB_TEAMS];
  lw6ker_cursor_array_t cursor_array;
  int32_t nb_slots;		// redundant but convenient
  lw6ker_slot_state_t *slots;
}
lw6ker_map_state_t;

/*
 * Game state contains all the data which is verstatile, stuff that
 * changes, this includes "where fighters are" of course but also
 * some parameters which might change in-game (such as an option
 * which can be changeable, an alliance between teams, and so on...)
 */
typedef struct lw6ker_game_state_s
{
  u_int32_t id;
  lw6ker_game_struct_t *game_struct;
  lw6ker_map_state_t map_state;
  lw6ker_server_array_t server_array;
  lw6ker_history_t global_history;
  lw6ker_history_t latest_history;
  u_int32_t moves;
  u_int32_t spreads;
  u_int32_t rounds;
  int max_reached_teams;
  int over;
}
lw6ker_game_state_t;

/*
 * in armies.c
 */
extern int32_t lw6ker_armies_add_fighter (lw6ker_armies_t * armies,
					  lw6ker_fighter_t fighter);
extern int lw6ker_armies_remove_fighter (lw6ker_armies_t * armies);

/*
 * in capture.c
 */
extern char *lw6ker_capture_str (lw6ker_game_state_t * game_state);

/*
 * in cursor.c
 */
extern int lw6ker_cursor_enable (lw6ker_cursor_t * cursor,
				 u_int64_t server_id,
				 u_int16_t cursor_id,
				 int team_color, int32_t x, int32_t y);
extern int lw6ker_cursor_disable (lw6ker_cursor_t * cursor);
extern int lw6ker_cursor_update (lw6ker_cursor_t * cursor, int32_t x,
				 int32_t y, int32_t pot_offset,
				 lw6sys_whd_t * shape,
				 lw6map_rules_t * rules);
extern int lw6ker_cursor_sanity_check (lw6ker_cursor_t * cursor,
				       lw6sys_whd_t * shape,
				       lw6map_rules_t * rules);

/*
 * in cursorarray.c
 */
extern lw6ker_cursor_t *lw6ker_cursor_array_get (lw6ker_cursor_array_t *
						 cursor_array,
						 u_int16_t cursor_id);
extern int lw6ker_cursor_array_enable (lw6ker_cursor_array_t * cursor_array,
				       u_int64_t server_id,
				       u_int16_t cursor_id, int team_color,
				       int32_t x, int32_t y);
extern int lw6ker_cursor_array_disable (lw6ker_cursor_array_t * cursor_array,
					u_int64_t server_id,
					u_int16_t cursor_id);
extern int lw6ker_cursor_array_update (lw6ker_cursor_array_t * cursor_array,
				       u_int64_t server_id,
				       u_int16_t cursor_id, int32_t x,
				       int32_t y, int32_t pot_offset,
				       lw6sys_whd_t * shape,
				       lw6map_rules_t * rules);
extern int lw6ker_cursor_array_sanity_check (lw6ker_cursor_array_t *
					     cursor_array,
					     lw6sys_whd_t * shape,
					     lw6map_rules_t * rules);

/*
 * in gamestate.c
 */
extern lw6ker_game_state_t *lw6ker_game_state_new (lw6ker_game_struct_t *
						   game_struct,
						   lw6sys_progress_t *
						   progress);
extern void lw6ker_game_state_free (lw6ker_game_state_t * game_state);
extern void lw6ker_game_state_point_to (lw6ker_game_state_t * game_state,
					lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_state_memory_footprint (lw6ker_game_state_t *
					       game_state);
extern char *lw6ker_game_state_repr (lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_can_sync (lw6ker_game_state_t * dst,
				       lw6ker_game_state_t * src);
extern int lw6ker_game_state_sync (lw6ker_game_state_t * dst,
				   lw6ker_game_state_t * src);
extern lw6ker_game_state_t *lw6ker_game_state_dup (lw6ker_game_state_t *
						   game_state,
						   lw6sys_progress_t *
						   progress);
extern u_int32_t lw6ker_game_state_checksum (lw6ker_game_state_t *
					     game_state);
extern int lw6ker_game_state_register_server (lw6ker_game_state_t *
					      game_state,
					      u_int64_t server_id);
extern int lw6ker_game_state_unregister_server (lw6ker_game_state_t *
						game_state,
						u_int64_t server_id);
extern int lw6ker_game_state_server_exists (lw6ker_game_state_t * game_state,
					    u_int64_t server_id);
extern int lw6ker_game_state_get_server_info (lw6ker_game_state_t *
					      game_state, u_int16_t server_id,
					      u_int32_t * last_command_round);
extern int lw6ker_game_state_add_cursor (lw6ker_game_state_t * game_state,
					 u_int64_t server_id,
					 u_int16_t cursor_id, int team_color);
extern int lw6ker_game_state_remove_cursor (lw6ker_game_state_t * game_state,
					    u_int64_t server_id,
					    u_int16_t cursor_id);
extern int lw6ker_game_state_cursor_exists (lw6ker_game_state_t * game_state,
					    u_int16_t cursor_id);
extern int lw6ker_game_state_get_cursor_info (lw6ker_game_state_t *
					      game_state, u_int16_t cursor_id,
					      u_int64_t * server_id,
					      char *letter, int *team_color,
					      int32_t * x, int32_t * y);
extern int lw6ker_game_state_set_cursor (lw6ker_game_state_t * game_state,
					 u_int64_t server_id,
					 u_int16_t cursor_id, int32_t x,
					 int32_t y);
extern int lw6ker_game_state_add_team (lw6ker_game_state_t * game_state,
				       u_int64_t server_id, int team_color);
extern int lw6ker_game_state_remove_team (lw6ker_game_state_t * game_state,
					  u_int64_t server_id,
					  int team_color);
extern int lw6ker_game_state_team_exists (lw6ker_game_state_t * game_state,
					  int team_color);
extern int lw6ker_game_state_get_team_info (lw6ker_game_state_t * game_state,
					    int team_color,
					    int32_t * nb_cursors,
					    int32_t * nb_fighters);
extern int lw6ker_game_state_get_nb_teams (lw6ker_game_state_t * game_state);
extern void lw6ker_game_state_do_spread (lw6ker_game_state_t *
					 game_state, u_int32_t team_mask);
extern void lw6ker_game_state_do_move (lw6ker_game_state_t *
				       game_state, u_int32_t team_mask);
extern void lw6ker_game_state_finish_round (lw6ker_game_state_t * game_state);
extern void lw6ker_game_state_do_round (lw6ker_game_state_t * game_state);
extern u_int32_t lw6ker_game_state_get_moves (lw6ker_game_state_t *
					      game_state);
extern u_int32_t lw6ker_game_state_get_spreads (lw6ker_game_state_t *
						game_state);
extern u_int32_t lw6ker_game_state_get_rounds (lw6ker_game_state_t *
					       game_state);
extern u_int32_t lw6ker_game_state_get_total_rounds (lw6ker_game_state_t *
						     game_state);
extern int lw6ker_game_state_is_over (lw6ker_game_state_t * game_state);
extern int32_t lw6ker_game_state_get_nb_active_fighters (lw6ker_game_state_t *
							 game_state);
extern int32_t lw6ker_game_state_get_time_elapsed (lw6ker_game_state_t *
						   game_state);
extern int32_t lw6ker_game_state_get_time_left (lw6ker_game_state_t *
						game_state);
extern int32_t lw6ker_game_state_get_global_history (lw6ker_game_state_t *
						     game_state, int i,
						     int team_id);
extern int32_t lw6ker_game_state_get_latest_history (lw6ker_game_state_t *
						     game_state, int i,
						     int team_id);
extern int32_t lw6ker_game_state_get_global_history_max (lw6ker_game_state_t *
							 game_state);
extern int32_t lw6ker_game_state_get_latest_history_max (lw6ker_game_state_t *
							 game_state);

/*
 * In gamestruct.c
 */
extern lw6ker_game_struct_t *lw6ker_game_struct_new (lw6map_level_t * level,
						     lw6sys_progress_t *
						     progress);
extern void lw6ker_game_struct_free (lw6ker_game_struct_t * game_struct);
extern void lw6ker_game_struct_point_to (lw6ker_game_struct_t * game_struct,
					 lw6map_level_t * level);
extern int lw6ker_game_struct_memory_footprint (lw6ker_game_struct_t *
						game_struct);
extern char *lw6ker_game_struct_repr (lw6ker_game_struct_t * game_struct);
extern lw6ker_game_struct_t *lw6ker_game_struct_dup (lw6ker_game_struct_t *
						     game_struct,
						     lw6sys_progress_t *
						     progress);
extern u_int32_t lw6ker_game_struct_checksum (lw6ker_game_struct_t *
					      game_struct);


/*
 * in mapstate.c
 */
static inline int
lw6ker_map_state_slot_index (lw6ker_map_state_t * map_state, int32_t x,
			     int32_t y, int32_t z)
{
  return (map_state->shape_surface * z + map_state->shape.w * y + x);
}

extern int lw6ker_map_state_get_free_team_color (lw6ker_map_state_t *
						 map_state);
extern int32_t lw6ker_map_state_populate_team (lw6ker_map_state_t * map_state,
					       int32_t team_color,
					       int32_t nb_fighters,
					       lw6sys_xyz_t desired_center,
					       lw6map_rules_t * rules);
extern int lw6ker_map_state_redistribute_team (lw6ker_map_state_t * map_state,
					       int32_t dst_team_color,
					       int32_t src_team_color,
					       int32_t nb_fighters,
					       lw6map_rules_t * rules);
extern int lw6ker_map_state_cancel_team (lw6ker_map_state_t * map_state,
					 int32_t team_color);
extern int lw6ker_map_state_remove_fighter (lw6ker_map_state_t * map_state,
					    int32_t fighter_id);
extern int lw6ker_map_state_remove_fighters (lw6ker_map_state_t * map_state,
					     int32_t nb_fighters);
extern int lw6ker_map_state_remove_team_fighters (lw6ker_map_state_t *
						  map_state,
						  int32_t team_color,
						  int32_t nb_fighters);
static inline void
lw6ker_map_state_set_fighter_id (lw6ker_map_state_t * map_state,
				 int32_t x,
				 int32_t y, int32_t z, int32_t fighter_id)
{
  map_state->slots[lw6ker_map_state_slot_index (map_state, x, y, z)].
    fighter_id = fighter_id;
};

static inline int32_t
lw6ker_map_state_get_fighter_id (lw6ker_map_state_t * map_state,
				 int32_t x, int32_t y, int32_t z)
{
  return (map_state->slots[lw6ker_map_state_slot_index (map_state, x, y, z)].
	  fighter_id);
};

extern lw6ker_fighter_t *lw6ker_map_state_get_fighter_safe (lw6ker_map_state_t
							    * map_state,
							    int32_t x,
							    int32_t y,
							    int32_t z);
/*
 * Carefull with this one, there must really be a fighter
 * or it segfaults directly.
 */
static inline lw6ker_fighter_t *
lw6ker_map_state_get_fighter_unsafe (lw6ker_map_state_t * map_state,
				     int32_t x, int32_t y, int32_t z)
{
  return (&
	  (map_state->armies.fighters[map_state->slots
				      [lw6ker_map_state_slot_index
				       (map_state, x, y, z)].fighter_id]));
};

extern void lw6ker_map_state_print_debug (lw6ker_map_state_t * map_state);
extern int lw6ker_map_state_sanity_check (lw6ker_map_state_t * map_state);
extern void lw6ker_map_state_spread_gradient (lw6ker_map_state_t * map_state,
					      lw6map_rules_t * rules,
					      int32_t nb_spreads,
					      u_int32_t team_mask);
extern void lw6ker_map_state_move_fighters (lw6ker_map_state_t * map_state,
					    int parity,
					    lw6map_rules_t * rules,
					    int32_t nb_moves,
					    u_int32_t team_mask);
extern void lw6ker_map_state_apply_cursors (lw6ker_map_state_t * map_state,
					    lw6map_rules_t * rules,
					    u_int32_t team_mask);

/*
 * In mapstruct.c
 */
static inline int
lw6ker_map_struct_slot_index (lw6ker_map_struct_t * map_struct, int32_t x,
			      int32_t y, int32_t z)
{
  return (map_struct->nb_places * z + map_struct->shape.w * y + x);
}

static inline void
lw6ker_map_struct_set_zone_id (lw6ker_map_struct_t * map_struct,
			       int32_t x, int32_t y, int32_t z,
			       int32_t zone_id)
{
  map_struct->slots[lw6ker_map_struct_slot_index (map_struct, x, y, z)].
    zone_id = zone_id;
};

static inline int32_t
lw6ker_map_struct_get_zone_id (lw6ker_map_struct_t * map_struct,
			       int32_t x, int32_t y, int32_t z)
{
  return (map_struct->slots
	  [lw6ker_map_struct_slot_index (map_struct, x, y, z)].zone_id);
};

extern void lw6ker_map_struct_find_free_slot_near (lw6ker_map_struct_t *
						   map_struct,
						   lw6sys_xyz_t * there,
						   lw6sys_xyz_t here);
extern int lw6ker_map_struct_sanity_check (lw6ker_map_struct_t * map_struct);

/*
 * in placestruct.c
 */
static inline int
lw6ker_map_struct_place_index (lw6ker_map_struct_t * map_struct, int32_t x,
			       int32_t y)
{
  return (map_struct->shape.w * y + x);
}

/*
 * in scorearrqy.c
 */
extern int lw6ker_score_array_update (lw6ker_score_array_t * score_array,
				      lw6ker_game_state_t * game_state);

/*
 * in server.c
 */
extern int lw6ker_server_enable (lw6ker_server_t * server,
				 u_int64_t server_id);
extern int lw6ker_server_disable (lw6ker_server_t * server);
extern int lw6ker_server_sanity_check (lw6ker_server_t * server,
				       lw6map_rules_t * rules);

/*
 * in serverarray.c
 */
extern lw6ker_server_t *lw6ker_server_array_get (lw6ker_server_array_t *
						 server_array,
						 u_int64_t server_id);
extern int lw6ker_server_array_enable (lw6ker_server_array_t * server_array,
				       u_int64_t server_id);
extern int lw6ker_server_array_disable (lw6ker_server_array_t * server_array,
					u_int64_t server_id);
extern int lw6ker_server_array_sanity_check (lw6ker_server_array_t *
					     server_array,
					     lw6map_rules_t * rules);

/*
 * In team.c
 */
extern void lw6ker_team_activate (lw6ker_team_t * team, lw6sys_xyz_t pos);
extern void lw6ker_team_unactivate (lw6ker_team_t * team);
extern void lw6ker_team_normalize_pot (lw6ker_team_t * team,
				       lw6map_rules_t * rules);

/*
 * in teammask.c
 */
extern void lw6ker_team_mask_get (u_int32_t * even, u_int32_t * odd,
				  int32_t round);
extern void lw6ker_team_mask_best (u_int32_t * even, u_int32_t * odd,
				   lw6ker_game_state_t * game_state);
extern int lw6ker_team_mask_is_concerned (int team_color,
					  u_int32_t team_mask);

/* ker-test.c */
extern int lw6ker_test (int mode);

/*
 * In trigo.c
 */
static inline int32_t
lw6ker_sin (int32_t alpha)
{
  alpha = alpha & LW6KER_TRIGO_2PI_MASK;
  return LW6KER_TRIGO_SIN_TABLE[alpha];
}

static inline int32_t
lw6ker_cos (int32_t alpha)
{
  alpha = (LW6KER_TRIGO_PI2 + alpha) & LW6KER_TRIGO_2PI_MASK;
  return LW6KER_TRIGO_SIN_TABLE[alpha];
}

/*
 * Various utils
 */
static inline int32_t
lw6ker_percent (int32_t n, int32_t percent)
{
  return (int32_t) ((((int64_t) n) * ((int64_t) percent)) / 100LL);
}

static inline int32_t
lw6ker_per1000 (int32_t n, int32_t per1000)
{
  return (int32_t) ((((int64_t) n) * ((int64_t) per1000)) / 100LL);
}

static inline int32_t
lw6ker_per100_2 (int32_t n, int32_t per100)
{
  return (int32_t) ((((int64_t) n) * ((int64_t) per100) *
		     ((int64_t) per100)) / 10000LL);
}

#endif
