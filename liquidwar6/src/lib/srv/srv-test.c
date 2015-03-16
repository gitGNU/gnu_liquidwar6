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

#include "srv.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"
#define _TEST_NET_LOG 0

#define _TEST_NB_BACKENDS 3

#define _NODE_INFO_ID 0x1234123412341234LL
#define _NODE_INFO_URL "http://localhost/"
#define _NODE_INFO_TITLE "test node"
#define _NODE_INFO_DESCRIPTION "this is a test node"
#define _NODE_INFO_BENCH 10
#define _NODE_INFO_IDLE_SCREENSHOT_SIZE 4
#define _NODE_INFO_IDLE_SCREENSHOT_DATA "test"

#define _TEST_FAKE_SOCK 7
#define _TEST_LINE "foo bar BARfoo bar.bar"

#define _TEST_OOB_REMOTE_IP "1.2.3.4"
#define _TEST_OOB_REMOTE_PORT 6789
#define _TEST_OOB_INVALID_SOCK -1

#define _TEST_PORT (LW6NET_DEFAULT_PORT+2)

typedef struct _lw6srv_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
  lw6srv_backend_t *backend[_TEST_NB_BACKENDS];
  lw6srv_listener_t *listener;
} _lw6srv_test_data_t;

static _lw6srv_test_data_t _test_data = { 0, NULL, {NULL, NULL, NULL}, NULL };

static void
_test_oob ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6srv_oob_t *oob;

    oob = lw6srv_oob_new (sys_context, _TEST_OOB_REMOTE_IP, _TEST_OOB_REMOTE_PORT, _TEST_OOB_INVALID_SOCK, _TEST_LINE);
    if (oob)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created oob object"));
	lw6srv_oob_free (sys_context, oob);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_tcp_accepter ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6srv_tcp_accepter_t *tcp_accepter = NULL;
    char *ip = NULL;

    ip = lw6sys_str_copy (sys_context, LW6NET_ADDRESS_LOOPBACK);
    if (ip)
      {
	tcp_accepter = lw6srv_tcp_accepter_new (sys_context, ip, _TEST_PORT, _TEST_FAKE_SOCK);
	if (tcp_accepter)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("tcp_accepter %s:%d"), tcp_accepter->client_id.client_ip, tcp_accepter->client_id.client_port);
	    lw6srv_tcp_accepter_free (sys_context, tcp_accepter);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create tcp_accepter object"));
	    LW6SYS_FREE (sys_context, ip);
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_udp_buffer ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6srv_udp_buffer_t *udp_buffer = NULL;
    char *ip = NULL;
    char *line = NULL;

    ip = lw6sys_str_copy (sys_context, LW6NET_ADDRESS_LOOPBACK);
    if (ip)
      {
	line = lw6sys_str_copy (sys_context, _TEST_LINE);
	if (line)
	  {
	    udp_buffer = lw6srv_udp_buffer_new (sys_context, ip, _TEST_PORT, line);
	    if (udp_buffer)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("udp_buffer %s:%d"), udp_buffer->client_id.client_ip, udp_buffer->client_id.client_port);
		lw6srv_udp_buffer_free (sys_context, udp_buffer);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create udp_buffer object"));
		LW6SYS_FREE (sys_context, line);
		LW6SYS_FREE (sys_context, ip);
		ret = 0;
	      }
	  }
	else
	  {
	    LW6SYS_FREE (sys_context, ip);
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Dummy test
 */
static void
_test_dummy ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("dummy test"));
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libsrv CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libsrv CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_init_listener ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };
  int i = 0;
  int ok = 1;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libsrv-listener CUnit test suite"));

  if (lw6net_init (sys_context, argc, argv, _TEST_NET_LOG))
    {
      _test_data.backend[0] = lw6srv_create_backend (sys_context, argc, argv, "tcpd");
      _test_data.backend[1] = lw6srv_create_backend (sys_context, argc, argv, "udpd");
      _test_data.backend[2] = lw6srv_create_backend (sys_context, argc, argv, "httpd");

      if (_test_data.backend[0] && _test_data.backend[1] && _test_data.backend[2])
	{
	  _test_data.listener = lw6srv_start (sys_context, LW6NET_ADDRESS_ANY, _TEST_PORT);
	  if (_test_data.listener)
	    {
	      for (i = 0; i < _TEST_NB_BACKENDS; ++i)
		{
		  if (ok)
		    {
		      if (lw6srv_init (sys_context, _test_data.backend[i], _test_data.listener))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				      _x_
				      ("successfull init for backend \"%s\", hint_timeout=%d seconds"),
				      _test_data.backend[i]->name, _test_data.backend[i]->properties.hint_timeout);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init backend \"%s\""), _test_data.backend[i]->name);
			  ok = 0;
			}
		      if (ok)
			{
			  ret = CUE_SUCCESS;
			}
		    }
		}
	    }
	}
    }

  return ret;
}

static int
_setup_quit_listener ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int i = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libsrv-listener CUnit test suite"));

  if (_test_data.listener)
    {
      if (_test_data.backend[0] && _test_data.backend[1] && _test_data.backend[2])
	{
	  for (i = 0; i < _TEST_NB_BACKENDS; ++i)
	    {
	      lw6srv_quit (sys_context, _test_data.backend[i]);
	    }
	  ret = CUE_SUCCESS;
	}
      lw6srv_stop (sys_context, _test_data.listener);
      _test_data.listener = NULL;
      if (_test_data.backend[0] && _test_data.backend[1] && _test_data.backend[2])
	{
	  for (i = 0; i < _TEST_NB_BACKENDS; ++i)
	    {
	      lw6srv_destroy_backend (sys_context, _test_data.backend[i]);
	      _test_data.backend[i] = NULL;
	    }
	}
    }
  lw6net_quit (sys_context);

  return ret;
}

/**
 * lw6srv_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libsrv module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6srv_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite;

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

  suite = CU_add_suite ("lw6srv", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_oob);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_tcp_accepter);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_udp_buffer);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  suite = CU_add_suite ("lw6srv-listener", _setup_init_listener, _setup_quit_listener);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dummy);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6srv_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @srv module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6srv_test_run (lw6sys_context_t * sys_context, int mode)
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
