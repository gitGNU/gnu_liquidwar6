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

#include "sys.h"
#include "sys-internal.h"

/**
 * lw6sys_context_new
 *
 * Create a new global system context. This is normally
 * called only once during a program execution and is
 * wrapped in lw6sys_context_init.
 *
 * Return value: newly allocated context
 */
lw6sys_context_t *
lw6sys_context_new ()
{
  _lw6sys_context_t *_sys_context = NULL;

  /*
   * Call to regular malloc and not LW6SYS_MALLOC
   * since context is not fully available.
   */
  _sys_context = (_lw6sys_context_t *) malloc (sizeof (_lw6sys_context_t));

  return (lw6sys_context_t *) _sys_context;
}

/**
 * lw6sys_context_free
 *
 * @sys_context: global system context
 *
 * Frees a global system context.
 * Called only once during a program execution and is
 * wrapped in lw6sys_context_quit.
 *
 * Return value: none
 */
void
lw6sys_context_free (lw6sys_context_t * sys_context)
{
  if (sys_context)
    {
      /*
       * Call to regular free and not LW6SYS_FREE
       * since context is not fully available.
       */
      free (sys_context);
    }
}

/**
 * lw6sys_context_begin
 *
 * @sys_context: global system context
 *
 * Performs all initializations required for lw6sys functions
 * to behave correctly, this includes locale settings,
 * timer initialization, memory management related stuff.
 * This is wrapped into lw6sys_context_init.
 *
 * Return value: none
 */
void
lw6sys_context_begin (lw6sys_context_t * sys_context)
{
  _lw6sys_context_t *_sys_context = (_lw6sys_context_t *) sys_context;

  memset (sys_context, 0, sizeof (_lw6sys_context_t));

  _sys_context->global.debug = _LW6SYS_DEBUG_DEFAULT;
  _sys_context->global.log_level = LW6SYS_LOG_DEFAULT_ID;
  _sys_context->global.log_backtrace_mode = LW6SYS_LOG_BACKTRACE_MODE_FUNC;
  _sys_context->global.quit = 0;	// redundant with memset
  _sys_context->global.log_filename[0] = '\0';	// redundant with memset
  _sys_context->global.dialog_timeout_msec = LW6SYS_DIALOG_TIMEOUT_DEFAULT;
  _sys_context->global.console_enable_state = 1;

  /*
   * Initializing sub structures.
   */
  _lw6sys_bazooka_context_init (sys_context, &(_sys_context->bazooka_context));

  /*
   * Performing global initializations, all programs
   * should need those.
   */
  setlocale (LC_ALL, "");
  bindtextdomain (lw6sys_build_get_package_tarname (), lw6sys_build_get_localedir ());
  textdomain (lw6sys_build_get_package_tarname ());
  lw6sys_time_init (sys_context);
  lw6sys_history_init (sys_context);
  lw6sys_default_memory_bazooka (sys_context);
  lw6sys_profiler_check (sys_context, 1);
}

/**
 * lw6sys_context_end
 *
 * @sys_context: global system context
 *
 * Performs all cleanup required after all lw6sys functions
 * have been called. Once this has been called, do not call
 * any function that requires a valid context, except
 * maybe lw6sys_context_free. 
 * This is wrapped into lw6sys_context_quit.
 *
 * Return value: 1 on success, 0 on failure. A failure
 * can reveal a problem that occured way upstream, typically
 * some memory not cleanly freed.
 */
int
lw6sys_context_end (lw6sys_context_t * sys_context)
{
  int ret = 0;

  if (sys_context)
    {
      ret = 1;
      ret = lw6sys_check_thread_count (sys_context) && ret;
      ret = lw6sys_memory_bazooka_report (sys_context) && ret;
      lw6sys_clear_memory_bazooka (sys_context);

      /*
       * This is not 100% usefull, but just to make clear
       * and make sure we don't use it afterwards...
       */
      memset (sys_context, 0, sizeof (_lw6sys_context_t));
    }

  return ret;
}

/**
 * lw6sys_context_init
 *
 * Create a new global system context, and initalizes
 * it, so that it's ready for general use.
 *
 * Return value: newly allocated and valid context
 */
lw6sys_context_t *
lw6sys_context_init ()
{
  lw6sys_context_t *sys_context = NULL;

  sys_context = lw6sys_context_new ();
  if (sys_context)
    {
      lw6sys_context_begin (sys_context);
    }

  return sys_context;
}

/**
 * lw6sys_context_end
 *
 * @sys_context: global system context
 *
 * Performs all cleanup required after all lw6sys functions
 * have been called, and frees the object.
 *
 * Return value: 1 on success, 0 on failure. A failure
 * can reveal a problem that occured way upstream, typically
 * some memory not cleanly freed.
 */
int
lw6sys_context_quit (lw6sys_context_t * sys_context)
{
  int ret = 0;

  if (sys_context)
    {
      ret = lw6sys_context_end (sys_context);
      lw6sys_context_free (sys_context);
      sys_context = NULL;
    }

  return ret;
}
