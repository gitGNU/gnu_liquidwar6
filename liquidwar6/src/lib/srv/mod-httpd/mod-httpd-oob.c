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

#define _SCREENSHOT_JPEG_REFRESH "screenshot.jpeg"
#define _DUMMY_RANGE 1000000

static void
_add_node_html (void *func_data, void *data)
{
  char **list = (char **) func_data;
  char *url = (char *) data;
  char *tmp = NULL;

  if (list && (*list) && url)
    {
      if (strlen (*list) > 0)
	{
	  tmp =
	    lw6sys_new_sprintf ("%s, <a href=\"%s\">%s</a>", *list, url, url);
	}
      else
	{
	  tmp = lw6sys_new_sprintf ("<a href=\"%s\">%s</a>", url, url);
	}
      if (tmp)
	{
	  LW6SYS_FREE (*list);
	  (*list) = tmp;
	}
    }
}

static _httpd_response_t *
_response_index_html (_httpd_context_t * httpd_context,
		      lw6nod_info_t * node_info, lw6nod_dyn_info_t * dyn_info)
{
  _httpd_response_t *response = NULL;
  char *content = NULL;
  char *screenshot_url = NULL;
  char *level = "";
  char *list = NULL;
  int dummy = 0;
  char *uptime = NULL;

  dummy = lw6sys_random (_DUMMY_RANGE);
  screenshot_url =
    lw6sys_str_concat (node_info->const_info.url, _SCREENSHOT_JPEG_REFRESH);
  if (screenshot_url)
    {
      uptime =
	lw6sys_readable_uptime (lw6sys_get_timestamp () -
				node_info->const_info.creation_timestamp);
      if (uptime)
	{
	  if (dyn_info->level)
	    {
	      level = dyn_info->level;
	    }
	  list = lw6sys_new_sprintf ("");
	  if (list)
	    {
	      lw6nod_info_map_verified_nodes (node_info,
					      _add_node_html, &list);
	      content =
		lw6sys_new_sprintf (httpd_context->data.htdocs.index_html,
				    /*
				     * Variables in the HEAD section
				     */
				    httpd_context->data.consts.
				    refresh_index_header,
				    node_info->const_info.url,
				    node_info->const_info.title,
				    node_info->const_info.id,
				    lw6sys_build_get_copyright (),
				    lw6sys_build_get_package_string (),
				    /*
				     * Variables for JavaScript use
				     */
				    node_info->const_info.url,
				    httpd_context->data.consts.
				    refresh_screenshot_js * 1000,
				    _DUMMY_RANGE, screenshot_url,
				    httpd_context->data.consts.
				    refresh_index_js * 1000,
				    httpd_context->data.consts.
				    refresh_screenshot_js * 1000,
				    /*
				     * Variables in the BODY section
				     */
				    node_info->const_info.title,
				    node_info->const_info.description, dummy,
				    /*
				     * Version
				     */
				    lw6sys_build_get_version (),
				    lw6sys_build_get_codename (),
				    lw6sys_build_get_stamp (),
				    /*
				     * Info
				     */
				    uptime,
				    level,
				    node_info->const_info.bench,
				    dyn_info->required_bench,
				    dyn_info->nb_colors,
				    dyn_info->max_nb_colors,
				    dyn_info->nb_cursors,
				    dyn_info->max_nb_cursors,
				    dyn_info->nb_nodes,
				    dyn_info->max_nb_nodes,
				    /*
				     * List
				     */
				    list);
	      if (content)
		{
		  response =
		    _mod_httpd_response_from_str (httpd_context,
						  _MOD_HTTPD_STATUS_200, 1,
						  httpd_context->data.consts.
						  refresh_index_header,
						  node_info->const_info.url,
						  httpd_context->data.consts.
						  content_type_html, content);

		  LW6SYS_FREE (content);
		}
	      LW6SYS_FREE (list);
	    }
	  LW6SYS_FREE (uptime);
	}
      LW6SYS_FREE (screenshot_url);
    }

  return response;
}

static _httpd_response_t *
_response_screenshot_jpeg (_httpd_context_t * httpd_context,
			   lw6nod_info_t * node_info,
			   lw6nod_dyn_info_t * dyn_info)
{
  _httpd_response_t *response = NULL;
  int screenshot_size = 0;
  void *screenshot_data = NULL;
  char *refresh_url = NULL;

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
  refresh_url =
    lw6sys_str_concat (node_info->const_info.url, _SCREENSHOT_JPEG_REFRESH);
  if (refresh_url)
    {
      if (screenshot_size > 0 && screenshot_data != NULL)
	{
	  response =
	    _mod_httpd_response_from_bin (httpd_context,
					  _MOD_HTTPD_STATUS_200, 1,
					  httpd_context->data.consts.
					  refresh_screenshot_header,
					  refresh_url,
					  httpd_context->data.consts.
					  content_type_jpeg, screenshot_size,
					  screenshot_data);
	}
      LW6SYS_FREE (refresh_url);
    }
  return response;
}

static _httpd_response_t *
_response_info_txt (_httpd_context_t * httpd_context,
		    lw6nod_info_t * node_info)
{
  _httpd_response_t *response = NULL;
  char *content = NULL;

  content = lw6nod_info_generate_oob_info (node_info);
  if (content)
    {
      response =
	_mod_httpd_response_from_str (httpd_context,
				      _MOD_HTTPD_STATUS_200, 1,
				      0,
				      NULL,
				      httpd_context->data.consts.
				      content_type_txt, content);
      LW6SYS_FREE (content);
    }

  return response;
}

static _httpd_response_t *
_response_list_txt (_httpd_context_t * httpd_context,
		    lw6nod_info_t * node_info)
{
  _httpd_response_t *response = NULL;
  char *content = NULL;

  content = lw6nod_info_generate_oob_list (node_info);
  if (content)
    {
      response =
	_mod_httpd_response_from_str (httpd_context,
				      _MOD_HTTPD_STATUS_200, 1,
				      0,
				      NULL,
				      httpd_context->data.consts.
				      content_type_txt, content);
      LW6SYS_FREE (content);
    }

  return response;
}

static _httpd_response_t *
_response_ping_txt (_httpd_context_t * httpd_context,
		    lw6nod_info_t * node_info)
{
  _httpd_response_t *response = NULL;
  char *content = NULL;

  content = lw6nod_info_generate_oob_pong (node_info);
  if (content)
    {
      response =
	_mod_httpd_response_from_str (httpd_context,
				      _MOD_HTTPD_STATUS_200, 1,
				      0,
				      NULL,
				      httpd_context->data.consts.
				      content_type_txt, content);
      LW6SYS_FREE (content);
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
  char *guessed_public_url = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("process httpd oob"));

  request = _mod_httpd_request_parse_oob (httpd_context, oob_data);
  if (request)
    {
      if (request->get_head_post == _MOD_HTTPD_GET
	  || request->get_head_post == _MOD_HTTPD_HEAD)
	{
	  if (!strcmp (request->uri, _ROOT)
	      || !strcmp (request->uri, _INDEX_HTML)
	      || !strcmp (request->uri, _SCREENSHOT_JPEG))
	    {
	      dyn_info = lw6nod_info_dup_dyn (node_info);
	      if (dyn_info)
		{
		  if (!strcmp (request->uri, _ROOT)
		      || !strcmp (request->uri, _INDEX_HTML))
		    {
		      response =
			_response_index_html (httpd_context, node_info,
					      dyn_info);
		    }
		  if (!strcmp (request->uri, _SCREENSHOT_JPEG))
		    {
		      response =
			_response_screenshot_jpeg (httpd_context, node_info,
						   dyn_info);
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
	      if (!strcmp (request->uri, _INFO_TXT))
		{
		  response = _response_info_txt (httpd_context, node_info);
		}
	      if (!strcmp (request->uri, _LIST_TXT))
		{
		  response = _response_list_txt (httpd_context, node_info);
		}
	      if (!strcmp (request->uri, _PING_TXT))
		{
		  response = _response_ping_txt (httpd_context, node_info);
		}
	      if (!strcmp (request->uri, _ROBOTS_TXT))
		{
		  response =
		    _mod_httpd_response_from_str (httpd_context,
						  _MOD_HTTPD_STATUS_200, 0, 0,
						  NULL,
						  httpd_context->data.consts.
						  content_type_txt,
						  httpd_context->data.htdocs.
						  robots_txt);
		}
	      if (!strcmp (request->uri, _GPL_TXT))
		{
		  response =
		    _mod_httpd_response_from_str (httpd_context,
						  _MOD_HTTPD_STATUS_200, 0, 0,
						  NULL,
						  httpd_context->data.consts.
						  content_type_txt,
						  httpd_context->data.htdocs.
						  gpl_txt);
		}
	      if (!strcmp (request->uri, _FAVICON_ICO))
		{
		  response =
		    _mod_httpd_response_from_bin (httpd_context,
						  _MOD_HTTPD_STATUS_200, 0, 0,
						  NULL,
						  httpd_context->data.consts.
						  content_type_txt,
						  httpd_context->data.htdocs.
						  favicon_ico_size,
						  httpd_context->data.htdocs.
						  favicon_ico_data);
		}
	    }
	}
      if (request->get_head_post == _MOD_HTTPD_POST)
	{
	  response =
	    _mod_httpd_http_error (httpd_context, _MOD_HTTPD_STATUS_405);
	}
      if (!response)
	{
	  /*
	   * OK, not treated, we return an error
	   */
	  if (request->get_head_post == _MOD_HTTPD_GET)
	    {
	      /*
	       * On "GET", 404 is probably the best bet, the request
	       * might or might not be syntaxically correct, but most
	       * of the time it's just that "what's after GET is wrong"
	       * and this corresponds to 404.
	       */
	      response =
		_mod_httpd_http_error (httpd_context, _MOD_HTTPD_STATUS_404);
	    }
	  else
	    {
	      /*
	       * A typicall case in which we land here is when the
	       * client goes timeout (not necessarly an http client)
	       * and we do not want the program to wait for ages,
	       * a client with 10 seconds lag is useless anyway, so
	       * we just return http error 500 in all cases, and
	       * client (if not http client) should figure this means
	       * error.
	       */
	      response =
		_mod_httpd_http_error (httpd_context, _MOD_HTTPD_STATUS_500);
	    }
	}
      if (response)
	{
	  ret =
	    _mod_httpd_response_send (httpd_context, response,
				      oob_data->sock,
				      request->get_head_post ==
				      _MOD_HTTPD_HEAD);
	  if (ret)
	    {
	      _mod_httpd_log (httpd_context, request, response);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("request \"%s\" failed"),
			  request->uri);
	    }

	  guessed_public_url =
	    lw6sys_url_http_from_ip_port (oob_data->remote_ip,
					  LW6NET_DEFAULT_PORT);
	  if (guessed_public_url)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("discovered node \"%s\" from guessed url"),
			  guessed_public_url);
	      lw6nod_info_add_discovered_node (node_info, guessed_public_url);
	      LW6SYS_FREE (guessed_public_url);
	    }

	  _mod_httpd_response_free (response);
	}
      _mod_httpd_request_free (request);
    }

  lw6net_socket_close (oob_data->sock);
  oob_data->sock = -1;

  return ret;
}

int
_mod_httpd_oob_should_continue (_httpd_context_t * httpd_context,
				lw6srv_oob_data_t * oob_data)
{
  int ret = 0;

  ret = (_mod_httpd_timeout_ok (httpd_context,
				oob_data->creation_timestamp)
	 && lw6net_tcp_is_alive (oob_data->sock)
	 && (!oob_data->do_not_finish));

  return ret;
}
