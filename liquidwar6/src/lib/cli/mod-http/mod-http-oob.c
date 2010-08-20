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
#include "mod-http-internal.h"

static int
_do_ping (_http_context_t * http_context, lw6nod_info_t * node_info,
	  lw6cli_oob_data_t * oob_data, char *url, lw6sys_url_t * parsed_url,
	  char *ip)
{
  int ret = 0;
  char *response = NULL;
  char *ping_url = NULL;
  char *given_url = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("connecting in HTTP on %s:%d"), ip,
	      parsed_url->port);
  ping_url = lw6sys_str_concat (url, _MOD_HTTP_OOB_PING_TXT);
  if (ping_url)
    {
      response = _mod_http_get (http_context, ping_url, NULL);
      if (response)
	{
	  if (_mod_http_oob_should_continue (http_context, oob_data))
	    {
	      given_url = lw6msg_oob_analyse_pong (response);
	      if (given_url)
		{
		  if (lw6sys_str_is_same (url, given_url))
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _
				  ("ping successfull on %s:%d  \"%s\""),
				  ip, parsed_url->port, url);
		      ret = 1;
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _
				  ("mod_http connected on %s:%d \"%s\" but server reports \"%s\""),
				  ip, parsed_url->port, url, given_url);
		      lw6nod_info_add_discovered_node (node_info, given_url);
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
      LW6SYS_FREE (ping_url);
    }

  return ret;
}

static int
_do_info (_http_context_t * http_context, lw6nod_info_t * node_info,
	  lw6cli_oob_data_t * oob_data, char *url, lw6sys_url_t * parsed_url,
	  char *ip, char *password_checksum)
{
  int ret = 0;
  char *response = NULL;
  char *info_url = NULL;
  int64_t origin = 0;
  lw6sys_assoc_t *assoc = NULL;
  char *seek = NULL;
  char *pos = NULL;
  char seek_c = '\0';

  lw6sys_log (LW6SYS_LOG_DEBUG, _("connecting in HTTP on %s:%d"), ip,
	      parsed_url->port);
  assoc = lw6sys_assoc_new (lw6sys_free_callback);
  if (assoc)
    {
      origin = lw6sys_get_timestamp ();
      info_url = lw6sys_str_concat (url, _MOD_HTTP_OOB_INFO_TXT);
      if (info_url)
	{
	  response =
	    _mod_http_get (http_context, info_url, password_checksum);
	  if (response)
	    {
	      if (_mod_http_oob_should_continue (http_context, oob_data))
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
			      if (lw6msg_utils_parse_key_value_to_assoc
				  (&assoc, pos))
				{
				  lw6sys_log (LW6SYS_LOG_DEBUG,
					      _("parsed line \"%s\""), pos);
				}
			      else
				{
				  lw6sys_log (LW6SYS_LOG_DEBUG,
					      _
					      ("unable to parse line \"%s\", ignoring"),
					      pos);
				}
			    }
			  *seek = (seek_c);
			  seek++;
			  pos = seek;
			}
		    }
		}
	      LW6SYS_FREE (response);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("no response from %s:%d"),
			  ip, parsed_url->port);
	    }
	  LW6SYS_FREE (info_url);
	}
      if (assoc)
	{
	  if (oob_data->verify_callback_func)
	    {
	      ret =
		oob_data->
		verify_callback_func (oob_data->verify_callback_data, url, ip,
				      parsed_url->port,
				      lw6sys_get_timestamp () - origin,
				      assoc);
	    }
	  lw6sys_assoc_free (assoc);
	}
    }

  return ret;
}

static int
_do_list (_http_context_t * http_context, lw6nod_info_t * node_info,
	  lw6cli_oob_data_t * oob_data, char *url, lw6sys_url_t * parsed_url,
	  char *ip, char *password_checksum)
{
  int ret = 0;
  char *response = NULL;
  char *list_url = NULL;
  char *seek = NULL;
  char *pos = NULL;
  char seek_c = '\0';

  lw6sys_log (LW6SYS_LOG_DEBUG, _("connecting in HTTP on %s:%d"), ip,
	      parsed_url->port);
  list_url = lw6sys_str_concat (url, _MOD_HTTP_OOB_LIST_TXT);
  if (list_url)
    {
      response = _mod_http_get (http_context, list_url, password_checksum);
      if (response)
	{
	  if (_mod_http_oob_should_continue (http_context, oob_data))
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
			  lw6sys_log (LW6SYS_LOG_DEBUG,
				      _
				      ("list from %s:%d \"%s\" contains \"%s\", registering it"),
				      ip, parsed_url->port, url, pos);
			  lw6nod_info_add_discovered_node (node_info, pos);
			}
		      *seek = (seek_c);
		      seek++;
		      pos = seek;
		    }
		}
	    }
	  LW6SYS_FREE (response);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("no response from %s:%d"),
		      ip, parsed_url->port);
	}
      LW6SYS_FREE (list_url);
    }

  return ret;
}

int
_mod_http_process_oob (_http_context_t * http_context,
		       lw6nod_info_t * node_info,
		       lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  lw6sys_url_t *parsed_url = NULL;
  char *ip = NULL;
  char *password_checksum = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("process http oob url=\"%s\""),
	      oob_data->public_url);

  parsed_url = lw6sys_url_parse (oob_data->public_url);
  if (parsed_url)
    {
      if (lw6sys_str_is_same (parsed_url->host, LW6NET_ADDRESS_ANY))
	{
	  // no broadcast in HTTP
	  ret = 1;
	}
      else
	{
	  if (node_info->const_info.password
	      && strlen (node_info->const_info.password) > 0)
	    {
	      password_checksum =
		lw6sys_password_checksum (oob_data->public_url,
					  node_info->const_info.password);
	    }
	  ip = lw6net_dns_gethostbyname (parsed_url->host);
	  if (ip)
	    {
	      if (_mod_http_oob_should_continue (http_context, oob_data))
		{
		  if (_do_ping
		      (http_context, node_info, oob_data,
		       oob_data->public_url, parsed_url, ip))
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _("mod_http client PING on node \"%s\" OK"),
				  oob_data->public_url);
		      if (_do_info
			  (http_context, node_info, oob_data,
			   oob_data->public_url, parsed_url, ip,
			   password_checksum))
			{
			  lw6sys_log (LW6SYS_LOG_INFO,
				      _
				      ("mod_http client INFO on node \"%s\" OK"),
				      oob_data->public_url);
			  if (_do_list
			      (http_context, node_info, oob_data,
			       oob_data->public_url, parsed_url, ip,
			       password_checksum))
			    {
			      lw6sys_log (LW6SYS_LOG_INFO,
					  _
					  ("mod_http client LIST on node \"%s\" OK"),
					  oob_data->public_url);
			      ret = 1;
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_INFO,
					  _
					  ("mod_http client LIST on node \"%s\" failed"),
					  oob_data->public_url);
			    }
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_INFO,
				      _
				      ("mod_http client INFO on node \"%s\" failed"),
				      oob_data->public_url);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _
				  ("mod_http client PING on node \"%s\" failed"),
				  oob_data->public_url);
		    }
		}
	      LW6SYS_FREE (ip);
	    }
	  if (password_checksum)
	    {
	      LW6SYS_FREE (password_checksum);
	    }
	}
      lw6sys_url_free (parsed_url);
    }

  return ret;
}

int
_mod_http_oob_should_continue (_http_context_t *
			       http_context, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;
  ret = (_mod_http_timeout_ok (http_context,
			       oob_data->creation_timestamp)
	 && (!oob_data->do_not_finish));
  return ret;
}
