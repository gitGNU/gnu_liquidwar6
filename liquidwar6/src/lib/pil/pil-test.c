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
#endif

#include "pil.h"
#include "pil-internal.h"

#define TEST_MAP_WIDTH 45
#define TEST_MAP_HEIGHT 15
#define TEST_MAP_NB_LAYERS 7
#define TEST_MAP_NOISE_PERCENT 5
#define TEST_CYCLE 0.5f
#define TEST_NB_PILOTS 3
#define TEST_SYNC_COMMAND_I 10
#define TEST_BACKUP_ROUND 70
#define TEST_BACKUP_CHECKSUM 0xe771d39e
#define TEST_CALIBRATE_TICKS1 12345
#define TEST_CALIBRATE_TICKS2 23456
#define TEST_COORDS_NB 5
#define TEST_COORDS_W 100
#define TEST_COORDS_H 50
#define TEST_COORDS_D 1
#define TEST_COORDS_X1 66.0f
#define TEST_COORDS_Y1 33.0f
#define TEST_COORDS_X2 130.0f
#define TEST_COORDS_Y2 20.0f
#define TEST_COORDS_X3 -30.3f
#define TEST_COORDS_Y3 20.2f
#define TEST_COORDS_X4 -10.1f
#define TEST_COORDS_Y4 -5.5f
#define TEST_COORDS_X5 1000.1f
#define TEST_COORDS_Y5 51.1f
#define TEST_COORDS_Z 0.0f
#define _TEST_LOCAL_CURSORS_ID1 0x1234
#define _TEST_LOCAL_CURSORS_X1 15
#define _TEST_LOCAL_CURSORS_Y1 30
#define _TEST_LOCAL_CURSORS_MOUSE_CONTROLLED1 1
#define _TEST_LOCAL_CURSORS_ID2 0x2345
#define _TEST_LOCAL_CURSORS_X2 45
#define _TEST_LOCAL_CURSORS_Y2 90
#define _TEST_LOCAL_CURSORS_MOUSE_CONTROLLED2 0

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
    float test_x[TEST_COORDS_NB] =
      { TEST_COORDS_X1, TEST_COORDS_X2, TEST_COORDS_X3, TEST_COORDS_X4,
      TEST_COORDS_X5
    };
    float test_y[TEST_COORDS_NB] =
      { TEST_COORDS_Y1, TEST_COORDS_Y2, TEST_COORDS_Y3, TEST_COORDS_Y4,
      TEST_COORDS_Y5
    };
    int i, px, py;
    float x, y, z;

    lw6map_rules_defaults (&rules);
    shape.w = TEST_COORDS_W;
    shape.h = TEST_COORDS_H;
    shape.d = TEST_COORDS_D;

    for (py = 1; py >= -1; --py)
      {
	for (px = 1; px >= -1; --px)
	  {
	    rules.x_polarity = px;
	    rules.y_polarity = py;
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("polarity set to %d,%d"), px,
			py);
	    for (i = 0; i < TEST_COORDS_NB; ++i)
	      {
		x = test_x[i];
		y = test_y[i];
		z = TEST_COORDS_Z;
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
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
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
			    lw6pil_pilot_commit (pilot);
			    lw6sys_sleep (TEST_CYCLE);

			    if (i == TEST_SYNC_COMMAND_I)
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
				       (game_state) < TEST_BACKUP_ROUND)
				  {
				    lw6sys_log (LW6SYS_LOG_WARNING,
						_x_
						("waiting for backup at round %d, is your computer slow or what?"),
						TEST_BACKUP_ROUND);
				    lw6sys_sleep (TEST_CYCLE);
				    lw6pil_pilot_sync_from_backup (game_state,
								   pilot);
				  }
				print_game_state (game_state,
						  _x_ ("backup 2"));
				if (lw6ker_game_state_get_rounds (game_state)
				    == TEST_BACKUP_ROUND)
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
			lw6sys_sleep (TEST_CYCLE);

			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("next_seq for ticks %"
					 LW6SYS_PRINTF_LL "d is %"
					 LW6SYS_PRINTF_LL "d"),
				    (long long) TEST_CALIBRATE_TICKS1,
				    (long long)
				    lw6pil_pilot_get_next_seq (pilot,
							       TEST_CALIBRATE_TICKS1));
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("next_seq for ticks %"
					 LW6SYS_PRINTF_LL "d is %"
					 LW6SYS_PRINTF_LL "d"),
				    (long long) TEST_CALIBRATE_TICKS2,
				    (long long)
				    lw6pil_pilot_get_next_seq (pilot,
							       TEST_CALIBRATE_TICKS2));
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
				    TEST_BACKUP_ROUND, checksum,
				    TEST_BACKUP_CHECKSUM);
			if (checksum == TEST_BACKUP_CHECKSUM)
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
    && test_pilot () && test_bench () && test_seq ();

  return ret;
}
