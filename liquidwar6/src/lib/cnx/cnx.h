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

#ifndef LIQUIDWAR6CNX_H
#define LIQUIDWAR6CNX_H

#include "../sys/sys.h"
#include "../glb/glb.h"

#define LW6CNX_WORST_PING_MSEC 3600000

typedef void (*lw6cnx_recv_callback_t) (void *recv_callback_data,
					void *connection,
					u_int32_t physical_ticket_sig,
					u_int32_t logical_ticket_sig,
					u_int64_t logical_from_id,
					u_int64_t logical_to_id,
					char *message);

/*
typedef struct lw6cnx_send_param_s
{
  u_int32_t physical_ticket_sig;
  u_int32_t logical_ticket_sig;
  u_int64_t logical_from_id;
  u_int64_t logical_to_id;
  char *message;
} lw6cnx_send_param_t;
*/

/**
 * This structure holds all data associated to a physical connexion
 * with a remote peer. This includes informations about the local node,
 * informations about the peer, and protocol specific details. Depending
 * on which backend is used to handle the connection, it will behave
 * differently. If you search for attributes such as socket id, search
 * for them in backend_specific_data which is, in fact, handle by the
 * backend code.
 */
typedef struct lw6cnx_connection_s
{
  /// URL of our local node.
  char *local_url;
  /// URL of the remote node.
  char *remote_url;
  /// IP address of the remote node.
  char *remote_ip;
  /// IP port of the remote node.
  int remote_port;
  /// Password as clear text.
  char *password;
  /// Password as a checksum, what will be sent on the network.
  char *password_send_checksum;
  /// ID of the local node, as an unsigned 64-bit integer.
  int64_t local_id_int;
  /// ID of the local node, as an hexa string.
  char *local_id_str;
  /// ID of the remote node, as an unsigned 64-bit integer.
  int64_t remote_id_int;
  /// ID of the local node, as an hexa string.
  char *remote_id_str;
  /**
   * Will be set to 1 if the peer domain name is the same
   * as the one reported in the URL. For instance, if we
   * get a connection from 23.45.23.45, but this host claims
   * to be on www.foo.bar and DNS reports www.foo.bar as being
   * 111.222.111.222 then there's something strange. It could
   * just be someone doing NAT, but in all cases it's worth
   * mentionning, so we keep the information here. Having 0
   * here is a bad point for the connection.
   */
  int dns_ok;
  /**
   * The higher, the most reliable message sending will be.
   * It can never be perfect, LW6 will always drop some packets
   * from time to time, just to simulate real packet loss and
   * be sure if it happens, it's handled nicely.
   */
  int network_reliability;
  /**
   * Function which will be called when receiving data. This one
   * must clearly be reentrant, depending on the backend it could
   * be called in a separate thread, at any time.
   */
  lw6cnx_recv_callback_t recv_callback_func;
  /// Data passed to recv_callback_func.
  void *recv_callback_data;
  /** 
   * Send mutex, this will be used so that sending operations
   * are properly serialized. Indeed, threads that respond on the
   * fly could be likely to call this concurrently.
   */
  void *send_mutex;
  /**
   * This is used to handle keepalive. Actually, the protocol
   * is that from time to time FOO key is sent and then each
   * connection (in a tentacle object, typically) is supposed
   * to respond BAR key to show it has received the latest message.
   * This field just stores the value so that when we receive
   * a BAR message we know which key to check against.
   */
  u_int32_t foo_bar_key;
  /// The last time FOO was sent.
  int64_t last_send_foo_timestamp;
  /// The next time FOO needs to be sent.
  int64_t next_send_foo_timestamp;
  /// The current ping, updated when receiving BAR message.
  int ping_msec;
  /// Last time something was received on this connection.
  int64_t last_recv_timestamp;
  /**
   * Store backend data, this is when, for instance, a socket handle
   * will be kept, or a library handle (CURL, to name it). Common
   * code does not know what's in there.
   */
  void *backend_specific_data;
}
lw6cnx_connection_t;

/**
 * A common, shared table, to store all the tickets associated with
 * various connections. This needs to be in-memory and quite fast
 * for it's called very often (at each message, in fact) to perform
 * sanity checks and avoid fakes/cheaters.
 */
typedef struct lw6cnx_ticket_table_s
{
  /// Lock for the recv_table hash.
  void *recv_spinlock;
  /// Lock for the recv_ack_table hash.
  void *recv_ack_spinlock;
  /// Lock for the send_table hash.
  void *send_spinlock;
  /**
   * Hash table containing the tickets for recv operations.
   * This table is auto-generated, if one asks for a ticket
   * for an unknown host, one is generated.
   * The key is the ID (64-bit integer) of the host, as an hexa string.
   */
  lw6sys_hash_t *recv_table;
  /**
   * Hash table containing wether the send ticket was received
   * by a given host. The data is just a NULL pointer, only if the
   * key is present, we know we don't need to resend our key to the peer.
   * An easy way to know that the key was sent is if the peer was abled
   * to produce a valid message/checksum.
   * The key is the ID (64-bit integer) of the host, as an hexa string.
   */
  lw6sys_hash_t *recv_ack_table;
  /**
   * Hash table containing the tickets for send operations.
   * Those tickets are typically received from the peers themselves
   * who generate them on the fly.
   * The key is the ID (64-bit integer) of the host, as an hexa string.
   */
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
