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

#define GUILE_LOAD_PATH_KEY "GUILE_LOAD_PATH"

#if LW6_MS_WINDOWS
#define GUILE_LOAD_PATH_SUFFIX "guile"
#endif

#if LW6_MAC_OS_X
#define GUILE_LOAD_PATH_SUFFIX "../Resources/guile"
#define DYLD_FALLBACK_LIBRARY_PATH "DYLD_FALLBACK_LIBRARY_PATH"
#define DYLD_LIBRARY_PATH "DYLD_LIBRARY_PATH"
#define LD_LIBRARY_PATH "LD_LIBRARY_PATH"
#define DEFAULT_MACPORTS_LIBDIR "/opt/local/lib"
#endif

static char *
_get_system_guile_load_path (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *ret = NULL;

#if LW6_MS_WINDOWS || LW6_MAC_OS_X
  char *run_dir = NULL;
  char *guile_dir = NULL;

  run_dir = lw6sys_get_run_dir (sys_context, argc, argv);
  if (run_dir)
    {
      guile_dir = lw6sys_path_concat (run_dir, GUILE_LOAD_PATH_SUFFIX);
      if (guile_dir)
	{
	  if (lw6sys_dir_exists (sys_context, guile_dir))
	    {
	      ret = guile_dir;	// will be freed by caller
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("Guile library directory \"%s\" does not exist"), guile_dir);
	      LW6SYS_FREE (sys_context, guile_dir);
	    }
	}
      LW6SYS_FREE (sys_context, run_dir);
    }
#endif

  return ret;
}

static void
_fix_guile_load_path (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *script_file = NULL;
  char *script_dir = NULL;
  char *system_dir = NULL;
  char *env = NULL;

  system_dir = _get_system_guile_load_path (sys_context, argc, argv);
  script_file = lw6sys_get_script_file (sys_context, argc, argv);
  if (script_file)
    {
      script_dir = lw6sys_path_parent (sys_context, script_file);
      if (script_dir)
	{
	  if (!lw6sys_dir_exists (sys_context, script_dir))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("Guile script directory \"%s\" does not exist"), script_dir);
	      LW6SYS_FREE (sys_context, script_dir);
	      script_dir = NULL;
	    }
	}
      LW6SYS_FREE (sys_context, script_file);
    }

  if (system_dir && script_dir)
    {
      env = lw6sys_new_sprintf (sys_context, "%s%c%s%c.", system_dir, lw6sys_env_separator_char (), script_dir, lw6sys_env_separator_char ());
    }
  else
    {
      if (system_dir)
	{
	  env = lw6sys_new_sprintf (sys_context, "%s%c.", system_dir, lw6sys_env_separator_char ());
	}
      else
	{
	  if (script_dir)
	    {
	      env = lw6sys_new_sprintf (sys_context, "%s%c.", script_dir, lw6sys_env_separator_char ());
	    }
	  else
	    {
	      env = lw6sys_str_copy (sys_context, ".");
	    }
	}
    }

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("setting Guile path using setenv %s=\"%s\""), GUILE_LOAD_PATH_KEY, env);
  if (env)
    {
      lw6sys_setenv (sys_context, GUILE_LOAD_PATH_KEY, env);
    }

  if (env)
    {
      LW6SYS_FREE (sys_context, env);
    }
  if (script_dir)
    {
      LW6SYS_FREE (sys_context, script_dir);
    }
  if (system_dir)
    {
      LW6SYS_FREE (sys_context, system_dir);
    }
}

#if LW6_MAC_OS_X
static void
_fix_library_path (lw6sys_context_t * sys_context, int argc, const char *argv[], char *library_path)
{
  /*
   * Fixes bug http://savannah.gnu.org/bugs/?30409
   * We need to force DYLD_LIBRARY_PATH to a value
   * that includes the distributed ./MacOS binary,
   * indeed install_name_tool can change hardcoded
   * references to libs but can't handle dynamic
   * loading (at run-time, "dlopening") which is
   * something SDL_image does, for instance. So we
   * tell the library loader to look into CWD and
   * run directory.
   */
  char *old_library_path = NULL;
  char *new_library_path = NULL;
  char *run_dir = NULL;
  char *cwd = NULL;

  /*
   * First, get old value and *append* program libdir to it
   */
  old_library_path = lw6sys_getenv (sys_context, library_path);
  if (old_library_path && strlen (old_library_path) > 0)
    {
      new_library_path = lw6sys_env_concat (sys_context, old_library_path, lw6sys_build_get_libdir (sys_context));
    }
  else
    {
      new_library_path = lw6sys_str_copy (sys_context, lw6sys_build_get_libdir ());
    }

  if (old_library_path)
    {
      LW6SYS_FREE (sys_context, old_library_path);
      old_library_path = NULL;
    }
  if (new_library_path)
    {
      old_library_path = new_library_path;
      new_library_path = NULL;
    }

  /*
   * Next, add default macports libdir *after* it
   */
  if (old_library_path && strlen (old_library_path) > 0)
    {
      new_library_path = lw6sys_env_concat (sys_context, old_library_path, DEFAULT_MACPORTS_LIBDIR);
    }
  else
    {
      new_library_path = lw6sys_str_copy (sys_context, sys_context, DEFAULT_MACPORTS_LIBDIR);
    }

  if (old_library_path)
    {
      LW6SYS_FREE (sys_context, old_library_path);
      old_library_path = NULL;
    }
  if (new_library_path)
    {
      old_library_path = new_library_path;
      new_library_path = NULL;
    }

  /*
   * Next, add CWD before it
   */
  cwd = lw6sys_get_cwd (sys_context, argc, argv);
  if (cwd)
    {
      if (old_library_path && strlen (old_library_path) > 0)
	{
	  new_library_path = lw6sys_env_concat (sys_context, cwd, old_library_path);
	}
      else
	{
	  new_library_path = lw6sys_str_copy (sys_context, cwd);
	}
      LW6SYS_FREE (sys_context, cwd);
    }

  if (old_library_path)
    {
      LW6SYS_FREE (sys_context, old_library_path);
      old_library_path = NULL;
    }
  if (new_library_path)
    {
      old_library_path = new_library_path;
      new_library_path = NULL;
    }

  /*
   * Finally, put run_dir in front
   */
  run_dir = lw6sys_get_run_dir (sys_context, argc, argv);
  if (run_dir)
    {
      if (old_library_path && strlen (old_library_path) > 0)
	{
	  new_library_path = lw6sys_env_concat (sys_context, run_dir, old_library_path);
	}
      else
	{
	  new_library_path = lw6sys_str_copy (sys_context, run_dir);
	}
      LW6SYS_FREE (sys_context, run_dir);
    }

  if (old_library_path)
    {
      LW6SYS_FREE (sys_context, old_library_path);
      old_library_path = NULL;
    }
  if (new_library_path)
    {
      lw6sys_setenv (sys_context, library_path, new_library_path);
      LW6SYS_FREE (sys_context, new_library_path);
      new_library_path = NULL;
    }
}

static void
_fix_dyld_fallback_library_path (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  _fix_library_path (sys_context, argc, argv, DYLD_FALLBACK_LIBRARY_PATH);
}

/*
static void
_fix_dyld_library_path (int argc, const char *argv[])
{
  _fix_library_path (argc, argv, DYLD_LIBRARY_PATH);
}

static void
_fix_ld_library_path (int argc, const char *argv[])
{
  _fix_library_path (argc, argv, LD_LIBRARY_PATH);
}
*/
#endif

/**
 * lw6_fix_env
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of strings as passed to main
 *
 * Fixes environment variables (path related) so that
 * program can find its requirements. This must be
 * called early in the program flow (else other calls
 * might fail).
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6_fix_env (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  int ret = 1;

  _fix_guile_load_path (sys_context, argc, argv);
#ifdef LW6_MAC_OS_X
  if (!lw6sys_is_executed_again (sys_context, argc, argv))
    {
      _fix_dyld_fallback_library_path (sys_context, argc, argv);
      //_fix_dyld_library_path (argc, argv);
      //_fix_ld_library_path (argc, argv);
      lw6sys_exec_again (sys_context, argc, argv);
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("couldn't fix environment variable by restarting program"));
    }
#endif

  return ret;
}
