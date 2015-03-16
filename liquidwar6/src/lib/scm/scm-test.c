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

#include "scm.h"

#define _TEST_UNEXISTING_FILE "/foo/bar.scm"
#define _TEST_UNEXISTING_FUNC "foo-bar"

#define _TEST_FUNCNAME_C "_scm_foo_bar"
#define _TEST_FUNCNAME_SCM "c-foo-bar"

#define _TEST_COVERAGE_SCM_FUNC1 "c-my-func1"
#define _TEST_COVERAGE_SCM_FUNC2 "c-my-function2"
#define _TEST_COVERAGE_SCM_FUNC3 "c-my-other3"
#define _TEST_COVERAGE_C_FUNC1 "_scm_my_func1"
#define _TEST_COVERAGE_C_FUNC2 "_scm_my_function2"
#define _TEST_COVERAGE_C_FUNC3 "_scm_my_other3"

#define _TEST_UTILS_STRING "This is a _test_string"
#define _TEST_UTILS_LIST_1 "to be"
#define _TEST_UTILS_LIST_2 "or not to be"
#define _TEST_UTILS_LIST_3 "that is the question"
#define _TEST_UTILS_ASSOC_KEY_1 "1"
#define _TEST_UTILS_ASSOC_KEY_2 "2"
#define _TEST_UTILS_ASSOC_KEY_3 "3"
#define _TEST_UTILS_ASSOC_VALUE_1 "one"
#define _TEST_UTILS_ASSOC_VALUE_2 "two"
#define _TEST_UTILS_ASSOC_VALUE_3 "three"

typedef struct _lw6scm_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6scm_test_data_t;

static _lw6scm_test_data_t _test_data = { 0, NULL };

static int _global_ret = 0;
static lw6sys_context_t *_global_sys_context = NULL;

/*
 * Testing functions in funcname.c
 */
static void
_test_funcname ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *funcname = NULL;

    funcname = lw6scm_funcname_scm2c (sys_context, _TEST_FUNCNAME_SCM);
    if (funcname)
      {
	if (lw6sys_str_is_same (sys_context, funcname, _TEST_FUNCNAME_C))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, can convert funcname from scm to C (\"%s\"->\"%s\")"), _TEST_FUNCNAME_SCM, funcname);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("could not convert funcname from scm to C, got \"%s\" instead of \"%s\""), funcname, _TEST_FUNCNAME_C);
	  }
	LW6SYS_FREE (sys_context, funcname);
      }

    funcname = lw6scm_funcname_c2scm (sys_context, _TEST_FUNCNAME_C);
    if (funcname)
      {
	if (lw6sys_str_is_same (sys_context, funcname, _TEST_FUNCNAME_SCM))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, can convert funcname from C to scm (\"%s\"->\"%s\")"), _TEST_FUNCNAME_C, funcname);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("could not convert funcname from C to scm, got \"%s\" instead of \"%s\""), funcname, _TEST_FUNCNAME_SCM);
	  }
	LW6SYS_FREE (sys_context, funcname);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void *
_guile_main_utils (lw6sys_context_t * sys_context, void *data)
{
  SCM _test_string = SCM_UNDEFINED;
  char *c_test_string = NULL;
  SCM _test_list = SCM_UNDEFINED;
  lw6sys_list_t *c_test_list_1 = NULL;
  lw6sys_list_t *c_test_list_2 = NULL;
  int c_test_list_1_length = 0;
  int c_test_list_2_length = 0;
  SCM _test_assoc = SCM_UNDEFINED;
  lw6sys_assoc_t *c_test_assoc_1 = NULL;
  lw6sys_assoc_t *c_test_assoc_2 = NULL;
  const char *c_test_assoc_1_value_1 = 0;
  const char *c_test_assoc_2_value_1 = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("entering Guile in %s"), __FUNCTION__);

  _test_string = scm_from_locale_string (_TEST_UTILS_STRING);
  c_test_string = lw6scm_utils_to_0str (sys_context, _test_string);
  if (c_test_string)
    {
      if (lw6sys_str_is_same (sys_context, c_test_string, _TEST_UTILS_STRING))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("was able to transfer string \"%s\" from C to Guile to C"), c_test_string);
	}
      LW6SYS_FREE (sys_context, c_test_string);
    }
  else
    {
      _global_ret = 0;
    }

  c_test_list_1 = lw6sys_list_new (sys_context, NULL);
  if (c_test_list_1)
    {
      lw6sys_list_push_front (sys_context, &c_test_list_1, _TEST_UTILS_LIST_1);
      lw6sys_list_push_front (sys_context, &c_test_list_1, _TEST_UTILS_LIST_2);
      lw6sys_list_push_front (sys_context, &c_test_list_1, _TEST_UTILS_LIST_3);
      if (c_test_list_1)
	{
	  _test_list = lw6scm_utils_to_scm_str_list (sys_context, c_test_list_1);
	  c_test_list_2 = lw6scm_utils_to_sys_str_list (sys_context, _test_list);
	  if (c_test_list_2)
	    {
	      c_test_list_1_length = lw6sys_list_length (sys_context, c_test_list_1);
	      c_test_list_2_length = lw6sys_list_length (sys_context, c_test_list_2);
	      if (c_test_list_1_length == c_test_list_2_length)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("was able to transfer list of length %d from C to Guile to C"), c_test_list_2_length);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("size mismatch c_test_list_1_lenght=%d c_test_list_2_length=%d"), c_test_list_1_length, c_test_list_2_length);
		  _global_ret = 0;
		}
	      lw6sys_list_free (sys_context, c_test_list_2);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create C list from SCM object"));
	      _global_ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("problem setting list values"));
	  _global_ret = 0;
	}
      lw6sys_list_free (sys_context, c_test_list_1);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create C list"));
      _global_ret = 0;
    }

  c_test_assoc_1 = lw6sys_assoc_new (sys_context, NULL);
  if (c_test_assoc_1)
    {
      lw6sys_assoc_set (sys_context, &c_test_assoc_1, _TEST_UTILS_ASSOC_KEY_1, _TEST_UTILS_ASSOC_VALUE_1);
      lw6sys_assoc_set (sys_context, &c_test_assoc_1, _TEST_UTILS_ASSOC_KEY_2, _TEST_UTILS_ASSOC_VALUE_2);
      lw6sys_assoc_set (sys_context, &c_test_assoc_1, _TEST_UTILS_ASSOC_KEY_3, _TEST_UTILS_ASSOC_VALUE_3);
      if (c_test_assoc_1)
	{
	  _test_assoc = lw6scm_utils_to_scm_str_assoc (sys_context, c_test_assoc_1);
	  c_test_assoc_2 = lw6scm_utils_to_sys_str_assoc (sys_context, _test_assoc);
	  if (c_test_assoc_2)
	    {
	      c_test_assoc_1_value_1 =
		lw6sys_str_empty_if_null (sys_context, (char *) lw6sys_assoc_get (sys_context, c_test_assoc_1, _TEST_UTILS_ASSOC_KEY_1));
	      c_test_assoc_2_value_1 =
		lw6sys_str_empty_if_null (sys_context, (char *) lw6sys_assoc_get (sys_context, c_test_assoc_2, _TEST_UTILS_ASSOC_KEY_1));
	      if (lw6sys_str_is_same (sys_context, c_test_assoc_1_value_1, c_test_assoc_2_value_1))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			      _x_
			      ("was able to transfer assoc from C to Guile to C, value for key \"%s\" is \"%s\""),
			      _TEST_UTILS_ASSOC_KEY_1, c_test_assoc_2_value_1);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_
			      ("content mismatch between assoc for key \"%s\" assoc_1 contains \"%s\" while assoc_2 contains \"%s\""),
			      _TEST_UTILS_ASSOC_KEY_1, c_test_assoc_1_value_1, c_test_assoc_2_value_1);
		  _global_ret = 0;
		}
	      lw6sys_assoc_free (sys_context, c_test_assoc_2);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create C assoc from SCM object"));
	      _global_ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("problem setting assoc values"));
	  _global_ret = 0;
	}
      lw6sys_assoc_free (sys_context, c_test_assoc_1);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create C assoc"));
      _global_ret = 0;
    }

  scm_gc ();
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _("leaving Guile in %s"), __FUNCTION__);

  return NULL;
}

/*
 * Testing functions in utils.c
 */
static void
_test_utils ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _global_ret = 1;
    lw6scm_with_guile (sys_context, _guile_main_utils, NULL);
    ret = _global_ret;
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in coverage.c
 */
static void
_test_coverage ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_hash_t *coverage = NULL;
    lw6sys_list_t *funcs = NULL;
    int percent = 0;

    funcs = lw6sys_list_new (sys_context, NULL);
    if (funcs)
      {
	lw6sys_list_push_front (sys_context, &funcs, _TEST_COVERAGE_SCM_FUNC1);
	lw6sys_list_push_front (sys_context, &funcs, _TEST_COVERAGE_SCM_FUNC2);
      }
    if (funcs)
      {
	coverage = lw6scm_coverage_new (sys_context, funcs);
	if (coverage)
	  {
	    lw6scm_coverage_call (sys_context, coverage, _TEST_COVERAGE_C_FUNC1);
	    lw6scm_coverage_call (sys_context, coverage, _TEST_COVERAGE_C_FUNC2);
	    lw6scm_coverage_call (sys_context, coverage, _TEST_COVERAGE_C_FUNC2);
	    lw6scm_coverage_log (sys_context, coverage);
	    if (lw6scm_coverage_check (sys_context, &percent, coverage, funcs))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("coverage OK, all functions reported as called"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("coverage problem, only %d percent of functions covered"), percent);
		ret = 0;
	      }
	    lw6sys_list_push_front (sys_context, &funcs, _TEST_COVERAGE_SCM_FUNC3);
	    if (funcs)
	      {
		if (!lw6scm_coverage_check (sys_context, &percent, coverage, funcs))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("coverage broken, only %d percent, this is right, error detection works"), percent);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("coverage problem, reported as OK when it should not"));
		    ret = 0;
		  }
	      }
	    lw6sys_hash_free (sys_context, coverage);
	  }
	else
	  {
	    ret = 0;
	  }
      }
    if (funcs)
      {
	lw6sys_list_free (sys_context, funcs);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static SCM
_scm_lw6sys_build_get_version ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = _global_sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;

  ret = scm_from_locale_string (lw6sys_build_get_version (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static void *
_guile_main_wrapper (lw6sys_context_t * sys_context, void *data)
{
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("entering Guile in %s"), __FUNCTION__);
  _global_sys_context = sys_context;

  if (lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_VERSION, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_version))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("next you should see a message complaining \"%s\" is not documented"), _TEST_UNEXISTING_FUNC);
      if (lw6scm_c_define_gsubr (sys_context, _TEST_UNEXISTING_FUNC, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_version))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("function \"%s\" was defined, should have been refused"), _TEST_UNEXISTING_FUNC);
	  _global_ret = 0;
	}
    }
  else
    {
      _global_ret = 0;
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("next you should see a message complaining \"%s\" does not exists"), _TEST_UNEXISTING_FILE);
  lw6scm_c_primitive_load (sys_context, _TEST_UNEXISTING_FILE);
  scm_gc ();
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _("leaving Guile in %s"), __FUNCTION__);

  return NULL;
}

/*
 * Testing functions in wrapper.c
 */
static void
_test_wrapper ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _global_ret = 1;
    lw6scm_with_guile (sys_context, _guile_main_wrapper, NULL);
    ret = _global_ret;
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libscm CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libscm CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6scm_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libscm module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6scm_test_register (lw6sys_context_t * sys_context, int mode)
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
      lw6hlp_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6scm", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_wrapper);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_funcname);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_utils);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_coverage);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6scm_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @scm module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6scm_test_run (lw6sys_context_t * sys_context, int mode)
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
