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
 * @node_id: the server unique ID
 * @public_url: the public URL we want to show
 * @password: the password to use
 * @title: the title of the node
 * @description: the description of the node
 * @bench: the bench of the node (its power)
 * @known_nodes: list of already known nodes
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
		 int bind_port, u_int64_t node_id, char *public_url,
		 char *password, char *title, char *description, int bench,
		 char *known_nodes)
{
  return (lw6p2p_node_t *) _lw6p2p_node_new (argc, argv, (_lw6p2p_db_t *) db,
					     client_backends, server_backends,
					     bind_ip, bind_port, node_id,
					     public_url, password, title,
					     description, bench, known_nodes);
}

_lw6p2p_node_t *
_lw6p2p_node_new (int argc, char *argv[], _lw6p2p_db_t * db,
		  char *client_backends, char *server_backends, char *bind_ip,
		  int bind_port, u_int64_t node_id, char *public_url,
		  char *password, char *title, char *description, int bench,
		  char *known_nodes)
{
  _lw6p2p_node_t *node = NULL;
  lw6sys_list_t *list_backends = NULL;
  char *backend = NULL;
  char *query = NULL;
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
      node->node_id_int = node_id;
      node->node_id_str = lw6sys_id_ltoa (node_id);
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
	lw6nod_info_new (node->node_id_int, node->public_url, title,
			 description, bench,
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
      if (ret)
	{
	  ret = 0;
	  list_backends = lw6sys_str_split_config_item (client_backends);
	  if (list_backends)
	    {
	      while (list_backends
		     && ((backend = lw6sys_list_pop_front (&list_backends)) !=
			 NULL))
		{
		  node->nb_cli_backends++;
		  if (node->cli_backends)
		    {
		      node->cli_backends =
			(lw6cli_backend_t **)
			LW6SYS_REALLOC (node->cli_backends,
					node->nb_cli_backends *
					sizeof (lw6cli_backend_t *));
		    }
		  else
		    {
		      node->cli_backends =
			(lw6cli_backend_t **)
			LW6SYS_MALLOC (node->nb_cli_backends *
				       sizeof (lw6cli_backend_t *));
		    }
		  if (node->cli_backends)
		    {
		      node->cli_backends[node->nb_cli_backends - 1] =
			lw6cli_create_backend (argc, argv, backend);
		      if (node->cli_backends[node->nb_cli_backends - 1])
			{
			  ret = 1;
			}
		      else
			{
			  node->cli_backends--;
			}
		    }
		  else
		    {
		      node->cli_backends--;
		    }
		  LW6SYS_FREE (backend);
		}
	      list_backends = NULL;
	    }
	}
      if (ret)
	{
	  ret = 0;
	  list_backends = lw6sys_str_split_config_item (server_backends);
	  if (list_backends)
	    {
	      while (list_backends
		     && ((backend = lw6sys_list_pop_front (&list_backends)) !=
			 NULL))
		{
		  node->nb_srv_backends++;
		  if (node->srv_backends)
		    {
		      node->srv_backends =
			(lw6srv_backend_t **)
			LW6SYS_REALLOC (node->srv_backends,
					node->nb_srv_backends *
					sizeof (lw6srv_backend_t *));
		    }
		  else
		    {
		      node->srv_backends =
			(lw6srv_backend_t **)
			LW6SYS_MALLOC (node->nb_srv_backends *
				       sizeof (lw6srv_backend_t *));
		    }
		  if (node->srv_backends)
		    {
		      node->srv_backends[node->nb_srv_backends - 1] =
			lw6srv_create_backend (argc, argv, backend);
		      if (node->srv_backends[node->nb_srv_backends - 1])
			{
			  if (lw6srv_init
			      (node->srv_backends[node->nb_srv_backends - 1],
			       node->listener))
			    {
			      /*
			       * OK, at this state, we have at
			       * least one server backend working,
			       * we consider it's fine.
			       */
			      ret = 1;
			    }
			  else
			    {
			      lw6srv_destroy_backend (node->srv_backends
						      [node->nb_srv_backends -
						       1]);
			      node->srv_backends[node->nb_srv_backends - 1] =
				NULL;
			      node->srv_backends--;
			    }
			}
		      else
			{
			  node->srv_backends--;
			}
		    }
		  else
		    {
		      node->srv_backends--;
		    }
		  LW6SYS_FREE (backend);
		}
	      list_backends = NULL;
	    }
	}

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
    }

  if (node && ret && node->node_id_str && node->public_url && node->node_info
      && node->node_info->const_info.title
      && node->node_info->const_info.description && node->bind_ip)
    {
      ret = 0;
      query =
	lw6sys_new_sprintf (_lw6p2p_db_get_query
			    (node->db,
			     _LW6P2P_INSERT_LOCAL_NODE_SQL),
			    (int) lw6sys_get_timestamp () / 1000,
			    node->node_id_str,
			    node->public_url,
			    node->node_info->const_info.title,
			    node->node_info->const_info.description,
			    node->node_info->const_info.bench,
			    node->bind_ip, node->bind_port, _LW6P2P_DB_TRUE);
      if (query)
	{
	  if (_lw6p2p_db_lock (node->db))
	    {
	      ret = _lw6p2p_db_exec_ignore_data (node->db, query);
	      _lw6p2p_db_unlock (node->db);
	    }
	  LW6SYS_FREE (query);
	}
    }

  if ((!ret) && node)
    {
      _lw6p2p_node_free (node);
      node = NULL;
    }

  if (node)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _("started node \"%s\""),
		  node->public_url);
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
      lw6sys_log (LW6SYS_LOG_WARNING, _("trying to free NULL node"));
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
	lw6sys_new_sprintf (_("%u %s %s:%d %s"), node->id,
			    node->node_id_str, node->bind_ip,
			    node->bind_port, node->public_url);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("can't repr NULL or unitialized node"));
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
_poll_step1_accept (_lw6p2p_node_t * node)
{
  int ret = 1;
  char *ip = NULL;
  int port = 0;
  int sock;
  lw6srv_tcp_accepter_t *tcp_accepter = NULL;
  char *guessed_public_url = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("polling node"));

  if (lw6srv_poll (node->listener))
    {
      if (node->listener->tcp_sock >= 0)
	{
	  sock =
	    lw6net_tcp_accept (&ip, &port, node->listener->tcp_sock,
			       node->db->data.consts.accept_delay / 1000.0f);
	  if (sock >= 0 && ip != NULL && port > 0)
	    {
	      tcp_accepter = lw6srv_tcp_accepter_new (ip, port, sock);
	      if (tcp_accepter)
		{
		  //lw6sys_log (LW6SYS_LOG_DEBUG, _("connection from %s:%d"),ip,port);
		  lw6sys_log (LW6SYS_LOG_INFO, _("connection from %s:%d"),
			      ip, port);
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
				  _
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
				      _
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
    }

  if (ip)
    {
      LW6SYS_FREE (ip);
    }

  return ret;
}

static int
_tcp_accepter_reply (void *func_data, void *data)
{
  int ret = 1;
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  lw6srv_tcp_accepter_t *tcp_accepter = (lw6srv_tcp_accepter_t *) data;
  lw6srv_connection_t *connection = NULL;
  _lw6p2p_srv_oob_callback_data_t *srv_oob = NULL;
  char *query = NULL;
  char *connection_ptr_str = NULL;
  char *backend_ptr_str = NULL;
  int i = 0;
  int analyse_tcp_ret = 0;

  lw6net_tcp_peek (tcp_accepter->sock, tcp_accepter->first_line,
		   LW6SRV_PROTOCOL_BUFFER_SIZE, 0.0f);

  for (i = 0; i < node->nb_srv_backends && ret; ++i)
    {
      analyse_tcp_ret =
	lw6srv_analyse_tcp (node->srv_backends[i], tcp_accepter);
      if (analyse_tcp_ret & LW6SRV_ANALYSE_DEAD)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("dead accepter, scheduling it for deletion"));
	  ret = 0;
	}
      else
	{
	  if (analyse_tcp_ret & LW6SRV_ANALYSE_UNDERSTANDABLE)
	    {
	      if (analyse_tcp_ret & LW6SRV_ANALYSE_OOB)
		{
		  srv_oob =
		    _lw6p2p_srv_oob_callback_data_new (node->srv_backends[i],
						       node->node_info,
						       tcp_accepter->
						       client_id.client_ip,
						       tcp_accepter->
						       client_id.client_port,
						       tcp_accepter->sock);
		  if (srv_oob)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG, _("process srv_oob"));
		      srv_oob->srv_oob->thread =
			lw6sys_thread_create (_lw6p2p_srv_oob_callback, NULL,
					      srv_oob);
		      lw6sys_lifo_push (&(node->srv_oobs), srv_oob);
		      ret = 0;
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG, _("new tcp"));
		  connection =
		    lw6srv_accept_tcp (node->srv_backends[i], tcp_accepter,
				       node->password);
		  if (connection)
		    {
		      connection_ptr_str =
			lw6sys_hexa_ptr_to_str (connection);
		      if (connection_ptr_str)
			{
			  backend_ptr_str =
			    lw6sys_hexa_ptr_to_str (node->srv_backends[i]);
			  if (backend_ptr_str)
			    {
			      query = lw6sys_new_sprintf (_lw6p2p_db_get_query
							  (node->db,
							   _LW6P2P_INSERT_CONNECTION_SQL),
							  connection_ptr_str,
							  backend_ptr_str,
							  node->node_id_str);
			      if (query)
				{
				  if (_lw6p2p_db_lock (node->db))
				    {
				      _lw6p2p_db_exec_ignore_data (node->db,
								   query);
				      _lw6p2p_db_unlock (node->db);
				    }
				  LW6SYS_FREE (query);
				}
			      LW6SYS_FREE (backend_ptr_str);
			    }
			  LW6SYS_FREE (connection_ptr_str);
			}
		    }
		}
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _
			  ("understood accepter, scheduling it for deletion"));
	      ret = 0;		// will be filtered
	    }
	}
    }

  return ret;
}

static int
_poll_step2_reply (_lw6p2p_node_t * node)
{
  int ret = 1;

  lw6sys_list_filter (&(node->listener->tcp_accepters), _tcp_accepter_reply,
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
_poll_step3_cli_oob (_lw6p2p_node_t * node)
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
_poll_step4_srv_oob (_lw6p2p_node_t * node)
{
  int ret = 1;

  lw6sys_list_filter (&(node->srv_oobs), _srv_oob_filter, NULL);

  return ret;
}

static int
_poll_step5_explore_discover_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_explore_discover_nodes_if_needed (node);

  return ret;
}

static int
_poll_step6_flush_discovered_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_flush_discovered_nodes_if_needed (node);

  return ret;
}

static int
_poll_step7_explore_verify_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_explore_verify_nodes_if_needed (node);

  return ret;
}

static int
_poll_step8_flush_verified_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _lw6p2p_flush_verified_nodes_if_needed (node);

  return ret;
}

int
_lw6p2p_node_poll (_lw6p2p_node_t * node)
{
  int ret = 0;

  ret = _poll_step1_accept (node) && ret;
  ret = _poll_step2_reply (node) && ret;
  ret = _poll_step3_cli_oob (node) && ret;
  ret = _poll_step4_srv_oob (node) && ret;
  ret = _poll_step5_explore_discover_nodes (node) && ret;
  ret = _poll_step6_flush_discovered_nodes (node) && ret;
  ret = _poll_step7_explore_verify_nodes (node) && ret;
  ret = _poll_step8_flush_verified_nodes (node) && ret;

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
  int i = 0;
  char *query = NULL;
  _lw6p2p_srv_oob_callback_data_t *srv_oob = NULL;
  _lw6p2p_cli_oob_callback_data_t *cli_oob = NULL;

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

	  query =
	    lw6sys_new_sprintf (_lw6p2p_db_get_query
				(node->db,
				 _LW6P2P_DELETE_NODE_BY_ID_SQL),
				node->node_id_str);
	  if (query)
	    {
	      if (_lw6p2p_db_lock (node->db))
		{
		  _lw6p2p_db_exec_ignore_data (node->db, query);
		  _lw6p2p_db_unlock (node->db);
		}
	      LW6SYS_FREE (query);
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
			      _("NULL srv_oobs after emptying"));
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
			      _("NULL cli_oobs after emptying"));
		}
	    }

	  if (node->srv_backends)
	    {
	      for (i = 0; i < node->nb_srv_backends; ++i)
		{
		  if (node->srv_backends[i])
		    {
		      lw6srv_quit (node->srv_backends[i]);
		      lw6srv_destroy_backend (node->srv_backends[i]);
		      node->srv_backends[i] = NULL;
		    }
		}
	      LW6SYS_FREE (node->srv_backends);
	      node->srv_backends = NULL;
	      node->nb_srv_backends = 0;
	    }
	  if (node->cli_backends)
	    {
	      for (i = 0; i < node->nb_cli_backends; ++i)
		{
		  if (node->cli_backends[i])
		    {
		      lw6cli_destroy_backend (node->cli_backends[i]);
		      node->cli_backends[i] = NULL;
		    }
		}
	      LW6SYS_FREE (node->cli_backends);
	      node->cli_backends = NULL;
	      node->nb_cli_backends = 0;
	    }
	  if (node->listener)
	    {
	      lw6srv_stop (node->listener);
	      node->listener = NULL;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("closing closed node"));
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("trying to close NULL node"));
    }
}

static int
_select_node_by_url_callback (void *func_data, int nb_fields,
			      char **fields_values, char **fields_names)
{
  int ret = 0;
  int *count = (int *) func_data;

  (*count)++;
  lw6sys_log (LW6SYS_LOG_DEBUG,
	      _
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

  if (_lw6p2p_db_lock (node->db))
    {
      query = lw6sys_new_sprintf (_lw6p2p_db_get_query
				  (node->db,
				   _LW6P2P_SELECT_NODE_BY_URL_SQL),
				  public_url);
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
			  _("there's already a node with url \"%s\""),
			  public_url);
	    }
	  else
	    {
	      query =
		lw6sys_new_sprintf (_lw6p2p_db_get_query
				    (node->db,
				     _LW6P2P_INSERT_DISCOVERED_NODE_SQL),
				    public_url);
	      if (query)
		{
		  ret = _lw6p2p_db_exec_ignore_data (node->db, query) && ret;
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

  return ret;
}
