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

#ifndef LIQUIDWAR6SRV_H
#define LIQUIDWAR6SRV_H

#include "../sys/sys.h"
#include "../dyn/dyn.h"
#include "../net/net.h"

#define LW6SRV_PROTOCOL_BUFFER_SIZE 32

typedef struct lw6srv_client_id_s
{
  char *client_ip;
  int client_port;
}
lw6srv_client_id_t;

typedef struct lw6srv_tcp_accepter_s
{
  lw6srv_client_id_t client_id;
  int sock;
  char first_line[LW6SRV_PROTOCOL_BUFFER_SIZE + 1];
} lw6srv_tcp_accepter_t;

typedef struct lw6srv_udp_buffer_s
{
  lw6srv_client_id_t client_id;
  char *line;
} lw6srv_udp_buffer_t;

typedef struct lw6srv_listener_s
{
  char *ip;
  int port;
  int tcp_sock;
  lw6sys_list_t *tcp_accepters;	// lw6srv_tcp_accepter_t
  int udp_sock;
  lw6sys_list_t *udp_buffers;	// lw6srv_udp_buffer_t
} lw6srv_listener_t;

typedef struct lw6srv_connection_s
{
  lw6srv_client_id_t client_id;
  char *server_url;
  void *connection_data;
}
lw6srv_connection_t;

typedef struct lw6srv_backend_s
{
  lw6dyn_dl_handle_t *dl_handle;
  void *srv_context;
  int argc;
  char **argv;
  u_int32_t id;

  void *(*init) (int argc, char *argv[], lw6srv_listener_t * listener);
  void (*quit) (void *srv_context);
  int (*can_handle_tcp) (void *srv_context,
			 lw6srv_tcp_accepter_t * tcp_accepter);
  int (*can_handle_udp) (void *srv_context, lw6srv_udp_buffer_t * udp_buffer);
  lw6srv_connection_t *(*accept_tcp) (void *srv_context,
				      lw6srv_tcp_accepter_t * tcp_accepter,
				      char *password);
  lw6srv_connection_t *(*new_udp) (void *srv_context,
				   lw6srv_udp_buffer_t * udp_buffer,
				   char *password);
  int (*is_associated_with_udp) (void *srv_context,
				 lw6srv_connection_t * connection,
				 lw6srv_udp_buffer_t * udp_buffer);
  int (*update_with_udp) (void *srv_context,
			  lw6srv_connection_t * connection,
			  lw6srv_udp_buffer_t * udp_buffer);
  void (*close) (void *srv_context, lw6srv_connection_t * connection);
  int (*send) (void *srv_context, lw6srv_connection_t * connection,
	       char *message);
  char *(*recv) (void *srv_context, lw6srv_connection_t * connection);
  int (*is_alive) (void *srv_context, lw6srv_connection_t * connection);
  char *(*repr) (void *srv_context, lw6srv_connection_t * connection);
  char *(*error) (void *srv_context, lw6srv_connection_t * connection);
}
lw6srv_backend_t;

/*
 * In api.c
 */
extern int lw6srv_init (lw6srv_backend_t * backend,
			lw6srv_listener_t * listener);
extern void lw6srv_quit (lw6srv_backend_t * backend);
extern int lw6srv_can_handle_tcp (lw6srv_backend_t * backend,
				  lw6srv_tcp_accepter_t * tcp_accepter);
extern int lw6srv_can_handle_udp (lw6srv_backend_t * backend,
				  lw6srv_udp_buffer_t * udp_buffer);
extern lw6srv_connection_t *lw6srv_accept_tcp (lw6srv_backend_t * backend,
					       lw6srv_tcp_accepter_t *
					       tcp_accepter, char *password);
extern lw6srv_connection_t *lw6srv_new_udp (lw6srv_backend_t * backend,
					    lw6srv_udp_buffer_t * udp_buffer,
					    char *password);
extern int lw6srv_is_associated_with_udp (lw6srv_backend_t * backend,
					  lw6srv_connection_t *
					  connection,
					  lw6srv_udp_buffer_t * udp_buffer);
extern int lw6srv_update_with_udp (lw6srv_backend_t * backend,
				   lw6srv_connection_t * connection,
				   lw6srv_udp_buffer_t * udp_buffer);
extern void lw6srv_close (lw6srv_backend_t * backend,
			  lw6srv_connection_t * connection);
extern int lw6srv_send (lw6srv_backend_t * backend,
			lw6srv_connection_t * connection, char *message);
extern char *lw6srv_recv (lw6srv_backend_t * backend,
			  lw6srv_connection_t * connection);
extern int lw6srv_is_alive (lw6srv_backend_t * backend,
			    lw6srv_connection_t * connection);
extern char *lw6srv_repr (lw6srv_backend_t * backend,
			  lw6srv_connection_t * connection);
extern char *lw6srv_error (lw6srv_backend_t * backend,
			   lw6srv_connection_t * connection);

/*
 * In control.c
 */
extern lw6srv_listener_t *lw6srv_start (char *ip, int port);
extern int lw6srv_poll (lw6srv_listener_t * listener);
extern void lw6srv_stop (lw6srv_listener_t * listener);

/*
 * In register.c
 */
extern char *lw6srv_default_backends ();
extern lw6sys_assoc_t *lw6srv_get_backends (int argc, char *argv[]);
extern lw6srv_backend_t *lw6srv_create_backend (int argc, char *argv[],
						char *name);
extern void lw6srv_destroy_backend (lw6srv_backend_t * backend);

/* srv-tcpaccepter.c */
extern lw6srv_tcp_accepter_t *lw6srv_tcp_accepter_new (char *client_ip,
						       int client_port,
						       int sock);
extern void lw6srv_tcp_accepter_free (lw6srv_tcp_accepter_t * tcp_accepter);

/* srv-test.c */
extern int lw6srv_test ();

/* srv-udpbuffer.c */
extern lw6srv_udp_buffer_t *lw6srv_udp_buffer_new (char *client_ip,
						   int client_port,
						   char *line);
extern void lw6srv_udp_buffer_free (lw6srv_udp_buffer_t * udp_buffer);

#endif
