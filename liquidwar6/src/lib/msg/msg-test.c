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
#define _TEST_PASSWORD "toto"
#define _TEST_BENCH 10
#define _TEST_IDLE_SCREENSHOT_SIZE 5
#define _TEST_IDLE_SCREENSHOT_DATA "1234"
#define _TEST_REQUEST_COMMAND "PING"
#define _TEST_REQUEST_REMOTE_URL "http://ufoot.hd.free.fr/"
#define _TEST_REQUEST_PASSWORD "toto"
#define _TEST_REQUEST_LOCAL_URL "http://ufoot.org/"
#define _TEST_KEY_VALUE_KO " \t"
#define _TEST_KEY_VALUE_OK_1 "KEY value"
#define _TEST_KEY_VALUE_OK_2 "  KEY2HASNOVALUE"
#define _TEST_KEY_VALUE_OK_3 "KEY3looks-STRANGE\t= value 3 even\t stranger but it does work"
#define _TEST_DEFAULT_KEY_OK "KEY"
#define _TEST_DEFAULT_KEY_KO "???"
#define _TEST_DEFAULT_VALUE_STR "abc"
#define _TEST_DEFAULT_VALUE_INT 123

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
    char *pong_url = NULL;
    char *request = NULL;
    int syntax_ok = 0;
    char *command = NULL;
    int password_ok = 0;
    char *remote_url = NULL;

    info =
      lw6nod_info_new (_TEST_ID, _TEST_URL, _TEST_TITLE, _TEST_DESCRIPTION,
		       _TEST_PASSWORD, _TEST_BENCH,
		       _TEST_IDLE_SCREENSHOT_SIZE,
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
	    pong_url = lw6msg_oob_analyse_pong (oob);
	    if (pong_url)
	      {
		if (lw6sys_str_is_same (pong_url, _TEST_URL))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_("got url \"%s\" from PONG"), pong_url);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_
				("PONG message parsed but read \"%s\" instead of \"%s\""),
				pong_url, _TEST_URL);
		    ret = 0;
		  }
		LW6SYS_FREE (pong_url);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _("unable to parse PONG message \"%s\""), oob);
		ret = 0;
	      }
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
				   _TEST_DESCRIPTION, _TEST_PASSWORD,
				   _TEST_BENCH, _TEST_IDLE_SCREENSHOT_SIZE,
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
				   _TEST_DESCRIPTION, _TEST_PASSWORD,
				   _TEST_BENCH, _TEST_IDLE_SCREENSHOT_SIZE,
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
				   _TEST_DESCRIPTION, _TEST_PASSWORD,
				   _TEST_BENCH, _TEST_IDLE_SCREENSHOT_SIZE,
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

	    oob = lw6msg_oob_generate_list (info);
	    if (oob)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("standard (populated) oob LIST is \"%s\""),
			    oob);
		LW6SYS_FREE (oob);
	      }
	  }

	request =
	  lw6msg_oob_generate_request (_TEST_REQUEST_COMMAND,
				       _TEST_REQUEST_REMOTE_URL,
				       _TEST_REQUEST_PASSWORD,
				       _TEST_REQUEST_LOCAL_URL);
	if (request)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_
			("request with command=\"%s\" password=\"%s\" url=\"%s\" is \"%s\""),
			_TEST_REQUEST_COMMAND, _TEST_REQUEST_PASSWORD,
			_TEST_REQUEST_LOCAL_URL, request);
	    if (lw6msg_oob_analyse_request
		(&syntax_ok, &command, &password_ok, &remote_url, request,
		 _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		if (remote_url)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_
				("request \"%s\" analysed syntax_ok=%d password_ok=%d remote_url=\"%s\""),
				request, syntax_ok, password_ok, remote_url);
		    LW6SYS_FREE (remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"),
			    request, syntax_ok, password_ok);
		ret = 0;
	      }
	    LW6SYS_FREE (request);
	  }
	request =
	  lw6msg_oob_generate_request (_TEST_REQUEST_COMMAND,
				       _TEST_REQUEST_REMOTE_URL,
				       _TEST_REQUEST_PASSWORD, NULL);
	if (request)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_
			("request with command=\"%s\" password=\"%s\" url=NULL is \"%s\""),
			_TEST_REQUEST_COMMAND, _TEST_REQUEST_PASSWORD,
			request);
	    if (lw6msg_oob_analyse_request
		(&syntax_ok, &command, &password_ok, &remote_url, request,
		 _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _
			    ("request \"%s\" analysed syntax_ok=%d password_ok=%d"),
			    request, syntax_ok, password_ok);
		if (remote_url)
		  {
		    LW6SYS_FREE (remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"),
			    request, syntax_ok, password_ok);
		ret = 0;
	      }
	    LW6SYS_FREE (request);
	  }
	request =
	  lw6msg_oob_generate_request (_TEST_REQUEST_COMMAND, NULL, NULL,
				       _TEST_REQUEST_LOCAL_URL);
	if (request)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_
			("request with command=\"%s\" password=NULL url=\"%s\" is \"%s\""),
			_TEST_REQUEST_COMMAND, _TEST_REQUEST_LOCAL_URL,
			request);
	    if (lw6msg_oob_analyse_request
		(&syntax_ok, &command, &password_ok, &remote_url, request,
		 NULL, NULL))
	      {
		if (remote_url)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_
				("request \"%s\" analysed syntax_ok=%d password_ok=%d remote_url=\"%s\""),
				request, syntax_ok, password_ok, remote_url);
		    LW6SYS_FREE (remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"),
			    request, syntax_ok, password_ok);
		ret = 0;
	      }
	    if (lw6msg_oob_analyse_request
		(&syntax_ok, &command, &password_ok, &remote_url, request,
		 _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("request \"%s\" is reported OK but password is wrong"),
			    request);
		if (remote_url)
		  {
		    LW6SYS_FREE (remote_url);
		  }
		ret = 0;
	      }
	    else
	      {
		if (password_ok)
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_
				("request \"%s\" is reported KO with password OK, this is not logic, password *is* wrong"),
				request);
		    ret = 0;
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_("password checking is OK"));
		  }
	      }
	    LW6SYS_FREE (request);
	  }
	request =
	  lw6msg_oob_generate_request (_TEST_REQUEST_COMMAND, NULL, NULL,
				       NULL);
	if (request)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_
			("request with command=\"%s\" password=NULL url=NULL is \"%s\""),
			_TEST_REQUEST_COMMAND, request);
	    if (lw6msg_oob_analyse_request
		(&syntax_ok, &command, &password_ok, &remote_url, request,
		 NULL, NULL))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _
			    ("request \"%s\" analysed syntax_ok=%d password_ok=%d"),
			    request, syntax_ok, password_ok);
		if (remote_url)
		  {
		    LW6SYS_FREE (remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"),
			    request, syntax_ok, password_ok);
		ret = 0;
	      }
	    if (lw6msg_oob_analyse_request
		(&syntax_ok, &command, &password_ok, &remote_url, request,
		 _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("request \"%s\" is reported OK but password is wrong"),
			    request);
		if (remote_url)
		  {
		    LW6SYS_FREE (remote_url);
		  }
		ret = 0;
	      }
	    else
	      {
		if (password_ok)
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_
				("request \"%s\" is reported KO with password OK, this is not logic, password *is* wrong"),
				request);
		    ret = 0;
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_("password checking is OK"));
		  }
	      }
	    LW6SYS_FREE (request);
	  }

	lw6nod_info_free (info);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
_key_value_assoc_callback (void *func_data, char *key, void *value)
{
  char *value_str = (char *) value;
  int *count = (int *) func_data;

  (*count)++;
  lw6sys_log (LW6SYS_LOG_NOTICE, _("count=%d found key=\"%s\" value=\"%s\""),
	      *count, key, value_str);
}

/*
 * Testing functions in oob.c
 */
static int
test_utils ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *key = NULL;
    char *value = NULL;
    lw6sys_assoc_t *assoc = NULL;
    int count = 0;
    char *value_str = NULL;
    int value_int = 0;

    if (lw6msg_utils_parse_key_value_to_ptr
	(&key, &value, _TEST_KEY_VALUE_OK_1))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("line \"%s\" parsed key=\"%s\" value=\"%s\""),
		    _TEST_KEY_VALUE_OK_1, key, value);
	LW6SYS_FREE (key);
	LW6SYS_FREE (value);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to parse \"%s\""),
		    _TEST_KEY_VALUE_OK_1);
	ret = 0;
      }
    if (lw6msg_utils_parse_key_value_to_ptr
	(&key, &value, _TEST_KEY_VALUE_KO))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _
		    ("line \"%s\" parsed key=\"%s\" value=\"%s\" but this should have returned NULL"),
		    _TEST_KEY_VALUE_KO, key, value);
	LW6SYS_FREE (key);
	LW6SYS_FREE (value);
	ret = 0;
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("unable to parse \"%s\", this is normal"),
		    _TEST_KEY_VALUE_KO);
      }
    assoc = lw6sys_assoc_new (lw6sys_free_callback);
    if (assoc)
      {
	lw6msg_utils_parse_key_value_to_assoc (&assoc, _TEST_KEY_VALUE_OK_1);
	lw6msg_utils_parse_key_value_to_assoc (&assoc, _TEST_KEY_VALUE_OK_2);
	lw6msg_utils_parse_key_value_to_assoc (&assoc, _TEST_KEY_VALUE_OK_3);
	lw6msg_utils_parse_key_value_to_assoc (&assoc, _TEST_KEY_VALUE_KO);
	count = 0;
	lw6sys_assoc_map (assoc, _key_value_assoc_callback, &count);
	ret = ret && (count == 3);

	value_str =
	  lw6msg_utils_get_assoc_str_with_default (assoc,
						   _TEST_DEFAULT_KEY_OK,
						   _TEST_DEFAULT_VALUE_STR);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("value for \"%s\" is \"%s\""),
		    _TEST_DEFAULT_KEY_OK, value_str);
	value_str =
	  lw6msg_utils_get_assoc_str_with_default (assoc,
						   _TEST_DEFAULT_KEY_KO,
						   _TEST_DEFAULT_VALUE_STR);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("value for \"%s\" is \"%s\""),
		    _TEST_DEFAULT_KEY_KO, value_str);
	value_int =
	  lw6msg_utils_get_assoc_int_with_default (assoc,
						   _TEST_DEFAULT_KEY_OK,
						   _TEST_DEFAULT_VALUE_INT);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("value for \"%s\" is %d"),
		    _TEST_DEFAULT_KEY_OK, value_int);
	value_int =
	  lw6msg_utils_get_assoc_int_with_default (assoc,
						   _TEST_DEFAULT_KEY_KO,
						   _TEST_DEFAULT_VALUE_INT);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("value for \"%s\" is %d"),
		    _TEST_DEFAULT_KEY_KO, value_int);

	lw6sys_assoc_free (assoc);
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
      lw6glb_test (mode);
      lw6nod_test (mode);
    }

  ret = test_oob () && test_utils ();

  return ret;
}
