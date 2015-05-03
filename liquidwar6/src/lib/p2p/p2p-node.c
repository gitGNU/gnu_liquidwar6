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

static volatile u_int32_t seq_id = 0;

/**
 * lw6p2p_node_new
 *
 * @sys_context: global system context
 * @argc: number of args, as passed to @main
 * @argv: args array, as passed to @main
 * @db: the database to use
 * @client_backends: the list of client backends to use
 * @server_backends: the list of server backends to use
 * @bind_ip: the IP address to bind on
 * @bind_port: the IP port to listen on
 * @broadcast: wether broadcast is allowed on this node
 * @node_id: the node id
 * @public_url: the public URL we want to show
 * @title: the title of the node
 * @description: the description of the node
 * @password: the password to use
 * @bench: the bench of the node (its power)
 * @open_relay: act as an open relay or not
 * @known_nodes: list of already known nodes
 * @network_reliability: drop 1 packet out of X
 * @trojan: act as a stupid trojan to test out automatic kick-off
 *
 * Creates a new "pear to pear" node. This will fire the server
 * and allow client access, on demand. A lot of stuff can be done
 * in the background once this is called.
 *
 * Return value: a pointer on the newly created objects.
 */
lw6p2p_node_t *
lw6p2p_node_new (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6p2p_db_t * db,
		 char *client_backends, char *server_backends, char *bind_ip,
		 int bind_port, int broadcast, u_int64_t node_id,
		 char *public_url, char *title,
		 char *description, char *password, int bench, int open_relay, char *known_nodes, int network_reliability, int trojan)
{
  return (lw6p2p_node_t *) _lw6p2p_node_new (sys_context, argc, argv, (_lw6p2p_db_t *) db,
					     client_backends, server_backends,
					     bind_ip, bind_port, broadcast,
					     node_id, public_url, title, description, password, bench, open_relay, known_nodes, network_reliability, trojan);
}

_lw6p2p_node_t *
_lw6p2p_node_new (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6p2p_db_t * db,
		  char *client_backends, char *server_backends, char *bind_ip,
		  int bind_port, int broadcast, u_int64_t node_id,
		  char *public_url, char *title,
		  char *description, char *password, int bench, int open_relay, char *known_nodes, int network_reliability, int trojan)
{
  _lw6p2p_node_t *node = NULL;
  char *query = NULL;
  char *escaped_public_url = NULL;
  char *escaped_title = NULL;
  char *escaped_description = NULL;
  int ret = 1;

  node = (_lw6p2p_node_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6p2p_node_t));
  if (node)
    {
      node->id = 0;
      while (!node->id)
	{
	  node->id = ++seq_id;
	}
      node->mutex = LW6SYS_MUTEX_CREATE (sys_context);
      node->closed = 0;
      node->db = db;
      node->bind_ip = lw6sys_str_copy (sys_context, bind_ip);
      node->bind_port = bind_port;
      node->broadcast = broadcast;
      /*
       * If submitted id is not valid, just generate a new one
       */
      if (!lw6sys_check_id_64 (sys_context, node_id))
	{
	  node_id = lw6sys_generate_id_64 (sys_context);
	}
      node->node_id_int = node_id;
      node->node_id_str = lw6sys_id_ltoa (sys_context, node->node_id_int);
      if (public_url && strlen (public_url) > 0)
	{
	  node->public_url = lw6sys_url_canonize (sys_context, public_url);
	}
      else
	{
	  node->public_url = lw6net_if_guess_public_url (sys_context, bind_ip, bind_port);
	}
      if (password)
	{
	  node->password = lw6sys_str_copy (sys_context, password);
	}
      else
	{
	  node->password = lw6sys_str_copy (sys_context, "");
	}
      node->node_info =
	lw6nod_info_new (sys_context, lw6sys_build_get_package_tarname (),
			 lw6sys_build_get_version (),
			 lw6sys_build_get_codename (),
			 lw6sys_atoi (sys_context, lw6sys_build_get_stamp ()),
			 node->node_id_int, node->public_url, title,
			 description, node->password, bench, open_relay, 0, node->db->data.idle_screenshot.size, node->db->data.idle_screenshot.data);
      if (known_nodes)
	{
	  node->known_nodes = lw6sys_str_copy (sys_context, known_nodes);
	}
      else
	{
	  node->known_nodes = lw6sys_str_copy (sys_context, "");
	}
      node->network_reliability = network_reliability;
      node->trojan = trojan;
      ret = (node->mutex && node->bind_ip && node->node_id_str && node->public_url && node->password && node->node_info);
      if (ret)
	{
	  node->listener = lw6srv_start (sys_context, node->bind_ip, node->bind_port);
	  /*
	   * Now deciding the node can't be created without
	   * the listener being OK means, basically, we can't
	   * really act as pure-client mode. This is probably
	   * want we want, if you don't want network, don't
	   * start the node. If starting the node, then one
	   * runs the server. Period. You can always protect
	   * you with iptables anyway.
	   */
	  ret = (node->listener != NULL);
	}

      ret = ret && _lw6p2p_backends_init_cli (sys_context, argc, argv, &(node->backends), client_backends);
      ret = ret && _lw6p2p_backends_init_srv (sys_context, argc, argv, &(node->backends), server_backends, node->listener);

      if (ret)
	{
	  node->srv_oobs = lw6sys_list_new (sys_context, (lw6sys_free_func_t) _lw6p2p_srv_oob_callback_data_free);
	  ret = (node->srv_oobs != NULL);
	}
      if (ret)
	{
	  node->cli_oobs = lw6sys_list_new (sys_context, (lw6sys_free_func_t) _lw6p2p_cli_oob_callback_data_free);
	  ret = (node->cli_oobs != NULL);
	}
      if (ret)
	{
	  ret = lw6cnx_ticket_table_init (sys_context, &(node->ticket_table), node->db->data.consts.ticket_table_hash_size);
	}
    }

  if (node && ret && node->node_id_str && node->public_url && node->node_info
      && node->node_info->const_info.title && node->node_info->const_info.description && node->bind_ip)
    {
      ret = 0;
      escaped_public_url = lw6sys_escape_sql_value (sys_context, node->public_url);
      if (escaped_public_url)
	{
	  escaped_title = lw6sys_escape_sql_value (sys_context, node->node_info->const_info.title);
	  if (escaped_title)
	    {
	      escaped_description = lw6sys_escape_sql_value (sys_context, node->node_info->const_info.description);
	      if (escaped_description)
		{
		  query =
		    lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query
					(sys_context, node->db,
					 _LW6P2P_INSERT_LOCAL_NODE_SQL),
					_lw6p2p_db_now (sys_context, node->db),
					lw6sys_build_get_version (),
					lw6sys_build_get_codename (),
					lw6sys_atoi (sys_context,
						     lw6sys_build_get_stamp
						     ()), node->node_id_str,
					escaped_public_url, escaped_title,
					escaped_description,
					node->node_info->const_info.bench, node->node_info->const_info.open_relay, node->bind_ip, node->bind_port);
		  if (query)
		    {
		      if (_lw6p2p_db_lock (sys_context, node->db))
			{
			  ret = _lw6p2p_db_exec_ignore_data (sys_context, node->db, query);
			  _lw6p2p_db_unlock (sys_context, node->db);
			}
		      LW6SYS_FREE (sys_context, query);
		    }
		  LW6SYS_FREE (sys_context, escaped_description);
		}
	      LW6SYS_FREE (sys_context, escaped_title);
	    }
	  LW6SYS_FREE (sys_context, escaped_public_url);
	}
    }

  if (node && ret)
    {
      node->warehouse = lw6dat_warehouse_new (sys_context, node->node_id_int, _LW6P2P_NODE_DEFAULT_SEQ_0);
      ret = (node->warehouse != NULL);
    }

  if ((!ret) && node)
    {
      _lw6p2p_node_free (sys_context, node);
      node = NULL;
    }

  if (node)
    {
      if (node->trojan)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _("started node \"%s\" in trojan mode, expect problems"), node->public_url);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _("started node \"%s\""), node->public_url);
	}
    }

  return node;
}

/**
 * lw6p2p_node_free
 *
 * @sys_context: global system context
 * @node: the node to free
 *
 * Frees a node object, all network communications will be shut.
 *
 * Return value: none.
 */
void
lw6p2p_node_free (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  _lw6p2p_node_free (sys_context, (_lw6p2p_node_t *) node);
}

void
_lw6p2p_node_free (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  if (node)
    {
      _lw6p2p_node_close (sys_context, node);
      lw6cnx_ticket_table_clear (sys_context, &(node->ticket_table));
      if (node->cli_oobs)
	{
	  lw6sys_list_free (sys_context, node->cli_oobs);
	}
      if (node->srv_oobs)
	{
	  lw6sys_list_free (sys_context, node->srv_oobs);
	}
      if (node->known_nodes)
	{
	  LW6SYS_FREE (sys_context, node->known_nodes);
	}
      if (node->node_info)
	{
	  lw6nod_info_free (sys_context, node->node_info);
	}
      if (node->password)
	{
	  LW6SYS_FREE (sys_context, node->password);
	}
      if (node->public_url)
	{
	  LW6SYS_FREE (sys_context, node->public_url);
	}
      if (node->node_id_str)
	{
	  LW6SYS_FREE (sys_context, node->node_id_str);
	}
      if (node->bind_ip)
	{
	  LW6SYS_FREE (sys_context, node->bind_ip);
	}
      if (node->warehouse)
	{
	  lw6dat_warehouse_free (sys_context, node->warehouse);
	}
      if (node->reference_msg)
	{
	  lw6sys_list_free (sys_context, node->reference_msg);
	}
      if (node->draft_msg)
	{
	  lw6sys_list_free (sys_context, node->draft_msg);
	}
      if (node->mutex)
	{
	  LW6SYS_MUTEX_DESTROY (sys_context, node->mutex);
	}
      LW6SYS_FREE (sys_context, node);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL node"));
    }
}

/**
 * lw6p2p_node_repr
 *
 * @sys_context: global system context
 * @node: the node to work on
 *
 * Gives a readable representation of the node
 *
 * Return value: a dynamically allocated string
 */
char *
lw6p2p_node_repr (lw6sys_context_t * sys_context, const lw6p2p_node_t * node)
{
  return _lw6p2p_node_repr (sys_context, (_lw6p2p_node_t *) node);
}

char *
_lw6p2p_node_repr (lw6sys_context_t * sys_context, const _lw6p2p_node_t * node)
{
  char *repr = NULL;
  int nb_nodes = 0;
  int64_t seq_min = 0LL;
  int64_t seq_max = 0LL;
  int64_t seq_draft = 0LL;
  int64_t seq_reference = 0LL;

  if (node && node->id && node->bind_ip)
    {
      if (node->warehouse)
	{
	  nb_nodes = lw6dat_warehouse_get_nb_nodes (sys_context, node->warehouse);
	  seq_min = lw6dat_warehouse_get_seq_min (sys_context, node->warehouse);
	  seq_max = lw6dat_warehouse_get_seq_max (sys_context, node->warehouse);
	  seq_draft = lw6dat_warehouse_get_seq_draft (sys_context, node->warehouse);
	  seq_reference = lw6dat_warehouse_get_seq_reference (sys_context, node->warehouse);
	}
      repr =
	lw6sys_new_sprintf (sys_context, _x_
			    ("%u %s %s:%d %s (%d nodes, seq_min=%"
			     LW6SYS_PRINTF_LL "d, seq_max=%" LW6SYS_PRINTF_LL
			     "d, seq_draft=%" LW6SYS_PRINTF_LL
			     "d, seq_reference=%" LW6SYS_PRINTF_LL "d)"),
			    node->id, node->node_id_str, node->bind_ip,
			    node->bind_port, node->public_url, nb_nodes,
			    (long long) seq_min, (long long) seq_max, (long long) seq_draft, (long long) seq_reference);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't repr NULL or unitialized node"));
    }

  return repr;
}

int
_lw6p2p_node_lock (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("lock node"));
  ret = LW6SYS_MUTEX_LOCK (sys_context, node->mutex);
  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("lock node OK"));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock node"));
    }

  return ret;
}

int
_lw6p2p_node_unlock (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unlock node"));
  ret = LW6SYS_MUTEX_UNLOCK (sys_context, node->mutex);
  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unlock node OK"));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to unlock node"));
    }

  return ret;
}

int
_lw6p2p_node_trylock (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trylock node"));
  ret = LW6SYS_MUTEX_TRYLOCK (sys_context, node->mutex);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trylock node ret=%d"), ret);

  return ret;
}

static int
_node_lock (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  return _lw6p2p_node_lock (sys_context, (_lw6p2p_node_t *) node);
}

static int
_node_unlock (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  return _lw6p2p_node_unlock (sys_context, (_lw6p2p_node_t *) node);
}

/**
 * lw6p2p_node_poll
 *
 * @sys_context: global system context
 * @node: the node to poll
 * @progress: progress indicator to show the advancement
 *
 * Polls a p2p node. This must be called on a regular basis, else
 * network communication is stalled.
 *
 * Return value: 1 on success, 0 on error.
 */
int
lw6p2p_node_poll (lw6sys_context_t * sys_context, lw6p2p_node_t * node, lw6sys_progress_t * progress)
{
  int ret = 0;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_poll (sys_context, (_lw6p2p_node_t *) node, progress);
      _node_unlock (sys_context, node);
    }

  return ret;
}

static int
_poll_step1_accept_tcp (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 1;
  char *ip = NULL;
  int port = 0;
  int sock = LW6NET_SOCKET_INVALID;
  lw6srv_tcp_accepter_t *tcp_accepter = NULL;
  char *guessed_public_url = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("polling node TCP"));

  if (lw6net_socket_is_valid (sys_context, node->listener->tcp_sock))
    {
      sock = lw6net_tcp_accept (sys_context, &ip, &port, node->listener->tcp_sock, node->db->data.consts.accept_delay);
      if (lw6net_socket_is_valid (sys_context, sock) && ip != NULL && port > 0)
	{
	  tcp_accepter = lw6srv_tcp_accepter_new (sys_context, ip, port, sock);
	  if (tcp_accepter)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("TCP connection from %s:%d"), ip, port);
	      lw6sys_list_push_front (sys_context, &(node->listener->tcp_accepters), tcp_accepter);

	      /*
	       * Now we register this peer as a potential server,
	       * it will be qualified as a real server (or not) later
	       */
	      guessed_public_url = lw6sys_url_http_from_ip_port (sys_context, ip, LW6NET_DEFAULT_PORT);
	      if (guessed_public_url)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("discovered node \"%s\" from guessed url"), guessed_public_url);
		  lw6nod_info_add_discovered_node (sys_context, node->node_info, guessed_public_url);
		  LW6SYS_FREE (sys_context, guessed_public_url);
		}
	      if (node->bind_port != LW6NET_DEFAULT_PORT)
		{
		  guessed_public_url = lw6sys_url_http_from_ip_port (sys_context, ip, node->bind_port);
		  if (guessed_public_url)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				  _x_ ("discovered node \"%s\" from guessed url (using non-standard port %d)"), guessed_public_url, node->bind_port);
		      lw6nod_info_add_discovered_node (sys_context, node->node_info, guessed_public_url);
		      LW6SYS_FREE (sys_context, guessed_public_url);
		    }
		}

	      ip = NULL;	// tcp_accepter will free it
	    }
	  else
	    {
	      ret = 0;
	    }
	}
    }

  if (ip)
    {
      LW6SYS_FREE (sys_context, ip);
    }

  return ret;
}

static int
_poll_step2_recv_udp (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 1;
  char buf[LW6NET_UDP_MINIMAL_BUF_SIZE + 1];
  char *ip1 = NULL;
  char *ip2 = NULL;
  int port1 = 0;
  int port2 = 0;
  char *line = NULL;
  lw6srv_udp_buffer_t *udp_buffer = NULL;
  char *guessed_public_url = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("polling node UDP"));
  memset (buf, 0, LW6NET_UDP_MINIMAL_BUF_SIZE + 1);
  if (node->listener->udp_sock >= 0)
    {
      if (lw6net_udp_peek (sys_context, node->listener->udp_sock, buf, LW6NET_UDP_MINIMAL_BUF_SIZE, &ip1, &port1))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("received data from %s:%d"), ip1, port1);
	  line = lw6net_recv_line_udp (sys_context, node->listener->udp_sock, &ip2, &port2);
	  if (line)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("UDP connection from %s:%d"), ip2, port2);
	      udp_buffer = lw6srv_udp_buffer_new (sys_context, ip2, port2, line);
	      if (udp_buffer)
		{
		  lw6sys_list_push_front (sys_context, &(node->listener->udp_buffers), udp_buffer);
		  /*
		   * Now we register this peer as a potential server,
		   * it will be qualified as a real server (or not) later
		   */
		  guessed_public_url = lw6sys_url_http_from_ip_port (sys_context, ip2, LW6NET_DEFAULT_PORT);
		  if (guessed_public_url)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("discovered node \"%s\" from guessed url"), guessed_public_url);
		      lw6nod_info_add_discovered_node (sys_context, node->node_info, guessed_public_url);
		      LW6SYS_FREE (sys_context, guessed_public_url);
		    }
		  if (node->bind_port != LW6NET_DEFAULT_PORT)
		    {
		      guessed_public_url = lw6sys_url_http_from_ip_port (sys_context, ip2, node->bind_port);
		      if (guessed_public_url)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				      _x_ ("discovered node \"%s\" from guessed url (using non-standard port %d)"), guessed_public_url, node->bind_port);
			  lw6nod_info_add_discovered_node (sys_context, node->node_info, guessed_public_url);
			  LW6SYS_FREE (sys_context, guessed_public_url);
			}
		    }
		  line = NULL;	// udp_buffer will free it
		  ip2 = NULL;	// udp_buffer will free it
		}
	      else
		{
		  ret = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("udp data received from %s:%d, but it's not correct"), ip1, port1);
	    }
	}
    }

  if (ip1)
    {
      LW6SYS_FREE (sys_context, ip1);
    }
  if (ip2)
    {
      LW6SYS_FREE (sys_context, ip2);
    }
  if (line)
    {
      LW6SYS_FREE (sys_context, line);
    }

  return ret;
}

static int
_tcp_accepter_reply (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int ret = 1;
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  lw6srv_tcp_accepter_t *tcp_accepter = (lw6srv_tcp_accepter_t *) data;
  _lw6p2p_srv_oob_callback_data_t *srv_oob = NULL;
  int i = 0;
  int analyse_tcp_ret = 0;
  u_int64_t remote_id = 0;
  char *remote_url = NULL;
  int tentacle_index = -1;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("processing tcp reply sock=%d"), tcp_accepter->sock);
  lw6net_tcp_peek (sys_context, &(tcp_accepter->sock), tcp_accepter->first_line, LW6SRV_PROTOCOL_BUFFER_SIZE, 0);

  for (i = 0; i < node->backends.nb_srv_backends && ret; ++i)
    {
      analyse_tcp_ret = lw6srv_analyse_tcp (sys_context, node->backends.srv_backends[i], tcp_accepter, node->node_info, &remote_id, NULL);
      if (analyse_tcp_ret & LW6SRV_ANALYSE_DEAD)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("dead accepter, scheduling it for deletion (sock=%d)"), tcp_accepter->sock);
	  lw6net_socket_close (sys_context, &(tcp_accepter->sock));
	  ret = 0;
	}
      else
	{
	  if (analyse_tcp_ret & LW6SRV_ANALYSE_UNDERSTANDABLE)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("understood accepter, scheduling it for deletion"));
	      ret = 0;		// will be filtered
	      if (analyse_tcp_ret & LW6SRV_ANALYSE_OOB)
		{
		  srv_oob =
		    _lw6p2p_srv_oob_callback_data_new (sys_context, node->backends.srv_backends[i],
						       node->node_info,
						       tcp_accepter->client_id.client_ip, tcp_accepter->client_id.client_port, tcp_accepter->sock, NULL);
		  if (srv_oob)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process srv_oob (tcp)"));
		      srv_oob->srv_oob->thread = lw6sys_thread_create (sys_context, _lw6p2p_srv_oob_callback, NULL, srv_oob);
		      lw6sys_lifo_push (sys_context, &(node->srv_oobs), srv_oob);
		    }
		}
	      else
		{
		  tentacle_index = _lw6p2p_node_find_tentacle (sys_context, node, remote_id);
		  if (tentacle_index < 0)
		    {
		      /*
		       * We analyse again, but this time querying for
		       * remote_url. The first call does not do this
		       * to save time, since protocol analysis must
		       * really remain simple. Besides this "double"
		       * anlysis is rare since it only concerns one
		       * message at the beginning of each incoming
		       * connection.
		       */
		      analyse_tcp_ret =
			lw6srv_analyse_tcp (sys_context, node->backends.srv_backends[i], tcp_accepter, node->node_info, &remote_id, &remote_url);
		      if (remote_id != 0 && remote_url != NULL)
			{
			  if (_lw6p2p_node_register_tentacle (sys_context, node, remote_url, tcp_accepter->client_id.client_ip, remote_id))
			    {
			      tentacle_index = _lw6p2p_node_find_tentacle (sys_context, node, remote_id);
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't create tentacle with \"%s\""), remote_url);
			    }
			  LW6SYS_FREE (sys_context, remote_url);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("wrong id/url"));
			}
		    }
		  if (tentacle_index >= 0 && tentacle_index < LW6P2P_MAX_NB_TENTACLES)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("feed srv (tcp)"));
		      lw6srv_feed_with_tcp (sys_context, node->backends.srv_backends[i], node->tentacles[tentacle_index].srv_connections[i], tcp_accepter);
		    }
		  else
		    {
		      lw6net_socket_close (sys_context, &(tcp_accepter->sock));
		    }
		}
	    }
	}
    }

  return ret;
}

static int
_udp_buffer_reply (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int ret = 1;
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  lw6srv_udp_buffer_t *udp_buffer = (lw6srv_udp_buffer_t *) data;
  _lw6p2p_srv_oob_callback_data_t *srv_oob = NULL;
  int i = 0;
  int analyse_udp_ret = 0;
  u_int64_t remote_id = 0;
  char *remote_url = NULL;
  int tentacle_index = -1;

  for (i = 0; i < node->backends.nb_srv_backends && ret; ++i)
    {
      analyse_udp_ret = lw6srv_analyse_udp (sys_context, node->backends.srv_backends[i], udp_buffer, node->node_info, &remote_id, NULL);
      if (analyse_udp_ret & LW6SRV_ANALYSE_DEAD)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("dead buffer, scheduling it for deletion"));
	  ret = 0;
	}
      else
	{
	  if (analyse_udp_ret & LW6SRV_ANALYSE_UNDERSTANDABLE)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("understood buffer, scheduling it for deletion"));
	      ret = 0;		// will be filtered
	      if (analyse_udp_ret & LW6SRV_ANALYSE_OOB)
		{
		  srv_oob =
		    _lw6p2p_srv_oob_callback_data_new (sys_context, node->backends.srv_backends[i],
						       node->node_info,
						       udp_buffer->client_id.client_ip,
						       udp_buffer->client_id.client_port, node->listener->udp_sock, udp_buffer->line);
		  if (srv_oob)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("process srv_oob (udp)"));
		      srv_oob->srv_oob->thread = lw6sys_thread_create (sys_context, _lw6p2p_srv_oob_callback, NULL, srv_oob);
		      lw6sys_lifo_push (sys_context, &(node->srv_oobs), srv_oob);
		    }
		}
	      else
		{
		  tentacle_index = _lw6p2p_node_find_tentacle (sys_context, node, remote_id);
		  if (tentacle_index < 0)
		    {
		      /*
		       * We analyse again, but this time querying for
		       * remote_url. The first call does not do this
		       * to save time, since protocol analysis must
		       * really remain simple. Besides this "double"
		       * anlysis is rare since it only concerns one
		       * message at the beginning of each incoming
		       * connection.
		       */
		      analyse_udp_ret = lw6srv_analyse_udp (sys_context, node->backends.srv_backends[i], udp_buffer, node->node_info, &remote_id, &remote_url);
		      if (remote_id != 0 && remote_url != NULL)
			{
			  if (_lw6p2p_node_register_tentacle (sys_context, node, remote_url, udp_buffer->client_id.client_ip, remote_id))
			    {
			      tentacle_index = _lw6p2p_node_find_tentacle (sys_context, node, remote_id);
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't create tentacle with \"%s\""), remote_url);
			    }
			  LW6SYS_FREE (sys_context, remote_url);
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("wrong id/url"));
			}
		    }
		  if (tentacle_index >= 0 && tentacle_index < LW6P2P_MAX_NB_TENTACLES)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("feed srv (udp)"));
		      lw6srv_feed_with_udp (sys_context, node->backends.srv_backends[i], node->tentacles[tentacle_index].srv_connections[i], udp_buffer);
		    }
		}
	    }
	}
    }

  return ret;
}

static int
_poll_step3_reply (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 1;

  /*
   * This is a place where, definitely, there's *really* an
   * interest in all those lock functions. The idea is that
   * the reply process might trigger recv callbacks which
   * require lock access.
   */
  if (_lw6p2p_node_unlock (sys_context, node))
    {
      lw6sys_list_filter (sys_context, &(node->listener->tcp_accepters), _tcp_accepter_reply, node);
      lw6sys_list_filter (sys_context, &(node->listener->udp_buffers), _udp_buffer_reply, node);
      _lw6p2p_node_lock (sys_context, node);
    }

  return ret;
}

static int
_cli_oob_filter (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int ret = 0;
  _lw6p2p_cli_oob_callback_data_t *cli_oob = (_lw6p2p_cli_oob_callback_data_t *) data;

  ret = _lw6p2p_cli_oob_filter (sys_context, cli_oob);

  return ret;
}

static int
_poll_step4_cli_oob (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 1;

  lw6sys_list_filter (sys_context, &(node->cli_oobs), _cli_oob_filter, NULL);

  return ret;
}

static int
_srv_oob_filter (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int ret = 0;
  _lw6p2p_srv_oob_callback_data_t *srv_oob = (_lw6p2p_srv_oob_callback_data_t *) data;

  ret = _lw6p2p_srv_oob_filter (sys_context, srv_oob);

  return ret;
}

static int
_poll_step5_srv_oob (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 1;

  lw6sys_list_filter (sys_context, &(node->srv_oobs), _srv_oob_filter, NULL);

  return ret;
}

static int
_poll_step6_explore_discover_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 0;

  ret = _lw6p2p_explore_discover_nodes_if_needed (sys_context, node);

  return ret;
}

static int
_poll_step7_flush_discovered_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 0;

  ret = _lw6p2p_flush_discovered_nodes_if_needed (sys_context, node);

  return ret;
}

static int
_poll_step8_explore_verify_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 0;

  ret = _lw6p2p_explore_verify_nodes_if_needed (sys_context, node);

  return ret;
}

static int
_poll_step9_flush_verified_nodes (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 0;

  ret = _lw6p2p_flush_verified_nodes_if_needed (sys_context, node);

  return ret;
}

static int
_poll_step10_send_atoms (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 1;
  int i = 0;
  u_int64_t remote_id_int = 0LL;
  char *remote_id_str = NULL;
  lw6sys_list_t *atom_str_list;
  char *atom_str = NULL;
  u_int32_t logical_ticket_sig = 0;
  int send_i = 0;

  for (i = 0; i < LW6P2P_MAX_NB_TENTACLES; ++i)
    {
      if (_lw6p2p_tentacle_enabled (sys_context, &(node->tentacles[i])))
	{
	  send_i = 0;
	  remote_id_int = node->tentacles[i].remote_id_int;
	  remote_id_str = node->tentacles[i].remote_id_str;
	  atom_str_list = lw6dat_warehouse_get_atom_str_list_not_sent (sys_context, node->warehouse, remote_id_int);
	  if (atom_str_list)
	    {
	      while ((atom_str = lw6sys_list_pop_front (sys_context, &atom_str_list)) != NULL)
		{
		  logical_ticket_sig =
		    lw6msg_ticket_calc_sig (sys_context, lw6cnx_ticket_table_get_send (sys_context, &(node->ticket_table), remote_id_str), node->node_id_int,
					    remote_id_int, atom_str);
		  if (!_lw6p2p_tentacle_send_best
		      (sys_context, &(node->tentacles[i]), now, &(node->ticket_table), logical_ticket_sig, node->node_id_int, remote_id_int, atom_str, 1))
		    {
		      ret = 0;
		    }

		  ++send_i;

		  LW6SYS_FREE (sys_context, atom_str);
		}
	    }
	}
    }

  return ret;
}

/*
 * This function *must* be called in locked mode
 */
static int
_poll_step11_tentacles (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 1;
  int i = 0;

  /*
   * This is a place where, definitely, there's *really* an
   * interest in all those lock functions. The idea is that
   * the tentacle polling might trigger recv messages which
   * can be asynchronous (in that case, the lock protects us,
   * typical case: mod_http with its threads) but also
   * synchronous (typical case: any backend which polls on
   * a socket, UDP or TCP). So we don't want to be in locked
   * when those are fired.
   */
  if (_lw6p2p_node_unlock (sys_context, node))
    {
      for (i = 0; i < LW6P2P_MAX_NB_TENTACLES; ++i)
	{
	  if (_lw6p2p_tentacle_enabled (sys_context, &(node->tentacles[i])))
	    {
	      _lw6p2p_tentacle_poll (sys_context, node, &(node->tentacles[i]), lw6dat_warehouse_get_local_serial (sys_context, node->warehouse));
	    }
	}
      _lw6p2p_node_lock (sys_context, node);
    }

  return ret;
}

static int
_poll_step12_miss_list (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now, lw6sys_progress_t * progress)
{
  int ret = 1;
  lw6sys_list_t *list = NULL;
  lw6dat_miss_t *miss = NULL;
  u_int32_t logical_ticket_sig = 0;
  u_int64_t remote_id_int = 0LL;
  char *remote_id_str = NULL;
  int i = 0;
  const char *msg = NULL;
  int nb_atom_parts_since_last_poll = 0;
  int disable_miss = 0;
  int tentacle_i;

  if (now - node->db->data.consts.miss_get_delay >= node->last_poll_miss_timestamp)
    {
      node->last_poll_miss_timestamp = now;

      list = lw6dat_warehouse_get_miss_list (sys_context, node->warehouse, node->db->data.consts.miss_max_range, progress);
      if (list)
	{
	  while ((miss = lw6sys_list_pop_front (sys_context, &list)) != NULL)
	    {
	      nb_atom_parts_since_last_poll = lw6dat_warehouse_get_nb_atom_parts_since_last_poll (sys_context, node->warehouse, miss->from_id);
	      disable_miss = (nb_atom_parts_since_last_poll > node->db->data.consts.received_atom_parts_per_poll_to_disable_miss) ? 1 : 0;
	      tentacle_i = _lw6p2p_node_find_tentacle (sys_context, node, miss->from_id);
	      if (tentacle_i >= 0)
		{
		  /*
		   * If the new miss is included in previous MISS message,
		   * and "not enough time" has elapsed then do not send it
		   * again as we suspect it's better to wait for a while before
		   * firing it again, this avoids spamming too many MISS messages.
		   */
		  disable_miss = 0;
		  if (lw6dat_miss_overlaps (sys_context, miss, &(node->tentacles[tentacle_i].last_miss)))
		    {
		      if (now - node->db->data.consts.miss_duplicate_delay < node->tentacles[tentacle_i].last_miss_timestamp)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				      _x_ ("not sending MISS from_id=%"
					   LW6SYS_PRINTF_LL
					   "x serial_min=%d serial_max=%d as it overlaps with previous serial_min=%d serial_max=%d"),
				      (long long) miss->from_id,
				      miss->serial_min, miss->serial_max,
				      node->tentacles[tentacle_i].last_miss.serial_min, node->tentacles[tentacle_i].last_miss.serial_max);
			  disable_miss = 1;
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				      _x_ ("sending MISS from_id=%"
					   LW6SYS_PRINTF_LL
					   "x serial_min=%d serial_max=%d, it overlaps with previous serial_min=%d serial_max=%d but we've waited for too long"),
				      (long long) miss->from_id,
				      miss->serial_min, miss->serial_max,
				      node->tentacles[tentacle_i].last_miss.serial_min, node->tentacles[tentacle_i].last_miss.serial_max);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				  _x_ ("sending MISS from_id=%"
				       LW6SYS_PRINTF_LL
				       "x serial_min=%d serial_max=%d, it does not overlap with previous serial_min=%d serial_max=%d"),
				  (long long) miss->from_id, miss->serial_min,
				  miss->serial_max, node->tentacles[tentacle_i].last_miss.serial_min, node->tentacles[tentacle_i].last_miss.serial_max);
		    }
		  if (!disable_miss)
		    {
		      lw6dat_miss_sync (sys_context, &(node->tentacles[tentacle_i].last_miss), miss);
		      node->tentacles[tentacle_i].last_miss_timestamp = now;
		    }
		}
	      if (!disable_miss)
		{
		  msg = lw6msg_cmd_generate_miss (sys_context, miss->from_id, node->node_id_int, miss->serial_min, miss->serial_max);
		  if (msg)
		    {
		      for (i = 0; i < LW6P2P_MAX_NB_TENTACLES; ++i)
			{
			  if (_lw6p2p_tentacle_enabled (sys_context, &(node->tentacles[i])))
			    {
			      remote_id_int = node->tentacles[i].remote_id_int;
			      remote_id_str = node->tentacles[i].remote_id_str;

			      logical_ticket_sig =
				lw6msg_ticket_calc_sig
				(sys_context, lw6cnx_ticket_table_get_send (sys_context, &(node->ticket_table), remote_id_str), node->node_id_int,
				 remote_id_int, msg);

			      /*
			       * Some hesitation on what to put in the
			       * logical_from parameter here, saying it's
			       * us is not such a bad idea as we are
			       * at the initiative of saying "this is missing"
			       */
			      _lw6p2p_tentacle_send_best (sys_context, &
							  (node->tentacles
							   [i]), now, &(node->ticket_table), logical_ticket_sig, node->node_id_int, remote_id_int, msg, 1);
			    }
			}
		      LW6SYS_FREE (sys_context, msg);
		    }
		}
	      lw6dat_miss_free (sys_context, miss);
	    }
	}
      else
	{
	  ret = 0;
	}
      lw6dat_warehouse_reset_nb_atom_parts_since_last_poll (sys_context, node->warehouse);
    }

  return ret;
}

/*
 * This function *must* be called in locked mode
 */
static int
_poll_step13_connect_registered (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t now)
{
  int ret = 0;

  ret = _lw6p2p_connect_registered_nodes_if_needed (sys_context, node);

  return ret;
}


/*
 * This function *must* be called in locked mode
 */
int
_lw6p2p_node_poll (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6sys_progress_t * progress)
{
  int ret = 1;
  int64_t now = 0LL;

  now = lw6sys_get_timestamp (sys_context);

  ret = _poll_step1_accept_tcp (sys_context, node, now) && ret;
  ret = _poll_step2_recv_udp (sys_context, node, now) && ret;
  ret = _poll_step3_reply (sys_context, node, now) && ret;
  ret = _poll_step4_cli_oob (sys_context, node, now) && ret;
  ret = _poll_step5_srv_oob (sys_context, node, now) && ret;
  ret = _poll_step6_explore_discover_nodes (sys_context, node, now) && ret;
  ret = _poll_step7_flush_discovered_nodes (sys_context, node, now) && ret;
  ret = _poll_step8_explore_verify_nodes (sys_context, node, now) && ret;
  ret = _poll_step9_flush_verified_nodes (sys_context, node, now) && ret;
  ret = _poll_step10_send_atoms (sys_context, node, now) && ret;
  ret = _poll_step11_tentacles (sys_context, node, now) && ret;
  ret = _poll_step12_miss_list (sys_context, node, now, progress) && ret;
  ret = _poll_step13_connect_registered (sys_context, node, now) && ret;

  return ret;
}

/**
 * lw6p2p_node_close
 *
 * @sys_context: global system context
 * @node: the node to close
 *
 * Closes a p2p node. Closing is necessary in some contexts, for instance
 * scheme/smob instanciation when you want to release the object ressources
 * (sockets, ports, threads...) *before* it is deleted by, for instance,
 * a garbage collector.
 *
 * Return value: 1 on success, 0 on error.
 */
void
lw6p2p_node_close (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      _lw6p2p_node_close (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }
}

void
_lw6p2p_node_close (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  char *query = NULL;
  _lw6p2p_srv_oob_callback_data_t *srv_oob = NULL;
  _lw6p2p_cli_oob_callback_data_t *cli_oob = NULL;
  int i = 0;

  if (node)
    {
      /*
       * It's important to test it here, indeed, we can't assume
       * db is still here when closing node *if* node is closed
       * by garbage collector.
       */
      if (!node->closed)
	{
	  node->closed = 1;

	  if (_lw6p2p_db_lock (sys_context, node->db))
	    {
	      query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query (sys_context, node->db, _LW6P2P_DELETE_NODE_BY_ID_SQL), node->node_id_str);
	      if (query)
		{
		  _lw6p2p_db_exec_ignore_data (sys_context, node->db, query);
		  LW6SYS_FREE (sys_context, query);
		}
	      /*
	       * OK this is paranoid, we already deleted it by ID but...
	       * who knows, well, never be too sure ;)
	       */
	      query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query (sys_context, node->db, _LW6P2P_DELETE_NODE_BY_URL_SQL), node->public_url);
	      if (query)
		{
		  _lw6p2p_db_exec_ignore_data (sys_context, node->db, query);
		  LW6SYS_FREE (sys_context, query);
		}
	      _lw6p2p_db_unlock (sys_context, node->db);
	    }

	  for (i = 0; i < LW6P2P_MAX_NB_TENTACLES; ++i)
	    {
	      _lw6p2p_tentacle_clear (sys_context, &(node->tentacles[i]));
	    }

	  if (node->srv_oobs)
	    {
	      while (node->srv_oobs && !lw6sys_list_is_empty (sys_context, node->srv_oobs))
		{
		  srv_oob = (_lw6p2p_srv_oob_callback_data_t *) lw6sys_list_pop_front (sys_context, &(node->srv_oobs));
		  _lw6p2p_srv_oob_callback_data_free (sys_context, srv_oob);
		}
	      if (!(node->srv_oobs))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("NULL srv_oobs after emptying"));
		}
	    }
	  if (node->cli_oobs)
	    {
	      while (node->cli_oobs && !lw6sys_list_is_empty (sys_context, node->cli_oobs))
		{
		  cli_oob = (_lw6p2p_cli_oob_callback_data_t *) lw6sys_list_pop_front (sys_context, &(node->cli_oobs));
		  _lw6p2p_cli_oob_callback_data_free (sys_context, cli_oob);
		}
	      if (!(node->cli_oobs))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("NULL cli_oobs after emptying"));
		}
	    }

	  _lw6p2p_backends_clear_srv (sys_context, &(node->backends));
	  _lw6p2p_backends_clear_cli (sys_context, &(node->backends));

	  if (node->listener)
	    {
	      lw6srv_stop (sys_context, node->listener);
	      node->listener = NULL;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("closing closed node"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to close NULL node"));
    }
}

/**
 * lw6p2p_node_get_id
 *
 * @sys_context: global system context
 * @node: the node to query
 *
 * Returns the node id, an id which is supposed to uniquely identify
 * the node at run-time.
 *
 * Return value: numerical id.
 */
u_int64_t
lw6p2p_node_get_id (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  u_int64_t id = 0;

  id = _lw6p2p_node_get_id (sys_context, (_lw6p2p_node_t *) node);

  return id;
}

u_int64_t
_lw6p2p_node_get_id (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  u_int64_t id = 0;

  id = node->node_id_int;

  return id;
}

static int
_select_node_by_url_callback (lw6sys_context_t * sys_context, void *func_data, int nb_fields, char **fields_values, char **fields_names)
{
  int ret = 0;
  int *count = (int *) func_data;

  (*count)++;
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("select_node_by_url_callback called with %d fields, count is %d"), nb_fields, (*count));

  return ret;
}

int
_lw6p2p_node_insert_discovered (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, char *public_url)
{
  int ret = 1;
  char *query = NULL;
  int count = 0;
  char *canonized_public_url;
  char *escaped_public_url;

  canonized_public_url = lw6sys_url_canonize (sys_context, public_url);
  if (canonized_public_url)
    {
      escaped_public_url = lw6sys_escape_sql_value (sys_context, canonized_public_url);
      if (escaped_public_url)
	{
	  if (_lw6p2p_db_lock (sys_context, node->db))
	    {
	      query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query (sys_context, node->db, _LW6P2P_SELECT_NODE_BY_URL_SQL), escaped_public_url);
	      if (query)
		{
		  ret = _lw6p2p_db_exec (sys_context, node->db, query, _select_node_by_url_callback, (void *) &count) && ret;
		  LW6SYS_FREE (sys_context, query);
		  if (count > 0)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("there's already a node with url \"%s\""), public_url);
		    }
		  else
		    {
		      query =
			lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query
					    (sys_context, node->db, _LW6P2P_INSERT_DISCOVERED_NODE_SQL), escaped_public_url, _lw6p2p_db_now (sys_context,
																	     node->db));
		      if (query)
			{
			  ret = _lw6p2p_db_exec_ignore_data (sys_context, node->db, query) && ret;
			  LW6SYS_FREE (sys_context, query);
			}
		      else
			{
			  ret = 0;
			}
		    }
		}
	      else
		{
		  ret = 0;
		}
	      _lw6p2p_db_unlock (sys_context, node->db);
	    }
	  LW6SYS_FREE (sys_context, escaped_public_url);
	}
      LW6SYS_FREE (sys_context, canonized_public_url);
    }

  return ret;
}

int
_lw6p2p_node_update_peer_info_x (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *version,
				 const char *codename, int stamp,
				 const char *id, const char *url,
				 const char *title, const char *description,
				 int has_password, int bench, int open_relay,
				 int creation_timestamp,
				 const char *community_id, int round,
				 const char *level, int required_bench,
				 int nb_colors, int max_nb_colors, int nb_cursors, int max_nb_cursors, int nb_nodes, int max_nb_nodes)
{
  int ret = 1;
  char *query = NULL;
  char *escaped_version = NULL;
  char *escaped_codename = NULL;
  char *escaped_id = NULL;
  char *escaped_url = NULL;
  char *escaped_title = NULL;
  char *escaped_description = NULL;
  char *escaped_community_id = NULL;
  char *escaped_level = NULL;
  int available;

  escaped_version = lw6sys_escape_sql_value (sys_context, version);
  escaped_codename = lw6sys_escape_sql_value (sys_context, codename);
  escaped_id = lw6sys_escape_sql_value (sys_context, id);
  escaped_url = lw6sys_escape_sql_value (sys_context, url);
  escaped_title = lw6sys_escape_sql_value (sys_context, title);
  escaped_description = lw6sys_escape_sql_value (sys_context, description);
  escaped_community_id = lw6sys_escape_sql_value (sys_context, community_id);
  escaped_level = lw6sys_escape_sql_value (sys_context, level);
  // todo : test for bench to set available the right way
  available = ((!lw6sys_str_is_null_or_empty (sys_context, id))
	       &&
	       (lw6sys_version_is_compatible
		(sys_context, version, node->node_info->const_info.version))
	       && (required_bench <= node->node_info->const_info.bench)
	       && (!lw6sys_str_is_same (sys_context, id, node->node_id_str))
	       && (!lw6sys_str_is_null_or_empty (sys_context, level))
	       && (nb_colors < max_nb_colors) && (nb_cursors < max_nb_cursors) && (nb_nodes < max_nb_nodes)) ? 1 : 0;

  query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query
			      (sys_context, node->db,
			       _LW6P2P_UPDATE_NODE_INFO_SQL),
			      creation_timestamp, escaped_version,
			      escaped_codename, stamp,
			      escaped_id, escaped_title,
			      escaped_description, has_password,
			      bench, open_relay, escaped_community_id, round,
			      escaped_level, required_bench, nb_colors,
			      max_nb_colors, nb_cursors, max_nb_cursors, nb_nodes, max_nb_nodes, available, escaped_url);
  if (query)
    {
      if (_lw6p2p_db_lock (sys_context, node->db))
	{
	  ret = _lw6p2p_db_exec_ignore_data (sys_context, node->db, query);
	  _lw6p2p_db_unlock (sys_context, node->db);
	}
      LW6SYS_FREE (sys_context, query);
    }

  if (escaped_version)
    {
      LW6SYS_FREE (sys_context, escaped_version);
    }
  if (escaped_codename)
    {
      LW6SYS_FREE (sys_context, escaped_codename);
    }
  if (escaped_id)
    {
      LW6SYS_FREE (sys_context, escaped_id);
    }
  if (escaped_url)
    {
      LW6SYS_FREE (sys_context, escaped_url);
    }
  if (escaped_title)
    {
      LW6SYS_FREE (sys_context, escaped_title);
    }
  if (escaped_description)
    {
      LW6SYS_FREE (sys_context, escaped_description);
    }
  if (escaped_community_id)
    {
      LW6SYS_FREE (sys_context, escaped_community_id);
    }
  if (escaped_level)
    {
      LW6SYS_FREE (sys_context, escaped_level);
    }

  return ret;
}

int
_lw6p2p_node_update_peer_info (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6nod_info_t * peer_info)
{
  int ret = 1;

  ret =
    _lw6p2p_node_update_peer_info_x (sys_context, node, peer_info->const_info.version,
				     peer_info->const_info.codename,
				     peer_info->const_info.stamp,
				     peer_info->const_info.ref_info.id_str,
				     peer_info->const_info.ref_info.url,
				     peer_info->const_info.title,
				     peer_info->const_info.description,
				     peer_info->const_info.has_password,
				     peer_info->const_info.bench,
				     peer_info->const_info.open_relay,
				     _lw6p2p_db_timestamp (sys_context, node->db,
							   peer_info->const_info.creation_timestamp),
				     lw6sys_str_empty_if_null (sys_context, peer_info->dyn_info.community_id_str),
				     peer_info->dyn_info.round,
				     lw6sys_str_empty_if_null (sys_context, peer_info->dyn_info.level),
				     peer_info->dyn_info.required_bench,
				     peer_info->dyn_info.nb_colors,
				     peer_info->dyn_info.max_nb_colors,
				     peer_info->dyn_info.nb_cursors,
				     peer_info->dyn_info.max_nb_cursors, peer_info->dyn_info.nb_nodes, peer_info->dyn_info.max_nb_nodes);

  return ret;
}

int
_lw6p2p_node_update_peer_net (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *id, const char *url, const char *ip, int port,
			      int last_ping_timestamp, int ping_delay_msec)
{
  int ret = 1;
  char *query = NULL;
  char *escaped_id = NULL;
  char *escaped_url = NULL;

  escaped_id = lw6sys_escape_sql_value (sys_context, id);
  escaped_url = lw6sys_escape_sql_value (sys_context, url);

  query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query
			      (sys_context, node->db, _LW6P2P_UPDATE_NODE_NET_SQL), ip, port, last_ping_timestamp, ping_delay_msec, escaped_url);
  if (query)
    {
      if (_lw6p2p_db_lock (sys_context, node->db))
	{
	  ret = _lw6p2p_db_exec_ignore_data (sys_context, node->db, query);
	  _lw6p2p_db_unlock (sys_context, node->db);
	}
      LW6SYS_FREE (sys_context, query);
    }

  if (escaped_id)
    {
      LW6SYS_FREE (sys_context, escaped_id);
    }
  if (escaped_url)
    {
      LW6SYS_FREE (sys_context, escaped_url);
    }

  return ret;
}

int
_lw6p2p_node_update_local (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6nod_info_t * node_info)
{
  int ret = 1;

  /*
   * Call the generic update_peer function for that matter, in fact
   * the update is pretty much the same except for the IP/port & timeout
   */
  ret = _lw6p2p_node_update_peer_info (sys_context, node, node_info) && ret;

  ret =
    _lw6p2p_node_update_peer_net (sys_context, node, node_info->const_info.ref_info.id_str,
				  node_info->const_info.ref_info.url, node->bind_ip, node->bind_port, 0, 0) && ret;

  return ret;
}

int
_lw6p2p_node_find_free_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = -1;
  int i;

  for (i = 0; i < LW6P2P_MAX_NB_TENTACLES && ret < 0; ++i)
    {
      if (!_lw6p2p_tentacle_enabled (sys_context, &(node->tentacles[i])))
	{
	  ret = i;
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("found free tentacle %d"), i);
	}
    }

  return ret;
}

int
_lw6p2p_node_find_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id)
{
  int ret = -1;
  int i;

  if (remote_id != 0)
    {
      for (i = 0; i < LW6P2P_MAX_NB_TENTACLES && ret < 0; ++i)
	{
	  if (node->tentacles[i].remote_id_int == remote_id)
	    {
	      ret = i;
	    }
	}
    }

  return ret;
}

int
_lw6p2p_node_register_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *remote_url, const char *real_remote_ip, u_int64_t remote_id)
{
  int ret = 0;
  int i = 0;

  i = _lw6p2p_node_find_tentacle (sys_context, node, remote_id);
  if (i < 0)
    {
      i = _lw6p2p_node_find_free_tentacle (sys_context, node);
      if (i >= 0 && i < LW6P2P_MAX_NB_TENTACLES)
	{
	  ret =
	    _lw6p2p_tentacle_init (sys_context, &(node->tentacles[i]), &(node->backends),
				   node->listener,
				   node->public_url, remote_url, real_remote_ip, node->password, node->node_id_int, remote_id, node->network_reliability);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find a free tentacle"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("tentacle already exists (i=%d)"), i);
    }

  return ret;
}

int
_lw6p2p_node_unregister_tentacle (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id)
{
  int ret = 0;
  int i = 0;

  i = _lw6p2p_node_find_tentacle (sys_context, node, remote_id);
  if (i >= 0 && i < LW6P2P_MAX_NB_TENTACLES)
    {
      _lw6p2p_tentacle_clear (sys_context, &(node->tentacles[i]));
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find tentacle"));
    }

  return ret;
}

int
_get_entries_callback (lw6sys_context_t * sys_context, void *func_data, int nb_fields, char **fields_values, char **fields_names)
{
  int ret = 0;
  lw6sys_list_t **list = (lw6sys_list_t **) func_data;
  lw6p2p_entry_t *entry = NULL;

  if (nb_fields == _LW6P2P_DB_NODE_NB_FIELDS)
    {
      entry =
	lw6p2p_entry_new (sys_context, lw6sys_atoi
			  (sys_context, fields_values
			   [_LW6P2P_DB_NODE_ORDER_CREATION_TIMESTAMP]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_VERSION],
			  fields_values[_LW6P2P_DB_NODE_ORDER_CODENAME],
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_STAMP]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_ID],
			  fields_values[_LW6P2P_DB_NODE_ORDER_URL],
			  fields_values[_LW6P2P_DB_NODE_ORDER_TITLE],
			  fields_values[_LW6P2P_DB_NODE_ORDER_DESCRIPTION],
			  lw6sys_atob (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_HAS_PASSWORD]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_BENCH]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_OPEN_RELAY]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_COMMUNITY_ID],
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_ROUND]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_LEVEL],
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_REQUIRED_BENCH]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_NB_COLORS]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_MAX_NB_COLORS]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_NB_CURSORS]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_MAX_NB_CURSORS]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_NB_NODES]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_MAX_NB_NODES]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_IP],
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_PORT]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_LAST_PING_TIMESTAMP]),
			  lw6sys_atoi (sys_context, fields_values
				       [_LW6P2P_DB_NODE_ORDER_PING_DELAY_MSEC]), lw6sys_atob (sys_context, fields_values[_LW6P2P_DB_NODE_ORDER_AVAILABLE]));
      if (entry)
	{
	  lw6sys_list_push_front (sys_context, list, entry);
	}
    }

  return ret;
}

lw6sys_list_t *
_lw6p2p_node_get_entries (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int skip_local)
{
  lw6sys_list_t *ret;
  char *query = NULL;

  ret = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6p2p_entry_free);
  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("flush verified nodes"));

      /*
       * We lock the whole session, to avoid double listing
       * something, or forgetting something if things change
       * between the two calls
       */
      if (_lw6p2p_db_lock (sys_context, node->db))
	{
	  query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query
				      (sys_context, node->db, _LW6P2P_SELECT_UNAVAILABLE_NODE_SQL), skip_local ? node->node_id_str : LW6SYS_STR_EMPTY);
	  if (query)
	    {
	      _lw6p2p_db_exec (sys_context, node->db, query, _get_entries_callback, &ret);
	      _lw6p2p_db_unlock (sys_context, node->db);
	    }
	  LW6SYS_FREE (sys_context, query);

	  query = lw6sys_new_sprintf (sys_context, _lw6p2p_db_get_query
				      (sys_context, node->db, _LW6P2P_SELECT_AVAILABLE_NODE_SQL), skip_local ? node->node_id_str : LW6SYS_STR_EMPTY);
	  if (query)
	    {
	      if (_lw6p2p_db_lock (sys_context, node->db))
		{
		  _lw6p2p_db_exec (sys_context, node->db, query, _get_entries_callback, &ret);
		}
	      LW6SYS_FREE (sys_context, query);
	    }
	  _lw6p2p_db_unlock (sys_context, node->db);
	}
    }

  return ret;
}

/**
 * lw6p2p_node_get_entries
 *
 * @sys_context: global system context
 * @node: node to query
 * @skip_local: wether to skip local node, 1 to skip, 0 to keep
 *
 * Returns a list of all known nodes, this is a plain
 * table dump, sorted so that the most likely to be interesting
 * to connect oneself to are listed *last*, this is just a (little)
 * optimization, since we know we'll need to parse this list
 * to construct a Guile object, we reverse the order.
 *
 * Return value: list object containing @lw6p2p_entry_t objects
 */
lw6sys_list_t *
lw6p2p_node_get_entries (lw6sys_context_t * sys_context, lw6p2p_node_t * node, int skip_local)
{
  lw6sys_list_t *ret = NULL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_entries (sys_context, (_lw6p2p_node_t *) node, skip_local);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int
_lw6p2p_node_server_start (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t seq_0)
{
  int ret = 0;

  /*
   * OK, so now start just calls disconnect, what's wrong?
   * Nothing, actually, it's the fact that we use update function
   * to set the dynamic params that will make the server alive
   * else it's just a mere node, and "starting" is really equivalent
   * to "restarting" so we just clear everything and hell, it's
   * just ready to serve.
   */
  _lw6p2p_node_disconnect (sys_context, node);

  lw6nod_info_update (sys_context, node->node_info, lw6sys_generate_id_64 (sys_context),
		      0, NULL, node->node_info->const_info.bench / LW6P2P_BENCH_NETWORK_DIVIDE, 0, 0, 0, 0, 0, 0, NULL, 0, NULL);
  _lw6p2p_node_calibrate (sys_context, node, lw6sys_get_timestamp (sys_context), seq_0);
  lw6dat_warehouse_set_local_seq_0 (sys_context, node->warehouse, seq_0);

  ret = 1;

  return ret;
}

/**
 * lw6p2p_node_server_start
 *
 * @sys_context: global system context
 * @node: node to start
 * @seq_0: seq when starting the server
 *
 * Starts a node in server mode, if node was previously connected to other
 * nodes, disconnect it from any peer.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6p2p_node_server_start (lw6sys_context_t * sys_context, lw6p2p_node_t * node, int64_t seq_0)
{
  int ret = 0;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_server_start (sys_context, (_lw6p2p_node_t *) node, seq_0);
      _node_unlock (sys_context, node);
    }

  return ret;
}

/*
 * This function *must* be called in locked mode
 */
int
_lw6p2p_node_client_join (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url, lw6sys_progress_t * progress)
{
  int ret = 1;
  int i;
  _lw6p2p_tentacle_t *tentacle = NULL;
  u_int32_t ticket_sig = 0;
  char *msg_join = NULL;
  char *remote_id_str = NULL;
  int64_t now = 0LL;
  int64_t limit_timestamp = 0LL;

  _lw6p2p_node_disconnect (sys_context, node);

  now = lw6sys_get_timestamp (sys_context);

  remote_id_str = lw6sys_id_ltoa (sys_context, remote_id);
  if (remote_id_str)
    {
      if ((!lw6nod_info_community_has_id (sys_context, node->node_info, remote_id))
	  && (!lw6nod_info_community_has_url (sys_context, node->node_info, remote_url)))
	{
	  i = _lw6p2p_node_find_tentacle (sys_context, node, remote_id);
	  if (i >= 0 && i < LW6P2P_MAX_NB_TENTACLES)
	    {
	      tentacle = &(node->tentacles[i]);
	      if (lw6sys_str_is_same (sys_context, remote_url, tentacle->remote_url))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("keep connected to  %" LW6SYS_PRINTF_LL "x at \"%s\""), (long long) remote_id, remote_url);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("won't join %" LW6SYS_PRINTF_LL
				   "x at \"%s\", it conflicts with %"
				   LW6SYS_PRINTF_LL "x at \"%s\""),
			      (long long) remote_id, remote_url, (long long) tentacle->remote_id_int, tentacle->remote_url);
		  tentacle = NULL;
		  ret = 0;
		}
	    }
	  if (ret && !tentacle)
	    {
	      i = _lw6p2p_node_find_free_tentacle (sys_context, node);
	      if (i >= 0 && i < LW6P2P_MAX_NB_TENTACLES)
		{
		  tentacle = &(node->tentacles[i]);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no more free tentacles"));
		  ret = 0;
		}
	      if (ret && tentacle)
		{
		  /*
		   * Adding to community, the warehouse_register
		   * part will come later when messages are
		   * actually received and join is complete.
		   */
		  lw6nod_info_community_add (sys_context, node->node_info, remote_id, remote_url);
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trying to join  %" LW6SYS_PRINTF_LL "x at \"%s\""), (long long) remote_id, remote_url);
		  ret =
		    _lw6p2p_tentacle_init (sys_context, tentacle, &(node->backends),
					   node->listener, node->public_url,
					   remote_url, NULL, node->password, node->node_id_int, remote_id, node->network_reliability);
		  if (ret)
		    {
		      ret = 0;

		      limit_timestamp = now + node->db->data.consts.join_delay;
		      while (!tentacle->data_exchanged && lw6sys_get_timestamp (sys_context) < limit_timestamp)
			{
			  _lw6p2p_node_poll (sys_context, node, progress);
			  if (_lw6p2p_node_unlock (sys_context, node))
			    {
			      /*
			       * Used to be lw6sys_idle but was generating
			       * way too many calls, snooze is an order
			       * of magnitude more reasonnable.
			       */
			      lw6sys_snooze (sys_context);
			      _lw6p2p_node_lock (sys_context, node);
			    }
			}
		      /*
		       * Generating a JOIN message with 0 seq
		       * means "I want to connect to you"
		       */
		      msg_join = lw6msg_cmd_generate_join (sys_context, node->node_info, 0, lw6dat_warehouse_get_local_serial (sys_context, node->warehouse));
		      if (msg_join)
			{
			  ticket_sig =
			    lw6msg_ticket_calc_sig
			    (sys_context, lw6cnx_ticket_table_get_send (sys_context, &(node->ticket_table), remote_id_str), node->node_id_int, remote_id,
			     msg_join);
			  ret =
			    _lw6p2p_tentacle_send_redundant (sys_context, tentacle, now, &(node->ticket_table), ticket_sig, node->node_id_int, remote_id,
							     msg_join);
			  if (ret)
			    {
			      ret = 0;

			      limit_timestamp = now + node->db->data.consts.join_delay;
			      while (!tentacle->joined && lw6sys_get_timestamp (sys_context) < limit_timestamp)
				{
				  _lw6p2p_node_poll (sys_context, node, progress);
				  if (_lw6p2p_node_unlock (sys_context, node))
				    {
				      lw6sys_idle (sys_context);
				      _lw6p2p_node_lock (sys_context, node);
				    }
				}
			      if (tentacle->joined)
				{
				  ret = 1;
				}
			    }
			  LW6SYS_FREE (sys_context, msg_join);
			}
		    }
		}
	    }
	}
      else
	{
	  if (lw6nod_info_community_is_member (sys_context, node->node_info, remote_id, remote_url))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("it's useless to join %" LW6SYS_PRINTF_LL "x at \"%s\", it's already a community member"), (long long) remote_id, remote_url);
	    }
	  else
	    {

	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_ ("won't join %" LW6SYS_PRINTF_LL
			       "x at \"%s\", it conflicts with an existing community member"), (long long) remote_id, remote_url);
	      ret = 0;
	    }
	}
      LW6SYS_FREE (sys_context, remote_id_str);
    }

  return ret;
}

/**
 * lw6p2p_node_client_join
 *
 * @sys_context: global system context
 * @node: node to use
 * @remote_id: id of remote node to join
 * @remote_url: url of remote node to join
 * @progress: progress indicator to show end-user the advancement of process
 *
 * Starts a node in client mode, joins the given node,
 * if node was previously connected to other
 * nodes, disconnect it from any peer.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6p2p_node_client_join (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url, lw6sys_progress_t * progress)
{
  int ret = 0;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_client_join (sys_context, (_lw6p2p_node_t *) node, remote_id, remote_url, progress);
      _node_unlock (sys_context, node);
    }

  return ret;
}

/*
 * This function *must* be called in locked mode
 */
int
_lw6p2p_node_refresh_peer (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url)
{
  int ret = 1;
  char *url_from_id = NULL;

  url_from_id = lw6nod_info_community_get_url_from_id (sys_context, node->node_info, remote_id);
  if (url_from_id)
    {
      if (!lw6sys_str_is_same (sys_context, remote_url, url_from_id))
	{
	  _lw6p2p_explore_start_verify_node (sys_context, node, url_from_id);
	}
      LW6SYS_FREE (sys_context, url_from_id);
    }
  _lw6p2p_explore_start_verify_node (sys_context, node, remote_url);

  return ret;
}

/**
 * lw6p2p_node_refresh_peer
 *
 * @sys_context: global system context
 * @node: node to use
 * @remote_id: id of remote node to refresh
 * @remote_url: url of remote node to refresh
 *
 * Forces a refresh of a remote node, that is, try and get more up-to-date
 * informations from it, not waiting for the standard update schedule.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6p2p_node_refresh_peer (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t remote_id, const char *remote_url)
{
  int ret = 0;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_refresh_peer (sys_context, (_lw6p2p_node_t *) node, remote_id, remote_url);
      _node_unlock (sys_context, node);
    }

  return ret;
}

void
_lw6p2p_node_disconnect (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int i;

  /*
   * Here, a purge is not enough for it doesn't clear the nodes list,
   * and a clear only is too much for it looses the local_id
   */
  lw6dat_warehouse_clear (sys_context, node->warehouse);
  lw6dat_warehouse_init (sys_context, node->warehouse, node->node_id_int, _LW6P2P_NODE_DEFAULT_SEQ_0);

  for (i = 0; i < LW6P2P_MAX_NB_TENTACLES; ++i)
    {
      _lw6p2p_tentacle_clear (sys_context, &(node->tentacles[i]));
    }
  lw6nod_info_community_reset (sys_context, node->node_info);
  lw6nod_info_update (sys_context, node->node_info, LW6NOD_COMMUNITY_ID_NONE, 0, NULL, 0, 0, 0, 0, 0, 0, 0, NULL, 0, NULL);
  node->seed_needed = 0;
  node->dump_needed = 0;
  node->last_seq_reference = lw6dat_warehouse_get_seq_min (sys_context, node->warehouse) - 1;
  if (node->reference_msg)
    {
      lw6sys_list_free (sys_context, node->reference_msg);
      node->reference_msg = NULL;
    }
  if (node->draft_msg)
    {
      lw6sys_list_free (sys_context, node->draft_msg);
      node->draft_msg = NULL;
    }
}

/**
 * lw6p2p_node_disconnect
 *
 * @sys_context: global system context
 * @node: node to disconnect
 *
 * Disconnects a node, if node was previously connected to other
 * nodes, disconnect it from any peer. Note that this does not
 * affect out-of-band connections, only real game-related links.
 *
 * Return value: 1 on success, 0 on failure.
 */
void
lw6p2p_node_disconnect (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      _lw6p2p_node_disconnect (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }
}

int
_lw6p2p_node_update_info (lw6sys_context_t * sys_context, _lw6p2p_node_t * node,
			  int round, const char *level,
			  int nb_colors, int max_nb_colors,
			  int nb_cursors, int max_nb_cursors, int nb_nodes, int max_nb_nodes, int game_screenshot_size, void *game_screenshot_data)
{
  int ret = 0;

  ret =
    lw6nod_info_update (sys_context, node->node_info,
			node->node_info->dyn_info.community_id_int, round,
			level, node->node_info->dyn_info.required_bench,
			nb_colors, max_nb_colors, nb_cursors, max_nb_cursors,
			nb_nodes, max_nb_nodes, NULL, game_screenshot_size, game_screenshot_data)
    && _lw6p2p_node_update_local (sys_context, node, node->node_info);

  return ret;
}

/**
 * lw6p2p_node_update_info
 *
 * @sys_context: global system context
 * @node: node to update
 * @round: the current round (can have an offset with real round number)
 * @level: the name of the current level (map)
 * @nb_colors: number of colors playing
 * @max_nb_colors: max number of colors allowed
 * @nb_cursors: number of cursors playing
 * @max_nb_cursors: max number of cursors allowed
 * @nb_nodes: number of nodes playing
 * @max_nb_nodes: max number of nodes allowed
 * @game_screenshot_size: size of screenshot (bytes)
 * @game_screenshot_data: screenshot data (byte buffer, contains JPEG)
 *
 * Updates node info, that is, all the data/metadata which can be displayed
 * to other peers and are, by nature, dynamic.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6p2p_node_update_info (lw6sys_context_t * sys_context, lw6p2p_node_t * node,
			 int round, const char *level,
			 int nb_colors, int max_nb_colors,
			 int nb_cursors, int max_nb_cursors, int nb_nodes, int max_nb_nodes, int game_screenshot_size, void *game_screenshot_data)
{
  int ret = 0;
  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_update_info (sys_context, (_lw6p2p_node_t *) node,
				      round, level,
				      nb_colors, max_nb_colors,
				      nb_cursors, max_nb_cursors, nb_nodes, max_nb_nodes, game_screenshot_size, game_screenshot_data);
      _node_unlock (sys_context, node);
    }

  return ret;
}

void
_lw6p2p_node_calibrate (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, int64_t timestamp, int64_t seq)
{
  node->calibrate_timestamp = timestamp;
  node->calibrate_seq = seq;
}

/**
 * lw6p2p_node_calibrate
 *
 * @sys_context: global system context
 * @node: the object to calibrate
 * @timestamp: the current ticks setting (1000 ticks per second)
 * @seq: the round expected to be returned with this ticks value
 *
 * Calibrates the node, so that sequence numbering is consistent
 * accross nodes.
 *
 * Return value: none.
 */
void
lw6p2p_node_calibrate (lw6sys_context_t * sys_context, lw6p2p_node_t * node, int64_t timestamp, int64_t seq)
{
  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      _lw6p2p_node_calibrate (sys_context, (_lw6p2p_node_t *) node, timestamp, seq);
      _node_unlock (sys_context, node);
    }
}

int64_t
_lw6p2p_node_get_local_seq_0 (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  ret = lw6dat_warehouse_get_local_seq_0 (sys_context, node->warehouse);

  return ret;
}

/**
 * lw6p2p_node_get_local_seq_0
 *
 * @sys_context: global system context
 * @node: the object to query
 *
 * Gets the reference local seq_0 for this node, the information
 * is taken from the warehouse, even if node->calibrate_seq
 * should probably return the same value.
 *
 * Return value: the seq.
 */
int64_t
lw6p2p_node_get_local_seq_0 (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_local_seq_0 (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int64_t
_lw6p2p_node_get_local_seq_last (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  ret = lw6dat_warehouse_get_local_seq_last (sys_context, node->warehouse);

  return ret;
}

/**
 * lw6p2p_node_get_local_seq_last
 *
 * @sys_context: global system context
 * @node: the object to query
 *
 * Gets the local seq_last for this node, the information
 * is taken from the warehouse, which has parsed the messages
 * and this information can in return be used to avoid maintaining
 * outside of the node the information about what was the last
 * seq used for a local message.
 *
 * Return value: the seq.
 */
int64_t
lw6p2p_node_get_local_seq_last (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_local_seq_last (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int64_t
_lw6p2p_node_get_seq_min (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  ret = lw6sys_llmax (lw6dat_warehouse_get_seq_min (sys_context, node->warehouse), node->calibrate_seq);

  return ret;
}

/**
 * lw6p2p_node_get_seq_min
 *
 * @sys_context: global system context
 * @node: the object to query
 *
 * Gets the minimum seq registered, not of utmost importance but
 * interesting for debugging purpose, to check what's in the
 * warehouse.
 *
 * Return value: the seq.
 */
int64_t
lw6p2p_node_get_seq_min (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_seq_min (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int64_t
_lw6p2p_node_get_seq_max (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  ret = lw6sys_llmax (lw6dat_warehouse_get_seq_max (sys_context, node->warehouse), node->calibrate_seq);

  return ret;
}

/**
 * lw6p2p_node_get_seq_max
 *
 * @sys_context: global system context
 * @node: the object to query
 *
 * Gets the maximum seq registered, this is typically used for guessing
 * which seq might make sense for this node, but in a real example one
 * should rely on algorithm/ker-side kept values.
 *
 * Return value: the seq.
 */
int64_t
lw6p2p_node_get_seq_max (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_seq_max (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int64_t
_lw6p2p_node_get_seq_draft (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  ret = lw6sys_llmax (lw6dat_warehouse_get_seq_draft (sys_context, node->warehouse), node->calibrate_seq);

  return ret;
}

/**
 * lw6p2p_node_get_seq_draft
 *
 * @sys_context: global system context
 * @node: the object to query
 *
 * Gets the seq of the current draft as the warehouse understands it.
 * Note that it's the responsibility of the caller to update the pilot
 * according to this, this information is just about what is in the
 * warehouse, not necessarly what is in the pilot / game_state.
 *
 * Return value: the seq.
 */
int64_t
lw6p2p_node_get_seq_draft (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_seq_draft (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int64_t
_lw6p2p_node_get_seq_reference (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  ret = lw6sys_llmax (lw6dat_warehouse_get_seq_reference (sys_context, node->warehouse), node->calibrate_seq);

  return ret;
}

/**
 * lw6p2p_node_get_seq_reference
 *
 * @sys_context: global system context
 * @node: the object to query
 *
 * Gets the seq of the current reference as the warehouse understands it.
 * Note that it's the responsibility of the caller to update the pilot
 * according to this, this information is just about what is in the
 * warehouse, not necessarly what is in the pilot / game_state.
 *
 * Return value: the seq.
 */
int64_t
lw6p2p_node_get_seq_reference (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_seq_reference (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int
_lw6p2p_node_is_peer_connected (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t peer_id)
{
  int ret = 0;

  ret = (peer_id == node->node_id_int) || (_lw6p2p_node_find_tentacle (sys_context, node, peer_id) >= 0);

  return ret;
}

/**
 * lw6p2p_node_is_peer_connected
 *
 * @sys_context: global system context
 * @node: the object to query
 * @peer_id: id of the peer we want to check
 *
 * Tests wether the node is connected to us.
 * This is a different question from being registered, being connected
 * means there's a tentacle connected to the peer, but it does not
 * necessarly means this peer actively takes part in the game.
 *
 * Return value: 1 if connected, 0 if not.
 */
int
lw6p2p_node_is_peer_connected (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t peer_id)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_is_peer_connected (sys_context, (_lw6p2p_node_t *) node, peer_id);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int
_lw6p2p_node_is_peer_registered (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, u_int64_t peer_id)
{
  int ret = 0;

  ret = (peer_id == node->node_id_int) || lw6dat_warehouse_is_node_registered (sys_context, node->warehouse, peer_id);

  return ret;
}

/**
 * lw6p2p_node_is_peer_registered
 *
 * @sys_context: global system context
 * @node: the object to query
 * @peer_id: id of the peer we want to check
 *
 * Tests wether the node is registered within the warehouse.
 * This is a different question from being connected, being registered
 * means we received a message (possibly from another peer) that
 * means "this peer is part of the game" regardless of the fact
 * it's connected or not. Returns true if test is performed with
 * the local id.
 *
 * Return value: 1 if registered, 0 if not.
 */
int
lw6p2p_node_is_peer_registered (lw6sys_context_t * sys_context, lw6p2p_node_t * node, u_int64_t peer_id)
{
  int64_t ret = 0LL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_is_peer_registered (sys_context, (_lw6p2p_node_t *) node, peer_id);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int
_lw6p2p_node_is_seed_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;

  if (node->seed_needed)
    {
      ret = 1;

      node->seed_needed = 0;
    }

  return ret;
}

/**
 * lw6p2p_node_is_seed_needed
 *
 * @sys_context: global system context
 * @node: node to query
 *
 * Returns true (1) if the local node needs to send a SEED message.
 * A SEED message will basically be sent at the beginning of a session
 * when a client connects to a server. Basically this message is of
 * no use but it's interesting to have a short message (not DUMP) to
 * start with.
 *
 * Return value: 1 if SEED must be sent.
 */
int
lw6p2p_node_is_seed_needed (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int ret = 0;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_is_seed_needed (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int
_lw6p2p_node_is_dump_needed (lw6sys_context_t * sys_context, _lw6p2p_node_t * node)
{
  int ret = 0;

  if (node->dump_needed)
    {
      ret = 1;

      node->dump_needed = 0;
    }

  return ret;
}

/**
 * lw6p2p_node_is_dump_needed
 *
 * @sys_context: global system context
 * @node: node to query
 *
 * Returns true (1) if the local node needs to send a DUMP message.
 * A DUMP message will basically reset level, game struct, game state,
 * it's typically sent when a new player is connected. This function
 * will return true once then always 0 so one should really act and
 * do something whenever it's called and returns 1.
 *
 * Return value: 1 if DUMP must be sent.
 */
int
lw6p2p_node_is_dump_needed (lw6sys_context_t * sys_context, lw6p2p_node_t * node)
{
  int ret = 0;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_is_dump_needed (sys_context, (_lw6p2p_node_t *) node);
      _node_unlock (sys_context, node);
    }

  return ret;
}

int
_lw6p2p_node_put_local_msg (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, const char *msg)
{
  int ret = 0;

  if (node->warehouse)
    {
      ret = lw6dat_warehouse_put_local_msg (sys_context, node->warehouse, msg);
    }

  return ret;
}

/**
 * lw6p2p_node_put_local_msg
 *
 * @sys_context: global system context
 * @node: node object to use
 * @msg: message
 *
 * Puts a message in the object. The message will be splitted into
 * several atoms if needed, it can be arbitrary long.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6p2p_node_put_local_msg (lw6sys_context_t * sys_context, lw6p2p_node_t * node, const char *msg)
{
  int ret = 0;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_put_local_msg (sys_context, (_lw6p2p_node_t *) node, msg);
      _node_unlock (sys_context, node);
    }

  return ret;
}

char *
_lw6p2p_node_get_next_reference_msg (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6sys_progress_t * progress)
{
  char *ret = NULL;
  int64_t seq_reference = 0LL;

  if ((!node->reference_msg) && lw6dat_warehouse_calc_serial_draft_and_reference (sys_context, node->warehouse))
    {
      seq_reference = lw6dat_warehouse_get_seq_reference (sys_context, node->warehouse);
      if (seq_reference > node->last_seq_reference)
	{
	  node->reference_msg = lw6dat_warehouse_get_msg_list_by_seq (sys_context, node->warehouse, node->last_seq_reference + 1, seq_reference, 1, progress);
	  node->last_seq_reference = seq_reference;
	}
    }

  if (node->reference_msg)
    {
      ret = lw6sys_list_pop_front (sys_context, &(node->reference_msg));
    }

  return ret;
}

/**
 * lw6p2p_node_get_next_reference_msg
 *
 * @sys_context: global system context
 * @node: node to query
 * @progress: progress indicator (read/write)
 *
 * Get the next waiting reference msg. This is used to maintain
 * the stable reference game state we can rely upon. One is
 * supposed to call this until it returns NULL, then switch
 * draft messages.
 *
 * Return value: newly allocated string, must be freed.
 */
char *
lw6p2p_node_get_next_reference_msg (lw6sys_context_t * sys_context, lw6p2p_node_t * node, lw6sys_progress_t * progress)
{
  char *ret = NULL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_next_reference_msg (sys_context, (_lw6p2p_node_t *) node, progress);
      _node_unlock (sys_context, node);
    }

  return ret;
}

char *
_lw6p2p_node_get_next_draft_msg (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6sys_progress_t * progress)
{
  char *ret = NULL;
  int64_t seq_draft = 0LL;
  int64_t seq_min = 0LL;
  int64_t seq_max = 0LL;

  if ((!node->draft_msg))
    {
      if (lw6dat_warehouse_calc_serial_draft_and_reference (sys_context, node->warehouse))
	{
	  seq_draft = lw6dat_warehouse_get_seq_draft (sys_context, node->warehouse);
	  if (seq_draft > node->last_seq_reference)
	    {
	      node->draft_msg = lw6dat_warehouse_get_msg_list_by_seq (sys_context, node->warehouse, node->last_seq_reference + 1, seq_draft, 0, progress);
	    }
	}
      else
	{
	  /*
	   * Special case, we do not even have *one* full message in stock
	   * so we just get any seq available (the last one for that matter) and
	   * call blindly the get_msg function, not to really get the message
	   * (there is none) but to trigger the code that will update MISS ranges
	   */
	  seq_min = lw6dat_warehouse_get_seq_min (sys_context, node->warehouse);
	  seq_max = lw6dat_warehouse_get_seq_max (sys_context, node->warehouse);
	  if (seq_min <= seq_max)
	    {
	      node->draft_msg = lw6dat_warehouse_get_msg_list_by_seq (sys_context, node->warehouse, seq_min, seq_max, 0, progress);
	    }
	  if (node->draft_msg)
	    {
	      if (lw6sys_list_length (sys_context, node->draft_msg) > 0)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("strange, got messages when one could not figure out what seq_draft was"));
		}
	      lw6sys_list_free (sys_context, node->draft_msg);
	      node->draft_msg = NULL;
	    }
	}
    }
  if (node->draft_msg)
    {
      ret = lw6sys_list_pop_front (sys_context, &(node->draft_msg));
    }

  return ret;
}

/**
 * lw6p2p_node_get_next_draft_msg
 *
 * @sys_context: global system context
 * @node: node to query
 * @progress: progress indicator (read/write)
 *
 * Get the next waiting draft msg. This is used to maintain
 * the anticipated draft game state we use for drawing. One is
 * supposed to call this after all reference messages have been
 * treated.
 *
 * Return value: newly allocated string, must be freed.
 */
char *
lw6p2p_node_get_next_draft_msg (lw6sys_context_t * sys_context, lw6p2p_node_t * node, lw6sys_progress_t * progress)
{
  char *ret = NULL;

  /*
   * We lock in public function, the private one does not use
   * the lock, because it could be used in other functions
   * that are themselves locked...
   */
  if (_node_lock (sys_context, node))
    {
      ret = _lw6p2p_node_get_next_draft_msg (sys_context, (_lw6p2p_node_t *) node, progress);
      _node_unlock (sys_context, node);
    }

  return ret;
}
