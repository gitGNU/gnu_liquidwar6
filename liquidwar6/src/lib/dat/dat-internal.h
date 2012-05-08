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

#ifndef LIQUIDWAR6DAT_INTERNAL_H
#define LIQUIDWAR6DAT_INTERNAL_H

#include "dat.h"

/*
 * This limit does not mean we can't store longer lines,
 * only if longer they'll be allocated dynamically.
 */
#define _LW6DAT_ATOM_STATIC_SIZE 99
/*
 * If longer than this, messages will be cut in slices
 * Note that this isn't an absolute size, atoms can store
 * a little more informations, for instance the header
 * will be added and is not included in this.
 */
#define _LW6DAT_ATOM_MAX_SIZE 1199
/*
 * This must be able to store the "serial i n seq from" 
 * header.
 */
#define _LW6DAT_HEADER_MAX_SIZE 119
/*
 * This makes blocks of approx. 250kb
 */
#define _LW6DAT_NB_ATOMS_PER_BLOCK 2000
/*
 * This is a fairly big limit, 500 blocks of 2000 atoms
 * is about 1000000 blocks. Up to 100 megs of data even
 * without requiring to use the extra "if_longer" pointer.
 * In extreme cases, could eat up to 4 gigs of RAM.
 */
#define _LW6DAT_MAX_NB_BLOCKS 500
#define _LW6DAT_MAX_NB_ATOMS (_LW6DAT_MAX_NB_BLOCKS*_LW6DAT_NB_ATOMS_PER_BLOCK)
/*
 * Local node messages are in the first stack
 */
#define _LW6DAT_LOCAL_NODE_INDEX 0
/*
 * Start at 1
 */
#define _LW6DAT_SERIAL_START 1
#define _LW6DAT_SEQ_START 0LL

#define _LW6DAT_SERIAL_INVALID 0
#define _LW6DAT_SEQ_INVALID -1LL

/*
 * Constant used to compensate for some possible overflow error
 */
#define _LW6DAT_GET_FACTOR 100
/*
 * Limit how far we can try and go back/forward in history
 */
#define _LW6DAT_BLOCK_DELTA_MAX (_LW6DAT_MAX_NB_BLOCKS*(_LW6DAT_GET_FACTOR-(_LW6DAT_GET_FACTOR/10)))

/*
 * This must match LW6DAT_MAX_NB_STACKS
 */
#define _LW6DAT_FLAG_ALL 0x3fffffff
#define _LW6DAT_FLAG_LOCAL 0x00000001
#define _LW6DAT_FLAG_REMOTE 0x3ffffffe

typedef struct _lw6dat_atom_s
{
  int not_null;
  int send_flag;
  int sent_status;
  int update_msg_called;
  int serial;
  int order_i;
  int order_n;
  int64_t seq;
  char full_str_if_short[_LW6DAT_ATOM_STATIC_SIZE + 1];
  char *full_str_if_longer;
  int seq_from_cmd_str_offset;
  int cmd_str_offset;
  char *recreated_cmd_cache;
} _lw6dat_atom_t;

typedef struct _lw6dat_block_s
{
  int serial_0;
  int serial_n_1;
  // no nb_atoms, there can be "holes" in the array
  _lw6dat_atom_t atoms[_LW6DAT_NB_ATOMS_PER_BLOCK];
} _lw6dat_block_t;

typedef struct _lw6dat_stack_s
{
  u_int64_t node_id;
  const char *node_id_str;
  int serial_0;
  int serial_n_1;
  int serial_min;
  int serial_max;
  int serial_draft;
  int serial_reference;
  int serial_miss_min;
  int serial_miss_max;
  int serial_min_to_send[LW6DAT_MAX_NB_STACKS];
  // no nb_blocks, there can be "holes" in the array
  _lw6dat_block_t *blocks[_LW6DAT_MAX_NB_BLOCKS];
} _lw6dat_stack_t;

typedef struct _lw6dat_warehouse_s
{
  int dummy;			// same as in lw6dat_warehouse_t
  _lw6dat_stack_t stacks[LW6DAT_MAX_NB_STACKS];
} _lw6dat_warehouse_t;

/* dat-atom.c */
extern void _lw6dat_atom_zero (_lw6dat_atom_t * atom);
extern void _lw6dat_atom_clear (_lw6dat_atom_t * atom);
extern int _lw6dat_atom_set_full_str (_lw6dat_atom_t * atom,
				      const char *full_str);
extern char *_lw6dat_atom_get_full_str (_lw6dat_atom_t * atom);
extern int _lw6dat_atom_parse_serial_i_n_seq_from_cmd (int *serial,
						       int *order_i,
						       int *order_n,
						       int64_t * seq,
						       u_int64_t *
						       logical_from,
						       int
						       *seq_from_cmd_str_offset,
						       int *cmd_str_offset,
						       const char *full_str);

/* dat-block.c */
extern _lw6dat_block_t *_lw6dat_block_new (int serial_0);
extern void _lw6dat_block_free (_lw6dat_block_t * block);
extern int _lw6dat_block_put_atom (_lw6dat_block_t * block,
				   int serial,
				   int order_i, int order_n, int64_t seq,
				   const char *full_str,
				   int seq_from_cmd_str_offset,
				   int cmd_str_offset, int send_flag);
extern _lw6dat_atom_t *_lw6dat_block_get_atom (_lw6dat_block_t * block,
					       int serial);

static inline int
_lw6dat_block_get_atom_index (_lw6dat_block_t * block, int serial)
{
  return (serial >= block->serial_0
	  && serial <= block->serial_n_1) ? serial - block->serial_0 : -1;
}

/* dat-flag.c */
inline static int
_lw6dat_flag (int index)
{
  return (1 << index);
};

inline static int
_lw6dat_not_flag (int index)
{
  return (_LW6DAT_FLAG_ALL ^ _lw6dat_flag (index));
};

/* dat-stack.c */
extern void _lw6dat_stack_zero (_lw6dat_stack_t * stack);
extern void _lw6dat_stack_clear (_lw6dat_stack_t * stack);
extern void _lw6dat_stack_purge (_lw6dat_stack_t * stack);
extern int _lw6dat_stack_init (_lw6dat_stack_t * stack, u_int64_t node_id,
			       int serial_0);
extern int _lw6dat_stack_get_serial (_lw6dat_stack_t * stack);
extern int _lw6dat_stack_put_atom (_lw6dat_stack_t * stack,
				   int serial,
				   int order_i, int order_n, int64_t seq,
				   const char *full_str,
				   int seq_from_cmd_str_offset,
				   int cmd_str_offset, int send_flag);
extern int _lw6dat_stack_put_atom_str (_lw6dat_stack_t * stack,
				       const char *full_str, int send_flag);
extern _lw6dat_atom_t *_lw6dat_stack_get_atom (_lw6dat_stack_t * stack,
					       int serial);
extern int _lw6dat_stack_put_msg (_lw6dat_stack_t * stack, const char *msg,
				  int send_flag);
extern int _lw6dat_stack_calc_serial_draft_and_reference (_lw6dat_stack_t *
							  stack);
extern int64_t _lw6dat_stack_get_seq_min (_lw6dat_stack_t * stack);
extern int64_t _lw6dat_stack_get_seq_max (_lw6dat_stack_t * stack);
extern int64_t _lw6dat_stack_get_seq_draft (_lw6dat_stack_t * stack);
extern int64_t _lw6dat_stack_get_seq_reference (_lw6dat_stack_t * stack);
extern int _lw6dat_stack_seq2serial (_lw6dat_stack_t * stack, int64_t seq);
extern int64_t _lw6dat_stack_serial2seq (_lw6dat_stack_t * stack, int serial);
extern lw6sys_list_t *_lw6dat_stack_init_list ();
extern int _lw6dat_stack_update_msg_list_by_seq (_lw6dat_stack_t * stack,
						 lw6sys_list_t ** msg_list,
						 int64_t seq, int get_all,
						 int clear_recent);
extern int _lw6dat_stack_update_atom_str_list_by_serial (_lw6dat_stack_t *
							 stack,
							 lw6sys_list_t **
							 msg_list,
							 int serial);
extern int _lw6dat_stack_update_atom_str_list_not_sent (_lw6dat_stack_t *
							stack,
							lw6sys_list_t **
							msg_list,
							int target_index);

static inline int
_lw6dat_stack_get_block_index (_lw6dat_stack_t * stack, int serial)
{
  /*
   * Now we add a big number before dividing so that negative
   * numbers are still correct, else seqing gives false indexes.
   */
  return
    ((serial + _LW6DAT_GET_FACTOR * _LW6DAT_MAX_NB_ATOMS -
      stack->serial_0) / _LW6DAT_NB_ATOMS_PER_BLOCK) -
    (_LW6DAT_GET_FACTOR * _LW6DAT_MAX_NB_BLOCKS);
}

/* dat-warehouse.c */
extern int _lw6dat_warehouse_init (_lw6dat_warehouse_t * warehouse,
				   u_int64_t local_node_id);
extern _lw6dat_warehouse_t *_lw6dat_warehouse_new (u_int64_t local_node_id);
extern void _lw6dat_warehouse_free (_lw6dat_warehouse_t * warehouse);
extern void _lw6dat_warehouse_clear (_lw6dat_warehouse_t * warehouse);
extern void _lw6dat_warehouse_purge (_lw6dat_warehouse_t * warehouse);
extern int _lw6dat_warehouse_get_nb_nodes (_lw6dat_warehouse_t * warehouse);
extern u_int64_t _lw6dat_warehouse_get_local_id (_lw6dat_warehouse_t *
						 warehouse);
extern int _lw6dat_warehouse_get_local_serial (_lw6dat_warehouse_t *
					       warehouse);
extern int _lw6dat_warehouse_get_stack_index (_lw6dat_warehouse_t * warehouse,
					      u_int64_t node_id);
extern int _lw6dat_warehouse_register_node (_lw6dat_warehouse_t * warehouse,
					    u_int64_t node_id, int serial_0);
extern int _lw6dat_warehouse_is_node_registered (_lw6dat_warehouse_t *
						 warehouse,
						 u_int64_t node_id);
extern int _lw6dat_warehouse_put_atom (_lw6dat_warehouse_t * warehouse,
				       u_int64_t logical_from, int serial,
				       int order_i, int order_n, int64_t seq,
				       const char *full_str,
				       int seq_from_cmd_str_offset,
				       int cmd_str_offset);
extern int _lw6dat_warehouse_put_atom_str (_lw6dat_warehouse_t * warehouse,
					   u_int64_t logical_from,
					   const char *full_str);
extern int _lw6dat_warehouse_put_local_msg (_lw6dat_warehouse_t * warehouse,
					    const char *msg);
extern int
_lw6dat_warehouse_calc_serial_draft_and_reference (_lw6dat_warehouse_t *
						   warehouse);
extern int64_t _lw6dat_warehouse_get_seq_min (_lw6dat_warehouse_t *
					      warehouse);
extern int64_t _lw6dat_warehouse_get_seq_max (_lw6dat_warehouse_t *
					      warehouse);
extern int64_t _lw6dat_warehouse_get_seq_draft (_lw6dat_warehouse_t *
						warehouse);
extern int64_t _lw6dat_warehouse_get_seq_reference (_lw6dat_warehouse_t *
						    warehouse);
extern lw6sys_list_t
  * _lw6dat_warehouse_get_msg_list_by_seq (_lw6dat_warehouse_t * warehouse,
					   int64_t seq_min, int64_t seq_max,
					   int for_reference);
extern lw6sys_list_t
  * _lw6dat_warehouse_get_atom_str_list_not_sent (_lw6dat_warehouse_t *
						  warehouse,
						  u_int64_t logical_to);

#endif
