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

static void *
_guile_main (lw6sys_context_t * sys_context, void *data)
{
  char *script = NULL;

  // following is deprecated in Guile 2.0
  // I didn't find any replacement :(
#ifdef SCM_DEVAL_P
  SCM_DEVAL_P = 1;
  SCM_BACKTRACE_P = 1;
  SCM_RECORD_POSITIONS_P = 1;
  SCM_RESET_DEBUG_MODE;
#endif

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("registering C types and functions for Guile"));
  lw6_register_smobs (sys_context);
  lw6_register_funcs (sys_context);

  script = lw6sys_get_script_file (sys_context, lw6_global.argc, lw6_global.argv);
  if (script)
    {
      if (lw6sys_file_exists (sys_context, script))
	{
	  lw6scm_c_primitive_load (sys_context, script);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("unable to read main script file \"%s\""), script);
	}
      LW6SYS_FREE (sys_context, script);
    }

  scm_gc ();
  /*
   * Now, this _is_ weird but if you don't snooze (or at
   * least go idle) then gc does not seem to work and
   * then our final global garbage collection goes nuts.
   * so we just wait, and Guile does the job for real.
   * Weird, I told you.
   */
  lw6sys_snooze (sys_context);
  scm_gc ();
  /*
   * In older versions there was a lw6_quit_global_1 function to
   * cleanup the dsp/gfx module in the same thread than the caller
   * (SDL limitation on Mac OS X). Now this is done before by the
   * script using a call to lw6_release (sys_context,c-lw6-release).
   */

  return NULL;
}

static void
_run (lw6sys_context_t * sys_context, void *data)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("main run data=%p"), data);
  lw6scm_with_guile (sys_context, _guile_main, data);
}

static void
_end (lw6sys_context_t * sys_context, void *data)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("main end data=%p"), data);
}

/**
 * lw6_main:
 *
 * @sys_context: global system context
 * @argc: the argc parameter of the @main function, that is, the number
 *   of command-line args.
 * @argv: the argv parameter of the @main function, that is, an array
 *   containing pointers on command-line args.
 *
 * This function is directly called by @main. This means by linking
 * against libliquidwar6 and calling it, you would have a program
 * that is almost exactly the "official" upstream liquidwar6 binary,
 * except you can tweak it and have all the power to call whatever
 * other functions you like, embed it. In short, everything the binary
 * does, you can do it in your own binarn, by linking against the
 * library and calling this function.
 *
 * Return value: 1 if success, zero if failure. Note that this
 *   is the "standard" C / liquidwar6 way to proceed, but your @main
 *   function should return 0 if success, else an error code. Typical
 *   use is "return !lw6_main(argc, argv);".
 */
int
lw6_main (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  int ret = 0;
  char *debug_str = NULL;
  int debug = 0;
  char *log_level_str = NULL;
  int log_level = 0;
  char *log_file = NULL;
  int run_game = 1;
  char *pid_file = NULL;

  ret = lw6_process_non_run_options (sys_context, argc, argv, &run_game);
  if (run_game)
    {
      debug_str = lw6sys_arg_get_value_with_env (sys_context, argc, argv, LW6DEF_DEBUG);
      if (debug_str)
	{
	  debug = lw6sys_atob (sys_context, debug_str);
	  lw6sys_debug_set (sys_context, debug);
	  LW6SYS_FREE (sys_context, debug_str);
	}
      log_level_str = lw6cfg_unified_get_value (sys_context, argc, argv, LW6DEF_LOG_LEVEL);
      if (log_level_str)
	{
	  log_level = lw6sys_atoi (sys_context, log_level_str);
	  lw6sys_log_set_level (sys_context, log_level);
	  LW6SYS_FREE (sys_context, log_level_str);
	}
      log_file = lw6cfg_unified_get_log_file (sys_context, argc, argv);
      if (log_file)
	{
	  lw6sys_log_clear (sys_context, log_file);
	  LW6SYS_FREE (sys_context, log_file);
	}
      else
	{
	  lw6sys_log_critical (sys_context, _x_ ("can't determine log file"));
	}

      lw6hlp_print_hello (sys_context, argc, argv);
      lw6_fix_env (sys_context, argc, argv);

      if (lw6_init_global (sys_context, argc, argv))
	{
#ifdef LW6_MAC_OS_X
	  if (!lw6sys_vthread_run (sys_context, _run, _end, NULL))
	    {
	      ret = 0;
	    }
#else
	  _run (sys_context, NULL);
	  _end (sys_context, NULL);
#endif

	  if (ret)
	    {
	      if (lw6_global.ret)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("script returned true"));
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("script returned false, something is wrong"));
		  ret = 0;
		}
	    }

	  /*
	   * It's important to call lw6_quit_global only now, when Guile
	   * is finished, otherwise Guile might call the destructors
	   * afterwards, when globals are zeroed. This way it works, the
	   * destructors (even for "chained" objects like game_struct)
	   * do not need to access SCM objects.
	   */
	  lw6_quit_global (sys_context);
	}

      lw6hlp_print_goodbye (sys_context);
    }
  else
    {
      lw6sys_clear_memory_bazooka (sys_context);
    }

  pid_file = lw6sys_daemon_pid_file (sys_context, argc, argv);
  if (pid_file)
    {
      lw6sys_daemon_stop (sys_context, pid_file);
      LW6SYS_FREE (sys_context, pid_file);
    }

  return ret;
}
