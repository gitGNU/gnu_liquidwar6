/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6SRV_H
#define LIQUIDWAR6SRV_H

#include "../sys/sys.h"
#include "../glb/glb.h"
#include "../dyn/dyn.h"
#include "../net/net.h"
#include "../nod/nod.h"
#include "../cnx/cnx.h"
#include "../msg/msg.h"

#define LW6SRV_PROTOCOL_BUFFER_SIZE LW6NET_DIALUP_MTU
#define LW6SRV_CONTENT_BUFFER_SIZE LW6NET_PPPOE_MTU

#define LW6SRV_ANALYSE_DEAD 0x01
#define LW6SRV_ANALYSE_UNDERSTANDABLE 0x02
#define LW6SRV_ANALYSE_OOB 0x04

/**
 * Parsed client ID, this is not the numerical 64-bit ID
 * but an IP:port pair which uniquely and physically
 * identifies the peer.
 */
typedef struct lw6srv_client_id_s
{
  /// Client IP address, as a string.
  char *client_ip;
  /// Client IP port.
  int client_port;
}
lw6srv_client_id_t;

/**
 * A TCP accepter is an object which is created after a listening
 * socket received some random information
 * (in TCP mode, hence its name). Itprovides
 * basic support to accept/reject requests and choose the
 * right protocol/backend for the answer.
 */
typedef struct lw6srv_tcp_accepter_s
{
  /// Where the data does come from.
  lw6srv_client_id_t client_id;
  /**
   * Socket returned by the accept POSIX function, this is the
   * one we can use to reply and send data back.
   */
  int sock;
  /**
   * First line received over the network. This is "peeked"
   * so it's still available for the actual backend, in fact
   * that's the very thing we need an object for, with the
   * information "these bytes came from ip:port" one can
   * take a decision on what to do with the request.
   */
  char first_line[LW6SRV_PROTOCOL_BUFFER_SIZE + 1];
  /**
   * Timestamp of accepter creation. This is more or less
   * the same that the instant we received data on the network.
   * There's a small lag, but not that bad. This is mostly
   * used for timeout.
   */
  int64_t creation_timestamp;
} lw6srv_tcp_accepter_t;

/**
 * A UDP datagram, this structure contains both the data
 * and information about who sent it.
 */
typedef struct lw6srv_udp_buffer_s
{
  /// Where the data does come from.
  lw6srv_client_id_t client_id;
  /**
   * The data itself. This is typically a C-string with a 0 char at
   * the end, anything else will be rejected anyway.
   */
  char *line;
} lw6srv_udp_buffer_t;

/**
 * The listener is the object which listens on the network
 * and can create tcp accepters or udp buffers depending on
 * what is received.
 */
typedef struct lw6srv_listener_s
{
  /// IP address we are binded to.
  char *ip;
  /// IP port we are binded to.
  int port;
  /// TCP socket, binded in listening mode.
  int tcp_sock;
  /**
   * List of lw6srv_tcp_accepter_t objects, created
   * when data is received.
   */
  lw6sys_list_t *tcp_accepters;
  /// UDP socket, binded.
  int udp_sock;
  /**
   * List of lw6srv_udp_buffer_t objects, created
   * when data is received.
   */
  lw6sys_list_t *udp_buffers;
} lw6srv_listener_t;

/**
 * Used to store out of band data. Typically,
 * when data is recognized as out of band, it's treated
 * in a separate thread, and not mainstream. This is both
 * because out-of-band data is the default (anything not
 * recognized and/or not trusted is OOB) and because
 * this can easily be treated separately as all we need is
 * to server nearly static informations.
 */
typedef struct lw6srv_oob_data_s
{
  /// Date of the request.
  int64_t creation_timestamp;
  /// Used to interrupt the OOB process before it's over.
  volatile int do_not_finish;
  /// IP address of peer.
  char *remote_ip;
  /// IP port of peer.
  int remote_port;
  /// Socket used, can either be TCP or UDP, depends on backend.
  int sock;
  /// First line of data.
  char *first_line;
}
lw6srv_oob_data_t;

/**
 * Used to handle OOB requests. This is a container
 * over the OOB data and its treatment thread.
 */
typedef struct lw6srv_oob_s
{
  /// Thread use to handle the data.
  lw6sys_thread_handler_t *thread;
  /// The OOB data, what we received from the network.
  lw6srv_oob_data_t data;
}
lw6srv_oob_t;

/**
 * The srv backend is the first argument passed to any srv function,
 * it contains reference to all the functions which can be used
 * as well as a pointer on associated data. In OO, this would just
 * be an object, with members and methods, using polymorphism through
 * opaque pointers.
 */
typedef struct lw6srv_backend_s
{
  /// Handle on dynamic library (if it makes sense).
  lw6dyn_dl_handle_t *dl_handle;
  /**
   * Srv specific data, what is behind this pointer really
   * depends on the srv engine.
   */
  void *srv_context;
  /// The argc value passed to main.
  int argc;
  /// The argv value passed to main.
  const char **argv;
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /// Module name.
  char *name;
  /// General backend properties.
  lw6cnx_properties_t properties;
  /// Information about local node.
  lw6nod_info_t *info;

  /// Pointer on lw6srv_init callback code.
  void *(*init) (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6cnx_properties_t * properties, lw6srv_listener_t * listener);
  /// Pointer on lw6srv_quit callback code.
  void (*quit) (lw6sys_context_t * sys_context, void *srv_context);
  /// Pointer on lw6srv_analyse_tcp callback code.
  int (*analyse_tcp) (lw6sys_context_t * sys_context, void *srv_context, lw6srv_tcp_accepter_t * tcp_accepter, lw6nod_info_t * node_info,
		      u_int64_t * remote_id, char **remote_url);
  /// Pointer on lw6srv_analyse_udp callback code.
  int (*analyse_udp) (lw6sys_context_t * sys_context, void *srv_context, lw6srv_udp_buffer_t * udp_buffer, lw6nod_info_t * node_info, u_int64_t * remote_id,
		      char **remote_url);
  /// Pointer on lw6srv_process_oob callback code.
  int (*process_oob) (lw6sys_context_t * sys_context, void *srv_context, lw6nod_info_t * node_info, lw6srv_oob_data_t * oob_data);
  /// Pointer on lw6srv_open callback code.
  lw6cnx_connection_t *(*open) (lw6sys_context_t * sys_context, void *srv_context,
				lw6srv_listener_t * listener,
				const char *local_url, const char *remote_url,
				const char *remote_ip, int remote_port,
				const char *password, u_int64_t local_id, u_int64_t remote_id, int dns_ok, int network_reliability);
  /// Pointer on lw6srv_feed_with_tcp callback code.
  int (*feed_with_tcp) (lw6sys_context_t * sys_context, void *srv_context, lw6cnx_connection_t * connection, lw6srv_tcp_accepter_t * tcp_accepter);
  /// Pointer on lw6srv_feed_with_udp callback code.
  int (*feed_with_udp) (lw6sys_context_t * sys_context, void *srv_context, lw6cnx_connection_t * connection, lw6srv_udp_buffer_t * udp_buffer);
  /// Pointer on lw6srv_close callback code.
  void (*close) (lw6sys_context_t * sys_context, void *srv_context, lw6cnx_connection_t * connection);
  /// Pointer on lw6srv_send callback code.
  int (*send) (lw6sys_context_t * sys_context, void *srv_context, lw6cnx_connection_t * connection,
	       int64_t now,
	       u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message);
  /// Pointer on lw6srv_can_send callback code.
  int (*can_send) (lw6sys_context_t * sys_context, void *srv_context, lw6cnx_connection_t * connection);
  /// Pointer on lw6srv_poll callback code.
  void (*poll) (lw6sys_context_t * sys_context, void *srv_context, lw6cnx_connection_t * connection);
  /// Pointer on lw6srv_repr callback code.
  char *(*repr) (lw6sys_context_t * sys_context, void *srv_context, lw6cnx_connection_t * connection);
}
lw6srv_backend_t;

/* srv-api.c */
extern int lw6srv_init (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6srv_listener_t * listener);
extern void lw6srv_quit (lw6sys_context_t * sys_context, lw6srv_backend_t * backend);
extern int lw6srv_analyse_tcp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend,
			       lw6srv_tcp_accepter_t * tcp_accepter, lw6nod_info_t * node_info, u_int64_t * remote_id, char **remote_url);
extern int lw6srv_analyse_udp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend,
			       lw6srv_udp_buffer_t * udp_buffer, lw6nod_info_t * node_info, u_int64_t * remote_id, char **remote_url);
extern int lw6srv_process_oob (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6nod_info_t * node_info, lw6srv_oob_data_t * oob_data);
extern lw6cnx_connection_t *lw6srv_open (lw6sys_context_t * sys_context, lw6srv_backend_t * backend,
					 lw6srv_listener_t * listener,
					 const char *local_url,
					 const char *remote_url,
					 const char *remote_ip,
					 int remote_port, const char *password, u_int64_t local_id, u_int64_t remote_id, int dns_ok, int network_reliability);
extern int lw6srv_feed_with_tcp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection,
				 lw6srv_tcp_accepter_t * tcp_accepter);
extern int lw6srv_feed_with_udp (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection,
				 lw6srv_udp_buffer_t * udp_buffer);
extern void lw6srv_close (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection);
extern int lw6srv_send (lw6sys_context_t * sys_context, lw6srv_backend_t * backend,
			lw6cnx_connection_t * connection,
			int64_t now,
			u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message);
extern int lw6srv_can_send (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection);
extern void lw6srv_poll (lw6sys_context_t * sys_context, lw6srv_backend_t * backend, lw6cnx_connection_t * connection);
extern char *lw6srv_repr (lw6sys_context_t * sys_context, const lw6srv_backend_t * backend, lw6cnx_connection_t * connection);

/* srv-control.c */
extern lw6srv_listener_t *lw6srv_start (lw6sys_context_t * sys_context, const char *ip, int port);
extern void lw6srv_stop (lw6sys_context_t * sys_context, lw6srv_listener_t * listener);

/* srv-oob.c */
extern lw6srv_oob_t *lw6srv_oob_new (lw6sys_context_t * sys_context, const char *remote_ip, int remote_port, int sock, const char *first_line);
extern void lw6srv_oob_free (lw6sys_context_t * sys_context, lw6srv_oob_t * oob);

/* srv-register.c */
extern char *lw6srv_default_backends (lw6sys_context_t * sys_context);
extern lw6sys_assoc_t *lw6srv_get_backends (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern lw6srv_backend_t *lw6srv_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name);
extern void lw6srv_destroy_backend (lw6sys_context_t * sys_context, lw6srv_backend_t * backend);

/* srv-tcpaccepter.c */
extern lw6srv_tcp_accepter_t *lw6srv_tcp_accepter_new (lw6sys_context_t * sys_context, char *client_ip, int client_port, int sock);
extern void lw6srv_tcp_accepter_free (lw6sys_context_t * sys_context, lw6srv_tcp_accepter_t * tcp_accepter);

/* srv-test.c */
extern int lw6srv_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6srv_test_run (lw6sys_context_t * sys_context, int mode);

/* srv-udpbuffer.c */
extern lw6srv_udp_buffer_t *lw6srv_udp_buffer_new (lw6sys_context_t * sys_context, char *client_ip, int client_port, char *line);
extern void lw6srv_udp_buffer_free (lw6sys_context_t * sys_context, lw6srv_udp_buffer_t * udp_buffer);

#endif
