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

  lw6sys_log (LW6SYS_LOG_NOTICE, _("connecting in TCP on %s:%d"), ip,
	      parsed_url->port);
  sock = lw6net_tcp_connect (ip, parsed_url->port);
  if (sock >= 0)
    {
      request =
	lw6msg_oob_generate_request (LW6MSG_OOB_PING, url,
				     node_info->const_info.password,
				     node_info->const_info.url);
      if (request)
	{
	  if (lw6net_send_line_tcp (sock, request))
	    {
	      while (!ret
		     && _mod_tcp_oob_should_continue (tcp_context, oob_data,
						      sock)
		     && ((response = lw6net_recv_line_tcp (sock)) == NULL))
		{
		  lw6sys_idle ();
		}
	      if (response)
		{
		  if (_mod_tcp_oob_should_continue
		      (tcp_context, oob_data, sock))
		    {
		      given_url = lw6msg_oob_analyse_pong (response);
		      if (given_url)
			{
			  if (lw6sys_str_is_same (url, given_url))
			    {
			      lw6sys_log (LW6SYS_LOG_INFO,
					  _
					  ("ping successfull on %s:%d \"%s\""),
					  ip, parsed_url->port, url);
			      ret = 1;
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_NOTICE,
					  _
					  ("connected on %s:%d using \"%s\" but server reports \"%s\""),
					  ip, parsed_url->port, url,
					  given_url);
			      lw6nod_info_add_discovered_node (node_info,
							       given_url);
			    }
			  LW6SYS_FREE (given_url);
			}
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

static int
_do_info (_tcp_context_t * tcp_context, lw6nod_info_t * node_info,
	  lw6cli_oob_data_t * oob_data, char *url, lw6sys_url_t * parsed_url,
	  char *ip)
{
  int ret = 0;
  int eom = 0;
  int sock;
  char *request = NULL;
  char *response = NULL;
  lw6sys_assoc_t *assoc = NULL;
  char *remote_program = NULL;
  char *remote_version = NULL;
  char *remote_codename = NULL;
  char *remote_stamp_str = NULL;
  int remote_stamp_int = 0;
  char *remote_id = NULL;
  char *remote_url = NULL;
  char *remote_title = NULL;
  char *remote_description = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("connecting in TCP on %s:%d"), ip,
	      parsed_url->port);
  assoc = lw6sys_assoc_new (lw6sys_free_callback);
  if (assoc)
    {
      sock = lw6net_tcp_connect (ip, parsed_url->port);
      if (sock >= 0)
	{
	  request =
	    lw6msg_oob_generate_request (LW6MSG_OOB_INFO, url,
					 node_info->const_info.password,
					 node_info->const_info.url);
	  if (request)
	    {
	      if (lw6net_send_line_tcp (sock, request))
		{
		  while (_mod_tcp_oob_should_continue
			 (tcp_context, oob_data, sock) && !eom)
		    {
		      while (_mod_tcp_oob_should_continue
			     (tcp_context, oob_data, sock)
			     && ((response = lw6net_recv_line_tcp (sock)) ==
				 NULL) && !eom)
			{
			  lw6sys_idle ();
			}
		      if (response)
			{
			  if (strlen (response) == 0)
			    {
			      lw6sys_log (LW6SYS_LOG_DEBUG,
					  _("end of message detected"));
			      eom = 1;
			    }
			  else
			    {
			      if (assoc)
				{
				  if (lw6msg_utils_parse_key_value_to_assoc
				      (&assoc, response))
				    {
				      lw6sys_log (LW6SYS_LOG_NOTICE,
						  _("parsed line \"%s\""),
						  response);
				    }
				  else
				    {
				      lw6sys_log (LW6SYS_LOG_DEBUG,
						  _
						  ("unable to parse line \"%s\", ignoring"),
						  response);
				    }
				}
			    }

			  LW6SYS_FREE (response);
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG,
				      _("inconsistent response from %s:%d"),
				      ip, parsed_url->port);
			}
		    }
		}
	      LW6SYS_FREE (request);
	    }
	  lw6net_socket_close (sock);
	}
      if (assoc)
	{
	  remote_program = lw6sys_assoc_get (LW6MSG_OOB_PROGRAM);
	  remote_version = lw6sys_assoc_get (LW6MSG_OOB_PROGRAM);
	  remote_codename = lw6sys_assoc_get (LW6MSG_OOB_PROGRAM);
	  if (lw6sys_assoc_get (LW6MSG_OOB_STAMP))
	    {
	      remote_stamp_int =
		lw6sys_atoi (lw6sys_assoc_get (LW6MSG_OOB_STAMP));
	    }
	  remote_id = lw6sys_assoc_get (LW6MSG_OOB_ID);
	  remote_url = lw6sys_assoc_get (LW6MSG_OOB_URL);
	  remote_title = lw6sys_assoc_get (LW6MSG_OOB_TITLE);
	  remote_description = lw6sys_assoc_get (LW6MSG_OOB_DESCRIPTION);

	  if (remote_program && remote_version && remote_codename
	      && remote_stamp_int > 0 && remote_id && remote_url
	      && remote_title && remote_description)
	    {
	      if (lw6sys_str_is_same
		  (remote_program, lw6sys_build_get_program_tarname ()))
		{
		  if (lw6sys_str_is_same (remote_url, url))
		    {
		      remote_version =
			lw6sys_escape_sql_value (remote_version);
		      remote_codename =
			lw6sys_escape_sql_value (remote_codename);
		      remote_stamp_str = lw6sys_itoa (remote_stamp_int);
		      remote_id = lw6sys_escape_sql_value (remote_id);
		      remote_title = lw6sys_escape_sql_value (remote_title);
		      remote_description =
			lw6sys_escape_sql_value (remote_description);
		      if (remote_version && remote_codename
			  && remote_stamp_str && remote_id && remote_title
			  && remote_description)
			{
			  lw6sys_log (LW6SYS_LOG_NOTICE,
				      _("confirmed node \"%s\""), remote_url);
			}
		      if (remote_version)
			{
			  LW6SYS_FREE (remote_version);
			}
		      if (remote_codename)
			{
			  LW6SYS_FREE (remote_codename);
			}
		      if (remote_stamp_str)
			{
			  LW6SYS_FREE (remote_stamp_str);
			}
		      if (remote_id)
			{
			  LW6SYS_FREE (remote_id);
			}
		      if (remote_title)
			{
			  LW6SYS_FREE (remote_title);
			}
		      if (remote_description)
			{
			  LW6SYS_FREE (remote_description);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _("wrong url \"%s\" vs \"%s\""), remote_url,
				  url);
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("wrong remote program \"%s\""),
			      remote_program);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("answer does not containn the required fields"));
	    }
	  lw6sys_assoc_free (assoc);
	}
    }

  return ret;
}

int
_mod_tcp_process_oob (_tcp_context_t * tcp_context,
		      lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  lw6sys_url_t *parsed_url = NULL;
  char *ip = NULL;
  lw6sys_log (LW6SYS_LOG_DEBUG,
	      _("process tcp oob url=\"%s\""), oob_data->public_url);
  parsed_url = lw6sys_url_parse (oob_data->public_url);
  if (parsed_url)
    {
      ip = lw6net_dns_gethostbyname (parsed_url->host);
      if (ip)
	{
	  if (_mod_tcp_oob_should_continue (tcp_context, oob_data, -1))
	    {
	      if (_do_ping
		  (tcp_context, node_info, oob_data,
		   oob_data->public_url, parsed_url, ip))
		{
		  if (_do_info
		      (tcp_context, node_info, oob_data,
		       oob_data->public_url, parsed_url, ip))
		    {
		      // OK
		    }
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
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _("unable to resolve host \"%s\""), parsed_url->host);
	}
      lw6sys_url_free (parsed_url);
    }

  return ret;
}

int
_mod_tcp_oob_should_continue (_tcp_context_t *
			      tcp_context,
			      lw6cli_oob_data_t * oob_data, int sock)
{
  int ret = 0;
  ret = (_mod_tcp_timeout_ok (tcp_context,
			      oob_data->creation_timestamp)
	 && (sock < 0 || lw6net_tcp_is_alive (sock))
	 && (!oob_data->do_not_finish));
  return ret;
}
