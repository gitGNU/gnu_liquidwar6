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

typedef struct _select_node_by_id_data_s
{
  char *node_id;
  char *node_url;
  char *node_ip;
  int ping_delay_msec;
} _select_node_by_id_data_t;

static int
_select_node_by_id_callback (lw6sys_context_t * sys_context, void *func_data, int nb_fields, char **fields_values, char **fields_names)
{
  int ret = 0;
  _select_node_by_id_data_t *node_by_id_data = (_select_node_by_id_data_t *) func_data;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("select_node_by_id_callback called with %d fields"), nb_fields);
  /*
   * Putting a high value by default (means "no valid connection"
   * when a zero value would be "optimal connection").
   */
  node_by_id_data->ping_delay_msec = LW6CNX_WORST_PING_MSEC;
  if (nb_fields == _LW6P2P_DB_NODE_NB_FIELDS)
    {
      if (fields_values[_LW6P2P_DB_NODE_ORDER_URL])
	{
	  /*
	   * Calling canonize is not formally necessary here,
	   * a copy should do the job. But since we need to
	   * duplicate the pointer, we might just canonize
	   * it on the fly, it won't harm. The real cost
	   * is memory allocation.
	   */
	  node_by_id_data->node_url = lw6sys_url_canonize (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_URL]);
	}
      if (fields_values[_LW6P2P_DB_NODE_ORDER_IP])
	{
	  node_by_id_data->node_ip = lw6sys_str_copy (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_IP]);
	}
      if (fields_values[_LW6P2P_DB_NODE_ORDER_PING_DELAY_MSEC])
	{
	  node_by_id_data->ping_delay_msec = lw6sys_atoi (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_PING_DELAY_MSEC]);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("request for node by id should return %d fields but returned %d"), _LW6P2P_DB_NODE_NB_FIELDS, nb_fields);
    }

  return ret;
}

int
_lw6p2p_connect_registered_nodes_if_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now = 0;
  int delay = node->db->data.consts.connect_registered_nodes_delay;

  now = lw6sys_get_timestamp (sys_context);
  if (node->connect_registered.next_connect_registered_nodes_timestamp < now)
    {
      node->connect_registered.next_connect_registered_nodes_timestamp = now + delay / 2 + lw6sys_random (sys_context, delay);
      ret = _lw6p2p_connect_registered_nodes (sys_context, node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

int
_lw6p2p_connect_registered_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 1;
  int index = 0;
  lw6msg_meta_array_t meta_array;
  char *query = NULL;
  _select_node_by_id_data_t node_by_id_data;

  lw6msg_meta_array_zero (sys_context, &meta_array);
  memset (&node_by_id_data, 0, sizeof (_select_node_by_id_data_t));
  lw6dat_warehouse_meta_get (sys_context, node->warehouse, &meta_array, lw6dat_warehouse_get_seq_max (sys_context, node->warehouse));

  for (index = 0; index < LW6MSG_NB_META_ARRAY_ITEMS; ++index)
    {
      if (meta_array.items[index].node_id)
	{
	  if (_lw6p2p_node_is_peer_registered
	      (sys_context, node, meta_array.items[index].node_id) && !_lw6p2p_node_is_peer_connected (sys_context, node, meta_array.items[index].node_id))
	    {
	      /*
	       * OK, we found a node which is registered in the
	       * warehouse but is not in our tentacle list.
	       */
	      node_by_id_data.node_id = lw6sys_id_ltoa (sys_context, meta_array.items[index].node_id);
	      if (node_by_id_data.node_id)
		{
		  query =
		    lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query (sys_context, node->db, _LW6P2P_SELECT_NODE_BY_ID_SQL), node_by_id_data.node_id);
		  if (query)
		    {
		      if (_lw6p2p_db_lock (sys_context, node->db))
			{
			  ret = _lw6p2p_db_exec (sys_context, node->db, query, _select_node_by_id_callback, (void *) &node_by_id_data) && ret;
			  _lw6p2p_db_unlock (sys_context, node->db);
			}
		      LW6SYS_FREE (sys_context, query);
		      query = NULL;

		      if (node_by_id_data.node_url && node_by_id_data.node_ip)
			{
			  /*
			   * OK, we got a node with that id in our database,
			   * now let's check if it's worth connecting to it
			   */
			  if (node_by_id_data.ping_delay_msec <= node->db->data.consts.connect_registered_nodes_delay)
			    {
			      /*
			       * Go ahead, establishing the tentacle should not be
			       * long as we already have a "correct" ping.
			       */
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
					  _x_ ("node %" LW6SYS_PRINTF_LL
					       "x \"%s\" at %s registered without a connection, trying to establish a link"),
					  (long long) meta_array.items[index].node_id, node_by_id_data.node_url, node_by_id_data.node_ip);
			      if (_lw6p2p_node_register_tentacle
				  (sys_context, node, node_by_id_data.node_url, node_by_id_data.node_ip, meta_array.items[index].node_id))
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
					      _x_
					      ("link established with node %"
					       LW6SYS_PRINTF_LL
					       "x \"%s\" at %s which was registered without a connection"),
					      (long long) meta_array.items[index].node_id, node_by_id_data.node_url, node_by_id_data.node_ip);
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
					      _x_
					      ("can't establish link with node %"
					       LW6SYS_PRINTF_LL
					       "x \"%s\" at %s, relying on forwarded messages"),
					      (long long) meta_array.items[index].node_id, node_by_id_data.node_url, node_by_id_data.node_ip);
				}
			    }
			  else
			    {
			      /*
			       * Note : what we could do here is to schedule
			       * a check for this host, see if ping gets better...
			       * Could be a problem if there are many many hosts and
			       * automatic ping check takes time to cycle through all the list.
			       * For now, as LW6 is beta, there's no hurr (servers not crowded).
			       */
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
					  _x_
					  ("won't establish link with node %"
					   LW6SYS_PRINTF_LL
					   "x \"%s\" at %s, as its ping is too high (%d>%d)"),
					  (long long) meta_array.items[index].node_id,
					  node_by_id_data.node_url,
					  node_by_id_data.node_ip, node_by_id_data.ping_delay_msec, node->db->data.consts.connect_registered_nodes_delay);
			    }
			}
		    }
		}
	    }
	}
    }

  if (node_by_id_data.node_id)
    {
      LW6SYS_FREE (sys_context, node_by_id_data.node_id);
    }
  if (node_by_id_data.node_url)
    {
      LW6SYS_FREE (sys_context, node_by_id_data.node_url);
    }
  if (node_by_id_data.node_ip)
    {
      LW6SYS_FREE (sys_context, node_by_id_data.node_ip);
    }

  return ret;
}
