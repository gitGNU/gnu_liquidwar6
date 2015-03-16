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
 * In liquidwar6map
 */

static SCM
_scm_lw6map_get_look (SCM level)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_level = NULL;
  lw6gui_look_t *c_look = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      c_look = lw6gui_look_new (sys_context, &(c_level->param.style));
      if (c_look)
	{
	  ret = lw6_make_scm_look (sys_context, c_look);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_param_get (SCM level, SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_level = NULL;
  char *c_key = NULL;
  char *c_value = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (key), key, SCM_ARG2, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      c_key = lw6scm_utils_to_0str (sys_context, key);
      if (c_key)
	{
	  c_value = lw6map_param_get (sys_context, &(c_level->param), c_key);
	  if (c_value)
	    {
	      ret = scm_from_locale_string (c_value);
	      LW6SYS_FREE (sys_context, c_value);
	    }
	  LW6SYS_FREE (sys_context, c_key);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_get_title (SCM level)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_level = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      ret = scm_from_locale_string (lw6map_get_title (sys_context, c_level));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_get_music_dir (SCM level)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_level = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      if (c_level->local_info.music_dir)
	{
	  ret = scm_from_locale_string (c_level->local_info.music_dir);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_team_color_index_to_key (SCM index)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_index = 0;

  SCM_ASSERT (scm_is_integer (index), index, SCM_ARG1, __FUNCTION__);

  c_index = scm_to_int (index);
  ret = scm_from_locale_string (lw6map_team_color_index_to_key (sys_context, c_index));

  return ret;
}

static SCM
_scm_lw6map_team_color_key_to_index (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      ret = scm_from_int (lw6map_team_color_key_to_index (sys_context, c_key));
      LW6SYS_FREE (sys_context, c_key);
    }

  return ret;
}

static SCM
_scm_lw6map_team_color_index_to_label (SCM index)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_index = 0;

  SCM_ASSERT (scm_is_integer (index), index, SCM_ARG1, __FUNCTION__);

  c_index = scm_to_int (index);
  ret = scm_from_locale_string (lw6map_team_color_index_to_label (sys_context, c_index));

  return ret;
}

static SCM
_scm_lw6map_team_color_list ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int i = 0;
  int exp = LW6MAP_RULES_MIN_EXP;
  char *color_key = NULL;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (user_dir)
    {
      lw6cfg_load_exp (sys_context, user_dir, &exp);
      ret = SCM_EOL;
      for (i = lw6map_exp_get_highest_team_color_allowed (sys_context, exp); i >= 0; --i)
	{
	  color_key = lw6map_team_color_index_to_key (sys_context, i);
	  if (color_key)
	    {
	      ret = scm_cons (scm_from_locale_string (color_key), ret);
	    }
	}
      LW6SYS_FREE (sys_context, user_dir);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_weapon_index_to_key (SCM index)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_index;

  SCM_ASSERT (scm_is_integer (index), index, SCM_ARG1, __FUNCTION__);

  c_index = scm_to_int (index);
  ret = scm_from_locale_string (lw6map_weapon_index_to_key (sys_context, c_index));

  return ret;
}

static SCM
_scm_lw6map_weapon_key_to_index (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      ret = scm_from_int (lw6map_weapon_key_to_index (sys_context, c_key));
      LW6SYS_FREE (sys_context, c_key);
    }

  return ret;
}

static SCM
_scm_lw6map_weapon_index_to_label (SCM index)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_index;

  SCM_ASSERT (scm_is_integer (index), index, SCM_ARG1, __FUNCTION__);

  c_index = scm_to_int (index);
  ret = scm_from_locale_string (lw6map_weapon_index_to_label (sys_context, c_index));

  return ret;
}

static SCM
_scm_lw6map_weapon_list ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int i = 0;
  int exp = 0;
  char *weapon_key = NULL;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (user_dir)
    {
      if (lw6cfg_load_exp (sys_context, user_dir, &exp))
	{
	  ret = SCM_EOL;
	  for (i = lw6map_exp_get_highest_weapon_allowed (sys_context, exp); i >= 0; --i)
	    {
	      weapon_key = lw6map_weapon_index_to_key (sys_context, i);
	      if (weapon_key)
		{
		  ret = scm_cons (scm_from_locale_string (weapon_key), ret);
		}
	    }
	}
      LW6SYS_FREE (sys_context, user_dir);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In static.c
 */
static SCM
_scm_lw6map_rules_get_default (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      ret = scm_from_int (lw6map_rules_get_default (sys_context, c_key));

      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_rules_get_min (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      ret = scm_from_int (lw6map_rules_get_min (sys_context, c_key));

      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_rules_get_max (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      ret = scm_from_int (lw6map_rules_get_max (sys_context, c_key));

      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_rules_get_int (SCM game_struct, SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  lw6ker_game_struct_t *c_game_struct;
  char *c_key;

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_struct, game_struct), game_struct, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (key), key, SCM_ARG2, __FUNCTION__);

  c_game_struct = lw6_scm_to_game_struct (sys_context, game_struct);
  if (c_game_struct)
    {
      c_key = lw6scm_utils_to_0str (sys_context, key);
      if (c_key)
	{
	  ret = scm_from_int (lw6map_rules_get_int (sys_context, &(c_game_struct->rules), c_key));
	  LW6SYS_FREE (sys_context, c_key);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_style_get_default (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;
  char *c_value = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      c_value = lw6map_style_get_default (sys_context, c_key);
      if (c_value)
	{
	  ret = scm_from_locale_string (c_value);
	  LW6SYS_FREE (sys_context, c_value);
	}
      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_teams_get_default (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;
  char *c_value = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      c_value = lw6map_teams_get_default (sys_context, c_key);
      if (c_value)
	{
	  ret = scm_from_locale_string (c_value);
	  LW6SYS_FREE (sys_context, c_value);
	}
      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_exp_is_team_color_allowed (SCM level, SCM team_color)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_level = NULL;
  int c_team_color = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (team_color), team_color, SCM_ARG2, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      c_team_color = scm_to_int (team_color);

      ret = scm_from_int (lw6map_exp_is_team_color_allowed (sys_context, &(c_level->param.rules), c_team_color));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_exp_is_weapon_allowed (SCM level, SCM weapon)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_level = NULL;
  int c_weapon = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (weapon), weapon, SCM_ARG2, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      c_weapon = scm_to_int (weapon);

      ret = scm_from_int (lw6map_exp_is_weapon_allowed (sys_context, &(c_level->param.rules), c_weapon));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_exp_get_unlocked_team_color ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int exp = LW6MAP_RULES_MIN_EXP;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (user_dir)
    {
      lw6cfg_load_exp (sys_context, user_dir, &exp);

      ret = scm_from_int (lw6map_exp_get_unlocked_team_color (sys_context, exp));

      LW6SYS_FREE (sys_context, user_dir);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_exp_get_unlocked_weapon (SCM weapon)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int exp = LW6MAP_RULES_MIN_EXP;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (user_dir)
    {
      lw6cfg_load_exp (sys_context, user_dir, &exp);

      ret = scm_from_int (lw6map_exp_get_unlocked_weapon (sys_context, exp));

      LW6SYS_FREE (sys_context, user_dir);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_get_max_nb_colors (SCM level)
{
  lw6map_level_t *c_level = NULL;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      ret = scm_from_int (lw6map_get_max_nb_colors (sys_context, c_level));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_get_max_nb_cursors (SCM level)
{
  lw6map_level_t *c_level = NULL;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      ret = scm_from_int (lw6map_get_max_nb_cursors (sys_context, c_level));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6map_get_max_nb_nodes (SCM level)
{
  lw6map_level_t *c_level = NULL;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      ret = scm_from_int (lw6map_get_max_nb_nodes (sys_context, c_level));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_map
 *
 * @sys_context: global system context
 *
 * Register the functions of the map module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_map (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6map
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_GET_LOOK, 1, 0, 0, (SCM (*)())_scm_lw6map_get_look);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_PARAM_GET, 2, 0, 0, (SCM (*)())_scm_lw6map_param_get);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_GET_TITLE, 1, 0, 0, (SCM (*)())_scm_lw6map_get_title);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_GET_MUSIC_DIR, 1, 0, 0, (SCM (*)())_scm_lw6map_get_music_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_TEAM_COLOR_INDEX_TO_KEY, 1, 0, 0, (SCM (*)())_scm_lw6map_team_color_index_to_key);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_TEAM_COLOR_KEY_TO_INDEX, 1, 0, 0, (SCM (*)())_scm_lw6map_team_color_key_to_index);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_TEAM_COLOR_INDEX_TO_LABEL, 1, 0, 0, (SCM (*)())_scm_lw6map_team_color_index_to_label);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_TEAM_COLOR_LIST, 0, 0, 0, (SCM (*)())_scm_lw6map_team_color_list);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_WEAPON_INDEX_TO_KEY, 1, 0, 0, (SCM (*)())_scm_lw6map_weapon_index_to_key);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_WEAPON_KEY_TO_INDEX, 1, 0, 0, (SCM (*)())_scm_lw6map_weapon_key_to_index);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_WEAPON_INDEX_TO_LABEL, 1, 0, 0, (SCM (*)())_scm_lw6map_weapon_index_to_label);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_WEAPON_LIST, 0, 0, 0, (SCM (*)())_scm_lw6map_weapon_list);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_RULES_GET_DEFAULT, 1, 0, 0, (SCM (*)())_scm_lw6map_rules_get_default);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_RULES_GET_MIN, 1, 0, 0, (SCM (*)())_scm_lw6map_rules_get_min);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_RULES_GET_MAX, 1, 0, 0, (SCM (*)())_scm_lw6map_rules_get_max);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_RULES_GET_INT, 2, 0, 0, (SCM (*)())_scm_lw6map_rules_get_int);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_STYLE_GET_DEFAULT, 1, 0, 0, (SCM (*)())_scm_lw6map_style_get_default);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_TEAMS_GET_DEFAULT, 1, 0, 0, (SCM (*)())_scm_lw6map_teams_get_default);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_EXP_IS_TEAM_COLOR_ALLOWED, 2, 0, 0, (SCM (*)())_scm_lw6map_exp_is_team_color_allowed);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_EXP_IS_WEAPON_ALLOWED, 2, 0, 0, (SCM (*)())_scm_lw6map_exp_is_weapon_allowed);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_EXP_GET_UNLOCKED_TEAM_COLOR, 0, 0, 0, (SCM (*)())_scm_lw6map_exp_get_unlocked_team_color);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_EXP_GET_UNLOCKED_WEAPON, 0, 0, 0, (SCM (*)())_scm_lw6map_exp_get_unlocked_weapon);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_GET_MAX_NB_COLORS, 1, 0, 0, (SCM (*)())_scm_lw6map_get_max_nb_colors);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_GET_MAX_NB_CURSORS, 1, 0, 0, (SCM (*)())_scm_lw6map_get_max_nb_cursors);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6MAP_GET_MAX_NB_NODES, 1, 0, 0, (SCM (*)())_scm_lw6map_get_max_nb_nodes);

  return ret;
}
