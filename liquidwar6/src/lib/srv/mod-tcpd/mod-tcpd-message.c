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
		u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
		u_int64_t logical_from_id, u_int64_t logical_to_id,
		const char *message)
{
  int ret = 0;
  _mod_tcpd_specific_data_t *specific_data =
    (_mod_tcpd_specific_data_t *) connection->backend_specific_data;
  char *line;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd send \"%s\""), message);
  line = lw6msg_envelope_generate (LW6MSG_ENVELOPE_MODE_TELNET,
				   lw6sys_build_get_version (),
				   connection->password_send_checksum,
				   physical_ticket_sig,
				   logical_ticket_sig,
				   connection->local_id_int,
				   connection->remote_id_int,
				   logical_from_id, logical_to_id, message);
  if (line)
    {
      if (lw6cnx_connection_lock_send (connection))
	{
	  if (lw6net_send_line_tcp (&(specific_data->sock), line))
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd sent \"%s\""),
			  line);
	      ret = 1;
	    }
	  else
	    {
	      /*
	       * Testing is_valid is enough here, is_alive makes no sense,
	       * we have the result from the previous send, should it fail,
	       * sock would have been set to an invalid value already.
	       * Usually, when send fails but socket is still valid,
	       * it means the buffer is full, in a general manner, try
	       * later. That means for us -> put in backlog.
	       */
	      if (lw6net_socket_is_valid (specific_data->sock))
		{
		  /*
		   * Put stuff in backlog because the send failed
		   */
		  lw6cnx_backlog_push (&(specific_data->backlog), line);
		  // don't want to free this as it's in the backlog
		  line = NULL;

		  /*
		   * The following test is here only to
		   * give a meaningfull message
		   */
		  if (now > specific_data->last_send_failed_timestamp)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("mod_tcpd send put line in backlog since TCP low-level call failed"));
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("mod_tcpd send put line in backlog since TCP low-level call was not even called, because it has failed very recently"));
		    }

		  ret = 1;
		}
	      specific_data->last_send_failed_timestamp = now;
	    }
	  lw6cnx_connection_unlock_send (connection);
	}
      if (line)
	{
	  LW6SYS_FREE (line);
	}
    }

  return ret;
}

static int
_backlog_filter_func (lw6cnx_backlog_filter_data_t * backlog_filter_data,
		      char *str)
{
  int ret = 1;
  _mod_tcpd_specific_data_t *specific_data =
    backlog_filter_data->specific_data;

  if (backlog_filter_data->now > specific_data->last_send_failed_timestamp)
    {
      if (lw6net_send_line_tcp (&(specific_data->sock), str))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("mod_tcp sent \"%s\" from backlog sock=%d"),
		      str, specific_data->sock);
	  /*
	   * Setting ret to 0, since sent succeeded, we want the line to
	   * be removed.
	   */
	  ret = 0;
	}
      else
	{
	  specific_data->last_send_failed_timestamp =
	    backlog_filter_data->now;
	}
    }

  return ret;
}

void
_mod_tcpd_poll (_mod_tcpd_context_t * tcpd_context,
		lw6cnx_connection_t * connection)
{
  _mod_tcpd_specific_data_t *specific_data =
    (_mod_tcpd_specific_data_t *) connection->backend_specific_data;;
  char buffer[LW6SRV_CONTENT_BUFFER_SIZE + 1];
  char *envelope_line = NULL;
  char *msg = NULL;
  u_int32_t physical_ticket_sig = 0;
  u_int32_t logical_ticket_sig = 0;
  u_int64_t physical_from_id = 0;
  u_int64_t physical_to_id = 0;
  u_int64_t logical_from_id = 0;
  u_int64_t logical_to_id = 0;
  lw6cnx_backlog_filter_data_t backlog_filter_data;

  memset (&backlog_filter_data, 0, sizeof (lw6cnx_backlog_filter_data_t));

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("mod_tcpd poll"));
  if (lw6net_socket_is_valid (specific_data->sock))
    {
      /*
       * Note: we don't explicitely close socket here, even if it's not
       * alive, in fact it's the recv/send code that will do it if needed
       * as it acts on the pointer directly.
       */
      if (lw6net_tcp_is_alive (&(specific_data->sock)))
	{
	  /*
	   * Send
	   */
	  if (lw6cnx_connection_lock_send (connection))
	    {
	      backlog_filter_data.specific_data = specific_data;
	      backlog_filter_data.now = lw6sys_get_timestamp ();
	      lw6cnx_backlog_filter (&(specific_data->backlog),
				     _backlog_filter_func,
				     &backlog_filter_data);

	      lw6cnx_connection_unlock_send (connection);
	    }

	  /*
	   * Recv
	   */
	  memset (buffer, 0, LW6SRV_CONTENT_BUFFER_SIZE + 1);
	  if (lw6net_tcp_peek
	      (&(specific_data->sock), buffer, LW6SRV_CONTENT_BUFFER_SIZE, 0))
	    {
	      if (strchr (buffer, '\n'))
		{
		  envelope_line =
		    lw6net_recv_line_tcp (&(specific_data->sock));
		  if (envelope_line)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("mod_tcpd received envelope \"%s\""),
				  envelope_line);
		      if (lw6msg_envelope_analyse
			  (envelope_line, LW6MSG_ENVELOPE_MODE_TELNET,
			   connection->local_url, connection->password,
			   connection->remote_id_int,
			   connection->local_id_int, &msg,
			   &physical_ticket_sig, &logical_ticket_sig,
			   &physical_from_id, &physical_to_id,
			   &logical_from_id, &logical_to_id, NULL))
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG,
				      _x_ ("mod_tcpd analysed msg \"%s\""),
				      msg);
			  if (connection->recv_callback_func)
			    {
			      connection->recv_callback_func
				(connection->recv_callback_data,
				 (void *) connection, physical_ticket_sig,
				 logical_ticket_sig, logical_from_id,
				 logical_to_id, msg);
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_DEBUG,
					  _x_ ("no recv callback defined"));
			    }
			  LW6SYS_FREE (msg);
			}
		      LW6SYS_FREE (envelope_line);
		    }
		}
	    }
	}
    }
}
