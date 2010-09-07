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

#ifndef LIQUIDWAR6SRV_HTTPD_INTERNAL_H
#define LIQUIDWAR6SRV_HTTPD_INTERNAL_H

#include "../srv.h"

#define _MOD_HTTPD_PROTOCOL_UNDERSTANDABLE_SIZE 8
#define _MOD_HTTPD_PROTOCOL_GET_STRING "GET"
#define _MOD_HTTPD_PROTOCOL_GET_SIZE 3
#define _MOD_HTTPD_PROTOCOL_POST_STRING "POST"
#define _MOD_HTTPD_PROTOCOL_POST_SIZE 4
#define _MOD_HTTPD_PROTOCOL_HEAD_STRING "HEAD"
#define _MOD_HTTPD_PROTOCOL_HEAD_SIZE 4
#define _MOD_HTTPD_PROTOCOL_LW6_STRING "/lw6"
#define _MOD_HTTPD_PROTOCOL_LW6_SIZE 4

#define _MOD_HTTPD_GET 1
#define _MOD_HTTPD_POST 2
#define _MOD_HTTPD_HEAD 3

#define _MOD_HTTPD_STATUS_200 200
#define _MOD_HTTPD_STATUS_401 401
#define _MOD_HTTPD_STATUS_403 403
#define _MOD_HTTPD_STATUS_404 404
#define _MOD_HTTPD_STATUS_405 405
#define _MOD_HTTPD_STATUS_500 500

#define _MOD_HTTPD_OOB_ROOT "/"
#define _MOD_HTTPD_OOB_INDEX_HTML "/index.html"
#define _MOD_HTTPD_OOB_SCREENSHOT_JPEG "/screenshot.jpeg"
#define _MOD_HTTPD_OOB_ROBOTS_TXT "/robots.txt"
#define _MOD_HTTPD_OOB_GPL_TXT "/gpl.txt"
#define _MOD_HTTPD_OOB_FAVICON_ICO "/favicon.ico"
#define _MOD_HTTPD_OOB_INFO_TXT "/info.txt"
#define _MOD_HTTPD_OOB_LIST_TXT "/list.txt"
#define _MOD_HTTPD_OOB_PING_TXT "/ping.txt"

typedef struct _httpd_consts_s
{
  int error_timeout;
  int max_age;
  int in_the_past;
  int refresh_index_header;
  int refresh_screenshot_header;
  int refresh_index_js;
  int refresh_screenshot_js;
  char *http_version;
  char *header_description;
  char *header_keywords;
  char *content_type_html;
  char *content_type_txt;
  char *content_type_jpeg;
  char *content_type_ico;
  char *error_401;
  char *error_403;
  char *error_404;
  char *error_405;
  char *error_500;
  char *auth_realm;
}
_httpd_consts_t;

typedef struct _httpd_htdocs_s
{
  char *index_html;
  char *error_html;
  char *robots_txt;
  char *gpl_txt;
  int favicon_ico_size;
  void *favicon_ico_data;
}
_httpd_htdocs_t;

typedef struct _httpd_data_s
{
  _httpd_consts_t consts;
  _httpd_htdocs_t htdocs;
}
_httpd_data_t;

typedef struct _httpd_context_s
{
  _httpd_data_t data;
  char *access_log_file;
  void *access_log_mutex;
}
_httpd_context_t;

typedef struct _httpd_request_s
{
  char *client_ip;
  char *first_line;
  int get_head_post;
  char *uri;
  char *http_user;
  char *http_password;
  int password_ok;
} _httpd_request_t;

typedef struct _httpd_response_s
{
  int status;
  int no_cache;
  int refresh_sec;
  char *refresh_url;
  char *content_type;
  int content_size;
  void *content_data;
} _httpd_response_t;

typedef struct _httpd_specific_data_s
{
  int dummy;
  // todo: list of messages waiting to be sent
}
_httpd_specific_data_t;

/* mod-httpd-data.c */
extern int _mod_httpd_load_data (_httpd_data_t * httpd_data, char *data_dir);
extern void _mod_httpd_unload_data (_httpd_data_t * httpd_data);

/* mod-httpd-error.c */
extern _httpd_response_t *_mod_httpd_http_error (_httpd_context_t *
						 httpd_context, int status);

/*
 * In setup.c
 */
extern _httpd_context_t *_mod_httpd_init (int argc, char *argv[],
					  lw6srv_listener_t * listener);
extern void _mod_httpd_quit (_httpd_context_t * httpd_context);

/*
 * In handshake.c
 */
extern int _mod_httpd_analyse_tcp (_httpd_context_t * httpd_context,
				   lw6srv_tcp_accepter_t * tcp_accepter,
				   lw6nod_info_t * node_info,
				   u_int64_t * remote_id, char **remote_url);
extern int _mod_httpd_analyse_udp (_httpd_context_t * httpd_context,
				   lw6srv_udp_buffer_t * udp_buffer,
				   lw6nod_info_t * node_info,
				   u_int64_t * remote_id, char **remote_url);
extern int _mod_httpd_feed_with_tcp (_httpd_context_t * httpd_context,
				     lw6cnx_connection_t * connection,
				     lw6srv_tcp_accepter_t * tcp_accepter);
extern int _mod_httpd_feed_with_udp (_httpd_context_t * httpd_context,
				     lw6cnx_connection_t * connection,
				     lw6srv_udp_buffer_t * udp_buffer);

/* mod-httpd-log.c */
int _mod_httpd_log (_httpd_context_t * httpd_context,
		    _httpd_request_t * request, _httpd_response_t * response);

/*
 * In state.c
 */
extern lw6cnx_connection_t *_mod_httpd_open (_httpd_context_t * httpd_context,
					     lw6srv_listener_t * listener,
					     char *local_url,
					     char *remote_url,
					     char *remote_ip, int remote_port,
					     char *password,
					     u_int64_t local_id,
					     u_int64_t remote_id,
					     lw6cnx_recv_callback_t
					     recv_callback_func, void *recv_callback_data);
extern void _mod_httpd_close (_httpd_context_t * httpd_context,
			      lw6cnx_connection_t * connection);
extern int _mod_httpd_is_alive (_httpd_context_t * httpd_context,
				lw6cnx_connection_t * connection);
extern int _mod_httpd_timeout_ok (_httpd_context_t * httpd_context,
				  int64_t origin_timestamp);

/*
 * In message.c
 */
extern int _mod_httpd_send (_httpd_context_t * httpd_context,
			    lw6cnx_connection_t * connection,
			    u_int32_t physical_ticket_sig,
			    u_int32_t logical_ticket_sig,
			    u_int64_t logical_from_id,
			    u_int64_t logical_to_id, char *message);
extern void _mod_httpd_poll (_httpd_context_t * httpd_context,
			     lw6cnx_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_httpd_repr (_httpd_context_t * httpd_context,
			      lw6cnx_connection_t * connection);
extern char *_mod_httpd_error (_httpd_context_t * httpd_context,
			       lw6cnx_connection_t * connection);

/* mod-httpd-oob.c */
extern int _mod_httpd_process_oob (_httpd_context_t * httpd_context,
				   lw6nod_info_t * node_info,
				   lw6srv_oob_data_t * oob_data);
extern int _mod_httpd_oob_should_continue (_httpd_context_t * httpd_context,
					   lw6srv_oob_data_t * oob_data);

/* mod-httpd-request.c */
extern _httpd_request_t *_mod_httpd_request_parse_oob (_httpd_context_t *
						       httpd_context,
						       lw6nod_info_t *
						       node_info,
						       lw6srv_oob_data_t *
						       oob_data);
extern void _mod_httpd_request_free (_httpd_request_t * request);

/* mod-httpd-response.c */
extern _httpd_response_t *_mod_httpd_response_from_bin (_httpd_context_t *
							httpd_context,
							int status,
							int no_cache,
							int refresh_sec,
							char *refresh_url,
							char *content_type,
							int content_size,
							void *content_data);
extern _httpd_response_t *_mod_httpd_response_from_str (_httpd_context_t *
							httpd_context,
							int status,
							int no_cache,
							int refresh_sec,
							char *refresh_url,
							char *content_type,
							char *content);
extern void _mod_httpd_response_free (_httpd_response_t * response);
extern int _mod_httpd_response_send (_httpd_context_t * httpd_context,
				     _httpd_response_t * response, int sock,
				     int headers_only);

#endif
