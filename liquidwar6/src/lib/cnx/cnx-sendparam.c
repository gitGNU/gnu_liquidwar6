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
 * lw6cnx_send_param_new
 *
 * @physical_ticket_sig: the signature associated to physical sender/receiver
 * @logical_ticket_sig: the signature associated to logical sender/receiver
 * @logical_from_id: the id of the message creator
 * @logical_to_id: the id of the message final target
 * @message: the message
 *
 * Creates a send_param object, this object can typically be used by
 * connections which need to keep messages in a buffer before sending
 * them for good. Message is copied internally so source can be deleted.
 *
 * Return value: newly allocated object.
 */
lw6cnx_send_param_t *
lw6cnx_send_param_new (u_int32_t physical_ticket_sig,
		       u_int32_t logical_ticket_sig,
		       u_int64_t logical_from_id,
		       u_int64_t logical_to_id, char *message)
{
  lw6cnx_send_param_t *send_param = NULL;

  send_param =
    (lw6cnx_send_param_t *) LW6SYS_CALLOC (sizeof (lw6cnx_send_param_t));
  if (send_param)
    {
      send_param->physical_ticket_sig = physical_ticket_sig;
      send_param->logical_ticket_sig = logical_ticket_sig;
      send_param->logical_from_id = logical_from_id;
      send_param->logical_to_id = logical_to_id;
      send_param->message = lw6sys_str_copy (message);
      if (!send_param->message)
	{
	  LW6SYS_FREE (send_param);
	  send_param = NULL;
	}
    }

  return send_param;
}

/**
 * lw6cnx_send_param_free
 *
 * @send_param: the object to free
 *
 * Frees a send_param object, this object can typically be used by
 * connections which need to keep messages in a buffer before sending
 * them for good.
 *
 * Return value: none.
 */
void
lw6cnx_send_param_free (lw6cnx_send_param_t * send_param)
{
  if (send_param)
    {
      if (send_param->message)
	{
	  LW6SYS_FREE (send_param->message);
	}
      LW6SYS_FREE (send_param);
    }
}
