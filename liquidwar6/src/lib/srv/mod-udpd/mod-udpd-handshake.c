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

#include "../srv.h"
#include "mod-udpd-internal.h"

int
_mod_udpd_analyse_tcp (_udpd_context_t * udpd_context,
		       lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;

  /*
   * TCP is never handled by mod_udpd.
   */

  return ret;
}

int
_mod_udpd_analyse_udp (_udpd_context_t * udpd_context,
		       lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  // todo

  return ret;
}

lw6srv_connection_t *
_mod_udpd_accept_tcp (_udpd_context_t * udpd_context,
		      lw6srv_tcp_accepter_t * tcp_accepter, char *password)
{
  lw6srv_connection_t *ret = NULL;

  /*
   * TCP is never handled by mod_tcpd, return always NULL
   */

  return ret;
}

lw6srv_connection_t *
_mod_udpd_new_udp (_udpd_context_t * udpd_context,
		   lw6srv_udp_buffer_t * udp_buffer, char *password)
{
  lw6srv_connection_t *ret = NULL;

  // todo

  return ret;
}

int
_mod_udpd_is_associated_with_udp (_udpd_context_t * udpd_context,
				  lw6srv_connection_t * connection,
				  lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  // todo

  return ret;
}

int
_mod_udpd_update_with_udp (_udpd_context_t * udpd_context,
			   lw6srv_connection_t * connection,
			   lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  // todo

  return ret;
}
