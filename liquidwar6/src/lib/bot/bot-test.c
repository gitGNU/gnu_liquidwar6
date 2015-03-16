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

#include <CUnit/CUnit.h>

#include "bot.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

#define _TEST_MAP_WIDTH 72
#define _TEST_MAP_HEIGHT 24
#define _TEST_MAP_NB_LAYERS 2
#define _TEST_MAP_NOISE_PERCENT 10
#define _TEST_NB_ROUNDS 1000
#define _TEST_ROUNDS_STEP 10
#define _TEST_NODE_ID 0x1234123412341234LL
#define _TEST_CURSOR_ID1 0x1234
#define _TEST_CURSOR_ID2 0x2345
#define _TEST_CURSOR_ID3 0x3456
#define _TEST_CURSOR_COLOR1 0
#define _TEST_CURSOR_COLOR2 1
#define _TEST_CURSOR_COLOR3 2
#define _TEST_SPEED 1.0f
#define _TEST_IQ 100

typedef struct _lw6bot_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
  lw6bot_backend_t *backend;
} _lw6bot_test_data_t;

static _lw6bot_test_data_t _test_data = { 0, NULL, NULL };

static int
_new_data (lw6sys_context_t * sys_context, lw6map_level_t ** level, lw6ker_game_struct_t ** game_struct, lw6ker_game_state_t ** game_state)
{
  int ret = 0;

  *level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
  *game_struct = lw6ker_game_struct_new (sys_context, *level, NULL);
  *game_state = lw6ker_game_state_new (sys_context, *game_struct, NULL);

  if (*game_state)
    {
      lw6ker_game_state_register_node (sys_context, *game_state, _TEST_NODE_ID);
      lw6ker_game_state_add_cursor (sys_context, *game_state, _TEST_NODE_ID, _TEST_CURSOR_ID1, _TEST_CURSOR_COLOR1);
      lw6ker_game_state_add_cursor (sys_context, *game_state, _TEST_NODE_ID, _TEST_CURSOR_ID2, _TEST_CURSOR_COLOR2);
      lw6ker_game_state_add_cursor (sys_context, *game_state, _TEST_NODE_ID, _TEST_CURSOR_ID3, _TEST_CURSOR_COLOR3);
    }

  ret = (*level && *game_struct && *game_state);

  return ret;
}

static void
_free_data (lw6sys_context_t * sys_context, lw6map_level_t * level, lw6ker_game_struct_t * game_struct, lw6ker_game_state_t * game_state)
{
  lw6ker_game_state_free (sys_context, game_state);
  lw6ker_game_struct_free (sys_context, game_struct);
  lw6map_free (sys_context, level);
}

static void
_test_backend ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

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
  lw6bot_backend_t *backend = _test_data.backend;

  memset (&bot_seed, 0, sizeof (lw6bot_seed_t));

  if (_new_data (sys_context, &level, &game_struct, &game_state))
    {
      bot_seed.param.speed = _TEST_SPEED;
      bot_seed.param.iq = _TEST_IQ;
      bot_seed.param.cursor_id = _TEST_CURSOR_ID3;
      bot_seed.game_state = game_state;
      bot_seed.dirty_read = LW6PIL_DIRTY_READ_NEVER;
      if (lw6bot_init (sys_context, backend, &bot_seed))
	{
	  while (lw6ker_game_state_get_rounds (sys_context, game_state) < _TEST_NB_ROUNDS)
	    {
	      lw6bot_next_move (sys_context, backend, &x, &y);
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("round %d moved to %d,%d"), lw6ker_game_state_get_rounds (sys_context, game_state), x, y);
	      lw6ker_cursor_reset (sys_context, &cursor);
	      cursor.node_id = _TEST_NODE_ID;
	      cursor.cursor_id = _TEST_CURSOR_ID3;
	      cursor.pos.x = x;
	      cursor.pos.y = y;
	      lw6ker_game_state_set_cursor (sys_context, game_state, &cursor);
	      for (i = 0; i < _TEST_ROUNDS_STEP; ++i)
		{
		  lw6ker_game_state_do_round (sys_context, game_state);
		}
	    }
	  repr = lw6bot_repr (sys_context, backend);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("bot repr is \"%s\""), repr);
	      LW6SYS_FREE (sys_context, repr);
	    }
	  capture_str = lw6ker_capture_str (sys_context, game_state);
	  if (capture_str)
	    {
	      if (lw6sys_log_get_console_state (sys_context))
		{
		  printf ("%s", capture_str);
		  fflush (stdout);
		}
	      LW6SYS_FREE (sys_context, capture_str);
	    }
	  lw6bot_quit (sys_context, backend);
	}
      _free_data (sys_context, level, game_struct, game_state);
    }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init_brute ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libbot-brute CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6bot_create_backend (sys_context, argc, argv, "brute");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_brute ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libbot-brute CUnit test suite"));

  if (_test_data.backend)
    {
      lw6bot_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_init_follow ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libbot-follow CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6bot_create_backend (sys_context, argc, argv, "follow");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_follow ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libbot-follow CUnit test suite"));

  if (_test_data.backend)
    {
      lw6bot_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_init_idiot ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libbot-idiot CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6bot_create_backend (sys_context, argc, argv, "idiot");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_idiot ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libbot-idiot CUnit test suite"));

  if (_test_data.backend)
    {
      lw6bot_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_init_random ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libbot-random CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6bot_create_backend (sys_context, argc, argv, "random");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_random ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libbot-random CUnit test suite"));

  if (_test_data.backend)
    {
      lw6bot_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

/**
 * lw6bot_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libbot module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6bot_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6map_test_register (sys_context, mode);
      lw6ker_test_register (sys_context, mode);
      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
    }

  suite = CU_add_suite ("lw6bot-brute", _setup_init_brute, _setup_quit_brute);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_backend);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  suite = CU_add_suite ("lw6bot-follow", _setup_init_follow, _setup_quit_follow);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_backend);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  suite = CU_add_suite ("lw6bot-idiot", _setup_init_idiot, _setup_quit_idiot);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_backend);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  suite = CU_add_suite ("lw6bot-random", _setup_init_random, _setup_quit_random);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_backend);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6bot_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @bot module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6bot_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
