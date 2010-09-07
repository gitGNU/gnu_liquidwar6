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
  lw6sys_log (LW6SYS_LOG_NOTICE, _("process \"%s\""), message);
}

void
_lw6p2p_recv_forward (_lw6p2p_node_t * node,
		      lw6cnx_connection_t * cnx,
		      u_int32_t logical_ticket_sig,
		      u_int64_t logical_from_id, u_int64_t logical_to_id,
		      char *message)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _("forward \"%s\""), message);
  // todo, forwarding
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
  u_int32_t recv_ticket = 0;
  int logical_sig_ok = 0;
  char *logical_from_id_str = NULL;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("recv_callback msg=\"%s\""), message);
  if (node && cnx)
    {
      recv_ticket =
	lw6cnx_ticket_table_get_recv (&(node->ticket_table),
				      cnx->remote_id_str);
      if ((!lw6cnx_ticket_table_was_recv_exchanged
	   (&node->ticket_table, cnx->remote_id_str))
	  || lw6msg_ticket_check_sig (recv_ticket, cnx->remote_id_int,
				      cnx->local_id_int, message,
				      physical_ticket_sig))
	{
	  if (recv_ticket)
	    {
	      TMP ("physical ticket OK");
	    }
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
		      recv_ticket =
			lw6cnx_ticket_table_get_recv (&(node->ticket_table),
						      logical_from_id_str);
		      if ((!lw6cnx_ticket_table_was_recv_exchanged
			   (&node->ticket_table, logical_from_id_str))
			  || lw6msg_ticket_check_sig (recv_ticket,
						      logical_from_id,
						      logical_to_id, message,
						      logical_ticket_sig))
			{
			  if (recv_ticket)
			    {
			      TMP ("logical ticket OK");
			    }
			}
		      LW6SYS_FREE (logical_from_id_str);
		    }
		}
	      if (logical_sig_ok)
		{
		  _lw6p2p_recv_process (node, cnx, logical_from_id, message);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _("bad logical ticket_sig from \"%s\""),
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
			      _("open relay not allowed (yet)"));
		}
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("bad physical ticket_sig from \"%s\""),
		      cnx->remote_url);
	}
    }
}
