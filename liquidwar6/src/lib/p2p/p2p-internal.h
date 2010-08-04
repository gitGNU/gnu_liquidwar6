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
#include <sqlite3.h>

#define _LW6P2P_DB_FALSE 0
#define _LW6P2P_DB_TRUE 1

#define _LW6P2P_DEFAULT_NAME "p2p.db"
#define _LW6P2P_CLEAN_DATABASE_SQL "clean-database.sql"
#define _LW6P2P_CREATE_DATABASE_SQL "create-database.sql"
#define _LW6P2P_DELETE_CONNECTION_BY_PTR_SQL "delete-connection-by-ptr.sql"
#define _LW6P2P_DELETE_NODE_BY_ID_SQL "delete-node-by-id.sql"
#define _LW6P2P_INSERT_CONNECTION_SQL "insert-connection.sql"
#define _LW6P2P_INSERT_NODE_SQL "insert-node.sql"
#define _LW6P2P_SELECT_CONNECTION_SQL "select-connection.sql"

typedef struct _lw6p2p_consts_s
{
  float sleep_delay;
}
_lw6p2p_consts_t;

typedef struct _lw6p2p_sql_s
{
  lw6sys_hash_t *queries;
}
_lw6p2p_sql_t;

typedef struct _lw6p2p_screenshot_s
{
  int size;
  void *data;
}
_lw6p2p_screenshot_t;

typedef struct _lw6p2p_data_s
{
  _lw6p2p_consts_t consts;
  _lw6p2p_sql_t sql;
  _lw6p2p_screenshot_t idle_screenshot;
} _lw6p2p_data_t;

typedef struct _lw6p2p_db_s
{
  u_int32_t id;
  char *filename;
  _lw6p2p_data_t data;
  sqlite3 *handler;
} _lw6p2p_db_t;

typedef struct _lw6p2p_node_s
{
  u_int32_t id;
  int closed;
  _lw6p2p_db_t *db;
  char *bind_ip;
  int bind_port;
  u_int64_t node_id_int;
  char *node_id_str;
  char *public_url;
  char *password;
  lw6nod_info_t *node_info;
  int nb_cli_backends;
  lw6cli_backend_t **cli_backends;
  lw6srv_listener_t *listener;
  int nb_srv_backends;
  lw6srv_backend_t **srv_backends;
  lw6sys_list_t *oobs;
} _lw6p2p_node_t;

typedef struct _lw6p2p_oob_callback_data_s
{
  lw6srv_backend_t *backend;
  lw6nod_info_t *node_info;
  lw6srv_oob_t *oob;
} _lw6p2p_oob_callback_data_t;

/* p2p-data.c */
extern int _lw6p2p_data_load (_lw6p2p_data_t * data, char *data_dir);
extern int _lw6p2p_data_unload (_lw6p2p_data_t * data);

/* p2p-db.c */
extern _lw6p2p_db_t *_lw6p2p_db_open (int argc, char *argv[], char *name);
extern void _lw6p2p_db_close (_lw6p2p_db_t * db);
extern char *_lw6p2p_db_repr (_lw6p2p_db_t * db);
extern char *_lw6p2p_db_get_query (_lw6p2p_db_t * db, char *key);
extern int _lw6p2p_db_exec_ignore_data (_lw6p2p_db_t * db, char *sql);
extern int _lw6p2p_db_create_database (_lw6p2p_db_t * db);
extern int _lw6p2p_db_clean_database (_lw6p2p_db_t * db);

/* p2p-node.c */
extern _lw6p2p_node_t *_lw6p2p_node_new (int argc, char *argv[],
					 _lw6p2p_db_t * db,
					 char *client_backends,
					 char *server_backends,
					 char *bind_ip,
					 int bind_port, u_int64_t node_id,
					 char *public_url, char *password,
					 char *title, char *description,
					 int bench);
extern void _lw6p2p_node_free (_lw6p2p_node_t * node);
extern char *_lw6p2p_node_repr (_lw6p2p_node_t * node);
extern int _lw6p2p_node_poll (_lw6p2p_node_t * node);
extern void _lw6p2p_node_close (_lw6p2p_node_t * node);

/* p2p-oob.c */
extern _lw6p2p_oob_callback_data_t
  * _lw6p2p_oob_callback_data_new (lw6srv_backend_t * backend,
				   lw6nod_info_t * node_info, char *remote_ip,
				   int remote_port, int sock);
extern void _lw6p2p_oob_callback_data_free (_lw6p2p_oob_callback_data_t *
					    oob);
extern int _lw6p2p_oob_filter (_lw6p2p_oob_callback_data_t * oob);
extern void _lw6p2p_oob_callback (void *callback_data);

#endif
