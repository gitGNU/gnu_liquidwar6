/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "scm.h"

#define TEST_UNEXISTING_FILE "/foo/bar.scm"
#define TEST_UNEXISTING_FUNC "foo-bar"

#define _TEST_FUNCNAME_C "_scm_foo_bar"
#define _TEST_FUNCNAME_SCM "c-foo-bar"

#define _TEST_COVERAGE_SCM_FUNC1 "c-my-func1"
#define _TEST_COVERAGE_SCM_FUNC2 "c-my-function2"
#define _TEST_COVERAGE_SCM_FUNC3 "c-my-other3"
#define _TEST_COVERAGE_C_FUNC1 "_scm_my_func1"
#define _TEST_COVERAGE_C_FUNC2 "_scm_my_function2"
#define _TEST_COVERAGE_C_FUNC3 "_scm_my_other3"

#define _TEST_UTILS_STRING "This is a test_string"
#define _TEST_UTILS_LIST_1 "to be"
#define _TEST_UTILS_LIST_2 "or not to be"
#define _TEST_UTILS_LIST_3 "that is the question"
#define _TEST_UTILS_ASSOC_KEY_1 "1"
#define _TEST_UTILS_ASSOC_KEY_2 "2"
#define _TEST_UTILS_ASSOC_KEY_3 "3"
#define _TEST_UTILS_ASSOC_VALUE_1 "one"
#define _TEST_UTILS_ASSOC_VALUE_2 "two"
#define _TEST_UTILS_ASSOC_VALUE_3 "three"

static int global_ret = 0;

/*
 * Testing functions in funcname.c
 */
static int
test_funcname ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *funcname = NULL;

    funcname = lw6scm_funcname_scm2c (_TEST_FUNCNAME_SCM);
    if (funcname)
      {
	if (lw6sys_str_is_same (funcname, _TEST_FUNCNAME_C))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("OK, can convert funcname from scm to C (\"%s\"->\"%s\")"),
			_TEST_FUNCNAME_SCM, funcname);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("could not convert funcname from scm to C, got \"%s\" instead of \"%s\""),
			funcname, _TEST_FUNCNAME_C);
	  }
	LW6SYS_FREE (funcname);
      }

    funcname = lw6scm_funcname_c2scm (_TEST_FUNCNAME_C);
    if (funcname)
      {
	if (lw6sys_str_is_same (funcname, _TEST_FUNCNAME_SCM))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("OK, can convert funcname from C to scm (\"%s\"->\"%s\")"),
			_TEST_FUNCNAME_C, funcname);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("could not convert funcname from C to scm, got \"%s\" instead of \"%s\""),
			funcname, _TEST_FUNCNAME_SCM);
	  }
	LW6SYS_FREE (funcname);
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

static void *
guile_main_utils (void *data)
{
  SCM test_string = SCM_UNDEFINED;
  char *c_test_string = NULL;
  SCM test_list = SCM_UNDEFINED;
  lw6sys_list_t *c_test_list_1 = NULL;
  lw6sys_list_t *c_test_list_2 = NULL;
  int c_test_list_1_length = 0;
  int c_test_list_2_length = 0;
  SCM test_assoc = SCM_UNDEFINED;
  lw6sys_assoc_t *c_test_assoc_1 = NULL;
  lw6sys_assoc_t *c_test_assoc_2 = NULL;
  const char *c_test_assoc_1_value_1 = 0;
  const char *c_test_assoc_2_value_1 = 0;

  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("entering Guile in %s"), __FUNCTION__);

  test_string = scm_from_locale_string (_TEST_UTILS_STRING);
  c_test_string = lw6scm_utils_to_0str (test_string);
  if (c_test_string)
    {
      if (lw6sys_str_is_same (c_test_string, _TEST_UTILS_STRING))
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _x_
		      ("was able to transfer string \"%s\" from C to Guile to C"),
		      c_test_string);
	}
      LW6SYS_FREE (c_test_string);
    }
  else
    {
      global_ret = 0;
    }

  c_test_list_1 = lw6sys_list_new (NULL);
  if (c_test_list_1)
    {
      lw6sys_list_push_front (&c_test_list_1, _TEST_UTILS_LIST_1);
      lw6sys_list_push_front (&c_test_list_1, _TEST_UTILS_LIST_2);
      lw6sys_list_push_front (&c_test_list_1, _TEST_UTILS_LIST_3);
      if (c_test_list_1)
	{
	  test_list = lw6scm_utils_to_scm_str_list (c_test_list_1);
	  c_test_list_2 = lw6scm_utils_to_sys_str_list (test_list);
	  if (c_test_list_2)
	    {
	      c_test_list_1_length = lw6sys_list_length (c_test_list_1);
	      c_test_list_2_length = lw6sys_list_length (c_test_list_2);
	      if (c_test_list_1_length == c_test_list_2_length)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_
			      ("was able to transfer list of length %d from C to Guile to C"),
			      c_test_list_2_length);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("size mismatch c_test_list_1_lenght=%d c_test_list_2_length=%d"),
			      c_test_list_1_length, c_test_list_2_length);
		  global_ret = 0;
		}
	      lw6sys_list_free (c_test_list_2);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("unable to create C list from SCM object"));
	      global_ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("problem setting list values"));
	  global_ret = 0;
	}
      lw6sys_list_free (c_test_list_1);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create C list"));
      global_ret = 0;
    }

  c_test_assoc_1 = lw6sys_assoc_new (NULL);
  if (c_test_assoc_1)
    {
      lw6sys_assoc_set (&c_test_assoc_1, _TEST_UTILS_ASSOC_KEY_1,
			_TEST_UTILS_ASSOC_VALUE_1);
      lw6sys_assoc_set (&c_test_assoc_1, _TEST_UTILS_ASSOC_KEY_2,
			_TEST_UTILS_ASSOC_VALUE_2);
      lw6sys_assoc_set (&c_test_assoc_1, _TEST_UTILS_ASSOC_KEY_3,
			_TEST_UTILS_ASSOC_VALUE_3);
      if (c_test_assoc_1)
	{
	  test_assoc = lw6scm_utils_to_scm_str_assoc (c_test_assoc_1);
	  c_test_assoc_2 = lw6scm_utils_to_sys_str_assoc (test_assoc);
	  if (c_test_assoc_2)
	    {
	      c_test_assoc_1_value_1 =
		lw6sys_str_empty_if_null ((char *)
					  lw6sys_assoc_get (c_test_assoc_1,
							    _TEST_UTILS_ASSOC_KEY_1));
	      c_test_assoc_2_value_1 =
		lw6sys_str_empty_if_null ((char *)
					  lw6sys_assoc_get (c_test_assoc_2,
							    _TEST_UTILS_ASSOC_KEY_1));
	      if (lw6sys_str_is_same
		  (c_test_assoc_1_value_1, c_test_assoc_2_value_1))
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_
			      ("was able to transfer assoc from C to Guile to C, value for key \"%s\" is \"%s\""),
			      _TEST_UTILS_ASSOC_KEY_1,
			      c_test_assoc_2_value_1);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("content mismatch between assoc for key \"%s\" assoc_1 contains \"%s\" while assoc_2 contains \"%s\""),
			      _TEST_UTILS_ASSOC_KEY_1, c_test_assoc_1_value_1,
			      c_test_assoc_2_value_1);
		  global_ret = 0;
		}
	      lw6sys_assoc_free (c_test_assoc_2);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("unable to create C assoc from SCM object"));
	      global_ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("problem setting assoc values"));
	  global_ret = 0;
	}
      lw6sys_assoc_free (c_test_assoc_1);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create C assoc"));
      global_ret = 0;
    }

  scm_gc ();
  lw6sys_log (LW6SYS_LOG_NOTICE, _("leaving Guile in %s"), __FUNCTION__);

  return NULL;
}

/*
 * Testing functions in utils.c
 */
static int
test_utils ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    global_ret = 1;
    scm_with_guile (guile_main_utils, NULL);
    ret = global_ret;
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing functions in coverage.c
 */
static int
test_coverage ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_hash_t *coverage = NULL;
    lw6sys_list_t *funcs = NULL;
    int percent = 0;

    funcs = lw6sys_list_new (NULL);
    if (funcs)
      {
	lw6sys_list_push_front (&funcs, _TEST_COVERAGE_SCM_FUNC1);
	lw6sys_list_push_front (&funcs, _TEST_COVERAGE_SCM_FUNC2);
      }
    if (funcs)
      {
	coverage = lw6scm_coverage_new (funcs);
	if (coverage)
	  {
	    lw6scm_coverage_call (coverage, _TEST_COVERAGE_C_FUNC1);
	    lw6scm_coverage_call (coverage, _TEST_COVERAGE_C_FUNC2);
	    lw6scm_coverage_call (coverage, _TEST_COVERAGE_C_FUNC2);
	    lw6scm_coverage_log (coverage);
	    if (lw6scm_coverage_check (&percent, coverage, funcs))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("coverage OK, all functions reported as called"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("coverage problem, only %d percent of functions covered"),
			    percent);
		ret = 0;
	      }
	    lw6sys_list_push_front (&funcs, _TEST_COVERAGE_SCM_FUNC3);
	    if (funcs)
	      {
		if (!lw6scm_coverage_check (&percent, coverage, funcs))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("coverage broken, only %d percent, this is right, error detection works"),
				percent);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("coverage problem, reported as OK when it should not"));
		    ret = 0;
		  }
	      }
	    lw6sys_hash_free (coverage);
	  }
	else
	  {
	    ret = 0;
	  }
      }
    if (funcs)
      {
	lw6sys_list_free (funcs);
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_version ()
{
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;

  ret = scm_from_locale_string (lw6sys_build_get_version ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static void *
guile_main_wrapper (void *data)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("entering Guile in %s"), __FUNCTION__);

  if (lw6scm_c_define_gsubr (LW6DEF_C_LW6SYS_BUILD_GET_VERSION, 0, 0, 0,
			     (SCM (*)())_scm_lw6sys_build_get_version))
    {
      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_
		  ("next you should see a message complaining \"%s\" is not documented"),
		  TEST_UNEXISTING_FUNC);
      if (lw6scm_c_define_gsubr (TEST_UNEXISTING_FUNC, 0, 0, 0,
				 (SCM (*)())_scm_lw6sys_build_get_version))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("function \"%s\" was defined, should have been refused"),
		      TEST_UNEXISTING_FUNC);
	  global_ret = 0;
	}
    }
  else
    {
      global_ret = 0;
    }

  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _x_
	      ("next you should see a message complaining \"%s\" does not exists"),
	      TEST_UNEXISTING_FILE);
  lw6scm_c_primitive_load (TEST_UNEXISTING_FILE);
  scm_gc ();
  lw6sys_log (LW6SYS_LOG_NOTICE, _("leaving Guile in %s"), __FUNCTION__);

  return NULL;
}

/*
 * Testing functions in wrapper.c
 */
static int
test_wrapper ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    global_ret = 1;
    scm_with_guile (guile_main_wrapper, NULL);
    ret = global_ret;
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/**
 * lw6scm_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @scm module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6scm_test (int mode)
{
  int ret = 0;
  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6hlp_test (mode);
    }

  ret = test_wrapper () && test_funcname () && test_utils ()
    && test_coverage ();
  return ret;
}
