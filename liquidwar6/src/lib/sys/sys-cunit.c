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
#endif // HAVE_CONFIG_H

#include "sys.h"
#include "sys-internal.h"

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Console.h>
#ifdef LW6_CUNIT_CURSES
#include <CUnit/CUCurses.h>
#endif // LW6_CUNIT_CURSES

#define _LW6SYS_CUNIT_DIALOG_TIMEOUT 10

/*
 * Base filename, -Listing.xml or -Results.xml will be appended to it
 */
#define _CUNIT_BASENAME "CUnit"

static void
_cunit_summary (lw6sys_context_t * sys_context, const char *cunit_basename)
{
  CU_pFailureRecord failure_record = NULL;
  int nb_failure_records = 0;
  const char *file = NULL;
  int line = 0;
  const char *suite = NULL;
  const char *test = NULL;
  const char *condition = NULL;

  failure_record = CU_get_failure_list ();
  while (failure_record)
    {
      file = failure_record->strFileName;
      line = failure_record->uiLineNumber;
      suite = failure_record->pSuite ? failure_record->pSuite->pName : NULL;
      test = failure_record->pTest ? failure_record->pTest->pName : NULL;
      condition = failure_record->strCondition;

      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("failure record %s:%d %s/%s \"%s\""),
		  lw6sys_str_empty_if_null (sys_context, file), line,
		  lw6sys_str_empty_if_null (sys_context, suite),
		  lw6sys_str_empty_if_null (sys_context, test), lw6sys_str_empty_if_null (sys_context, condition));
      failure_record = (failure_record->pNext && failure_record->pNext != failure_record) ? failure_record->pNext : NULL;
      nb_failure_records++;
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d failure records, complete test results in \"%s-Results.xml\""), nb_failure_records, cunit_basename);
}

/**
 * lw6sys_cunit_run_tests
 *
 * @sys_context: global system context
 * @mode: mode passed to program (bit mask)
 *
 * Run all registered suites and their tests, will interpret mode
 * and call the right CUnit function (Batch, Console, NCurses...).
 *
 * Return value: 1 if tests or OK, 0 if not.
 */
int
lw6sys_cunit_run_tests (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;
  CU_pRunSummary run_summary = NULL;
  int console_state = 0;
  char *user_dir = NULL;
  char *cunit_basename = NULL;

  lw6sys_log_set_dialog_timeout (sys_context, _LW6SYS_CUNIT_DIALOG_TIMEOUT);
  if (mode & LW6SYS_TEST_MODE_INTERACTIVE)
    {
      console_state = lw6sys_log_get_console_state (sys_context);
      lw6sys_log_set_console_state (sys_context, 0);
#ifdef LW6_CUNIT_CURSES
      CU_curses_run_tests ();
#else
      CU_console_run_tests ();
#endif // LW6_CUNIT_CURSES
      lw6sys_log_set_console_state (sys_context, console_state);
    }
  else
    {
      user_dir = lw6sys_get_default_user_dir (sys_context);
      if (user_dir)
	{
	  cunit_basename = lw6sys_path_concat (sys_context, user_dir, _CUNIT_BASENAME);
	  if (cunit_basename)
	    {
	      CU_set_output_filename (cunit_basename);
	    }
	}
      CU_automated_run_tests ();
      CU_list_tests_to_file ();
    }

  run_summary = CU_get_run_summary ();
  if (run_summary)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d CUnit suites run"), run_summary->nSuitesRun);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d CUnit suites failed"), run_summary->nSuitesFailed);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d CUnit tests"), run_summary->nTestsRun);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d CUnit tests failed"), run_summary->nTestsFailed);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d CUnit asserts"), run_summary->nAsserts);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d CUnit successes"), run_summary->nAssertsFailed);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d CUnit failures"), run_summary->nFailureRecords);

      ret = (run_summary->nTestsFailed == 0);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("CU_get_run_summary failed"));
    }

  if (user_dir)
    {
      LW6SYS_FREE (sys_context, user_dir);
    }
  if (cunit_basename)
    {
      if (!(mode & LW6SYS_TEST_MODE_INTERACTIVE))
	{
	  _cunit_summary (sys_context, cunit_basename);
	}
      LW6SYS_FREE (sys_context, cunit_basename);
    }

  return ret;
}

/**
 * lw6sys_cunit_clear
 *
 * @sys_context: global system context
 *
 * Clears the global CUnit related lock.
 *
 * Return value: none.
 */
void
lw6sys_cunit_clear (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (global->cunit_mutex)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, global->cunit_mutex);
      global->cunit_mutex = NULL;
    }
}

/**
 * lw6sys_cunit_lock
 *
 * @sys_context: global system context
 *
 * Locks a global CUnit related lock, this is to allow the use of
 * test macro LW6SYS_TEST_ACK in multithreaded environment, as CUnit
 * does not, by default garantee that concurrent accesses to its API
 * will work. Just to be sure... we lock.
 *
 * Return value: 1 if locked, 0 on failure.
 */
int
lw6sys_cunit_lock (lw6sys_context_t * sys_context)
{
  int ret = 1;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (!global->cunit_mutex)
    {
      /*
       * Initializing global mutex on-the-fly
       */
      global->cunit_mutex = LW6SYS_MUTEX_CREATE (sys_context);
    }
  if (global->cunit_mutex)
    {
      ret = LW6SYS_MUTEX_LOCK (sys_context, global->cunit_mutex);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to initialize CUnit global mutex"));
      ret = 0;
    }

  return ret;
}

/**
 * lw6sys_cunit_unlock
 *
 * @sys_context: global system context
 *
 * Unlocks the global CUnit related lock, this is to allow the use of
 * test macro LW6SYS_TEST_ACK in multithreaded environment, as CUnit
 * does not, by default garantee that concurrent accesses to its API
 * will work. Just to be sure... we lock.
 *
 * Return value: 1 if unlocked, 0 on failure.
 */
int
lw6sys_cunit_unlock (lw6sys_context_t * sys_context)
{
  int ret = 1;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (global->cunit_mutex)
    {
      ret = LW6SYS_MUTEX_UNLOCK (sys_context, global->cunit_mutex);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to unlock CUnit global mutex but it's not defined"));
      ret = 0;
    }

  return ret;
}
