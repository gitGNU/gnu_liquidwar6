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

#include <math.h>

#include "dat-internal.h"

#define _TEST_ATOM_TEXT_SHORT "this is a short text"
#define _TEST_ATOM_TEXT_LONG "this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text..."
#define _TEST_ATOM_LOGICAL_FROM_STR "1234123412341234"

#define _TEST_BLOCK_SERIAL_0 456
#define _TEST_BLOCK_ORDER_N 7
#define _TEST_BLOCK_SEQ 10000000000002LL
#define _TEST_BLOCK_STEP 3
#define _TEST_BLOCK_TEXT_1 "0+0=zero"
#define _TEST_BLOCK_TEXT_2 "2+2=4"
#define _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET 0
#define _TEST_BLOCK_CMD_STR_OFFSET 0
#define _TEST_BLOCK_SEND_FLAG 6

#define _TEST_MISS_FROM_ID 0x1234123412341234LL
#define _TEST_MISS_SERIAL_MIN 123
#define _TEST_MISS_SERIAL_MAX 456

#define _TEST_STACK_NODE_ID 0x2345234523452345LL
#define _TEST_STACK_SERIAL_0 123
#define _TEST_STACK_SEQ_0 10LL
#define _TEST_STACK_ORDER_I 0
#define _TEST_STACK_ORDER_N 1
#define _TEST_STACK_SEQ 10000000000005LL
#define _TEST_STACK_TEXT "..."
#define _TEST_STACK_SEQ_FROM_CMD_STR_OFFSET 0
#define _TEST_STACK_CMD_STR_OFFSET 0
#define _TEST_STACK_NB_RANDOM_PUT (_LW6DAT_MAX_NB_ATOMS/100)
#define _TEST_STACK_RANDOM_BASE _LW6DAT_NB_ATOMS_PER_BLOCK
#define _TEST_STACK_RANDOM_RANGE (_LW6DAT_MAX_NB_ATOMS-2*_LW6DAT_NB_ATOMS_PER_BLOCK)
#define _TEST_STACK_SEND_FLAG 5
#define _TEST_STACK_TARGET_INDEX 2
#define _TEST_STACK_MSG_1 "10000000000002 2345234523452345 first message, short one"
#define _TEST_STACK_MSG_2 "10000000000003 2345234523452345 second message, short one too"
#define _TEST_STACK_MSG_3 "10000000000011 2345234523452345 third message, short one again"
#define _TEST_STACK_MSG_4 "10000000000011 2345234523452345 fourth message, this one is long, will be filled with random string %s"
#define _TEST_STACK_MSG_4_RANDOM_PART_SIZE 500000
#define _TEST_STACK_MSG_5 "10000000000011 2345234523452345 fifth message, with same round/seq than the two previous ones"
#define _TEST_STACK_MSG_6 "10000000000013 2345234523452345 last message"
#define _TEST_STACK_MSG_GET_SEQ 10000000000011LL
#define _TEST_STACK_MSG_GET_NB 3
#define _TEST_STACK_MISS_MAX_RANGE 100

#define _TEST_WAREHOUSE_LOCAL_NODE_ID 0x1234123412341234LL
#define _TEST_WAREHOUSE_OTHER_NODE_ID 0x2345234523452345LL
#define _TEST_WAREHOUSE_SERIAL 789
#define _TEST_WAREHOUSE_ORDER_I 0
#define _TEST_WAREHOUSE_ORDER_N 1
#define _TEST_WAREHOUSE_SEQ_0 10LL
#define _TEST_WAREHOUSE_SEQ 10000000000007LL
#define _TEST_WAREHOUSE_TEXT "spam and ham"
#define _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET 0
#define _TEST_WAREHOUSE_CMD_STR_OFFSET 0
#define _TEST_WAREHOUSE_NB_STACKS_OVERFLOW (LW6DAT_MAX_NB_STACKS*2)
#define _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW (_LW6DAT_MAX_NB_ATOMS*2)
#define _TEST_WAREHOUSE_SERIAL_I_N_MSG "DATA 123 2 4 10 2345234523452345 foo bar"
#define _TEST_WAREHOUSE_PUT_MIN_SIZE 1
#define _TEST_WAREHOUSE_PUT_MAX_SIZE 3000
#define _TEST_WAREHOUSE_OTHER_NODE_MSG "toto"
#define _TEST_WAREHOUSE_MISS_MAX_RANGE LW6DAT_MISS_MAX_RANGE_UNLIMITED

typedef struct _test_stack_msg_data_s
{
  char *msg4;
  int msg3_found;
  int msg4_found;
  int msg5_found;
} _test_stack_msg_data_t;

/*
 * Testing functions in atom.c
 */
static int
test_atom ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6dat_atom_t atom;
    char *text = NULL;
    //    char *cmd = NULL;

    _lw6dat_atom_zero (&atom);
    if (_lw6dat_atom_get_full_str (&atom))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("get text returned something on a zeroed atom"));
	ret = 0;
      }
    _lw6dat_atom_set_full_str (&atom, _TEST_ATOM_TEXT_SHORT);
    text = _lw6dat_atom_get_full_str (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_full_str (&atom, _TEST_ATOM_TEXT_LONG);
    text = _lw6dat_atom_get_full_str (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_full_str (&atom, _TEST_ATOM_TEXT_SHORT);
    text = _lw6dat_atom_get_full_str (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_full_str (&atom, _TEST_ATOM_TEXT_LONG);
    text = _lw6dat_atom_get_full_str (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    /*
       cmd =
       _lw6dat_atom_recreate_atom_str_from_atom (&atom,
       _TEST_ATOM_LOGICAL_FROM_STR);
       if (cmd)
       {
       lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("recreated cmd is \"%s\""), cmd);
       LW6SYS_FREE (cmd);
       }
     */
    _lw6dat_atom_clear (&atom);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in block.c
 */
static int
test_block ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int serial = 0;
    int order_i = 0;
    _lw6dat_block_t *block = NULL;
    _lw6dat_atom_t *atom = NULL;

    block = _lw6dat_block_new (_TEST_BLOCK_SERIAL_0);
    if (block)
      {
	for (serial = _TEST_BLOCK_SERIAL_0;
	     serial < _LW6DAT_NB_ATOMS_PER_BLOCK + _TEST_BLOCK_SERIAL_0;
	     serial += _TEST_BLOCK_STEP, order_i += _TEST_BLOCK_STEP)
	  {
	    if (_lw6dat_block_put_atom
		(block, serial, order_i, _TEST_BLOCK_ORDER_N,
		 _TEST_BLOCK_SEQ, _TEST_BLOCK_TEXT_1,
		 _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET,
		 _TEST_BLOCK_CMD_STR_OFFSET, _TEST_BLOCK_SEND_FLAG))
	      {
		// ok
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("unable to add atom into block serial=%d"),
			    serial);
		ret = 0;
	      }
	  }
	if (ret)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("added %d atoms"),
			_LW6DAT_NB_ATOMS_PER_BLOCK / _TEST_BLOCK_STEP);
	  }
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("trying various invalid atom \"put\" calls"));
	if (_lw6dat_block_put_atom
	    (block, _TEST_BLOCK_SERIAL_0, 0, _TEST_BLOCK_ORDER_N,
	     _TEST_BLOCK_SEQ, _TEST_BLOCK_TEXT_2,
	     _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET, _TEST_BLOCK_CMD_STR_OFFSET,
	     _TEST_BLOCK_SEND_FLAG))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("wrong duplicate detection"));
	    ret = 0;
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("duplicate detection OK"));
	  }
	if (_lw6dat_block_put_atom
	    (block, _TEST_BLOCK_SERIAL_0 - 1, 0, _TEST_BLOCK_ORDER_N,
	     _TEST_BLOCK_SEQ, _TEST_BLOCK_TEXT_1,
	     _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET, _TEST_BLOCK_CMD_STR_OFFSET,
	     _TEST_BLOCK_SEND_FLAG)
	    || _lw6dat_block_put_atom (block,
				       _TEST_BLOCK_SERIAL_0 +
				       _LW6DAT_NB_ATOMS_PER_BLOCK, 0,
				       _TEST_BLOCK_ORDER_N, _TEST_BLOCK_SEQ,
				       _TEST_BLOCK_TEXT_1,
				       _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET,
				       _TEST_BLOCK_CMD_STR_OFFSET,
				       _TEST_BLOCK_SEND_FLAG))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("wrong range check"));
	    ret = 0;
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("range check OK"));
	  }
	atom = _lw6dat_block_get_atom (block, _TEST_BLOCK_SERIAL_0);
	if (atom)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("got atom serial=%d"),
			atom->serial);
	    atom =
	      _lw6dat_block_get_atom (block,
				      _TEST_BLOCK_SERIAL_0 +
				      _TEST_BLOCK_STEP - 1);
	    if (atom)
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("non-set atom returns non-NULL"));
		ret = 0;
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("OK, non-set atoms returned as NULL"));
	      }
	  }
	_lw6dat_block_free (block);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create block"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in miss.c
 */
static int
test_miss ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6dat_miss_t *miss = NULL;

    miss =
      lw6dat_miss_new (_TEST_MISS_FROM_ID, _TEST_MISS_SERIAL_MIN,
		       _TEST_MISS_SERIAL_MAX);
    if (miss)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("miss struct created from_id=%" LW6SYS_PRINTF_LL
			 "x serial_min=%d serial_%d"),
		    (long long) miss->from_id, miss->serial_min,
		    miss->serial_max);
	lw6dat_miss_free (miss);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
_test_stack_msg_callback (void *func_data, void *data)
{
  _test_stack_msg_data_t *msg_data = (_test_stack_msg_data_t *) func_data;
  char *msg = (char *) data;

  if (lw6sys_str_is_same (msg, _TEST_STACK_MSG_3))
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("message 3 found (length=%d)"),
		  (int) strlen (msg));
      msg_data->msg3_found = 1;
    }
  if (lw6sys_str_is_same (msg, msg_data->msg4))
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("message 4 found (length=%d)"),
		  (int) strlen (msg));
      msg_data->msg4_found = 1;
    }
  if (lw6sys_str_is_same (msg, _TEST_STACK_MSG_5))
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("message 5 found (length=%d)"),
		  (int) strlen (msg));
      msg_data->msg5_found = 1;
    }
}

/*
 * Testing functions in stack.c
 */
static int
test_stack ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6dat_stack_t stack;
    int i;
    int serial = _LW6DAT_SERIAL_INVALID;
    int64_t seq = _LW6DAT_SEQ_INVALID;
    int64_t tmp;
    int found_null = 0;
    int found_not_null = 0;
    lw6sys_list_t *msg_list = NULL;
    char *msg = NULL;
    int length = 0;
    char *msg4 = NULL;
    char *msg4_random_part = NULL;
    _test_stack_msg_data_t msg_data = { NULL, 0, 0, 0 };
    lw6dat_miss_t *miss = NULL;
    int worst_msg_i = 0;
    int worst_msg_n = 0;

    _lw6dat_stack_zero (&stack);
    _lw6dat_stack_init (&stack, _TEST_STACK_NODE_ID, _TEST_STACK_SERIAL_0,
			_TEST_STACK_SEQ_0);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("putting %d atoms in stack"),
		_TEST_STACK_NB_RANDOM_PUT);
    for (i = 0; i < _TEST_STACK_NB_RANDOM_PUT; ++i)
      {
	serial =
	  _TEST_STACK_SERIAL_0 + _TEST_STACK_RANDOM_BASE +
	  lw6sys_random (_TEST_STACK_RANDOM_RANGE);
	seq = _TEST_STACK_SEQ + i;
	if (!_lw6dat_stack_put_atom
	    (&stack, serial, _TEST_STACK_ORDER_I, _TEST_STACK_ORDER_N,
	     seq, _TEST_STACK_TEXT, _TEST_STACK_SEQ_FROM_CMD_STR_OFFSET,
	     _TEST_STACK_CMD_STR_OFFSET, _TEST_STACK_SEND_FLAG))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("error putting atom on stack i=%d serial=%d"), i,
			serial);
	    ret = 0;
	  }
      }
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("last put serial=%d seq=%" LW6SYS_PRINTF_LL "d"), serial,
		(long long) seq);
    if (ret)
      {
	/*
	 * The tests for seq2serial and serial2seq must
	 * be done now as serial and seq values are untouched.
	 */
	if (_lw6dat_stack_seq2serial (&stack, _LW6DAT_SEQ_INVALID) ==
	    _LW6DAT_SERIAL_INVALID)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("invalid seq number correctly treated by seq2serial"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("invalid seq number *NOT* correctly treated by seq2serial"));
	    ret = 0;
	  }
	if (_lw6dat_stack_serial2seq (&stack, _LW6DAT_SERIAL_INVALID) ==
	    _LW6DAT_SEQ_INVALID)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("invalid serial number correctly treated by serial2seq"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("invalid serial number *NOT* correctly treated by serial2seq"));
	    ret = 0;
	  }
	tmp = _lw6dat_stack_serial2seq (&stack, serial);
	if (tmp == seq)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("serial2seq OK %d->%" LW6SYS_PRINTF_LL "d"),
			serial, (long long) seq);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("serial2seq problem %d->%" LW6SYS_PRINTF_LL
			     "d instead of %" LW6SYS_PRINTF_LL "d"), serial,
			(long long) tmp, (long long) seq);
	    ret = 0;
	  }
	tmp = _lw6dat_stack_seq2serial (&stack, seq);
	if (tmp == serial)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("seq2serial OK %" LW6SYS_PRINTF_LL "d->%d"),
			(long long) seq, serial);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("seq2serial problem %" LW6SYS_PRINTF_LL "d->%"
			     LW6SYS_PRINTF_LL "d instead of %d"),
			(long long) seq, (long long) tmp, serial);
	    ret = 0;
	  }
	for (serial = stack.serial_0; serial <= stack.serial_n_1; ++serial)
	  {
	    if (_lw6dat_stack_get_atom (&stack, serial))
	      {
		found_not_null++;
	      }
	    else
	      {
		found_null++;
	      }
	  }
	if (found_null && found_not_null)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("OK, found both NULL and non-NULL entries found_not_null=%d found_null=%d"),
			found_not_null, found_null);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("couldn't find both NULL and non-NULL entries found_not_null=%d found_null=%d"),
			found_not_null, found_null);
	    ret = 0;
	  }
	msg_list = _lw6dat_stack_init_list ();
	if (msg_list)
	  {
	    _lw6dat_stack_update_atom_str_list_by_serial (&stack, &msg_list,
							  stack.serial_max);
	    length = lw6sys_list_length (msg_list);
	    if (length == 1)
	      {
		msg = (char *) lw6sys_list_pop_front (&msg_list);
		if (msg)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("got atom str by serial \"%s\""), msg);
		    LW6SYS_FREE (msg);
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("there should be an atom at serial %d, unable to get it"),
			    stack.serial_max);
		ret = 0;
	      }

	    _lw6dat_stack_update_atom_str_list_not_sent (&stack, &msg_list,
							 _TEST_STACK_TARGET_INDEX);
	    length = lw6sys_list_length (msg_list);
	    if (length == found_not_null)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("got %d unsent atoms"),
			    length);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("error getting list of atoms length=%d found_not_null=%d"),
			    length, found_not_null);
		ret = 0;
	      }
	    while (!lw6sys_list_is_empty (msg_list))
	      {
		msg = (char *) lw6sys_list_pop_front (&msg_list);
		if (msg)
		  {
		    LW6SYS_FREE (msg);
		  }
	      }

	    _lw6dat_stack_miss_invalidate (&stack, _TEST_STACK_TARGET_INDEX,
					   stack.serial_min,
					   stack.serial_max);
	    _lw6dat_stack_update_atom_str_list_not_sent (&stack, &msg_list,
							 _TEST_STACK_TARGET_INDEX);
	    length = lw6sys_list_length (msg_list);
	    if (length == found_not_null)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("got %d unsent atoms"),
			    length);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("error getting list of atoms length=%d found_not_null=%d"),
			    length, found_not_null);
		ret = 0;
	      }
	    while (!lw6sys_list_is_empty (msg_list))
	      {
		msg = (char *) lw6sys_list_pop_front (&msg_list);
		if (msg)
		  {
		    LW6SYS_FREE (msg);
		  }
	      }

	    _lw6dat_stack_update_atom_str_list_not_sent (&stack, &msg_list,
							 _TEST_STACK_TARGET_INDEX);
	    length = lw6sys_list_length (msg_list);
	    if (length == 0)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("got no more unsent atoms on second call"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("error getting list of atoms length=%d, should be 0"),
			    length);
		ret = 0;
	      }
	    lw6sys_list_free (msg_list);
	  }
      }

    /*
     * We purge the whole stuff, we don't want previous random
     * data to interfere
     */
    _lw6dat_stack_purge (&stack);

    msg4_random_part =
      lw6sys_str_random_words (_TEST_STACK_MSG_4_RANDOM_PART_SIZE);
    if (msg4_random_part)
      {
	msg4 = lw6sys_new_sprintf (_TEST_STACK_MSG_4, msg4_random_part);
	if (msg4)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("put a few messages in stack to test recollection of atoms into a full message"));
	    if (_lw6dat_stack_put_msg
		(&stack, _TEST_STACK_MSG_1, _TEST_STACK_SEND_FLAG)
		&& _lw6dat_stack_put_msg (&stack, _TEST_STACK_MSG_2,
					  _TEST_STACK_SEND_FLAG)
		&& _lw6dat_stack_put_msg (&stack, _TEST_STACK_MSG_3,
					  _TEST_STACK_SEND_FLAG)
		&& _lw6dat_stack_put_msg (&stack, msg4, _TEST_STACK_SEND_FLAG)
		&& _lw6dat_stack_put_msg (&stack, _TEST_STACK_MSG_5,
					  _TEST_STACK_SEND_FLAG)
		&& _lw6dat_stack_put_msg (&stack, _TEST_STACK_MSG_6,
					  _TEST_STACK_SEND_FLAG))
	      {
		msg_list = _lw6dat_stack_init_list ();
		if (msg_list)
		  {
		    _lw6dat_stack_update_msg_list_by_seq (&stack, &msg_list,
							  _TEST_STACK_MSG_GET_SEQ,
							  1, 1, &worst_msg_i,
							  &worst_msg_n);
		    length = lw6sys_list_length (msg_list);
		    if (length == _TEST_STACK_MSG_GET_NB)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("OK, got %d messages"), length);
			msg_data.msg4 = msg4;
			lw6sys_list_map (msg_list, _test_stack_msg_callback,
					 (void *) &msg_data);
			if (msg_data.msg3_found && msg_data.msg4_found
			    && msg_data.msg5_found)
			  {
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_ ("all messages OK"));
			  }
			else
			  {
			    lw6sys_log (LW6SYS_LOG_WARNING,
					_x_ ("some messages were wrong"));
			    ret = 0;
			  }
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("error, should have got %d messages and got %d"),
				    _TEST_STACK_MSG_GET_NB, length);
			ret = 0;
		      }
		    lw6sys_list_free (msg_list);

		    miss =
		      _lw6dat_stack_get_miss (&stack,
					      _TEST_STACK_MISS_MAX_RANGE,
					      &worst_msg_i, &worst_msg_n);
		    if (miss)
		      {
			if (miss->serial_min <= stack.serial_max)
			  {
			    lw6sys_log (LW6SYS_LOG_WARNING,
					_x_ ("miss from_id=%" LW6SYS_PRINTF_LL
					     "x miss->serial_min=%d miss->serial_max=%d stack.serial_min=%d stack.serial_max=%d worst_msg_i=%d worst_msg_n=%d, normally all messages should be complete"),
					(long long) miss->from_id,
					miss->serial_min, miss->serial_max,
					stack.serial_min, stack.serial_max,
					worst_msg_i, worst_msg_n);
			    ret = 0;
			  }
			else
			  {
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_ ("miss from_id=%" LW6SYS_PRINTF_LL
					     "x miss->serial_min=%d miss->serial_max=%d stack.serial_min=%d stack.serial_max=%d worst_msg_i=%d worst_msg_n=%d, since the range is outside (later than) our current data, it will simply cause a new fetch, this does not harm (and can help if remote host is stall)"),
					(long long) miss->from_id,
					miss->serial_min, miss->serial_max,
					stack.serial_min, stack.serial_max,
					worst_msg_i, worst_msg_n);
			  }
			lw6dat_miss_free (miss);
		      }
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("unable to put one of the messages"));
		ret = 0;
	      }
	    LW6SYS_FREE (msg4);
	  }
	LW6SYS_FREE (msg4_random_part);
      }

    _lw6dat_stack_clear (&stack);
    _lw6dat_stack_clear (&stack);	// call it twice, to check it's allowed
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
_test_warehouse_copy_atoms_callback (void *func_data, void *data)
{
  lw6dat_warehouse_t *warehouse2 = (lw6dat_warehouse_t *) func_data;
  char *atom_str = (char *) data;

  if (lw6dat_warehouse_put_atom_str
      (warehouse2, _TEST_WAREHOUSE_LOCAL_NODE_ID, atom_str))
    {
      // ok
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("error copying atom \"%s\""),
		  atom_str);
    }
}

/*
 * Testing functions in warehouse.c
 */
static int
test_warehouse ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6dat_warehouse_t *_warehouse;
    lw6dat_warehouse_t *warehouse;
    lw6dat_warehouse_t *warehouse2;
    int overflow = 0;
    int i;
    char *cmd = NULL;
    char *id_str = NULL;
    char *msg = NULL;
    lw6sys_list_t *list_not_sent = NULL;
    lw6sys_list_t *list_by_seq = NULL;
    lw6sys_list_t *list_by_seq2 = NULL;
    int length = 0;
    int length2 = 0;
    int64_t seq_min = 0;
    int64_t seq_max = 0;
    int64_t seq_draft = 0;
    int64_t seq_reference = 0;
    lw6sys_list_t *miss_list = NULL;
    int miss_list_length = 0;
    int nb_atom_parts_since_last_poll = 0;
    float progress_value = 0.0f;
    lw6sys_progress_t progress;

    lw6sys_progress_default (&progress, &progress_value);
    lw6sys_progress_begin (&progress);

    _warehouse =
      _lw6dat_warehouse_new (_TEST_WAREHOUSE_LOCAL_NODE_ID,
			     _TEST_WAREHOUSE_SEQ_0);
    if (_warehouse)
      {
	if (_lw6dat_warehouse_put_atom
	    (_warehouse, _TEST_WAREHOUSE_LOCAL_NODE_ID,
	     _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_ORDER_I,
	     _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_SEQ,
	     _TEST_WAREHOUSE_TEXT, _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET,
	     _TEST_WAREHOUSE_CMD_STR_OFFSET))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("text \"%s\" put into warehouse"),
			_TEST_WAREHOUSE_TEXT);
	  }
	for (i = 0; i < _TEST_WAREHOUSE_NB_STACKS_OVERFLOW; ++i)
	  {
	    if (!_lw6dat_warehouse_put_atom
		(_warehouse, lw6sys_generate_id_64 (), _TEST_WAREHOUSE_SERIAL,
		 _TEST_WAREHOUSE_ORDER_I, _TEST_WAREHOUSE_ORDER_N,
		 _TEST_WAREHOUSE_SEQ, _TEST_WAREHOUSE_TEXT,
		 _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET,
		 _TEST_WAREHOUSE_CMD_STR_OFFSET))
	      {
		overflow = 1;
	      }
	  }
	if (overflow)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("OK, warehouse has a stack number limit (%d) this is right"),
			_lw6dat_warehouse_get_nb_nodes (_warehouse));
	  }
	else
	  {
	    ret = 0;
	  }
	_lw6dat_warehouse_purge (_warehouse);
	_lw6dat_warehouse_clear (_warehouse);
	_lw6dat_warehouse_free (_warehouse);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("couldn't create _warehouse object"));
	ret = 0;
      }

    _warehouse =
      _lw6dat_warehouse_new (_TEST_WAREHOUSE_LOCAL_NODE_ID,
			     _TEST_WAREHOUSE_SEQ_0);
    if (_warehouse)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("putting %d atoms in warehouse"),
		    _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW);
	if (_lw6dat_warehouse_put_atom
	    (_warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
	     _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_ORDER_I,
	     _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_SEQ,
	     _TEST_WAREHOUSE_TEXT, _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET,
	     _TEST_WAREHOUSE_CMD_STR_OFFSET))
	  {
	    for (i = 1; i < _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW; ++i)
	      {
		if (!_lw6dat_warehouse_put_atom
		    (_warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
		     _TEST_WAREHOUSE_SERIAL + i, _TEST_WAREHOUSE_ORDER_I,
		     _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_SEQ,
		     _TEST_WAREHOUSE_TEXT,
		     _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET,
		     _TEST_WAREHOUSE_CMD_STR_OFFSET))
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_ ("unable to put atom %d"), i);
		    ret = 0;
		  }
	      }
	    if (_lw6dat_warehouse_put_atom
		(_warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
		 _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_ORDER_I,
		 _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_SEQ,
		 _TEST_WAREHOUSE_TEXT,
		 _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET,
		 _TEST_WAREHOUSE_CMD_STR_OFFSET))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("could put again 1st atom in warehouse, shifting should work"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("couldn't put (again) 1st atom in warehouse"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("couldn't put 1st atom in warehouse"));
	    ret = 0;
	  }

	_lw6dat_warehouse_free (_warehouse);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("couldn't create _warehouse object"));
	ret = 0;
      }

    warehouse =
      lw6dat_warehouse_new (_TEST_WAREHOUSE_LOCAL_NODE_ID,
			    _TEST_WAREHOUSE_SEQ_0);
    if (warehouse)
      {
	if (lw6dat_warehouse_get_local_id (warehouse) !=
	    _TEST_WAREHOUSE_LOCAL_NODE_ID)
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("lw6dat_warehouse_get_local_id failed"));
	    ret = 0;
	  }
	if (lw6dat_warehouse_put_atom_str
	    (warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
	     _TEST_WAREHOUSE_SERIAL_I_N_MSG))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("lw6dat_warehouse_put_atom_str seem to work"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("lw6dat_warehouse_put_atom_str failed"));
	    ret = 0;
	  }
	nb_atom_parts_since_last_poll =
	  lw6dat_warehouse_get_nb_atom_parts_since_last_poll (warehouse,
							      _TEST_WAREHOUSE_OTHER_NODE_ID);
	if (nb_atom_parts_since_last_poll > 0)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("OK, got %d atom parts since last poll"),
			nb_atom_parts_since_last_poll);
	    lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (warehouse);
	    nb_atom_parts_since_last_poll =
	      lw6dat_warehouse_get_nb_atom_parts_since_last_poll (warehouse,
								  _TEST_WAREHOUSE_OTHER_NODE_ID);
	    if (nb_atom_parts_since_last_poll == 0)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("OK, got no atom parts since last poll"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("got %d atom parts since last poll but there should be none"),
			    nb_atom_parts_since_last_poll);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("got no atom parts since last poll"));
	    ret = 0;
	  }

	lw6dat_warehouse_purge (warehouse);

	id_str = lw6sys_id_ltoa (_TEST_WAREHOUSE_LOCAL_NODE_ID);
	if (id_str)
	  {
	    for (i = _TEST_WAREHOUSE_PUT_MIN_SIZE;
		 i < _TEST_WAREHOUSE_PUT_MAX_SIZE; ++i)
	      {
		cmd = lw6sys_str_random_words (i);
		if (cmd)
		  {
		    msg =
		      lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %s %s",
					  (long long) (_TEST_WAREHOUSE_SEQ +
						       (int) sqrt (i)),
					  id_str, cmd);
		    if (msg)
		      {
			if (lw6dat_warehouse_put_local_msg (warehouse, msg))
			  {
			    // ok
			  }
			else
			  {
			    lw6sys_log (LW6SYS_LOG_WARNING,
					_x_
					("unable to put msg \"%s\" into warehouse"),
					msg);
			    ret = 0;
			  }
			LW6SYS_FREE (msg);
		      }
		    LW6SYS_FREE (cmd);
		  }
	      }
	    LW6SYS_FREE (id_str);
	  }
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("last put msg local_serial=%d i=%d"),
		    lw6dat_warehouse_get_local_serial (warehouse), i);

	lw6dat_warehouse_calc_serial_draft_and_reference (warehouse);
	seq_min = lw6dat_warehouse_get_seq_min (warehouse);
	seq_max = lw6dat_warehouse_get_seq_max (warehouse);
	seq_draft = lw6dat_warehouse_get_seq_draft (warehouse);
	seq_reference = lw6dat_warehouse_get_seq_reference (warehouse);
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("seq info min=%" LW6SYS_PRINTF_LL "d max=%"
			 LW6SYS_PRINTF_LL "d draft=%" LW6SYS_PRINTF_LL
			 "d reference=%" LW6SYS_PRINTF_LL "d"),
		    (long long) seq_min, (long long) seq_max,
		    (long long) seq_draft, (long long) seq_reference);

	warehouse2 =
	  lw6dat_warehouse_new (_TEST_WAREHOUSE_OTHER_NODE_ID,
				_TEST_WAREHOUSE_SEQ_0);
	if (warehouse2)
	  {
	    id_str = lw6sys_id_ltoa (_TEST_WAREHOUSE_LOCAL_NODE_ID);
	    if (id_str)
	      {
		msg =
		  lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %s %s",
				      (long long) (_TEST_WAREHOUSE_SEQ),
				      id_str, _TEST_WAREHOUSE_OTHER_NODE_MSG);
		if (msg)
		  {
		    if (lw6dat_warehouse_put_local_msg (warehouse2, msg))
		      {
			list_not_sent =
			  lw6dat_warehouse_get_atom_str_list_not_sent
			  (warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID);
			if (list_not_sent)
			  {
			    length = lw6sys_list_length (list_not_sent);
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_
					("putting %d atoms from warehouse to warehouse2"),
					length);
			    lw6sys_list_map (list_not_sent,
					     _test_warehouse_copy_atoms_callback,
					     (void *) warehouse2);
			    lw6dat_warehouse_calc_serial_draft_and_reference
			      (warehouse2);
			    seq_min =
			      lw6dat_warehouse_get_seq_min (warehouse2);
			    seq_max =
			      lw6dat_warehouse_get_seq_max (warehouse2);
			    seq_draft =
			      lw6dat_warehouse_get_seq_draft (warehouse2);
			    seq_reference =
			      lw6dat_warehouse_get_seq_reference (warehouse2);
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_
					("seq info min=%" LW6SYS_PRINTF_LL
					 "d max=%" LW6SYS_PRINTF_LL
					 "d draft=%" LW6SYS_PRINTF_LL
					 "d reference=%" LW6SYS_PRINTF_LL
					 "d"), (long long) seq_min,
					(long long) seq_max,
					(long long) seq_draft,
					(long long) seq_reference);
			    list_by_seq =
			      lw6dat_warehouse_get_msg_list_by_seq (warehouse,
								    seq_reference
								    + 1,
								    seq_draft,
								    1,
								    &progress);
			    if (list_by_seq)
			      {
				length = lw6sys_list_length (list_by_seq);
				list_by_seq2 =
				  lw6dat_warehouse_get_msg_list_by_seq
				  (warehouse2, seq_reference + 1, seq_draft,
				   0, &progress);
				if (list_by_seq2)
				  {
				    length2 =
				      lw6sys_list_length (list_by_seq2);
				    if (length2 == (length + 1))
				      {
					lw6sys_log (LW6SYS_LOG_NOTICE,
						    _x_
						    ("ok, length=%d length2=%d this means there's the same number of messages in both warehouses, only the 1st has been purged and the second contains one local message"),
						    length, length2);
				      }
				    else
				      {
					lw6sys_log (LW6SYS_LOG_WARNING,
						    _x_
						    ("length of lists mismatch length=%d length2=%d"),
						    length, length2);
					ret = 0;
				      }
				    miss_list =
				      lw6dat_warehouse_get_miss_list
				      (warehouse,
				       _TEST_WAREHOUSE_MISS_MAX_RANGE,
				       &progress);
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_ ("progress_value=%0.6f"),
						progress_value);
				    if (miss_list)
				      {
					miss_list_length =
					  lw6sys_list_length (miss_list);
					if (miss_list_length == 0)
					  {
					    lw6sys_log (LW6SYS_LOG_NOTICE,
							_x_
							("OK, miss_list contains nothing"));
					  }
					else
					  {
					    lw6sys_log (LW6SYS_LOG_NOTICE,
							_x_
							("miss_list contains %d members, should be empty"),
							miss_list_length);
					  }
					lw6sys_list_free (miss_list);
				      }
				    else
				      {
					lw6sys_log (LW6SYS_LOG_WARNING,
						    _x_
						    ("miss list is NULL, it could technically be empty, but here it's NULL, and this is not what we expect"));
					ret = 0;
				      }
				    lw6sys_list_free (list_by_seq2);
				  }
				lw6sys_list_free (list_by_seq);
			      }
			    lw6sys_list_free (list_not_sent);
			  }
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("unable to put message in other warehouse"));
			ret = 0;
		      }
		    LW6SYS_FREE (msg);
		  }
		LW6SYS_FREE (id_str);
	      }
	    lw6dat_warehouse_free (warehouse2);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("couldn't create warehouse2 object"));
	    ret = 0;
	  }
	lw6dat_warehouse_free (warehouse);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("couldn't create warehouse object"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6dat_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @dat module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dat_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6glb_test (mode);
      lw6nod_test (mode);
      lw6cnx_test (mode);
      lw6msg_test (mode);
    }

  ret = test_atom () && test_block () && test_miss () && test_stack ()
    && test_warehouse ();

  return ret;
}
