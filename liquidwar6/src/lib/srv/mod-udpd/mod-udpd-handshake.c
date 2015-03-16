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

#include "../srv.h"
#include "mod-udpd-internal.h"

int
_mod_udpd_analyse_tcp (lw6sys_context_t * sys_context, _mod_udpd_context_t * udpd_context,
		       lw6srv_tcp_accepter_t * tcp_accepter, lw6nod_info_t * node_info, u_int64_t * remote_id, char **remote_url)
{
  int ret = 0;

  if (remote_id)
    {
      (*remote_id) = 0;
    }
  if (remote_url)
    {
      (*remote_url) = NULL;
    }

  /*
   * TCP is never handled by mod_udpd.
   */

  return ret;
}

int
_mod_udpd_analyse_udp (lw6sys_context_t * sys_context, _mod_udpd_context_t * udpd_context,
		       lw6srv_udp_buffer_t * udp_buffer, lw6nod_info_t * node_info, u_int64_t * remote_id, char **remote_url)
{
  int ret = 0;
  char *line = NULL;
  char *msg = NULL;

  line = udp_buffer->line;
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trying to recognize udpd protocol in \"%s\""), line);

  if (remote_id)
    {
      (*remote_id) = 0;
    }
  if (remote_url)
    {
      (*remote_url) = NULL;
    }

  if (lw6sys_str_starts_with_no_case (sys_context, line,
				      LW6MSG_OOB_PING)
      || lw6sys_str_starts_with_no_case (sys_context, line, LW6MSG_OOB_INFO) || lw6sys_str_starts_with_no_case (sys_context, line, LW6MSG_OOB_LIST))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("recognized udpd protocol (OOB)"));
      ret |= (LW6SRV_ANALYSE_UNDERSTANDABLE | LW6SRV_ANALYSE_OOB);
    }

  if (lw6sys_str_starts_with_no_case (sys_context, line, LW6MSG_LW6))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("recognized udpd protocol"));
      ret |= LW6SRV_ANALYSE_UNDERSTANDABLE;
      if (lw6msg_envelope_analyse
	  (sys_context, line, LW6MSG_ENVELOPE_MODE_TELNET,
	   node_info->const_info.ref_info.url, node_info->const_info.password,
	   0, node_info->const_info.ref_info.id_int, &msg, NULL, NULL, remote_id, NULL, NULL, NULL, remote_url))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("udpd message \"%s\" OK"), line);
	  if (msg)
	    {
	      /*
	       * We need to pass msg else remote_url isn't processed
	       */
	      LW6SYS_FREE (sys_context, msg);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to analyse message \"%s\""), line);
	  ret |= LW6SRV_ANALYSE_DEAD;
	}
    }

  return ret;
}

int
_mod_udpd_feed_with_tcp (lw6sys_context_t * sys_context, _mod_udpd_context_t * udpd_context, lw6cnx_connection_t * connection,
			 lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to feed mod_udpd with tcp data"));

  return ret;
}

int
_mod_udpd_feed_with_udp (lw6sys_context_t * sys_context, _mod_udpd_context_t * udpd_context, lw6cnx_connection_t * connection,
			 lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;
  _mod_udpd_specific_data_t *specific_data = (_mod_udpd_specific_data_t *) connection->backend_specific_data;
  char *envelope_line = NULL;
  char *msg = NULL;
  u_int32_t physical_ticket_sig = 0;
  u_int32_t logical_ticket_sig = 0;
  u_int64_t physical_from_id = 0;
  u_int64_t physical_to_id = 0;
  u_int64_t logical_from_id = 0;
  u_int64_t logical_to_id = 0;
  lw6cnx_packet_t *packet = NULL;

  envelope_line = udp_buffer->line;
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_udpd received envelope \"%s\""), envelope_line);
  if (lw6msg_envelope_analyse
      (sys_context, envelope_line, LW6MSG_ENVELOPE_MODE_TELNET, connection->local_url,
       connection->password, connection->remote_id_int,
       connection->local_id_int, &msg, &physical_ticket_sig, &logical_ticket_sig, &physical_from_id, &physical_to_id, &logical_from_id, &logical_to_id, NULL))
    {
      ret = 1;

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_udpd analysed msg \"%s\""), msg);
      /*
       * Now that we've received a packet we set the
       * connection remote port to the port it came
       * from, this way we'll send back data to the
       * right socket. This enables mod_udp to work
       * even if mod_udpd isn't activated.
       */
      specific_data->remote_port = udp_buffer->client_id.client_port;
      packet = lw6cnx_packet_new (sys_context, logical_ticket_sig, physical_ticket_sig, logical_from_id, logical_to_id, msg);
      if (packet && connection->recv_list)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("pushing msg \"%s\" to list_r"), msg);
	  lw6sys_list_r_push_front (sys_context, connection->recv_list, packet);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to push msg \"%s\" to list_r packet=%p recv_list=%p"), msg, packet, connection->recv_list);
	}
      LW6SYS_FREE (sys_context, msg);
    }

  return ret;
}
