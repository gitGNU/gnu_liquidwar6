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

#include "cnx.h"

#include "../msg/msg.h"		// just for header, no link

#define _DEFAULT_SEND_PASSWORD_CHECKSUM LW6MSG_UNDEF

/**
 * lw6cnx_connection_new
 *
 * @local_url: the local public URL
 * @remote_url: the remote public URL
 * @remote_ip: the remote IP address
 * @remote_port: the remote port
 * @password: the password to use
 * @local_id: the local ID
 * @remote_id: the remote ID
 * @dns_ok: 1 if no DNS mismatch, 0 if IP does not match public URL
 * @network_reliability: drop 1 out of X packets
 * @recv_callback_func: a callback to call when data is received
 * @recv_callback_data: additionnal data to pass to the callback
 *
 * Create a connection object. This object in itself does
 * nothing, it's just to share common structures among
 * modules, more precisely, between cli and srv code.
 * It's the responsability off the caller/backend to handle
 * the @backend_specific_data field which is NULL after this
 * call.
 *
 * Return value: newly allocated object.
 */
lw6cnx_connection_t *
lw6cnx_connection_new (const char *local_url, const char *remote_url,
		       const char *remote_ip, int remote_port,
		       const char *password, u_int64_t local_id,
		       u_int64_t remote_id, int dns_ok, int network_reliability, lw6cnx_recv_callback_t recv_callback_func, void *recv_callback_data)
{
  lw6cnx_connection_t *ret = NULL;

  ret = (lw6cnx_connection_t *) LW6SYS_CALLOC (sizeof (lw6cnx_connection_t));
  if (ret)
    {
      ret->local_url = lw6sys_str_copy (sys_context, local_url);
      ret->remote_url = lw6sys_str_copy (sys_context, remote_url);
      ret->remote_ip = lw6sys_str_copy (sys_context, remote_ip);
      ret->remote_port = remote_port;
      if (password && strlen (password) > 0)
	{
	  ret->password = lw6sys_str_copy (password);
	  ret->password_send_checksum = lw6cnx_password_checksum (remote_url, password);
	}
      else
	{
	  ret->password = lw6sys_str_copy ("");
	  ret->password_send_checksum = lw6sys_str_copy (sys_context, _DEFAULT_SEND_PASSWORD_CHECKSUM);
	}
      ret->local_id_int = local_id;
      ret->local_id_str = lw6sys_id_ltoa (sys_context, local_id);
      ret->remote_id_int = remote_id;
      ret->remote_id_str = lw6sys_id_ltoa (sys_context, remote_id);
      ret->dns_ok = dns_ok ? 1 : 0;
      ret->network_reliability = network_reliability;
      ret->recv_callback_func = recv_callback_func;
      ret->recv_callback_data = recv_callback_data;
      ret->send_mutex = lw6sys_mutex_create ();
      ret->ping_msec = LW6CNX_WORST_PING_MSEC;

      if (ret->local_url && ret->remote_url && ret->remote_ip && ret->password
	  && ret->password_send_checksum && ret->local_id_str && ret->remote_id_str && ret->send_mutex)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("created connection with \"%s\""), remote_url);
	}
      else
	{
	  lw6cnx_connection_free (ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6cnx_connection_free
 *
 * @connection: object to free
 *
 * Frees a connection object. It's the responsibility of
 * the caller/backend to handle the @backend_specific_data field.
 *
 * Return value: none.
 */
void
lw6cnx_connection_free (lw6cnx_connection_t * connection)
{
  if (connection->local_url)
    {
      LW6SYS_FREE (sys_context, connection->local_url);
    }
  if (connection->remote_url)
    {
      LW6SYS_FREE (sys_context, connection->remote_url);
    }
  if (connection->remote_ip)
    {
      LW6SYS_FREE (sys_context, connection->remote_ip);
    }
  if (connection->password)
    {
      LW6SYS_FREE (sys_context, connection->password);
    }
  if (connection->password_send_checksum)
    {
      LW6SYS_FREE (sys_context, connection->password_send_checksum);
    }
  if (connection->local_id_str)
    {
      LW6SYS_FREE (sys_context, connection->local_id_str);
    }
  if (connection->remote_id_str)
    {
      LW6SYS_FREE (sys_context, connection->remote_id_str);
    }
  if (connection->send_mutex)
    {
      lw6sys_mutex_destroy (sys_context, connection->send_mutex);
    }
  LW6SYS_FREE (sys_context, connection);
}

/**
 * lw6cnx_connection_should_send_foo
 *
 * @connection: the connection concerned
 * @now: the current timestamp
 *
 * Tells wether a new foo message must be issued.
 *
 * Return value: 1 if true, 0 if false.
 */
int
lw6cnx_connection_should_send_foo (lw6cnx_connection_t * connection, int64_t now)
{
  return (now > connection->next_send_foo_timestamp);
}

/**
 * lw6cnx_connection_init_foo_bar_key
 *
 * @connection: the connection concerned
 * @now: the current timestamp
 * @next_foo_delay: the delay (msec) before next foo message is sent
 *
 * Generates a new foo_bar_key, and schedules the next foo message send timestamp.
 *
 * Return value: none.
 */
void
lw6cnx_connection_init_foo_bar_key (lw6cnx_connection_t * connection, int64_t now, int next_foo_delay)
{
  if (connection->foo_bar_key)
    {
      /*
       * If foo_bar_key is still set it means that since last call,
       * message still has not been received. This is *bad* so we consider
       * that connection is just dead, next bar response should hopefully
       * prove the contrary.
       */
      connection->ping_msec = LW6CNX_WORST_PING_MSEC;
    }
  connection->last_send_foo_timestamp = now;
  connection->next_send_foo_timestamp = now + next_foo_delay / 2 + lw6sys_random (sys_context, next_foo_delay);
  connection->foo_bar_key = lw6sys_generate_id_32 ();
}

/**
 * lw6cnx_connection_lock_send
 *
 * @connection: the connexion to lock
 *
 * Acquires a "send" lock on the connexion, the idea is to avoid
 * too threads sending data using the same socket at the same time.
 * Note that each backend must call this when accessing the socket,
 * there's no top-level lock for the sake of performance.
 *
 * Return value: 1 on success, 0 if not.
 */
int
lw6cnx_connection_lock_send (lw6cnx_connection_t * connection)
{
  return lw6sys_mutex_lock (sys_context, connection->send_mutex);
}

/**
 * lw6cnx_connection_unlock_send
 *
 * @connection: the connexion to lock
 *
 * Releases a "send" lock on the connexion, the idea is to avoid
 * too threads sending data using the same socket at the same time.
 * Note that each backend must call this when accessing the socket,
 * there's no top-level lock for the sake of performance.
 *
 * Return value: none.
 */
void
lw6cnx_connection_unlock_send (lw6cnx_connection_t * connection)
{
  lw6sys_mutex_unlock (sys_context, connection->send_mutex);
}

/**
 * lw6cnx_connection_reliability_filter
 *
 * @connection: the connexion concerned
 *
 * Will filter and return true only in "rare" cases when packets
 * must be artificially dropped for testing purpose.
 *
 * Return value: 1 if message must be sent/received, 0 if not
 */
int
lw6cnx_connection_reliability_filter (lw6cnx_connection_t * connection)
{
  int ret = 0;

  ret = (lw6sys_random (sys_context, connection->network_reliability) != (connection->network_reliability >> 1));
  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("filtering message due to reliability %d"), connection->network_reliability);
    }

  return ret;
}
