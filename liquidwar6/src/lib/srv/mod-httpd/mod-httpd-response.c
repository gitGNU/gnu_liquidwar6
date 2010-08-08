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

_httpd_response_t *
_mod_httpd_response_from_bin (_httpd_context_t *
			      httpd_context,
			      int status,
			      int no_cache,
			      int refresh_sec,
			      char *refresh_url,
			      char *content_type,
			      int content_size, void *content_data)
{
  _httpd_response_t *response = NULL;

  response = (_httpd_response_t *) LW6SYS_CALLOC (sizeof (_httpd_response_t));
  if (response)
    {
      response->status = status;
      response->no_cache = no_cache;
      response->refresh_sec = refresh_sec;
      if (refresh_url)
	{
	  response->refresh_url = lw6sys_str_copy (refresh_url);
	}
      response->content_type = lw6sys_str_copy (content_type);
      response->content_size = content_size;
      response->content_data = LW6SYS_MALLOC (content_size + 1);
      if (response->content_data)
	{
	  memcpy (response->content_data, content_data, content_size);
	  ((char *) response->content_data)[content_size] = '\0';
	}
      if (response->content_type && response->content_data)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("create response content_type=\"%s\" size=%d"),
		      content_type, content_size);
	}
      else
	{
	  _mod_httpd_response_free (response);
	  response = NULL;
	}
    }

  return response;
}

_httpd_response_t *
_mod_httpd_response_from_str (_httpd_context_t *
			      httpd_context,
			      int status,
			      int no_cache, int refresh_sec,
			      char *refresh_url, char *content_type,
			      char *content)
{
  _httpd_response_t *response = NULL;

  response =
    _mod_httpd_response_from_bin (httpd_context, status, no_cache,
				  refresh_sec, refresh_url, content_type,
				  strlen (content), content);

  return response;
}

void
_mod_httpd_response_free (_httpd_response_t * response)
{
  if (response)
    {
      if (response->refresh_url)
	{
	  LW6SYS_FREE (response->refresh_url);
	}
      if (response->content_type)
	{
	  LW6SYS_FREE (response->content_type);
	}
      if (response->content_data)
	{
	  LW6SYS_FREE (response->content_data);
	}
      LW6SYS_FREE (response);
    }
}

int
_mod_httpd_response_send (_httpd_context_t * httpd_context,
			  _httpd_response_t * response, int sock,
			  int headers_only)
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
      lw6sys_log (LW6SYS_LOG_WARNING, _("unknown response code %d"), status);
      status = _MOD_HTTPD_STATUS_500;
      tmp = _HTTP_500;
    }
  if (tmp)
    {
      first_line =
	lw6sys_new_sprintf ("HTTP/%s %d %s",
			    httpd_context->data.consts.http_version, status,
			    tmp);
      if (first_line)
	{
	  lw6net_send_line_tcp (sock, first_line);

	  now_str = lw6sys_date_rfc1123 (0);
	  if (now_str)
	    {
	      line = lw6sys_new_sprintf ("Date: %s", now_str);
	      if (line)
		{
		  lw6net_send_line_tcp (sock, line);
		  LW6SYS_FREE (line);
		  line = NULL;
		}
	      line =
		lw6sys_new_sprintf ("Server: %s",
				    lw6sys_build_get_package_string ());
	      if (line)
		{
		  lw6net_send_line_tcp (sock, line);
		  LW6SYS_FREE (line);
		  line = NULL;
		}
	      line = lw6sys_new_sprintf ("Last-Modified: %s", now_str);
	      if (line)
		{
		  lw6net_send_line_tcp (sock, line);
		  LW6SYS_FREE (line);
		  line = NULL;
		}
	      if (response->no_cache)
		{
		  lw6net_send_line_tcp (sock, "Pragma: no-cache");
		  lw6net_send_line_tcp (sock, "Cache-Control: no-cache");
		  expire_delta = -httpd_context->data.consts.in_the_past;
		}
	      else
		{
		  line =
		    lw6sys_new_sprintf ("Cache-Control: max-age=%d",
					httpd_context->data.consts.max_age);
		  if (line)
		    {
		      lw6net_send_line_tcp (sock, line);
		      LW6SYS_FREE (line);
		      line = NULL;
		    }
		  expire_delta = httpd_context->data.consts.max_age;
		}
	      expire_str = lw6sys_date_rfc1123 (expire_delta);
	      if (expire_str)
		{
		  line = lw6sys_new_sprintf ("Expires: %s", expire_str);
		  if (line)
		    {
		      lw6net_send_line_tcp (sock, line);
		      LW6SYS_FREE (line);
		      line = NULL;
		    }
		  LW6SYS_FREE (expire_str);
		}
	      if (response->refresh_sec > 0 && response->refresh_url != NULL)
		{
		  line =
		    lw6sys_new_sprintf ("Refresh: %d; url=%s",
					response->refresh_sec,
					response->refresh_url);
		  if (line)
		    {
		      lw6net_send_line_tcp (sock, line);
		      LW6SYS_FREE (line);
		      line = NULL;
		    }
		}
	      LW6SYS_FREE (now_str);
	    }
	  lw6net_send_line_tcp (sock, "Connection: close");
	  line =
	    lw6sys_new_sprintf ("Content-Length: %d", response->content_size);
	  if (line)
	    {
	      lw6net_send_line_tcp (sock, line);
	      LW6SYS_FREE (line);
	      line = NULL;
	    }
	  line =
	    lw6sys_new_sprintf ("Content-Type: %s", response->content_type);
	  if (line)
	    {
	      lw6net_send_line_tcp (sock, line);
	      LW6SYS_FREE (line);
	      line = NULL;
	    }

	  LW6SYS_FREE (first_line);
	  first_line = NULL;
	}
      lw6net_send_line_tcp (sock, "");
      if (lw6net_tcp_is_alive (sock) && !headers_only)
	{
	  ret =
	    lw6net_tcp_send (sock, response->content_data,
			     response->content_size,
			     httpd_context->data.consts.error_timeout, 1);
	}
    }

  return ret;
}
