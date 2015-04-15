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
#else // LW6_MS_WINDOWS
#define DIR_SEP_CHAR DIR_SEP_CHAR_UNIX
#define DIR_SEP_STR DIR_SEP_STR_UNIX
#endif // LW6_MS_WINDOWS

#define _CREATE_DIR_NB_PARENTS 3

static int
_is_dir_sep (lw6sys_context_t * sys_context, char c)
{
  return ((c == DIR_SEP_CHAR_UNIX) || (c == DIR_SEP_CHAR_MS_WINDOWS));
}

static void
_normalize (lw6sys_context_t * sys_context, char *path)
{
  char *pos = NULL;

  for (pos = path; (*pos); pos++)
    {
      if (_is_dir_sep (sys_context, *pos))
	{
	  (*pos) = DIR_SEP_CHAR;
	}
    }
}

/**
 * lw6sys_file_exists
 *
 * @sys_context: global system context
 * @filename: the file to test
 *
 * Tests the existence of a file on the filesystem. File is considered
 * to exists if it's at least readable.
 *
 * Return value: 1 if OK, 0 if file doesn't exist or can't be read.
 */
int
lw6sys_file_exists (lw6sys_context_t * sys_context, const char *filename)
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
 * @sys_context: global system context
 * @dirname: the directory to test
 *
 * Tests the existence of a directory on the filesystem.
 *
 * Return value: 1 if OK, 0 if directory doesn't exist.
 */
int
lw6sys_dir_exists (lw6sys_context_t * sys_context, const char *dirname)
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

/**
 * lw6sys_dir_exists_with_readme
 *
 * @sys_context: global system context
 * @dirname: the directory to test
 *
 * Tests the existence of a directory on the filesystem, will also
 * validate that it contains a README or readme.txt file.
 *
 * Return value: 1 if OK, 0 if directory doesn't exist.
 */
int
lw6sys_dir_exists_with_readme (lw6sys_context_t * sys_context, const char *dirname)
{
  int ret = 0;
  char *readme_filename = NULL;

  if (lw6sys_dir_exists (sys_context, dirname))
    {
      if (!ret)
	{
	  readme_filename = lw6sys_path_concat (sys_context, dirname, LW6SYS_FILE_README);
	  if (readme_filename)
	    {
	      if (lw6sys_file_exists (sys_context, readme_filename))
		{
		  ret = 1;
		}
	      LW6SYS_FREE (sys_context, readme_filename);
	    }
	}
      if (!ret)
	{
	  readme_filename = lw6sys_path_concat (sys_context, dirname, LW6SYS_FILE_README_TXT);
	  if (readme_filename)
	    {
	      if (lw6sys_file_exists (sys_context, readme_filename))
		{
		  ret = 1;
		}
	      LW6SYS_FREE (sys_context, readme_filename);
	    }
	}
      if (!ret)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("directory \"%s\" exists but has no %s or %s file, ignoring"), dirname, LW6SYS_FILE_README, LW6SYS_FILE_README_TXT);
	}
    }

  return ret;
}

/**
 * lw6sys_dir_exists_with_readme_containing_text
 *
 * @sys_context: global system context
 * @dirname: the directory to test
 * @needle: string to search, can be NULL
 *
 * Tests the existence of a directory on the filesystem, with a README
 * or readme.txt file, which contains the string needle.
 *
 * Return value: 1 if OK, 0 if directory doesn't exist.
 */
int
lw6sys_dir_exists_with_readme_containing_text (lw6sys_context_t * sys_context, const char *dirname, const char *needle)
{
  int ret = 0;
  char *readme_filename = NULL;
  char *readme_content = NULL;

  if (lw6sys_dir_exists_with_readme (sys_context, dirname))
    {
      if (needle)
	{
	  if (!ret)
	    {
	      readme_filename = lw6sys_path_concat (sys_context, dirname, LW6SYS_FILE_README);
	      if (readme_filename)
		{
		  if (lw6sys_file_exists (sys_context, readme_filename))
		    {
		      readme_content = lw6sys_read_file_content (sys_context, readme_filename);
		      if (readme_content)
			{
			  if (strstr (readme_content, needle))
			    {
			      ret = 1;
			    }
			  LW6SYS_FREE (sys_context, readme_content);
			}
		    }
		  LW6SYS_FREE (sys_context, readme_filename);
		}
	    }
	  if (!ret)
	    {
	      readme_filename = lw6sys_path_concat (sys_context, dirname, LW6SYS_FILE_README_TXT);
	      if (readme_filename)
		{
		  if (lw6sys_file_exists (sys_context, readme_filename))
		    {
		      readme_content = lw6sys_read_file_content (sys_context, readme_filename);
		      if (readme_content)
			{
			  if (strstr (readme_content, needle))
			    {
			      ret = 1;
			    }
			  LW6SYS_FREE (sys_context, readme_content);
			}
		    }
		  LW6SYS_FREE (sys_context, readme_filename);
		}
	    }

	  if (!ret)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_
			  ("directory \"%s\" exists, has a %s or %s file, but this file does not contain the magic words \%s\", ignoring"),
			  dirname, LW6SYS_FILE_README, LW6SYS_FILE_README_TXT, needle);
	    }
	}
    }

  return ret;
}

static int
_create_dir (lw6sys_context_t * sys_context, const char *dirname, int verbose, int nb_parents)
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
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating dir \"%s\""), dirname);
	}
      ret = lw6sys_dir_exists (sys_context, dirname);
    }
  else
    {
      if (nb_parents > 0)
	{
	  char *parent = lw6sys_path_parent (sys_context, dirname);

	  if (parent)
	    {
	      ret = _create_dir (sys_context, parent, verbose, nb_parents - 1) && _create_dir (sys_context, dirname, verbose, 0);
	      LW6SYS_FREE (sys_context, parent);
	    }
	}
      else
	{
	  if (verbose)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create dir \"%s\""), dirname);
	    }
	}
    }

  return ret;
}

/**
 * lw6sys_create_dir
 *
 * @sys_context: global system context
 * @dirname: the directory to create
 *
 * Creates a directory, performing sanity checks such as verifying
 * the directory really exists after being created.
 *
 * Return value: 1 if OK, 0 if error.
 */
int
lw6sys_create_dir (lw6sys_context_t * sys_context, const char *dirname)
{
  return _create_dir (sys_context, dirname, 1, _CREATE_DIR_NB_PARENTS);
}

/**
 * lw6sys_create_dir_silent
 *
 * @sys_context: global system context
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
lw6sys_create_dir_silent (lw6sys_context_t * sys_context, const char *dirname)
{
  return _create_dir (sys_context, dirname, 0, _CREATE_DIR_NB_PARENTS);
}

/**
 * lw6sys_path_add_slash
 *
 * @sys_context: global system context
 * @path: a path
 *
 * Adds a slash, or in a general manner, a directory separator,
 * at the end of a path, if needed. So /foo/bar will become
 * /foo/bar/ but /bar/foo/ will remain /bar/foo/.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_add_slash (lw6sys_context_t * sys_context, const char *path)
{
  char *ret;
  int len;

  len = strlen (path);
  if (len == 0 || (len > 0 && !_is_dir_sep (sys_context, path[len - 1])))
    {
      ret = lw6sys_str_concat (sys_context, path, DIR_SEP_STR);
    }
  else
    {
      ret = lw6sys_str_copy (sys_context, path);
    }

  if (ret)
    {
      _normalize (sys_context, ret);
    }

  return ret;
}

/**
 * lw6sys_path_strip_slash
 *
 * @sys_context: global system context
 * @path: a path
 *
 * Strips the slash, or in a general manner, the directory separator,
 * at the end of a path, if needed. So /foo/bar/ will become
 * /foo/bar but /bar/foo will remain /bar/foo.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_strip_slash (lw6sys_context_t * sys_context, const char *path)
{
  char *ret;
  int len;

  ret = lw6sys_str_copy (sys_context, path);
  if (ret)
    {
      len = strlen (ret);
      if (len > 0 && _is_dir_sep (sys_context, ret[len - 1]))
	{
	  ret[len - 1] = '\0';
	}
    }

  if (ret)
    {
      _normalize (sys_context, ret);
    }

  return ret;
}

/**
 * lw6sys_path_concat
 *
 * @sys_context: global system context
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
lw6sys_path_concat (lw6sys_context_t * sys_context, const char *path1, const char *path2)
{
  char *path1_stripped = NULL;
  char *path2_stripped = NULL;
  char *path2_start = NULL;
  char *ret = NULL;

  path1_stripped = lw6sys_path_strip_slash (sys_context, path1);
  if (path1_stripped)
    {
      path2_stripped = lw6sys_path_strip_slash (sys_context, path2);
      if (path2_stripped)
	{
	  path2_start = path2_stripped;
	  if (_is_dir_sep (sys_context, path2_start[0]))
	    {
	      path2_start++;
	    }
	  if (!lw6sys_path_is_cwd (sys_context, path2_start))
	    {
	      if (strlen (path1_stripped) > 0)
		{
		  ret = lw6sys_new_sprintf (sys_context, "%s" DIR_SEP_STR "%s", path1_stripped, path2_start);
		}
	      else
		{
		  ret = lw6sys_str_copy (sys_context, path2_start);
		}
	    }
	  else
	    {
	      ret = lw6sys_str_copy (sys_context, path1_stripped);
	    }
	  LW6SYS_FREE (sys_context, path2_stripped);
	}
      LW6SYS_FREE (sys_context, path1_stripped);
    }

  if (ret)
    {
      _normalize (sys_context, ret);
    }

  return ret;
}

/**
 * lw6sys_path_split
 *
 * @sys_context: global system context
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
lw6sys_path_split (lw6sys_context_t * sys_context, const char *path)
{
  char *stripped_path = NULL;
  lw6sys_list_t *ret = NULL;

  stripped_path = lw6sys_path_strip_slash (sys_context, path);
  if (stripped_path)
    {
      ret = lw6sys_str_split_no_0 (sys_context, stripped_path, DIR_SEP_CHAR);
      LW6SYS_FREE (sys_context, stripped_path);
    }

  return ret;
}

const char *
_lw6sys_path_file_only_raw (const char *file)
{
  const char *file_only = NULL;

  file_only = strrchr (file, DIR_SEP_CHAR_UNIX);
  if (file_only && *file_only)
    {
      file_only++;
    }
  else
    {
      file_only = strrchr (file, DIR_SEP_CHAR_MS_WINDOWS);
      if (file_only && *file_only)
	{
	  file_only++;
	}
      else
	{
	  file_only = file;
	}
    }

  return file_only;
}

/**
 * lw6sys_path_file_only
 *
 * @sys_context: global system context
 * @path: a path
 *
 * Returns the file name only, without heading directories.
 *
 * Return value: file name, must be freed
 */
char *
lw6sys_path_file_only (lw6sys_context_t * sys_context, const char *path)
{
  char *ret = NULL;
  int i = 0;

  /*
   * We don't use strrchr for we want to trap both
   * UNIX & MS Windows separators.
   */
  i = strlen (path) - 1;

  while (i >= 0 && !_is_dir_sep (sys_context, path[i]))
    {
      --i;
    }
  ++i;

  ret = lw6sys_str_copy (sys_context, path + i);

  return ret;
}

/**
 * lw6sys_path_is_relative
 *
 * @sys_context: global system context
 * @path: a path
 *
 * Checks wether a path is relative or absolute.
 *
 * Return value: 1 if relative, 0 if absolute.
 */
int
lw6sys_path_is_relative (lw6sys_context_t * sys_context, const char *path)
{
  int ret = 0;

  if (strcmp (".", path) == 0 ||
      strncmp ("." DIR_SEP_STR_UNIX, path, 2) == 0 ||
      strncmp ("." DIR_SEP_STR_MS_WINDOWS, path, 2) == 0 ||
      strncmp (".." DIR_SEP_STR_UNIX, path, 3) == 0 || strncmp (".." DIR_SEP_STR_MS_WINDOWS, path, 3) == 0)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_path_is_cwd
 *
 * @sys_context: global system context
 * @path: a path
 *
 * Checks wether a path is "." or not. Will also trap "" and "./".
 *
 * Return value: 1 if relative, 0 if absolute.
 */
int
lw6sys_path_is_cwd (lw6sys_context_t * sys_context, const char *path)
{
  int ret = 0;

  if (strcmp ("", path) == 0 || strcmp (".", path) == 0 || strcmp ("." DIR_SEP_STR_UNIX, path) == 0 || strcmp ("." DIR_SEP_STR_MS_WINDOWS, path) == 0)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_path_parent
 *
 * @sys_context: global system context
 * @path: a path
 *
 * Returns the parent path. That will return /foo when given
 * /foo/bar in input.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_parent (lw6sys_context_t * sys_context, const char *path)
{
  char *parent = NULL;
  char *stripped_path = NULL;
  char *pos = NULL;

  if (path)
    {
      stripped_path = lw6sys_path_strip_slash (sys_context, path);
      if (stripped_path)
	{
	  pos = strrchr (stripped_path, DIR_SEP_CHAR);
	  if (pos)
	    {
	      if (strcmp (pos + 1, "..") == 0)
		{
		  /*
		   * Path ends with "..", so we just append ".."
		   * again on it.
		   */
		  parent = lw6sys_path_concat (sys_context, stripped_path, "..");
		}
	      else
		{
		  /*
		   * Path has a slash (or whatever dir sep is
		   * so we just get rid of that item
		   */
		  (*pos) = '\0';
		  parent = lw6sys_str_copy (sys_context, stripped_path);
		}
	    }
	  else
	    {
	      if (strcmp (stripped_path, "") == 0 || strcmp (stripped_path, ".") == 0)
		{
		  /*
		   * Path is cwd, or empty, so we just return
		   * the standard shell for parent : ..
		   */
		  parent = lw6sys_str_copy (sys_context, "..");
		}
	      else
		{
		  if (strcmp (stripped_path, "..") == 0)
		    {
		      /*
		       * Special case, if path was "..", then
		       * return "../.."
		       */
		      parent = lw6sys_path_concat (sys_context, stripped_path, "..");
		    }
		  else
		    {
		      /*
		       * Path is just a file name, with no / no nothing,
		       * we consider parent is "." since an unqualified
		       * filename is by default taken from cwd
		       */
		      parent = lw6sys_str_copy (sys_context, ".");
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, stripped_path);
	}
    }

  if (!parent)
    {
      parent = lw6sys_str_copy (sys_context, "..");
    }
  if (parent)
    {
      _normalize (sys_context, parent);
    }

  return parent;
}

/**
 * lw6sys_path_unparent
 *
 * @sys_context: global system context
 * @path: a path
 *
 * Given the ../foo/bar path, will return foo/bar. Usefull to
 * get rid of heading ../ when a path is known to start with it.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_path_unparent (lw6sys_context_t * sys_context, const char *path)
{
  char *unparent = NULL;
  char *slashed_path = NULL;

  slashed_path = lw6sys_path_add_slash (sys_context, path);
  if (slashed_path)
    {
      if (strncmp (".." DIR_SEP_STR_UNIX, slashed_path, 3) == 0 || strncmp (".." DIR_SEP_STR_MS_WINDOWS, slashed_path, 3) == 0)
	{
	  unparent = lw6sys_str_copy (sys_context, path + 3);
	}
      else
	{
	  unparent = lw6sys_str_copy (sys_context, path);
	}
      LW6SYS_FREE (sys_context, slashed_path);
    }

  if (unparent)
    {
      _normalize (sys_context, unparent);
    }

  return unparent;
}

/**
 * lw6sys_path_unparent_no_malloc
 *
 * @sys_context: global system context
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
lw6sys_path_unparent_no_malloc (lw6sys_context_t * sys_context, char *path)
{
  char *unparent = NULL;

  if (strncmp (".." DIR_SEP_STR_UNIX, path, 3) == 0 || strncmp (".." DIR_SEP_STR_MS_WINDOWS, path, 3) == 0)
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
_dir_list_add_entry (lw6sys_context_t * sys_context, lw6sys_list_t ** list,
		     const char *dir, const char *item, lw6sys_dir_list_filter_func_t filter_func, void *func_data, int *n)
{
  int ok = 0;
  char *path = NULL;

  path = lw6sys_path_concat (sys_context, dir, item);
  if (path)
    {
      if (filter_func)
	{
	  ok = filter_func (sys_context, func_data, path);
	}
      else
	{
	  ok = 1;
	}
      if (ok)
	{
	  lw6sys_list_push_front (sys_context, list, path);
	  if (n)
	    {
	      (*n)++;
	    }
	}
      else
	{
	  LW6SYS_FREE (sys_context, path);
	}
    }
}

static void
_dir_list_add_entries (lw6sys_context_t * sys_context, lw6sys_list_t ** list,
		       const char *dir, lw6sys_dir_list_filter_func_t filter_func, void *func_data, int *n)
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
      dir_wildcard = lw6sys_path_concat (sys_context, dir, "*");
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
	  _dir_list_add_entry (sys_context, list, dir, dir_entry.cFileName, filter_func, func_data, n);
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
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("no files in dir \"%s\""), dir);
    }
#else
  dir_handle = opendir (dir);
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
		  _dir_list_add_entry (sys_context, list, dir, dir_entry->d_name, filter_func, func_data, n);
		}
	      else
		{
		  eod = 1;
		}
	      LW6SYS_FREE (sys_context, dir_entry);
	    }
	}
      closedir (dir_handle);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't read dir \"%s\""), dir);
    }
#endif
  if (n && ((*n) == old_n))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("nothing in dir \"%s\""), dir);
    }
}

/**
 * lw6sys_dir_list
 *
 * @sys_context: global system context
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
lw6sys_dir_list (lw6sys_context_t * sys_context, const char *dir, lw6sys_dir_list_filter_func_t filter_func, void *func_data, int *n)
{
  lw6sys_list_t *ret = NULL;

  if (n)
    {
      (*n) = 0;
    }
  ret = lw6sys_list_new (sys_context, lw6sys_free_callback);
  if (ret)
    {
      if (lw6sys_dir_exists (sys_context, dir))
	{
	  _dir_list_add_entries (sys_context, &ret, dir, filter_func, func_data, n);
	}
    }

  return ret;
}

/**
 * lw6sys_path_list
 *
 * @sys_context: global system context
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
lw6sys_path_list (lw6sys_context_t * sys_context, const char *path, lw6sys_dir_list_filter_func_t filter_func, void *func_data, int *n)
{
  lw6sys_list_t *ret = NULL;
  lw6sys_list_t *dirs = NULL;
  char *dir = NULL;

  if (n)
    {
      (*n) = 0;
    }
  ret = lw6sys_list_new (sys_context, lw6sys_free_callback);
  if (ret)
    {
      dirs = lw6sys_env_split (sys_context, path);
      if (dirs)
	{
	  while (dirs && ((dir = (char *) lw6sys_list_pop_front (sys_context, &dirs)) != NULL))
	    {
	      _dir_list_add_entries (sys_context, &ret, dir, filter_func, func_data, n);
	      LW6SYS_FREE (sys_context, dir);
	    }
	  // normally dirs is already freed here
	}
    }

  return ret;
}

/**
 * lw6sys_find_in_dir_and_path
 *
 * @sys_context: global system context
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
lw6sys_find_in_dir_and_path (lw6sys_context_t * sys_context, const char *dir, const char *path, const char *file)
{
  char *ret = NULL;
  int file_only_offset = 0;
  char *tmp = NULL;
  char *full_path;
  lw6sys_list_t *dirs = NULL;
  char *d = NULL;

  file_only_offset = 0;		// file
  tmp = strrchr (file, DIR_SEP_CHAR_UNIX);
  if (tmp)
    {
      file_only_offset = (tmp - file) + 1;
    }
  tmp = strrchr (file, DIR_SEP_CHAR_MS_WINDOWS);
  if (tmp)
    {
      file_only_offset = (tmp - file) + 1;
    }
  if (dir && (!ret) && file_only_offset >= 0)
    {
      full_path = lw6sys_path_concat (sys_context, dir, file + file_only_offset);
      if (full_path)
	{
	  if ((!ret) && lw6sys_file_exists (sys_context, full_path))
	    {
	      ret = full_path;
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, full_path);
	    }
	}
    }
  if (path && (!ret) && file_only_offset >= 0)
    {
      dirs = lw6sys_env_split (sys_context, path);
      if (dirs)
	{
	  while (dirs && ((d = (char *) lw6sys_list_pop_front (sys_context, &dirs)) != NULL))
	    {
	      full_path = lw6sys_path_concat (sys_context, d, file + file_only_offset);
	      if (full_path)
		{
		  if ((!ret) && lw6sys_file_exists (sys_context, full_path))
		    {
		      ret = full_path;
		    }
		  else
		    {
		      LW6SYS_FREE (sys_context, full_path);
		    }
		}
	      LW6SYS_FREE (sys_context, d);
	    }
	  // normally dirs is already freed here
	}
    }

  return ret;
}
