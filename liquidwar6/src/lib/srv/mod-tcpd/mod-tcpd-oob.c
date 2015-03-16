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
_mod_tcpd_process_oob (lw6sys_context_t * sys_context, _mod_tcpd_context_t * tcpd_context, lw6nod_info_t * node_info, lw6srv_oob_data_t * oob_data)
{
  int ret = 0;
  char *request_line = NULL;
  int syntax_ok = 0;
  char *command = NULL;
  int password_ok = 0;
  char *given_public_url = NULL;
  char *response = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process tcpd oob"));
  if (_mod_tcpd_oob_should_continue (sys_context, tcpd_context, oob_data))
    {
      request_line = lw6net_recv_line_tcp (sys_context, &(oob_data->sock));
      if (request_line)
	{
	  if (lw6msg_oob_analyse_request (sys_context, &syntax_ok, &command, &password_ok,
					  &given_public_url, request_line, node_info->const_info.ref_info.url, node_info->const_info.password))
	    {
	      if (lw6sys_str_is_same_no_case (sys_context, command, LW6MSG_OOB_PING))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("mod_tcpd %s response to %s:%d"), LW6MSG_OOB_PONG, oob_data->remote_ip, oob_data->remote_port);
		  response = lw6msg_oob_generate_pong (sys_context, node_info);
		}
	      if (lw6sys_str_is_same_no_case (sys_context, command, LW6MSG_OOB_INFO))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("mod_tcpd %s response to %s:%d"), LW6MSG_OOB_INFO, oob_data->remote_ip, oob_data->remote_port);
		  response = lw6msg_oob_generate_info (sys_context, node_info);
		}
	      if (lw6sys_str_is_same_no_case (sys_context, command, LW6MSG_OOB_LIST))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("mod_tcpd %s response to %s:%d"), LW6MSG_OOB_LIST, oob_data->remote_ip, oob_data->remote_port);
		  response = lw6msg_oob_generate_list (sys_context, node_info);
		}
	      if (given_public_url)
		{
		  if (strlen (given_public_url) > 0)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("discovered node \"%s\" from given url"), given_public_url);
		      lw6nod_info_add_discovered_node (sys_context, node_info, given_public_url);
		    }
		  LW6SYS_FREE (sys_context, given_public_url);
		}
	    }
	  else
	    {
	      if (strlen (request_line) == 0 || lw6sys_chr_is_eol (request_line[0]))
		{
		  if (node_info->const_info.password && strlen (node_info->const_info.password) > 0)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("mod_tcpd %s response to %s:%d"), LW6MSG_FORBIDDEN, oob_data->remote_ip, oob_data->remote_port);
		      response = lw6sys_new_sprintf (sys_context, "%s\n", LW6MSG_FORBIDDEN);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("mod_tcpd %s response to %s:%d"), LW6MSG_OOB_INFO, oob_data->remote_ip, oob_data->remote_port);
		      response = lw6msg_oob_generate_info (sys_context, node_info);
		    }
		}
	      else
		{
		  if (syntax_ok && !password_ok)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("mod_tcpd %s response to %s:%d"), LW6MSG_FORBIDDEN, oob_data->remote_ip, oob_data->remote_port);
		      response = lw6sys_new_sprintf (sys_context, "%s\n", LW6MSG_FORBIDDEN);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("mod_tcpd %s response to %s:%d"), LW6MSG_ERROR, oob_data->remote_ip, oob_data->remote_port);
		      response = lw6sys_new_sprintf (sys_context, "%s\n", LW6MSG_ERROR);
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, request_line);
	}
    }

  if (lw6net_tcp_is_alive (sys_context, &(oob_data->sock)))
    {
      if (response)
	{
	  lw6net_tcp_send (sys_context, &(oob_data->sock), response, strlen (response), tcpd_context->data.consts.error_timeout * 1000, 1);
	}
      else
	{
	  lw6net_send_line_tcp (sys_context, &(oob_data->sock), LW6MSG_ERROR);
	}
    }

  if (response)
    {
      LW6SYS_FREE (sys_context, response);
    }

  lw6net_socket_close (sys_context, &(oob_data->sock));

  return ret;
}

int
_mod_tcpd_oob_should_continue (lw6sys_context_t * sys_context, _mod_tcpd_context_t * tcpd_context, lw6srv_oob_data_t * oob_data)
{
  int ret = 0;

  ret = (_mod_tcpd_timeout_ok (sys_context, tcpd_context, oob_data->creation_timestamp) && lw6net_tcp_is_alive (sys_context, &(oob_data->sock))
	 && (!oob_data->do_not_finish));

  return ret;
}
