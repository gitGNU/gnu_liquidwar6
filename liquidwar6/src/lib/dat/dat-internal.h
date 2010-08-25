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

#ifndef LIQUIDWAR6DAT_INTERNAL_H
#define LIQUIDWAR6DAT_INTERNAL_H

#include "dat.h"

#include "../map/map.h"		// only needed for #defines

/*
 * This limit does not mean we can't store longer lines,
 * only if longer they'll be allocated dynamically.
 */
#define _LW6DAT_ATOM_STATIC_SIZE 99
/*
 * This makes blocks of approx. 200kb
 */
#define _LW6DAT_NB_ATOMS_PER_BLOCK 2000
/*
 * This is a fairly big limit, 2000 blocks of 5000 atoms
 * is about 1000000 blocks. Up to 100 megs of data even
 * without requiring to use the extra "if_longer" pointer.
 */
#define _LW6DAT_MAX_NB_BLOCKS 500

typedef struct _lw6dat_atom_s
{
  int not_null;
  int serial;
  int order_major;
  int order_minor;
  char text_if_short[_LW6DAT_ATOM_STATIC_SIZE + 1];
  char *text_if_longer;
} _lw6dat_atom_t;

typedef struct _lw6dat_block_s
{
  int serial_0;
  int serial_n_1;
  int nb_atoms;
  _lw6dat_atom_t *atoms;
} _lw6dat_block_t;

typedef struct _lw6dat_stack_s
{
  u_int64_t remote_node_id;
  int serial_0;
  int serial_n_1;
  int nb_blocks;
  _lw6dat_block_t *blocks[_LW6DAT_MAX_NB_BLOCKS];
} _lw6dat_stack_t;

typedef struct _lw6dat_warehouse_s
{
  int dummy;			// same as in lw6dat_warehouse_t
  int serial_0;
  int serial_n_1;
  _lw6dat_stack_t stacks[LW6MAP_MAX_NB_NODES];
} _lw6dat_warehouse_t;

/* dat-atom.c */
extern void _lw6dat_atom_zero (_lw6dat_atom_t * atom);
extern void _lw6dat_atom_clear (_lw6dat_atom_t * atom);
extern int _lw6dat_atom_set_text (_lw6dat_atom_t * atom, char *text);
extern char *_lw6dat_atom_get_text (_lw6dat_atom_t * atom);

/* dat-block.c */
extern void _lw6dat_block_zero (_lw6dat_block_t * block);
extern void _lw6dat_block_clear (_lw6dat_block_t * block);
static inline _lw6dat_atom_t *
_lw6dat_block_get_atom (_lw6dat_block_t * block, int serial)
{
  return (serial >= block->serial_0
	  && serial <=
	  block->serial_n_1) ? &(block->atoms[serial -
					      block->serial_0]) : NULL;
}

/* dat-stack.c */
extern void _lw6dat_stack_zero (_lw6dat_stack_t * stack);
extern void _lw6dat_stack_clear (_lw6dat_stack_t * stack);
static inline _lw6dat_atom_t *
_lw6dat_stack_get_atom (_lw6dat_stack_t * stack, int serial)
{
  int i = serial - stack->serial_0;

  return (serial >= stack->serial_0
	  && serial <=
	  stack->serial_n_1) ? &(stack->blocks[i /
					       _LW6DAT_NB_ATOMS_PER_BLOCK]->atoms
				 [i % _LW6DAT_NB_ATOMS_PER_BLOCK]) : NULL;
}

/* dat-warehouse.c */
extern _lw6dat_warehouse_t *_lw6dat_warehouse_new ();
extern void _lw6dat_warehouse_free (_lw6dat_warehouse_t * warehouse);

#endif
