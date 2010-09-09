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

#include "cnx.h"

/**
 * lw6cnx_ticket_table_zero
 *
 * @ticket_table: the ticket table to fill with zero
 *
 * Fills the ticket table struct with 0s.
 *
 * Return value: none.
 */
void
lw6cnx_ticket_table_zero (lw6cnx_ticket_table_t * ticket_table)
{
  memset (ticket_table, 0, sizeof (lw6cnx_ticket_table_t));
}

/**
 * lw6cnx_ticket_table_init
 *
 * @ticket_table: the ticket table to init
 * @hash_size: the hash size for both recv/send hashs
 *
 * Initialize a ticket table, that is, set it up with two
 * empty hashs. Recv hash is filled automatically as it's queried
 * for tickets, send hash must be filled explicitely with info 
 * from the network.
 *
 * Return value: none.
 */
int
lw6cnx_ticket_table_init (lw6cnx_ticket_table_t * ticket_table, int hash_size)
{
  int ret = 0;

  ticket_table->recv_spinlock = lw6sys_spinlock_create ();
  ticket_table->recv_ack_spinlock = lw6sys_spinlock_create ();
  ticket_table->send_spinlock = lw6sys_spinlock_create ();
  ticket_table->recv_table =
    lw6sys_hash_new (lw6sys_free_callback, hash_size);
  ticket_table->recv_ack_table = lw6sys_hash_new (NULL, hash_size);
  ticket_table->send_table =
    lw6sys_hash_new (lw6sys_free_callback, hash_size);

  ret = (ticket_table->recv_spinlock && ticket_table->recv_ack_spinlock
	 && ticket_table->send_spinlock && ticket_table->recv_table
	 && ticket_table->recv_ack_table && ticket_table->send_table);
  if (!ret)
    {
      lw6cnx_ticket_table_clear (ticket_table);
    }

  return ret;
}

/**
 * lw6cnx_ticket_table_clear
 *
 * @ticket_table: the ticket table to clear
 *
 * Clears the object (frees memory).
 *
 * Return value: none.
 */
void
lw6cnx_ticket_table_clear (lw6cnx_ticket_table_t * ticket_table)
{
  if (ticket_table->recv_spinlock)
    {
      lw6sys_spinlock_destroy (ticket_table->recv_spinlock);
    }
  if (ticket_table->recv_ack_spinlock)
    {
      lw6sys_spinlock_destroy (ticket_table->recv_ack_spinlock);
    }
  if (ticket_table->send_spinlock)
    {
      lw6sys_spinlock_destroy (ticket_table->send_spinlock);
    }
  if (ticket_table->recv_table)
    {
      lw6sys_hash_free (ticket_table->recv_table);
    }
  if (ticket_table->recv_ack_table)
    {
      lw6sys_hash_free (ticket_table->recv_ack_table);
    }
  if (ticket_table->send_table)
    {
      lw6sys_hash_free (ticket_table->send_table);
    }
  lw6cnx_ticket_table_zero (ticket_table);
}

/**
 * lw6cnx_ticket_table_get_recv
 *
 * @ticket_table: the ticket table to query
 * @peer_id: the id of remote node
 *
 * Gets the ticket used to communicate with peer, to check its incoming
 * (recv) messages. If ticket does not exist yet, it's automatically
 * generated so tunction will always return a non-zero value.
 *
 * Return value: the ticket used to check incoming messages.
 */
u_int64_t
lw6cnx_ticket_table_get_recv (lw6cnx_ticket_table_t * ticket_table,
			      char *peer_id)
{
  u_int64_t recv_ticket = 0;
  u_int64_t *recv_ticket_ptr = NULL;

  if (lw6sys_spinlock_lock (ticket_table->recv_spinlock))
    {
      recv_ticket_ptr = lw6sys_hash_get (ticket_table->recv_table, peer_id);
      lw6sys_spinlock_unlock (ticket_table->recv_spinlock);
    }

  if (recv_ticket_ptr)
    {
      recv_ticket = (*recv_ticket_ptr);
    }
  else
    {
      recv_ticket = lw6sys_generate_id_64 ();
      recv_ticket_ptr = (u_int64_t *) LW6SYS_MALLOC (sizeof (u_int64_t));
      if (recv_ticket_ptr)
	{
	  (*recv_ticket_ptr) = recv_ticket;
	  if (lw6sys_spinlock_lock (ticket_table->recv_spinlock))
	    {
	      lw6sys_hash_set (ticket_table->recv_table, peer_id,
			       recv_ticket_ptr);
	      lw6sys_spinlock_unlock (ticket_table->recv_spinlock);
	    }
	}
    }

  return recv_ticket;
}

/**
 * lw6cnx_ticket_table_ack_recv
 *
 * @ticket_table: the ticket table to query
 * @peer_id: the id of remote node
 *
 * Acknowledges the ticket used to communicate with peer, to check its incoming
 * (recv) messages has been received. This is to avoid sending it again when
 * it has been received, as it's kept "forever" by peer, we never need to
 * send it again.
 *
 * Return value: none.
 */
void
lw6cnx_ticket_table_ack_recv (lw6cnx_ticket_table_t * ticket_table,
			      char *peer_id)
{
  if (lw6sys_spinlock_lock (ticket_table->recv_ack_spinlock))
    {
      lw6sys_hash_set (ticket_table->recv_ack_table, peer_id, NULL);
      lw6sys_spinlock_unlock (ticket_table->recv_ack_spinlock);
    }
}

/**
 * lw6cnx_ticket_table_was_recv_exchanged
 *
 * @ticket_table: the ticket table to query
 * @peer_id: the id of remote node
 *
 * Acknowledges the ticket used to communicate with peer, to check its incoming
 * (recv) messages has been received. This is to avoid sending it again when
 * it has been received, as it's kept "forever" by peer, we never need to
 * send it again.
 *
 * Return value: the ticket used to check incoming messages.
 */
int
lw6cnx_ticket_table_was_recv_exchanged (lw6cnx_ticket_table_t * ticket_table,
					char *peer_id)
{
  int ret = 0;

  if (lw6sys_spinlock_lock (ticket_table->recv_ack_spinlock))
    {
      ret = lw6sys_hash_has_key (ticket_table->recv_ack_table, peer_id);
      lw6sys_spinlock_unlock (ticket_table->recv_ack_spinlock);
    }

  return ret;
}

/**
 * lw6cnx_ticket_table_get_send
 *
 * @ticket_table: the ticket table to query
 * @peer_id: the id of remote node
 *
 * Gets the ticket used to communicate with peer, to stamp the outgoing
 * messages. If ticket does not exist yet, 0 is returned, indeed this value
 * must be initialized with the value the peer gives us.
 *
 * Return value: the ticket used to stamp outgoing messages.
 */
u_int64_t
lw6cnx_ticket_table_get_send (lw6cnx_ticket_table_t * ticket_table,
			      char *peer_id)
{
  u_int64_t send_ticket = 0;
  u_int64_t *send_ticket_ptr = NULL;

  if (lw6sys_spinlock_lock (ticket_table->send_spinlock))
    {
      send_ticket_ptr = lw6sys_hash_get (ticket_table->send_table, peer_id);
      lw6sys_spinlock_unlock (ticket_table->send_spinlock);
    }

  if (send_ticket_ptr)
    {
      send_ticket = (*send_ticket_ptr);
    }
  else
    {
      send_ticket = 0;
    }

  return send_ticket;
}

/**
 * lw6cnx_ticket_table_set_send
 *
 * @ticket_table: the ticket table to query
 * @peer_id: the id of remote node
 * @send_ticket: the ticket to use to stamp outgoing messages
 *
 * Sets the ticket used to communicate with peer, to stamp the outgoing (send)
 * messages. This value should be received from the network. Note that
 * once it's set, it's impossible to change it, it will remain the same
 * for the whole duration of the node existence.
 *
 * Return value: NULL
 */
void
lw6cnx_ticket_table_set_send (lw6cnx_ticket_table_t * ticket_table,
			      char *peer_id, u_int64_t send_ticket)
{
  u_int64_t *send_ticket_ptr = NULL;

  if (lw6sys_spinlock_lock (ticket_table->send_spinlock))
    {
      send_ticket_ptr = lw6sys_hash_get (ticket_table->send_table, peer_id);
      lw6sys_spinlock_unlock (ticket_table->send_spinlock);
    }

  if (!send_ticket_ptr)
    {
      send_ticket_ptr = (u_int64_t *) LW6SYS_MALLOC (sizeof (u_int64_t));
      if (send_ticket_ptr)
	{
	  (*send_ticket_ptr) = send_ticket;
	  if (lw6sys_spinlock_lock (ticket_table->send_spinlock))
	    {
	      lw6sys_hash_set (ticket_table->send_table, peer_id,
			       send_ticket_ptr);
	      lw6sys_spinlock_unlock (ticket_table->send_spinlock);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("send_ticket for %s already exists, ignoring set"),
		  peer_id);
    }
}
