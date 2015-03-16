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

#ifndef LIQUIDWAR6NOD_INTERNAL_H
#define LIQUIDWAR6NOD_INTERNAL_H

#include "nod.h"

#define _LW6NOD_HASH_SIZE 31

/* nod-community.c */
extern lw6nod_ref_info_t *_lw6nod_node_info_community_get_by_id (lw6sys_context_t * sys_context, lw6nod_info_t * node_info, u_int64_t id);
extern lw6nod_ref_info_t *_lw6nod_node_info_community_get_by_url (lw6sys_context_t * sys_context, lw6nod_info_t * node_info, const char *url);

/* nod-constinfo.c */
extern int _lw6nod_const_info_init (lw6sys_context_t * sys_context, lw6nod_const_info_t * const_info,
				    const char *program, const char *version,
				    const char *codename, int stamp,
				    u_int64_t id, const char *url,
				    const char *title,
				    const char *description,
				    const char *password, int bench, int open_relay, int uptime, int idle_screenshot_size, void *idle_screenshot_data);
extern void _lw6nod_const_info_reset (lw6sys_context_t * sys_context, lw6nod_const_info_t * const_info);

/* nod-dyninfo.c */
extern void _lw6nod_dyn_info_reset (lw6sys_context_t * sys_context, lw6nod_dyn_info_t * dyn_info);
extern int _lw6nod_dyn_info_update (lw6sys_context_t * sys_context, lw6nod_dyn_info_t * dyn_info,
				    u_int64_t community_id, int round,
				    const char *level, int required_bench,
				    int nb_colors, int max_nb_colors,
				    int nb_cursors, int max_nb_cursors, int nb_nodes, int max_nb_nodes, int game_screenshot_size, void *game_screenshot_data);

/* nod-refinfo.c */
extern void _lw6nod_ref_info_reset (lw6sys_context_t * sys_context, lw6nod_ref_info_t * ref_info);
extern int _lw6nod_ref_info_update (lw6sys_context_t * sys_context, lw6nod_ref_info_t * ref_info, u_int64_t id, const char *url);

#endif
