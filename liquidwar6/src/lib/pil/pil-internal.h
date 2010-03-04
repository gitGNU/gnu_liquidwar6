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

#ifndef LIQUIDWAR6PIL_INTERNAL_H
#define LIQUIDWAR6PIL_INTERNAL_H

#include "pil.h"

typedef struct _lw6pil_spread_data_s
{
  lw6ker_game_state_t *game_state;
  u_int32_t team_mask;
} _lw6pil_spread_data_t;

/* pil-bench.c */
extern void _lw6pil_bench_dummy_nop (int64_t * dummy);

/* pil-command.c */
extern int _lw6pil_command_sort_callback (lw6sys_list_t ** list_a,
					  lw6sys_list_t ** list_b);

/* pil-compute.c */
extern void _lw6pil_compute_thread_func (lw6pil_worker_t * worker);
extern void _lw6pil_compute_thread_join (lw6pil_worker_t * worker);

/* pil-spread.c */
extern void _lw6pil_spread_thread_func (_lw6pil_spread_data_t * spread_data);

/* pil-worker.c */
extern int _lw6pil_worker_init (lw6pil_worker_t * worker,
				lw6ker_game_state_t * game_state,
				lw6sys_progress_t * progress);
extern int _lw6pil_worker_quit (lw6pil_worker_t * worker);

#endif
