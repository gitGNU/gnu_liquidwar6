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

#include "bot.h"

#define TEST_ARGC 1
#define TEST_ARGV0 "prog"

#define TEST_NB_BACKENDS 4

#define TEST_MAP_WIDTH 72
#define TEST_MAP_HEIGHT 24
#define TEST_MAP_NB_LAYERS 2
#define TEST_MAP_NOISE_PERCENT 10
#define TEST_NB_ROUNDS 1000
#define TEST_ROUNDS_STEP 10
#define TEST_NODE_ID 0x1234123412341234LL
#define TEST_CURSOR_ID1 0x1234
#define TEST_CURSOR_ID2 0x2345
#define TEST_CURSOR_ID3 0x3456
#define TEST_CURSOR_COLOR1 0
#define TEST_CURSOR_COLOR2 1
#define TEST_CURSOR_COLOR3 2
#define TEST_SPEED 1.0f
#define TEST_IQ 100

static int
new_data (lw6map_level_t ** level,
	  lw6ker_game_struct_t ** game_struct,
	  lw6ker_game_state_t ** game_state)
{
  int ret = 0;

  *level =
    lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			   TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
  *game_struct = lw6ker_game_struct_new (*level, NULL);
  *game_state = lw6ker_game_state_new (*game_struct, NULL);

  if (*game_state)
    {
      lw6ker_game_state_register_node (*game_state, TEST_NODE_ID);
      lw6ker_game_state_add_cursor (*game_state, TEST_NODE_ID,
				    TEST_CURSOR_ID1, TEST_CURSOR_COLOR1);
      lw6ker_game_state_add_cursor (*game_state, TEST_NODE_ID,
				    TEST_CURSOR_ID2, TEST_CURSOR_COLOR2);
      lw6ker_game_state_add_cursor (*game_state, TEST_NODE_ID,
				    TEST_CURSOR_ID3, TEST_CURSOR_COLOR3);
    }

  ret = (*level && *game_struct && *game_state);

  return ret;
}

static void
free_data (lw6map_level_t * level,
	   lw6ker_game_struct_t * game_struct,
	   lw6ker_game_state_t * game_state)
{
  lw6ker_game_state_free (game_state);
  lw6ker_game_struct_free (game_struct);
  lw6map_free (level);
}

static int
test_backend (lw6bot_backend_t * backend)
{
  int ret = 1;

  LW6SYS_TEST_FUNCTION_BEGIN;

  lw6map_level_t *level = NULL;
  lw6ker_game_struct_t *game_struct = NULL;
  lw6ker_game_state_t *game_state = NULL;
  lw6bot_seed_t bot_seed;
  int x = 0, y = 0;
  int i;
  char *capture_str = NULL;
  char *repr = NULL;
  lw6ker_cursor_t cursor;

  memset (&bot_seed, 0, sizeof (lw6bot_seed_t));

  if (new_data (&level, &game_struct, &game_state))
    {
      bot_seed.param.speed = TEST_SPEED;
      bot_seed.param.iq = TEST_IQ;
      bot_seed.param.cursor_id = TEST_CURSOR_ID3;
      bot_seed.game_state = game_state;
      bot_seed.dirty_read = LW6PIL_DIRTY_READ_NEVER;
      if (lw6bot_init (backend, &bot_seed))
	{
	  while (lw6ker_game_state_get_rounds (game_state) < TEST_NB_ROUNDS)
	    {
	      lw6bot_next_move (backend, &x, &y);
	      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("round %d moved to %d,%d"),
			  lw6ker_game_state_get_rounds (game_state), x, y);
	      lw6ker_cursor_reset (&cursor);
	      cursor.node_id = TEST_NODE_ID;
	      cursor.cursor_id = TEST_CURSOR_ID3;
	      cursor.pos.x = x;
	      cursor.pos.y = y;
	      lw6ker_game_state_set_cursor (game_state, &cursor);
	      for (i = 0; i < TEST_ROUNDS_STEP; ++i)
		{
		  lw6ker_game_state_do_round (game_state);
		}
	    }
	  repr = lw6bot_repr (backend);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("bot repr is \"%s\""),
			  repr);
	      LW6SYS_FREE (repr);
	    }
	  capture_str = lw6ker_capture_str (game_state);
	  if (capture_str)
	    {
	      printf ("%s", capture_str);
	      fflush (stdout);
	      LW6SYS_FREE (capture_str);
	    }
	  lw6bot_quit (backend);
	}
      free_data (level, game_struct, game_state);
    }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6bot_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @bot module test suite. Will try several engines
 * and query basic moves.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6bot_test (int mode)
{
  int ret = 0;
  int argc = TEST_ARGC;
  char *argv[TEST_ARGC] = { TEST_ARGV0 };
  lw6bot_backend_t *backend[TEST_NB_BACKENDS];
  int i;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
      lw6dyn_test (mode);
    }

  backend[0] = lw6bot_create_backend (argc, argv, "brute");
  backend[1] = lw6bot_create_backend (argc, argv, "follow");
  backend[2] = lw6bot_create_backend (argc, argv, "idiot");
  backend[3] = lw6bot_create_backend (argc, argv, "random");

  ret = 1;

  for (i = 0; i < TEST_NB_BACKENDS; ++i)
    {
      if (backend[i])
	{
	  ret = test_backend (backend[i]) && ret;
	  lw6bot_destroy_backend (backend[i]);
	}
    }

  return ret;
}
