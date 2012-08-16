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
#endif // HAVE_CONFIG_H

#include "liquidwar6.h"

#define _TEST_FILE_0_MAIN "test-0-main.scm"
#define _TEST_FILE_0_CLIENT "test-0-client.scm"
#define _TEST_FILE_0_SERVER "test-0-server.scm"
#define _TEST_FILE_1_MAIN "test-1-main.scm"
#define _TEST_FILE_1_CLIENT "test-1-client.scm"
#define _TEST_FILE_1_SERVER "test-1-server.scm"
#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

#define _TEST_COVERAGE_PERCENT_MIN 50

#define _TEST_SUITE_MAIN 0
#define _TEST_SUITE_CLIENT 1
#define _TEST_SUITE_SERVER 2

#define _TEST_RUN_RANDOM_RANGE 2

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

static char *
get_test_file (int argc, const char **argv, int mode, int suite)
{
  char *script_file = NULL;
  char *script_dir = NULL;
  char *ret = NULL;

  script_file = lw6sys_get_script_file (argc, argv);
  if (script_file)
    {
      script_dir = lw6sys_path_parent (script_file);
      if (script_dir)
	{
	  if (!mode)
	    {
	      switch (suite)
		{
		case _TEST_SUITE_CLIENT:
		  ret = lw6sys_path_concat (script_dir, _TEST_FILE_0_CLIENT);
		  break;
		case _TEST_SUITE_SERVER:
		  ret = lw6sys_path_concat (script_dir, _TEST_FILE_0_SERVER);
		  break;
		default:
		  ret = lw6sys_path_concat (script_dir, _TEST_FILE_0_MAIN);
		  break;
		}
	    }
	  else
	    {
	      switch (suite)
		{
		case _TEST_SUITE_CLIENT:
		  ret = lw6sys_path_concat (script_dir, _TEST_FILE_1_CLIENT);
		  break;
		case _TEST_SUITE_SERVER:
		  ret = lw6sys_path_concat (script_dir, _TEST_FILE_1_SERVER);
		  break;
		default:
		  ret = lw6sys_path_concat (script_dir, _TEST_FILE_1_MAIN);
		  break;
		}
	    }
	  LW6SYS_FREE (script_dir);
	}
      LW6SYS_FREE (script_file);
    }

  if (ret && !lw6sys_file_exists (ret))
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("can't open \"%s\""), ret);
      LW6SYS_FREE (ret);
      ret = NULL;
    }

  return ret;
}

static void
_test_callback (_lw6_test_param_t * param)
{
  int ret = 1;

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
    lw6sys_log_set_level (param->log_level_id);

    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("registering Guile smobs"));
    if (lw6_register_smobs ())
      {

	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("registering Guile functions"));
	if (lw6_register_funcs ())
	  {
	    test_file =
	      get_test_file (param->argc, param->argv, param->mode,
			     param->suite);
	    if (test_file)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("loading \"%s\""),
			    test_file);
		lw6scm_c_primitive_load (test_file);
		LW6SYS_FREE (test_file);
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to register funcs"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to register smobs"));
	ret = 0;
      }

    scm_gc ();

    if (param->coverage_check)
      {
	funcs = lw6hlp_list_funcs ();
	if (funcs)
	  {
	    if (lw6scm_coverage_check
		(&coverage_percent, lw6_global.coverage, funcs)
		|| coverage_percent >= _TEST_COVERAGE_PERCENT_MIN)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("script coverage OK %d%% (%d%% required)"),
			    coverage_percent, _TEST_COVERAGE_PERCENT_MIN);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("script coverage is only %d%% (%d%% required)"),
			    coverage_percent, _TEST_COVERAGE_PERCENT_MIN);
		ret = 0;
	      }
	    lw6sys_list_free (funcs);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to get funcs list"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("skipping coverage check"));
      }

    if (ret)
      {
	if (lw6_global.ret)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("script returned true, looks like tests were OK"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("script returned false, at least one test failed"));
	    ret = 0;
	  }
      }

    lw6_quit_global ();
  }

  LW6SYS_TEST_FUNCTION_END;

  param->ret = ret;
}

static void *
_guile_test (void *data)
{
  _lw6_test_param_t *param = (_lw6_test_param_t *) data;

  _test_callback (param);

  return NULL;
}

static void
_guile_test_run (void *data)
{
  /*
   * We wait (or not) for some time, to simulate, (randomly!)
   * some time shift between various network flavors of tests.
   */
  if (!lw6sys_random (_TEST_RUN_RANDOM_RANGE))
    {
      lw6sys_snooze ();
    }

  lw6scm_with_guile (_guile_test, data);
}

/**
 * lw6_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the liquidwar6 core module test suite, this will mostly
 * test how Guile script integration works, loading a sample
 * script and running it. It does not launch all the other sub
 * modules tests.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6_test (int mode)
{
  _lw6_test_param_t param;
  const int argc = _TEST_ARGC;
  const char *argv[] = { _TEST_ARGV0 };
  u_int64_t pid = 0LL;
  int default_log_level_id = LW6SYS_LOG_INFO_ID;

  memset (&param, 0, sizeof (_lw6_test_param_t));

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6glb_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
      lw6pil_test (mode);
      lw6bot_test (mode);
      lw6sim_test (mode);
      lw6cns_test (mode);
      lw6hlp_test (mode);
      lw6cfg_test (mode);
      lw6ldr_test (mode);
      lw6tsk_test (mode);
      lw6gui_test (mode);
      lw6vox_test (mode);
      lw6gfx_test (mode);
      lw6dsp_test (mode);
      lw6snd_test (mode);
      lw6img_test (mode);
      lw6net_test (mode);
      lw6nod_test (mode);
      lw6cnx_test (mode);
      lw6msg_test (mode);
      lw6cli_test (mode);
      lw6srv_test (mode);
      lw6dat_test (mode);
      lw6p2p_test (mode);
      lw6scm_test (mode);
    }

  if (lw6_init_global (argc, argv))
    {
      default_log_level_id = lw6sys_log_get_level ();

      param.argc = argc;
      param.argv = argv;
      param.mode = mode;
      param.suite = _TEST_SUITE_MAIN;
      param.coverage_check = 1;
      param.log_level_id = default_log_level_id;

      _guile_test_run (&param);

      param.coverage_check = 0;

      if (param.ret && lw6_init_global (argc, argv))
	{
	  if (lw6sys_process_is_fully_supported ())
	    {
	      /*
	       * First network test, we launch the client in our main process
	       * and fire a server in a fork. Won't get the result from the
	       * server, here we just test wether the client has consistent
	       * informations. The other test will come later.
	       */
	      param.suite = _TEST_SUITE_SERVER;
	      param.log_level_id = LW6SYS_LOG_ERROR_ID;
	      if ((pid =
		   lw6sys_process_fork_and_call (_guile_test_run,
						 &param)) != 0)
		{
		  param.suite = _TEST_SUITE_CLIENT;
		  param.log_level_id = default_log_level_id;
		  _guile_test_run (&param);
		  lw6sys_process_kill_1_9 (pid);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_
			  ("skipping client/server test, platform does not have adequate process support and/or it's likely to fail anyway"));
              /*
               * We need to call quit_global manually
               * as the callback we skip obviously
               * won't run it.
               */
              lw6_quit_global ();
	    }
	}

      if (param.ret && lw6_init_global (argc, argv))
	{
	  if (lw6sys_process_is_fully_supported ())
	    {
	      /*
	       * Second network test, we launch the server in our main process
	       * and fire a client in a fork. Won't get the result from the
	       * client, here we just test wether the server has consistent
	       * informations. The other test was done before.
	       */
	      param.suite = _TEST_SUITE_CLIENT;
	      param.log_level_id = LW6SYS_LOG_ERROR_ID;
	      if ((pid =
		   lw6sys_process_fork_and_call (_guile_test_run,
						 &param)) != 0)
		{
		  param.suite = _TEST_SUITE_SERVER;
		  param.log_level_id = default_log_level_id;
		  _guile_test_run (&param);
		  lw6sys_process_kill_1_9 (pid);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_
			  ("skipping server/client test, platform does not have adequate process support and/or it's likely to fail anyway"));
              /*
               * We need to call quit_global manually
               * as the callback we skip obviously
               * won't run it.
               */
              lw6_quit_global ();
	    }
	}
    }

  return param.ret;
}
