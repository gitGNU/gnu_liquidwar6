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

#define _TEST_STACK_NODE_ID 0x2345234523452345LL
#define _TEST_STACK_SERIAL_0 123

#define _TEST_WAREHOUSE_LOCAL_NODE_ID 0x1234123412341234LL

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

    _lw6dat_stack_zero (&stack);
    _lw6dat_stack_init (&stack, _TEST_STACK_NODE_ID, _TEST_STACK_SERIAL_0);
    _lw6dat_stack_clear (&stack);
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
    _lw6dat_warehouse_t *warehouse;

    warehouse = _lw6dat_warehouse_new (_TEST_WAREHOUSE_LOCAL_NODE_ID);
    if (warehouse)
      {
	_lw6dat_warehouse_free (warehouse);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("couldn't create warehouse object"));
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
