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

#include "../srv.h"
#include "mod-tcpd-internal.h"

int
_mod_tcpd_send (_mod_tcpd_context_t * tcpd_context,
		lw6cnx_connection_t * connection,
		int64_t now,
		u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message)
{
  int ret = 0;
  _mod_tcpd_specific_data_t *specific_data = (_mod_tcpd_specific_data_t *) connection->backend_specific_data;
  char *line;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd send \"%s\""), message);
  line =
    lw6msg_envelope_generate (LW6MSG_ENVELOPE_MODE_TELNET,
			      lw6sys_build_get_version (),
			      connection->password_send_checksum,
			      physical_ticket_sig, logical_ticket_sig,
			      connection->local_id_int, connection->remote_id_int, logical_from_id, logical_to_id, message);
  if (line)
    {
      if (lw6net_socket_is_valid (sys_context, specific_data->sock))
	{
	  if (lw6cnx_connection_lock_send (connection))
	    {
	      if (lw6net_send_line_tcp (sys_context, &(specific_data->sock), line))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd sent \"%s\""), line);
		  specific_data->last_send_success_timestamp = now;
		  ret = 1;
		}
	      lw6cnx_connection_unlock_send (connection);
	    }
	}
      else
	{
	  if (specific_data->last_send_success_timestamp)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd send failed but has succeeded before"));
	    }
	  specific_data->last_send_fail_timestamp = now;
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd can't send \"%s\", not connected sock=%d"), message, specific_data->sock);
	}
    }

  if (!ret)
    {
      if (line)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd did not send \"%s\" sock=%d"), line, specific_data->sock);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd did not send NULL sock=%d"), specific_data->sock);
	}
    }

  if (line)
    {
      LW6SYS_FREE (sys_context, line);
    }

  return ret;
}

int
_mod_tcpd_can_send (_mod_tcpd_context_t * tcpd_context, lw6cnx_connection_t * connection)
{
  int ret = 0;
  _mod_tcpd_specific_data_t *specific_data = (_mod_tcpd_specific_data_t *) connection->backend_specific_data;

  ret = lw6net_socket_is_valid (sys_context, specific_data->sock);

  return ret;
}

void
_mod_tcpd_poll (_mod_tcpd_context_t * tcpd_context, lw6cnx_connection_t * connection)
{
  _mod_tcpd_specific_data_t *specific_data = (_mod_tcpd_specific_data_t *) connection->backend_specific_data;;
  char buffer[LW6SRV_CONTENT_BUFFER_SIZE + 1];
  char *envelope_line = NULL;
  char *msg = NULL;
  u_int32_t physical_ticket_sig = 0;
  u_int32_t logical_ticket_sig = 0;
  u_int64_t physical_from_id = 0;
  u_int64_t physical_to_id = 0;
  u_int64_t logical_from_id = 0;
  u_int64_t logical_to_id = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd poll"));
  if (lw6net_socket_is_valid (sys_context, specific_data->sock))
    {
      /*
       * Note: we don't explicitely close socket here, even if it's not
       * alive, in fact it's the recv/send code that will do it if needed
       * as it acts on the pointer directly.
       */
      if (lw6net_tcp_is_alive (sys_context, &(specific_data->sock)))
	{
	  /*
	   * Recv
	   */
	  memset (buffer, 0, LW6SRV_CONTENT_BUFFER_SIZE + 1);
	  if (lw6net_tcp_peek (sys_context, &(specific_data->sock), buffer, LW6SRV_CONTENT_BUFFER_SIZE, 0))
	    {
	      if (strchr (buffer, '\n'))
		{
		  envelope_line = lw6net_recv_line_tcp (sys_context, &(specific_data->sock));
		  if (envelope_line)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd received envelope \"%s\""), envelope_line);
		      if (lw6msg_envelope_analyse
			  (envelope_line, LW6MSG_ENVELOPE_MODE_TELNET,
			   connection->local_url, connection->password,
			   connection->remote_id_int,
			   connection->local_id_int, &msg,
			   &physical_ticket_sig, &logical_ticket_sig, &physical_from_id, &physical_to_id, &logical_from_id, &logical_to_id, NULL))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd analysed msg \"%s\""), msg);
			  if (connection->recv_callback_func)
			    {
			      connection->recv_callback_func
				(connection->recv_callback_data,
				 (void *) connection, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id, msg);
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no recv callback defined"));
			    }
			  LW6SYS_FREE (sys_context, msg);
			}
		      LW6SYS_FREE (sys_context, envelope_line);
		    }
		}
	    }
	}
    }
}
