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

#include "cli.h"

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("cli backend function \"%s\" is not defined"), func_name);
}

/**
 * lw6cli_init
 *
 * @sys_context: global system context
 * @backend: backend to use
 *
 * Initializes a client backend. Must be performed before any other call.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cli_init (lw6sys_context_t * sys_context, lw6cli_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  backend->cli_context = NULL;
  if (backend->init)
    {
      backend->cli_context = backend->init (sys_context, backend->argc, backend->argv, &(backend->properties));
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->cli_context ? 1 : 0;
}

/**
 * lw6cli_quit
 *
 * @sys_context: global system context
 * @backend: unitialize a cli backend
 *
 * Closes a cli, but does not free all ressources.
 */
void
lw6cli_quit (lw6sys_context_t * sys_context, lw6cli_backend_t * backend)
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
	  backend->quit (sys_context, backend->cli_context);
	  backend->cli_context = NULL;
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6cli_process_oob
 *
 * @sys_context: global system context
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
lw6cli_process_oob (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->process_oob)
    {
      ret = backend->process_oob (sys_context, backend->cli_context, node_info, oob_data);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6cli_open
 *
 * @sys_context: global system context
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
 *
 * Opens a connection with a remote host.
 *
 * Return value: new object.
 */
lw6cnx_connection_t *
lw6cli_open (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, const char *local_url,
	     const char *remote_url, const char *remote_ip, int remote_port,
	     const char *password, u_int64_t local_id, u_int64_t remote_id, int dns_ok, int network_reliability)
{
  lw6cnx_connection_t *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->open)
    {
      ret =
	backend->open (sys_context, backend->cli_context, local_url, remote_url, remote_ip,
		       remote_port, password, local_id, remote_id, dns_ok, network_reliability);
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
 * lw6cli_close
 *
 * @sys_context: global system context
 * @backend: backend to use
 * @connection: connection to use
 *
 * Closes a connection, this will free the connection object.
 *
 * Return value: none.
 */
void
lw6cli_close (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->close)
    {
      backend->close (sys_context, backend->cli_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6cli_send
 *
 * @sys_context: global system context
 * @backend: backend to use
 * @connection: connection to use
 * @now: current timestamp
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
lw6cli_send (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, lw6cnx_connection_t * connection,
	     int64_t now, u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->send)
    {
      if (lw6cnx_connection_reliability_filter (sys_context, connection))
	{
	  ret =
	    backend->send (sys_context, backend->cli_context, connection, now, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id,
			   message);
	  ++(connection->sent_nb_total);
	  if (ret)
	    {
	      ++(connection->sent_nb_success);
	    }
	  else
	    {
	      ++(connection->sent_nb_fail);
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
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6cli_can_send
 *
 * @sys_context: global system context
 * @backend: backend to use
 * @connection: connection to use
 *
 * Tells wether a client connection can technically send messages.
 * This does not garantee send will succeed, but if it's not OK
 * at this stage, it's not even worth trying.
 *
 * Return value: 1 if it can be used to send messages, 0 if not ready.
 */
int
lw6cli_can_send (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->send)
    {
      ret = backend->can_send (sys_context, backend->cli_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6cli_poll
 *
 * @sys_context: global system context
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
lw6cli_poll (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->poll)
    {
      backend->poll (sys_context, backend->cli_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6cli_repr
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
lw6cli_repr (lw6sys_context_t * sys_context, const lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (sys_context, backend->cli_context, connection);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
