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

#include "../cli.h"
#include "mod-tcp-internal.h"

static int
_do_ping (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, char *url,
	  lw6sys_url_t * parsed_url, char *ip)
{
  int ret = 0;
  int sock = LW6NET_SOCKET_INVALID;
  char *request = NULL;
  char *response = NULL;
  char *given_url = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in TCP on %s:%d"), ip, parsed_url->port);
  sock = lw6net_tcp_connect (sys_context, ip, parsed_url->port, tcp_context->data.consts.connect_timeout * 1000);
  if (lw6net_socket_is_valid (sys_context, sock))
    {
      request = lw6msg_oob_generate_request (sys_context, LW6MSG_OOB_PING, url, node_info->const_info.password, node_info->const_info.ref_info.url);
      if (request)
	{
	  if (lw6net_send_line_tcp (sys_context, &sock, request))
	    {
	      lw6sys_snooze (sys_context);
	      while (!ret && _mod_tcp_oob_should_continue (sys_context, tcp_context, oob_data, &sock)
		     && ((response = lw6net_recv_line_tcp (sys_context, &sock)) == NULL))
		{
		  lw6sys_snooze (sys_context);
		}
	      if (response)
		{
		  if (_mod_tcp_oob_should_continue (sys_context, tcp_context, oob_data, &sock))
		    {
		      given_url = lw6msg_oob_analyse_pong (sys_context, response);
		      if (given_url)
			{
			  if (lw6sys_str_is_same (sys_context, url, given_url))
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("ping successfull on %s:%d \"%s\""), ip, parsed_url->port, url);
			      ret = 1;
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
					  _x_ ("mod_tcp connected on %s:%d using \"%s\" but server reports \"%s\""), ip, parsed_url->port, url, given_url);
			      lw6nod_info_add_discovered_node (sys_context, node_info, given_url);
			    }
			  LW6SYS_FREE (sys_context, given_url);
			}
		    }
		  LW6SYS_FREE (sys_context, response);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no response from %s:%d"), ip, parsed_url->port);
		}
	    }
	  LW6SYS_FREE (sys_context, request);
	}
      lw6net_socket_close (sys_context, &sock);
    }

  return ret;
}

static int
_do_info (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, char *url,
	  lw6sys_url_t * parsed_url, char *ip)
{
  int ret = 0;
  int eom = 0;
  int sock = LW6NET_SOCKET_INVALID;
  int64_t origin = 0;
  char *request = NULL;
  char *response = NULL;
  lw6sys_assoc_t *assoc = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in TCP on %s:%d"), ip, parsed_url->port);
  assoc = lw6sys_assoc_new (sys_context, lw6sys_free_callback);
  if (assoc)
    {
      sock = lw6net_tcp_connect (sys_context, ip, parsed_url->port, tcp_context->data.consts.connect_timeout * 1000);
      if (lw6net_socket_is_valid (sys_context, sock))
	{
	  request = lw6msg_oob_generate_request (sys_context, LW6MSG_OOB_INFO, url, node_info->const_info.password, node_info->const_info.ref_info.url);
	  if (request)
	    {
	      /*
	       * We calculate origin only now for connect() time isn't
	       * relevant on mod-tcp when connected, since we do it
	       * only once at the beginning. This might give mod-tcp
	       * an advantage over mod-http but after all, this is
	       * logical.
	       */
	      origin = lw6sys_get_timestamp (sys_context);
	      if (lw6net_send_line_tcp (sys_context, &sock, request))
		{
		  /*
		   * Here we use idle and not snooze for we're concerned
		   * with the exact time it takes to ping the server.
		   */
		  lw6sys_idle (sys_context);
		  while (_mod_tcp_oob_should_continue (sys_context, tcp_context, oob_data, &sock) && !eom)
		    {
		      while (_mod_tcp_oob_should_continue (sys_context, tcp_context, oob_data, &sock)
			     && ((response = lw6net_recv_line_tcp (sys_context, &sock)) == NULL) && !eom)
			{
			  /*
			   * Here we use idle and not snooze for we're concerned
			   * with the exact time it takes to ping the server.
			   */
			  lw6sys_idle (sys_context);
			}
		      if (response)
			{
			  if (strlen (response) == 0)
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("end of message detected"));
			      eom = 1;
			    }
			  else
			    {
			      if (assoc)
				{
				  if (lw6msg_utils_parse_key_value_to_assoc (sys_context, &assoc, response))
				    {
				      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("parsed line \"%s\""), response);
				    }
				  else
				    {
				      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to parse line \"%s\", ignoring"), response);
				    }
				}
			    }

			  LW6SYS_FREE (sys_context, response);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("inconsistent response from %s:%d"), ip, parsed_url->port);
			}
		    }
		}
	      LW6SYS_FREE (sys_context, request);
	    }
	  lw6net_socket_close (sys_context, &sock);
	}
      if (assoc)
	{
	  if (oob_data->verify_callback_func)
	    {
	      /*
	       * It's safe to call this here even if, for instance,
	       * origin is wrong for the verify callback should check
	       * for right values in the assoc.
	       */
	      ret =
		oob_data->verify_callback_func (sys_context, oob_data->verify_callback_data, url, ip,
						parsed_url->port, lw6sys_get_timestamp (sys_context) - origin, assoc);
	    }
	  lw6sys_assoc_free (sys_context, assoc);
	}
    }

  return ret;
}

static int
_do_list (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, char *url,
	  lw6sys_url_t * parsed_url, char *ip)
{
  int ret = 0;
  int eom = 0;
  int sock = LW6NET_SOCKET_INVALID;
  char *request = NULL;
  char *response = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in TCP on %s:%d"), ip, parsed_url->port);

  sock = lw6net_tcp_connect (sys_context, ip, parsed_url->port, tcp_context->data.consts.connect_timeout * 1000);
  if (lw6net_socket_is_valid (sys_context, sock))
    {
      request = lw6msg_oob_generate_request (sys_context, LW6MSG_OOB_LIST, url, node_info->const_info.password, node_info->const_info.ref_info.url);
      if (request)
	{
	  if (lw6net_send_line_tcp (sys_context, &sock, request))
	    {
	      lw6sys_snooze (sys_context);
	      while (_mod_tcp_oob_should_continue (sys_context, tcp_context, oob_data, &sock) && !eom)
		{
		  while (_mod_tcp_oob_should_continue (sys_context, tcp_context, oob_data, &sock)
			 && ((response = lw6net_recv_line_tcp (sys_context, &sock)) == NULL) && !eom)
		    {
		      lw6sys_snooze (sys_context);
		    }
		  if (response)
		    {
		      ret = 1;
		      if (strlen (response) == 0)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("end of message detected"));
			  eom = 1;
			}
		      else
			{
			  if (lw6sys_url_is_canonized (sys_context, response))
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
					  _x_ ("list from %s:%d \"%s\" contains \"%s\", registering it"), ip, parsed_url->port, url, response);
			      lw6nod_info_add_discovered_node (sys_context, node_info, response);
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
					  _x_ ("list from %s:%d \"%s\" contains non-canonized url \"%s\""), ip, parsed_url->port, url, response);
			      ret = 0;
			    }
			}
		      LW6SYS_FREE (sys_context, response);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("inconsistent response from %s:%d"), ip, parsed_url->port);
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, request);
	}
      lw6net_socket_close (sys_context, &sock);
    }

  return ret;
}

int
_mod_tcp_process_oob (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  lw6sys_url_t *parsed_url = NULL;
  char *ip = NULL;
  int sock = LW6NET_SOCKET_INVALID;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process tcp oob url=\"%s\""), oob_data->public_url);
  parsed_url = lw6sys_url_parse (sys_context, oob_data->public_url);
  if (parsed_url)
    {
      if (lw6sys_str_is_same (sys_context, parsed_url->host, LW6NET_ADDRESS_BROADCAST))
	{
	  // no broadcast in TCP
	  ret = 1;
	}
      else
	{
	  ip = lw6net_dns_gethostbyname (sys_context, parsed_url->host);
	  if (ip)
	    {
	      if (_mod_tcp_oob_should_continue (sys_context, tcp_context, oob_data, &sock))
		{
		  if (_do_ping (sys_context, tcp_context, node_info, oob_data, oob_data->public_url, parsed_url, ip))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_tcp client PING on node \"%s\" OK"), oob_data->public_url);
		      if (_do_info (sys_context, tcp_context, node_info, oob_data, oob_data->public_url, parsed_url, ip))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_tcp client INFO on node \"%s\" OK"), oob_data->public_url);
			  if (_do_list (sys_context, tcp_context, node_info, oob_data, oob_data->public_url, parsed_url, ip))
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_tcp client LIST on node \"%s\" OK"), oob_data->public_url);
			      ret = 1;
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_tcp client LIST on node \"%s\" failed"), oob_data->public_url);
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_tcp client INFO on node \"%s\" failed"), oob_data->public_url);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_tcp client PING on node \"%s\" failed"), oob_data->public_url);
		    }
		}
	      LW6SYS_FREE (sys_context, ip);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to resolve host \"%s\""), parsed_url->host);
	    }
	}
      lw6sys_url_free (sys_context, parsed_url);
    }

  return ret;
}

int
_mod_tcp_oob_should_continue (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context, lw6cli_oob_data_t * oob_data, int *sock)
{
  int ret = 0;
  ret = (_mod_tcp_timeout_ok (sys_context, tcp_context,
			      oob_data->creation_timestamp)
	 && ((!lw6net_socket_is_valid (sys_context, *sock)) || lw6net_tcp_is_alive (sys_context, sock)) && (!oob_data->do_not_finish));
  return ret;
}
