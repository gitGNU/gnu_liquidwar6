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

/*
 * In theory, more platforms than pure GNU could handle this,
 * however for testing purposes having the tests depending on these
 * features (mostly, high-level network tests) being run only on
 * GNU/Linux is acceptable, typically, all low-level features are
 * tested anyway. In practice, both MS-Windows and Mac OS X have flaws
 * that make it complicated, mingw32 does not emulate a complete POSIX
 * process support (fork, PID...) and SDL for Mac requires heavy fiddling
 * with threads, so anyway this is rotten on those platforms. Sorry
 * for BSD guys, if you really want to compile this, go ahead, but this
 * is probably not really worth it unless there's some serious network
 * bug specific to this platform that requires this kind of debugging.
 */
#ifdef LW6_GNU
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#endif // LW6_GNU

/*
 * Wait for 5 minutes when done, logically we'll be killed
 * before, but well, there must be a limit so that if for
 * some reason kill does not work, process stills ends not
 * too long after it's done its job
 */
#define _SLEEP_WHEN_DONE 300

/**
 * lw6sys_process_is_fully_supported
 *
 * @sys_context: global system context
 *
 * Tells wether functions related to fork and pids are likely
 * to work allright or not. Typically, those functions will
 * return false (0) systematically if called on a platform that
 * does not support them fully. In practice this is only for
 * a few tests, so it's not that bad if it does not really work.
 * Would be better if, but well, it's OK.
 *
 * Return value: 1 if supported, 0 if not.
 */
int
lw6sys_process_is_fully_supported (lw6sys_context_t * sys_context)
{
#ifdef LW6_GNU
  return 1;
#else // LW6_GNU
  return 0;
#endif // LW6_GNU
}

/**
 * lw6sys_process_fork_and_call
 *
 * @sys_context: global system context
 * @callback: the function to be called
 * @data: pointer on arbitrary data used by func
 *
 * This is not a standard fork function, it will return to
 * the caller (parent) with something >0 if a child has been
 * created, or 0 if failed. On the child it will launch
 * the callback func, run it and exit right away.
 *
 * Return value: a process ID on success, 0 on failure.
 */
u_int64_t
lw6sys_process_fork_and_call (lw6sys_context_t * sys_context, lw6sys_fork_func_t func, void *data)
{
#ifdef LW6_GNU
  pid_t pid = 0;

  pid = fork ();
  if (!pid)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("process forked, callback begin func=%p data=%p"), &func, data);
      // run the callback, this is what we're here for
      func (sys_context, data);
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("process forked, callback end func=%p data=%p"), &func, data);

      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("now naively trying to sleep for %d minutes"), _SLEEP_WHEN_DONE);
      lw6sys_sleep (sys_context, _SLEEP_WHEN_DONE);

      /*
       * Here we exit in "dirty" mode without continuing, freeing
       * memory etc etc, the idea is that anyway, this would be very
       * complex to track and the fork facility is mostly used for
       * testing, so there's no point in being pedantic here
       */
      exit (0);
    }
  else
    {
      if (pid > 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("process forked, child pid=%" LW6SYS_PRINTF_LL "d"), (long long) pid);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to fork"));
	  pid = 0LL;
	}
    }

  return (u_int64_t) pid;
#else // LW6_GNU
  return 0;
#endif // LW6_GNU
}

/**
 * lw6sys_process_kill_1_9
 *
 * @sys_context: global system context
 * @pid: pid to kill
 *
 * Kills a process with the given PID. The kill will first use a signal 1 SIGTERM
 * the a signal 9 SIGKILL. This is mostly for testing, the idea is to be sure to
 * vaccum after we're done. We use 64-bit for PIDs, yes, they are very likely
 * 32 only, but had to choose (and pid_t is a pain because for logging one would
 * needed to check the length before calling printf like functions...).
 *
 * Return value: 1 on success, 0 if failed
 */
int
lw6sys_process_kill_1_9 (lw6sys_context_t * sys_context, u_int64_t pid)
{
#ifdef LW6_GNU
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("kill pid=%" LW6SYS_PRINTF_LL "d"), (long long) pid);

  if (!kill (pid, SIGTERM))
    {
      /*
       * Wait a little bit to give a chance to the process to
       * stop cleanly
       */
      lw6sys_snooze (sys_context);
      ret = 1;
    }


  if (!kill (pid, SIGKILL))
    {
      /*
       * Still wait, just to make sure the call has made
       * its way to the program, now there's nothing else
       * we can do to stop the child.
       */
      lw6sys_snooze (sys_context);
      ret = 1;
    }

  /*
   * Note about return value, it's OK if any of the 2 kills worked,
   * actually the first one could have succeded but the signal have
   * been trapped (and therefore process not stopped...) this is
   * why we fire the second call. OTOH the second call is likely
   * to fail if only first one worked... So in any case, one of
   * those tests can fail, it's not that bad. Really checking
   * wether process is still here is beyond the scope of this
   * function.
   */
  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't kill pid=%" LW6SYS_PRINTF_LL "d"), (long long) pid);
    }

  return ret;
#else // LW6_GNU
  return 0;
#endif // LW6_GNU
}
