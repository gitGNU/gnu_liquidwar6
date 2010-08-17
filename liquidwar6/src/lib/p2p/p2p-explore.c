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

int
_lw6p2p_explore_discover_nodes_if_needed (_lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now = 0;
  int delay = node->db->data.consts.explore_discover_nodes_delay;

  now = lw6sys_get_timestamp ();
  if (node->explore.next_discover_nodes_timestamp < now)
    {
      node->explore.next_discover_nodes_timestamp =
	now + delay / 2 + lw6sys_random (delay);
      ret = _lw6p2p_explore_discover_nodes (node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

static void
_known_nodes_callback (void *func_data, void *data)
{
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  char *public_url = (char *) data;

  lw6sys_log (LW6SYS_LOG_DEBUG,
	      _("_known_nodes_callback with public_url=\"%s\""), public_url);
  if (node && public_url)
    {
      _lw6p2p_node_insert_discovered (node, public_url);
    }
}

int
_lw6p2p_explore_discover_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;
  lw6sys_list_t *list = NULL;

  if (node->known_nodes)
    {
      list = lw6sys_str_split_config_item (node->known_nodes);
      if (list)
	{
	  lw6sys_list_map (list, _known_nodes_callback, node);
	  lw6sys_list_free (list);
	}
    }

  // todo: let all backends try & discover things
  ret = 1;

  return ret;
}

int
_lw6p2p_explore_verify_nodes_if_needed (_lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now = 0;
  int delay = node->db->data.consts.explore_verify_nodes_delay;

  now = lw6sys_get_timestamp ();
  if (node->explore.next_verify_nodes_timestamp < now)
    {
      node->explore.next_verify_nodes_timestamp =
	now + delay / 2 + lw6sys_random (delay);
      ret = _lw6p2p_explore_verify_nodes (node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

static void
_start_verify_node (_lw6p2p_node_t * node, char *public_url)
{
  _lw6p2p_cli_oob_callback_data_t *cli_oob = NULL;
  int i;

  for (i = 0; i < node->nb_cli_backends; ++i)
    {
      cli_oob =
	_lw6p2p_cli_oob_callback_data_new (node->cli_backends[i],
					   node, public_url);
      if (cli_oob)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("process cli_oob url=\"%s\""),
		      public_url);
	  cli_oob->cli_oob->thread =
	    lw6sys_thread_create (_lw6p2p_cli_oob_callback, NULL, cli_oob);
	  lw6sys_lifo_push (&(node->cli_oobs), cli_oob);
	}
    }
}

int
_select_unverified_node_callback (void *func_data, int nb_fields,
				  char **fields_values, char **fields_names)
{
  int ret = 0;
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;

  if (nb_fields == 1)
    {
      if (fields_values[0])
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("node with NULL id found url=\"%s\""),
		      fields_values[0]);
	  _start_verify_node (node, fields_values[0]);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("node_url is NULL"));
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("request returned %d fields, one and only one should be present"),
		  nb_fields);
    }

  return ret;
}

int
_lw6p2p_explore_verify_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;
  char *query = NULL;
  int time_limit = 0;
  int max_at_once = 0;

  time_limit =
    lw6p2p_db_now () - node->db->data.consts.node_info_expire_delay / 1000;
  max_at_once = node->db->data.consts.node_verify_max_at_once;

  query = lw6sys_new_sprintf (_lw6p2p_db_get_query
			      (node->db, _LW6P2P_SELECT_UNVERIFIED_NODE_SQL),
			      time_limit, node->node_id_str, max_at_once);
  if (query)
    {
      if (_lw6p2p_db_lock (node->db))
	{
	  ret =
	    _lw6p2p_db_exec (node->db, query,
			     _select_unverified_node_callback, node);
	  _lw6p2p_db_unlock (node->db);
	}
      LW6SYS_FREE (query);
    }

  return ret;
}
