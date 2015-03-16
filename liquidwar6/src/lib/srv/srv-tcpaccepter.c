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

#include "srv.h"

/**
 * lw6srv_tcp_accepter_new
 *
 * @client_ip: the client ip, will be freed when accepter is freed, do not copy it
 * @client_port: the client port
 * @sock: the socket used
 *
 * Creates a tcp_accepter object.
 *
 * Return value: none
 */
lw6srv_tcp_accepter_t *
lw6srv_tcp_accepter_new (lw6sys_context_t * sys_context, char *client_ip, int client_port, int sock)
{
  lw6srv_tcp_accepter_t *ret = NULL;

  ret = (lw6srv_tcp_accepter_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6srv_tcp_accepter_t));
  if (ret)
    {
      ret->client_id.client_ip = client_ip;	// we'll destroy this ourselves
      ret->client_id.client_port = client_port;
      ret->sock = sock;
      ret->creation_timestamp = lw6sys_get_timestamp (sys_context);
    }

  return ret;
}

/**
 * lw6srv_tcp_accepter_free
 *
 * @tcp_accepter: the object to free
 *
 * Frees a tcp_accepter object.
 *
 * Return value: none
 */
void
lw6srv_tcp_accepter_free (lw6sys_context_t * sys_context, lw6srv_tcp_accepter_t * tcp_accepter)
{
  /*
   * We don't close the socket for it might be used by
   * a spawned thread, among other possibilities.
   */
  if (tcp_accepter->client_id.client_ip)
    {
      LW6SYS_FREE (sys_context, tcp_accepter->client_id.client_ip);
    }
  LW6SYS_FREE (sys_context, tcp_accepter);
}
