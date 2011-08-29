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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pil.h"
#include "pil-internal.h"

#define BENCH_DURATION 10000L
#define DUMMY_INIT 0LL
#define DUMMY_INC 1LL
#define NOP_LOOP 100000

static char *bench_init_commands[] = {
  "1 1234abcd1234abcd REGISTER",
  "2 1234abcd1234abcd ADD 1234 RED",
  "2 1234abcd1234abcd ADD 2345 GREEN",
  "2 1234abcd1234abcd ADD 3456 BLUE",
  "2 1234abcd1234abcd ADD 5678 YELLOW",
  "3 1234abcd1234abcd SET 1234 180 140 1 1",
  "3 1234abcd1234abcd SET 2345 220 160 1 1",
  "3 1234abcd1234abcd SET 3456 220 140 1 1",
  "3 1234abcd1234abcd SET 5678 180 160 1 1",
  NULL
};

/*
 * max round by default is MAX_TOTAL_TIME*DEFAULTS_ROUND_PER_SEC
 * which is about 2*10^7
 */
#define BENCH_INFINITE_ROUND 10000000
#define BENCH_NOP_COMMAND "%d 1234abcd1234abcd NOP"
#define BENCH_ANTICIPATION_DELTA LW6MAP_RULES_DEFAULT_ROUNDS_PER_SEC

#define _PROGRESS_INIT_HERE 0.1f

void
_lw6pil_bench_dummy_nop (int64_t * dummy)
{
  int i;

  for (i = 0; i < NOP_LOOP; ++i)
    {
      (*dummy) += DUMMY_INC;
    }
}

static void
do_bench (lw6pil_pilot_t * pilot, float *value, lw6sys_progress_t * progress)
{
  int64_t start_time = 0L;
  int64_t current_time = 0L;
  int64_t dummy = DUMMY_INIT;
  char **command;
  int last_round = -BENCH_ANTICIPATION_DELTA;
  int current_round = 0;
  char *command_str;
  int rounds_reference = 0;
  int rounds_draft = 0;
  int computed_reference_begin = 0;
  int computed_draft_begin = 0;
  int computed_reference_end = 0;
  int computed_draft_end = 0;
  int computed_reference_delta = 0;
  int computed_draft_delta = 0;

  start_time = lw6sys_get_timestamp ();
  command = bench_init_commands;
  while (*command)
    {
      lw6pil_pilot_send_command (pilot, *command, 0);
      lw6pil_pilot_send_command (pilot, *command, 1);
      command++;
    }
  command_str = lw6sys_new_sprintf (BENCH_NOP_COMMAND, BENCH_INFINITE_ROUND);
  if (command_str)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("infinite command \"%s\""),
		  command_str);
      lw6pil_pilot_send_command (pilot, command_str, 0);
      lw6pil_pilot_send_command (pilot, command_str, 1);
      LW6SYS_FREE (command_str);
    }
  lw6pil_pilot_commit (pilot);

  computed_reference_begin = pilot->reference.computed_rounds;
  computed_draft_begin = pilot->draft.computed_rounds;

  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_ ("stress start dummy=%" LW6SYS_PRINTF_LL "d"), dummy);
  while ((current_time =
	  lw6sys_get_timestamp ()) < start_time + BENCH_DURATION)
    {
      current_round = lw6pil_pilot_get_reference_current_round (pilot);
      if (current_round - BENCH_ANTICIPATION_DELTA >= last_round)
	{
	  command_str =
	    lw6sys_new_sprintf (BENCH_NOP_COMMAND,
				BENCH_INFINITE_ROUND + current_round + 1);
	  if (command_str)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_ ("anticipate at round %d with \"%s\""),
			  current_round, command_str);
	      lw6pil_pilot_send_command (pilot, command_str, 0);
	      LW6SYS_FREE (command_str);
	    }
	  lw6pil_pilot_commit (pilot);
	  last_round = current_round;
	}
      lw6sys_progress_update (progress, 0, BENCH_DURATION,
			      current_time - start_time);
      _lw6pil_bench_dummy_nop (&dummy);
    }
  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_ ("stress stop dummy=%" LW6SYS_PRINTF_LL "d"), dummy);

  rounds_reference =
    lw6ker_game_state_get_rounds (pilot->reference.game_state);
  rounds_draft = lw6ker_game_state_get_rounds (pilot->draft.game_state);
  computed_reference_end = pilot->reference.computed_rounds;
  computed_draft_end = pilot->draft.computed_rounds;
  computed_reference_delta =
    computed_reference_end - computed_reference_begin;
  computed_draft_delta = computed_draft_end - computed_draft_begin;
  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_ ("bench rounds_reference=%d rounds_draft=%d"),
	      rounds_reference, rounds_draft);
  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_
	      ("bench computed_reference_begin=%d computed_draft_begin=%d"),
	      computed_reference_begin, computed_draft_begin);

  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_
	      ("bench computed_reference_end=%d computed_draft_end=%d"),
	      computed_reference_end, computed_draft_end);

  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_
	      ("bench computed_reference_delta=%d computed_draft_delta=%d"),
	      computed_reference_delta, computed_draft_delta);

  /*
     Previous versions of bench used to take draft_delta in account
     but it sometimes gives erratic results.
     (*value) =
     (((float) (computed_reference_delta + computed_draft_delta)) *
     LW6SYS_TICKS_PER_SEC) / (2.0f * BENCH_DURATION);
   */

  (*value) =
    ((float) (computed_reference_delta *
	      LW6SYS_TICKS_PER_SEC)) / BENCH_DURATION;
}

/**
 * lw6pil_bench
 *
 * @bench_result: pointer to float, will contain the bench result
 * @progress: to inform the caller of the process advancement
 *
 * Runs a standard, normalized bench on a default map. Results can
 * be interpreted as an estimated speed/power of your computer.
 *
 * Return value: 1 on success, 0 if failure
 */
int
lw6pil_bench (float *bench_result, lw6sys_progress_t * progress)
{
  int ret = 0;

  lw6map_level_t *level = NULL;
  lw6ker_game_struct_t *game_struct = NULL;
  lw6ker_game_state_t *game_state = NULL;
  lw6pil_pilot_t *pilot = NULL;
  float value;
  lw6sys_progress_t progress_init;
  lw6sys_progress_t progress_level;
  lw6sys_progress_t progress_game_struct;
  lw6sys_progress_t progress_game_state;
  lw6sys_progress_t progress_pilot;
  lw6sys_progress_t progress_do;

  lw6sys_progress_split_here (&progress_init, &progress_do, progress,
			      _PROGRESS_INIT_HERE);
  lw6sys_progress_split4 (&progress_level, &progress_game_struct,
			  &progress_game_state, &progress_pilot,
			  &progress_init);
  lw6sys_progress_begin (&progress_level);
  level = lw6map_builtin_defaults ();
  lw6sys_progress_end (&progress_level);
  if (level)
    {
      level->param.rules.total_time = LW6MAP_RULES_MAX_TOTAL_TIME;
      game_struct = lw6ker_game_struct_new (level, &progress_game_struct);
      if (game_struct)
	{
	  game_state =
	    lw6ker_game_state_new (game_struct, &progress_game_state);
	  if (game_state)
	    {
	      pilot = lw6pil_pilot_new (game_state, 0, &progress_pilot);
	      if (pilot)
		{
		  /*
		   * We free things now on purpose, just to check
		   * pilot continues to work when objects used
		   * to build it have disappeared.
		   */
		  lw6ker_game_state_free (game_state);
		  game_state = NULL;
		  lw6ker_game_struct_free (game_struct);
		  game_struct = NULL;
		  lw6map_free (level);
		  level = NULL;

		  do_bench (pilot, &value, &progress_do);
		  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("bench value=%0.3f"),
			      value);
		  if (bench_result)
		    {
		      (*bench_result) = value;
		    }
		  ret = 1;
		  lw6pil_pilot_free (pilot);
		}
	      if (game_state)
		{
		  lw6ker_game_state_free (game_state);
		  game_state = NULL;
		}
	    }
	  if (game_struct)
	    {
	      lw6ker_game_struct_free (game_struct);
	      game_struct = NULL;
	    }
	}
      if (level)
	{
	  lw6map_free (level);
	  level = NULL;
	}
    }

  return ret;
}
