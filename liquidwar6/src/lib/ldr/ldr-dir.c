/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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
  char *map_path;
  int player_exp;
  int recursive;
  lw6sys_list_callback_func_t callback_func;
  void *func_data;
} for_all_entries_callback_data_t;

static lw6ldr_entry_t *
new_entry (char *absolute_path, char *relative_path, char *entry_path,
	   int player_exp)
{
  lw6ldr_entry_t *entry = NULL;
  lw6map_rules_t rules;
  char *map_filename = NULL;
  int map_exp = LW6MAP_RULES_DEFAULT_EXP;

  if (entry_path[0] != '.')
    {
      entry = (lw6ldr_entry_t *) LW6SYS_CALLOC (sizeof (lw6ldr_entry_t));
      if (entry)
	{
	  entry->absolute_path =
	    lw6sys_path_concat (absolute_path, entry_path);
	  if (entry->absolute_path)
	    {
	      if (lw6sys_dir_exists (entry->absolute_path))
		{
		  entry->title = lw6sys_str_copy (entry_path);
		  if (lw6sys_path_is_cwd (relative_path))
		    {
		      entry->relative_path = lw6sys_str_copy (entry_path);
		    }
		  else
		    {
		      entry->relative_path =
			lw6sys_path_concat (relative_path, entry_path);
		    }
		  map_filename =
		    lw6sys_path_concat (entry->absolute_path,
					_LW6LDR_FILE_MAP_PNG);
		  if (map_filename)
		    {
		      if (!lw6sys_file_exists (map_filename))
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
			  lw6map_rules_defaults (&rules);
			  if (lw6ldr_rules_read
			      (&rules, entry->absolute_path))
			    {
			      map_exp = rules.exp;
			    }
			}
		      LW6SYS_FREE (map_filename);
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
      if (!entry->title || !entry->absolute_path || !entry->relative_path)
	{
	  lw6ldr_free_entry (entry);
	  entry = NULL;
	}
    }

  if (entry)
    {
      if ((map_exp > player_exp) && (entry->absolute_path != NULL))
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_
		      ("not enough exp to load \"%s\", excluded from list"),
		      entry->absolute_path);
	  lw6ldr_free_entry (entry);
	  entry = NULL;
	}
    }

  if (entry)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("name \"%s\" in \"%s\" is a valid entry"), entry_path,
		  absolute_path);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("name \"%s\" in \"%s\" is not a valid entry"),
		  entry_path, absolute_path);
    }

  return entry;
}

static void
free_entry_callback (void *data)
{
  lw6ldr_free_entry ((lw6ldr_entry_t *) data);
}

/**
 * lw6ldr_free_entry
 *
 * @entry: the entry to free
 *
 * Frees a map entry.
 *
 * Return value: none.
 */
void
lw6ldr_free_entry (lw6ldr_entry_t * entry)
{
  if (entry)
    {
      // needs to be rock solid for construction bugs
      if (entry->title)
	{
	  LW6SYS_FREE (entry->title);
	  entry->title = NULL;
	}
      if (entry->absolute_path)
	{
	  LW6SYS_FREE (entry->absolute_path);
	  entry->absolute_path = NULL;
	}
      if (entry->relative_path)
	{
	  LW6SYS_FREE (entry->relative_path);
	  entry->relative_path = NULL;
	}
    }
  LW6SYS_FREE (entry);
}

static void
add_entry (lw6sys_list_t ** entries, lw6sys_assoc_t ** entries_index,
	   char *absolute_path, char *relative_path, char *entry_path, int *n,
	   int player_exp)
{
  lw6ldr_entry_t *entry = NULL;

  entry = new_entry (absolute_path, relative_path, entry_path, player_exp);
  if (entry)
    {
      if (!lw6sys_assoc_has_key (*entries_index, entry->relative_path))
	{
	  lw6sys_lifo_push (entries, (void *) entry);
	  lw6sys_assoc_set (entries_index, entry->relative_path, NULL);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_
		      ("duplicate map \"%s\", it is present in at least two places, last found (and ignored) in \"%s\""),
		      entry->relative_path, entry->absolute_path);
	  lw6ldr_free_entry (entry);
	  entry = NULL;
	}
      (*n)++;
    }
}

static void
add_subdirs (lw6sys_list_t ** entries, lw6sys_assoc_t ** entries_index,
	     char *absolute_path, char *relative_path, int player_exp)
{
#ifdef LW6_MS_WINDOWS
  WIN32_FIND_DATA dir_entry;
  HANDLE dir_handle = INVALID_HANDLE_VALUE;
  char *dir_wildcard = NULL;
#else
  struct dirent *dir_entry = NULL;
  struct dirent *dir_entry_result = NULL;
  int dir_entry_size = 0;
  DIR *dir_handle = NULL;
#endif
  int n = 0;
  int eod = 0;

#ifdef LW6_MS_WINDOWS
  memset (&dir_entry, 0, sizeof (WIN32_FIND_DATA));
  if (strlen (absolute_path) + 2 < MAX_PATH)
    {
      dir_wildcard = lw6sys_path_concat (absolute_path, "*");
      if (dir_wildcard)
	{
	  dir_handle = FindFirstFile (dir_wildcard, &dir_entry);
	  LW6SYS_FREE (dir_wildcard);
	}
    }
  if (dir_handle != INVALID_HANDLE_VALUE)
    {
      while (!eod)
	{
	  add_entry (entries, entries_index, absolute_path, relative_path,
		     dir_entry.cFileName, &n, player_exp);
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
      lw6sys_log (LW6SYS_LOG_INFO, "", _x_ ("no files in dir \"%s\""),
		  absolute_path);
    }
#else
  dir_handle = opendir (absolute_path);
  if (dir_handle)
    {
      while (!eod)
	{
	  dir_entry_size = sizeof (struct dirent) + NAME_MAX + 1;
	  dir_entry = (struct dirent *) LW6SYS_CALLOC (dir_entry_size);
	  if (dir_entry)
	    {
	      readdir_r (dir_handle, dir_entry, &dir_entry_result);
	      if (dir_entry_result && (dir_entry_result == dir_entry))
		{
		  add_entry (entries, entries_index, absolute_path,
			     relative_path, dir_entry->d_name, &n,
			     player_exp);
		}
	      else
		{
		  eod = 1;
		}
	      LW6SYS_FREE (dir_entry);
	    }
	}
      if (n == 0)
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("no maps in dir \"%s\""), absolute_path);
	}
      closedir (dir_handle);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, "",
		  _x_ ("couldn't read map dir \"%s\""), absolute_path);
    }
#endif
}

static int
entries_sort_callback (lw6sys_list_t ** list_a, lw6sys_list_t ** list_b)
{
  int ret = 0;
  lw6ldr_entry_t *entry_a;
  lw6ldr_entry_t *entry_b;

  entry_a = (lw6ldr_entry_t *) ((*list_a)->data);
  entry_b = (lw6ldr_entry_t *) ((*list_b)->data);

  if (entry_a->has_subdirs && (!entry_b->has_subdirs))
    {
      ret = -1;
    }
  else if ((!entry_a->has_subdirs) && entry_b->has_subdirs)
    {
      ret = 1;
    }
  else
    {
      ret = strcmp (entry_a->title, entry_b->title);
    }

  return ret;
}

static lw6sys_list_t *
_get_entries (char *map_path, char *relative_path, int player_exp)
{
  lw6sys_list_t *entries = NULL;
  lw6sys_assoc_t *entries_index = NULL;
  lw6sys_list_t *dirs = NULL;
  char *dir = NULL;
  char *absolute_path = NULL;

  entries_index = lw6sys_assoc_new (NULL);
  if (entries_index)
    {
      entries = lw6sys_list_new (free_entry_callback);
      if (entries)
	{
	  dirs = lw6sys_env_split (map_path);
	  if (dirs)
	    {
	      while (dirs && (dir = (char *) lw6sys_lifo_pop (&dirs)) != NULL)
		{
		  if (dir)
		    {
		      if (strlen (dir) > 0)
			{
			  absolute_path =
			    lw6sys_path_concat (dir, relative_path);
			  if (absolute_path)
			    {
			      if (lw6sys_dir_exists (absolute_path))
				{
				  add_subdirs (&entries, &entries_index,
					       absolute_path, relative_path,
					       player_exp);
				}
			      LW6SYS_FREE (absolute_path);
			    }
			}
		      LW6SYS_FREE (dir);
		    }
		}
	    }
	}
      lw6sys_assoc_free (entries_index);
    }

  if (entries)
    {
      lw6sys_sort (&entries, entries_sort_callback);
    }

  return entries;
}


/**
 * lw6ldr_get_entries
 *
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
lw6ldr_get_entries (char *map_path, char *relative_path, char *user_dir)
{
  int player_exp = 0;
  lw6sys_list_t *entries = NULL;

  lw6cfg_load_exp (user_dir, &player_exp);
  entries = _get_entries (map_path, relative_path, player_exp);

  return entries;
}

static void
for_all_entries_callback_func (void *func_data, void *data)
{
  for_all_entries_callback_data_t *callback_data =
    (for_all_entries_callback_data_t *) func_data;
  lw6ldr_entry_t *entry = (lw6ldr_entry_t *) data;
  lw6sys_list_t *entries;

  if (entry && callback_data)
    {
      if (entry->has_subdirs)
	{
	  if (callback_data->recursive)
	    {
	      entries =
		_get_entries (callback_data->map_path,
			      entry->relative_path,
			      callback_data->player_exp);
	      if (entries)
		{
		  lw6sys_list_map (entries, for_all_entries_callback_func,
				   func_data);
		  lw6sys_list_free (entries);
		}
	    }
	}
      else
	{
	  callback_data->callback_func (callback_data->func_data, data);
	}
    }
}

/**
 * lw6ldr_for_all_entries
 *
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
lw6ldr_for_all_entries (char *map_path, char *relative_path, char *user_dir,
			int recursive,
			lw6sys_list_callback_func_t callback_func,
			void *func_data)
{
  lw6sys_list_t *entries = NULL;
  for_all_entries_callback_data_t callback_data;
  int player_exp;

  memset (&callback_data, 0, sizeof (for_all_entries_callback_data_t));
  lw6cfg_load_exp (user_dir, &player_exp);
  entries = _get_entries (map_path, relative_path, player_exp);
  if (entries)
    {
      callback_data.map_path = map_path;
      callback_data.player_exp = player_exp;
      callback_data.recursive = recursive;
      callback_data.callback_func = callback_func;
      callback_data.func_data = func_data;
      lw6sys_list_map (entries, for_all_entries_callback_func,
		       (void *) &callback_data);
      lw6sys_list_free (entries);
    }
}
