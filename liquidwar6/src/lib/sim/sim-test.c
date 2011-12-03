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

#include "sim.h"

#define _TEST_NB_TEAMS 2

/*
 * Testing functions in simulate.c
 */
static int
test_simulate ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sim_simulation_results_t results;
    
    ret=lw6sim_simulate(&results,_TEST_NB_TEAMS);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6sim_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @sim module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sim_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
      lw6pil_test (mode);
      lw6dyn_test (mode);
      lw6bot_test (mode);
    }

  ret = test_simulate ();

  return ret;
}
