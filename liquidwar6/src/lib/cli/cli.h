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

#ifndef LIQUIDWAR6CLI_H
#define LIQUIDWAR6CLI_H

#include "../sys/sys.h"
#include "../glb/glb.h"
#include "../dyn/dyn.h"
#include "../cfg/cfg.h"
#include "../net/net.h"
#include "../nod/nod.h"
#include "../cnx/cnx.h"
#include "../msg/msg.h"

#define LW6CLI_CONTENT_BUFFER_SIZE LW6NET_PPPOE_MTU

typedef int (*lw6cli_verify_callback_func_t) (void *func_data, char *url,
					      char *ip, int port,
					      int ping_delay_msec,
					      lw6sys_assoc_t * assoc);

typedef struct lw6cli_oob_data_s
{
  int64_t creation_timestamp;
  int do_not_finish;
  char *public_url;
  lw6cli_verify_callback_func_t verify_callback_func;
  void *verify_callback_data;
}
lw6cli_oob_data_t;

typedef struct lw6cli_oob_s
{
  void *thread;
  lw6cli_oob_data_t data;
}
lw6cli_oob_t;

typedef struct lw6cli_backend_s
{
  lw6dyn_dl_handle_t *dl_handle;
  void *cli_context;
  int argc;
  char **argv;
  u_int32_t id;
  char *name;

  void *(*init) (int argc, char *argv[]);
  void (*quit) (void *cli_context);
  int (*process_oob) (void *cli_context, lw6nod_info_t * node_info,
		      lw6cli_oob_data_t * oob_data);
  lw6cnx_connection_t *(*open) (void *cli_context, char *local_url,
				char *remote_url, char *remote_ip,
				int remote_port, char *password,
				u_int64_t local_id, u_int64_t remote_id,
				int dns_ok,
				int network_reliability,
				lw6cnx_recv_callback_t recv_callback_func,
				void *recv_callback_data);
  void (*close) (void *cli_context, lw6cnx_connection_t * connection);
  int (*send) (void *cli_context, lw6cnx_connection_t * connection,
	       u_int32_t physical_ticket_sig,
	       u_int32_t logical_ticket_sig,
	       u_int64_t logical_from_id,
	       u_int64_t logical_to_id, char *message);
  void (*poll) (void *cli_context, lw6cnx_connection_t * connection);
  int (*is_alive) (void *cli_context, lw6cnx_connection_t * connection);
  char *(*repr) (void *cli_context, lw6cnx_connection_t * connection);
  char *(*error) (void *cli_context, lw6cnx_connection_t * connection);
}
lw6cli_backend_t;

/*
 * In api.c
 */
extern int lw6cli_init (lw6cli_backend_t * backend);
extern void lw6cli_quit (lw6cli_backend_t * backend);
extern int lw6cli_process_oob (lw6cli_backend_t * backend,
			       lw6nod_info_t * node_info,
			       lw6cli_oob_data_t * oob_data);
extern lw6cnx_connection_t *lw6cli_open (lw6cli_backend_t * backend,
					 char *local_url,
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
extern void lw6cli_close (lw6cli_backend_t * backend,
			  lw6cnx_connection_t * connection);
extern int lw6cli_send (lw6cli_backend_t * backend,
			lw6cnx_connection_t * connection,
			u_int32_t physical_ticket_sig,
			u_int32_t logical_ticket_sig,
			u_int64_t logical_from_id, u_int64_t logical_to_id,
			char *message);
extern void lw6cli_poll (lw6cli_backend_t * backend,
			 lw6cnx_connection_t * connection);
extern int lw6cli_is_alive (lw6cli_backend_t * backend,
			    lw6cnx_connection_t * connection);
extern char *lw6cli_repr (lw6cli_backend_t * backend,
			  lw6cnx_connection_t * connection);
extern char *lw6cli_error (lw6cli_backend_t * backend,
			   lw6cnx_connection_t * connection);

/* cli-oob.c */
extern lw6cli_oob_t *lw6cli_oob_new (char *public_url,
				     lw6cli_verify_callback_func_t
				     verify_callback_func,
				     void *verify_callback_data);
extern void lw6cli_oob_free (lw6cli_oob_t * oob);

/*
 * In register.c
 */
extern char *lw6cli_default_backends ();
extern lw6sys_assoc_t *lw6cli_get_backends (int argc, char *argv[]);
extern lw6cli_backend_t *lw6cli_create_backend (int argc, char *argv[],
						char *name);
extern void lw6cli_destroy_backend (lw6cli_backend_t * backend);

/* cli-test.c */
extern int lw6cli_test (int mode);

#endif
