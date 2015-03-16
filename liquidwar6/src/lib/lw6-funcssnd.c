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
 * In liquidwar6snd
 */

/*
 * In backend.c
 */
static SCM
_scm_lw6snd_get_backends ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_assoc_t *backends = NULL;
  lw6sys_list_t *keys = NULL;
  lw6sys_list_t *key = NULL;
  const char *module_id = NULL;
  const char *module_name = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  backends = lw6snd_get_backends (sys_context, lw6_global.argc, lw6_global.argv);
  if (backends)
    {
      keys = lw6sys_assoc_keys (sys_context, backends);
      if (keys)
	{
	  ret = SCM_EOL;
	  key = keys;
	  while (key)
	    {
	      if (key->data)
		{
		  module_id = (const char *) key->data;
		  module_name = (const char *) lw6sys_assoc_get (sys_context, backends, module_id);
		  ret = scm_cons (scm_cons (scm_from_locale_string (module_id), scm_from_locale_string (module_name)), ret);
		}
	      key = lw6sys_list_next (sys_context, key);
	    }
	  lw6sys_list_free (sys_context, keys);
	  ret = scm_reverse (ret);
	}
      lw6sys_assoc_free (sys_context, backends);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_new (SCM backend_name, SCM fx_volume, SCM water_volume, SCM music_volume)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_backend_name = NULL;
  float c_fx_volume = 0.0f;
  float c_water_volume = 0.0f;
  float c_music_volume = 0.0f;
  lw6snd_backend_t *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (backend_name), backend_name, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_REALP (fx_volume), fx_volume, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_REALP (music_volume), music_volume, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (SCM_REALP (water_volume), water_volume, SCM_ARG4, __FUNCTION__);

  c_backend_name = lw6scm_utils_to_0str (sys_context, backend_name);
  if (backend_name)
    {
      c_fx_volume = scm_to_double (fx_volume);
      c_water_volume = scm_to_double (water_volume);
      c_music_volume = scm_to_double (music_volume);

      c_ret = lw6snd_create_backend (sys_context, lw6_global.argc, lw6_global.argv, c_backend_name);
      if (c_ret)
	{
	  if (lw6snd_init (sys_context, c_ret, c_fx_volume, c_water_volume, c_music_volume))
	    {
	      ret = lw6_make_scm_snd (sys_context, c_ret);
	    }
	  else
	    {
	      lw6snd_destroy_backend (sys_context, c_ret);
	    }
	}
      LW6SYS_FREE (sys_context, c_backend_name);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_poll (SCM snd)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      lw6snd_poll (sys_context, c_snd);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_release (SCM snd)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      lw6snd_quit (sys_context, c_snd);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_play_fx (SCM snd, SCM fx_id)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;
  int c_fx_id = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (fx_id), fx_id, SCM_ARG2, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      c_fx_id = scm_to_int (fx_id);

      ret = scm_from_int (lw6snd_play_fx (sys_context, c_snd, c_fx_id));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_set_fx_volume (SCM snd, SCM fx_volume)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;
  float c_fx_volume = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_REALP (fx_volume), fx_volume, SCM_ARG2, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      c_fx_volume = scm_to_double (fx_volume);
      lw6snd_set_fx_volume (sys_context, c_snd, c_fx_volume);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6snd_set_water_volume (SCM snd, SCM water_volume)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;
  float c_water_volume = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_REALP (water_volume), water_volume, SCM_ARG2, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      c_water_volume = scm_to_double (water_volume);
      lw6snd_set_water_volume (sys_context, c_snd, c_water_volume);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6snd_is_music_file (SCM snd, SCM map_dir, SCM music_path, SCM music_file)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;
  char *c_map_dir = NULL;
  char *c_music_path = NULL;
  char *c_music_file = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (map_dir), map_dir, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (music_path), music_path, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_string (music_file), music_file, SCM_ARG4, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      c_map_dir = lw6scm_utils_to_0str (sys_context, map_dir);
      if (c_map_dir)
	{
	  c_music_path = lw6scm_utils_to_0str (sys_context, music_path);
	  if (c_music_path)
	    {
	      c_music_file = lw6scm_utils_to_0str (sys_context, music_file);
	      if (c_music_file)
		{
		  if (lw6snd_is_music_file (sys_context, c_snd, c_map_dir, c_music_path, c_music_file))
		    {
		      ret = SCM_BOOL_T;
		    }
		  LW6SYS_FREE (sys_context, c_music_file);
		}
	      LW6SYS_FREE (sys_context, c_music_path);
	    }
	  LW6SYS_FREE (sys_context, c_map_dir);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_play_music_file (SCM snd, SCM map_dir, SCM music_path, SCM music_file)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;
  char *c_map_dir = NULL;
  char *c_music_path = NULL;
  char *c_music_file = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (map_dir), map_dir, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (music_path), music_path, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_string (music_file), music_file, SCM_ARG4, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      c_map_dir = lw6scm_utils_to_0str (sys_context, map_dir);
      if (c_map_dir)
	{
	  c_music_path = lw6scm_utils_to_0str (sys_context, music_path);
	  if (c_music_path)
	    {
	      c_music_file = lw6scm_utils_to_0str (sys_context, music_file);
	      if (c_music_file)
		{
		  ret = scm_from_int (lw6snd_play_music_file (sys_context, c_snd, c_map_dir, c_music_path, c_music_file));
		  LW6SYS_FREE (sys_context, c_music_file);
		}
	      LW6SYS_FREE (sys_context, c_music_path);
	    }
	  LW6SYS_FREE (sys_context, c_map_dir);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_play_music_random (SCM snd, SCM music_path, SCM music_filter, SCM music_exclude)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;
  char *c_music_path = NULL;
  char *c_music_filter = NULL;
  char *c_music_exclude = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (music_path), music_path, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (music_filter), music_filter, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_string (music_exclude), music_exclude, SCM_ARG3, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      c_music_path = lw6scm_utils_to_0str (sys_context, music_path);
      if (c_music_path)
	{
	  c_music_filter = lw6scm_utils_to_0str (sys_context, music_filter);
	  if (c_music_filter)
	    {
	      c_music_exclude = lw6scm_utils_to_0str (sys_context, music_exclude);
	      if (c_music_exclude)
		{
		  ret = scm_from_int (lw6snd_play_music_random (sys_context, c_snd, c_music_path, c_music_filter, c_music_exclude));
		  LW6SYS_FREE (sys_context, c_music_exclude);
		}
	      LW6SYS_FREE (sys_context, c_music_filter);
	    }
	  LW6SYS_FREE (sys_context, c_music_path);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6snd_stop_music (SCM snd)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      lw6snd_stop_music (sys_context, c_snd);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6snd_set_music_volume (SCM snd, SCM music_volume)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = NULL;
  float c_music_volume = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.snd, snd), snd, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_REALP (music_volume), music_volume, SCM_ARG2, __FUNCTION__);

  c_snd = lw6_scm_to_snd (sys_context, snd);
  if (c_snd)
    {
      c_music_volume = scm_to_double (music_volume);
      lw6snd_set_music_volume (sys_context, c_snd, c_music_volume);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/**
 * lw6_register_funcs_snd
 *
 * @sys_context: global system context
 *
 * Register the functions of the snd module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_snd (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6snd
   */
  /*
   * In backend.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_GET_BACKENDS, 0, 0, 0, (SCM (*)())_scm_lw6snd_get_backends);
  /*
   * In setup.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_NEW, 4, 0, 0, (SCM (*)())_scm_lw6snd_new);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_POLL, 1, 0, 0, (SCM (*)())_scm_lw6snd_poll);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_RELEASE, 1, 0, 0, (SCM (*)())_scm_lw6snd_release);

  /*
   * In fx.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_PLAY_FX, 2, 0, 0, (SCM (*)())_scm_lw6snd_play_fx);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_SET_FX_VOLUME, 2, 0, 0, (SCM (*)())_scm_lw6snd_set_fx_volume);

  /*
   * In water.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_SET_WATER_VOLUME, 2, 0, 0, (SCM (*)())_scm_lw6snd_set_water_volume);

  /*
   * In music.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_IS_MUSIC_FILE, 4, 0, 0, (SCM (*)())_scm_lw6snd_is_music_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_PLAY_MUSIC_FILE, 4, 0, 0, (SCM (*)())_scm_lw6snd_play_music_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_PLAY_MUSIC_RANDOM, 4, 0, 0, (SCM (*)())_scm_lw6snd_play_music_random);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_STOP_MUSIC, 1, 0, 0, (SCM (*)())_scm_lw6snd_stop_music);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SND_SET_MUSIC_VOLUME, 2, 0, 0, (SCM (*)())_scm_lw6snd_set_music_volume);

  return ret;
}
