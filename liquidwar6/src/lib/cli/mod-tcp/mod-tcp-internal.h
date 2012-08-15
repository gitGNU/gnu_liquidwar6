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

#ifndef LIQUIDWAR6CLI_TCP_INTERNAL_H
#define LIQUIDWAR6CLI_TCP_INTERNAL_H

#include "../cli.h"

typedef struct _mod_tcp_consts_s
{
  int global_timeout;
  int connect_timeout;
  int reconnect_delay;
  int ping_alter_base;
  int ping_alter_percent;
}
_mod_tcp_consts_t;

typedef struct _mod_tcp_data_s
{
  _mod_tcp_consts_t consts;
}
_mod_tcp_data_t;

typedef struct _mod_tcp_context_s
{
  _mod_tcp_data_t data;
}
_mod_tcp_context_t;

typedef enum _mod_tcp_state_e
{
  _MOD_TCP_STATE_CLOSED = 0,
  _MOD_TCP_STATE_CONNECTING = 1,
  _MOD_TCP_STATE_CONNECT_DONE = 2,
  _MOD_TCP_STATE_CONNECTED = 3
}
_mod_tcp_state_t;

typedef struct _mod_tcp_specific_data_s
{
  int sock;
  _mod_tcp_state_t state;
  int send_failed_once;
  int send_succeeded_once;
  lw6sys_list_t *send_backlog;
  void *connect_thread;
}
_mod_tcp_specific_data_t;

typedef struct _mod_tcp_connect_data_s
{
  _mod_tcp_context_t *tcp_context;
  lw6cnx_connection_t *connection;
}
_mod_tcp_connect_data_t;

/* mod-tcp-data.c */
extern int _mod_tcp_load_data (_mod_tcp_data_t * tcp_data,
			       const char *data_dir);
extern void _mod_tcp_unload_data (_mod_tcp_data_t * tcp_data);

/* mod-tcp-setup.c */
extern _mod_tcp_context_t *_mod_tcp_init (int argc, const char *argv[],
					  lw6cnx_properties_t * properties);
extern void _mod_tcp_quit (_mod_tcp_context_t * tcp_context);

/* mod-tcp-state.c */
extern lw6cnx_connection_t *_mod_tcp_open (_mod_tcp_context_t * tcp_context,
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
extern void _mod_tcp_close (_mod_tcp_context_t * tcp_context,
			    lw6cnx_connection_t * connection);
extern int _mod_tcp_timeout_ok (_mod_tcp_context_t * tcp_context,
				int64_t origin_timestamp);
extern void _mod_tcp_connect_func (void *func_data);

/* mod-tcp-message.c */
extern int _mod_tcp_send (_mod_tcp_context_t * tcp_context,
			  lw6cnx_connection_t * connection,
			  u_int32_t physical_ticket_sig,
			  u_int32_t logical_ticket_sig,
			  u_int64_t logical_from_id, u_int64_t logical_to_id,
			  const char *message);
extern void _mod_tcp_poll (_mod_tcp_context_t * tcp_context,
			   lw6cnx_connection_t * connection);

/* mod-tcp-info.c */
extern char *_mod_tcp_repr (_mod_tcp_context_t * tcp_context,
			    lw6cnx_connection_t * connection);

/* mod-tcp-oob.c */
extern int _mod_tcp_process_oob (_mod_tcp_context_t * tcp_context,
				 lw6nod_info_t * node_info,
				 lw6cli_oob_data_t * oob_data);
extern int _mod_tcp_oob_should_continue (_mod_tcp_context_t * tcp_context,
					 lw6cli_oob_data_t * oob_data,
					 int sock);

#endif
