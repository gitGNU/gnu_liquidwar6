/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "cnx.h"

#define _TEST_LOCAL_URL "http://ufoot.org:8056/"
#define _TEST_REMOTE_URL "http://ufoot.hd.free.fr:8056/"
#define _TEST_REMOTE_IP "127.0.0.1"
#define _TEST_REMOTE_PORT 8888
#define _TEST_PASSWORD "toto"
#define _TEST_LOCAL_ID "1234123412341234"
#define _TEST_REMOTE_ID "2345234523452345"

static void
_recv_callback_func (void *func_data, char *msg)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _("received \"%s\""), msg);
}

/*
 * Testing functions in connection.c
 */
static int
test_connection ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6cnx_connection_t *cnx = NULL;

    cnx =
      lw6cnx_connection_new (_TEST_LOCAL_URL, _TEST_REMOTE_URL,
			     _TEST_REMOTE_IP, _TEST_REMOTE_PORT,
			     _TEST_PASSWORD, _TEST_LOCAL_ID, _TEST_REMOTE_ID,
			     _recv_callback_func, NULL);
    if (cnx)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("cnx_connection object creation works"));
	lw6cnx_connection_free (cnx);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("cnx_connection object creation failed"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6cnx_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @cnx module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cnx_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
    }

  ret = test_connection ();

  return ret;
}
