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

#include "cli.h"

#define TEST_ARGC 1
#define TEST_ARGV0 "prog"
#define _TEST_NET_LOG 0

static int
test_init (lw6cli_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret && lw6cli_init (backend);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_quit (lw6cli_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  lw6cli_quit (backend);

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6cli_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @cli module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cli_test (int mode)
{
  int ret = 0;
  lw6cli_backend_t *backend;
  int argc = TEST_ARGC;
  char *argv[TEST_ARGC] = { TEST_ARGV0 };

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
    }

  if (lw6net_init (argc, argv, _TEST_NET_LOG))
    {
      ret = 1;

      backend = lw6cli_create_backend (argc, argv, "tcp");
      if (backend)
	{
	  ret = test_init (backend) && test_quit (backend) && ret;
	  lw6cli_destroy_backend (backend);
	}

      backend = lw6cli_create_backend (argc, argv, "udp");
      if (backend)
	{
	  ret = test_init (backend) && test_quit (backend) && ret;
	  lw6cli_destroy_backend (backend);
	}

#ifdef MOD_HTTP
      backend = lw6cli_create_backend (argc, argv, "http");
      if (backend)
	{
	  ret = test_init (backend) && test_quit (backend) && ret;
	  lw6cli_destroy_backend (backend);
	}
#endif
      lw6net_quit ();
    }

  return ret;
}
