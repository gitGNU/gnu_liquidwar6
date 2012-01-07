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

/**
 * Holds the data for the process_oob function, this is
 * merely a utility struct to simplify headers/
 */
typedef struct lw6cli_oob_data_s
{
  /**
   * Creation timestamp of the OOB request, this is used
   * to know wether we have already timed out or not. This is
   * not the node creation timestamp.
   */
  int64_t creation_timestamp;
  /**
   * Flag used to force finish, for instance when we want to
   * delete the object quickly and do not want to wait until
   * a long polling-based network operation finishes completely.
   */
  int do_not_finish;
  /**
   * Public URL of the node, we need this at hand to serve
   * it quickly to peers, and be able to perform basic checks.
   */
  char *public_url;
  /**
   * Pointer on a function which will verify if peer is OK,
   * and act accordinlgy if it's OK or not. Note that the
   * callback function might be called pretty much anytime
   * in the heavily multithreaded context of LW6 so it must
   * be reentrant and have mutexes if needed. Indeed, it's very
   * likely to update some shared list of available nodes.
   */
  lw6cli_verify_callback_func_t verify_callback_func;
  /// Data passed to the verify_callback function.
  void *verify_callback_data;
}
lw6cli_oob_data_t;

/**
 * Structure containing both the thread running an OOB request
 * and its data. It was advantagious to separate thoses two
 * and not make the thread a permanent member of the OOB data
 * struct, since it allows the actual OOB code to be totally
 * unaware of the thread running it, which is, to some extent,
 * safer.
 */
typedef struct lw6cli_oob_s
{
  /// Pointer on thread running the OOB request.
  void *thread;
  /// Data used by the OOB request.
  lw6cli_oob_data_t data;
}
lw6cli_oob_t;

typedef struct lw6cli_backend_s
{
  lw6dyn_dl_handle_t *dl_handle;
  void *cli_context;
  int argc;
  const char **argv;
  u_int32_t id;
  char *name;

  void *(*init) (int argc, const char *argv[]);
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
  char *(*repr) (void *cli_context, lw6cnx_connection_t * connection);
}
lw6cli_backend_t;

/* cli-api.c */
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
extern char *lw6cli_repr (lw6cli_backend_t * backend,
			  lw6cnx_connection_t * connection);

/* cli-oob.c */
extern lw6cli_oob_t *lw6cli_oob_new (char *public_url,
				     lw6cli_verify_callback_func_t
				     verify_callback_func,
				     void *verify_callback_data);
extern void lw6cli_oob_free (lw6cli_oob_t * oob);

/* cli-register.c */
extern char *lw6cli_default_backends ();
extern lw6sys_assoc_t *lw6cli_get_backends (int argc, const char *argv[]);
extern lw6cli_backend_t *lw6cli_create_backend (int argc, const char *argv[],
						char *name);
extern void lw6cli_destroy_backend (lw6cli_backend_t * backend);

/* cli-test.c */
extern int lw6cli_test (int mode);

#endif
