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
#include "mod-httpd-internal.h"

#define _ERROR_FIRST_LINE "ERROR"
#define _ERROR_URI "error"

static int
_parse_first_line (_httpd_request_t * request)
{
  int ret = 0;
  char *pos = NULL;
  char *seek = NULL;
  char seek_c = '\0';

  if (!strncmp
      (request->first_line, _MOD_HTTPD_PROTOCOL_GET_STRING,
       _MOD_HTTPD_PROTOCOL_GET_SIZE))
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("this is a GET"));
      request->get_head_post = _MOD_HTTPD_GET;
    }
  if (!strncmp
      (request->first_line, _MOD_HTTPD_PROTOCOL_HEAD_STRING,
       _MOD_HTTPD_PROTOCOL_HEAD_SIZE))
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("this is a HEAD"));
      request->get_head_post = _MOD_HTTPD_HEAD;
    }
  if (!strncmp
      (request->first_line, _MOD_HTTPD_PROTOCOL_POST_STRING,
       _MOD_HTTPD_PROTOCOL_POST_SIZE))
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("this is a POST"));
      request->get_head_post = _MOD_HTTPD_POST;
    }

  pos = request->first_line;
  while ((*pos) && !lw6sys_chr_is_sapce (*pos))
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
  while ((*seek) && (!lw6sys_chr_is_space (*seek)) && (*seek) != '?'
	 && (*seek) != '#')
    {
      seek++;
    }
  seek_c = (*seek);
  (*seek) = '\0';
  request->uri = lw6sys_str_copy (pos);
  (*seek) = seek_c;

  if (request->uri)
    {
      if (strlen (request->uri) == 0)
	{
	  LW6SYS_FREE (request->uri);
	  request->uri = lw6sys_str_copy ("/");
	}
    }

  if (request->uri)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("REQUEST_URI=\"%s\""), request->uri);
      ret = 1;
    }

  return ret;
}

_httpd_request_t *
_mod_httpd_request_parse_oob (_httpd_context_t * httpd_context,
			      lw6srv_oob_data_t * oob_data)
{
  _httpd_request_t *request = NULL;
  int eof = 0;
  char *line = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("process httpd oob"));
  request = (_httpd_request_t *) LW6SYS_CALLOC (sizeof (_httpd_request_t));
  if (request)
    {
      request->client_ip = lw6sys_str_copy (oob_data->remote_ip);

      if (_mod_httpd_oob_should_continue (httpd_context, oob_data))
	{
	  request->first_line = lw6net_recv_line_tcp (oob_data->sock);
	  if (request->first_line)
	    {
	      if (_parse_first_line (request))
		{
		  while ((!eof)
			 && _mod_httpd_oob_should_continue (httpd_context,
							    oob_data))
		    {
		      line = lw6net_recv_line_tcp (oob_data->sock);
		      if (line)
			{
			  // here should check for Authentication
			  if (strlen (line) == 0)
			    {
			      eof = 1;
			    }
			  LW6SYS_FREE (line);
			}
		    }
		}
	    }
	}
      if ((!request->first_line) || (request->get_head_post == 0)
	  || (!request->uri))
	{
	  /*
	   * OK, not parseable, we put dummy (empty) values in all fields to
	   * avoid suspicious NULL-string bugs, and let the following code
	   * return error 500.
	   */
	  if (request->first_line)
	    {
	      LW6SYS_FREE (request->first_line);
	    }
	  if (request->uri)
	    {
	      LW6SYS_FREE (request->uri);
	    }
	  request->first_line = lw6sys_str_copy (_ERROR_FIRST_LINE);
	  request->get_head_post = 0;
	  request->uri = lw6sys_str_copy (_ERROR_URI);
	}
    }

  return request;
}

void
_mod_httpd_request_free (_httpd_request_t * request)
{
  if (request)
    {
      if (request->client_ip)
	{
	  LW6SYS_FREE (request->client_ip);
	}
      if (request->first_line)
	{
	  LW6SYS_FREE (request->first_line);
	}
      if (request->uri)
	{
	  LW6SYS_FREE (request->uri);
	}
      if (request->http_user)
	{
	  LW6SYS_FREE (request->http_user);
	}
      if (request->http_password)
	{
	  LW6SYS_FREE (request->http_password);
	}
      LW6SYS_FREE (request);
    }
}
