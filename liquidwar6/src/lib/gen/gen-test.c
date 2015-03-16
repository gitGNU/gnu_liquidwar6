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
#endif

#include <CUnit/CUnit.h>

#include "gen-internal.h"

#define _TEST_SEED "abcdef0123456789"
#define _TEST_WIDTH 320
#define _TEST_HEIGHT 180

#define _TEST_SEED_SHORT_SRC "abc{}"
#define _TEST_SEED_SHORT_DST "abcfhabcfhabcfha"
#define _TEST_SEED_LONG_SRC "FOO' BAR' 1234567890 you'll never read this!"
#define _TEST_SEED_LONG_DST "foo3wbar3w123456"

typedef struct _lw6gen_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6gen_test_data_t;

static _lw6gen_test_data_t _test_data = { 0, NULL };

/*
 * test create
 */
static void
_test_create ()
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("creating map from seed \"%s\" with size %dx%d"), _TEST_SEED, _TEST_WIDTH, _TEST_HEIGHT);
    level = lw6gen_create_from_seed (sys_context, _TEST_SEED, _TEST_WIDTH, _TEST_HEIGHT);
    if (level)
      {
	repr = lw6map_repr (sys_context, level);
	if (repr)
	  {
	    if (level->body.shape.w == _TEST_WIDTH && level->body.shape.h == _TEST_HEIGHT)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("built map \"%s\" from seed \"%s\""), repr, _TEST_SEED);
		LW6SYS_FREE (sys_context, repr);

		ret = 1;
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_
			    ("map created from seed \"%s\" with bad size %dx%d, should have been %dx%d"),
			    _TEST_SEED, level->body.shape.w, level->body.shape.h, _TEST_WIDTH, _TEST_HEIGHT);
	      }
	  }
	lw6map_free (sys_context, level);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create map from seed \"%s\" with size %dx%d"), _TEST_SEED, _TEST_WIDTH, _TEST_HEIGHT);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * test seed
 */
static void
_test_seed ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *seed = NULL;

    seed = lw6gen_seed_new (sys_context);
    if (seed)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("generated new random seed \"%s\""), seed);
	LW6SYS_FREE (sys_context, seed);
      }
    else
      {
	ret = 0;
      }

    seed = lw6gen_seed_normalize (sys_context, _TEST_SEED_SHORT_SRC);
    if (seed)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("normalizing short seed \"%s\" -> \"%s\""), _TEST_SEED_SHORT_SRC, seed);
	if (lw6sys_str_is_same (sys_context, seed, _TEST_SEED_SHORT_DST))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ouput is correct"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad ouput, should have been \"%s\" but is \"%s\""), _TEST_SEED_SHORT_DST, seed);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, seed);
      }
    else
      {
	ret = 0;
      }

    seed = lw6gen_seed_normalize (sys_context, _TEST_SEED_LONG_SRC);
    if (seed)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("normalizing long seed \"%s\" -> \"%s\""), _TEST_SEED_LONG_SRC, seed);
	if (lw6sys_str_is_same (sys_context, seed, _TEST_SEED_LONG_DST))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ouput is correct"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad ouput, should have been \"%s\" but is \"%s\""), _TEST_SEED_LONG_DST, seed);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, seed);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libgen CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libgen CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6gen_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libgen module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gen_test_register (lw6sys_context_t * sys_context, int mode)
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
    }

  suite = CU_add_suite ("lw6gen", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_create);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_seed);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6gen_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @gen module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gen_test_run (lw6sys_context_t * sys_context, int mode)
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
