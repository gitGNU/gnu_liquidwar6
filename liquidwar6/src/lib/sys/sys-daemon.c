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

#include <unistd.h>
#ifndef LW6_MS_WINDOWS
#include <fcntl.h>
#endif // LW6_MS_WINDOWS

#define _EXECUTED_AGAIN_TRUE "1"
#define _PID_FILE "daemon.pid"

#if LW6_MS_WINDOWS || LW6_MAC_OS_X
// no daemon support on those platforms
#else // LW6_MS_WINDOWS || LW6_MAC_OS_X
static int _pid_file_descriptor = -1;
#endif // LW6_MS_WINDOWS || LW6_MAC_OS_X

/**
 * lw6sys_daemon_pid_file
 *
 * @sys_context: global system context
 * @argc: argc as passed to @main
 * @argv: argv as passed to @main
 *
 * Get the default pid file, used to lock daemon and avoid
 * 2 daemons running at the same time.
 *
 * Return value: newly allocated string
 */
char *
lw6sys_daemon_pid_file (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *user_dir = NULL;
  char *pid_file = NULL;

  user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
  if (user_dir)
    {
      pid_file = lw6sys_path_concat (sys_context, user_dir, _PID_FILE);
      LW6SYS_FREE (sys_context, user_dir);
    }

  return pid_file;
}

/**
 * lw6sys_daemon_start
 *
 * @sys_context: global system context
 * @pid_file: the pid file used for the daemon
 *
 * Calls @fork internally to put the process in the program,
 * make it a daemon. Note this won't work on all platforms,
 * for instance it won't work on MS-Windows but this is rarely
 * an issue as MS-Windows users are rarely concerned with
 * detaching a program from a tty. Note that this isn't a
 * wrapper on @fork, the return value is different,
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sys_daemon_start (lw6sys_context_t * sys_context, char *pid_file)
{
  int ret = 0;

#if LW6_MS_WINDOWS || LW6_MAC_OS_X
  /*
   * fork() is definitely broken and hopeless on msys, but on Mac OS X
   * one could think it's OK (the code compiles). However program goes
   * dead when launching SDL, when forked that way. Rather than having
   * complaints about game not running the right way, I think it wiser
   * to disable the feature, however running a headless server on Mac OS X
   * does not seem a very common case, any sane person should use GNU/Linux
   * instead for that purpose.
   */
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("daemon mode not available on platform \"%s\""), lw6sys_build_get_host_os ());
#else // LW6_MS_WINDOWS || LW6_MAC_OS_X
  pid_t fork_ret = 0;
  char *pid_str = NULL;
  pid_t pid_int = 0;

  if (_pid_file_descriptor < 0)
    {
      fork_ret = fork ();
      if (fork_ret < 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("fork error code=%d"), fork_ret);
	}
      else
	{
	  if (fork_ret > 0)
	    {
	      /*
	       * We're in parent, here we use a brute-force immediate exit,
	       * this supposes the function isn't called when ressources
	       * such as graphics/sounds backends have been started.
	       */
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("parent dies"));
	      exit (0);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("child lives"));
	      /*
	       * We consider we start the daemon even if pid file
	       * is present and/or not lockable, but at least
	       * we display a warning.
	       */
	      ret = 1;

	      pid_int = getpid ();
	      _pid_file_descriptor = open (pid_file, O_RDWR | O_CREAT, 0640);
	      if (_pid_file_descriptor >= 0)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("pid file \"%s\""), pid_file);
		  if (lockf (_pid_file_descriptor, F_TLOCK, 0) >= 0)
		    {
		      pid_str = lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d\n", (long long) pid_int);
		      if (pid_str)
			{
			  if (write (_pid_file_descriptor, pid_str, strlen (pid_str)) > 0)
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					  _x_ ("daemon started pid=%" LW6SYS_PRINTF_LL "d, pid file is \"%s\""), (long long) pid_int, pid_file);
			    }
			  LW6SYS_FREE (sys_context, pid_str);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				  _x_ ("daemon pid=%" LW6SYS_PRINTF_LL "d unable to lock \"%s\""), (long long) pid_int, pid_file);
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("daemon pid=%" LW6SYS_PRINTF_LL "d unable to open \"%s\""), (long long) pid_int, pid_file);
		}
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't start daemon twice"));
    }
#endif // LW6_MS_WINDOWS || LW6_MAC_OS_X

  return ret;
}

/**
 * lw6sys_daemon_stop
 *
 * @sys_context: global system context
 * @pid_file: the pid file used for the daemon
 *
 * Removes the daemon pid file. Can be called safely even
 * if daemon wasn't started.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6sys_daemon_stop (lw6sys_context_t * sys_context, char *pid_file)
{
  int ret = 0;

#if LW6_MS_WINDOWS || LW6_MAC_OS_X
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no daemon on platform \"%s\""), lw6sys_build_get_host_os ());
  ret = 1;
#else // LW6_MS_WINDOWS || LW6_MAC_OS_X
  if (_pid_file_descriptor >= 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("closing pid file \"%s\""), pid_file);
      close (_pid_file_descriptor);
      _pid_file_descriptor = -1;
    }

  if (lw6sys_file_exists (sys_context, pid_file))
    {
      unlink (pid_file);
      if (!lw6sys_file_exists (sys_context, pid_file))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("removed pid file \"%s\""), pid_file);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to remove pid file \"%s\""), pid_file);
	}
    }
#endif // LW6_MS_WINDOWS || LW6_MAC_OS_X

  return ret;
}
