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
_mod_tcpd_process_oob (_tcpd_context_t * tcpd_context,
		       lw6nod_info_t * node_info,
		       lw6srv_oob_data_t * oob_data)
{
  int ret = 0;
  char *request_line = NULL;
  int syntax_ok = 0;
  char *command = NULL;
  int password_ok = 0;
  char *given_public_url = NULL;
  char *response = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("process tcpd oob"));
  if (_mod_tcpd_oob_should_continue (tcpd_context, oob_data))
    {
      request_line = lw6net_recv_line_tcp (oob_data->sock);
      if (request_line)
	{
	  if (lw6msg_oob_analyse_request (&syntax_ok, &command, &password_ok,
					  &given_public_url, request_line,
					  node_info->const_info.url,
					  node_info->const_info.password))
	    {
	      if (lw6sys_str_is_same (command, LW6MSG_OOB_PING))
		{
		  response = lw6msg_oob_generate_pong (node_info);
		}
	      if (lw6sys_str_is_same (command, LW6MSG_OOB_INFO))
		{
		  response = lw6msg_oob_generate_info (node_info);
		}
	      if (lw6sys_str_is_same (command, LW6MSG_OOB_LIST))
		{
		  response = lw6msg_oob_generate_list (node_info);
		}
	      if (given_public_url)
		{
		  if (strlen (given_public_url) > 0)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _("discovered node \"%s\" from given url"),
				  given_public_url);
		      lw6nod_info_add_discovered_node (node_info,
						       given_public_url);
		    }
		  LW6SYS_FREE (given_public_url);
		}
	    }
	  else
	    {
	      if (syntax_ok && !password_ok)
		{
		  response = lw6sys_new_sprintf ("%s\n", LW6MSG_FORBIDDEN);
		}
	      else
		{
		  response = lw6sys_new_sprintf ("%s\n", LW6MSG_ERROR);
		}
	    }
	  LW6SYS_FREE (request_line);
	}
    }

  if (_mod_tcpd_oob_should_continue (tcpd_context, oob_data))
    {
      if (response)
	{
	  lw6net_tcp_send (oob_data->sock, response,
			   strlen (response), _MOD_TCPD_TIMEOUT, 1);
	}
      else
	{
	  lw6net_send_line_tcp (oob_data->sock, _MOD_TCPD_ERROR);
	}
    }

  if (response)
    {
      LW6SYS_FREE (response);
    }

  lw6net_socket_close (oob_data->sock);
  oob_data->sock = -1;

  return ret;
}

int
_mod_tcpd_oob_should_continue (_tcpd_context_t * tcpd_context,
			       lw6srv_oob_data_t * oob_data)
{
  int ret = 0;

  ret = (_mod_tcpd_timeout_ok (tcpd_context, oob_data->creation_timestamp)
	 && lw6net_tcp_is_alive (oob_data->sock)
	 && (!oob_data->do_not_finish));

  return ret;
}
