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

#include "sys.h"
#include "sys-internal.h"

#define _HTTP_PORT 80
#define _HTTP_URL_MASK "http://%s:%d/"
#define _HTTP_URL_MASK_PORT_80 "http://%s/"

/**
 * lw6sys_url_http_from_ip_port
 *
 * @ip: IP address
 * @port: IP port
 *
 * Returns an http URL pointing to ip:port that is, adds a heading
 * http:// and a trailing /, and treats port 80 as default. This
 * is used to create public_url in net modules.
 *
 * Return value: a newly allocated string, NULL on error.
 */
extern char *
lw6sys_url_http_from_ip_port (char *ip, int port)
{
  char *ret = NULL;

  if (port == _HTTP_PORT)
    {
      ret = lw6sys_new_sprintf (_HTTP_URL_MASK_PORT_80, ip);
    }
  else
    {
      ret = lw6sys_new_sprintf (_HTTP_URL_MASK, ip, port);
    }

  return ret;
}
