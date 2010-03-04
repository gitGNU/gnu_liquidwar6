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
 * @client_backends: the list of client backends to use
 * @server_backends: the list of server backends to use
 * @bind_ip: the IP address to bind on
 * @bind_port: the IP port to listen on
 * @server_id: the server unique ID
 * @public_url: the public URL we want to show
 *
 * Creates a new "pear to pear" node. This will fire the server
 * and allow client access, on demand. A lot of stuff can be done
 * in the background once this is called.
 *
 * Return value: a pointer on the newly created objects.
 */
lw6p2p_node_t *
lw6p2p_node_new (int argc, char *argv[], char *client_backends,
		 char *server_backends, char *bind_ip,
		 int bind_port, u_int64_t server_id, char *public_url)
{
  return (lw6p2p_node_t *) _lw6p2p_node_new (argc, argv, client_backends,
					     server_backends, bind_ip,
					     bind_port, server_id,
					     public_url);
}

_lw6p2p_node_t *
_lw6p2p_node_new (int argc, char *argv[], char *client_backends,
		  char *server_backends, char *bind_ip, int bind_port,
		  u_int64_t server_id, char *public_url)
{
  _lw6p2p_node_t *node = NULL;
  lw6sys_list_t *list_backends = NULL;
  char *backend;
  int ret = 1;

  node = (_lw6p2p_node_t *) LW6SYS_CALLOC (sizeof (_lw6p2p_node_t));
  if (node)
    {
      node->id = 0;
      while (!node->id)
	{
	  node->id = ++seq_id;
	}
      node->bind_ip = lw6sys_str_copy (bind_ip);
      node->bind_port = bind_port;
      node->server_id = server_id;
      node->public_url = lw6sys_str_copy (public_url);
      ret = (node->bind_ip && node->public_url);
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
    }

  if ((!ret) && node)
    {
      _lw6p2p_node_free (node);
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
      if (node->bind_ip)
	{
	  LW6SYS_FREE (node->bind_ip);
	}
      if (node->public_url)
	{
	  LW6SYS_FREE (node->public_url);
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
	lw6sys_new_sprintf (_("%u %s:%d"), node->id, node->bind_ip,
			    node->bind_port);
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

int
_lw6p2p_node_poll (_lw6p2p_node_t * node)
{
  int ret = 0;
  char *ip = NULL;
  int port = 0;
  int sock;
  lw6srv_tcp_accepter_t *tcp_accepter = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("polling node"));

  if (lw6srv_poll (node->listener))
    {
      if (node->listener->tcp_sock >= 0)
	{
	  sock =
	    lw6net_tcp_accept (&ip, &port, node->listener->tcp_sock,
			       _LW6P2P_SLEEP_DELAY);
	  if (sock >= 0 && ip != NULL && port > 0)
	    {
	      tcp_accepter = lw6srv_tcp_accepter_new (ip, port, sock);
	      if (tcp_accepter)
		{
		  //lw6sys_log (LW6SYS_LOG_DEBUG, _("connection from %s:%d"),ip,port);
		  lw6sys_log (LW6SYS_LOG_NOTICE, _("connection from %s:%d"),
			      ip, port);
		  ip = NULL;	// tcp_accepter will free it
		  lw6sys_list_push_front (&(node->listener->tcp_accepters),
					  tcp_accepter);
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

  if (node)
    {
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
      lw6sys_log (LW6SYS_LOG_WARNING, _("trying to close NULL node"));
    }
}
