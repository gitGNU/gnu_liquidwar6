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

#include "cli.h"

static void
_warning (const char *func_name)
{
  lw6sys_log (LW6SYS_LOG_WARNING,
	      _x_ ("cli backend function \"%s\" is not defined"), func_name);
}

int
lw6cli_init (lw6cli_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->init)
    {
      backend->cli_context = backend->init (backend->argc, backend->argv);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->cli_context ? 1 : 0;
}

void
lw6cli_quit (lw6cli_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->quit)
    {
      /*
       * It's important to check that backend is not NULL for
       * quit can *really* be called several times on the same backend
       */
      if (backend->cli_context)
	{
	  backend->quit (backend->cli_context);
	  backend->cli_context = NULL;
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6cli_process_oob (lw6cli_backend_t * backend,
		    lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->process_oob)
    {
      ret = backend->process_oob (backend->cli_context, node_info, oob_data);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

lw6cnx_connection_t *
lw6cli_open (lw6cli_backend_t * backend, char *local_url, char *remote_url,
	     char *remote_ip, int remote_port,
	     char *password, u_int64_t local_id, u_int64_t remote_id,
	     int dns_ok, lw6cnx_recv_callback_t recv_callback_func,
	     void *recv_callback_data)
{
  lw6cnx_connection_t *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->open)
    {
      ret =
	backend->open (backend->cli_context, local_url, remote_url, remote_ip,
		       remote_port, password, local_id, remote_id, dns_ok,
		       recv_callback_func, recv_callback_data);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

void
lw6cli_close (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->close)
    {
      backend->close (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6cli_send (lw6cli_backend_t * backend, lw6cnx_connection_t * connection,
	     u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
	     u_int64_t logical_from_id,
	     u_int64_t logical_to_id, char *message)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->send)
    {
      ret =
	backend->send (backend->cli_context, connection, physical_ticket_sig,
		       logical_ticket_sig, logical_from_id, logical_to_id,
		       message);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

void
lw6cli_poll (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->poll)
    {
      backend->poll (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6cli_is_alive (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->is_alive)
    {
      ret = backend->is_alive (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

char *
lw6cli_repr (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

char *
lw6cli_error (lw6cli_backend_t * backend, lw6cnx_connection_t * connection)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->error)
    {
      ret = backend->error (backend->cli_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
