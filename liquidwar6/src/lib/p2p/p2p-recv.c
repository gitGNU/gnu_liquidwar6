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

void
_lw6p2p_recv_process (_lw6p2p_node_t * node,
		      lw6cnx_connection_t * cnx,
		      u_int64_t logical_from_id, char *message)
{
  lw6nod_info_t *remote_node_info = NULL;
  u_int64_t ticket = 0;
  u_int32_t foo_bar_key = 0;
  u_int32_t logical_ticket_sig = 0;
  int serial = 0;
  int i = 0;
  int n = 0;
  char *ker_message = NULL;
  char *reply_msg = NULL;
  int tentacle_i = 0;
  lw6cnx_connection_t *foo_cnx = NULL;
  lw6cnx_connection_t *fastest_cnx = NULL;
  int now = 0;
  int uptime = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("process \"%s\""), message);

  /*
   * We consider data is received even if we don't have the proof
   * it's correct. At least some bytes came and looked globally
   * ok, since the envelope is syntaxically right
   */
  cnx->last_recv_timestamp = lw6sys_get_timestamp ();

  if (lw6sys_str_starts_with_no_case (message, LW6MSG_CMD_HELLO))
    {
      if (lw6msg_cmd_analyse_hello (&remote_node_info, message))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("hello from \"%s\""),
		      cnx->remote_url);
	  /*
	   * We do nothing special on a hello (expect updating
	   * node info if needed, but this is done later)
	   */
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad hello from \"%s\" (\"%s\")"), cnx->remote_url,
		      message);
	}
    }
  else if (lw6sys_str_starts_with_no_case (message, LW6MSG_CMD_TICKET))
    {
      if (lw6msg_cmd_analyse_ticket (&remote_node_info, &ticket, message))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("received ticket from \"%s\""),
		      cnx->remote_url);
	  lw6cnx_ticket_table_set_send (&(node->ticket_table),
					cnx->remote_id_str, ticket);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad ticket from \"%s\" (\"%s\")"),
		      cnx->remote_url, message);
	}
    }
  else if (lw6sys_str_starts_with_no_case (message, LW6MSG_CMD_FOO))
    {
      if (lw6msg_cmd_analyse_foo (&remote_node_info, &foo_bar_key, message))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("received foo from \"%s\""),
		      cnx->remote_url);
	  reply_msg = lw6msg_cmd_generate_bar (node->node_info, foo_bar_key);
	  if (reply_msg)
	    {
	      logical_ticket_sig =
		lw6msg_ticket_calc_sig (lw6cnx_ticket_table_get_send
					(&(node->ticket_table),
					 cnx->remote_id_str),
					cnx->local_id_int, cnx->remote_id_int,
					reply_msg);

	      tentacle_i =
		_lw6p2p_node_find_tentacle (node, cnx->remote_id_int);
	      if (tentacle_i >= 0)
		{
		  _lw6p2p_tentacle_send_redundant (&
						   (node->tentacles
						    [tentacle_i]),
						   &(node->ticket_table),
						   logical_ticket_sig,
						   cnx->local_id_int,
						   cnx->remote_id_int,
						   reply_msg);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("unable to find the tentacle for a node which has an active connection"));
		}
	      LW6SYS_FREE (reply_msg);
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad foo from \"%s\" (\"%s\")"), cnx->remote_url,
		      message);
	}
    }
  else if (lw6sys_str_starts_with_no_case (message, LW6MSG_CMD_BAR))
    {
      if (lw6msg_cmd_analyse_bar (&remote_node_info, &foo_bar_key, message))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("received bar from \"%s\" foo_bar_key=%08x"),
		      cnx->remote_url, foo_bar_key);
	  tentacle_i = _lw6p2p_node_find_tentacle (node, cnx->remote_id_int);
	  if (tentacle_i >= 0)
	    {
	      foo_cnx =
		_lw6p2p_tentacle_find_connection_with_foo_bar_key (&
								   (node->tentacles
								    [tentacle_i]),
								   foo_bar_key);
	      /*
	       * It's important to check that remote_id_int is the same
	       * on both the receiver and the guessed sender, for we
	       * can't garantee *other* wouldn't disappear in a multithreaded
	       * context. If we garantee it's the same tentacle for
	       * both, then we garantee connection won't vanish. In practice
	       * having both a race condition *and* a key collision should
	       * be rare, but it's no use to take the risk.
	       */
	      if (foo_cnx && foo_cnx->remote_id_int == cnx->remote_id_int)
		{
		  foo_cnx->ping_msec =
		    lw6sys_get_timestamp () -
		    foo_cnx->last_send_foo_timestamp;
		  foo_cnx->foo_bar_key = 0;
		  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("ping=%d"),
			      foo_cnx->ping_msec);
		  if (cnx->dns_ok)
		    {
		      fastest_cnx =
			_lw6p2p_tentacle_find_connection_with_lowest_ping (&
									   (node->tentacles
									    [tentacle_i]));
		      if (fastest_cnx)
			{
			  if (!node->tentacles[tentacle_i].data_exchanged)
			    {
			      node->tentacles[tentacle_i].data_exchanged = 1;
			      lw6sys_log (LW6SYS_LOG_NOTICE,
					  _
					  ("communication established \"%s\" <-> \"%s\" on %s:%d"),
					  cnx->local_url, cnx->remote_url,
					  cnx->remote_ip, cnx->remote_port);
			    }
			  now = _lw6p2p_db_now (node->db);
			  uptime =
			    (lw6sys_get_timestamp () -
			     remote_node_info->
			     const_info.creation_timestamp) / 1000;
			  _lw6p2p_node_update_peer (node,
						    remote_node_info->
						    const_info.version,
						    remote_node_info->
						    const_info.codename,
						    remote_node_info->
						    const_info.stamp,
						    remote_node_info->
						    const_info.id_str,
						    remote_node_info->
						    const_info.url,
						    remote_node_info->
						    const_info.title,
						    remote_node_info->
						    const_info.description,
						    remote_node_info->
						    const_info.has_password,
						    remote_node_info->
						    const_info.bench,
						    remote_node_info->
						    const_info.open_relay,
						    now - uptime,
						    remote_node_info->
						    dyn_info.level,
						    remote_node_info->
						    dyn_info.required_bench,
						    remote_node_info->
						    dyn_info.nb_colors,
						    remote_node_info->
						    dyn_info.max_nb_colors,
						    remote_node_info->
						    dyn_info.nb_cursors,
						    remote_node_info->
						    dyn_info.max_nb_cursors,
						    remote_node_info->
						    dyn_info.nb_nodes,
						    remote_node_info->
						    dyn_info.max_nb_nodes,
						    cnx->remote_ip,
						    cnx->remote_port, now,
						    fastest_cnx->ping_msec);
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("serious problem, not finding a \"fastest\" connection, when we were just working on one?"));
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("node \"%s\" can't be trusted (DNS mismatch), not updating list"),
				  cnx->remote_url);
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_
			      ("unable to find connection with foo_bar_key %08x"),
			      foo_bar_key);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_
			  ("unable to find the tentacle for a node which has an active connection"));
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad bar from \"%s\" (\"%s\")"), cnx->remote_url,
		      message);
	}
    }
  else if (lw6sys_str_starts_with_no_case (message, LW6MSG_CMD_GOODBYE))
    {
      if (lw6msg_cmd_analyse_goodbye (&remote_node_info, message))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("received goodbye from \"%s\""),
		      cnx->remote_url);
	  // todo : close cnx
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad goodbye from \"%s\" (\"%s\")"),
		      cnx->remote_url, message);
	}
    }
  else if (lw6sys_str_starts_with_no_case (message, LW6MSG_CMD_DATA))
    {
      if (lw6msg_cmd_analyse_data (&serial, &i, &n, &ker_message, message))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("received data from \"%s\""),
		      cnx->remote_url);
	  // todo : process data!
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad data from \"%s\" (\"%s\")"), cnx->remote_url,
		      message);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unhandled message \"%s\""),
		  message);
    }

  if (remote_node_info)
    {
      lw6nod_info_free (remote_node_info);
    }
}

void
_lw6p2p_recv_forward (_lw6p2p_node_t * node,
		      lw6cnx_connection_t * cnx,
		      u_int32_t logical_ticket_sig,
		      u_int64_t logical_from_id, u_int64_t logical_to_id,
		      char *message)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("forward \"%s\""), message);
  // todo, forwarding
}

static int
_check_sig (lw6cnx_ticket_table_t * ticket_table, u_int64_t remote_id_int,
	    u_int64_t local_id_int, char *remote_id_str, char *message,
	    u_int32_t ticket_sig)
{
  int ret = 0;
  u_int64_t recv_ticket = 0;
  int was_recv_exchanged = 0;

  recv_ticket = lw6cnx_ticket_table_get_recv (ticket_table, remote_id_str);
  was_recv_exchanged = lw6cnx_ticket_table_was_recv_exchanged
    (ticket_table, remote_id_str);
  if (lw6msg_ticket_check_sig (recv_ticket, remote_id_int,
			       local_id_int, message, ticket_sig))
    {
      if (was_recv_exchanged)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("good sig from \"%s\""),
		      remote_id_str);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("first good signature from \"%s\""),
		      remote_id_str);
	  lw6cnx_ticket_table_ack_recv (ticket_table, remote_id_str);
	}
      ret = 1;
    }
  else
    {
      if (was_recv_exchanged)
	{
	  /*
	   * OK, messages have been once correctly signed, but
	   * this one is *badly* signed.
	   */
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("bad sig from \"%s\""),
		      remote_id_str);
	}
      else
	{
	  /*
	   * Peer probably never got the ticket, so for now we 
	   * ignore signatures.
	   */
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("not checking sig for \"%s\" because ticket was never received"),
		      remote_id_str);
	  ret = 1;
	}
    }

  return ret;
}

void
_lw6p2p_recv_callback (void *recv_callback_data,
		       void *connection,
		       u_int32_t physical_ticket_sig,
		       u_int32_t logical_ticket_sig,
		       u_int64_t logical_from_id, u_int64_t logical_to_id,
		       char *message)
{
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) recv_callback_data;
  lw6cnx_connection_t *cnx = (lw6cnx_connection_t *) connection;
  int physical_sig_ok = 0;
  int logical_sig_ok = 0;
  char *logical_from_id_str = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("recv_callback msg=\"%s\""), message);
  if (node && cnx)
    {
      /*
       * We filter here, for it's node the backend responsibility
       * to do it, and there's no way to trap it in generic cli/srv
       * code.
       */
      if (lw6cnx_connection_reliability_filter (cnx))
	{
	  physical_sig_ok =
	    _check_sig (&(node->ticket_table), cnx->remote_id_int,
			cnx->local_id_int, cnx->remote_id_str, message,
			physical_ticket_sig);
	  if (physical_sig_ok)
	    {
	      if (cnx->local_id_int == logical_to_id)
		{
		  if (logical_from_id == cnx->remote_id_int)
		    {
		      /*
		       * In this case, test was performed just before...
		       */
		      logical_sig_ok = 1;
		    }
		  else
		    {
		      logical_from_id_str = lw6sys_id_ltoa (logical_from_id);
		      if (logical_from_id_str)
			{
			  logical_sig_ok =
			    _check_sig (&(node->ticket_table),
					logical_from_id, logical_to_id,
					logical_from_id_str, message,
					logical_ticket_sig);
			  LW6SYS_FREE (logical_from_id_str);
			}
		    }
		  if (logical_sig_ok)
		    {
		      _lw6p2p_recv_process (node, cnx, logical_from_id,
					    message);
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _x_ ("bad logical ticket_sig from \"%s\""),
				  cnx->remote_url);
		    }
		}
	      else
		{
		  if (cnx->remote_id_int == logical_from_id)
		    {
		      _lw6p2p_recv_forward (node, cnx, logical_ticket_sig,
					    logical_from_id, logical_to_id,
					    message);
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_ ("open relay not allowed (yet)"));
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_ ("bad physical ticket_sig from \"%s\""),
			  cnx->remote_url);
	    }
	}
    }
}
