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

#include <math.h>
#include <CUnit/CUnit.h>

#include "dat-internal.h"

#define _TEST_ATOM_TEXT_SHORT "this is a short text"
#define _TEST_ATOM_TEXT_LONG "this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text..."

#define _TEST_BLOCK_ATOM_TYPE _LW6DAT_ATOM_TYPE_DATA
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
#define _TEST_STACK_ATOM_TYPE _LW6DAT_ATOM_TYPE_DATA
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
#define _TEST_STACK_SHIFT_SERIAL_DELTA 3
#define _TEST_STACK_SHIFT_SEQ_DELTA 10000000000LL
#define _TEST_STACK_SHIFT_REAL_WORLD_SERIAL_BASE 1
#define _TEST_STACK_SHIFT_REAL_WORLD_SERIAL_REAL 126
#define _TEST_STACK_SHIFT_REAL_WORLD_SEQ 10000000000011LL
#define _TEST_STACK_SHIFT_GUESSED_SERIAL_BASE 1
#define _TEST_STACK_SHIFT_GUESSED_SERIAL_REAL 129
#define _TEST_STACK_SHIFT_GUESSED_SEQ 10000000000011LL

#define _TEST_WAREHOUSE_LOCAL_NODE_ID 0x1234123412341234LL
#define _TEST_WAREHOUSE_OTHER_NODE_ID 0x2345234523452345LL
#define _TEST_WAREHOUSE_ATOM_TYPE _LW6DAT_ATOM_TYPE_DATA
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

#define _TEST_MORE_WAREHOUSE_NB_NODES 3
#define _TEST_MORE_WAREHOUSE_NB_MSGS_PER_STAGE 2
#define _TEST_MORE_WAREHOUSE_NODE_A_ID 0x1234123412341234LL
#define _TEST_MORE_WAREHOUSE_NODE_B_ID 0x2345234523452345LL
#define _TEST_MORE_WAREHOUSE_NODE_C_ID 0x3456345634563456LL
/*
 * On node A, there is 3 * a sequence of 1+4+1 = 6 messages
 * On node B, there is 3 * a sequence of 1+4+1+4+1 = 11 messages
 * On node C, there is 3 * a sequence of 1+4+1+4+1+4+1 = 16 messages
 *
 * On node A, everyone comes at its lower start, on node B the second
 * or later stage, on node C the last one.
 */
#define _TEST_MORE_WAREHOUSE_NODE_A_A_SERIAL 1
#define _TEST_MORE_WAREHOUSE_NODE_A_B_SERIAL 12
#define _TEST_MORE_WAREHOUSE_NODE_A_C_SERIAL 33
#define _TEST_MORE_WAREHOUSE_NODE_B_A_SERIAL 7
#define _TEST_MORE_WAREHOUSE_NODE_B_B_SERIAL 12
#define _TEST_MORE_WAREHOUSE_NODE_B_C_SERIAL 33
#define _TEST_MORE_WAREHOUSE_NODE_C_A_SERIAL 13
#define _TEST_MORE_WAREHOUSE_NODE_C_B_SERIAL 23
#define _TEST_MORE_WAREHOUSE_NODE_C_C_SERIAL 33
#define _TEST_MORE_WAREHOUSE_NODE_A_SEQ_0 10000000100000LL
#define _TEST_MORE_WAREHOUSE_NODE_B_SEQ_0 10000001000000LL
#define _TEST_MORE_WAREHOUSE_NODE_C_SEQ_0 10000010000000LL
#define _TEST_MORE_WAREHOUSE_NODE_A_SEQ_0_OFFSET 100
#define _TEST_MORE_WAREHOUSE_NODE_B_SEQ_0_OFFSET 1000
#define _TEST_MORE_WAREHOUSE_NODE_C_SEQ_0_OFFSET 10000
#define _TEST_MORE_WAREHOUSE_NODE_FINAL_SEQ 10000066666666LL
#define _TEST_MORE_WAREHOUSE_MSG_LENGTH_SHORT 10
#define _TEST_MORE_WAREHOUSE_MSG_CHAR_SHORT 's'
#define _TEST_MORE_WAREHOUSE_MSG_LENGTH_LONG 10000
#define _TEST_MORE_WAREHOUSE_MSG_CHAR_LONG 'l'
/*
 * Below are the actual values expected for the "not sent" messages
 * list length. It's important that they are different, for a given
 * warehouse, for all other warehouses, the idea is that depending
 * at which time the peer connected, some messages are irrelevant.
 * For instance if foo joins at seq 1010000000000 there's no point
 * in sending informations about what happened between 1000000000000
 * and 1010000000000 even if local node has done stuff during that
 * time.
 *
 * Note that 5 messages (3 short ones, 2 long ones) give a result
 * of 11 atomes (3+2*4) since long messages, after compression,
 * still give 4 atoms. One of the aspects of the test is to check
 * that messages spanned on several atoms behave well.
 */
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_0_0 0
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_0_1 19
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_0_2 19
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_1_0 23
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_1_1 0
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_1_2 23
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_2_0 17
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_2_1 17
#define _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_2_2 0
/*
 * Now this is the equivalent of the above, but figures
 * are different since now we'll be handling "cross"
 * messages, that is messages sent from 0 to 1 on behalf
 * of 2.
 */
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_0_0 0
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_0_1 17
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_0_2 23
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_1_0 17
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_1_1 0
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_1_2 13
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_2_0 12
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_2_1 7
#define _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_2_2 0

/*
 * All range limits, not that seq_max, seq_draft
 * and seq_reference are the same but seq_min differs
 */
#define _TEST_MORE_WAREHOUSE_SEQ_MIN_0 10000000100100LL
#define _TEST_MORE_WAREHOUSE_SEQ_MIN_1 10000000101000LL
#define _TEST_MORE_WAREHOUSE_SEQ_MIN_2 10000000110000LL
#define _TEST_MORE_WAREHOUSE_SEQ_MAX_0 10000066666666LL
#define _TEST_MORE_WAREHOUSE_SEQ_MAX_1 10000066666666LL
#define _TEST_MORE_WAREHOUSE_SEQ_MAX_2 10000066666666LL
#define _TEST_MORE_WAREHOUSE_SEQ_DRAFT_0 10000066666666LL
#define _TEST_MORE_WAREHOUSE_SEQ_DRAFT_1 10000066666666LL
#define _TEST_MORE_WAREHOUSE_SEQ_DRAFT_2 10000066666666LL
#define _TEST_MORE_WAREHOUSE_SEQ_REFERENCE_0 10000066666665LL
#define _TEST_MORE_WAREHOUSE_SEQ_REFERENCE_1 10000066666665LL
#define _TEST_MORE_WAREHOUSE_SEQ_REFERENCE_2 10000066666665LL

/*
 * Number of messages expected per type of message,
 * here we are talking of high level meaningfull
 * messages, not only compressed atoms.
 */
#define _TEST_MORE_WAREHOUSE_DRAFT_LEN_0 29
#define _TEST_MORE_WAREHOUSE_DRAFT_LEN_1 31
#define _TEST_MORE_WAREHOUSE_DRAFT_LEN_2 40
#define _TEST_MORE_WAREHOUSE_REFERENCE_LEN_0 26
#define _TEST_MORE_WAREHOUSE_REFERENCE_LEN_1 28
#define _TEST_MORE_WAREHOUSE_REFERENCE_LEN_2 37

typedef struct _lw6dat_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6dat_test_data_t;

static _lw6dat_test_data_t _test_data = { 0, NULL };

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
static void
_test_atom ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6dat_atom_t atom;
    char *text = NULL;
    //    char *cmd = NULL;

    _lw6dat_atom_zero (sys_context, &atom);
    if (!LW6SYS_TEST_ACK (!_lw6dat_atom_get_full_str (sys_context, &atom)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("get text returned something on a zeroed atom"));
	ret = 0;
      }
    _lw6dat_atom_set_full_str (sys_context, &atom, _TEST_ATOM_TEXT_SHORT);
    text = _lw6dat_atom_get_full_str (sys_context, &atom);
    if (LW6SYS_TEST_ACK (text))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_full_str (sys_context, &atom, _TEST_ATOM_TEXT_LONG);
    text = _lw6dat_atom_get_full_str (sys_context, &atom);
    if (LW6SYS_TEST_ACK (text))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_full_str (sys_context, &atom, _TEST_ATOM_TEXT_SHORT);
    text = _lw6dat_atom_get_full_str (sys_context, &atom);
    if (LW6SYS_TEST_ACK (text))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_full_str (sys_context, &atom, _TEST_ATOM_TEXT_LONG);
    text = _lw6dat_atom_get_full_str (sys_context, &atom);
    if (LW6SYS_TEST_ACK (text))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_clear (sys_context, &atom);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in block.c
 */
static void
_test_block ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int serial = 0;
    int order_i = 0;
    _lw6dat_block_t *block = NULL;
    _lw6dat_atom_t *atom = NULL;

    block = _lw6dat_block_new (sys_context, _TEST_BLOCK_SERIAL_0);
    if (LW6SYS_TEST_ACK (block))
      {
	for (serial = _TEST_BLOCK_SERIAL_0;
	     serial < _LW6DAT_NB_ATOMS_PER_BLOCK + _TEST_BLOCK_SERIAL_0; serial += _TEST_BLOCK_STEP, order_i += _TEST_BLOCK_STEP)
	  {
	    if (LW6SYS_TEST_ACK (_lw6dat_block_put_atom
				 (sys_context, block, _TEST_BLOCK_ATOM_TYPE, serial,
				  order_i, _TEST_BLOCK_ORDER_N,
				  _TEST_BLOCK_SEQ, _TEST_BLOCK_TEXT_1,
				  _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET, _TEST_BLOCK_CMD_STR_OFFSET, _TEST_BLOCK_SEND_FLAG)))
	      {
		// ok
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add atom into block serial=%d"), serial);
		ret = 0;
	      }
	  }
	if (LW6SYS_TEST_ACK (ret))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("added %d atoms"), _LW6DAT_NB_ATOMS_PER_BLOCK / _TEST_BLOCK_STEP);
	  }
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("trying various invalid atom \"put\" calls"));
	if (LW6SYS_TEST_ACK (!_lw6dat_block_put_atom
			     (sys_context, block, _TEST_BLOCK_ATOM_TYPE,
			      _TEST_BLOCK_SERIAL_0, 0, _TEST_BLOCK_ORDER_N,
			      _TEST_BLOCK_SEQ, _TEST_BLOCK_TEXT_2, _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET, _TEST_BLOCK_CMD_STR_OFFSET, _TEST_BLOCK_SEND_FLAG)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("duplicate detection OK"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong duplicate detection"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (!(_lw6dat_block_put_atom
			       (sys_context, block, _TEST_BLOCK_ATOM_TYPE,
				_TEST_BLOCK_SERIAL_0 - 1, 0,
				_TEST_BLOCK_ORDER_N, _TEST_BLOCK_SEQ,
				_TEST_BLOCK_TEXT_1,
				_TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET,
				_TEST_BLOCK_CMD_STR_OFFSET,
				_TEST_BLOCK_SEND_FLAG)
			       || _lw6dat_block_put_atom (sys_context, block,
							  _TEST_BLOCK_ATOM_TYPE,
							  _TEST_BLOCK_SERIAL_0
							  +
							  _LW6DAT_NB_ATOMS_PER_BLOCK,
							  0,
							  _TEST_BLOCK_ORDER_N,
							  _TEST_BLOCK_SEQ,
							  _TEST_BLOCK_TEXT_1,
							  _TEST_BLOCK_SEQ_FROM_CMD_STR_OFFSET, _TEST_BLOCK_CMD_STR_OFFSET, _TEST_BLOCK_SEND_FLAG))))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("range check OK"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong range check"));
	    ret = 0;
	  }
	atom = _lw6dat_block_get_atom (sys_context, block, _TEST_BLOCK_SERIAL_0);
	if (LW6SYS_TEST_ACK (atom))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got atom serial=%d"), atom->serial);
	    atom = _lw6dat_block_get_atom (sys_context, block, _TEST_BLOCK_SERIAL_0 + _TEST_BLOCK_STEP - 1);
	    if (LW6SYS_TEST_ACK (!atom))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, non-set atoms returned as NULL"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("non-set atom returns non-NULL"));
		ret = 0;
	      }
	  }
	_lw6dat_block_free (sys_context, block);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create block"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in miss.c
 */
static void
_test_miss ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6dat_miss_t *miss = NULL;
    lw6dat_miss_t miss2;

    miss = lw6dat_miss_new (sys_context, _TEST_MISS_FROM_ID, _TEST_MISS_SERIAL_MIN, _TEST_MISS_SERIAL_MAX);
    if (LW6SYS_TEST_ACK (miss))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("miss struct created from_id=%" LW6SYS_PRINTF_LL
			 "x serial_min=%d serial_%d"), (long long) miss->from_id, miss->serial_min, miss->serial_max);

	memset (&miss2, 0xff, sizeof (lw6dat_miss_t));
	if (LW6SYS_TEST_ACK (!lw6dat_miss_is_same (sys_context, miss, &miss2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("miss is_same works when different"));
	    lw6dat_miss_sync (sys_context, &miss2, miss);
	    if (LW6SYS_TEST_ACK (lw6dat_miss_is_same (sys_context, miss, &miss2)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("miss is_same works when equal"));
		if (LW6SYS_TEST_ACK (lw6dat_miss_is_included (sys_context, miss, &miss2)))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("miss is_included works when equal"));
		    if (LW6SYS_TEST_ACK (lw6dat_miss_overlaps (sys_context, miss, &miss2)))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("miss overlaps works when equal"));
			miss->serial_max++;
			if (LW6SYS_TEST_ACK (!lw6dat_miss_is_included (sys_context, miss, &miss2)))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("miss is_included works when different"));
			    if (LW6SYS_TEST_ACK (lw6dat_miss_overlaps (sys_context, miss, &miss2)))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("miss overlaps works when different"));
			      }
			    else
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("miss overlaps does not work when different"));
				ret = 0;
			      }
			  }
			else
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("miss is_included does not work when different"));
			    ret = 0;
			  }
		      }
		    else
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("miss overlaps does not work when equal"));
			ret = 0;
		      }
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("miss is_included does not work when equal"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("miss is_same does not work when equal"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("miss is_same does not work when different"));
	    ret = 0;
	  }
	lw6dat_miss_free (sys_context, miss);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("miss creation problem"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_stack_msg_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  _test_stack_msg_data_t *msg_data = (_test_stack_msg_data_t *) func_data;
  char *msg = (char *) data;

  if (lw6sys_str_is_same (sys_context, msg, _TEST_STACK_MSG_3))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message 3 found (length=%d)"), (int) strlen (msg));
      msg_data->msg3_found = 1;
    }
  if (lw6sys_str_is_same (sys_context, msg, msg_data->msg4))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message 4 found (length=%d)"), (int) strlen (msg));
      msg_data->msg4_found = 1;
    }
  if (lw6sys_str_is_same (sys_context, msg, _TEST_STACK_MSG_5))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message 5 found (length=%d)"), (int) strlen (msg));
      msg_data->msg5_found = 1;
    }
}

/*
 * Testing functions in stack.c
 */
static void
_test_stack ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6dat_stack_t stack;
    int i;
    int serial = LW6DAT_SERIAL_INVALID;
    int64_t seq = LW6DAT_SEQ_INVALID;
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
    _lw6dat_atom_t *tmp_atom = NULL;
    int64_t new_seq_0 = 0LL;

    _lw6dat_stack_zero (sys_context, &stack);
    _lw6dat_stack_init (sys_context, &stack, _TEST_STACK_NODE_ID, _TEST_STACK_SERIAL_0, _TEST_STACK_SEQ_0);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("putting %d atoms in stack"), _TEST_STACK_NB_RANDOM_PUT);
    for (i = 0; i < _TEST_STACK_NB_RANDOM_PUT; ++i)
      {
	serial = _TEST_STACK_SERIAL_0 + _TEST_STACK_RANDOM_BASE + lw6sys_random (sys_context, _TEST_STACK_RANDOM_RANGE);
	seq = _TEST_STACK_SEQ + i;
	if (!LW6SYS_TEST_ACK (_lw6dat_stack_put_atom
			      (sys_context, &stack, _TEST_STACK_ATOM_TYPE, serial,
			       _TEST_STACK_ORDER_I, _TEST_STACK_ORDER_N, seq,
			       _TEST_STACK_TEXT, _TEST_STACK_SEQ_FROM_CMD_STR_OFFSET, _TEST_STACK_CMD_STR_OFFSET, _TEST_STACK_SEND_FLAG)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error putting atom on stack i=%d serial=%d"), i, serial);
	    ret = 0;
	  }
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("last put serial=%d seq=%" LW6SYS_PRINTF_LL "d"), serial, (long long) seq);
    if (LW6SYS_TEST_ACK (ret))
      {
	/*
	 * The tests for seq2serial and serial2seq must
	 * be done now as serial and seq values are untouched.
	 */
	if (LW6SYS_TEST_ACK (_lw6dat_stack_seq2serial (sys_context, &stack, LW6DAT_SEQ_INVALID) == LW6DAT_SERIAL_INVALID))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("invalid seq number correctly treated by seq2serial"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid seq number *NOT* correctly treated by seq2serial"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (_lw6dat_stack_serial2seq (sys_context, &stack, LW6DAT_SERIAL_INVALID) == LW6DAT_SEQ_INVALID))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("invalid serial number correctly treated by serial2seq"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid serial number *NOT* correctly treated by serial2seq"));
	    ret = 0;
	  }
	tmp = _lw6dat_stack_serial2seq (sys_context, &stack, serial);
	if (LW6SYS_TEST_ACK (tmp == seq))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("serial2seq OK %d->%" LW6SYS_PRINTF_LL "d"), serial, (long long) seq);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("serial2seq problem %d->%" LW6SYS_PRINTF_LL "d instead of %" LW6SYS_PRINTF_LL "d"), serial, (long long) tmp, (long long) seq);
	    ret = 0;
	  }
	tmp = _lw6dat_stack_seq2serial (sys_context, &stack, seq);
	if (LW6SYS_TEST_ACK (tmp == serial))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("seq2serial OK %" LW6SYS_PRINTF_LL "d->%d"), (long long) seq, serial);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("seq2serial problem %" LW6SYS_PRINTF_LL "d->%" LW6SYS_PRINTF_LL "d instead of %d"), (long long) seq, (long long) tmp, serial);
	    ret = 0;
	  }
	for (serial = stack.serial_0_real; serial <= stack.serial_n_1; ++serial)
	  {
	    if (_lw6dat_stack_get_atom (sys_context, &stack, serial))
	      {
		found_not_null++;
	      }
	    else
	      {
		found_null++;
	      }
	  }
	if (LW6SYS_TEST_ACK (found_null && found_not_null))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("OK, found both NULL and non-NULL entries found_not_null=%d found_null=%d"), found_not_null, found_null);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("couldn't find both NULL and non-NULL entries found_not_null=%d found_null=%d"), found_not_null, found_null);
	    ret = 0;
	  }
	msg_list = _lw6dat_stack_init_list (sys_context);
	if (LW6SYS_TEST_ACK (msg_list))
	  {
	    _lw6dat_stack_update_atom_str_list_by_serial (sys_context, &stack, &msg_list, stack.serial_max);
	    length = lw6sys_list_length (sys_context, msg_list);
	    if (LW6SYS_TEST_ACK (length == 1))
	      {
		msg = (char *) lw6sys_list_pop_front (sys_context, &msg_list);
		if (LW6SYS_TEST_ACK (msg))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got atom str by serial \"%s\""), msg);
		    LW6SYS_FREE (sys_context, msg);
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("there should be an atom at serial %d, unable to get it"), stack.serial_max);
		ret = 0;
	      }

	    _lw6dat_stack_update_atom_str_list_not_sent (sys_context, &stack, &msg_list, _TEST_STACK_TARGET_INDEX);
	    length = lw6sys_list_length (sys_context, msg_list);
	    if (LW6SYS_TEST_ACK (length == found_not_null))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got %d unsent atoms"), length);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error getting list of atoms length=%d found_not_null=%d"), length, found_not_null);
		ret = 0;
	      }
	    while (!lw6sys_list_is_empty (sys_context, msg_list))
	      {
		msg = (char *) lw6sys_list_pop_front (sys_context, &msg_list);
		if (msg)
		  {
		    LW6SYS_FREE (sys_context, msg);
		  }
	      }

	    _lw6dat_stack_miss_invalidate (sys_context, &stack, _TEST_STACK_TARGET_INDEX, stack.serial_min, stack.serial_max);
	    _lw6dat_stack_update_atom_str_list_not_sent (sys_context, &stack, &msg_list, _TEST_STACK_TARGET_INDEX);
	    length = lw6sys_list_length (sys_context, msg_list);
	    if (LW6SYS_TEST_ACK (length == found_not_null))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got %d unsent atoms"), length);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error getting list of atoms length=%d found_not_null=%d"), length, found_not_null);
		ret = 0;
	      }
	    while (!lw6sys_list_is_empty (sys_context, msg_list))
	      {
		msg = (char *) lw6sys_list_pop_front (sys_context, &msg_list);
		if (msg)
		  {
		    LW6SYS_FREE (sys_context, msg);
		  }
	      }

	    _lw6dat_stack_update_atom_str_list_not_sent (sys_context, &stack, &msg_list, _TEST_STACK_TARGET_INDEX);
	    length = lw6sys_list_length (sys_context, msg_list);
	    if (LW6SYS_TEST_ACK (length == 0))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got no more unsent atoms on second call"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error getting list of atoms length=%d, should be 0"), length);
		ret = 0;
	      }
	    lw6sys_list_free (sys_context, msg_list);
	  }
      }

    /*
     * We purge the whole stuff, we don't want previous random
     * data to interfere
     */
    _lw6dat_stack_purge (sys_context, &stack);

    msg4_random_part = lw6sys_str_random_words (sys_context, _TEST_STACK_MSG_4_RANDOM_PART_SIZE);
    if (LW6SYS_TEST_ACK (msg4_random_part))
      {
	msg4 = lw6sys_new_sprintf (sys_context, _TEST_STACK_MSG_4, msg4_random_part);
	if (LW6SYS_TEST_ACK (msg4))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("put a few messages in stack to test recollection of atoms into a full message"));
	    if (LW6SYS_TEST_ACK (_lw6dat_stack_put_msg
				 (sys_context, &stack, NULL, _TEST_STACK_MSG_1,
				  _TEST_STACK_SEND_FLAG)
				 && _lw6dat_stack_put_msg (sys_context, &stack, NULL,
							   _TEST_STACK_MSG_2,
							   _TEST_STACK_SEND_FLAG)
				 && _lw6dat_stack_put_msg (sys_context, &stack, NULL,
							   _TEST_STACK_MSG_3,
							   _TEST_STACK_SEND_FLAG)
				 && _lw6dat_stack_put_msg (sys_context, &stack, NULL, msg4,
							   _TEST_STACK_SEND_FLAG)
				 && _lw6dat_stack_put_msg (sys_context, &stack, NULL,
							   _TEST_STACK_MSG_5,
							   _TEST_STACK_SEND_FLAG)
				 && _lw6dat_stack_put_msg (sys_context, &stack, NULL, _TEST_STACK_MSG_6, _TEST_STACK_SEND_FLAG)))
	      {
		msg_list = _lw6dat_stack_init_list (sys_context);
		if (LW6SYS_TEST_ACK (msg_list))
		  {
		    _lw6dat_stack_update_msg_list_by_seq (sys_context, &stack, &msg_list, _TEST_STACK_MSG_GET_SEQ, 1, 1, &worst_msg_i, &worst_msg_n);
		    length = lw6sys_list_length (sys_context, msg_list);
		    if (LW6SYS_TEST_ACK (length == _TEST_STACK_MSG_GET_NB))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, got %d messages"), length);
			msg_data.msg4 = msg4;
			lw6sys_list_map (sys_context, msg_list, _test_stack_msg_callback, (void *) &msg_data);
			if (LW6SYS_TEST_ACK (msg_data.msg3_found && msg_data.msg4_found && msg_data.msg5_found))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("all messages OK"));
			  }
			else
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("some messages were wrong"));
			    ret = 0;
			  }
		      }
		    else
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error, should have got %d messages and got %d"), _TEST_STACK_MSG_GET_NB, length);
			ret = 0;
		      }
		    lw6sys_list_free (sys_context, msg_list);

		    miss = _lw6dat_stack_get_miss (sys_context, &stack, _TEST_STACK_MISS_MAX_RANGE, &worst_msg_i, &worst_msg_n);
		    if (LW6SYS_TEST_ACK (miss))
		      {
			if (LW6SYS_TEST_ACK (miss->serial_min > stack.serial_max))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					_x_ ("miss from_id=%" LW6SYS_PRINTF_LL
					     "x miss->serial_min=%d miss->serial_max=%d stack.serial_min=%d stack.serial_max=%d worst_msg_i=%d worst_msg_n=%d, since the range is outside (later than) our current data, it will simply cause a new fetch, this does not harm (and can help if remote host is stall)"),
					(long long) miss->from_id,
					miss->serial_min, miss->serial_max, stack.serial_min, stack.serial_max, worst_msg_i, worst_msg_n);
			  }
			else
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					_x_ ("miss from_id=%" LW6SYS_PRINTF_LL
					     "x miss->serial_min=%d miss->serial_max=%d stack.serial_min=%d stack.serial_max=%d worst_msg_i=%d worst_msg_n=%d, normally all messages should be complete"),
					(long long) miss->from_id,
					miss->serial_min, miss->serial_max, stack.serial_min, stack.serial_max, worst_msg_i, worst_msg_n);
			    ret = 0;
			  }
			lw6dat_miss_free (sys_context, miss);
		      }
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to put one of the messages"));
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, msg4);
	  }
	LW6SYS_FREE (sys_context, msg4_random_part);
      }

    if (LW6SYS_TEST_ACK (_lw6dat_stack_shift (sys_context, &stack, stack.serial_0_real, stack.seq_0)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("shifted stack with same values OK"));
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("shifted stack with same values failed"));
	ret = 0;
      }

    if (LW6SYS_TEST_ACK
	(!_lw6dat_stack_shift (sys_context, &stack, stack.serial_0_base - _TEST_STACK_SHIFT_SERIAL_DELTA, stack.seq_0 - _TEST_STACK_SHIFT_SEQ_DELTA)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("shifted stack with inconsistent values did not work, this is fine"));
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("shifted stack with inconsistent values worked, it should have raised an error"));
	ret = 0;
      }

    tmp_atom = _lw6dat_stack_get_atom (sys_context, &stack, _TEST_STACK_SERIAL_0 + _TEST_STACK_SHIFT_SERIAL_DELTA);
    if (LW6SYS_TEST_ACK (tmp_atom))
      {
	new_seq_0 = tmp_atom->seq;
	if (LW6SYS_TEST_ACK (_lw6dat_stack_shift (sys_context, &stack, _TEST_STACK_SERIAL_0 + _TEST_STACK_SHIFT_SERIAL_DELTA, new_seq_0)))
	  {
	    tmp_atom = _lw6dat_stack_get_atom (sys_context, &stack, _TEST_STACK_SERIAL_0 + _TEST_STACK_SHIFT_SERIAL_DELTA);
	    if (LW6SYS_TEST_ACK (tmp_atom))
	      {
		if (LW6SYS_TEST_ACK (stack.serial_0_base ==
				     _TEST_STACK_SHIFT_REAL_WORLD_SERIAL_BASE
				     && stack.serial_0_real == _TEST_STACK_SHIFT_REAL_WORLD_SERIAL_REAL && tmp_atom->seq == _TEST_STACK_SHIFT_REAL_WORLD_SEQ))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_
				("shifted stack with real world delta OK serial_0_base=%d serial_0_real=%d seq=%"
				 LW6SYS_PRINTF_LL "d"), stack.serial_0_base, stack.serial_0_real, (long long) tmp_atom->seq);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_
				("shifted stack with real world delta OK serial_0_base=%d serial_0_real=%d seq=%"
				 LW6SYS_PRINTF_LL "d"), stack.serial_0_base, stack.serial_0_real, (long long) tmp_atom->seq);
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get first atom"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("shifted stack with real world delta failed"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get atom for serial %d"), _TEST_STACK_SERIAL_0 + _TEST_STACK_SHIFT_SERIAL_DELTA);
	ret = 0;
      }

    if (LW6SYS_TEST_ACK (_lw6dat_stack_shift (sys_context, &stack, _TEST_STACK_SERIAL_0 + 2 * _TEST_STACK_SHIFT_SERIAL_DELTA, LW6DAT_SEQ_INVALID)))
      {
	tmp_atom = _lw6dat_stack_get_atom (sys_context, &stack, _TEST_STACK_SERIAL_0 + 2 * _TEST_STACK_SHIFT_SERIAL_DELTA);
	if (LW6SYS_TEST_ACK (tmp_atom))
	  {
	    if (LW6SYS_TEST_ACK
		(stack.serial_0_base == _TEST_STACK_SHIFT_GUESSED_SERIAL_BASE
		 && stack.serial_0_real == _TEST_STACK_SHIFT_GUESSED_SERIAL_REAL && tmp_atom->seq == _TEST_STACK_SHIFT_GUESSED_SEQ))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_
			    ("shifted stack with guessed delta OK serial_0_base=%d serial_0_real=%d seq=%"
			     LW6SYS_PRINTF_LL "d"), stack.serial_0_base, stack.serial_0_real, (long long) tmp_atom->seq);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_
			    ("shifted stack with guessed delta OK serial_0_base=%d serial_0_real=%d seq=%"
			     LW6SYS_PRINTF_LL "d"), stack.serial_0_base, stack.serial_0_real, (long long) tmp_atom->seq);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get first atom"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("shifted stack with guessed delta failed"));
	ret = 0;
      }

    _lw6dat_stack_clear (sys_context, &stack);
    _lw6dat_stack_clear (sys_context, &stack);	// call it twice, to check it's allowed
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_warehouse_copy_atoms_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  lw6dat_warehouse_t *warehouse2 = (lw6dat_warehouse_t *) func_data;
  char *atom_str = (char *) data;

  if (LW6SYS_TEST_ACK (lw6dat_warehouse_put_atom_str (sys_context, warehouse2, _TEST_WAREHOUSE_LOCAL_NODE_ID, atom_str)))
    {
      // ok
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error copying atom \"%s\""), atom_str);
    }
}

static void
_test_miss_print_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  lw6dat_miss_t *miss = (lw6dat_miss_t *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
	      _x_ ("MISS item from_id=%" LW6SYS_PRINTF_LL "x serial_min=%d serial_max=%d"), (long long) miss->from_id, miss->serial_min, miss->serial_max);
}

/*
 * Testing functions in warehouse.c
 */
static void
_test_warehouse ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6dat_warehouse_t *_warehouse;
    lw6dat_warehouse_t *warehouse;
    lw6dat_warehouse_t *warehouse2;
    int overflow = 0;
    int i = 0;
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

    lw6sys_progress_default (sys_context, &progress, &progress_value);
    lw6sys_progress_begin (sys_context, &progress);

    _warehouse = _lw6dat_warehouse_new (sys_context, _TEST_WAREHOUSE_LOCAL_NODE_ID, _TEST_WAREHOUSE_SEQ_0);
    if (LW6SYS_TEST_ACK (_warehouse))
      {
	if (LW6SYS_TEST_ACK (_lw6dat_warehouse_put_atom
			     (sys_context, _warehouse, _TEST_WAREHOUSE_LOCAL_NODE_ID,
			      _TEST_WAREHOUSE_ATOM_TYPE,
			      _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_ORDER_I,
			      _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_SEQ,
			      _TEST_WAREHOUSE_TEXT, _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET, _TEST_WAREHOUSE_CMD_STR_OFFSET)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("text \"%s\" put into warehouse"), _TEST_WAREHOUSE_TEXT);
	  }
	for (i = 0; i < _TEST_WAREHOUSE_NB_STACKS_OVERFLOW; ++i)
	  {
	    if (!_lw6dat_warehouse_put_atom
		(sys_context, _warehouse, lw6sys_generate_id_64 (sys_context),
		 _TEST_WAREHOUSE_ATOM_TYPE, _TEST_WAREHOUSE_SERIAL,
		 _TEST_WAREHOUSE_ORDER_I, _TEST_WAREHOUSE_ORDER_N,
		 _TEST_WAREHOUSE_SEQ, _TEST_WAREHOUSE_TEXT, _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET, _TEST_WAREHOUSE_CMD_STR_OFFSET))
	      {
		overflow = 1;
	      }
	  }
	if (LW6SYS_TEST_ACK (overflow))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("OK, warehouse has a stack number limit (%d) this is right"), _lw6dat_warehouse_get_nb_nodes (sys_context, _warehouse));
	  }
	else
	  {
	    ret = 0;
	  }
	_lw6dat_warehouse_purge (sys_context, _warehouse);
	_lw6dat_warehouse_clear (sys_context, _warehouse);
	_lw6dat_warehouse_free (sys_context, _warehouse);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't create _warehouse object"));
	ret = 0;
      }

    _warehouse = _lw6dat_warehouse_new (sys_context, _TEST_WAREHOUSE_LOCAL_NODE_ID, _TEST_WAREHOUSE_SEQ_0);
    if (LW6SYS_TEST_ACK (_warehouse))
      {
	_lw6dat_warehouse_register_node (sys_context, _warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID, _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_SEQ);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("putting %d atoms in warehouse"), _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW);
	if (LW6SYS_TEST_ACK
	    (_lw6dat_warehouse_put_atom
	     (sys_context, _warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
	      _TEST_WAREHOUSE_ATOM_TYPE, _TEST_WAREHOUSE_SERIAL,
	      _TEST_WAREHOUSE_ORDER_I, _TEST_WAREHOUSE_ORDER_N,
	      _TEST_WAREHOUSE_SEQ, _TEST_WAREHOUSE_TEXT, _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET, _TEST_WAREHOUSE_CMD_STR_OFFSET)))
	  {
	    for (i = 1; i < _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW; ++i)
	      {
		if (!LW6SYS_TEST_ACK (_lw6dat_warehouse_put_atom
				      (sys_context, _warehouse,
				       _TEST_WAREHOUSE_OTHER_NODE_ID,
				       _TEST_WAREHOUSE_ATOM_TYPE,
				       _TEST_WAREHOUSE_SERIAL + i,
				       _TEST_WAREHOUSE_ORDER_I,
				       _TEST_WAREHOUSE_ORDER_N,
				       _TEST_WAREHOUSE_SEQ, _TEST_WAREHOUSE_TEXT, _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET, _TEST_WAREHOUSE_CMD_STR_OFFSET)))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to put atom %d"), i);
		    ret = 0;
		  }
	      }
	    if (LW6SYS_TEST_ACK (_lw6dat_warehouse_put_atom
				 (sys_context, _warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
				  _TEST_WAREHOUSE_ATOM_TYPE,
				  _TEST_WAREHOUSE_SERIAL,
				  _TEST_WAREHOUSE_ORDER_I,
				  _TEST_WAREHOUSE_ORDER_N,
				  _TEST_WAREHOUSE_SEQ, _TEST_WAREHOUSE_TEXT, _TEST_WAREHOUSE_SEQ_FROM_CMD_STR_OFFSET, _TEST_WAREHOUSE_CMD_STR_OFFSET)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could put again 1st atom in warehouse, shifting should work"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't put (again) 1st atom in warehouse"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't put 1st atom in warehouse"));
	    ret = 0;
	  }

	_lw6dat_warehouse_free (sys_context, _warehouse);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't create _warehouse object"));
	ret = 0;
      }

    warehouse = lw6dat_warehouse_new (sys_context, _TEST_WAREHOUSE_LOCAL_NODE_ID, _TEST_WAREHOUSE_SEQ_0);
    if (LW6SYS_TEST_ACK (warehouse))
      {
	if (!LW6SYS_TEST_ACK (lw6dat_warehouse_get_local_id (sys_context, warehouse) == _TEST_WAREHOUSE_LOCAL_NODE_ID))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("lw6dat_warehouse_get_local_id failed"));
	    ret = 0;
	  }
	_lw6dat_warehouse_register_node (sys_context, _warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID, _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_SEQ);
	if (LW6SYS_TEST_ACK (lw6dat_warehouse_put_atom_str (sys_context, warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID, _TEST_WAREHOUSE_SERIAL_I_N_MSG)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lw6dat_warehouse_put_atom_str seem to work"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("lw6dat_warehouse_put_atom_str failed"));
	    ret = 0;
	  }
	nb_atom_parts_since_last_poll = lw6dat_warehouse_get_nb_atom_parts_since_last_poll (sys_context, warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID);
	if (LW6SYS_TEST_ACK (nb_atom_parts_since_last_poll > 0))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, got %d atom parts since last poll"), nb_atom_parts_since_last_poll);
	    lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (sys_context, warehouse);
	    nb_atom_parts_since_last_poll = lw6dat_warehouse_get_nb_atom_parts_since_last_poll (sys_context, warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID);
	    if (LW6SYS_TEST_ACK (nb_atom_parts_since_last_poll == 0))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, got no atom parts since last poll"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("got %d atom parts since last poll but there should be none"), nb_atom_parts_since_last_poll);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("got no atom parts since last poll"));
	    ret = 0;
	  }

	lw6dat_warehouse_purge (sys_context, warehouse);

	id_str = lw6sys_id_ltoa (sys_context, _TEST_WAREHOUSE_LOCAL_NODE_ID);
	if (LW6SYS_TEST_ACK (id_str))
	  {
	    for (i = _TEST_WAREHOUSE_PUT_MIN_SIZE; i < _TEST_WAREHOUSE_PUT_MAX_SIZE; ++i)
	      {
		cmd = lw6sys_str_random_words (sys_context, i);
		if (LW6SYS_TEST_ACK (cmd))
		  {
		    msg = lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %s %s", (long long) (_TEST_WAREHOUSE_SEQ + (int) sqrt (i)), id_str, cmd);
		    if (LW6SYS_TEST_ACK (msg))
		      {
			if (LW6SYS_TEST_ACK (lw6dat_warehouse_put_local_msg (sys_context, warehouse, msg)))
			  {
			    if (LW6SYS_TEST_ACK
				(lw6dat_warehouse_meta_put (sys_context, warehouse, lw6dat_warehouse_get_local_seq_last (sys_context, warehouse))))
			      {
				// ok
			      }
			    else
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to put meta"));
				ret = 0;
			      }
			  }
			else
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to put msg \"%s\" into warehouse"), msg);
			    ret = 0;
			  }
			LW6SYS_FREE (sys_context, msg);
		      }
		    LW6SYS_FREE (sys_context, cmd);
		  }
	      }
	    LW6SYS_FREE (sys_context, id_str);
	  }
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("last put msg local_serial=%d i=%d"), lw6dat_warehouse_get_local_serial (sys_context, warehouse), i);

	lw6dat_warehouse_calc_serial_draft_and_reference (sys_context, warehouse);
	seq_min = lw6dat_warehouse_get_seq_min (sys_context, warehouse);
	seq_max = lw6dat_warehouse_get_seq_max (sys_context, warehouse);
	seq_draft = lw6dat_warehouse_get_seq_draft (sys_context, warehouse);
	seq_reference = lw6dat_warehouse_get_seq_reference (sys_context, warehouse);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("seq info min=%" LW6SYS_PRINTF_LL "d max=%"
			 LW6SYS_PRINTF_LL "d draft=%" LW6SYS_PRINTF_LL
			 "d reference=%" LW6SYS_PRINTF_LL "d"), (long long) seq_min, (long long) seq_max, (long long) seq_draft, (long long) seq_reference);

	warehouse2 = lw6dat_warehouse_new (sys_context, _TEST_WAREHOUSE_OTHER_NODE_ID, _TEST_WAREHOUSE_SEQ_0);
	if (LW6SYS_TEST_ACK (warehouse2))
	  {
	    lw6dat_warehouse_register_node (sys_context, warehouse2, _TEST_WAREHOUSE_LOCAL_NODE_ID, _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_SEQ);
	    id_str = lw6sys_id_ltoa (sys_context, _TEST_WAREHOUSE_LOCAL_NODE_ID);
	    if (LW6SYS_TEST_ACK (id_str))
	      {
		msg =
		  lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %s %s", (long long) (_TEST_WAREHOUSE_SEQ), id_str, _TEST_WAREHOUSE_OTHER_NODE_MSG);
		if (LW6SYS_TEST_ACK (msg))
		  {
		    if (LW6SYS_TEST_ACK (lw6dat_warehouse_put_local_msg (sys_context, warehouse2, msg)))
		      {
			list_not_sent = lw6dat_warehouse_get_atom_str_list_not_sent (sys_context, warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID);
			if (LW6SYS_TEST_ACK (list_not_sent))
			  {
			    length = lw6sys_list_length (sys_context, list_not_sent);
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("putting %d atoms from warehouse to warehouse2"), length);
			    lw6sys_list_map (sys_context, list_not_sent, _test_warehouse_copy_atoms_callback, (void *) warehouse2);
			    lw6dat_warehouse_calc_serial_draft_and_reference (sys_context, warehouse2);
			    seq_min = lw6dat_warehouse_get_seq_min (sys_context, warehouse2);
			    seq_max = lw6dat_warehouse_get_seq_max (sys_context, warehouse2);
			    seq_draft = lw6dat_warehouse_get_seq_draft (sys_context, warehouse2);
			    seq_reference = lw6dat_warehouse_get_seq_reference (sys_context, warehouse2);
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					_x_
					("seq info min=%" LW6SYS_PRINTF_LL
					 "d max=%" LW6SYS_PRINTF_LL
					 "d draft=%" LW6SYS_PRINTF_LL
					 "d reference=%" LW6SYS_PRINTF_LL
					 "d"), (long long) seq_min, (long long) seq_max, (long long) seq_draft, (long long) seq_reference);
			    list_by_seq = lw6dat_warehouse_get_msg_list_by_seq (sys_context, warehouse, seq_reference + 1, seq_draft, 1, &progress);
			    if (LW6SYS_TEST_ACK (list_by_seq))
			      {
				length = lw6sys_list_length (sys_context, list_by_seq);
				list_by_seq2 = lw6dat_warehouse_get_msg_list_by_seq (sys_context, warehouse2, seq_reference + 1, seq_draft, 0, &progress);
				if (LW6SYS_TEST_ACK (list_by_seq2))
				  {
				    length2 = lw6sys_list_length (sys_context, list_by_seq2);
				    if (LW6SYS_TEST_ACK (length2 == (length + 1)))
				      {
					lw6sys_log (sys_context,
						    LW6SYS_LOG_NOTICE,
						    _x_
						    ("ok, length=%d length2=%d this means there's the same number of messages in both warehouses, only the 1st has been purged and the second contains one local message"),
						    length, length2);
				      }
				    else
				      {
					lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("length of lists mismatch length=%d length2=%d"), length, length2);
					ret = 0;
				      }
				    miss_list = lw6dat_warehouse_get_miss_list (sys_context, warehouse, _TEST_WAREHOUSE_MISS_MAX_RANGE, &progress);
				    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("progress_value=%0.6f"), progress_value);
				    if (LW6SYS_TEST_ACK (miss_list))
				      {
					miss_list_length = lw6sys_list_length (sys_context, miss_list);
					if (LW6SYS_TEST_ACK (miss_list_length == 0))
					  {
					    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, miss_list contains nothing"));
					  }
					else
					  {
					    lw6sys_log (sys_context,
							LW6SYS_LOG_NOTICE, _x_ ("miss_list contains %d members, should be empty"), miss_list_length);
					    lw6sys_list_map (sys_context, miss_list, _test_miss_print_callback, NULL);
					  }
					lw6sys_list_free (sys_context, miss_list);
				      }
				    else
				      {
					lw6sys_log (sys_context,
						    LW6SYS_LOG_WARNING,
						    _x_
						    ("miss list is NULL, it could technically be empty, but here it's NULL, and this is not what we expect"));
					ret = 0;
				      }
				    lw6sys_list_free (sys_context, list_by_seq2);
				  }
				lw6sys_list_free (sys_context, list_by_seq);
			      }
			    lw6sys_list_free (sys_context, list_not_sent);
			  }
			else
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("list_not_sent is NULL"));
			    ret = 0;
			  }
		      }
		    else
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to put message in other warehouse"));
			ret = 0;
		      }
		    LW6SYS_FREE (sys_context, msg);
		  }
		LW6SYS_FREE (sys_context, id_str);
	      }
	    lw6dat_warehouse_free (sys_context, warehouse2);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't create warehouse2 object"));
	    ret = 0;
	  }
	lw6dat_warehouse_free (sys_context, warehouse);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't create warehouse object"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_print_msg_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  const char *msg = (const char *) data;
  int len = 0;
  int type = 0;
  int serial = 0;
  int order_i = 0;
  int order_n = 0;
  int64_t seq = 0;
  u_int64_t logical_from2 = 0LL;
  int seq_from_cmd_str_offset = 0;
  int cmd_str_offset = 0;

  if (LW6SYS_TEST_ACK ((!func_data) && msg))
    {
      len = strlen (msg);
      _lw6dat_atom_parse_from_cmd (sys_context, &type, &serial, &order_i, &order_n, &seq, &logical_from2, &seq_from_cmd_str_offset, &cmd_str_offset, msg);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		  _x_
		  ("serial=%d order_i=%d order_n=%d seq=%"
		   LW6SYS_PRINTF_LL "d logicial_from=%" LW6SYS_PRINTF_LL
		   "x len=%d"), serial, order_i, order_n, (long long) seq, (long long) logical_from2, len);
    }
}

typedef struct _fake_send_data_s
{
  lw6dat_warehouse_t *warehouse;
  u_int64_t logical_from;
  int ret;
} _fake_send_data_t;

static void
_fake_send_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  _fake_send_data_t *fake_send_data = (_fake_send_data_t *) func_data;
  const char *msg = (const char *) data;
  int len = 0;
  int type = 0;
  int serial = 0;
  int order_i = 0;
  int order_n = 0;
  int64_t seq = 0;
  u_int64_t logical_from2 = 0LL;
  int seq_from_cmd_str_offset = 0;
  int cmd_str_offset = 0;

  if (LW6SYS_TEST_ACK (fake_send_data && msg))
    {
      len = strlen (msg);

      /*
       * Now, in a real network context, the logical
       * sender can been parsed upstream but here we
       * need to figure it out "manually".
       */
      _lw6dat_atom_parse_from_cmd (sys_context, &type, &serial, &order_i, &order_n, &seq, &logical_from2, &seq_from_cmd_str_offset, &cmd_str_offset, msg);
      if (fake_send_data->logical_from)
	{
	  logical_from2 = fake_send_data->logical_from;
	}
      if (LW6SYS_TEST_ACK (lw6dat_warehouse_put_atom_str (sys_context, fake_send_data->warehouse, logical_from2, msg)))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		      _x_
		      ("faking net send/recv and putting msg of length %d into warehouse (%"
		       LW6SYS_PRINTF_LL "x -> %" LW6SYS_PRINTF_LL
		       "x serial=%d seq=%" LW6SYS_PRINTF_LL "d)"), len,
		      (long long) logical_from2, (long long) lw6dat_warehouse_get_local_id (sys_context, fake_send_data->warehouse), serial, (long long) seq);
	  if (!LW6SYS_TEST_ACK (serial && seq))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_ ("strange, serial=%d and seq=%" LW6SYS_PRINTF_LL "d msg was \"%s\""), serial, (long long) seq, msg);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("failed to put message \"%s\" into warehouse"), msg);
	  fake_send_data->ret = 0;
	}
    }
}

/*
 * Even more tests (paranoid mode)
 */
static void
_test_more ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6dat_warehouse_t *warehouse[_TEST_MORE_WAREHOUSE_NB_NODES] = { NULL, NULL, NULL };
    int warehouse_index = 0;
    int stage = 0;
    int node_index = 0;
    int64_t msg_index = 0LL;
    int64_t seq = 0LL;
    u_int64_t node_id = 0LL;
    u_int64_t node_ids[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_NODE_A_ID, _TEST_MORE_WAREHOUSE_NODE_B_ID,
      _TEST_MORE_WAREHOUSE_NODE_C_ID
    };
    int node_serials[_TEST_MORE_WAREHOUSE_NB_NODES][_TEST_MORE_WAREHOUSE_NB_NODES] = {
      {_TEST_MORE_WAREHOUSE_NODE_A_A_SERIAL,
       _TEST_MORE_WAREHOUSE_NODE_A_B_SERIAL,
       _TEST_MORE_WAREHOUSE_NODE_A_C_SERIAL},
      {_TEST_MORE_WAREHOUSE_NODE_B_A_SERIAL,
       _TEST_MORE_WAREHOUSE_NODE_B_B_SERIAL,
       _TEST_MORE_WAREHOUSE_NODE_B_C_SERIAL},
      {_TEST_MORE_WAREHOUSE_NODE_C_A_SERIAL,
       _TEST_MORE_WAREHOUSE_NODE_C_B_SERIAL,
       _TEST_MORE_WAREHOUSE_NODE_C_C_SERIAL}
    }
    ;
    int64_t node_seq_0s[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_NODE_A_SEQ_0, _TEST_MORE_WAREHOUSE_NODE_B_SEQ_0,
      _TEST_MORE_WAREHOUSE_NODE_C_SEQ_0
    };
    int node_seq_0_offsets[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_NODE_A_SEQ_0_OFFSET,
      _TEST_MORE_WAREHOUSE_NODE_B_SEQ_0_OFFSET,
      _TEST_MORE_WAREHOUSE_NODE_C_SEQ_0_OFFSET
    };
    int nb_init_not_sent[_TEST_MORE_WAREHOUSE_NB_NODES][_TEST_MORE_WAREHOUSE_NB_NODES] = { {_TEST_MORE_WAREHOUSE_INIT_NOT_SENT_0_0,
											    _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_0_1,
											    _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_0_2},
    {_TEST_MORE_WAREHOUSE_INIT_NOT_SENT_1_0,
     _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_1_1,
     _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_1_2},
    {_TEST_MORE_WAREHOUSE_INIT_NOT_SENT_2_0,
     _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_2_1,
     _TEST_MORE_WAREHOUSE_INIT_NOT_SENT_2_2}
    };
    int nb_cross_not_sent[_TEST_MORE_WAREHOUSE_NB_NODES][_TEST_MORE_WAREHOUSE_NB_NODES] = { {_TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_0_0,
											     _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_0_1,
											     _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_0_2},
    {_TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_1_0,
     _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_1_1,
     _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_1_2},
    {_TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_2_0,
     _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_2_1,
     _TEST_MORE_WAREHOUSE_CROSS_NOT_SENT_2_2}
    };
    lw6sys_list_t *not_sent_list[_TEST_MORE_WAREHOUSE_NB_NODES][_TEST_MORE_WAREHOUSE_NB_NODES];
    char *msg = NULL;
    char *short_text = NULL;
    char *long_text = NULL;
    int i = 0;
    int not_sent_length = 0;
    _fake_send_data_t fake_send_data;
    int64_t seq_min = 0LL;
    int64_t seq_max = 0LL;
    int64_t seq_draft = 0LL;
    int64_t seq_reference = 0LL;
    lw6sys_list_t *msg_list = NULL;
    int64_t seq_mins[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_SEQ_MIN_0, _TEST_MORE_WAREHOUSE_SEQ_MIN_1,
      _TEST_MORE_WAREHOUSE_SEQ_MIN_2
    };
    int64_t seq_maxs[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_SEQ_MAX_0, _TEST_MORE_WAREHOUSE_SEQ_MAX_1,
      _TEST_MORE_WAREHOUSE_SEQ_MAX_2
    };
    int64_t seq_drafts[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_SEQ_DRAFT_0, _TEST_MORE_WAREHOUSE_SEQ_DRAFT_1,
      _TEST_MORE_WAREHOUSE_SEQ_DRAFT_2
    };
    int64_t seq_references[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_SEQ_REFERENCE_0,
      _TEST_MORE_WAREHOUSE_SEQ_REFERENCE_1,
      _TEST_MORE_WAREHOUSE_SEQ_REFERENCE_2
    };
    int draft_len = 0;
    int reference_len = 0;
    int draft_lens[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_DRAFT_LEN_0, _TEST_MORE_WAREHOUSE_DRAFT_LEN_1,
      _TEST_MORE_WAREHOUSE_DRAFT_LEN_2
    };
    int reference_lens[_TEST_MORE_WAREHOUSE_NB_NODES] = { _TEST_MORE_WAREHOUSE_REFERENCE_LEN_0,
      _TEST_MORE_WAREHOUSE_REFERENCE_LEN_1,
      _TEST_MORE_WAREHOUSE_REFERENCE_LEN_2
    };
    lw6sys_list_t *miss_list = NULL;
    int miss_list_length = 0;

    short_text = LW6SYS_MALLOC (sys_context, _TEST_MORE_WAREHOUSE_MSG_LENGTH_SHORT + 1);
    if (LW6SYS_TEST_ACK (short_text))
      {
	memset (short_text, _TEST_MORE_WAREHOUSE_MSG_CHAR_SHORT, _TEST_MORE_WAREHOUSE_MSG_LENGTH_SHORT);
	short_text[_TEST_MORE_WAREHOUSE_MSG_LENGTH_SHORT] = '\0';
	long_text = LW6SYS_MALLOC (sys_context, _TEST_MORE_WAREHOUSE_MSG_LENGTH_LONG + 1);
	if (LW6SYS_TEST_ACK (long_text))
	  {
	    memset (long_text, _TEST_MORE_WAREHOUSE_MSG_CHAR_LONG, _TEST_MORE_WAREHOUSE_MSG_LENGTH_LONG);
	    long_text[_TEST_MORE_WAREHOUSE_MSG_LENGTH_LONG] = '\0';
	    /*
	     * OK, it proved the simple memset is not enough to make
	     * a good string, for it compresses to one single atom,
	     * so we introduce some entropy by putting pseudo-random
	     * but absolutely repeatable stuff. The idea is that with
	     * a real random stuff, compression could change the number
	     * of atoms/msgs sent. OTOH, beware, if the compression code
	     * is refactored, this test suite might fail.
	     */
	    for (i = _TEST_MORE_WAREHOUSE_MSG_LENGTH_LONG / 2; i < _TEST_MORE_WAREHOUSE_MSG_LENGTH_LONG; ++i)
	      {
		long_text[i] = 'a' + lw6sys_checksum_int32 (sys_context, i) % 26;
	      }
	    for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
	      {
		warehouse[warehouse_index] = lw6dat_warehouse_new (sys_context, node_ids[warehouse_index], node_seq_0s[warehouse_index]);
	      }
	    if (LW6SYS_TEST_ACK (warehouse[0] && warehouse[1] && warehouse[2]))
	      {
		for (stage = 0; stage < _TEST_MORE_WAREHOUSE_NB_NODES; ++stage)
		  {
		    /*
		     * Following line might look wrong but yes, we initialize
		     * using stage as a node index : the idea is that each
		     * node is coming in its own time, this is to simulate
		     * a real life example with several nodes coming one after
		     * the other.
		     */
		    for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		      {
			if (warehouse_index != stage)
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					_x_ ("registering node %"
					     LW6SYS_PRINTF_LL
					     "x in warehouse %d with serial %d seq %"
					     LW6SYS_PRINTF_LL "d"),
					(long long) node_ids[stage],
					warehouse_index, node_serials[warehouse_index][stage], (long long) node_seq_0s[warehouse_index]);
			    lw6dat_warehouse_register_node (sys_context, warehouse
							    [warehouse_index],
							    node_ids[stage], node_serials[warehouse_index][stage], node_seq_0s[warehouse_index]);
			  }
		      }
		    for (node_index = 0; node_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++node_index)
		      {
			for (msg_index = 0LL; msg_index < ((node_index + 1) * _TEST_MORE_WAREHOUSE_NB_MSGS_PER_STAGE) + 1; ++msg_index)
			  {
			    seq = node_seq_0s[stage] + node_seq_0_offsets[node_index] + msg_index;
			    node_id = node_ids[node_index];
			    msg =
			      lw6sys_new_sprintf (sys_context,
						  "%" LW6SYS_PRINTF_LL "d %"
						  LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) node_id, msg_index & 1LL ? long_text : short_text);
			    if (LW6SYS_TEST_ACK (msg))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					    _x_ ("PUT seq=%" LW6SYS_PRINTF_LL
						 "d id=%" LW6SYS_PRINTF_LL "x len=%d"), (long long) seq, (long long) node_id, (int) strlen (msg));
				lw6dat_warehouse_put_local_msg (sys_context, warehouse[node_index], msg);
				LW6SYS_FREE (sys_context, msg);
			      }
			  }
		      }
		  }

		for (stage = 0; stage < _TEST_MORE_WAREHOUSE_NB_NODES; ++stage)
		  {
		    /*
		     * Insert a final message, else we are stuck at N-1
		     */
		    node_id = node_ids[stage];
		    msg =
		      lw6sys_new_sprintf (sys_context,
					  "%" LW6SYS_PRINTF_LL "d %"
					  LW6SYS_PRINTF_LL "x %s", (long long) _TEST_MORE_WAREHOUSE_NODE_FINAL_SEQ, (long long) node_id, short_text);
		    if (LW6SYS_TEST_ACK (msg))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				    _x_ ("PUT seq=%" LW6SYS_PRINTF_LL
					 "d id=%" LW6SYS_PRINTF_LL
					 "x len=%d"), (long long) _TEST_MORE_WAREHOUSE_NODE_FINAL_SEQ, (long long) node_id, (int) strlen (msg));
			lw6dat_warehouse_put_local_msg (sys_context, warehouse[stage], msg);
			LW6SYS_FREE (sys_context, msg);
		      }
		  }

		memset (&not_sent_list, 0, sizeof (not_sent_list));
		for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		  {
		    for (node_index = 0; node_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++node_index)
		      {
			node_id = node_ids[node_index];
			not_sent_list[warehouse_index][node_index] =
			  lw6dat_warehouse_get_atom_str_list_not_sent (sys_context, warehouse[warehouse_index], node_id);
			if (not_sent_list[warehouse_index][node_index])
			  {
			    lw6sys_list_map (sys_context, not_sent_list[warehouse_index][node_index], _print_msg_callback, NULL);
			    not_sent_length = lw6sys_list_length (sys_context, not_sent_list[warehouse_index][node_index]);
			    if (LW6SYS_TEST_ACK (not_sent_length == nb_init_not_sent[warehouse_index][node_index]))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					    _x_
					    ("check nb_init_not_sent, in warehouse %d are %d messages not sent for node index=%d id=%"
					     LW6SYS_PRINTF_LL
					     "x local_serial=%d local_seq_0=%"
					     LW6SYS_PRINTF_LL "d"),
					    warehouse_index, not_sent_length,
					    node_index, (long long) node_id,
					    lw6dat_warehouse_get_local_serial
					    (sys_context, warehouse[warehouse_index]), (long long) lw6dat_warehouse_get_local_seq_0 (sys_context,
																     warehouse
																     [warehouse_index]));
			      }
			    else
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					    _x_
					    ("check nb_init_ot_sent, in warehouse %d are %d messages not sent for node index=%d id=%"
					     LW6SYS_PRINTF_LL
					     "x, expecting %d local_serial=%d local_seq_0=%"
					     LW6SYS_PRINTF_LL "d"),
					    warehouse_index, not_sent_length,
					    node_index, (long long) node_id,
					    nb_init_not_sent[warehouse_index]
					    [node_index],
					    lw6dat_warehouse_get_local_serial
					    (sys_context, warehouse[warehouse_index]), (long long) lw6dat_warehouse_get_local_seq_0 (sys_context,
																     warehouse
																     [warehouse_index]));
				ret = 0;
			      }
			  }
		      }
		  }
		memset (&fake_send_data, 0, sizeof (_fake_send_data_t));
		fake_send_data.ret = 1;
		for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		  {
		    for (node_index = 0; node_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++node_index)
		      {
			if (not_sent_list[warehouse_index][node_index])
			  {
			    /*
			     * In the following, we're referencing warehouse[node_index]
			     * (and not warehouse_index) because, indeed, we want
			     * to ventilate all messages to the right warehouses,
			     * as if they had been sent on the network and received
			     * by peers.
			     */
			    fake_send_data.warehouse = warehouse[node_index];
			    fake_send_data.logical_from = node_ids[warehouse_index];
			    lw6sys_list_map (sys_context, not_sent_list[warehouse_index][node_index], _fake_send_callback, &fake_send_data);
			    if (!LW6SYS_TEST_ACK (fake_send_data.ret))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("problem with fake send/recv of messages"));
				ret = 0;
			      }
			    lw6sys_list_free (sys_context, not_sent_list[warehouse_index][node_index]);
			  }
		      }
		  }
		/*
		 * Now at this stage it's like all messages have been sent
		 * over the network, so warehouse 0 is finally aware of
		 * all the messages from warehouse 1, etc. So, there are
		 * new unsent messages!
		 */
		memset (&not_sent_list, 0, sizeof (not_sent_list));
		for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		  {
		    for (node_index = 0; node_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++node_index)
		      {
			node_id = node_ids[node_index];
			not_sent_list[warehouse_index][node_index] =
			  lw6dat_warehouse_get_atom_str_list_not_sent (sys_context, warehouse[warehouse_index], node_id);
			if (not_sent_list[warehouse_index][node_index])
			  {
			    lw6sys_list_map (sys_context, not_sent_list[warehouse_index][node_index], _print_msg_callback, NULL);
			    not_sent_length = lw6sys_list_length (sys_context, not_sent_list[warehouse_index][node_index]);
			    if (LW6SYS_TEST_ACK (not_sent_length == nb_cross_not_sent[warehouse_index][node_index]))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					    _x_
					    ("check nb_cross_not_sent, in warehouse %d are %d messages not sent for node index=%d id=%"
					     LW6SYS_PRINTF_LL
					     "x local_serial=%d local_seq_0=%"
					     LW6SYS_PRINTF_LL "d"),
					    warehouse_index, not_sent_length,
					    node_index, (long long) node_id,
					    lw6dat_warehouse_get_local_serial
					    (sys_context, warehouse[warehouse_index]), (long long) lw6dat_warehouse_get_local_seq_0 (sys_context,
																     warehouse
																     [warehouse_index]));
			      }
			    else
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					    _x_
					    ("check nb_cross_ot_sent, in warehouse %d are %d messages not sent for node index=%d id=%"
					     LW6SYS_PRINTF_LL
					     "x, expecting %d local_serial=%d local_seq_0=%"
					     LW6SYS_PRINTF_LL "d"),
					    warehouse_index, not_sent_length,
					    node_index, (long long) node_id,
					    nb_cross_not_sent[warehouse_index]
					    [node_index],
					    lw6dat_warehouse_get_local_serial
					    (sys_context, warehouse[warehouse_index]), (long long) lw6dat_warehouse_get_local_seq_0 (sys_context,
																     warehouse
																     [warehouse_index]));
				ret = 0;
			      }
			  }
		      }
		  }

		memset (&fake_send_data, 0, sizeof (_fake_send_data_t));
		fake_send_data.ret = 1;
		for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		  {
		    for (node_index = 0; node_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++node_index)
		      {
			if (not_sent_list[warehouse_index][node_index])
			  {
			    fake_send_data.warehouse = warehouse[node_index];
			    fake_send_data.logical_from = 0LL;
			    lw6sys_list_map (sys_context, not_sent_list[warehouse_index][node_index], _fake_send_callback, &fake_send_data);
			    if (!LW6SYS_TEST_ACK (fake_send_data.ret))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("problem with fake send/recv of messages"));
				ret = 0;
			      }
			    lw6sys_list_free (sys_context, not_sent_list[warehouse_index][node_index]);
			  }
		      }
		  }

		/*
		 * Now that we have faked the send/recv process
		 * we check that there's nothing to send left,
		 * since all has been done.
		 */
		memset (&not_sent_list, 0, sizeof (not_sent_list));
		for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		  {
		    for (node_index = 0; node_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++node_index)
		      {
			node_id = node_ids[node_index];
			not_sent_list[warehouse_index][node_index] =
			  lw6dat_warehouse_get_atom_str_list_not_sent (sys_context, warehouse[warehouse_index], node_id);
			if (not_sent_list[warehouse_index][node_index])
			  {
			    lw6sys_list_map (sys_context, not_sent_list[warehouse_index][node_index], _print_msg_callback, NULL);
			    not_sent_length = lw6sys_list_length (sys_context, not_sent_list[warehouse_index][node_index]);
			    if (LW6SYS_TEST_ACK (not_sent_length == 0))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					    _x_
					    ("in warehouse %d are no messages not sent for node %"
					     LW6SYS_PRINTF_LL "x"), warehouse_index, (long long) node_id);
			      }
			    else
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					    _x_
					    ("in warehouse %d are %d messages not sent for node %"
					     LW6SYS_PRINTF_LL "x but there should be none"), warehouse_index, not_sent_length, (long long) node_id);
				ret = 0;
			      }
			    lw6sys_list_free (sys_context, not_sent_list[warehouse_index][node_index]);
			  }
		      }
		  }

		/*
		 * Now testing that actual messages can be extracted from
		 * the warehouses, and not only atoms.
		 */
		for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_ ("checking node index=%d id=%"
				     LW6SYS_PRINTF_LL "x"), warehouse_index, (long long) lw6dat_warehouse_get_local_id (sys_context,
															warehouse[warehouse_index]));

		    lw6dat_warehouse_calc_serial_draft_and_reference (sys_context, warehouse[warehouse_index]);
		    seq_min = lw6dat_warehouse_get_seq_min (sys_context, warehouse[warehouse_index]);
		    seq_max = lw6dat_warehouse_get_seq_max (sys_context, warehouse[warehouse_index]);
		    seq_draft = lw6dat_warehouse_get_seq_draft (sys_context, warehouse[warehouse_index]);
		    seq_reference = lw6dat_warehouse_get_seq_reference (sys_context, warehouse[warehouse_index]);

		    miss_list = lw6dat_warehouse_get_miss_list (sys_context, warehouse[warehouse_index], _TEST_WAREHOUSE_MISS_MAX_RANGE, NULL);
		    if (LW6SYS_TEST_ACK (miss_list))
		      {
			miss_list_length = lw6sys_list_length (sys_context, miss_list);
			if (LW6SYS_TEST_ACK (miss_list_length == 0))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, no messages in MISS list"));
			  }
			else
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("MISS list contains %d elements"), miss_list_length);
			    lw6sys_list_map (sys_context, miss_list, _test_miss_print_callback, NULL);
			    ret = 0;
			  }
			lw6sys_list_free (sys_context, miss_list);
		      }

		    if (LW6SYS_TEST_ACK (seq_min == seq_mins[warehouse_index]
					 && seq_max ==
					 seq_maxs[warehouse_index]
					 && seq_draft == seq_drafts[warehouse_index] && seq_reference == seq_references[warehouse_index]))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				    _x_ ("warehouse %d seq_min=%"
					 LW6SYS_PRINTF_LL "d seq_max=%"
					 LW6SYS_PRINTF_LL "d seq_draft=%"
					 LW6SYS_PRINTF_LL "d seq_reference=%"
					 LW6SYS_PRINTF_LL "d"),
				    warehouse_index, (long long) seq_min, (long long) seq_max, (long long) seq_draft, (long long) seq_reference);
			msg_list = lw6dat_warehouse_get_msg_list_by_seq (sys_context, warehouse[warehouse_index], seq_min, seq_draft, 0, NULL);
			if (LW6SYS_TEST_ACK (msg_list))
			  {
			    draft_len = lw6sys_list_length (sys_context, msg_list);
			    if (LW6SYS_TEST_ACK (draft_len == draft_lens[warehouse_index]))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d draft messages in warehouse %d, OK"), draft_len, warehouse_index);
			      }
			    else
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					    _x_ ("%d draft  messages in warehouse %d, expected %d"), draft_len, warehouse_index, draft_lens[warehouse_index]);
				ret = 0;
			      }
			    lw6sys_list_free (sys_context, msg_list);
			  }
			msg_list = lw6dat_warehouse_get_msg_list_by_seq (sys_context, warehouse[warehouse_index], seq_min, seq_reference, 1, NULL);
			if (LW6SYS_TEST_ACK (msg_list))
			  {
			    reference_len = lw6sys_list_length (sys_context, msg_list);
			    if (LW6SYS_TEST_ACK (reference_len == reference_lens[warehouse_index]))
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d reference messages in warehouse %d, OK"), reference_len, warehouse_index);
			      }
			    else
			      {
				lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					    _x_
					    ("%d reference messages in warehouse %d, expected %d"),
					    reference_len, warehouse_index, reference_lens[warehouse_index]);
				ret = 0;
			      }
			    lw6sys_list_free (sys_context, msg_list);
			  }
		      }
		    else
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				    _x_ ("warehouse %d seq_min=%"
					 LW6SYS_PRINTF_LL "d seq_max=%"
					 LW6SYS_PRINTF_LL "d seq_draft=%"
					 LW6SYS_PRINTF_LL "d seq_reference=%"
					 LW6SYS_PRINTF_LL
					 "d, should have been seq_min=%"
					 LW6SYS_PRINTF_LL "d seq_max=%"
					 LW6SYS_PRINTF_LL "d seq_draft=%"
					 LW6SYS_PRINTF_LL "d seq_reference=%"
					 LW6SYS_PRINTF_LL "d"),
				    warehouse_index, (long long) seq_min,
				    (long long) seq_max,
				    (long long) seq_draft,
				    (long long) seq_reference,
				    (long long) seq_mins[warehouse_index],
				    (long long) seq_maxs[warehouse_index],
				    (long long) seq_drafts[warehouse_index], (long long) seq_references[warehouse_index]);
			ret = 0;
		      }
		  }

		for (warehouse_index = 0; warehouse_index < _TEST_MORE_WAREHOUSE_NB_NODES; ++warehouse_index)
		  {
		    if (warehouse[warehouse_index])
		      {
			lw6dat_warehouse_free (sys_context, warehouse[warehouse_index]);
		      }
		  }
	      }
	    LW6SYS_FREE (sys_context, long_text);
	  }
	LW6SYS_FREE (sys_context, short_text);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libdat CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libdat CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6dat_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libdat module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dat_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6glb_test_register (sys_context, mode);
      lw6nod_test_register (sys_context, mode);
      lw6cnx_test_register (sys_context, mode);
      lw6msg_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6dat", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_atom);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_block);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_miss);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_stack);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_warehouse);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_more);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6dat_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @dat module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dat_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.sys_context = sys_context;

  _test_data.ret = 1;
  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
