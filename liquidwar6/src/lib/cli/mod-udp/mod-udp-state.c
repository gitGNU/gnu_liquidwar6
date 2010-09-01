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

#include "../cli.h"
#include "mod-udp-internal.h"

lw6cli_connection_t *
_mod_udp_open (_udp_context_t * udp_context, char *remote_url,
	       char *remote_ip, int remote_port,
	       char *password_checksum, char *local_id, char *remote_id)
{
  lw6cli_connection_t *ret = NULL;
  _udp_specific_data_t *specific_data = NULL;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("_mod_udp_open \"%s\""), remote_url);
  ret = (lw6cli_connection_t *) LW6SYS_CALLOC (sizeof (lw6cli_connection_t));
  if (ret)
    {
      ret->remote_url = lw6sys_str_copy (remote_url);
      ret->remote_ip = lw6sys_str_copy (remote_ip);
      ret->remote_port = remote_port;
      ret->password_checksum = lw6sys_str_copy (password_checksum);
      ret->local_id = lw6sys_str_copy (local_id);
      ret->remote_id = lw6sys_str_copy (remote_id);

      ret->backend_specific_data =
	LW6SYS_CALLOC (sizeof (_udp_specific_data_t));
      specific_data = (_udp_specific_data_t *) ret->backend_specific_data;
      specific_data->sock = lw6net_udp_client ();

      if (ret->remote_url && ret->password_checksum
	  && ret->backend_specific_data
	  && lw6net_socket_is_valid (specific_data->sock))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("open udp connection with \"%s\""),
		      remote_url);
	}
      else
	{
	  _mod_udp_close (udp_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_mod_udp_close (_udp_context_t * udp_context,
		lw6cli_connection_t * connection)
{
  _udp_specific_data_t *specific_data =
    (_udp_specific_data_t *) connection->backend_specific_data;

  if (connection->backend_specific_data)
    {
      if (lw6net_socket_is_valid (specific_data->sock))
	{
	  lw6net_socket_close (specific_data->sock);
	}
      LW6SYS_FREE (connection->backend_specific_data);
    }
  if (connection->remote_url)
    {
      LW6SYS_FREE (connection->remote_url);
    }
  if (connection->remote_ip)
    {
      LW6SYS_FREE (connection->remote_ip);
    }
  if (connection->password_checksum)
    {
      LW6SYS_FREE (connection->password_checksum);
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

int
_mod_udp_timeout_ok (_udp_context_t * udp_context, int64_t origin_timestamp,
		     int broadcast)
{
  int ret = 0;
  int d = 0;

  d =
    origin_timestamp +
    ((broadcast ? udp_context->data.consts.
      broadcast_timeout : udp_context->data.consts.global_timeout) * 1000) -
    lw6sys_get_timestamp ();
  ret = (d > 0);

  return ret;
}
