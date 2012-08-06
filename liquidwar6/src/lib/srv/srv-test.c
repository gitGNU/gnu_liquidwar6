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
#endif

#include "srv.h"

#define TEST_ARGC 1
#define TEST_ARGV0 "prog"
#define _TEST_NET_LOG 0

#define TEST_NB_BACKENDS 3

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

static int
_test_oob ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6srv_oob_t *oob;

    oob =
      lw6srv_oob_new (_TEST_OOB_REMOTE_IP, _TEST_OOB_REMOTE_PORT,
		      _TEST_OOB_INVALID_SOCK, _TEST_LINE);
    if (oob)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created oob object"));
	lw6srv_oob_free (oob);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
_test_tcp_accepter ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6srv_tcp_accepter_t *tcp_accepter = NULL;
    char *ip = NULL;

    ip = lw6sys_str_copy (LW6NET_ADDRESS_LOOPBACK);
    if (ip)
      {
	tcp_accepter =
	  lw6srv_tcp_accepter_new (ip, LW6NET_DEFAULT_PORT, _TEST_FAKE_SOCK);
	if (tcp_accepter)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("tcp_accepter %s:%d"),
			tcp_accepter->client_id.client_ip,
			tcp_accepter->client_id.client_port);
	    lw6srv_tcp_accepter_free (tcp_accepter);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("unable to create tcp_accepter object"));
	    LW6SYS_FREE (ip);
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
_test_udp_buffer ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6srv_udp_buffer_t *udp_buffer = NULL;
    char *ip = NULL;
    char *line = NULL;

    ip = lw6sys_str_copy (LW6NET_ADDRESS_LOOPBACK);
    if (ip)
      {
	line = lw6sys_str_copy (_TEST_LINE);
	if (line)
	  {
	    udp_buffer =
	      lw6srv_udp_buffer_new (ip, LW6NET_DEFAULT_PORT, line);
	    if (udp_buffer)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("udp_buffer %s:%d"),
			    udp_buffer->client_id.client_ip,
			    udp_buffer->client_id.client_port);
		lw6srv_udp_buffer_free (udp_buffer);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("unable to create udp_buffer object"));
		LW6SYS_FREE (line);
		LW6SYS_FREE (ip);
		ret = 0;
	      }
	  }
	else
	  {
	    LW6SYS_FREE (ip);
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
_test_init (lw6srv_backend_t ** backend, lw6srv_listener_t * listener)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;

    for (i = 0; i < TEST_NB_BACKENDS; ++i)
      {
	if (ret)
	  {
	    if (lw6srv_init (backend[i], listener))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("successfull init for backend \"%s\", hint_timeout=%d seconds"),
			    backend[i]->name,
			    backend[i]->properties.hint_timeout);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("unable to init backend \"%s\""),
			    backend[i]->name);
		ret = 0;
	      }
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
_test_quit (lw6srv_backend_t ** backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;

    for (i = 0; i < TEST_NB_BACKENDS; ++i)
      {
	lw6srv_quit (backend[i]);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6srv_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @srv module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6srv_test (int mode)
{
  int ret = 0;
  lw6srv_backend_t *backend[TEST_NB_BACKENDS];
  lw6srv_listener_t *listener = NULL;
  const int argc = TEST_ARGC;
  const char *argv[TEST_ARGC] = { TEST_ARGV0 };

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6glb_test (mode);
      lw6cfg_test (mode);
      lw6net_test (mode);
      lw6nod_test (mode);
      lw6cnx_test (mode);
      lw6msg_test (mode);
      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
    }

  ret = _test_oob () && _test_tcp_accepter () && _test_udp_buffer ();

  if (ret)
    {
      if (lw6net_init (argc, argv, _TEST_NET_LOG))
	{
	  backend[0] = lw6srv_create_backend (argc, argv, "tcpd");
	  backend[1] = lw6srv_create_backend (argc, argv, "udpd");
	  backend[2] = lw6srv_create_backend (argc, argv, "httpd");

	  if (backend[0] && backend[1] && backend[2])
	    {
	      listener =
		lw6srv_start (LW6NET_ADDRESS_ANY, LW6NET_DEFAULT_PORT);
	      if (listener)
		{
		  ret = _test_init (backend, listener) && _test_quit (backend)
		    && ret;
		  lw6srv_stop (listener);
		}
	      lw6srv_destroy_backend (backend[0]);
	      lw6srv_destroy_backend (backend[1]);
	      lw6srv_destroy_backend (backend[2]);
	    }
	  lw6net_quit ();
	}
    }

  return ret;
}
