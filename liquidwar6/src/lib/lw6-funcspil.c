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

// math.h needed because ceil is used for rounding
#include <math.h>

/*
 * In liquidwar6pil
 */
static SCM
_scm_lw6pil_bench ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_progress_t progress;
  float c_ret = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  lw6sys_progress_default (sys_context, &progress, &(lw6_global.progress));
  lw6sys_progress_begin (sys_context, &progress);

  if (lw6pil_bench (sys_context, lw6_global.argc, lw6_global.argv, &c_ret, &progress))
    {
      ret = scm_from_int (ceil (c_ret));
    }

  lw6sys_progress_begin (sys_context, &progress);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_seed_command_generate (SCM pilot, SCM server_id, SCM seq)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_ret = NULL;
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_server_id_str = NULL;
  u_int64_t c_server_id_int = 0LL;
  int64_t c_seq = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (server_id), server_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq), seq, SCM_ARG3, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_server_id_str = lw6scm_utils_to_0str (sys_context, server_id);
      if (c_server_id_str)
	{
	  c_server_id_int = lw6sys_id_atol (sys_context, c_server_id_str);
	  if (c_server_id_int > 0)
	    {
	      c_seq = scm_to_long_long (seq);
	      c_ret = lw6pil_seed_command_generate (sys_context, c_pilot, c_server_id_int, c_seq);
	      if (c_ret)
		{
		  ret = scm_from_locale_string (c_ret);
		  LW6SYS_FREE (sys_context, c_ret);
		}
	    }
	  LW6SYS_FREE (sys_context, c_server_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}


static SCM
_scm_lw6pil_dump_command_generate (SCM pilot, SCM server_id, SCM seq)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_server_id_str = NULL;
  u_int64_t c_server_id_int = 0LL;
  int64_t c_seq = 0LL;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (server_id), server_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq), seq, SCM_ARG3, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_server_id_str = lw6scm_utils_to_0str (sys_context, server_id);
      if (c_server_id_str)
	{
	  c_server_id_int = lw6sys_id_atol (sys_context, c_server_id_str);
	  if (c_server_id_int > 0)
	    {
	      c_seq = scm_to_long_long (seq);
	      c_ret = lw6pil_dump_command_generate (sys_context, c_pilot, c_server_id_int, c_seq);
	      if (c_ret)
		{
		  ret = scm_from_locale_string (c_ret);
		  LW6SYS_FREE (sys_context, c_ret);
		}
	    }
	  LW6SYS_FREE (sys_context, c_server_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_poll_dump (SCM command_text, SCM timestamp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_command_text = NULL;
  int64_t c_timestamp;
  lw6pil_dump_t c_dump;
  SCM ret_level;
  SCM ret_game_struct;
  SCM ret_game_state;
  SCM ret_pilot;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (command_text), command_text, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG2, __FUNCTION__);

  c_command_text = lw6scm_utils_to_0str (sys_context, command_text);
  if (c_command_text)
    {
      c_timestamp = scm_to_long_long (timestamp);
      if (lw6pil_nopilot_poll_dump (sys_context, &c_dump, c_command_text, c_timestamp))
	{
	  if (lw6pil_dump_exists (sys_context, &c_dump))
	    {
	      ret_level = lw6_make_scm_map (sys_context, c_dump.level);
	      ret_game_struct = lw6_make_scm_game_struct (sys_context, c_dump.game_struct, ret_level);
	      ret_game_state = lw6_make_scm_game_state (sys_context, c_dump.game_state, ret_game_struct);
	      ret_pilot = lw6_make_scm_pilot (sys_context, c_dump.pilot);
	      ret = scm_list_4 (scm_cons
				(scm_from_locale_string ("level"), ret_level),
				scm_cons
				(scm_from_locale_string ("game-struct"),
				 ret_game_struct),
				scm_cons (scm_from_locale_string ("game-state"), ret_game_state), scm_cons (scm_from_locale_string ("pilot"), ret_pilot));
	    }
	}
      LW6SYS_FREE (sys_context, c_command_text);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_build_pilot (SCM game_state, SCM seq_0, SCM timestamp)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;
  lw6ker_game_state_t *c_game_state = NULL;
  int64_t c_seq_0;
  int64_t c_timestamp;
  lw6pil_pilot_t *c_ret;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG3, __FUNCTION__);

  lw6sys_progress_default (sys_context, &progress, &(lw6_global.progress));
  lw6sys_progress_begin (sys_context, &progress);

  c_game_state = lw6_scm_to_game_state (sys_context, game_state);
  if (c_game_state)
    {
      c_seq_0 = scm_to_long_long (seq_0);
      c_timestamp = scm_to_long_long (timestamp);
      c_ret = lw6pil_pilot_new (sys_context, c_game_state, c_seq_0, c_timestamp, &progress);
      if (c_ret)
	{
	  ret = lw6_make_scm_pilot (sys_context, c_ret);
	}
    }

  lw6sys_progress_begin (sys_context, &progress);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_send_command (SCM pilot, SCM command_text, SCM verified)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_command_text = NULL;
  int c_verified;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (command_text), command_text, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (verified), verified, SCM_ARG3, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_command_text = lw6scm_utils_to_0str (sys_context, command_text);
      if (c_command_text)
	{
	  c_verified = SCM_NFALSEP (verified);
	  ret = lw6pil_pilot_send_command (sys_context, c_pilot, c_command_text, c_verified) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (sys_context, c_command_text);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_local_command (SCM pilot, SCM command_text)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_command_text = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (command_text), command_text, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_command_text = lw6scm_utils_to_0str (sys_context, command_text);
      if (c_command_text)
	{
	  ret = lw6pil_pilot_local_command (sys_context, c_pilot, c_command_text) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (sys_context, c_command_text);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_commit (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  lw6pil_dump_t c_dump;
  SCM ret_level;
  SCM ret_game_struct;
  SCM ret_game_state;
  SCM ret_pilot;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      if (lw6pil_pilot_commit (sys_context, &c_dump, c_pilot))
	{
	  if (lw6pil_dump_exists (sys_context, &c_dump))
	    {
	      ret_level = lw6_make_scm_map (sys_context, c_dump.level);
	      ret_game_struct = lw6_make_scm_game_struct (sys_context, c_dump.game_struct, ret_level);
	      ret_game_state = lw6_make_scm_game_state (sys_context, c_dump.game_state, ret_game_struct);
	      ret_pilot = lw6_make_scm_pilot (sys_context, c_dump.pilot);
	      ret = scm_list_4 (scm_cons
				(scm_from_locale_string ("level"), ret_level),
				scm_cons
				(scm_from_locale_string ("game-struct"),
				 ret_game_struct),
				scm_cons (scm_from_locale_string ("game-state"), ret_game_state), scm_cons (scm_from_locale_string ("pilot"), ret_pilot));
	    }
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_fix_coords (SCM game_state, SCM x, SCM y, SCM z)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_state_t *c_game_state = NULL;
  lw6sys_whd_t shape;
  float c_x = 0.0f;
  float c_y = 0.0f;
  float c_z = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_number (x), x, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (y), y, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (z), z, SCM_ARG3, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (sys_context, game_state);
  if (c_game_state)
    {
      c_x = scm_to_double (x);
      c_y = scm_to_double (y);
      c_z = scm_to_double (z);

      lw6ker_game_state_get_shape (sys_context, c_game_state, &shape);
      lw6pil_coords_fix (sys_context, &(c_game_state->game_struct->rules), &shape, &c_x, &c_y, &c_z);
      ret =
	scm_list_3 (scm_cons
		    (scm_from_locale_string ("x"), scm_from_double (c_x)),
		    scm_cons (scm_from_locale_string ("y"), scm_from_double (c_y)), scm_cons (scm_from_locale_string ("z"), scm_from_double (c_z)));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_fix_coords_x10 (SCM game_state, SCM x, SCM y, SCM z)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_state_t *c_game_state = NULL;
  lw6sys_whd_t shape;
  float c_x = 0.0f;
  float c_y = 0.0f;
  float c_z = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_number (x), x, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (y), y, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (z), z, SCM_ARG3, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (sys_context, game_state);
  if (c_game_state)
    {
      c_x = scm_to_double (x);
      c_y = scm_to_double (y);
      c_z = scm_to_double (z);

      lw6ker_game_state_get_shape (sys_context, c_game_state, &shape);
      lw6pil_coords_fix_x10 (sys_context, &(c_game_state->game_struct->rules), &shape, &c_x, &c_y, &c_z);
      ret =
	scm_list_3 (scm_cons
		    (scm_from_locale_string ("x"), scm_from_double (c_x)),
		    scm_cons (scm_from_locale_string ("y"), scm_from_double (c_y)), scm_cons (scm_from_locale_string ("z"), scm_from_double (c_z)));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_execute_command (SCM game_state, SCM command_text, SCM seq_0)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_state_t *c_game_state = NULL;
  char *c_command_text = NULL;
  int64_t c_seq_0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (command_text), command_text, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG3, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (sys_context, game_state);
  if (c_game_state)
    {
      c_command_text = lw6scm_utils_to_0str (sys_context, command_text);
      c_seq_0 = scm_to_long_long (seq_0);
      if (c_command_text)
	{
	  ret = lw6pil_command_execute_text (sys_context, NULL, 0LL, c_game_state, c_command_text, c_seq_0) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (sys_context, c_command_text);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_local_cursors_set_main (SCM pilot, SCM cursor_id)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_cursor_id_str = NULL;
  u_int16_t c_cursor_id_int;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (sys_context, cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (sys_context, c_cursor_id_str);
	  ret = lw6pil_local_cursors_set_main (sys_context, lw6pil_pilot_get_local_cursors (sys_context, c_pilot), c_cursor_id_int) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (sys_context, c_cursor_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_local_cursors_set_mouse_controlled (SCM pilot, SCM cursor_id, SCM mouse_controlled)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_cursor_id_str = NULL;
  u_int16_t c_cursor_id_int;
  int c_mouse_controlled;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (mouse_controlled), mouse_controlled, SCM_ARG3, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (sys_context, cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (sys_context, c_cursor_id_str);
	  c_mouse_controlled = SCM_NFALSEP (mouse_controlled);
	  ret =
	    lw6pil_local_cursors_set_mouse_controlled
	    (sys_context, lw6pil_pilot_get_local_cursors (sys_context, c_pilot), c_cursor_id_int, c_mouse_controlled) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (sys_context, c_cursor_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_make_backup (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      ret = lw6pil_pilot_make_backup (sys_context, c_pilot) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_sync_from_backup (SCM target, SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_state_t *c_target = NULL;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, target), target, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG2, __FUNCTION__);

  c_target = lw6_scm_to_game_state (sys_context, target);
  if (c_target)
    {
      c_pilot = lw6_scm_to_pilot (sys_context, pilot);
      if (c_pilot)
	{
	  ret = lw6pil_pilot_sync_from_backup (sys_context, c_target, c_pilot) ? SCM_BOOL_T : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_sync_from_reference (SCM target, SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_state_t *c_target = NULL;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, target), target, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG2, __FUNCTION__);

  c_target = lw6_scm_to_game_state (sys_context, target);
  if (c_target)
    {
      c_pilot = lw6_scm_to_pilot (sys_context, pilot);
      if (c_pilot)
	{
	  ret = lw6pil_pilot_sync_from_reference (sys_context, c_target, c_pilot) ? SCM_BOOL_T : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_sync_from_draft (SCM target, SCM pilot, SCM dirty_read)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_state_t *c_target = NULL;
  lw6pil_pilot_t *c_pilot = NULL;
  int c_dirty_read = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, target), target, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (dirty_read), dirty_read, SCM_ARG3, __FUNCTION__);

  c_target = lw6_scm_to_game_state (sys_context, target);
  if (c_target)
    {
      c_pilot = lw6_scm_to_pilot (sys_context, pilot);
      if (c_pilot)
	{
	  c_dirty_read = SCM_NFALSEP (dirty_read);
	  ret = lw6pil_pilot_sync_from_draft (sys_context, c_target, c_pilot, c_dirty_read) ? SCM_BOOL_T : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_calibrate (SCM pilot, SCM timestamp, SCM round)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int64_t c_timestamp = 0LL;
  int c_round = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (round), round, SCM_ARG3, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_timestamp = scm_to_long_long (timestamp);
      c_round = scm_to_int (round);

      lw6pil_pilot_calibrate (sys_context, c_pilot, c_timestamp, c_round);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6pil_speed_up (SCM pilot, SCM round_inc)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int c_round_inc;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (round_inc), round_inc, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_round_inc = scm_to_int (round_inc);

      lw6pil_pilot_speed_up (sys_context, c_pilot, c_round_inc);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6pil_slow_down (SCM pilot, SCM round_dec)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int c_round_dec;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (round_dec), round_dec, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_round_dec = scm_to_int (round_dec);

      lw6pil_pilot_slow_down (sys_context, c_pilot, c_round_dec);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6pil_get_round_0 (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      ret = scm_from_int (lw6pil_pilot_get_round_0 (sys_context, c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_seq_0 (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      ret = scm_from_long_long (lw6pil_pilot_get_seq_0 (sys_context, c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_seq2round (SCM pilot, SCM seq)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int64_t c_seq = LW6DAT_SEQ_INVALID;;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq), seq, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_seq = scm_to_long_long (seq);

      ret = scm_from_int (lw6pil_pilot_seq2round (sys_context, c_pilot, c_seq));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_round2seq (SCM pilot, SCM round)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int64_t c_round;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (round), round, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_round = scm_to_int (round);

      ret = scm_from_long_long (lw6pil_pilot_round2seq (sys_context, c_pilot, c_round));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_next_seq (SCM pilot, SCM timestamp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int64_t c_timestamp;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_timestamp = scm_to_long_long (timestamp);

      ret = scm_from_long_long (lw6pil_pilot_get_next_seq (sys_context, c_pilot, c_timestamp));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_last_commit_seq (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      ret = scm_from_long_long (lw6pil_pilot_get_last_commit_seq (sys_context, c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_reference_current_seq (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      ret = scm_from_long_long (lw6pil_pilot_get_reference_current_seq (sys_context, c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_reference_target_seq (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      ret = scm_from_long_long (lw6pil_pilot_get_reference_target_seq (sys_context, c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_max_seq (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      ret = scm_from_long_long (lw6pil_pilot_get_max_seq (sys_context, c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_is_over (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      if (lw6pil_pilot_is_over (sys_context, c_pilot))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_did_cursor_win (SCM pilot, SCM cursor_id)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_cursor_id_str = NULL;
  u_int16_t c_cursor_id_int = 0;
  lw6pil_pilot_t *c_pilot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (sys_context, cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (sys_context, c_cursor_id_str);
	  LW6SYS_FREE (sys_context, c_cursor_id_str);
	}

      if (lw6sys_check_id (sys_context, c_cursor_id_int))
	{
	  ret = lw6pil_pilot_did_cursor_win (sys_context, c_pilot, c_cursor_id_int) ? SCM_BOOL_T : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_winner (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int c_ret = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_ret = lw6pil_pilot_get_winner (sys_context, c_pilot);
      if (lw6map_team_color_is_valid (c_ret))
	{
	  ret = scm_from_int (c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_looser (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = NULL;
  int c_ret = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  if (c_pilot)
    {
      c_ret = lw6pil_pilot_get_looser (sys_context, c_pilot);
      if (lw6map_team_color_is_valid (c_ret))
	{
	  ret = scm_from_int (c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_seq_random_0 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_long_long (lw6pil_seq_random_0 (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_suite_init (SCM timestamp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_dump_t c_dump;
  int64_t c_timestamp = 0LL;;
  SCM ret_level;
  SCM ret_game_struct;
  SCM ret_game_state;
  SCM ret_pilot;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG1, __FUNCTION__);

  c_timestamp = scm_to_long_long (timestamp);

  if (lw6pil_suite_init (sys_context, &c_dump, c_timestamp))
    {
      ret_level = lw6_make_scm_map (sys_context, c_dump.level);
      ret_game_struct = lw6_make_scm_game_struct (sys_context, c_dump.game_struct, ret_level);
      ret_game_state = lw6_make_scm_game_state (sys_context, c_dump.game_state, ret_game_struct);
      ret_pilot = lw6_make_scm_pilot (sys_context, c_dump.pilot);
      ret = scm_list_4 (scm_cons
			(scm_from_locale_string ("level"), ret_level),
			scm_cons
			(scm_from_locale_string ("game-struct"),
			 ret_game_struct),
			scm_cons (scm_from_locale_string ("game-state"), ret_game_state), scm_cons (scm_from_locale_string ("pilot"), ret_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_suite_get_seq_0 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int64_t c_ret = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_ret = lw6pil_suite_get_seq_0 (sys_context);
  ret = scm_from_long_long (c_ret);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_suite_get_node_id (SCM node_index)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_node_index = 0;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (node_index), node_index, SCM_ARG1, __FUNCTION__);

  c_node_index = scm_to_int (node_index);

  c_ret = lw6sys_id_ltoa (sys_context, lw6pil_suite_get_node_id (sys_context, c_node_index));
  if (c_ret)
    {
      ret = scm_from_locale_string (c_ret);
      LW6SYS_FREE (sys_context, c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_suite_get_commands_by_node_index (SCM node_index, SCM stage)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_node_index = 0;
  int c_stage = 0;
  int c_step = 0;
  const char *c_command = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (node_index), node_index, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (stage), stage, SCM_ARG2, __FUNCTION__);

  c_node_index = scm_to_int (node_index);
  c_stage = scm_to_int (stage);

  ret = SCM_LIST0;
  while ((c_command = lw6pil_suite_get_command_by_node_index (sys_context, c_node_index, c_stage, c_step)) != NULL)
    {
      ret = scm_cons (scm_from_locale_string (c_command), ret);
      ++c_step;
    }
  ret = scm_reverse (ret);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_suite_get_commands_by_stage (SCM stage)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_stage = 0;
  int c_step = 0;
  const char *c_command = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (stage), stage, SCM_ARG1, __FUNCTION__);

  c_stage = scm_to_int (stage);

  ret = SCM_LIST0;
  while ((c_command = lw6pil_suite_get_command_by_stage (sys_context, c_stage, c_step)) != NULL)
    {
      ret = scm_cons (scm_from_locale_string (c_command), ret);
      ++c_step;
    }
  ret = scm_reverse (ret);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_suite_get_checkpoint (SCM stage)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_stage = 0;
  u_int32_t c_game_state_checksum = 0;
  int64_t c_seq = 0LL;
  int c_round = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (stage), stage, SCM_ARG1, __FUNCTION__);

  c_stage = scm_to_int (stage);

  lw6pil_suite_get_checkpoint (sys_context, &c_game_state_checksum, &c_seq, &c_round, c_stage);

  ret =
    scm_list_3 (scm_cons
		(scm_from_locale_string ("game-state-checksum"),
		 scm_from_int (c_game_state_checksum)),
		scm_cons (scm_from_locale_string ("seq"), scm_from_long_long (c_seq)), scm_cons (scm_from_locale_string ("round"), scm_from_int (c_round)));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_pil
 *
 * @sys_context: global system context
 *
 * Register the functions of the pil module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_pil (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6pil
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_BENCH, 0, 0, 0, (SCM (*)())_scm_lw6pil_bench);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SEED_COMMAND_GENERATE, 3, 0, 0, (SCM (*)())_scm_lw6pil_seed_command_generate);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_DUMP_COMMAND_GENERATE, 3, 0, 0, (SCM (*)())_scm_lw6pil_dump_command_generate);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_POLL_DUMP, 2, 0, 0, (SCM (*)())_scm_lw6pil_poll_dump);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_BUILD_PILOT, 3, 0, 0, (SCM (*)())_scm_lw6pil_build_pilot);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SEND_COMMAND, 3, 0, 0, (SCM (*)())_scm_lw6pil_send_command);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_LOCAL_COMMAND, 2, 0, 0, (SCM (*)())_scm_lw6pil_local_command);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_COMMIT, 1, 0, 0, (SCM (*)())_scm_lw6pil_commit);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_FIX_COORDS, 4, 0, 0, (SCM (*)())_scm_lw6pil_fix_coords);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_FIX_COORDS_X10, 4, 0, 0, (SCM (*)())_scm_lw6pil_fix_coords_x10);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_EXECUTE_COMMAND, 3, 0, 0, (SCM (*)())_scm_lw6pil_execute_command);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_LOCAL_CURSORS_SET_MAIN, 2, 0, 0, (SCM (*)())_scm_lw6pil_local_cursors_set_main);
  ret = ret
    && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_LOCAL_CURSORS_SET_MOUSE_CONTROLLED, 3, 0, 0,
			      (SCM (*)())_scm_lw6pil_local_cursors_set_mouse_controlled);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_MAKE_BACKUP, 1, 0, 0, (SCM (*)())_scm_lw6pil_make_backup);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SYNC_FROM_BACKUP, 2, 0, 0, (SCM (*)())_scm_lw6pil_sync_from_backup);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SYNC_FROM_REFERENCE, 2, 0, 0, (SCM (*)())_scm_lw6pil_sync_from_reference);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SYNC_FROM_DRAFT, 3, 0, 0, (SCM (*)())_scm_lw6pil_sync_from_draft);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_CALIBRATE, 3, 0, 0, (SCM (*)())_scm_lw6pil_calibrate);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SPEED_UP, 2, 0, 0, (SCM (*)())_scm_lw6pil_speed_up);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SLOW_DOWN, 2, 0, 0, (SCM (*)())_scm_lw6pil_slow_down);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_ROUND_0, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_round_0);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_SEQ_0, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_seq_0);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SEQ2ROUND, 2, 0, 0, (SCM (*)())_scm_lw6pil_seq2round);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_ROUND2SEQ, 2, 0, 0, (SCM (*)())_scm_lw6pil_round2seq);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_NEXT_SEQ, 2, 0, 0, (SCM (*)())_scm_lw6pil_get_next_seq);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_LAST_COMMIT_SEQ, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_last_commit_seq);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_REFERENCE_CURRENT_SEQ, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_reference_current_seq);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_REFERENCE_TARGET_SEQ, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_reference_target_seq);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_MAX_SEQ, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_max_seq);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_IS_OVER, 1, 0, 0, (SCM (*)())_scm_lw6pil_is_over);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_DID_CURSOR_WIN, 2, 0, 0, (SCM (*)())_scm_lw6pil_did_cursor_win);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_WINNER, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_winner);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_GET_LOOSER, 1, 0, 0, (SCM (*)())_scm_lw6pil_get_looser);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SEQ_RANDOM_0, 0, 0, 0, (SCM (*)())_scm_lw6pil_seq_random_0);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SUITE_INIT, 1, 0, 0, (SCM (*)())_scm_lw6pil_suite_init);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SUITE_GET_SEQ_0, 0, 0, 0, (SCM (*)())_scm_lw6pil_suite_get_seq_0);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SUITE_GET_NODE_ID, 1, 0, 0, (SCM (*)())_scm_lw6pil_suite_get_node_id);
  ret = ret
    && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SUITE_GET_COMMANDS_BY_NODE_INDEX, 2, 0, 0, (SCM (*)())_scm_lw6pil_suite_get_commands_by_node_index);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SUITE_GET_COMMANDS_BY_STAGE, 1, 0, 0, (SCM (*)())_scm_lw6pil_suite_get_commands_by_stage);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6PIL_SUITE_GET_CHECKPOINT, 1, 0, 0, (SCM (*)())_scm_lw6pil_suite_get_checkpoint);

  return ret;
}
