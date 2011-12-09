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
#include "sim-internal.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

#define _TEST_SIMULATE_NB_TEAMS 2
#define _TEST_SIMULATE_BOT_BACKEND "random"
#define _TEST_MASK_NB_TEAMS 3

/*
 * Testing functions in mask.c
 */
static int
test_mask ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i, n;
    int mask, valid;

    for (i = 0; i <= LW6MAP_MAX_NB_TEAMS; ++i)
      {
	mask = _lw6sim_mask_get_max (i);
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("max mask for %d is %d"), i,
		    mask);
      }

    n = _lw6sim_mask_get_max (_TEST_MASK_NB_TEAMS);
    for (i = _LW6SIM_MASK_MIN; i < n; ++i)
      {
	valid = _lw6sim_mask_is_valid (i);
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("mask=%d valid=%d"), i, valid);
      }

    ret = 1;
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in simulate.c
 */
static int
test_simulate ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int argc = _TEST_ARGC;
    char *argv[_TEST_ARGC] = { _TEST_ARGV0 };
    lw6sim_results_t results;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("simulating %d teams with bot backend \"%s\""),
		_TEST_SIMULATE_NB_TEAMS, _TEST_SIMULATE_BOT_BACKEND);
    lw6sim_results_zero (&results);
    ret =
      lw6sim_simulate (argc, argv, &results, _TEST_SIMULATE_NB_TEAMS,
		       _TEST_SIMULATE_BOT_BACKEND);
    if (ret)
      {
	lw6sim_print (&results, stderr);
      }
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
      lw6bot_test (mode);
      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
  }

  ret = test_mask () && test_simulate ();

  return ret;
}
