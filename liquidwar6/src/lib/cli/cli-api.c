/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "cli.h"

static void
_warning (const char *func_name)
{
  lw6sys_log (LW6SYS_LOG_WARNING,
	      _x_ ("cli backend function \"%s\" is not defined"), func_name);
}

/**
 * lw6cli_init
 *
 * @backend: backend to use
 *
 * Initializes a client backend. Must be performed before any other call.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cli_init (lw6cli_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  backend->cli_context = NULL;
  if (backend->init)
    {
      backend->cli_context =
	backend->init (backend->argc, backend->argv, &(backend->properties));
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->cli_context ? 1 : 0;
}

/**
 * lw6cli_quit
 *
 * @backend: unitialize a cli backend
 *
 * Closes a cli, but does not free all ressources.
 */
void
lw6cli_quit (lw6cli_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->quit)
    {
      /*
       * It's important to check that backend is not NULL for
       * quit can *really* be called several times on the same backend
       */
      if (backend->cli_context)
	{
	  backend->quit (backend->cli_context);
	  backend->cli_context = NULL;
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6cli_process_oob
 *
 * @backend: backend to use
 * @node_info: information on the current node
 * @oob_data: data of the out-of-band request
 *
 * Processes the required out-of-band tasks, this typically, for a client,
 * includes broadcasting. Depending on parameters passed in oob_data, might
 * actually do a broadcast or simply call a given host and see what's the answer.
 * A typicall exchange is PING then INFO and finally LIST. It's the responsability
 * of the client to take the OOB initiative and contact the server.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6cli_process_oob (lw6cli_backend_t * backend,
		    lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->process_oob)
    {
      ret = backend->process_oob (backend->cli_context, node_info, oob_data);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6cli_open
 * 
 * @backend: backend to use
 * @local_url: our local public url
 * @remote_url: the remote url we want to connect to
 * @remote_ip: remote IP address
 * @remote_port: remote IP port
 * @password: password to use (if needed)
 * @local_id: our local id
 * @remote_id: the remote id
 * @dns_ok: wether the remote announced URL matches DNS information
 * @network_reliability: network reliability (the highest, the better)
 * @recv_callback_func: callback func to be called when data is received
 * @recv_callback_data: pointer on additionnal data to pass to callback func
 *
 * Opens a connection with a remote host. Be carefull with the implementation
 * of @recv_callback_func, it should be at least reentrant, and when it accesses
 * shared data, use locks.
 *
 * Return value: new object.
 */
lw6cnx_connection_t *
lw6cli_open (lw6cli_backend_t * backend, const char *local_url,
	     const char *remote_url, const char *remote_ip, int remote_port,
	     const char *password, u_int64_t local_id, u_int64_t remote_id,
	     int dns_ok, int network_reliability,
	     lw6cnx_recv_callback_t recv_callback_func,
	     void *recv_callback_data)
{
  lw6cnx_connection_t *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->open)
    {
      ret =
	backend->open (backend->cli_context, local_url, remote_url, remote_ip,
		       remote_port, password, local_id, remote_id, dns_ok,
		       network_reliability, recv_callback_func,
		       recv_callback_data);
      if (ret)
	{
	  ret->properties = backend->properties;
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6cli_close
 *
 * @backend: backend to use
 * @connection: connection to use
 *
 * Closes a connection, this will free the connection object.
 *
 * Return value: none.
 */
void
lw6cli_close (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->close)
    {
      backend->close (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6cli_send
 *
 * @backend: backend to use
 * @connection: connection to use
 * @physical_ticket_sig: signature of physical sender
 * @logical_ticket_sig: signature of logical sender
 * @logical_from_id: id of logical sender
 * @logical_to_id: id of logicial target
 * @message: text of message to send
 *
 * Sends a message to a peer over a given connection.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6cli_send (lw6cli_backend_t * backend, lw6cnx_connection_t * connection,
	     u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
	     u_int64_t logical_from_id,
	     u_int64_t logical_to_id, const char *message)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->send)
    {
      if (lw6cnx_connection_reliability_filter (connection))
	{
	  ret =
	    backend->send (backend->cli_context, connection,
			   physical_ticket_sig, logical_ticket_sig,
			   logical_from_id, logical_to_id, message);
	  if (!ret)
	    {
	      /*
	       * If we can't send data, we mark this connection with
	       * the worst possible ping, it basically means the
	       * connexion just does not work, we do not want to send
	       * data on it, at least, we do not want it to be identified
	       * as a good connection any more.
	       */
	      connection->ping_msec = LW6CNX_WORST_PING_MSEC;
	    }
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
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6cli_poll
 *
 * @backend: backend to use
 * @connection: connection to use
 *
 * Performs required duty on connection, depending on the backend,
 * this can include sending messages and/or receiving them.
 * Must be called on a regular basis.
 *
 * Return value: none.
 */
void
lw6cli_poll (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->poll)
    {
      backend->poll (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6cli_repr
 *
 * @backend: backend to use
 * @connection: connection to represent
 *
 * Gives a human readable representation of the connection.
 *
 * Return value: dynamically allocated string.
 */
char *
lw6cli_repr (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
