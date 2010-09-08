/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "tsk.h"

#define TEST_MANAGER_SLEEP 1.0f
#define TEST_LOOP_SLEEP 0.1f
#define TEST_LOOP_N 600
#define TEST_LOAD_MAP "subflower"
#define TEST_OPTION_KEY1 LW6DEF_TOTAL_TIME
#define TEST_OPTION_VALUE1 "250"
#define TEST_OPTION_KEY2 LW6DEF_BACKGROUND_STYLE
#define TEST_OPTION_VALUE2 "air"
#define TEST_DISPLAY_WIDTH 640
#define TEST_DISPLAY_HEIGHT 480

/*
 * Testing task manager
 */
static int
test_manager ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6tsk_loader_t *manager = NULL;
    char *repr;
    int i;
    char *map_path;
    lw6sys_assoc_t *default_param;
    lw6sys_assoc_t *forced_param;
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    int done = 0;
    float progress = 0.0f;

    manager = lw6tsk_loader_new (TEST_MANAGER_SLEEP, &progress);
    if (manager)
      {
	map_path = lw6sys_get_default_map_path ();
	if (map_path)
	  {
	    default_param = lw6sys_assoc_new (lw6sys_free_callback);
	    forced_param = lw6sys_assoc_new (lw6sys_free_callback);
	    lw6sys_assoc_set (&default_param, TEST_OPTION_KEY1,
			      lw6sys_str_copy (TEST_OPTION_VALUE1));
	    lw6sys_assoc_set (&forced_param, TEST_OPTION_KEY2,
			      lw6sys_str_copy (TEST_OPTION_VALUE2));
	    if (default_param && forced_param)
	      {
		lw6tsk_loader_push (manager, map_path, TEST_LOAD_MAP,
				    default_param, forced_param,
				    TEST_DISPLAY_WIDTH, TEST_DISPLAY_HEIGHT,
				    LW6LDR_DEFAULT_BENCH_VALUE,
				    LW6LDR_DEFAULT_MAGIC_NUMBER);
		lw6sys_sleep (TEST_LOOP_SLEEP);
		lw6tsk_loader_push (manager, map_path, TEST_LOAD_MAP,
				    default_param, forced_param,
				    TEST_DISPLAY_WIDTH, TEST_DISPLAY_HEIGHT,
				    LW6LDR_DEFAULT_BENCH_VALUE,
				    LW6LDR_DEFAULT_MAGIC_NUMBER);
		for (i = 0; i < TEST_LOOP_N && !done; i++)
		  {
		    lw6sys_sleep (TEST_LOOP_SLEEP);
		    repr = lw6tsk_loader_repr (manager);
		    if (repr)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("waiting for manager \"%s\""), repr);
			LW6SYS_FREE (repr);
		      }
		    if (lw6tsk_loader_pop
			(&level, &game_struct, &game_state, manager))
		      {
			if (level && game_struct && game_state)
			  {
			    done = 1;
			    repr = lw6ker_game_struct_repr (game_struct);
			    if (repr)
			      {
				lw6sys_log (LW6SYS_LOG_NOTICE,
					    _x_ ("loaded game_struct \"%s\""),
					    repr);
				LW6SYS_FREE (repr);
			      }
			    repr = lw6ker_game_state_repr (game_state);
			    if (repr)
			      {
				lw6sys_log (LW6SYS_LOG_NOTICE,
					    _x_ ("loaded game_state \"%s\""),
					    repr);
				LW6SYS_FREE (repr);
			      }
			    lw6ker_game_state_free (game_state);
			    game_state = NULL;
			    lw6ker_game_struct_free (game_struct);
			    game_struct = NULL;
			    lw6map_free (level);
			    level = NULL;
			  }
			if (level)
			  {
			    repr = lw6map_repr (level);
			    if (repr)
			      {
				lw6sys_log (LW6SYS_LOG_NOTICE,
					    _x_ ("loaded map \"%s\""), repr);
				LW6SYS_FREE (repr);
			      }
			    lw6map_free (level);
			    level = NULL;
			  }
		      }
		  }
		lw6sys_assoc_free (default_param);
		lw6sys_assoc_free (forced_param);

	      }
	    LW6SYS_FREE (map_path);
	  }
	lw6tsk_loader_free (manager);
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
 * lw6tsk_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @tsk module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6tsk_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6cfg_test (mode);
      lw6hlp_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
      lw6ldr_test (mode);
    }

  ret = test_manager ();

  return ret;
}
