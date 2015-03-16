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

void
_mod_http_query_thread_func (lw6sys_context_t * sys_context, void *callback_data)
{
  _mod_http_query_thread_data_t *query_thread_data = (_mod_http_query_thread_data_t *) callback_data;
  char *response = NULL;
  char *pos = NULL;
  char *seek = NULL;
  char seek_c = '\0';

  if (query_thread_data)
    {
      if (query_thread_data->url)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("processing \"%s\""), query_thread_data->url);
	  response =
	    _mod_http_get (sys_context, query_thread_data->http_context,
			   query_thread_data->url,
			   query_thread_data->cnx->password_send_checksum, query_thread_data->cnx->remote_ip, query_thread_data->cnx->remote_port);
	  if (response)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_http received content \"%s\""), response);
	      seek = pos = response;
	      while (*seek)
		{
		  while ((*seek) && !lw6sys_chr_is_eol (*seek))
		    {
		      seek++;
		    }
		  if (pos != seek)
		    {
		      seek_c = (*seek);
		      (*seek) = '\0';
		      _mod_http_query_thread_process_response_line (sys_context, query_thread_data, pos);
		      (*seek) = seek_c;
		    }
		  if (lw6sys_chr_is_eol (*seek))
		    {
		      seek++;
		    }
		  pos = seek;
		}

	      LW6SYS_FREE (sys_context, response);
	    }
	}
    }
}

void
_mod_http_query_thread_join (lw6sys_context_t * sys_context, void *callback_data)
{
  _mod_http_query_thread_data_t *query_thread_data = (_mod_http_query_thread_data_t *) callback_data;

  if (query_thread_data)
    {
      if (query_thread_data->url)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("done with processing \"%s\""), query_thread_data->url);
	  LW6SYS_FREE (sys_context, query_thread_data->url);
	}
      LW6SYS_FREE (sys_context, query_thread_data);
    }
}

void
_mod_http_query_thread_free_list_item (lw6sys_context_t * sys_context, void *data)
{
  lw6sys_thread_join (sys_context, data);
}

int
_mod_http_query_thread_filter (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int ret = 0;

  /*
   * If callback is done, free item, that is, return 0
   */
  ret = !lw6sys_thread_is_callback_done (sys_context, data);

  return ret;
}

int
_mod_http_query_thread_process_response_line (lw6sys_context_t * sys_context, _mod_http_query_thread_data_t * query_thread_data, const char *response_line)
{
  int ret = 0;
  lw6cnx_connection_t *connection = query_thread_data->cnx;
  char *msg = NULL;
  u_int32_t physical_ticket_sig = 0;
  u_int32_t logical_ticket_sig = 0;
  u_int64_t physical_from_id = 0;
  u_int64_t physical_to_id = 0;
  u_int64_t logical_from_id = 0;
  u_int64_t logical_to_id = 0;
  lw6cnx_packet_t *packet = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_http received envelope \"%s\""), response_line);
  if (lw6msg_envelope_analyse
      (sys_context, response_line, LW6MSG_ENVELOPE_MODE_TELNET,
       connection->local_url, connection->password,
       connection->remote_id_int,
       connection->local_id_int, &msg, &physical_ticket_sig, &logical_ticket_sig, &physical_from_id, &physical_to_id, &logical_from_id, &logical_to_id, NULL))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mod_http analysed msg \"%s\""), msg);
      ret = 1;
      packet = lw6cnx_packet_new (sys_context, logical_ticket_sig, physical_ticket_sig, logical_from_id, logical_to_id, msg);
      if (packet && connection->recv_list)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("pushing msg \"%s\" to list_r"), msg);
	  lw6sys_list_r_push_front (sys_context, connection->recv_list, packet);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to push msg \"%s\" to list_r packet=%p recv_list=%p"), msg, packet, connection->recv_list);
	}
      LW6SYS_FREE (sys_context, msg);
    }

  return ret;
}
