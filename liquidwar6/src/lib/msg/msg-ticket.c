/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "msg.h"

/**
 * lw6msg_ticket_calc_sig
 *
 * @ticket: the (private) ticket to use
 * @from_id: the sender/creator
 * @to_id: the receiver/target
 * @msg: the message to sign
 * 
 * Produces a little signature, which is clearly
 * vulnerable to brute-force attacks but makes it
 * possible to be 100% sure if it's wrong, someone
 * is trying to do something nasty (or there's a 
 * serious bug!).
 *
 * Return value: the sig, always non-zero
 */
u_int32_t
lw6msg_ticket_calc_sig (u_int64_t ticket, u_int64_t from_id, u_int64_t to_id,
			char *msg)
{
  u_int32_t calc_sig = 0;
  unsigned char key_buf[3 * sizeof (u_int64_t)];

  memset (key_buf, 0, 3 * sizeof (u_int64_t));
  lw6sys_serialize_int64 (key_buf, ticket);
  lw6sys_serialize_int64 (key_buf + sizeof (u_int64_t), from_id);
  lw6sys_serialize_int64 (key_buf + 2 * sizeof (u_int64_t), to_id);
  calc_sig =
    lw6glb_sha1_hmac_32_bin ((char *) key_buf,
			     3 * sizeof (u_int64_t), msg, strlen (msg));
  if (!calc_sig)
    {
      /*
       * Never return a 0 sig, as 0 means "no sig"
       */
      calc_sig--;
    }

  return calc_sig;
}

/**
 * lw6msg_ticket_check_sig
 *
 * @ticket: the (private) ticket to use
 * @from_id: the sender/creator
 * @to_id: the receiver/target
 * @msg: the message to sign
 * @ticket_sig: the signature to check against
 * 
 * Checks a sig is OK.
 *
 * Return value: 1 if they are the same, 0 if not.
 */
int
lw6msg_ticket_check_sig (u_int64_t ticket, u_int64_t from_id, u_int64_t to_id,
			 char *msg, u_int32_t ticket_sig)
{
  int ret = 0;
  u_int32_t calc_sig = 0;

  calc_sig = lw6msg_ticket_calc_sig (ticket, from_id, to_id, msg);
  ret = (calc_sig == ticket_sig);

  return ret;
}
