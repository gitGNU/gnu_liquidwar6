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

#include "../cli.h"
#include "mod-udp-internal.h"

lw6cnx_connection_t *
_mod_udp_open (_udp_context_t * udp_context, char *local_url,
	       char *remote_url, char *remote_ip, int remote_port,
	       char *password, u_int64_t local_id, u_int64_t remote_id,
	       int dns_ok, int network_reliability,
	       lw6cnx_recv_callback_t recv_callback_func,
	       void *recv_callback_data)
{
  lw6cnx_connection_t *ret = NULL;
  _udp_specific_data_t *specific_data = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("_mod_udp_open \"%s\""), remote_url);
  ret =
    lw6cnx_connection_new (local_url, remote_url, remote_ip, remote_port,
			   password, local_id, remote_id, dns_ok,
			   network_reliability, recv_callback_func,
			   recv_callback_data);
  if (ret)
    {
      ret->backend_specific_data =
	LW6SYS_CALLOC (sizeof (_udp_specific_data_t));
      specific_data = (_udp_specific_data_t *) ret->backend_specific_data;
      if (ret->backend_specific_data)
	{
	  specific_data->sock = lw6net_udp_client ();
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("open udp connection with \"%s\""), remote_url);
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
		lw6cnx_connection_t * connection)
{
  _udp_specific_data_t *specific_data =
    (_udp_specific_data_t *) connection->backend_specific_data;

  if (specific_data)
    {
      if (lw6net_socket_is_valid (specific_data->sock))
	{
	  lw6net_socket_close (specific_data->sock);
	}
      LW6SYS_FREE (specific_data);
    }
  lw6cnx_connection_free (connection);
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
