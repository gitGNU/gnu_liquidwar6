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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ker.h"

#define TEST_MAP_WIDTH 72
#define TEST_MAP_HEIGHT 24
#define TEST_MAP_NB_LAYERS 5
#define TEST_MAP_NOISE_PERCENT 20
#define TEST_NB_ROUNDS 100
#define TEST_GAME_STRUCT_CHECKSUM 0xbcc67d4d
#define TEST_GAME_STATE_CHECKSUM 0xa3c8ad4d
#define TEST_GAME_STATE_POPULATE_CHECKSUM 0xf714c77c
#define TEST_GAME_STATE_ALGORITHM_CHECKSUM 0x4c403a8c
#define TEST_NODE_ID 0x1234123412341234LL
#define TEST_CURSOR1_ID 0x1234
#define TEST_CURSOR2_ID 0x2345
#define TEST_CURSOR3_ID 0x3456
#define TEST_COLOR1 LW6MAP_TEAM_COLOR_RED
#define TEST_COLOR2 LW6MAP_TEAM_COLOR_GREEN
#define TEST_COLOR3 LW6MAP_TEAM_COLOR_BLUE
#define TEST_TEAM_MASK_SEED 123
#define TEST_HISTORY_TEAM 0

static void
print_game_struct_repr (lw6ker_game_struct_t * game_struct)
{
  char *repr = NULL;

  repr = lw6ker_game_struct_repr (game_struct);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("game_struct repr is \"%s\""),
		  repr);
      LW6SYS_FREE (repr);
    }
}

static void
print_game_state_repr (lw6ker_game_state_t * game_state)
{
  char *repr = NULL;
  char *capture = NULL;

  repr = lw6ker_game_state_repr (game_state);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("game_state repr is \"%s\""), repr);
      LW6SYS_FREE (repr);
    }
  lw6ker_map_state_print_debug (&(game_state->map_state));
  capture = lw6ker_capture_str (game_state);
  if (capture)
    {
      printf ("%s", capture);
      fflush (stdout);
      LW6SYS_FREE (capture);
    }
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("time_elapsed=%d time_left=%d"),
	      lw6ker_game_state_get_time_elapsed (game_state),
	      lw6ker_game_state_get_time_left (game_state));
}

static int
test_struct ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    int checksum;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    checksum = lw6ker_game_struct_checksum (game_struct);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("game struct checksum is %08x and should be %08x"),
			checksum, TEST_GAME_STRUCT_CHECKSUM);
	    lw6ker_game_struct_free (game_struct);
	    ret = (checksum == TEST_GAME_STRUCT_CHECKSUM);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_state ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    int checksum;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("is_over returns %d"),
			    lw6ker_game_state_is_over (game_state));
		print_game_state_repr (game_state);
		checksum = lw6ker_game_state_checksum (game_state);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("game state checksum is %08x and should be %08x"),
			    checksum, TEST_GAME_STATE_CHECKSUM);
		ret = lw6ker_map_state_sanity_check (&(game_state->map_state))
		  && (checksum == TEST_GAME_STATE_CHECKSUM);
		lw6ker_game_state_free (game_state);
	      }
	    lw6ker_game_struct_free (game_struct);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_population ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    int checksum;

    ret = 0;

    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		print_game_state_repr (game_state);
		lw6ker_game_state_register_node (game_state, TEST_NODE_ID);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR1_ID, TEST_COLOR1);
		print_game_state_repr (game_state);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR2_ID, TEST_COLOR2);
		print_game_state_repr (game_state);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR3_ID, TEST_COLOR3);
		print_game_state_repr (game_state);
		lw6ker_game_state_remove_cursor (game_state, TEST_NODE_ID,
						 TEST_CURSOR2_ID);
		print_game_state_repr (game_state);
		checksum = lw6ker_game_state_checksum (game_state);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("game state checksum is %08x and should be %08x"),
			    checksum, TEST_GAME_STATE_POPULATE_CHECKSUM);
		ret = lw6ker_map_state_sanity_check (&(game_state->map_state))
		  && (checksum == TEST_GAME_STATE_POPULATE_CHECKSUM);
		lw6ker_game_state_free (game_state);
	      }
	    lw6ker_game_struct_free (game_struct);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_algorithm ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    lw6ker_score_array_t score_array;
    int checksum;
    int i;
    int cursor_x, cursor_y;
    u_int64_t node_id = 0;
    char letter = ' ';
    int team_color = -1;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		print_game_state_repr (game_state);
		lw6ker_game_state_register_node (game_state, TEST_NODE_ID);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR1_ID, TEST_COLOR1);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR2_ID, TEST_COLOR2);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR3_ID, TEST_COLOR3);
		print_game_state_repr (game_state);
		cursor_x = game_state->map_state.shape.w / 2;
		cursor_y = game_state->map_state.shape.h / 2;
		lw6ker_game_state_set_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR1_ID, cursor_x,
					      cursor_y);
		lw6ker_game_state_set_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR2_ID, cursor_x,
					      cursor_y);
		lw6ker_game_state_set_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR3_ID, cursor_x,
					      cursor_y);
		lw6ker_game_state_get_cursor_info (game_state,
						   TEST_CURSOR1_ID,
						   &node_id, &letter,
						   &team_color, &cursor_x,
						   &cursor_y);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("cursor %x letter='%c' color=%d x=%d y=%d"),
			    TEST_CURSOR1_ID, letter, team_color, cursor_x,
			    cursor_y);
		for (i = 0; i < TEST_NB_ROUNDS; ++i)
		  {
		    lw6ker_game_state_do_round (game_state);
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("round %d, game_state checksum=%08x"),
				game_state->rounds,
				lw6ker_game_state_checksum (game_state));
		  }
		lw6ker_score_array_update (&score_array, game_state);
		for (i = 0; i < score_array.nb_scores; ++i)
		  {
		    if (score_array.scores[i].fighters_absolute > 0)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("order=%d team_color=%d score=%d%%"),
				    i, score_array.scores[i].team_color,
				    score_array.scores[i].fighters_percent);
		      }
		  }
		for (i = 0;
		     i < lw6sys_min (LW6KER_HISTORY_SIZE,
				     lw6ker_game_state_get_rounds
				     (game_state)); ++i)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("history entry %d for team %d is global=%d latest=%d"),
				i, TEST_HISTORY_TEAM,
				lw6ker_game_state_get_global_history
				(game_state, i, TEST_HISTORY_TEAM),
				lw6ker_game_state_get_latest_history
				(game_state, i, TEST_HISTORY_TEAM));
		  }
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("max history entry global=%d latest=%d"),
			    lw6ker_game_state_get_global_history_max
			    (game_state),
			    lw6ker_game_state_get_latest_history_max
			    (game_state));
		print_game_state_repr (game_state);
		checksum = lw6ker_game_state_checksum (game_state);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("game state checksum is %08x and should be %08x"),
			    checksum, TEST_GAME_STATE_ALGORITHM_CHECKSUM);
		ret = lw6ker_map_state_sanity_check (&(game_state->map_state))
		  && (checksum == TEST_GAME_STATE_ALGORITHM_CHECKSUM);
		lw6ker_game_state_free (game_state);
	      }
	    lw6ker_game_struct_free (game_struct);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_dup ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    lw6ker_game_struct_t *game_struct2;
    lw6ker_game_state_t *game_state2;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_struct2 = lw6ker_game_struct_dup (game_struct, NULL);
	    lw6ker_game_struct_free (game_struct);
	    game_struct = NULL;
	    print_game_struct_repr (game_struct2);
	    game_state = lw6ker_game_state_new (game_struct2, NULL);
	    if (game_state)
	      {
		print_game_state_repr (game_state);
		game_state2 = lw6ker_game_state_dup (game_state, NULL);
		lw6ker_game_state_free (game_state);
		game_state = NULL;
		print_game_state_repr (game_state2);
		lw6ker_game_state_free (game_state2);
		game_state2 = NULL;
		ret = 1;
	      }
	    lw6ker_game_struct_free (game_struct2);
	    game_struct2 = NULL;
	  }
	lw6map_free (level);
	level = NULL;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_team_mask ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int32_t even = 0;
    u_int32_t odd = 0;
    int i;

    lw6ker_team_mask_get (&even, &odd, TEST_TEAM_MASK_SEED);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("even team mask %x"), even);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("odd team mask %x"), odd);
    for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
      {
	if (lw6ker_team_mask_is_concerned (i, even))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("team %d is even"), i);
	  }
	else if (lw6ker_team_mask_is_concerned (i, odd))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("team %d is odd"), i);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("team mask inconsistency for %d"), i);
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6ker_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @ker module test suite. Will perform deep checksums
 * and *really* check many things. If this passes, the algorithm
 * is fine. What could make it fail is a serious bug and/or some
 * weird combination of endianess, byte alignment...
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6ker_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6map_test (mode);
    }

  ret = test_team_mask () && test_struct () && test_state ()
    && test_population () && test_algorithm () && test_dup ();

  return ret;
}
