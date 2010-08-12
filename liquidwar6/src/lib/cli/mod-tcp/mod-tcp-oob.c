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

#include "../cli.h"
#include "mod-tcp-internal.h"

static int
_do_ping (_tcp_context_t * tcp_context, lw6nod_info_t * node_info,
	  lw6cli_oob_data_t * oob_data, char *url, lw6sys_url_t * parsed_url,
	  char *ip)
{
  int ret = 0;
  int sock;
  char *request = NULL;
  char *response = NULL;
  char *given_url = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("connecting in TCP on %s:%d"), ip,
	      parsed_url->port);
  sock = lw6net_tcp_connect (ip, parsed_url->port);
  if (sock >= 0)
    {
      request =
	lw6sys_new_sprintf ("%s %s", LW6MSG_OOB_PING,
			    node_info->const_info.url);
      if (request)
	{
	  if (lw6net_send_line_tcp (sock, request))
	    {
	      while (!ret
		     && _mod_tcp_oob_should_continue (tcp_context, oob_data)
		     && ((response = lw6net_recv_line_tcp (sock)) == NULL))
		{
		  lw6sys_idle ();
		}
	      if (response)
		{
		  if (_mod_tcp_oob_should_continue (tcp_context, oob_data))
		    {
		      given_url = lw6msg_oob_analyse_pong (response);
		      if (given_url)
			{
			  if (lw6sys_str_is_same (url, given_url))
			    {
			      // OK, they are the same
			      ret = 1;
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_DEBUG,
					  _
					  ("connected on %s:%d using \"%s\" but server reports \"%s\""),
					  ip, parsed_url->port, url,
					  given_url);
			      lw6nod_info_add_discovered_node (node_info,
							       given_url);
			    }
			  LW6SYS_FREE (given_url);
			}
		      ret = 1;
		    }
		  LW6SYS_FREE (response);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG, _("no response from %s:%d"),
			      ip, parsed_url->port);
		}
	    }
	  LW6SYS_FREE (request);
	}
      lw6net_socket_close (sock);
    }

  return ret;
}

int
_mod_tcp_process_oob (_tcp_context_t * tcp_context, lw6nod_info_t * node_info,
		      lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  lw6sys_url_t *parsed_url = NULL;
  char *ip = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("process tcp oob url=\"%s\""),
	      oob_data->public_url);

  parsed_url = lw6sys_url_parse (oob_data->public_url);
  if (parsed_url)
    {
      ip = lw6net_dns_gethostbyname (parsed_url->host);
      if (ip)
	{
	  if (_mod_tcp_oob_should_continue (tcp_context, oob_data))
	    {
	      if (_do_ping
		  (tcp_context, node_info, oob_data, oob_data->public_url,
		   parsed_url, ip))
		{
		  // OK
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _("done with node \"%s\" after ping"));
		}
	    }
	  LW6SYS_FREE (ip);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("unable to resolve host \"%s\""),
		      parsed_url->host);
	}
      lw6sys_url_free (parsed_url);
    }

  return ret;
}

int
_mod_tcp_oob_should_continue (_tcp_context_t * tcp_context,
			      lw6cli_oob_data_t * oob_data)
{
  int ret = 0;

  ret = (_mod_tcp_timeout_ok (tcp_context,
			      oob_data->creation_timestamp)
	 && (!oob_data->do_not_finish));

  return ret;
}
