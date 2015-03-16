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
#include "mod-httpd-internal.h"

int
_mod_httpd_send (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context,
		 lw6cnx_connection_t * connection,
		 int64_t now,
		 u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message)
{
  int ret = 0;
  _mod_httpd_specific_data_t *specific_data = (_mod_httpd_specific_data_t *) connection->backend_specific_data;
  char *line;
  char *tmp = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_httpd send \"%s\""), message);
  line =
    lw6msg_envelope_generate (sys_context, LW6MSG_ENVELOPE_MODE_TELNET,
			      lw6sys_build_get_version (),
			      connection->password_send_checksum,
			      physical_ticket_sig, logical_ticket_sig,
			      connection->local_id_int, connection->remote_id_int, logical_from_id, logical_to_id, message);
  if (line)
    {
      if (lw6cnx_connection_lock_send (sys_context, connection))
	{
	  if (specific_data->send_buffer)
	    {
	      tmp = lw6sys_new_sprintf (sys_context, "%s%s\n", specific_data->send_buffer, line);
	      if (tmp)
		{
		  LW6SYS_FREE (sys_context, specific_data->send_buffer);
		  specific_data->send_buffer = tmp;
		  tmp = NULL;
		  ret = 1;
		}
	    }
	  else
	    {
	      specific_data->send_buffer = lw6sys_new_sprintf (sys_context, "%s\n", line);
	      if (specific_data->send_buffer)
		{
		  ret = 1;
		}
	    }

	  if (ret)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_httpd scheduled send \"%s\""), line);
	    }
	  lw6cnx_connection_unlock_send (sys_context, connection);
	}
      LW6SYS_FREE (sys_context, line);
    }

  return ret;
}

int
_mod_httpd_can_send (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6cnx_connection_t * connection)
{
  return 1;
}

void
_mod_httpd_poll (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6cnx_connection_t * connection)
{
  _mod_httpd_specific_data_t *specific_data = (_mod_httpd_specific_data_t *) connection->backend_specific_data;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_httpd poll"));

  lw6sys_list_filter (sys_context, &(specific_data->reply_threads), _mod_httpd_reply_thread_filter, NULL);
}
