/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "sys.h"

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#ifndef LW6_MS_WINDOWS
#include <fcntl.h>
#endif

/**
 * lw6sys_process_fork_and_call
 *
 * @callback: the function to be called
 *
 * This is not a standard fork function, it will return to
 * the caller (parent) with something >0 if a child has been
 * created, or 0 if failed. On the child it will launch
 * the callback func, run it and exit right away.
 *
 * Return value: a process ID on success, 0 on failure.
 */
u_int64_t
lw6sys_process_fork_and_call (lw6sys_fork_func_t func)
{
  pid_t pid = 0;

  pid = fork ();
  if (!pid)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("process forked, callback begin"));
      // run the callback, this is what we're here for
      func ();
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("process forked, callback end"));
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
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("process forked, child pid=%" LW6SYS_PRINTF_LL
			   "d"), (long long) pid);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to fork"));
	  pid = 0LL;
	}
    }

  return (u_int64_t) pid;
}

/**
 * lw6sys_process_kill_1_9
 *
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
lw6sys_process_kill_1_9 (u_int64_t pid)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("kill pid=%" LW6SYS_PRINTF_LL "d"),
	      (long long) pid);

  if (!kill (pid, SIGTERM))
    {
      /*
       * Wait a little bit to give a chance to the process to
       * stop cleanly
       */
      lw6sys_snooze ();
      ret = 1;
    }


  if (!kill (pid, SIGKILL))
    {
      /*
       * Still wait, just to make sure the call has made 
       * its way to the program, now there's nothing else
       * we can do to stop the child.
       */
      lw6sys_snooze ();
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
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("couldn't kill pid=%" LW6SYS_PRINTF_LL "d"),
		  (long long) pid);
    }

  return ret;
}
