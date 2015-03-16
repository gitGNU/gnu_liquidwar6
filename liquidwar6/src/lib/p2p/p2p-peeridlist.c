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

#include "p2p.h"
#include "p2p-internal.h"

static void
_peer_id_list_process_join_callback (lw6sys_context_t * sys_context, void *func_data, u_int64_t id)
{
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  int serial = 1;		// should match LW6DAT_SERIAL_START
  int64_t seq = 0LL;

  /*
   * If it's already register, don't do anything, this is just a last
   * chance update in case the peer is unreachable, but if we already
   * had informations through another channel -> don't do anything
   */
  if (!lw6dat_warehouse_is_node_registered (sys_context, node->warehouse, id))
    {
      /*
       * We register with low values for serial and seq, those could
       * technically be updated later, but for sure below these, there's
       * no need to handle anything.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("following a JOIN message, registering id=%"
		       LW6SYS_PRINTF_LL "x with seq=%" LW6SYS_PRINTF_LL "d serial=%d"), (long long) id, (long long) seq, serial);
      lw6dat_warehouse_register_node (sys_context, node->warehouse, id, serial, seq);
    }
}

void
_lw6p2p_peer_id_list_process_join (lw6sys_context_t * sys_context, _lw6p2p_node_t * node, lw6nod_info_t * remote_node_info)
{
  lw6nod_info_community_id_without_url_map (sys_context, remote_node_info, _peer_id_list_process_join_callback, (void *) node);
}
