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

#include "vox.h"
#include "vox-internal.h"

/*
 * Testing functions in renderer.c
 */
static int
test_renderer ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    lw6vox_renderer_t *renderer;

    level = lw6map_builtin_defaults ();
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		renderer = lw6vox_renderer_new (game_state);
		if (renderer)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("vox renderer created"));
		    lw6vox_renderer_free (renderer);
		  }
		else
		  {
		    ret = 0;
		  }
		lw6ker_game_state_free (game_state);
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6ker_game_struct_free (game_struct);
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
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6vox_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @vox module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6vox_test (int mode)
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
      lw6gui_test (mode);
    }

  ret = test_renderer ();

  return ret;
}
