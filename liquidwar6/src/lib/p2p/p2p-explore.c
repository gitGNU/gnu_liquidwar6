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
_lw6p2p_explore_discover_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now = 0;
  int delay = node->db->data.consts.explore_discover_nodes_delay;

  now = lw6sys_get_timestamp (sys_context);
  if (node->explore.next_discover_nodes_timestamp < now)
    {
      node->explore.next_discover_nodes_timestamp = now + delay / 2 + lw6sys_random (sys_context, delay);
      ret = _lw6p2p_explore_discover_nodes (sys_context, node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

static void
_known_nodes_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  char *public_url = (char *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("_known_nodes_callback with public_url=\"%s\""), public_url);
  if (node && public_url)
    {
      _lw6p2p_node_insert_discovered (sys_context, node, public_url);
    }
}

int
_lw6p2p_explore_discover_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  lw6sys_list_t *list = NULL;
  _lw6p2p_cli_oob_callback_data_t *cli_oob = NULL;
  int i;
  char *broadcast_url = NULL;

  if (node->known_nodes)
    {
      list = lw6sys_str_split_config_item (sys_context, node->known_nodes);
      if (list)
	{
	  lw6sys_list_map (sys_context, list, _known_nodes_callback, node);
	  lw6sys_list_free (sys_context, list);
	}
    }

  /*
   * Important to check for the 0 backend case, since it can
   * happen in real-life cases
   */
  if (node->backends.nb_cli_backends > 0)
    {
      i = node->explore.last_cli_oob_broadcast_backend;
      i = (i + 1) % node->backends.nb_cli_backends;
      node->explore.last_cli_oob_broadcast_backend = i;

      if (node->broadcast)
	{
	  broadcast_url = lw6sys_url_http_from_ip_port (sys_context, LW6NET_ADDRESS_BROADCAST, node->bind_port);
	  if (broadcast_url)
	    {
	      cli_oob = _lw6p2p_cli_oob_callback_data_new (sys_context, node->backends.cli_backends[i], node, broadcast_url);
	      if (cli_oob)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process cli_oob (broadcast) url=\"%s\""), broadcast_url);
		  cli_oob->cli_oob->thread = lw6sys_thread_create (sys_context, _lw6p2p_cli_oob_callback, NULL, cli_oob);
		  lw6sys_lifo_push (sys_context, &(node->cli_oobs), cli_oob);
		}
	      LW6SYS_FREE (sys_context, broadcast_url);
	    }
	  if (node->bind_port != LW6NET_DEFAULT_PORT)
	    {
	      broadcast_url = lw6sys_url_http_from_ip_port (sys_context, LW6NET_ADDRESS_BROADCAST, LW6NET_DEFAULT_PORT);
	      if (broadcast_url)
		{
		  cli_oob = _lw6p2p_cli_oob_callback_data_new (sys_context, node->backends.cli_backends[i], node, broadcast_url);
		  if (cli_oob)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process cli_oob (broadcast) url=\"%s\""), broadcast_url);
		      cli_oob->cli_oob->thread = lw6sys_thread_create (sys_context, _lw6p2p_cli_oob_callback, NULL, cli_oob);
		      lw6sys_lifo_push (sys_context, &(node->cli_oobs), cli_oob);
		    }
		  LW6SYS_FREE (sys_context, broadcast_url);
		}
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("BROADCAST disabled by user configuration"));
	}
    }

  return ret;
}

void
_lw6p2p_explore_start_verify_node (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *public_url)
{
  _lw6p2p_cli_oob_callback_data_t *cli_oob = NULL;
  int i;

  /*
   * Important to check for the 0 backend case, since it can
   * happen in real-life cases
   */
  if (node->backends.nb_cli_backends > 0)
    {
      /*
       * We choose a random backend, it's better than a round-robin
       * since with a round-robin we could end up always using the
       * same backend for the same node. This is not true for
       * broadcast, in which a round-robin is better.
       */
      i = lw6sys_random (sys_context, node->backends.nb_cli_backends);

      cli_oob = _lw6p2p_cli_oob_callback_data_new (sys_context, node->backends.cli_backends[i], node, public_url);
      if (cli_oob)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process cli_oob url=\"%s\""), public_url);
	  cli_oob->cli_oob->thread = lw6sys_thread_create (sys_context, _lw6p2p_cli_oob_callback, NULL, cli_oob);
	  lw6sys_lifo_push (sys_context, &(node->cli_oobs), cli_oob);
	}
    }
}

int
_lw6p2p_explore_verify_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now = 0;
  int delay = node->db->data.consts.explore_verify_nodes_delay;

  now = lw6sys_get_timestamp (sys_context);
  if (node->explore.next_verify_nodes_timestamp < now)
    {
      node->explore.next_verify_nodes_timestamp = now + delay / 2 + lw6sys_random (sys_context, delay);
      ret = _lw6p2p_explore_verify_nodes (sys_context, node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

static int
_select_unverified_node_callback (lw6sys_context_t * sys_context, void *func_data, int nb_fields, char **fields_values, char **fields_names)
{
  int ret = 0;

  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;

  if (nb_fields == 1)
    {
      if (fields_values[0])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("node with NULL id found url=\"%s\""), fields_values[0]);
	  _lw6p2p_explore_start_verify_node (sys_context, node, fields_values[0]);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("node_url is NULL"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request returned %d fields, one and only one should be present"), nb_fields);
    }

  return ret;
}

int
_lw6p2p_explore_verify_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  char *query = NULL;
  int now = 0;
  int time_limit_soft = 0;
  int time_limit_hard = 0;
  int max_at_once = 0;

  now = _lw6p2p_db_now (sys_context, node->db);
  time_limit_soft = now - node->db->data.consts.node_expire_soft_delay / 1000;
  time_limit_hard = now - node->db->data.consts.node_expire_hard_delay / 1000;
  /*
   * Now to calculate how many nodes we'll check we randomize a little bit
   * on the limit number, this is to avoid all nodes being checked at once,
   * it artificially makes some servers be checked more often than others,
   * and should induce, after some time, enough randomness in timeouts.
   */
  max_at_once = 1 + (2 * lw6sys_random (sys_context, node->db->data.consts.node_verify_max_at_once));

  if (_lw6p2p_db_lock (sys_context, node->db))
    {
      query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query (sys_context, node->db, _LW6P2P_DELETE_OLD_NODE_SQL), time_limit_hard, node->public_url);
      if (query)
	{
	  ret = _lw6p2p_db_exec_ignore_data (sys_context, node->db, query);
	  LW6SYS_FREE (sys_context, query);
	}
      query =
	lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query (sys_context, node->db, _LW6P2P_SELECT_UNVERIFIED_NODE_SQL), time_limit_soft, node->node_id_str,
			    max_at_once);
      if (query)
	{
	  ret = _lw6p2p_db_exec (sys_context, node->db, query, _select_unverified_node_callback, node);
	  LW6SYS_FREE (sys_context, query);
	}
      _lw6p2p_db_unlock (sys_context, node->db);
    }

  return ret;
}
