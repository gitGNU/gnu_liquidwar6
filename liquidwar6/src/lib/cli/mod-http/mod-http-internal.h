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

#ifndef LIQUIDWAR6CLI_HTTP_INTERNAL_H
#define LIQUIDWAR6CLI_HTTP_INTERNAL_H

#include "../cli.h"

#define CURL_NO_OLDIES 1
#include <curl/curl.h>
/*
 * curl/types.h used to be usefull but recent
 * versions of curl seem to not have it anymore
 */
#ifdef HAVE_CURL_TYPES_H
#include <curl/types.h>
#endif
#include <curl/easy.h>

#define _MOD_HTTP_OOB_INFO_TXT "info.txt"
#define _MOD_HTTP_OOB_LIST_TXT "list.txt"
#define _MOD_HTTP_OOB_PING_TXT "ping.txt"

typedef struct _mod_http_consts_s
{
  int global_timeout;
  int connect_timeout;
}
_mod_http_consts_t;

typedef struct _mod_http_data_s
{
  _mod_http_consts_t consts;
}
_mod_http_data_t;

typedef struct _mod_http_context_s
{
  int curl_init_ret;
  _mod_http_data_t data;
}
_mod_http_context_t;

typedef struct _mod_http_query_thread_data_s
{
  _mod_http_context_t *http_context;
  lw6cnx_connection_t *cnx;
  char *url;
} _mod_http_query_thread_data_t;

typedef struct _mod_http_specific_data_s
{
  lw6sys_list_t *query_threads;
}
_mod_http_specific_data_t;

/* mod-http-data.c */
extern int _mod_http_load_data (_mod_http_data_t * http_data,
				const char *data_dir);
extern void _mod_http_unload_data (_mod_http_data_t * http_data);

/*
 * In setup.c
 */
extern _mod_http_context_t *_mod_http_init (int argc, const char *argv[]);
extern void _mod_http_quit (_mod_http_context_t * http_context);

/* http-get.c */
extern char *_mod_http_get (_mod_http_context_t * http_context, char *url,
			    char *password);

/* http-querythread.c */
extern void _mod_http_query_thread_func (void *callback_data);
extern void _mod_http_query_thread_join (void *callback_data);
extern void _mod_http_query_thread_free_list_item (void *data);
extern int _mod_http_query_thread_filter (void *func_data, void *data);
extern int
_mod_http_query_thread_process_response_line (_mod_http_query_thread_data_t *
					      query_thread_data,
					      char *response_line);

/*
 * In state.c
 */
extern lw6cnx_connection_t *_mod_http_open (_mod_http_context_t *
					    http_context, char *local_url,
					    char *remote_url, char *remote_ip,
					    int remote_port, char *password,
					    u_int64_t local_id,
					    u_int64_t remote_id, int dns_ok,
					    int network_reliability,
					    lw6cnx_recv_callback_t
					    recv_callback_func,
					    void *recv_callback_data);
extern void _mod_http_close (_mod_http_context_t * http_context,
			     lw6cnx_connection_t * connection);
extern int _mod_http_timeout_ok (_mod_http_context_t * http_context,
				 int64_t origin_timestamp);

/*
 * In message.c
 */
extern int _mod_http_send (_mod_http_context_t * http_context,
			   lw6cnx_connection_t * connection,
			   u_int32_t physical_ticket_sig,
			   u_int32_t logical_ticket_sig,
			   u_int64_t logical_from_id, u_int64_t logical_to_id,
			   char *message);
extern void _mod_http_poll (_mod_http_context_t * http_context,
			    lw6cnx_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_http_repr (_mod_http_context_t * http_context,
			     lw6cnx_connection_t * connection);

/*
 * In oob.c
 */
extern int _mod_http_process_oob (_mod_http_context_t * http_context,
				  lw6nod_info_t * node_info,
				  lw6cli_oob_data_t * oob_data);
extern int _mod_http_oob_should_continue (_mod_http_context_t *
					  http_context,
					  lw6cli_oob_data_t * oob_data);

#endif
