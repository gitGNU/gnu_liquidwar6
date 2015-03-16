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

#include "hlp.h"

#define _TEST_KEYWORD LW6DEF_PREFIX
#define _TEST_MAX_CREDITS 50

typedef struct _lw6hlp_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6hlp_test_data_t;

static _lw6hlp_test_data_t _test_data = { 0, NULL };

/*
 * Testing about
 */
static void
_test_about ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const char *help_string = NULL;
    lw6hlp_type_t type;
    const char *default_value;
    int min_value;
    int max_value;

    if (!lw6hlp_is_documented (sys_context, _TEST_KEYWORD))
      {
	ret = 0;
      }

    help_string = lw6hlp_about (sys_context, NULL, NULL, NULL, NULL, _TEST_KEYWORD);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("help for \"%s\" is \"%s\""), _TEST_KEYWORD, help_string);
    type = lw6hlp_get_type (sys_context, _TEST_KEYWORD);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("type for \"%s\" is \"%d\""), _TEST_KEYWORD, (int) type);
    default_value = lw6hlp_get_default_value (sys_context, _TEST_KEYWORD);
    if (default_value)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("default value for \"%s\" is \"%s\""), _TEST_KEYWORD, default_value);
      }
    min_value = lw6hlp_get_min_value (sys_context, _TEST_KEYWORD);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("min value for \"%s\" is \"%d\""), _TEST_KEYWORD, min_value);
    max_value = lw6hlp_get_max_value (sys_context, _TEST_KEYWORD);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("max value for \"%s\" is \"%d\""), _TEST_KEYWORD, max_value);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing credits
 */
static void
_test_credits ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;
    char *credits = NULL;

    for (i = 0; i < _TEST_MAX_CREDITS; ++i)
      {
	credits = lw6hlp_get_credits (sys_context, i);
	if (credits)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("credit line %d: \"%s\""), i, credits);
	    LW6SYS_FREE (sys_context, credits);
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing print
 */
static void
_test_print ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_list_t *list;

    list = lw6hlp_list_quick (sys_context);
    if (list)
      {
	lw6hlp_print_keyword (sys_context, &list, stdout);
	lw6sys_list_free (sys_context, list);
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("now showing what \"--about=%s\" would look like"), _TEST_KEYWORD);
    lw6hlp_print_about (sys_context, _TEST_KEYWORD, stdout);
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libhlp CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libhlp CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6hlp_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libhlp module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6hlp_test_register (lw6sys_context_t * sys_context, int mode)
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

  suite = CU_add_suite ("lw6hlp", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_about);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_credits);
      if (!(mode & LW6SYS_TEST_MODE_INTERACTIVE))
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_print);
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
 * lw6hlp_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @hlp module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6hlp_test_run (lw6sys_context_t * sys_context, int mode)
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
