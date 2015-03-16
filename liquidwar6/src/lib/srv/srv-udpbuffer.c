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
 * lw6srv_udp_buffer_new
 *
 * @sys_context: global system context
 * @client_ip: the client ip, will be freed when object is freed, do not free it
 * @client_port: the client port
 * @line: the line received, will be freed when object is freed, do not free it
 *
 * Creates an udp_buffer object.
 *
 * Return value: none
 */
lw6srv_udp_buffer_t *
lw6srv_udp_buffer_new (lw6sys_context_t * sys_context, char *client_ip, int client_port, char *line)
{
  lw6srv_udp_buffer_t *ret = NULL;

  ret = (lw6srv_udp_buffer_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6srv_udp_buffer_t));
  if (ret)
    {
      ret->client_id.client_ip = client_ip;	// we'll destroy this ourselves
      ret->client_id.client_port = client_port;
      ret->line = line;		// we'll destroy this ourselves
    }

  return ret;
}

/**
 * lw6srv_udp_buffer_free
 *
 * @sys_context: global system context
 * @udp_buffer: the object to free
 *
 * Frees a udp_buffer object.
 *
 * Return value: none
 */
void
lw6srv_udp_buffer_free (lw6sys_context_t * sys_context, lw6srv_udp_buffer_t * udp_buffer)
{
  if (udp_buffer->client_id.client_ip)
    {
      LW6SYS_FREE (sys_context, udp_buffer->client_id.client_ip);
    }
  if (udp_buffer->line)
    {
      LW6SYS_FREE (sys_context, udp_buffer->line);
    }
  LW6SYS_FREE (sys_context, udp_buffer);
}
