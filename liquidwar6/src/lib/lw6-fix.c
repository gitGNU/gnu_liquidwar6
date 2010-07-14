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

#include "liquidwar6.h"

#define DYLD_LIBRARY_PATH "DYLD_LIBRARY_PATH"
#define LD_LIBRARY_PATH "LD_LIBRARY_PATH"

#if LW6_MS_WINDOWS || LW6_MAC_OS_X
#define GUILE_LOAD_PATH_KEY "GUILE_LOAD_PATH"
#if LW6_MS_WINDOWS
#define GUILE_LOAD_PATH_SUFFIX "guile"
#endif
#if LW6_MAC_OS_X
#define  GUILE_LOAD_PATH_SUFFIX "../Resources/guile"
#endif

static void
_fix_guile_load_path (int argc, char *argv[])
{
  char *run_dir = NULL;
  char *guile_dir = NULL;

  run_dir = lw6sys_get_run_dir (argc, argv);
  if (run_dir)
    {
      guile_dir = lw6sys_path_concat (run_dir, GUILE_LOAD_PATH_SUFFIX);
      if (guile_dir)
	{
	  if (lw6sys_dir_exists (guile_dir))
	    {
	      lw6sys_setenv (GUILE_LOAD_PATH_KEY, guile_dir);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _("Guile library directory \"%s\" does not exist"),
			  guile_dir);
	    }
	  LW6SYS_FREE (guile_dir);
	}
      LW6SYS_FREE (run_dir);
    }
}
#endif

#if LW6_MAC_OS_X
static void
_fix_library_path (int argc, char *argv[], char *library_path)
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

  cwd = lw6sys_get_cwd (argc, argv);
  if (cwd)
    {
      old_library_path = lw6sys_getenv (library_path);
      if (old_library_path && strlen (old_library_path) > 0)
	{
	  new_library_path =
	    lw6sys_env_concat (cwd, old_library_path);
	}
      else
	{
	  new_library_path = lw6sys_str_copy (cwd);
	}
      LW6SYS_FREE (cwd);
    }

  if (old_library_path)
    {
      LW6SYS_FREE (old_library_path);
      old_library_path = NULL;
    }
  if (new_library_path)
    {
      old_library_path = new_library_path;
      new_library_path = NULL;
    }

  run_dir = lw6sys_get_run_dir (argc, argv);
  if (run_dir)
    {
      if (old_library_path && strlen (old_library_path) > 0)
	{
	  new_library_path =
	    lw6sys_env_concat (run_dir, old_library_path);
	}
      else
	{
	  new_library_path = lw6sys_str_copy (run_dir);
	}
      LW6SYS_FREE (run_dir);
    }

  if (old_library_path)
    {
      LW6SYS_FREE (old_library_path);
      old_library_path = NULL;
    }
  if (new_library_path)
    {
      lw6sys_setenv (library_path, new_library_path);
      LW6SYS_FREE (new_library_path);
      new_library_path = NULL;
    }
}

static void
_fix_dyld_library_path (int argc, char *argv[])
{
  _fix_library_path(argc,argv,_DYLD_LIBRARY_PATH);
}

static void
_fix_ld_library_path (int argc, char *argv[])
{
  _fix_library_path(argc,argv,_LD_LIBRARY_PATH);
}
#endif

/**
 * lw6_fix_env
 *
 * argc: number of args as passed to main
 * argv: array of strings as passed to main
 *
 * Fixes environment variables (path related) so that
 * program can find its requirements. This must be
 * called early in the program flow (else other calls
 * might fail).
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6_fix_env (int argc, char *argv[])
{
  int ret = 1;

#if LW6_MS_WINDOWS || LW6_MAC_OS_X
  _fix_guile_load_path (argc, argv);
#endif
#ifdef LW6_MAC_OS_X
  _fix_dyld_library_path (argc, argv);
  _fix_ld_library_path (argc, argv);
#endif

  return ret;
}
