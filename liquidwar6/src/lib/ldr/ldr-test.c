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

#include <string.h>

#include "ldr.h"

#define TEST_MAP "subflower/"
#define TEST_ARGC 2
#define TEST_ARGV0 "prog"
#define TEST_ARGV1 "--arg=X"
#define TEST_DIR_RELATIVE_PATH ""
#define TEST_PARAM_KEY1 LW6DEF_TOTAL_TIME
#define TEST_PARAM_VALUE1 "180"
#define TEST_PARAM_KEY2 LW6DEF_BACKGROUND_STYLE
#define TEST_PARAM_VALUE2 "bubbles"
#define TEST_PARAM_KEY3 LW6DEF_ZOOM
#define TEST_PARAM_VALUE3 "1.0"
#define TEST_HINTS_KEY1 LW6DEF_RESAMPLE
#define TEST_HINTS_VALUE1 "true"
#define TEST_HINTS_KEY2 LW6DEF_FIGHTER_SCALE
#define TEST_HINTS_VALUE2 "3.5"
#define TEST_HINTS_KEY3 LW6DEF_MAX_MAP_SURFACE
#define TEST_HINTS_VALUE3 "1000000"
#define TEST_TEAMS_KEY1 LW6DEF_PLAYER_COLOR
#define TEST_TEAMS_VALUE1 "red"
#define TEST_TEAMS_KEY2 LW6DEF_BOT_SPEED
#define TEST_TEAMS_VALUE2 "1.5"
#define TEST_TEAMS_KEY3 LW6DEF_BOT1_AI
#define TEST_TEAMS_VALUE3 "foo"
#define TEST_DISPLAY_WIDTH 640
#define TEST_DISPLAY_HEIGHT 480
#define TEST_PROGRESS_MIN 0.1f
#define TEST_PROGRESS_MAX 0.9f

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
  if (user_dir)
    {
      level1 =
	lw6ldr_read (absolute_path, NULL, NULL, TEST_DISPLAY_WIDTH,
		     TEST_DISPLAY_HEIGHT, LW6LDR_DEFAULT_BENCH_VALUE,
		     LW6LDR_DEFAULT_MAGIC_NUMBER, user_dir, NULL);
      if (level1)
	{
	  hexa_level1 = lw6map_to_hexa (level1);
	  if (hexa_level1)
	    {
	      level2 = lw6map_from_hexa (hexa_level1);
	      if (level2)
		{
		  hexa_level2 = lw6map_to_hexa (level2);
		  if (hexa_level2)
		    {
		      if (strcmp (hexa_level1, hexa_level2) == 0)
			{
			  level3 = lw6map_dup (level2, NULL);
			  if (level3)
			    {
			      repr = lw6map_repr (level3);
			      if (repr)
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
test_data_callback_quick (void *func_data, void *data)
{
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  int *ret = (int *) func_data;
  int ok = 0;

  if (entry)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("quick check \"%s\""),
		  entry->absolute_path);
      ok = lw6sys_dir_exists (entry->absolute_path);
      if (ret)
	{
	  (*ret) = (*ret) && ok;
	}
    }
}

static void
test_data_callback_deep (void *func_data, void *data)
{
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  int *ret = (int *) func_data;
  int ok = 0;

  if (entry)
    {
      ok = check_map_with_absolute_path (entry->absolute_path);
      if (ret)
	{
	  (*ret) = (*ret) && ok;
	}
    }
}

/*
 * test_data
 */
static int
test_data ()
{
  int ret = 0;

  LW6SYS_TEST_FUNCTION_BEGIN;

  int argc = TEST_ARGC;
  char *argv[TEST_ARGC] = { TEST_ARGV0, TEST_ARGV1 };

  {
    char *map_path = NULL;
    char *user_dir = NULL;

    ret = 1;
    map_path = lw6cfg_unified_get_map_path (argc, argv);
    if (map_path)
      {
	user_dir = lw6sys_get_user_dir (argc, argv);
	if (user_dir)
	  {
	    lw6ldr_for_all_entries (map_path, "", user_dir, 1,
				    test_data_callback_quick, &ret);
	    lw6ldr_for_all_entries (map_path, "", user_dir, 0,
				    test_data_callback_deep, &ret);
	    LW6SYS_FREE (user_dir);
	  }
	LW6SYS_FREE (map_path);
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing dirs/entries
 */
static int
test_dir ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;
  int argc = TEST_ARGC;
  char *argv[TEST_ARGC] = { TEST_ARGV0, TEST_ARGV1 };

  {
    lw6sys_list_t *entries = NULL;
    char *map_path = NULL;
    char *user_dir = NULL;
    lw6ldr_entry_t *entry = NULL;
    int entry_found = 0;

    map_path = lw6cfg_unified_get_map_path (argc, argv);
    if (map_path)
      {
	user_dir = lw6sys_get_user_dir (argc, argv);
	if (user_dir)
	  {
	    entries =
	      lw6ldr_get_entries (map_path, TEST_DIR_RELATIVE_PATH, user_dir);
	    if (entries)
	      {
		while (entries
		       && (entry = lw6sys_lifo_pop (&entries)) != NULL)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("found map \"%s\" in \"%s\""),
				entry->title, entry->absolute_path);
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
	LW6SYS_FREE (map_path);
      }
    else
      {
	ret = 0;
      }
    if (!entry_found)
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing param
 */
static int
test_param ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_param_t param;
    lw6sys_assoc_t *values = NULL;

    memset (&param, 0, sizeof (lw6map_param_t));
    lw6map_param_defaults (&param);

    values = lw6sys_assoc_new (NULL);
    if (values)
      {
	lw6sys_assoc_set (&values, TEST_PARAM_KEY1, TEST_PARAM_VALUE1);
	lw6sys_assoc_set (&values, TEST_PARAM_KEY2, TEST_PARAM_VALUE2);
	lw6sys_assoc_set (&values, TEST_PARAM_KEY3, TEST_PARAM_VALUE3);

	if (lw6ldr_param_update (&param, values))
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
  return ret;
}

/*
 * Testing hints
 */
static int
test_hints ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6ldr_hints_t hints;
    lw6sys_assoc_t *values = NULL;

    lw6ldr_hints_zero (&hints);
    lw6ldr_hints_defaults (&hints);

    values = lw6sys_assoc_new (NULL);
    if (values)
      {
	lw6sys_assoc_set (&values, TEST_HINTS_KEY1, TEST_HINTS_VALUE1);
	lw6sys_assoc_set (&values, TEST_HINTS_KEY2, TEST_HINTS_VALUE2);
	lw6sys_assoc_set (&values, TEST_HINTS_KEY3, TEST_HINTS_VALUE3);

	if (lw6ldr_hints_update (&hints, values))
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
  return ret;
}

/*
 * Testing teams
 */
static int
test_teams ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6ldr_teams_t teams;
    lw6sys_assoc_t *values = NULL;

    lw6ldr_teams_zero (&teams);
    lw6ldr_teams_defaults (&teams);

    values = lw6sys_assoc_new (NULL);
    if (values)
      {
	lw6sys_assoc_set (&values, TEST_TEAMS_KEY1, TEST_TEAMS_VALUE1);
	lw6sys_assoc_set (&values, TEST_TEAMS_KEY2, TEST_TEAMS_VALUE2);
	lw6sys_assoc_set (&values, TEST_TEAMS_KEY3, TEST_TEAMS_VALUE3);

	if (lw6ldr_teams_update (&teams, values))
	  {
	    ret = 1;
	  }

	lw6sys_assoc_free (values);
      }
    else
      {
	ret = 0;
      }

    lw6ldr_teams_clear (&teams);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing reading
 */
static int
test_read ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;
  int argc = TEST_ARGC;
  char *argv[TEST_ARGC] = { TEST_ARGV0, TEST_ARGV1 };

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;
    char *map_path = NULL;
    char *user_dir = NULL;
    lw6sys_progress_t progress;
    float done = 0.0f;

    progress.min = TEST_PROGRESS_MIN;
    progress.max = TEST_PROGRESS_MAX;
    progress.value = &done;
    map_path = lw6cfg_unified_get_map_path (argc, argv);
    if (map_path)
      {
	user_dir = lw6sys_get_user_dir (argc, argv);
	if (user_dir)
	  {
	    level =
	      lw6ldr_read_relative (map_path, TEST_MAP, NULL, NULL,
				    TEST_DISPLAY_WIDTH, TEST_DISPLAY_HEIGHT,
				    LW6LDR_DEFAULT_BENCH_VALUE,
				    LW6LDR_DEFAULT_MAGIC_NUMBER, user_dir,
				    &progress);
	    if (level)
	      {
		repr = lw6map_repr (level);
		if (repr)
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
  return ret;
}

/**
 * lw6ldr_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @ldr module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6ldr_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6hlp_test (mode);
      lw6cfg_test (mode);
      lw6map_test (mode);
    }

  ret = test_dir () && test_hints () && test_teams () && test_param ()
    && test_read () && test_data ();

  return ret;
}
