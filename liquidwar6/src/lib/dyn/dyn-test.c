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

#include "dyn.h"

#define _TEST_DYN_TOP_LEVEL_LIB "gfx"
#define _TEST_DYN_BACKEND_NAME "gl1"
#define _TEST_DYN_SHARED_NAME "sdl"

#define _TEST_ARGC 2
#define _TEST_ARGV0 "program"
#define _TEST_ARGV1 "--toto=this-is-a-test"

typedef struct _lw6dyn_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6dyn_test_data_t;

static _lw6dyn_test_data_t _test_data = { 0, NULL };

static void
_display_module (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  int *found;
  const char *id;
  char *name;

  found = (int *) func_data;
  id = key;
  name = (char *) value;
  (*found) = 1;
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("module \"%s/%s\" \"%s\" found"), _TEST_DYN_TOP_LEVEL_LIB, id, name);
}

static void
_test_list ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_assoc_t *list = NULL;
    int found = 0;
    const int argc = _TEST_ARGC;
    const char *argv[] = { _TEST_ARGV0, _TEST_ARGV1 };

    list = lw6dyn_list_backends (sys_context, argc, argv, _TEST_DYN_TOP_LEVEL_LIB);
    if (list)
      {
	lw6sys_assoc_map (sys_context, list, _display_module, (void *) &found);
	lw6sys_assoc_free (sys_context, list);
      }
    if (!found)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no module found for \"%s\""), _TEST_DYN_TOP_LEVEL_LIB);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing path functions
 */
static void
_test_path ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *library_path;
    const int argc = _TEST_ARGC;
    const char *argv[] = { _TEST_ARGV0, _TEST_ARGV1 };

    library_path = lw6dyn_path_find_backend (sys_context, argc, argv, _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_BACKEND_NAME);
    if (library_path && lw6sys_file_exists (sys_context, library_path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("found library \"%s/mod-%s\" in \"%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_BACKEND_NAME, library_path);
	LW6SYS_FREE (sys_context, library_path);
      }
    else
      {
	/*
	 * Displaying a warning but not considering this an error,
	 * after all, this module can just not be compiled at all
	 * if we didn't have the prerequisites.
	 */
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("couldn't find library \"%s/mod-%s\" in \"%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_BACKEND_NAME, library_path);
      }

    library_path = lw6dyn_path_find_shared (sys_context, argc, argv, _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_SHARED_NAME);
    if (library_path && lw6sys_file_exists (sys_context, library_path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("found library \"%s/shared-%s\" in \"%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_SHARED_NAME, library_path);
	LW6SYS_FREE (sys_context, library_path);
      }
    else
      {
	/*
	 * Displaying a warning but not considering this an error,
	 * after all, this shared code can just not be compiled at all
	 * if we didn't have the prerequisites.
	 */
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("couldn't find library \"%s/shared-%s\" in \"%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_BACKEND_NAME, library_path);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing dl functions
 */
static void
_test_dl ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[] = { _TEST_ARGV0, _TEST_ARGV1 };
    lw6dyn_dl_handle_t *handle = NULL;

    /*
     * Note : this test won't fail even even if modules are
     * not here, but one of the good reason it's here is that
     * this way all the functions are compiled in, this is
     * very important since lw6dyn_dlopen_shared is typically
     * called from dynamic modules, so compiler has no idea it's
     * needed when stuffing stuff in the main binary.
     */

    handle = lw6dyn_dlopen_backend (sys_context, argc, argv, _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_BACKEND_NAME);
    if (handle)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("opened library \"%s/mod-%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_BACKEND_NAME);
	lw6dyn_dlclose_backend (sys_context, handle);
      }
    else
      {
	/*
	 * Displaying a warning but not considering this an error,
	 * after all, this module can just not be compiled at all
	 * if we didn't have the prerequisites.
	 */
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't find library \"%s/mod-%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_BACKEND_NAME);
      }

    handle = lw6dyn_dlopen_shared (sys_context, argc, argv, _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_SHARED_NAME);
    if (handle)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("opened library \"%s/shared-%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_SHARED_NAME);
	lw6dyn_dlclose_shared (sys_context, handle);
      }
    else
      {
	/*
	 * Displaying a warning but not considering this an error,
	 * after all, this shared_code can just not be compiled at all
	 * if we didn't have the prerequisites.
	 */
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't find library \"%s/shared-%s\""), _TEST_DYN_TOP_LEVEL_LIB, _TEST_DYN_SHARED_NAME);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libdyn CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libdyn CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6dyn_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libdyn module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dyn_test_register (lw6sys_context_t * sys_context, int mode)
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

  suite = CU_add_suite ("lw6dyn", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_path);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_list);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dl);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6dyn_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @dyn module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dyn_test_run (lw6sys_context_t * sys_context, int mode)
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
