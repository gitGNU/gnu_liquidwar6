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

#include "nod.h"

#define _TEST_NB_IP 3
#define _TEST_IP_1 "123.123.123.123"
#define _TEST_IP_2 "1.2.3.4"
#define _TEST_IP_3 "10.10.10.10"
#define _TEST_PORT 10000
#define _TEST_DELAY_MS 10000
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
#define _TEST_LEVEL "toto.map"
#define _TEST_REQUIRED_BENCH 7
#define _TEST_NB_COLORS 3
#define _TEST_MAX_NB_COLORS 10
#define _TEST_NB_CURSORS 6
#define _TEST_MAX_NB_CURSORS 26
#define _TEST_NB_NODES 2
#define _TEST_MAX_NB_NODES 12
#define _TEST_GAME_SCREENSHOT_SIZE 10
#define _TEST_GAME_SCREENSHOT_DATA "123456789"

static void
_node_dup_dyn_callback (void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  int64_t stop_timestamp = 0;
  int first_time = 1;
  lw6nod_dyn_info_t *dyn_info = NULL;
  int len = 0;

  stop_timestamp = lw6sys_get_timestamp () + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp () < stop_timestamp)
    {
      dyn_info = lw6nod_info_dup_dyn (info);
      if (dyn_info)
	{
	  len = strlen (dyn_info->level);	// would segfault on bug
	  if (first_time)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _("dup dyn level=\"%s\""),
			  dyn_info->level);
	      first_time = 0;
	    }
	  lw6nod_dyn_info_free (dyn_info);
	  dyn_info = NULL;
	}
    }
}

static void
_node_update_callback (void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  int64_t stop_timestamp = 0;
  int first_time = 1;
  int ret = 0;

  stop_timestamp = lw6sys_get_timestamp () + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp () < stop_timestamp)
    {
      ret =
	lw6nod_info_update (info, _TEST_LEVEL, _TEST_REQUIRED_BENCH,
			    _TEST_NB_COLORS, _TEST_MAX_NB_COLORS,
			    _TEST_NB_CURSORS, _TEST_MAX_NB_CURSORS,
			    _TEST_NB_NODES, _TEST_MAX_NB_NODES,
			    _TEST_GAME_SCREENSHOT_SIZE,
			    _TEST_GAME_SCREENSHOT_DATA);
      if (first_time)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE, _("update dyn info ret=%d"), ret);
	  first_time = 0;
	}
    }
}

static void
_node_add_discovered_callback (void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  char *ip = NULL;
  char *url = NULL;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp () + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp () < stop_timestamp)
    {
      switch (lw6sys_random (_TEST_NB_IP))
	{
	case 0:
	  ip = _TEST_IP_1;
	  break;
	case 1:
	  ip = _TEST_IP_2;
	  break;
	default:
	  ip = _TEST_IP_3;
	  break;
	}
      url = lw6sys_url_http_from_ip_port (ip, _TEST_PORT);
      if (url)
	{
	  if (first_time)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _("add_discovered_node \"%s\""),
			  url);
	      first_time = 0;
	    }
	  lw6nod_info_add_discovered_node (info, url);
	  LW6SYS_FREE (url);
	}
    }
}

static void
_node_pop_discovered_callback (void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  lw6sys_list_t *discovered_nodes = NULL;
  char *url = NULL;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp () + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp () < stop_timestamp)
    {
      discovered_nodes = lw6nod_info_pop_discovered_nodes (info);
      if (discovered_nodes)
	{
	  if (first_time)
	    {
	      url = lw6sys_list_pop_front (&discovered_nodes);
	      if (url)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _("pop_discovered_node \"%s\""), url);
		  LW6SYS_FREE (url);
		}
	      first_time = 0;
	    }
	  // could be NULL because of pop
	  if (discovered_nodes)
	    {
	      lw6sys_list_free (discovered_nodes);
	    }
	}
    }
}

static void
_node_set_verified_callback (void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  lw6sys_hash_t *hash = NULL;
  char *url = NULL;
  lw6nod_info_t *verified_node = NULL;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp () + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp () < stop_timestamp)
    {
      hash = lw6nod_info_new_verified_nodes ();
      if (hash)
	{
	  url = lw6sys_url_http_from_ip_port (_TEST_IP_1, _TEST_PORT);
	  if (url)
	    {
	      verified_node =
		lw6nod_info_new (_TEST_ID_1, url, _TEST_TITLE,
				 _TEST_DESCRIPTION, _TEST_BENCH,
				 _TEST_IDLE_SCREENSHOT_SIZE,
				 _TEST_IDLE_SCREENSHOT_DATA);
	      if (verified_node)
		{
		  lw6sys_hash_set (hash, url, verified_node);
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
	      if (verified_node)
		{
		  lw6sys_hash_set (hash, url, verified_node);
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
	      if (verified_node)
		{
		  lw6sys_hash_set (hash, url, verified_node);
		}
	      LW6SYS_FREE (url);
	    }

	  if (first_time)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _("setting list of verified nodes"));
	      first_time = 0;
	    }
	  lw6nod_info_set_verified_nodes (info, hash);
	  // no need to free hash
	}
    }
}

static void
_node_map_verified_callback_callback (void *func_data, char *key, void *value)
{
  int *first_time = (int *) func_data;
  lw6nod_info_t *verified_node = (lw6nod_info_t *) value;

  if (key && verified_node)
    {
      if (!strcmp (key, verified_node->const_info.url))
	{
	  if (*first_time)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _("verified node \"%s\""), key);
	      (*first_time) = 0;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("inconsistent data for verified nodes callback key=\"%s\" const_info.url=\"%s\""),
		      key, verified_node->const_info.url);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("inconsistent pointers for verified nodes callback (key=%p, value=%p)"),
		  key, value);
    }
}

static void
_node_map_verified_callback (void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp () + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp () < stop_timestamp)
    {
      lw6nod_info_map_verified_nodes (info,
				      _node_map_verified_callback_callback,
				      &first_time);
      // no need to free list
    }
}

/*
 * Testing functions in node.c
 */
static int
test_node ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6nod_info_t *info = NULL;
    void *thread_add_discovered = NULL;
    void *thread_pop_discovered = NULL;
    void *thread_set_verified = NULL;
    void *thread_map_verified = NULL;
    void *thread_update = NULL;
    void *thread_dup_dyn = NULL;
    char *url = NULL;
    char *oob = NULL;
    lw6sys_list_t *list = NULL;

    ret = 0;
    info =
      lw6nod_info_new (_TEST_ID, _TEST_URL, _TEST_TITLE, _TEST_DESCRIPTION,
		       _TEST_BENCH, _TEST_IDLE_SCREENSHOT_SIZE,
		       _TEST_IDLE_SCREENSHOT_DATA);
    if (info)
      {
	lw6nod_info_update (info, _TEST_LEVEL, _TEST_REQUIRED_BENCH,
			    _TEST_NB_COLORS, _TEST_MAX_NB_COLORS,
			    _TEST_NB_CURSORS, _TEST_MAX_NB_CURSORS,
			    _TEST_NB_NODES, _TEST_MAX_NB_NODES,
			    _TEST_GAME_SCREENSHOT_SIZE,
			    _TEST_GAME_SCREENSHOT_DATA);
	lw6nod_info_idle (info);
	lw6nod_info_update (info, _TEST_LEVEL, _TEST_REQUIRED_BENCH,
			    _TEST_NB_COLORS, _TEST_MAX_NB_COLORS,
			    _TEST_NB_CURSORS, _TEST_MAX_NB_CURSORS,
			    _TEST_NB_NODES, _TEST_MAX_NB_NODES,
			    _TEST_GAME_SCREENSHOT_SIZE,
			    _TEST_GAME_SCREENSHOT_DATA);

	if (lw6nod_info_add_discovered_node (info, _TEST_URL))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("add \"%s\""), _TEST_URL);
	    list = lw6nod_info_pop_discovered_nodes (info);
	    if (list && !lw6sys_list_is_empty (list))
	      {
		url = lw6sys_list_pop_front (&list);
		if (url)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE, _("pop \"%s\""), url);
		    LW6SYS_FREE (url);
		  }
	      }
	    if (list)
	      {
		lw6sys_list_free (list);
	      }
	  }
	/*
	 * Threads are (on purpose) started in what seems an illogical order,
	 * the idea is to be sure to see how program behaves when querying the
	 * object in various cases, including unprobable ones.
	 */
	thread_map_verified =
	  lw6sys_thread_create (_node_map_verified_callback, NULL, info);
	if (thread_map_verified)
	  {
	    thread_set_verified =
	      lw6sys_thread_create (_node_set_verified_callback, NULL, info);
	    if (thread_set_verified)
	      {
		thread_pop_discovered =
		  lw6sys_thread_create (_node_pop_discovered_callback, NULL,
					info);
		if (thread_pop_discovered)
		  {
		    thread_add_discovered =
		      lw6sys_thread_create (_node_add_discovered_callback,
					    NULL, info);
		    if (thread_add_discovered)
		      {

			thread_update =
			  lw6sys_thread_create (_node_update_callback,
						NULL, info);
			if (thread_update)
			  {

			    thread_dup_dyn =
			      lw6sys_thread_create (_node_dup_dyn_callback,
						    NULL, info);
			    if (thread_dup_dyn)
			      {


				lw6sys_log (LW6SYS_LOG_NOTICE,
					    _
					    ("6 threads started, each one querying the same node info object"));
				ret = 1;
				lw6sys_thread_join (thread_dup_dyn);
			      }
			    lw6sys_thread_join (thread_update);
			  }
			lw6sys_thread_join (thread_add_discovered);
		      }
		    lw6sys_thread_join (thread_pop_discovered);
		  }
		lw6sys_thread_join (thread_set_verified);
	      }
	    lw6sys_thread_join (thread_map_verified);
	  }

	oob = lw6nod_info_generate_oob_info (info);
	if (oob)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("standard oob INFO is \"%s\""),
			oob);
	    LW6SYS_FREE (oob);
	  }
	oob = lw6nod_info_generate_oob_list (info);
	if (oob)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("standard oob LIST is \"%s\""),
			oob);
	    LW6SYS_FREE (oob);
	  }

	oob = lw6nod_info_generate_oob_pong (info);
	if (oob)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("standard oob PONG is \"%s\""),
			oob);
	    LW6SYS_FREE (oob);
	  }

	lw6nod_info_free (info);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6nod_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @nod module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6nod_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
    }

  ret = test_node ();

  return ret;
}
