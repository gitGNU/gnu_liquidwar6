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

#include "srv.h"

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("srv backend function \"%s\" is not defined"), func_name);
}

/**
 * lw6srv_init
 *
 * @sys_context: global system context
 * @backend: backend to use
 * @listener: listener object to use when constructing the backend.
 *
 * Initializes a server backend. Must be performed before any other call.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6srv_init (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6srv_listener_t * listener)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  backend->srv_context = NULL;
  if (backend->init)
    {
      backend->srv_context = backend->init (sys_context, backend->argc, backend->argv, &(backend->properties), listener);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->srv_context ? 1 : 0;
}

/**
 * lw6srv_quit
 *
 * @sys_context: global system context
 * @backend: unitialize a srv backend
 *
 * Closes a srv, but does not free all ressources.
 */
void
lw6srv_quit (lw6sys_context_t * sys_context, lw6srv_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->quit)
    {
      /*
       * It's important to check that backend is not NULL for
       * quit can *really* be called several times on the same backend
       */
      if (backend->srv_context)
	{
	  backend->quit (sys_context, backend->srv_context);
	  backend->srv_context = NULL;
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6srv_analyse_tcp
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @tcp_accepter: TCP mode accepter
 * @node_info: this node information
 * @remote_id: remote host id (out param)
 * @remote_url: remote host URL (out param, dynamically allocated)
 *
 * Analyzes new TCP messages, typically handled within the accepter
 * object. The result is a combination of bitwise flags, namely
 * namely @LW6SRV_ANALYSE_DEAD, @LW6SRV_ANALYSE_UNDERSTANDABLE,
 * and @LW6SRV_ANALYSE_OOB which helps knowing what to do with
 * message.
 *
 * Return value: bitwise flag.
 */
int
lw6srv_analyse_tcp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6srv_tcp_accepter_t * tcp_accepter, lw6nod_info_t * node_info,
		    u_int64_t * remote_id, char **remote_url)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->analyse_tcp)
    {
      ret = backend->analyse_tcp (sys_context, backend->srv_context, tcp_accepter, node_info, remote_id, remote_url);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_analyse_tcp
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @udp_buffer: UDP buffer
 * @node_info: this node information
 * @remote_id: remote host id (out param)
 * @remote_url: remote host URL (out param, dynamically allocated)
 *
 * Analyzes an UDP buffer, received on a socket.
 * The result is a combination of bitwise flags, namely
 * namely @LW6SRV_ANALYSE_DEAD, @LW6SRV_ANALYSE_UNDERSTANDABLE,
 * and @LW6SRV_ANALYSE_OOB which helps knowing what to do with
 * message.
 *
 * Return value: bitwise flag.
 */
int
lw6srv_analyse_udp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6srv_udp_buffer_t * udp_buffer, lw6nod_info_t * node_info,
		    u_int64_t * remote_id, char **remote_url)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->analyse_udp)
    {
      ret = backend->analyse_udp (sys_context, backend->srv_context, udp_buffer, node_info, remote_id, remote_url);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_process_oob
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @node_info: this node information
 * @oob_data: OOB data received
 *
 * Processes an OOB message sent from a client.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6srv_process_oob (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6nod_info_t * node_info, lw6srv_oob_data_t * oob_data)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->process_oob)
    {
      ret = backend->process_oob (sys_context, backend->srv_context, node_info, oob_data);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_open
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @lw6srv_listener: listener object
 * @local_url: local url to use (to send to peer)
 * @remote_url: remote url to communicate with
 * @remote_ip: remote ip to communicate with
 * @remote_port: remote port to communicate with
 * @password: password to use (the real password, not a hash)
 * @local_id: the local 64-bit id
 * @remote_id: the remove 64-bit id
 * @dns_ok: 1 if no DNS mismatch, 0 if situation is unclear
 * @network_reliability: the greater, the more reliable it is
 *
 * Opens a server connection. One might wonder, clients open
 * connections, but servers? To some extent, this is the equivalent
 * of @accept in the socket API, it will actually create an object
 * one can then use to communicate. Be carefull with the implementation
 * of the callback, keep in mind it can be called any time in
 * multithreaded mode, you need to set up locks when accessing shared
 * objects, including, but not limited to, your own data buffers.
 *
 * Return value: new connection object.
 */
lw6cnx_connection_t *
lw6srv_open (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6srv_listener_t * listener,
	     const char *local_url, const char *remote_url,
	     const char *remote_ip, int remote_port, const char *password, u_int64_t local_id, u_int64_t remote_id, int dns_ok, int network_reliability)
{
  lw6cnx_connection_t *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->open)
    {
      ret =
	backend->open (sys_context, backend->srv_context, listener, local_url, remote_url,
		       remote_ip, remote_port, password, local_id, remote_id, dns_ok, network_reliability);
      if (ret)
	{
	  ret->properties = backend->properties;
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_feed_with_tcp
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @connection: connection to use
 * @tcp_accepter: TCP accepter holding data
 *
 * When data is receivedm feeds the server object with data.
 * Will typically fire the callback receive function if there
 * are actually some data stuff.
 *
 * Return value: 1 some data processed, else 0
 */
int
lw6srv_feed_with_tcp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection, lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->feed_with_tcp)
    {
      ret = backend->feed_with_tcp (sys_context, backend->srv_context, connection, tcp_accepter);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_feed_with_udp
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @connection: connection to use
 * @tcp_accepter: TCP accepter holding data
 *
 * When data is receivedm feeds the server object with data.
 * Will typically fire the callback receive function if there
 * are actually some data stuff.
 *
 * Return value: 1 some data processed, else 0
 */
int
lw6srv_feed_with_udp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection, lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->feed_with_udp)
    {
      ret = backend->feed_with_udp (sys_context, backend->srv_context, connection, udp_buffer);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_close
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @connection: connection to close
 *
 * Closes a connection, will also free it.
 *
 * Return value: none.
 */
void
lw6srv_close (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->close)
    {
      backend->close (sys_context, backend->srv_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6srv_send
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @connection: connection to use
 * @now: current timestamp
 * @physical_ticket_sig: physical ticket
 * @logical_ticket_sig: logical ticket
 * @logical_from_id: logical id of sender
 * @logical_to_id: logical id of receiver
 * @message: string with the message to send
 *
 * Sends a message. The added value with a plain send is that it handles
 * all the special ticket fields.
 *
 * Return value: 1 on success, 0 if not
 */
int
lw6srv_send (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection,
	     int64_t now, u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->send)
    {
      if (lw6cnx_connection_reliability_filter (sys_context, connection))
	{
	  ret =
	    backend->send (sys_context, backend->srv_context, connection, now, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id,
			   message);
	}
      else
	{
	  /*
	   * We return the "reliable" attribute of the backend, the
	   * idea is that on so-called reliable backends we do not
	   * want an error to be fired and reset the connection,
	   * on unreliable backends, well it failed, it failed,
	   * not that bad.
	   */
	  ret = backend->properties.reliable;
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_can_send
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @connection: connection to use
 *
 * Tells wether a server connection can technically send messages.
 * This does not garantee send will succeed, but if it's not OK
 * at this stage, it's not even worth trying.
 *
 * Return value: 1 if it can be used to send messages, 0 if not ready.
 */
int
lw6srv_can_send (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->send)
    {
      ret = backend->can_send (sys_context, backend->srv_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6srv_poll
 *
 * @sys_context: global system context
 * @backend: server backend to use
 * @connection: connection to use
 *
 * Polling function, to be called on a regular basis.
 *
 * Return value: none.
 */
void
lw6srv_poll (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->poll)
    {
      backend->poll (sys_context, backend->srv_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6srv_repr
 *
 * @sys_context: global system context
 * @backend: backend to use
 * @connection: connection to represent
 *
 * Gives a human readable representation of the connection.
 *
 * Return value: dynamically allocated string.
 */
char *
lw6srv_repr (lw6sys_context_t * sys_context, const lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (sys_context, backend->srv_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
