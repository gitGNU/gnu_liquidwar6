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

#include "vox.h"
#include "vox-internal.h"

typedef struct _lw6vox_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6vox_test_data_t;

static _lw6vox_test_data_t _test_data = { 0, NULL };

/*
 * Testing functions in renderer.c
 */
static void
_test_renderer ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    lw6vox_renderer_t *renderer;

    level = lw6map_builtin_defaults (sys_context);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	if (game_struct)
	  {
	    game_state = lw6ker_game_state_new (sys_context, game_struct, NULL);
	    if (game_state)
	      {
		renderer = lw6vox_renderer_new (sys_context, game_state);
		if (renderer)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("vox renderer created"));
		    lw6vox_renderer_free (sys_context, renderer);
		  }
		else
		  {
		    ret = 0;
		  }
		lw6ker_game_state_free (sys_context, game_state);
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6ker_game_struct_free (sys_context, game_struct);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (sys_context, level);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libvox CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libvox CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6vox_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libvox module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6vox_test_register (lw6sys_context_t * sys_context, int mode)
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
      lw6cfg_test_register (sys_context, mode);
      lw6hlp_test_register (sys_context, mode);
      lw6map_test_register (sys_context, mode);
      lw6ker_test_register (sys_context, mode);
      lw6mat_test_register (sys_context, mode);
      lw6gui_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6vox", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_renderer);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6vox_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @vox module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6vox_test_run (lw6sys_context_t * sys_context, int mode)
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
