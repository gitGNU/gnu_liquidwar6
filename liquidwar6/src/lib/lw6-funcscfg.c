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
 * In defaults.c
 */
static SCM
_scm_lw6cfg_defaults ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cfg_context != NULL)
    {
      if (lw6cfg_defaults (sys_context, lw6_global.cfg_context))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cfg_load (SCM filename)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cfg_context != NULL)
    {
      char *c_filename;

      SCM_ASSERT (scm_is_string (filename), filename, SCM_ARG1, __FUNCTION__);

      c_filename = lw6scm_utils_to_0str (sys_context, filename);
      if (c_filename)
	{
	  if (lw6cfg_load (sys_context, lw6_global.cfg_context, c_filename))
	    {
	      ret = SCM_BOOL_T;
	    }
	  else
	    {
	      ret = SCM_BOOL_F;
	    }
	  LW6SYS_FREE (sys_context, c_filename);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In option.c
 */
static SCM
_scm_lw6cfg_option_exists (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cfg_context != NULL)
    {
      char *c_key;

      SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

      c_key = lw6scm_utils_to_0str (sys_context, key);
      if (c_key)
	{
	  if (lw6cfg_option_exists (sys_context, lw6_global.cfg_context, c_key))
	    {
	      ret = SCM_BOOL_T;
	    }
	  else
	    {
	      ret = SCM_BOOL_F;
	    }
	  LW6SYS_FREE (sys_context, c_key);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cfg_get_option (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cfg_context != NULL)
    {
      char *c_key;

      SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

      c_key = lw6scm_utils_to_0str (sys_context, key);
      if (c_key)
	{
	  char *value = NULL;

	  value = lw6cfg_get_option (sys_context, lw6_global.cfg_context, c_key);
	  if (value)
	    {
	      ret = scm_from_locale_string (value);
	      LW6SYS_FREE (sys_context, value);
	    }

	  LW6SYS_FREE (sys_context, c_key);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cfg_set_option (SCM key, SCM value)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cfg_context != NULL)
    {
      char *c_key;
      char *c_value;

      SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);
      SCM_ASSERT (scm_is_string (value), value, SCM_ARG2, __FUNCTION__);

      c_key = lw6scm_utils_to_0str (sys_context, key);
      if (c_key)
	{
	  c_value = lw6scm_utils_to_0str (sys_context, value);
	  if (c_value)
	    {
	      lw6cfg_set_option (sys_context, lw6_global.cfg_context, c_key, c_value);

	      LW6SYS_FREE (sys_context, c_value);
	    }
	  LW6SYS_FREE (sys_context, c_key);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/*
 * In save.c
 */
static SCM
_scm_lw6cfg_save (SCM filename)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cfg_context != NULL)
    {
      char *c_filename;

      SCM_ASSERT (scm_is_string (filename), filename, SCM_ARG1, __FUNCTION__);

      c_filename = lw6scm_utils_to_0str (sys_context, filename);
      if (c_filename)
	{
	  if (lw6cfg_save (sys_context, lw6_global.cfg_context, c_filename))
	    {
	      ret = SCM_BOOL_T;
	    }
	  else
	    {
	      ret = SCM_BOOL_F;
	    }
	  LW6SYS_FREE (sys_context, c_filename);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In setup.c
 */
static SCM
_scm_lw6cfg_init ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  lw6_global.cfg_context = lw6cfg_init (sys_context, lw6_global.argc, lw6_global.argv);
  lw6_global.cfg_initialized = lw6_global.cfg_context ? 1 : 0;
  ret = lw6_global.cfg_context ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cfg_quit ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cfg_context != NULL)
    {
      lw6cfg_quit (sys_context, lw6_global.cfg_context);
    }

  lw6_global.cfg_context = NULL;
  lw6_global.cfg_initialized = 0;

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/*
 * Unified
 */

static SCM
_scm_lw6cfg_unified_get_user_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cfg_unified_get_log_file ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6cfg_unified_get_log_file (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cfg_unified_get_music_path ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6cfg_unified_get_music_path (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cfg_unified_get_map_path ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6cfg_unified_get_map_path (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_cfg
 *
 * @sys_context: global system context
 *
 * Register the functions of the cfg module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_cfg (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In defaults.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_DEFAULTS, 0, 0, 0, (SCM (*)())_scm_lw6cfg_defaults);
  /*
   * In load.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_LOAD, 1, 0, 0, (SCM (*)())_scm_lw6cfg_load);
  /*
   * In option.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_OPTION_EXISTS, 1, 0, 0, (SCM (*)())_scm_lw6cfg_option_exists);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_GET_OPTION, 1, 0, 0, (SCM (*)())_scm_lw6cfg_get_option);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_SET_OPTION, 2, 0, 0, (SCM (*)())_scm_lw6cfg_set_option);
  /*
   * In save.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_SAVE, 1, 0, 0, (SCM (*)())_scm_lw6cfg_save);
  /*
   * In setup.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_INIT, 0, 0, 0, (SCM (*)())_scm_lw6cfg_init);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_QUIT, 0, 0, 0, (SCM (*)())_scm_lw6cfg_quit);
  /*
   * In unified.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_UNIFIED_GET_USER_DIR, 0, 0, 0, (SCM (*)())_scm_lw6cfg_unified_get_user_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_UNIFIED_GET_LOG_FILE, 0, 0, 0, (SCM (*)())_scm_lw6cfg_unified_get_log_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_UNIFIED_GET_MUSIC_PATH, 0, 0, 0, (SCM (*)())_scm_lw6cfg_unified_get_music_path);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CFG_UNIFIED_GET_MAP_PATH, 0, 0, 0, (SCM (*)())_scm_lw6cfg_unified_get_map_path);

  return ret;
}
