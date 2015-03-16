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

/**
 * Contains the parameters of a fighter, one of those
 * little squares that are that at the very heart of
 * Liquid War.
 */
typedef struct lw6ker_fighter_s
{
  /// Team color from 0 to 9, -1 if invalid.
  u_int32_t team_color:8;
  /**
   * Last direction this fighter used, this is important
   * for in some cases we want to know where the fighter
   * was heading before, our current choice might rely
   * on previous state.
   */
  int32_t last_direction:8;
  /// Fighter health from 0 to 10000.
  int32_t health:16;
  /**
   * This counter is used to handle speed up/slow down.
   * At each round it's incremented, whenever it reaches 100
   * then the fighter really acts. Basically, there's a Bresenham
   * algorithm behind all that.
   */
  int32_t act_counter:16;
  /// Free for later use.
  int32_t pad:16;
  /// Fighter position.
  lw6sys_xyz_t pos;
}
lw6ker_fighter_t;

/**
 * Contains a cursor controls state, basically a cursor is
 * a position plus a fire and fire2 booleans.
 */
typedef struct lw6ker_cursor_control_s
{
  /// Cursor position, z isn't relevant for now.
  lw6sys_xyz_t pos;
  /// Fire, 1 if primary weapon must be used.
  int fire;
  /// Fire2, 1 if secondary weapon must be used.
  int fire2;
} lw6ker_cursor_control_t;

/**
 * Data about a given cursor.
 */
typedef struct lw6ker_cursor_s
{
  /// The id of the node this cursor belongs to.
  u_int64_t node_id;
  /// The id of this cursor.
  u_int16_t cursor_id;
  /// ASCII code of the letter associated to the cursor.
  char letter;
  /// Wether the cursor is enabled/active or not.
  int enabled;
  /// Team associated with this cursor.
  int team_color;
  /// Cursor position, z isn't relevant for now.
  lw6sys_xyz_t pos;
  /// Primary fire button state
  int fire;
  /// Alternate fire button state.
  int fire2;
  /**
   * Position to apply cursor on. Problem follows: cursor
   * might be hanging on a wall, somewhere fighters can't go.
   * In that case an alternate position is find, usually
   * the closest free space. But this can take time to calculate
   * so we cache this value here, as it is convenient to
   * have it at hand.
   */
  lw6sys_xyz_t apply_pos;
  /**
   * Potential offset. Whenever the cursor is applied to some
   * place, one can add a potential to it, this can be used
   * to make some cursor more attractive than others.
   */
  int32_t pot_offset;
}
lw6ker_cursor_t;

/**
 * Stores the score information about a team, this structure
 * is used to get informations from the game, and display them.
 */
typedef struct lw6ker_score_s
{
  /**
   * Wether this team (this color) has been active at some
   * point during the game. This is important for at score
   * time, many teams might have been disabled, this is
   * typical of dead teams in the LW5 last player wins scheme.
   * It can also happen in network games after a team leaves.
   * Note that this way of counting active teams does not
   * allow fine grain knowledge of who played, for the yellow
   * team might have been played by different nodes through
   * a single game session.
   */
  int has_been_active;
  /// The color of the team this score is about.
  int team_color;
  /**
   * Percentage of fighters for this team. The global score
   * array object will take care of making the sum of
   * fighters_percent be exactly 100, regardless of exactitude,
   * it will round this number to make a nice total and
   * hide rounding errors.
   */
  int fighters_percent;
  /// Absolute number of fighters for this team.
  int fighters_absolute;
  /**
   * One of the rare float in lw6ker, only for eye candy, this
   * is the equivalent of fighters_percent but as a float between
   * 0.0f and 1.0f. It gives the possibility of more precise
   * graphical displays, will its integer companion value is usefull
   * for writing down scores.
   */
  float fighters_ratio;
  /**
   * Number of frags. Note that depending on game rules, this
   * can have very different meanings.
   */
  int frags;
  /**
   * OK, this is probably the most non-intuitive number but
   * still the most usefull. It will show a percentage that is
   * greater as we estimate the team in a stronger position.
   * For instance, it can be higher if the team has very few
   * fighters on the field but has a great number of frags.
   * The one who has the greatest number here is the winner.
   */
  int consolidated_percent;
}
lw6ker_score_t;

/**
 * This is an array which contains all scores for all teams,
 * it's calculated from game_state and should be used read
 * only by code which is not within lw6ker.
 */
typedef struct lw6ker_score_array_s
{
  /// Number of fighters on the battlefield.
  int active_fighters;
  /**
   * Number of score items. This can be greater than the
   * number of active teams, since it retains informations
   * about teams which have disappeared.
   */
  int nb_scores;
  /**
   * Scores for each team, they are sorted, the first
   * one with index 0 is the current winner, then all
   * other teams follow, the last one being Mr Looser.
   */
  lw6ker_score_t scores[LW6MAP_NB_TEAM_COLORS];
}
lw6ker_score_array_t;

/**
 * Game struct is very similar to the level struct of the
 * lw6map module. Indeed, it's immutable and won't change
 * during the game. The difference with lw6map_level is that
 * game_struct is algorithm aware and has many tricks, special
 * internals, cached data, so that it speeds up the overall
 * algorithm. In fact it contains everything lw6ker_game_state
 * needs to have but need not change during the game.
 * The 3 first members, id, level, rules are the same as the internal
 * _lw6ker_game_struct_t structure. The rest of it is hidden.
 * The program will cast from lw6ker_game_struct_t to _lw6ker_game_struct_t
 * internally.
 */
typedef struct lw6ker_game_struct_s
{
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /**
   * Pointer on the level source structure. This one might still
   * retain informations we don't want to duplicate here, for
   * instance the textures, which are of no use for the core
   * algorithm so are pointless to backup here, but are still of
   * interest for high-level functions such as display stuff.
   */
  lw6map_level_t *level;
  /**
   * Game rules, this is just a cached copy to avoid derefencing
   * the level pointer any time we need to query a parameter.
   */
  lw6map_rules_t rules;
}
lw6ker_game_struct_t;

/**
 * Game structure containing all changeable data state, this will
 * hold the fighter positions, their health, the cursors position,
 * the gradient, anything that is dynamic.
 * Note that this structure is a wrapper over the internal structure
 * which contains the real members, the first two members need be
 * the same as it is casted internally.
 */
typedef struct lw6ker_game_state_s
{
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /**
   * Pointer on the game non-mutable structure, which holds the
   * data that is never changed within a game.
   */
  lw6ker_game_struct_t *game_struct;
}
lw6ker_game_state_t;

/* ker-capture.c */
extern char *lw6ker_capture_str (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);

/* ker-checksumlog.c */
extern void lw6ker_game_state_checksum_log_set_interval (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int checksum_log_interval);

/* ker-cursor.c */
extern void lw6ker_cursor_reset (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor);

/* ker-gamestate.c */
extern lw6ker_game_state_t *lw6ker_game_state_new (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress);
extern void lw6ker_game_state_free (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state);
extern void lw6ker_game_state_point_to (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, const lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_state_memory_footprint (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state);
extern char *lw6ker_game_state_repr (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_can_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * dst, const lw6ker_game_state_t * src);
extern int lw6ker_game_state_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * dst, const lw6ker_game_state_t * src);
extern lw6ker_game_state_t *lw6ker_game_state_dup (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, lw6sys_progress_t * progress);
extern u_int32_t lw6ker_game_state_checksum (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern void lw6ker_game_state_get_shape (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, lw6sys_whd_t * shape);
extern int lw6ker_game_state_get_w (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_get_h (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_get_d (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_register_node (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id);
extern int lw6ker_game_state_unregister_node (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id);
extern int lw6ker_game_state_node_exists (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id);
extern int lw6ker_game_state_get_node_info (lw6sys_context_t * sys_context,
					    lw6ker_game_state_t * game_state, u_int16_t node_id, u_int32_t * last_command_round);
extern int lw6ker_game_state_add_cursor (lw6sys_context_t * sys_context,
					 lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id, int team_color);
extern int lw6ker_game_state_remove_cursor (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id);
extern int lw6ker_game_state_cursor_exists (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, u_int16_t cursor_id);
extern int lw6ker_game_state_get_cursor (lw6sys_context_t * sys_context,
					 const lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, u_int16_t cursor_id);
extern void lw6ker_game_state_get_cursor_by_index (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, int i);
extern int lw6ker_game_state_set_cursor (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor);
extern int lw6ker_game_state_team_exists (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int team_color);
extern int lw6ker_game_state_get_team_info (lw6sys_context_t * sys_context,
					    const lw6ker_game_state_t * game_state, int team_color, int32_t * nb_cursors, int32_t * nb_fighters);
extern int lw6ker_game_state_get_nb_teams (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern void lw6ker_game_state_do_spread (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int32_t team_mask);
extern void lw6ker_game_state_do_move (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int32_t team_mask);
extern void lw6ker_game_state_finish_round (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state);
extern void lw6ker_game_state_do_round (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state);
extern u_int32_t lw6ker_game_state_get_moves (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern u_int32_t lw6ker_game_state_get_spreads (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern u_int32_t lw6ker_game_state_get_rounds (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern u_int32_t lw6ker_game_state_get_total_rounds (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_is_over (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_did_cursor_win (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, u_int16_t cursor_id);
extern int lw6ker_game_state_get_winner (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int excluded_team);
extern int lw6ker_game_state_get_looser (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int excluded_team);
extern int32_t lw6ker_game_state_get_nb_active_fighters (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int32_t lw6ker_game_state_get_time_elapsed (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int32_t lw6ker_game_state_get_time_left (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int32_t lw6ker_game_state_get_global_history (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int i, int team_id);
extern int32_t lw6ker_game_state_get_latest_history (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int i, int team_id);
extern int32_t lw6ker_game_state_get_global_history_max (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int32_t lw6ker_game_state_get_latest_history_max (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int32_t lw6ker_game_state_get_fighter_id (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z);
extern lw6ker_fighter_t *lw6ker_game_state_get_fighter_rw_by_id (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int32_t fighter_id);
extern lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_rw_safe (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z);
extern lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_rw_unsafe (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z);
extern const lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_ro_by_id (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t fighter_id);
extern const lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_ro_safe (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z);
extern const lw6ker_fighter_t
  * lw6ker_game_state_get_fighter_ro_unsafe (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z);
extern int lw6ker_game_state_get_zone_potential (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int zone_i, int team_id);
extern int lw6ker_game_state_get_charge_per1000 (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int team_color);
extern int lw6ker_game_state_get_weapon_per1000_left (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int team_color);
extern int lw6ker_game_state_get_latest_weapon (lw6sys_context_t *
						sys_context, const lw6ker_game_state_t * game_state, int *team_color, int *weapon_id, int *per1000_left);
extern int lw6ker_game_state_get_nb_colors (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_get_nb_cursors (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern int lw6ker_game_state_get_nb_nodes (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);

/* ker-gamestruct.c */
extern lw6ker_game_struct_t *lw6ker_game_struct_new (lw6sys_context_t * sys_context, const lw6map_level_t * level, lw6sys_progress_t * progress);
extern void lw6ker_game_struct_free (lw6sys_context_t * sys_context, lw6ker_game_struct_t * game_struct);
extern void lw6ker_game_struct_point_to (lw6sys_context_t * sys_context, lw6ker_game_struct_t * game_struct, const lw6map_level_t * level);
extern int lw6ker_game_struct_memory_footprint (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct);
extern char *lw6ker_game_struct_repr (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct);
extern lw6ker_game_struct_t *lw6ker_game_struct_dup (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress);
extern u_int32_t lw6ker_game_struct_checksum (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct);
extern void lw6ker_game_struct_get_shape (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_whd_t * shape);
extern int lw6ker_game_struct_get_w (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_struct_get_h (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_struct_get_d (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct);
extern int lw6ker_game_struct_is_fg (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int32_t x, int32_t y, int32_t z);
extern int lw6ker_game_struct_is_bg (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int32_t x, int32_t y, int32_t z);
extern void lw6ker_game_struct_get_zones_info (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int *nb_zones, int *max_zone_size);
extern void lw6ker_game_struct_get_zone_info (lw6sys_context_t * sys_context,
					      const lw6ker_game_struct_t * game_struct, int i, lw6sys_xyz_t * zone_pos, int *zone_size);
extern int32_t lw6ker_game_struct_get_zone_id (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int32_t x, int32_t y, int32_t z);
extern void lw6ker_game_struct_find_free_slot_near (lw6sys_context_t *
						    sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_xyz_t * there, lw6sys_xyz_t here);

/* ker-hexa.c */
extern char *lw6ker_game_struct_to_hexa (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct);
extern lw6ker_game_struct_t *lw6ker_game_struct_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const lw6map_level_t * level);
extern char *lw6ker_game_state_to_hexa (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state);
extern lw6ker_game_state_t *lw6ker_game_state_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const lw6ker_game_struct_t * game_struct);

/* ker-move.c */
extern int lw6ker_move_get_best_next_pos (lw6sys_context_t * sys_context,
					  const lw6ker_game_state_t * game_state, lw6sys_xyz_t * next_pos, lw6sys_xyz_t * current_pos, int team_color);
/* ker-scorearray.c */
extern int lw6ker_score_array_update (lw6sys_context_t * sys_context, lw6ker_score_array_t * score_array, const lw6ker_game_state_t * game_state);

/* ker-teammask.c */
extern void lw6ker_team_mask_get (lw6sys_context_t * sys_context, u_int32_t * even, u_int32_t * odd, int32_t round);
extern void lw6ker_team_mask_best (lw6sys_context_t * sys_context, u_int32_t * even, u_int32_t * odd, lw6ker_game_state_t * game_state);
extern int lw6ker_team_mask_is_concerned (lw6sys_context_t * sys_context, int team_color, u_int32_t team_mask);
extern int lw6ker_team_mask_color2mask (lw6sys_context_t * sys_context, int team_color);

/* ker-test.c */
extern int lw6ker_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6ker_test_run (lw6sys_context_t * sys_context, int mode);

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
  return (int32_t) ((((int64_t) n) * ((int64_t) per100) * ((int64_t) per100)) / 10000LL);
}

#endif // LIQUIDWAR6KER_H
