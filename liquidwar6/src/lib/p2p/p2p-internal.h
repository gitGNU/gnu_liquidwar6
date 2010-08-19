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

#define _LW6P2P_DEFAULT_NAME "p2p"
#define _LW6P2P_CLEAN_DATABASE_SQL "clean-database.sql"
#define _LW6P2P_CREATE_DATABASE_SQL "create-database.sql"
#define _LW6P2P_DELETE_CONNECTION_BY_PTR_SQL "delete-connection-by-ptr.sql"
#define _LW6P2P_DELETE_NODE_BY_ID_SQL "delete-node-by-id.sql"
#define _LW6P2P_DELETE_NODE_BY_URL_SQL "delete-node-by-url.sql"
#define _LW6P2P_DELETE_OLD_NODE_SQL "delete-old-node.sql"
#define _LW6P2P_INSERT_CONNECTION_SQL "insert-connection.sql"
#define _LW6P2P_INSERT_LOCAL_NODE_SQL "insert-local-node.sql"
#define _LW6P2P_INSERT_DISCOVERED_NODE_SQL "insert-discovered-node.sql"
#define _LW6P2P_SELECT_CONNECTION_SQL "select-connection.sql"
#define _LW6P2P_SELECT_NODE_BY_URL_SQL "select-node-by-url.sql"
#define _LW6P2P_SELECT_UNVERIFIED_NODE_SQL "select-unverified-node.sql"
#define _LW6P2P_SELECT_OTHER_NODE_SQL "select-other-node.sql"
#define _LW6P2P_UPDATE_NODE_SQL "update-node.sql"

#define _LW6P2P_DB_NODE_NB_FIELDS 22
#define _LW6P2P_DB_NODE_ORDER_CREATION_TIMESTAMP 0
#define _LW6P2P_DB_NODE_ORDER_VERSION 1
#define _LW6P2P_DB_NODE_ORDER_CODENAME 2
#define _LW6P2P_DB_NODE_ORDER_STAMP 3
#define _LW6P2P_DB_NODE_ORDER_ID 4
#define _LW6P2P_DB_NODE_ORDER_URL 5
#define _LW6P2P_DB_NODE_ORDER_TITLE 6
#define _LW6P2P_DB_NODE_ORDER_DESCRIPTION 7
#define _LW6P2P_DB_NODE_ORDER_PASSWORD 8
#define _LW6P2P_DB_NODE_ORDER_BENCH 9
#define _LW6P2P_DB_NODE_ORDER_LEVEL 10
#define _LW6P2P_DB_NODE_ORDER_REQUIRED_BENCH 11
#define _LW6P2P_DB_NODE_ORDER_NB_COLORS 12
#define _LW6P2P_DB_NODE_ORDER_MAX_NB_COLORS 13
#define _LW6P2P_DB_NODE_ORDER_NB_CURSORS 14
#define _LW6P2P_DB_NODE_ORDER_MAX_NB_CURSORS 15
#define _LW6P2P_DB_NODE_ORDER_NB_NODES 16
#define _LW6P2P_DB_NODE_ORDER_MAX_NB_NODES 17
#define _LW6P2P_DB_NODE_ORDER_IP 18
#define _LW6P2P_DB_NODE_ORDER_PORT 19
#define _LW6P2P_DB_NODE_ORDER_LAST_PING_TIMESTAMP 20
#define _LW6P2P_DB_NODE_ORDER_PING_DELAY_MSEC 21

typedef int (*_lw6p2p_db_callback_t) (void *func_data, int nb_fields,
				      char **fields_values,
				      char **fields_names);

typedef struct _lw6p2p_consts_s
{
  int accept_delay;
  int flush_verified_nodes_delay;
  int flush_discovered_nodes_delay;
  int explore_discover_nodes_delay;
  int explore_verify_nodes_delay;
  int node_expire_soft_delay;
  int node_expire_hard_delay;
  int node_verify_max_at_once;
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
  char *db_filename;
  char *log_filename;
  _lw6p2p_data_t data;
  void *mutex;
  sqlite3 *handler;
  int64_t t0;
} _lw6p2p_db_t;

typedef struct _lw6p2p_flush_s
{
  int64_t next_verified_nodes_timestamp;
  int64_t next_discovered_nodes_timestamp;
}
_lw6p2p_flush_t;

typedef struct _lw6p2p_explore_s
{
  int64_t next_discover_nodes_timestamp;
  int64_t next_verify_nodes_timestamp;
}
_lw6p2p_explore_t;

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
  char *known_nodes;
  int nb_cli_backends;
  lw6cli_backend_t **cli_backends;
  lw6srv_listener_t *listener;
  int nb_srv_backends;
  lw6srv_backend_t **srv_backends;
  lw6sys_list_t *srv_oobs;
  lw6sys_list_t *cli_oobs;
  _lw6p2p_flush_t flush;
  _lw6p2p_explore_t explore;
} _lw6p2p_node_t;

typedef struct _lw6p2p_srv_oob_callback_data_s
{
  lw6srv_backend_t *backend;
  lw6nod_info_t *node_info;
  lw6srv_oob_t *srv_oob;
} _lw6p2p_srv_oob_callback_data_t;

typedef struct _lw6p2p_cli_oob_callback_data_s
{
  lw6cli_backend_t *backend;
  lw6nod_info_t *node_info;
  lw6cli_oob_t *cli_oob;
} _lw6p2p_cli_oob_callback_data_t;

/* p2p-clioob.c */
extern _lw6p2p_cli_oob_callback_data_t
  * _lw6p2p_cli_oob_callback_data_new (lw6cli_backend_t * backend,
				       _lw6p2p_node_t * node,
				       char *public_url);
extern void
_lw6p2p_cli_oob_callback_data_free (_lw6p2p_cli_oob_callback_data_t *
				    cli_oob);
extern int _lw6p2p_cli_oob_filter (_lw6p2p_cli_oob_callback_data_t * cli_oob);
extern void _lw6p2p_cli_oob_callback (void *callback_data);
extern int _lw6p2p_cli_oob_verify_callback_func (void *func_data, char *url,
						 char *ip, int port,
						 int ping_delay_msec,
						 lw6sys_assoc_t * assoc);

/* p2p-data.c */
extern int _lw6p2p_data_load (_lw6p2p_data_t * data, char *data_dir);
extern int _lw6p2p_data_unload (_lw6p2p_data_t * data);

/* p2p-db.c */
extern _lw6p2p_db_t *_lw6p2p_db_open (int argc, char *argv[], char *name);
extern void _lw6p2p_db_close (_lw6p2p_db_t * db);
extern char *_lw6p2p_db_repr (_lw6p2p_db_t * db);
extern char *_lw6p2p_db_get_query (_lw6p2p_db_t * db, char *key);
extern void _lw6p2p_db_log (_lw6p2p_db_t * db, char *message);
extern int _lw6p2p_db_lock (_lw6p2p_db_t * db);
extern int _lw6p2p_db_unlock (_lw6p2p_db_t * db);
extern int _lw6p2p_db_trylock (_lw6p2p_db_t * db);
extern int _lw6p2p_db_exec_ignore_data (_lw6p2p_db_t * db, char *sql);
extern int _lw6p2p_db_exec (_lw6p2p_db_t * db, char *sql,
			    _lw6p2p_db_callback_t func, void *func_data);
extern int _lw6p2p_db_create_database (_lw6p2p_db_t * db);
extern int _lw6p2p_db_clean_database (_lw6p2p_db_t * db);
extern int _lw6p2p_db_now (_lw6p2p_db_t * db);

/* p2p-explore.c */
extern int _lw6p2p_explore_discover_nodes_if_needed (_lw6p2p_node_t * node);
extern int _lw6p2p_explore_discover_nodes (_lw6p2p_node_t * node);
extern int _lw6p2p_explore_verify_nodes_if_needed (_lw6p2p_node_t * node);
extern int _lw6p2p_explore_verify_nodes (_lw6p2p_node_t * node);

/* p2p-flush.c */
extern int _lw6p2p_flush_verified_nodes_if_needed (_lw6p2p_node_t * node);
extern int _lw6p2p_flush_verified_nodes (_lw6p2p_node_t * node);
extern int _lw6p2p_flush_discovered_nodes_if_needed (_lw6p2p_node_t * node);
extern int _lw6p2p_flush_discovered_nodes (_lw6p2p_node_t * node);

/* p2p-node.c */
extern _lw6p2p_node_t *_lw6p2p_node_new (int argc, char *argv[],
					 _lw6p2p_db_t * db,
					 char *client_backends,
					 char *server_backends,
					 char *bind_ip,
					 int bind_port, u_int64_t node_id,
					 char *public_url, char *password,
					 char *title, char *description,
					 int bench, char *known_nodes);
extern void _lw6p2p_node_free (_lw6p2p_node_t * node);
extern char *_lw6p2p_node_repr (_lw6p2p_node_t * node);
extern int _lw6p2p_node_poll (_lw6p2p_node_t * node);
extern void _lw6p2p_node_close (_lw6p2p_node_t * node);
extern int _lw6p2p_node_insert_discovered (_lw6p2p_node_t * node,
					   char *public_url);

/* p2p-srvoob.c */
extern _lw6p2p_srv_oob_callback_data_t
  * _lw6p2p_srv_oob_callback_data_new (lw6srv_backend_t * backend,
				       lw6nod_info_t * node_info,
				       char *remote_ip, int remote_port,
				       int sock, char *first_line);
extern void
_lw6p2p_srv_oob_callback_data_free (_lw6p2p_srv_oob_callback_data_t *
				    srv_oob);
extern int _lw6p2p_srv_oob_filter (_lw6p2p_srv_oob_callback_data_t * srv_oob);
extern void _lw6p2p_srv_oob_callback (void *callback_data);

#endif
