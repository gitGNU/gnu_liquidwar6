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

#include "p2p.h"
#include "p2p-internal.h"

#define _P2P_SUBDIR "p2p"
#define _CONSTS_FILE "p2p-const.xml"
#define _SQL_DIR "sql"
#define _QUERIES_HASH_SIZE 97
#define _SCREENSHOT_DIR "screenshot"
#define _SCREENSHOT_FILE "idle.jpg"

static void
_read_callback (void *callback_data, char *element, char *key, char *value)
{
  _lw6p2p_consts_t *consts;

  consts = (_lw6p2p_consts_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (key, value, "accept-delay", &consts->accept_delay);
      lw6cfg_read_xml_int (key, value, "flush-verified-nodes-delay",
			   &consts->flush_verified_nodes_delay);
      lw6cfg_read_xml_int (key, value, "flush-discovered-nodes-delay",
			   &consts->flush_discovered_nodes_delay);
    }
}

static int
_load_consts (_lw6p2p_consts_t * consts, char *consts_file)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_INFO, _("reading \"%s\""), consts_file);
  ret =
    lw6cfg_read_key_value_xml_file (consts_file, _read_callback,
				    (void *) consts);

  return ret;
}

static int
_read_query (lw6sys_hash_t * queries, char *sql_dir, char *query_file)
{
  int ret = 0;
  char *filename = NULL;
  char *query = NULL;

  filename = lw6sys_path_concat (sql_dir, query_file);
  if (filename)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("reading \"%s\""), filename);
      query = lw6sys_read_file_content (filename);
      if (query)
	{
	  lw6sys_hash_set (queries, query_file, query);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_ERROR,
		      _("can't read query \"%s\" in \"%s\""), query_file,
		      filename);
	}
      LW6SYS_FREE (filename);
    }

  return ret;
}

static void
_check_query_not_null (void *func_data, char *key, void *value)
{
  int *ret = (int *) func_data;
  char *query = (char *) value;

  if (query == NULL)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("query \"%s\" is NULL"), key);
      (*ret) = 0;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("query \"%s\" is \"%s\""), key, query);
    }
}

static int
_load_sql (_lw6p2p_sql_t * sql, char *sql_dir)
{
  int ret = 0;

  sql->queries = lw6sys_hash_new (lw6sys_free_callback, _QUERIES_HASH_SIZE);
  if (sql->queries)
    {
      ret = 1;

      _read_query (sql->queries, sql_dir, _LW6P2P_CLEAN_DATABASE_SQL);
      _read_query (sql->queries, sql_dir, _LW6P2P_CREATE_DATABASE_SQL);
      _read_query (sql->queries, sql_dir,
		   _LW6P2P_DELETE_CONNECTION_BY_PTR_SQL);
      _read_query (sql->queries, sql_dir, _LW6P2P_DELETE_NODE_BY_ID_SQL);
      _read_query (sql->queries, sql_dir, _LW6P2P_INSERT_CONNECTION_SQL);
      _read_query (sql->queries, sql_dir, _LW6P2P_INSERT_NODE_SQL);
      _read_query (sql->queries, sql_dir, _LW6P2P_SELECT_CONNECTION_SQL);

      lw6sys_hash_map (sql->queries, _check_query_not_null, &ret);
    }

  return ret;
}

static int
_load_screenshot (_lw6p2p_screenshot_t * screenshot, char *screenshot_file)
{
  int ret = 0;

  if (lw6sys_file_exists (screenshot_file))
    {
      screenshot->data =
	lw6sys_read_file_content_bin (&(screenshot->size), screenshot_file);
      if (screenshot->data && screenshot->size > 0)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("read screenshot \"%s\", %d bytes"),
		      screenshot_file, screenshot->size);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_ERROR,
		      _("unable to read screenshot file \"%s\""),
		      screenshot_file);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_ERROR,
		  _("unable to find screenshot file \"%s\""),
		  screenshot_file);
    }

  return ret;
}

int
_lw6p2p_data_load (_lw6p2p_data_t * data, char *data_dir)
{
  int ret = 0;
  char *p2p_subdir = NULL;
  char *consts_file = NULL;
  char *sql_dir = NULL;
  char *screenshot_dir = NULL;
  char *screenshot_file = NULL;

  p2p_subdir = lw6sys_path_concat (data_dir, _P2P_SUBDIR);
  if (p2p_subdir)
    {
      consts_file = lw6sys_path_concat (p2p_subdir, _CONSTS_FILE);
      sql_dir = lw6sys_path_concat (p2p_subdir, _SQL_DIR);
      screenshot_dir = lw6sys_path_concat (p2p_subdir, _SCREENSHOT_DIR);
      if (screenshot_dir)
	{
	  screenshot_file =
	    lw6sys_path_concat (screenshot_dir, _SCREENSHOT_FILE);
	}
      if (consts_file && sql_dir && screenshot_file)
	{
	  ret = _load_consts (&(data->consts), consts_file)
	    && _load_sql (&(data->sql), sql_dir)
	    && _load_screenshot (&(data->idle_screenshot), screenshot_file);
	}
      if (consts_file)
	{
	  LW6SYS_FREE (consts_file);
	}
      if (sql_dir)
	{
	  LW6SYS_FREE (sql_dir);
	}
      if (screenshot_dir)
	{
	  LW6SYS_FREE (screenshot_dir);
	}
      if (screenshot_file)
	{
	  LW6SYS_FREE (screenshot_file);
	}
      LW6SYS_FREE (p2p_subdir);
    }

  return ret;
}

static int
_unload_consts (_lw6p2p_consts_t * consts)
{
  int ret = 1;

  memset (consts, 0, sizeof (_lw6p2p_consts_t));

  return ret;
}

static int
_unload_sql (_lw6p2p_sql_t * sql)
{
  int ret = 1;

  if (sql->queries)
    {
      lw6sys_hash_free (sql->queries);
      sql->queries = NULL;
    }

  return ret;
}

static int
_unload_screenshot (_lw6p2p_screenshot_t * screenshot)
{
  int ret = 1;

  if (screenshot->data)
    {
      LW6SYS_FREE (screenshot->data);
      screenshot->data = NULL;
    }

  return ret;
}

int
_lw6p2p_data_unload (_lw6p2p_data_t * data)
{
  int ret = 0;

  ret = _unload_consts (&(data->consts)) && _unload_sql (&(data->sql))
    && _unload_screenshot (&(data->idle_screenshot));

  return ret;
}
