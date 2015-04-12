/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>

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
#define _LW6P2P_SELECT_NODE_BY_ID_SQL "select-node-by-id.sql"
#define _LW6P2P_SELECT_NODE_BY_URL_SQL "select-node-by-url.sql"
#define _LW6P2P_SELECT_UNVERIFIED_NODE_SQL "select-unverified-node.sql"
#define _LW6P2P_SELECT_OTHER_NODE_SQL "select-other-node.sql"
#define _LW6P2P_SELECT_AVAILABLE_NODE_SQL "select-available-node.sql"
#define _LW6P2P_SELECT_UNAVAILABLE_NODE_SQL "select-unavailable-node.sql"
#define _LW6P2P_UPDATE_NODE_INFO_SQL "update-node-info.sql"
#define _LW6P2P_UPDATE_NODE_NET_SQL "update-node-net.sql"

#define _LW6P2P_DB_NODE_NB_FIELDS 26
#define _LW6P2P_DB_NODE_ORDER_CREATION_TIMESTAMP 0
#define _LW6P2P_DB_NODE_ORDER_VERSION 1
#define _LW6P2P_DB_NODE_ORDER_CODENAME 2
#define _LW6P2P_DB_NODE_ORDER_STAMP 3
#define _LW6P2P_DB_NODE_ORDER_ID 4
#define _LW6P2P_DB_NODE_ORDER_URL 5
#define _LW6P2P_DB_NODE_ORDER_TITLE 6
#define _LW6P2P_DB_NODE_ORDER_DESCRIPTION 7
#define _LW6P2P_DB_NODE_ORDER_HAS_PASSWORD 8
#define _LW6P2P_DB_NODE_ORDER_BENCH 9
#define _LW6P2P_DB_NODE_ORDER_OPEN_RELAY 10
#define _LW6P2P_DB_NODE_ORDER_COMMUNITY_ID 11
#define _LW6P2P_DB_NODE_ORDER_ROUND 12
#define _LW6P2P_DB_NODE_ORDER_LEVEL 13
#define _LW6P2P_DB_NODE_ORDER_REQUIRED_BENCH 14
#define _LW6P2P_DB_NODE_ORDER_NB_COLORS 15
#define _LW6P2P_DB_NODE_ORDER_MAX_NB_COLORS 16
#define _LW6P2P_DB_NODE_ORDER_NB_CURSORS 17
#define _LW6P2P_DB_NODE_ORDER_MAX_NB_CURSORS 18
#define _LW6P2P_DB_NODE_ORDER_NB_NODES 19
#define _LW6P2P_DB_NODE_ORDER_MAX_NB_NODES 20
#define _LW6P2P_DB_NODE_ORDER_IP 21
#define _LW6P2P_DB_NODE_ORDER_PORT 22
#define _LW6P2P_DB_NODE_ORDER_LAST_PING_TIMESTAMP 23
#define _LW6P2P_DB_NODE_ORDER_PING_DELAY_MSEC 24
#define _LW6P2P_DB_NODE_ORDER_AVAILABLE 25

#define _LW6P2P_NODE_DEFAULT_SEQ_0 0LL

typedef int (*_lw6p2p_db_callback_t) (lw6sys_context_t * sys_context, void *func_data, int nb_fields, char **fields_values, char **fields_names);

typedef struct _lw6p2p_db_callback_data_s
{
  lw6sys_context_t *sys_context;
  _lw6p2p_db_callback_t callback_func;
  void *callback_func_data;
} _lw6p2p_db_callback_data_t;

typedef struct _lw6p2p_consts_s
{
  int accept_delay;
  int flush_verified_nodes_delay;
  int flush_discovered_nodes_delay;
  int explore_discover_nodes_delay;
  int explore_verify_nodes_delay;
  int connect_registered_nodes_delay;
  int connect_registered_max_ping;
  int node_expire_soft_delay;
  int node_expire_hard_delay;
  int node_verify_max_at_once;
  int foo_delay;
  int join_delay;
  int miss_get_delay;
  int miss_duplicate_delay;
  int miss_wake_up_interval;
  int miss_max_range;
  int ticket_table_hash_size;
  int send_per_poll_limit_n;
  int received_atom_parts_per_poll_to_disable_miss;
  int meta_nb_duplicates;
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
  const char *db_filename;
  const char *log_filename;
  _lw6p2p_data_t data;
  lw6sys_mutex_t *mutex;
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

typedef struct _lw6p2p_connect_registered_s
{
  int64_t next_connect_registered_nodes_timestamp;
}
_lw6p2p_connect_registered_t;

typedef struct _lw6p2p_backends_s
{
  int hint_timeout_max;
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
  int data_exchanged;
  int joined;
  int dns_ok;
  int nb_cli_connections;
  lw6cnx_connection_t **cli_connections;
  int nb_srv_connections;
  lw6cnx_connection_t **srv_connections;
  lw6sys_list_t *unsent_reliable_queue;
  lw6sys_list_t *unsent_unreliable_queue;
  int last_poll_received_messages;
  lw6dat_miss_t last_miss;
  int64_t last_miss_timestamp;
} _lw6p2p_tentacle_t;

typedef struct _lw6p2p_node_s
{
  u_int32_t id;
  lw6sys_mutex_t *mutex;
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
  int network_reliability;
  int trojan;
  lw6srv_listener_t *listener;
  _lw6p2p_backends_t backends;
  lw6sys_list_t *srv_oobs;
  lw6sys_list_t *cli_oobs;
  _lw6p2p_flush_t flush;
  _lw6p2p_explore_t explore;
  _lw6p2p_connect_registered_t connect_registered;
  lw6cnx_ticket_table_t ticket_table;
  _lw6p2p_tentacle_t tentacles[LW6P2P_MAX_NB_TENTACLES];
  lw6dat_warehouse_t *warehouse;
  //  int64_t seq_0;
  int64_t calibrate_timestamp;
  int64_t calibrate_seq;
  //  int64_t serialized_seq;
  //  char *serialized_level;
  //  char *serialized_game_struct;
  //  char *serialized_game_state;
  int seed_needed;
  int dump_needed;
  int64_t last_seq_reference;
  //int64_t last_seq_draft;
  lw6sys_list_t *reference_msg;
  lw6sys_list_t *draft_msg;
  int64_t last_poll_miss_timestamp;
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
extern int _lw6p2p_backends_init_cli (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6p2p_backends_t * backends, char *client_backends);
extern int _lw6p2p_backends_init_srv (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6p2p_backends_t * backends, char *server_backends,
				      lw6srv_listener_t * listener);
extern void _lw6p2p_backends_clear_cli (lw6sys_context_t * sys_context, _lw6p2p_backends_t * backends);
extern void _lw6p2p_backends_clear_srv (lw6sys_context_t * sys_context, _lw6p2p_backends_t * backends);

/* p2p-clioob.c */
extern _lw6p2p_cli_oob_callback_data_t *_lw6p2p_cli_oob_callback_data_new (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, _lw6p2p_node_t * node,
									   const char *public_url);
extern void _lw6p2p_cli_oob_callback_data_free (lw6sys_context_t * sys_context, _lw6p2p_cli_oob_callback_data_t * cli_oob);
extern int _lw6p2p_cli_oob_filter (lw6sys_context_t * sys_context, _lw6p2p_cli_oob_callback_data_t * cli_oob);
extern void _lw6p2p_cli_oob_callback (lw6sys_context_t * sys_context, void *callback_data);
extern int _lw6p2p_cli_oob_verify_callback_func (lw6sys_context_t * sys_context, void *func_data, const char *url, const char *ip, int port,
						 int ping_delay_msec, lw6sys_assoc_t * assoc);

/* p2p-connectregistered.c */
extern int _lw6p2p_connect_registered_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_connect_registered_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);

/* p2p-data.c */
extern int _lw6p2p_data_load (lw6sys_context_t * sys_context, _lw6p2p_data_t * data, const char *data_dir);
extern int _lw6p2p_data_unload (lw6sys_context_t * sys_context, _lw6p2p_data_t * data);

/* p2p-db.c */
extern _lw6p2p_db_t *_lw6p2p_db_open (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name);
extern void _lw6p2p_db_close (lw6sys_context_t * sys_context, _lw6p2p_db_t * db);
extern char *_lw6p2p_db_repr (lw6sys_context_t * sys_context, const _lw6p2p_db_t * db);
extern char *_lw6p2p_db_get_query (lw6sys_context_t * sys_context, _lw6p2p_db_t * db, char *key);
extern void _lw6p2p_db_log (lw6sys_context_t * sys_context, _lw6p2p_db_t * db, char *message);
extern int _lw6p2p_db_lock (lw6sys_context_t * sys_context, _lw6p2p_db_t * db);
extern int _lw6p2p_db_unlock (lw6sys_context_t * sys_context, _lw6p2p_db_t * db);
extern int _lw6p2p_db_trylock (lw6sys_context_t * sys_context, _lw6p2p_db_t * db);
extern int _lw6p2p_db_sql_callback (void *func_data, int nb_fields, char **fields_values, char **fields_names);
extern int _lw6p2p_db_exec_ignore_data (lw6sys_context_t * sys_context, _lw6p2p_db_t * db, char *sql);
extern int _lw6p2p_db_exec (lw6sys_context_t * sys_context, _lw6p2p_db_t * db, char *sql, _lw6p2p_db_callback_t func, void *func_data);
extern int _lw6p2p_db_create_database (lw6sys_context_t * sys_context, _lw6p2p_db_t * db);
extern int _lw6p2p_db_clean_database (lw6sys_context_t * sys_context, _lw6p2p_db_t * db);
extern int _lw6p2p_db_timestamp (lw6sys_context_t * sys_context, _lw6p2p_db_t * db, int64_t timestamp);
extern int _lw6p2p_db_now (lw6sys_context_t * sys_context, _lw6p2p_db_t * db);

/* p2p-explore.c */
extern int _lw6p2p_explore_discover_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_explore_discover_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern void _lw6p2p_explore_start_verify_node (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *public_url);
extern int _lw6p2p_explore_verify_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_explore_verify_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);

/* p2p-flush.c */
extern int _lw6p2p_flush_verified_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_flush_verified_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_flush_discovered_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_flush_discovered_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);

/* p2p-node.c */
extern _lw6p2p_node_t *_lw6p2p_node_new (lw6sys_context_t * sys_context, int argc, const char *argv[],
					 _lw6p2p_db_t * db,
					 char *client_backends,
					 char *server_backends,
					 char *bind_ip,
					 int bind_port, int broadcast,
					 u_int64_t node_id,
					 char *public_url,
					 char *title,
					 char *description, char *password, int bench, int open_relay, char *known_nodes, int network_reliability, int trojan);
extern void _lw6p2p_node_free (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern char *_lw6p2p_node_repr (lw6sys_context_t * sys_context, const _lw6p2p_node_t * node);
extern int _lw6p2p_node_lock (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_unlock (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_trylock (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_poll (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6sys_progress_t * progress);
extern void _lw6p2p_node_close (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern u_int64_t _lw6p2p_node_get_id (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_insert_discovered (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, char *public_url);
extern int _lw6p2p_node_update_peer_info_x (lw6sys_context_t * sys_context, _lw6p2p_node_t * node,
					    const char *version,
					    const char *codename,
					    int stamp,
					    const char *id,
					    const char *url,
					    const char *title,
					    const char *description,
					    int password, int bench,
					    int open_relay,
					    int creation_timestamp,
					    const char *community_id,
					    int round, const char *level,
					    int required_bench, int nb_colors,
					    int max_nb_colors, int nb_cursors, int max_nb_cursors, int nb_nodes, int max_nb_nodes);
extern int _lw6p2p_node_update_peer_info (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6nod_info_t * peer_info);
extern int _lw6p2p_node_update_peer_net (lw6sys_context_t * sys_context, _lw6p2p_node_t * node,
					 const char *id, const char *url, const char *ip, int port, int last_ping_timestamp, int ping_delay_msec);
extern int _lw6p2p_node_update_local (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6nod_info_t * node_info);
extern int _lw6p2p_node_find_free_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_find_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id);
extern int _lw6p2p_node_register_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *remote_url, const char *real_remote_ip,
					   u_int64_t remote_id);
extern int _lw6p2p_node_unregister_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id);
extern lw6sys_list_t *_lw6p2p_node_get_entries (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int skip_local);
extern int _lw6p2p_node_server_start (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t seq_0);
extern int _lw6p2p_node_client_join (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url,
				     lw6sys_progress_t * progress);
extern int _lw6p2p_node_refresh_peer (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url);
extern void _lw6p2p_node_disconnect (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_update_info (lw6sys_context_t * sys_context, _lw6p2p_node_t * node,
				     int round, const char *level,
				     int nb_colors,
				     int max_nb_colors, int nb_cursors,
				     int max_nb_cursors, int nb_nodes, int max_nb_nodes, int game_screenshot_size, void *game_screenshot_data);
extern void _lw6p2p_node_calibrate (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t timestamp, int64_t seq);
extern int64_t _lw6p2p_node_get_local_seq_0 (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int64_t _lw6p2p_node_get_local_seq_last (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int64_t _lw6p2p_node_get_seq_min (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int64_t _lw6p2p_node_get_seq_max (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int64_t _lw6p2p_node_get_seq_draft (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int64_t _lw6p2p_node_get_seq_reference (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_is_peer_connected (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t peer_id);
extern int _lw6p2p_node_is_peer_registered (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t peer_id);
extern int _lw6p2p_node_is_seed_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_is_dump_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node);
extern int _lw6p2p_node_put_local_msg (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *msg);
extern char *_lw6p2p_node_get_next_reference_msg (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6sys_progress_t * progress);
extern char *_lw6p2p_node_get_next_draft_msg (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6sys_progress_t * progress);

/* p2p-peeridlist.c */
extern void _lw6p2p_peer_id_list_process_join (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6nod_info_t * remote_node_info);

/* p2p-recv.c */
extern void _lw6p2p_recv_process (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6cnx_connection_t * connection, const lw6cnx_packet_t * packet);
extern void _lw6p2p_recv_forward (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6cnx_connection_t * connection, const lw6cnx_packet_t * packet);
extern void _lw6p2p_recv_callback (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6cnx_connection_t * connection, const lw6cnx_packet_t * packet);

/* p2p-srvoob.c */
extern _lw6p2p_srv_oob_callback_data_t
  * _lw6p2p_srv_oob_callback_data_new (lw6sys_context_t * sys_context, lw6srv_backend_t * backend,
				       lw6nod_info_t * node_info, const char *remote_ip, int remote_port, int sock, const char *first_line);
extern void _lw6p2p_srv_oob_callback_data_free (lw6sys_context_t * sys_context, _lw6p2p_srv_oob_callback_data_t * srv_oob);
extern int _lw6p2p_srv_oob_filter (lw6sys_context_t * sys_context, _lw6p2p_srv_oob_callback_data_t * srv_oob);
extern void _lw6p2p_srv_oob_callback (lw6sys_context_t * sys_context, void *callback_data);

/* p2p-tentacle.c */
extern int _lw6p2p_tentacle_init (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle,
				  _lw6p2p_backends_t * backends,
				  lw6srv_listener_t * listener,
				  const char *local_url,
				  const char *remote_url,
				  const char *real_remote_ip, const char *password, u_int64_t local_id, u_int64_t remote_id, int network_reliability);
extern void _lw6p2p_tentacle_clear (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle);
extern int _lw6p2p_tentacle_enabled (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle);
extern void _lw6p2p_tentacle_poll_protocol (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle,
					    lw6nod_info_t * node_info, lw6cnx_ticket_table_t * ticket_table, const _lw6p2p_consts_t * consts, int serial);
extern void _lw6p2p_tentacle_poll_queues (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, _lw6p2p_tentacle_t * tentacle);
extern void _lw6p2p_tentacle_poll (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, _lw6p2p_tentacle_t * tentacle, int serial);
extern int _lw6p2p_tentacle_send_best (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle,
				       int64_t now,
				       lw6cnx_ticket_table_t * ticket_table,
				       u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *msg, int reliable);
extern int _lw6p2p_tentacle_send_redundant (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle,
					    int64_t now,
					    lw6cnx_ticket_table_t *
					    ticket_table, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *msg);
extern lw6cnx_connection_t *_lw6p2p_tentacle_find_connection_with_foo_bar_key (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle,
									       u_int32_t foo_bar_key);
extern lw6cnx_connection_t *_lw6p2p_tentacle_find_connection_with_lowest_ping (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle, int reliable);

#endif // LIQUIDWAR6P2P_INTERNAL_H
