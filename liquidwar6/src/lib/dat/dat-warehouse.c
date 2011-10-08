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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dat-internal.h"

_lw6dat_warehouse_t *
_lw6dat_warehouse_new (u_int64_t local_node_id)
{
  _lw6dat_warehouse_t *warehouse;
  int ok = 0;
  int stack_index = 0;

  warehouse =
    (_lw6dat_warehouse_t *) LW6SYS_CALLOC (sizeof (_lw6dat_warehouse_t));
  if (warehouse)
    {
      for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
	{
	  if (stack_index == _LW6DAT_LOCAL_NODE_INDEX)
	    {
	      if (_lw6dat_stack_init
		  (&(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]),
		   local_node_id, _LW6DAT_SERIAL_START))
		{
		  ok = 1;
		}
	    }
	  else
	    {
	      _lw6dat_stack_clear (&(warehouse->stacks[stack_index]));
	    }
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
  _lw6dat_warehouse_purge (warehouse);
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

void
_lw6dat_warehouse_purge (_lw6dat_warehouse_t * warehouse)
{
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      _lw6dat_stack_purge (&(warehouse->stacks[i]));
    }
}

/**
 * lw6dat_warehouse_purge
 *
 * @warehouse: the object to purge
 *
 * Purges a warehouse object.
 *
 * Return value: new object, allocated dynamically
 */
void
lw6dat_warehouse_purge (lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_purge ((_lw6dat_warehouse_t *) warehouse);
}

int
_lw6dat_warehouse_get_stack_index (_lw6dat_warehouse_t * warehouse,
				   u_int64_t node_id)
{
  int ret = -1;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS && ret < 0; ++i)
    {
      if (warehouse->stacks[i].node_id == node_id)
	{
	  ret = i;
	}
    }

  return ret;
}

int
_lw6dat_warehouse_get_nb_nodes (_lw6dat_warehouse_t * warehouse)
{
  int ret = 0;
  int stack_index;

  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
    {
      if (warehouse->stacks[stack_index].node_id != 0)
	{
	  ++ret;
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_nb_nodes
 *
 * @warehouse: the warehouse object to query.
 *
 * Tells how many nodes are registered in the object.
 *
 * Return value: integer, number of nodes
 */
int
lw6dat_warehouse_get_nb_nodes (lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_nb_nodes ((_lw6dat_warehouse_t *) warehouse);
}

int
_lw6dat_warehouse_register_node (_lw6dat_warehouse_t * warehouse,
				 u_int64_t node_id, int serial_0)
{
  int ret = -1;
  int stack_index;

  ret = _lw6dat_warehouse_get_stack_index (warehouse, node_id);
  if (ret < 0)
    {
      for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS && ret < 0;
	   ++stack_index)
	{
	  if (warehouse->stacks[stack_index].node_id == 0)
	    {
	      if (_lw6dat_stack_init
		  (&(warehouse->stacks[stack_index]), node_id, serial_0))
		{
		  ret = stack_index;
		}
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("registering already registered node"));
    }

  return ret;
}

int
_lw6dat_warehouse_put_atom (_lw6dat_warehouse_t * warehouse,
			    u_int64_t logical_from,
			    int serial, int order_i, int order_n, char *text)
{
  int stack_index;
  int ret = 0;

  stack_index = _lw6dat_warehouse_get_stack_index (warehouse, logical_from);
  if (stack_index < 0)
    {
      stack_index =
	_lw6dat_warehouse_register_node (warehouse, logical_from, serial);
    }
  if (stack_index >= 0)
    {
      ret =
	_lw6dat_stack_put_atom (&(warehouse->stacks[stack_index]), serial,
				order_i, order_n, text);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("warehouse is full, too many stacks"));
    }

  return ret;
}

int
_lw6dat_warehouse_put_atom_str (_lw6dat_warehouse_t * warehouse,
				u_int64_t logical_from,
				char *atom_str_serial_i_n_msg)
{
  int ret = 0;
  char *next = atom_str_serial_i_n_msg;
  int serial = 0;
  int i = 0;
  int n = 0;

  if (lw6msg_word_first_int_ge0 (&serial, &next, next))
    {
      if (lw6msg_word_first_int_ge0 (&i, &next, next))
	{
	  if (lw6msg_word_first_int_gt0 (&n, &next, next))
	    {
	      ret =
		_lw6dat_warehouse_put_atom (warehouse, logical_from, serial,
					    i, n, next);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("bad value for n in atom \"%s\""),
			  atom_str_serial_i_n_msg);
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad value for i in atom \"%s\""),
		      atom_str_serial_i_n_msg);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("bad value for serial in atom \"%s\""),
		  atom_str_serial_i_n_msg);
    }

  return ret;
}

/**
 * lw6dat_warehouse_put_atom_str
 *
 * @warehouse: warehouse object to use
 * @logical_from: from who the message came from originally
 * @atom_str_serial_i_n_msg: message of the form serial i n msg
 *
 * Puts an atomic string in the object, this kind of string is
 * typically received on the network.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6dat_warehouse_put_atom_str (lw6dat_warehouse_t * warehouse,
			       u_int64_t logical_from,
			       char *atom_str_serial_i_n_msg)
{
  int ret = 0;

  ret =
    _lw6dat_warehouse_put_atom_str ((_lw6dat_warehouse_t *) warehouse,
				    logical_from, atom_str_serial_i_n_msg);

  return ret;
}
