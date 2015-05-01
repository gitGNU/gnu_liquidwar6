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
#include "mod-udp-internal.h"

static int
_do_broadcast (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, int port)
{
  int ret = 0;
  int sock;
  char *request = NULL;
  char *response = NULL;
  char *given_url = NULL;
  char *incoming_ip = NULL;
  int incoming_port = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("UDP broadcast on %d"), port);
  sock = lw6net_udp_client (sys_context);
  if (lw6net_socket_is_valid (sys_context, sock))
    {
      request = lw6msg_oob_generate_request (sys_context, LW6MSG_OOB_PING, NULL, NULL, node_info->const_info.ref_info.url);
      if (request)
	{
	  if (lw6net_send_line_udp (sys_context, sock, request, LW6NET_ADDRESS_BROADCAST, port))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sent UDP OOB request \"%s\" to %s:%d"), request, LW6NET_ADDRESS_BROADCAST, port);
	      lw6sys_snooze (sys_context);
	      ret = 1;
	      while (_mod_udp_oob_should_continue (sys_context, udp_context, oob_data, 1))
		{
		  response = lw6net_recv_line_udp (sys_context, sock, &incoming_ip, &incoming_port);
		  if (response)
		    {
		      given_url = lw6msg_oob_analyse_pong (sys_context, response);
		      if (given_url)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client BROADCAST discovered %s:%d \"%s\""), incoming_ip, port, given_url);
			  lw6nod_info_add_discovered_node (sys_context, node_info, given_url);
			  LW6SYS_FREE (sys_context, given_url);
			}
		      LW6SYS_FREE (sys_context, response);
		      LW6SYS_FREE (sys_context, incoming_ip);
		    }
		  else
		    {
		      lw6sys_snooze (sys_context);
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, request);
	}
      lw6net_socket_close (sys_context, &sock);
    }

  return ret;
}

static int
_do_ping (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, char *url,
	  lw6sys_url_t * parsed_url, char *ip)
{
  int ret = 0;
  int eom = 0;
  int sock;
  char *request = NULL;
  char *response = NULL;
  char *given_url = NULL;
  char *incoming_ip = NULL;
  int incoming_port = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in UDP on %s:%d"), ip, parsed_url->port);
  sock = lw6net_udp_client (sys_context);
  if (lw6net_socket_is_valid (sys_context, sock))
    {
      request = lw6msg_oob_generate_request (sys_context, LW6MSG_OOB_PING, url, node_info->const_info.password, node_info->const_info.ref_info.url);
      if (request)
	{
	  if (lw6net_send_line_udp (sys_context, sock, request, ip, parsed_url->port))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sent UDP OOB request \"%s\" to %s:%d"), request, ip, parsed_url->port);
	      lw6sys_snooze (sys_context);
	      while (!eom && _mod_udp_oob_should_continue (sys_context, udp_context, oob_data, 0))
		{
		  response = lw6net_recv_line_udp (sys_context, sock, &incoming_ip, &incoming_port);
		  if (response)
		    {
		      eom = 1;
		      if (lw6sys_str_is_same (sys_context, ip, incoming_ip) && parsed_url->port == incoming_port)
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
					      _x_ ("mod_udp connected on %s:%d using \"%s\" but server reports \"%s\""), ip, parsed_url->port, url, given_url);
				  lw6nod_info_add_discovered_node (sys_context, node_info, given_url);
				}
			      LW6SYS_FREE (sys_context, given_url);
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				      _x_
				      ("received UDP data from %s:%d when expecting it to come from %s:%d"), incoming_ip, incoming_port, ip, parsed_url->port);
			}
		      LW6SYS_FREE (sys_context, response);
		      LW6SYS_FREE (sys_context, incoming_ip);
		    }
		  lw6sys_snooze (sys_context);
		}
	    }
	  LW6SYS_FREE (sys_context, request);
	}
      lw6net_socket_close (sys_context, &sock);
    }

  return ret;
}

static void
_info_response_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  lw6sys_assoc_t **assoc = (lw6sys_assoc_t **) func_data;
  char *line = (char *) data;

  if (strlen (line) > 0)
    {
      if (lw6msg_utils_parse_key_value_to_assoc (sys_context, assoc, line))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("parsed line \"%s\""), line);
	}
    }
}

static int
_do_info (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, char *url,
	  lw6sys_url_t * parsed_url, char *ip)
{
  int ret = 0;
  int eom = 0;
  int sock = LW6NET_SOCKET_INVALID;;
  int64_t origin = 0;
  char *request = NULL;
  lw6sys_list_t *response = NULL;
  lw6sys_assoc_t *assoc = NULL;
  char *incoming_ip = NULL;
  int incoming_port = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in UDP on %s:%d"), ip, parsed_url->port);
  assoc = lw6sys_assoc_new (sys_context, lw6sys_free_callback);
  if (assoc)
    {
      origin = lw6sys_get_timestamp (sys_context);
      sock = lw6net_udp_client (sys_context);
      if (lw6net_socket_is_valid (sys_context, sock))
	{
	  request = lw6msg_oob_generate_request (sys_context, LW6MSG_OOB_INFO, url, node_info->const_info.password, node_info->const_info.ref_info.url);
	  if (request)
	    {
	      if (lw6net_send_line_udp (sys_context, sock, request, ip, parsed_url->port))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sent UDP OOB request \"%s\" to %s:%d"), request, ip, parsed_url->port);
		  /*
		   * Here we use idle and not snooze for we're concerned
		   * with the exact time it takes to ping the server.
		   */
		  lw6sys_idle (sys_context);
		  while (!eom && _mod_udp_oob_should_continue (sys_context, udp_context, oob_data, 0))
		    {
		      response = lw6net_recv_lines_udp (sys_context, sock, &incoming_ip, &incoming_port);
		      if (response)
			{
			  eom = 1;
			  lw6sys_list_map (sys_context, response, _info_response_callback, (void *) &assoc);
			  lw6sys_list_free (sys_context, response);
			  LW6SYS_FREE (sys_context, incoming_ip);
			}
		      /*
		       * Here we use idle and not snooze for we're concerned
		       * with the exact time it takes to ping the server.
		       */
		      lw6sys_idle (sys_context);
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
	      ret =
		oob_data->verify_callback_func (sys_context, oob_data->verify_callback_data, url, ip,
						parsed_url->port, lw6sys_get_timestamp (sys_context) - origin, assoc);
	    }
	  lw6sys_assoc_free (sys_context, assoc);
	}
    }

  return ret;
}

static void
_list_response_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  lw6nod_info_t *node_info = (lw6nod_info_t *) func_data;
  char *line = (char *) data;

  if (strlen (line) > 0)
    {
      if (lw6sys_url_is_canonized (sys_context, line))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("list contains \"%s\", registering it"), line);
	  lw6nod_info_add_discovered_node (sys_context, node_info, line);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("list contains non-canonized url \"%s\""), line);
	}
    }
}

static int
_do_list (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, char *url,
	  lw6sys_url_t * parsed_url, char *ip)
{
  int ret = 0;
  int sock = LW6NET_SOCKET_INVALID;
  char *request = NULL;
  lw6sys_list_t *response = NULL;
  char *incoming_ip = NULL;
  int incoming_port = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in UDP on %s:%d"), ip, parsed_url->port);

  sock = lw6net_udp_client (sys_context);
  if (lw6net_socket_is_valid (sys_context, sock))
    {
      request = lw6msg_oob_generate_request (sys_context, LW6MSG_OOB_LIST, url, node_info->const_info.password, node_info->const_info.ref_info.url);
      if (request)
	{
	  if (lw6net_send_line_udp (sys_context, sock, request, ip, parsed_url->port))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sent UDP OOB request \"%s\" to %s:%d"), request, ip, parsed_url->port);
	      lw6sys_snooze (sys_context);
	      while (!ret && _mod_udp_oob_should_continue (sys_context, udp_context, oob_data, 0))
		{
		  response = lw6net_recv_lines_udp (sys_context, sock, &incoming_ip, &incoming_port);
		  if (response)
		    {
		      ret = 1;
		      lw6sys_list_map (sys_context, response, _list_response_callback, node_info);
		      lw6sys_list_free (sys_context, response);
		      LW6SYS_FREE (sys_context, incoming_ip);
		    }
		  lw6sys_snooze (sys_context);
		}
	    }
	  LW6SYS_FREE (sys_context, request);
	}
      lw6net_socket_close (sys_context, &sock);
    }

  return ret;
}

int
_mod_udp_process_oob (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  lw6sys_url_t *parsed_url = NULL;
  char *ip = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process udp oob url=\"%s\""), oob_data->public_url);
  parsed_url = lw6sys_url_parse (sys_context, oob_data->public_url);
  if (parsed_url)
    {
      if (lw6sys_str_is_same (sys_context, parsed_url->host, LW6NET_ADDRESS_BROADCAST))
	{
	  if (_do_broadcast (sys_context, udp_context, node_info, oob_data, parsed_url->port))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client BROADCAST on \"%s\" OK"), oob_data->public_url);
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client BROADCAST on \"%s\" failed"), oob_data->public_url);
	    }
	}
      else
	{
	  ip = lw6net_dns_gethostbyname (sys_context, parsed_url->host);
	  if (ip)
	    {
	      if (_mod_udp_oob_should_continue (sys_context, udp_context, oob_data, 0))
		{
		  if (_do_ping (sys_context, udp_context, node_info, oob_data, oob_data->public_url, parsed_url, ip))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client PING on node \"%s\" OK"), oob_data->public_url);
		      if (_do_info (sys_context, udp_context, node_info, oob_data, oob_data->public_url, parsed_url, ip))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client INFO on node \"%s\" OK"), oob_data->public_url);
			  if (_do_list (sys_context, udp_context, node_info, oob_data, oob_data->public_url, parsed_url, ip))
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client LIST on node \"%s\" OK"), oob_data->public_url);
			      ret = 1;	// todo LIST
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client LIST on node \"%s\" failed"), oob_data->public_url);
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client INFO on node \"%s\" failed"), oob_data->public_url);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_udp client PING on node \"%s\" failed"), oob_data->public_url);
		    }
		}
	      LW6SYS_FREE (sys_context, ip);
	    }
	}
      lw6sys_url_free (sys_context, parsed_url);
    }

  return ret;
}

int
_mod_udp_oob_should_continue (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6cli_oob_data_t * oob_data, int broadcast)
{
  int ret = 0;
  ret = (_mod_udp_timeout_ok (sys_context, udp_context, oob_data->creation_timestamp, broadcast) && (!oob_data->do_not_finish));
  return ret;
}
