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

void
_mod_udpd_close (_udpd_context_t * udpd_context,
		 lw6srv_connection_t * connection)
{
  // todo
}

int
_mod_udpd_is_alive (_udpd_context_t * udpd_context,
		    lw6srv_connection_t * connection)
{
  int ret = 0;

  // todo

  return ret;
}

int
_mod_udpd_timeout_ok (_udpd_context_t * udpd_context,
		      int64_t origin_timestamp)
{
  int ret = 0;
  int d = 0;

  d = origin_timestamp + (_MOD_UDPD_TIMEOUT * 1000) - lw6sys_get_timestamp ();
  ret = (d > 0);

  return ret;
}
