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

static int
_prepare_update_param_bootstrap (lw6sys_context_t * sys_context, lw6dsp_param_t * c_param, SCM param)
{
  int ret = 0;
  SCM value;

  lw6dsp_param_zero (sys_context, c_param);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_SPLASH), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_SPLASH);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_SPLASH;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_BACKGROUND), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_BACKGROUND);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_BACKGROUND;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_PREVIEW), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_PREVIEW);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_PREVIEW;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_MAP), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_MAP);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_MAP;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_FIGHTERS), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_FIGHTERS);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_FIGHTERS;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_CURSORS), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_CURSORS);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_CURSORS;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_HUD), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_HUD);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_HUD;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_SCORE), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_SCORE);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_SCORE;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_MENU), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_MENU);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_MENU;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_META), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_META);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_META;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_PROGRESS), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_PROGRESS);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_PROGRESS;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_LOG), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_LOG);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_LOG;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_FPS), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_FPS);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_FPS;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_MPS), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_MPS);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_MPS;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_URL), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_URL);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_URL;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_MOUSE), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_MOUSE);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_MOUSE;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_DEBUG_ZONES), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_DEBUG_ZONES);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_DEBUG_ZONES;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DISPLAY_DEBUG_GRADIENT), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_DISPLAY_DEBUG_GRADIENT);
  if (SCM_NFALSEP (value))
    {
      c_param->misc.mask |= LW6GUI_DISPLAY_DEBUG_GRADIENT;
    }

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_TARGET_FPS), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_TARGET_FPS);
  c_param->misc.target_fps = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_GFX_CPU_USAGE), SCM_BOOL_F);
  SCM_ASSERT (scm_is_number (value), value, SCM_ARGn, LW6DEF_GFX_CPU_USAGE);
  c_param->misc.gfx_cpu_usage = scm_to_double (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DIRTY_READ), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_DIRTY_READ);
  c_param->misc.dirty_read = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_CAPTURE), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_CAPTURE);
  c_param->misc.capture = SCM_NFALSEP (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_GFX_DEBUG), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_GFX_DEBUG);
  c_param->misc.gfx_debug = SCM_NFALSEP (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DEBUG_TEAM_ID), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_DEBUG_TEAM_ID);
  c_param->misc.debug_team_id = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DEBUG_LAYER_ID), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_DEBUG_LAYER_ID);
  c_param->misc.debug_layer_id = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_REPEAT_DELAY), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_REPEAT_DELAY);
  c_param->misc.repeat_settings.delay = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_REPEAT_INTERVAL), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_REPEAT_INTERVAL);
  c_param->misc.repeat_settings.interval = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_DOUBLE_CLICK_DELAY), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_DOUBLE_CLICK_DELAY);
  c_param->misc.repeat_settings.double_click_delay = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_AUTO_RELEASE_DELAY), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_AUTO_RELEASE_DELAY);
  c_param->misc.repeat_settings.auto_release_delay = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_LOG_TIMEOUT), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_LOG_TIMEOUT);
  c_param->misc.log_timeout = scm_to_int (value);

  /*
   * Progress taken from C globals
   */
  c_param->misc.progress = &(lw6_global.progress);

  /*
   * Video mode
   */
  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_WIDTH), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_WIDTH);
  c_param->video_mode.width = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_HEIGHT), SCM_BOOL_F);
  SCM_ASSERT (scm_is_integer (value), value, SCM_ARGn, LW6DEF_HEIGHT);
  c_param->video_mode.height = scm_to_int (value);

  value = scm_hash_ref (param, scm_from_locale_string (LW6DEF_FULLSCREEN), SCM_BOOL_F);
  SCM_ASSERT (SCM_BOOLP (value), value, SCM_ARGn, LW6DEF_FULLSCREEN);
  c_param->video_mode.fullscreen = SCM_NFALSEP (value);

  /*
   * Pointers on complex data types
   */
  value = scm_hash_ref (param, scm_from_locale_string ("look"), SCM_BOOL_F);
  if (SCM_SMOB_PREDICATE (lw6_global.smob_types.look, value))
    {
      c_param->look = lw6_scm_to_look (sys_context, value);
    }

  value = scm_hash_ref (param, scm_from_locale_string ("menu"), SCM_BOOL_F);
  if (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, value))
    {
      c_param->menu = lw6_scm_to_menu (sys_context, value);
    }

  ret = 1;

  return ret;
}

static int
_prepare_update_param (lw6sys_context_t * sys_context, SCM dsp, lw6dsp_param_t * c_param, SCM param)
{
  int ret = 0;
  SCM value;
  lw6_dsp_smob_t *dsp_smob = NULL;

  dsp_smob = (lw6_dsp_smob_t *) SCM_SMOB_DATA (dsp);
  if (dsp_smob)
    {
      _prepare_update_param_bootstrap (sys_context, c_param, param);

      value = scm_hash_ref (param, scm_from_locale_string ("level"), SCM_BOOL_F);
      if (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, value))
	{
	  /*
	   * It's really (really) important to do this so that level
	   * is marked when Guile garbage collects
	   */
	  dsp_smob->level = value;
	  c_param->level = lw6_scm_to_map (sys_context, value);
	}
      else
	{
	  dsp_smob->level = SCM_BOOL_F;
	  c_param->level = NULL;
	}

      value = scm_hash_ref (param, scm_from_locale_string ("game-struct"), SCM_BOOL_F);
      if (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_struct, value))
	{
	  /*
	   * It's really (really) important to do this so that game_struct
	   * is marked when Guile garbage collects
	   */
	  dsp_smob->game_struct = value;
	  c_param->game_struct = lw6_scm_to_game_struct (sys_context, value);
	}
      else
	{
	  dsp_smob->game_struct = SCM_BOOL_F;
	  c_param->game_struct = NULL;
	}

      value = scm_hash_ref (param, scm_from_locale_string ("game-state"), SCM_BOOL_F);
      if (SCM_SMOB_PREDICATE (lw6_global.smob_types.game_state, value))
	{
	  /*
	   * It's really (really) important to do this so that game_state
	   * is marked when Guile garbage collects
	   */
	  dsp_smob->game_state = value;
	  c_param->game_state = lw6_scm_to_game_state (sys_context, value);
	}
      else
	{
	  dsp_smob->game_state = SCM_BOOL_F;
	  c_param->game_state = NULL;
	}

      value = scm_hash_ref (param, scm_from_locale_string ("pilot"), SCM_BOOL_F);
      if (SCM_SMOB_PREDICATE (lw6_global.smob_types.pilot, value))
	{
	  /*
	   * It's really (really) important to do this so that game_state
	   * is marked when Guile garbage collects
	   */
	  dsp_smob->pilot = value;
	  c_param->pilot = lw6_scm_to_pilot (sys_context, value);
	}
      else
	{
	  dsp_smob->pilot = SCM_BOOL_F;
	  c_param->pilot = NULL;
	}

      ret = 1;
    }

  return ret;
}

/*
 * In liquidwar6dsp
 */
static SCM
_scm_lw6dsp_new (SCM backend_name, SCM param)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_backend_name = NULL;
  lw6dsp_param_t c_param;
  lw6dsp_backend_t *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (backend_name), backend_name, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_hash_table_p (param), param, SCM_ARG2, __FUNCTION__);

  c_backend_name = lw6scm_utils_to_0str (sys_context, backend_name);
  if (backend_name)
    {
      if (_prepare_update_param_bootstrap (sys_context, &c_param, param))
	{
	  c_ret = lw6dsp_create_backend (sys_context, lw6_global.argc, lw6_global.argv, c_backend_name);
	  if (c_ret)
	    {
	      if (lw6dsp_init (sys_context, c_ret, &c_param, lw6_resize_callback))
		{
		  ret = lw6_make_scm_dsp (sys_context, c_ret);
		}
	      else
		{
		  lw6dsp_destroy_backend (sys_context, c_ret);
		}
	    }
	}
      LW6SYS_FREE (sys_context, c_backend_name);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_release (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      lw6dsp_quit (sys_context, c_dsp);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_update (SCM dsp, SCM param)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;
  lw6dsp_param_t c_param;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_hash_table_p (param), param, SCM_ARG2, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (_prepare_update_param (sys_context, dsp, &c_param, param))
	{
	  ret = lw6dsp_update (sys_context, c_dsp, &c_param) ? SCM_BOOL_T : SCM_BOOL_F;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_get_nb_frames (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      ret = scm_from_int (lw6dsp_get_nb_frames (sys_context, c_dsp));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_get_last_frame_rendering_time (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      ret = scm_from_int (lw6dsp_get_last_frame_rendering_time (sys_context, c_dsp));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_get_instant_fps (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      ret = scm_from_int (lw6dsp_get_instant_fps (sys_context, c_dsp));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_get_average_fps (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      ret = scm_from_int (lw6dsp_get_average_fps (sys_context, c_dsp));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_get_video_mode (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      lw6gui_video_mode_t video_mode;

      if (lw6dsp_get_video_mode (sys_context, c_dsp, &video_mode))
	{
	  ret =
	    scm_list_3 (scm_cons
			(scm_from_locale_string ("width"),
			 scm_from_int (video_mode.width)),
			scm_cons (scm_from_locale_string ("height"),
				  scm_from_int (video_mode.height)),
			scm_cons (scm_from_locale_string ("fullscreen"), video_mode.fullscreen ? SCM_BOOL_T : SCM_BOOL_F));
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6dsp_get_fullscreen_modes (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      lw6gui_fullscreen_modes_t fullscreen_modes;

      if (lw6dsp_get_fullscreen_modes (sys_context, c_dsp, &fullscreen_modes))
	{
	  ret = scm_list_3 (scm_cons (scm_from_locale_string ("low"),
				      scm_list_3 (scm_cons
						  (scm_from_locale_string
						   ("width"),
						   scm_from_int
						   (fullscreen_modes.low.width)),
						  scm_cons
						  (scm_from_locale_string
						   ("height"),
						   scm_from_int
						   (fullscreen_modes.low.height)),
						  scm_cons
						  (scm_from_locale_string
						   ("fullscreen"),
						   fullscreen_modes.low.fullscreen ? SCM_BOOL_T :
						   SCM_BOOL_F))),
			    scm_cons (scm_from_locale_string ("standard"),
				      scm_list_3 (scm_cons
						  (scm_from_locale_string
						   ("width"),
						   scm_from_int
						   (fullscreen_modes.standard.width)),
						  scm_cons
						  (scm_from_locale_string
						   ("height"),
						   scm_from_int
						   (fullscreen_modes.standard.height)),
						  scm_cons
						  (scm_from_locale_string
						   ("fullscreen"),
						   fullscreen_modes.standard.fullscreen ? SCM_BOOL_T :
						   SCM_BOOL_F))),
			    scm_cons (scm_from_locale_string ("high"),
				      scm_list_3 (scm_cons
						  (scm_from_locale_string
						   ("width"),
						   scm_from_int
						   (fullscreen_modes.high.width)),
						  scm_cons
						  (scm_from_locale_string
						   ("height"),
						   scm_from_int
						   (fullscreen_modes.high.height)),
						  scm_cons
						  (scm_from_locale_string ("fullscreen"), fullscreen_modes.high.fullscreen ? SCM_BOOL_T : SCM_BOOL_F))));

	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_dsp
 *
 * @sys_context: global system context
 *
 * Register the functions of the dsp module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_dsp (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6dsp
   */
  /*
   * In display.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_NEW, 2, 0, 0, (SCM (*)())_scm_lw6dsp_new);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_RELEASE, 1, 0, 0, (SCM (*)())_scm_lw6dsp_release);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_UPDATE, 2, 0, 0, (SCM (*)())_scm_lw6dsp_update);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_GET_NB_FRAMES, 1, 0, 0, (SCM (*)())_scm_lw6dsp_get_nb_frames);
  ret = ret
    && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_GET_LAST_FRAME_RENDERING_TIME, 1, 0, 0, (SCM (*)())_scm_lw6dsp_get_last_frame_rendering_time);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_GET_INSTANT_FPS, 1, 0, 0, (SCM (*)())_scm_lw6dsp_get_instant_fps);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_GET_AVERAGE_FPS, 1, 0, 0, (SCM (*)())_scm_lw6dsp_get_average_fps);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_GET_VIDEO_MODE, 1, 0, 0, (SCM (*)())_scm_lw6dsp_get_video_mode);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6DSP_GET_FULLSCREEN_MODES, 1, 0, 0, (SCM (*)())_scm_lw6dsp_get_fullscreen_modes);

  return ret;
}
