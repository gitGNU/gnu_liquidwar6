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

#ifndef LIQUIDWAR6KER_H
#define LIQUIDWAR6KER_H

#include "../sys/sys.h"
#include "../map/map.h"

#define LW6KER_NB_DIRS 14

/*
 * Historically, first dir (0) was NNE, but it happened
 * a minor optimization is to have ENE being 0, this allows
 * dirs 0-2, 3-5, 6-8 and 9-11 to be consider 4 packs of
 * alike dirs when spreading.
 */
#define LW6KER_DIR_NNE 10
#define LW6KER_DIR_NE 11
#define LW6KER_DIR_ENE 0
#define LW6KER_DIR_ESE 1
#define LW6KER_DIR_SE 2
#define LW6KER_DIR_SSE 3
#define LW6KER_DIR_SSW 4
#define LW6KER_DIR_SW 5
#define LW6KER_DIR_WSW 6
#define LW6KER_DIR_WNW 7
#define LW6KER_DIR_NW 8
#define LW6KER_DIR_NNW 9
#define LW6KER_DIR_DOWN 12
#define LW6KER_DIR_UP 13

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

#include "ker-trigo.h"

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

typedef struct lw6ker_cursor_control_s
{
  lw6sys_xyz_t pos;
  int fire;
  int fire2;
} lw6ker_cursor_control_t;

typedef struct lw6ker_cursor_s
{
  u_int64_t node_id;
  u_int16_t cursor_id;
  char letter;
  int enabled;
  int team_color;
  lw6sys_xyz_t pos;
  int fire;
  int fire2;
  lw6sys_xyz_t apply_pos;
  int32_t pot_offset;
}
lw6ker_cursor_t;

typedef struct lw6ker_score_s
{
  int team_color;
  int fighters_percent;
  int fighters_absolute;
  float fighters_ratio;		// one of the rare float in lw6ker, only for eye candy 
  int frags;
  int consolidated_percent;
}
lw6ker_score_t;

typedef struct lw6ker_score_array_s
{
  int active_fighters;
  int nb_scores;
  lw6ker_score_t scores[LW6MAP_NB_TEAM_COLORS];
}
lw6ker_score_array_t;

typedef struct lw6ker_game_struct_s
{
  /*
   * The 3 first members, id, level, rules are the same as the internal
   * _lw6ker_game_struct_t structure. The rest of it is hidden.
   * The program will cast from lw6ker_game_struct_t to _lw6ker_game_struct_t
   * internally.
   */
  u_int32_t id;
  lw6map_level_t *level;
  lw6map_rules_t rules;
}
lw6ker_game_struct_t;

typedef struct lw6ker_game_state_s
{
  /*
   * The 2 first members, id and game struct, are the same as the internal
   * _lw6ker_game_state_t structure. The rest of it is hidden.
   * The program will cast from lw6ker_game_state_t to _lw6ker_game_state_t
   * internally.
   */
  u_int32_t id;
  lw6ker_game_struct_t *game_struct;
}
lw6ker_game_state_t;

/* ker-capture.c */
extern char *lw6ker_capture_str (lw6ker_game_state_t * game_state);

/* ker-cursor.c */
extern void lw6ker_cursor_reset (lw6ker_cursor_t * cursor);

/* ker-gamestate.c */
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
extern void lw6ker_game_state_get_shape (lw6ker_game_state_t * game_state,
					 lw6sys_whd_t * shape);
extern int lw6ker_game_state_get_w (lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_get_h (lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_get_d (lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_register_node (lw6ker_game_state_t *
					    game_state, u_int64_t node_id);
extern int lw6ker_game_state_unregister_node (lw6ker_game_state_t *
					      game_state, u_int64_t node_id);
extern int lw6ker_game_state_node_exists (lw6ker_game_state_t * game_state,
					  u_int64_t node_id);
extern int lw6ker_game_state_get_node_info (lw6ker_game_state_t *
					    game_state, u_int16_t node_id,
					    u_int32_t * last_command_round);
extern int lw6ker_game_state_add_cursor (lw6ker_game_state_t * game_state,
					 u_int64_t node_id,
					 u_int16_t cursor_id, int team_color);
extern int lw6ker_game_state_remove_cursor (lw6ker_game_state_t * game_state,
					    u_int64_t node_id,
					    u_int16_t cursor_id);
extern int lw6ker_game_state_cursor_exists (lw6ker_game_state_t * game_state,
					    u_int16_t cursor_id);
extern int lw6ker_game_state_get_cursor (lw6ker_game_state_t *
					 game_state, lw6ker_cursor_t * cursor,
					 u_int16_t cursor_id);
extern void lw6ker_game_state_get_cursor_by_index (lw6ker_game_state_t *
						   game_state,
						   lw6ker_cursor_t * cursor,
						   int i);
extern int lw6ker_game_state_set_cursor (lw6ker_game_state_t * game_state,
					 lw6ker_cursor_t * cursor);
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
extern int lw6ker_game_state_did_cursor_win (lw6ker_game_state_t * game_state,
					     u_int16_t cursor_id);
extern int lw6ker_game_state_get_winner (lw6ker_game_state_t * game_state,
					 int excluded_team);
extern int lw6ker_game_state_get_looser (lw6ker_game_state_t * game_state,
					 int excluded_team);
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
extern int32_t lw6ker_game_state_get_fighter_id (lw6ker_game_state_t *
						 game_state, int32_t x,
						 int32_t y, int32_t z);
extern lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_by_id (lw6ker_game_state_t * game_state,
					 int32_t fighter_id);
extern lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_safe (lw6ker_game_state_t * game_state,
					int32_t x, int32_t y, int32_t z);
extern lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_unsafe (lw6ker_game_state_t * game_state,
					  int32_t x, int32_t y, int32_t z);
extern int lw6ker_game_state_get_zone_potential (lw6ker_game_state_t *
						 game_state, int zone_i,
						 int team_id);
extern int lw6ker_game_state_get_charge_per1000 (lw6ker_game_state_t *
						 game_state, int team_color);
extern int lw6ker_game_state_get_weapon_per1000_left (lw6ker_game_state_t *
						      game_state,
						      int team_color);
extern int lw6ker_game_state_get_latest_weapon (lw6ker_game_state_t *
						game_state, int *team_color,
						int *weapon_id,
						int *per1000_left);

/* ker-gamestruct.c */
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
extern void lw6ker_game_struct_get_shape (lw6ker_game_struct_t * game_struct,
					  lw6sys_whd_t * shape);
extern int lw6ker_game_struct_get_w (lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_struct_get_h (lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_struct_get_d (lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_struct_is_fg (lw6ker_game_struct_t * game_struct,
				     int32_t x, int32_t y, int32_t z);
extern int lw6ker_game_struct_is_bg (lw6ker_game_struct_t * game_struct,
				     int32_t x, int32_t y, int32_t z);
extern void lw6ker_game_struct_get_zones_info (lw6ker_game_struct_t *
					       game_struct, int *nb_zones,
					       int *max_zone_size);
extern void lw6ker_game_struct_get_zone_info (lw6ker_game_struct_t *
					      game_struct, int i,
					      lw6sys_xyz_t * zone_pos,
					      int *zone_size);
extern int32_t lw6ker_game_struct_get_zone_id (lw6ker_game_struct_t *
					       game_struct, int32_t x,
					       int32_t y, int32_t z);
extern void lw6ker_game_struct_find_free_slot_near (lw6ker_game_struct_t *
						    game_struct,
						    lw6sys_xyz_t * there,
						    lw6sys_xyz_t here);

/* ker-move.c */
extern int lw6ker_move_get_best_next_pos (lw6ker_game_state_t * game_state,
					  lw6sys_xyz_t * next_pos,
					  lw6sys_xyz_t * current_pos,
					  int team_color);
/* ker-scorearray.c */
extern int lw6ker_score_array_update (lw6ker_score_array_t * score_array,
				      lw6ker_game_state_t * game_state);

/* ker-teammask.c */
extern void lw6ker_team_mask_get (u_int32_t * even, u_int32_t * odd,
				  int32_t round);
extern void lw6ker_team_mask_best (u_int32_t * even, u_int32_t * odd,
				   lw6ker_game_state_t * game_state);
extern int lw6ker_team_mask_is_concerned (int team_color,
					  u_int32_t team_mask);
extern int lw6ker_team_mask_color2mask (int team_color);

/* ker-test.c */
extern int lw6ker_test (int mode);

/* ker-trigo.c */
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

// utils
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
