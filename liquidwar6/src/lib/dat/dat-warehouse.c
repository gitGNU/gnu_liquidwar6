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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dat-internal.h"

#define _SEQ_RANGE_CHECK_LIMIT 1000

int
_lw6dat_warehouse_init (_lw6dat_warehouse_t * warehouse,
			u_int64_t local_node_id)
{
  int ok = 0;
  int stack_index = 0;

  memset (warehouse, 0, sizeof (_lw6dat_warehouse_t));
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

  return ok;
}

/**
 * lw6dat_warehouse_init
 *
 * Initializes a warehouse object. Won't free anything, will just
 * erase values if they're here
 *
 * @warehouse: object to initialize 
 * @local_node_id: id of local node, used to handle local messages
 *
 * Return value: new object, allocated dynamically
 */
int
lw6dat_warehouse_init (lw6dat_warehouse_t * warehouse,
		       u_int64_t local_node_id)
{
  return _lw6dat_warehouse_init ((_lw6dat_warehouse_t *) warehouse,
				 local_node_id);
}

_lw6dat_warehouse_t *
_lw6dat_warehouse_new (u_int64_t local_node_id)
{
  _lw6dat_warehouse_t *warehouse;
  int ok = 0;

  warehouse =
    (_lw6dat_warehouse_t *) LW6SYS_MALLOC (sizeof (_lw6dat_warehouse_t));
  if (warehouse)
    {
      ok = _lw6dat_warehouse_init (warehouse, local_node_id);
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
 * @local_node_id: id of local node, used to handle local messages
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
  _lw6dat_warehouse_clear (warehouse);

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
_lw6dat_warehouse_clear (_lw6dat_warehouse_t * warehouse)
{
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id != 0)
	{
	  _lw6dat_stack_clear (&(warehouse->stacks[i]));
	}
    }
}

/**
 * lw6dat_warehouse_clear
 *
 * @warehouse: the object to clear
 *
 * Clears a warehouse object. Clears means emptying everything
 * and resetting the current seq_id to the minimal/start value.
 *
 * Return value: none.
 */
void
lw6dat_warehouse_clear (lw6dat_warehouse_t * warehouse)
{
  _lw6dat_warehouse_clear ((_lw6dat_warehouse_t *) warehouse);
}

void
_lw6dat_warehouse_purge (_lw6dat_warehouse_t * warehouse)
{
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id != 0)
	{
	  _lw6dat_stack_purge (&(warehouse->stacks[i]));
	}
    }
}

/**
 * lw6dat_warehouse_purge
 *
 * @warehouse: the object to purge
 *
 * Purges a warehouse object. Purges means emptying everything
 * but keeping the current seq_id and the nodes list unchanged.
 *
 * Return value: none.
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

  serial_0 = lw6sys_imax (serial_0, _LW6DAT_SERIAL_START);
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

/**
 * lw6dat_warehouse_register_node
 *
 * @warehouse: object to update
 * @node_id: id of node to register
 * @serial_0: serial number of first message 
 *
 * Registers a node, in practice this is automatically done when receiving
 * a data message but it might be interesting to do it elsewhere and force it.
 *
 * Return value: the stack index of the registered node, <0 is invalid.
 */
int
lw6dat_warehouse_register_node (lw6dat_warehouse_t * warehouse,
				u_int64_t node_id, int serial_0)
{
  return _lw6dat_warehouse_register_node ((_lw6dat_warehouse_t *) warehouse,
					  node_id, serial_0);
}

int
_lw6dat_warehouse_is_node_registered (_lw6dat_warehouse_t * warehouse,
				      u_int64_t node_id)
{
  int ret = 0;

  ret = (_lw6dat_warehouse_get_stack_index (warehouse, node_id) >= 0);

  return ret;
}

/**
 * lw6dat_warehouse_is_node_registered
 *
 * @warehouse: object to update
 * @node_id: id of node to register
 *
 * Tells wether a node is registered or not in our list.
 *
 * Return value: 1 if registered, 0 if not.
 */
int
lw6dat_warehouse_is_node_registered (lw6dat_warehouse_t * warehouse,
				     u_int64_t node_id)
{
  return _lw6dat_warehouse_is_node_registered ((_lw6dat_warehouse_t *)
					       warehouse, node_id);
}

int
_lw6dat_warehouse_put_atom (_lw6dat_warehouse_t * warehouse,
			    u_int64_t logical_from,
			    int serial, int order_i, int order_n, int64_t seq,
			    const char *full_str, int seq_from_cmd_str_offset,
			    int cmd_str_offset)
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
				order_i, order_n, seq, full_str,
				seq_from_cmd_str_offset, cmd_str_offset,
				send_flag);
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
				u_int64_t logical_from, const char *full_str)
{
  int ret = 0;
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
  if (_lw6dat_atom_parse_serial_i_n_seq_from_cmd
      (&serial, &order_i, &order_n, &seq, &logical_from2,
       &seq_from_cmd_str_offset, &cmd_str_offset, full_str))
    {
      if (logical_from == logical_from2)
	{
	  ret =
	    _lw6dat_warehouse_put_atom (warehouse, logical_from,
					serial, order_i, order_n, seq,
					full_str, seq_from_cmd_str_offset,
					cmd_str_offset);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("cant't put atom string, logical_from inconsistency logical_from=%"
		       LW6SYS_PRINTF_LL "x logical_from2=%" LW6SYS_PRINTF_LL
		       "x"), (long long) logical_from,
		      (long long) logical_from2);
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_put_atom_str
 *
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
lw6dat_warehouse_put_atom_str (lw6dat_warehouse_t * warehouse,
			       u_int64_t logical_from, const char *full_str)
{
  int ret = 0;

  ret =
    _lw6dat_warehouse_put_atom_str ((_lw6dat_warehouse_t *) warehouse,
				    logical_from, full_str);

  return ret;
}

int
_lw6dat_warehouse_calc_serial_draft_and_reference (_lw6dat_warehouse_t *
						   warehouse)
{
  int ret = 0;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  if (_lw6dat_stack_calc_serial_draft_and_reference (&
							     (warehouse->stacks
							      [i])))
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
lw6dat_warehouse_calc_serial_draft_and_reference (lw6dat_warehouse_t *
						  warehouse)
{
  int ret = 0;

  ret =
    _lw6dat_warehouse_calc_serial_draft_and_reference ((_lw6dat_warehouse_t *)
						       warehouse);

  return ret;
}

int
_lw6dat_warehouse_put_local_msg (_lw6dat_warehouse_t * warehouse,
				 const char *msg)
{
  int ret = 0;

  ret =
    _lw6dat_stack_put_msg (&(warehouse->stacks[_LW6DAT_LOCAL_NODE_INDEX]),
			   msg, _LW6DAT_FLAG_REMOTE);

  return ret;
}

/**
 * lw6dat_warehouse_put_local_msg
 *
 * @warehouse: warehouse object to use
 * @msg: message
 *
 * Puts a message in the object. The message will be splitted into
 * several atoms if needed, it can be arbitrary long.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6dat_warehouse_put_local_msg (lw6dat_warehouse_t * warehouse,
				const char *msg)
{
  int ret = 0;

  ret =
    _lw6dat_warehouse_put_local_msg ((_lw6dat_warehouse_t *) warehouse, msg);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_min (_lw6dat_warehouse_t * warehouse)
{
  int64_t ret = LLONG_MAX;
  int64_t seq_min;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  seq_min = _lw6dat_stack_get_seq_min (&(warehouse->stacks[i]));
	  if (seq_min > _LW6DAT_SEQ_INVALID)
	    {
	      ret = lw6sys_llmin (ret, seq_min);
	    }
	}
    }
  if (ret == LLONG_MAX)
    {
      ret = _LW6DAT_SEQ_INVALID;
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_min
 *
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
lw6dat_warehouse_get_seq_min (lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0LL;

  ret = _lw6dat_warehouse_get_seq_min ((_lw6dat_warehouse_t *) warehouse);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_max (_lw6dat_warehouse_t * warehouse)
{
  int64_t ret = _LW6DAT_SEQ_INVALID;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  ret =
	    lw6sys_llmax (ret,
			  _lw6dat_stack_get_seq_max (&
						     (warehouse->stacks[i])));
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_max
 *
 * @warehouse: object to query
 *
 * Tells the highest seq referenced in the warehouse. Does not
 * mean an actual message can be built from it, only we've
 * got some traces of such a high seq.
 *
 * Return value: integer.
 */
int64_t
lw6dat_warehouse_get_seq_max (lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0LL;

  ret = _lw6dat_warehouse_get_seq_max ((_lw6dat_warehouse_t *) warehouse);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_draft (_lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0LL;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  ret =
	    lw6sys_llmax (ret,
			  _lw6dat_stack_get_seq_draft (&
						       (warehouse->stacks
							[i])));
	}
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_draft
 *
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
lw6dat_warehouse_get_seq_draft (lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0;

  ret = _lw6dat_warehouse_get_seq_draft ((_lw6dat_warehouse_t *) warehouse);

  return ret;
}

int64_t
_lw6dat_warehouse_get_seq_reference (_lw6dat_warehouse_t * warehouse)
{
  int64_t ret = LLONG_MAX;
  int i;

  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      if (warehouse->stacks[i].node_id)
	{
	  ret =
	    lw6sys_llmin (ret,
			  _lw6dat_stack_get_seq_reference (&
							   (warehouse->stacks
							    [i])));
	}
    }
  if (ret >= INT_MAX)
    {
      ret = _LW6DAT_SEQ_INVALID;
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_seq_reference
 *
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
lw6dat_warehouse_get_seq_reference (lw6dat_warehouse_t * warehouse)
{
  int64_t ret = 0;

  ret =
    _lw6dat_warehouse_get_seq_reference ((_lw6dat_warehouse_t *) warehouse);

  return ret;
}

lw6sys_list_t *
_lw6dat_warehouse_get_msg_list_by_seq (_lw6dat_warehouse_t * warehouse,
				       int64_t seq_min, int64_t seq_max,
				       int for_reference,
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

      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("before range check seq_min=%" LW6SYS_PRINTF_LL
		       "d seq_max=%" LW6SYS_PRINTF_LL "d"),
		  (long long) seq_min, (long long) seq_max);

      warehouse_seq_min = _lw6dat_warehouse_get_seq_min (warehouse);
      warehouse_seq_max = _lw6dat_warehouse_get_seq_max (warehouse);
      seq_min = lw6sys_llmax (warehouse_seq_min - 1, seq_min);
      seq_max = lw6sys_llmin (warehouse_seq_max + 1, seq_max);

      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("after range check seq_min=%" LW6SYS_PRINTF_LL
		       "d seq_max=%" LW6SYS_PRINTF_LL "d"),
		  (long long) seq_min, (long long) seq_max);
    }
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
							seq, for_reference,
							for_reference
							&& (seq == seq_max),
							&tmp_worst_msg_i,
							&tmp_worst_msg_n);
		  if (tmp_worst_msg_i * main_worst_msg_n <=
		      main_worst_msg_i * tmp_worst_msg_n)
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
      lw6sys_sort (&ret, lw6msg_sort_str_by_seq_callback);
    }

  if (main_worst_msg_n > 0)
    {
      lw6sys_progress_update (progress, 0, main_worst_msg_n,
			      main_worst_msg_i);
    }
  else
    {
      lw6sys_progress_end (progress);
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_msg_list_by_seq
 *
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
lw6dat_warehouse_get_msg_list_by_seq (lw6dat_warehouse_t * warehouse,
				      int64_t seq_min, int64_t seq_max,
				      int for_reference,
				      lw6sys_progress_t * progress)
{
  lw6sys_list_t *ret = NULL;

  ret =
    _lw6dat_warehouse_get_msg_list_by_seq ((_lw6dat_warehouse_t *) warehouse,
					   seq_min, seq_max, for_reference,
					   progress);

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
      for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
	{
	  if ((stack_index != target_index)
	      && (warehouse->stacks[stack_index].node_id != 0))
	    {
	      _lw6dat_stack_update_atom_str_list_not_sent (&
							   (warehouse->stacks
							    [stack_index]),
							   &ret,
							   target_index);
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
  lw6sys_list_t *ret = NULL;

  ret =
    _lw6dat_warehouse_get_atom_str_list_not_sent ((_lw6dat_warehouse_t *)
						  warehouse, logical_to);

  return ret;
}

lw6sys_list_t *
_lw6dat_warehouse_get_miss_list (_lw6dat_warehouse_t * warehouse,
				 int max_range, lw6sys_progress_t * progress)
{
  lw6sys_list_t *ret = NULL;
  lw6dat_miss_t *miss = NULL;
  int i = 0;
  int tmp_worst_msg_i = 0;
  int tmp_worst_msg_n = 0;
  int main_worst_msg_i = 0;
  int main_worst_msg_n = 0;

  ret = lw6sys_list_new ((lw6sys_free_func_t) lw6dat_miss_free);
  if (ret)
    {
      for (i = 0; i < LW6DAT_MAX_NB_STACKS && ret; ++i)
	{
	  if (i != _LW6DAT_LOCAL_NODE_INDEX && warehouse->stacks[i].node_id)
	    {
	      miss =
		_lw6dat_stack_get_miss (&(warehouse->stacks[i]), max_range,
					&tmp_worst_msg_i, &tmp_worst_msg_n);
	      if (miss)
		{
		  if (tmp_worst_msg_i * main_worst_msg_n <=
		      main_worst_msg_i * tmp_worst_msg_n)
		    {
		      main_worst_msg_i = tmp_worst_msg_i;
		      main_worst_msg_n = tmp_worst_msg_n;
		    }
		  lw6sys_list_push_front (&ret, miss);
		}
	    }
	}
    }

  if (main_worst_msg_n > 0)
    {
      lw6sys_progress_update (progress, 0, main_worst_msg_n,
			      main_worst_msg_i);
    }
  else
    {
      lw6sys_progress_end (progress);
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_miss_list
 *
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
lw6dat_warehouse_get_miss_list (lw6dat_warehouse_t * warehouse, int max_range,
				lw6sys_progress_t * progress)
{
  lw6sys_list_t *ret = NULL;

  ret =
    _lw6dat_warehouse_get_miss_list ((_lw6dat_warehouse_t *) warehouse,
				     max_range, progress);

  return ret;
}

void
_lw6dat_warehouse_miss_invalidate (_lw6dat_warehouse_t * warehouse,
				   u_int64_t from_id, u_int64_t to_id,
				   int serial_min, int serial_max)
{
  int stack_index = 0;
  int target_index = 0;

  target_index = _lw6dat_warehouse_get_stack_index (warehouse, to_id);
  for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS; ++stack_index)
    {
      if ((stack_index != target_index)
	  && (warehouse->stacks[stack_index].node_id != 0))
	{
	  _lw6dat_stack_miss_invalidate (&(warehouse->stacks[stack_index]),
					 target_index, serial_min,
					 serial_max);
	}
    }
}

/**
 * lw6dat_warehouse_miss_invalidate
 *
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
lw6dat_warehouse_miss_invalidate (lw6dat_warehouse_t * warehouse,
				  u_int64_t from_id, u_int64_t to_id,
				  int serial_min, int serial_max)
{
  _lw6dat_warehouse_miss_invalidate ((_lw6dat_warehouse_t *) warehouse,
				     from_id, to_id, serial_min, serial_max);
}

void
_lw6dat_warehouse_update_serial_miss_max (_lw6dat_warehouse_t * warehouse,
					  u_int64_t remote_id, int serial)
{
  int index = 0;

  index = _lw6dat_warehouse_get_stack_index (warehouse, remote_id);
  if (index >= 0 && index < LW6DAT_MAX_NB_STACKS
      && index != _LW6DAT_LOCAL_NODE_INDEX)
    {
      warehouse->stacks[index].serial_miss_max =
	lw6sys_imax (warehouse->stacks[index].serial_miss_max, serial);
    }
}

/**
 * lw6dat_warehouse_update_serial_miss_max
 *
 * @warehouse: object to update
 * @remote_id: id of remote host to update (which stack)
 * @serial: new max serial value
 *
 * Return value: none
 */
void
lw6dat_warehouse_update_serial_miss_max (lw6dat_warehouse_t * warehouse,
					 u_int64_t remote_id, int serial)
{
  _lw6dat_warehouse_update_serial_miss_max ((_lw6dat_warehouse_t *) warehouse,
					    remote_id, serial);
}

void
_lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (_lw6dat_warehouse_t *
						       warehouse)
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
 * @warehouse: data warehouse to reset
 *
 * Resets the nb_atom_parts_since_last_poll attribute of
 * every stack.
 *
 * Return value: none
 */
void
lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (lw6dat_warehouse_t *
						      warehouse)
{
  _lw6dat_warehouse_reset_nb_atom_parts_since_last_poll ((_lw6dat_warehouse_t
							  *) warehouse);
}

int
_lw6dat_warehouse_get_nb_atom_parts_since_last_poll (_lw6dat_warehouse_t *
						     warehouse,
						     u_int64_t remote_id)
{
  int stack_index = 0;
  int ret = 0;

  stack_index = _lw6dat_warehouse_get_stack_index (warehouse, remote_id);
  if (stack_index >= 0 && stack_index < LW6DAT_MAX_NB_STACKS)
    {
      ret = warehouse->stacks[stack_index].nb_atom_parts_since_last_poll;
    }

  return ret;
}

/**
 * lw6dat_warehouse_get_nb_atom_parts_since_last_poll
 *
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
lw6dat_warehouse_get_nb_atom_parts_since_last_poll (lw6dat_warehouse_t *
						    warehouse,
						    u_int64_t remote_id)
{
  return
    _lw6dat_warehouse_get_nb_atom_parts_since_last_poll ((_lw6dat_warehouse_t
							  *) warehouse,
							 remote_id);
}
