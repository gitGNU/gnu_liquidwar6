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
#include "mod-httpd-internal.h"

_httpd_request_t *
_mod_httpd_request_parse (_httpd_context_t * httpd_context,
			  lw6srv_oob_data_t * oob_data)
{
  _httpd_request_t *request = NULL;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("process httpd oob"));

  request = (_httpd_request_t *) LW6SYS_CALLOC (sizeof (_httpd_request_t));
  if (request)
    {
    }

  return request;
}

void
_mod_httpd_request_free (_httpd_request_t * request)
{
  if (request)
    {
      if (request->uri)
	{
	  LW6SYS_FREE (request->uri);
	}
      if (request->http_user)
	{
	  LW6SYS_FREE (request->http_user);
	}
      if (request->http_password)
	{
	  LW6SYS_FREE (request->http_password);
	}
    }
}
