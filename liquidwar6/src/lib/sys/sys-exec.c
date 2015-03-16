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

#define _EXECUTED_AGAIN_TRUE "1"

/**
 * lw6sys_exec_find_myself
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of args as passed to main
 *
 * Finds the path of the program currently run, this is typically to
 * pass it to @lw6sys_exec_again and run it again.
 *
 * Return value: the path (newly allocated string).
 */
char *
lw6sys_exec_find_myself (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *myself = NULL;

  if (argc >= 1)
    {
      myself = lw6sys_str_copy (sys_context, argv[0]);
      if (lw6sys_file_exists (sys_context, myself))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("found myself in \"%s\""), myself);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("file \"%s\" does not exist"), myself);
	}
    }

  return myself;
}

/**
 * lw6sys_is_executed_again
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of args as passed to main
 *
 * Tells wether the program is already executed by itself by
 * @lw6sys_exec_again function. Based on environment and command switches.
 *
 * Return value: 1 if executed again, 0 if not.
 */
int
lw6sys_is_executed_again (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  int ret = 0;

  ret = lw6sys_env_exists_prefixed (sys_context, LW6DEF_EXECUTED_AGAIN) || lw6sys_arg_exists (sys_context, argc, argv, LW6DEF_EXECUTED_AGAIN);

  return ret;
}

/**
 * lw6sys_exec_again
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of args as passed to main
 *
 * Runs the program from itsef, that is fires a new program (the same running)
 * and ends up the current one. This is used to fix some environment variable
 * issues. If LW6_EXECUTED_AGAIN (environment variable) is set, will not
 * run the program so this is not really like @exec as in the C standard
 * library, this function will actually return and be successfull even
 * if no other process was started. It's just designed to bootstrap/launch
 * the process once.
 *
 * Return value: 1 on success, 0 on failure (always fail)
 */
int
lw6sys_exec_again (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *myself;
  char **new_argv;
  int i;
  int ret = 0;

  if (!lw6sys_is_executed_again (sys_context, argc, argv))
    {
      if (argc >= 1)
	{
	  myself = lw6sys_exec_find_myself (sys_context, argc, argv);
	  if (myself)
	    {
	      new_argv = (char **) LW6SYS_CALLOC (sys_context, sizeof (char *) * (argc + 2));
	      if (new_argv)
		{
		  for (i = 0; i < argc; ++i)
		    {
		      new_argv[i] = lw6sys_str_copy (sys_context, argv[i]);
		    }
		  lw6sys_setenv_prefixed (sys_context, LW6DEF_EXECUTED_AGAIN, _EXECUTED_AGAIN_TRUE);
		  new_argv[argc] = "--" LW6DEF_EXECUTED_AGAIN;
		  new_argv[argc + 1] = NULL;
		  execvp (myself, (void *) new_argv);
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("execvp(%s) failed"), myself);
		  for (i = 0; i < argc; ++i)
		    {
		      if (new_argv[i])
			{
			  LW6SYS_FREE (sys_context, new_argv[i]);
			}
		    }
		  LW6SYS_FREE (sys_context, new_argv);
		}
	      LW6SYS_FREE (sys_context, myself);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find myself"));
	    }
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't run \"%s\" again"), argv[0]);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("executed again so no new process started"));
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_exec_restart
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of args as passed to main
 *
 * Restart the program with exactly the same arguments it was given
 * the first time.
 *
 * Return value: 1 on success, 0 on failure (always fail)
 */
int
lw6sys_exec_restart (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *myself;
  char **new_argv;
  int i;
  int ret = 0;

  if (argc >= 1)
    {
      myself = lw6sys_exec_find_myself (sys_context, argc, argv);
      if (myself)
	{
	  new_argv = (char **) LW6SYS_CALLOC (sys_context, sizeof (char *) * (argc + 1));
	  if (new_argv)
	    {
	      for (i = 0; i < argc; ++i)
		{
		  new_argv[i] = lw6sys_str_copy (sys_context, argv[i]);
		}
	      new_argv[argc] = NULL;
	      execvp (myself, (void *) new_argv);
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("execvp(%s) failed"), myself);
	      for (i = 0; i < argc; ++i)
		{
		  if (new_argv[i])
		    {
		      LW6SYS_FREE (sys_context, new_argv[i]);
		    }
		}
	      LW6SYS_FREE (sys_context, new_argv);
	    }
	  LW6SYS_FREE (sys_context, myself);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find myself"));
	}
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't restart \"%s\""), argv[0]);
    }

  return ret;
}
