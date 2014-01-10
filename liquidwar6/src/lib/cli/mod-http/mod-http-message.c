/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

int
_mod_http_send (_mod_http_context_t * http_context,
		lw6cnx_connection_t * connection,
		int64_t now,
		u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
		u_int64_t logical_from_id, u_int64_t logical_to_id,
		const char *message)
{
  int ret = 0;
  _mod_http_specific_data_t *specific_data =
    (_mod_http_specific_data_t *) connection->backend_specific_data;
  char *line = NULL;
  char *url = NULL;
  _mod_http_query_thread_data_t *query_thread_data = NULL;
  void *thread_handler = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("mod_http send \"%s\""), message);
  /*
   * We don't even try to use http protocol if there's a DNS problem,
   * no use to query a 192.168.X.X address (case of peer behind a
   * NAT firewall). URL needs to be *correct* for http to work.
   */
  if (connection->dns_ok)
    {
      line = lw6msg_envelope_generate (LW6MSG_ENVELOPE_MODE_URL,
				       lw6sys_build_get_version (),
				       connection->password_send_checksum,
				       physical_ticket_sig,
				       logical_ticket_sig,
				       connection->local_id_int,
				       connection->remote_id_int,
				       logical_from_id, logical_to_id,
				       message);
      if (line)
	{
	  url = lw6sys_str_concat (connection->remote_url, line);
	  if (url)
	    {
	      /*
	       * Note: here, we don't bother to acquire a connection
	       * lock, multiple URLs can be fetched in any order after
	       * all, it's not like we would be using a socket. Still
	       * there will be another lock later (over libcurl calls)
	       * because of non-reentrant gethostbyname, but this is
	       * another question. So another lock. And, even, given that,
	       * all we do here is fire another thread!
	       */
	      query_thread_data =
		(_mod_http_query_thread_data_t *)
		LW6SYS_MALLOC (sizeof (_mod_http_query_thread_data_t));
	      if (query_thread_data)
		{
		  query_thread_data->http_context = http_context;
		  query_thread_data->cnx = connection;
		  query_thread_data->url = url;
		  thread_handler =
		    lw6sys_thread_create (_mod_http_query_thread_func,
					  _mod_http_query_thread_join,
					  (void *) query_thread_data);
		  if (thread_handler)
		    {
		      lw6sys_list_push_back (&(specific_data->query_threads),
					     thread_handler);
		      ret = 1;
		    }
		}
	    }
	  LW6SYS_FREE (line);
	}
      if (!ret)
	{
	  if (url)
	    {
	      LW6SYS_FREE (url);
	    }
	  if (query_thread_data)
	    {
	      LW6SYS_FREE (query_thread_data);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("mod_http can't send, DNS mismatch"));
    }

  return ret;
}

void
_mod_http_poll (_mod_http_context_t * http_context,
		lw6cnx_connection_t * connection)
{
  _mod_http_specific_data_t *specific_data =
    (_mod_http_specific_data_t *) connection->backend_specific_data;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("mod_http poll"));

  lw6sys_list_filter (&(specific_data->query_threads),
		      _mod_http_query_thread_filter, NULL);
}
