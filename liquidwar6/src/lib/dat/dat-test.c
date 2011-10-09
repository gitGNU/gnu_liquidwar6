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

#define _TEST_ATOM_TEXT_SHORT "this is a short text"
#define _TEST_ATOM_TEXT_LONG "this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text..."

#define _TEST_BLOCK_SERIAL_0 456
#define _TEST_BLOCK_ORDER_N 7
#define _TEST_BLOCK_STEP 3
#define _TEST_BLOCK_TEXT_1 "0+0=zero"
#define _TEST_BLOCK_TEXT_2 "2+2=4"

#define _TEST_STACK_NODE_ID 0x2345234523452345LL
#define _TEST_STACK_SERIAL_0 123
#define _TEST_STACK_ORDER_I 0
#define _TEST_STACK_ORDER_N 1
#define _TEST_STACK_TEXT "..."
#define _TEST_STACK_NB_RANDOM_PUT (_LW6DAT_MAX_NB_ATOMS/10)
#define _TEST_STACK_RANDOM_RANGE _LW6DAT_MAX_NB_ATOMS

#define _TEST_WAREHOUSE_LOCAL_NODE_ID 0x1234123412341234LL
#define _TEST_WAREHOUSE_OTHER_NODE_ID 0x2345234523452345LL
#define _TEST_WAREHOUSE_SERIAL 789
#define _TEST_WAREHOUSE_ORDER_I 0
#define _TEST_WAREHOUSE_ORDER_N 1
#define _TEST_WAREHOUSE_TEXT "spam and ham"
#define _TEST_WAREHOUSE_NB_STACKS_OVERFLOW (LW6DAT_MAX_NB_STACKS*2)
#define _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW (_LW6DAT_MAX_NB_ATOMS*2)
#define _TEST_WAREHOUSE_SERIAL_I_N_MSG "123 2 4 foo bar"
#define _TEST_WAREHOUSE_PUT_MIN_SIZE 1
#define _TEST_WAREHOUSE_PUT_MAX_SIZE 3000

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

    _lw6dat_atom_zero (&atom);
    if (_lw6dat_atom_get_text (&atom))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("get text returned something on a zeroed atom"));
	ret = 0;
      }
    _lw6dat_atom_set_text (&atom, _TEST_ATOM_TEXT_SHORT);
    text = _lw6dat_atom_get_text (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_text (&atom, _TEST_ATOM_TEXT_LONG);
    text = _lw6dat_atom_get_text (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_text (&atom, _TEST_ATOM_TEXT_SHORT);
    text = _lw6dat_atom_get_text (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
    _lw6dat_atom_set_text (&atom, _TEST_ATOM_TEXT_LONG);
    text = _lw6dat_atom_get_text (&atom);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("atom text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get atom text"));
      }
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
		 _TEST_BLOCK_TEXT_1))
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
	     _TEST_BLOCK_TEXT_2))
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
	     _TEST_BLOCK_TEXT_1)
	    || _lw6dat_block_put_atom (block,
				       _TEST_BLOCK_SERIAL_0 +
				       _LW6DAT_NB_ATOMS_PER_BLOCK, 0,
				       _TEST_BLOCK_ORDER_N,
				       _TEST_BLOCK_TEXT_1))
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
    int serial;
    int found_null = 0;
    int found_not_null = 0;

    _lw6dat_stack_zero (&stack);
    _lw6dat_stack_init (&stack, _TEST_STACK_NODE_ID, _TEST_STACK_SERIAL_0);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("putting %d atoms in stack"),
		_TEST_STACK_NB_RANDOM_PUT);
    for (i = 0; i < _TEST_STACK_NB_RANDOM_PUT; ++i)
      {
	serial =
	  _TEST_STACK_SERIAL_0 + lw6sys_random (_TEST_STACK_RANDOM_RANGE);
	if (!_lw6dat_stack_put_atom
	    (&stack, serial, _TEST_STACK_ORDER_I, _TEST_STACK_ORDER_N,
	     _TEST_STACK_TEXT))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("error putting atom on stack i=%d serial=%d"), i,
			serial);
	    ret = 0;
	  }
      }
    if (ret)
      {
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
      }

    _lw6dat_stack_clear (&stack);
    _lw6dat_stack_clear (&stack);	// call it twice, to check it's allowed
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
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
    int overflow = 0;
    int i;
    char *msg = NULL;

    _warehouse = _lw6dat_warehouse_new (_TEST_WAREHOUSE_LOCAL_NODE_ID);
    if (_warehouse)
      {
	if (_lw6dat_warehouse_put_atom
	    (_warehouse, _TEST_WAREHOUSE_LOCAL_NODE_ID,
	     _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_ORDER_I,
	     _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_TEXT))
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
		 _TEST_WAREHOUSE_TEXT))
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
	_lw6dat_warehouse_free (_warehouse);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("couldn't create _warehouse object"));
	ret = 0;
      }

    _warehouse = _lw6dat_warehouse_new (_TEST_WAREHOUSE_LOCAL_NODE_ID);
    if (_warehouse)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("putting %d atoms in warehouse"),
		    _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW);
	if (_lw6dat_warehouse_put_atom
	    (_warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
	     _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_ORDER_I,
	     _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_TEXT))
	  {
	    for (i = 1; i < _TEST_WAREHOUSE_NB_ATOMS_OVERFLOW; ++i)
	      {
		if (!_lw6dat_warehouse_put_atom
		    (_warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
		     _TEST_WAREHOUSE_SERIAL + i, _TEST_WAREHOUSE_ORDER_I,
		     _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_TEXT))
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_ ("unable to put atom %d"), i);
		    ret = 0;
		  }
	      }
	    if (_lw6dat_warehouse_put_atom
		(_warehouse, _TEST_WAREHOUSE_OTHER_NODE_ID,
		 _TEST_WAREHOUSE_SERIAL, _TEST_WAREHOUSE_ORDER_I,
		 _TEST_WAREHOUSE_ORDER_N, _TEST_WAREHOUSE_TEXT))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("could put again 1st atom in warehouse, shifting shoud work"));
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

    warehouse = lw6dat_warehouse_new (_TEST_WAREHOUSE_LOCAL_NODE_ID);
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

	_lw6dat_warehouse_purge (_warehouse);

	for (i = _TEST_WAREHOUSE_PUT_MIN_SIZE;
	     i < _TEST_WAREHOUSE_PUT_MAX_SIZE; ++i)
	  {
	    msg = lw6sys_str_random_words (i);
	    if (msg)
	      {
		if (lw6dat_warehouse_put_msg (warehouse, msg))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("put msg serial=%d len=%d"),
				lw6dat_warehouse_get_local_serial (warehouse),
				i);
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

  ret = test_atom () && test_block () && test_stack () && test_warehouse ();

  return ret;
}
