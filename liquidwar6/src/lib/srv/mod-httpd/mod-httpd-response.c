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

/*
 * Those are stored on another form in the XML resource
 * file but here there're hardcoded for they are really
 * protocol related, what we'll send in the headers. The
 * other values in the XML are for human reading.
 */
#define _HTTP_200 "OK"
#define _HTTP_401 "Unauthorized"
#define _HTTP_403 "Forbidden"
#define _HTTP_404 "Not Found"
#define _HTTP_405 "Method Not Allowed"
#define _HTTP_500 "Internal Server Error"

_mod_httpd_response_t *
_mod_httpd_response_from_bin (lw6sys_context_t * sys_context, _mod_httpd_context_t *
			      httpd_context,
			      int status,
			      int no_cache, int refresh_sec, const char *refresh_url, const char *content_type, int content_size, void *content_data)
{
  _mod_httpd_response_t *response = NULL;

  response = (_mod_httpd_response_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_httpd_response_t));
  if (response)
    {
      response->status = status;
      response->no_cache = no_cache;
      response->refresh_sec = refresh_sec;
      if (refresh_url)
	{
	  response->refresh_url = lw6sys_str_copy (sys_context, refresh_url);
	}
      response->content_type = lw6sys_str_copy (sys_context, content_type);
      response->content_size = content_size;
      response->content_data = LW6SYS_MALLOC (sys_context, content_size + 1);
      if (response->content_data)
	{
	  memcpy (response->content_data, content_data, content_size);
	  ((char *) response->content_data)[content_size] = '\0';
	}
      if (response->content_type && response->content_data)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("create response content_type=\"%s\" size=%d"), content_type, content_size);
	}
      else
	{
	  _mod_httpd_response_free (sys_context, response);
	  response = NULL;
	}
    }

  return response;
}

_mod_httpd_response_t *
_mod_httpd_response_from_str (lw6sys_context_t * sys_context, _mod_httpd_context_t *
			      httpd_context, int status, int no_cache, int refresh_sec, const char *refresh_url, const char *content_type, const char *content)
{
  _mod_httpd_response_t *response = NULL;

  response =
    _mod_httpd_response_from_bin (sys_context, httpd_context, status, no_cache, refresh_sec, refresh_url, content_type, strlen (content), (void *) content);

  return response;
}

void
_mod_httpd_response_free (lw6sys_context_t * sys_context, _mod_httpd_response_t * response)
{
  if (response)
    {
      if (response->refresh_url)
	{
	  LW6SYS_FREE (sys_context, response->refresh_url);
	}
      if (response->content_type)
	{
	  LW6SYS_FREE (sys_context, response->content_type);
	}
      if (response->content_data)
	{
	  LW6SYS_FREE (sys_context, response->content_data);
	}
      LW6SYS_FREE (sys_context, response);
    }
}

int
_mod_httpd_response_send (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, _mod_httpd_response_t * response, int *sock, int headers_only)
{
  int ret = 0;
  char *first_line = NULL;
  char *line = NULL;
  char *tmp = NULL;
  int status = response->status;
  char *now_str = NULL;
  char *expire_str = NULL;
  int expire_delta = 0;

  switch (status)
    {
    case _MOD_HTTPD_STATUS_200:
      tmp = _HTTP_200;
      break;
    case _MOD_HTTPD_STATUS_401:
      tmp = _HTTP_401;
      break;
    case _MOD_HTTPD_STATUS_403:
      tmp = _HTTP_403;
      break;
    case _MOD_HTTPD_STATUS_404:
      tmp = _HTTP_404;
      break;
    case _MOD_HTTPD_STATUS_405:
      tmp = _HTTP_405;
      break;
    case _MOD_HTTPD_STATUS_500:
      tmp = _HTTP_500;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unknown response code %d"), status);
      status = _MOD_HTTPD_STATUS_500;
      tmp = _HTTP_500;
    }
  if (tmp)
    {
      first_line = lw6sys_new_sprintf (sys_context, "HTTP/%s %d %s", httpd_context->data.consts.http_version, status, tmp);
      if (first_line)
	{
	  lw6net_send_line_tcp (sys_context, sock, first_line);

	  now_str = lw6sys_date_rfc1123 (sys_context, 0);
	  if (now_str)
	    {
	      line = lw6sys_new_sprintf (sys_context, "Server: %s", lw6sys_build_get_package_tarname ());
	      if (line)
		{
		  lw6net_send_line_tcp (sys_context, sock, line);
		  LW6SYS_FREE (sys_context, line);
		  line = NULL;
		}
	      line = lw6sys_new_sprintf (sys_context, "Date: %s", now_str);
	      if (line)
		{
		  lw6net_send_line_tcp (sys_context, sock, line);
		  LW6SYS_FREE (sys_context, line);
		  line = NULL;
		}
	      if (status == _MOD_HTTPD_STATUS_401)
		{
		  line = lw6sys_new_sprintf (sys_context, "WWW-Authenticate: Basic realm=\"%s\"", httpd_context->data.consts.auth_realm);
		  if (line)
		    {
		      lw6net_send_line_tcp (sys_context, sock, line);
		      LW6SYS_FREE (sys_context, line);
		      line = NULL;
		    }
		}
	      line = lw6sys_new_sprintf (sys_context, "Last-Modified: %s", now_str);
	      if (line)
		{
		  lw6net_send_line_tcp (sys_context, sock, line);
		  LW6SYS_FREE (sys_context, line);
		  line = NULL;
		}
	      if (response->no_cache)
		{
		  lw6net_send_line_tcp (sys_context, sock, "Pragma: no-cache");
		  lw6net_send_line_tcp (sys_context, sock, "Cache-Control: no-cache");
		  expire_delta = -httpd_context->data.consts.in_the_past;
		}
	      else
		{
		  line = lw6sys_new_sprintf (sys_context, "Cache-Control: max-age=%d", httpd_context->data.consts.max_age);
		  if (line)
		    {
		      lw6net_send_line_tcp (sys_context, sock, line);
		      LW6SYS_FREE (sys_context, line);
		      line = NULL;
		    }
		  expire_delta = httpd_context->data.consts.max_age;
		}
	      expire_str = lw6sys_date_rfc1123 (sys_context, expire_delta);
	      if (expire_str)
		{
		  line = lw6sys_new_sprintf (sys_context, "Expires: %s", expire_str);
		  if (line)
		    {
		      lw6net_send_line_tcp (sys_context, sock, line);
		      LW6SYS_FREE (sys_context, line);
		      line = NULL;
		    }
		  LW6SYS_FREE (sys_context, expire_str);
		}
	      if (response->refresh_sec > 0 && response->refresh_url != NULL)
		{
		  line = lw6sys_new_sprintf (sys_context, "Refresh: %d; url=%s", response->refresh_sec, response->refresh_url);
		  if (line)
		    {
		      lw6net_send_line_tcp (sys_context, sock, line);
		      LW6SYS_FREE (sys_context, line);
		      line = NULL;
		    }
		}
	      LW6SYS_FREE (sys_context, now_str);
	    }
	  lw6net_send_line_tcp (sys_context, sock, "Connection: close");
	  line = lw6sys_new_sprintf (sys_context, "Content-Length: %d", response->content_size);
	  if (line)
	    {
	      lw6net_send_line_tcp (sys_context, sock, line);
	      LW6SYS_FREE (sys_context, line);
	      line = NULL;
	    }
	  line = lw6sys_new_sprintf (sys_context, "Content-Type: %s", response->content_type);
	  if (line)
	    {
	      lw6net_send_line_tcp (sys_context, sock, line);
	      LW6SYS_FREE (sys_context, line);
	      line = NULL;
	    }

	  LW6SYS_FREE (sys_context, first_line);
	  first_line = NULL;
	}
      lw6net_send_line_tcp (sys_context, sock, "");
      if (lw6net_tcp_is_alive (sys_context, sock) && !headers_only)
	{
	  ret = lw6net_tcp_send (sys_context, sock, response->content_data, response->content_size, httpd_context->data.consts.error_timeout * 1000, 1);
	}
    }

  return ret;
}
