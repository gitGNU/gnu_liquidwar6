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

#include "cnx.h"

/**
 * lw6cnx_packet_new
 *
 * @sys_context: global system context
 * @logical_ticket_sig: logical signature
 * @physical_ticket_sig: physical signature
 * @logical_from_id: logical sender
 * @logical_to_id: logical receiver
 * @msg: the message text
 *
 * Creates a new packet object, this simply allocates
 * memory and copy the string. The string is duplicated,
 * param msg can be freed.
 *
 * Return value: new object.
 */
lw6cnx_packet_t *
lw6cnx_packet_new (lw6sys_context_t * sys_context, u_int32_t logical_ticket_sig, u_int32_t physical_ticket_sig, u_int64_t logical_from_id,
		   u_int64_t logical_to_id, const char *msg)
{
  lw6cnx_packet_t *ret = NULL;

  ret = (lw6cnx_packet_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6cnx_packet_t));
  if (ret)
    {
      ret->logical_ticket_sig = logical_ticket_sig;
      ret->physical_ticket_sig = physical_ticket_sig;
      ret->logical_from_id = logical_from_id;
      ret->logical_to_id = logical_to_id;
      ret->msg = lw6sys_str_copy (sys_context, msg);
      if (!(ret->msg))
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6cnx_packet_free
 *
 * @sys_context: global system context
 * @packet: object to free
 *
 * Frees a packet object.
 *
 * Return value: none.
 */
void
lw6cnx_packet_free (lw6sys_context_t * sys_context, lw6cnx_packet_t * packet)
{
  if (packet)
    {
      if (packet->msg)
	{
	  LW6SYS_FREE (sys_context, packet->msg);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("freeing packet with NULL msg"));
	}
      LW6SYS_FREE (sys_context, packet);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL packet"));
    }
}

/**
 * lw6cnx_packet_checksum
 *
 * @sys_context: global system context
 * @packet: packet to analyse
 *
 * Calculates a checksum for a packet.
 *
 * Return value: 32-bit checksum.
 */
u_int32_t
lw6cnx_packet_checksum (lw6sys_context_t * sys_context, const lw6cnx_packet_t * packet)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update_int32 (sys_context, &ret, packet->logical_ticket_sig);
  lw6sys_checksum_update_int32 (sys_context, &ret, packet->physical_ticket_sig);
  lw6sys_checksum_update_int64 (sys_context, &ret, packet->logical_from_id);
  lw6sys_checksum_update_int64 (sys_context, &ret, packet->logical_to_id);
  lw6sys_checksum_update_str (sys_context, &ret, packet->msg);

  return ret;
}

/**
 * lw6cnx_packet_compare
 *
 * @sys_context: global system context
 * @a: first packet to analyse
 * @b: second packet to analyse
 *
 * Compares two packets. The comparison is made using checksums,
 * the result is a strcmp-like integer, which is adapted to sorting.
 * The order has no real signification, it's mostly used to re-order
 * packets in a pseudo-random order, to ensure no algorithm relies
 * on packets arriving in the right order.
 *
 * Return value: -1 if a lower than b, 0 if a equals b, 1 if a is greater than b.
 */
int
lw6cnx_packet_compare (lw6sys_context_t * sys_context, const lw6cnx_packet_t * a, const lw6cnx_packet_t * b)
{
  int ret = 0;
  u_int32_t checksum_a = lw6cnx_packet_checksum (sys_context, a);
  u_int32_t checksum_b = lw6cnx_packet_checksum (sys_context, b);

  ret = (checksum_a == checksum_b) ? 0 : ((checksum_a < checksum_b) ? -1 : +1);

  return ret;
}

/**
 * lw6cnx_packet_sort_callback
 *
 * @sys_context: global system context
 * @func_data: additionnal data, function specific
 * @ptr_a: first packet to analyse
 * @ptr_b: second packet to analyse
 *
 * Callback usable to sort packets, relies on @lw6cnx_packet_compare internally.
 *
 * Return value: -1 if ptr_a lower than ptr_b, 0 if ptr_a equals ptr_b, 1 if ptr_a is greater than ptr_b.
 */
int
lw6cnx_packet_sort_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;

  ret = lw6cnx_packet_compare (sys_context, (lw6cnx_packet_t *) ptr_a, (lw6cnx_packet_t *) ptr_b);

  return ret;
}
