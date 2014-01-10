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
#endif // HAVE_CONFIG_H

#include "liquidwar6.h"

/*
 * In liquidwar6img
 */

static SCM
_scm_lw6img_screenshot (SCM game_state, SCM quality)
{
  SCM ret = SCM_BOOL_F;
  lw6ker_game_state_t *c_game_state = NULL;
  char *c_user_dir = NULL;
  int c_quality;
  lw6img_jpeg_t *c_jpeg = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state, game_state), game_state,
	      SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (quality), quality, SCM_ARG2, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_user_dir =
	lw6cfg_unified_get_user_dir (lw6_global.argc, lw6_global.argv);
      if (c_user_dir)
	{
	  c_quality = scm_to_int (quality);

	  c_jpeg =
	    lw6img_screenshot_new (c_game_state, c_user_dir, c_quality);
	  /*
	   * Note that it's valid *not* to have a screenshot for a game,
	   * this is typically the case if there's no active game,
	   * however, here, c_jpeg should be something as c_game_state
	   * is valid.
	   */
	  if (c_jpeg)
	    {
	      ret = lw6_make_scm_jpeg (c_jpeg);
	    }
	  LW6SYS_FREE (c_user_dir);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_img
 *
 * Register the functions of the img module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_img ()
{
  int ret = 1;

  /*
   * In liquidwar6img
   */
  ret = ret && lw6scm_c_define_gsubr
    (LW6DEF_C_LW6IMG_SCREENSHOT, 2, 0, 0, (SCM (*)())_scm_lw6img_screenshot);

  return ret;
}
