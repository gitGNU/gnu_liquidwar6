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
#endif

#include "cnx.h"

/**
 * lw6cnx_ticket_table_zero
 *
 * @sys_context: global system context
 * @ticket_table: the ticket table to fill with zero
 *
 * Fills the ticket table struct with 0s.
 *
 * Return value: none.
 */
void
lw6cnx_ticket_table_zero (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table)
{
  memset (ticket_table, 0, sizeof (lw6cnx_ticket_table_t));
}

/**
 * lw6cnx_ticket_table_init
 *
 * @sys_context: global system context
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
lw6cnx_ticket_table_init (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table, int hash_size)
{
  int ret = 0;

  ticket_table->recv_spinlock = lw6sys_spinlock_create (sys_context);
  ticket_table->recv_ack_spinlock = lw6sys_spinlock_create (sys_context);
  ticket_table->send_spinlock = lw6sys_spinlock_create (sys_context);
  ticket_table->recv_table = lw6sys_hash_new (sys_context, lw6sys_free_callback, hash_size);
  ticket_table->recv_ack_table = lw6sys_hash_new (sys_context, lw6sys_free_callback, hash_size);
  ticket_table->send_table = lw6sys_hash_new (sys_context, lw6sys_free_callback, hash_size);

  ret = (ticket_table->recv_spinlock && ticket_table->recv_ack_spinlock
	 && ticket_table->send_spinlock && ticket_table->recv_table && ticket_table->recv_ack_table && ticket_table->send_table);
  if (!ret)
    {
      lw6cnx_ticket_table_clear (sys_context, ticket_table);
    }

  return ret;
}

/**
 * lw6cnx_ticket_table_clear
 *
 * @sys_context: global system context
 * @ticket_table: the ticket table to clear
 *
 * Clears the object (frees memory).
 *
 * Return value: none.
 */
void
lw6cnx_ticket_table_clear (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table)
{
  if (ticket_table->recv_spinlock)
    {
      lw6sys_spinlock_destroy (sys_context, ticket_table->recv_spinlock);
    }
  if (ticket_table->recv_ack_spinlock)
    {
      lw6sys_spinlock_destroy (sys_context, ticket_table->recv_ack_spinlock);
    }
  if (ticket_table->send_spinlock)
    {
      lw6sys_spinlock_destroy (sys_context, ticket_table->send_spinlock);
    }
  if (ticket_table->recv_table)
    {
      lw6sys_hash_free (sys_context, ticket_table->recv_table);
    }
  if (ticket_table->recv_ack_table)
    {
      lw6sys_hash_free (sys_context, ticket_table->recv_ack_table);
    }
  if (ticket_table->send_table)
    {
      lw6sys_hash_free (sys_context, ticket_table->send_table);
    }
  lw6cnx_ticket_table_zero (sys_context, ticket_table);
}

/**
 * lw6cnx_ticket_table_get_recv
 *
 * @sys_context: global system context
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
lw6cnx_ticket_table_get_recv (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table, const char *peer_id)
{
  u_int64_t recv_ticket = 0;
  u_int64_t *recv_ticket_ptr = NULL;

  if (lw6sys_spinlock_lock (sys_context, ticket_table->recv_spinlock))
    {
      recv_ticket_ptr = lw6sys_hash_get (sys_context, ticket_table->recv_table, peer_id);
      lw6sys_spinlock_unlock (sys_context, ticket_table->recv_spinlock);
    }

  if (recv_ticket_ptr)
    {
      recv_ticket = (*recv_ticket_ptr);
    }
  else
    {
      recv_ticket = lw6sys_generate_id_64 (sys_context);
      recv_ticket_ptr = (u_int64_t *) LW6SYS_MALLOC (sys_context, sizeof (u_int64_t));
      if (recv_ticket_ptr)
	{
	  (*recv_ticket_ptr) = recv_ticket;
	  if (lw6sys_spinlock_lock (sys_context, ticket_table->recv_spinlock))
	    {
	      lw6sys_hash_set (sys_context, ticket_table->recv_table, peer_id, recv_ticket_ptr);
	      lw6sys_spinlock_unlock (sys_context, ticket_table->recv_spinlock);
	    }
	}
    }

  return recv_ticket;
}

/**
 * lw6cnx_ticket_table_ack_recv
 *
 * @sys_context: global system context
 * @ticket_table: the ticket table to query
 * @peer_id: the id of remote node
 * @ack_delay_msec: delay before which we'll consider the ticket as really received
 *
 * Acknowledges the ticket used to communicate with peer, to check its incoming
 * (recv) messages has been received. This is to avoid sending it again when
 * it has been received, as it's kept "forever" by peer, we never need to
 * send it again. The delay is here to avoid checking tickets too quickly,
 * for instance one could have sent the ticket yet, but for some reason some
 * unsigned messages are still in the pipe, typically they transit through
 * another slow channel such as httpd while the ticket was sent on udp.
 *
 * Return value: none.
 */
void
lw6cnx_ticket_table_ack_recv (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table, const char *peer_id, int ack_delay_msec)
{
  int64_t *limit = NULL;
  int useless = 0;

  limit = (int64_t *) LW6SYS_MALLOC (sys_context, sizeof (int64_t));
  if (limit)
    {
      (*limit) = lw6sys_get_timestamp (sys_context) + ack_delay_msec;
      if (lw6sys_spinlock_lock (sys_context, ticket_table->recv_ack_spinlock))
	{
	  /*
	   * If there was already something received, we don't acknowledge
	   * it again, else on regular sends the limit would be endlessly
	   * updated, thus checks would never be performed.
	   */
	  if (!lw6sys_hash_has_key (sys_context, ticket_table->recv_ack_table, peer_id))
	    {
	      lw6sys_hash_set (sys_context, ticket_table->recv_ack_table, peer_id, (void *) limit);
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, limit);
	      useless = 1;
	    }
	  lw6sys_spinlock_unlock (sys_context, ticket_table->recv_ack_spinlock);
	  if (!useless)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("acknowledging ticket for peer_id=%s, ack_delay_msec=%d"), peer_id, ack_delay_msec);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("acknowledging ticket for peer_id=%s, but this is useless, we already got one"), peer_id);
	    }
	}
      else
	{
	  LW6SYS_FREE (sys_context, limit);
	}
    }
}

/**
 * lw6cnx_ticket_table_was_recv_exchanged
 *
 * @sys_context: global system context
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
lw6cnx_ticket_table_was_recv_exchanged (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table, const char *peer_id)
{
  int ret = 0;
  int64_t now;
  int64_t *limit;

  now = lw6sys_get_timestamp (sys_context);
  if (lw6sys_spinlock_lock (sys_context, ticket_table->recv_ack_spinlock))
    {
      limit = (int64_t *) lw6sys_hash_get (sys_context, ticket_table->recv_ack_table, peer_id);
      if (limit)
	{
	  if ((*limit) <= now)
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("ticket for \"%s\" exists but is too recent, still %d msec"), peer_id, (int) ((*limit) - now));
	    }
	}
      lw6sys_spinlock_unlock (sys_context, ticket_table->recv_ack_spinlock);
    }

  return ret;
}

/**
 * lw6cnx_ticket_table_get_send
 *
 * @sys_context: global system context
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
lw6cnx_ticket_table_get_send (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table, const char *peer_id)
{
  u_int64_t send_ticket = 0;
  u_int64_t *send_ticket_ptr = NULL;

  if (lw6sys_spinlock_lock (sys_context, ticket_table->send_spinlock))
    {
      send_ticket_ptr = lw6sys_hash_get (sys_context, ticket_table->send_table, peer_id);
      lw6sys_spinlock_unlock (sys_context, ticket_table->send_spinlock);
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
 * @sys_context: global system context
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
lw6cnx_ticket_table_set_send (lw6sys_context_t * sys_context, lw6cnx_ticket_table_t * ticket_table, const char *peer_id, u_int64_t send_ticket)
{
  u_int64_t *send_ticket_ptr = NULL;

  if (lw6sys_spinlock_lock (sys_context, ticket_table->send_spinlock))
    {
      send_ticket_ptr = lw6sys_hash_get (sys_context, ticket_table->send_table, peer_id);
      lw6sys_spinlock_unlock (sys_context, ticket_table->send_spinlock);
    }

  if (!send_ticket_ptr)
    {
      send_ticket_ptr = (u_int64_t *) LW6SYS_MALLOC (sys_context, sizeof (u_int64_t));
      if (send_ticket_ptr)
	{
	  (*send_ticket_ptr) = send_ticket;
	  if (lw6sys_spinlock_lock (sys_context, ticket_table->send_spinlock))
	    {
	      lw6sys_hash_set (sys_context, ticket_table->send_table, peer_id, send_ticket_ptr);
	      lw6sys_spinlock_unlock (sys_context, ticket_table->send_spinlock);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("send_ticket for %s already exists, ignoring set"), peer_id);
    }
}
