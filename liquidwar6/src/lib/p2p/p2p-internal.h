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

#ifndef LIQUIDWAR6P2P_INTERNAL_H
#define LIQUIDWAR6P2P_INTERNAL_H

#include "p2p.h"

/*
 * This should certainly be made smoe config entry
 */
#define _LW6P2P_SLEEP_DELAY 0.01f

typedef struct _lw6p2p_node_s
{
  u_int32_t id;
  char *bind_ip;
  int bind_port;
  u_int64_t server_id;
  char *public_url;
  int nb_cli_backends;
  lw6cli_backend_t **cli_backends;
  lw6srv_listener_t *listener;
  int nb_srv_backends;
  lw6srv_backend_t **srv_backends;
} _lw6p2p_node_t;

/* p2p-node.c */
extern _lw6p2p_node_t *_lw6p2p_node_new (int argc, char *argv[],
					 char *client_backends,
					 char *server_backends, char *bind_ip,
					 int bind_port, u_int64_t server_id,
					 char *public_url);
extern void _lw6p2p_node_free (_lw6p2p_node_t * node);
extern char *_lw6p2p_node_repr (_lw6p2p_node_t * node);
extern int _lw6p2p_node_poll (_lw6p2p_node_t * node);
extern void _lw6p2p_node_close (_lw6p2p_node_t * node);

#endif
