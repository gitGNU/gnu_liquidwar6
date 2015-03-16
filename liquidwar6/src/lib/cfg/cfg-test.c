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

#include "cfg.h"

#define _TEST_CFG "test.xml"
#define _TEST_ARGC 3
#define _TEST_ARGV0 "abc"
#define _TEST_ARGV1 "--fullscreen"
#define _TEST_ARGV2 "--width=800"
#define _TEST_UNIFIED_KEY_YES LW6DEF_WIDTH
#define _TEST_UNIFIED_KEY_NO "unknown"
#define _TEST_EXP_DIR "/tmp"

typedef struct _lw6cfg_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6cfg_test_data_t;

static _lw6cfg_test_data_t _test_data = { 0, NULL };

/*
 * Testing loading/saving
 */
static void
_test_load_save ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    void *context;
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1, _TEST_ARGV2 };
    char *user_dir = NULL;
    char *_test_cfg = NULL;

    context = lw6cfg_init (sys_context, argc, argv);
    if (context)
      {
	user_dir = lw6sys_get_default_user_dir (sys_context);
	if (user_dir)
	  {
	    if (!lw6sys_dir_exists (sys_context, user_dir))
	      {
		lw6sys_create_dir (sys_context, user_dir);
	      }
	    _test_cfg = lw6sys_path_concat (sys_context, user_dir, _TEST_CFG);
	    if (_test_cfg)
	      {
		/*
		 * Note: we don't consider the test as invalid if the
		 * file does not exist, after all, it's a normal behavior
		 * of the game to be launched without any config file.
		 */
		lw6cfg_load (sys_context, context, _test_cfg);
		lw6cfg_save (sys_context, context, _test_cfg);

		LW6SYS_FREE (sys_context, _test_cfg);
	      }
	    LW6SYS_FREE (sys_context, user_dir);
	  }
	lw6cfg_quit (sys_context, context);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Test unified options
 */
static void
_test_unified ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1, _TEST_ARGV2 };
    char *value;

    value = lw6cfg_unified_get_value (sys_context, argc, argv, _TEST_UNIFIED_KEY_YES);
    if (value)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("value for \"%s\" is \"%s\""), _TEST_UNIFIED_KEY_YES, value);
	LW6SYS_FREE (sys_context, value);
      }
    else
      {
	ret = 0;
      }

    value = lw6cfg_unified_get_value (sys_context, argc, argv, _TEST_UNIFIED_KEY_NO);
    if (value)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("key \"%s\" has value \"%s\", should be NULL"), _TEST_UNIFIED_KEY_NO, value);
	LW6SYS_FREE (sys_context, value);
	ret = 0;
      }
    else
      {
	// OK
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Test exp functions
 */
static void
_test_exp ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int exp = 0;
    int new_exp = 0;

    lw6cfg_load_exp (sys_context, _TEST_EXP_DIR, &exp);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("first exp read exp=%d"), exp);
    if (lw6cfg_save_exp (sys_context, _TEST_EXP_DIR, exp))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("exp saved"));
	if (lw6cfg_load_exp (sys_context, _TEST_EXP_DIR, &new_exp))
	  {
	    if (new_exp == exp)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("new exp read exp=%d"), new_exp);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong new exp should be %d but is %d"), exp, new_exp);
		ret = 0;
	      }
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to save exp"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libcfg CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libcfg CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6cfg_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libcfg module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cfg_test_register (lw6sys_context_t * sys_context, int mode)
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
    }

  suite = CU_add_suite ("lw6cfg", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_load_save);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_unified);
      if (mode & LW6SYS_TEST_MODE_FULL_TEST)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_exp);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6cfg_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @cfg module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cfg_test_run (lw6sys_context_t * sys_context, int mode)
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
