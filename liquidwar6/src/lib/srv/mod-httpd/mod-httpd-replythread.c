/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

void
_mod_httpd_reply_thread_func (void *callback_data)
{
  _mod_httpd_reply_thread_data_t *reply_thread_data =
    (_mod_httpd_reply_thread_data_t *) callback_data;
  _mod_httpd_context_t *httpd_context = reply_thread_data->httpd_context;
  lw6cnx_connection_t *cnx = reply_thread_data->cnx;
  _mod_httpd_request_t *request = NULL;
  _mod_httpd_response_t *response = NULL;
  char *envelope_line = NULL;
  char *msg = NULL;
  u_int32_t physical_ticket_sig = 0;
  u_int32_t logical_ticket_sig = 0;
  u_int64_t physical_from_id = 0;
  u_int64_t physical_to_id = 0;
  u_int64_t logical_from_id = 0;
  u_int64_t logical_to_id = 0;

  if (reply_thread_data)
    {
      if (_mod_httpd_reply_thread_should_continue (reply_thread_data))
	{
	  request = _mod_httpd_request_parse_cmd (reply_thread_data);
	  if (request)
	    {
	      if (request->get_head_post == _MOD_HTTPD_GET)
		{
		  envelope_line = request->uri;
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_ ("mod_httpd received envelope \"%s\""),
			      envelope_line);
		  if (lw6msg_envelope_analyse
		      (envelope_line, LW6MSG_ENVELOPE_MODE_URL,
		       cnx->local_url, cnx->password,
		       cnx->remote_id_int,
		       cnx->local_id_int, &msg,
		       &physical_ticket_sig, &logical_ticket_sig,
		       &physical_from_id, &physical_to_id,
		       &logical_from_id, &logical_to_id, NULL))
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("mod_httpd analysed msg \"%s\""), msg);
		      if (cnx->recv_callback_func)
			{
			  cnx->recv_callback_func
			    (cnx->recv_callback_data,
			     (void *) cnx, physical_ticket_sig,
			     logical_ticket_sig, logical_from_id,
			     logical_to_id, msg);
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG,
				      _x_ ("no recv callback defined"));
			}
		      /*
		       * Give response only once we've called
		       * recv_callback_func, in case we
		       * need to reply right back to
		       * a ping for instance.
		       */
		      response =
			_mod_httpd_reply_thread_response (reply_thread_data);
		      LW6SYS_FREE (msg);
		    }
		}
	      else
		{
		  /*
		   * We don't accept HEAD or POST for in-band 
		   * messages.
		   */
		  response =
		    _mod_httpd_http_error (httpd_context,
					   _MOD_HTTPD_STATUS_405);
		}
	      if (!response)
		{
		  response =
		    _mod_httpd_http_error (httpd_context,
					   _MOD_HTTPD_STATUS_500);
		}
	      if (response)
		{
		  if (_mod_httpd_response_send (httpd_context, response,
						reply_thread_data->sock, 0))
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("mod_httpd sent \"%s\""),
				  response->content_data);
		      _mod_httpd_log (httpd_context, request, response);
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _x_ ("request \"%s\" failed"),
				  request->uri);
		    }
		  _mod_httpd_response_free (response);
		}
	      _mod_httpd_request_free (request);
	    }
	}
    }
}

void
_mod_httpd_reply_thread_join (void *callback_data)
{
  _mod_httpd_reply_thread_data_t *reply_thread_data =
    (_mod_httpd_reply_thread_data_t *) callback_data;

  if (reply_thread_data)
    {
      if (lw6net_socket_is_valid (reply_thread_data->sock))
	{
	  lw6net_socket_close (reply_thread_data->sock);
	}
      LW6SYS_FREE (reply_thread_data);
    }
}

void
_mod_httpd_reply_thread_free_list_item (void *handler)
{
  _mod_httpd_reply_thread_data_t *reply_thread_data = NULL;

  reply_thread_data =
    (_mod_httpd_reply_thread_data_t *) lw6sys_thread_get_data (handler);
  reply_thread_data->do_not_finish = 1;

  lw6sys_thread_join (handler);
}

int
_mod_httpd_reply_thread_filter (void *func_data, void *data)
{
  int ret = 0;

  /*
   * If callback is done, free item, that is, return 0
   */
  ret = !lw6sys_thread_is_callback_done (data);

  return ret;
}

int
_mod_httpd_reply_thread_should_continue (_mod_httpd_reply_thread_data_t *
					 reply_thread_data)
{
  int ret = 0;

  ret = (_mod_httpd_timeout_ok (reply_thread_data->httpd_context,
				reply_thread_data->creation_timestamp)
	 && lw6net_tcp_is_alive (reply_thread_data->sock)
	 && (!reply_thread_data->do_not_finish));

  return ret;
}

_mod_httpd_response_t *
_mod_httpd_reply_thread_response (_mod_httpd_reply_thread_data_t *
				  reply_thread_data)
{
  _mod_httpd_context_t *httpd_context = reply_thread_data->httpd_context;
  lw6cnx_connection_t *cnx = reply_thread_data->cnx;
  _mod_httpd_specific_data_t *specific_data =
    (_mod_httpd_specific_data_t *) cnx->backend_specific_data;
  _mod_httpd_response_t *response = NULL;
  char *send_buffer = NULL;

  if (lw6cnx_connection_lock_send (cnx))
    {
      if (specific_data->send_buffer)
	{
	  send_buffer = specific_data->send_buffer;
	  specific_data->send_buffer = NULL;
	}
      lw6cnx_connection_unlock_send (cnx);
    }

  if (!send_buffer)
    {
      send_buffer = lw6sys_str_copy (LW6SYS_STR_EMPTY);
    }

  if (send_buffer)
    {
      response =
	_mod_httpd_response_from_str (httpd_context,
				      _MOD_HTTPD_STATUS_200,
				      0, 0, NULL,
				      httpd_context->data.
				      consts.content_type_txt, send_buffer);
      LW6SYS_FREE (send_buffer);
    }

  return response;
}
