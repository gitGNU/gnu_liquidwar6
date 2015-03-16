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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dat-internal.h"

#define _SEQ_RANGE_CHECK_LIMIT 1000

int
_lw6dat_warehouse_init (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t local_node_id, int64_t seq_0)
{
  int ok = 0;
  int stack_index = 0;

  memset (warehouse, 0, sizeof (_lw6dat_warehouse_t));
  warehouse->local_seq_last = lw6sys_llmax (seq_0, LW6DAT_SEQ_START);
  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
    {
      if (stack_index == _LW6DAT_LOCAL_NODE_INDEX)
	{
	  if (_lw6dat_stack_init (sys_context, &(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]), local_node_id, LW6DAT_SERIAL_START, seq_0))
	    {
	      ok = 1;
	    }
	}
      else
	{
	  _lw6dat_stack_clear (sys_context, &(warehouse->stacks[stack_index]));
	}
    }

  return ok;
}

/**
 * lw6dat_warehouse_init
 *
 * Initializes a warehouse object. Won't free anything, will just
 * erase values if they're here
 *
 * @sys_context: global system context
 * @warehouse: object to initialize
 * @local_node_id: id of local node, used to handle local messages
 * @seq_0: initial seq number
 *
 * Return value: new object, allocated dynamically
 */
int
lw6dat_warehouse_init (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t local_node_id, int64_t seq_0)
{
  return _lw6dat_warehouse_init (sys_context, (_lw6dat_warehouse_t *) warehouse, local_node_id, seq_0);
}

_lw6dat_warehouse_t *
_lw6dat_warehouse_new (lw6sys_context_t * sys_context, u_int64_t local_node_id, int64_t seq_0)
{
  _lw6dat_warehouse_t *warehouse;
  int ok = 0;

  warehouse = (_lw6dat_warehouse_t *) LW6SYS_MALLOC (sys_context, sizeof (_lw6dat_warehouse_t));
  if (warehouse)
    {
      ok = _lw6dat_warehouse_init (sys_context, warehouse, local_node_id, seq_0);
    }
  if (warehouse && !ok)
    {
      _lw6dat_warehouse_free (sys_context, warehouse);
      warehouse = NULL;
    }

  return warehouse;
}

/**
 * lw6dat_warehouse_new
 *
 * Creates a new warehouse object.
 *
 * @sys_context: global system context
 * @local_node_id: id of local node, used to handle local messages
 * @seq_0: initial seq number
 *
 * Return value: new object, allocated dynamically
 */
lw6dat_warehouse_t *
lw6dat_warehouse_new (lw6sys_context_t * sys_context, u_int64_t local_node_id, int64_t seq_0)
{
  lw6dat_warehouse_t *warehouse;

  warehouse = (lw6dat_warehouse_t *) _lw6dat_warehouse_new (sys_context, local_node_id, seq_0);

  return warehouse;
}

void
_lw6dat_warehouse_free (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_clear (sys_context, warehouse);

  LW6SYS_FREE (sys_context, warehouse);
}

/**
 * lw6dat_warehouse_free
 *
 * @sys_context: global system context
 * @warehouse: the object to free
 *
 * Frees a warehouse object.
 *
 * Return value: new object, allocated dynamically
 */
void
lw6dat_warehouse_free (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_free (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

void
_lw6dat_warehouse_clear (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int i;

  warehouse->local_seq_last = LW6DAT_SEQ_START;
  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id != 0)
	{
	  _lw6dat_stack_clear (sys_context, &(warehouse->stacks[i]));
	}
    }
}

/**
 * lw6dat_warehouse_clear
 *
 * @sys_context: global system context
 * @warehouse: the object to clear
 *
 * Clears a warehouse object. Clears means emptying everything
 * and resetting the current seq_id to the minimal/start value.
 *
 * Return value: none.
 */
void
lw6dat_warehouse_clear (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_clear (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

void
_lw6dat_warehouse_purge (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id != 0)
	{
	  _lw6dat_stack_purge (sys_context, &(warehouse->stacks[i]));
	}
    }
}

/**
 * lw6dat_warehouse_purge
 *
 * @sys_context: global system context
 * @warehouse: the object to purge
 *
 * Purges a warehouse object. Purges means emptying everything
 * but keeping the current seq_id and the nodes list unchanged.
 *
 * Return value: none.
 */
void
lw6dat_warehouse_purge (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_purge (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

int
_lw6dat_warehouse_get_stack_index (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t node_id)
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
_lw6dat_warehouse_get_nb_nodes (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
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
 * @sys_context: global system context
 * @warehouse: the warehouse object to query.
 *
 * Tells how many nodes are registered in the object.
 *
 * Return value: integer, number of nodes
 */
int
lw6dat_warehouse_get_nb_nodes (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_nb_nodes (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

u_int64_t
_lw6dat_warehouse_get_local_id (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  u_int64_t ret = 0;

  ret = warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX].node_id;

  return ret;
}

/**
 * lw6dat_warehouse_get_local_id
 *
 * @sys_context: global system context
 * @warehouse: the warehouse object to query.
 *
 * Returns the local id.
 *
 * Return value: 64-bit id.
 */
u_int64_t
lw6dat_warehouse_get_local_id (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_local_id (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

int
_lw6dat_warehouse_get_local_serial (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int ret = 0;

  ret = _lw6dat_stack_get_serial (sys_context, &(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]));

  return ret;
}

/**
 * lw6dat_warehouse_get_local_serial
 *
 * @sys_context: global system context
 * @warehouse: the warehouse object to query.
 *
 * Returns the latest (highest) serial number given for local node.
 *
 * Return value: integer, latest serial number
 */
int
lw6dat_warehouse_get_local_serial (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_local_serial (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

int64_t
_lw6dat_warehouse_get_local_seq_0 (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  /*
     return warehouse->
     stacks[_LW6DAT_LOCAL_NODE_INDEX].seq_0s[_LW6DAT_LOCAL_NODE_INDEX];
   */
  return warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX].seq_0;
}

/**
 * lw6dat_warehouse_get_local_seq_0
 *
 * @sys_context: global system context
 * @warehouse: the warehouse object to query
 *
 * Gives the warehouse seq_0 number, any seq below does not make sense.
 *
 * Return value: long integer.
 */
int64_t
lw6dat_warehouse_get_local_seq_0 (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_local_seq_0 (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

void
_lw6dat_warehouse_set_local_seq_0 (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, int64_t seq_0)
{
  int stack_index = 0;

  warehouse->local_seq_last = lw6sys_llmax (seq_0, warehouse->local_seq_last);
  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
    {
      warehouse->stacks[stack_index].seq_0s[_LW6DAT_LOCAL_NODE_INDEX] = seq_0;
    }
}

/**
 * lw6dat_warehouse_set_local_seq_0
 *
 * @sys_context: global system context
 * @warehouse: the warehouse object to modify
 * @seq_0: the new seq value
 *
 * Set the warehouse seq_0 number, any seq below does not make sense.
 *
 * Return value: none.
 */
void
lw6dat_warehouse_set_local_seq_0 (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, int64_t seq_0)
{
  _lw6dat_warehouse_set_local_seq_0 (sys_context, (_lw6dat_warehouse_t *) warehouse, seq_0);
}

int64_t
_lw6dat_warehouse_get_local_seq_last (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  return warehouse->local_seq_last;
}

/**
 * lw6dat_warehouse_get_local_seq_last
 *
 * @sys_context: global system context
 * @warehouse: the warehouse object to query
 *
 * Gives the warehouse seq_last number, this is the seq that corresponds
 * to the last local message put in this warehouse. This is usefull
 * to get the last seq used and, for instance, put a NOP message just
 * for keepalive purposes.
 *
 * Return value: long integer.
 */
int64_t
lw6dat_warehouse_get_local_seq_last (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  return _lw6dat_warehouse_get_local_seq_last (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

int
_lw6dat_warehouse_register_node (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t node_id, int serial_0, int64_t seq_0)
{
  int ret = -1;
  int stack_index = -1;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
	      _x_ ("registering %" LW6SYS_PRINTF_LL "x serial_0=%d seq_0=%" LW6SYS_PRINTF_LL "d"), (long long) node_id, serial_0, (long long) seq_0);

  serial_0 = lw6sys_imax (serial_0, LW6DAT_SERIAL_START);
  ret = _lw6dat_warehouse_get_stack_index (sys_context, warehouse, node_id);
  if (ret < 0)
    {
      for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS && ret < 0; ++stack_index)
	{
	  if (warehouse->stacks[stack_index].node_id == 0)
	    {
	      if (_lw6dat_stack_init (sys_context, &(warehouse->stacks[stack_index]), node_id, serial_0, seq_0))
		{
		  ret = stack_index;
		}
	    }
	}
    }
  else if (ret >= 0 && ret < LW6DAT_MAX_NB_STACKS)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("registering already registered node"));
      _lw6dat_stack_shift (sys_context, &(warehouse->stacks[ret]), serial_0, seq_0);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad stack index %d"), ret);
    }

  if (ret >= 0 && ret < LW6DAT_MAX_NB_STACKS)
    {
      /*
       * Now ret is valid but in the case the node is already registered
       * we perform another check to make sure the udpdate makes sense,
       * we don't want seq_0 and serial_min_to_send to decrease. What
       * is likely to happen is that those are initialized when the
       * host is listed as a simple ID, then updated whenever a real
       * contact is established.
       */
      for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
	{
	  /*
	   * For affectations below, use the seq_0 llmax
	   */
	  /*
	   * Tell all stacks that for this node, it's useless to consider
	   * messages that have a seq that is older than seq_0
	   */
	  warehouse->stacks[stack_index].seq_0s[ret] = lw6sys_llmax (warehouse->stacks[stack_index].seq_0s[ret], seq_0);
	  /*
	   * Tell this stack about all other nodes limits, this is done
	   * by copying informations from local stack.
	   */
	  warehouse->stacks[ret].seq_0s[stack_index] =
	    lw6sys_llmax (warehouse->stacks[ret].seq_0s[stack_index], warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX].seq_0s[stack_index]);
	  /*
	   * Not sure how the line below is totally required, might
	   * need some tuning, but at least it shouldn't harm from
	   * a functionnal point of view, only performance is concerned.
	   * The risk is to re-parse the whole stack for nothing.
	   */
	  warehouse->stacks[stack_index].serial_min_to_send[ret] = lw6sys_imin (warehouse->stacks[stack_index].serial_min_to_send[ret], serial_0);
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_register_node
 *
 * @sys_context: global system context
 * @warehouse: object to update
 * @node_id: id of node to register
 * @serial_0: serial number of first message
 * @seq_0: initial seq number
 *
 * Registers a node, in practice this is automatically done when receiving
 * a data message but it might be interesting to do it elsewhere and force it.
 *
 * Return value: the stack index of the registered node, <0 is invalid.
 */
int
lw6dat_warehouse_register_node (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t node_id, int serial_0, int64_t seq_0)
{
  return _lw6dat_warehouse_register_node (sys_context, (_lw6dat_warehouse_t *) warehouse, node_id, serial_0, seq_0);
}

int
_lw6dat_warehouse_is_node_registered (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t node_id)
{
  int ret = 0;

  ret = (_lw6dat_warehouse_get_stack_index (sys_context, warehouse, node_id) >= 0);

  return ret;
}

/**
 * lw6dat_warehouse_is_node_registered
 *
 * @sys_context: global system context
 * @warehouse: object to update
 * @node_id: id of node to register
 *
 * Tells wether a node is registered or not in our list.
 *
 * Return value: 1 if registered, 0 if not.
 */
int
lw6dat_warehouse_is_node_registered (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t node_id)
{
  return _lw6dat_warehouse_is_node_registered (sys_context, (_lw6dat_warehouse_t *) warehouse, node_id);
}

int
_lw6dat_warehouse_put_atom (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse,
			    u_int64_t logical_from,
			    int type, int serial, int order_i, int order_n, int64_t seq, const char *full_str, int seq_from_cmd_str_offset, int cmd_str_offset)
{
  int stack_index = -1;
  int send_flag = 0;
  int ret = 0;

  stack_index = _lw6dat_warehouse_get_stack_index (sys_context, warehouse, logical_from);
  /*
   * Previous versions of the code used to have a reg attribute
   * for messages to self-register nodes on the fly. This has been
   * superseded by the META mecanism.
   */

  if (stack_index >= 0)
    {
      send_flag = _lw6dat_not_flag (stack_index);
      ret =
	_lw6dat_stack_put_atom (sys_context, &(warehouse->stacks[stack_index]), type,
				serial, order_i, order_n, seq, full_str, seq_from_cmd_str_offset, cmd_str_offset, send_flag);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		  _x_ ("peer %" LW6SYS_PRINTF_LL "x is not registered in warehouse, rejecting message"), (long long) logical_from);
    }

  return ret;
}

int
_lw6dat_warehouse_put_atom_str (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t logical_from, const char *full_str)
{
  int ret = 0;
  int type = 0;
  int serial = 0;
  int order_i = 0;
  int order_n = 0;
  int64_t seq = 0;
  u_int64_t logical_from2 = 0L;
  int seq_from_cmd_str_offset = 0;
  int cmd_str_offset = 0;

  /*
   * It's more efficient to use _lw6dat_warehouse_put_atom
   * than _lw6dat_stack_put_atom_str for we need to check if
   * logical ids match, and else we'd parse the message twice
   * in most cases.
   */
  if (_lw6dat_atom_parse_from_cmd (sys_context, &type, &serial, &order_i, &order_n, &seq, &logical_from2, &seq_from_cmd_str_offset, &cmd_str_offset, full_str))
    {
      if (logical_from != logical_from2)
	{
	  /*
	   * Now, in practice, logical_from and logical_from2
	   * can be different, after, this is the whole thing
	   * about this dat-warehouse, it allows messages issued
	   * by B to be forwarded to C by A.
	   *
	   * One option would be to consider that messages
	   * that have the same logical_from and logical_from2
	   * are condidered "safer" than other messages as
	   * they are issued by the real owner. One could
	   * imagine that in case of conflict/difference
	   * this information is used. Yet to be implemented.
	   */
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_
		      ("unsure message logical_from=%"
		       LW6SYS_PRINTF_LL "x and logical_from2=%" LW6SYS_PRINTF_LL "x"), (long long) logical_from, (long long) logical_from2);
	}
      /*
       * Note that we operate on logical_from2, indeed this is
       * the real original writer of the message.
       */
      ret =
	_lw6dat_warehouse_put_atom (sys_context, warehouse, logical_from2, type, serial, order_i, order_n, seq, full_str, seq_from_cmd_str_offset,
				    cmd_str_offset);
    }

  return ret;
}

/**
 * lw6dat_warehouse_put_atom_str
 *
 * @sys_context: global system context
 * @warehouse: warehouse object to use
 * @logical_from: from who the message came from originally
 * @full_str: message of the form serial i n seq from cmd
 *
- * Puts an atomic string in the object, this kind of string is
 * typically received on the network.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6dat_warehouse_put_atom_str (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t logical_from, const char *full_str)
{
  int ret = 0;

  ret = _lw6dat_warehouse_put_atom_str (sys_context, (_lw6dat_warehouse_t *) warehouse, logical_from, full_str);

  return ret;
}

int
_lw6dat_warehouse_calc_serial_draft_and_reference (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int ret = 0;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  if (_lw6dat_stack_calc_serial_draft_and_reference (sys_context, &(warehouse->stacks[i])))
	    {
	      /*
	       * At least one stack has something interesting, so we consider
	       * that popping messages is likely to be of some interest.
	       */
	      ret = 1;
	    }
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_calc_serial_draft_and_reference
 *
 * @sys_context: global system context
 * @warehouse: object to work on
 *
 * The various @get_seq functions can perform slowly if
 * we don't pre-calculate the serial number of draft and reference
 * atoms. So this calculation is not within the functions themselves
 * but can be cached by using this function. Just call it and
 * after you might query the object for reference and draft info.
 *
 * Return value: 1 if some valid informations were found, else 0,
 * which means no interesting (complete) atoms where found, and
 * there's probably nothing to do yet.
 */
int
lw6dat_warehouse_calc_serial_draft_and_reference (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  int ret = 0;

  ret = _lw6dat_warehouse_calc_serial_draft_and_reference (sys_context, (_lw6dat_warehouse_t *) warehouse);

  return ret;
}

int
_lw6dat_warehouse_put_local_msg (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, const char *msg)
{
  int ret = 0;

  ret = _lw6dat_stack_put_msg (sys_context, &(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]), &(warehouse->local_seq_last), msg, _LW6DAT_FLAG_REMOTE);

  return ret;
}

/**
 * lw6dat_warehouse_put_local_msg
 *
 * @sys_context: global system context
 * @warehouse: warehouse object to use
 * @msg: message
 *
 * Puts a message in the object. The message will be splitted into
 * several atoms if needed, it can be arbitrary long.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6dat_warehouse_put_local_msg (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, const char *msg)
{
  int ret = 0;

  ret = _lw6dat_warehouse_put_local_msg (sys_context, (_lw6dat_warehouse_t *) warehouse, msg);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_min (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int64_t ret = LLONG_MAX;
  int64_t seq_min;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  seq_min = _lw6dat_stack_get_seq_min (sys_context, &(warehouse->stacks[i]));
	  if (seq_min > LW6DAT_SEQ_INVALID)
	    {
	      ret = lw6sys_llmin (ret, seq_min);
	    }
	}
    }
  if (ret == LLONG_MAX)
    {
      ret = LW6DAT_SEQ_INVALID;
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_min
 *
 * @sys_context: global system context
 * @warehouse: object to query
 *
 * Tells the lowest seq referenced in the warehouse. Does not
 * mean this is the lowest ever received, only we really have
 * no chances of going below that point, nothing is stored, either
 * complete or partial, below that.
 *
 * Return value: integer.
 */
int64_t
lw6dat_warehouse_get_seq_min (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0LL;

  ret = _lw6dat_warehouse_get_seq_min (sys_context, (_lw6dat_warehouse_t *) warehouse);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_max (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int64_t ret = LW6DAT_SEQ_INVALID;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  ret = lw6sys_llmax (ret, _lw6dat_stack_get_seq_max (sys_context, &(warehouse->stacks[i])));
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_max
 *
 * @sys_context: global system context
 * @warehouse: object to query
 *
 * Tells the highest seq referenced in the warehouse. Does not
 * mean an actual message can be built from it, only we've
 * got some traces of such a high seq.
 *
 * Return value: integer.
 */
int64_t
lw6dat_warehouse_get_seq_max (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0LL;

  ret = _lw6dat_warehouse_get_seq_max (sys_context, (_lw6dat_warehouse_t *) warehouse);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_draft (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0LL;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  ret = lw6sys_llmax (ret, _lw6dat_stack_get_seq_draft (sys_context, &(warehouse->stacks[i])));
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_draft
 *
 * @sys_context: global system context
 * @warehouse: object to query
 *
 * Tells the highest seq that can be considered as a valid draft.
 * This is not exactly the maximimum seq encountered, for here we
 * want at least one complete message and not just one chunk of
 * data (an atom) referring to a high seq, we want the complete
 * stuff. However there can be missing messages in between.
 *
 * Return value: integer.
 */
int64_t
lw6dat_warehouse_get_seq_draft (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0;

  ret = _lw6dat_warehouse_get_seq_draft (sys_context, (_lw6dat_warehouse_t *) warehouse);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_reference (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int64_t ret = LLONG_MAX;
  int64_t seq_reference = LW6DAT_SEQ_INVALID;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  seq_reference = _lw6dat_stack_get_seq_reference (sys_context, &(warehouse->stacks[i]));
	  if (seq_reference == LW6DAT_SEQ_INVALID)
	    {
	      /*
	       * If we could not find real messages with actual seqs within the
	       * stack, then consider seq_0 and substract 1, this way we still
	       * report some value which makes sense, while not jeopardizing the
	       * rest of the process (we could possibly return seqs as being
	       * completed while they aren't). This usually happens when a
	       * peer connects, at some point it has no messages yet, but
	       * we do know about its seq_0.
	       */
	      seq_reference = warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX].seq_0s[i] - 1LL;
	    }
	  ret = lw6sys_llmin (ret, seq_reference);
	}
    }
  if ((ret < LW6DAT_SEQ_INVALID) || (ret >= LLONG_MAX))
    {
      ret = LW6DAT_SEQ_INVALID;
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_reference
 *
 * @sys_context: global system context
 * @warehouse: object to query
 *
 * Tells the highest seq that can be considered as a reference.
 * Being considered as a reference means we received all messages
 * for this seq *and* at least one message from the following seq,
 * and this for every node involved. This being said, we're sure
 * to have the right information, nothing is missing.
 *
 * Return value: integer.
 */
int64_t
lw6dat_warehouse_get_seq_reference (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0;

  ret = _lw6dat_warehouse_get_seq_reference (sys_context, (_lw6dat_warehouse_t *) warehouse);

  return ret;
}

lw6sys_list_t *
_lw6dat_warehouse_get_msg_list_by_seq (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, int64_t seq_min, int64_t seq_max, int for_reference,
				       lw6sys_progress_t * progress)
{
  lw6sys_list_t *ret = NULL;
  int stack_index = 0;
  int64_t seq = 0;
  int tmp_worst_msg_i = 0;
  int tmp_worst_msg_n = 0;
  int main_worst_msg_i = 0;
  int main_worst_msg_n = 0;

  if (seq_max - seq_min > _SEQ_RANGE_CHECK_LIMIT)
    {
      int64_t warehouse_seq_min = 0LL;
      int64_t warehouse_seq_max = 0LL;

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("before range check seq_min=%" LW6SYS_PRINTF_LL "d seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq_min, (long long) seq_max);

      warehouse_seq_min = _lw6dat_warehouse_get_seq_min (sys_context, warehouse);
      warehouse_seq_max = _lw6dat_warehouse_get_seq_max (sys_context, warehouse);
      seq_min = lw6sys_llmax (warehouse_seq_min - 1, seq_min);
      seq_max = lw6sys_llmin (warehouse_seq_max + 1, seq_max);

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("after range check seq_min=%" LW6SYS_PRINTF_LL "d seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq_min, (long long) seq_max);
    }
  ret = _lw6dat_stack_init_list (sys_context);
  if (ret)
    {
      for (seq = seq_max; seq >= seq_min; --seq)
	{
	  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
	    {
	      if (warehouse->stacks[stack_index].node_id != 0)
		{
		  _lw6dat_stack_update_msg_list_by_seq (sys_context, &
							(warehouse->stacks
							 [stack_index]), &ret,
							seq, for_reference, for_reference && (seq == seq_max), &tmp_worst_msg_i, &tmp_worst_msg_n);
		  if (tmp_worst_msg_i * main_worst_msg_n <= main_worst_msg_i * tmp_worst_msg_n)
		    {
		      main_worst_msg_i = tmp_worst_msg_i;
		      main_worst_msg_n = tmp_worst_msg_n;
		    }
		}
	    }
	}
    }
  if (ret)
    {
      /*
       * Normally, pilot will sort those afterwards anyway, but just in case,
       * we sort them upstream. Besides, this eases up the tests, they would
       * show up the same order.
       */
      lw6sys_sort (sys_context, &ret, lw6msg_sort_str_by_seq_callback, NULL);
    }

  if (main_worst_msg_n > 0)
    {
      lw6sys_progress_update (sys_context, progress, 0, main_worst_msg_n, main_worst_msg_i);
    }
  else
    {
      lw6sys_progress_end (sys_context, progress);
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_msg_list_by_seq
 *
 * @sys_context: global system context
 * @warehouse: object to query
 * @seq_min: lowest sequence number (round or chat index)
 * @seq_max: highest sequence number (round or chat index)
 * @for_reference: set to 1 if this is for reference building else 0 for draft
 * @progress: progress indicator (read/write).
 *
 * Gets the list of messages for a given sequence (round or chat index),
 * polling all the nodes. The from and to boundaries are included.
 *
 * Return value: a list of strings.
 */
lw6sys_list_t *
lw6dat_warehouse_get_msg_list_by_seq (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, int64_t seq_min, int64_t seq_max, int for_reference,
				      lw6sys_progress_t * progress)
{
  lw6sys_list_t *ret = NULL;

  ret = _lw6dat_warehouse_get_msg_list_by_seq (sys_context, (_lw6dat_warehouse_t *) warehouse, seq_min, seq_max, for_reference, progress);

  return ret;
}

lw6sys_list_t *
_lw6dat_warehouse_get_atom_str_list_not_sent (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t logical_to)
{
  lw6sys_list_t *ret = 0;
  int stack_index = 0;
  int target_index = 0;

  ret = _lw6dat_stack_init_list (sys_context);
  if (ret)
    {
      if (logical_to != _lw6dat_warehouse_get_local_id (sys_context, warehouse))
	{
	  target_index = _lw6dat_warehouse_get_stack_index (sys_context, warehouse, logical_to);
	  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
	    {
	      if ((stack_index != target_index) && (warehouse->stacks[stack_index].node_id != 0))
		{
		  _lw6dat_stack_update_atom_str_list_not_sent (sys_context, &(warehouse->stacks[stack_index]), &ret, target_index);
		}
	    }
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_atom_str_list_not_sent
 *
 * @sys_context: global system context
 * @warehouse: object to query
 * @logical_to: the id of the node that we want to send data to
 *
 * Returns all the messages that were not sent for the given warehouse.
 *
 * Return value: a list of strings, containing atoms.
 */
lw6sys_list_t *
lw6dat_warehouse_get_atom_str_list_not_sent (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t logical_to)
{
  lw6sys_list_t *ret = NULL;

  ret = _lw6dat_warehouse_get_atom_str_list_not_sent (sys_context, (_lw6dat_warehouse_t *) warehouse, logical_to);

  return ret;
}

lw6sys_list_t *
_lw6dat_warehouse_get_miss_list (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, int max_range, lw6sys_progress_t * progress)
{
  lw6sys_list_t *ret = NULL;
  lw6dat_miss_t *miss = NULL;
  int i = 0;
  int tmp_worst_msg_i = 0;
  int tmp_worst_msg_n = 0;
  int main_worst_msg_i = 0;
  int main_worst_msg_n = 0;

  ret = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6dat_miss_free);
  if (ret)
    {
      for (i = 0; i < LW6DAT_MAX_NB_STACKS && ret; ++i)
	{
	  if (i != _LW6DAT_LOCAL_NODE_INDEX && warehouse->stacks[i].node_id)
	    {
	      miss = _lw6dat_stack_get_miss (sys_context, &(warehouse->stacks[i]), max_range, &tmp_worst_msg_i, &tmp_worst_msg_n);
	      if (miss)
		{
		  if (tmp_worst_msg_i * main_worst_msg_n <= main_worst_msg_i * tmp_worst_msg_n)
		    {
		      main_worst_msg_i = tmp_worst_msg_i;
		      main_worst_msg_n = tmp_worst_msg_n;
		    }
		  lw6sys_list_push_front (sys_context, &ret, miss);
		}
	    }
	}
    }

  if (main_worst_msg_n > 0)
    {
      lw6sys_progress_update (sys_context, progress, 0, main_worst_msg_n, main_worst_msg_i);
    }
  else
    {
      lw6sys_progress_end (sys_context, progress);
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_miss_list
 *
 * @sys_context: global system context
 * @warehouse: object to query
 * @max_range: max range of the returned list (-1 if none)
 * @progress: progress indicator, to show advancement to end-user
 *
 * Returns a list of @lw6dat_miss_t objects which contains informations about
 * the messages which need to be re-sent by peers. The function will always
 * return something, the list is not cleared if it's called several times,
 * so one should not poll this too often. Additionnally, the max_range parameter
 * can be used to limit the size of the returned ranges, to avoid querying for
 * too many messages at once.
 *
 * Return value: a list of pointers to @lw6dat_miss_t structs, NULL on failure.
 */
lw6sys_list_t *
lw6dat_warehouse_get_miss_list (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, int max_range, lw6sys_progress_t * progress)
{
  lw6sys_list_t *ret = NULL;

  ret = _lw6dat_warehouse_get_miss_list (sys_context, (_lw6dat_warehouse_t *) warehouse, max_range, progress);

  return ret;
}

void
_lw6dat_warehouse_miss_invalidate (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t from_id, u_int64_t to_id, int serial_min,
				   int serial_max)
{
  int stack_index = 0;
  int target_index = 0;

  target_index = _lw6dat_warehouse_get_stack_index (sys_context, warehouse, to_id);
  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
    {
      if ((stack_index != target_index) && (warehouse->stacks[stack_index].node_id != 0))
	{
	  _lw6dat_stack_miss_invalidate (sys_context, &(warehouse->stacks[stack_index]), target_index, serial_min, serial_max);
	}
    }
}

/**
 * lw6dat_warehouse_miss_invalidate
 *
 * @sys_context: global system context
 * @warehouse: object to modify
 * @from_id: node which needs to resend data
 * @to_id: node which needs to get the data
 * @serial_min: minimum serial number to send
 * @serial_max: maximum serial number to send
 *
 * Re-sends messages in a given serial range. Actually, does not really resend
 * messages, but marks them as needing to be sent again. We might, or not, have
 * those messages in stock, if we have them we send them, if not we just do nothing,
 * someone else might have them.
 *
 * Return value: none
 */
void
lw6dat_warehouse_miss_invalidate (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t from_id, u_int64_t to_id, int serial_min,
				  int serial_max)
{
  _lw6dat_warehouse_miss_invalidate (sys_context, (_lw6dat_warehouse_t *) warehouse, from_id, to_id, serial_min, serial_max);
}

void
_lw6dat_warehouse_update_serial_miss_max (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t remote_id, int serial)
{
  int index = 0;

  index = _lw6dat_warehouse_get_stack_index (sys_context, warehouse, remote_id);
  if (index >= 0 && index < LW6DAT_MAX_NB_STACKS && index != _LW6DAT_LOCAL_NODE_INDEX)
    {
      warehouse->stacks[index].serial_miss_max = lw6sys_imax (warehouse->stacks[index].serial_miss_max, serial);
    }
}

/**
 * lw6dat_warehouse_update_serial_miss_max
 *
 * @sys_context: global system context
 * @warehouse: object to update
 * @remote_id: id of remote host to update (which stack)
 * @serial: new max serial value
 *
 * Return value: none
 */
void
lw6dat_warehouse_update_serial_miss_max (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t remote_id, int serial)
{
  _lw6dat_warehouse_update_serial_miss_max (sys_context, (_lw6dat_warehouse_t *) warehouse, remote_id, serial);
}

void
_lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse)
{
  int stack_index = 0;

  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
    {
      warehouse->stacks[stack_index].nb_atom_parts_since_last_poll = 0;
    }

}

/**
 * lw6dat_warehouse_reset_nb_atom_parts_since_last_poll
 *
 * @sys_context: global system context
 * @warehouse: data warehouse to reset
 *
 * Resets the nb_atom_parts_since_last_poll attribute of
 * every stack.
 *
 * Return value: none
 */
void
lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (sys_context, (_lw6dat_warehouse_t *) warehouse);
}

int
_lw6dat_warehouse_get_nb_atom_parts_since_last_poll (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, u_int64_t remote_id)
{
  int stack_index = 0;
  int ret = 0;

  stack_index = _lw6dat_warehouse_get_stack_index (sys_context, warehouse, remote_id);
  if (stack_index >= 0 && stack_index < LW6DAT_MAX_NB_STACKS)
    {
      ret = warehouse->stacks[stack_index].nb_atom_parts_since_last_poll;
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_nb_atom_parts_since_last_poll
 *
 * @sys_context: global system context
 * @warehouse: data warehouse to get info from
 * @remote_id: remote id to get info about
 *
 * Returns the nb_atom_parts_since_last_poll attribute of the stack
 * concerning the remote_id. The idea is to track down, since the
 * last call to the companion reset function, how many "atom parts"
 * have been received, by "atom part" we mean atoms representing a
 * splitted message. Usually this corresponds to very long messages
 * and if this number of splitted messages is large enough, we don't
 * send MISS commands, as it would overload the network for nothing.
 *
 * Return value: number of atom parts received since last reset.
 */
int
lw6dat_warehouse_get_nb_atom_parts_since_last_poll (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, u_int64_t remote_id)
{
  return _lw6dat_warehouse_get_nb_atom_parts_since_last_poll (sys_context, (_lw6dat_warehouse_t *) warehouse, remote_id);
}

int
_lw6dat_warehouse_meta_put (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, int64_t seq)
{
  int ret = 0;
  int64_t local_seq_last = 0LL;
  lw6msg_meta_array_t meta_array;

  local_seq_last = _lw6dat_warehouse_get_local_seq_last (sys_context, warehouse);
  if (seq >= local_seq_last)
    {
      lw6msg_meta_array_zero (sys_context, &meta_array);
      meta_array.logical_from = _lw6dat_warehouse_get_local_id (sys_context, warehouse);
      ret = _lw6dat_warehouse_meta_get (sys_context, warehouse, &meta_array, seq);
      if (ret)
	{
	  ret = _lw6dat_stack_meta_put (sys_context, &(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]), seq, &meta_array);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("unable to put META message at seq %" LW6SYS_PRINTF_LL
		       "d when local_seq_last=%" LW6SYS_PRINTF_LL "d"), (long long) seq, (long long) local_seq_last);
    }

  return ret;
}

/**
 * lw6dat_warehouse_meta_put
 *
 * @sys_context: global system context
 * @warehouse: data warehouse to put message into
 * @seq: seq to use to stamp the message
 *
 * Puts a META message in the warehouse. The META message purpose is
 * to tell other warehouses (on other nodes, peers) that the list of peers
 * is updated. This typically happens when a node joins in.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6dat_warehouse_meta_put (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, int64_t seq)
{
  return (_lw6dat_warehouse_meta_put (sys_context, (_lw6dat_warehouse_t *) warehouse, seq));
}

int
_lw6dat_warehouse_meta_get (lw6sys_context_t * sys_context, _lw6dat_warehouse_t * warehouse, lw6msg_meta_array_t * meta_array, int64_t seq)
{
  int ret = 1;
  int i = 0;
  int64_t lowest_seq = LW6DAT_SEQ_INVALID;

  /*
   * Check that seq is great enough, setting a seq lower than what
   * we have within the warehouse is just, err, well, suicide ;)
   * Note that later, receiver of the meta information might
   * decide to use another seq depending on what relationship is
   * already established.
   *
   * Earlier version used to bail out and block this, now we
   * just force the seq to a higher value.
   */
  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  if (warehouse->stacks[i].seq_0s[i] >= LW6DAT_SEQ_START)
	    {
	      if (lowest_seq == LW6DAT_SEQ_INVALID)
		{
		  lowest_seq = warehouse->stacks[i].seq_0s[i];
		}
	      else
		{
		  lowest_seq = lw6sys_llmin (lowest_seq, warehouse->stacks[i].seq_0s[i]);
		}
	    }
	}
    }

  if (seq < lowest_seq)
    {
      seq = lw6sys_llmax (seq, lowest_seq);
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("updating META message at seq %" LW6SYS_PRINTF_LL "d with lowes_seq=%" LW6SYS_PRINTF_LL "d"), (long long) seq, (long long) lowest_seq);
    }

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  _lw6dat_stack_meta_update (sys_context, &(warehouse->stacks[i]), meta_array, seq);
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_meta_get
 *
 * @sys_context: global system context
 * @warehouse: data warehouse to put message into
 * @meta_array: current informations sendable by a meta message
 * @seq: seq to use to stamp the message
 *
 * Gets the data required for a META message. Note that this function
 * can be called in other contexts to know who is registered within the
 * warehouse, which, as an opaque type, doesn't export that info otherwise.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6dat_warehouse_meta_get (lw6sys_context_t * sys_context, lw6dat_warehouse_t * warehouse, lw6msg_meta_array_t * meta_array, int64_t seq)
{
  return _lw6dat_warehouse_meta_get (sys_context, (_lw6dat_warehouse_t *) warehouse, meta_array, seq);
}
