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
#include "../dyn/dyn.h"
#include "../cfg/cfg.h"
#include "../net/net.h"

typedef struct lw6cli_connection_s
{
  void *cli_context;
  char *server_url;
  char *client_url;
  void *connection_data;
}
lw6cli_connection_t;

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
  int (*can_handle) (void *cli_context, char *server_url);
  lw6cli_connection_t *(*connect) (void *cli_context, char *server_url,
				   char *client_url, char *password);
  void (*close) (void *cli_context, lw6cli_connection_t * connection);
  int (*send) (void *cli_context, lw6cli_connection_t * connection,
	       char *message);
  char *(*recv) (void *cli_context, lw6cli_connection_t * connection);
  int (*is_alive) (void *cli_context, lw6cli_connection_t * connection);
  char *(*repr) (void *cli_context, lw6cli_connection_t * connection);
  char *(*error) (void *cli_context, lw6cli_connection_t * connection);
}
lw6cli_backend_t;

/*
 * In api.c
 */
extern int lw6cli_init (lw6cli_backend_t * backend);
extern void lw6cli_quit (lw6cli_backend_t * backend);
extern int lw6cli_can_handle (lw6cli_backend_t * backend, char *server_url);
extern lw6cli_connection_t *lw6cli_connect (lw6cli_backend_t * backend,
					    char *server_url,
					    char *client_url, char *password);
extern void lw6cli_close (lw6cli_backend_t * backend,
			  lw6cli_connection_t * connection);
extern int lw6cli_send (lw6cli_backend_t * backend,
			lw6cli_connection_t * connection, char *message);
extern char *lw6cli_recv (lw6cli_backend_t * backend,
			  lw6cli_connection_t * connection);
extern int lw6cli_is_alive (lw6cli_backend_t * backend,
			    lw6cli_connection_t * connection);
extern char *lw6cli_repr (lw6cli_backend_t * backend,
			  lw6cli_connection_t * connection);
extern char *lw6cli_error (lw6cli_backend_t * backend,
			   lw6cli_connection_t * connection);

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
