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

#define _MOD_HTTPD_PROTOCOL_GET_STRING "GET"
#define _MOD_HTTPD_PROTOCOL_GET_SIZE 3
#define _MOD_HTTPD_PROTOCOL_POST_STRING "POST"
#define _MOD_HTTPD_PROTOCOL_POST_SIZE 4
#define _MOD_HTTPD_PROTOCOL_HEAD_STRING "HEAD"
#define _MOD_HTTPD_PROTOCOL_HEAD_SIZE 4

typedef struct _httpd_context_s
{
  int dummy_httpd;
}
_httpd_context_t;

/*
 * In setup.c
 */
extern _httpd_context_t *_mod_httpd_init ();
extern void _mod_httpd_quit (_httpd_context_t * httpd_context);

/*
 * In handshake.c
 */
extern int _mod_httpd_analyse_tcp (_httpd_context_t * httpd_context,
				   lw6srv_tcp_accepter_t * tcp_accepter);
extern int _mod_httpd_analyse_udp (_httpd_context_t * httpd_context,
				   lw6srv_udp_buffer_t * udp_buffer);
extern lw6srv_connection_t *_mod_httpd_accept_tcp (_httpd_context_t *
						   httpd_context,
						   lw6srv_tcp_accepter_t *
						   tcp_accepter,
						   char *password);
extern lw6srv_connection_t *_mod_httpd_new_udp (_httpd_context_t *
						httpd_context,
						lw6srv_udp_buffer_t *
						udp_buffer, char *password);
extern int _mod_httpd_is_associated_with_udp (_httpd_context_t *
					      httpd_context,
					      lw6srv_connection_t *
					      connection,
					      lw6srv_udp_buffer_t *
					      udp_buffer);
extern int _mod_httpd_update_with_udp (_httpd_context_t * httpd_context,
				       lw6srv_connection_t * connection,
				       lw6srv_udp_buffer_t * udp_buffer);

/*
 * In state.c
 */
extern void _mod_httpd_close (_httpd_context_t * httpd_context,
			      lw6srv_connection_t * connection);
extern int _mod_httpd_is_alive (_httpd_context_t * httpd_context,
				lw6srv_connection_t * connection);

/*
 * In message.c
 */
extern int _mod_httpd_send (_httpd_context_t * httpd_context,
			    lw6srv_connection_t * connection, char *message);
extern char *_mod_httpd_recv (_httpd_context_t * httpd_context,
			      lw6srv_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_httpd_repr (_httpd_context_t * httpd_context,
			      lw6srv_connection_t * connection);
extern char *_mod_httpd_error (_httpd_context_t * httpd_context,
			       lw6srv_connection_t * connection);

/* mod-httpd-oob.c */
extern int _mod_httpd_process_oob (_httpd_context_t * httpd_context,
				   lw6srv_oob_data_t * oob_data);

#endif
