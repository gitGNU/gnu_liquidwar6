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

#ifndef LIQUIDWAR6P2P_H
#define LIQUIDWAR6P2P_H

#include "../sys/sys.h"
#include "../cfg/cfg.h"
#include "../net/net.h"
#include "../nod/nod.h"
#include "../cli/cli.h"
#include "../srv/srv.h"

typedef struct lw6p2p_db_s
{
  /*
   * The first member, id, is the same as the internal
   * _lw6p2p_db_t structure. The rest of it is hidden.
   * The program will cast from lw6p2p_db_t to _lw6p2p_db_t
   * internally.
   */
  u_int32_t id;
} lw6p2p_db_t;

typedef struct lw6p2p_node_s
{
  /*
   * The first member, id, is the same as the internal
   * _lw6p2p_node_t structure. The rest of it is hidden.
   * The program will cast from lw6p2p_node_t to _lw6p2p_node_t
   * internally.
   */
  u_int32_t id;
} lw6p2p_node_t;

/* p2p-db.c */
extern lw6p2p_db_t *lw6p2p_db_open (int argc, char *argv[], char *name);
extern void lw6p2p_db_close (lw6p2p_db_t * db);
extern char *lw6p2p_db_repr (lw6p2p_db_t * db);
extern int lw6p2p_db_reset (int argc, char *argv[], char *name);
extern char *lw6p2p_db_default_name ();

/* p2p-node.c */
extern lw6p2p_node_t *lw6p2p_node_new (int argc, char *argv[],
				       lw6p2p_db_t * db,
				       char *client_backends,
				       char *server_backends,
				       char *bind_ip,
				       int bind_port, u_int64_t node_id,
				       char *public_url, char *password,
				       char *title, int bench);
extern void lw6p2p_node_free (lw6p2p_node_t * node);
extern char *lw6p2p_node_repr (lw6p2p_node_t * node);
extern int lw6p2p_node_poll (lw6p2p_node_t * node);
extern void lw6p2p_node_close (lw6p2p_node_t * node);

/* p2p-test.c */
extern int lw6p2p_test (int mode);

#endif
