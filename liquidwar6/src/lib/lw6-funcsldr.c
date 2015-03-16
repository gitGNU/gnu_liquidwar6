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
 * In liquidwar6ldr
 */
static SCM
_scm_lw6ldr_get_entries (SCM map_path, SCM relative_path)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6sys_list_t *c_maps = NULL;
  lw6ldr_entry_t *c_entry = NULL;
  char *c_map_path = NULL;
  char *c_relative_path = NULL;
  char *user_dir = NULL;
  SCM item = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (map_path), relative_path, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (relative_path), relative_path, SCM_ARG2, __FUNCTION__);

  ret = SCM_LIST0;
  c_map_path = lw6scm_utils_to_0str (sys_context, map_path);
  if (c_map_path)
    {
      c_relative_path = lw6scm_utils_to_0str (sys_context, relative_path);
      if (c_relative_path)
	{
	  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
	  if (user_dir)
	    {
	      c_maps = lw6ldr_get_entries (sys_context, c_map_path, c_relative_path, user_dir);
	      if (c_maps)
		{
		  while (!lw6sys_list_is_empty (sys_context, c_maps))
		    {
		      c_entry = (lw6ldr_entry_t *) lw6sys_lifo_pop (sys_context, &c_maps);
		      if (c_entry)
			{
			  item = SCM_EOL;
			  item = scm_cons (scm_cons (scm_from_locale_string ("title"), scm_from_locale_string (c_entry->metadata.title)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("author"), scm_from_locale_string (c_entry->metadata.author)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("description"), scm_from_locale_string (c_entry->metadata.description)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("license"), scm_from_locale_string (c_entry->metadata.license)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("absolute-path"), scm_from_locale_string (c_entry->absolute_path)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("relative-path"), scm_from_locale_string (c_entry->relative_path)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("has-subdirs"), c_entry->has_subdirs ? SCM_BOOL_T : SCM_BOOL_F), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("nb-submaps"), scm_from_int (c_entry->nb_submaps)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("exp"), scm_from_int (c_entry->metadata.vanilla_exp)), item);
			  item = scm_cons (scm_cons (scm_from_locale_string ("forbidden"), c_entry->forbidden ? SCM_BOOL_T : SCM_BOOL_F), item);
			  ret = scm_cons (item, ret);
			  lw6ldr_free_entry (sys_context, c_entry);
			}
		    }
		  lw6sys_list_free (sys_context, c_maps);
		}
	      LW6SYS_FREE (sys_context, user_dir);
	    }
	  LW6SYS_FREE (sys_context, c_relative_path);
	}
      LW6SYS_FREE (sys_context, c_map_path);
    }

  ret = scm_reverse (ret);
  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ldr_read (SCM dirname, SCM default_param, SCM forced_param, SCM display_width, SCM display_height, SCM bench_value, SCM magic_number)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_dirname = NULL;
  lw6sys_assoc_t *c_default_param = NULL;
  lw6sys_assoc_t *c_forced_param = NULL;
  int c_display_width = 0;
  int c_display_height = 0;
  int c_bench_value = 0;
  int c_magic_number = 0;
  lw6map_level_t *c_level = NULL;
  char *user_dir = NULL;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (dirname), dirname, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (default_param) || default_param == SCM_EOL, default_param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (forced_param) || forced_param == SCM_EOL, forced_param, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_width), display_width, SCM_ARG4, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_height), display_height, SCM_ARG5, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (bench_value), bench_value, SCM_ARG6, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (magic_number), magic_number, SCM_ARG7, __FUNCTION__);

  lw6sys_progress_default (sys_context, &progress, &(lw6_global.progress));
  lw6sys_progress_begin (sys_context, &progress);

  c_dirname = lw6scm_utils_to_0str (sys_context, dirname);
  if (c_dirname)
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

	      user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
	      if (user_dir)
		{
		  c_level =
		    lw6ldr_read (sys_context, c_dirname, c_default_param, c_forced_param,
				 c_display_width, c_display_height, c_bench_value, c_magic_number, user_dir, &progress);
		  if (c_level)
		    {
		      ret = lw6_make_scm_map (sys_context, c_level);
		    }
		  LW6SYS_FREE (sys_context, user_dir);
		}
	      lw6sys_assoc_free (sys_context, c_forced_param);
	    }
	  lw6sys_assoc_free (sys_context, c_default_param);
	}
      LW6SYS_FREE (sys_context, c_dirname);
    }
  lw6sys_progress_begin (sys_context, &progress);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ldr_read_relative (SCM map_path, SCM relative_path, SCM default_param,
			   SCM forced_param, SCM display_width, SCM display_height, SCM bench_value, SCM magic_number)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_map_path = NULL;
  char *c_relative_path = NULL;
  lw6sys_assoc_t *c_default_param = NULL;
  lw6sys_assoc_t *c_forced_param = NULL;
  int c_display_width = 0;
  int c_display_height = 0;
  int c_bench_value = 0;
  int c_magic_number = 0;
  lw6map_level_t *c_level = NULL;
  char *user_dir = NULL;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (map_path), map_path, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (relative_path), relative_path, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (default_param) || default_param == SCM_EOL, default_param, SCM_ARG3, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (forced_param) || forced_param == SCM_EOL, forced_param, SCM_ARG4, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_width), display_width, SCM_ARG5, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (display_height), display_height, SCM_ARG6, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (bench_value), bench_value, SCM_ARG7, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (magic_number), magic_number, SCM_ARGn, __FUNCTION__);

  progress.min = 0.0f;
  progress.max = 1.0f;
  progress.value = &(lw6_global.progress);

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

		  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
		  if (user_dir)
		    {
		      c_level =
			lw6ldr_read_relative (sys_context, c_map_path, c_relative_path,
					      c_default_param, c_forced_param,
					      c_display_width, c_display_height, c_bench_value, c_magic_number, user_dir, &progress);
		      if (c_level)
			{
			  ret = lw6_make_scm_map (sys_context, c_level);
			}
		      LW6SYS_FREE (sys_context, user_dir);
		    }
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
_scm_lw6ldr_print_examples ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (user_dir)
    {
      lw6ldr_print_examples (sys_context, user_dir);
      LW6SYS_FREE (sys_context, user_dir);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6ldr_hints_get_default (SCM key)
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
      c_value = lw6ldr_hints_get_default (sys_context, c_key);
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
_scm_lw6ldr_exp_validate (SCM level)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_level;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.map, level), level, SCM_ARG1, __FUNCTION__);

  c_level = lw6_scm_to_map (sys_context, level);
  if (c_level)
    {
      user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
      if (user_dir)
	{
	  ret = lw6ldr_exp_validate (sys_context, c_level, user_dir) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (sys_context, user_dir);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6ldr_chain_entry (SCM map_path, SCM relative_path)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_map_path = NULL;
  char *c_relative_path = NULL;
  char *user_dir = NULL;
  lw6ldr_entry_t *c_entry = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (map_path), map_path, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (relative_path), relative_path, SCM_ARG2, __FUNCTION__);

  c_map_path = lw6scm_utils_to_0str (sys_context, map_path);
  if (c_map_path)
    {
      c_relative_path = lw6scm_utils_to_0str (sys_context, relative_path);
      if (c_relative_path)
	{
	  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
	  if (user_dir)
	    {
	      c_entry = lw6ldr_chain_entry (sys_context, c_map_path, c_relative_path, user_dir);
	      if (c_entry)
		{
		  ret = SCM_EOL;
		  ret = scm_cons (scm_cons (scm_from_locale_string ("title"), scm_from_locale_string (c_entry->metadata.title)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("author"), scm_from_locale_string (c_entry->metadata.author)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("description"), scm_from_locale_string (c_entry->metadata.description)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("license"), scm_from_locale_string (c_entry->metadata.license)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("absolute-path"), scm_from_locale_string (c_entry->absolute_path)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("relative-path"), scm_from_locale_string (c_entry->relative_path)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("has-subdirs"), c_entry->has_subdirs ? SCM_BOOL_T : SCM_BOOL_F), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("nb-submaps"), scm_from_int (c_entry->nb_submaps)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("exp"), scm_from_int (c_entry->metadata.vanilla_exp)), ret);
		  ret = scm_cons (scm_cons (scm_from_locale_string ("forbidden"), c_entry->forbidden ? SCM_BOOL_T : SCM_BOOL_F), ret);
		  lw6ldr_free_entry (sys_context, c_entry);
		}
	      LW6SYS_FREE (sys_context, user_dir);
	    }
	  LW6SYS_FREE (sys_context, c_relative_path);
	}
      LW6SYS_FREE (sys_context, c_map_path);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_ldr
 *
 * @sys_context: global system context
 *
 * Register the functions of the ldr module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_ldr (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6ldr
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6LDR_GET_ENTRIES, 2, 0, 0, (SCM (*)())_scm_lw6ldr_get_entries);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6LDR_READ, 7, 0, 0, (SCM (*)())_scm_lw6ldr_read);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6LDR_READ_RELATIVE, 8, 0, 0, (SCM (*)())_scm_lw6ldr_read_relative);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6LDR_PRINT_EXAMPLES, 0, 0, 0, (SCM (*)())_scm_lw6ldr_print_examples);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6LDR_HINTS_GET_DEFAULT, 1, 0, 0, (SCM (*)())_scm_lw6ldr_hints_get_default);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6LDR_EXP_VALIDATE, 1, 0, 0, (SCM (*)())_scm_lw6ldr_exp_validate);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6LDR_CHAIN_ENTRY, 2, 0, 0, (SCM (*)())_scm_lw6ldr_chain_entry);

  return ret;
}
