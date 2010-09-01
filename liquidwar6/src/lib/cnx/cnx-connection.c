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

#include "cnx.h"

#define _DEFAULT_SEND_PASSWORD_CHECKSUM "00000000"

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
lw6cnx_connection_new (char *local_url, char *remote_url,
		       char *remote_ip, int remote_port,
		       char *password, char *local_id,
		       char *remote_id,
		       lw6cnx_recv_callback_t recv_callback_func,
		       void *recv_callback_data)
{
  lw6cnx_connection_t *ret = NULL;

  ret = (lw6cnx_connection_t *) LW6SYS_CALLOC (sizeof (lw6cnx_connection_t));
  if (ret)
    {
      ret->local_url = lw6sys_str_copy (local_url);
      ret->remote_url = lw6sys_str_copy (remote_url);
      ret->remote_ip = lw6sys_str_copy (remote_ip);
      ret->remote_port = remote_port;
      if (password && strlen (password) > 0)
	{
	  ret->password = lw6sys_str_copy (password);
	  ret->password_send_checksum =
	    lw6sys_password_checksum (remote_url, password);
	}
      else
	{
	  ret->password = lw6sys_str_copy ("");
	  ret->password_send_checksum =
	    lw6sys_str_copy (_DEFAULT_SEND_PASSWORD_CHECKSUM);
	}
      ret->local_id = lw6sys_str_copy (local_id);
      ret->remote_id = lw6sys_str_copy (remote_id);

      if (ret->local_url && ret->remote_url && ret->remote_ip && ret->password
	  && ret->password_send_checksum && ret->local_id && ret->remote_id)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("created connection with \"%s\""),
		      remote_url);
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
      LW6SYS_FREE (connection->local_url);
    }
  if (connection->remote_url)
    {
      LW6SYS_FREE (connection->remote_url);
    }
  if (connection->remote_ip)
    {
      LW6SYS_FREE (connection->remote_ip);
    }
  if (connection->password)
    {
      LW6SYS_FREE (connection->password);
    }
  if (connection->password_send_checksum)
    {
      LW6SYS_FREE (connection->password_send_checksum);
    }
  if (connection->local_id)
    {
      LW6SYS_FREE (connection->local_id);
    }
  if (connection->remote_id)
    {
      LW6SYS_FREE (connection->remote_id);
    }
  LW6SYS_FREE (connection);
}
