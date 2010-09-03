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
lw6net_udp_client ()
{
  int sock = -1;
  int enable = 1;

  sock = socket (AF_INET, SOCK_DGRAM, 0);

  sock = (sock >= 0) ? sock : LW6NET_SOCKET_INVALID;

  if (lw6net_socket_is_valid (sock))
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("new UDP socket %d"), sock);

      _lw6net_counters_register_socket (&(_lw6net_global_context->counters));

      if (setsockopt (sock, SOL_SOCKET, SO_BROADCAST,
		      (char *) &enable, sizeof (int)))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("setsockopt(SO_BROADCAST) failed"));
	  lw6net_last_error ();
	}

      if (lw6net_socket_set_blocking_mode (sock, 0))
	{
	  //ok
	}
      else
	{
	  lw6net_socket_close (sock);
	  sock = LW6NET_SOCKET_INVALID;
	}
    }

  return sock;
}

int
lw6net_udp_server (char *ip, int port)
{
  int sock = -1;
  int enable = 1;

  sock = _lw6net_socket_bind (ip, port, SOCK_DGRAM);
  if (sock >= 0)
    {
      if (setsockopt (sock, SOL_SOCKET, SO_BROADCAST,
		      (char *) &enable, sizeof (int)))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("setsockopt(SO_BROADCAST) failed"));
	  lw6net_last_error ();
	}

      if (lw6net_socket_set_blocking_mode (sock, 0))
	{
	  //ok
	}
      else
	{
	  lw6net_socket_close (sock);
	  sock = LW6NET_SOCKET_INVALID;
	}
    }

  return sock;
}

int
lw6net_udp_send (int sock, char *buf, int len, char *ip, int port)
{
  int send_size = 0;
  struct sockaddr_in name;
  int res;

  name.sin_family = AF_INET;
  if (_lw6net_inet_aton (&name.sin_addr, ip))
    {
      name.sin_port = htons (port);
      res =
	sendto (sock, buf, len, 0, (struct sockaddr *) &name, sizeof (name));
      if (res >= 0)
	{
	  send_size = res;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("%d bytes sent on UDP socket %d"),
		      send_size, sock);
	  _lw6net_counters_register_send (&(_lw6net_global_context->counters),
					  send_size);
	  _lw6net_log_udp_send (&(_lw6net_global_context->log), buf,
				send_size);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("unable to send %d bytes on UDP socket %d"), len,
		      sock);
	}
    }

  return send_size;
}

static int
udp_recv (int sock, char *buf,
	  int len, char **incoming_ip, int *incoming_port, int flag)
{
  int recv_size = 0;
  struct sockaddr_in name;
  socklen_t namelen = sizeof (struct sockaddr_in);
  int res;

  memset (&name, 0, sizeof (name));

  res = recvfrom (sock, buf, len, flag, (struct sockaddr *) &name, &namelen);
  if (res >= 0)
    {
      recv_size = res;
      lw6sys_log (LW6SYS_LOG_DEBUG, _("%d bytes received on UDP socket %d"),
		  recv_size, sock);
      if (flag != MSG_PEEK)
	{
	  _lw6net_counters_register_recv (&(_lw6net_global_context->counters),
					  recv_size);
	  _lw6net_log_udp_recv (&(_lw6net_global_context->log), buf,
				recv_size);
	}
      if (incoming_ip)
	{
	  (*incoming_ip) = _lw6net_inet_ntoa (name.sin_addr);
	}
      if (incoming_port)
	{
	  (*incoming_port) = (int) ntohs (name.sin_port);
	}
    }
  else
    {
      if (incoming_ip)
	{
	  (*incoming_ip) = NULL;
	}
      if (incoming_port)
	{
	  (*incoming_port) = 0;
	}
    }

  return recv_size;
}

int
lw6net_udp_peek (int sock, char *buf, int len,
		 char **incoming_ip, int *incoming_port)
{
  return udp_recv (sock, buf, len, incoming_ip, incoming_port, MSG_PEEK);
}

int
lw6net_udp_recv (int sock, char *buf,
		 int len, char **incoming_ip, int *incoming_port)
{
  return udp_recv (sock, buf, len, incoming_ip, incoming_port, 0);
}
