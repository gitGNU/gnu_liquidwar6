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

#include "srv.h"

static void
_warning (const char *func_name)
{
  lw6sys_log (LW6SYS_LOG_WARNING,
	      _x_ ("srv backend function \"%s\" is not defined"), func_name);
}

int
lw6srv_init (lw6srv_backend_t * backend, lw6srv_listener_t * listener)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->init)
    {
      backend->srv_context =
	backend->init (backend->argc, backend->argv, listener);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->srv_context ? 1 : 0;
}

void
lw6srv_quit (lw6srv_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->quit)
    {
      /*
       * It's important to check that backend is not NULL for
       * quit can *really* be called several times on the same backend
       */
      if (backend->srv_context)
	{
	  backend->quit (backend->srv_context);
	  backend->srv_context = NULL;
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6srv_analyse_tcp (lw6srv_backend_t * backend,
		    lw6srv_tcp_accepter_t * tcp_accepter,
		    lw6nod_info_t * node_info,
		    u_int64_t * remote_id, char **remote_url)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->analyse_tcp)
    {
      ret =
	backend->analyse_tcp (backend->srv_context, tcp_accepter, node_info,
			      remote_id, remote_url);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

int
lw6srv_analyse_udp (lw6srv_backend_t * backend,
		    lw6srv_udp_buffer_t * udp_buffer,
		    lw6nod_info_t * node_info, u_int64_t * remote_id,
		    char **remote_url)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->analyse_udp)
    {
      ret =
	backend->analyse_udp (backend->srv_context, udp_buffer, node_info,
			      remote_id, remote_url);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

int
lw6srv_process_oob (lw6srv_backend_t * backend, lw6nod_info_t * node_info,
		    lw6srv_oob_data_t * oob_data)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->process_oob)
    {
      ret = backend->process_oob (backend->srv_context, node_info, oob_data);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

lw6cnx_connection_t *
lw6srv_open (lw6srv_backend_t * backend, lw6srv_listener_t * listener,
	     char *local_url, char *remote_url, char *remote_ip,
	     int remote_port, char *password, u_int64_t local_id,
	     u_int64_t remote_id, int dns_ok, int network_reliability,
	     lw6cnx_recv_callback_t recv_callback_func,
	     void *recv_callback_data)
{
  lw6cnx_connection_t *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->open)
    {
      ret =
	backend->open (backend->srv_context, listener, local_url, remote_url,
		       remote_ip, remote_port, password, local_id, remote_id,
		       dns_ok, network_reliability, recv_callback_func,
		       recv_callback_data);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

int
lw6srv_feed_with_tcp (lw6srv_backend_t * backend,
		      lw6cnx_connection_t * connection,
		      lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->feed_with_tcp)
    {
      ret =
	backend->feed_with_tcp (backend->srv_context, connection,
				tcp_accepter);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

int
lw6srv_feed_with_udp (lw6srv_backend_t * backend,
		      lw6cnx_connection_t * connection,
		      lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->feed_with_udp)
    {
      ret =
	backend->feed_with_udp (backend->srv_context, connection, udp_buffer);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

void
lw6srv_close (lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->close)
    {
      backend->close (backend->srv_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6srv_send (lw6srv_backend_t * backend, lw6cnx_connection_t * connection,
	     u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
	     u_int64_t logical_from_id,
	     u_int64_t logical_to_id, char *message)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->send)
    {
      if (lw6cnx_connection_reliability_filter (connection))
	{
	  ret =
	    backend->send (backend->srv_context, connection,
			   physical_ticket_sig, logical_ticket_sig,
			   logical_from_id, logical_to_id, message);
	}
      else
	{
	  /*
	   * Yes, we return true, the idea is to pretend success
	   * but in fact real send failed.
	   */
	  ret = 1;
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

void
lw6srv_poll (lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->poll)
    {
      backend->poll (backend->srv_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6srv_is_alive (lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->is_alive)
    {
      ret = backend->is_alive (backend->srv_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

char *
lw6srv_repr (lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (backend->srv_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

char *
lw6srv_error (lw6srv_backend_t * backend, lw6cnx_connection_t * connection)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->error)
    {
      ret = backend->error (backend->srv_context, connection);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
