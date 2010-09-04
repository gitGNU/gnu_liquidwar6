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

typedef void (*lw6cnx_recv_callback_t) (void *func_data, char *msg);

typedef struct lw6cnx_connection_s
{
  char *local_url;
  char *remote_url;
  char *remote_ip;
  int remote_port;
  char *password;
  char *password_send_checksum;
  char *local_id;
  char *remote_id;
  lw6cnx_recv_callback_t recv_callback_func;
  void *recv_callback_data;
  u_int32_t foo_bar_key;
  int64_t next_send_foo_timestamp;
  void *backend_specific_data;
}
lw6cnx_connection_t;

/* cnx-connection.c */
extern lw6cnx_connection_t *lw6cnx_connection_new (char *local_url,
						   char *remote_url,
						   char *remote_ip,
						   int remote_port,
						   char *password,
						   char *local_id,
						   char *remote_id,
						   lw6cnx_recv_callback_t
						   recv_callback_func,
						   void *recv_callback_data);
extern void lw6cnx_connection_free (lw6cnx_connection_t * connection);
extern int lw6cnx_connection_should_send_foo (lw6cnx_connection_t *
					      connection, int64_t now);
extern void lw6cnx_connection_init_foo_bar_key (lw6cnx_connection_t *
						connection, int64_t now,
						int next_foo_delay);

/* cnx-test.c */
extern int lw6cnx_test (int mode);

#endif
