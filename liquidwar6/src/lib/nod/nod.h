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

#ifndef LIQUIDWAR6NOD_H
#define LIQUIDWAR6NOD_H

#include "../sys/sys.h"

#define LW6NOD_COMMUNITY_ID_NONE 0LL

/**
 * Constant informations about a node, these are informations
 * that do not vary through the node's life, so they are set
 * object creation then left unchanged.
 */
typedef struct lw6nod_const_info_s
{
  /// Program, this should be set to liquidwar6.
  char *program;
  /// The version of the program.
  char *version;
  /// The codename of the program.
  char *codename;
  /// The stamp of the program.
  int stamp;
  /// The id of the node, as an integer (64-bit unsigned).
  u_int64_t id_int;
  /// The id of the node, as a string (64-bit converted to hexa).
  char *id_str;
  /// The public URL of the node.
  char *url;
  /// The title of the node, its readable short name.
  char *title;
  /// More details about the node.
  char *description;
  /// Wether the node is password protected or not.
  int has_password;
  /// The password used, cleartext.
  char *password;
  /// The node bench.
  int bench;
  /// Wether the node acts as an open relay or not.
  int open_relay;
  /// The node creation timestamp.
  int64_t creation_timestamp;
  /// Size of the screenshot (in bytes) when in idle mode
  int idle_screenshot_size;
  /**
   * Idle mode screenshot data. This is just a plain JPEG buffer,
   * which will be used as a fallback if there's no real screenshot
   * available.
   */
  void *idle_screenshot_data;
} lw6nod_const_info_t;

/**
 * Dynamic informations about a node, these are informations
 * that vary through the node's life, so they are unset
 * at object creation and can then be updated from game
 * context.
 */
typedef struct lw6nod_dyn_info_s
{
  /**
   * The ID of the community this node belongs to,
   * as a 64-bit unsigned integer.
   */
  u_int64_t community_id_int;
  /**
   * The ID of the community this node belongs to,
   * as a string (64-bit integer converted to hexa).
   */
  char *community_id_str;
  /// The current round.
  int round;
  /// The current level.
  char *level;
  /// The required bench to connect to this node.
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
  /// Maximum number of nodes allowed to play.
  int max_nb_nodes;
  /// Size of the screenshot, in bytes.
  int game_screenshot_size;
  /**
   * Game screenshot data. This is just a plain JPEG buffer,
   * which will be served when peers and/or web clients want
   * to gather informations about the game. This can be NULL,
   * in that the fallback constant data will be used.
   */
  void *game_screenshot_data;
} lw6nod_dyn_info_t;

/**
 * Informations about a node. Note that in practice
 * this structure is just used to describe our current node,
 * there might be several instances of this if a program 
 * instanciates several nodes, but another data structure is
 * used to store information about peers.typedef struct lw6nod_info_s
 */
typedef struct lw6nod_info_s
{
  /// Mutex used to access dynamic informations.
  void *mutex;
  /// Constant informations, never changes.
  lw6nod_const_info_t const_info;
  /**
   * Dynamic informations, can be updated. Do not modify
   * this directly, instead use appropriate functions
   * which will use mutexes properly.
   */
  lw6nod_dyn_info_t dyn_info;
  /**
   * List of discovered nodes, do not access this directly,
   * instead use proper query functions which, in turn,
   * will use mutexes properly.
   */
  lw6sys_hash_t *discovered_nodes;
  /**
   * List of verified nodes, do not access this directly,
   * instead use proper query functions which, in turn,
   * will use mutexes properly.
   */
  lw6sys_list_t *verified_nodes;
} lw6nod_info_t;

/* nod-dyninfo.c */
extern void lw6nod_dyn_info_free (lw6nod_dyn_info_t * info);

/* nod-info.c */
extern lw6nod_info_t *lw6nod_info_new (char *program,
				       char *version,
				       char *codename,
				       int stamp,
				       u_int64_t id, char *url, char *title,
				       char *description, char *password,
				       int bench, int open_relay, int uptime,
				       int idle_screenshot_size,
				       void *idle_screenshot_data);
extern void lw6nod_info_free (lw6nod_info_t * info);
extern int lw6nod_info_lock (lw6nod_info_t * info);
extern int lw6nod_info_unlock (lw6nod_info_t * info);
extern void lw6nod_info_idle (lw6nod_info_t * info);
extern int lw6nod_info_update (lw6nod_info_t * info, u_int64_t community_id,
			       int round, char *level, int required_bench,
			       int nb_colors, int max_nb_colors,
			       int nb_cursors, int max_nb_cursors,
			       int nb_nodes, int max_nb_nodes,
			       int game_screenshot_size,
			       void *game_screenshot_data);
extern lw6nod_dyn_info_t *lw6nod_info_dup_dyn (lw6nod_info_t * info);
extern lw6sys_hash_t *lw6nod_info_new_discovered_nodes ();
extern int lw6nod_info_add_discovered_node (lw6nod_info_t * info,
					    char *public_url);
extern lw6sys_list_t *lw6nod_info_pop_discovered_nodes (lw6nod_info_t * info);
extern lw6sys_list_t *lw6nod_info_new_verified_nodes ();
extern int lw6nod_info_set_verified_nodes (lw6nod_info_t * info,
					   lw6sys_list_t * list);
extern void lw6nod_info_map_verified_nodes (lw6nod_info_t * info,
					    lw6sys_list_callback_func_t func,
					    void *func_data);

/* nod-test.c */
extern int lw6nod_test (int mode);

#endif
