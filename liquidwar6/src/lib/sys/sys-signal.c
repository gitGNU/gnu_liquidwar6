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

#include <signal.h>

static lw6sys_context_t *_signal_context = NULL;

/**
 * lw6sys_signal_custom
 *
 * @sys_context: global system context
 * @trap_errors: set to 1 if you want to trap SIGSEGV and SIGFPE
 *
 * Set up our signal handlers. This will probably be overrided
 * later by other libs such as libSDL, but at least in pure server
 * mode it gives a way to treat SIGTERM the right way.
 * The callbacks will use the @sys_context passed here, ignoring
 * whatever thread and/or whatever value for this context was used
 * when the error was detected. However, one needs at least one
 * context, for instance to log messages.
 *
 * Return value: none.
 */
void
lw6sys_signal_custom (lw6sys_context_t * sys_context, int trap_errors)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("setting custom SIGTERM, SIGINT, SIGHUP handlers"));
  _signal_context = sys_context;
#ifdef SIGTERM
  if (signal (SIGTERM, lw6sys_signal_term_handler) == SIG_IGN)
    {
      signal (SIGTERM, SIG_IGN);
    }
#endif //SIGTERM
#ifdef SIGINT
  if (signal (SIGINT, lw6sys_signal_int_handler) == SIG_IGN)
    {
      signal (SIGINT, SIG_IGN);
    }
#endif //SIGINT
#ifdef SIGHUP
  if (signal (SIGHUP, lw6sys_signal_hup_handler) == SIG_IGN)
    {
      signal (SIGHUP, SIG_IGN);
    }
#endif // SIGHUP
  if (trap_errors)
    {
#ifdef SIGSEGV
      if (signal (SIGSEGV, lw6sys_signal_segv_handler) == SIG_IGN)
	{
	  signal (SIGSEGV, SIG_IGN);
	}
#endif // SIGSEGV
#ifdef SIGFPE
      if (signal (SIGFPE, lw6sys_signal_fpe_handler) == SIG_IGN)
	{
	  signal (SIGFPE, SIG_IGN);
	}
#endif // SIGFPE
    }
}

/**
 * lw6sys_signal_default
 *
 * @sys_context: global system context
 *
 * Restore default signal handlers for those modified by @lw6sys_signal_custom.
 *
 * Return value: none.
 */
void
lw6sys_signal_default (lw6sys_context_t * sys_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("setting default SIGTERM, SIGINT, SIGHUP, SIGSEGV, SIGFPE handlers"));
  _signal_context = NULL;
#ifdef SIGTERM
  if (signal (SIGTERM, SIG_DFL) == SIG_IGN)
    {
      signal (SIGTERM, SIG_IGN);
    }
#endif // SIGTERM
#ifdef SIGINT
  if (signal (SIGINT, SIG_DFL) == SIG_IGN)
    {
      signal (SIGINT, SIG_IGN);
    }
#endif // SIGINT
#ifdef SIGHUP
  if (signal (SIGHUP, SIG_DFL) == SIG_IGN)
    {
      signal (SIGHUP, SIG_IGN);
    }
#endif // SIGHUP
#ifdef SIGSEGV
  if (signal (SIGSEGV, SIG_DFL) == SIG_IGN)
    {
      signal (SIGSEGV, SIG_IGN);
    }
#endif // SIGSEGV
#ifdef SIGFPE
  if (signal (SIGFPE, SIG_DFL) == SIG_IGN)
    {
      signal (SIGFPE, SIG_IGN);
    }
#endif // SIGFPE
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
  lw6sys_context_t *sys_context = _signal_context;

  if (sys_context)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("caught SIGTERM"));
      lw6sys_signal_send_quit (sys_context);
    }
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
  lw6sys_context_t *sys_context = _signal_context;

  if (sys_context)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("caught SIGINT"));
      lw6sys_signal_send_quit (sys_context);
    }
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
  char *uptime = NULL;
  lw6sys_context_t *sys_context = _signal_context;

  if (sys_context)
    {
      uptime = lw6sys_readable_uptime (sys_context, lw6sys_get_uptime (sys_context));
      if (uptime)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("caught SIGHUP, uptime=\"%s\""), uptime);
	  LW6SYS_FREE (sys_context, uptime);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("caught SIGHUP"));
	}
    }
}

/**
 * lw6sys_signal_segv_handler
 *
 * @signum: SIGTERM
 *
 * The own SEGV signal handler, will display a backtrace and exit.
 *
 * Return value: none.
 */
void
lw6sys_signal_segv_handler (int signum)
{
#ifdef SIGSEGV
  lw6sys_context_t *sys_context = _signal_context;

  if (sys_context)
    {
      /*
       * OK here we test wether signum is really SIGSEGV,
       * if this is the case the error is fired for good so
       * we exit, if not, we keep on going, probably this
       * is just a test.
       */
      if (signum == SIGSEGV)
	{
	  /*
	   * Now we restore default, in case our routine
	   * fires the same error once again...
	   */
	  if (signal (SIGSEGV, SIG_DFL) == SIG_IGN)
	    {
	      signal (SIGSEGV, SIG_IGN);
	    }
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("Segmentation fault"));
	  exit (LW6SYS_EXIT_ERROR);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("Testing segmentation fault handler"));
	}
    }
#endif
}

/**
 * lw6sys_signal_fpe_handler
 *
 * @signum: SIGTERM
 *
 * The own FPE signal handler, will display a backtrace and exit.
 *
 * Return value: none.
 */
void
lw6sys_signal_fpe_handler (int signum)
{
#ifdef SIGFPE
  lw6sys_context_t *sys_context = _signal_context;

  if (sys_context)
    {
      /*
       * OK here we test wether signum is really SIGFPE,
       * if this is the case the error is fired for good so
       * we exit, if not, we keep on going, probably this
       * is just a test.
       */
      if (signum == SIGFPE)
	{
	  /*
	   * Now we restore default, in case our routine
	   * fires the same error once again...
	   */
	  if (signal (SIGFPE, SIG_DFL) == SIG_IGN)
	    {
	      signal (SIGFPE, SIG_IGN);
	    }
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("Floating point exception"));
	  exit (LW6SYS_EXIT_ERROR);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("Testing floating point exception handler"));
	}
    }
#endif
}

/**
 * lw6sys_signal_send_quit
 *
 * @sys_context: global system context
 *
 * Sets the quit flag to 1, so that @lw6sys_signal_poll_quit returns
 * true, that is, tells the polling loop to stop.
 *
 * Return value: none.
 */
void
lw6sys_signal_send_quit (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("send QUIT"));
  global->quit = 1;
}

/**
 * lw6sys_signal_poll_quit
 *
 * @sys_context: global system context
 *
 * Tests wether we need to stop right now.
 *
 * Return value: 1 if we need to stop now, 0 if program can continue.
 */
int
lw6sys_signal_poll_quit (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  int ret = 0;

  ret = (global->quit != 0);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("poll quit ret=%d"), ret);

  return ret;
}
