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

#ifndef LIQUIDWAR6SRV_TCPD_INTERNAL_H
#define LIQUIDWAR6SRV_TCPD_INTERNAL_H

#include "../srv.h"
#include "mod-tcpd.h"

#define _MOD_TCPD_PROTOCOL_LW6_STRING "LW6"
#define _MOD_TCPD_PROTOCOL_LW6_SIZE 3
#define _MOD_TCPD_PROTOCOL_N_STRING "\n"
#define _MOD_TCPD_PROTOCOL_N_SIZE 1
#define _MOD_TCPD_PROTOCOL_R_STRING "\r"
#define _MOD_TCPD_PROTOCOL_R_SIZE 1

typedef struct _mod_tcpd_consts_s
{
  int error_timeout;
  int ping_alter_base;
  int ping_alter_percent;
}
_mod_tcpd_consts_t;

typedef struct _mod_tcpd_data_s
{
  _mod_tcpd_consts_t consts;
}
_mod_tcpd_data_t;

typedef struct _mod_tcpd_context_s
{
  _mod_tcpd_data_t data;
}
_mod_tcpd_context_t;

typedef struct _mod_tcpd_specific_data_s
{
  int sock;
  lw6cnx_backlog_t backlog;
  int64_t last_send_failed_timestamp;
}
_mod_tcpd_specific_data_t;

/* mod-tcpd-data.c */
extern int _mod_tcpd_load_data (_mod_tcpd_data_t * tcpd_data,
				const char *data_dir);
extern void _mod_tcpd_unload_data (_mod_tcpd_data_t * tcpd_data);

/* mod-tcpd-setup.c */
extern _mod_tcpd_context_t *_mod_tcpd_init (int argc, const char *argv[],
					    lw6cnx_properties_t * properties,
					    lw6srv_listener_t * listener);
extern void _mod_tcpd_quit (_mod_tcpd_context_t * tcpd_context);

/* mod-tcpd-handshake.c */
extern int _mod_tcpd_analyse_tcp (_mod_tcpd_context_t * tcpd_context,
				  lw6srv_tcp_accepter_t * tcp_accepter,
				  lw6nod_info_t * node_info,
				  u_int64_t * remote_id, char **remote_url);
extern int _mod_tcpd_analyse_udp (_mod_tcpd_context_t * tcpd_context,
				  lw6srv_udp_buffer_t * udp_buffer,
				  lw6nod_info_t * node_info,
				  u_int64_t * remote_id, char **remote_url);
extern int _mod_tcpd_feed_with_tcp (_mod_tcpd_context_t * tcpd_context,
				    lw6cnx_connection_t * connection,
				    lw6srv_tcp_accepter_t * tcp_accepter);
extern int _mod_tcpd_feed_with_udp (_mod_tcpd_context_t * tcpd_context,
				    lw6cnx_connection_t * connection,
				    lw6srv_udp_buffer_t * udp_buffer);

/* mod-tcpd-state.c */
extern lw6cnx_connection_t *_mod_tcpd_open (_mod_tcpd_context_t *
					    tcpd_context,
					    lw6srv_listener_t * listener,
					    const char *local_url,
					    const char *remote_url,
					    const char *remote_ip,
					    int remote_port,
					    const char *password,
					    u_int64_t local_id,
					    u_int64_t remote_id, int dns_ok,
					    int network_reliability,
					    lw6cnx_recv_callback_t
					    recv_callback_func,
					    void *recv_callback_data);
extern void _mod_tcpd_close (_mod_tcpd_context_t * tcpd_context,
			     lw6cnx_connection_t * connection);
extern int _mod_tcpd_timeout_ok (_mod_tcpd_context_t * tcpd_context,
				 int64_t origin_timestamp);

/* mod-tcpd-message.c */
extern int _mod_tcpd_send (_mod_tcpd_context_t * tcpd_context,
			   lw6cnx_connection_t * connection,
			   int64_t now,
			   u_int32_t physical_ticket_sig,
			   u_int32_t logical_ticket_sig,
			   u_int64_t logical_from_id, u_int64_t logical_to_id,
			   const char *message);
extern void _mod_tcpd_poll (_mod_tcpd_context_t * tcpd_context,
			    lw6cnx_connection_t * connection);

/* mod-tcpd-info.c */
extern char *_mod_tcpd_repr (_mod_tcpd_context_t * tcpd_context,
			     lw6cnx_connection_t * connection);

/* mod-tcpd-oob.c */
extern int _mod_tcpd_process_oob (_mod_tcpd_context_t * tcpd_context,
				  lw6nod_info_t * node_info,
				  lw6srv_oob_data_t * oob_data);
extern int _mod_tcpd_oob_should_continue (_mod_tcpd_context_t * tcpd_context,
					  lw6srv_oob_data_t * oob_data);

#endif
