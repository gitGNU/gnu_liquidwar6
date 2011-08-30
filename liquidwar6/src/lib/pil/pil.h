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

#define LW6PIL_DIRTY_READ_NEVER 0
#define LW6PIL_DIRTY_READ_SYNC_ONLY 1
#define LW6PIL_DIRTY_READ_ALWAYS 2

typedef enum lw6pil_command_code_e
{
  LW6PIL_COMMAND_CODE_INVALID = 0,
  LW6PIL_COMMAND_CODE_NOP = 1,
  LW6PIL_COMMAND_CODE_REGISTER = 2,
  LW6PIL_COMMAND_CODE_ADD = 3,
  LW6PIL_COMMAND_CODE_SET = 4,
  LW6PIL_COMMAND_CODE_REMOVE = 5,
  LW6PIL_COMMAND_CODE_UNREGISTER = 6
} lw6pil_command_code_t;

typedef struct lw6pil_add_args_s
{
  u_int16_t cursor_id;
  int team_color;
} lw6pil_add_args_t;

typedef struct lw6pil_remove_args_s
{
  u_int16_t cursor_id;
} lw6pil_remove_args_t;

typedef struct lw6pil_set_args_s
{
  u_int64_t cursor_id;
  int x;
  int y;
  int fire;
  int fire2;
} lw6pil_set_args_t;

typedef union lw6pil_command_args_u
{
  lw6pil_add_args_t add;
  lw6pil_remove_args_t remove;
  lw6pil_set_args_t set;
} lw6pil_command_args_t;

typedef struct lw6pil_command_s
{
  int round;
  u_int64_t node_id;
  lw6pil_command_code_t code;
  lw6pil_command_args_t args;
  char *text;
} lw6pil_command_t;

typedef struct lw6pil_worker_s
{
  int run;
  int current_round;
  int target_round;
  int computed_rounds;
  int over;
  void *compute_thread;
  void *global_mutex;
  void *compute_mutex;
  void *commands_spinlock;
  lw6ker_game_state_t *game_state;
  lw6sys_list_t *commands;
} lw6pil_worker_t;

typedef struct lw6pil_local_cursor_s
{
  u_int16_t cursor_id;
  int x;
  int y;
  int mouse_controlled;
  int is_main;
} lw6pil_local_cursor_t;

typedef struct lw6pil_local_cursors_s
{
  u_int16_t main_cursor_id;
  int main_i;
  int nb_cursors;
  lw6pil_local_cursor_t cursors[LW6MAP_MAX_NB_CURSORS];
}
lw6pil_local_cursors_t;

typedef struct lw6pil_pilot_s
{
  u_int32_t id;
  int64_t calibrate_ticks;
  int calibrate_round;
  lw6map_level_t *level;
  lw6ker_game_struct_t *game_struct;
  lw6ker_game_state_t *backup;
  lw6pil_worker_t reference;
  lw6pil_worker_t draft;
  lw6sys_list_t *verified_queue;
  lw6sys_list_t *unverified_queue;
  lw6sys_list_t *replay;
  int last_commit_round;
  int last_sync_draft_from_reference;
  lw6pil_local_cursors_t local_cursors;
} lw6pil_pilot_t;

/* pil-bench.c */
extern int lw6pil_bench (float *bench_result, lw6sys_progress_t * progress);

/* pil-command.c */
extern lw6pil_command_t *lw6pil_command_new (char *command_text);
extern lw6pil_command_t *lw6pil_command_dup (lw6pil_command_t * command);
extern void lw6pil_command_free (lw6pil_command_t * command);
extern char *lw6pil_command_repr (lw6pil_command_t * command);
extern int lw6pil_command_execute (lw6ker_game_state_t * game_state,
				   lw6pil_command_t * command);
extern int lw6pil_command_execute_text (lw6ker_game_state_t * game_state,
					char *command_text);
extern int lw6pil_command_execute_local (lw6pil_local_cursors_t *
					 local_cursors,
					 lw6pil_command_t * command);
extern int lw6pil_command_execute_local_text (lw6pil_local_cursors_t *
					      local_cursors,
					      char *command_text);

/* pil-coords.c */
extern void lw6pil_coords_fix (lw6map_rules_t * rules, lw6sys_whd_t * shape,
			       float *x, float *y, float *z);

/* pil-localcursors.c */
extern void lw6pil_local_cursors_reset (lw6pil_local_cursors_t *
					local_cursors);
extern lw6pil_local_cursor_t
  * lw6pil_local_cursors_get_cursor (lw6pil_local_cursors_t * local_cursors,
				     u_int16_t cursor_id);
extern int lw6pil_local_cursors_get_info (lw6pil_local_cursors_t *
					  local_cursors, int *x, int *y,
					  int *mouse_controlled,
					  u_int16_t cursor_id);
extern int lw6pil_local_cursors_set_xy (lw6pil_local_cursors_t *
					local_cursors, u_int16_t cursor_id,
					int x, int y);
extern int lw6pil_local_cursors_set_mouse_controlled (lw6pil_local_cursors_t *
						      local_cursors,
						      u_int16_t cursor_id,
						      int mouse_controlled);
extern int lw6pil_local_cursors_get_main_info (lw6pil_local_cursors_t *
					       local_cursors,
					       u_int16_t * cursor_id, int *x,
					       int *y, int *mouse_controlled);
extern int lw6pil_local_cursors_set_main (lw6pil_local_cursors_t *
					  local_cursors, u_int16_t cursor_id);

/* pil-pilot.c */
extern lw6pil_pilot_t *lw6pil_pilot_new (lw6ker_game_state_t * game_state,
					 int64_t timestamp,
					 lw6sys_progress_t * progress);
extern void lw6pil_pilot_free (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_send_command (lw6pil_pilot_t * pilot,
				      char *command_text, int verified);
extern int lw6pil_pilot_local_command (lw6pil_pilot_t * pilot,
				       char *command_text);
extern int lw6pil_pilot_commit (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_make_backup (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_can_sync (lw6ker_game_state_t * target,
				  lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_sync_from_backup (lw6ker_game_state_t * target,
					  lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_sync_from_reference (lw6ker_game_state_t * target,
					     lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_sync_from_draft (lw6ker_game_state_t * target,
					 lw6pil_pilot_t * pilot,
					 int dirty_read);
extern lw6ker_game_state_t *lw6pil_pilot_dirty_read (lw6pil_pilot_t * pilot);
extern char *lw6pil_pilot_repr (lw6pil_pilot_t * pilot);
extern void lw6pil_pilot_calibrate (lw6pil_pilot_t * pilot, int64_t timestamp,
				    int round);
extern void lw6pil_pilot_speed_up (lw6pil_pilot_t * pilot, int round_inc);
extern void lw6pil_pilot_slow_down (lw6pil_pilot_t * pilot, int round_dec);
extern int lw6pil_pilot_get_next_round (lw6pil_pilot_t * pilot,
					int64_t timestamp);
extern int lw6pil_pilot_get_last_commit_round (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_get_reference_target_round (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_get_reference_current_round (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_get_max_round (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_is_over (lw6pil_pilot_t * pilot);
extern int lw6pil_pilot_did_cursor_win (lw6pil_pilot_t * pilot,
					u_int16_t cursor_id);

/* pil-test.c */
extern int lw6pil_test (int mode);

#endif
