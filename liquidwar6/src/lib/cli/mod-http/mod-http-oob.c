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
#include "mod-http-internal.h"

static int
_do_ping (lw6sys_context_t * sys_context, _mod_http_context_t * http_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data, char *url,
	  lw6sys_url_t * parsed_url, char *ip)
{
  int ret = 0;
  char *response = NULL;
  char *ping_url = NULL;
  char *given_url = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in HTTP on %s:%d"), ip, parsed_url->port);
  ping_url = lw6sys_str_concat (sys_context, url, _MOD_HTTP_OOB_PING_TXT);
  if (ping_url)
    {
      response = _mod_http_get (sys_context, http_context, ping_url, NULL, ip, parsed_url->port);
      if (response)
	{
	  if (_mod_http_oob_should_continue (sys_context, http_context, oob_data))
	    {
	      given_url = lw6msg_oob_analyse_pong (sys_context, response);
	      if (given_url)
		{
		  if (lw6sys_str_is_same (sys_context, url, given_url))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("ping successfull on %s:%d  \"%s\""), ip, parsed_url->port, url);
		      ret = 1;
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("mod_http connected on %s:%d \"%s\" but server reports \"%s\""), ip, parsed_url->port, url, given_url);
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
      LW6SYS_FREE (sys_context, ping_url);
    }

  return ret;
}

static int
_do_info (lw6sys_context_t * sys_context, _mod_http_context_t * http_context, lw6nod_info_t * node_info,
	  lw6cli_oob_data_t * oob_data, char *url, lw6sys_url_t * parsed_url, char *ip, char *password_checksum)
{
  int ret = 0;
  char *response = NULL;
  char *info_url = NULL;
  int64_t origin = 0;
  lw6sys_assoc_t *assoc = NULL;
  char *seek = NULL;
  char *pos = NULL;
  char seek_c = '\0';

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in HTTP on %s:%d"), ip, parsed_url->port);
  assoc = lw6sys_assoc_new (sys_context, lw6sys_free_callback);
  if (assoc)
    {
      origin = lw6sys_get_timestamp (sys_context);
      info_url = lw6sys_str_concat (sys_context, url, _MOD_HTTP_OOB_INFO_TXT);
      if (info_url)
	{
	  response = _mod_http_get (sys_context, http_context, info_url, password_checksum, ip, parsed_url->port);
	  if (response)
	    {
	      if (_mod_http_oob_should_continue (sys_context, http_context, oob_data))
		{
		  pos = seek = response;
		  while (*seek)
		    {
		      while (*seek && !lw6sys_chr_is_eol (*seek))
			{
			  seek++;
			}
		      if (*seek)
			{
			  seek_c = *seek;
			  (*seek) = '\0';
			  if (strlen (pos) > 0)
			    {
			      if (lw6msg_utils_parse_key_value_to_assoc (sys_context, &assoc, pos))
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("parsed line \"%s\""), pos);
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to parse line \"%s\", ignoring"), pos);
				}
			    }
			  *seek = (seek_c);
			  seek++;
			  pos = seek;
			}
		    }
		}
	      LW6SYS_FREE (sys_context, response);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no response from %s:%d"), ip, parsed_url->port);
	    }
	  LW6SYS_FREE (sys_context, info_url);
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

static int
_do_list (lw6sys_context_t * sys_context, _mod_http_context_t * http_context, lw6nod_info_t * node_info,
	  lw6cli_oob_data_t * oob_data, char *url, lw6sys_url_t * parsed_url, char *ip, char *password_checksum)
{
  int ret = 0;
  char *response = NULL;
  char *list_url = NULL;
  char *seek = NULL;
  char *pos = NULL;
  char seek_c = '\0';

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connecting in HTTP on %s:%d"), ip, parsed_url->port);
  list_url = lw6sys_str_concat (sys_context, url, _MOD_HTTP_OOB_LIST_TXT);
  if (list_url)
    {
      response = _mod_http_get (sys_context, http_context, list_url, password_checksum, ip, parsed_url->port);
      if (response)
	{
	  if (_mod_http_oob_should_continue (sys_context, http_context, oob_data))
	    {
	      ret = 1;
	      pos = seek = response;
	      while (*seek)
		{
		  while (*seek && !lw6sys_chr_is_eol (*seek))
		    {
		      seek++;
		    }
		  if (*seek)
		    {
		      seek_c = *seek;
		      (*seek) = '\0';
		      if (strlen (pos) > 0)
			{
			  if (lw6sys_url_is_canonized (sys_context, pos))
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
					  _x_ ("list from %s:%d \"%s\" contains \"%s\", registering it"), ip, parsed_url->port, url, pos);
			      lw6nod_info_add_discovered_node (sys_context, node_info, pos);
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
					  _x_ ("list from %s:%d \"%s\" contains non-canonized url \"%s\""), ip, parsed_url->port, url, pos);
			      ret = 0;
			    }
			}
		      *seek = (seek_c);
		      seek++;
		      pos = seek;
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, response);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no response from %s:%d"), ip, parsed_url->port);
	}
      LW6SYS_FREE (sys_context, list_url);
    }

  return ret;
}

int
_mod_http_process_oob (lw6sys_context_t * sys_context, _mod_http_context_t * http_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  lw6sys_url_t *parsed_url = NULL;
  char *ip = NULL;
  char *password_checksum = NULL;
  int tcp_connect_ok = 0;
  int tcp_connect_sock = LW6NET_SOCKET_INVALID;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process http oob url=\"%s\""), oob_data->public_url);

  parsed_url = lw6sys_url_parse (sys_context, oob_data->public_url);
  if (parsed_url)
    {
      if (lw6sys_str_is_same (sys_context, parsed_url->host, LW6NET_ADDRESS_BROADCAST))
	{
	  // no broadcast in HTTP
	  ret = 1;
	}
      else
	{
	  if (node_info->const_info.password && strlen (node_info->const_info.password) > 0)
	    {
	      password_checksum = lw6cnx_password_checksum (sys_context, oob_data->public_url, node_info->const_info.password);
	    }
	  ip = lw6net_dns_gethostbyname (sys_context, parsed_url->host);
	  if (ip)
	    {
	      if (_mod_http_oob_should_continue (sys_context, http_context, oob_data))
		{
		  if (http_context->data.consts.tcp_connect_before_http_get)
		    {
		      /*
		       * We make a first dummy connect on the socket just to check
		       * if it's available. The idea is not to wait ressources,
		       * the idea is just to try and see wether this ip:port
		       * is likely to respond. If not, we won't even waste
		       * our time to try and connect to it using libcurl,
		       * doing so requires locking and other nasty stuff, if
		       * we can find out right away that something is wrong,
		       * we just return immediately.
		       */
		      tcp_connect_sock =
			lw6net_tcp_connect (sys_context, ip, parsed_url->port, http_context->data.consts.connect_timeout * LW6SYS_TICKS_PER_SEC);
		      if (lw6net_socket_is_valid (sys_context, tcp_connect_sock))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				      _x_ ("TCP check before HTTP request, connected to \"%s\" on %s:%d"), oob_data->public_url, ip, parsed_url->port);
			  tcp_connect_ok = 1;
			  lw6net_socket_close (sys_context, &tcp_connect_sock);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				      _x_
				      ("TCP check before HTTP request, can't connect to node \"%s\" on %s:%d"), oob_data->public_url, ip, parsed_url->port);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_
				  ("TCP check before HTTP request disabled, will use libcurl directly to connect to node \"%s\" on %s:%d"),
				  oob_data->public_url, ip, parsed_url->port);
		    }
		}

	      if (_mod_http_oob_should_continue (sys_context, http_context, oob_data)
		  && (tcp_connect_ok || !http_context->data.consts.tcp_connect_before_http_get))
		{
		  if (_do_ping (sys_context, http_context, node_info, oob_data, oob_data->public_url, parsed_url, ip))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_http client PING on node \"%s\" OK"), oob_data->public_url);
		      if (_do_info (sys_context, http_context, node_info, oob_data, oob_data->public_url, parsed_url, ip, password_checksum))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_http client INFO on node \"%s\" OK"), oob_data->public_url);
			  if (_do_list (sys_context, http_context, node_info, oob_data, oob_data->public_url, parsed_url, ip, password_checksum))
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_http client LIST on node \"%s\" OK"), oob_data->public_url);
			      ret = 1;
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_http client LIST on node \"%s\" failed"), oob_data->public_url);
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_http client INFO on node \"%s\" failed"), oob_data->public_url);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("mod_http client PING on node \"%s\" failed"), oob_data->public_url);
		    }
		}
	      LW6SYS_FREE (sys_context, ip);
	    }
	  if (password_checksum)
	    {
	      LW6SYS_FREE (sys_context, password_checksum);
	    }
	}
      lw6sys_url_free (sys_context, parsed_url);
    }

  return ret;
}

int
_mod_http_oob_should_continue (lw6sys_context_t * sys_context, _mod_http_context_t * http_context, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  ret = (_mod_http_timeout_ok (sys_context, http_context, oob_data->creation_timestamp) && (!oob_data->do_not_finish));
  return ret;
}
