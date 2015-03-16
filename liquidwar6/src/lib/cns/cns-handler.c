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
#endif

#ifdef LW6_CONSOLE
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
/*
 * Need stdio.h for some old readline headers, see bug #24315 on Savannah
 */
#include <stdio.h>
#include <readline/readline.h>
#endif

#include "cns.h"

#define _PROMPT "lw6> "

#ifdef LW6_CONSOLE
static lw6sys_context_t *_console_context = NULL;
static lw6cns_callback_func_t _console_callback = NULL;
#endif // LW6_CONSOLE

#ifdef LW6_CONSOLE
static void
_lw6cns_flushall ()
{
  fflush (stdin);
  fflush (stdout);
  fflush (stderr);
}
#endif // LW6_CONSOLE

#ifdef LW6_CONSOLE
static int _console_handler_installed = 0;
#endif // LW6_CONSOLE

/**
 * lw6cns_handler_callback
 *
 * @line: the input line
 *
 * The global console handler. Because readline does not, or at
 * least, because I did not find any proper way to pass it a general
 * pointer along with the string, and since we need @sys_context
 * pretty much everywhere, we rely on using this handler which,
 * in turn, calls the handler passed to @lw6cns_handler_install
 * with @sys_context added as a first argument.
 *
 * Return value: none.
 */
void
lw6cns_handler_callback (char *line)
{
  lw6sys_context_t *sys_context = _console_context;

  if (sys_context)
    {
      if (_console_callback && line)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("calling rl callback with line=\"%s\""), line);
	  _console_callback (sys_context, line);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to call rl callback with _console_callback=%p and line=%p"), _console_callback, line);
	}
    }
}

/**
 * lw6cns_handler_install
 *
 * @sys_context: global system context
 * @callback: handler function.
 *
 * Installs a console handler.
 *
 * Return value: none.
 */
void
lw6cns_handler_install (lw6sys_context_t * sys_context, lw6cns_callback_func_t callback)
{
#ifdef LW6_CONSOLE
  if (!_console_handler_installed)
    {
      _lw6cns_flushall ();

      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("opening console"));

      _lw6cns_flushall ();

      if (lw6cns_term_support (sys_context))
	{
	  rl_set_keyboard_input_timeout (1);

	  _console_context = sys_context;
	  _console_callback = callback;

	  rl_callback_handler_install (_PROMPT, lw6cns_handler_callback);
	  _console_handler_installed = 1;
	}

      _lw6cns_flushall ();
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to install console handler but it looks like it's already installed..."));
    }
#else
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("no console support"));
#endif // LW6_CONSOLE
}

/**
 * lw6cns_handler_poll
 *
 * @sys_context: global system context
 *
 * Polling function for console, must be called on a regular basis.
 *
 * Return value: none.
 */
void
lw6cns_handler_poll (lw6sys_context_t * sys_context)
{
#ifdef LW6_CONSOLE
  fd_set rfds;
  struct timeval tv;
  int retval = 1;
  int c;

  if (_console_handler_installed)
    {
      /*
       * It's important to have a while here, for characters
       * are read one by one, and input could be slowed if
       * we had a simple "if".
       */
      while (retval)
	{
	  _lw6cns_flushall ();

	  FD_ZERO (&rfds);
	  FD_SET (0, &rfds);	// stdin == fd 0

	  tv.tv_sec = 0;
	  tv.tv_usec = 0;

	  retval = select (1, &rfds, NULL, NULL, &tv);

	  if (retval > 0 && FD_ISSET (0, &rfds) && !feof (stdin))
	    {
	      c = fgetc (stdin);
	      ungetc (c, stdin);
	      rl_callback_read_char ();
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to poll console handler but it looks like it's not installed..."));
    }
#endif // LW6_CONSOLE
}

/**
 * lw6cns_handler_remove
 *
 * @sys_context: global system context
 *
 * Remove console handler.
 *
 * Return value: none.
 */
void
lw6cns_handler_remove (lw6sys_context_t * sys_context)
{
#ifdef LW6_CONSOLE
  if (_console_handler_installed)
    {
      _lw6cns_flushall ();

      rl_callback_handler_remove ();
      _console_handler_installed = 0;

      _console_context = NULL;
      _console_callback = NULL;

      fprintf (stdout, "\n");

      _lw6cns_flushall ();
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to remove console handler but it looks like it's not installed..."));
    }

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("console closed"));
#endif // LW6_CONSOLE
}
