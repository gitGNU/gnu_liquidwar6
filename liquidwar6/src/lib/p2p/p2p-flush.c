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

int
_lw6p2p_flush_verified_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now = 0;
  int delay = node->db->data.consts.flush_verified_nodes_delay;

  now = lw6sys_get_timestamp (sys_context);
  if (node->flush.next_verified_nodes_timestamp < now)
    {
      node->flush.next_verified_nodes_timestamp = now + delay / 2 + lw6sys_random (sys_context, delay);
      ret = _lw6p2p_flush_verified_nodes (sys_context, node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

int
_select_other_node_callback (lw6sys_context_t * sys_context, void *func_data, int nb_fields, char **fields_values, char **fields_names)
{
  int ret = 0;
  lw6sys_list_t **list_of_node = (lw6sys_list_t **) func_data;
  lw6nod_info_t *verified_node = NULL;
  char *program = NULL;
  char *version = NULL;
  char *codename = NULL;
  int stamp = 0;
  u_int64_t id = 0;
  char *url = NULL;
  char *title = NULL;
  char *description = NULL;
  int bench = 0;
  int open_relay = 0;
  int uptime = 0;

  if (nb_fields == _LW6P2P_DB_NODE_NB_FIELDS)
    {
      if (fields_values[_LW6P2P_DB_NODE_ORDER_ID])
	{
	  id = lw6sys_id_atol (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_ID]);
	}
      program = lw6sys_build_get_package_tarname ();
      version = fields_values[_LW6P2P_DB_NODE_ORDER_VERSION];
      codename = fields_values[_LW6P2P_DB_NODE_ORDER_CODENAME];
      if (fields_values[_LW6P2P_DB_NODE_ORDER_STAMP])
	{
	  stamp = lw6sys_atoi (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_STAMP]);
	}
      url = fields_values[_LW6P2P_DB_NODE_ORDER_URL];
      title = fields_values[_LW6P2P_DB_NODE_ORDER_TITLE];
      description = fields_values[_LW6P2P_DB_NODE_ORDER_DESCRIPTION];
      if (fields_values[_LW6P2P_DB_NODE_ORDER_BENCH])
	{
	  bench = lw6sys_atoi (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_BENCH]);
	}
      if (fields_values[_LW6P2P_DB_NODE_ORDER_OPEN_RELAY])
	{
	  open_relay = lw6sys_atoi (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_OPEN_RELAY]);
	}
      /*
       * uptime will be wrong (0), it could be possible to
       * have it but would require a call to _lw6p2p_db_now
       * which in turn requires a pointer on node object...
       */
      if (id && url && title && description)
	{
	  verified_node =
	    lw6nod_info_new (sys_context, program, version, codename, stamp, id, url, title, description, NULL, bench, open_relay, uptime, 0, NULL);
	  if (verified_node && list_of_node)
	    {
	      lw6sys_list_push_front (sys_context, list_of_node, verified_node);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("database contains uncomplete entry for some node"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("request for other nodes should return %d fields but returned %d"), _LW6P2P_DB_NODE_NB_FIELDS, nb_fields);
    }

  return ret;
}

int
_lw6p2p_flush_verified_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  char *query = NULL;
  lw6sys_list_t *list_of_node;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("flush verified nodes"));

  list_of_node = lw6nod_info_new_verified_nodes (sys_context);
  if (list_of_node)
    {
      query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query (sys_context, node->db, _LW6P2P_SELECT_OTHER_NODE_SQL), node->public_url);
      if (query)
	{
	  if (_lw6p2p_db_lock (sys_context, node->db))
	    {
	      _lw6p2p_db_exec (sys_context, node->db, query, _select_other_node_callback, &list_of_node);
	      _lw6p2p_db_unlock (sys_context, node->db);
	    }
	  LW6SYS_FREE (sys_context, query);
	}
      if (list_of_node)
	{
	  ret = lw6nod_info_set_verified_nodes (sys_context, node->node_info, list_of_node);
	}
    }

  return ret;
}

int
_lw6p2p_flush_discovered_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now = 0;
  int delay = node->db->data.consts.flush_discovered_nodes_delay;

  now = lw6sys_get_timestamp (sys_context);
  if (node->flush.next_discovered_nodes_timestamp < now)
    {
      node->flush.next_discovered_nodes_timestamp = now + delay / 2 + lw6sys_random (sys_context, delay);
      ret = _lw6p2p_flush_discovered_nodes (sys_context, node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

int
_lw6p2p_flush_discovered_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  char *url = NULL;
  lw6sys_list_t *list = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("flush discovered nodes"));
  ret = 1;

  list = lw6nod_info_pop_discovered_nodes (sys_context, node->node_info);
  if (list)
    {
      while ((url = lw6sys_list_pop_front (sys_context, &list)) != NULL)
	{
	  ret = _lw6p2p_node_insert_discovered (sys_context, node, url);
	  LW6SYS_FREE (sys_context, url);
	}
    }
  list = lw6sys_str_split_config_item (sys_context, node->known_nodes);
  if (list)
    {
      while ((url = lw6sys_list_pop_front (sys_context, &list)) != NULL)
	{
	  ret = _lw6p2p_node_insert_discovered (sys_context, node, url);
	  LW6SYS_FREE (sys_context, url);
	}
    }

  return ret;
}
