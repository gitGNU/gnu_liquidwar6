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
#endif // HAVE_CONFIG_H

#include "sys.h"

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Console.h>
#ifdef LW6_CUNIT_CURSES
#include <CUnit/CUCurses.h>
#endif // LW6_CUNIT_CURSES

/**
 * lw6sys_cunit_run_tests
 *
 * @mode: mode passed to program (bit mask)
 * 
 * Run all registered suites and their tests, will interpret mode
 * and call the right CUnit function (Batch, Console, NCurses...).
 *
 * Return value: 1 if tests or OK, 0 if not.
 */
int
lw6sys_cunit_run_tests (int mode)
{
  int ret = 0;
  CU_pRunSummary run_summary=NULL;

  if (mode & LW6SYS_TEST_MODE_INTERACTIVE) {
#ifdef LW6_CUNIT_CURSES
    CU_curses_run_tests();
#else
    CU_console_run_tests();
#endif // LW6_CUNIT_CURSES
  } else {
    CU_automated_run_tests();
  }

  run_summary=CU_get_run_summary();
  if (run_summary) {
    lw6sys_log(LW6SYS_LOG_NOTICE,_x_("%d CUnit suites run"),run_summary->nSuitesRun);
    lw6sys_log(LW6SYS_LOG_NOTICE,_x_("%d CUnit suites failed"),run_summary->nSuitesFailed);
    lw6sys_log(LW6SYS_LOG_NOTICE,_x_("%d CUnit tests"),run_summary->nTestsRun);
    lw6sys_log(LW6SYS_LOG_NOTICE,_x_("%d CUnit tests failed"),run_summary->nTestsFailed);
    lw6sys_log(LW6SYS_LOG_NOTICE,_x_("%d CUnit asserts"),run_summary->nAsserts);
    lw6sys_log(LW6SYS_LOG_NOTICE,_x_("%d CUnit successes"),run_summary->nAssertsFailed);
    lw6sys_log(LW6SYS_LOG_NOTICE,_x_("%d CUnit failures"),run_summary->nFailureRecords);
  } else {
    lw6sys_log(LW6SYS_LOG_WARNING,_x_("CU_get_run_summary failed"));
  }

  return ret;
}
