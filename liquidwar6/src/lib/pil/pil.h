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

#ifndef LIQUIDWAR6PIL_H
#define LIQUIDWAR6PIL_H

#include "../sys/sys.h"
#include "../map/map.h"
#include "../ker/ker.h"

#define LW6PIL_COMMAND_TEXT_INVALID "INVALID"
#define LW6PIL_COMMAND_TEXT_NOP "NOP"
#define LW6PIL_COMMAND_TEXT_REGISTER "REGISTER"
#define LW6PIL_COMMAND_TEXT_ADD "ADD"
#define LW6PIL_COMMAND_TEXT_SET "SET"
#define LW6PIL_COMMAND_TEXT_REMOVE "REMOVE"
#define LW6PIL_COMMAND_TEXT_UNREGISTER "UNREGISTER"
#define LW6PIL_COMMAND_TEXT_SEED "SEED"
#define LW6PIL_COMMAND_TEXT_DUMP "DUMP"

#define LW6PIL_DIRTY_READ_NEVER 0
#define LW6PIL_DIRTY_READ_SYNC_ONLY 1
#define LW6PIL_DIRTY_READ_ALWAYS 2

#define LW6PIL_COORDS_X10 10
#define LW6PIL_COORDS_X2 2

#define LW6PIL_SUITE_NB_NODES 3
#define LW6PIL_SUITE_NB_STAGES 6

#define LW6PIL_SUITE_NODE_A 0
#define LW6PIL_SUITE_NODE_B 1
#define LW6PIL_SUITE_NODE_C 2

#define LW6PIL_SUITE_STAGE_1 0
#define LW6PIL_SUITE_STAGE_2 1
#define LW6PIL_SUITE_STAGE_3 2
#define LW6PIL_SUITE_STAGE_4 3
#define LW6PIL_SUITE_STAGE_5 4
#define LW6PIL_SUITE_STAGE_6 5

typedef enum lw6pil_command_code_e
{
  LW6PIL_COMMAND_CODE_INVALID = 0,
  LW6PIL_COMMAND_CODE_NOP = 1,
  LW6PIL_COMMAND_CODE_REGISTER = 2,
  LW6PIL_COMMAND_CODE_ADD = 3,
  LW6PIL_COMMAND_CODE_SET = 4,
  LW6PIL_COMMAND_CODE_REMOVE = 5,
  LW6PIL_COMMAND_CODE_UNREGISTER = 6,
  LW6PIL_COMMAND_CODE_SEED = 7,
  /*
   * It's important that DUMP is the last one, so that it's sorted
   * at the end as far as messages with the same ID are concerned.
   * This might be superfluous as anyway, all messages should be
   * here and sorted before they get into the pipe, but well,
   * DUMP is generated out of the flow, and will generally come
   * after the rest of the battle, so it's safer to put it at
   * the end.
   */
  LW6PIL_COMMAND_CODE_DUMP = 8
} lw6pil_command_code_t;

/**
 * Arguments passed to the ADD command.
 */
typedef struct lw6pil_add_args_s
{
  /// Cursor ID (16-bit non-zero unsigned int).
  u_int16_t cursor_id;
  /// Team color (0 to 9).
  int team_color;
} lw6pil_add_args_t;

/**
 * Arguments passed to the REMOVE command.
 */
typedef struct lw6pil_remove_args_s
{
  /// Cursor ID (16-bit non-zero unsigned int).
  u_int16_t cursor_id;
} lw6pil_remove_args_t;

/**
 * Arguments passed to the SET command.
 */
typedef struct lw6pil_set_args_s
{
  /// Cursor ID (16-bit non-zero unsigned int).
  u_int64_t cursor_id;
  /// X position (map coords).
  int x;
  /// Y position (map coords).
  int y;
  /// Wether to activate primary weapon.
  int fire;
  /// Wether to activate secondary weapon.
  int fire2;
} lw6pil_set_args_t;

/**
 * Arguments passed to the DUMP command.
 */
typedef struct lw6pil_dump_args_s
{
  /// Hexa dump of level
  char *level_hexa;
  /// Hexa dump of game struct.
  char *game_struct_hexa;
  /// Hexa dump of game state.
  char *game_state_hexa;
} lw6pil_dump_args_t;

/**
 * Arguments passed to various commands.
 */
typedef union lw6pil_command_args_u
{
  /// Arguments when command is ADD.
  lw6pil_add_args_t add;
  /// Arguments when command is REMOVE.
  lw6pil_remove_args_t remove;
  /// Arguments when command is SET.
  lw6pil_set_args_t set;
  /// Arguments when command is DUMP.
  lw6pil_dump_args_t dump;
} lw6pil_command_args_t;

/**
 * Command structure, contains both full-text
 * version and parsed information.
 */
typedef struct lw6pil_command_s
{
  /**
   * The sequence number, a very large 64-bit integer.
   * The sequence is here because the round wouldn't be
   * able to stand multiple games for long as it is
   * only 32-bit. OTOH making round a 64-bit would cause
   * multithread problems because of non-atomicity of
   * 64-bit affectation on truely 32-bit platforms.
   */
  int64_t seq;
  /// The game round
  int round;
  /// The node ID issuing that command.
  u_int64_t node_id;
  /// The command code.
  lw6pil_command_code_t code;
  /// The command arguments, parsed.
  lw6pil_command_args_t args;
  /// The original full text of the command.
  char *text;
} lw6pil_command_t;

typedef struct lw6pil_pilot_s *lw6pil_pilot_p;

/**
 * Stores the information contained in a dump, this is just
 * an utility struct used to hold all the data (level, game struct,
 * game state and pilot) together
 */
typedef struct lw6pil_dump_s
{
  /// Will hold the new level if a dump is received
  lw6map_level_t *level;
  /// Will hold the new game struct if a dump is received
  lw6ker_game_struct_t *game_struct;
  /// Will hold the new game state if a dump is received
  lw6ker_game_state_t *game_state;
  /// Will hold the new pilot if a dump is received
  lw6pil_pilot_p pilot;
} lw6pil_dump_t;

/**
 * Worker thread data, used to calculate stuff
 * in a separate thread. The principle is simple,
 * it tries to keep up with a given target round,
 * whenever this round isn't reached, it computes
 * more and more rounds.
 */
typedef struct lw6pil_worker_s
{
  /// Wether this thread should run, 0 will stop it.
  volatile int run;
  /// Wether this is running in verified mode or not.
  int verified;
  /// Current game round.
  volatile int current_round;
  /// Round up to which we should compute stuff.
  volatile int target_round;
  /// How many rounds where computed since object creation.
  volatile int computed_rounds;
  /// Wether the game is over or not.
  volatile int over;
  /// The thread that does the job.
  lw6sys_thread_handler_t *compute_thread;
  /// Global data mutex.
  lw6sys_mutex_t *global_mutex;
  /// Mutex used for the computing thread.
  lw6sys_mutex_t *compute_mutex;
  /// Game state the computing thread is working on.
  lw6ker_game_state_t *game_state;
  /// List of commands to be processed.
  lw6sys_list_r_t *commands;
  /// Dump information
  lw6pil_dump_t dump;
} lw6pil_worker_t;

/**
 * Stores informations about local cursors. This is usefull
 * for user feedback. Indeed there can be some delay between,
 * for instance, a mouse move or a keyboard press, and the
 * time this information makes it through the whole pipeline.
 * Players wouldn't understand such a lag so for local cursors
 * we override the information from the game state with that
 * information we get right from the GUI.
 */
typedef struct lw6pil_local_cursor_s
{
  /// Cursor ID (16-bit non-zero unsigned int).
  u_int16_t cursor_id;
  /// X position (map coords).
  int x;
  /// Y position (map coords).
  int y;
  /**
   * Wether this cursor is mouse controlled. If yes, then
   * information will be taken directly from the mouse driver.
   */
  int mouse_controlled;
  /// Wether this cursor is the main cursor.
  int is_main;
} lw6pil_local_cursor_t;

/**
 * Contains information about all local cursors,
 * which will override information from game state.
 */
typedef struct lw6pil_local_cursors_s
{
  /**
   * Main cursor ID (16-bit non-zero unsigned int).
   * By main, we usually mean the mouse-driven one but
   * necessarily. But still, this is a cursor which will
   * have a special role, it will be used to center the
   * map if needed, among other things.
   */
  u_int16_t main_cursor_id;
  /// Main cursor index.
  int main_i;
  /// Number of cursors.
  int nb_cursors;
  /// The cursors array.
  lw6pil_local_cursor_t cursors[LW6MAP_MAX_NB_CURSORS];
}
lw6pil_local_cursors_t;

/**
 * Pilot is a container for several game states, a reference
 * state which we can rely upon and a draft state which is
 * anticipated to provide interactive feedback but reflects
 * non-validated informations.
 * The first member, id, is the same as the internal
 * _lw6pil_pilot_t structure. The rest of it is hidden.
 * The program will cast from lw6pil_pilot_t to _lw6pil_pilot_t
 * internally.
 */
typedef struct lw6pil_pilot_s
{
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
} lw6pil_pilot_t;

/* pil-bench.c */
extern int lw6pil_bench (lw6sys_context_t * sys_context, int argc, const char *argv[], float *bench_result, lw6sys_progress_t * progress);

/* pil-command.c */
extern lw6pil_command_t *lw6pil_command_new (lw6sys_context_t * sys_context, const char *command_text, int64_t seq_0, int round_0);
extern lw6pil_command_t *lw6pil_command_dup (lw6sys_context_t * sys_context, lw6pil_command_t * command);
extern void lw6pil_command_free (lw6sys_context_t * sys_context, lw6pil_command_t * command);
extern char *lw6pil_command_repr (lw6sys_context_t * sys_context, const lw6pil_command_t * command);
extern int lw6pil_command_execute (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp, lw6ker_game_state_t * game_state,
				   lw6pil_command_t * command);
extern int lw6pil_command_execute_text (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp, lw6ker_game_state_t * game_state,
					const char *command_text, int64_t seq_0);
extern int lw6pil_command_execute_local (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, lw6pil_command_t * command);
extern int lw6pil_command_execute_local_text (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, const char *command_text);

/* pil-coords.c */
extern void lw6pil_coords_fix (lw6sys_context_t * sys_context, lw6map_rules_t * rules, lw6sys_whd_t * shape, float *x, float *y, float *z);
extern void lw6pil_coords_fix_x10 (lw6sys_context_t * sys_context, lw6map_rules_t * rules, lw6sys_whd_t * shape, float *x, float *y, float *z);

/* pil-dump.c */
extern void lw6pil_dump_zero (lw6sys_context_t * sys_context, lw6pil_dump_t * dump);
extern void lw6pil_dump_clear (lw6sys_context_t * sys_context, lw6pil_dump_t * dump);
extern int lw6pil_dump_exists (lw6sys_context_t * sys_context, const lw6pil_dump_t * dump);
extern char *lw6pil_dump_command_generate (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq);
extern int lw6pil_dump_command_execute (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp, lw6pil_command_t * command,
					lw6sys_progress_t * progress);

/* pil-localcursors.c */
extern void lw6pil_local_cursors_reset (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors);
extern lw6pil_local_cursor_t *lw6pil_local_cursors_get_cursor (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id);
extern int lw6pil_local_cursors_get_info (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, int *x, int *y, int *mouse_controlled,
					  u_int16_t cursor_id);
extern int lw6pil_local_cursors_set_xy (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id, int x, int y);
extern int lw6pil_local_cursors_set_mouse_controlled (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id,
						      int mouse_controlled);
extern int lw6pil_local_cursors_get_main_info (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t * cursor_id, int *x, int *y,
					       int *mouse_controlled);
extern int lw6pil_local_cursors_set_main (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id);

/* pil-nopilot.c */
extern int lw6pil_nopilot_poll_dump (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, const char *command_text, int64_t timestamp);

/* pil-pilot.c */
extern lw6pil_pilot_t *lw6pil_pilot_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int64_t seq_0, int64_t timestamp,
					 lw6sys_progress_t * progress);
extern void lw6pil_pilot_free (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_send_command (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, const char *command_text, int verified);
extern int lw6pil_pilot_local_command (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, const char *command_text);
extern int lw6pil_pilot_commit (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_make_backup (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_can_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_sync_from_backup (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_sync_from_reference (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_sync_from_draft (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot, int dirty_read);
extern lw6ker_game_state_t *lw6pil_pilot_dirty_read (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot);
extern char *lw6pil_pilot_repr (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern void lw6pil_pilot_calibrate (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int64_t timestamp, int64_t seq);
extern void lw6pil_pilot_speed_up (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int seq_inc);
extern void lw6pil_pilot_slow_down (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int seq_dec);
extern int lw6pil_pilot_get_round_0 (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int64_t lw6pil_pilot_get_seq_0 (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_seq2round (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, int64_t seq);
extern int64_t lw6pil_pilot_round2seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, int round);
extern int64_t lw6pil_pilot_get_next_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, int64_t timestamp);
extern int64_t lw6pil_pilot_get_last_commit_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int64_t lw6pil_pilot_get_reference_target_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int64_t lw6pil_pilot_get_reference_current_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int64_t lw6pil_pilot_get_max_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_is_over (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_did_cursor_win (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, u_int16_t cursor_id);
extern int lw6pil_pilot_get_winner (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_get_looser (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot);
extern lw6pil_local_cursors_t *lw6pil_pilot_get_local_cursors (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot);
extern void lw6pil_pilot_checksum_log_set_interval (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int checksum_log_interval);

/* pil-seed.c */
extern char *lw6pil_seed_command_generate (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq);

/* pil-seq.c */
extern int64_t lw6pil_seq_random_0 (lw6sys_context_t * sys_context);

/* pil-suite.c */
extern int lw6pil_suite_init (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp);
extern int64_t lw6pil_suite_get_seq_0 (lw6sys_context_t * sys_context);
extern u_int64_t lw6pil_suite_get_node_id (lw6sys_context_t * sys_context, int node_index);
extern const char *lw6pil_suite_get_command_by_node_index (lw6sys_context_t * sys_context, int node_index, int stage, int step);
extern const char *lw6pil_suite_get_command_by_stage (lw6sys_context_t * sys_context, int stage, int step);
extern const char *lw6pil_suite_get_command_by_step (lw6sys_context_t * sys_context, int step);
extern void lw6pil_suite_get_checkpoint (lw6sys_context_t * sys_context, u_int32_t * game_state_checksum, int64_t * seq, int *round, int stage);

/* pil-test.c */
extern int lw6pil_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6pil_test_run (lw6sys_context_t * sys_context, int mode);

#endif // LIQUIDWAR6PIL_H
