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

#ifndef LIQUIDWAR6DAT_H
#define LIQUIDWAR6DAT_H

#include "../sys/sys.h"
#include "../glb/glb.h"
#include "../nod/nod.h"
#include "../cnx/cnx.h"
#include "../msg/msg.h"

/*
 * Must be greater than LW6MAP_MAX_NB_NODES
 * but still lower than 32.
 */
#define LW6DAT_MAX_NB_STACKS 30

/**
 * Contains all recent messages sent to peers, and received
 * from them, int fact this is a local database, think of it as
 * a giant array, containing all messages. This is usefull both
 * resend messages if one peer wants one and also to check wether
 * informations are consistent and nobody is cheating. This structure
 * is hiddent, casted to the real stuff internally if needed, so
 * that other parts of the code don't rely on specific implementation.
 */
typedef struct lw6dat_warehouse_s
{
  /// Dummy field, unused.
  int dummy;
} lw6dat_warehouse_t;

/**
 * Used to get informations about "what messages are missing",
 * The struct typically contains informations to send a MISS message
 * on the network, that is, who needs to resend the information, and
 * the message serial range. The informations "who needs it" need not
 * be stored as this is logically our local node.
 */
typedef struct lw6dat_miss_s
{
  /// Id of node which needs to resend the information.
  u_int64_t from_id;
  /// Minimum serial, included in the range
  int serial_min;
  /// Maximum serial, included in the range
  int serial_max;
} lw6dat_miss_t;

/* dat-miss.c */
extern lw6dat_miss_t *lw6dat_miss_new (u_int64_t from_id, int serial_min,
				       int serial_max);
extern void lw6dat_miss_free (lw6dat_miss_t * miss);

/* dat-warehouse.c */
extern int lw6dat_warehouse_init (lw6dat_warehouse_t * warehouse,
				  u_int64_t local_node_id);
extern lw6dat_warehouse_t *lw6dat_warehouse_new (u_int64_t local_node_id);
extern void lw6dat_warehouse_free (lw6dat_warehouse_t * warehouse);
extern void lw6dat_warehouse_clear (lw6dat_warehouse_t * warehouse);
extern void lw6dat_warehouse_purge (lw6dat_warehouse_t * warehouse);
extern int lw6dat_warehouse_get_nb_nodes (lw6dat_warehouse_t * warehouse);
extern u_int64_t lw6dat_warehouse_get_local_id (lw6dat_warehouse_t *
						warehouse);
extern int lw6dat_warehouse_get_local_serial (lw6dat_warehouse_t * warehouse);
extern int lw6dat_warehouse_register_node (lw6dat_warehouse_t * warehouse,
					   u_int64_t node_id, int serial_0);
extern int lw6dat_warehouse_is_node_registered (lw6dat_warehouse_t *
						warehouse, u_int64_t node_id);
extern int lw6dat_warehouse_put_atom_str (lw6dat_warehouse_t * warehouse,
					  u_int64_t logical_from,
					  const char *full_str);
extern int lw6dat_warehouse_put_local_msg (lw6dat_warehouse_t * warehouse,
					   const char *msg);
extern int
lw6dat_warehouse_calc_serial_draft_and_reference (lw6dat_warehouse_t *
						  warehouse);
extern int64_t lw6dat_warehouse_get_seq_min (lw6dat_warehouse_t * warehouse);
extern int64_t lw6dat_warehouse_get_seq_max (lw6dat_warehouse_t * warehouse);
extern int64_t lw6dat_warehouse_get_seq_draft (lw6dat_warehouse_t *
					       warehouse);
extern int64_t lw6dat_warehouse_get_seq_reference (lw6dat_warehouse_t *
						   warehouse);
extern lw6sys_list_t *lw6dat_warehouse_get_msg_list_by_seq (lw6dat_warehouse_t
							    * warehouse,
							    int64_t seq_min,
							    int64_t seq_max,
							    int
							    for_reference);
extern lw6sys_list_t
  * lw6dat_warehouse_get_atom_str_list_not_sent (lw6dat_warehouse_t *
						 warehouse,
						 u_int64_t logical_to);
extern lw6sys_list_t *lw6dat_warehouse_get_miss_list (lw6dat_warehouse_t *
						      warehouse);
extern void lw6dat_warehouse_miss_invalidate (lw6dat_warehouse_t * warehouse,
					      u_int64_t from_id,
					      u_int64_t to_id,
					      int serial_min, int serial_max);

/* dat-test.c */
extern int lw6dat_test (int mode);

#endif
