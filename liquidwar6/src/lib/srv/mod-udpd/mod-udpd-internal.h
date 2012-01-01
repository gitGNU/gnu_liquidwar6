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

#ifndef LIQUIDWAR6SRV_UDPD_INTERNAL_H
#define LIQUIDWAR6SRV_UDPD_INTERNAL_H

#include "../srv.h"

typedef struct _udpd_consts_s
{
  int error_timeout;
}
_udpd_consts_t;

typedef struct _udpd_data_s
{
  _udpd_consts_t consts;
}
_udpd_data_t;

typedef struct _udpd_context_s
{
  _udpd_data_t data;
}
_udpd_context_t;

typedef struct _udpd_specific_data_s
{
  int sock;
  int remote_port;
}
_udpd_specific_data_t;

/* mod-udpd-data.c */
extern int _mod_udpd_load_data (_udpd_data_t * udpd_data,
				const char *data_dir);
extern void _mod_udpd_unload_data (_udpd_data_t * udpd_data);

/*
 * In setup.c
 */
extern _udpd_context_t *_mod_udpd_init (int argc, const char *argv[],
					lw6srv_listener_t * listener);
extern void _mod_udpd_quit (_udpd_context_t * udpd_context);

/*
 * In handshake.c
 */
extern int _mod_udpd_analyse_tcp (_udpd_context_t * udpd_context,
				  lw6srv_tcp_accepter_t * tcp_accepter,
				  lw6nod_info_t * node_info,
				  u_int64_t * remote_id, char **public_url);
extern int _mod_udpd_analyse_udp (_udpd_context_t * udpd_context,
				  lw6srv_udp_buffer_t * udp_buffer,
				  lw6nod_info_t * node_info,
				  u_int64_t * remote_id, char **public_url);
extern int _mod_udpd_feed_with_tcp (_udpd_context_t * udpd_context,
				    lw6cnx_connection_t * connection,
				    lw6srv_tcp_accepter_t * tcp_accepter);
extern int _mod_udpd_feed_with_udp (_udpd_context_t * udpd_context,
				    lw6cnx_connection_t * connection,
				    lw6srv_udp_buffer_t * udp_buffer);

/*
 * In state.c
 */
extern lw6cnx_connection_t *_mod_udpd_open (_udpd_context_t * udpd_context,
					    lw6srv_listener_t * listener,
					    char *local_url, char *remote_url,
					    char *remote_ip, int remote_port,
					    char *password,
					    u_int64_t local_id,
					    u_int64_t remote_id,
					    int dns_ok,
					    int network_reliability,
					    lw6cnx_recv_callback_t
					    recv_callback_func,
					    void *recv_callback_data);
extern void _mod_udpd_close (_udpd_context_t * udpd_context,
			     lw6cnx_connection_t * connection);
extern int _mod_udpd_timeout_ok (_udpd_context_t * udpd_context,
				 int64_t origin_timestamp);

/*
 * In message.c
 */
extern int _mod_udpd_send (_udpd_context_t * udpd_context,
			   lw6cnx_connection_t * connection,
			   u_int32_t physical_ticket_sig,
			   u_int32_t logical_ticket_sig,
			   u_int64_t logical_from_id, u_int64_t logical_to_id,
			   char *message);
extern void _mod_udpd_poll (_udpd_context_t * udpd_context,
			    lw6cnx_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_udpd_repr (_udpd_context_t * udpd_context,
			     lw6cnx_connection_t * connection);

/* mod-udpd-oob.c */
extern int _mod_udpd_process_oob (_udpd_context_t * udpd_context,
				  lw6nod_info_t * node_info,
				  lw6srv_oob_data_t * oob_data);
extern int _mod_udpd_oob_should_continue (_udpd_context_t * udpd_context,
					  lw6srv_oob_data_t * oob_data);

#endif
