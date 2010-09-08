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

#include <signal.h>

#include "net.h"
#include "net-internal.h"

_lw6net_context_t *_lw6net_global_context = NULL;

/**
 * lw6net_init
 *
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
lw6net_init (int argc, char *argv[], int net_log)
{
  int ok = 0;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("net init"));

  _lw6net_global_context =
    (_lw6net_context_t *) LW6SYS_CALLOC (sizeof (_lw6net_context_t));
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
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("WSAStartup() OK, returned 0"));
	  ok = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("WSAStartup() failed with code %d \"%s\""), err,
		      _lw6net_wsa_str (err));
	}
#else
      /*
       * Well, it seems that SIGPIPE is sent to the application when
       * there's a brutal socket close or things like that, so we
       * just trap it and do nothing. Normally, the socket functions
       * should return an error anyway...
       */
      if (signal (SIGPIPE, SIG_IGN) != SIG_ERR)
	{
	  ok = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("signal() failed"));
	}
#endif

      ok = ok
	&& _lw6net_const_init (argc, argv,
			       &(_lw6net_global_context->const_data));
      ok = ok
	&& _lw6net_counters_init (argc, argv,
				  &(_lw6net_global_context->counters));
      ok = ok
	&& _lw6net_log_init (argc, argv, &(_lw6net_global_context->log),
			     net_log);
      ok = ok
	&& _lw6net_dns_init (&(_lw6net_global_context->dns),
			     _lw6net_global_context->
			     const_data.dns_cache_hash_size);
    }

  if (!ok)
    {
      if (_lw6net_global_context)
	{
	  LW6SYS_FREE (_lw6net_global_context);
	  _lw6net_global_context = NULL;
	}
    }

  return _lw6net_global_context != NULL;
}

/**
 * lw6net_quit:
 *
 * Frees memory, joins active threads, and releases everything
 * set up by network code.
 *
 * Return value: void
 */
void
lw6net_quit ()
{
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("net quit"));

#ifdef LW6_MS_WINDOWS
  WSACleanup ();
#endif

  if (_lw6net_global_context)
    {
      _lw6net_dns_quit (&(_lw6net_global_context->dns));
      _lw6net_log_quit (&(_lw6net_global_context->log));
      _lw6net_counters_quit (&(_lw6net_global_context->counters));
      _lw6net_const_quit (&(_lw6net_global_context->const_data));
      LW6SYS_FREE (_lw6net_global_context);
    }

  _lw6net_global_context = NULL;
}
