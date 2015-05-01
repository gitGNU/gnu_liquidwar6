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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "net.h"
#include "net-internal.h"

int
_lw6net_socket_bind (lw6sys_context_t * sys_context, const char *ip, int port, int protocol)
{
  int sock = LW6NET_SOCKET_INVALID;
  int binded = 0;
  struct sockaddr_in name;
  int enable = 1;

  sock = socket (AF_INET, protocol, 0);
  if (lw6net_socket_is_valid (sys_context, sock))
    {
      if (!setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, (char *) &enable, sizeof (int)))
	{
	  /*
	   * SO_NOSIGPIPE not defined on GNU/Linux, in that case
	   * we use MSG_NOSIGNAL on send calls
	   */
#ifdef SO_NOSIGPIPE
	  if (setsockopt (sock, SOL_SOCKET, SO_NOSIGPIPE, (char *) &enable, sizeof (int)))
	    {
	      lw6net_last_error (sys_context);
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_NOSIGPIPE) failed"));
	    }
#endif // SO_NOSIGPIPE

	  name.sin_family = AF_INET;
	  name.sin_addr.s_addr = INADDR_ANY;
	  if (_lw6net_inet_aton (sys_context, &name.sin_addr, ip))
	    {
	      name.sin_port = htons (port);
	      if (bind (sock, (struct sockaddr *) &name, sizeof (name)) >= 0)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bind socket %d on %s:%d"), sock, ip, port);
		  _lw6net_counters_register_socket (sys_context, &(_lw6net_global_context->counters));
		  binded = 1;
		}
	      else
		{
		  name.sin_addr.s_addr = INADDR_ANY;
		  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) >= 0)
		    {
		      /*
		       * OK, we could not bind on specific IP, this does happen
		       * on Mac OS X with 127.0.0.1, it could possibly happen
		       * on other platforms if there's a network interface problem.
		       * In that case, we just bind on any/all interfaces, period.
		       */
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bind socket %d on port *:%d"), sock, port);
		      _lw6net_counters_register_socket (sys_context, &(_lw6net_global_context->counters));
		      binded = 1;
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bind() on %s:%d failed"), ip, port);
		      lw6net_last_error (sys_context);
		    }
		}
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt() failed"));
	  lw6net_last_error (sys_context);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("socket() failed"));
      lw6net_last_error (sys_context);
    }

  if (lw6net_socket_is_valid (sys_context, sock) && !binded)
    {
#ifdef LW6_MS_WINDOWS
      if (closesocket (sock))
#else
      if (close (sock))
#endif
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("close() failed"));
	  lw6net_last_error (sys_context);
	}
      sock = LW6NET_SOCKET_INVALID;
    }

  return sock;
}

/**
 * lw6net_socket_set_blocking_mode
 *
 * @sys_context: global system context
 * @sock: the socket to modify
 * @mode: the mode to use (1 -> blocking mode, 0 -> non-blocking)
 *
 * Sets the blocking mode of a socket, the reason we use
 * this is that @ioctl isn't portable (@ioctlsocket on MS-Windows).
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6net_socket_set_blocking_mode (lw6sys_context_t * sys_context, int sock, int mode)
{
  int ret = 0;

  if (lw6net_socket_is_valid (sys_context, sock))
    {
#ifdef LW6_MS_WINDOWS
      u_long enable_ul = !mode;

      if (!ioctlsocket (sock, FIONBIO, &enable_ul))
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("ioctlsocket failed on socket %d"), sock);
	  lw6net_last_error (sys_context);
	}
#else
      int flags = 0;

      flags = fcntl (sock, F_GETFL, 0);
      if (flags != -1)
	{
	  if (mode)
	    {
	      flags = flags & (~O_NONBLOCK);
	    }
	  else
	    {
	      flags = flags | O_NONBLOCK;
	    }
	  if (fcntl (sock, F_SETFL, flags) != -1)
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("fcntl failed on socket %d"), sock);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("fcntl failed on socket %d"), sock);
	}
#endif
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't set blocking mode on invalid socket %d"), sock);
    }

  return ret;
}

/**
 * lw6net_socket_is_valid
 *
 * @sys_context: global system context
 * @sock: the socket to test
 *
 * Tells if a socket is valid or not. This does not mean the
 * socket is opened/connected and/or the peer is reachable,
 * it just checks the socket is a valid descriptor. In practice
 * it's just to avoid copy/pasting if (sock>=0)" everywhere.
 *
 * Return value: 1 if valid, 0 if not
 */
int
lw6net_socket_is_valid (lw6sys_context_t * sys_context, int sock)
{
  return (sock >= 0);
}

/**
 * lw6net_socket_close
 *
 * @sys_context: global system context
 * @sock: the socket to close
 *
 * Closes a socket, that is, stop activity and free its descriptor.
 * This function will take a pointer on the socket, this is done
 * on purpose, the idea is to make sure once the socket is closed
 * it's never used again within the code, so we modify the pointed
 * value in place.
 *
 * Return value: none.
 */
void
lw6net_socket_close (lw6sys_context_t * sys_context, int *sock)
{
  int tmp_sock = LW6NET_SOCKET_INVALID;

  if (sock)
    {
      if (lw6net_socket_is_valid (sys_context, *sock))
	{
	  tmp_sock = (*sock);
	  (*sock) = LW6NET_SOCKET_INVALID;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("close socket %d"), tmp_sock);
	  _lw6net_counters_unregister_socket (sys_context, &(_lw6net_global_context->counters));
#ifdef LW6_MS_WINDOWS
	  if (closesocket (tmp_sock))
#else
	  if (close (tmp_sock))
#endif
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("close() failed"));
	      lw6net_last_error (sys_context);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't close invalid socket %d"), tmp_sock);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to close NULL socket"));
    }
}
