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

#include <libguile.h>

#include "liquidwar6.h"

#define DYLD_LIBRARY_PATH "DYLD_LIBRARY_PATH"

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
_fix_dyld_library_path (int argc, char *argv[])
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
  char *old_dyld_library_path = NULL;
  char *new_dyld_library_path = NULL;
  char *run_dir = NULL;
  char *cwd = NULL;

  cwd = lw6sys_get_cwd (argc, argv);
  if (cwd)
    {
      old_dyld_library_path = lw6sys_getenv (DYLD_LIBRARY_PATH);
      if (old_dyld_library_path && strlen (old_dyld_library_path) > 0)
	{
	  new_dyld_library_path =
	    lw6sys_env_concat (cwd, old_dyld_library_path);
	}
      else
	{
	  new_dyld_library_path = lw6sys_str_copy (cwd);
	}
      LW6SYS_FREE (cwd);
    }

  if (old_dyld_library_path)
    {
      LW6SYS_FREE (old_dyld_library_path);
      old_dyld_library_path = NULL;
    }
  if (new_dyld_library_path)
    {
      old_dyld_library_path = new_dyld_library_path;
      new_dyld_library_path = NULL;
    }

  run_dir = lw6sys_get_run_dir (argc, argv);
  if (run_dir)
    {
      if (old_dyld_library_path && strlen (old_dyld_library_path) > 0)
	{
	  new_dyld_library_path =
	    lw6sys_env_concat (run_dir, old_dyld_library_path);
	}
      else
	{
	  new_dyld_library_path = lw6sys_str_copy (run_dir);
	}
      LW6SYS_FREE (run_dir);
    }

  if (old_dyld_library_path)
    {
      LW6SYS_FREE (old_dyld_library_path);
      old_dyld_library_path = NULL;
    }
  if (new_dyld_library_path)
    {
      lw6sys_setenv (DYLD_LIBRARY_PATH, new_dyld_library_path);
      LW6SYS_FREE (new_dyld_library_path);
      new_dyld_library_path = NULL;
    }
}
#endif

static void *
guile_main (void *data)
{
  char *script = NULL;

  SCM_DEVAL_P = 1;
  SCM_BACKTRACE_P = 1;
  SCM_RECORD_POSITIONS_P = 1;
  SCM_RESET_DEBUG_MODE;

  lw6sys_log (LW6SYS_LOG_INFO,
	      _("registering C types and functions for Guile"));
  lw6_register_smobs ();
  lw6_register_funcs ();

  script = lw6sys_get_script_file (lw6_global.argc, lw6_global.argv);
  if (script)
    {
      if (lw6sys_file_exists (script))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("loading \"%s\""), script);
	  scm_c_primitive_load (script);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_ERROR,
		      _("unable to read main script file \"%s\""), script);
	}
      LW6SYS_FREE (script);
    }

  scm_gc ();
  /*
   * It's important to call lw6_quit_global_1 only now, for in vthread
   * mode we need the possible dsp/gfx module cleanup to be in the
   * same thread than the caller (SDL limitation on Mac OS X).
   */
  lw6_quit_global_1 ();

  return NULL;
}

static void
_run (void *data)
{
  lw6sys_log (LW6SYS_LOG_DEBUG, _("main run data=%p"), data);
  scm_with_guile (guile_main, NULL);
}

static void
_end (void *data)
{
  lw6sys_log (LW6SYS_LOG_DEBUG, _("main end data=%p"), data);
}

/**
 * lw6_main:
 * 
 * @argc: the argc parameter of the @main() function, that is, the number
 *   of command-line args.
 * @argv: the argv parameter of the @main() function, that is, an array
 *   containing pointers on command-line args.
 *
 * This function is directly called by @main(). This means by linking
 * against libliquidwar6 and calling it, you would have a program
 * that is almost exactly the "official" upstream liquidwar6 binary,
 * except you can tweak it and have all the power to call whatever
 * other functions you like, embed it. In short, everything the binary
 * does, you can do it in your own binarn, by linking against the
 * library and calling this function.
 *
 * Return value: 1 if success, zero if failure. Note that this
 *   is the "standard" C / liquidwar6 way to proceed, but your @main()
 *   function should return 0 if success, else an error code. Typical
 *   use is "return !lw6_main(argc, argv);".
 */
int
lw6_main (int argc, char *argv[])
{
  int ret = 0;
  char *debug_str = NULL;
  int debug = 0;
  char *log_level_str = NULL;
  int log_level = 0;
  char *log_file = NULL;
  int run_game = 1;

  ret = lw6_process_non_run_options (argc, argv, &run_game);
  if (run_game)
    {
      debug_str = lw6sys_arg_get_value_with_env (argc, argv, LW6DEF_DEBUG);
      if (debug_str)
	{
	  debug = lw6sys_atob (debug_str);
	  lw6sys_debug_set (debug);
	  LW6SYS_FREE (debug_str);
	}
      log_level_str = lw6cfg_unified_get_value (argc, argv, LW6DEF_LOG_LEVEL);
      if (log_level_str)
	{
	  log_level = lw6sys_atoi (log_level_str);
	  lw6sys_log_set_level (log_level);
	  LW6SYS_FREE (log_level_str);
	}
      log_file = lw6cfg_unified_get_log_file (argc, argv);
      if (log_file)
	{
	  lw6sys_log_clear (log_file);
	  LW6SYS_FREE (log_file);
	}
      else
	{
	  lw6sys_log_critical (_("can't determine log file"));
	}
      lw6sys_log (LW6SYS_LOG_NOTICE, "v%s \"%s\" #%s (%s) %s/%s",
		  lw6sys_build_get_version (),
		  lw6sys_build_get_codename (),
		  lw6sys_build_get_stamp (),
		  lw6sys_build_get_date (),
		  lw6sys_build_get_host_os (), lw6sys_build_get_host_cpu ());
      lw6_print_hello (argc, argv);

      if (lw6_init_global (argc, argv))
	{
#if LW6_MS_WINDOWS || LW6_MAC_OS_X
	  _fix_guile_load_path (argc, argv);
#endif
#ifdef LW6_MAC_OS_X
	  _fix_dyld_library_path (argc, argv);
	  if (!lw6sys_vthread_run (_run, _end, NULL))
	    {
	      ret = 0;
	    }
#else
	  _run (NULL);
	  _end (NULL);
#endif

	  /*
	   * It's important to call lw6_quit_global_2 only now, when Guile
	   * is finished, otherwise Guile might call the destructors
	   * afterwards, when globals are zeroed. This way it works, the
	   * destructors (even for "chained" objects like game_struct)
	   * do not need to access SCM objects.
	   */
	  lw6_quit_global_2 ();
	}

      lw6_print_goodbye ();
    }
  else
    {
      lw6sys_clear_memory_bazooka ();
    }

  return ret;
}
