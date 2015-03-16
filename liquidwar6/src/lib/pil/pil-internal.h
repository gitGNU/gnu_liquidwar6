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

#ifndef LIQUIDWAR6PIL_INTERNAL_H
#define LIQUIDWAR6PIL_INTERNAL_H

#include "pil.h"

#define _LW6PIL_MIN_SEQ_0 10000000000LL
#define _LW6PIL_MIN_ROUND_0 0
#define _LW6PIL_RANGE_SEQ_0 1000000000
#define _LW6PIL_YIELD_LIMIT_MSEC 3000
#define _LW6PIL_YIELD_PERIOD_MSEC 500
#define _LW6PIL_BENCH_TXT "bench.txt"

typedef struct _lw6pil_pilot_s
{
  u_int32_t id;
  int round_0;
  int64_t seq_0;
  int64_t calibrate_timestamp;
  int64_t calibrate_seq;
  lw6map_level_t *level;
  lw6ker_game_struct_t *game_struct;
  lw6ker_game_state_t *backup;
  lw6pil_worker_t reference;
  lw6pil_worker_t draft;
  lw6sys_list_t *verified_queue;
  lw6sys_list_t *unverified_queue;
  lw6sys_list_t *replay;
  int64_t last_commit_seq;
  int64_t last_sync_draft_from_reference_seq;
  lw6pil_local_cursors_t local_cursors;
} _lw6pil_pilot_t;

typedef struct _lw6pil_spread_data_s
{
  lw6ker_game_state_t *game_state;
  u_int32_t team_mask;
} _lw6pil_spread_data_t;
/* pil-bench.c */
extern void _lw6pil_bench_dummy_nop (lw6sys_context_t * sys_context, int64_t * dummy);

/* pil-command.c */
extern int _lw6pil_command_sort_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b);

/* pil-compute.c */
extern void _lw6pil_compute_thread_func (lw6sys_context_t * sys_context, lw6pil_worker_t * worker);
extern void _lw6pil_compute_thread_join (lw6sys_context_t * sys_context, lw6pil_worker_t * worker);
extern void _lw6pil_compute_pump_command_callback (lw6sys_context_t * sys_context, void *func_data, void *data);

/* pil-dump.c */
extern char *_lw6pil_dump_command_generate (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq);

/* pil-pilot.c */
extern _lw6pil_pilot_t *_lw6pil_pilot_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int64_t seq_0, int64_t timestamp,
					   lw6sys_progress_t * progress);
extern void _lw6pil_pilot_free (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_send_command (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, const char *command_text, int verified);
extern int _lw6pil_pilot_local_command (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, const char *command_text);
extern int _lw6pil_pilot_commit (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_make_backup (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_can_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_sync_from_backup (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_sync_from_reference (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_sync_from_draft (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot, int dirty_read);
extern lw6ker_game_state_t *_lw6pil_pilot_dirty_read (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot);
extern char *_lw6pil_pilot_repr (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern void _lw6pil_pilot_calibrate (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int64_t timestamp, int64_t seq);
extern void _lw6pil_pilot_speed_up (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int seq_inc);
extern void _lw6pil_pilot_slow_down (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int seq_dec);
extern int _lw6pil_pilot_get_round_0 (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int64_t _lw6pil_pilot_get_seq_0 (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_seq2round (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, int64_t seq);
extern int64_t _lw6pil_pilot_round2seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, int round);
extern int64_t _lw6pil_pilot_get_next_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, int64_t timestamp);
extern int64_t _lw6pil_pilot_get_last_commit_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int64_t _lw6pil_pilot_get_reference_target_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int64_t _lw6pil_pilot_get_reference_current_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int64_t _lw6pil_pilot_get_max_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_is_over (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_did_cursor_win (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, u_int16_t cursor_id);
extern int _lw6pil_pilot_get_winner (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern int _lw6pil_pilot_get_looser (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot);
extern lw6pil_local_cursors_t *_lw6pil_pilot_get_local_cursors (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot);
extern void _lw6pil_pilot_checksum_log_set_interval (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int checksum_log_interval);

/* pil-seed.c */
extern char *_lw6pil_seed_command_generate (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, u_int64_t server_id, int64_t seq);

/* pil-spread.c */
extern void _lw6pil_spread_thread_func (lw6sys_context_t * sys_context, _lw6pil_spread_data_t * spread_data);

/* pil-worker.c */
extern int _lw6pil_worker_init (lw6sys_context_t * sys_context, lw6pil_worker_t * worker, lw6ker_game_state_t * game_state, int verified,
				lw6sys_progress_t * progress);
extern int _lw6pil_worker_quit (lw6sys_context_t * sys_context, lw6pil_worker_t * worker);

#endif // LIQUIDWAR6PIL_INTERNAL_H
