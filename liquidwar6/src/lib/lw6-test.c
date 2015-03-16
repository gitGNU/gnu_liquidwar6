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

#include <CUnit/CUnit.h>

#include "liquidwar6.h"

#define _TEST_FILE_0_MAIN "test-0-main.scm"
#define _TEST_FILE_0_NODE_A "test-0-node-a.scm"
#define _TEST_FILE_0_NODE_B "test-0-node-b.scm"
#define _TEST_FILE_0_NODE_C "test-0-node-c.scm"
#define _TEST_FILE_1_MAIN "test-1-main.scm"
#define _TEST_FILE_1_NODE_A "test-1-node-a.scm"
#define _TEST_FILE_1_NODE_B "test-1-node-b.scm"
#define _TEST_FILE_1_NODE_C "test-1-node-c.scm"
#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

#define _TEST_COVERAGE_PERCENT_MIN 10

#define _TEST_SUITE_MAIN 0
#define _TEST_SUITE_NODE_A 1
#define _TEST_SUITE_NODE_B 2
#define _TEST_SUITE_NODE_C 3

#define _TEST_RUN_RANDOM_RANGE 2

#define _TEST_NET_SLEEP 3.0f

typedef struct _lw6_test_param_s
{
  int ret;
  int argc;
  const char **argv;
  int mode;
  int suite;
  int coverage_check;
  int log_level_id;
}
_lw6_test_param_t;

typedef struct _lw6_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
  _lw6_test_param_t param;
  _lw6_test_param_t param_a;
  _lw6_test_param_t param_b;
  _lw6_test_param_t param_c;
  int default_log_level_id;
} _lw6_test_data_t;

static _lw6_test_data_t _test_data = { 0, NULL, {0, 0, NULL, 0, 0, 0, 0}, {0, 0, NULL, 0, 0, 0, 0}, {0, 0, NULL, 0, 0,
												     0, 0}, {0, 0,
													     NULL, 0,
													     0, 0, 0},
0
};

static char *
_get_test_file (lw6sys_context_t * sys_context, int argc, const char **argv, int mode, int suite)
{
  char *script_file = NULL;
  char *script_dir = NULL;
  char *ret = NULL;

  script_file = lw6sys_get_script_file (sys_context, argc, argv);
  if (script_file)
    {
      script_dir = lw6sys_path_parent (sys_context, script_file);
      if (script_dir)
	{
	  if (!(mode & LW6SYS_TEST_MODE_FULL_TEST))
	    {
	      switch (suite)
		{
		case _TEST_SUITE_NODE_A:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_0_NODE_A);
		  break;
		case _TEST_SUITE_NODE_B:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_0_NODE_B);
		  break;
		case _TEST_SUITE_NODE_C:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_0_NODE_C);
		  break;
		default:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_0_MAIN);
		  break;
		}
	    }
	  else
	    {
	      switch (suite)
		{
		case _TEST_SUITE_NODE_A:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_1_NODE_A);
		  break;
		case _TEST_SUITE_NODE_B:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_1_NODE_B);
		  break;
		case _TEST_SUITE_NODE_C:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_1_NODE_C);
		  break;
		default:
		  ret = lw6sys_path_concat (sys_context, script_dir, _TEST_FILE_1_MAIN);
		  break;
		}
	    }
	  LW6SYS_FREE (sys_context, script_dir);
	}
      LW6SYS_FREE (sys_context, script_file);
    }

  if (ret && !lw6sys_file_exists (sys_context, ret))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't open \"%s\""), ret);
      LW6SYS_FREE (sys_context, ret);
      ret = NULL;
    }

  return ret;
}

static void
_guile_test_callback (_lw6_test_param_t * param)
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *test_file = NULL;
    lw6sys_list_t *funcs = NULL;
    int coverage_percent = 0;

    /*
     * Change log level, this way on spawned processes there
     * are no logs and this avoids dirty interferences.
     * Will will run each test in the main thread anyway.
     */
    lw6sys_log_set_level (sys_context, param->log_level_id);

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("registering Guile smobs"));
    if (lw6_register_smobs (sys_context))
      {

	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("registering Guile functions"));
	if (lw6_register_funcs (sys_context))
	  {
	    test_file = _get_test_file (sys_context, param->argc, param->argv, param->mode, param->suite);
	    if (test_file)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("loading \"%s\""), test_file);
		lw6scm_c_primitive_load (sys_context, test_file);
		LW6SYS_FREE (sys_context, test_file);
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to register funcs"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to register smobs"));
	ret = 0;
      }

    scm_gc ();

    if (param->coverage_check)
      {
	funcs = lw6hlp_list_funcs (sys_context);
	if (funcs)
	  {
	    if (lw6scm_coverage_check (sys_context, &coverage_percent, lw6_global.coverage, funcs) || coverage_percent >= _TEST_COVERAGE_PERCENT_MIN)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("script coverage OK %d%% (%d%% required)"), coverage_percent, _TEST_COVERAGE_PERCENT_MIN);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("script coverage is only %d%% (%d%% required)"), coverage_percent, _TEST_COVERAGE_PERCENT_MIN);
		ret = 0;
	      }
	    lw6sys_list_free (sys_context, funcs);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get funcs list"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("skipping coverage check"));
      }

    if (ret)
      {
	if (lw6_global.ret)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("script returned true, looks like tests were OK"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("script returned false, at least one test failed"));
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  /*
   * Local ret is initialized to 1, but the caller's param->ret
   * was set to 0 (else non-running script would return 1).
   */
  param->ret = ret;
}

static void *
_guile_test (lw6sys_context_t * sys_context, void *data)
{
  _lw6_test_param_t *param = (_lw6_test_param_t *) data;

  _guile_test_callback (param);

  return NULL;
}

static void
_guile_test_run (lw6sys_context_t * sys_context, void *data)
{
  /*
   * We wait (or not) for some time, to simulate, (randomly!)
   * some time shift between various network flavors of tests.
   */
  if (!lw6sys_random (sys_context, _TEST_RUN_RANDOM_RANGE))
    {
      lw6sys_snooze (sys_context);
    }

  lw6scm_with_guile (sys_context, _guile_test, data);
}

/*
 * Test test-?-main.scm
 */
static void
_test_main ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  _guile_test_run (sys_context, &_test_data.param);
}

static void
_test_node_abc ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;
  u_int64_t pid_b = 0LL;
  u_int64_t pid_c = 0LL;

  /*
   * Sleeping/waiting for a while to make sure everyone has
   * closed sockets, make double-sure a previous node won't
   * be left and used by next set of nodes.
   */
  lw6sys_sleep (sys_context, _TEST_NET_SLEEP);

  /*
   * First network test, we launch node_a in the main thread,
   * and fire node_b and node_c in separate threads.
   * We won't get the result from the node_b and node_c but
   * this is not a problem, we run the same test in other
   * configurations to gather that output.
   */
  _test_data.param_b.log_level_id = LW6SYS_LOG_ERROR_ID;
  _test_data.param_c.log_level_id = LW6SYS_LOG_ERROR_ID;
  pid_b = lw6sys_process_fork_and_call (sys_context, _guile_test_run, &_test_data.param_b);
  pid_c = lw6sys_process_fork_and_call (sys_context, _guile_test_run, &_test_data.param_c);
  if (pid_b != 0 && pid_c != 0)
    {
      _guile_test_run (sys_context, &_test_data.param_a);
    }
  if (pid_b != 0)
    {
      lw6sys_process_kill_1_9 (sys_context, pid_b);
    }
  if (pid_c != 0)
    {
      lw6sys_process_kill_1_9 (sys_context, pid_c);
    }
  _test_data.param_b.log_level_id = _test_data.default_log_level_id;
  _test_data.param_c.log_level_id = _test_data.default_log_level_id;
}

static void
_test_node_bca ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;
  u_int64_t pid_c = 0LL;
  u_int64_t pid_a = 0LL;

  /*
   * Sleeping/waiting for a while to make sure everyone has
   * closed sockets, make double-sure a previous node won't
   * be left and used by next set of nodes.
   */
  lw6sys_sleep (sys_context, _TEST_NET_SLEEP);

  /*
   * Second network test, we launch node_b in the main thread,
   * and fire node_c and node_a in separate threads.
   * We won't get the result from the node_c and node_a but
   * this is not a problem, we run the same test in other
   * configurations to gather that output.
   */
  _test_data.param_c.log_level_id = LW6SYS_LOG_ERROR_ID;
  _test_data.param_a.log_level_id = LW6SYS_LOG_ERROR_ID;
  pid_c = lw6sys_process_fork_and_call (sys_context, _guile_test_run, &_test_data.param_c);
  pid_a = lw6sys_process_fork_and_call (sys_context, _guile_test_run, &_test_data.param_a);
  if (pid_c != 0 && pid_a != 0)
    {
      _guile_test_run (sys_context, &_test_data.param_b);
    }
  if (pid_c != 0)
    {
      lw6sys_process_kill_1_9 (sys_context, pid_c);
    }
  if (pid_a != 0)
    {
      lw6sys_process_kill_1_9 (sys_context, pid_a);
    }
  _test_data.param_c.log_level_id = _test_data.default_log_level_id;
  _test_data.param_a.log_level_id = _test_data.default_log_level_id;
}

static void
_test_node_cab ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;
  u_int64_t pid_a = 0LL;
  u_int64_t pid_b = 0LL;

  /*
   * Sleeping/waiting for a while to make sure everyone has
   * closed sockets, make double-sure a previous node won't
   * be left and used by next set of nodes.
   */
  lw6sys_sleep (sys_context, _TEST_NET_SLEEP);

  /*
   * Third network test, we launch node_c in the main thread,
   * and fire node_a and node_b in separate threads.
   * We won't get the result from the node_a and node_b but
   * this is not a problem, we run the same test in other
   * configurations to gather that output.
   */
  _test_data.param_a.log_level_id = LW6SYS_LOG_ERROR_ID;
  _test_data.param_b.log_level_id = LW6SYS_LOG_ERROR_ID;
  pid_a = lw6sys_process_fork_and_call (sys_context, _guile_test_run, &_test_data.param_a);
  pid_b = lw6sys_process_fork_and_call (sys_context, _guile_test_run, &_test_data.param_b);
  if (pid_a != 0 && pid_b != 0)
    {
      _guile_test_run (sys_context, &_test_data.param_c);
    }
  if (pid_a != 0)
    {
      lw6sys_process_kill_1_9 (sys_context, pid_a);
    }
  if (pid_b != 0)
    {
      lw6sys_process_kill_1_9 (sys_context, pid_b);
    }
  _test_data.param_a.log_level_id = _test_data.default_log_level_id;
  _test_data.param_b.log_level_id = _test_data.default_log_level_id;
}

static int
_setup_init ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  const int argc = _TEST_ARGC;
  const char *argv[] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init lw6 CUnit test suite"));

  if (lw6_init_global (sys_context, argc, argv))
    {
      _test_data.param.argc = argc;
      _test_data.param.argv = argv;
      _test_data.param.suite = _TEST_SUITE_MAIN;
      _test_data.param.coverage_check = 1;
      _test_data.param.log_level_id = lw6sys_log_get_level (sys_context);
      _test_data.default_log_level_id = _test_data.param.log_level_id;

      memcpy (&_test_data.param_a, &_test_data.param, sizeof (_lw6_test_param_t));
      memcpy (&_test_data.param_b, &_test_data.param, sizeof (_lw6_test_param_t));
      memcpy (&_test_data.param_c, &_test_data.param, sizeof (_lw6_test_param_t));

      _test_data.param_a.suite = _TEST_SUITE_NODE_A;
      _test_data.param_b.suite = _TEST_SUITE_NODE_B;
      _test_data.param_c.suite = _TEST_SUITE_NODE_C;

      _test_data.param_a.coverage_check = 0;
      _test_data.param_b.coverage_check = 0;
      _test_data.param_c.coverage_check = 0;

      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_quit ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit lw6 CUnit test suite"));

  lw6_quit_global (sys_context);
  lw6sys_log_set_level (sys_context, _test_data.default_log_level_id);

  ret = CUE_SUCCESS;

  return ret;
}

/**
 * lw6_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the lw6 module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6_test_register (lw6sys_context_t * sys_context, int mode)
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
      lw6map_test_register (sys_context, mode);
      lw6ker_test_register (sys_context, mode);
      lw6gen_test_register (sys_context, mode);
      lw6pil_test_register (sys_context, mode);
      lw6bot_test_register (sys_context, mode);
      lw6sim_test_register (sys_context, mode);
      lw6cns_test_register (sys_context, mode);
      lw6hlp_test_register (sys_context, mode);
      lw6cfg_test_register (sys_context, mode);
      lw6ldr_test_register (sys_context, mode);
      lw6tsk_test_register (sys_context, mode);
      lw6mat_test_register (sys_context, mode);
      lw6gui_test_register (sys_context, mode);
      lw6vox_test_register (sys_context, mode);
      lw6gfx_test_register (sys_context, mode);
      lw6dsp_test_register (sys_context, mode);
      lw6snd_test_register (sys_context, mode);
      lw6img_test_register (sys_context, mode);
      lw6net_test_register (sys_context, mode);
      lw6nod_test_register (sys_context, mode);
      lw6cnx_test_register (sys_context, mode);
      lw6msg_test_register (sys_context, mode);
      lw6cli_test_register (sys_context, mode);
      lw6srv_test_register (sys_context, mode);
      lw6dat_test_register (sys_context, mode);
      lw6p2p_test_register (sys_context, mode);
      lw6scm_test_register (sys_context, mode);
    }

  memset (&_test_data.param, 0, sizeof (_lw6_test_param_t));
  memset (&_test_data.param_a, 0, sizeof (_lw6_test_param_t));
  memset (&_test_data.param_b, 0, sizeof (_lw6_test_param_t));
  memset (&_test_data.param_c, 0, sizeof (_lw6_test_param_t));

  _test_data.param.mode = mode;

  suite = CU_add_suite ("lw6", _setup_init, _setup_quit);
  if (suite)
    {
      /*
       * Switch between lw6sys_true / lw6sys_false to
       * speed up test while debugging.
       */
      if (lw6sys_true ())
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_main);
	}
      if (lw6sys_process_is_fully_supported (sys_context))
	{
	  /*
	   * Switch between lw6sys_true / lw6sys_false to
	   * speed up test while debugging.
	   */
	  if (lw6sys_true ())
	    {
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_node_abc);
	    }
	  /*
	   * Switch between lw6sys_true / lw6sys_false to
	   * speed up test while debugging.
	   */
	  if (lw6sys_true ())
	    {
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_node_bca);
	    }
	  /*
	   * Switch between lw6sys_true / lw6sys_false to
	   * speed up test while debugging.
	   */
	  if (lw6sys_true ())
	    {
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_node_cab);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("skipping client/server test, platform does not have adequate process support and/or it's likely to fail anyway"));
	  _test_data.param_a.ret = 1;
	  _test_data.param_b.ret = 1;
	  _test_data.param_c.ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the liquidwar6 core module test suite, this will mostly
 * test how Guile script integration works, loading a sample
 * script and running it. It does not launch all the other sub
 * modules tests.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      if (mode & LW6SYS_TEST_MODE_FULL_TEST)
	{
	  ret = _test_data.ret && _test_data.param.ret && _test_data.param_a.ret && _test_data.param_b.ret && _test_data.param_c.ret;;
	}
      else
	{
	  ret = _test_data.ret;
	}
    }

  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("scripts tests OK"));
    }
  else
    {
      /*
       * Displaying a message detailing the status, this is important
       * else standard output is pretty much undreadable since one must
       * scroll up to the test that failed, this being akward.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("script tests failed main=%d node-a=%d node-b=%d node-c=%d"),
		  _test_data.param.ret, _test_data.param_a.ret, _test_data.param_b.ret, _test_data.param_c.ret);
    }

  return ret;
}
