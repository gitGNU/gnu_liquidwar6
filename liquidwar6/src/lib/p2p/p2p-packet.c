/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

_lw6p2p_packet_t *
_lw6p2p_packet_new (u_int32_t logical_ticket_sig,
		    u_int32_t physical_ticket_sig, u_int64_t logical_from_id,
		    u_int64_t logical_to_id, const char *msg)
{
  _lw6p2p_packet_t *ret = NULL;

  ret = (_lw6p2p_packet_t *) LW6SYS_CALLOC (sizeof (_lw6p2p_packet_t));
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

void
_lw6p2p_packet_free (_lw6p2p_packet_t * packet)
{
  if (packet)
    {
      if (packet->msg)
	{
	  LW6SYS_FREE (sys_context, packet->msg);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("freeing packet with NULL msg"));
	}
      LW6SYS_FREE (sys_context, packet);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("trying to free NULL packet"));
    }
}

u_int32_t
_lw6p2p_packet_checksum (const _lw6p2p_packet_t * packet)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update_int32 (&ret, packet->logical_ticket_sig);
  lw6sys_checksum_update_int32 (&ret, packet->physical_ticket_sig);
  lw6sys_checksum_update_int64 (&ret, packet->logical_from_id);
  lw6sys_checksum_update_int64 (&ret, packet->logical_to_id);
  lw6sys_checksum_update_str (&ret, packet->msg);

  return ret;
}

int
_lw6p2p_packet_compare (const _lw6p2p_packet_t * a,
			const _lw6p2p_packet_t * b)
{
  int ret = 0;
  u_int32_t checksum_a = _lw6p2p_packet_checksum (a);
  u_int32_t checksum_b = _lw6p2p_packet_checksum (b);

  ret =
    (checksum_a == checksum_b) ? 0 : ((checksum_a < checksum_b) ? -1 : +1);

  return ret;
}

int
_lw6p2p_packet_sort_callback (const lw6sys_list_t ** list_a,
			      const lw6sys_list_t ** list_b)
{
  int ret = 0;

  ret =
    _lw6p2p_packet_compare ((_lw6p2p_packet_t *) ((*list_a)->data),
			    (_lw6p2p_packet_t *) ((*list_b)->data));

  return ret;
}
