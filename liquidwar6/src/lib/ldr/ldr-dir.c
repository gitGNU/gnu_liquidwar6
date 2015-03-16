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
#endif

#include <dirent.h>
#include <limits.h>

#include "ldr.h"
#include "ldr-internal.h"

typedef struct for_all_entries_callback_data_s
{
  const char *map_path;
  int player_exp;
  int recursive;
  lw6sys_list_callback_func_t callback_func;
  void *func_data;
} for_all_entries_callback_data_t;

typedef struct _count_submaps_callback_func1_data_s
{
  const char *map_path;
  const char *user_dir;
} _count_submaps_callback_func1_data_t;

typedef struct _chain_ret_s
{
  int exp;
  lw6ldr_entry_t *found_entry;
}
_chain_ret_t;

static lw6ldr_entry_t *
_new_entry (lw6sys_context_t * sys_context, const char *absolute_path, const char *relative_path, const char *entry_path, int player_exp)
{
  lw6ldr_entry_t *entry = NULL;
  lw6map_rules_t rules;
  char *map_filename = NULL;

  if (entry_path[0] != '.')
    {
      entry = (lw6ldr_entry_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6ldr_entry_t));
      if (entry)
	{
	  entry->metadata.vanilla_exp = LW6MAP_RULES_DEFAULT_EXP;
	  entry->absolute_path = lw6sys_path_concat (sys_context, absolute_path, entry_path);
	  if (entry->absolute_path)
	    {
	      if (lw6sys_dir_exists (sys_context, entry->absolute_path))
		{
		  lw6ldr_metadata_read (sys_context, &(entry->metadata), entry->absolute_path);
		  if (lw6sys_path_is_cwd (sys_context, relative_path))
		    {
		      entry->relative_path = lw6sys_str_copy (sys_context, entry_path);
		    }
		  else
		    {
		      entry->relative_path = lw6sys_path_concat (sys_context, relative_path, entry_path);
		    }
		  map_filename = lw6sys_path_concat (sys_context, entry->absolute_path, _LW6LDR_FILE_MAP_PNG);
		  if (map_filename)
		    {
		      if (!lw6sys_file_exists (sys_context, map_filename))
			{
			  /*
			   * OK there's no map.png, let's assume there are
			   * some sub directories in it. At least, that's all
			   * we can do...
			   */
			  entry->has_subdirs = 1;
			}
		      else
			{
			  lw6map_rules_defaults (sys_context, &rules);
			  if (lw6ldr_rules_read (sys_context, &rules, entry->absolute_path))
			    {
			      entry->metadata.vanilla_exp = rules.exp;
			    }
			}
		      LW6SYS_FREE (sys_context, map_filename);
		    }
		}
	      else
		{
		  // not a directory, ignore
		}
	    }
	}
    }

  if (entry)
    {
      if (!entry->metadata.title || !entry->metadata.author
	  || !entry->metadata.description || !entry->metadata.license || !entry->absolute_path || !entry->relative_path)
	{
	  lw6ldr_free_entry (sys_context, entry);
	  entry = NULL;
	}
    }

  if (entry)
    {
      if ((entry->metadata.vanilla_exp > player_exp) && (entry->absolute_path != NULL))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("not enough exp to load \"%s\", we load it but mark it as forbidden"), entry->absolute_path);
	  entry->forbidden = 1;
	}
    }

  if (entry)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("name \"%s\" in \"%s\" is a valid entry"), entry_path, absolute_path);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("name \"%s\" in \"%s\" is not a valid entry"), entry_path, absolute_path);
    }

  return entry;
}

static void
free_entry_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6ldr_free_entry (sys_context, (lw6ldr_entry_t *) data);
}

/**
 * lw6ldr_free_entry
 *
 * @sys_context: global system context
 * @entry: the entry to free
 *
 * Frees a map entry.
 *
 * Return value: none.
 */
void
lw6ldr_free_entry (lw6sys_context_t * sys_context, lw6ldr_entry_t * entry)
{
  if (entry)
    {
      // needs to be rock solid for construction bugs
      lw6map_metadata_clear (sys_context, &(entry->metadata));
      if (entry->absolute_path)
	{
	  LW6SYS_FREE (sys_context, entry->absolute_path);
	  entry->absolute_path = NULL;
	}
      if (entry->relative_path)
	{
	  LW6SYS_FREE (sys_context, entry->relative_path);
	  entry->relative_path = NULL;
	}
    }
  LW6SYS_FREE (sys_context, entry);
}

/**
 * lw6ldr_dup_entry
 *
 * @sys_context: global system context
 * @entry: the entry to dup
 *
 * Dup a map entry.
 *
 * Return value: newly allocated object.
 */
lw6ldr_entry_t *
lw6ldr_dup_entry (lw6sys_context_t * sys_context, const lw6ldr_entry_t * entry)
{
  lw6ldr_entry_t *ret;

  ret = (lw6ldr_entry_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6ldr_entry_t));
  if (ret)
    {
      memcpy (ret, entry, sizeof (lw6ldr_entry_t));

      ret->metadata.title = lw6sys_str_copy (sys_context, ret->metadata.title);
      ret->metadata.author = lw6sys_str_copy (sys_context, ret->metadata.author);
      ret->metadata.description = lw6sys_str_copy (sys_context, ret->metadata.description);
      ret->metadata.license = lw6sys_str_copy (sys_context, ret->metadata.license);
      ret->metadata.vanilla_exp = ret->metadata.vanilla_exp;
      ret->absolute_path = lw6sys_str_copy (sys_context, ret->absolute_path);
      ret->relative_path = lw6sys_str_copy (sys_context, ret->relative_path);

      if (!ret->metadata.title || !ret->metadata.author || !ret->metadata.description || !ret->metadata.license || !ret->absolute_path || !ret->relative_path)
	{
	  lw6ldr_free_entry (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

static void
_add_entry (lw6sys_context_t * sys_context, lw6sys_list_t ** entries, lw6sys_assoc_t ** entries_index,
	    const char *absolute_path, const char *relative_path, const char *entry_path, int *n, int player_exp)
{
  lw6ldr_entry_t *entry = NULL;

  entry = _new_entry (sys_context, absolute_path, relative_path, entry_path, player_exp);
  if (entry)
    {
      if (!lw6sys_assoc_has_key (sys_context, *entries_index, entry->relative_path))
	{
	  lw6sys_lifo_push (sys_context, entries, (void *) entry);
	  lw6sys_assoc_set (sys_context, entries_index, entry->relative_path, NULL);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_
		      ("duplicate map \"%s\", it is present in at least two places, last found (and ignored) in \"%s\""),
		      entry->relative_path, entry->absolute_path);
	  lw6ldr_free_entry (sys_context, entry);
	  entry = NULL;
	}
      (*n)++;
    }
}

static void
_add_subdirs (lw6sys_context_t * sys_context, lw6sys_list_t ** entries, lw6sys_assoc_t ** entries_index, const char *absolute_path, const char *relative_path,
	      int player_exp)
{
#ifdef LW6_MS_WINDOWS
  WIN32_FIND_DATA dir_entry;
  HANDLE dir_handle = INVALID_HANDLE_VALUE;
  char *dir_wildcard = NULL;
#else // LW6_MS_WINDOWS
  struct dirent *dir_entry = NULL;
  struct dirent *dir_entry_result = NULL;
  int dir_entry_size = 0;
  DIR *dir_handle = NULL;
#endif // LW6_MS_WINDOWS
  int n = 0;
  int eod = 0;

#ifdef LW6_MS_WINDOWS
  memset (&dir_entry, 0, sizeof (WIN32_FIND_DATA));
  if (strlen (absolute_path) + 2 < MAX_PATH)
    {
      dir_wildcard = lw6sys_path_concat (sys_context, absolute_path, "*");
      if (dir_wildcard)
	{
	  dir_handle = FindFirstFile (dir_wildcard, &dir_entry);
	  LW6SYS_FREE (sys_context, dir_wildcard);
	}
    }
  if (dir_handle != INVALID_HANDLE_VALUE)
    {
      while (!eod)
	{
	  _add_entry (sys_context, entries, entries_index, absolute_path, relative_path, dir_entry.cFileName, &n, player_exp);
	  memset (&dir_entry, 0, sizeof (WIN32_FIND_DATA));
	  if (!FindNextFile (dir_handle, &dir_entry))
	    {
	      eod = 1;
	    }
	}
      FindClose (dir_handle);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("no files in dir \"%s\""), absolute_path);
    }
#else // LW6_MS_WINDOWS
  dir_handle = opendir (absolute_path);
  if (dir_handle)
    {
      while (!eod)
	{
	  dir_entry_size = sizeof (struct dirent) + NAME_MAX + 1;
	  dir_entry = (struct dirent *) LW6SYS_CALLOC (sys_context, dir_entry_size);
	  if (dir_entry)
	    {
	      readdir_r (dir_handle, dir_entry, &dir_entry_result);
	      if (dir_entry_result && (dir_entry_result == dir_entry))
		{
		  _add_entry (sys_context, entries, entries_index, absolute_path, relative_path, dir_entry->d_name, &n, player_exp);
		}
	      else
		{
		  eod = 1;
		}
	      LW6SYS_FREE (sys_context, dir_entry);
	    }
	}
      if (n == 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("no maps in dir \"%s\""), absolute_path);
	}
      closedir (dir_handle);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't read map dir \"%s\""), absolute_path);
    }
#endif // LW6_MS_WINDOWS
}

static int
_entries_sort_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;
  const lw6ldr_entry_t *entry_a;
  const lw6ldr_entry_t *entry_b;

  entry_a = (const lw6ldr_entry_t *) ptr_a;
  entry_b = (const lw6ldr_entry_t *) ptr_b;

  if (entry_a->has_subdirs && (!entry_b->has_subdirs))
    {
      ret = -1;
    }
  else if ((!entry_a->has_subdirs) && entry_b->has_subdirs)
    {
      ret = 1;
    }
  else if (entry_a->metadata.vanilla_exp < entry_b->metadata.vanilla_exp)
    {
      ret = -1;
    }
  else if (entry_a->metadata.vanilla_exp > entry_b->metadata.vanilla_exp)
    {
      ret = 1;
    }
  else
    {
      ret = strcmp (entry_a->metadata.title, entry_b->metadata.title);
    }

  return ret;
}

static lw6sys_list_t *
_get_entries (lw6sys_context_t * sys_context, const char *map_path, const char *relative_path, int player_exp)
{
  lw6sys_list_t *entries = NULL;
  lw6sys_assoc_t *entries_index = NULL;
  lw6sys_list_t *dirs = NULL;
  char *dir = NULL;
  char *absolute_path = NULL;

  entries_index = lw6sys_assoc_new (sys_context, NULL);
  if (entries_index)
    {
      entries = lw6sys_list_new (sys_context, free_entry_callback);
      if (entries)
	{
	  dirs = lw6sys_env_split (sys_context, map_path);
	  if (dirs)
	    {
	      while (dirs && (dir = (char *) lw6sys_lifo_pop (sys_context, &dirs)) != NULL)
		{
		  if (dir)
		    {
		      if (strlen (dir) > 0)
			{
			  absolute_path = lw6sys_path_concat (sys_context, dir, relative_path);
			  if (absolute_path)
			    {
			      if (lw6sys_dir_exists (sys_context, absolute_path))
				{
				  _add_subdirs (sys_context, &entries, &entries_index, absolute_path, relative_path, player_exp);
				}
			      LW6SYS_FREE (sys_context, absolute_path);
			    }
			}
		      LW6SYS_FREE (sys_context, dir);
		    }
		}
	    }
	}
      lw6sys_assoc_free (sys_context, entries_index);
    }

  if (entries)
    {
      lw6sys_sort (sys_context, &entries, _entries_sort_callback, NULL);
    }

  return entries;
}

static void
_count_submaps_callback_func2 (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  int *nb_submaps = (int *) func_data;

  if (!entry->has_subdirs)
    {
      (*nb_submaps)++;
    }
}

static void
_count_submaps_callback_func1 (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  _count_submaps_callback_func1_data_t *func1_data = (_count_submaps_callback_func1_data_t *) func_data;

  lw6ldr_for_all_entries (sys_context, func1_data->map_path, entry->relative_path, func1_data->user_dir, 1, _count_submaps_callback_func2,
			  &(entry->nb_submaps));
}

/**
 * lw6ldr_get_entries
 *
 * @sys_context: global system context
 * @map_path: the map_path environment config variable, delimited path list
 * @relative_path: the relative path to use to find the map directory
 * @user_dir: the user directory
 *
 * Lists all maps in a given directory. Returns a list of lw6ldr_entry_t
 * which can contain both directories with subdirs and actual maps. Maps
 * are sorted before being returned, first directories, then maps, sorted
 * in alphabetical order.
 *
 * Return value: a list of dynamically allocated lw6ldr_entry_t.
 */
lw6sys_list_t *
lw6ldr_get_entries (lw6sys_context_t * sys_context, const char *map_path, const char *relative_path, const char *user_dir)
{
  int player_exp = 0;
  lw6sys_list_t *entries = NULL;
  _count_submaps_callback_func1_data_t func1_data;

  lw6cfg_load_exp (sys_context, user_dir, &player_exp);
  entries = _get_entries (sys_context, map_path, relative_path, player_exp);
  func1_data.map_path = map_path;
  func1_data.user_dir = user_dir;
  lw6sys_list_map (sys_context, entries, _count_submaps_callback_func1, (void *) &func1_data);

  return entries;
}

static void
for_all_entries_callback_func (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  for_all_entries_callback_data_t *callback_data = (for_all_entries_callback_data_t *) func_data;
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  lw6sys_list_t *entries;

  if (entry && callback_data)
    {
      if (entry->has_subdirs)
	{
	  if (callback_data->recursive)
	    {
	      entries = _get_entries (sys_context, callback_data->map_path, entry->relative_path, callback_data->player_exp);
	      if (entries)
		{
		  lw6sys_list_map (sys_context, entries, for_all_entries_callback_func, func_data);
		  lw6sys_list_free (sys_context, entries);
		}
	    }
	}
      else
	{
	  callback_data->callback_func (sys_context, callback_data->func_data, data);
	}
    }
}

/**
 * lw6ldr_for_all_entries
 *
 * @sys_context: global system context
 * @map_path: the map_path environment config variable, delimited path list
 * @relative_path: the relative path to use to find the map directory
 * @user_dir: the user directory
 * @recursive: if non-zero, map search will recurse in subdirs
 * @callback_func: the function which will be called on each entry
 * @func_data: an extra pointer to pass data to callback_func
 *
 * Executes a given function on all maps in a given place, typically
 * used in test programs.
 *
 * Return value: none.
 */
void
lw6ldr_for_all_entries (lw6sys_context_t * sys_context, const char *map_path, const char *relative_path,
			const char *user_dir, int recursive, lw6sys_list_callback_func_t callback_func, void *func_data)
{
  lw6sys_list_t *entries = NULL;
  for_all_entries_callback_data_t callback_data;
  int player_exp;

  memset (&callback_data, 0, sizeof (for_all_entries_callback_data_t));
  lw6cfg_load_exp (sys_context, user_dir, &player_exp);
  entries = _get_entries (sys_context, map_path, relative_path, player_exp);
  if (entries)
    {
      callback_data.map_path = map_path;
      callback_data.player_exp = player_exp;
      callback_data.recursive = recursive;
      callback_data.callback_func = callback_func;
      callback_data.func_data = func_data;
      lw6sys_list_map (sys_context, entries, for_all_entries_callback_func, (void *) &callback_data);
      lw6sys_list_free (sys_context, entries);
    }
}

static void
_chain_func (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  _chain_ret_t *chain_ret = (_chain_ret_t *) func_data;
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("analyzing \"%s\" exp=%d"), entry->absolute_path, entry->metadata.vanilla_exp);

  if (!(chain_ret->found_entry))
    {
      /*
       * We only search for the case where exps are the same
       * if map exp is inferior means this is a level we're likely
       * to have already done, if map exp is greater, there's a bug
       * or we're cheating.
       */
      if (entry->metadata.vanilla_exp == chain_ret->exp)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("ok, found \"%s\" exp=%d which is suitable"), entry->absolute_path, entry->metadata.vanilla_exp);
	  chain_ret->found_entry = lw6ldr_dup_entry (sys_context, entry);
	}
    }
}

/**
 * lw6ldr_chain_entry
 *
 * @sys_context: global system context
 * @map_path: the map_path environment config variable, delimited path list
 * @relative_path: the relative path to use to find the map directory
 * @user_dir: the user directory
 *
 * Gets the next entry
 * used in test programs.
 *
 * Return value: none.
 */
lw6ldr_entry_t *
lw6ldr_chain_entry (lw6sys_context_t * sys_context, const char *map_path, const char *relative_path, const char *user_dir)
{
  char *parent = NULL;
  lw6ldr_entry_t *ret = NULL;
  _chain_ret_t chain_ret;

  chain_ret.exp = LW6MAP_RULES_MIN_EXP;
  chain_ret.found_entry = NULL;
  lw6cfg_load_exp (sys_context, user_dir, &chain_ret.exp);
  parent = lw6sys_path_parent (sys_context, relative_path);
  if (parent)
    {
      lw6ldr_for_all_entries (sys_context, map_path, parent, user_dir, 0, &_chain_func, &chain_ret);
      ret = chain_ret.found_entry;
      LW6SYS_FREE (sys_context, parent);
    }

  return ret;
}
