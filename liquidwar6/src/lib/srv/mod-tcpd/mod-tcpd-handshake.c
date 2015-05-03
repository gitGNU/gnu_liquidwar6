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
#include "mod-tcpd-internal.h"

int
_mod_tcpd_analyse_tcp (lw6sys_context_t * sys_context, _mod_tcpd_context_t * tcpd_context,
		       lw6srv_tcp_accepter_t * tcp_accepter, lw6nod_info_t * node_info, u_int64_t * remote_id, char **remote_url)
{
  int ret = 0;
  char *line = NULL;
  char *msg = NULL;

  line = tcp_accepter->first_line;
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trying to recognize tcpd protocol in \"%s\""), line);

  if (remote_id)
    {
      (*remote_id) = 0;
    }
  if (remote_url)
    {
      (*remote_url) = NULL;
    }

  if (lw6net_tcp_is_alive (sys_context, &(tcp_accepter->sock)))
    {
      if (lw6sys_chr_is_eol (line[0])
	  || lw6sys_str_starts_with_no_case (sys_context, line,
					     LW6MSG_OOB_PING)
	  || lw6sys_str_starts_with_no_case (sys_context, line, LW6MSG_OOB_INFO) || lw6sys_str_starts_with_no_case (sys_context, line, LW6MSG_OOB_LIST))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("recognized tcpd protocol (OOB)"));
	  ret |= (LW6SRV_ANALYSE_UNDERSTANDABLE | LW6SRV_ANALYSE_OOB);
	}

      if (lw6sys_str_starts_with_no_case (sys_context, line, _MOD_TCPD_PROTOCOL_LW6_STRING))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("recognized tcpd protocol"));
	  if (lw6msg_envelope_analyse
	      (sys_context, line, LW6MSG_ENVELOPE_MODE_TELNET,
	       node_info->const_info.ref_info.url,
	       node_info->const_info.password, 0, node_info->const_info.ref_info.id_int, &msg, NULL, NULL, remote_id, NULL, NULL, NULL, remote_url))
	    {
	      ret |= LW6SRV_ANALYSE_UNDERSTANDABLE;
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("tcpd message \"%s\" OK"), line);
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
	      if (strchr (line, '\n'))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to analyse message \"%s\""), line);
		  ret |= LW6SRV_ANALYSE_DEAD;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_
			      ("unable to analyse message \"%s\" but line does not seem complete, assuming some data is still missing, giving it another chance"),
			      line);
		}
	    }
	}
    }
  else
    {
      ret |= LW6SRV_ANALYSE_DEAD;
    }

  return ret;
}

int
_mod_tcpd_analyse_udp (lw6sys_context_t * sys_context, _mod_tcpd_context_t * tcpd_context,
		       lw6srv_udp_buffer_t * udp_buffer, lw6nod_info_t * node_info, u_int64_t * remote_id, char **remote_url)
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
   * UDP is never handled by mod_tcpd, return always false
   */

  return ret;
}

int
_mod_tcpd_feed_with_tcp (lw6sys_context_t * sys_context, _mod_tcpd_context_t * tcpd_context, lw6cnx_connection_t * connection,
			 lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;
  _mod_tcpd_specific_data_t *specific_data = (_mod_tcpd_specific_data_t *) connection->backend_specific_data;;
  int tmp_sock = LW6NET_SOCKET_INVALID;

  if (specific_data)
    {
      tmp_sock = tcp_accepter->sock;
      if (lw6net_socket_is_valid (sys_context, specific_data->sock))
	{
	  if (!lw6net_tcp_is_alive (sys_context, &(specific_data->sock)))
	    {
	      /*
	       * Close our old socket, use the new one
	       */
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("old connection dead, replacing with new connection, using sock %d"), tmp_sock);
	      tcp_accepter->sock = LW6NET_SOCKET_INVALID;
	      specific_data->sock = tmp_sock;
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_ ("double connection from \"%s\" (%s:%d), ignoring"), connection->remote_url, connection->remote_ip, connection->remote_port);
	      lw6net_socket_close (sys_context, &(tcp_accepter->sock));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new connection, using sock %d"), tmp_sock);
	  tcp_accepter->sock = LW6NET_SOCKET_INVALID;
	  specific_data->sock = tmp_sock;
	  ret = 1;
	}
    }

  /*
   * Note, we don't even bother to fetch data here, in fact, the
   * poll loop will just do it next time it's called, no hurry,
   * this is just first connection, speed battle is held elsewhere.
   */

  return ret;
}

int
_mod_tcpd_feed_with_udp (lw6sys_context_t * sys_context, _mod_tcpd_context_t * tcpd_context, lw6cnx_connection_t * connection,
			 lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  /*
   * UDP is never handled by mod_tcpd, return always false
   */

  return ret;
}
