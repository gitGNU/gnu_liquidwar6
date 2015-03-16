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

#ifndef LIQUIDWAR6P2P_H
#define LIQUIDWAR6P2P_H

#include "../sys/sys.h"
#include "../glb/glb.h"
#include "../cfg/cfg.h"
#include "../net/net.h"
#include "../nod/nod.h"
#include "../cnx/cnx.h"
#include "../msg/msg.h"
#include "../cli/cli.h"
#include "../srv/srv.h"
#include "../dat/dat.h"

/*
 * Must be greater than LW6DAT_MAX_NB_STACKS
 */
#define LW6P2P_MAX_NB_TENTACLES 100

#define LW6P2P_BENCH_NETWORK_DIVIDE 2

/*
 * Hardcoded size for database stored data, carefull
 * that these match the sizes in create-database.sql
 */
#define LW6P2P_VERSION_SIZE 64
#define LW6P2P_CODENAME_SIZE 64
#define LW6P2P_ID_SIZE 16
#define LW6P2P_URL_SIZE 1024
#define LW6P2P_TITLE_SIZE 256
#define LW6P2P_DESCRIPTION_SIZE 2048
#define LW6P2P_COMMUNITY_ID_SIZE 16
#define LW6P2P_LEVEL_SIZE 1024
#define LW6P2P_IP_SIZE 64

/**
 * Handler on a database connection, this must be used to
 * pass order to store/retrieve persistent informations
 * about peers.
 */
typedef struct lw6p2p_db_s
{
  /**
   * The first member, id, is the same as the internal
   * _lw6p2p_db_t structure. The rest of it is hidden.
   * The program will cast from lw6p2p_db_t to _lw6p2p_db_t
   * internally.
   */
  u_int32_t id;
} lw6p2p_db_t;

/**
 * Node object, the main network object, this one will
 * encaspulate everything else, the node can connect to
 * other peers, listen on the network, it's the high-level
 * interface.
 */
typedef struct lw6p2p_node_s
{
  /**
   * The first member, id, is the same as the internal
   * _lw6p2p_node_t structure. The rest of it is hidden.
   * The program will cast from lw6p2p_node_t to _lw6p2p_node_t
   * internally.
   */
  u_int32_t id;
} lw6p2p_node_t;

/**
 * This entry object matches as close as possible the
 * corresponding (node) entry in the database.
 */
typedef struct lw6p2p_entry_s
{
  /*
   * constant data
   */
  /// Node creation timestamp.
  int creation_timestamp;
  /// Node version.
  char version[LW6P2P_VERSION_SIZE + 1];
  /// Node codename.
  char codename[LW6P2P_CODENAME_SIZE + 1];
  /// Node stamp.
  int stamp;
  /// Node id, 64-bit integer as an hexa string.
  char id[LW6P2P_ID_SIZE + 1];
  /// Node URL, the public URL it displays to others.
  char url[LW6P2P_URL_SIZE + 1];
  /// Node title, the short readable name for the node.
  char title[LW6P2P_TITLE_SIZE + 1];
  /// Node description, mode details about this node.
  char description[LW6P2P_DESCRIPTION_SIZE + 1];
  /// Wether it requires a password or not.
  int has_password;
  /// Node bench, reflects how powerfull it is.
  int bench;
  /// Wether this node acts as an open relay or not.
  int open_relay;
  /*
   * variable data
   */
  /// Community id, 64-bit integer as an hexa string.
  char community_id[LW6P2P_COMMUNITY_ID_SIZE + 1];
  /// Current round.
  int round;
  /// Level used.
  char level[LW6P2P_LEVEL_SIZE + 1];
  /// Required bench to connect to this community.
  int required_bench;
  /// Number of colors playing.
  int nb_colors;
  /// Maximum number of colors allowed to play.
  int max_nb_colors;
  /// Number of cursors playing.
  int nb_cursors;
  /// Maximum number of cursors allowed to play.
  int max_nb_cursors;
  /// Number of nodes playing.
  int nb_nodes;
  /// Maximum number of nodes playing.
  int max_nb_nodes;
  /*
   * additionnal data
   */
  /// IP addess of node.
  char ip[LW6P2P_IP_SIZE + 1];
  /// IP port of node.
  int port;
  /// Last time this node has been pinged.
  int last_ping_timestamp;
  /// Ping delay, in milliseconds.
  int ping_delay_msec;
  /// Wether this node is ready to accept connections.
  int available;
} lw6p2p_entry_t;

/* p2p-db.c */
extern lw6p2p_db_t *lw6p2p_db_open (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name);
extern void lw6p2p_db_close (lw6sys_context_t * sys_context, lw6p2p_db_t * db);
extern char *lw6p2p_db_repr (lw6sys_context_t * sys_context, const lw6p2p_db_t * db);
extern int lw6p2p_db_reset (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name);
extern char *lw6p2p_db_default_name (lw6sys_context_t * sys_context);

/* p2p-entry.c */
extern lw6p2p_entry_t *lw6p2p_entry_new (lw6sys_context_t * sys_context, int creation_timestamp, char *version,
					 char *codename, int stamp, char *node_id,
					 char *node_url, char *node_title,
					 char *node_description, int has_password,
					 int bench, int open_relay,
					 char *community_id, int round, char *level,
					 int required_bench, int nb_colors,
					 int max_nb_colors, int nb_cursors,
					 int max_nb_cursors, int nb_nodes,
					 int max_nb_nodes, char *ip, int port, int last_ping_timestamp, int ping_delay_msec, int available);
extern void lw6p2p_entry_free (lw6sys_context_t * sys_context, lw6p2p_entry_t * entry);
extern char *lw6p2p_entry_repr (lw6sys_context_t * sys_context, const lw6p2p_entry_t * entry);

/* p2p-node.c */
extern lw6p2p_node_t *lw6p2p_node_new (lw6sys_context_t * sys_context, int argc, const char *argv[],
				       lw6p2p_db_t * db,
				       char *client_backends,
				       char *server_backends,
				       char *bind_ip,
				       int bind_port, int broadcast,
				       u_int64_t node_id,
				       char *public_url,
				       char *title,
				       char *description, char *password, int bench, int open_relay, char *known_nodes, int network_reliability, int trojan);
extern void lw6p2p_node_free (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern char *lw6p2p_node_repr (lw6sys_context_t * sys_context, const lw6p2p_node_t * node);
extern int lw6p2p_node_poll (lw6sys_context_t * sys_context, lw6p2p_node_t * node, lw6sys_progress_t * progress);
extern void lw6p2p_node_close (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern u_int64_t lw6p2p_node_get_id (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern lw6sys_list_t *lw6p2p_node_get_entries (lw6sys_context_t * sys_context, lw6p2p_node_t * node, int skip_local);
extern int lw6p2p_node_server_start (lw6sys_context_t * sys_context, lw6p2p_node_t * node, int64_t seq_0);
extern int lw6p2p_node_client_join (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url,
				    lw6sys_progress_t * progress);
extern int lw6p2p_node_refresh_peer (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url);
extern void lw6p2p_node_disconnect (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int lw6p2p_node_update_info (lw6sys_context_t * sys_context, lw6p2p_node_t * node,
				    int round, const char *level,
				    int nb_colors,
				    int max_nb_colors, int nb_cursors,
				    int max_nb_cursors, int nb_nodes, int max_nb_nodes, int game_screenshot_size, void *game_screenshot_data);
extern void lw6p2p_node_calibrate (lw6sys_context_t * sys_context, lw6p2p_node_t * node, int64_t timestamp, int64_t seq);
extern int64_t lw6p2p_node_get_local_seq_0 (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int64_t lw6p2p_node_get_local_seq_last (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int64_t lw6p2p_node_get_seq_min (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int64_t lw6p2p_node_get_seq_max (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int64_t lw6p2p_node_get_seq_draft (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int64_t lw6p2p_node_get_seq_reference (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int lw6p2p_node_is_peer_connected (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t peer_id);
extern int lw6p2p_node_is_peer_registered (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t peer_id);
extern int lw6p2p_node_is_seed_needed (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int lw6p2p_node_is_dump_needed (lw6sys_context_t * sys_context, lw6p2p_node_t * node);
extern int lw6p2p_node_put_local_msg (lw6sys_context_t * sys_context, lw6p2p_node_t * node, const char *msg);
extern char *lw6p2p_node_get_next_reference_msg (lw6sys_context_t * sys_context, lw6p2p_node_t * node, lw6sys_progress_t * progress);
extern char *lw6p2p_node_get_next_draft_msg (lw6sys_context_t * sys_context, lw6p2p_node_t * node, lw6sys_progress_t * progress);

/* p2p-test.c */
extern int lw6p2p_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6p2p_test_run (lw6sys_context_t * sys_context, int mode);

#endif // LIQUIDWAR6P2P_H
