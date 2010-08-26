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

_lw6dat_warehouse_t *
_lw6dat_warehouse_new (u_int64_t local_node_id)
{
  _lw6dat_warehouse_t *warehouse;
  int ok = 0;

  warehouse =
    (_lw6dat_warehouse_t *) LW6SYS_CALLOC (sizeof (_lw6dat_warehouse_t));
  if (warehouse)
    {
      if (_lw6dat_stack_init
	  (&(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]), local_node_id,
	   _LW6DAT_SERIAL_START))
	{
	  ok = 1;
	}
    }
  if (warehouse && !ok)
    {
      _lw6dat_warehouse_free (warehouse);
      warehouse = NULL;
    }

  return warehouse;
}

/**
 * lw6dat_warehouse_new
 *
 * Creates a new warehouse object.
 *
 * Return value: new object, allocated dynamically
 */
lw6dat_warehouse_t *
lw6dat_warehouse_new (u_int64_t local_node_id)
{
  lw6dat_warehouse_t *warehouse;

  warehouse = (lw6dat_warehouse_t *) _lw6dat_warehouse_new (local_node_id);

  return warehouse;
}

void
_lw6dat_warehouse_free (_lw6dat_warehouse_t * warehouse)
{
  LW6SYS_FREE (warehouse);
}

/**
 * lw6dat_warehouse_free
 *
 * @warehouse: the object to free
 *
 * Frees a warehouse object.
 *
 * Return value: new object, allocated dynamically
 */
void
lw6dat_warehouse_free (lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_free ((_lw6dat_warehouse_t *) warehouse);
}
