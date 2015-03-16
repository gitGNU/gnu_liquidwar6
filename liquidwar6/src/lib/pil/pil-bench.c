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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "pil.h"
#include "pil-internal.h"

#include <math.h>

#define BENCH_DURATION 10000L
#define DUMMY_INIT 0LL
#define DUMMY_INC 1LL
#define NOP_LOOP 100000
#define BENCH_SEQ_0 _LW6PIL_MIN_SEQ_0
#define BENCH_TIMESTAMP 0L

static char *bench_init_commands[] = {
  "10000000001 1234abcd1234abcd REGISTER",
  "10000000002 1234abcd1234abcd ADD 1234 RED",
  "10000000002 1234abcd1234abcd ADD 2345 GREEN",
  "10000000002 1234abcd1234abcd ADD 3456 BLUE",
  "10000000002 1234abcd1234abcd ADD 5678 YELLOW",
  "10000000003 1234abcd1234abcd SET 1234 180 140 1 1",
  "10000000003 1234abcd1234abcd SET 2345 220 160 1 1",
  "10000000003 1234abcd1234abcd SET 3456 220 140 1 1",
  "10000000003 1234abcd1234abcd SET 5678 180 160 1 1",
  NULL
};

/*
 * max round by default is MAX_TOTAL_TIME*DEFAULTS_ROUND_PER_SEC
 * which is about 2*10^7
 */
#define BENCH_INFINITE_SEQ ((int64_t) (BENCH_SEQ_0+10000000))
#define BENCH_NOP_COMMAND ("%" LW6SYS_PRINTF_LL "d 1234abcd1234abcd NOP")
#define BENCH_ANTICIPATION_DELTA LW6MAP_RULES_DEFAULT_ROUNDS_PER_SEC

#define _PROGRESS_INIT_HERE 0.1f

void
_lw6pil_bench_dummy_nop (lw6sys_context_t * sys_context, int64_t * dummy)
{
  int i;

  for (i = 0; i < NOP_LOOP; ++i)
    {
      (*dummy) += DUMMY_INC;
    }
}

static void
_do_bench (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, float *value, lw6sys_progress_t * progress)
{
  int64_t start_time = 0L;
  int64_t current_time = 0L;
  int64_t dummy = DUMMY_INIT;
  char **command;
  int64_t current_seq = BENCH_SEQ_0;
  int64_t last_seq = BENCH_SEQ_0 - BENCH_ANTICIPATION_DELTA;
  char *command_str;
  int64_t seq_reference = 0;
  int64_t seq_draft = 0;
  int computed_reference_begin = 0;
  int computed_draft_begin = 0;
  int computed_reference_end = 0;
  int computed_draft_end = 0;
  int computed_reference_delta = 0;
  int computed_draft_delta = 0;

  start_time = lw6sys_get_timestamp (sys_context);
  command = bench_init_commands;
  while (*command)
    {
      _lw6pil_pilot_send_command (sys_context, pilot, *command, 0);
      _lw6pil_pilot_send_command (sys_context, pilot, *command, 1);
      command++;
    }
  command_str = lw6sys_new_sprintf (sys_context, BENCH_NOP_COMMAND, (long long) BENCH_INFINITE_SEQ);
  if (command_str)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("infinite command \"%s\""), command_str);
      _lw6pil_pilot_send_command (sys_context, pilot, command_str, 0);
      _lw6pil_pilot_send_command (sys_context, pilot, command_str, 1);
      LW6SYS_FREE (sys_context, command_str);
    }
  _lw6pil_pilot_commit (sys_context, NULL, pilot);

  computed_reference_begin = pilot->reference.computed_rounds;
  computed_draft_begin = pilot->draft.computed_rounds;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("stress start dummy=%" LW6SYS_PRINTF_LL "d"), (long long) dummy);
  while ((current_time = lw6sys_get_timestamp (sys_context)) < start_time + BENCH_DURATION)
    {
      current_seq = _lw6pil_pilot_get_reference_current_seq (sys_context, pilot);
      if (current_seq - BENCH_ANTICIPATION_DELTA >= last_seq)
	{
	  command_str = lw6sys_new_sprintf (sys_context, BENCH_NOP_COMMAND, (long long) (BENCH_INFINITE_SEQ + current_seq + 1));
	  if (command_str)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("anticipate at seq %" LW6SYS_PRINTF_LL "d with \"%s\""), (long long) current_seq, command_str);
	      _lw6pil_pilot_send_command (sys_context, pilot, command_str, 0);
	      LW6SYS_FREE (sys_context, command_str);
	    }
	  _lw6pil_pilot_commit (sys_context, NULL, pilot);
	  last_seq = current_seq;
	}
      lw6sys_progress_update (sys_context, progress, 0, BENCH_DURATION, current_time - start_time);
      _lw6pil_bench_dummy_nop (sys_context, &dummy);
    }
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("stress stop dummy=%" LW6SYS_PRINTF_LL "d"), (long long) dummy);

  seq_reference = _lw6pil_pilot_round2seq (sys_context, pilot, lw6ker_game_state_get_rounds (sys_context, pilot->reference.game_state));
  seq_draft = _lw6pil_pilot_round2seq (sys_context, pilot, lw6ker_game_state_get_rounds (sys_context, pilot->draft.game_state));
  computed_reference_end = pilot->reference.computed_rounds;
  computed_draft_end = pilot->draft.computed_rounds;
  computed_reference_delta = computed_reference_end - computed_reference_begin;
  computed_draft_delta = computed_draft_end - computed_draft_begin;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
	      _x_ ("bench seq_reference=%" LW6SYS_PRINTF_LL "d seq_draft=%" LW6SYS_PRINTF_LL "d"), (long long) seq_reference, (long long) seq_draft);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bench computed_reference_begin=%d computed_draft_begin=%d"), computed_reference_begin, computed_draft_begin);

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bench computed_reference_end=%d computed_draft_end=%d"), computed_reference_end, computed_draft_end);

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bench computed_reference_delta=%d computed_draft_delta=%d"), computed_reference_delta, computed_draft_delta);

  /*
     Previous versions of bench used to take draft_delta in account
     but it sometimes gives erratic results.
     (*value) =
     (((float) (computed_reference_delta + computed_draft_delta)) *
     LW6SYS_TICKS_PER_SEC) / (2.0f * BENCH_DURATION);
   */

  (*value) = lw6sys_math_lin2log (sys_context, ((float) (computed_reference_delta * LW6SYS_TICKS_PER_SEC)) / BENCH_DURATION, LW6MAP_LOG2LIN_BASE);
}

static int
_write_bench (lw6sys_context_t * sys_context, int argc, const char *argv[], float value)
{
  int ret = 0;
  char *user_dir = NULL;
  char *bench_txt_path = NULL;
  char *bench_txt_content = NULL;
  char *locale;
  char *old_locale;

  user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
  if (user_dir)
    {
      bench_txt_path = lw6sys_path_concat (sys_context, user_dir, _LW6PIL_BENCH_TXT);
      if (bench_txt_path)
	{
	  locale = setlocale (LC_ALL, NULL);
	  if (locale)
	    {
	      /*
	       * We do need to make a copy in a separate buffer,
	       * otherwise the content pointed by *locale
	       * might change dynamically when calling setlocale
	       */
	      old_locale = lw6sys_str_copy (sys_context, locale);

	      setlocale (LC_ALL, "C");

	      bench_txt_content = lw6sys_new_sprintf (sys_context, "%0.2f\n", value);

	      setlocale (LC_ALL, old_locale);
	      if (old_locale)
		{
		  LW6SYS_FREE (sys_context, old_locale);
		}
	    }
	  if (bench_txt_content)
	    {
	      ret = lw6sys_write_file_content (sys_context, bench_txt_path, bench_txt_content);
	      LW6SYS_FREE (sys_context, bench_txt_content);
	    }
	  LW6SYS_FREE (sys_context, bench_txt_path);
	}
      LW6SYS_FREE (sys_context, user_dir);
    }

  return ret;
}

/**
 * lw6pil_bench
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: args passed to main
 * @bench_result: pointer to float, will contain the bench result
 * @progress: to inform the caller of the process advancement
 *
 * Runs a standard, normalized bench on a default map. Results can
 * be interpreted as an estimated speed/power of your computer.
 *
 * Return value: 1 on success, 0 if failure
 */
int
lw6pil_bench (lw6sys_context_t * sys_context, int argc, const char *argv[], float *bench_result, lw6sys_progress_t * progress)
{
  int ret = 0;

  lw6map_level_t *level = NULL;
  lw6ker_game_struct_t *game_struct = NULL;
  lw6ker_game_state_t *game_state = NULL;
  _lw6pil_pilot_t *pilot = NULL;
  float value;
  lw6sys_progress_t progress_init;
  lw6sys_progress_t progress_level;
  lw6sys_progress_t progress_game_struct;
  lw6sys_progress_t progress_game_state;
  lw6sys_progress_t progress_pilot;
  lw6sys_progress_t progress_do;

  lw6sys_progress_split_here (sys_context, &progress_init, &progress_do, progress, _PROGRESS_INIT_HERE);
  lw6sys_progress_split4 (sys_context, &progress_level, &progress_game_struct, &progress_game_state, &progress_pilot, &progress_init);
  lw6sys_progress_begin (sys_context, &progress_level);
  level = lw6map_builtin_defaults (sys_context);
  lw6sys_progress_end (sys_context, &progress_level);
  if (level)
    {
      level->param.rules.total_time = LW6MAP_RULES_MAX_TOTAL_TIME;
      game_struct = lw6ker_game_struct_new (sys_context, level, &progress_game_struct);
      if (game_struct)
	{
	  game_state = lw6ker_game_state_new (sys_context, game_struct, &progress_game_state);
	  if (game_state)
	    {
	      pilot = _lw6pil_pilot_new (sys_context, game_state, BENCH_SEQ_0, BENCH_TIMESTAMP, &progress_pilot);
	      if (pilot)
		{
		  /*
		   * We free things now on purpose, just to check
		   * pilot continues to work when objects used
		   * to build it have disappeared.
		   */
		  lw6ker_game_state_free (sys_context, game_state);
		  game_state = NULL;
		  lw6ker_game_struct_free (sys_context, game_struct);
		  game_struct = NULL;
		  lw6map_free (sys_context, level);
		  level = NULL;

		  _do_bench (sys_context, pilot, &value, &progress_do);
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bench value=%0.3f"), value);
		  if (bench_result)
		    {
		      (*bench_result) = value;
		    }

		  _write_bench (sys_context, argc, argv, value);

		  ret = 1;
		  _lw6pil_pilot_free (sys_context, pilot);
		}
	      if (game_state)
		{
		  lw6ker_game_state_free (sys_context, game_state);
		  game_state = NULL;
		}
	    }
	  if (game_struct)
	    {
	      lw6ker_game_struct_free (sys_context, game_struct);
	      game_struct = NULL;
	    }
	}
      if (level)
	{
	  lw6map_free (sys_context, level);
	  level = NULL;
	}
    }

  return ret;
}
