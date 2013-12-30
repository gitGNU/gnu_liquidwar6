/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#include "mat.h"

typedef struct _lw6mat_test_data_s
{
  int ret;
} _lw6mat_test_data_t;

static _lw6mat_test_data_t _test_data = { 0 };

/*
 * Testing functions in ... .c
 */
static void
_test_dummy ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("todo..."));
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("init libmat CUnit test suite"));
  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("quit libmat CUnit test suite"));
  return CUE_SUCCESS;
}

/**
 * lw6mat_test_register
 *
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libmat module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6mat_test_register (int mode)
{
  int ret = 1;
  CU_Suite *suite;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (mode);
    }

  suite = CU_add_suite ("lw6mat", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dummy);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to add CUnit test suite, error msg is \"%s\""),
		  CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6mat_test_run
 *
 * @mode: test mode (bitmask)
 *
 * Runs the @mat module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6mat_test_run (int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  if (lw6sys_cunit_run_tests (mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
