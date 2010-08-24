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

#include "sys.h"

#include <unistd.h>
#ifndef LW6_MS_WINDOWS
#include <fcntl.h>
#endif

#define _EXECUTED_AGAIN_TRUE "1"
#define _PID_FILE "daemon.pid"

static int _pid_file_descriptor = -1;

/**
 * lw6sys_daemon_pid_file
 *
 * @argc: argc as passed to @main
 * @argv: argv as passed to @main
 *
 * Get the default pid file, used to lock daemon and avoid
 * 2 daemons running at the same time.
 *
 * Return value: newly allocated string
 */
char *
lw6sys_daemon_pid_file (int argc, char *argv[])
{
  char *user_dir = NULL;
  char *pid_file = NULL;

  user_dir = lw6sys_get_user_dir (argc, argv);
  if (user_dir)
    {
      pid_file = lw6sys_path_concat (user_dir, _PID_FILE);
      LW6SYS_FREE (user_dir);
    }

  return pid_file;
}

/**
 * lw6sys_daemon_start
 *
 * @pid_file: the pid file used for the daemon
 *
 * Calls @fork() internally to put the process in the program, 
 * make it a daemon. Note this won't work on all platforms,
 * for instance it won't work on MS-Windows but this is rarely
 * an issue as MS-Windows users are rarely concerned with
 * detaching a program from a tty. Note that this isn't a
 * wrapper on @fork(), the return value is different,
 * 1 on success, 0 if failed.
 *
 * Return value: a process ID on success, 0 on failure.
 */
int
lw6sys_daemon_start (char *pid_file)
{
  int ret = 0;

#ifdef LW6_MS_WINDOWS
  lw6sys_log (LW6SYS_LOG_WARNING,
	      _("can't start daemon on Microsoft Windows, not supported"));
#else
  int fork_ret = 0;
  char *pid_str = NULL;
  int pid_int = 0;

  if (_pid_file_descriptor < 0)
    {
      fork_ret = fork ();
      if (fork_ret < 0)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("fork error code=%d"));
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
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("parent dies"));
	      exit (0);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("child lives"));
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
		  lw6sys_log (LW6SYS_LOG_DEBUG, _("pid file \"%s\""),
			      pid_file);
		  if (lockf (_pid_file_descriptor, F_TLOCK, 0) >= 0)
		    {
		      pid_str = lw6sys_new_sprintf ("%d\n", pid_int);
		      if (pid_str)
			{
			  if (write
			      (_pid_file_descriptor, pid_str,
			       strlen (pid_str)) > 0)
			    {
			      lw6sys_log (LW6SYS_LOG_NOTICE,
					  _
					  ("daemon started pid=%d, pid file is \"%s\""),
					  pid_int, pid_file);
			    }
			  LW6SYS_FREE (pid_str);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _
				  ("daemon pid=%d unable to lock \"%s\""),
				  pid_int, pid_file);
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("daemon pid=%d unable to open \"%s\""),
			      pid_int, pid_file);
		}
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("can't start daemon twice"));
    }
#endif

  return ret;
}

/**
 * lw6sys_daemon_stop
 *
 * @pid_file: the pid file used for the daemon
 *
 * Removes the daemon pid file. Can be called safely even
 * if daemon wasn't started.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6sys_daemon_stop (char *pid_file)
{
  int ret = 0;

#ifdef LW6SYS_MS_WINDOWS
  lw6sys_log (LW6SYS_LOG_DEBUG, _("no daemon on MS-Windows"));
  ret = 1;
#else
  if (_pid_file_descriptor >= 0)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("closing pid file \"%s\""), pid_file);
      close (_pid_file_descriptor);
      _pid_file_descriptor = -1;
    }

  if (lw6sys_file_exists (pid_file))
    {
      unlink (pid_file);
      if (!lw6sys_file_exists (pid_file))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("removed pid file \"%s\""),
		      pid_file);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to remove pid file \"%s\""), pid_file);
	}
    }
#endif

  return ret;
}
