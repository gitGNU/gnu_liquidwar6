/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#include "sim.h"
#include "sim-internal.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

#define _TEST_SIMULATE_NB_TEAMS 2
#define _TEST_SIMULATE_BOT_BACKEND "random"
#define _TEST_MASK_NB_TEAMS 3

typedef struct _lw6sim_test_data_s
{
  int ret;
} _lw6sim_test_data_t;

static _lw6sim_test_data_t _test_data = { 0 };

/*
 * Testing functions in mask.c
 */
static void
_test_mask ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i, n;
    int mask, valid;

    for (i = 0; i <= LW6MAP_MAX_NB_TEAMS; ++i)
      {
	mask = _lw6sim_mask_get_max (i);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("max mask for %d is %d"), i, mask);
      }

    n = _lw6sim_mask_get_max (_TEST_MASK_NB_TEAMS);
    for (i = _LW6SIM_MASK_MIN; i < n; ++i)
      {
	valid = _lw6sim_mask_is_valid (i);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("mask=%d valid=%d"),
		    i, valid);
      }

    ret = 1;
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in simulate.c
 */
static void
_test_simulate ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };
    lw6sim_results_t results;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("simulating %d teams with bot backend \"%s\""),
		_TEST_SIMULATE_NB_TEAMS, _TEST_SIMULATE_BOT_BACKEND);
    lw6sim_results_zero (&results);
    ret =
      lw6sim_simulate (argc, argv, &results, _TEST_SIMULATE_NB_TEAMS,
		       _TEST_SIMULATE_BOT_BACKEND);
    if (ret)
      {
	if (lw6sys_log_get_console_state ())
	  {
	    lw6sim_print (&results, stderr);
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
	      _x_ ("init libsim CUnit test suite"));
  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
	      _x_ ("quit libsim CUnit test suite"));
  return CUE_SUCCESS;
}

/**
 * lw6sim_test_register
 *
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libsim module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sim_test_register (int mode)
{
  int ret = 1;
  CU_Suite *suite;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (mode);
      lw6map_test_register (mode);
      lw6ker_test_register (mode);
      lw6pil_test_register (mode);
      lw6bot_test_register (mode);
    }

  suite = CU_add_suite ("lw6sim", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_mask);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_simulate);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("unable to add CUnit test suite, error msg is \"%s\""),
		  CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6sim_test_run
 *
 * @mode: test mode (bitmask)
 *
 * Runs the @sim module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sim_test_run (int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  if (lw6sys_cunit_run_tests (mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
