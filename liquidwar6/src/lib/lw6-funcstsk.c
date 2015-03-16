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
#endif // HAVE_CONFIG_H

#include "liquidwar6.h"

/*
 * In liquidwar6tsk
 */
static SCM
_scm_lw6tsk_loader_new (SCM sleep)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  float c_sleep = 0.0f;
  char *user_dir = NULL;
  lw6tsk_loader_t *c_loader = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_number (sleep), sleep, SCM_ARG1, __FUNCTION__);

  c_sleep = scm_to_double (sleep);
  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (user_dir)
    {
      c_loader = lw6tsk_loader_new (sys_context, c_sleep, user_dir, &(lw6_global.progress));
      if (c_loader)
	{
	  ret = lw6_make_scm_loader (sys_context, c_loader);
	}
      LW6SYS_FREE (sys_context, user_dir);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6tsk_loader_push_ldr (SCM loader, SCM map_path, SCM relative_path,
			     SCM default_param, SCM forced_param, SCM display_width, SCM display_height, SCM bench_value, SCM magic_number)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6tsk_loader_t *c_loader = NULL;
  char *c_map_path = NULL;
  char *c_relative_path = NULL;
  lw6sys_assoc_t *c_default_param = NULL;
  lw6sys_assoc_t *c_forced_param = NULL;
  int c_display_width = 0;
  int c_display_height = 0;
  int c_bench_value = 0;
  int c_magic_number = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.loader, loader), loader, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (map_path), map_path, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (relative_path), relative_path, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (default_param) || default_param == SCM_EOL, default_param, SCM_ARG4, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (forced_param) || forced_param == SCM_EOL, forced_param, SCM_ARG5, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_width), display_width, SCM_ARG6, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_height), display_height, SCM_ARG7, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (bench_value), bench_value, SCM_ARGn, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (magic_number), magic_number, SCM_ARGn, __FUNCTION__);

  c_loader = lw6_scm_to_loader (sys_context, loader);
  c_map_path = lw6scm_utils_to_0str (sys_context, map_path);
  if (c_map_path)
    {
      c_relative_path = lw6scm_utils_to_0str (sys_context, relative_path);
      if (c_relative_path)
	{
	  c_default_param = lw6scm_utils_to_sys_str_assoc (sys_context, default_param);
	  if (c_default_param)
	    {
	      c_forced_param = lw6scm_utils_to_sys_str_assoc (sys_context, forced_param);
	      if (c_forced_param)
		{
		  c_display_width = scm_to_int (display_width);
		  c_display_height = scm_to_int (display_height);
		  c_bench_value = scm_to_int (bench_value);
		  c_magic_number = scm_to_int (magic_number);

		  lw6tsk_loader_push_ldr (sys_context, c_loader, c_map_path,
					  c_relative_path, c_default_param, c_forced_param, c_display_width, c_display_height, c_bench_value, c_magic_number);
		  ret = SCM_BOOL_T;

		  lw6sys_assoc_free (sys_context, c_forced_param);
		}
	      lw6sys_assoc_free (sys_context, c_default_param);
	    }
	  LW6SYS_FREE (sys_context, c_relative_path);
	}
      LW6SYS_FREE (sys_context, c_map_path);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6tsk_loader_push_gen (SCM loader, SCM seed, SCM display_width, SCM display_height, SCM bench_value, SCM magic_number)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6tsk_loader_t *c_loader = NULL;
  char *c_seed = NULL;
  int c_display_width = 0;
  int c_display_height = 0;
  int c_bench_value = 0;
  int c_magic_number = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.loader, loader), loader, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (seed), seed, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_width), display_width, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_height), display_height, SCM_ARG4, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (bench_value), bench_value, SCM_ARG5, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (magic_number), magic_number, SCM_ARG6, __FUNCTION__);

  c_loader = lw6_scm_to_loader (sys_context, loader);
  c_seed = lw6scm_utils_to_0str (sys_context, seed);
  if (c_seed)
    {
      c_display_width = scm_to_int (display_width);
      c_display_height = scm_to_int (display_height);
      c_bench_value = scm_to_int (bench_value);
      c_magic_number = scm_to_int (magic_number);

      lw6tsk_loader_push_gen (sys_context, c_loader, c_seed, c_display_width, c_display_height, c_bench_value, c_magic_number);
      ret = SCM_BOOL_T;

      LW6SYS_FREE (sys_context, c_seed);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6tsk_loader_pop (SCM loader)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6tsk_loader_t *c_loader = NULL;
  lw6map_level_t *c_level = NULL;
  lw6ker_game_struct_t *c_game_struct = NULL;
  lw6ker_game_state_t *c_game_state = NULL;
  int c_bench_value = 0;
  SCM level = SCM_BOOL_F;
  SCM game_struct = SCM_BOOL_F;
  SCM game_state = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.loader, loader), loader, SCM_ARG1, __FUNCTION__);

  c_loader = lw6_scm_to_loader (sys_context, loader);
  if (c_loader)
    {
      if (lw6tsk_loader_pop (sys_context, &c_level, &c_game_struct, &c_game_state, &c_bench_value, c_loader))
	{
	  if (c_level)
	    {
	      level = lw6_make_scm_map (sys_context, c_level);
	      if (c_game_struct)
		{
		  game_struct = lw6_make_scm_game_struct (sys_context, c_game_struct, level);
		  if (c_game_state)
		    {
		      game_state = lw6_make_scm_game_state (sys_context, c_game_state, game_struct);
		    }
		}
	    }
	  ret =
	    scm_list_4 (scm_cons
			(scm_from_locale_string ("level"), level),
			scm_cons
			(scm_from_locale_string ("game-struct"), game_struct),
			scm_cons (scm_from_locale_string ("game-state"),
				  game_state), scm_cons (scm_from_locale_string ("bench-value"), scm_from_int (c_bench_value)));
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6tsk_loader_get_stage (SCM loader)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6tsk_loader_t *c_loader = NULL;
  int c_ret = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.loader, loader), loader, SCM_ARG1, __FUNCTION__);

  c_loader = lw6_scm_to_loader (sys_context, loader);
  if (c_loader)
    {
      c_ret = lw6tsk_loader_get_stage (sys_context, c_loader);
      ret = scm_from_int (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_tsk
 *
 * @sys_context: global system context
 *
 * Register the functions of the tsk module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_tsk (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6tsk
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6TSK_LOADER_NEW, 1, 0, 0, (SCM (*)())_scm_lw6tsk_loader_new);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6TSK_LOADER_PUSH_LDR, 9, 0, 0, (SCM (*)())_scm_lw6tsk_loader_push_ldr);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6TSK_LOADER_PUSH_GEN, 6, 0, 0, (SCM (*)())_scm_lw6tsk_loader_push_gen);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6TSK_LOADER_POP, 1, 0, 0, (SCM (*)())_scm_lw6tsk_loader_pop);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6TSK_LOADER_GET_STAGE, 1, 0, 0, (SCM (*)())_scm_lw6tsk_loader_get_stage);

  return ret;
}
