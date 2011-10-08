/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#define _LW6DAT_FLAG_ALL 0xffffffff

typedef struct _lw6dat_atom_s
{
  int not_null;
  int send_flag;
  int sent_status;
  int serial;
  int order_i;
  int order_n;
  char text_if_short[_LW6DAT_ATOM_STATIC_SIZE + 1];
  char *text_if_longer;
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
  int serial_0;
  int serial_n_1;
  // no nb_blocks, there can be "holes" in the array
  _lw6dat_block_t *blocks[_LW6DAT_MAX_NB_BLOCKS];
} _lw6dat_stack_t;

typedef struct _lw6dat_warehouse_s
{
  int dummy;			// same as in lw6dat_warehouse_t
  int serial_0;
  int serial_n_1;
  _lw6dat_stack_t stacks[LW6DAT_MAX_NB_STACKS];
} _lw6dat_warehouse_t;

/* dat-atom.c */
extern void _lw6dat_atom_zero (_lw6dat_atom_t * atom);
extern void _lw6dat_atom_clear (_lw6dat_atom_t * atom);
extern int _lw6dat_atom_set_text (_lw6dat_atom_t * atom, char *text);
extern char *_lw6dat_atom_get_text (_lw6dat_atom_t * atom);

/* dat-block.c */
extern _lw6dat_block_t *_lw6dat_block_new (int serial_0);
extern void _lw6dat_block_free (_lw6dat_block_t * block);
extern int _lw6dat_block_put_atom (_lw6dat_block_t * block,
				   int serial,
				   int order_i, int order_n, char *text);
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

/* dat-stack.c */
extern void _lw6dat_stack_zero (_lw6dat_stack_t * stack);
extern void _lw6dat_stack_clear (_lw6dat_stack_t * stack);
extern void _lw6dat_stack_purge (_lw6dat_stack_t * stack);
extern int _lw6dat_stack_init (_lw6dat_stack_t * stack, u_int64_t node_id,
			       int serial_0);
extern int _lw6dat_stack_put_atom (_lw6dat_stack_t * stack,
				   int serial,
				   int order_i, int order_n, char *text);
extern _lw6dat_atom_t *_lw6dat_stack_get_atom (_lw6dat_stack_t * stack,
					       int serial);
static inline int
_lw6dat_stack_get_block_index (_lw6dat_stack_t * stack, int serial)
{
  /*
   * Now we add a big number before dividing so that negative
   * numbers are still correct, else rounding gives false indexes.
   */
  return
    ((serial + _LW6DAT_MAX_NB_ATOMS -
      stack->serial_0) / _LW6DAT_NB_ATOMS_PER_BLOCK) - _LW6DAT_MAX_NB_BLOCKS;
}

/* dat-warehouse.c */
extern _lw6dat_warehouse_t *_lw6dat_warehouse_new (u_int64_t local_node_id);
extern void _lw6dat_warehouse_free (_lw6dat_warehouse_t * warehouse);
extern void _lw6dat_warehouse_purge (_lw6dat_warehouse_t * warehouse);
extern int _lw6dat_warehouse_get_nb_nodes (_lw6dat_warehouse_t * warehouse);
extern int _lw6dat_warehouse_get_stack_index (_lw6dat_warehouse_t * warehouse,
					      u_int64_t node_id);
extern int _lw6dat_warehouse_register_node (_lw6dat_warehouse_t * warehouse,
					    u_int64_t node_id, int serial_0);
extern int _lw6dat_warehouse_put_atom (_lw6dat_warehouse_t * warehouse,
				       u_int64_t logical_from, int serial,
				       int order_i, int order_n, char *text);
extern int _lw6dat_warehouse_put_atom_str (_lw6dat_warehouse_t * warehouse,
					   u_int64_t logical_from,
					   char *atom_str_serial_i_n_msg);
extern int _lw6dat_warehouse_put_msg (_lw6dat_warehouse_t * warehouse,
				      u_int64_t logical_from, char *msg);

#endif
