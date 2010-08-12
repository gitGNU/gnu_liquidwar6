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

#include "msg.h"

#define _TEST_IP_1 "123.123.123.123"
#define _TEST_IP_2 "1.2.3.4"
#define _TEST_IP_3 "10.10.10.10"
#define _TEST_PORT 10000
#define _TEST_ID 0x1212323242425252LL
#define _TEST_ID_1 0x1234123412341234LL
#define _TEST_ID_2 0x2345234523452345LL
#define _TEST_ID_3 0x3456345634563456LL
#define _TEST_URL "http://192.168.20.20:8000/"
#define _TEST_TITLE "This is not a sentence"
#define _TEST_DESCRIPTION "This is not an explanation about what this is."
#define _TEST_BENCH 10
#define _TEST_IDLE_SCREENSHOT_SIZE 5
#define _TEST_IDLE_SCREENSHOT_DATA "1234"

/*
 * Testing functions in oob.c
 */
static int
test_oob ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6nod_info_t *info = NULL;
    char *oob = NULL;
    lw6sys_list_t *list = NULL;
    char *url = NULL;
    lw6nod_info_t *verified_node = NULL;

    info =
      lw6nod_info_new (_TEST_ID, _TEST_URL, _TEST_TITLE, _TEST_DESCRIPTION,
		       _TEST_BENCH, _TEST_IDLE_SCREENSHOT_SIZE,
		       _TEST_IDLE_SCREENSHOT_DATA);
    if (info)
      {
	oob = lw6msg_oob_generate_info (info);
	if (oob)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("standard oob INFO is \"%s\""),
			oob);
	    LW6SYS_FREE (oob);
	  }
	oob = lw6msg_oob_generate_list (info);
	if (oob)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("standard (empty) oob LIST is \"%s\""), oob);
	    LW6SYS_FREE (oob);
	  }

	oob = lw6msg_oob_generate_pong (info);
	if (oob)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("standard oob PONG is \"%s\""),
			oob);
	    LW6SYS_FREE (oob);
	  }

	list = lw6nod_info_new_verified_nodes ();
	if (list)
	  {
	    url = lw6sys_url_http_from_ip_port (_TEST_IP_1, _TEST_PORT);
	    if (url)
	      {
		verified_node =
		  lw6nod_info_new (_TEST_ID_1, url, _TEST_TITLE,
				   _TEST_DESCRIPTION, _TEST_BENCH,
				   _TEST_IDLE_SCREENSHOT_SIZE,
				   _TEST_IDLE_SCREENSHOT_DATA);
		if (verified_node && list)
		  {
		    lw6sys_list_push_front (&list, verified_node);
		  }
		LW6SYS_FREE (url);
	      }
	    url = lw6sys_url_http_from_ip_port (_TEST_IP_2, _TEST_PORT);
	    if (url)
	      {
		verified_node =
		  lw6nod_info_new (_TEST_ID_2, url, _TEST_TITLE,
				   _TEST_DESCRIPTION, _TEST_BENCH,
				   _TEST_IDLE_SCREENSHOT_SIZE,
				   _TEST_IDLE_SCREENSHOT_DATA);
		if (verified_node && list)
		  {
		    lw6sys_list_push_front (&list, verified_node);
		  }
		LW6SYS_FREE (url);
	      }
	    url = lw6sys_url_http_from_ip_port (_TEST_IP_3, _TEST_PORT);
	    if (url)
	      {
		verified_node =
		  lw6nod_info_new (_TEST_ID_3, url, _TEST_TITLE,
				   _TEST_DESCRIPTION, _TEST_BENCH,
				   _TEST_IDLE_SCREENSHOT_SIZE,
				   _TEST_IDLE_SCREENSHOT_DATA);
		if (verified_node && list)
		  {
		    lw6sys_list_push_front (&list, verified_node);
		  }
		LW6SYS_FREE (url);
	      }

	    if (list)
	      {
		lw6nod_info_set_verified_nodes (info, list);
	      }

	    if (oob)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("standard (populated) oob LIST is \"%s\""),
			    oob);
		LW6SYS_FREE (oob);
	      }
	  }

	lw6nod_info_free (info);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6msg_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @nod module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6msg_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6nod_test (mode);
    }

  ret = test_oob ();

  return ret;
}
