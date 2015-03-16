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

#include <signal.h>

#include "net.h"
#include "net-internal.h"

_lw6net_context_t *_lw6net_global_context = NULL;

/**
 * lw6net_init
 *
 * @sys_context: global system context
 * @argc: argc as passed to @main
 * @argv: argv as passed to @main
 * @net_log: 1 if you want to enable net log, 0 if not
 *
 * Initializes the low-level network API, you must call this before
 * calling any other network related function, for it allocates a dynamic
 * context which is in turn used by every function.
 *
 * Return value: non-zero if success
 */
int
lw6net_init (lw6sys_context_t * sys_context, int argc, const char *argv[], int net_log)
{
  int ok = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("net init"));

  _lw6net_global_context = (_lw6net_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6net_context_t));
  if (_lw6net_global_context)
    {
#ifdef LW6_MS_WINDOWS
      WORD version_requested;
      WSADATA wsa_data;
      int err = 0;

      version_requested = MAKEWORD (2, 2);
      memset (&wsa_data, 0, sizeof (WSADATA));
      err = WSAStartup (version_requested, &wsa_data);
      if (err == 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("WSAStartup() OK, returned 0"));
	  ok = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("WSAStartup() failed with code %d \"%s\""), err, _lw6net_wsa_str (sys_context, err));
	}
#else
      /*
       * Well, it seems that SIGPIPE is sent to the application when
       * there's a brutal socket close or things like that, so we
       * just trap it and do nothing. Normally, the socket functions
       * should return an error anyway...
       */
      /*
       * Update: actually, using SO_NOSIGPIPE on socket creation
       * and/or MSG_NOSIGNAL on send/recv operations does seem
       * to work much (much) better, as this signal handling is
       * more or less (at least, this is what real-life examples
       * with LW6 code base show) limited to the main thread.
       * Still, we leave the signal trap here, it's anyway important
       * to remember that *THIS SIGPIPE SIGNAL MUST NEVER BE RAISED*
       * else LW6 will have bogus behaviors. Period.
       */
      if (signal (SIGPIPE, SIG_IGN) != SIG_ERR)
	{
	  ok = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("signal() failed"));
	}
#endif

      ok = ok && _lw6net_const_init (sys_context, argc, argv, &(_lw6net_global_context->const_data));
      ok = ok && _lw6net_counters_init (sys_context, argc, argv, &(_lw6net_global_context->counters));
      ok = ok && _lw6net_log_init (sys_context, argc, argv, &(_lw6net_global_context->log), net_log);
      ok = ok
	&& _lw6net_dns_init (sys_context, &(_lw6net_global_context->dns),
			     _lw6net_global_context->const_data.dns_cache_hash_size, _lw6net_global_context->const_data.dns_cache_delay_sec);
      ok = ok
	&& _lw6net_connectable_init (sys_context, &(_lw6net_global_context->connectable),
				     _lw6net_global_context->const_data.connectable_cache_hash_size,
				     _lw6net_global_context->const_data.connectable_cache_delay_sec);
    }

  if (!ok)
    {
      /*
       * Todo -> fix this and do proper deinit since some members
       * might have been allocated/initialized and require specific
       * functions
       */
      if (_lw6net_global_context)
	{
	  LW6SYS_FREE (sys_context, _lw6net_global_context);
	  _lw6net_global_context = NULL;
	}
    }

  return _lw6net_global_context != NULL;
}

/**
 * lw6net_quit:
 *
 * @sys_context: global system context
 *
 * Frees memory, joins active threads, and releases everything
 * set up by network code.
 *
 * Return value: void
 */
void
lw6net_quit (lw6sys_context_t * sys_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("net quit"));

#ifdef LW6_MS_WINDOWS
  WSACleanup ();
#endif

  if (_lw6net_global_context)
    {
      _lw6net_connectable_quit (sys_context, &(_lw6net_global_context->connectable));
      _lw6net_dns_quit (sys_context, &(_lw6net_global_context->dns));
      _lw6net_log_quit (sys_context, &(_lw6net_global_context->log));
      _lw6net_counters_quit (sys_context, &(_lw6net_global_context->counters));
      _lw6net_const_quit (sys_context, &(_lw6net_global_context->const_data));
      LW6SYS_FREE (sys_context, _lw6net_global_context);
    }

  _lw6net_global_context = NULL;
}
