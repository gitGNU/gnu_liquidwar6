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
#endif // HAVE_CONFIG_H

#include "liquidwar6.h"

/*
 * In liquidwar6bot
 */

/*
 * In backend.c
 */
static SCM
_scm_lw6bot_get_backends ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_assoc_t *backends;
  lw6sys_list_t *keys;
  lw6sys_list_t *key;
  char *module_id;
  char *module_name;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  backends = lw6bot_get_backends (lw6_global.argc, lw6_global.argv);
  if (backends)
    {
      keys = lw6sys_assoc_keys (backends);
      if (keys)
	{
	  ret = SCM_EOL;
	  key = keys;
	  while (key)
	    {
	      if (key->data)
		{
		  module_id = (char *) key->data;
		  module_name =
		    (char *) lw6sys_assoc_get (backends, module_id);
		  ret =
		    scm_cons (scm_cons
			      (scm_from_locale_string (module_id),
			       scm_from_locale_string (module_name)), ret);
		}
	      key = lw6sys_list_next (key);
	    }
	  lw6sys_list_free (keys);
	  ret = scm_reverse (ret);
	}
      lw6sys_assoc_free (backends);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6bot_new (SCM backend_name, SCM game_state, SCM pilot, SCM dirty_read,
		 SCM cursor_id, SCM speed, SCM iq)
{
  lw6ker_game_state_t *c_game_state;
  lw6pil_pilot_t *c_pilot;
  int c_dirty_read;
  char *c_backend_name;
  char *c_cursor_id_str = NULL;
  u_int16_t c_cursor_id_int;
  float c_speed;
  int c_iq;
  lw6bot_backend_t *c_ret;
  lw6bot_seed_t c_seed;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (backend_name), backend_name, SCM_ARG1,
	      __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG5, __FUNCTION__);
  SCM_ASSERT (scm_is_number (speed), speed, SCM_ARG6, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (iq), iq, SCM_ARG7, __FUNCTION__);

  c_backend_name = lw6scm_utils_to_0str (backend_name);
  if (backend_name)
    {
      c_game_state = lw6_scm_to_game_state (game_state);
      if (c_game_state)
	{
	  c_pilot = lw6_scm_to_pilot (pilot);
	  if (c_pilot)
	    {
	      c_dirty_read = scm_to_int (dirty_read);
	      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
	      if (c_cursor_id_str)
		{
		  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
		  c_speed = scm_to_double (speed);
		  c_iq = scm_to_int (iq);
		  c_ret =
		    lw6bot_create_backend (lw6_global.argc, lw6_global.argv,
					   c_backend_name);
		  if (c_ret)
		    {
		      memset (&c_seed, 0, sizeof (lw6bot_seed_t));
		      c_seed.game_state = c_game_state;
		      c_seed.pilot = c_pilot;
		      c_seed.dirty_read = c_dirty_read;
		      c_seed.param.cursor_id = c_cursor_id_int;
		      c_seed.param.speed = c_speed;
		      c_seed.param.iq = c_iq;
		      if (lw6bot_init (c_ret, &c_seed))
			{
			  ret = lw6_make_scm_bot (c_ret, game_state, pilot);
			}
		      else
			{
			  lw6bot_destroy_backend (c_ret);
			}
		    }
		  LW6SYS_FREE (c_cursor_id_str);
		}
	    }
	}
      LW6SYS_FREE (c_backend_name);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6bot_next_move (SCM bot)
{
  lw6bot_backend_t *c_bot;
  int c_x = 0;
  int c_y = 0;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.bot, bot), bot, SCM_ARG1, __FUNCTION__);

  c_bot = lw6_scm_to_bot (bot);
  if (c_bot)
    {
      if (lw6bot_next_move (c_bot, &c_x, &c_y))
	{
	  ret = scm_list_2 (scm_cons
			    (scm_from_locale_string ("x"),
			     scm_from_int (c_x)),
			    scm_cons (scm_from_locale_string ("y"),
				      scm_from_int (c_y)));
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_bot
 *
 * Register the functions of the bot module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_bot ()
{
  int ret = 1;

  /*
   * In liquidwar6bot
   */
  /*
   * In backend.c
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6BOT_GET_BACKENDS, 0, 0, 0,
				      (SCM (*)())_scm_lw6bot_get_backends);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6BOT_NEW, 7, 0, 0,
				      (SCM (*)())_scm_lw6bot_new);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6BOT_NEXT_MOVE, 1, 0, 0,
				      (SCM (*)())_scm_lw6bot_next_move);

  return ret;
}
