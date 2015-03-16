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

/**
 * lw6net_udp_client
 *
 * @sys_context: global system context
 *
 * Creates an UDP client socket, that is, creates it and does
 * not bind it to any address.
 *
 * Return value: socket (>=0) on success, else -1
 */
int
lw6net_udp_client (lw6sys_context_t * sys_context)
{
  int sock = -1;
  int enable = 1;

  sock = socket (AF_INET, SOCK_DGRAM, 0);

  sock = (lw6net_socket_is_valid (sys_context, sock)) ? sock : LW6NET_SOCKET_INVALID;

  if (lw6net_socket_is_valid (sys_context, sock))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("new UDP socket %d"), sock);

      _lw6net_counters_register_socket (sys_context, &(_lw6net_global_context->counters));

      if (setsockopt (sock, SOL_SOCKET, SO_BROADCAST, (char *) &enable, sizeof (int)))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_BROADCAST) failed"));
	  lw6net_last_error (sys_context);
	}
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

      if (lw6net_socket_set_blocking_mode (sys_context, sock, 0))
	{
	  //ok
	}
      else
	{
	  lw6net_socket_close (sys_context, &sock);
	}
    }

  return sock;
}

/**
 * lw6net_udp_server
 *
 * @sys_context: global system context
 * @ip: IP address to bind to
 * @port: IP port to listen on
 *
 * Creates an UDP listening socket, that is, creates it
 * and binds it on a given address.
 *
 * Return value: socket (>=0) on success, else -1
 */
int
lw6net_udp_server (lw6sys_context_t * sys_context, const char *ip, int port)
{
  int sock = -1;
  int enable = 1;

  sock = _lw6net_socket_bind (sys_context, ip, port, SOCK_DGRAM);
  if (lw6net_socket_is_valid (sys_context, sock))
    {
      if (setsockopt (sock, SOL_SOCKET, SO_BROADCAST, (char *) &enable, sizeof (int)))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_BROADCAST) failed"));
	  lw6net_last_error (sys_context);
	}

      if (lw6net_socket_set_blocking_mode (sys_context, sock, 0))
	{
	  //ok
	}
      else
	{
	  lw6net_socket_close (sys_context, &sock);
	}
    }

  return sock;
}

/**
 * lw6net_udp_send
 *
 * @sys_context: global system context
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @ip: IP address to send data to
 * @port: IP port to send data to
 *
 * Sends an UDP datagram. Size can't be longer than about 1400 bytes,
 * see problems about MTU, in practice all values arround 1000 are quite safe,
 * 500 is pretty much garanteed to work everywhere, and for various reasons
 * 1452 is a good maximum bet.
 *
 * Return value: number of bytes sent
 */
int
lw6net_udp_send (lw6sys_context_t * sys_context, int sock, const char *buf, int len, const char *ip, int port)
{
  int send_size = 0;
  struct sockaddr_in name;
  int res;

  name.sin_family = AF_INET;
  if (_lw6net_inet_aton (sys_context, &name.sin_addr, ip))
    {
      name.sin_port = htons (port);
      res = sendto (sock, buf, len, 0, (struct sockaddr *) &name, sizeof (name));
      if (res >= 0)
	{
	  send_size = res;
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%d bytes sent on UDP socket %d"), send_size, sock);
	  _lw6net_counters_register_send (sys_context, &(_lw6net_global_context->counters), send_size);
	  _lw6net_log_udp_send (sys_context, &(_lw6net_global_context->log), buf, send_size);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to send %d bytes on UDP socket %d"), len, sock);
	}
    }

  return send_size;
}

static int
_udp_recv (lw6sys_context_t * sys_context, int sock, char *buf, int len, char **incoming_ip, int *incoming_port, int flag)
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
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%d bytes received on UDP socket %d"), recv_size, sock);
      if (flag != MSG_PEEK)
	{
	  _lw6net_counters_register_recv (sys_context, &(_lw6net_global_context->counters), recv_size);
	  _lw6net_log_udp_recv (sys_context, &(_lw6net_global_context->log), buf, recv_size);
	}
      if (incoming_ip)
	{
	  (*incoming_ip) = _lw6net_inet_ntoa (sys_context, name.sin_addr);
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

/**
 * lw6net_udp_peek
 *
 * @sys_context: global system context
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @ip: IP address of sender (out param, dynamically allocated)
 * @port: IP port to send data to (out param)
 *
 * Peeks for a UDP datagram. Will not remove the data from queue.
 *
 * Return value: number of bytes received
 */
int
lw6net_udp_peek (lw6sys_context_t * sys_context, int sock, char *buf, int len, char **incoming_ip, int *incoming_port)
{
  return _udp_recv (sys_context, sock, buf, len, incoming_ip, incoming_port, MSG_PEEK);
}

/**
 * lw6net_udp_recv
 *
 * @sys_context: global system context
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @ip: IP address of sender (out param, dynamically allocated)
 * @port: IP port to send data to (out param)
 *
 * Receives a UDP datagram. Will remove the data from queue.
 *
 * Return value: number of bytes received
 */
int
lw6net_udp_recv (lw6sys_context_t * sys_context, int sock, char *buf, int len, char **incoming_ip, int *incoming_port)
{
  return _udp_recv (sys_context, sock, buf, len, incoming_ip, incoming_port, 0);
}
