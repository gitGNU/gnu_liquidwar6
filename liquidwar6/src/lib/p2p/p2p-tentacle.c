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
_lw6p2p_tentacle_init (_lw6p2p_tentacle_t * tentacle,
		       _lw6p2p_backends_t * backends,
		       lw6srv_listener_t * listener, char *local_url,
		       char *remote_url, char *real_remote_ip, char *password,
		       u_int64_t local_id, u_int64_t remote_id,
		       int network_reliability,
		       lw6cnx_recv_callback_t recv_callback_func,
		       void *recv_callback_data)
{
  int ret = 1;
  int i = 0;
  //_lw6p2p_tentacle_clear(tentacle,backends);
  char *repr = NULL;
  lw6sys_url_t *parsed_url = NULL;

  tentacle->backends = backends;
  tentacle->local_url = lw6sys_str_copy (local_url);
  tentacle->remote_url = lw6sys_str_copy (remote_url);
  parsed_url = lw6sys_url_parse (remote_url);
  if (parsed_url)
    {
      /*
       * In theory this could be a long blocking call,
       * in practise, when we're at this stage OOB messages
       * probably have already initialized the DNS cache
       * so response will be fast.
       */
      tentacle->remote_ip = lw6net_dns_gethostbyname (parsed_url->host);
      tentacle->remote_port = parsed_url->port;
      if (real_remote_ip)
	{
	  if (tentacle->remote_ip)
	    {
	      if (lw6sys_str_is_same (tentacle->remote_ip, real_remote_ip))
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_
			      ("OK, public URL \"%s\" for \"%s:%d\" is fine"),
			      remote_url, tentacle->remote_ip,
			      tentacle->remote_port);
		  tentacle->dns_ok = 1;
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("URL \"%s\" appears to be associated to \"%s:%d\" but DNS reports it to be associated to \"%s:%d\", using \"%s:%d\""),
			      remote_url, real_remote_ip,
			      tentacle->remote_port, tentacle->remote_ip,
			      tentacle->remote_port, real_remote_ip,
			      tentacle->remote_port);
		  LW6SYS_FREE (tentacle->remote_ip);
		  tentacle->remote_ip = lw6sys_str_copy (real_remote_ip);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("unable to get IP from DNS for \"%s\", using \"%s:%d\" instead"),
			  remote_url, real_remote_ip, tentacle->remote_port);
	      tentacle->remote_ip = lw6sys_str_copy (real_remote_ip);
	    }
	}
      else
	{
	  if (tentacle->remote_ip)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_
			  ("no \"real\" IP passed, using default \"%s:%d\""),
			  tentacle->remote_ip, tentacle->remote_port);
	      tentacle->dns_ok = 1;
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("unable to find host \"%s\""),
			  parsed_url->host);
	    }
	}
      lw6sys_url_free (parsed_url);
    }
  if (password && strlen (password) > 0)
    {
      tentacle->password = lw6sys_str_copy (password);
    }
  else
    {
      tentacle->password = lw6sys_str_copy ("");
    }
  tentacle->local_id_int = local_id;
  tentacle->local_id_str = lw6sys_id_ltoa (local_id);
  tentacle->remote_id_int = remote_id;
  tentacle->remote_id_str = lw6sys_id_ltoa (remote_id);

  if (tentacle->local_url && tentacle->remote_url && tentacle->remote_ip
      && tentacle->password && tentacle->local_id_str
      && tentacle->remote_id_str)
    {
      tentacle->nb_cli_connections = backends->nb_cli_backends;
      if (tentacle->nb_cli_connections > 0)
	{
	  tentacle->cli_connections =
	    (lw6cnx_connection_t **)
	    LW6SYS_CALLOC (tentacle->nb_cli_connections *
			   sizeof (lw6cnx_connection_t *));
	  if (tentacle->cli_connections)
	    {
	      for (i = 0; i < tentacle->nb_cli_connections; ++i)
		{
		  tentacle->cli_connections[i] =
		    lw6cli_open (tentacle->backends->cli_backends[i],
				 local_url, remote_url, tentacle->remote_ip,
				 tentacle->remote_port,
				 tentacle->password,
				 tentacle->local_id_int,
				 tentacle->remote_id_int, tentacle->dns_ok,
				 network_reliability, recv_callback_func,
				 recv_callback_data);
		  if (tentacle->cli_connections[i])
		    {
		      repr =
			lw6cli_repr (tentacle->backends->cli_backends[i],
				     tentacle->cli_connections[i]);
		      if (repr)
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG,
				      _x_ ("connection \"%s\" opened"), repr);
			  LW6SYS_FREE (repr);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_
				  ("unable to create connection %d to connect on \"%s\""),
				  i, tentacle->remote_url);
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
	  tentacle->srv_connections =
	    (lw6cnx_connection_t **)
	    LW6SYS_CALLOC (tentacle->nb_srv_connections *
			   sizeof (lw6cnx_connection_t *));
	  if (tentacle->srv_connections)
	    {
	      for (i = 0; i < tentacle->nb_srv_connections; ++i)
		{
		  tentacle->srv_connections[i] =
		    lw6srv_open (tentacle->backends->srv_backends[i],
				 listener,
				 local_url, remote_url, tentacle->remote_ip,
				 tentacle->remote_port,
				 tentacle->password,
				 tentacle->local_id_int,
				 tentacle->remote_id_int, tentacle->dns_ok,
				 network_reliability, recv_callback_func,
				 recv_callback_data);
		  if (tentacle->srv_connections[i])
		    {
		      repr =
			lw6srv_repr (tentacle->backends->srv_backends[i],
				     tentacle->srv_connections[i]);
		      if (repr)
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG,
				      _x_ ("connection \"%s\" opened"), repr);
			  LW6SYS_FREE (repr);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_
				  ("unable to create connection %d to connect on \"%s\""),
				  i, tentacle->remote_url);
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
_lw6p2p_tentacle_clear (_lw6p2p_tentacle_t * tentacle)
{
  int i = 0;

  if (tentacle->nb_srv_connections > 0 && tentacle->srv_connections)
    {
      for (i = 0; i < tentacle->nb_srv_connections; ++i)
	{
	  if (tentacle->srv_connections[i])
	    {
	      lw6srv_close (tentacle->backends->srv_backends[i],
			    tentacle->srv_connections[i]);
	    }
	}
      LW6SYS_FREE (tentacle->srv_connections);
    }
  if (tentacle->nb_cli_connections > 0 && tentacle->cli_connections)
    {
      for (i = 0; i < tentacle->nb_cli_connections; ++i)
	{
	  if (tentacle->cli_connections[i])
	    {
	      lw6cli_close (tentacle->backends->cli_backends[i],
			    tentacle->cli_connections[i]);
	    }
	}
      LW6SYS_FREE (tentacle->cli_connections);
    }
  if (tentacle->local_url)
    {
      LW6SYS_FREE (tentacle->local_url);
    }
  if (tentacle->remote_url)
    {
      LW6SYS_FREE (tentacle->remote_url);
    }
  if (tentacle->remote_ip)
    {
      LW6SYS_FREE (tentacle->remote_ip);
    }
  if (tentacle->password)
    {
      LW6SYS_FREE (tentacle->password);
    }
  if (tentacle->local_id_str)
    {
      LW6SYS_FREE (tentacle->local_id_str);
    }
  if (tentacle->remote_id_str)
    {
      LW6SYS_FREE (tentacle->remote_id_str);
    }
  memset (tentacle, 0, sizeof (_lw6p2p_tentacle_t));
}

int
_lw6p2p_tentacle_enabled (_lw6p2p_tentacle_t * tentacle)
{
  int ret = 0;

  ret = (tentacle->remote_id_int != 0);

  return ret;
}

void
_lw6p2p_tentacle_poll (_lw6p2p_tentacle_t * tentacle,
		       lw6nod_info_t * node_info,
		       lw6cnx_ticket_table_t * ticket_table,
		       int next_foo_delay)
{
  int i;
  char *msg;
  int64_t now;
  lw6cnx_connection_t *cnx = NULL;
  u_int32_t ticket_sig = 0;

  now = lw6sys_get_timestamp ();
  if (!tentacle->hello_sent)
    {
      msg = lw6msg_cmd_generate_hello (node_info);
      if (msg)
	{
	  for (i = 0; i < tentacle->nb_cli_connections; ++i)
	    {
	      cnx = tentacle->cli_connections[i];
	      ticket_sig =
		lw6msg_ticket_calc_sig (lw6cnx_ticket_table_get_send
					(ticket_table, cnx->remote_id_str),
					cnx->local_id_int, cnx->remote_id_int,
					msg);
	      if (lw6cli_send
		  (tentacle->backends->cli_backends[i], cnx, ticket_sig,
		   ticket_sig, cnx->local_id_int, cnx->remote_id_int, msg))
		{
		  tentacle->hello_sent = 1;
		}
	    }
	  LW6SYS_FREE (msg);
	}
    }
  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      cnx = tentacle->cli_connections[i];
      if (lw6cnx_connection_should_send_foo (cnx, now))
	{
	  lw6cnx_connection_init_foo_bar_key (cnx, now, next_foo_delay);
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("preparing foo with foo_bar_key=%08x"),
		      cnx->foo_bar_key);
	  msg = lw6msg_cmd_generate_foo (node_info, cnx->foo_bar_key);
	  if (msg)
	    {
	      ticket_sig =
		lw6msg_ticket_calc_sig (lw6cnx_ticket_table_get_send
					(ticket_table, cnx->remote_id_str),
					cnx->local_id_int, cnx->remote_id_int,
					msg);
	      lw6cli_send (tentacle->backends->cli_backends[i], cnx,
			   ticket_sig, ticket_sig, cnx->local_id_int,
			   cnx->remote_id_int, msg);
	      LW6SYS_FREE (msg);
	    }
	  if (!lw6cnx_ticket_table_was_recv_exchanged
	      (ticket_table, cnx->remote_id_str))
	    {
	      msg =
		lw6msg_cmd_generate_ticket (node_info,
					    lw6cnx_ticket_table_get_recv
					    (ticket_table,
					     cnx->remote_id_str));
	      if (msg)
		{
		  ticket_sig =
		    lw6msg_ticket_calc_sig (lw6cnx_ticket_table_get_send
					    (ticket_table,
					     cnx->remote_id_str),
					    cnx->local_id_int,
					    cnx->remote_id_int, msg);
		  lw6cli_send (tentacle->backends->cli_backends[i], cnx,
			       ticket_sig, ticket_sig, cnx->local_id_int,
			       cnx->remote_id_int, msg);
		  LW6SYS_FREE (msg);
		}
	    }
	}
      lw6cli_poll (tentacle->backends->cli_backends[i], cnx);
    }
  for (i = 0; i < tentacle->nb_srv_connections; ++i)
    {
      cnx = tentacle->srv_connections[i];
      if (lw6cnx_connection_should_send_foo (cnx, now))
	{
	  lw6cnx_connection_init_foo_bar_key (cnx, now, next_foo_delay);
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("preparing foo with foo_bar_key=%08x"),
		      cnx->foo_bar_key);
	  msg = lw6msg_cmd_generate_foo (node_info, cnx->foo_bar_key);
	  if (msg)
	    {
	      ticket_sig =
		lw6msg_ticket_calc_sig (lw6cnx_ticket_table_get_send
					(ticket_table, cnx->remote_id_str),
					cnx->local_id_int, cnx->remote_id_int,
					msg);
	      lw6srv_send (tentacle->backends->srv_backends[i], cnx,
			   ticket_sig, ticket_sig, cnx->local_id_int,
			   cnx->remote_id_int, msg);
	      LW6SYS_FREE (msg);
	    }
	  if (!lw6cnx_ticket_table_was_recv_exchanged
	      (ticket_table, cnx->remote_id_str))
	    {
	      msg =
		lw6msg_cmd_generate_ticket (node_info,
					    lw6cnx_ticket_table_get_recv
					    (ticket_table,
					     cnx->remote_id_str));
	      if (msg)
		{
		  ticket_sig =
		    lw6msg_ticket_calc_sig (lw6cnx_ticket_table_get_send
					    (ticket_table,
					     cnx->remote_id_str),
					    cnx->local_id_int,
					    cnx->remote_id_int, msg);
		  lw6srv_send (tentacle->backends->srv_backends[i], cnx,
			       ticket_sig, ticket_sig, cnx->local_id_int,
			       cnx->remote_id_int, msg);
		  LW6SYS_FREE (msg);
		}
	    }
	}
      lw6srv_poll (tentacle->backends->srv_backends[i], cnx);
    }
}

int
_lw6p2p_tentacle_send_best (_lw6p2p_tentacle_t * tentacle,
			    lw6cnx_ticket_table_t * ticket_table,
			    u_int32_t logical_ticket_sig,
			    u_int64_t logical_from_id,
			    u_int64_t logical_to_id, char *msg)
{
  int ret = 0;

  // todo: find a way to pick up best cnx
  ret =
    _lw6p2p_tentacle_send_redundant (tentacle, ticket_table,
				     logical_ticket_sig, logical_from_id,
				     logical_to_id, msg);

  return ret;
}

int
_lw6p2p_tentacle_send_redundant (_lw6p2p_tentacle_t * tentacle,
				 lw6cnx_ticket_table_t * ticket_table,
				 u_int32_t logical_ticket_sig,
				 u_int64_t logical_from_id,
				 u_int64_t logical_to_id, char *msg)
{
  int ret = 0;
  lw6cnx_connection_t *cnx = NULL;
  int i = 0;
  u_int32_t physical_ticket_sig = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("redundant send of \"%s\""), msg);
  physical_ticket_sig =
    lw6msg_ticket_calc_sig (lw6cnx_ticket_table_get_send
			    (ticket_table, tentacle->remote_id_str),
			    tentacle->local_id_int, tentacle->remote_id_int,
			    msg);

  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      cnx = tentacle->cli_connections[i];
      ret |= lw6cli_send (tentacle->backends->cli_backends[i], cnx,
			  physical_ticket_sig, logical_ticket_sig,
			  cnx->local_id_int, cnx->remote_id_int, msg);
    }

  for (i = 0; i < tentacle->nb_srv_connections; ++i)
    {
      cnx = tentacle->srv_connections[i];
      ret |= lw6srv_send (tentacle->backends->srv_backends[i], cnx,
			  physical_ticket_sig, logical_ticket_sig,
			  cnx->local_id_int, cnx->remote_id_int, msg);
    }

  return ret;
}

lw6cnx_connection_t *
_lw6p2p_tentacle_find_connection_with_foo_bar_key (_lw6p2p_tentacle_t *
						   tentacle,
						   u_int32_t foo_bar_key)
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
