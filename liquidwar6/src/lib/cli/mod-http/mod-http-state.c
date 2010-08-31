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

#include "../cli.h"
#include "mod-http-internal.h"

lw6cli_connection_t *
_mod_http_open (_http_context_t * http_context, char *remote_url,
		char *password_checksum, u_int64_t local_id,
		u_int64_t remote_id)
{
  lw6cli_connection_t *ret = NULL;

  ret =
    (lw6cli_connection_t *) LW6SYS_CALLOC (sizeof (lw6cli_connection_t *));
  if (ret)
    {
      ret->remote_url = lw6sys_str_copy (remote_url);
      ret->password_checksum = lw6sys_str_copy (password_checksum);
      ret->local_id = local_id;
      ret->remote_id = remote_id;
      ret->backend_specific_data = NULL;	// todo

      if (ret->remote_url && ret->password_checksum
	  && ret->backend_specific_data)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("open http connection with \"%s\""),
		      remote_url);
	}
      else
	{
	  _mod_http_close (http_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_mod_http_close (_http_context_t * http_context,
		 lw6cli_connection_t * connection)
{
  if (connection->remote_url)
    {
      LW6SYS_FREE (connection->remote_url);
    }
  if (connection->password_checksum)
    {
      LW6SYS_FREE (connection->password_checksum);
    }
  if (connection->backend_specific_data)
    {
      // todo
    }
  LW6SYS_FREE (connection);
}

int
_mod_http_is_alive (_http_context_t * http_context,
		    lw6cli_connection_t * connection)
{
  int ret = 0;

  // todo

  return ret;
}

int
_mod_http_timeout_ok (_http_context_t * http_context,
		      int64_t origin_timestamp)
{
  int ret = 0;
  int d = 0;

  d =
    origin_timestamp + (http_context->data.consts.global_timeout * 1000) -
    lw6sys_get_timestamp ();
  ret = (d > 0);

  return ret;
}
