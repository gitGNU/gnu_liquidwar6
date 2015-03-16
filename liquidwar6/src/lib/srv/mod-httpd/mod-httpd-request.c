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

#define _ERROR_FIRST_LINE "ERROR"
#define _ERROR_URI "error"
#define _HTTP_AUTHORIZATION "Authorization"
#define _HTTP_BASIC "Basic"

static int
_parse_first_line (lw6sys_context_t * sys_context, _mod_httpd_request_t * request)
{
  int ret = 0;
  char *pos = NULL;
  char *seek = NULL;
  char seek_c = '\0';

  if (lw6sys_str_starts_with (sys_context, request->first_line, _MOD_HTTPD_PROTOCOL_GET_STRING))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("this is a GET"));
      request->get_head_post = _MOD_HTTPD_GET;
    }
  if (lw6sys_str_starts_with (sys_context, request->first_line, _MOD_HTTPD_PROTOCOL_HEAD_STRING))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("this is a HEAD"));
      request->get_head_post = _MOD_HTTPD_HEAD;
    }
  if (lw6sys_str_starts_with (sys_context, request->first_line, _MOD_HTTPD_PROTOCOL_POST_STRING))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("this is a POST"));
      request->get_head_post = _MOD_HTTPD_POST;
    }

  pos = request->first_line;
  while ((*pos) && !lw6sys_chr_is_space (*pos))
    {
      pos++;
    }
  while (lw6sys_chr_is_space (*pos))
    {
      pos++;
    }
  seek = pos;
  /*
   * Here we ignore what's after ? or #, this is typically not
   * standard compliant but LW6 does not interpret this.
   */
  while ((*seek) && (!lw6sys_chr_is_space (*seek)) && (*seek) != '?' && (*seek) != '#')
    {
      seek++;
    }
  seek_c = (*seek);
  (*seek) = '\0';
  request->uri = lw6sys_str_copy (sys_context, pos);
  (*seek) = seek_c;

  if (request->uri)
    {
      if (strlen (request->uri) == 0)
	{
	  LW6SYS_FREE (sys_context, request->uri);
	  request->uri = lw6sys_str_copy (sys_context, "/");
	}
    }

  if (request->uri)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("REQUEST_URI=\"%s\""), request->uri);
      if (!strcmp (request->uri, _MOD_HTTPD_OOB_PING_TXT))
	{
	  request->password_ok = 1;
	}
      ret = 1;
    }

  return ret;
}

static int
_parse_header (lw6sys_context_t * sys_context, _mod_httpd_request_t * request, char *line, char *public_url, char *password)
{
  int ret = 1;
  char *basic = NULL;
  char *seek = NULL;
  char *clear_authorization = NULL;
  char *double_dot = NULL;
  char *received_password = NULL;

  if (lw6sys_str_starts_with (sys_context, line, _HTTP_AUTHORIZATION))
    {
      if (!lw6sys_str_is_null_or_empty (sys_context, password))
	{
	  basic = strstr (line, _HTTP_BASIC);
	  if (basic)
	    {
	      seek = basic + strlen (_HTTP_BASIC);
	      while (lw6sys_chr_is_space (*seek))
		{
		  seek++;
		}
	      clear_authorization = lw6glb_base64_decode_str (sys_context, seek);
	      if (clear_authorization)
		{
		  double_dot = strrchr (clear_authorization, ':');
		  if (double_dot)
		    {
		      received_password = double_dot + 1;
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("received HTTP password \"%s\""), received_password);
		    }
		  if (public_url && received_password)
		    {
		      request->password_ok = lw6cnx_password_verify (sys_context, public_url, password, received_password);
		    }
		  LW6SYS_FREE (sys_context, clear_authorization);
		}
	    }
	}
    }

  return ret;
}

_mod_httpd_request_t *
_mod_httpd_request_parse_oob (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6nod_info_t * node_info, lw6srv_oob_data_t * oob_data)
{
  _mod_httpd_request_t *request = NULL;
  int eof = 0;
  char *line = NULL;
  char b = '\0';

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process httpd oob"));

  /*
   * If there's not even one byte available right now, it's no
   * need to go further, we'll just return NULL and skip
   * even the 500 error default trick, it's not even worth it.
   */
  if (lw6net_tcp_peek (sys_context, &(oob_data->sock), &b, sizeof (b), 0) > 0)
    {
      request = (_mod_httpd_request_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_httpd_request_t));
      if (request)
	{
	  request->client_ip = lw6sys_str_copy (sys_context, oob_data->remote_ip);

	  if (_mod_httpd_oob_should_continue (sys_context, httpd_context, oob_data))
	    {
	      request->first_line = lw6net_recv_line_tcp (sys_context, &(oob_data->sock));
	      if (request->first_line)
		{
		  if (_parse_first_line (sys_context, request))
		    {
		      if (lw6sys_str_is_null_or_empty (sys_context, node_info->const_info.password))
			{
			  request->password_ok = 1;
			}
		      while ((!eof) && _mod_httpd_oob_should_continue (sys_context, httpd_context, oob_data))
			{
			  line = lw6net_recv_line_tcp (sys_context, &(oob_data->sock));
			  if (line)
			    {
			      if (strlen (line) == 0)
				{
				  eof = 1;
				}
			      else
				{
				  _parse_header (sys_context, request, line, node_info->const_info.ref_info.url, node_info->const_info.password);
				}
			      LW6SYS_FREE (sys_context, line);
			    }
			}
		    }
		}
	    }
	  if ((!request->first_line) || (request->get_head_post == 0) || (!request->uri))
	    {
	      /*
	       * Not parseable, user entered garbage but did enter something,
	       * we put dummy (empty) values in all fields to
	       * avoid suspicious NULL-string bugs, and let the following code
	       * return error 500.
	       */
	      if (request->first_line)
		{
		  LW6SYS_FREE (sys_context, request->first_line);
		}
	      if (request->uri)
		{
		  LW6SYS_FREE (sys_context, request->uri);
		}
	      request->first_line = lw6sys_str_copy (sys_context, _ERROR_FIRST_LINE);
	      request->get_head_post = 0;
	      request->uri = lw6sys_str_copy (sys_context, _ERROR_URI);
	    }
	}
    }

  return request;
}

_mod_httpd_request_t *
_mod_httpd_request_parse_cmd (lw6sys_context_t * sys_context, _mod_httpd_reply_thread_data_t * reply_thread_data)
{
  _mod_httpd_request_t *request = NULL;
  lw6cnx_connection_t *cnx = reply_thread_data->cnx;
  int eof = 0;
  char *line = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process httpd cmd"));
  request = (_mod_httpd_request_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_httpd_request_t));
  if (request)
    {
      request->client_ip = lw6sys_str_copy (sys_context, cnx->remote_ip);

      if (_mod_httpd_reply_thread_should_continue (sys_context, reply_thread_data))
	{
	  request->first_line = lw6net_recv_line_tcp (sys_context, &(reply_thread_data->sock));
	  if (request->first_line)
	    {
	      if (_parse_first_line (sys_context, request))
		{
		  if (lw6sys_str_is_null_or_empty (sys_context, cnx->password))
		    {
		      request->password_ok = 1;
		    }
		  while ((!eof) && _mod_httpd_reply_thread_should_continue (sys_context, reply_thread_data))
		    {
		      line = lw6net_recv_line_tcp (sys_context, &(reply_thread_data->sock));
		      if (line)
			{
			  if (strlen (line) == 0)
			    {
			      eof = 1;
			    }
			  else
			    {
			      _parse_header (sys_context, request, line, cnx->local_url, cnx->password);
			    }
			  LW6SYS_FREE (sys_context, line);
			}
		    }
		}
	    }
	}
      if ((!request->first_line) || (request->get_head_post == 0) || (!request->uri))
	{
	  /*
	   * OK, not parseable, we put dummy (empty) values in all fields to
	   * avoid suspicious NULL-string bugs, and let the following code
	   * return error 500.
	   */
	  if (request->first_line)
	    {
	      LW6SYS_FREE (sys_context, request->first_line);
	    }
	  if (request->uri)
	    {
	      LW6SYS_FREE (sys_context, request->uri);
	    }
	  request->first_line = lw6sys_str_copy (sys_context, _ERROR_FIRST_LINE);
	  request->get_head_post = 0;
	  request->uri = lw6sys_str_copy (sys_context, _ERROR_URI);
	}
    }

  return request;
}

void
_mod_httpd_request_free (lw6sys_context_t * sys_context, _mod_httpd_request_t * request)
{
  if (request)
    {
      if (request->client_ip)
	{
	  LW6SYS_FREE (sys_context, request->client_ip);
	}
      if (request->first_line)
	{
	  LW6SYS_FREE (sys_context, request->first_line);
	}
      if (request->uri)
	{
	  LW6SYS_FREE (sys_context, request->uri);
	}
      if (request->http_user)
	{
	  LW6SYS_FREE (sys_context, request->http_user);
	}
      if (request->http_password)
	{
	  LW6SYS_FREE (sys_context, request->http_password);
	}
      LW6SYS_FREE (sys_context, request);
    }
}
