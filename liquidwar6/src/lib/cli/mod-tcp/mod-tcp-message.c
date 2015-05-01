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

#include "../cli.h"
#include "mod-tcp-internal.h"

int
_mod_tcp_send (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context,
	       lw6cnx_connection_t * connection,
	       int64_t now,
	       u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message)
{
  int ret = 0;
  _mod_tcp_specific_data_t *specific_data = (_mod_tcp_specific_data_t *) connection->backend_specific_data;
  char *line = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp send \"%s\""), message);
  line =
    lw6msg_envelope_generate (sys_context, LW6MSG_ENVELOPE_MODE_TELNET,
			      lw6sys_build_get_version (),
			      connection->password_send_checksum,
			      physical_ticket_sig, logical_ticket_sig,
			      connection->local_id_int, connection->remote_id_int, logical_from_id, logical_to_id, message);
  if (line)
    {
      if (specific_data->state == _MOD_TCP_STATE_CONNECTED && lw6net_socket_is_valid (sys_context, specific_data->sock))
	{
	  if (lw6cnx_connection_lock_send (sys_context, connection))
	    {
	      if (lw6net_send_line_tcp (sys_context, &(specific_data->sock), line))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp sent \"%s\""), line);
		  specific_data->last_send_success_timestamp = now;
		  ret = 1;
		}
	      else
		{
		  specific_data->last_send_fail_timestamp = now;
		}
	      lw6cnx_connection_unlock_send (sys_context, connection);
	    }
	}
      else
	{
	  if (specific_data->last_send_success_timestamp)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp send failed but has succeeded before"));
	    }
	  specific_data->last_send_fail_timestamp = now;
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_ ("mod_tcp can't send \"%s\", not connected sock=%d state=%d"), message, specific_data->sock, specific_data->state);
	}
    }

  if (!ret)
    {
      if (line)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp did not send \"%s\" sock=%d state=%d"), line, specific_data->sock, specific_data->state);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp did not send NULL sock=%d state=%d"), specific_data->sock, specific_data->state);
	}
    }

  if (line)
    {
      LW6SYS_FREE (sys_context, line);
    }

  return ret;
}

int
_mod_tcp_can_send (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context, lw6cnx_connection_t * connection)
{
  int ret = 0;
  _mod_tcp_specific_data_t *specific_data = (_mod_tcp_specific_data_t *) connection->backend_specific_data;

  ret = specific_data->state == _MOD_TCP_STATE_CONNECTED && lw6net_socket_is_valid (sys_context, specific_data->sock);

  return ret;
}

void
_mod_tcp_poll (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context, lw6cnx_connection_t * connection)
{
  _mod_tcp_specific_data_t *specific_data = (_mod_tcp_specific_data_t *) connection->backend_specific_data;
  _mod_tcp_connect_data_t *connect_data = NULL;
  char buffer[LW6CLI_CONTENT_BUFFER_SIZE + 1];
  char *envelope_line = NULL;
  char *msg = NULL;
  u_int32_t physical_ticket_sig = 0;
  u_int32_t logical_ticket_sig = 0;
  u_int64_t physical_from_id = 0;
  u_int64_t physical_to_id = 0;
  u_int64_t logical_from_id = 0;
  u_int64_t logical_to_id = 0;
  lw6cnx_packet_t *packet = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp poll sock=%d state=%d"), specific_data->sock, specific_data->state);
  switch (specific_data->state)
    {
    case _MOD_TCP_STATE_CLOSED:
      connect_data = (_mod_tcp_connect_data_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_tcp_connect_data_t));
      if (connect_data)
	{
	  connect_data->tcp_context = tcp_context;
	  connect_data->connection = connection;
	  specific_data->state = _MOD_TCP_STATE_CONNECTING;
	  specific_data->connect_thread = lw6sys_thread_create (sys_context, _mod_tcp_connect_func, NULL, connect_data);
	  if (specific_data->connect_thread)
	    {
	      // OK
	    }
	  else
	    {
	      specific_data->state = _MOD_TCP_STATE_CLOSED;
	      LW6SYS_FREE (sys_context, connect_data);
	    }
	}
      break;
    case _MOD_TCP_STATE_CONNECTING:
      // nothing to do, just wait until it's connected or not
      break;
    case _MOD_TCP_STATE_CONNECT_DONE:
      if (specific_data->connect_thread)
	{
	  lw6sys_thread_join (sys_context, specific_data->connect_thread);
	  specific_data->connect_thread = NULL;
	}
      if (lw6net_socket_is_valid (sys_context, specific_data->sock))
	{
	  specific_data->state = _MOD_TCP_STATE_CONNECTED;
	  /*
	   * We schedule a foo/bar soon so that connection does not stay
	   * "idle" and fires a server error because of a timeout.
	   */
	  lw6cnx_connection_init_foo_bar_key (sys_context, connection, lw6sys_get_timestamp (sys_context), LW6SYS_SLEEP_DELAY);
	}
      else
	{
	  specific_data->state = _MOD_TCP_STATE_CLOSED;
	}
      break;
    case _MOD_TCP_STATE_CONNECTED:
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
	      memset (buffer, 0, LW6CLI_CONTENT_BUFFER_SIZE + 1);
	      if (lw6net_tcp_peek (sys_context, &(specific_data->sock), buffer, LW6CLI_CONTENT_BUFFER_SIZE, 0))
		{
		  if (strchr (buffer, '\n'))
		    {
		      envelope_line = lw6net_recv_line_tcp (sys_context, &(specific_data->sock));
		      if (envelope_line)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp received envelope \"%s\""), envelope_line);
			  if (lw6msg_envelope_analyse
			      (sys_context, envelope_line, LW6MSG_ENVELOPE_MODE_TELNET,
			       connection->local_url, connection->password,
			       connection->remote_id_int,
			       connection->local_id_int, &msg,
			       &physical_ticket_sig, &logical_ticket_sig, &physical_from_id, &physical_to_id, &logical_from_id, &logical_to_id, NULL))
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_tcp analysed msg \"%s\""), msg);
			      packet = lw6cnx_packet_new (sys_context, logical_ticket_sig, physical_ticket_sig, logical_from_id, logical_to_id, msg);
			      if (packet && connection->recv_list)
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("pushing msg \"%s\" to list_r"), msg);
				  lw6sys_list_r_push_front (sys_context, connection->recv_list, packet);
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to push msg \"%s\" to list_r packet=%p recv_list=%p"), msg, packet,
					      connection->recv_list);
				}
			      LW6SYS_FREE (sys_context, msg);
			    }
			  LW6SYS_FREE (sys_context, envelope_line);
			}
		    }
		}
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("invalid socket %d, closing "), specific_data->sock);
	  specific_data->sock = LW6NET_SOCKET_INVALID;
	  specific_data->state = _MOD_TCP_STATE_CLOSED;
	}
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unvalid state %d"), specific_data->state);
    }
}
