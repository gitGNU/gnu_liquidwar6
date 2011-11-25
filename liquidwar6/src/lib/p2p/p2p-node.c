/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

static u_int32_t seq_id = 0;

/**
 * lw6p2p_node_new
 *
 * @argc: number of args, as passed to @main
 * @argv: args array, as passed to @main
 * @db: the database to use
 * @client_backends: the list of client backends to use
 * @server_backends: the list of server backends to use
 * @bind_ip: the IP address to bind on
 * @bind_port: the IP port to listen on
 * @broadcast: wether broadcast is allowed on this node
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
lw6p2p_node_new (int argc, char *argv[], lw6p2p_db_t * db,
		 char *client_backends, char *server_backends, char *bind_ip,
		 int bind_port, int broadcast,
		 char *public_url, char *title,
		 char *description, char *password, int bench, int open_relay,
		 char *known_nodes, int network_reliability, int trojan)
{
  return (lw6p2p_node_t *) _lw6p2p_node_new (argc, argv, (_lw6p2p_db_t *) db,
					     client_backends, server_backends,
					     bind_ip, bind_port, broadcast,
					     public_url,
					     title, description, password,
					     bench, open_relay, known_nodes,
					     network_reliability, trojan);
}

_lw6p2p_node_t *
_lw6p2p_node_new (int argc, char *argv[], _lw6p2p_db_t * db,
		  char *client_backends, char *server_backends, char *bind_ip,
		  int bind_port, int broadcast,
		  char *public_url, char *title,
		  char *description, char *password, int bench,
		  int open_relay, char *known_nodes, int network_reliability,
		  int trojan)
{
  _lw6p2p_node_t *node = NULL;
  char *query = NULL;
  char *escaped_public_url = NULL;
  char *escaped_title = NULL;
  char *escaped_description = NULL;
  int ret = 1;

  node = (_lw6p2p_node_t *) LW6SYS_CALLOC (sizeof (_lw6p2p_node_t));
  if (node)
    {
      node->id = 0;
      while (!node->id)
	{
	  node->id = ++seq_id;
	}
      node->closed = 0;
      node->db = db;
      node->bind_ip = lw6sys_str_copy (bind_ip);
      node->bind_port = bind_port;
      node->broadcast = broadcast;
      node->node_id_int = lw6sys_generate_id_64 ();
      node->node_id_str = lw6sys_id_ltoa (node->node_id_int);
      if (public_url && strlen (public_url) > 0)
	{
	  node->public_url = lw6sys_url_canonize (public_url);
	}
      else
	{
	  node->public_url = lw6net_if_guess_public_url (bind_ip, bind_port);
	}
      if (password)
	{
	  node->password = lw6sys_str_copy (password);
	}
      else
	{
	  node->password = lw6sys_str_copy ("");
	}
      node->node_info =
	lw6nod_info_new (lw6sys_build_get_package_tarname (),
			 lw6sys_build_get_version (),
			 lw6sys_build_get_codename (),
			 lw6sys_atoi (lw6sys_build_get_stamp ()),
			 node->node_id_int, node->public_url, title,
			 description, node->password, bench, open_relay, 0,
			 node->db->data.idle_screenshot.size,
			 node->db->data.idle_screenshot.data);
      if (known_nodes)
	{
	  node->known_nodes = lw6sys_str_copy (known_nodes);
	}
      else
	{
	  node->known_nodes = lw6sys_str_copy ("");
	}
      node->network_reliability = network_reliability;
      node->trojan = trojan;
      ret = (node->bind_ip && node->node_id_str && node->public_url
	     && node->password && node->node_info);
      if (ret)
	{
	  node->listener = lw6srv_start (node->bind_ip, node->bind_port);
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

      ret = ret
	&& _lw6p2p_backends_init_cli (argc, argv, &(node->backends),
				      client_backends);
      ret = ret
	&& _lw6p2p_backends_init_srv (argc, argv, &(node->backends),
				      server_backends, node->listener);

      if (ret)
	{
	  node->srv_oobs =
	    lw6sys_list_new ((lw6sys_free_func_t)
			     _lw6p2p_srv_oob_callback_data_free);
	  ret = (node->srv_oobs != NULL);
	}
      if (ret)
	{
	  node->cli_oobs =
	    lw6sys_list_new ((lw6sys_free_func_t)
			     _lw6p2p_cli_oob_callback_data_free);
	  ret = (node->cli_oobs != NULL);
	}
      if (ret)
	{
	  ret =
	    lw6cnx_ticket_table_init (&(node->ticket_table),
				      node->db->data.consts.
				      ticket_table_hash_size);
	}
    }

  if (node && ret && node->node_id_str && node->public_url && node->node_info
      && node->node_info->const_info.title
      && node->node_info->const_info.description && node->bind_ip)
    {
      ret = 0;
      escaped_public_url = lw6sys_escape_sql_value (node->public_url);
      if (escaped_public_url)
	{
	  escaped_title =
	    lw6sys_escape_sql_value (node->node_info->const_info.title);
	  if (escaped_title)
	    {
	      escaped_description =
		lw6sys_escape_sql_value (node->node_info->const_info.
					 description);
	      if (escaped_description)
		{
		  query =
		    lw6sys_new_sprintf (_lw6p2p_db_get_query
					(node->db,
					 _LW6P2P_INSERT_LOCAL_NODE_SQL),
					_lw6p2p_db_now (node->db),
					lw6sys_build_get_version (),
					lw6sys_build_get_codename (),
					lw6sys_atoi (lw6sys_build_get_stamp
						     ()), node->node_id_str,
					escaped_public_url, escaped_title,
					escaped_description,
					node->node_info->const_info.bench,
					node->node_info->const_info.
					open_relay, node->bind_ip,
					node->bind_port);
		  if (query)
		    {
		      if (_lw6p2p_db_lock (node->db))
			{
			  ret = _lw6p2p_db_exec_ignore_data (node->db, query);
			  _lw6p2p_db_unlock (node->db);
			}
		      LW6SYS_FREE (query);
		    }
		  LW6SYS_FREE (escaped_description);
		}
	      LW6SYS_FREE (escaped_title);
	    }
	  LW6SYS_FREE (escaped_public_url);
	}
    }

  if ((!ret) && node)
    {
      _lw6p2p_node_free (node);
      node = NULL;
    }

  if (node)
    {
      if (node->trojan)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("started node \"%s\" in trojan mode, expect problems"),
		      node->public_url);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE, _("started node \"%s\""),
		      node->public_url);
	}
    }

  return node;
}

/**
 * lw6p2p_node_free
 *
 * @node: the node to free
 *
 * Frees a node object, all network communications will be shut.
 *
 * Return value: none.
 */
void
lw6p2p_node_free (lw6p2p_node_t * node)
{
  _lw6p2p_node_free ((_lw6p2p_node_t *) node);
}

void
_lw6p2p_node_free (_lw6p2p_node_t * node)
{
  if (node)
    {
      _lw6p2p_node_close (node);
      lw6cnx_ticket_table_clear (&(node->ticket_table));
      if (node->cli_oobs)
	{
	  lw6sys_list_free (node->cli_oobs);
	}
      if (node->srv_oobs)
	{
	  lw6sys_list_free (node->srv_oobs);
	}
      if (node->known_nodes)
	{
	  LW6SYS_FREE (node->known_nodes);
	}
      if (node->node_info)
	{
	  lw6nod_info_free (node->node_info);
	}
      if (node->password)
	{
	  LW6SYS_FREE (node->password);
	}
      if (node->public_url)
	{
	  LW6SYS_FREE (node->public_url);
	}
      if (node->node_id_str)
	{
	  LW6SYS_FREE (node->node_id_str);
	}
      if (node->bind_ip)
	{
	  LW6SYS_FREE (node->bind_ip);
	}
      LW6SYS_FREE (node);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("trying to free NULL node"));
    }
}

/**
 * lw6p2p_node_repr
 *
 * @node: the node to work on
 *
 * Gives a readable representation of the node
 *
 * Return value: a dynamically allocated string
 */
char *
lw6p2p_node_repr (lw6p2p_node_t * node)
{
  return _lw6p2p_node_repr ((_lw6p2p_node_t *) node);
}

char *
_lw6p2p_node_repr (_lw6p2p_node_t * node)
{
  char *repr = NULL;

  if (node && node->id && node->bind_ip)
    {
      repr =
	lw6sys_new_sprintf (_x_ ("%u %s %s:%d %s"), node->id,
			    node->node_id_str, node->bind_ip,
			    node->bind_port, node->public_url);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't repr NULL or unitialized node"));
    }

  return repr;
}

/**
 * lw6p2p_node_poll
 *
 * @node: the node to poll
 *
 * Polls a p2p node. This must be called on a regular basis, else
 * network communication is stalled.
 *
 * Return value: 1 on success, 0 on error.
 */
int
lw6p2p_node_poll (lw6p2p_node_t * node)
{
  return _lw6p2p_node_poll ((_lw6p2p_node_t *) node);
}

static int
_poll_step1_accept_tcp (_lw6p2p_node_t * node)
{
  int ret = 1;
  char *ip = NULL;
  int port = 0;
  int sock;
  lw6srv_tcp_accepter_t *tcp_accepter = NULL;
  char *guessed_public_url = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("polling node TCP"));

  if (lw6net_socket_is_valid (node->listener->tcp_sock))
    {
      sock =
	lw6net_tcp_accept (&ip, &port, node->listener->tcp_sock,
			   node->db->data.consts.accept_delay);
      if (sock >= 0 && ip != NULL && port > 0)
	{
	  tcp_accepter = lw6srv_tcp_accepter_new (ip, port, sock);
	  if (tcp_accepter)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_ ("TCP connection from %s:%d"), ip, port);
	      lw6sys_list_push_front (&(node->listener->tcp_accepters),
				      tcp_accepter);

	      /*
	       * Now we register this peer as a potential server,
	       * it will be qualified as a real server (or not) later
	       */
	      guessed_public_url =
		lw6sys_url_http_from_ip_port (ip, LW6NET_DEFAULT_PORT);
	      if (guessed_public_url)
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_
			      ("discovered node \"%s\" from guessed url"),
			      guessed_public_url);
		  lw6nod_info_add_discovered_node (node->node_info,
						   guessed_public_url);
		  LW6SYS_FREE (guessed_public_url);
		}
	      if (node->bind_port != LW6NET_DEFAULT_PORT)
		{
		  guessed_public_url =
		    lw6sys_url_http_from_ip_port (ip, node->bind_port);
		  if (guessed_public_url)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("discovered node \"%s\" from guessed url (using non-standard port %d)"),
				  guessed_public_url, node->bind_port);
		      lw6nod_info_add_discovered_node (node->node_info,
						       guessed_public_url);
		      LW6SYS_FREE (guessed_public_url);
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
      LW6SYS_FREE (ip);
    }

  return ret;
}

static int
_poll_step2_recv_udp (_lw6p2p_node_t * node)
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

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("polling node UDP"));
  memset (buf, 0, LW6NET_UDP_MINIMAL_BUF_SIZE + 1);
  if (node->listener->udp_sock >= 0)
    {
      if (lw6net_udp_peek
	  (node->listener->udp_sock, buf, LW6NET_UDP_MINIMAL_BUF_SIZE, &ip1,
	   &port1))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("received data from %s:%d"),
		      ip1, port1);
	  line =
	    lw6net_recv_line_udp (node->listener->udp_sock, &ip2, &port2);
	  if (line)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_ ("UDP connection from %s:%d"), ip2, port2);
	      udp_buffer = lw6srv_udp_buffer_new (ip2, port2, line);
	      if (udp_buffer)
		{
		  lw6sys_list_push_front (&(node->listener->udp_buffers),
					  udp_buffer);
		  /*
		   * Now we register this peer as a potential server,
		   * it will be qualified as a real server (or not) later
		   */
		  guessed_public_url =
		    lw6sys_url_http_from_ip_port (ip2, LW6NET_DEFAULT_PORT);
		  if (guessed_public_url)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("discovered node \"%s\" from guessed url"),
				  guessed_public_url);
		      lw6nod_info_add_discovered_node (node->node_info,
						       guessed_public_url);
		      LW6SYS_FREE (guessed_public_url);
		    }
		  if (node->bind_port != LW6NET_DEFAULT_PORT)
		    {
		      guessed_public_url =
			lw6sys_url_http_from_ip_port (ip2, node->bind_port);
		      if (guessed_public_url)
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG,
				      _x_
				      ("discovered node \"%s\" from guessed url (using non-standard port %d)"),
				      guessed_public_url, node->bind_port);
			  lw6nod_info_add_discovered_node
			    (node->node_info, guessed_public_url);
			  LW6SYS_FREE (guessed_public_url);
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
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("udp data received from %s:%d, but it's not correct"),
			  ip1, port1);
	    }
	}
    }

  if (ip1)
    {
      LW6SYS_FREE (ip1);
    }
  if (ip2)
    {
      LW6SYS_FREE (ip2);
    }
  if (line)
    {
      LW6SYS_FREE (line);
    }

  return ret;
}

static int
_tcp_accepter_reply (void *func_data, void *data)
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

  lw6net_tcp_peek (tcp_accepter->sock, tcp_accepter->first_line,
		   LW6SRV_PROTOCOL_BUFFER_SIZE, 0);

  for (i = 0; i < node->backends.nb_srv_backends && ret; ++i)
    {
      analyse_tcp_ret =
	lw6srv_analyse_tcp (node->backends.srv_backends[i], tcp_accepter,
			    node->node_info, &remote_id, NULL);
      if (analyse_tcp_ret & LW6SRV_ANALYSE_DEAD)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("dead accepter, scheduling it for deletion"));
	  if (lw6net_socket_is_valid (tcp_accepter->sock))
	    {
	      lw6net_socket_close (tcp_accepter->sock);
	      tcp_accepter->sock = LW6NET_SOCKET_INVALID;
	    }
	  ret = 0;
	}
      else
	{
	  if (analyse_tcp_ret & LW6SRV_ANALYSE_UNDERSTANDABLE)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_
			  ("understood accepter, scheduling it for deletion"));
	      ret = 0;		// will be filtered
	      if (analyse_tcp_ret & LW6SRV_ANALYSE_OOB)
		{
		  srv_oob =
		    _lw6p2p_srv_oob_callback_data_new (node->backends.
						       srv_backends[i],
						       node->node_info,
						       tcp_accepter->
						       client_id.client_ip,
						       tcp_accepter->
						       client_id.client_port,
						       tcp_accepter->sock,
						       NULL);
		  if (srv_oob)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("process srv_oob (tcp)"));
		      srv_oob->srv_oob->thread =
			lw6sys_thread_create (_lw6p2p_srv_oob_callback, NULL,
					      srv_oob);
		      lw6sys_lifo_push (&(node->srv_oobs), srv_oob);
		    }
		}
	      else
		{
		  tentacle_index =
		    _lw6p2p_node_find_tentacle (node, remote_id);
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
			lw6srv_analyse_tcp (node->backends.srv_backends[i],
					    tcp_accepter, node->node_info,
					    &remote_id, &remote_url);
		      if (remote_id != 0 && remote_url != NULL)
			{
			  if (_lw6p2p_node_register_tentacle
			      (node, remote_url,
			       tcp_accepter->client_id.client_ip, remote_id))
			    {
			      tentacle_index =
				_lw6p2p_node_find_tentacle (node, remote_id);
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_DEBUG,
					  _x_
					  ("can't create tentacle with \"%s\""),
					  remote_url);
			    }
			  LW6SYS_FREE (remote_url);
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("wrong id/url"));
			}
		    }
		  if (tentacle_index >= 0
		      && tentacle_index < LW6P2P_MAX_NB_TENTACLES)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("feed srv (tcp)"));
		      lw6srv_feed_with_tcp (node->backends.srv_backends[i],
					    node->tentacles
					    [tentacle_index].srv_connections
					    [i], tcp_accepter);
		    }
		  else
		    {
		      lw6net_socket_close (tcp_accepter->sock);
		      tcp_accepter->sock = LW6NET_SOCKET_INVALID;
		    }
		}
	    }
	}
    }

  return ret;
}

static int
_udp_buffer_reply (void *func_data, void *data)
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
      analyse_udp_ret =
	lw6srv_analyse_udp (node->backends.srv_backends[i], udp_buffer,
			    node->node_info, &remote_id, NULL);
      if (analyse_udp_ret & LW6SRV_ANALYSE_DEAD)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("dead buffer, scheduling it for deletion"));
	  ret = 0;
	}
      else
	{
	  if (analyse_udp_ret & LW6SRV_ANALYSE_UNDERSTANDABLE)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_
			  ("understood buffer, scheduling it for deletion"));
	      ret = 0;		// will be filtered
	      if (analyse_udp_ret & LW6SRV_ANALYSE_OOB)
		{
		  srv_oob =
		    _lw6p2p_srv_oob_callback_data_new (node->backends.
						       srv_backends[i],
						       node->node_info,
						       udp_buffer->client_id.
						       client_ip,
						       udp_buffer->client_id.
						       client_port,
						       node->listener->
						       udp_sock,
						       udp_buffer->line);
		  if (srv_oob)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("process srv_oob (udp)"));
		      srv_oob->srv_oob->thread =
			lw6sys_thread_create (_lw6p2p_srv_oob_callback, NULL,
					      srv_oob);
		      lw6sys_lifo_push (&(node->srv_oobs), srv_oob);
		    }
		}
	      else
		{
		  tentacle_index =
		    _lw6p2p_node_find_tentacle (node, remote_id);
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
		      analyse_udp_ret =
			lw6srv_analyse_udp (node->backends.srv_backends[i],
					    udp_buffer, node->node_info,
					    &remote_id, &remote_url);
		      if (remote_id != 0 && remote_url != NULL)
			{
			  if (_lw6p2p_node_register_tentacle
			      (node, remote_url,
			       udp_buffer->client_id.client_ip, remote_id))
			    {
			      tentacle_index =
				_lw6p2p_node_find_tentacle (node, remote_id);
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_DEBUG,
					  _x_
					  ("can't create tentacle with \"%s\""),
					  remote_url);
			    }
			  LW6SYS_FREE (remote_url);
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("wrong id/url"));
			}
		    }
		  if (tentacle_index >= 0
		      && tentacle_index < LW6P2P_MAX_NB_TENTACLES)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("feed srv (udp)"));
		      lw6srv_feed_with_udp (node->backends.srv_backends[i],
					    node->tentacles
					    [tentacle_index].srv_connections
					    [i], udp_buffer);
		    }
		}
	    }
	}
    }

  return ret;
}

static int
_poll_step3_reply (_lw6p2p_node_t * node)
{
  int ret = 1;

  lw6sys_list_filter (&(node->listener->tcp_accepters), _tcp_accepter_reply,
		      node);
  lw6sys_list_filter (&(node->listener->udp_buffers), _udp_buffer_reply,
		      node);

  return ret;
}

static int
_cli_oob_filter (void *func_data, void *data)
{
  int ret = 0;
  _lw6p2p_cli_oob_callback_data_t *cli_oob =
    (_lw6p2p_cli_oob_callback_data_t *) data;

  ret = _lw6p2p_cli_oob_filter (cli_oob);

  return ret;
}

static int
_poll_step4_cli_oob (_lw6p2p_node_t * node)
{
  int ret = 1;

  lw6sys_list_filter (&(node->cli_oobs), _cli_oob_filter, NULL);

  return ret;
}

static int
_srv_oob_filter (void *func_data, void *data)
{
  int ret = 0;
  _lw6p2p_srv_oob_callback_data_t *srv_oob =
    (_lw6p2p_srv_oob_callback_data_t *) data;

  ret = _lw6p2p_srv_oob_filter (srv_oob);

  return ret;
}

static int
_poll_step5_srv_oob (_lw6p2p_node_t * node)
{
  int ret = 1;

  lw6sys_list_filter (&(node->srv_oobs), _srv_oob_filter, NULL);

  return ret;
}

static int
_poll_step6_explore_discover_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_explore_discover_nodes_if_needed (node);

  return ret;
}

static int
_poll_step7_flush_discovered_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_flush_discovered_nodes_if_needed (node);

  return ret;
}

static int
_poll_step8_explore_verify_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_explore_verify_nodes_if_needed (node);

  return ret;
}

static int
_poll_step9_flush_verified_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_flush_verified_nodes_if_needed (node);

  return ret;
}

static int
_poll_step10_poll_tentacles (_lw6p2p_node_t * node)
{
  int ret = 1;
  int i = 0;

  for (i = 0; i < LW6P2P_MAX_NB_TENTACLES; ++i)
    {
      if (_lw6p2p_tentacle_enabled (&(node->tentacles[i])))
	{
	  _lw6p2p_tentacle_poll (&(node->tentacles[i]), node->node_info,
				 &(node->ticket_table),
				 node->db->data.consts.foo_delay);
	}
    }

  return ret;
}

int
_lw6p2p_node_poll (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _poll_step1_accept_tcp (node) && ret;
  ret = _poll_step2_recv_udp (node) && ret;
  ret = _poll_step3_reply (node) && ret;
  ret = _poll_step4_cli_oob (node) && ret;
  ret = _poll_step5_srv_oob (node) && ret;
  ret = _poll_step6_explore_discover_nodes (node) && ret;
  ret = _poll_step7_flush_discovered_nodes (node) && ret;
  ret = _poll_step8_explore_verify_nodes (node) && ret;
  ret = _poll_step9_flush_verified_nodes (node) && ret;
  ret = _poll_step10_poll_tentacles (node) && ret;

  return ret;
}

/**
 * lw6p2p_node_close
 *
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
lw6p2p_node_close (lw6p2p_node_t * node)
{
  _lw6p2p_node_close ((_lw6p2p_node_t *) node);
}

void
_lw6p2p_node_close (_lw6p2p_node_t * node)
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

	  if (_lw6p2p_db_lock (node->db))
	    {
	      query =
		lw6sys_new_sprintf (_lw6p2p_db_get_query
				    (node->db,
				     _LW6P2P_DELETE_NODE_BY_ID_SQL),
				    node->node_id_str);
	      if (query)
		{
		  _lw6p2p_db_exec_ignore_data (node->db, query);
		  LW6SYS_FREE (query);
		}
	      /*
	       * OK this is paranoid, we already deleted it by ID but...
	       * who knows, well, never be too sure ;)
	       */
	      query =
		lw6sys_new_sprintf (_lw6p2p_db_get_query
				    (node->db,
				     _LW6P2P_DELETE_NODE_BY_URL_SQL),
				    node->public_url);
	      if (query)
		{
		  _lw6p2p_db_exec_ignore_data (node->db, query);
		  LW6SYS_FREE (query);
		}
	      _lw6p2p_db_unlock (node->db);
	    }

	  for (i = 0; i < LW6P2P_MAX_NB_TENTACLES; ++i)
	    {
	      _lw6p2p_tentacle_clear (&(node->tentacles[i]));
	    }

	  if (node->srv_oobs)
	    {
	      while (node->srv_oobs && !lw6sys_list_is_empty (node->srv_oobs))
		{
		  srv_oob =
		    (_lw6p2p_srv_oob_callback_data_t *)
		    lw6sys_list_pop_front (&(node->srv_oobs));
		  _lw6p2p_srv_oob_callback_data_free (srv_oob);
		}
	      if (!(node->srv_oobs))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("NULL srv_oobs after emptying"));
		}
	    }
	  if (node->cli_oobs)
	    {
	      while (node->cli_oobs && !lw6sys_list_is_empty (node->cli_oobs))
		{
		  cli_oob =
		    (_lw6p2p_cli_oob_callback_data_t *)
		    lw6sys_list_pop_front (&(node->cli_oobs));
		  _lw6p2p_cli_oob_callback_data_free (cli_oob);
		}
	      if (!(node->cli_oobs))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("NULL cli_oobs after emptying"));
		}
	    }

	  _lw6p2p_backends_clear_srv (&(node->backends));
	  _lw6p2p_backends_clear_cli (&(node->backends));

	  if (node->listener)
	    {
	      lw6srv_stop (node->listener);
	      node->listener = NULL;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("closing closed node"));
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("trying to close NULL node"));
    }
}

/**
 * lw6p2p_node_get_id
 *
 * @node: the node to query
 *
 * Returns the node id, an id which is supposed to uniquely identify
 * the node at run-time.
 *
 * Return value: numerical id.
 */
u_int64_t
lw6p2p_node_get_id (lw6p2p_node_t * node)
{
  u_int64_t id = 0;

  id = _lw6p2p_node_get_id ((_lw6p2p_node_t *) node);

  return id;
}

u_int64_t
_lw6p2p_node_get_id (_lw6p2p_node_t * node)
{
  u_int64_t id = 0;

  id = node->node_id_int;

  return id;
}

static int
_select_node_by_url_callback (void *func_data, int nb_fields,
			      char **fields_values, char **fields_names)
{
  int ret = 0;
  int *count = (int *) func_data;

  (*count)++;
  lw6sys_log (LW6SYS_LOG_DEBUG,
	      _x_
	      ("select_node_by_url_callback called with %d fields, count is %d"),
	      nb_fields, (*count));

  return ret;
}

int
_lw6p2p_node_insert_discovered (_lw6p2p_node_t * node, char *public_url)
{
  int ret = 1;
  char *query = NULL;
  int count = 0;
  char *canonized_public_url;
  char *escaped_public_url;

  canonized_public_url = lw6sys_url_canonize (public_url);
  if (canonized_public_url)
    {
      escaped_public_url = lw6sys_escape_sql_value (canonized_public_url);
      if (escaped_public_url)
	{
	  if (_lw6p2p_db_lock (node->db))
	    {
	      query = lw6sys_new_sprintf (_lw6p2p_db_get_query
					  (node->db,
					   _LW6P2P_SELECT_NODE_BY_URL_SQL),
					  escaped_public_url);
	      if (query)
		{
		  ret =
		    _lw6p2p_db_exec (node->db, query,
				     _select_node_by_url_callback,
				     (void *) &count) && ret;
		  LW6SYS_FREE (query);
		  if (count > 0)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("there's already a node with url \"%s\""),
				  public_url);
		    }
		  else
		    {
		      query =
			lw6sys_new_sprintf (_lw6p2p_db_get_query
					    (node->db,
					     _LW6P2P_INSERT_DISCOVERED_NODE_SQL),
					    escaped_public_url,
					    _lw6p2p_db_now (node->db));
		      if (query)
			{
			  ret = _lw6p2p_db_exec_ignore_data (node->db, query)
			    && ret;
			  LW6SYS_FREE (query);
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
	      _lw6p2p_db_unlock (node->db);
	    }
	  LW6SYS_FREE (escaped_public_url);
	}
      LW6SYS_FREE (canonized_public_url);
    }

  return ret;
}

int
_lw6p2p_node_update_peer (_lw6p2p_node_t * node, char *version,
			  char *codename, int stamp, char *id, char *url,
			  char *title, char *description, int has_password,
			  int bench, int open_relay, int creation_timestamp,
			  char *community_id, int round, char *level,
			  int required_bench, int nb_colors,
			  int max_nb_colors, int nb_cursors,
			  int max_nb_cursors, int nb_nodes, int max_nb_nodes,
			  char *ip, int port, int last_ping_timestamp,
			  int ping_delay_msec)
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

  escaped_version = lw6sys_escape_sql_value (version);
  escaped_codename = lw6sys_escape_sql_value (codename);
  escaped_id = lw6sys_escape_sql_value (id);
  escaped_url = lw6sys_escape_sql_value (url);
  escaped_title = lw6sys_escape_sql_value (title);
  escaped_description = lw6sys_escape_sql_value (description);
  escaped_community_id = lw6sys_escape_sql_value (community_id);
  escaped_level = lw6sys_escape_sql_value (level);
  // todo : test for bench to set available the right way
  available = ((!lw6sys_str_is_null_or_empty (id))
	       &&
	       (lw6sys_str_is_same
		(version, node->node_info->const_info.version))
	       && (required_bench <= node->node_info->const_info.bench)
	       && (!lw6sys_str_is_same (id, node->node_id_str))
	       && (!lw6sys_str_is_null_or_empty (level))
	       && (nb_colors < max_nb_colors) && (nb_cursors < max_nb_cursors)
	       && (nb_nodes < max_nb_nodes)) ? 1 : 0;

  query = lw6sys_new_sprintf (_lw6p2p_db_get_query
			      (node->db,
			       _LW6P2P_UPDATE_NODE_SQL),
			      creation_timestamp, escaped_version,
			      escaped_codename, stamp,
			      escaped_id, escaped_title,
			      escaped_description, has_password,
			      bench, open_relay, escaped_community_id, round,
			      escaped_level, required_bench, nb_colors,
			      max_nb_colors, nb_cursors, max_nb_cursors,
			      nb_nodes, max_nb_nodes, ip, port,
			      last_ping_timestamp, ping_delay_msec, available,
			      escaped_url);
  if (query)
    {
      if (_lw6p2p_db_lock (node->db))
	{
	  ret = _lw6p2p_db_exec_ignore_data (node->db, query);
	  _lw6p2p_db_unlock (node->db);
	}
      LW6SYS_FREE (query);
    }

  if (escaped_version)
    {
      LW6SYS_FREE (escaped_version);
    }
  if (escaped_codename)
    {
      LW6SYS_FREE (escaped_codename);
    }
  if (escaped_id)
    {
      LW6SYS_FREE (escaped_id);
    }
  if (escaped_url)
    {
      LW6SYS_FREE (escaped_url);
    }
  if (escaped_title)
    {
      LW6SYS_FREE (escaped_title);
    }
  if (escaped_description)
    {
      LW6SYS_FREE (escaped_description);
    }
  if (escaped_community_id)
    {
      LW6SYS_FREE (escaped_community_id);
    }
  if (escaped_level)
    {
      LW6SYS_FREE (escaped_level);
    }

  return ret;
}

int
_lw6p2p_node_find_free_tentacle (_lw6p2p_node_t * node)
{
  int ret = -1;
  int i;

  for (i = 0; i < LW6P2P_MAX_NB_TENTACLES && ret < 0; ++i)
    {
      if (!_lw6p2p_tentacle_enabled (&(node->tentacles[i])))
	{
	  ret = i;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("found free tentacle %d"), i);
	}
    }

  return ret;
}

int
_lw6p2p_node_find_tentacle (_lw6p2p_node_t * node, u_int64_t remote_id)
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
_lw6p2p_node_register_tentacle (_lw6p2p_node_t * node, char *remote_url,
				char *real_remote_ip, u_int64_t remote_id)
{
  int ret = 0;
  int i = 0;

  i = _lw6p2p_node_find_tentacle (node, remote_id);
  if (i < 0)
    {
      i = _lw6p2p_node_find_free_tentacle (node);
      if (i >= 0 && i < LW6P2P_MAX_NB_TENTACLES)
	{
	  ret =
	    _lw6p2p_tentacle_init (&(node->tentacles[i]), &(node->backends),
				   node->listener,
				   node->public_url, remote_url,
				   real_remote_ip, node->password,
				   node->node_id_int, remote_id,
				   node->network_reliability,
				   _lw6p2p_recv_callback, (void *) node);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("unable to find a free tentacle"));
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("tentacle already exists (i=%d)"),
		  i);
    }

  return ret;
}

int
_lw6p2p_node_unregister_tentacle (_lw6p2p_node_t * node, u_int64_t remote_id)
{
  int ret = 0;
  int i = 0;

  i = _lw6p2p_node_find_tentacle (node, remote_id);
  if (i >= 0 && i < LW6P2P_MAX_NB_TENTACLES)
    {
      _lw6p2p_tentacle_clear (&(node->tentacles[i]));
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to find tentacle"));
    }

  return ret;
}

int
_get_entries_callback (void *func_data, int nb_fields,
		       char **fields_values, char **fields_names)
{
  int ret = 0;
  lw6sys_list_t **list = (lw6sys_list_t **) func_data;
  lw6p2p_entry_t *entry = NULL;

  if (nb_fields == _LW6P2P_DB_NODE_NB_FIELDS)
    {
      entry =
	lw6p2p_entry_new (lw6sys_atoi
			  (fields_values
			   [_LW6P2P_DB_NODE_ORDER_CREATION_TIMESTAMP]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_VERSION],
			  fields_values[_LW6P2P_DB_NODE_ORDER_CODENAME],
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_STAMP]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_ID],
			  fields_values[_LW6P2P_DB_NODE_ORDER_URL],
			  fields_values[_LW6P2P_DB_NODE_ORDER_TITLE],
			  fields_values[_LW6P2P_DB_NODE_ORDER_DESCRIPTION],
			  lw6sys_atob (fields_values
				       [_LW6P2P_DB_NODE_ORDER_HAS_PASSWORD]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_BENCH]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_OPEN_RELAY]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_COMMUNITY_ID],
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_ROUND]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_LEVEL],
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_REQUIRED_BENCH]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_NB_COLORS]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_MAX_NB_COLORS]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_NB_CURSORS]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_MAX_NB_CURSORS]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_NB_NODES]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_MAX_NB_NODES]),
			  fields_values[_LW6P2P_DB_NODE_ORDER_IP],
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_PORT]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_LAST_PING_TIMESTAMP]),
			  lw6sys_atoi (fields_values
				       [_LW6P2P_DB_NODE_ORDER_PING_DELAY_MSEC]),
			  lw6sys_atob (fields_values
				       [_LW6P2P_DB_NODE_ORDER_AVAILABLE]));
      if (entry)
	{
	  lw6sys_list_push_front (list, entry);
	}
    }

  return ret;
}

lw6sys_list_t *
_lw6p2p_node_get_entries (_lw6p2p_node_t * node)
{
  lw6sys_list_t *ret;
  char *query = NULL;

  ret = lw6sys_list_new ((lw6sys_free_func_t) lw6p2p_entry_free);
  if (ret)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("flush verified nodes"));

      /*
       * We lock the whole session, to avoid double listing
       * something, or forgetting something if things change
       * between the two calls
       */
      if (_lw6p2p_db_lock (node->db))
	{
	  query = lw6sys_new_sprintf (_lw6p2p_db_get_query
				      (node->db,
				       _LW6P2P_SELECT_UNAVAILABLE_NODE_SQL));
	  if (query)
	    {
	      _lw6p2p_db_exec (node->db, query, _get_entries_callback, &ret);
	      _lw6p2p_db_unlock (node->db);
	    }
	  LW6SYS_FREE (query);

	  query = lw6sys_new_sprintf (_lw6p2p_db_get_query
				      (node->db,
				       _LW6P2P_SELECT_AVAILABLE_NODE_SQL));
	  if (query)
	    {
	      if (_lw6p2p_db_lock (node->db))
		{
		  _lw6p2p_db_exec (node->db, query,
				   _get_entries_callback, &ret);
		}
	      LW6SYS_FREE (query);
	    }
	  _lw6p2p_db_unlock (node->db);
	}
    }

  return ret;
}

/**
 * lw6p2p_node_get_entries
 *
 * @node: node to query
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
lw6p2p_node_get_entries (lw6p2p_node_t * node)
{
  return _lw6p2p_node_get_entries ((_lw6p2p_node_t *) node);
}
