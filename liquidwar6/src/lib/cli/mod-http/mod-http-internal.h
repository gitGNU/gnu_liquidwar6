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

#ifndef LIQUIDWAR6CLI_HTTP_INTERNAL_H
#define LIQUIDWAR6CLI_HTTP_INTERNAL_H

#include "../cli.h"

#define CURL_NO_OLDIES 1
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#define _MOD_HTTP_OOB_INFO_TXT "info.txt"
#define _MOD_HTTP_OOB_LIST_TXT "list.txt"
#define _MOD_HTTP_OOB_PING_TXT "ping.txt"

typedef struct _http_consts_s
{
  int global_timeout;
  int connect_timeout;
}
_http_consts_t;

typedef struct _http_data_s
{
  _http_consts_t consts;
}
_http_data_t;

typedef struct _http_context_s
{
  int curl_init_ret;
  _http_data_t data;
}
_http_context_t;

typedef struct _http_specific_data_s
{
  int dummy;
  // todo: list of messages waiting to be sent
}
_http_specific_data_t;

/* mod-http-data.c */
extern int _mod_http_load_data (_http_data_t * http_data, char *data_dir);
extern void _mod_http_unload_data (_http_data_t * http_data);

/*
 * In setup.c
 */
extern _http_context_t *_mod_http_init (int argc, char *argv[]);
extern void _mod_http_quit (_http_context_t * http_context);

/* http-get.c */
extern char *_mod_http_get (_http_context_t * http_context, char *url,
			    char *password);

/*
 * In state.c
 */
extern lw6cli_connection_t *_mod_http_open (_http_context_t * http_context,
					    char *remote_url,
					    char *password_checksum,
					    u_int64_t local_id,
					    u_int64_t remote_id);
extern void _mod_http_close (_http_context_t * http_context,
			     lw6cli_connection_t * connection);
extern int _mod_http_is_alive (_http_context_t * http_context,
			       lw6cli_connection_t * connection);
extern int _mod_http_timeout_ok (_http_context_t * http_context,
				 int64_t origin_timestamp);

/*
 * In message.c
 */
extern int _mod_http_send (_http_context_t * http_context,
			   lw6cli_connection_t * connection, char *message);
extern void _mod_http_poll (_http_context_t * http_context,
			    lw6cli_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_http_repr (_http_context_t * http_context,
			     lw6cli_connection_t * connection);
extern char *_mod_http_error (_http_context_t * http_context,
			      lw6cli_connection_t * connection);

/*
 * In oob.c
 */
extern int _mod_http_process_oob (_http_context_t * http_context,
				  lw6nod_info_t * node_info,
				  lw6cli_oob_data_t * oob_data);
extern int _mod_http_oob_should_continue (_http_context_t *
					  http_context,
					  lw6cli_oob_data_t * oob_data);

#endif
