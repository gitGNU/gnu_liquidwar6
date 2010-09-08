/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include <sys/stat.h>

#include "sys.h"

#define DIR_SEP_CHAR_UNIX '/'
#define DIR_SEP_STR_UNIX "/"
#define DIR_SEP_CHAR_MS_WINDOWS '\\'
#define DIR_SEP_STR_MS_WINDOWS "\\"

#ifdef LW6_MS_WINDOWS
#define DIR_SEP_CHAR DIR_SEP_CHAR_MS_WINDOWS
#define DIR_SEP_STR DIR_SEP_STR_MS_WINDOWS
#else
#define DIR_SEP_CHAR DIR_SEP_CHAR_UNIX
#define DIR_SEP_STR DIR_SEP_STR_UNIX
#endif

static int
is_dir_sep (char c)
{
  return ((c == DIR_SEP_CHAR_UNIX) || (c == DIR_SEP_CHAR_MS_WINDOWS));
}

static void
normalize (char *path)
{
  char *pos = NULL;

  for (pos = path; (*pos); pos++)
    {
      if (is_dir_sep (*pos))
	{
	  (*pos) = DIR_SEP_CHAR;
	}
    }
}

/**
 * lw6sys_file_exists
 *
 * @filename: the file to test
 *
 * Tests the existence of a file on the filesystem. File is considered
 * to exists if it's at least readable.
 *
 * Return value: 1 if OK, 0 if file doesn't exist or can't be read.
 */
int
lw6sys_file_exists (char *filename)
{
  int ret = 0;
  FILE *f;

  f = fopen (filename, "r");
  if (f)
    {
      ret = 1;
      fclose (f);
    }

  return ret;
}

/**
 * lw6sys_dir_exists
 *
 * @dirname: the directory to test
 *
 * Tests the existence of a directory on the filesystem.
 *
 * Return value: 1 if OK, 0 if directory doesn't exist.
 */
int
lw6sys_dir_exists (char *dirname)
{
  int ret = 0;
  DIR *dir = NULL;

  dir = opendir (dirname);
  if (dir)
    {
      closedir (dir);
      dir = NULL;
      ret = 1;
    }

  return ret;
}

static int
create_dir (char *dirname, int verbose)
{
  int ret = 0;

#ifdef LW6_MS_WINDOWS
  if (mkdir (dirname))
#else
  if (mkdir (dirname, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP) == 0)
#endif
    {
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("creating dir \"%s\""), dirname);
	}
      ret = lw6sys_dir_exists (dirname);
    }
  else
    {
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create dir \"%s\""),
		      dirname);
	}
    }

  return ret;
}

/**
 * lw6sys_create_dir
 *
 * @dirname: the directory to create
 *
 * Creates a directory, performing sanity checks such as verifying
 * the directory really exists after being created.
 *
 * Return value: 1 if OK, 0 if error.
 */
int
lw6sys_create_dir (char *dirname)
{
  return create_dir (dirname, 1);
}

/**
 * lw6sys_create_dir_silent
 *
 * @dirname: the directory to create
 *
 * Creates a directory like @lw6sys_create_dir but this function
 * is silent in the sense that it won't log any error. Usefull
 * to create the log directory itself, for instance, and avoid
 * infinite loops on error.
 *
 * Return value: 1 if OK, 0 if error.
 */
int
lw6sys_create_dir_silent (char *dirname)
{
  return create_dir (dirname, 0);
}

/**
 * lw6sys_path_add_slash
 *
 * @path: a path
 *
 * Adds a slash, or in a general manner, a directory separator,
 * at the end of a path, if needed. So /foo/bar will become
 * /foo/bar/ but /bar/foo/ will remain /bar/foo/.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_add_slash (char *path)
{
  char *ret;
  int len;

  len = strlen (path);
  if (len == 0 || (len > 0 && !is_dir_sep (path[len - 1])))
    {
      ret = lw6sys_str_concat (path, DIR_SEP_STR);
    }
  else
    {
      ret = lw6sys_str_copy (path);
    }

  if (ret)
    {
      normalize (ret);
    }

  return ret;
}

/**
 * lw6sys_path_strip_slash
 *
 * @path: a path
 *
 * Strips the slash, or in a general manner, the directory separator,
 * at the end of a path, if needed. So /foo/bar/ will become
 * /foo/bar but /bar/foo will remain /bar/foo.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_strip_slash (char *path)
{
  char *ret;
  int len;

  ret = lw6sys_str_copy (path);
  if (ret)
    {
      len = strlen (ret);
      if (len > 0 && is_dir_sep (ret[len - 1]))
	{
	  ret[len - 1] = '\0';
	}
    }

  if (ret)
    {
      normalize (ret);
    }

  return ret;
}

/**
 * lw6sys_path_concat
 *
 * @path1: left part of the path
 * @path2: right part of the path
 *
 * Concatenates 2 parts of a path. Function will try to avoid
 * stupid "double-slash" when concatenating /foo/ with /bar/
 * and conversely insert a directory separator when concatenating
 * /foo with bar/.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_concat (char *path1, char *path2)
{
  char *path1_stripped = NULL;
  char *path2_stripped = NULL;
  char *path2_start = NULL;
  char *ret = NULL;

  path1_stripped = lw6sys_path_strip_slash (path1);
  if (path1_stripped)
    {
      path2_stripped = lw6sys_path_strip_slash (path2);
      if (path2_stripped)
	{
	  path2_start = path2_stripped;
	  if (is_dir_sep (path2_start[0]))
	    {
	      path2_start++;
	    }
	  if (!lw6sys_path_is_cwd (path2_start))
	    {
	      if (strlen (path1_stripped) > 0)
		{
		  ret =
		    lw6sys_new_sprintf ("%s" DIR_SEP_STR "%s", path1_stripped,
					path2_start);
		}
	      else
		{
		  ret = lw6sys_str_copy (path2_start);
		}
	    }
	  else
	    {
	      ret = lw6sys_str_copy (path1_stripped);
	    }
	  LW6SYS_FREE (path2_stripped);
	}
      LW6SYS_FREE (path1_stripped);
    }

  if (ret)
    {
      normalize (ret);
    }

  return ret;
}

/**
 * lw6sys_path_split
 *
 * @path: a path
 *
 * Splits a path into all its parts. For instance /boo/bar/foo2/bar2
 * returns a 4 elements list. This is more than a plain split, for
 * heading and tailing slashes will be ignored, and various path
 * separators will be interpreted (depends on platform).
 *
 * Return value: a list containing 0-terminated strings.
 */
lw6sys_list_t *
lw6sys_path_split (char *path)
{
  char *stripped_path = NULL;
  lw6sys_list_t *ret = NULL;

  stripped_path = lw6sys_path_strip_slash (path);
  if (stripped_path)
    {
      ret = lw6sys_str_split_no_0 (stripped_path, DIR_SEP_CHAR);
      LW6SYS_FREE (stripped_path);
    }

  return ret;
}

/**
 * lw6sys_path_is_relative
 *
 * @path: a path
 *
 * Checks wether a path is relative or absolute.
 *
 * Return value: 1 if relative, 0 if absolute.
 */
int
lw6sys_path_is_relative (char *path)
{
  int ret = 0;

  if (strcmp (".", path) == 0 ||
      strncmp ("." DIR_SEP_STR_UNIX, path, 2) == 0 ||
      strncmp ("." DIR_SEP_STR_MS_WINDOWS, path, 2) == 0 ||
      strncmp (".." DIR_SEP_STR_UNIX, path, 3) == 0 ||
      strncmp (".." DIR_SEP_STR_MS_WINDOWS, path, 3) == 0)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_path_is_cwd
 *
 * @path: a path
 *
 * Checks wether a path is "." or not. Will also trap "" and "./".
 *
 * Return value: 1 if relative, 0 if absolute.
 */
int
lw6sys_path_is_cwd (char *path)
{
  int ret = 0;

  if (strcmp ("", path) == 0 ||
      strcmp (".", path) == 0 ||
      strcmp ("." DIR_SEP_STR_UNIX, path) == 0 ||
      strcmp ("." DIR_SEP_STR_MS_WINDOWS, path) == 0)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_path_parent
 *
 * @path: a path
 *
 * Returns the parent path. That will return /foo when given
 * /foo/bar in input. 
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_parent (char *path)
{
  char *parent = NULL;
  char *stripped_path = NULL;
  char *pos = NULL;

  stripped_path = lw6sys_path_strip_slash (path);
  if (stripped_path)
    {
      pos = strrchr (stripped_path, DIR_SEP_CHAR);
      if (pos)
	{
	  if (strcmp (pos + 1, "..") == 0)
	    {
	      parent = lw6sys_path_concat (stripped_path, "..");
	    }
	  else
	    {
	      (*pos) = '\0';
	      parent = lw6sys_str_copy (stripped_path);
	    }
	}
      else
	{
	  if (strcmp (stripped_path, "") == 0
	      || strcmp (stripped_path, ".") == 0)
	    {
	      parent = lw6sys_str_copy ("..");
	    }
	  else
	    {
	      parent = lw6sys_str_copy (".");
	    }
	}
      LW6SYS_FREE (stripped_path);
    }

  if (parent)
    {
      normalize (parent);
    }

  return parent;
}

/**
 * lw6sys_path_unparent
 *
 * @path: a path
 *
 * Given the ../foo/bar path, will return foo/bar. Usefull to
 * get rid of heading ../ when a path is known to start with it.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_unparent (char *path)
{
  char *unparent = NULL;
  char *slashed_path = NULL;

  slashed_path = lw6sys_path_add_slash (path);
  if (slashed_path)
    {
      if (strncmp (".." DIR_SEP_STR_UNIX, slashed_path, 3) == 0 ||
	  strncmp (".." DIR_SEP_STR_MS_WINDOWS, slashed_path, 3) == 0)
	{
	  unparent = lw6sys_str_copy (path + 3);
	}
      else
	{
	  unparent = lw6sys_str_copy (path);
	}
      LW6SYS_FREE (slashed_path);
    }

  if (unparent)
    {
      normalize (unparent);
    }

  return unparent;
}

/**
 * lw6sys_path_unparent_no_malloc
 *
 * @path: a path
 *
 * Given the ../foo/bar path, will return foo/bar. Usefull to
 * get rid of heading ../ when a path is known to start with it.
 * This is different from @lw6sys_path_unparent just because
 * the result is not dynamically allocated and copied from source.
 *
 * Return value: a pointer which points somewhere within the
 *   string passed as an argument.
 */
char *
lw6sys_path_unparent_no_malloc (char *path)
{
  char *unparent = NULL;

  if (strncmp (".." DIR_SEP_STR_UNIX, path, 3) == 0 ||
      strncmp (".." DIR_SEP_STR_MS_WINDOWS, path, 3) == 0)
    {
      unparent = path + 3;
    }
  else
    {
      unparent = path;
    }

  return unparent;
}

static void
_dir_list_add_entry (lw6sys_list_t ** list, char *dir, char *item,
		     lw6sys_dir_list_filter_func_t filter_func,
		     void *func_data, int *n)
{
  int ok = 0;
  char *path = NULL;

  path = lw6sys_path_concat (dir, item);
  if (path)
    {
      if (filter_func)
	{
	  ok = filter_func (func_data, path);
	}
      else
	{
	  ok = 1;
	}
      if (ok)
	{
	  lw6sys_list_push_front (list, path);
	  if (n)
	    {
	      (*n)++;
	    }
	}
      else
	{
	  LW6SYS_FREE (path);
	}
    }
}

static void
_dir_list_add_entries (lw6sys_list_t ** list, char *dir,
		       lw6sys_dir_list_filter_func_t filter_func,
		       void *func_data, int *n)
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
  int old_n = 0;
  int eod = 0;

  if (n)
    {
      old_n = *n;
    }
#ifdef LW6_MS_WINDOWS
  memset (&dir_entry, 0, sizeof (WIN32_FIND_DATA));
  if (strlen (dir) + 2 < MAX_PATH)
    {
      dir_wildcard = lw6sys_path_concat (dir, "*");
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
	  _dir_list_add_entry (list, dir,
			       dir_entry.cFileName, filter_func, func_data,
			       n);
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
      lw6sys_log (LW6SYS_LOG_INFO, "", _x_ ("no files in dir \"%s\""), dir);
    }
#else
  dir_handle = opendir (dir);
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
		  _dir_list_add_entry (list,
				       dir, dir_entry->d_name, filter_func,
				       func_data, n);
		}
	      else
		{
		  eod = 1;
		}
	      LW6SYS_FREE (dir_entry);
	    }
	}
      closedir (dir_handle);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("couldn't read dir \"%s\""), dir);
    }
#endif
  if (n && ((*n) == old_n))
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("nothing in dir \"%s\""), dir);
    }
}

/**
 * lw6sys_dir_list
 *
 * @dir: the path of the directory to list
 * @filter_func: a function which will filter entries, can be NULL
 * @func_data: additionnal data passed to filter_func
 * @n: will contain the number of items found
 *
 * This list a directory. The filter will be passed the file path
 * as an argument. If it returns 1, the file is kept, if it returns 0
 * it's suppressed from the list.
 *
 * Return value: a list containing strings (file paths).
 */
lw6sys_list_t *
lw6sys_dir_list (char *dir, lw6sys_dir_list_filter_func_t filter_func,
		 void *func_data, int *n)
{
  lw6sys_list_t *ret = NULL;

  if (n)
    {
      (*n) = 0;
    }
  ret = lw6sys_list_new (lw6sys_free_callback);
  if (ret)
    {
      if (lw6sys_dir_exists (dir))
	{
	  _dir_list_add_entries (&ret, dir, filter_func, func_data, n);
	}
    }

  return ret;
}

/**
 * lw6sys_path_list
 *
 * @path: the path of the path to list
 * @filter_func: a function which will filter entries, can be NULL
 * @func_data: additionnal data passed to filter_func
 * @n: will contain the number of items found
 *
 * This list a directory. By path we mean here a list of separated
 * directories, separated by : for instance.
 * The filter will be passed the file path
 * as an argument. If it returns 1, the file is kept, if it returns 0
 * it's suppressed from the list. It's like performing a call
 * to @lw6sys_dir_list on each of the path members.
 *
 * Return value: a list containing strings (file paths).
 */
lw6sys_list_t *
lw6sys_path_list (char *path,
		  lw6sys_dir_list_filter_func_t
		  filter_func, void *func_data, int *n)
{
  lw6sys_list_t *ret = NULL;
  lw6sys_list_t *dirs = NULL;
  char *dir = NULL;

  if (n)
    {
      (*n) = 0;
    }
  ret = lw6sys_list_new (lw6sys_free_callback);
  if (ret)
    {
      dirs = lw6sys_env_split (path);
      if (dirs)
	{
	  while (dirs
		 && ((dir = (char *) lw6sys_list_pop_front (&dirs)) != NULL))
	    {
	      _dir_list_add_entries (&ret, dir, filter_func, func_data, n);
	      LW6SYS_FREE (dir);
	    }
	  // normally dirs is already freed here
	}
    }

  return ret;
}

/**
 * lw6sys_find_in_dir_and_path
 *
 * @dir: a directory, when to search the file first
 * @path: the path to search too, a separated list of dirs
 * @file: the filename to search for
 *
 * Tries to find a file in the given paths. The function is typically
 * used to find music files. First it tries to find the file in dir,
 * then it tries to find it in each dir of path. @file must be only
 * a file name and not contain any directory. The function will use
 * the filename only anyway.
 *
 * Return value: the full path of the found file.
 */
char *
lw6sys_find_in_dir_and_path (char *dir, char *path, char *file)
{
  char *ret = NULL;
  char *file_only = NULL;
  char *tmp = NULL;
  char *full_path;
  lw6sys_list_t *dirs = NULL;

  file_only = file;
  tmp = strrchr (file, DIR_SEP_CHAR_UNIX);
  if (tmp)
    {
      file_only = tmp + 1;
    }
  tmp = strrchr (file, DIR_SEP_CHAR_MS_WINDOWS);
  if (tmp)
    {
      file_only = tmp + 1;
    }
  if (dir && !ret)
    {
      full_path = lw6sys_path_concat (dir, file_only);
      if (full_path)
	{
	  if ((!ret) && lw6sys_file_exists (full_path))
	    {
	      ret = full_path;
	    }
	  else
	    {
	      LW6SYS_FREE (full_path);
	    }
	}
    }
  if (path && !ret)
    {
      dirs = lw6sys_env_split (path);
      if (dirs)
	{
	  while (dirs
		 && ((dir = (char *) lw6sys_list_pop_front (&dirs)) != NULL))
	    {
	      full_path = lw6sys_path_concat (dir, file_only);
	      if (full_path)
		{
		  if ((!ret) && lw6sys_file_exists (full_path))
		    {
		      ret = full_path;
		    }
		  else
		    {
		      LW6SYS_FREE (full_path);
		    }
		}
	      LW6SYS_FREE (dir);
	    }
	  // normally dirs is already freed here
	}
    }

  return ret;
}
