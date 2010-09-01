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
#include "mod-udpd-internal.h"

int
_mod_udpd_analyse_tcp (_udpd_context_t * udpd_context,
		       lw6srv_tcp_accepter_t * tcp_accepter,
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
   * TCP is never handled by mod_udpd.
   */

  return ret;
}

int
_mod_udpd_analyse_udp (_udpd_context_t * udpd_context,
		       lw6srv_udp_buffer_t * udp_buffer,
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

  if (lw6sys_str_starts_with_no_case (udp_buffer->line,
				      LW6MSG_OOB_PING)
      || lw6sys_str_starts_with_no_case (udp_buffer->line,
					 LW6MSG_OOB_INFO)
      || lw6sys_str_starts_with_no_case (udp_buffer->line, LW6MSG_OOB_LIST))
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("recognized tcpd protocol (OOB)"));
      ret |= (LW6SRV_ANALYSE_UNDERSTANDABLE | LW6SRV_ANALYSE_OOB);
    }

  return ret;
}

int
_mod_udpd_feed_with_tcp (_udpd_context_t * udpd_context,
			 lw6cnx_connection_t * connection,
			 lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;

  // todo

  return ret;
}

int
_mod_udpd_feed_with_udp (_udpd_context_t * udpd_context,
			 lw6cnx_connection_t * connection,
			 lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  // todo

  return ret;
}
