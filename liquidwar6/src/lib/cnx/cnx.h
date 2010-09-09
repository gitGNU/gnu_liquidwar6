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

#ifndef LIQUIDWAR6CNX_H
#define LIQUIDWAR6CNX_H

#include "../sys/sys.h"
#include "../glb/glb.h"

typedef void (*lw6cnx_recv_callback_t) (void *recv_callback_data,
					void *connection,
					u_int32_t physical_ticket_sig,
					u_int32_t logical_ticket_sig,
					u_int64_t logical_from_id,
					u_int64_t logical_to_id,
					char *message);

typedef struct lw6cnx_connection_s
{
  char *local_url;
  char *remote_url;
  char *remote_ip;
  int remote_port;
  char *password;
  char *password_send_checksum;
  int64_t local_id_int;
  char *local_id_str;
  int64_t remote_id_int;
  char *remote_id_str;
  int dns_ok;
  int network_reliability;
  lw6cnx_recv_callback_t recv_callback_func;
  void *recv_callback_data;
  void *send_mutex;
  u_int32_t foo_bar_key;
  int64_t last_send_foo_timestamp;
  int64_t next_send_foo_timestamp;
  int ping_msec;
  void *backend_specific_data;
}
lw6cnx_connection_t;

typedef struct lw6cnx_ticket_table_s
{
  void *recv_spinlock;
  void *recv_ack_spinlock;
  void *send_spinlock;
  lw6sys_hash_t *recv_table;
  lw6sys_hash_t *recv_ack_table;
  lw6sys_hash_t *send_table;
}
lw6cnx_ticket_table_t;

/* cnx-connection.c */
extern lw6cnx_connection_t *lw6cnx_connection_new (char *local_url,
						   char *remote_url,
						   char *remote_ip,
						   int remote_port,
						   char *password,
						   u_int64_t local_id,
						   u_int64_t remote_id,
						   int dns_ok,
						   int network_reliability,
						   lw6cnx_recv_callback_t
						   recv_callback_func,
						   void *recv_callback_data);
extern void lw6cnx_connection_free (lw6cnx_connection_t * connection);
extern int lw6cnx_connection_should_send_foo (lw6cnx_connection_t *
					      connection, int64_t now);
extern void lw6cnx_connection_init_foo_bar_key (lw6cnx_connection_t *
						connection, int64_t now,
						int next_foo_delay);
extern int lw6cnx_connection_lock_send (lw6cnx_connection_t * connection);
extern void lw6cnx_connection_unlock_send (lw6cnx_connection_t * connection);
extern int lw6cnx_connection_reliability_filter (lw6cnx_connection_t *
						 connection);

/* cnx-password.c */
extern char *lw6cnx_password_checksum (char *seed, char *password);
extern int lw6cnx_password_verify (char *seed, char *password_here,
				   char *password_received);

/* cnx-test.c */
extern int lw6cnx_test (int mode);

/* cnx-tickettable.c */
extern void lw6cnx_ticket_table_zero (lw6cnx_ticket_table_t * ticket_table);
extern int lw6cnx_ticket_table_init (lw6cnx_ticket_table_t * ticket_table,
				     int hash_size);
extern void lw6cnx_ticket_table_clear (lw6cnx_ticket_table_t * ticket_table);
extern u_int64_t lw6cnx_ticket_table_get_recv (lw6cnx_ticket_table_t *
					       ticket_table, char *peer_id);
extern void lw6cnx_ticket_table_ack_recv (lw6cnx_ticket_table_t *
					  ticket_table, char *peer_id);
extern int lw6cnx_ticket_table_was_recv_exchanged (lw6cnx_ticket_table_t *
						   ticket_table,
						   char *peer_id);
extern u_int64_t lw6cnx_ticket_table_get_send (lw6cnx_ticket_table_t *
					       ticket_table, char *peer_id);
extern void lw6cnx_ticket_table_set_send (lw6cnx_ticket_table_t *
					  ticket_table, char *peer_id,
					  u_int64_t send_ticket);

#endif
