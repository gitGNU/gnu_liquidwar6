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
guile_main (void *data)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("entering Guile"));

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
  lw6sys_log (LW6SYS_LOG_NOTICE, _("leaving Guile"));

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
    scm_with_guile (guile_main, NULL);
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

  ret = test_wrapper () && test_funcname () && test_coverage ();
  return ret;
}
