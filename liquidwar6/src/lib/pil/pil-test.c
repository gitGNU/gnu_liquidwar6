/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

/*
 * The checksums below will change any time the core algorithm
 * is changed. The standard backup checksum and the dump backup
 * checksum differ because one uses the spread_thread algorithm
 * and the other one does not.
 */
#define _TEST_BACKUP_CHECKSUM 0xe771d39e
#define _TEST_DUMP_BACKUP_CHECKSUM 0xe55fc651
/*
 * At some point a checksum on command was implemented but
 * this is not a very good idea, a map can have (int its dump)
 * some floats which are possibly not absolutely totally exact
 * and could behave differently on different hardware, so it's
 * been disabled and it's probably a good idea to keep it
 * disabled.
 */
//#define _TEST_DUMP_COMMAND_CHECKSUM 0x3ce57a70

#define _TEST_MAP_WIDTH 45
#define _TEST_MAP_HEIGHT 15
#define _TEST_MAP_NB_LAYERS 7
#define _TEST_MAP_NOISE_PERCENT 5
#define _TEST_CYCLE 0.5f
#define _TEST_NB_PILOTS 3
#define _TEST_SYNC_COMMAND_I 10
#define _TEST_BACKUP_ROUND 70
#define _TEST_CALIBRATE_TICKS1 12345
#define _TEST_CALIBRATE_TICKS2 23456
#define _TEST_COORDS_NB 5
#define _TEST_COORDS_W 100
#define _TEST_COORDS_H 50
#define _TEST_COORDS_D 1
#define _TEST_COORDS_X1 66.0f
#define _TEST_COORDS_Y1 33.0f
#define _TEST_COORDS_X2 130.0f
#define _TEST_COORDS_Y2 20.0f
#define _TEST_COORDS_X3 -30.3f
#define _TEST_COORDS_Y3 20.2f
#define _TEST_COORDS_X4 -10.1f
#define _TEST_COORDS_Y4 -5.5f
#define _TEST_COORDS_X5 1000.1f
#define _TEST_COORDS_Y5 51.1f
#define _TEST_COORDS_Z 0.0f
#define _TEST_LOCAL_CURSORS_ID1 0x1234
#define _TEST_LOCAL_CURSORS_X1 15
#define _TEST_LOCAL_CURSORS_Y1 30
#define _TEST_LOCAL_CURSORS_MOUSE_CONTROLLED1 1
#define _TEST_LOCAL_CURSORS_ID2 0x2345
#define _TEST_LOCAL_CURSORS_X2 45
#define _TEST_LOCAL_CURSORS_Y2 90
#define _TEST_LOCAL_CURSORS_MOUSE_CONTROLLED2 0
#define _TEST_DUMP_ID 0x1234123412341234LL
#define _TEST_DUMP_SLEEP 1.0f
#define _TEST_DUMP_NB_TRIES 10
#define _TEST_DUMP_NB_COMMANDS 6
#define _TEST_DUMP_LAST_COMMIT_SEQ 10000000010LL
#define _TEST_DUMP_PREVIEW_LEN 100
#define _TEST_CHECKSUM_LOG_INTERVAL 1

static char *test_commands[] = {
  "10000000002 1234abcd1234abcd REGISTER",
  "10000000003 1234abcd1234abcd ADD 5678 YELLOW",
  "10000000003 1234abcd1234abcd ADD 6789 PURPLE",
  "10000000004 1234abcd1234abcd SET 5678 20 5 0 0",
  "10000000005 1234abcd1234abcd SET 6789 20 10 1 1",
  "10000000010 1234abcd1234abcd NOP",
  "10000000030 1234abcd1234abcd NOP",
  "10000000040 1234abcd1234abcd NOP",
  "10000000050 1234abcd1234abcd NOP",
  "10000000060 1234abcd1234abcd NOP",
  "10000000070 1234abcd1234abcd NOP",
  "10000000080 1234abcd1234abcd NOP",
  "10000000090 1234abcd1234abcd NOP",
  "10000000100 1234abcd1234abcd NOP",
  "10000000200 1234abcd1234abcd NOP",
  "10000000300 1234abcd1234abcd NOP",
  "10000000400 1234abcd1234abcd REMOVE 5678",
  "10000001000 1234abcd1234abcd UNREGISTER",
  "5 foo bar",
  NULL, NULL			// need two NULLs here for there's a +=2 in loop
};

/*
 * Testing functions in bench.c
 */
static int
test_bench ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    float bench_result = 0.0f;

    if (lw6pil_bench (&bench_result, NULL))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("bench=%0.1f"), bench_result);
	ret = 1;
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bench failed"));
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
command_map_func (void *func_data, void *data)
{
  int *ret = (int *) func_data;
  lw6pil_command_t *command = (lw6pil_command_t *) data;
  char *repr;

  repr = lw6pil_command_repr (command);
  if (repr)
    {
      (*ret) = 1;
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("command=\"%s\""), repr);
      LW6SYS_FREE (repr);
    }
}

/*
 * Testing functions in command.c
 */
static int
test_command ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;
    lw6pil_command_t *command;
    char *repr;
    lw6sys_list_t *commands;

    commands = lw6sys_list_new ((lw6sys_free_func_t) lw6pil_command_free);
    if (commands)
      {
	for (i = 0; test_commands[i] && commands; ++i)
	  {
	    command =
	      lw6pil_command_new (test_commands[i], _LW6PIL_MIN_SEQ_0);
	    if (command)
	      {
		repr = lw6pil_command_repr (command);
		if (repr)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("command \"%s\" interpreted as \"%s\""),
				test_commands[i], repr);
		    LW6SYS_FREE (repr);
		    ret = 1;
		  }
		lw6sys_list_push_front (&commands, command);
	      }
	  }
	if (commands)
	  {
	    if (ret)
	      {
		ret = 0;
		lw6sys_sort (&commands, _lw6pil_command_sort_callback);
		lw6sys_list_map (commands, command_map_func, &ret);
	      }
	    if (commands)
	      {
		lw6sys_list_free (commands);
	      }
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_coords ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_rules_t rules;
    lw6sys_whd_t shape;
    float test_x[_TEST_COORDS_NB] =
      { _TEST_COORDS_X1, _TEST_COORDS_X2, _TEST_COORDS_X3, _TEST_COORDS_X4,
      _TEST_COORDS_X5
    };
    float test_y[_TEST_COORDS_NB] =
      { _TEST_COORDS_Y1, _TEST_COORDS_Y2, _TEST_COORDS_Y3, _TEST_COORDS_Y4,
      _TEST_COORDS_Y5
    };
    int i, px, py;
    float x, y, z;

    lw6map_rules_defaults (&rules);
    shape.w = _TEST_COORDS_W;
    shape.h = _TEST_COORDS_H;
    shape.d = _TEST_COORDS_D;

    for (py = 1; py >= -1; --py)
      {
	for (px = 1; px >= -1; --px)
	  {
	    rules.x_polarity = px;
	    rules.y_polarity = py;
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("polarity set to %d,%d"), px,
			py);
	    for (i = 0; i < _TEST_COORDS_NB; ++i)
	      {
		x = test_x[i];
		y = test_y[i];
		z = _TEST_COORDS_Z;
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("coords before fix %0.2f,%0.2f,%0.2f"), x, y,
			    z);
		lw6pil_coords_fix_x10 (&rules, &shape, &x, &y, &z);
		lw6pil_coords_fix (&rules, &shape, &x, &y, &z);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("coords after fix %0.2f,%0.2f,%0.2f"), x, y,
			    z);
	      }
	  }
      }

    ret = 1;
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
print_game_state (lw6ker_game_state_t * game_state, char *text)
{
  char *capture_str;

  capture_str = lw6ker_capture_str (game_state);
  if (capture_str)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("%s round=%d"), text,
		  lw6ker_game_state_get_rounds (game_state));
      printf ("%s", capture_str);
      fflush (stdout);
      LW6SYS_FREE (capture_str);
    }
}

/*
 * Testing local_cursors
 */
static int
test_local_cursors ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6pil_local_cursors_t local_cursors;
    u_int16_t cursor_id = 0;
    int x = 0;
    int y = 0;
    int mouse_controlled = 0;

    ret = 0;
    lw6pil_local_cursors_reset (&local_cursors);
    if (lw6pil_local_cursors_set_xy
	(&local_cursors, _TEST_LOCAL_CURSORS_ID1, _TEST_LOCAL_CURSORS_X1,
	 _TEST_LOCAL_CURSORS_Y1))
      {
	if (lw6pil_local_cursors_set_xy
	    (&local_cursors, _TEST_LOCAL_CURSORS_ID2, _TEST_LOCAL_CURSORS_X2,
	     _TEST_LOCAL_CURSORS_Y2))
	  {
	    if (lw6pil_local_cursors_set_mouse_controlled
		(&local_cursors, _TEST_LOCAL_CURSORS_ID1,
		 _TEST_LOCAL_CURSORS_MOUSE_CONTROLLED1))
	      {
		if (lw6pil_local_cursors_set_mouse_controlled
		    (&local_cursors, _TEST_LOCAL_CURSORS_ID2,
		     _TEST_LOCAL_CURSORS_MOUSE_CONTROLLED2))
		  {
		    if (lw6pil_local_cursors_get_info
			(&local_cursors, &x, &y, &mouse_controlled,
			 _TEST_LOCAL_CURSORS_ID1))
		      lw6sys_log (LW6SYS_LOG_NOTICE,
				  _x_
				  ("cursor %x is at %d,%d mouse_controlled=%d"),
				  _TEST_LOCAL_CURSORS_ID1, x, y,
				  mouse_controlled);
		    if (lw6pil_local_cursors_set_main
			(&local_cursors, _TEST_LOCAL_CURSORS_ID2))
		      {
			if (lw6pil_local_cursors_get_main_info
			    (&local_cursors, &cursor_id, &x, &y,
			     &mouse_controlled))
			  {
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_
					("main cursor %x is at %d,%d mouse_controlled=%d"),
					cursor_id, x, y, mouse_controlled);
			    if (lw6pil_local_cursors_get_cursor
				(&local_cursors, cursor_id))
			      {
				ret = 1;
			      }
			  }
		      }
		  }
	      }
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing functions in dump.c
 */
static int
test_dump ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6pil_pilot_t *pilot = NULL;
    char *repr = NULL;
    char *dump_command = NULL;
    int rounds = 0;
    int dump_rounds = 0;
    u_int32_t checksum = 0;
    u_int32_t dump_checksum = 0;
    int dump_len = 0;
    char dump_preview[_TEST_DUMP_PREVIEW_LEN + 1];
    lw6pil_dump_t dump;
    int i = 0;
    int commands_ok = 0;
    int64_t seq = 0LL;

    lw6pil_dump_zero (&dump);
    level =
      lw6map_builtin_custom (_TEST_MAP_WIDTH, _TEST_MAP_HEIGHT,
			     _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	/*
	 * Here we do not fiddle with the spread_thread parameter, so the checksum
	 * is different than the checksum in the other standard case. This is also
	 * a way to test both algorithms, including the standard one (this case).
	 */
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		pilot =
		  lw6pil_pilot_new (game_state, _LW6PIL_MIN_SEQ_0, 0, NULL);
		if (pilot)
		  {
		    repr = lw6pil_pilot_repr (pilot);
		    if (repr)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("pilot \"%s\" created"), repr);
			LW6SYS_FREE (repr);
		      }
		    lw6pil_pilot_checksum_log_set_interval (pilot,
							    _TEST_CHECKSUM_LOG_INTERVAL);
		    dump_command =
		      lw6pil_dump_command_generate (pilot, _TEST_DUMP_ID);
		    if (dump_command)
		      {
			dump_len = strlen (dump_command);
			memset (dump_preview, 0, _TEST_DUMP_PREVIEW_LEN + 1);
			strncpy (dump_preview, dump_command,
				 _TEST_DUMP_PREVIEW_LEN);
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("dump command with length=%d, %d first chars are \"%s\""),
				    dump_len, _TEST_DUMP_PREVIEW_LEN,
				    dump_preview);

			commands_ok = 1;
			if (lw6pil_pilot_send_command
			    (pilot, dump_command, 1))
			  {
			    for (i = 0;
				 i < _TEST_DUMP_NB_COMMANDS
				 && test_commands[i]; ++i)
			      {
				if (!lw6pil_pilot_send_command
				    (pilot, test_commands[i], 1))
				  {
				    commands_ok = 0;
				  }
			      }
			  }
			else
			  {
			    commands_ok = 0;
			  }
			if (commands_ok && lw6pil_pilot_commit (NULL, pilot))
			  {
			    for (i = 0;
				 i < _TEST_DUMP_NB_TRIES
				 && !lw6pil_dump_exists (&dump); ++i)
			      {
				lw6sys_sleep (_TEST_DUMP_SLEEP);
				lw6pil_pilot_commit (&dump, pilot);
			      }
			  }
			if (lw6pil_dump_exists (&dump))
			  {
			    lw6pil_pilot_checksum_log_set_interval
			      (dump.pilot, _TEST_CHECKSUM_LOG_INTERVAL);
			    lw6pil_pilot_commit (NULL, dump.pilot);
			    seq =
			      lw6pil_pilot_get_last_commit_seq (dump.pilot);
			    if (seq == _TEST_DUMP_LAST_COMMIT_SEQ)
			      {
				lw6sys_log (LW6SYS_LOG_NOTICE,
					    _x_
					    ("got dump from pilot, OK, last_commit_seq=%"
					     LW6SYS_PRINTF_LL "d"),
					    (long long) seq);
				for (i = _TEST_DUMP_NB_COMMANDS;
				     i <= _TEST_SYNC_COMMAND_I
				     && test_commands[i]; ++i)
				  {
				    lw6pil_pilot_send_command (pilot,
							       test_commands
							       [i], 1);
				    lw6pil_pilot_send_command
				      (dump.pilot, test_commands[i], 1);
				  }
				lw6pil_pilot_commit (NULL, pilot);
				lw6pil_pilot_commit (NULL, dump.pilot);

				lw6sys_sleep (_TEST_CYCLE);

				while (lw6pil_pilot_seq2round
				       (pilot,
					lw6pil_pilot_get_reference_current_seq
					(pilot)) < _TEST_BACKUP_ROUND
				       || lw6pil_pilot_seq2round (pilot,
								  lw6pil_pilot_get_reference_current_seq
								  (dump.pilot))
				       < _TEST_BACKUP_ROUND)
				  {
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_
						("waiting for reference round to increase"));
				    lw6sys_sleep (_TEST_CYCLE);
				  }

				lw6pil_pilot_make_backup (pilot);
				lw6pil_pilot_make_backup (dump.pilot);

				lw6pil_pilot_sync_from_backup
				  (game_state, pilot);
				lw6pil_pilot_sync_from_backup
				  (dump.game_state, dump.pilot);

				while ((lw6ker_game_state_get_rounds
					(game_state) <
					_TEST_BACKUP_ROUND)
				       ||
				       (lw6ker_game_state_get_rounds
					(dump.game_state) <
					_TEST_BACKUP_ROUND))
				  {
				    lw6sys_log (LW6SYS_LOG_WARNING,
						_x_
						("waiting for backup at round %d, is your computer slow or what?"),
						_TEST_BACKUP_ROUND);
				    lw6sys_sleep (_TEST_CYCLE);
				    lw6pil_pilot_sync_from_backup
				      (game_state, pilot);
				    lw6pil_pilot_sync_from_backup
				      (dump.game_state, dump.pilot);
				  }

				print_game_state (game_state,
						  _x_
						  ("backup of game_state"));
				print_game_state (game_state,
						  _x_
						  ("backup of dump.game_state"));
				rounds = lw6ker_game_state_get_rounds
				  (game_state);
				dump_rounds = lw6ker_game_state_get_rounds
				  (dump.game_state);
				if (rounds == _TEST_BACKUP_ROUND
				    && dump_rounds == _TEST_BACKUP_ROUND)
				  {
				    checksum =
				      lw6ker_game_state_checksum (game_state);
				    dump_checksum =
				      lw6ker_game_state_checksum
				      (dump.game_state);
				  }
				else
				  {
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_
						("rounds mismatch, should be %d but rounds for game_state is %d and and rounds for dump.game_state is %d"),
						_TEST_BACKUP_ROUND,
						rounds, dump_rounds);
				  }
				if (checksum ==
				    _TEST_DUMP_BACKUP_CHECKSUM
				    && dump_checksum ==
				    _TEST_DUMP_BACKUP_CHECKSUM)
				  {
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_
						("checksum of game_state and dump.game_state at round %d are both %x, OK"),
						_TEST_BACKUP_ROUND,
						_TEST_DUMP_BACKUP_CHECKSUM);
				    ret = 1;
				  }
				else
				  {
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_
						("checksum of game_state and dump.game_state at round %d should be %x but got %x for game_state and %x for dump.game_state"),
						_TEST_BACKUP_ROUND,
						_TEST_DUMP_BACKUP_CHECKSUM,
						checksum, dump_checksum);
				  }
			      }
			    else
			      {
				lw6sys_log (LW6SYS_LOG_WARNING,
					    _x_
					    ("got dump from pilot but seq mismatch last_commit_seq=%"
					     LW6SYS_PRINTF_LL
					     "d and should be %"
					     LW6SYS_PRINTF_LL "d"),
					    (long long) seq,
					    (long long)
					    _TEST_DUMP_LAST_COMMIT_SEQ);
			      }

			    lw6pil_dump_clear (&dump);
			  }
			else
			  {
			    lw6sys_log (LW6SYS_LOG_WARNING,
					_x_ ("didn't get dump from pilot"));
			  }

			LW6SYS_FREE (dump_command);
		      }

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
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in pilot.c
 */
static int
test_pilot ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6pil_pilot_t *pilot = NULL;
    char *repr = NULL;
    int i;
    u_int32_t checksum = 0;

    level =
      lw6map_builtin_custom (_TEST_MAP_WIDTH, _TEST_MAP_HEIGHT,
			     _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	/*
	 * We do the test with spread_thread switched on, this
	 * is the most complicated and error-prone setting.
	 */
	level->param.rules.spread_thread = 1;
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		pilot =
		  lw6pil_pilot_new (game_state, _LW6PIL_MIN_SEQ_0, 0, NULL);
		if (pilot)
		  {
		    repr = lw6pil_pilot_repr (pilot);
		    if (repr)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("pilot \"%s\" start"), repr);

			for (i = 0; test_commands[i]; i += 2)
			  {
			    if (i > 1 && test_commands[i - 1])
			      {
				lw6pil_pilot_send_command (pilot,
							   test_commands[i -
									 1],
							   1);
			      }
			    lw6pil_pilot_send_command (pilot,
						       test_commands[i], 1);
			    if (test_commands[i + 1])
			      {
				lw6pil_pilot_send_command (pilot,
							   test_commands[i +
									 1],
							   0);
				if (test_commands[i + 2])
				  {
				    lw6pil_pilot_send_command (pilot,
							       test_commands[i
									     +
									     2],
							       0);
				    lw6pil_pilot_local_command (pilot,
								test_commands
								[i + 2]);
				  }
			      }
			    lw6pil_pilot_commit (NULL, pilot);
			    lw6sys_sleep (_TEST_CYCLE);

			    if (i == _TEST_SYNC_COMMAND_I)
			      {
				lw6sys_log (LW6SYS_LOG_NOTICE,
					    _x_ ("can_sync: %d"),
					    lw6pil_pilot_can_sync (game_state,
								   pilot));
				lw6pil_pilot_sync_from_reference (game_state,
								  pilot);
				print_game_state (game_state,
						  _x_ ("reference"));

				lw6pil_pilot_sync_from_backup (game_state,
							       pilot);
				print_game_state (game_state,
						  _x_ ("backup 1"));

				lw6pil_pilot_make_backup (pilot);

				lw6pil_pilot_sync_from_backup (game_state,
							       pilot);
				while (lw6ker_game_state_get_rounds
				       (game_state) < _TEST_BACKUP_ROUND)
				  {
				    lw6sys_log (LW6SYS_LOG_WARNING,
						_x_
						("waiting for backup at round %d, is your computer slow or what?"),
						_TEST_BACKUP_ROUND);
				    lw6sys_sleep (_TEST_CYCLE);
				    lw6pil_pilot_sync_from_backup (game_state,
								   pilot);
				  }
				print_game_state (game_state,
						  _x_ ("backup 2"));
				if (lw6ker_game_state_get_rounds (game_state)
				    == _TEST_BACKUP_ROUND)
				  {
				    checksum =
				      lw6ker_game_state_checksum (game_state);
				  }
				print_game_state (lw6pil_pilot_dirty_read
						  (pilot),
						  _x_ ("dirty read"));
			      }
			    else
			      {
				lw6pil_pilot_sync_from_draft (game_state,
							      pilot, i % 2);
				print_game_state (game_state, _x_ ("draft"));
			      }
			  }

			/*
			 * We free things now on purpose, just to check
			 * pilot continues to work when objects used
			 * to build it have disappeared, the "sleep" after
			 * deletion is usefull too.
			 */
			lw6ker_game_struct_free (game_struct);
			game_struct = NULL;
			lw6map_free (level);
			level = NULL;
			lw6ker_game_state_free (game_state);
			game_state = NULL;
			lw6sys_sleep (_TEST_CYCLE);

			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("next_seq for ticks %"
					 LW6SYS_PRINTF_LL "d is %"
					 LW6SYS_PRINTF_LL "d"),
				    (long long) _TEST_CALIBRATE_TICKS1,
				    (long long)
				    lw6pil_pilot_get_next_seq (pilot,
							       _TEST_CALIBRATE_TICKS1));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("next_seq for ticks %"
					 LW6SYS_PRINTF_LL "d is %"
					 LW6SYS_PRINTF_LL "d"),
				    (long long) _TEST_CALIBRATE_TICKS2,
				    (long long)
				    lw6pil_pilot_get_next_seq (pilot,
							       _TEST_CALIBRATE_TICKS2));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("last_commit_seq=%" LW6SYS_PRINTF_LL
					 "d"),
				    (long long)
				    lw6pil_pilot_get_last_commit_seq (pilot));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("reference_current_seq=%"
					 LW6SYS_PRINTF_LL "d"),
				    (long long)
				    lw6pil_pilot_get_reference_current_seq
				    (pilot));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("reference_target_seq=%"
					 LW6SYS_PRINTF_LL "d"),
				    (long long)
				    lw6pil_pilot_get_reference_target_seq
				    (pilot));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("max_seq=%" LW6SYS_PRINTF_LL "d"),
				    (long long)
				    lw6pil_pilot_get_max_seq (pilot));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("pilot \"%s\" stop"), repr);
			lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("is_over=%d"),
				    lw6pil_pilot_is_over (pilot));
			lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("winner=%d"),
				    lw6pil_pilot_get_winner (pilot));
			lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("looser=%d"),
				    lw6pil_pilot_get_looser (pilot));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("checksum at round %d is %x and should be %x"),
				    _TEST_BACKUP_ROUND, checksum,
				    _TEST_BACKUP_CHECKSUM);
			if (checksum == _TEST_BACKUP_CHECKSUM)
			  {
			    ret = 1;
			  }
			LW6SYS_FREE (repr);
		      }
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
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in seq.c
 */
static int
test_seq ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int64_t random_0;

    random_0 = lw6pil_seq_random_0 ();
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("random_0=%" LW6SYS_PRINTF_LL "d"),
		(long long) random_0);

    ret = 1;
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6pil_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @pil module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6pil_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
    }

  ret = test_command () && test_coords () && test_local_cursors ()
    && test_pilot () && test_dump () && test_bench () && test_seq ();

  return ret;
}
