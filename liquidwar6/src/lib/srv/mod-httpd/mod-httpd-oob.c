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

#define _SCREENSHOT_JPEG_REFRESH "screenshot.jpeg"
#define _DUMMY_RANGE 1000000

static void
_add_node_html (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  char **list = (char **) func_data;
  char *tmp = NULL;
  lw6nod_info_t *verified_node = (lw6nod_info_t *) data;
  char *title = NULL;
  char *description = NULL;
  char *escaped_url = NULL;
  char *escaped_title = NULL;
  char *escaped_description = NULL;

  if (list && (*list) && verified_node && verified_node->const_info.ref_info.url)
    {
      title = verified_node->const_info.title;
      if ((!title) || (title && strlen (title) == 0))
	{
	  title = verified_node->const_info.ref_info.url;
	}
      description = verified_node->const_info.description;
      if ((!description) || (description && strlen (description) == 0))
	{
	  description = verified_node->const_info.ref_info.url;
	}
      escaped_url = lw6sys_escape_html_attribute (sys_context, verified_node->const_info.ref_info.url);
      if (escaped_url)
	{
	  escaped_title = lw6sys_escape_html_attribute (sys_context, title);
	  if (escaped_title)
	    {
	      escaped_description = lw6sys_escape_html_attribute (sys_context, description);
	      if (escaped_description)
		{
		  if (strlen (*list) > 0)
		    {
		      tmp = lw6sys_new_sprintf (sys_context, "%s, <a href=\"%s\" title=\"%s\">%s</a>", *list, escaped_url, escaped_description, escaped_title);
		    }
		  else
		    {
		      tmp = lw6sys_new_sprintf (sys_context, "<a href=\"%s\" title=\"%s\">%s</a>", escaped_url, escaped_description, escaped_title);
		    }
		  LW6SYS_FREE (sys_context, escaped_description);
		}
	      LW6SYS_FREE (sys_context, escaped_title);
	    }
	  LW6SYS_FREE (sys_context, escaped_url);
	}
      if (tmp)
	{
	  LW6SYS_FREE (sys_context, *list);
	  (*list) = tmp;
	}
    }
}

static _mod_httpd_response_t *
_response_index_html (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6nod_info_t * node_info, lw6nod_dyn_info_t * dyn_info)
{
  _mod_httpd_response_t *response = NULL;
  char *content = NULL;
  char *screenshot_url = NULL;
  char *level = "";
  char *list = NULL;
  int dummy = 0;
  char *uptime = NULL;
  char *escaped_title = NULL;
  char *escaped_description = NULL;
  char *escaped_level = NULL;

  dummy = lw6sys_random (sys_context, _DUMMY_RANGE);

  escaped_title = lw6sys_escape_html_attribute (sys_context, node_info->const_info.title);
  if (escaped_title)
    {
      escaped_description = lw6sys_escape_html_attribute (sys_context, node_info->const_info.description);
      if (escaped_description)
	{
	  if (dyn_info->level)
	    {
	      level = dyn_info->level;
	    }
	  escaped_level = lw6sys_escape_html_attribute (sys_context, level);
	  if (escaped_level)
	    {
	      screenshot_url = lw6sys_str_concat (sys_context, node_info->const_info.ref_info.url, _SCREENSHOT_JPEG_REFRESH);
	      if (screenshot_url)
		{
		  uptime = lw6sys_readable_uptime (sys_context, lw6sys_get_timestamp (sys_context) - node_info->const_info.creation_timestamp);
		  if (uptime)
		    {
		      list = lw6sys_str_copy (sys_context, LW6SYS_STR_EMPTY);
		      if (list)
			{
			  lw6nod_info_map_verified_nodes (sys_context, node_info, _add_node_html, &list);
			  content = lw6sys_new_sprintf (sys_context, httpd_context->data.htdocs.index_html,
							/*
							 * Variables in the HEAD section
							 */
							httpd_context->data.consts.refresh_index_header,
							node_info->const_info.ref_info.url,
							escaped_title,
							lw6sys_build_get_package_name
							(),
							httpd_context->data.consts.header_description,
							httpd_context->data.consts.header_keywords,
							lw6sys_build_get_copyright (), lw6sys_build_get_package_tarname (),
							/*
							 * Variables for JavaScript use
							 */
							node_info->const_info.ref_info.url,
							httpd_context->data.consts.refresh_screenshot_js *
							1000, _DUMMY_RANGE,
							screenshot_url,
							httpd_context->data.consts.refresh_index_js *
							1000, httpd_context->data.consts.refresh_screenshot_js * 1000,
							/*
							 * Variables in the BODY section
							 */
							escaped_title, escaped_description, dummy,
							/*
							 * Version
							 */
							lw6sys_build_get_version (), lw6sys_build_get_codename (),
							/*
							 * Info
							 */
							uptime,
							escaped_level,
							node_info->const_info.bench,
							dyn_info->required_bench,
							dyn_info->nb_colors,
							dyn_info->max_nb_colors,
							dyn_info->nb_cursors, dyn_info->max_nb_cursors, dyn_info->nb_nodes, dyn_info->max_nb_nodes,
							/*
							 * List
							 */
							list);
			  if (content)
			    {
			      response =
				_mod_httpd_response_from_str (sys_context, httpd_context,
							      _MOD_HTTPD_STATUS_200,
							      1,
							      httpd_context->data.consts.refresh_index_header,
							      node_info->const_info.ref_info.url, httpd_context->data.consts.content_type_html, content);

			      LW6SYS_FREE (sys_context, content);
			    }
			  LW6SYS_FREE (sys_context, list);
			}
		      LW6SYS_FREE (sys_context, uptime);
		    }
		  LW6SYS_FREE (sys_context, screenshot_url);
		}
	      LW6SYS_FREE (sys_context, escaped_level);
	    }
	  LW6SYS_FREE (sys_context, escaped_description);
	}
      LW6SYS_FREE (sys_context, escaped_title);
    }

  return response;
}

static _mod_httpd_response_t *
_response_screenshot_jpeg (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6nod_info_t * node_info, lw6nod_dyn_info_t * dyn_info)
{
  _mod_httpd_response_t *response = NULL;
  int screenshot_size = 0;
  void *screenshot_data = NULL;
  char *refresh_url = NULL;

  if (dyn_info->game_screenshot_size > 0 && dyn_info->game_screenshot_data != NULL)
    {
      screenshot_size = dyn_info->game_screenshot_size;
      screenshot_data = dyn_info->game_screenshot_data;
    }
  else
    {
      screenshot_size = node_info->const_info.idle_screenshot_size;
      screenshot_data = node_info->const_info.idle_screenshot_data;
    }
  refresh_url = lw6sys_str_concat (sys_context, node_info->const_info.ref_info.url, _SCREENSHOT_JPEG_REFRESH);
  if (refresh_url)
    {
      if (screenshot_size > 0 && screenshot_data != NULL)
	{
	  response =
	    _mod_httpd_response_from_bin (sys_context, httpd_context,
					  _MOD_HTTPD_STATUS_200, 1,
					  httpd_context->data.consts.refresh_screenshot_header,
					  refresh_url, httpd_context->data.consts.content_type_jpeg, screenshot_size, screenshot_data);
	}
      LW6SYS_FREE (sys_context, refresh_url);
    }
  return response;
}

static _mod_httpd_response_t *
_response_info_txt (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6nod_info_t * node_info)
{
  _mod_httpd_response_t *response = NULL;
  char *content = NULL;

  content = lw6msg_oob_generate_info (sys_context, node_info);
  if (content)
    {
      response =
	_mod_httpd_response_from_str (sys_context, httpd_context, _MOD_HTTPD_STATUS_200, 1, 0, NULL, httpd_context->data.consts.content_type_txt, content);
      LW6SYS_FREE (sys_context, content);
    }

  return response;
}

static _mod_httpd_response_t *
_response_list_txt (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6nod_info_t * node_info)
{
  _mod_httpd_response_t *response = NULL;
  char *content = NULL;

  content = lw6msg_oob_generate_list (sys_context, node_info);
  if (content)
    {
      response =
	_mod_httpd_response_from_str (sys_context, httpd_context, _MOD_HTTPD_STATUS_200, 1, 0, NULL, httpd_context->data.consts.content_type_txt, content);
      LW6SYS_FREE (sys_context, content);
    }

  return response;
}

static _mod_httpd_response_t *
_response_ping_txt (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6nod_info_t * node_info)
{
  _mod_httpd_response_t *response = NULL;
  char *content = NULL;

  content = lw6msg_oob_generate_pong (sys_context, node_info);
  if (content)
    {
      response =
	_mod_httpd_response_from_str (sys_context, httpd_context, _MOD_HTTPD_STATUS_200, 1, 0, NULL, httpd_context->data.consts.content_type_txt, content);
      LW6SYS_FREE (sys_context, content);
    }

  return response;
}

int
_mod_httpd_process_oob (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6nod_info_t * node_info, lw6srv_oob_data_t * oob_data)
{
  int ret = 0;
  _mod_httpd_request_t *request = NULL;
  _mod_httpd_response_t *response = NULL;
  lw6nod_dyn_info_t *dyn_info = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process httpd oob"));

  request = _mod_httpd_request_parse_oob (sys_context, httpd_context, node_info, oob_data);
  if (request)
    {
      if (request->get_head_post == _MOD_HTTPD_GET || request->get_head_post == _MOD_HTTPD_HEAD)
	{
	  if (request->password_ok)
	    {
	      if (!strcmp (request->uri, _MOD_HTTPD_OOB_ROOT)
		  || !strcmp (request->uri, _MOD_HTTPD_OOB_INDEX_HTML) || !strcmp (request->uri, _MOD_HTTPD_OOB_SCREENSHOT_JPEG))
		{
		  dyn_info = lw6nod_info_dup_dyn (sys_context, node_info);
		  if (dyn_info)
		    {
		      if (!strcmp (request->uri, _MOD_HTTPD_OOB_ROOT) || !strcmp (request->uri, _MOD_HTTPD_OOB_INDEX_HTML))
			{
			  response = _response_index_html (sys_context, httpd_context, node_info, dyn_info);
			}
		      if (!strcmp (request->uri, _MOD_HTTPD_OOB_SCREENSHOT_JPEG))
			{
			  response = _response_screenshot_jpeg (sys_context, httpd_context, node_info, dyn_info);
			}
		      lw6nod_dyn_info_free (sys_context, dyn_info);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate dyn_info"));
		    }
		}
	      else
		{
		  if (!strcmp (request->uri, _MOD_HTTPD_OOB_INFO_TXT))
		    {
		      response = _response_info_txt (sys_context, httpd_context, node_info);
		    }
		  if (!strcmp (request->uri, _MOD_HTTPD_OOB_LIST_TXT))
		    {
		      response = _response_list_txt (sys_context, httpd_context, node_info);
		    }
		  if (!strcmp (request->uri, _MOD_HTTPD_OOB_PING_TXT))
		    {
		      response = _response_ping_txt (sys_context, httpd_context, node_info);
		    }
		  if (!strcmp (request->uri, _MOD_HTTPD_OOB_ROBOTS_TXT))
		    {
		      response =
			_mod_httpd_response_from_str (sys_context, httpd_context,
						      _MOD_HTTPD_STATUS_200,
						      0, 0, NULL, httpd_context->data.consts.content_type_txt, httpd_context->data.htdocs.robots_txt);
		    }
		  if (!strcmp (request->uri, _MOD_HTTPD_OOB_GPL_TXT))
		    {
		      response =
			_mod_httpd_response_from_str (sys_context, httpd_context,
						      _MOD_HTTPD_STATUS_200,
						      0, 0, NULL, httpd_context->data.consts.content_type_txt, httpd_context->data.htdocs.gpl_txt);
		    }
		  if (!strcmp (request->uri, _MOD_HTTPD_OOB_FAVICON_ICO))
		    {
		      response =
			_mod_httpd_response_from_bin (sys_context, httpd_context,
						      _MOD_HTTPD_STATUS_200,
						      0, 0, NULL,
						      httpd_context->data.consts.content_type_txt,
						      httpd_context->data.htdocs.favicon_ico_size, httpd_context->data.htdocs.favicon_ico_data);
		    }
		}
	    }
	  else
	    {
	      response = _mod_httpd_http_error (sys_context, httpd_context, _MOD_HTTPD_STATUS_401);
	    }
	}
      if (request->get_head_post == _MOD_HTTPD_POST)
	{
	  response = _mod_httpd_http_error (sys_context, httpd_context, _MOD_HTTPD_STATUS_405);
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
	      response = _mod_httpd_http_error (sys_context, httpd_context, _MOD_HTTPD_STATUS_404);
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
	      response = _mod_httpd_http_error (sys_context, httpd_context, _MOD_HTTPD_STATUS_500);
	    }
	}
      if (response)
	{
	  ret = _mod_httpd_response_send (sys_context, httpd_context, response, &(oob_data->sock), request->get_head_post == _MOD_HTTPD_HEAD);
	  if (ret)
	    {
	      _mod_httpd_log (sys_context, httpd_context, request, response);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request \"%s\" failed"), request->uri);
	    }

	  _mod_httpd_response_free (sys_context, response);
	}
      _mod_httpd_request_free (sys_context, request);
    }

  lw6net_socket_close (sys_context, &(oob_data->sock));

  return ret;
}

int
_mod_httpd_oob_should_continue (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, lw6srv_oob_data_t * oob_data)
{
  int ret = 0;

  ret = (_mod_httpd_timeout_ok (sys_context, httpd_context, oob_data->creation_timestamp) && lw6net_tcp_is_alive (sys_context, &(oob_data->sock))
	 && (!oob_data->do_not_finish));

  return ret;
}
