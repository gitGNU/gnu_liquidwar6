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
#endif

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
  lw6sys_progress_t progress;
  float c_ret = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  lw6sys_progress_default (&progress, &(lw6_global.progress));
  lw6sys_progress_begin (&progress);

  if (lw6pil_bench (&c_ret, &progress))
    {
      ret = scm_from_int (ceil (c_ret));
    }

  lw6sys_progress_begin (&progress);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_dump_command_generate (SCM pilot, SCM server_id)
{
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_server_id_str = NULL;
  u_int64_t c_server_id_int = 0LL;
  char *c_ret = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (server_id), server_id, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_server_id_str = lw6scm_utils_to_0str (server_id);
      if (c_server_id_str)
	{
	  c_server_id_int = lw6sys_id_atol (c_server_id_str);
	  if (c_server_id_int > 0)
	    {
	      c_ret = lw6pil_dump_command_generate (c_pilot, c_server_id_int);
	      if (c_ret)
		{
		  ret = scm_from_locale_string (c_ret);
		  LW6SYS_FREE (c_ret);
		}
	    }
	  LW6SYS_FREE (c_server_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}


static SCM
_scm_lw6pil_build_pilot (SCM game_state, SCM seq_0, SCM timestamp)
{
  lw6ker_game_state_t *c_game_state;
  int64_t c_seq_0;
  int64_t c_timestamp;
  lw6pil_pilot_t *c_ret;
  SCM ret = SCM_BOOL_F;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG3, __FUNCTION__);

  lw6sys_progress_default (&progress, &(lw6_global.progress));
  lw6sys_progress_begin (&progress);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_seq_0 = scm_to_long_long (seq_0);
      c_timestamp = scm_to_long_long (timestamp);
      c_ret =
	lw6pil_pilot_new (c_game_state, c_seq_0, c_timestamp, &progress);
      if (c_ret)
	{
	  ret = lw6_make_scm_pilot (c_ret);
	}
    }

  lw6sys_progress_begin (&progress);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_send_command (SCM pilot, SCM command_text, SCM verified)
{
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_command_text = NULL;
  int c_verified;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (command_text), command_text, SCM_ARG2,
	      __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (verified), verified, SCM_ARG3, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_command_text = lw6scm_utils_to_0str (command_text);
      if (c_command_text)
	{
	  c_verified = SCM_NFALSEP (verified);
	  ret =
	    lw6pil_pilot_send_command (c_pilot, c_command_text,
				       c_verified) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (c_command_text);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_local_command (SCM pilot, SCM command_text)
{
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_command_text = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (command_text), command_text, SCM_ARG2,
	      __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_command_text = lw6scm_utils_to_0str (command_text);
      if (c_command_text)
	{
	  ret =
	    lw6pil_pilot_local_command (c_pilot,
					c_command_text) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	  LW6SYS_FREE (c_command_text);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_commit (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;
  lw6pil_dump_t c_dump;
  SCM ret_level;
  SCM ret_game_struct;
  SCM ret_game_state;
  SCM ret_pilot;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      if (lw6pil_pilot_commit (&c_dump, c_pilot))
	{
	  if (lw6pil_dump_exists (&c_dump))
	    {
	      ret_level = lw6_make_scm_map (c_dump.level);
	      ret_game_struct =
		lw6_make_scm_game_struct (c_dump.game_struct, ret_level);
	      ret_game_state =
		lw6_make_scm_game_state (c_dump.game_state, ret_game_struct);
	      ret_pilot = lw6_make_scm_pilot (c_dump.pilot);
	      ret = scm_list_4 (scm_cons
				(scm_from_locale_string ("level"), ret_level),
				scm_cons
				(scm_from_locale_string ("game-struct"),
				 ret_game_struct),
				scm_cons (scm_from_locale_string
					  ("game-state"), ret_game_state),
				scm_cons (scm_from_locale_string ("pilot"),
					  ret_pilot));
	    }
	  else
	    {
	      ret = SCM_EOL;
	    }
	}
      else
	{
	  ret = SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_fix_coords (SCM game_state, SCM x, SCM y, SCM z)
{
  lw6ker_game_state_t *c_game_state = NULL;
  lw6sys_whd_t shape;
  float c_x = 0.0f;
  float c_y = 0.0f;
  float c_z = 0.0f;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_number (x), x, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (y), y, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (z), z, SCM_ARG3, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_x = scm_to_double (x);
      c_y = scm_to_double (y);
      c_z = scm_to_double (z);

      lw6ker_game_state_get_shape (c_game_state, &shape);
      lw6pil_coords_fix (&(c_game_state->game_struct->rules),
			 &shape, &c_x, &c_y, &c_z);
      ret =
	scm_list_3 (scm_cons
		    (scm_from_locale_string ("x"), scm_from_double (c_x)),
		    scm_cons (scm_from_locale_string ("y"),
			      scm_from_double (c_y)),
		    scm_cons (scm_from_locale_string ("z"),
			      scm_from_double (c_z)));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_fix_coords_x10 (SCM game_state, SCM x, SCM y, SCM z)
{
  lw6ker_game_state_t *c_game_state = NULL;
  lw6sys_whd_t shape;
  float c_x = 0.0f;
  float c_y = 0.0f;
  float c_z = 0.0f;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_number (x), x, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (y), y, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_number (z), z, SCM_ARG3, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_x = scm_to_double (x);
      c_y = scm_to_double (y);
      c_z = scm_to_double (z);

      lw6ker_game_state_get_shape (c_game_state, &shape);
      lw6pil_coords_fix_x10 (&(c_game_state->game_struct->rules),
			     &shape, &c_x, &c_y, &c_z);
      ret =
	scm_list_3 (scm_cons
		    (scm_from_locale_string ("x"), scm_from_double (c_x)),
		    scm_cons (scm_from_locale_string ("y"),
			      scm_from_double (c_y)),
		    scm_cons (scm_from_locale_string ("z"),
			      scm_from_double (c_z)));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_execute_command (SCM game_state, SCM command_text, SCM seq_0)
{
  lw6ker_game_state_t *c_game_state = NULL;
  char *c_command_text = NULL;
  int64_t c_seq_0;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       game_state), game_state, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (command_text), command_text, SCM_ARG2,
	      __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG3, __FUNCTION__);

  c_game_state = lw6_scm_to_game_state (game_state);
  if (c_game_state)
    {
      c_command_text = lw6scm_utils_to_0str (command_text);
      c_seq_0 = scm_to_long_long (seq_0);
      if (c_command_text)
	{
	  ret =
	    lw6pil_command_execute_text (NULL, 0LL, c_game_state,
					 c_command_text,
					 c_seq_0) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (c_command_text);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_local_cursors_set_main (SCM pilot, SCM cursor_id)
{
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_cursor_id_str = NULL;
  u_int16_t c_cursor_id_int;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
	  ret =
	    lw6pil_local_cursors_set_main (lw6pil_pilot_get_local_cursors
					   (c_pilot),
					   c_cursor_id_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	  LW6SYS_FREE (c_cursor_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_local_cursors_set_mouse_controlled (SCM pilot, SCM cursor_id,
						SCM mouse_controlled)
{
  lw6pil_pilot_t *c_pilot = NULL;
  char *c_cursor_id_str = NULL;
  u_int16_t c_cursor_id_int;
  int c_mouse_controlled;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (mouse_controlled), mouse_controlled, SCM_ARG3,
	      __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_cursor_id_str = lw6scm_utils_to_0str (cursor_id);
      if (c_cursor_id_str)
	{
	  c_cursor_id_int = lw6sys_id_atol (c_cursor_id_str);
	  c_mouse_controlled = SCM_NFALSEP (mouse_controlled);
	  ret =
	    lw6pil_local_cursors_set_mouse_controlled
	    (lw6pil_pilot_get_local_cursors (c_pilot), c_cursor_id_int,
	     c_mouse_controlled) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (c_cursor_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_make_backup (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      ret = lw6pil_pilot_make_backup (c_pilot) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_sync_from_backup (SCM target, SCM pilot)
{
  lw6ker_game_state_t *c_target = NULL;
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       target), target, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG2, __FUNCTION__);

  c_target = lw6_scm_to_game_state (target);
  if (c_target)
    {
      c_pilot = lw6_scm_to_pilot (pilot);
      if (c_pilot)
	{
	  ret =
	    lw6pil_pilot_sync_from_backup (c_target,
					   c_pilot) ? SCM_BOOL_T : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_sync_from_reference (SCM target, SCM pilot)
{
  lw6ker_game_state_t *c_target = NULL;
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       target), target, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG2, __FUNCTION__);

  c_target = lw6_scm_to_game_state (target);
  if (c_target)
    {
      c_pilot = lw6_scm_to_pilot (pilot);
      if (c_pilot)
	{
	  ret =
	    lw6pil_pilot_sync_from_reference (c_target,
					      c_pilot) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_sync_from_draft (SCM target, SCM pilot, SCM dirty_read)
{
  lw6ker_game_state_t *c_target = NULL;
  lw6pil_pilot_t *c_pilot = NULL;
  int c_dirty_read = 0;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.game_state,
	       target), target, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (dirty_read), dirty_read, SCM_ARG3, __FUNCTION__);

  c_target = lw6_scm_to_game_state (target);
  if (c_target)
    {
      c_pilot = lw6_scm_to_pilot (pilot);
      if (c_pilot)
	{
	  c_dirty_read = SCM_NFALSEP (dirty_read);
	  ret =
	    lw6pil_pilot_sync_from_draft (c_target,
					  c_pilot,
					  c_dirty_read) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_calibrate (SCM pilot, SCM timestamp, SCM round)
{
  lw6pil_pilot_t *c_pilot = NULL;
  int64_t c_timestamp;
  int c_round;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (round), round, SCM_ARG3, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_timestamp = scm_to_long_long (timestamp);
      c_round = scm_to_int (round);

      lw6pil_pilot_calibrate (c_pilot, c_timestamp, c_round);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6pil_speed_up (SCM pilot, SCM round_inc)
{
  lw6pil_pilot_t *c_pilot = NULL;
  int c_round_inc;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (round_inc), round_inc, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_round_inc = scm_to_int (round_inc);

      lw6pil_pilot_speed_up (c_pilot, c_round_inc);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6pil_slow_down (SCM pilot, SCM round_dec)
{
  lw6pil_pilot_t *c_pilot = NULL;
  int c_round_dec;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (round_dec), round_dec, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_round_dec = scm_to_int (round_dec);

      lw6pil_pilot_slow_down (c_pilot, c_round_dec);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6pil_get_next_seq (SCM pilot, SCM timestamp)
{
  lw6pil_pilot_t *c_pilot = NULL;
  int64_t c_timestamp;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (timestamp), timestamp, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_timestamp = scm_to_long_long (timestamp);

      ret =
	scm_from_long_long (lw6pil_pilot_get_next_seq (c_pilot, c_timestamp));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_last_commit_seq (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      ret = scm_from_long_long (lw6pil_pilot_get_last_commit_seq (c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_reference_current_seq (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      ret =
	scm_from_long_long (lw6pil_pilot_get_reference_current_seq (c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_reference_target_seq (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      ret =
	scm_from_long_long (lw6pil_pilot_get_reference_target_seq (c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_max_seq (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      ret = scm_from_long_long (lw6pil_pilot_get_max_seq (c_pilot));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_is_over (SCM pilot)
{
  lw6pil_pilot_t *c_pilot = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      if (lw6pil_pilot_is_over (c_pilot))
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
  char *c_cursor_id_str;
  u_int16_t c_cursor_id_int = 0;
  lw6pil_pilot_t *c_pilot;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (cursor_id), cursor_id, SCM_ARG2, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
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
	    lw6pil_pilot_did_cursor_win (c_pilot,
					 c_cursor_id_int) ? SCM_BOOL_T :
	    SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6pil_get_winner (SCM pilot)
{
  SCM ret = SCM_BOOL_F;
  lw6pil_pilot_t *c_pilot;
  int c_ret;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_ret = lw6pil_pilot_get_winner (c_pilot);
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
  lw6pil_pilot_t *c_pilot;
  int c_ret;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.pilot,
	       pilot), pilot, SCM_ARG1, __FUNCTION__);

  c_pilot = lw6_scm_to_pilot (pilot);
  if (c_pilot)
    {
      c_ret = lw6pil_pilot_get_looser (c_pilot);
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

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  ret = scm_from_long_long (lw6pil_seq_random_0 ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_pil
 *
 * Register the functions of the pil module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_pil ()
{
  int ret = 1;

  /*
   * In liquidwar6pil
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_BENCH,
				      0, 0, 0, (SCM (*)())_scm_lw6pil_bench);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_DUMP_COMMAND_GENERATE,
				      2, 0, 0,
				      (SCM (*)
				       ())_scm_lw6pil_dump_command_generate);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_BUILD_PILOT, 3, 0, 0,
			      (SCM (*)())_scm_lw6pil_build_pilot);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_SEND_COMMAND, 3, 0, 0,
			      (SCM (*)())_scm_lw6pil_send_command);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_LOCAL_COMMAND, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_local_command);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_COMMIT, 1, 0, 0,
			      (SCM (*)())_scm_lw6pil_commit);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_FIX_COORDS, 4, 0, 0,
			      (SCM (*)())_scm_lw6pil_fix_coords);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_FIX_COORDS_X10, 4, 0, 0,
			      (SCM (*)())_scm_lw6pil_fix_coords_x10);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_EXECUTE_COMMAND, 3, 0, 0,
			      (SCM (*)())_scm_lw6pil_execute_command);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_LOCAL_CURSORS_SET_MAIN, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_local_cursors_set_main);
  ret = ret
    &&
    lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_LOCAL_CURSORS_SET_MOUSE_CONTROLLED,
			   3, 0, 0,
			   (SCM (*)
			    ())_scm_lw6pil_local_cursors_set_mouse_controlled);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_MAKE_BACKUP, 1, 0, 0,
			      (SCM (*)())_scm_lw6pil_make_backup);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_SYNC_FROM_BACKUP, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_sync_from_backup);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_SYNC_FROM_REFERENCE, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_sync_from_reference);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_SYNC_FROM_DRAFT, 3, 0, 0,
			      (SCM (*)())_scm_lw6pil_sync_from_draft);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_CALIBRATE, 3, 0, 0,
			      (SCM (*)())_scm_lw6pil_calibrate);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_SPEED_UP, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_speed_up);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_SLOW_DOWN, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_slow_down);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_GET_NEXT_SEQ, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_get_next_seq);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_GET_LAST_COMMIT_SEQ, 1, 0, 0,
			      (SCM (*)())_scm_lw6pil_get_last_commit_seq);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_GET_REFERENCE_CURRENT_SEQ, 1, 0,
			      0,
			      (SCM (*)
			       ())_scm_lw6pil_get_reference_current_seq);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_GET_REFERENCE_TARGET_SEQ, 1, 0,
			      0,
			      (SCM (*)
			       ())_scm_lw6pil_get_reference_target_seq);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_GET_MAX_SEQ, 1, 0, 0,
			      (SCM (*)())_scm_lw6pil_get_max_seq);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_IS_OVER, 1, 0, 0,
			      (SCM (*)())_scm_lw6pil_is_over);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_DID_CURSOR_WIN, 2, 0, 0,
			      (SCM (*)())_scm_lw6pil_did_cursor_win);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_GET_WINNER, 1, 0, 0,
			      (SCM (*)())_scm_lw6pil_get_winner);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_GET_LOOSER, 1, 0, 0,
			      (SCM (*)())_scm_lw6pil_get_looser);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6PIL_SEQ_RANDOM_0, 0, 0, 0,
			      (SCM (*)())_scm_lw6pil_seq_random_0);

  return ret;
}
