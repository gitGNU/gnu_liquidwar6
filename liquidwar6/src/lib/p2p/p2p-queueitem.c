/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

_lw6p2p_queue_item_t *
_lw6p2p_queue_item_new (u_int32_t logical_ticket_sig,
			u_int64_t logical_from_id,
			u_int64_t logical_to_id, const char *msg)
{
  _lw6p2p_queue_item_t *ret = NULL;

  ret =
    (_lw6p2p_queue_item_t *) LW6SYS_CALLOC (sizeof (_lw6p2p_queue_item_t));
  if (ret)
    {
      ret->logical_ticket_sig = logical_ticket_sig;
      ret->logical_from_id = logical_from_id;
      ret->logical_to_id = logical_to_id;
      ret->msg = lw6sys_str_copy (msg);
      if (!(ret->msg))
	{
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_lw6p2p_queue_item_free (_lw6p2p_queue_item_t * queue_item)
{
  if (queue_item)
    {
      if (queue_item->msg)
	{
	  LW6SYS_FREE (queue_item->msg);
	}
      LW6SYS_FREE (queue_item);
    }
}
