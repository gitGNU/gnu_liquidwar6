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

#ifndef LIQUIDWAR6NOD_H
#define LIQUIDWAR6NOD_H

#include "../sys/sys.h"

#define LW6NOD_MSG_HELLO "HELLO"
#define LW6NOD_MSG_GOODBYE "GOODBYE"

#define LW6NOD_MSG_INFO "INFO"
#define LW6NOD_MSG_LIST "LIST"

#define LW6NOD_MSG_PROGRAM "PROGRAM"
#define LW6NOD_MSG_VERSION "VERSION"
#define LW6NOD_MSG_UPTIME "UPTIME"
#define LW6NOD_MSG_NODE "ID"
#define LW6NOD_MSG_URL "URL"
#define LW6NOD_MSG_TITLE "TITLE"
#define LW6NOD_MSG_DESCRIPTION "DESCRIPTION"
#define LW6NOD_MSG_BENCH "BENCH"
#define LW6NOD_MSG_LEVEL "LEVEL"
#define LW6NOD_MSG_REQUIRED "REQUIRED"
#define LW6NOD_MSG_LIMIT "LIMIT"
#define LW6NOD_MSG_COLORS "COLORS"
#define LW6NOD_MSG_NODES "NODES"
#define LW6NOD_MSG_CURSORS "CURSORS"

typedef struct lw6nod_info_s
{
  // these never change
  void *mutex;
  int64_t creation_timestamp;
  char *id;
  char *url;
  char *title;
  char *description;
  int bench;
  int idle_screenshot_size;
  void *idle_screenshot_data;
  // these change during game
  char *level;
  int required;
  int limit;
  int colors;
  int nodes;
  int cursors;
  int game_screenshot_size;
  void *game_screenshot_data;
  // these are updated by oob threads
  lw6sys_list_t *discovered_nodes;
  lw6sys_list_t *verified_nodes;
} lw6nod_info_t;

/*
  in info.c 
*/
extern lw6nod_info_t *lw6nod_info_new (u_int64_t id, char *url, char *title,
				       char *descroption, int bench,
				       int idle_screenshot_size,
				       void *idle_screenshot_data);
extern void lw6nod_info_free (lw6nod_info_t * info);
extern int lw6nod_info_lock (lw6nod_info_t * info);
extern int lw6nod_info_unlock (lw6nod_info_t * info);
extern void lw6nod_info_idle (lw6nod_info_t * info);
extern int lw6nod_info_update (lw6nod_info_t * info, char *level,
			       int required,
			       int limit,
			       int colors,
			       int nodes,
			       int cursors,
			       int game_screenshot_size,
			       void *game_screenshot_data);
extern int lw6nod_info_add_discovered_node (lw6nod_info_t * info,
					    char *public_url);
extern char *lw6nod_info_pop_discovered_node (lw6nod_info_t * info);
extern int lw6nod_info_set_verified_nodes (lw6nod_info_t * info,
					   lw6sys_list_t * list);
extern void lw6nod_info_map_verified_nodes (lw6nod_info_t * info,
					    lw6sys_list_callback_func_t func,
					    void *func_data);

/* nod-test.c */
extern int lw6nod_test (int mode);

#endif
