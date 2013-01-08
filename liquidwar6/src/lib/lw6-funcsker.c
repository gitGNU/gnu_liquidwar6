/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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
 * In liquidwar6ker
 */
static SCM
_scm_lw6ker_build_game_struct (SCM level)
{
  lw6map_level_t *c_level;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.map, level), level,
	      SCM_ARG1, __FUNCTION__);
  c_level = lw6_scm_to_map (level);
  if (c_level)
    {
      lw6ker_game_struct_t *c_game_struct;

      c_game_struct = lw6ker_game_struct_new (c_level, NULL);
      if (c_game_struct)
	{
	  ret = lw6_make_scm_game_struct (c_game_struct, level);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_build_game_state (SCM game_struct)
{
  lw6ker_game_struct_t *c_game_struct;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_struct,
	       game_struct), game_struct, SCM_ARG1, __FUNCTION__);
  c_game_struct = lw6_scm_to_game_struct (game_struct);
  if (c_game_struct)
    {
      lw6ker_game_state_t *c_game_state;
      c_game_state = lw6ker_game_state_new (c_game_struct, NULL);
      if (c_game_state)
	{
	  ret = lw6_make_scm_game_state (c_game_state, game_struct);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_sync_game_state (SCM dst, SCM src)
{
  lw6ker_game_state_t *c_dst;
  lw6ker_game_state_t *c_src;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       dst), dst, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       src), src, SCM_ARG2, __FUNCTION__);
  c_dst = lw6_scm_to_game_state (dst);
  c_src = lw6_scm_to_game_state (src);
  if (c_dst && c_src)
    {
      ret = lw6ker_game_state_sync (c_dst, c_src) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_dup_game_state (SCM game_struct, SCM game_state)
{
  lw6ker_game_struct_t *c_game_struct;
  lw6ker_game_state_t *c_game_state;
  lw6ker_game_state_t *c_ret;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_struct,
	       game_struct), game_struct, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG2, __FUNCTION__);

  c_game_struct = lw6_scm_to_game_struct (game_struct);
  if (c_game_struct)
    {
      c_game_state = lw6_scm_to_game_state (game_state);
      if (c_game_state)
	{
	  c_ret = lw6ker_game_state_new (c_game_struct, NULL);
	  if (c_ret)
	    {
	      lw6ker_game_state_sync (c_ret, c_game_state);
	      ret = lw6_make_scm_game_state (c_ret, game_struct);
	    }
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_game_struct_checksum (SCM game_struct)
{
  lw6ker_game_struct_t *c_game_struct;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_struct,
	       game_struct), game_struct, SCM_ARG1, __FUNCTION__);
  c_game_struct = lw6_scm_to_game_struct (game_struct);
  if (c_game_struct)
    {
      ret = scm_from_int (lw6ker_game_struct_checksum (c_game_struct));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_game_state_checksum (SCM game_state)
{
  lw6ker_game_state_t *c_game_state;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      ret = scm_from_int (lw6ker_game_state_checksum (c_game_state));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_register_node (SCM game_state, SCM node_id)
{
  SCM ret = SCM_BOOL_F;
  char *c_node_id_str;
  u_int64_t c_node_id_int = 0LL;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (node_id), node_id, SCM_ARG2, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_node_id_str = lw6scm_utils_to_0str (node_id);
      if (c_node_id_str)
	{
	  c_node_id_int = lw6sys_id_atol (c_node_id_str);
	  LW6SYS_FREE (c_node_id_str);
	}

      if (lw6sys_check_id (c_node_id_int))
	{
	  ret =
	    lw6ker_game_state_register_node (c_game_state,
					     c_node_id_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_unregister_node (SCM game_state, SCM node_id)
{
  SCM ret = SCM_BOOL_F;
  char *c_node_id_str;
  u_int64_t c_node_id_int = 0LL;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (node_id), node_id, SCM_ARG2, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_node_id_str = lw6scm_utils_to_0str (node_id);
      if (c_node_id_str)
	{
	  c_node_id_int = lw6sys_id_atol (c_node_id_str);
	  LW6SYS_FREE (c_node_id_str);
	}

      if (lw6sys_check_id (c_node_id_int))
	{
	  ret =
	    lw6ker_game_state_unregister_node (c_game_state,
					       c_node_id_int) ? SCM_BOOL_T
	    : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_node_exists (SCM game_state, SCM node_id)
{
  SCM ret = SCM_BOOL_F;
  char *c_node_id_str;
  u_int64_t c_node_id_int = 0;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (node_id), node_id, SCM_ARG2, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_node_id_str = lw6scm_utils_to_0str (node_id);
      if (c_node_id_str)
	{
	  c_node_id_int = lw6sys_id_atol (c_node_id_str);
	  LW6SYS_FREE (c_node_id_str);
	}

      if (lw6sys_check_id (c_node_id_int))
	{
	  ret =
	    lw6ker_game_state_node_exists (c_game_state,
					   c_node_id_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_add_cursor (SCM game_state, SCM node_id, SCM cursor_id,
			SCM team_color)
{
  SCM ret = SCM_BOOL_F;
  char *c_node_id_str;
  char *c_cursor_id_str;
  char *c_team_color_str;
  u_int64_t c_node_id_int = 0LL;
  u_int16_t c_cursor_id_int = 0;
  int c_team_color_int = LW6MAP_TEAM_COLOR_INVALID;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (node_id), node_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_string (team_color), team_color, SCM_ARG4, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_node_id_str = lw6scm_utils_to_0str (node_id);
      if (c_node_id_str)
	{
	  c_node_id_int = lw6sys_id_atol (c_node_id_str);
	  LW6SYS_FREE (c_node_id_str);
	}
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
	  LW6SYS_FREE (c_cursor_id_str);
	}
      c_team_color_str = lw6scm_utils_to_0str (team_color);
      if (c_team_color_str)
	{
	  c_team_color_int =
	    lw6map_team_color_key_to_index (c_team_color_str);
	  LW6SYS_FREE (c_team_color_str);
	}

      if (lw6sys_check_id (c_node_id_int)
	  && lw6sys_check_id (c_cursor_id_int)
	  && c_team_color_int != LW6MAP_TEAM_COLOR_INVALID)
	{
	  ret =
	    lw6ker_game_state_add_cursor (c_game_state, c_node_id_int,
					  c_cursor_id_int,
					  c_team_color_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_remove_cursor (SCM game_state, SCM node_id, SCM cursor_id)
{
  SCM ret = SCM_BOOL_F;
  char *c_node_id_str;
  char *c_cursor_id_str;
  u_int64_t c_node_id_int = 0LL;
  u_int16_t c_cursor_id_int = 0;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (node_id), node_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG3, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_node_id_str = lw6scm_utils_to_0str (node_id);
      if (c_node_id_str)
	{
	  c_node_id_int = lw6sys_id_atol (c_node_id_str);
	  LW6SYS_FREE (c_node_id_str);
	}
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
	  LW6SYS_FREE (c_cursor_id_str);
	}

      if (lw6sys_check_id (c_node_id_int)
	  && lw6sys_check_id (c_cursor_id_int))
	{
	  ret =
	    lw6ker_game_state_remove_cursor (c_game_state, c_node_id_int,
					     c_cursor_id_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_cursor_exists (SCM game_state, SCM cursor_id)
{
  SCM ret = SCM_BOOL_F;
  char *c_cursor_id_str;
  u_int16_t c_cursor_id_int = 0;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
	  LW6SYS_FREE (c_cursor_id_str);
	}

      if (lw6sys_check_id (c_cursor_id_int))
	{
	  ret =
	    lw6ker_game_state_cursor_exists (c_game_state,
					     c_cursor_id_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_get_cursor (SCM game_state, SCM cursor_id)
{
  SCM ret = SCM_BOOL_F;
  char *c_cursor_id_str;
  u_int16_t c_cursor_id_int = 0;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
	  LW6SYS_FREE (c_cursor_id_str);
	}

      if (lw6sys_check_id (c_cursor_id_int))
	{
	  char *c_team_color_str = NULL;
	  char *c_node_id_str = NULL;
	  char c_letter_char = '\0';
	  char c_letter_str[2] = { 0, 0 };
	  lw6ker_cursor_t c_cursor;

	  if (lw6ker_game_state_get_cursor (c_game_state,
					    &c_cursor, c_cursor_id_int))
	    {
	      c_team_color_str =
		lw6map_team_color_index_to_key (c_cursor.team_color);
	      if (c_team_color_str)
		{
		  c_node_id_str = lw6sys_id_ltoa (c_cursor.node_id);
		  if (c_node_id_str)
		    {
		      c_letter_str[0] = c_letter_char;
		      ret = scm_list_5 (scm_cons
					(scm_from_locale_string ("node-id"),
					 scm_from_locale_string
					 (c_node_id_str)),
					scm_cons (scm_from_locale_string
						  ("letter"),
						  scm_from_locale_string
						  (c_letter_str)),
					scm_cons (scm_from_locale_string
						  ("team-color"),
						  scm_from_locale_string
						  (c_team_color_str)),
					scm_cons (scm_from_locale_string
						  ("x"),
						  scm_from_int (c_cursor.
								pos.x)),
					scm_cons (scm_from_locale_string
						  ("y"),
						  scm_from_int (c_cursor.
								pos.y)));
		      LW6SYS_FREE (c_node_id_str);
		    }
		}
	    }
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_set_cursor (SCM game_state, SCM node_id, SCM cursor_id, SCM x,
			SCM y)
{
  SCM ret = SCM_BOOL_F;
  char *c_node_id_str;
  char *c_cursor_id_str;
  lw6ker_cursor_t c_cursor;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (node_id), node_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (x), x, SCM_ARG4, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (y), y, SCM_ARG5, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      lw6ker_cursor_reset (&c_cursor);

      c_node_id_str = lw6scm_utils_to_0str (node_id);
      if (c_node_id_str)
	{
	  c_cursor.node_id = lw6sys_id_atol (c_node_id_str);
	  LW6SYS_FREE (c_node_id_str);
	}
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor.cursor_id = lw6sys_id_atol (c_cursor_id_str);
	  LW6SYS_FREE (c_cursor_id_str);
	}
      c_cursor.pos.x = scm_to_int (x);
      c_cursor.pos.y = scm_to_int (y);

      if (lw6sys_check_id (c_cursor.node_id)
	  && lw6sys_check_id (c_cursor.cursor_id))
	{
	  ret =
	    lw6ker_game_state_set_cursor (c_game_state, &c_cursor) ?
	    SCM_BOOL_T : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_do_round (SCM game_state)
{
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  lw6ker_game_state_t *c_game_state;
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      lw6ker_game_state_do_round (c_game_state);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6ker_get_moves (SCM game_state)
{
  SCM ret = SCM_BOOL_F;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      ret = scm_from_int (lw6ker_game_state_get_moves (c_game_state));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_get_spreads (SCM game_state)
{
  SCM ret = SCM_BOOL_F;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      ret = scm_from_int (lw6ker_game_state_get_spreads (c_game_state));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_get_rounds (SCM game_state)
{
  SCM ret = SCM_BOOL_F;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      ret = scm_from_int (lw6ker_game_state_get_rounds (c_game_state));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_is_over (SCM game_state)
{
  SCM ret = SCM_BOOL_F;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      if (lw6ker_game_state_is_over (c_game_state))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_did_cursor_win (SCM game_state, SCM cursor_id)
{
  SCM ret = SCM_BOOL_F;
  char *c_cursor_id_str;
  u_int16_t c_cursor_id_int = 0;
  lw6ker_game_state_t *c_game_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
	  LW6SYS_FREE (c_cursor_id_str);
	}

      if (lw6sys_check_id (c_cursor_id_int))
	{
	  ret =
	    lw6ker_game_state_did_cursor_win (c_game_state,
					      c_cursor_id_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_get_nb_colors (SCM game_state)
{
  lw6ker_game_state_t *c_game_state = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      ret = scm_from_int (lw6ker_game_state_get_nb_colors (c_game_state));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_get_nb_cursors (SCM game_state)
{
  lw6ker_game_state_t *c_game_state = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      ret = scm_from_int (lw6ker_game_state_get_nb_cursors (c_game_state));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ker_get_nb_nodes (SCM game_state)
{
  lw6ker_game_state_t *c_game_state = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      ret = scm_from_int (lw6ker_game_state_get_nb_nodes (c_game_state));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_ker
 *
 * Register the functions of the ker module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_ker ()
{
  int ret = 1;

  /*
   * In liquidwar6ker
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_BUILD_GAME_STRUCT,
				      1, 0, 0,
				      (SCM (*)
				       ())_scm_lw6ker_build_game_struct);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_BUILD_GAME_STATE, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_build_game_state);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_SYNC_GAME_STATE, 2, 0, 0,
			      (SCM (*)())_scm_lw6ker_sync_game_state);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_DUP_GAME_STATE, 2, 0, 0,
			      (SCM (*)())_scm_lw6ker_dup_game_state);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GAME_STRUCT_CHECKSUM, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_game_struct_checksum);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GAME_STATE_CHECKSUM, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_game_state_checksum);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_REGISTER_NODE, 2, 0, 0,
			      (SCM (*)())_scm_lw6ker_register_node);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_UNREGISTER_NODE, 2, 0, 0,
			      (SCM (*)())_scm_lw6ker_unregister_node);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_NODE_EXISTS, 2, 0, 0,
			      (SCM (*)())_scm_lw6ker_node_exists);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_ADD_CURSOR, 4, 0, 0,
			      (SCM (*)())_scm_lw6ker_add_cursor);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_REMOVE_CURSOR, 3, 0, 0,
			      (SCM (*)())_scm_lw6ker_remove_cursor);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_CURSOR_EXISTS, 2, 0, 0,
			      (SCM (*)())_scm_lw6ker_cursor_exists);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GET_CURSOR, 2, 0, 0,
			      (SCM (*)())_scm_lw6ker_get_cursor);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_SET_CURSOR, 5, 0, 0,
			      (SCM (*)())_scm_lw6ker_set_cursor);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_DO_ROUND, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_do_round);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GET_MOVES, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_get_moves);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GET_SPREADS, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_get_spreads);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GET_ROUNDS, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_get_rounds);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_IS_OVER, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_is_over);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_DID_CURSOR_WIN, 5, 0, 0,
			      (SCM (*)())_scm_lw6ker_did_cursor_win);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GET_NB_COLORS, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_get_nb_colors);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GET_NB_CURSORS, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_get_nb_cursors);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6KER_GET_NB_NODES, 1, 0, 0,
			      (SCM (*)())_scm_lw6ker_get_nb_nodes);

  return ret;
}
