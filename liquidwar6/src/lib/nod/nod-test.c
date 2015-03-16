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

#include "nod.h"

#define _TEST_NB_IP 3
#define _TEST_IP_1 "123.123.123.123"
#define _TEST_IP_2 "1.2.3.4"
#define _TEST_IP_3 "10.10.10.10"
#define _TEST_PORT 10000
#define _TEST_DELAY_MS 5000
#define _TEST_PROGRAM "beewar7"
#define _TEST_VERSION "777"
#define _TEST_CODENAME "laia"
#define _TEST_STAMP 2
#define _TEST_ID 0x1212323242425252LL
#define _TEST_ID_1 0x1234123412341234LL
#define _TEST_ID_2 0x2345234523452345LL
#define _TEST_ID_3 0x3456345634563456LL
#define _TEST_URL "http://192.168.20.20:8000/"
#define _TEST_URL_1 "http://foo.com/"
#define _TEST_URL_2 "http://bar.com/"
#define _TEST_URL_3 "http://void.com/"
#define _TEST_TITLE "This is not a sentence"
#define _TEST_DESCRIPTION "This is not an explanation about what this is."
#define _TEST_PASSWORD "toto"
#define _TEST_BENCH 10
#define _TEST_OPEN_RELAY 0
#define _TEST_UPTIME 60
#define _TEST_IDLE_SCREENSHOT_SIZE 5
#define _TEST_IDLE_SCREENSHOT_DATA "1234"
#define _TEST_COMMUNITY 0x4321432143214321LL
#define _TEST_COMMUNITY_NB_IDS_WITHOUT_URL 2
#define _TEST_ROUND 5432
#define _TEST_LEVEL "toto.map"
#define _TEST_REQUIRED_BENCH 7
#define _TEST_NB_COLORS 3
#define _TEST_MAX_NB_COLORS 10
#define _TEST_NB_CURSORS 6
#define _TEST_MAX_NB_CURSORS 26
#define _TEST_NB_NODES 2
#define _TEST_MAX_NB_NODES 12
#define _TEST_PEER_ID_LIST "1111222233334444,2222333344441111,3333444411112222,4444111122223333"
#define _TEST_GAME_SCREENSHOT_SIZE 10
#define _TEST_GAME_SCREENSHOT_DATA "123456789"
#define _TEST_PEER_ID_LIST_STR "2345234523452345,3456345634563456"

typedef struct _lw6nod_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6nod_test_data_t;

static _lw6nod_test_data_t _test_data = { 0, NULL };

static void
_node_dup_dyn_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  int64_t stop_timestamp = 0;
  int first_time = 1;
  lw6nod_dyn_info_t *dyn_info = NULL;

  stop_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp (sys_context) < stop_timestamp)
    {
      dyn_info = lw6nod_info_dup_dyn (sys_context, info);
      if (dyn_info)
	{
	  if (first_time)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("dup dyn level=\"%s\""), dyn_info->level);
	      first_time = 0;
	    }
	  lw6nod_dyn_info_free (sys_context, dyn_info);
	  dyn_info = NULL;
	}
    }
}

static void
_node_update_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  int64_t stop_timestamp = 0;
  int first_time = 1;
  int ret = 0;

  stop_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp (sys_context) < stop_timestamp)
    {
      ret =
	lw6nod_info_update (sys_context, info, _TEST_COMMUNITY, _TEST_ROUND, _TEST_LEVEL,
			    _TEST_REQUIRED_BENCH, _TEST_NB_COLORS,
			    _TEST_MAX_NB_COLORS, _TEST_NB_CURSORS,
			    _TEST_MAX_NB_CURSORS, _TEST_NB_NODES,
			    _TEST_MAX_NB_NODES, _TEST_PEER_ID_LIST, _TEST_GAME_SCREENSHOT_SIZE, _TEST_GAME_SCREENSHOT_DATA);
      if (first_time)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("update dyn info ret=%d"), ret);
	  first_time = 0;
	}
    }
}

static void
_node_add_discovered_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  char *ip = NULL;
  char *url = NULL;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp (sys_context) < stop_timestamp)
    {
      switch (lw6sys_random (sys_context, _TEST_NB_IP))
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
      url = lw6sys_url_http_from_ip_port (sys_context, ip, _TEST_PORT);
      if (url)
	{
	  if (first_time)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("add_discovered_node \"%s\""), url);
	      first_time = 0;
	    }
	  lw6nod_info_add_discovered_node (sys_context, info, url);
	  LW6SYS_FREE (sys_context, url);
	}
    }
}

static void
_node_pop_discovered_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  lw6sys_list_t *discovered_nodes = NULL;
  char *url = NULL;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp (sys_context) < stop_timestamp)
    {
      discovered_nodes = lw6nod_info_pop_discovered_nodes (sys_context, info);
      if (discovered_nodes)
	{
	  if (first_time)
	    {
	      url = lw6sys_list_pop_front (sys_context, &discovered_nodes);
	      if (url)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_discovered_node \"%s\""), url);
		  LW6SYS_FREE (sys_context, url);
		}
	      first_time = 0;
	    }
	  // could be NULL because of pop
	  if (discovered_nodes)
	    {
	      lw6sys_list_free (sys_context, discovered_nodes);
	    }
	}
    }
}

static void
_node_set_verified_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  lw6sys_list_t *list = NULL;
  char *url = NULL;
  lw6nod_info_t *verified_node = NULL;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp (sys_context) < stop_timestamp)
    {
      list = lw6nod_info_new_verified_nodes (sys_context);
      if (list)
	{
	  url = lw6sys_url_http_from_ip_port (sys_context, _TEST_IP_1, _TEST_PORT);
	  if (url)
	    {
	      verified_node =
		lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
				 _TEST_STAMP, _TEST_ID_1, url, _TEST_TITLE,
				 _TEST_DESCRIPTION, NULL, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
	      if (verified_node && list)
		{
		  lw6sys_list_push_front (sys_context, &list, verified_node);
		}
	      LW6SYS_FREE (sys_context, url);
	    }
	  url = lw6sys_url_http_from_ip_port (sys_context, _TEST_IP_2, _TEST_PORT);
	  if (url)
	    {
	      verified_node =
		lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
				 _TEST_STAMP, _TEST_ID_2, url, _TEST_TITLE,
				 _TEST_DESCRIPTION, NULL, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
	      if (verified_node && list)
		{
		  lw6sys_list_push_front (sys_context, &list, verified_node);
		}
	      LW6SYS_FREE (sys_context, url);
	    }
	  url = lw6sys_url_http_from_ip_port (sys_context, _TEST_IP_3, _TEST_PORT);
	  if (url)
	    {
	      verified_node =
		lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
				 _TEST_STAMP, _TEST_ID_3, url, _TEST_TITLE,
				 _TEST_DESCRIPTION, NULL, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
	      if (verified_node && list)
		{
		  lw6sys_list_push_front (sys_context, &list, verified_node);
		}
	      LW6SYS_FREE (sys_context, url);
	    }

	  if (first_time)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("setting list of verified nodes"));
	      first_time = 0;
	    }
	  if (list)
	    {
	      lw6nod_info_set_verified_nodes (sys_context, info, list);
	    }
	  // no need to free hash
	}
    }
}

static void
_node_map_verified_callback_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int *first_time = (int *) func_data;
  lw6nod_info_t *verified_node = (lw6nod_info_t *) data;

  if (verified_node && verified_node->const_info.ref_info.url)
    {
      if (*first_time)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("verified node \"%s\""), verified_node->const_info.ref_info.url);
	  (*first_time) = 0;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistent verified_node data"));
    }
}

static void
_node_map_verified_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6nod_info_t *info = (lw6nod_info_t *) data;
  int64_t stop_timestamp = 0;
  int first_time = 1;

  stop_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_DELAY_MS;

  while (lw6sys_get_timestamp (sys_context) < stop_timestamp)
    {
      lw6nod_info_map_verified_nodes (sys_context, info, _node_map_verified_callback_callback, &first_time);
      // no need to free list
    }
}

/*
 * Testing functions in node.c
 */
static void
_test_node ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

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
    lw6sys_list_t *list = NULL;

    ret = 0;
    info =
      lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
		       _TEST_STAMP, _TEST_ID, _TEST_URL, _TEST_TITLE,
		       _TEST_DESCRIPTION, _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
    if (LW6SYS_TEST_ACK (info))
      {
	lw6nod_info_update (sys_context, info, _TEST_COMMUNITY, _TEST_ROUND, _TEST_LEVEL,
			    _TEST_REQUIRED_BENCH, _TEST_NB_COLORS,
			    _TEST_MAX_NB_COLORS, _TEST_NB_CURSORS,
			    _TEST_MAX_NB_CURSORS, _TEST_NB_NODES,
			    _TEST_MAX_NB_NODES, _TEST_PEER_ID_LIST, _TEST_GAME_SCREENSHOT_SIZE, _TEST_GAME_SCREENSHOT_DATA);
	lw6nod_info_idle (sys_context, info);
	lw6nod_info_update (sys_context, info, _TEST_COMMUNITY, _TEST_ROUND, _TEST_LEVEL,
			    _TEST_REQUIRED_BENCH, _TEST_NB_COLORS,
			    _TEST_MAX_NB_COLORS, _TEST_NB_CURSORS,
			    _TEST_MAX_NB_CURSORS, _TEST_NB_NODES,
			    _TEST_MAX_NB_NODES, _TEST_PEER_ID_LIST, _TEST_GAME_SCREENSHOT_SIZE, _TEST_GAME_SCREENSHOT_DATA);

	if (LW6SYS_TEST_ACK (lw6nod_info_add_discovered_node (sys_context, info, _TEST_URL)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("add \"%s\""), _TEST_URL);
	    list = lw6nod_info_pop_discovered_nodes (sys_context, info);
	    if (LW6SYS_TEST_ACK (list && !lw6sys_list_is_empty (sys_context, list)))
	      {
		url = lw6sys_list_pop_front (sys_context, &list);
		if (LW6SYS_TEST_ACK (url))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop \"%s\""), url);
		    LW6SYS_FREE (sys_context, url);
		  }
	      }
	    if (LW6SYS_TEST_ACK (list))
	      {
		lw6sys_list_free (sys_context, list);
	      }
	  }
	/*
	 * Threads are (on purpose) started in what seems an illogical order,
	 * the idea is to be sure to see how program behaves when querying the
	 * object in various cases, including unprobable ones.
	 */
	thread_map_verified = lw6sys_thread_create (sys_context, _node_map_verified_callback, NULL, info);
	if (LW6SYS_TEST_ACK (thread_map_verified))
	  {
	    thread_set_verified = lw6sys_thread_create (sys_context, _node_set_verified_callback, NULL, info);
	    if (LW6SYS_TEST_ACK (thread_set_verified))
	      {
		thread_pop_discovered = lw6sys_thread_create (sys_context, _node_pop_discovered_callback, NULL, info);
		if (LW6SYS_TEST_ACK (thread_pop_discovered))
		  {
		    thread_add_discovered = lw6sys_thread_create (sys_context, _node_add_discovered_callback, NULL, info);
		    if (LW6SYS_TEST_ACK (thread_add_discovered))
		      {

			thread_update = lw6sys_thread_create (sys_context, _node_update_callback, NULL, info);
			if (LW6SYS_TEST_ACK (thread_update))
			  {

			    thread_dup_dyn = lw6sys_thread_create (sys_context, _node_dup_dyn_callback, NULL, info);
			    if (LW6SYS_TEST_ACK (thread_dup_dyn))
			      {


				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("6 threads started, each one querying the same node info object"));
				ret = 1;
				lw6sys_thread_join (sys_context, thread_dup_dyn);
			      }
			    lw6sys_thread_join (sys_context, thread_update);
			  }
			lw6sys_thread_join (sys_context, thread_add_discovered);
		      }
		    lw6sys_thread_join (sys_context, thread_pop_discovered);
		  }
		lw6sys_thread_join (sys_context, thread_set_verified);
	      }
	    lw6sys_thread_join (sys_context, thread_map_verified);
	  }

	lw6nod_info_free (sys_context, info);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_community_id_without_url_callback (lw6sys_context_t * sys_context, void *func_data, u_int64_t id)
{
  int *nb_ids_without_url = (int *) func_data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("calling id_wihout_url callback with id=%" LW6SYS_PRINTF_LL "x"), (long long) id);
  if (nb_ids_without_url)
    {
      ++(*nb_ids_without_url);
    }
}

/*
 * Testing functions in commmunity.c
 */
static void
_test_community ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6nod_info_t *info = NULL;
    int count = 0;
    char *peer_id_list_str = NULL;
    int nb_ids_without_url = 0;
    int64_t id_from_url = 0LL;
    char *url_from_id = NULL;

    info =
      lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
		       _TEST_STAMP, _TEST_ID_1, _TEST_URL_1, _TEST_TITLE,
		       _TEST_DESCRIPTION, _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
    if (LW6SYS_TEST_ACK (info))
      {
	count = lw6nod_info_community_count (sys_context, info);
	if (LW6SYS_TEST_ACK (count == 1))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("right count=%d"), count);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong count=%d"), count);
	    ret = 0;
	  }

	if (LW6SYS_TEST_ACK (!lw6nod_info_community_add (sys_context, info, _TEST_ID_1, _TEST_URL_1)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("couldn't add ourselves, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could add ourselves, should have returned an error"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6nod_info_community_add (sys_context, info, _TEST_ID_2, _TEST_URL_2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could add peer, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't add peer"));
	    ret = 0;
	  }
	count = lw6nod_info_community_count (sys_context, info);
	if (LW6SYS_TEST_ACK (count == 2))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("right count=%d"), count);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong count=%d"), count);
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (!lw6nod_info_community_add (sys_context, info, _TEST_ID_2, _TEST_URL_2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("couldn't node twice, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could add node twice, should have returned an error"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6nod_info_community_has_id (sys_context, info, _TEST_ID_1)
			     && lw6nod_info_community_has_id (sys_context, info, _TEST_ID_2) && !lw6nod_info_community_has_id (sys_context, info, _TEST_ID_3)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("has_id works"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("has_id problem"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6nod_info_community_has_url (sys_context, info, _TEST_URL_1)
			     && lw6nod_info_community_has_url (sys_context, info, _TEST_URL_2)
			     && !lw6nod_info_community_has_url (sys_context, info, _TEST_URL_3)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("has_url works"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("has_url problem"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK
	    (lw6nod_info_community_is_member (sys_context, info, _TEST_ID_2, _TEST_URL_3)
	     && lw6nod_info_community_is_member (sys_context, info, _TEST_ID_3,
						 _TEST_URL_2)
	     && lw6nod_info_community_is_member (sys_context, info, _TEST_ID_1, _TEST_URL_1)
	     && !lw6nod_info_community_is_member (sys_context, info, _TEST_ID_3, _TEST_URL_3)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("is_member works"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("is_member problem"));
	    ret = 0;
	  }
	id_from_url = lw6nod_info_community_get_id_from_url (sys_context, info, _TEST_URL_3);
	if (LW6SYS_TEST_ACK (!id_from_url))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("no id for url\"%s\", OK"), _TEST_URL_3);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("returned id %" LW6SYS_PRINTF_LL "d for url\"%s\", should not be here"), (long long) id_from_url, _TEST_URL_3);
	    ret = 0;
	  }
	url_from_id = lw6nod_info_community_get_url_from_id (sys_context, info, _TEST_ID_3);
	if (LW6SYS_TEST_ACK (!url_from_id))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("no url for id %" LW6SYS_PRINTF_LL "d, OK"), _TEST_ID_3);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("returned url \"%s\" for id %" LW6SYS_PRINTF_LL "d, should not be here"), url_from_id, (long long) _TEST_ID_3);
	    LW6SYS_FREE (sys_context, url_from_id);
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6nod_info_community_add (sys_context, info, _TEST_ID_3, _TEST_URL_3)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could add peer, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't add peer"));
	    ret = 0;
	  }
	id_from_url = lw6nod_info_community_get_id_from_url (sys_context, info, _TEST_URL_3);
	if (LW6SYS_TEST_ACK (id_from_url))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("returned id %" LW6SYS_PRINTF_LL "d for url\"%s\", OK"), (long long) id_from_url, _TEST_URL_3);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no id for url\"%s\", should be here"), _TEST_URL_3);
	    ret = 0;
	  }
	url_from_id = lw6nod_info_community_get_url_from_id (sys_context, info, _TEST_ID_3);
	if (LW6SYS_TEST_ACK (url_from_id))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("returned url \"%s\" for id %" LW6SYS_PRINTF_LL "d, OK"), url_from_id, (long long) _TEST_ID_3);
	    LW6SYS_FREE (sys_context, url_from_id);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no url for id %" LW6SYS_PRINTF_LL "d, should be here"), _TEST_ID_3);
	    ret = 0;
	  }
	count = lw6nod_info_community_count (sys_context, info);
	if (LW6SYS_TEST_ACK (count == 3))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("right count=%d"), count);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong count=%d"), count);
	    ret = 0;
	  }
	peer_id_list_str = lw6nod_info_community_get_peer_id_list_str (sys_context, info);
	if (LW6SYS_TEST_ACK (peer_id_list_str))
	  {
	    if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, _TEST_PEER_ID_LIST_STR, peer_id_list_str)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("peer_id_list_str = \"%s\""), peer_id_list_str);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("peer_id_list_str = \"%s\", should have been \"%s\""), peer_id_list_str, _TEST_PEER_ID_LIST_STR);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, peer_id_list_str);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create peer_id_list_str"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6nod_info_community_remove_by_id (sys_context, info, _TEST_ID_2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could remove peer, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't remove peer"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (!lw6nod_info_community_remove_by_id (sys_context, info, _TEST_ID_2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("couldn't remove peer, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could remove peer, should no exist any more"));
	    ret = 0;
	  }
	count = lw6nod_info_community_count (sys_context, info);
	if (LW6SYS_TEST_ACK (count == 2))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("right count=%d"), count);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong count=%d"), count);
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6nod_info_community_remove_by_url (sys_context, info, _TEST_URL_3)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could remove peer, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't remove peer"));
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (!lw6nod_info_community_remove_by_url (sys_context, info, _TEST_URL_3)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("couldn't remove peer, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could remove peer, should no exist any more"));
	    ret = 0;
	  }
	count = lw6nod_info_community_count (sys_context, info);
	if (LW6SYS_TEST_ACK (count == 1))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("right count=%d"), count);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong count=%d"), count);
	    ret = 0;
	  }
	lw6nod_info_community_reset (sys_context, info);
	if (LW6SYS_TEST_ACK (!lw6nod_info_community_remove_by_url (sys_context, info, _TEST_URL_1)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("couldn't ourselves, that's fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could remove ourselves, this is shocking"));
	    ret = 0;
	  }
	/*
	 * Setting this should set nodes that have been removed before
	 */
	lw6nod_info_community_set_peer_id_list_str (sys_context, info, _TEST_PEER_ID_LIST_STR);
	lw6nod_info_community_id_without_url_map (sys_context, info, _community_id_without_url_callback, (void *) &nb_ids_without_url);
	if (LW6SYS_TEST_ACK (nb_ids_without_url == _TEST_COMMUNITY_NB_IDS_WITHOUT_URL))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got the right number of ids without url %d"), nb_ids_without_url);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("bad number of ids without url got %d expected %d"), nb_ids_without_url, _TEST_COMMUNITY_NB_IDS_WITHOUT_URL);
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6nod_info_community_has_id_without_url (sys_context, info, _TEST_ID_2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node %" LW6SYS_PRINTF_LL "d reported as present without an URL, OK"), (long long) _TEST_ID_2);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("node %" LW6SYS_PRINTF_LL "d not reported as present without an URL, this is a problem"), (long long) _TEST_ID_2);
	    ret = 0;
	  }
	lw6nod_info_free (sys_context, info);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libnod CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libnod CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6nod_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libnod module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6nod_test_register (lw6sys_context_t * sys_context, int mode)
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
    }

  suite = CU_add_suite ("lw6nod", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_node);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_community);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6nod_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @nod module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6nod_test_run (lw6sys_context_t * sys_context, int mode)
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
