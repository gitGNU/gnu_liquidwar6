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

#include "ldr.h"

#define _TEST_MAP "subflower/"
#define _TEST_ARGC 2
#define _TEST_ARGV0 "prog"
#define _TEST_ARGV1 "--arg=X"
#define _TEST_DIR_RELATIVE_PATH ""
#define _TEST_DIR_CHAIN_PATH _TEST_MAP
#define _TEST_PARAM_KEY1 LW6DEF_TOTAL_TIME
#define _TEST_PARAM_VALUE1 "180"
#define _TEST_PARAM_KEY2 LW6DEF_BACKGROUND_STYLE
#define _TEST_PARAM_VALUE2 "bubbles"
#define _TEST_PARAM_KEY3 LW6DEF_ZOOM
#define _TEST_PARAM_VALUE3 "1.0"
#define _TEST_HINTS_KEY1 LW6DEF_RESAMPLE
#define _TEST_HINTS_VALUE1 "true"
#define _TEST_HINTS_KEY2 LW6DEF_FIGHTER_SCALE
#define _TEST_HINTS_VALUE2 "3.5"
#define _TEST_HINTS_KEY3 LW6DEF_MAX_MAP_SURFACE
#define _TEST_HINTS_VALUE3 "1000000"
#define _TEST_TEAMS_KEY1 LW6DEF_PLAYER1_COLOR
#define _TEST_TEAMS_VALUE1 "red"
#define _TEST_TEAMS_KEY2 LW6DEF_BOT_SPEED
#define _TEST_TEAMS_VALUE2 "1.5"
#define _TEST_TEAMS_KEY3 LW6DEF_BOT1_AI
#define _TEST_TEAMS_VALUE3 "foo"
#define _TEST_DISPLAY_WIDTH 640
#define _TEST_DISPLAY_HEIGHT 480
#define _TEST_PROGRESS_MIN 0.1f
#define _TEST_PROGRESS_MAX 0.9f
#define _TEST_RESAMPLER_DISPLAY_W 1920
#define _TEST_RESAMPLER_DISPLAY_H 1080

typedef struct _lw6ldr_test_data_s
{
  int ret;
} _lw6ldr_test_data_t;

static _lw6ldr_test_data_t _test_data = { 0 };

static int
check_map_with_absolute_path (char *absolute_path)
{
  lw6map_level_t *level1 = NULL;
  lw6map_level_t *level2 = NULL;
  lw6map_level_t *level3 = NULL;
  char *hexa_level1;
  char *hexa_level2;
  int ret = 0;
  char *repr = NULL;
  char *user_dir = NULL;

  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("deep check \"%s\""), absolute_path);

  user_dir = lw6sys_get_default_user_dir ();
  if (LW6SYS_TEST_ACK (user_dir))
    {
      level1 =
	lw6ldr_read (absolute_path, NULL, NULL, _TEST_DISPLAY_WIDTH,
		     _TEST_DISPLAY_HEIGHT, LW6LDR_DEFAULT_BENCH_VALUE,
		     LW6LDR_DEFAULT_MAGIC_NUMBER, user_dir, NULL);
      if (LW6SYS_TEST_ACK (level1))
	{
	  hexa_level1 = lw6map_to_hexa (level1);
	  if (LW6SYS_TEST_ACK (hexa_level1))
	    {
	      level2 = lw6map_from_hexa (hexa_level1);
	      if (LW6SYS_TEST_ACK (level2))
		{
		  hexa_level2 = lw6map_to_hexa (level2);
		  if (LW6SYS_TEST_ACK (hexa_level2))
		    {
		      if (LW6SYS_TEST_ACK
			  (strcmp (hexa_level1, hexa_level2) == 0))
			{
			  level3 = lw6map_dup (level2, NULL);
			  if (LW6SYS_TEST_ACK (level3))
			    {
			      repr = lw6map_repr (level3);
			      if (LW6SYS_TEST_ACK (repr))
				{
				  lw6sys_log (LW6SYS_LOG_NOTICE,
					      _x_ ("map repr=\"%s\""), repr);
				  LW6SYS_FREE (repr);
				  ret = 1;
				}
			      lw6map_free (level3);
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_WARNING,
					  _x_ ("unable to copy \"%s\""),
					  absolute_path);
			    }
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("second serialization of \"%s\" returned inconsistent result"),
				      absolute_path);
			}
		      LW6SYS_FREE (hexa_level2);
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_ ("unable to serialize \"%s\" (2)"),
				  absolute_path);
		    }
		  lw6map_free (level2);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("unable to unserialize \"%s\""),
			      absolute_path);
		}
	      LW6SYS_FREE (hexa_level1);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("unable to serialize \"%s\" (1)"),
			  absolute_path);
	    }
	  lw6map_free (level1);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to read \"%s\""),
		      absolute_path);
	}
      LW6SYS_FREE (user_dir);
    }

  return ret;
}

static void
_test_data_callback_quick (void *func_data, void *data)
{
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  int *ret = (int *) func_data;
  int ok = 0;

  if (LW6SYS_TEST_ACK (entry))
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("quick check \"%s\""),
		  entry->absolute_path);
      ok = lw6sys_dir_exists (entry->absolute_path);
      if (LW6SYS_TEST_ACK (ret))
	{
	  (*ret) = (*ret) && ok;
	}
    }
}

static void
_test_data_callback_deep (void *func_data, void *data)
{
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  int *ret = (int *) func_data;
  int ok = 0;

  if (LW6SYS_TEST_ACK (entry && (!entry->forbidden)))
    {
      ok = check_map_with_absolute_path (entry->absolute_path);
      if (LW6SYS_TEST_ACK (ret))
	{
	  (*ret) = (*ret) && ok;
	}
    }
}

/*
 * Quick test
 */
static void
_test_quick ()
{
  int ret = 0;

  LW6SYS_TEST_FUNCTION_BEGIN;

  const int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1 };

  {
    char *map_path = NULL;
    char *user_dir = NULL;

    ret = 1;
    map_path = lw6cfg_unified_get_map_path (argc, argv);
    if (LW6SYS_TEST_ACK (map_path))
      {
	user_dir = lw6sys_get_user_dir (argc, argv);
	if (LW6SYS_TEST_ACK (user_dir))
	  {
	    lw6ldr_for_all_entries (map_path, "", user_dir, 1,
				    _test_data_callback_quick, &ret);
	    LW6SYS_FREE (user_dir);
	  }
	LW6SYS_FREE (map_path);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Deep test
 */
static void
_test_deep ()
{
  int ret = 0;

  LW6SYS_TEST_FUNCTION_BEGIN;

  const int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1 };

  {
    char *map_path = NULL;
    char *user_dir = NULL;

    ret = 1;
    map_path = lw6cfg_unified_get_map_path (argc, argv);
    if (LW6SYS_TEST_ACK (map_path))
      {
	user_dir = lw6sys_get_user_dir (argc, argv);
	if (LW6SYS_TEST_ACK (user_dir))
	  {
	    lw6ldr_for_all_entries (map_path, "", user_dir, 0,
				    _test_data_callback_deep, &ret);
	    LW6SYS_FREE (user_dir);
	  }
	LW6SYS_FREE (map_path);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing dirs/entries
 */
static void
_test_dir ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;
  const int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1 };

  {
    lw6sys_list_t *entries = NULL;
    char *map_path = NULL;
    char *user_dir = NULL;
    lw6ldr_entry_t *entry = NULL;
    int entry_found = 0;

    map_path = lw6cfg_unified_get_map_path (argc, argv);
    if (LW6SYS_TEST_ACK (map_path))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("map_path=\"%s\""), map_path);
	user_dir = lw6sys_get_user_dir (argc, argv);
	if (LW6SYS_TEST_ACK (user_dir))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("user_dir=\"%s\""), user_dir);
	    entries =
	      lw6ldr_get_entries (map_path, _TEST_DIR_RELATIVE_PATH,
				  user_dir);
	    if (LW6SYS_TEST_ACK (entries))
	      {
		while (entries
		       && (entry = lw6sys_lifo_pop (&entries)) != NULL)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("found map \"%s\" in \"%s\""),
				entry->metadata.title, entry->absolute_path);
		    entry_found = 1;
		    lw6ldr_free_entry (entry);
		  };
	      }
	    else
	      {
		ret = 0;
	      }
	    LW6SYS_FREE (user_dir);
	  }
	else
	  {
	    ret = 0;
	  }

	entry = lw6ldr_chain_entry (map_path, _TEST_DIR_CHAIN_PATH, user_dir);
	if (LW6SYS_TEST_ACK (entry))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("chain for \"%s\" is \"%s\""),
			_TEST_DIR_CHAIN_PATH, entry->relative_path);
	    lw6ldr_free_entry (entry);
	  }
	else
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (map_path);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (!entry_found))
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing param
 */
static void
_test_param ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_param_t param;
    lw6sys_assoc_t *values = NULL;

    memset (&param, 0, sizeof (lw6map_param_t));
    lw6map_param_defaults (&param);

    values = lw6sys_assoc_new (NULL);
    if (LW6SYS_TEST_ACK (values))
      {
	lw6sys_assoc_set (&values, _TEST_PARAM_KEY1, _TEST_PARAM_VALUE1);
	lw6sys_assoc_set (&values, _TEST_PARAM_KEY2, _TEST_PARAM_VALUE2);
	lw6sys_assoc_set (&values, _TEST_PARAM_KEY3, _TEST_PARAM_VALUE3);

	if (LW6SYS_TEST_ACK (lw6ldr_param_update (&param, values)))
	  {
	    ret = 1;
	  }

	lw6sys_assoc_free (values);
      }
    else
      {
	ret = 0;
      }

    lw6map_param_clear (&param);

  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing hints
 */
static void
_test_hints ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6ldr_hints_t hints;
    lw6sys_assoc_t *values = NULL;

    lw6ldr_hints_zero (&hints);
    lw6ldr_hints_defaults (&hints);

    values = lw6sys_assoc_new (NULL);
    if (LW6SYS_TEST_ACK (values))
      {
	lw6sys_assoc_set (&values, _TEST_HINTS_KEY1, _TEST_HINTS_VALUE1);
	lw6sys_assoc_set (&values, _TEST_HINTS_KEY2, _TEST_HINTS_VALUE2);
	lw6sys_assoc_set (&values, _TEST_HINTS_KEY3, _TEST_HINTS_VALUE3);

	if (LW6SYS_TEST_ACK (lw6ldr_hints_update (&hints, values)))
	  {
	    ret = 1;
	  }

	lw6sys_assoc_free (values);
      }
    else
      {
	ret = 0;
      }

    lw6ldr_hints_clear (&hints);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing teams
 */
static void
_test_teams ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_teams_t teams;
    lw6sys_assoc_t *values = NULL;

    lw6map_teams_zero (&teams);
    lw6map_teams_defaults (&teams);

    values = lw6sys_assoc_new (NULL);
    if (LW6SYS_TEST_ACK (values))
      {
	lw6sys_assoc_set (&values, _TEST_TEAMS_KEY1, _TEST_TEAMS_VALUE1);
	lw6sys_assoc_set (&values, _TEST_TEAMS_KEY2, _TEST_TEAMS_VALUE2);
	lw6sys_assoc_set (&values, _TEST_TEAMS_KEY3, _TEST_TEAMS_VALUE3);

	if (LW6SYS_TEST_ACK (lw6ldr_teams_update (&teams, values)))
	  {
	    ret = 1;
	  }

	lw6sys_assoc_free (values);
      }
    else
      {
	ret = 0;
      }

    lw6map_teams_clear (&teams);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing reading
 */
static void
_test_read ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;
  const int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1 };

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;
    char *map_path = NULL;
    char *user_dir = NULL;
    lw6sys_progress_t progress;
    float done = 0.0f;

    progress.min = _TEST_PROGRESS_MIN;
    progress.max = _TEST_PROGRESS_MAX;
    progress.value = &done;
    map_path = lw6cfg_unified_get_map_path (argc, argv);
    if (LW6SYS_TEST_ACK (map_path))
      {
	user_dir = lw6sys_get_user_dir (argc, argv);
	if (LW6SYS_TEST_ACK (user_dir))
	  {
	    level =
	      lw6ldr_read_relative (map_path, _TEST_MAP, NULL, NULL,
				    _TEST_DISPLAY_WIDTH, _TEST_DISPLAY_HEIGHT,
				    LW6LDR_DEFAULT_BENCH_VALUE,
				    LW6LDR_DEFAULT_MAGIC_NUMBER, user_dir,
				    &progress);
	    if (LW6SYS_TEST_ACK (level))
	      {
		repr = lw6map_repr (level);
		if (LW6SYS_TEST_ACK (repr))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("after reading, map repr is \"%s\""),
				repr);
		    LW6SYS_FREE (repr);
		  }
		else
		  {
		    ret = 0;
		  }
		lw6map_free (level);
	      }
	    else
	      {
		ret = 0;
	      }
	    LW6SYS_FREE (user_dir);
	  }
	else
	  {
	    ret = 0;
	  }

	LW6SYS_FREE (map_path);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing resampler
 */
static void
_test_resampler ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int map_w = 0;
    int map_h = 0;

    lw6ldr_resampler_use_for_gen (&map_w, &map_h, _TEST_RESAMPLER_DISPLAY_W,
				  _TEST_RESAMPLER_DISPLAY_H,
				  LW6LDR_DEFAULT_BENCH_VALUE,
				  LW6LDR_DEFAULT_MAGIC_NUMBER);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("gen would create a %dx%d map with a display of %dx%d"),
		map_w, map_h, _TEST_RESAMPLER_DISPLAY_W,
		_TEST_RESAMPLER_DISPLAY_H);
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("init libldr CUnit test suite"));
  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("quit libldr CUnit test suite"));
  return CUE_SUCCESS;
}

/**
 * lw6ldr_test_register
 *
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libldr module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6ldr_test_register (int mode)
{
  int ret = 1;
  CU_Suite *suite;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (mode);
      lw6hlp_test_register (mode);
      lw6cfg_test_register (mode);
      lw6map_test_register (mode);
    }

  suite = CU_add_suite ("lw6ldr", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dir);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_hints);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_teams);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_param);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_read);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_quick);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_deep);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_resampler);
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
 * lw6ldr_test_run
 *
 * @mode: test mode (bitmask)
 *
 * Runs the @ldr module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6ldr_test_run (int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  if (lw6sys_cunit_run_tests (mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
