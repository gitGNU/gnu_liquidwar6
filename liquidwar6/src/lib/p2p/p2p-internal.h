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

#include "../map/map.h"		// only needed for #defines

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
  int foo_delay;
  int ticket_table_hash_size;
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
  int last_cli_oob_broadcast_backend;
  int64_t next_discover_nodes_timestamp;
  int64_t next_verify_nodes_timestamp;
}
_lw6p2p_explore_t;

typedef struct _lw6p2p_backends_s
{
  int nb_cli_backends;
  lw6cli_backend_t **cli_backends;
  int nb_srv_backends;
  lw6srv_backend_t **srv_backends;
} _lw6p2p_backends_t;

typedef struct _lw6p2p_tentacle_s
{
  _lw6p2p_backends_t *backends;
  char *local_url;
  char *remote_url;
  char *remote_ip;
  int remote_port;
  char *password;
  u_int64_t local_id_int;
  char *local_id_str;
  u_int64_t remote_id_int;
  char *remote_id_str;
  int hello_sent;
  int dns_ok;
  int nb_cli_connections;
  lw6cnx_connection_t **cli_connections;
  int nb_srv_connections;
  lw6cnx_connection_t **srv_connections;
} _lw6p2p_tentacle_t;

typedef struct _lw6p2p_node_s
{
  u_int32_t id;
  int closed;
  _lw6p2p_db_t *db;
  char *bind_ip;
  int bind_port;
  int broadcast;
  u_int64_t node_id_int;
  char *node_id_str;
  char *public_url;
  char *password;
  lw6nod_info_t *node_info;
  char *known_nodes;
  lw6srv_listener_t *listener;
  _lw6p2p_backends_t backends;
  lw6sys_list_t *srv_oobs;
  lw6sys_list_t *cli_oobs;
  _lw6p2p_flush_t flush;
  _lw6p2p_explore_t explore;
  lw6cnx_ticket_table_t ticket_table;
  _lw6p2p_tentacle_t tentacles[LW6MAP_MAX_NB_NODES];
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

/* p2p-backends.c */
extern int _lw6p2p_backends_init_cli (int argc, char *argv[],
				      _lw6p2p_backends_t * backends,
				      char *client_backends);
extern int _lw6p2p_backends_init_srv (int argc, char *argv[],
				      _lw6p2p_backends_t * backends,
				      char *server_backends,
				      lw6srv_listener_t * listener);
extern void _lw6p2p_backends_clear_cli (_lw6p2p_backends_t * backends);
extern void _lw6p2p_backends_clear_srv (_lw6p2p_backends_t * backends);

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
					 int bind_port, int broadcast,
					 char *public_url,
					 char *password, char *title,
					 char *description, int bench,
					 char *known_nodes);
extern void _lw6p2p_node_free (_lw6p2p_node_t * node);
extern char *_lw6p2p_node_repr (_lw6p2p_node_t * node);
extern int _lw6p2p_node_poll (_lw6p2p_node_t * node);
extern void _lw6p2p_node_close (_lw6p2p_node_t * node);
extern u_int64_t _lw6p2p_node_get_id (_lw6p2p_node_t * node);
extern int _lw6p2p_node_insert_discovered (_lw6p2p_node_t * node,
					   char *public_url);
extern int _lw6p2p_node_find_free_tentacle (_lw6p2p_node_t * node);
extern int _lw6p2p_node_find_tentacle (_lw6p2p_node_t * node,
				       u_int64_t remote_id);
extern int _lw6p2p_node_register_tentacle (_lw6p2p_node_t * node,
					   char *remote_url,
					   char *real_remote_ip,
					   u_int64_t remote_id);
extern int _lw6p2p_node_unregister_tentacle (_lw6p2p_node_t * node,
					     u_int64_t remote_id);

/* p2p-recv.c */
extern void _lw6p2p_recv_process (_lw6p2p_node_t *node,
		      lw6cnx_connection_t *cnx,
		      u_int64_t logical_from_id, char *message);
extern void _lw6p2p_recv_forward (_lw6p2p_node_t *node,
		      lw6cnx_connection_t *cnx,
		      u_int32_t logical_ticket_sig,
		      u_int64_t logical_from_id, u_int64_t logical_to_id,char *message);
extern void _lw6p2p_recv_callback (void *recv_callback_data,
					void * connection,
					u_int32_t physical_ticket_sig,
					u_int32_t logical_ticket_sig,
					u_int64_t logical_from_id, u_int64_t logical_to_id,
		       char *message);

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

/* p2p-tentacle.c */
extern int _lw6p2p_tentacle_init (_lw6p2p_tentacle_t * tentacle,
				  _lw6p2p_backends_t * backends,
				  lw6srv_listener_t * listener,
				  char *local_url,
				  char *remote_url,
				  char *real_remote_ip,
				  char *password,
				  u_int64_t local_id, u_int64_t remote_id,
				  lw6cnx_recv_callback_t recv_callback_func,
				  void *recv_callback_data);
extern void _lw6p2p_tentacle_clear (_lw6p2p_tentacle_t * tentacle);
extern int _lw6p2p_tentacle_enabled (_lw6p2p_tentacle_t * tentacle);
extern void _lw6p2p_tentacle_poll (_lw6p2p_tentacle_t * tentacle,
				   lw6nod_info_t * node_info,
				   lw6cnx_ticket_table_t * ticket_table,
				   int foo_delay);

#endif
