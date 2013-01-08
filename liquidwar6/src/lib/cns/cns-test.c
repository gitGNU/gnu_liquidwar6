/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#include "../sys/sys.h"
#include "cns.h"

#define TEST_CON_NB_LOOPS 100
#define TEST_CON_SLEEP 0.1f

static void
handler (char *line)
{
  if (line)
    {
      lw6cns_history_add_if_needed (line);
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("read \"%s\""), line);
      free (line);		// exceptionnally, don't use LW6SYS_FREE
    }
  else
    {
      exit (0);
    }
}

/*
 * Testing handler functions
 */
static int
test_handler ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("you can enter random lines, test will end after %d seconds"),
		(int) (TEST_CON_NB_LOOPS * TEST_CON_SLEEP));
    lw6cns_handler_install (handler);
    for (i = 0; i < TEST_CON_NB_LOOPS; ++i)
      {
	lw6cns_handler_poll ();
	lw6sys_sleep (TEST_CON_SLEEP);
      }
    lw6cns_handler_remove (handler);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing support functions
 */
static int
test_support ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("console support -> %d"), lw6cns_support ());
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6cns_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @cns module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cns_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
    }

  ret = test_handler () && test_support ();

  return ret;
}
