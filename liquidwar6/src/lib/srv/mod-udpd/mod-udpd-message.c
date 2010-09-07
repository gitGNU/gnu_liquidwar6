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
_mod_udpd_send (_udpd_context_t * udpd_context,
		lw6cnx_connection_t * connection,
		u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
		u_int64_t logical_from_id, u_int64_t logical_to_id,
		char *message)
{
  int ret = 0;
  _udpd_specific_data_t *specific_data =
    (_udpd_specific_data_t *) connection->backend_specific_data;
  char *line;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("mod_udpd send \"%s\""), message);
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
      if (lw6net_send_line_udp
	  (specific_data->sock, line, connection->remote_ip,
	   specific_data->remote_port))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("mod_udpd sent \"%s\""), line);
	  ret = 1;
	}
      LW6SYS_FREE (line);
    }

  return ret;
}

void
_mod_udpd_poll (_udpd_context_t * udpd_context,
		lw6cnx_connection_t * connection)
{
  lw6sys_log (LW6SYS_LOG_DEBUG, _("mod_udpd poll"));
  /*
   * No real polling here, polling is done in the main loop, 
   * at the listener level.
   */
}
