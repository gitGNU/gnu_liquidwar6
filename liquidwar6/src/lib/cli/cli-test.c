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

#include <CUnit/CUnit.h>

#include "cli.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"
#define _TEST_NET_LOG 0

typedef struct _lw6cli_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
  lw6cli_backend_t *backend;
} _lw6cli_test_data_t;

static _lw6cli_test_data_t _test_data = { 0, NULL, NULL };

static int
_call_init (lw6sys_context_t * sys_context, lw6cli_backend_t * backend)
{
  int ret = 1;

  {
    if (lw6cli_init (sys_context, backend))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("successfull init for backend \"%s\", hint_timeout=%d seconds"), backend->name, backend->properties.hint_timeout);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init backend \"%s\""), backend->name);
	ret = 0;
      }
  }

  return ret;
}

static void
_call_quit (lw6sys_context_t * sys_context, lw6cli_backend_t * backend)
{
  lw6cli_quit (sys_context, backend);
}

/*
 * Dummy test
 */
static void
_test_dummy ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("dummy test"));
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init_tcp ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libcli-tcp CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6cli_create_backend (sys_context, argc, argv, "tcp");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6cli_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_tcp ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libcli-tcp CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6cli_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_init_udp ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libcli-udp CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6cli_create_backend (sys_context, argc, argv, "udp");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6cli_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_udp ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libcli-udp CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6cli_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

#ifdef MOD_HTTP
static int
_setup_init_http ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libcli-http CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6cli_create_backend (sys_context, argc, argv, "http");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6cli_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_http ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libcli-http CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6cli_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}
#endif // MOD_HTTP

/**
 * lw6cli_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libcli module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cli_test_register (lw6sys_context_t * sys_context, int mode)
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
      lw6cfg_test_register (sys_context, mode);
      lw6net_test_register (sys_context, mode);
      lw6nod_test_register (sys_context, mode);
      lw6cnx_test_register (sys_context, mode);
      lw6msg_test_register (sys_context, mode);

      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
    }

  suite = CU_add_suite ("lw6cli-tcp", _setup_init_tcp, _setup_quit_tcp);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dummy);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  suite = CU_add_suite ("lw6cli-udp", _setup_init_udp, _setup_quit_udp);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dummy);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

#ifdef MOD_HTTP
  suite = CU_add_suite ("lw6cli-http", _setup_init_http, _setup_quit_http);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dummy);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }
#endif // MOD_HTTP

  return ret;
}

/**
 * lw6cli_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @cli module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cli_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
