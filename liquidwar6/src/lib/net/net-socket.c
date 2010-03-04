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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "net.h"
#include "net-internal.h"

int
_lw6net_socket_init ()
{
  int ret = 1;

  return ret;
}

void
_lw6net_socket_quit ()
{
  if (_lw6net_global_context->socket_counters.open_counter <
      _lw6net_global_context->socket_counters.close_counter)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("%d sockets opened, but %d closed, there's probably a bug"),
		  _lw6net_global_context->socket_counters.open_counter,
		  _lw6net_global_context->socket_counters.close_counter);
    }
  if (_lw6net_global_context->socket_counters.open_counter >
      _lw6net_global_context->socket_counters.close_counter)
    {
      lw6sys_log (LW6SYS_LOG_INFO,
		  _
		  ("%d sockets opened, but %d closed, the only acceptable explanation is that there's a detached thread or something, which was idle when program ended"),
		  _lw6net_global_context->socket_counters.open_counter,
		  _lw6net_global_context->socket_counters.close_counter);
    }
  if (_lw6net_global_context->socket_counters.close_counter ==
      _lw6net_global_context->socket_counters.open_counter)
    {
      lw6sys_log (LW6SYS_LOG_INFO,
		  _("%d sockets opened and closed"),
		  _lw6net_global_context->socket_counters.open_counter);
    }
}

int
_lw6net_socket_bind (char *ip, int port, int protocol)
{
  int sock = -1;
  int binded = 0;
  struct sockaddr_in name;
  int enable = 1;

  sock = socket (AF_INET, protocol, 0);
  if (sock >= 0)
    {
      if (!setsockopt (sock, SOL_SOCKET, SO_REUSEADDR,
		       (char *) &enable, sizeof (int)))
	{
	  name.sin_family = AF_INET;
	  name.sin_addr.s_addr = INADDR_ANY;
#ifdef LW6_MS_WINDOWS
	  name.sin_addr.s_addr = inet_addr (ip);
#else
	  if (inet_aton (ip, &name.sin_addr) != 0)
#endif
	    {
	      name.sin_port = htons (port);
	      if (bind (sock, (struct sockaddr *) &name, sizeof name) >= 0)
		{
		  lw6sys_log (LW6SYS_LOG_INFO,
			      _("bind socket %d on %s:%d"), sock, ip, port);
		  _lw6net_global_context->socket_counters.open_counter++;
		  binded = 1;
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING, _("bind() on %s:%d failed"),
			      ip, port);
		  lw6net_last_error ();
		}
	    }
#ifndef LW6_MS_WINDOWS
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _("inetaton() failed, ip=\"%s\""), ip);
	      lw6net_last_error ();
	    }
#endif
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("setsockopt() failed"));
	  lw6net_last_error ();
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("socket() failed"));
      lw6net_last_error ();
    }

  if (sock >= 0 && !binded)
    {
#ifdef LW6_MS_WINDOWS
      if (closesocket (sock))
#else
      if (close (sock))
#endif
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("close() failed"));
	  lw6net_last_error ();
	}
      sock = -1;
    }

  return sock;
}

void
lw6net_socket_close (int sock)
{
  if (sock >= 0)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("close socket %d"), sock);
      _lw6net_global_context->socket_counters.close_counter++;
#ifdef LW6_MS_WINDOWS
      if (closesocket (sock))
#else
      if (close (sock))
#endif
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("close() failed"));
	  lw6net_last_error ();
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("can't close negative socket %d"), sock);
    }
}
