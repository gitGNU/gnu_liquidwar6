/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

static int global_ret = 0;

static SCM
_scm_lw6sys_build_get_version ()
{
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;

  ret = scm_makfrom0str (lw6sys_build_get_version ());

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

  ret = test_wrapper ();
  return ret;
}
