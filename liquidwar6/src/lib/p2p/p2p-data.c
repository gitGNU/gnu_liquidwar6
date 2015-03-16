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
#endif // HAVE_CONFIG_H

#include "p2p.h"
#include "p2p-internal.h"

#define _P2P_SUBDIR "p2p"
#define _CONSTS_FILE "p2p-const.xml"
#define _SQL_DIR "sql"
#define _QUERIES_HASH_SIZE 97
#define _SCREENSHOT_DIR "screenshot"
#define _SCREENSHOT_FILE "idle.jpg"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _lw6p2p_consts_t *consts;

  consts = (_lw6p2p_consts_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "accept-delay", &consts->accept_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "flush-verified-nodes-delay", &consts->flush_verified_nodes_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "flush-discovered-nodes-delay", &consts->flush_discovered_nodes_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "explore-discover-nodes-delay", &consts->explore_discover_nodes_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "explore-verify-nodes-delay", &consts->explore_verify_nodes_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "connect-registered-nodes-delay", &consts->connect_registered_nodes_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "connect-registered-max-ping", &consts->connect_registered_max_ping);
      lw6cfg_read_xml_int (sys_context, key, value, "node-expire-soft-delay", &consts->node_expire_soft_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "node-expire-hard-delay", &consts->node_expire_hard_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "node-verify-max-at-once", &consts->node_verify_max_at_once);
      lw6cfg_read_xml_int (sys_context, key, value, "foo-delay", &consts->foo_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "join-delay", &consts->join_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "miss-get-delay", &consts->miss_get_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "miss-duplicate-delay", &consts->miss_duplicate_delay);
      lw6cfg_read_xml_int (sys_context, key, value, "miss-wake-up-interval", &consts->miss_wake_up_interval);
      lw6cfg_read_xml_int (sys_context, key, value, "miss-max-range", &consts->miss_max_range);
      lw6cfg_read_xml_int (sys_context, key, value, "ticket-table-hash-size", &consts->ticket_table_hash_size);
      lw6cfg_read_xml_int (sys_context, key, value, "send-per-poll-limit-n", &consts->send_per_poll_limit_n);
      lw6cfg_read_xml_int (sys_context, key, value, "received-atom-parts-per-poll-to-disable-miss", &consts->received_atom_parts_per_poll_to_disable_miss);
      lw6cfg_read_xml_int (sys_context, key, value, "meta-nb-duplicates", &consts->meta_nb_duplicates);
    }
}

static int
_load_consts (lw6sys_context_t * sys_context, _lw6p2p_consts_t * consts, const char *consts_file)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), consts_file);
  ret = lw6cfg_read_key_value_xml_file (sys_context, consts_file, _read_callback, (void *) consts);

  return ret;
}

static int
_read_query (lw6sys_context_t * sys_context, lw6sys_hash_t * queries, const char *sql_dir, const char *query_file)
{
  int ret = 0;
  char *filename = NULL;
  char *query = NULL;

  filename = lw6sys_path_concat (sys_context, sql_dir, query_file);
  if (filename)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), filename);
      query = lw6sys_read_file_content (sys_context, filename);
      if (query)
	{
	  lw6sys_hash_set (sys_context, queries, query_file, query);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("can't read query \"%s\" in \"%s\""), query_file, filename);
	}
      LW6SYS_FREE (sys_context, filename);
    }

  return ret;
}

static void
_check_query_not_null (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  int *ret = (int *) func_data;
  char *query = (char *) value;

  if (query == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("query \"%s\" is NULL"), key);
      (*ret) = 0;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("query \"%s\" is \"%s\""), key, query);
    }
}

static int
_load_sql (lw6sys_context_t * sys_context, _lw6p2p_sql_t * sql, const char *sql_dir)
{
  int ret = 0;

  sql->queries = lw6sys_hash_new (sys_context, lw6sys_free_callback, _QUERIES_HASH_SIZE);
  if (sql->queries)
    {
      ret = 1;

      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_CLEAN_DATABASE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_CREATE_DATABASE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_DELETE_CONNECTION_BY_PTR_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_DELETE_NODE_BY_ID_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_DELETE_NODE_BY_URL_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_DELETE_OLD_NODE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_INSERT_CONNECTION_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_INSERT_LOCAL_NODE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_INSERT_DISCOVERED_NODE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_SELECT_CONNECTION_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_SELECT_NODE_BY_ID_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_SELECT_NODE_BY_URL_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_SELECT_UNVERIFIED_NODE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_SELECT_OTHER_NODE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_SELECT_AVAILABLE_NODE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_SELECT_UNAVAILABLE_NODE_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_UPDATE_NODE_INFO_SQL);
      _read_query (sys_context, sql->queries, sql_dir, _LW6P2P_UPDATE_NODE_NET_SQL);

      lw6sys_hash_map (sys_context, sql->queries, _check_query_not_null, &ret);
    }

  return ret;
}

static int
_load_screenshot (lw6sys_context_t * sys_context, _lw6p2p_screenshot_t * screenshot, const char *screenshot_file)
{
  int ret = 0;

  if (lw6sys_file_exists (sys_context, screenshot_file))
    {
      screenshot->data = lw6sys_read_file_content_bin (sys_context, &(screenshot->size), screenshot_file);
      if (screenshot->data && screenshot->size > 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("read screenshot \"%s\", %d bytes"), screenshot_file, screenshot->size);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("unable to read screenshot file \"%s\""), screenshot_file);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("unable to find screenshot file \"%s\""), screenshot_file);
    }

  return ret;
}

int
_lw6p2p_data_load (lw6sys_context_t * sys_context, _lw6p2p_data_t * data, const char *data_dir)
{
  int ret = 0;
  char *p2p_subdir = NULL;
  char *consts_file = NULL;
  char *sql_dir = NULL;
  char *screenshot_dir = NULL;
  char *screenshot_file = NULL;

  p2p_subdir = lw6sys_path_concat (sys_context, data_dir, _P2P_SUBDIR);
  if (p2p_subdir)
    {
      consts_file = lw6sys_path_concat (sys_context, p2p_subdir, _CONSTS_FILE);
      sql_dir = lw6sys_path_concat (sys_context, p2p_subdir, _SQL_DIR);
      screenshot_dir = lw6sys_path_concat (sys_context, p2p_subdir, _SCREENSHOT_DIR);
      if (screenshot_dir)
	{
	  screenshot_file = lw6sys_path_concat (sys_context, screenshot_dir, _SCREENSHOT_FILE);
	}
      if (consts_file && sql_dir && screenshot_file)
	{
	  ret = _load_consts (sys_context, &(data->consts), consts_file)
	    && _load_sql (sys_context, &(data->sql), sql_dir) && _load_screenshot (sys_context, &(data->idle_screenshot), screenshot_file);
	}
      if (consts_file)
	{
	  LW6SYS_FREE (sys_context, consts_file);
	}
      if (sql_dir)
	{
	  LW6SYS_FREE (sys_context, sql_dir);
	}
      if (screenshot_dir)
	{
	  LW6SYS_FREE (sys_context, screenshot_dir);
	}
      if (screenshot_file)
	{
	  LW6SYS_FREE (sys_context, screenshot_file);
	}
      LW6SYS_FREE (sys_context, p2p_subdir);
    }

  return ret;
}

static int
_unload_consts (lw6sys_context_t * sys_context, _lw6p2p_consts_t * consts)
{
  int ret = 1;

  memset (consts, 0, sizeof (_lw6p2p_consts_t));

  return ret;
}

static int
_unload_sql (lw6sys_context_t * sys_context, _lw6p2p_sql_t * sql)
{
  int ret = 1;

  if (sql->queries)
    {
      lw6sys_hash_free (sys_context, sql->queries);
      sql->queries = NULL;
    }

  return ret;
}

static int
_unload_screenshot (lw6sys_context_t * sys_context, _lw6p2p_screenshot_t * screenshot)
{
  int ret = 1;

  if (screenshot->data)
    {
      LW6SYS_FREE (sys_context, screenshot->data);
      screenshot->data = NULL;
    }

  return ret;
}

int
_lw6p2p_data_unload (lw6sys_context_t * sys_context, _lw6p2p_data_t * data)
{
  int ret = 0;

  ret = _unload_consts (sys_context, &(data->consts)) && _unload_sql (sys_context, &(data->sql)) && _unload_screenshot (sys_context, &(data->idle_screenshot));

  return ret;
}
