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
_lw6p2p_tentacle_init (lw6sys_context_t * sys_context,
		       _lw6p2p_tentacle_t * tentacle,
		       _lw6p2p_backends_t * backends,
		       lw6srv_listener_t * listener, const char *local_url,
		       const char *remote_url, const char *real_remote_ip,
		       const char *password, u_int64_t local_id, u_int64_t remote_id, int network_reliability)
{
  int ret = 1;
  int i = 0;
  char *repr = NULL;
  lw6sys_url_t *parsed_url = NULL;

  tentacle->backends = backends;
  tentacle->local_url = lw6sys_str_copy (sys_context, local_url);
  tentacle->remote_url = lw6sys_str_copy (sys_context, remote_url);
  parsed_url = lw6sys_url_parse (sys_context, remote_url);
  if (parsed_url)
    {
      /*
       * In theory this could be a long blocking call,
       * in practise, when we're at this stage OOB messages
       * probably have already initialized the DNS cache
       * so response will be fast.
       */
      tentacle->remote_ip = lw6net_dns_gethostbyname (sys_context, parsed_url->host);
      tentacle->remote_port = parsed_url->port;
      if (real_remote_ip)
	{
	  if (tentacle->remote_ip)
	    {
	      if (lw6sys_str_is_same (sys_context, tentacle->remote_ip, real_remote_ip))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_ ("OK, public URL \"%s\" for \"%s:%d\" is fine"), remote_url, tentacle->remote_ip, tentacle->remote_port);
		  tentacle->dns_ok = 1;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_
			      ("URL \"%s\" appears to be associated to \"%s:%d\" but DNS reports it to be associated to \"%s:%d\", using \"%s:%d\""),
			      remote_url, real_remote_ip,
			      tentacle->remote_port, tentacle->remote_ip, tentacle->remote_port, real_remote_ip, tentacle->remote_port);
		  LW6SYS_FREE (sys_context, tentacle->remote_ip);
		  tentacle->remote_ip = lw6sys_str_copy (sys_context, real_remote_ip);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("unable to get IP from DNS for \"%s\", using \"%s:%d\" instead"), remote_url, real_remote_ip, tentacle->remote_port);
	      tentacle->remote_ip = lw6sys_str_copy (sys_context, real_remote_ip);
	    }
	}
      else
	{
	  if (tentacle->remote_ip)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no \"real\" IP passed, using default \"%s:%d\""), tentacle->remote_ip, tentacle->remote_port);
	      tentacle->dns_ok = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find host \"%s\""), parsed_url->host);
	    }
	}
      lw6sys_url_free (sys_context, parsed_url);
    }
  if (password && strlen (password) > 0)
    {
      tentacle->password = lw6sys_str_copy (sys_context, password);
    }
  else
    {
      tentacle->password = lw6sys_str_copy (sys_context, "");
    }
  tentacle->local_id_int = local_id;
  tentacle->local_id_str = lw6sys_id_ltoa (sys_context, local_id);
  tentacle->remote_id_int = remote_id;
  tentacle->remote_id_str = lw6sys_id_ltoa (sys_context, remote_id);

  if (tentacle->local_url && tentacle->remote_url && tentacle->remote_ip && tentacle->password && tentacle->local_id_str && tentacle->remote_id_str)
    {
      tentacle->nb_cli_connections = backends->nb_cli_backends;
      if (tentacle->nb_cli_connections > 0)
	{
	  tentacle->cli_connections = (lw6cnx_connection_t **) LW6SYS_CALLOC (sys_context, tentacle->nb_cli_connections * sizeof (lw6cnx_connection_t *));
	  if (tentacle->cli_connections)
	    {
	      for (i = 0; i < tentacle->nb_cli_connections; ++i)
		{
		  tentacle->cli_connections[i] =
		    lw6cli_open (sys_context,
				 tentacle->backends->cli_backends[i],
				 local_url, remote_url, tentacle->remote_ip,
				 tentacle->remote_port, tentacle->password,
				 tentacle->local_id_int, tentacle->remote_id_int, tentacle->dns_ok, network_reliability);
		  if (tentacle->cli_connections[i])
		    {
		      repr = lw6cli_repr (sys_context, tentacle->backends->cli_backends[i], tentacle->cli_connections[i]);
		      if (repr)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connection \"%s\" opened"), repr);
			  LW6SYS_FREE (sys_context, repr);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create connection %d to connect on \"%s\""), i, tentacle->remote_url);
		      ret = 0;
		    }
		}
	    }
	  else
	    {
	      ret = 0;
	    }
	}
      tentacle->nb_srv_connections = backends->nb_srv_backends;
      if (tentacle->nb_srv_connections > 0)
	{
	  tentacle->srv_connections = (lw6cnx_connection_t **) LW6SYS_CALLOC (sys_context, tentacle->nb_srv_connections * sizeof (lw6cnx_connection_t *));
	  if (tentacle->srv_connections)
	    {
	      for (i = 0; i < tentacle->nb_srv_connections; ++i)
		{
		  tentacle->srv_connections[i] =
		    lw6srv_open (sys_context,
				 tentacle->backends->srv_backends[i],
				 listener, local_url, remote_url,
				 tentacle->remote_ip, tentacle->remote_port,
				 tentacle->password, tentacle->local_id_int, tentacle->remote_id_int, tentacle->dns_ok, network_reliability);
		  if (tentacle->srv_connections[i])
		    {
		      repr = lw6srv_repr (sys_context, tentacle->backends->srv_backends[i], tentacle->srv_connections[i]);
		      if (repr)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("connection \"%s\" opened"), repr);
			  LW6SYS_FREE (sys_context, repr);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create connection %d to connect on \"%s\""), i, tentacle->remote_url);
		      ret = 0;
		    }
		}
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

  return ret;
}

void
_lw6p2p_tentacle_clear (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle)
{
  int i = 0;

  if (tentacle->unsent_reliable_queue)
    {
      lw6sys_list_free (sys_context, tentacle->unsent_reliable_queue);
    }
  if (tentacle->unsent_unreliable_queue)
    {
      lw6sys_list_free (sys_context, tentacle->unsent_unreliable_queue);
    }
  if (tentacle->nb_srv_connections > 0 && tentacle->srv_connections)
    {
      for (i = 0; i < tentacle->nb_srv_connections; ++i)
	{
	  if (tentacle->srv_connections[i])
	    {
	      lw6srv_close (sys_context, tentacle->backends->srv_backends[i], tentacle->srv_connections[i]);
	    }
	}
      LW6SYS_FREE (sys_context, tentacle->srv_connections);
    }
  if (tentacle->nb_cli_connections > 0 && tentacle->cli_connections)
    {
      for (i = 0; i < tentacle->nb_cli_connections; ++i)
	{
	  if (tentacle->cli_connections[i])
	    {
	      lw6cli_close (sys_context, tentacle->backends->cli_backends[i], tentacle->cli_connections[i]);
	    }
	}
      LW6SYS_FREE (sys_context, tentacle->cli_connections);
    }
  if (tentacle->local_url)
    {
      LW6SYS_FREE (sys_context, tentacle->local_url);
    }
  if (tentacle->remote_url)
    {
      LW6SYS_FREE (sys_context, tentacle->remote_url);
    }
  if (tentacle->remote_ip)
    {
      LW6SYS_FREE (sys_context, tentacle->remote_ip);
    }
  if (tentacle->password)
    {
      LW6SYS_FREE (sys_context, tentacle->password);
    }
  if (tentacle->local_id_str)
    {
      LW6SYS_FREE (sys_context, tentacle->local_id_str);
    }
  if (tentacle->remote_id_str)
    {
      LW6SYS_FREE (sys_context, tentacle->remote_id_str);
    }
  memset (tentacle, 0, sizeof (_lw6p2p_tentacle_t));
}

int
_lw6p2p_tentacle_enabled (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle)
{
  int ret = 0;

  ret = (tentacle->remote_id_int != 0);

  return ret;
}

typedef struct _send_best_data_s
{
  _lw6p2p_tentacle_t *tentacle;
  int64_t now;
  lw6cnx_ticket_table_t *ticket_table;
  lw6cnx_connection_t *best_cnx;
}
_send_best_data_t;

static int
_send_best_filter (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int keep = 1;
  int found_cnx = 0;
  int i = 0;
  int ping_msec = 0;
  _send_best_data_t *send_best_data = (_send_best_data_t *) func_data;
  lw6cnx_packet_t *packet = (lw6cnx_packet_t *) data;
  _lw6p2p_tentacle_t *tentacle = send_best_data->tentacle;
  lw6cnx_connection_t *best_cnx = send_best_data->best_cnx;
  int64_t now = send_best_data->now;
  lw6cnx_ticket_table_t *ticket_table = send_best_data->ticket_table;
  u_int32_t logical_ticket_sig = packet->logical_ticket_sig;
  u_int32_t physical_ticket_sig = packet->physical_ticket_sig;
  u_int64_t logical_from_id = packet->logical_from_id;
  u_int64_t logical_to_id = packet->logical_to_id;
  char *msg = packet->msg;

  /*
   * We store the cnx ping into a local variable since it might
   * be altered if the send fails, and we want the original
   * ping to be used/shown in logs.
   */
  ping_msec = best_cnx->ping_msec;
  if (ping_msec > 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("send of \"%s\" on fastest connection"), msg);
      /*
       * Now we have the cnx, we must figure out its type (cnx/srv)
       * and index to fire the right code on it.
       */
      for (i = 0; i < tentacle->nb_cli_connections && !found_cnx; ++i)
	{
	  if (best_cnx && (best_cnx == tentacle->cli_connections[i]) && !found_cnx)
	    {
	      found_cnx = 1;
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_
			  ("found fastest connection to \"%s\", it's a client connection, backend name=\"%s\""),
			  tentacle->remote_url, tentacle->backends->cli_backends[i]->name);
	      if (lw6cli_send
		  (sys_context, tentacle->backends->cli_backends[i], best_cnx,
		   now, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id, msg))
		{
		  keep = 0;
		}
	    }
	}
      for (i = 0; i < tentacle->nb_srv_connections && !found_cnx; ++i)
	{
	  if (best_cnx && (best_cnx == tentacle->srv_connections[i]) && !found_cnx)
	    {
	      found_cnx = 1;
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_
			  ("found fastest connection to \"%s\", it's a server connection, backend name=\"%s\""),
			  tentacle->remote_url, tentacle->backends->srv_backends[i]->name);
	      if (lw6srv_send
		  (sys_context, tentacle->backends->srv_backends[i], best_cnx,
		   now, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id, msg))
		{
		  keep = 0;
		}
	    }
	}
      if (keep)
	{
	  if (found_cnx && best_cnx)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_
			  ("cnx found, but unable to send message, stats success=%d/fail=%d/total=%d"),
			  best_cnx->sent_nb_success, best_cnx->sent_nb_fail, best_cnx->sent_nb_total);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("cnx not found, unable to send message \"%s\""), msg);
	    }
	}
    }
  else
    {
      /*
       * OK, we did not really find a "best connection" since it pretends
       * to have a zero ping, and ping zero is just a plain imaginary stuff,
       * and actually, we always force real pings to be at least one.
       * Well, anyway, just means we did not really find it, so meanwhile,
       * until we get a real "best" one, we go redundant.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("couldn't really find a \"best\" connection for now, fallback on redundant mode"));
      _lw6p2p_tentacle_send_redundant (sys_context, tentacle, now, ticket_table, logical_ticket_sig, logical_from_id, logical_to_id, msg);
    }

  return keep;
}

void
_lw6p2p_tentacle_poll_protocol (lw6sys_context_t * sys_context,
				_lw6p2p_tentacle_t * tentacle,
				lw6nod_info_t * node_info, lw6cnx_ticket_table_t * ticket_table, const _lw6p2p_consts_t * consts, int serial)
{
  int i = 0;
  char *msg = NULL;
  int64_t now = 0LL;
  lw6cnx_connection_t *cnx = NULL;
  u_int32_t ticket_sig = 0;

  now = lw6sys_get_timestamp (sys_context);

  if (!tentacle->hello_sent)
    {
      msg = lw6msg_cmd_generate_hello (sys_context, node_info);
      if (msg)
	{
	  for (i = 0; i < tentacle->nb_cli_connections; ++i)
	    {
	      cnx = tentacle->cli_connections[i];
	      ticket_sig =
		lw6msg_ticket_calc_sig (sys_context,
					lw6cnx_ticket_table_get_send
					(sys_context, ticket_table, cnx->remote_id_str), cnx->local_id_int, cnx->remote_id_int, msg);
	      if (lw6cli_send (sys_context, tentacle->backends->cli_backends[i], cnx, now, ticket_sig, ticket_sig, cnx->local_id_int, cnx->remote_id_int, msg))
		{
		  tentacle->hello_sent = 1;
		}
	    }
	  LW6SYS_FREE (sys_context, msg);
	}
    }

  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      cnx = tentacle->cli_connections[i];
      if (lw6cnx_connection_should_send_foo (sys_context, cnx, now))
	{
	  lw6cnx_connection_init_foo_bar_key (sys_context, cnx, now, consts->foo_delay);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("preparing foo with foo_bar_key=%08x"), cnx->foo_bar_key);
	  msg = lw6msg_cmd_generate_foo (sys_context, node_info, cnx->foo_bar_key, serial);
	  if (msg)
	    {
	      ticket_sig =
		lw6msg_ticket_calc_sig (sys_context,
					lw6cnx_ticket_table_get_send
					(sys_context, ticket_table, cnx->remote_id_str), cnx->local_id_int, cnx->remote_id_int, msg);
	      lw6cli_send (sys_context, tentacle->backends->cli_backends[i], cnx, now, ticket_sig, ticket_sig, cnx->local_id_int, cnx->remote_id_int, msg);
	      LW6SYS_FREE (sys_context, msg);
	    }
	  if (!lw6cnx_ticket_table_was_recv_exchanged (sys_context, ticket_table, cnx->remote_id_str))
	    {
	      msg = lw6msg_cmd_generate_ticket (sys_context, node_info, lw6cnx_ticket_table_get_recv (sys_context, ticket_table, cnx->remote_id_str));
	      if (msg)
		{
		  ticket_sig =
		    lw6msg_ticket_calc_sig (sys_context,
					    lw6cnx_ticket_table_get_send
					    (sys_context, ticket_table, cnx->remote_id_str), cnx->local_id_int, cnx->remote_id_int, msg);
		  lw6cli_send (sys_context, tentacle->backends->cli_backends[i], cnx, now, ticket_sig, ticket_sig, cnx->local_id_int, cnx->remote_id_int, msg);
		  LW6SYS_FREE (sys_context, msg);
		}
	    }
	}
    }

  for (i = 0; i < tentacle->nb_srv_connections; ++i)
    {
      cnx = tentacle->srv_connections[i];
      if (lw6cnx_connection_should_send_foo (sys_context, cnx, now))
	{
	  lw6cnx_connection_init_foo_bar_key (sys_context, cnx, now, consts->foo_delay);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("preparing foo with foo_bar_key=%08x"), cnx->foo_bar_key);
	  msg = lw6msg_cmd_generate_foo (sys_context, node_info, cnx->foo_bar_key, serial);
	  if (msg)
	    {
	      ticket_sig =
		lw6msg_ticket_calc_sig (sys_context,
					lw6cnx_ticket_table_get_send
					(sys_context, ticket_table, cnx->remote_id_str), cnx->local_id_int, cnx->remote_id_int, msg);
	      lw6srv_send (sys_context, tentacle->backends->srv_backends[i], cnx, now, ticket_sig, ticket_sig, cnx->local_id_int, cnx->remote_id_int, msg);
	      LW6SYS_FREE (sys_context, msg);
	    }
	  if (!lw6cnx_ticket_table_was_recv_exchanged (sys_context, ticket_table, cnx->remote_id_str))
	    {
	      msg = lw6msg_cmd_generate_ticket (sys_context, node_info, lw6cnx_ticket_table_get_recv (sys_context, ticket_table, cnx->remote_id_str));
	      if (msg)
		{
		  ticket_sig =
		    lw6msg_ticket_calc_sig (sys_context,
					    lw6cnx_ticket_table_get_send
					    (sys_context, ticket_table, cnx->remote_id_str), cnx->local_id_int, cnx->remote_id_int, msg);
		  lw6srv_send (sys_context, tentacle->backends->srv_backends[i], cnx, now, ticket_sig, ticket_sig, cnx->local_id_int, cnx->remote_id_int, msg);
		  LW6SYS_FREE (sys_context, msg);
		}
	    }
	}
    }
}

void
_lw6p2p_tentacle_poll_queues (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, _lw6p2p_tentacle_t * tentacle)
{
  int i = 0;
  int64_t now = 0LL;
  lw6cnx_connection_t *cnx = NULL;
  _send_best_data_t send_best_data;
  lw6sys_list_t *packets = NULL;
  lw6cnx_packet_t *packet = NULL;

  memset (&send_best_data, 0, sizeof (_send_best_data_t));
  now = lw6sys_get_timestamp (sys_context);

  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      cnx = tentacle->cli_connections[i];
      lw6cli_poll (sys_context, tentacle->backends->cli_backends[i], cnx);
      packets = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6cnx_packet_free);
      lw6sys_list_r_transfer_from (sys_context, cnx->recv_list, &packets);
      while (packets && (packet = (lw6cnx_packet_t *) lw6sys_list_pop_front (sys_context, &packets)))
	{
	  if (node)
	    {
	      _lw6p2p_recv_callback (sys_context, node, cnx, packet);
	    }
	  else
	    {
	      lw6cnx_packet_free (sys_context, packet);
	    }
	}
    }

  for (i = 0; i < tentacle->nb_srv_connections; ++i)
    {
      cnx = tentacle->srv_connections[i];
      lw6srv_poll (sys_context, tentacle->backends->srv_backends[i], cnx);
      packets = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6cnx_packet_free);
      lw6sys_list_r_transfer_from (sys_context, cnx->recv_list, &packets);
      while (packets && (packet = (lw6cnx_packet_t *) lw6sys_list_pop_front (sys_context, &packets)))
	{
	  if (node)
	    {
	      _lw6p2p_recv_callback (sys_context, node, cnx, packet);
	    }
	  else
	    {
	      lw6cnx_packet_free (sys_context, packet);
	    }
	}
    }

  if (node)
    {
      lw6cnx_ticket_table_t *ticket_table = &(node->ticket_table);

      if (tentacle->unsent_reliable_queue)
	{
	  send_best_data.tentacle = tentacle;
	  send_best_data.now = now;
	  send_best_data.ticket_table = ticket_table;
	  send_best_data.best_cnx = _lw6p2p_tentacle_find_connection_with_lowest_ping (sys_context, tentacle, 1);
	  if (send_best_data.best_cnx)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("flushing unsent_reliable_queue"));

	      /*
	       * Sort messages in a pseudo-random order on purpose,
	       * to make sure they arrive totally not in the right
	       * order.
	       */
	      lw6sys_sort (sys_context, &(tentacle->unsent_reliable_queue), lw6cnx_packet_sort_callback, NULL);
	      lw6sys_list_filter (sys_context, &(tentacle->unsent_reliable_queue), _send_best_filter, &send_best_data);
	    }
	  /*
	     else
	     {
	     lw6sys_list_free (sys_context,tentacle->unsent_reliable_queue);
	     tentacle->unsent_reliable_queue = NULL;
	     }
	   */
	}

      if (tentacle->unsent_unreliable_queue)
	{
	  send_best_data.tentacle = tentacle;
	  send_best_data.now = now;
	  send_best_data.ticket_table = ticket_table;
	  send_best_data.best_cnx = _lw6p2p_tentacle_find_connection_with_lowest_ping (sys_context, tentacle, 0);
	  if (send_best_data.best_cnx)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("flushing unsent_unreliable_queue"));

	      /*
	       * Sort messages in a pseudo-random order on purpose,
	       * to make sure they arrive totally not in the right
	       * order.
	       */
	      lw6sys_sort (sys_context, &(tentacle->unsent_unreliable_queue), lw6cnx_packet_sort_callback, NULL);
	      lw6sys_list_filter (sys_context, &(tentacle->unsent_unreliable_queue), _send_best_filter, &send_best_data);
	    }
	  else
	    {
	      lw6sys_list_free (sys_context, tentacle->unsent_unreliable_queue);
	      tentacle->unsent_unreliable_queue = NULL;
	    }
	}
    }
}

void
_lw6p2p_tentacle_poll (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, _lw6p2p_tentacle_t * tentacle, int serial)
{
  /*
   * IMPORTANT -> this function is called in unlock mode, it can
   * be in conflict with code from the recv callback. Normally this
   * should not be a problem.
   */
  _lw6p2p_tentacle_poll_protocol (sys_context, tentacle, node->node_info, &(node->ticket_table), &(node->db->data.consts), serial);
  _lw6p2p_tentacle_poll_queues (sys_context, node, tentacle);
}


int
_lw6p2p_tentacle_send_best (lw6sys_context_t * sys_context,
			    _lw6p2p_tentacle_t * tentacle, int64_t now,
			    lw6cnx_ticket_table_t * ticket_table,
			    u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *msg, int reliable)
{
  int ret = 0;
  u_int32_t physical_ticket_sig = 0;
  lw6cnx_packet_t *packet = NULL;

  physical_ticket_sig =
    lw6msg_ticket_calc_sig (sys_context,
			    lw6cnx_ticket_table_get_send (sys_context,
							  ticket_table, tentacle->remote_id_str), tentacle->local_id_int, tentacle->remote_id_int, msg);

  if (reliable)
    {
      if (!tentacle->unsent_reliable_queue)
	{
	  tentacle->unsent_reliable_queue = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6cnx_packet_free);
	}
      if (tentacle->unsent_reliable_queue)
	{
	  packet = lw6cnx_packet_new (sys_context, logical_ticket_sig, physical_ticket_sig, logical_from_id, logical_to_id, msg);
	  if (packet)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("message \"%s\" not sent, pushing it to unsent_reliable_queue"), packet->msg);
	      /*
	       * Use a lifo, a fifo would be cleaner but is more expensive
	       * on big lists, this is a fallback action anyway, packets
	       * will probably be in the wrong order anyway and we are
	       * very likely sending a big hudge message splitted into
	       * many atoms so order is irrelevant.
	       */
	      lw6sys_lifo_push (sys_context, &(tentacle->unsent_reliable_queue), packet);
	      ret = 1;
	    }
	}
    }

  if (!tentacle->unsent_unreliable_queue)
    {
      tentacle->unsent_unreliable_queue = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6cnx_packet_free);
    }
  if (tentacle->unsent_unreliable_queue)
    {
      packet = lw6cnx_packet_new (sys_context, logical_ticket_sig, physical_ticket_sig, logical_from_id, logical_to_id, msg);
      if (packet)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("message \"%s\" not sent, pushing it to unsent_unreliable_queue"), packet->msg);
	  /*
	   * Use a lifo, a fifo would be cleaner but is more expensive
	   * on big lists, this is a fallback action anyway, packets
	   * will probably be in the wrong order anyway and we are
	   * very likely sending a big hudge message splitted into
	   * many atoms so order is irrelevant.
	   */
	  lw6sys_lifo_push (sys_context, &(tentacle->unsent_unreliable_queue), packet);
	  if (!reliable)
	    {
	      /*
	       * If sending in reliable mode, ret should have been
	       * set before in the reliable section.
	       */
	      ret = 1;
	    }
	}
    }

  return ret;
}

int
_lw6p2p_tentacle_send_redundant (lw6sys_context_t * sys_context,
				 _lw6p2p_tentacle_t * tentacle, int64_t now,
				 lw6cnx_ticket_table_t * ticket_table,
				 u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *msg)
{
  int ret = 0;
  lw6cnx_connection_t *cnx = NULL;
  int i = 0;
  u_int32_t physical_ticket_sig = 0;
  char *repr = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("redundant send of \"%s\""), msg);
  physical_ticket_sig =
    lw6msg_ticket_calc_sig (sys_context,
			    lw6cnx_ticket_table_get_send (sys_context,
							  ticket_table, tentacle->remote_id_str), tentacle->local_id_int, tentacle->remote_id_int, msg);

  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      cnx = tentacle->cli_connections[i];
      repr = lw6cli_repr (sys_context, tentacle->backends->cli_backends[i], tentacle->cli_connections[i]);
      if (repr)
	{
	  if (lw6cli_send
	      (sys_context, tentacle->backends->cli_backends[i], cnx, now, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id, msg))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sent redundant message on client connexion \"%s\""), repr);
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't send redundant message on client connexion \"%s\""), repr);
	    }
	  LW6SYS_FREE (sys_context, repr);
	  repr = NULL;
	}
    }

  for (i = 0; i < tentacle->nb_srv_connections; ++i)
    {
      cnx = tentacle->srv_connections[i];
      repr = lw6srv_repr (sys_context, tentacle->backends->srv_backends[i], tentacle->srv_connections[i]);
      if (repr)
	{
	  if (lw6srv_send
	      (sys_context, tentacle->backends->srv_backends[i], cnx, now, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id, msg))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sent redundant message on server connexion \"%s\""), repr);
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't send redundant message on server connexion \"%s\""), repr);
	    }
	  LW6SYS_FREE (sys_context, repr);
	  repr = NULL;
	}
    }

  return ret;
}

lw6cnx_connection_t *
_lw6p2p_tentacle_find_connection_with_foo_bar_key (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle, u_int32_t foo_bar_key)
{
  lw6cnx_connection_t *ret = NULL;
  lw6cnx_connection_t *cnx = NULL;
  int i = 0;

  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      cnx = tentacle->cli_connections[i];
      if (cnx->foo_bar_key == foo_bar_key)
	{
	  ret = cnx;
	}
    }

  for (i = 0; i < tentacle->nb_srv_connections; ++i)
    {
      cnx = tentacle->srv_connections[i];
      if (cnx->foo_bar_key == foo_bar_key)
	{
	  ret = cnx;
	}
    }

  return ret;
}

lw6cnx_connection_t *
_lw6p2p_tentacle_find_connection_with_lowest_ping (lw6sys_context_t * sys_context, _lw6p2p_tentacle_t * tentacle, int reliable)
{
  lw6cnx_connection_t *ret = NULL;
  lw6cnx_connection_t *cnx = NULL;
  int i = 0;
  int best_ping_msec = LW6CNX_WORST_PING_MSEC;

  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      cnx = tentacle->cli_connections[i];
      if (cnx->ping_msec > 0
	  && lw6cli_can_send (sys_context,
			      tentacle->backends->cli_backends[i], cnx) && cnx->ping_msec < best_ping_msec && (cnx->properties.reliable || !reliable))
	{
	  best_ping_msec = cnx->ping_msec;
	  ret = cnx;
	}
    }

  for (i = 0; i < tentacle->nb_srv_connections; ++i)
    {
      cnx = tentacle->srv_connections[i];
      if (cnx->ping_msec > 0
	  && lw6srv_can_send (sys_context,
			      tentacle->backends->srv_backends[i], cnx) && cnx->ping_msec < best_ping_msec && (cnx->properties.reliable || !reliable))
	{
	  best_ping_msec = cnx->ping_msec;
	  ret = cnx;
	}
    }

  /*
   * If we couldn't find a reliable connexion, try to fallback
   * on a non-reliable one...
   */
  if (reliable && !ret)
    {
      ret = _lw6p2p_tentacle_find_connection_with_lowest_ping (sys_context, tentacle, 0);
    }

  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("best ping is %d with backend \"%s\" reliable=%d"), ret->ping_msec, ret->properties.backend_id, ret->properties.reliable);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("unable to find any suitable connection, no reliable connection at all and no connection marked as ping better than %d"),
		  LW6CNX_WORST_PING_MSEC);
    }

  return ret;
}
