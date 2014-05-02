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
 * In liquidwar6hlp
 */
static SCM
_scm_lw6hlp_about (SCM key)
{
  char *c_key;
  const char *c_ret;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (key);
  if (c_key)
    {
      c_ret = lw6hlp_about (NULL, NULL, NULL, NULL, c_key);
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
  char *c_key;
  const char *c_ret;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (key);
  if (c_key)
    {
      c_ret = lw6hlp_get_default_value (c_key);
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
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_quick ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_doc ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_doc ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_show ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_show ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_path ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_path ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_players ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_players ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_input ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_input ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_graphics ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_graphics ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_sound ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_sound ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_network ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_network ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_rules ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_rules ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_hints ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_hints ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_style ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_style ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_map_teams ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_map_teams ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_funcs ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_funcs ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_hooks ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_hooks ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_advanced ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_advanced ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_aliases ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_aliases ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_team_colors ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_team_colors ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list_weapons ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list_weapons ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6hlp_list ()
{
  lw6sys_list_t *c_list = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  c_list = lw6hlp_list ();
  if (c_list)
    {
      ret = lw6scm_utils_to_scm_str_list (c_list);
      lw6sys_list_free (sys_context, c_list);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_hlp
 *
 * Register the functions of the hlp module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_hlp ()
{
  int ret = 1;

  /*
   * In liquidwar6hlp
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_ABOUT, 1, 0, 0,
				      (SCM (*)())_scm_lw6hlp_about);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_GET_DEFAULT_VALUE, 1, 0, 0,
			      (SCM (*)())_scm_lw6hlp_get_default_value);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_QUICK, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_quick);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_DOC, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_doc);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_SHOW, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_show);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_PATH, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_path);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_PLAYERS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_players);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_INPUT, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_input);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_GRAPHICS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_graphics);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_SOUND, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_sound);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_NETWORK, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_network);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_MAP, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_map);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_MAP_RULES, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_map_rules);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_MAP_HINTS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_map_hints);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_MAP_STYLE, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_map_style);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_MAP_TEAMS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_map_teams);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_FUNCS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_funcs);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_HOOKS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_hooks);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_ADVANCED, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_advanced);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_ALIASES, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_aliases);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_TEAM_COLORS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_team_colors);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST_WEAPONS, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list_weapons);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6HLP_LIST, 0, 0, 0,
			      (SCM (*)())_scm_lw6hlp_list);

  return ret;
}
