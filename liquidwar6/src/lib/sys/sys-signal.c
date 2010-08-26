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
#include "sys-internal.h"

#include <signal.h>

/*
 * Exceptionnally we keep those global/static,
 * found no other way to do it since signal
 * handler does not have a (void *) as a parameter.
 */
static int _argc = 0;
static char **_argv = NULL;

/**
 * lw6sys_signal_custom
 *
 * @argc: argc as passed to @main()
 * @argv: argv as passed to @main()
 *
 * Set up our signal handlers. This will probably be overrided
 * later by other libs such as libSDL, but at least in pure server
 * mode it gives a way to treat SIGTERM the right way.
 *
 * Return value: none.
 */
void
lw6sys_signal_custom (int argc, char *argv[])
{
  lw6sys_log (LW6SYS_LOG_INFO,
	      _("setting custom SIGTERM, SIGINT, SIGHUP handlers"));
  _argc = argc;
  _argv = argv;
  if (signal (SIGTERM, lw6sys_signal_term_handler) == SIG_IGN)
    {
      signal (SIGTERM, SIG_IGN);
    }
  if (signal (SIGINT, lw6sys_signal_int_handler) == SIG_IGN)
    {
      signal (SIGINT, SIG_IGN);
    }
  if (signal (SIGHUP, lw6sys_signal_hup_handler) == SIG_IGN)
    {
      signal (SIGHUP, SIG_IGN);
    }
}

/**
 * lw6sys_signal_default
 *
 * Restore default signal handlers for those modified by @lw6sys_signal_custom.
 *
 * Return value: none.
 */
void
lw6sys_signal_default ()
{
  lw6sys_log (LW6SYS_LOG_INFO,
	      _("setting default SIGTERM, SIGINT, SIGHUP handlers"));
  if (signal (SIGTERM, SIG_DFL) == SIG_IGN)
    {
      signal (SIGTERM, SIG_IGN);
    }
  if (signal (SIGINT, SIG_DFL) == SIG_IGN)
    {
      signal (SIGINT, SIG_IGN);
    }
  if (signal (SIGHUP, SIG_DFL) == SIG_IGN)
    {
      signal (SIGHUP, SIG_IGN);
    }
}

/**
 * lw6sys_signal_term_handler
 *
 * @signum: SIGTERM
 *
 * The own TERM signal handler, will basically call 
 * the @lw6sys_signal_send_quit function, which will
 * set a flag used later by @lw6sys_signal_poll_quit.
 * 
 * Return value: none.
 */
void
lw6sys_signal_term_handler (int signum)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _("caught SIGTERM"));
  lw6sys_signal_send_quit ();
}

/**
 * lw6sys_signal_int_handler
 *
 * @signum: SIGINT
 *
 * The own INT signal handler, will basically call 
 * the @lw6sys_signal_send_quit function, which will
 * set a flag used later by @lw6sys_signal_poll_quit.
 * 
 * Return value: none.
 */
void
lw6sys_signal_int_handler (int signum)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _("caught SIGINT"));
  lw6sys_signal_send_quit ();
}

/**
 * lw6sys_signal_hup_handler
 *
 * @signum: SIGTERM
 *
 * The own HUP signal handler, will basically do
 * something that shows the program is alive, typically
 * display a NOTICE message.
 * 
 * Return value: none.
 */
void
lw6sys_signal_hup_handler (int signum)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _("caught SIGHUP"));
  if (_argc >= 1 && _argv != NULL)
    {
      lw6sys_exec_restart (_argc, _argv);
    }
}

/**
 * lw6sys_signal_send_quit
 *
 * Sets the quit flag to 1, so that @lw6sys_signal_poll_quit returns
 * true, that is, tells the polling loop to stop.
 * 
 * Return value: none.
 */
void
lw6sys_signal_send_quit ()
{
  lw6sys_log (LW6SYS_LOG_INFO, _("send QUIT"));
  _lw6sys_global.quit = 1;
}

/**
 * lw6sys_signal_poll_quit
 *
 * Tests wether we need to stop right now.
 * 
 * Return value: 1 if we need to stop now, 0 if program can continue.
 */
int
lw6sys_signal_poll_quit ()
{
  int ret = 0;

  ret = (_lw6sys_global.quit != 0);
  lw6sys_log (LW6SYS_LOG_DEBUG, _("poll quit ret=%d"), ret);

  return ret;
}
