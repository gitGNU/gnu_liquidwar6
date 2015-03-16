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

#include <string.h>

#include "sys.h"

#ifndef LW6_MS_WINDOWS
#include <unistd.h>
#endif // LW6_MS_WINDOWS

#define CONFIG_FILE "config.xml"
#define LOG_FILE "log.csv"
#define MOD_LIBDIR_MASK "%s/%s-%s"
#define MOD_PREFIX_MASK "%s/lib/%s-%s"
#define SHARE_DATADIR_MASK "%s/%s-%s"
#define SHARE_PREFIX_MASK "%s/share/%s-%s"
#ifdef LW6_MAC_OS_X
#define RUN_REL "../Resources"
#define APPLICATION_SUPPORT "Library/Application Support"
#else // LW6_MAC_OS_X
#define RUN_REL "."
#endif // LW6_MAX_OS_X
#define DATA_DIR "data"
#define MUSIC_DIR "music"
#define MAP_DIR "map"
#define SCRIPT_FILE "script/liquidwar6.scm"
#ifdef LW6_MS_WINDOWS
#define README_FILE "readme.txt"
#else // LW6_MS_WINDOWS
#define README_FILE "README"
#endif // LW6_MS_WINDOWS
#define CWD_SIZE 2048
#define _DIR_IF_NOT_FOUND_NB_PARENTS 5
#define _FILE_IF_NOT_FOUND_NB_PARENTS 5

/**
 * lw6sys_get_cwd
 *
 * @sys_context: global system context
 *
 * Returns the current working directory (absolute path).
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_cwd (lw6sys_context_t * sys_context)
{
  char *cwd = NULL;
  char *cwd_tmp = NULL;

  cwd_tmp = (char *) LW6SYS_CALLOC (sys_context, CWD_SIZE + 1);

  if (cwd_tmp)
    {
#ifdef LW6_MS_WINDOWS
      if (!GetCurrentDirectoryA (CWD_SIZE, cwd_tmp))
#else
      if (!getcwd (cwd_tmp, CWD_SIZE))
#endif
	{
	  LW6SYS_FREE (sys_context, cwd_tmp);
	  cwd_tmp = NULL;
	}
    }

  if (cwd_tmp)
    {
      cwd = lw6sys_path_strip_slash (sys_context, cwd_tmp);
      LW6SYS_FREE (sys_context, cwd_tmp);
    }
  else
    {
      cwd = lw6sys_str_copy (sys_context, ".");
    }

  return cwd;
}

/**
 * lw6sys_get_default_user_dir
 *
 * @sys_context: global system context
 *
 * Returns the default user directory. Note that this value
 * is not static, it can depend, for instance, of the environment
 * variable @HOME.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_user_dir (lw6sys_context_t * sys_context)
{
  char *home_dir = NULL;
  char *user_dir = NULL;
  char *relative = NULL;

  home_dir = lw6sys_get_home (sys_context);
  if (home_dir)
    {
#ifdef LW6_MS_WINDOWS
      relative = lw6sys_str_copy (sys_context, lw6sys_build_get_package_name ());
#else // LW6_MS_WINDOWS
#ifdef LW6_MAC_OS_X
      relative = lw6sys_path_concat (sys_context, APPLICATION_SUPPORT, lw6sys_build_get_package_name ());
#else // LW6_MAC_OS_X
      relative = lw6sys_str_concat (sys_context, ".", lw6sys_build_get_package_tarname ());
#endif // LW6_MAC_OS_X
#endif // LW6_MS_WINDOWS
      if (relative)
	{
	  user_dir = lw6sys_path_concat (sys_context, home_dir, relative);
	  LW6SYS_FREE (sys_context, relative);
	}
      LW6SYS_FREE (sys_context, home_dir);
    }

  return user_dir;
}

static char *
_get_config_file_with_user_dir (lw6sys_context_t * sys_context, char *user_dir)
{
  char *config_file;

  config_file = lw6sys_path_concat (sys_context, user_dir, CONFIG_FILE);

  return config_file;
}

/**
 * lw6sys_get_default_config_file
 *
 * @sys_context: global system context
 *
 * Returns the default config file. Note that this value
 * is not static, it can depend, for instance, of the environment
 * variable @HOME.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_config_file (lw6sys_context_t * sys_context)
{
  char *config_file = NULL;
  char *user_dir = NULL;

  user_dir = lw6sys_get_default_user_dir (sys_context);
  if (user_dir)
    {
      config_file = _get_config_file_with_user_dir (sys_context, user_dir);
      LW6SYS_FREE (sys_context, user_dir);
    }

  return config_file;
}

static char *
_get_log_file_with_user_dir (lw6sys_context_t * sys_context, char *user_dir)
{
  char *log_file;

  log_file = lw6sys_path_concat (sys_context, user_dir, LOG_FILE);

  return log_file;
}

/**
 * lw6sys_get_default_log_file
 *
 * @sys_context: global system context
 *
 * Returns the default log file. Note that this value
 * is not static, it can depend, for instance, of the environment
 * variable @HOME.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_log_file (lw6sys_context_t * sys_context)
{
  char *log_file = NULL;
  char *user_dir = NULL;

  user_dir = lw6sys_get_default_user_dir (sys_context);
  if (user_dir)
    {
      log_file = _get_log_file_with_user_dir (sys_context, user_dir);
      LW6SYS_FREE (sys_context, user_dir);
    }

  return log_file;
}

/**
 * lw6sys_get_default_prefix
 *
 * @sys_context: global system context
 *
 * Returns the default prefix, could be /usr/local for instance.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_prefix (lw6sys_context_t * sys_context)
{
  char *prefix = NULL;

  prefix = lw6sys_path_strip_slash (sys_context, lw6sys_build_get_prefix ());

  return prefix;
}

static char *
_get_dir_common (lw6sys_context_t * sys_context, char *mask1, char *prefix1, char *mask2, char *prefix2, char *sub, int check_readme)
{
  char *system_dir = NULL;
  char *abs_srcdir = NULL;
  char *dir = NULL;
  int dir_exists = 0;

  if (dir == NULL)
    {
      system_dir = lw6sys_new_sprintf (sys_context, mask1, prefix1, lw6sys_build_get_package_tarname (), lw6sys_build_get_version_base ());
      if (system_dir)
	{
	  dir = lw6sys_path_concat (sys_context, system_dir, sub);
	  if (dir)
	    {
	      dir_exists =
		check_readme ?
		lw6sys_dir_exists_with_readme_containing_text (sys_context, dir, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, dir);
	      if (!dir_exists)
		{
		  // directory doesn't exist, we ignore it
		  LW6SYS_FREE (sys_context, dir);
		  dir = NULL;
		}
	    }
	  LW6SYS_FREE (sys_context, system_dir);
	}
    }

  if (dir == NULL)
    {
      system_dir = lw6sys_new_sprintf (sys_context, mask2, prefix2, lw6sys_build_get_package_tarname (), lw6sys_build_get_version_base ());
      if (system_dir)
	{
	  dir = lw6sys_path_concat (sys_context, system_dir, sub);
	  if (dir)
	    {
	      dir_exists =
		check_readme ?
		lw6sys_dir_exists_with_readme_containing_text (sys_context, dir, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, dir);
	      if (!dir_exists)
		{
		  // directory doesn't exist, we ignore it
		  LW6SYS_FREE (sys_context, dir);
		  dir = NULL;
		}
	    }
	  LW6SYS_FREE (sys_context, system_dir);
	}
    }

  if (dir == NULL && strlen (sub) > 0)
    {
      abs_srcdir = lw6sys_build_get_abs_srcdir ();
      dir = lw6sys_path_concat (sys_context, abs_srcdir, sub);
      if (dir)
	{
	  dir_exists =
	    check_readme ?
	    lw6sys_dir_exists_with_readme_containing_text (sys_context, dir, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, dir);
	  if (!dir_exists)
	    {
	      // directory doesn't exist, we ignore it
	      LW6SYS_FREE (sys_context, dir);
	      dir = NULL;
	    }
	}
    }

  return dir;
}

static void
_dir_if_not_found (lw6sys_context_t * sys_context, char **dir, const char *sub, int check_readme)
{
  char *tmp = NULL;
  char *parent = NULL;
  int dir_exists = 0;
  int i = 0;

  if (dir && *dir)
    {
      dir_exists =
	check_readme ? lw6sys_dir_exists_with_readme_containing_text (sys_context, *dir, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, *dir);
      if (!dir_exists)
	{
	  parent = lw6sys_path_parent (sys_context, ".");
	  for (i = _DIR_IF_NOT_FOUND_NB_PARENTS; i > 0; --i)
	    {
	      if (parent)
		{
		  tmp = lw6sys_path_concat (sys_context, parent, sub);
		  dir_exists =
		    check_readme ?
		    lw6sys_dir_exists_with_readme_containing_text (sys_context, tmp, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, tmp);
		  if (dir_exists)
		    {
		      LW6SYS_FREE (sys_context, *dir);
		      (*dir) = tmp;
		      i = 0;
		    }
		  else
		    {
		      LW6SYS_FREE (sys_context, tmp);
		    }
		  tmp = lw6sys_path_parent (sys_context, parent);
		  LW6SYS_FREE (sys_context, parent);
		  parent = tmp;
		}
	    }
	  if (parent)
	    {
	      LW6SYS_FREE (sys_context, parent);
	      parent = NULL;
	    }

	  if (!lw6sys_dir_exists (sys_context, *dir))
	    {
	      tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_id (), sub);
	      if (tmp)
		{
		  dir_exists =
		    check_readme ?
		    lw6sys_dir_exists_with_readme_containing_text (sys_context, tmp, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, tmp);
		  if (dir_exists)
		    {
		      LW6SYS_FREE (sys_context, *dir);
		      (*dir) = tmp;
		    }
		  else
		    {
		      LW6SYS_FREE (sys_context, tmp);
		    }
		}
	      tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_string (), sub);
	      if (tmp)
		{
		  dir_exists =
		    check_readme ?
		    lw6sys_dir_exists_with_readme_containing_text (sys_context, tmp, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, tmp);
		  if (dir_exists)
		    {
		      LW6SYS_FREE (sys_context, *dir);
		      (*dir) = tmp;
		    }
		  else
		    {
		      LW6SYS_FREE (sys_context, tmp);
		    }
		}
	      tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_tarname (), sub);
	      if (tmp)
		{
		  dir_exists =
		    check_readme ?
		    lw6sys_dir_exists_with_readme_containing_text (sys_context, tmp, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, tmp);
		  if (dir_exists)
		    {
		      LW6SYS_FREE (sys_context, *dir);
		      (*dir) = tmp;
		    }
		  else
		    {
		      LW6SYS_FREE (sys_context, tmp);
		    }
		}
	      tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_name (), sub);
	      if (tmp)
		{
		  dir_exists =
		    check_readme ?
		    lw6sys_dir_exists_with_readme_containing_text (sys_context, tmp, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, tmp);
		  if (dir_exists)
		    {
		      LW6SYS_FREE (sys_context, *dir);
		      (*dir) = tmp;
		    }
		  else
		    {
		      LW6SYS_FREE (sys_context, tmp);
		    }
		}
	    }
	}
    }
}

static char *
_get_dir (lw6sys_context_t * sys_context, char *mask1, char *prefix1, char *mask2, char *prefix2, char *sub, int check_readme)
{
  char *dir = NULL;

  dir = _get_dir_common (sys_context, mask1, prefix1, mask2, prefix2, sub, check_readme);

  if (dir == NULL)
    {
      // if not installed, if no source, then search in local directory
      dir = lw6sys_path_concat (sys_context, ".", sub);
    }

  _dir_if_not_found (sys_context, &dir, sub, check_readme);

  return dir;
}

static char *
_get_dir_argc_argv (lw6sys_context_t * sys_context, int argc,
		    const char *argv[], char *mask1, char *prefix1, char *mask2, char *prefix2, char *sub, int check_readme)
{
  char *dir = NULL;
  char *run_dir = NULL;
  char *run_dir_rel = NULL;
  int dir_exists = 0;

  dir = _get_dir_common (sys_context, mask1, prefix1, mask2, prefix2, sub, check_readme);

  if (dir == NULL && strlen (sub) > 0)
    {
      run_dir = lw6sys_get_run_dir (sys_context, argc, argv);
      if (run_dir)
	{
	  dir = lw6sys_path_concat (sys_context, run_dir, sub);
	  if (dir)
	    {
	      dir_exists =
		check_readme ?
		lw6sys_dir_exists_with_readme_containing_text (sys_context, dir, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, dir);
	      if (!dir_exists)
		{
		  // directory doesn't exist, we ignore it
		  LW6SYS_FREE (sys_context, dir);
		  dir = NULL;

		  if (RUN_REL && strlen (RUN_REL))
		    {
		      run_dir_rel = lw6sys_path_concat (sys_context, run_dir, RUN_REL);
		      if (run_dir_rel)
			{
			  if (lw6sys_dir_exists (sys_context, run_dir_rel))
			    {
			      dir = lw6sys_path_concat (sys_context, run_dir_rel, sub);
			      if (dir)
				{
				  dir_exists =
				    check_readme ?
				    lw6sys_dir_exists_with_readme_containing_text
				    (sys_context, dir, lw6sys_build_get_home_url ()) : lw6sys_dir_exists (sys_context, dir);
				  if (!dir_exists)
				    {
				      LW6SYS_FREE (sys_context, dir);
				      dir = NULL;
				      // directory doesn't exist, we ignore it
				    }
				}
			    }
			  LW6SYS_FREE (sys_context, run_dir_rel);
			}
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, run_dir);
	}
    }

  if (dir == NULL)
    {
      // if not installed, if no source, then search in local directory
      dir = lw6sys_path_concat (sys_context, ".", sub);
    }

  _dir_if_not_found (sys_context, &dir, sub, check_readme);

  return dir;
}

static char *
_get_file_common (lw6sys_context_t * sys_context, char *mask1, char *prefix1, char *mask2, char *prefix2, char *sub)
{
  char *system_dir = NULL;
  char *abs_srcdir = NULL;
  char *file = NULL;

  if (file == NULL)
    {
      system_dir = lw6sys_new_sprintf (sys_context, mask1, prefix1, lw6sys_build_get_package_tarname (), lw6sys_build_get_version_base ());
      if (system_dir)
	{
	  file = lw6sys_path_concat (sys_context, system_dir, sub);
	  if (file)
	    {
	      if (!lw6sys_file_exists (sys_context, file))
		{
		  // file doesn't exist, we ignore it
		  LW6SYS_FREE (sys_context, file);
		  file = NULL;
		}
	    }
	  LW6SYS_FREE (sys_context, system_dir);
	}
    }

  if (file == NULL)
    {
      system_dir = lw6sys_new_sprintf (sys_context, mask2, prefix2, lw6sys_build_get_package_tarname (), lw6sys_build_get_version_base ());
      if (system_dir)
	{
	  file = lw6sys_path_concat (sys_context, system_dir, sub);
	  if (file)
	    {
	      if (!lw6sys_file_exists (sys_context, file))
		{
		  // file doesn't exist, we ignore it
		  LW6SYS_FREE (sys_context, file);
		  file = NULL;
		}
	    }
	  LW6SYS_FREE (sys_context, system_dir);
	}
    }

  if (file == NULL && strlen (sub) > 0)
    {
      abs_srcdir = lw6sys_build_get_abs_srcdir ();
      file = lw6sys_path_concat (sys_context, abs_srcdir, sub);
      if (file)
	{
	  if (!lw6sys_file_exists (sys_context, file))
	    {
	      // file doesn't exist, we ignore it
	      LW6SYS_FREE (sys_context, file);
	      file = NULL;
	    }
	}
    }

  return file;
}

static void
_file_if_not_found (lw6sys_context_t * sys_context, char **file, const char *sub)
{
  char *tmp = NULL;
  char *parent = NULL;
  int i = 0;

  if (file && *file && !lw6sys_file_exists (sys_context, *file))
    {
      parent = lw6sys_path_parent (sys_context, ".");
      for (i = _FILE_IF_NOT_FOUND_NB_PARENTS; i > 0; --i)
	{
	  if (parent)
	    {
	      tmp = lw6sys_path_concat (sys_context, parent, sub);
	      if (lw6sys_file_exists (sys_context, tmp))
		{
		  LW6SYS_FREE (sys_context, *file);
		  (*file) = tmp;
		  i = 0;
		}
	      else
		{
		  LW6SYS_FREE (sys_context, tmp);
		}
	      tmp = lw6sys_path_parent (sys_context, parent);
	      LW6SYS_FREE (sys_context, parent);
	      parent = tmp;
	    }
	}
      if (parent)
	{
	  LW6SYS_FREE (sys_context, parent);
	  parent = NULL;
	}
      if (!lw6sys_file_exists (sys_context, *file))
	{
	  tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_id (), sub);
	  if (tmp)
	    {
	      if (lw6sys_file_exists (sys_context, tmp))
		{
		  LW6SYS_FREE (sys_context, *file);
		  (*file) = tmp;
		}
	      else
		{
		  LW6SYS_FREE (sys_context, tmp);
		}
	    }
	  tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_string (), sub);
	  if (tmp)
	    {
	      if (lw6sys_file_exists (sys_context, tmp))
		{
		  LW6SYS_FREE (sys_context, *file);
		  (*file) = tmp;
		}
	      else
		{
		  LW6SYS_FREE (sys_context, tmp);
		}
	    }
	  tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_tarname (), sub);
	  if (tmp)
	    {
	      if (lw6sys_file_exists (sys_context, tmp))
		{
		  LW6SYS_FREE (sys_context, *file);
		  (*file) = tmp;
		}
	      else
		{
		  LW6SYS_FREE (sys_context, tmp);
		}
	    }
	  tmp = lw6sys_path_concat (sys_context, lw6sys_build_get_package_name (), sub);
	  if (tmp)
	    {
	      if (lw6sys_file_exists (sys_context, tmp))
		{
		  LW6SYS_FREE (sys_context, *file);
		  (*file) = tmp;
		}
	      else
		{
		  LW6SYS_FREE (sys_context, tmp);
		}
	    }
	}
    }
}

static char *
_get_file (lw6sys_context_t * sys_context, char *mask1, char *prefix1, char *mask2, char *prefix2, char *sub)
{
  char *file = NULL;

  file = _get_file_common (sys_context, mask1, prefix1, mask2, prefix2, sub);

  if (file == NULL)
    {
      // if not installed, if no source, then search in local directory
      file = lw6sys_path_concat (sys_context, ".", sub);
    }

  _file_if_not_found (sys_context, &file, sub);

  return file;
}

static char *
_get_file_argc_argv (lw6sys_context_t * sys_context, int argc, const char *argv[], char *mask1, char *prefix1, char *mask2, char *prefix2, char *sub)
{
  char *file = NULL;
  char *run_dir = NULL;
  char *run_dir_rel = NULL;

  file = _get_file_common (sys_context, mask1, prefix1, mask2, prefix2, sub);

  if (file == NULL && strlen (sub) > 0)
    {
      run_dir = lw6sys_get_run_dir (sys_context, argc, argv);
      if (run_dir)
	{
	  file = lw6sys_path_concat (sys_context, run_dir, sub);
	  if (file)
	    {
	      if (!lw6sys_file_exists (sys_context, file))
		{
		  LW6SYS_FREE (sys_context, file);
		  file = NULL;
		  // file doesn't exist, we ignore it

		  if (RUN_REL && strlen (RUN_REL))
		    {
		      run_dir_rel = lw6sys_path_concat (sys_context, run_dir, RUN_REL);
		      if (run_dir_rel)
			{
			  if (lw6sys_dir_exists (sys_context, run_dir_rel))
			    {
			      file = lw6sys_path_concat (sys_context, run_dir_rel, sub);
			      if (file)
				{
				  if (!lw6sys_file_exists (sys_context, file))
				    {
				      LW6SYS_FREE (sys_context, file);
				      file = NULL;
				      // file doesn't exist, we ignore it
				    }
				}
			    }
			  LW6SYS_FREE (sys_context, run_dir_rel);
			}
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, run_dir);
	}
    }

  if (file == NULL)
    {
      // if not installed, if no source, then search in local directory
      file = lw6sys_path_concat (sys_context, ".", sub);
    }

  _file_if_not_found (sys_context, &file, sub);

  return file;
}

static char *
_get_mod_dir_with_prefix (lw6sys_context_t * sys_context, char *prefix)
{
  char *mod_dir = NULL;
  char *libdir = NULL;

  libdir = lw6sys_build_get_libdir ();
  mod_dir = _get_dir (sys_context, MOD_PREFIX_MASK, prefix, MOD_LIBDIR_MASK, libdir, "", 0);

  return mod_dir;
}

static char *
_get_mod_dir_with_prefix_argc_argc (lw6sys_context_t * sys_context, int argc, const char *argv[], char *prefix)
{
  char *mod_dir = NULL;
  char *libdir = NULL;

  libdir = lw6sys_build_get_libdir ();
  mod_dir = _get_dir_argc_argv (sys_context, argc, argv, MOD_PREFIX_MASK, prefix, MOD_LIBDIR_MASK, libdir, "", 0);

  return mod_dir;
}

/**
 * lw6sys_get_default_mod_dir
 *
 * @sys_context: global system context
 *
 * Returns the default module directory (dynamically loaded libraries).
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_mod_dir (lw6sys_context_t * sys_context)
{
  char *mod_dir = NULL;
  char *prefix = NULL;

  prefix = lw6sys_get_default_prefix (sys_context);
  if (prefix)
    {
      mod_dir = _get_mod_dir_with_prefix (sys_context, prefix);
      LW6SYS_FREE (sys_context, prefix);
    }

  return mod_dir;
}

static char *
_get_share_dir_with_prefix (lw6sys_context_t * sys_context, char *prefix, char *sub, int check_readme)
{
  char *share_dir = NULL;
  char *datadir = NULL;

  datadir = lw6sys_build_get_datadir ();
  share_dir = _get_dir (sys_context, SHARE_PREFIX_MASK, prefix, SHARE_DATADIR_MASK, datadir, sub, check_readme);

  return share_dir;
}

static char *
_get_share_dir_with_prefix_argc_argv (lw6sys_context_t * sys_context, int argc, const char *argv[], char *prefix, char *sub, int check_readme)
{
  char *share_dir = NULL;
  char *datadir = NULL;

  datadir = lw6sys_build_get_datadir ();
  share_dir = _get_dir_argc_argv (sys_context, argc, argv, SHARE_PREFIX_MASK, prefix, SHARE_DATADIR_MASK, datadir, sub, check_readme);

  return share_dir;
}

/**
 * lw6sys_get_default_data_dir
 *
 * @sys_context: global system context
 *
 * Returns the default data directory.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_data_dir (lw6sys_context_t * sys_context)
{
  char *data_dir = NULL;
  char *prefix = NULL;

  prefix = lw6sys_get_default_prefix (sys_context);
  if (prefix)
    {
      data_dir = _get_share_dir_with_prefix (sys_context, prefix, DATA_DIR, 1);
      LW6SYS_FREE (sys_context, prefix);
    }

  return data_dir;
}

static char *
_get_path_with_dir_and_user_dir (lw6sys_context_t * sys_context, char *dir, char *user_dir, char *sub, int create_dir, char *readme_comment)
{
  char *path = NULL;
  char *user_sub_dir = NULL;
  char *user_sub_dir_readme = NULL;
  char *readme_content = NULL;

  user_sub_dir = lw6sys_path_concat (sys_context, user_dir, sub);
  if (user_sub_dir)
    {
      if (create_dir)
	{
	  if (!lw6sys_dir_exists (sys_context, user_sub_dir))
	    {
	      lw6sys_create_dir (sys_context, user_sub_dir);
	    }
	  user_sub_dir_readme = lw6sys_path_concat (sys_context, user_sub_dir, README_FILE);
	  if (user_sub_dir_readme)
	    {
	      if (readme_comment)
		{
		  readme_content = lw6sys_str_reformat (sys_context, readme_comment, LW6SYS_REFORMAT_DEFAULT_PREFIX, LW6SYS_REFORMAT_DEFAULT_NB_COLUMNS);
		  if (readme_content)
		    {
		      lw6sys_write_file_content (sys_context, user_sub_dir_readme, readme_content);
		      LW6SYS_FREE (sys_context, readme_content);
		    }
		}
	      LW6SYS_FREE (sys_context, user_sub_dir_readme);
	    }
	}
      path = lw6sys_env_concat (sys_context, user_sub_dir, dir);
      LW6SYS_FREE (sys_context, user_sub_dir);
    }

  return path;
}

/**
 * lw6sys_get_default_music_dir
 *
 * @sys_context: global system context
 *
 * Returns the default music directory.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_music_dir (lw6sys_context_t * sys_context)
{
  char *music_dir = NULL;
  char *prefix = NULL;

  prefix = lw6sys_get_default_prefix (sys_context);
  if (prefix)
    {
      music_dir = _get_share_dir_with_prefix (sys_context, prefix, MUSIC_DIR, 1);
      LW6SYS_FREE (sys_context, prefix);
    }

  return music_dir;
}

/**
 * lw6sys_get_default_music_path
 *
 * @sys_context: global system context
 *
 * Returns the default music path, which can be composed of several directories.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_music_path (lw6sys_context_t * sys_context)
{
  char *music_path = NULL;
  char *music_dir = NULL;
  char *user_dir = NULL;

  music_dir = lw6sys_get_default_music_dir (sys_context);
  if (music_dir)
    {
      user_dir = lw6sys_get_default_user_dir (sys_context);
      if (user_dir)
	{
	  music_path = _get_path_with_dir_and_user_dir (sys_context, music_dir, user_dir, MUSIC_DIR, 0, NULL);
	  LW6SYS_FREE (sys_context, user_dir);
	}
      LW6SYS_FREE (sys_context, music_dir);
    }

  return music_path;
}

/**
 * lw6sys_get_default_map_dir
 *
 * @sys_context: global system context
 *
 * Returns the default map directory.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_map_dir (lw6sys_context_t * sys_context)
{
  char *map_dir = NULL;
  char *prefix = NULL;

  prefix = lw6sys_get_default_prefix (sys_context);
  if (prefix)
    {
      map_dir = _get_share_dir_with_prefix (sys_context, prefix, MAP_DIR, 1);
      LW6SYS_FREE (sys_context, prefix);
    }

  return map_dir;
}

/**
 * lw6sys_get_default_map_path
 *
 * @sys_context: global system context
 *
 * Returns the default map path, which can be composed of several directories.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_map_path (lw6sys_context_t * sys_context)
{
  char *map_path = NULL;
  char *map_dir = NULL;
  char *user_dir = NULL;

  map_dir = lw6sys_get_default_map_dir (sys_context);
  if (map_dir)
    {
      user_dir = lw6sys_get_default_user_dir (sys_context);
      if (user_dir)
	{
	  map_path = _get_path_with_dir_and_user_dir (sys_context, map_dir, user_dir, MAP_DIR, 0, NULL);
	  LW6SYS_FREE (sys_context, user_dir);
	}
      LW6SYS_FREE (sys_context, map_dir);
    }

  return map_path;
}

static char *
_get_share_file_with_prefix (lw6sys_context_t * sys_context, char *prefix, char *sub)
{
  char *share_file = NULL;
  char *datadir;

  datadir = lw6sys_build_get_datadir ();
  share_file = _get_file (sys_context, SHARE_PREFIX_MASK, prefix, SHARE_DATADIR_MASK, datadir, sub);

  return share_file;
}

static char *
_get_share_file_with_prefix_argc_argv (lw6sys_context_t * sys_context, int argc, const char *argv[], char *prefix, char *sub)
{
  char *share_file = NULL;
  char *datadir;

  datadir = lw6sys_build_get_datadir ();
  share_file = _get_file_argc_argv (sys_context, argc, argv, SHARE_PREFIX_MASK, prefix, SHARE_DATADIR_MASK, datadir, sub);

  return share_file;
}

/**
 * lw6sys_get_default_script_file
 *
 * @sys_context: global system context
 *
 * Returns the default script file.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_default_script_file (lw6sys_context_t * sys_context)
{
  char *script_file = NULL;
  char *prefix = NULL;

  prefix = lw6sys_get_default_prefix (sys_context);
  if (prefix)
    {
      script_file = _get_share_file_with_prefix (sys_context, prefix, SCRIPT_FILE);
      LW6SYS_FREE (sys_context, prefix);
    }

  return script_file;
}

/**
 * lw6sys_options_log_defaults
 *
 * @sys_context: global system context
 *
 * Logs all default values to log file. Usefull for debugging, to
 * know where the program is searching for its informations.
 */
void
lw6sys_options_log_defaults (lw6sys_context_t * sys_context)
{
  char *path = NULL;

  path = lw6sys_get_default_user_dir (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default user dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_config_file (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default config file is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_log_file (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default log file is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_prefix (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default prefix is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_mod_dir (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default mod dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_data_dir (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default data dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_music_dir (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default music dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_music_path (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default music path is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_map_dir (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default map dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_map_path (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default map path is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_default_script_file (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("default script file is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
}

static void *
get_command_arg_with_env (lw6sys_context_t * sys_context, int argc, const char *argv[], char *keyword)
{
  char *arg_value = NULL;
  char *ret = NULL;

  arg_value = lw6sys_arg_get_value_with_env (sys_context, argc, argv, keyword);
  if (arg_value)
    {
      ret = lw6sys_path_strip_slash (sys_context, arg_value);
      LW6SYS_FREE (sys_context, arg_value);
    }

  return ret;
}

/**
 * lw6sys_get_run_dir
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the binary directory, that is, the directory the binary
 * is stored in. This is calculated dynamically, by interpreting
 * command-line arguments.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_run_dir (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *run_dir = NULL;

  if (argc >= 1 && argv[0] != NULL)
    {
      run_dir = lw6sys_path_parent (sys_context, argv[0]);
    }
  else
    {
      run_dir = lw6sys_str_copy (sys_context, ".");
    }

  return run_dir;
}

/**
 * lw6sys_get_user_dir
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the user dir, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_user_dir (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *user_dir = NULL;

  user_dir = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_USER_DIR);
  if (user_dir == NULL)
    {
      user_dir = lw6sys_get_default_user_dir (sys_context);
    }

  if (user_dir)
    {
      if (!lw6sys_dir_exists (sys_context, user_dir))
	{
	  lw6sys_create_dir_silent (sys_context, user_dir);
	  if (!lw6sys_dir_exists (sys_context, user_dir))
	    {
	      lw6sys_log_critical (sys_context, _x_ ("can't open user dir \"%s\""), user_dir);
	    }
	}
    }

  return user_dir;
}

/**
 * lw6sys_get_config_file
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the config file, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_config_file (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *config_file = NULL;
  char *user_dir = NULL;

  config_file = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_CONFIG_FILE);
  if (config_file == NULL)
    {
      user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
      if (user_dir)
	{
	  config_file = _get_config_file_with_user_dir (sys_context, user_dir);
	  LW6SYS_FREE (sys_context, user_dir);
	}
    }

  return config_file;
}

/**
 * lw6sys_get_log_file
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the log file, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_log_file (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *log_file = NULL;
  char *user_dir = NULL;

  log_file = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_LOG_FILE);
  if (log_file == NULL)
    {
      user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
      if (user_dir)
	{
	  log_file = _get_log_file_with_user_dir (sys_context, user_dir);
	  LW6SYS_FREE (sys_context, user_dir);
	}
    }

  return log_file;
}

/**
 * lw6sys_get_prefix
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the prefix, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_prefix (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *prefix = NULL;

  prefix = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_PREFIX);
  if (prefix == NULL)
    {
      prefix = lw6sys_get_default_prefix (sys_context);
    }

  return prefix;
}

/**
 * lw6sys_get_mod_dir
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the mod dir (modules, shared .so),
 * taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_mod_dir (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *mod_dir = NULL;
  char *prefix = NULL;

  mod_dir = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_MOD_DIR);
  if (mod_dir == NULL)
    {
      prefix = lw6sys_get_prefix (sys_context, argc, argv);
      if (prefix)
	{
	  mod_dir = _get_mod_dir_with_prefix_argc_argc (sys_context, argc, argv, prefix);
	  LW6SYS_FREE (sys_context, prefix);
	}
    }

  return mod_dir;
}

/**
 * lw6sys_get_data_dir
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the data dir, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_data_dir (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *data_dir = NULL;
  char *prefix = NULL;

  data_dir = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_DATA_DIR);
  if (data_dir == NULL)
    {
      prefix = lw6sys_get_prefix (sys_context, argc, argv);
      if (prefix)
	{
	  data_dir = _get_share_dir_with_prefix_argc_argv (sys_context, argc, argv, prefix, DATA_DIR, 1);
	  LW6SYS_FREE (sys_context, prefix);
	}
    }

  return data_dir;
}

/**
 * lw6sys_get_music_dir
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the music dir, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_music_dir (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *music_dir = NULL;
  char *prefix = NULL;

  music_dir = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_MUSIC_DIR);
  if (music_dir == NULL)
    {
      prefix = lw6sys_get_prefix (sys_context, argc, argv);
      if (prefix)
	{
	  music_dir = _get_share_dir_with_prefix_argc_argv (sys_context, argc, argv, prefix, MUSIC_DIR, 1);
	  LW6SYS_FREE (sys_context, prefix);
	}
    }

  return music_dir;
}

/**
 * lw6sys_get_music_path
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the music path, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result. Music path can contain several directories.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_music_path (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *music_path = NULL;
  char *music_path_cmd = NULL;
  char *music_path_env = NULL;
  char *music_dir = NULL;
  char *user_dir = NULL;
  char *tmp = NULL;

  music_dir = lw6sys_get_music_dir (sys_context, argc, argv);
  if (music_dir)
    {
      user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
      if (user_dir)
	{
	  music_path =
	    _get_path_with_dir_and_user_dir (sys_context, music_dir, user_dir,
					     MUSIC_DIR, 1,
					     _x_
					     ("This is your Liquid War 6 user music directory. You can put your own musics here, they will be automatically loaded by the game, and played."));
	  if (music_path)
	    {
	      music_path_env = lw6sys_getenv_prefixed (sys_context, LW6DEF_MUSIC_PATH);
	      if (music_path && music_path_env)
		{
		  tmp = music_path;
		  music_path = lw6sys_env_concat (sys_context, music_path_env, music_path);
		  LW6SYS_FREE (sys_context, tmp);
		  LW6SYS_FREE (sys_context, music_path_env);
		}
	      music_path_cmd = lw6sys_arg_get_value (sys_context, argc, argv, LW6DEF_MUSIC_PATH);
	      if (music_path && music_path_cmd)
		{
		  tmp = music_path;
		  music_path = lw6sys_env_concat (sys_context, music_path_cmd, music_path);
		  LW6SYS_FREE (sys_context, tmp);
		  LW6SYS_FREE (sys_context, music_path_cmd);
		}
	    }
	  LW6SYS_FREE (sys_context, user_dir);
	}
      LW6SYS_FREE (sys_context, music_dir);
    }

  return music_path;
}

/**
 * lw6sys_get_map_dir
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the map dir, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_map_dir (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *map_dir = NULL;
  char *prefix = NULL;

  map_dir = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_MAP_DIR);
  if (map_dir == NULL)
    {
      prefix = lw6sys_get_prefix (sys_context, argc, argv);
      if (prefix)
	{
	  map_dir = _get_share_dir_with_prefix_argc_argv (sys_context, argc, argv, prefix, MAP_DIR, 1);
	  LW6SYS_FREE (sys_context, prefix);
	}
    }

  return map_dir;
}

/**
 * lw6sys_get_map_path
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the map path, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result. Map path can contain several directories.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_map_path (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *map_path = NULL;
  char *map_path_cmd = NULL;
  char *map_path_env = NULL;
  char *map_dir = NULL;
  char *user_dir = NULL;
  char *tmp = NULL;

  map_dir = lw6sys_get_map_dir (sys_context, argc, argv);
  if (map_dir)
    {
      user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
      if (user_dir)
	{
	  map_path =
	    _get_path_with_dir_and_user_dir (sys_context, map_dir, user_dir,
					     MAP_DIR, 1,
					     _x_
					     ("This is your Liquid War 6 user map directory. You can put your own maps here, they will be automatically loaded by the game, and appear in the menus. To create maps, you need to 1) read the documentation 2) study existing map to see how things work in practice and 3) be creative. FYI the Liquid War 6 map format is rather simple and hopefully well documented, the general idea is that a map is a directory containing files, the most important of them being 'map.png'. Again, read the documentation. I repeat: 'Read the docs!'. http://www.gnu.org/software/liquidwar6/manual/"));
	  if (map_path)
	    {
	      map_path_env = lw6sys_getenv_prefixed (sys_context, LW6DEF_MAP_PATH);
	      if (map_path && map_path_env)
		{
		  tmp = map_path;
		  map_path = lw6sys_env_concat (sys_context, map_path_env, map_path);
		  LW6SYS_FREE (sys_context, tmp);
		  LW6SYS_FREE (sys_context, map_path_env);
		}
	      map_path_cmd = lw6sys_arg_get_value (sys_context, argc, argv, LW6DEF_MAP_PATH);
	      if (map_path && map_path_cmd)
		{
		  tmp = map_path;
		  map_path = lw6sys_env_concat (sys_context, map_path_cmd, map_path);
		  LW6SYS_FREE (sys_context, tmp);
		  LW6SYS_FREE (sys_context, map_path_cmd);
		}
	    }
	  LW6SYS_FREE (sys_context, user_dir);
	}
      LW6SYS_FREE (sys_context, map_dir);
    }

  return map_path;
}

/**
 * lw6sys_get_script_file
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Returns the script file, taking in account command-line and
 * environment variables. However config file content has no impact
 * on the result.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_get_script_file (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *script_file = NULL;
  char *prefix = NULL;

  script_file = get_command_arg_with_env (sys_context, argc, argv, LW6DEF_SCRIPT_FILE);
  if (script_file == NULL)
    {
      prefix = lw6sys_get_prefix (sys_context, argc, argv);
      if (prefix)
	{
	  script_file = _get_share_file_with_prefix_argc_argv (sys_context, argc, argv, prefix, SCRIPT_FILE);
	  LW6SYS_FREE (sys_context, prefix);
	}
    }

  return script_file;
}

/**
 * lw6sys_options_log
 *
 * @sys_context: global system context
 * @argc: argc, number of arguments, as given to @main
 * @argv: argv, pointer to arguments, as given to @main
 *
 * Logs all the main options values which are not config-file
 * dependant but depend on built-in defaults, command-line arguments
 * and environment variables. Usefull to debug and know where the
 * program is searching for things.
 */
void
lw6sys_options_log (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *path = NULL;

  path = lw6sys_get_cwd (sys_context);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("cwd is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_run_dir (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("run dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_user_dir (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("user dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_config_file (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("config file is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_log_file (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("log file is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_prefix (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("prefix is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_mod_dir (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_data_dir (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("data dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_music_dir (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("music dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_music_path (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("music path is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_map_dir (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("map dir is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_map_path (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("map path is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_script_file (sys_context, argc, argv);
  if (path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("script file is \"%s\""), path);
      LW6SYS_FREE (sys_context, path);
    }
}
