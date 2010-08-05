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

#define _ROOT "/"
#define _INDEX_HTML "/index.html"
#define _SCREENSHOT_JPEG "/screenshot.jpeg"
#define _ROBOTS_TXT "/robots.txt"
#define _GPL_TXT "/gpl.txt"
#define _FAVICON_ICO "/favicon.ico"
#define _INFO_TXT "/info.txt"
#define _LIST_TXT "/list.txt"
#define _PING_TXT "/ping.txt"

#define _PONG "PONG"

static _httpd_response_t *
_response_index_html (_httpd_context_t * httpd_context,
		      lw6nod_info_t * node_info,
		      lw6nod_dyn_info_t * dyn_info,
		      lw6srv_oob_data_t * oob_data)
{
  _httpd_response_t *response = NULL;
  char *content = NULL;

  content = lw6sys_new_sprintf (httpd_context->data.htdocs.index_html,
				node_info->const_info.title,
				lw6sys_build_get_package_name (),
				lw6sys_build_get_copyright (),
				lw6sys_build_get_package_string (),
				node_info->const_info.title,
				node_info->const_info.description);
  if (content)
    {
      response =
	_mod_httpd_response_from_str (httpd_context,
				      _MOD_HTTPD_STATUS_200, 1,
				      httpd_context->data.
				      consts.content_type_html, content);

      LW6SYS_FREE (content);
    }

  return response;
}

static _httpd_response_t *
_response_screenshot_jpeg (_httpd_context_t * httpd_context,
			   lw6nod_info_t * node_info,
			   lw6nod_dyn_info_t * dyn_info,
			   lw6srv_oob_data_t * oob_data)
{
  _httpd_response_t *response = NULL;
  int screenshot_size = 0;
  void *screenshot_data = NULL;

  if (dyn_info->game_screenshot_size > 0
      && dyn_info->game_screenshot_data != NULL)
    {
      screenshot_size = dyn_info->game_screenshot_size;
      screenshot_data = dyn_info->game_screenshot_data;
    }
  else
    {
      screenshot_size = node_info->const_info.idle_screenshot_size;
      screenshot_data = node_info->const_info.idle_screenshot_data;
    }
  if (screenshot_size > 0 && screenshot_data != NULL)
    {
      response =
	_mod_httpd_response_from_bin (httpd_context,
				      _MOD_HTTPD_STATUS_200, 1,
				      httpd_context->data.
				      consts.content_type_jpeg,
				      screenshot_size, screenshot_data);
    }

  return response;
}

int
_mod_httpd_process_oob (_httpd_context_t * httpd_context,
			lw6nod_info_t * node_info,
			lw6srv_oob_data_t * oob_data)
{
  int ret = 0;
  _httpd_request_t *request = NULL;
  _httpd_response_t *response = NULL;
  lw6nod_dyn_info_t *dyn_info = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("process httpd oob"));

  request = _mod_httpd_request_parse (httpd_context, oob_data);
  if (request)
    {
      if (request->get_head_post == _MOD_HTTPD_GET
	  || request->get_head_post == _MOD_HTTPD_HEAD)
	{
	  if (!strcmp (request->uri, _ROOT)
	      || !strcmp (request->uri, _INDEX_HTML)
	      || !strcmp (request->uri, _SCREENSHOT_JPEG)
	      || !strcmp (request->uri, _INFO_TXT)
	      || !strcmp (request->uri, _LIST_TXT))
	    {
	      dyn_info = lw6nod_info_dup_dyn (node_info);
	      if (dyn_info)
		{
		  if (!strcmp (request->uri, _ROOT)
		      || !strcmp (request->uri, _INDEX_HTML))
		    {
		      response =
			_response_index_html (httpd_context, node_info,
					      dyn_info, oob_data);
		    }
		  if (!strcmp (request->uri, _SCREENSHOT_JPEG))
		    {
		      response =
			_response_screenshot_jpeg (httpd_context, node_info,
						   dyn_info, oob_data);
		    }
		  if (!strcmp (request->uri, _INFO_TXT))
		    {
		      //response=_response_info_txt(httpd_context,node_info,dyn_info,oob_data);
		    }
		  if (!strcmp (request->uri, _LIST_TXT))
		    {
		      //response=_response_list_txt(httpd_context,node_info,dyn_info,oob_data);
		    }
		  lw6nod_dyn_info_free (dyn_info);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("unable to duplicate dyn_info"));
		}
	    }
	  else
	    {
	      if (!strcmp (request->uri, _ROBOTS_TXT))
		{
		  response =
		    _mod_httpd_response_from_str (httpd_context,
						  _MOD_HTTPD_STATUS_200, 0,
						  httpd_context->data.
						  consts.content_type_txt,
						  httpd_context->data.
						  htdocs.robots_txt);
		}
	      if (!strcmp (request->uri, _GPL_TXT))
		{
		  response =
		    _mod_httpd_response_from_str (httpd_context,
						  _MOD_HTTPD_STATUS_200, 0,
						  httpd_context->data.
						  consts.content_type_txt,
						  httpd_context->data.
						  htdocs.gpl_txt);
		}
	      if (!strcmp (request->uri, _FAVICON_ICO))
		{
		  response =
		    _mod_httpd_response_from_bin (httpd_context,
						  _MOD_HTTPD_STATUS_200, 0,
						  httpd_context->data.
						  consts.content_type_txt,
						  httpd_context->data.
						  htdocs.favicon_ico_size,
						  httpd_context->data.
						  htdocs.favicon_ico_data);
		}
	      if (!strcmp (request->uri, _PING_TXT))
		{
		  response =
		    _mod_httpd_response_from_str (httpd_context,
						  _MOD_HTTPD_STATUS_200, 0,
						  httpd_context->data.
						  consts.content_type_txt,
						  _PONG);
		}
	    }
	}
      else
	{
	  response =
	    _mod_httpd_http_error (httpd_context, _MOD_HTTPD_STATUS_405);
	}
      if (!response)
	{
	  /*
	   * OK, not treated, we consider it's not found
	   */
	  response =
	    _mod_httpd_http_error (httpd_context, _MOD_HTTPD_STATUS_404);
	}
      if (response)
	{
	  ret =
	    _mod_httpd_response_send (httpd_context, response,
				      oob_data->sock,
				      request->get_head_post ==
				      _MOD_HTTPD_HEAD);
	  _mod_httpd_response_free (response);
	}
      _mod_httpd_request_free (request);
    }

  lw6net_socket_close (oob_data->sock);
  oob_data->sock = -1;

  return ret;
}

int
_mod_httpd_oob_timeout_ok (_httpd_context_t * httpd_context,
			   lw6srv_oob_data_t * oob_data)
{
  int ret = 0;

  ret = (lw6sys_get_timestamp () +
	 httpd_context->data.consts.timeout_msec >
	 oob_data->creation_timestamp)
    && lw6net_tcp_is_alive (oob_data->sock);

  return ret;
}
