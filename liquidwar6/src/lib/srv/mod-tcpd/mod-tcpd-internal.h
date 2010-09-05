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

#ifndef LIQUIDWAR6SRV_TCPD_INTERNAL_H
#define LIQUIDWAR6SRV_TCPD_INTERNAL_H

#include "../srv.h"

#define _MOD_TCPD_PROTOCOL_LW6_STRING "LW6"
#define _MOD_TCPD_PROTOCOL_LW6_SIZE 3
#define _MOD_TCPD_PROTOCOL_N_STRING "\n"
#define _MOD_TCPD_PROTOCOL_N_SIZE 1
#define _MOD_TCPD_PROTOCOL_R_STRING "\r"
#define _MOD_TCPD_PROTOCOL_R_SIZE 1

typedef struct _tcpd_consts_s
{
  int error_timeout;
}
_tcpd_consts_t;

typedef struct _tcpd_data_s
{
  _tcpd_consts_t consts;
}
_tcpd_data_t;

typedef struct _tcpd_context_s
{
  _tcpd_data_t data;
}
_tcpd_context_t;

typedef struct _tcpd_specific_data_s
{
  int dummy;
}
_tcpd_specific_data_t;

/* mod-tcpd-data.c */
extern int _mod_tcpd_load_data (_tcpd_data_t * tcpd_data, char *data_dir);
extern void _mod_tcpd_unload_data (_tcpd_data_t * tcpd_data);

/*
 * In setup.c
 */
extern _tcpd_context_t *_mod_tcpd_init (int argc, char *argv[],
					lw6srv_listener_t * listener);
extern void _mod_tcpd_quit (_tcpd_context_t * tcpd_context);

/*
 * In handshake.c
 */
extern int _mod_tcpd_analyse_tcp (_tcpd_context_t * tcpd_context,
				  lw6srv_tcp_accepter_t * tcp_accepter,
				  u_int64_t * remote_id, char **remote_url);
extern int _mod_tcpd_analyse_udp (_tcpd_context_t * tcpd_context,
				  lw6srv_udp_buffer_t * udp_buffer,
				  u_int64_t * remote_id, char **remote_url);
extern int _mod_tcpd_feed_with_tcp (_tcpd_context_t * tcpd_context,
				    lw6cnx_connection_t * connection,
				    lw6srv_tcp_accepter_t * tcp_accepter);
extern int _mod_tcpd_feed_with_udp (_tcpd_context_t * tcpd_context,
				    lw6cnx_connection_t * connection,
				    lw6srv_udp_buffer_t * udp_buffer);

/*
 * In state.c
 */
extern lw6cnx_connection_t *_mod_tcpd_open (_tcpd_context_t * tcpd_context,
					    lw6srv_listener_t * listener,
					    char *local_url, char *remote_url,
					    char *remote_ip, int remote_port,
					    char *password,
					    u_int64_t local_id,
					    u_int64_t remote_id,
					    lw6cnx_recv_callback_t
					    recv_callback_func,
					    void *recv_callback_data);
extern void _mod_tcpd_close (_tcpd_context_t * tcpd_context,
			     lw6cnx_connection_t * connection);
extern int _mod_tcpd_is_alive (_tcpd_context_t * tcpd_context,
			       lw6cnx_connection_t * connection);
extern int _mod_tcpd_timeout_ok (_tcpd_context_t * tcpd_context,
				 int64_t origin_timestamp);

/*
 * In message.c
 */
extern int _mod_tcpd_send (_tcpd_context_t * tcpd_context,
			   lw6cnx_connection_t * connection,
			   u_int32_t ticket_sig, u_int64_t logical_from_id,
			   u_int64_t logical_to_id, char *message);
extern void _mod_tcpd_poll (_tcpd_context_t * tcpd_context,
			    lw6cnx_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_tcpd_repr (_tcpd_context_t * tcpd_context,
			     lw6cnx_connection_t * connection);
extern char *_mod_tcpd_error (_tcpd_context_t * tcpd_context,
			      lw6cnx_connection_t * connection);

/* mod-tcpd-oob.c */
extern int _mod_tcpd_process_oob (_tcpd_context_t * tcpd_context,
				  lw6nod_info_t * node_info,
				  lw6srv_oob_data_t * oob_data);
extern int _mod_tcpd_oob_should_continue (_tcpd_context_t * tcpd_context,
					  lw6srv_oob_data_t * oob_data);

#endif
