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

#include "img.h"

#define TEST_ARGC 1
#define TEST_ARGV0 "foo"

/*
 * Testing functions in screenshot.c
 */
static int
test_screenshot ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = TEST_ARGC;
    const char *argv[TEST_ARGC] = { TEST_ARGV0 };
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6sys_whd_t shape = { 0, 0, 0 };
    char *user_dir = NULL;
    lw6img_jpeg_t *jpeg = NULL;

    level = lw6map_builtin_defaults ();
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		user_dir = lw6sys_get_user_dir (argc, argv);
		if (user_dir)
		  {
		    jpeg =
		      lw6img_screenshot_new (game_state, user_dir,
					     LW6IMG_JPEG_QUALITY_DEFAULT);
		    if (jpeg)
		      {
			lw6ker_game_state_get_shape (game_state, &shape);
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("generated screenshot %dx%d"),
				    shape.w, shape.h);
			lw6img_screenshot_free (jpeg);
			jpeg = NULL;
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_ ("unable to generate screenshot"));
			ret = 0;
		      }
		    LW6SYS_FREE (user_dir);
		    user_dir = NULL;
		  }
		else
		  {
		    ret = 0;
		  }
		lw6ker_game_state_free (game_state);
		game_state = NULL;
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6ker_game_struct_free (game_struct);
	    game_struct = NULL;
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (level);
	level = NULL;
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
 * lw6img_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @img module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6img_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6cfg_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
    }

  ret = test_screenshot ();

  return ret;
}
