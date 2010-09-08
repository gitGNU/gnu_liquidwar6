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

#include "../srv.h"
#include "mod-tcpd-internal.h"

int
_mod_tcpd_analyse_tcp (_tcpd_context_t * tcpd_context,
		       lw6srv_tcp_accepter_t * tcp_accepter,
		       lw6nod_info_t * node_info,
		       u_int64_t * remote_id, char **remote_url)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("trying to recognize tcpd protocol"));

  if (remote_id)
    {
      (*remote_id) = 0;
    }
  if (remote_url)
    {
      (*remote_url) = NULL;
    }

  if (!lw6net_tcp_is_alive (tcp_accepter->sock))
    {
      ret |= LW6SRV_ANALYSE_DEAD;
      lw6net_socket_close (tcp_accepter->sock);
      tcp_accepter->sock = -1;
    }

  if (!strncmp
      (tcp_accepter->first_line, _MOD_TCPD_PROTOCOL_LW6_STRING,
       _MOD_TCPD_PROTOCOL_LW6_SIZE))
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("recognized tcpd protocol"));
      ret |= LW6SRV_ANALYSE_UNDERSTANDABLE;
    }
  if (lw6sys_chr_is_eol (tcp_accepter->first_line[0])
      || lw6sys_str_starts_with_no_case (tcp_accepter->first_line,
					 LW6MSG_OOB_PING)
      || lw6sys_str_starts_with_no_case (tcp_accepter->first_line,
					 LW6MSG_OOB_INFO)
      || lw6sys_str_starts_with_no_case (tcp_accepter->first_line,
					 LW6MSG_OOB_LIST))
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("recognized tcpd protocol (OOB)"));
      ret |= (LW6SRV_ANALYSE_UNDERSTANDABLE | LW6SRV_ANALYSE_OOB);
    }

  return ret;
}

int
_mod_tcpd_analyse_udp (_tcpd_context_t * tcpd_context,
		       lw6srv_udp_buffer_t * udp_buffer,
		       lw6nod_info_t * node_info,
		       u_int64_t * remote_id, char **remote_url)
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
_mod_tcpd_feed_with_tcp (_tcpd_context_t * tcpd_context,
			 lw6cnx_connection_t * connection,
			 lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;

  // todo

  return ret;
}

int
_mod_tcpd_feed_with_udp (_tcpd_context_t * tcpd_context,
			 lw6cnx_connection_t * connection,
			 lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  // todo

  return ret;
}
