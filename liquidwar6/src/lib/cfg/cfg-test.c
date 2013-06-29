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

#include "cfg.h"

#define TEST_CFG "test.xml"
#define TEST_ARGC 3
#define TEST_ARGV0 "abc"
#define TEST_ARGV1 "--fullscreen"
#define TEST_ARGV2 "--width=800"
#define TEST_UNIFIED_KEY_YES LW6DEF_WIDTH
#define TEST_UNIFIED_KEY_NO "unknown"
#define TEST_EXP_DIR "/tmp"

/*
 * Testing loading/saving
 */
static int
test_load_save ()
{
  int ret = 1;
  void *context;
  const int argc = TEST_ARGC;
  const char *argv[TEST_ARGC] = { TEST_ARGV0, TEST_ARGV1, TEST_ARGV2 };
  char *user_dir = NULL;
  char *test_cfg = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  context = lw6cfg_init (argc, argv);
  if (context)
    {
      user_dir = lw6sys_get_default_user_dir ();
      if (user_dir)
	{
	  if (!lw6sys_dir_exists (user_dir))
	    {
	      lw6sys_create_dir (user_dir);
	    }
	  test_cfg = lw6sys_path_concat (user_dir, TEST_CFG);
	  if (test_cfg)
	    {
	      /*
	       * Note: we don't consider the test as invalid if the
	       * file does not exist, after all, it's a normal behavior
	       * of the game to be launched without any config file.
	       */
	      lw6cfg_load (context, test_cfg);
	      lw6cfg_save (context, test_cfg);

	      LW6SYS_FREE (test_cfg);
	    }
	  LW6SYS_FREE (user_dir);
	}
      lw6cfg_quit (context);
    }
  else
    {
      ret = 0;
    }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Test unified options
 */
static int
test_unified ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;
  const int argc = TEST_ARGC;
  const char *argv[TEST_ARGC] = { TEST_ARGV0, TEST_ARGV1, TEST_ARGV2 };

  {
    char *value;

    value = lw6cfg_unified_get_value (argc, argv, TEST_UNIFIED_KEY_YES);
    if (value)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("value for \"%s\" is \"%s\""),
		    TEST_UNIFIED_KEY_YES, value);
	LW6SYS_FREE (value);
      }
    else
      {
	ret = 0;
      }

    value = lw6cfg_unified_get_value (argc, argv, TEST_UNIFIED_KEY_NO);
    if (value)
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("key \"%s\" has value \"%s\", should be NULL"),
		    TEST_UNIFIED_KEY_NO, value);
	LW6SYS_FREE (value);
	ret = 0;
      }
    else
      {
	// OK
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Test exp functions
 */
static int
test_exp ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int exp = 0;
    int new_exp = 0;

    lw6cfg_load_exp (TEST_EXP_DIR, &exp);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("first exp read exp=%d"), exp);
    if (lw6cfg_save_exp (TEST_EXP_DIR, exp))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("exp saved"));
	if (lw6cfg_load_exp (TEST_EXP_DIR, &new_exp))
	  {
	    if (new_exp == exp)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("new exp read exp=%d"), new_exp);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("wrong new exp should be %d but is %d"),
			    exp, new_exp);
		ret = 0;
	      }
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to save exp"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6cfg_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @cfg module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cfg_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
    }

  ret = test_load_save () && test_unified ();

  if (mode)
    {
      ret = ret && test_exp ();
    }

  return ret;
}
