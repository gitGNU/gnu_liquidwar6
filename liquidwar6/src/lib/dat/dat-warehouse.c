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
  int i;

  _lw6dat_warehouse_purge (warehouse);

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      _lw6dat_stack_clear (&(warehouse->stacks[i]));
    }

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

u_int64_t
_lw6dat_warehouse_get_local_id (_lw6dat_warehouse_t * warehouse)
{
  u_int64_t ret = 0;

  ret = warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX].node_id;

  return ret;
}

/**
 * lw6dat_warehouse_get_local_id
 *
 * @warehouse: the warehouse object to query.
 *
 * Returns the local id.
 *
 * Return value: 64-bit id.
 */
u_int64_t
lw6dat_warehouse_get_local_id (lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_local_id ((_lw6dat_warehouse_t *) warehouse);
}

int
_lw6dat_warehouse_get_local_serial (_lw6dat_warehouse_t * warehouse)
{
  int ret = 0;

  ret =
    _lw6dat_stack_get_serial (&(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]));

  return ret;
}

/**
 * lw6dat_warehouse_get_local_serial
 *
 * @warehouse: the warehouse object to query.
 *
 * Returns the latest (highest) serial number given for local node.
 *
 * Return value: integer, latest serial number
 */
int
lw6dat_warehouse_get_local_serial (lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_local_serial ((_lw6dat_warehouse_t *)
					     warehouse);
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
			    int serial, int order_i, int order_n, int seq,
			    char *text)
{
  int stack_index = -1;
  int send_flag = 0;
  int ret = 0;

  stack_index = _lw6dat_warehouse_get_stack_index (warehouse, logical_from);
  if (stack_index < 0)
    {
      stack_index =
	_lw6dat_warehouse_register_node (warehouse, logical_from, serial);
    }
  if (stack_index >= 0)
    {
      send_flag = _lw6dat_not_flag (stack_index);
      ret =
	_lw6dat_stack_put_atom (&(warehouse->stacks[stack_index]), serial,
				order_i, order_n, seq, text, send_flag);
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
				char *atom_str_serial_i_n_seq_from_cmd)
{
  int ret = 0;
  int serial = 0;
  int order_i = 0;
  int order_n = 0;
  int seq = 0;
  u_int64_t logical_from2 = 0L;
  char *cmd = NULL;

  /*
   * It's more efficient to use _lw6dat_warehouse_put_atom
   * than _lw6dat_stack_put_atom_str for we need to check if
   * logical ids match, and else we'd parse the message twice
   * in most cases.
   */
  if (_lw6dat_atom_parse_serial_i_n_seq_from_cmd
      (&serial, &order_i, &order_n, &seq, &logical_from2, &cmd,
       atom_str_serial_i_n_seq_from_cmd))
    {
      if (logical_from == logical_from2)
	{
	  ret =
	    _lw6dat_warehouse_put_atom (warehouse, logical_from,
					serial, order_i, order_n, seq, cmd);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("cant't put atom string, logical_from inconsistency logical_from=%"
		       LW6SYS_PRINTF_LL "x logical_from2=%" LW6SYS_PRINTF_LL
		       "x"), logical_from, logical_from2);
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_put_atom_str
 *
 * @warehouse: warehouse object to use
 * @logical_from: from who the message came from originally
 * @atom_str_serial_i_n_seq_from_cmd: message of the form serial i n seq from cmd
 *
 * Puts an atomic string in the object, this kind of string is
 * typically received on the network.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6dat_warehouse_put_atom_str (lw6dat_warehouse_t * warehouse,
			       u_int64_t logical_from,
			       char *atom_str_serial_i_n_seq_from_cmd)
{
  int ret = 0;

  ret =
    _lw6dat_warehouse_put_atom_str ((_lw6dat_warehouse_t *) warehouse,
				    logical_from,
				    atom_str_serial_i_n_seq_from_cmd);

  return ret;
}

int
_lw6dat_warehouse_put_local_msg (_lw6dat_warehouse_t * warehouse, char *msg)
{
  int ret = 0;

  ret =
    _lw6dat_stack_put_msg (&(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]),
			   msg, _LW6DAT_FLAG_LOCAL);

  return ret;
}

/**
 * lw6dat_warehouse_put_local_msg
 *
 * @warehouse: warehouse object to use
 * @logical_from: from who the message came from originally
 * @msg: message
 *
 * Puts a message in the object. The message will be splitted into
 * several atoms if needed, it can be arbitrary long.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6dat_warehouse_put_local_msg (lw6dat_warehouse_t * warehouse, char *msg)
{
  int ret = 0;

  ret =
    _lw6dat_warehouse_put_local_msg ((_lw6dat_warehouse_t *) warehouse, msg);

  return ret;
}

lw6sys_list_t *
_lw6dat_warehouse_get_msg_list_by_seq (_lw6dat_warehouse_t * warehouse,
				       int seq_min, int seq_max)
{
  lw6sys_list_t *ret = NULL;
  int stack_index = 0;
  int seq = 0;

  ret = _lw6dat_stack_init_list ();
  if (ret)
    {
      for (seq = seq_max; seq >= seq_min; --seq)
	{
	  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS;
	       ++stack_index)
	    {
	      if (warehouse->stacks[stack_index].node_id != 0)
		{
		  _lw6dat_stack_update_msg_list_by_seq (&
							(warehouse->stacks
							 [stack_index]), &ret,
							seq);
		}
	    }
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_msg_list_by_seq
 *
 * @warehouse: object to query
 * @seq_min: lowest sequence number (round or chat index)
 * @seq_max: highest sequence number (round or chat index)
 * 
 * Gets the list of messages for a given sequence (round or chat index),
 * polling all the nodes. The from and to boundaries are included.
 * 
 * Return value: a list of strings.
 */
lw6sys_list_t *
lw6dat_warehouse_get_msg_list_by_seq (lw6dat_warehouse_t * warehouse,
				      int seq_min, int seq_max)
{
  lw6sys_list_t *ret = NULL;

  ret =
    _lw6dat_warehouse_get_msg_list_by_seq ((_lw6dat_warehouse_t *) warehouse,
					   seq_min, seq_max);

  return ret;
}

lw6sys_list_t *
_lw6dat_warehouse_get_atom_str_list_not_sent (_lw6dat_warehouse_t *
					      warehouse, u_int64_t logical_to)
{
  lw6sys_list_t *ret = 0;
  int stack_index = 0;
  int target_index = 0;

  ret = _lw6dat_stack_init_list ();
  if (ret)
    {
      target_index =
	_lw6dat_warehouse_get_stack_index (warehouse, logical_to);
      if (target_index >= 0 && target_index < LW6DAT_MAX_NB_STACKS)
	{
	  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS;
	       ++stack_index)
	    {
	      if (warehouse->stacks[stack_index].node_id != 0)
		{
		  _lw6dat_stack_update_atom_str_list_not_sent (&
							       (warehouse->stacks
								[stack_index]),
							       &ret,
							       target_index);
		}
	    }
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_atom_str_list_not_sent
 *
 * @warehouse: object to query
 * @logical_to: the id of the node that we want to send data to
 *
 * Returns all the messages that were not sent for the given warehouse.
 *
 * Return value: a list of strings, containing atoms.
 */
lw6sys_list_t *
lw6dat_warehouse_get_atom_str_list_not_sent (lw6dat_warehouse_t *
					     warehouse, u_int64_t logical_to)
{
  lw6sys_list_t *ret = 0;

  ret =
    _lw6dat_warehouse_get_atom_str_list_not_sent ((_lw6dat_warehouse_t *)
						  warehouse, logical_to);

  return ret;
}
