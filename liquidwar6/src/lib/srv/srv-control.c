/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#include "srv.h"

/**
 * lw6srv_start
 *
 * @ip: ip address to listen on
 * @port: port IP to bind to
 *
 * Starts a server, binds the socket(s) and returns a listener
 * object which can in turn be used to create connections.
 *
 * Return value: new listener object.
 */
lw6srv_listener_t *
lw6srv_start (const char *ip, int port)
{
  lw6srv_listener_t *listener = NULL;

  listener = LW6SYS_CALLOC (sizeof (lw6srv_listener_t));
  if (listener)
    {
      listener->ip = lw6sys_str_copy (ip);
      listener->port = port;
      listener->tcp_sock = lw6net_tcp_listen (ip, port);
      listener->tcp_accepters = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6srv_tcp_accepter_free);
      listener->udp_sock = lw6net_udp_server (ip, port);
      listener->udp_buffers = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6srv_udp_buffer_free);
      if (listener->ip && listener->port && (listener->tcp_sock >= 0) && listener->tcp_accepters && (listener->udp_sock >= 0) && listener->udp_buffers)
	{
	  // OK
	}
      else
	{
	  lw6srv_stop (listener);
	  listener = NULL;
	}
    }

  return listener;
}

static void
_accepter_close_callback (void *func_data, void *data)
{
  lw6srv_tcp_accepter_t *tcp_accepter = (lw6srv_tcp_accepter_t *) data;

  lw6net_socket_close (&(tcp_accepter->sock));
}

/**
 * lw6srv_stop
 *
 * @listener: listener to stop
 *
 * Stops a listener object, and frees it.
 *
 * Return value: none.
 */
void
lw6srv_stop (lw6srv_listener_t * listener)
{
  if (listener)
    {
      if (listener->udp_buffers)
	{
	  lw6sys_list_free (listener->udp_buffers);
	}
      lw6net_socket_close (&(listener->udp_sock));
      if (listener->tcp_accepters)
	{
	  lw6sys_list_map (listener->tcp_accepters, _accepter_close_callback, NULL);
	  lw6sys_list_free (listener->tcp_accepters);
	}
      lw6net_socket_close (&(listener->tcp_sock));
      if (listener->ip)
	{
	  LW6SYS_FREE (listener->ip);
	}
      LW6SYS_FREE (listener);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL listener"));
    }
}
