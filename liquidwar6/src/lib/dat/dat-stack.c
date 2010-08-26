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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dat-internal.h"

void
_lw6dat_stack_zero (_lw6dat_stack_t * stack)
{
  memset (stack, 0, sizeof (_lw6dat_stack_t));
}

void
_lw6dat_stack_clear (_lw6dat_stack_t * stack)
{
  int i = 0;

  stack->node_id = 0;
  stack->serial_0 = 0;
  stack->serial_n_1 = -1;
  stack->nb_blocks = 0;
  for (i = 0; i < _LW6DAT_MAX_NB_BLOCKS; ++i)
    {
      if (stack->blocks[i])
	{
	  _lw6dat_block_free (stack->blocks[i]);
	  stack->blocks[i] = NULL;
	}
    }
}

int
_lw6dat_stack_init (_lw6dat_stack_t * stack, u_int64_t node_id, int serial_0)
{
  int ret = 0;

  if (lw6sys_check_id (node_id) && serial_0 > 0)
    {
      _lw6dat_stack_clear (stack);
      stack->node_id = node_id;
      stack->serial_0 = serial_0;
      stack->serial_n_1 = serial_0 - 1;
      ret = 1;
    }

  return ret;
}
