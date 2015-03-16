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
 * In liquidwar6hlp
 */
static SCM
_scm_lw6hlp_about (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;
  const char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      c_ret = lw6hlp_about (sys_context, NULL, NULL, NULL, NULL, c_key);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  // no need to free c_ret
	}
      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_get_default_value (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;
  const char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      c_ret = lw6hlp_get_default_value (sys_context, c_key);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  // no need to free c_ret
	}
      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_quick ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_quick (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_doc ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_doc (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_show ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_show (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_path ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_path (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_players ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_players (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_input ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_input (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_graphics ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_graphics (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_sound ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_sound (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_network ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_network (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_rules ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_rules (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_hints ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_hints (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_style ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_style (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_teams ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_teams (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_funcs ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_funcs (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_hooks ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_hooks (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_advanced ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_advanced (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_aliases ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_aliases (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_team_colors ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_team_colors (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_weapons ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_weapons (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_list = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list (sys_context);
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (sys_context, c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_hlp
 *
 * @sys_context: global system context
 *
 * Register the functions of the hlp module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_hlp (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6hlp
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_ABOUT, 1, 0, 0, (SCM (*)())_scm_lw6hlp_about);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_GET_DEFAULT_VALUE, 1, 0, 0, (SCM (*)())_scm_lw6hlp_get_default_value);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_QUICK, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_quick);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_DOC, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_doc);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_SHOW, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_show);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_PATH, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_path);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_PLAYERS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_players);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_INPUT, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_input);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_GRAPHICS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_graphics);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_SOUND, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_sound);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_NETWORK, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_network);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_MAP, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_map);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_MAP_RULES, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_map_rules);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_MAP_HINTS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_map_hints);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_MAP_STYLE, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_map_style);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_MAP_TEAMS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_map_teams);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_FUNCS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_funcs);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_HOOKS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_hooks);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_ADVANCED, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_advanced);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_ALIASES, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_aliases);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_TEAM_COLORS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_team_colors);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST_WEAPONS, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list_weapons);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6HLP_LIST, 0, 0, 0, (SCM (*)())_scm_lw6hlp_list);

  return ret;
}
