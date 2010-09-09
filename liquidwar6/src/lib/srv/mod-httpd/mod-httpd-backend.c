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
#include "mod-httpd.h"
#include "mod-httpd-internal.h"

/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
void
mod_httpd_is_GPL_compatible ()
{
}

static void *
_init (int argc, char *argv[], lw6srv_listener_t * listener)
{
  _httpd_context_t *httpd_context = _mod_httpd_init (argc, argv, listener);

  return (void *) httpd_context;
}

static void
_quit (void *srv_context)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;

  if (httpd_context)
    {
      _mod_httpd_quit (httpd_context);
    }
}

static int
_analyse_tcp (void *srv_context, lw6srv_tcp_accepter_t * tcp_accepter,
	      lw6nod_info_t * node_info,
	      u_int64_t * remote_id, char **remote_url)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  int ret = 0;

  if (httpd_context)
    {
      ret =
	_mod_httpd_analyse_tcp (httpd_context, tcp_accepter, node_info,
				remote_id, remote_url);
    }

  return ret;
}

static int
_analyse_udp (void *srv_context, lw6srv_udp_buffer_t * udp_buffer,
	      lw6nod_info_t * node_info,
	      u_int64_t * remote_id, char **remote_url)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  int ret = 0;

  if (httpd_context)
    {
      ret =
	_mod_httpd_analyse_udp (httpd_context, udp_buffer, node_info,
				remote_id, remote_url);
    }

  return ret;
}

static int
_process_oob (void *srv_context, lw6nod_info_t * node_info,
	      lw6srv_oob_data_t * oob_data)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  int ret = 0;

  if (httpd_context)
    {
      ret = _mod_httpd_process_oob (httpd_context, node_info, oob_data);
    }

  return ret;
}

static lw6cnx_connection_t *
_open (void *srv_context, lw6srv_listener_t * listener, char *local_url,
       char *remote_url, char *remote_ip,
       int remote_port, char *password,
       u_int64_t local_id, u_int64_t remote_id, int dns_ok,
       int network_reliability, lw6cnx_recv_callback_t recv_callback_func,
       void *recv_callback_data)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  lw6cnx_connection_t *ret = NULL;

  if (httpd_context)
    {
      ret =
	_mod_httpd_open (httpd_context, listener, local_url, remote_url,
			 remote_ip, remote_port, password, local_id,
			 remote_id, dns_ok, network_reliability,
			 recv_callback_func, recv_callback_data);
    }

  return ret;
}

static int
_feed_with_tcp (void *srv_context, lw6cnx_connection_t * connection,
		lw6srv_tcp_accepter_t * tcp_accepter)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  int ret = 0;

  if (connection)
    {
      ret =
	_mod_httpd_feed_with_tcp (httpd_context, connection, tcp_accepter);
    }

  return ret;
}

static int
_feed_with_udp (void *srv_context, lw6cnx_connection_t * connection,
		lw6srv_udp_buffer_t * udp_buffer)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  int ret = 0;

  if (connection)
    {
      ret = _mod_httpd_feed_with_udp (httpd_context, connection, udp_buffer);
    }

  return ret;
}

static void
_close (void *srv_context, lw6cnx_connection_t * connection)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;

  if (connection)
    {
      _mod_httpd_close (httpd_context, connection);
    }
}

static int
_send (void *srv_context, lw6cnx_connection_t * connection,
       u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
       u_int64_t logical_from_id, u_int64_t logical_to_id, char *message)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  int ret = 0;

  if (connection)
    {
      ret =
	_mod_httpd_send (httpd_context, connection, physical_ticket_sig,
			 logical_ticket_sig, logical_from_id, logical_to_id,
			 message);
    }

  return ret;
}

static void
_poll (void *srv_context, lw6cnx_connection_t * connection)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;

  if (connection)
    {
      _mod_httpd_poll (httpd_context, connection);
    }
}

static int
_is_alive (void *srv_context, lw6cnx_connection_t * connection)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  int ret = 0;

  if (connection)
    {
      ret = _mod_httpd_is_alive (httpd_context, connection);
    }

  return ret;
}

static char *
_repr (void *srv_context, lw6cnx_connection_t * connection)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  char *ret = NULL;

  if (connection)
    {
      ret = _mod_httpd_repr (httpd_context, connection);
    }

  return ret;
}

static char *
_error (void *srv_context, lw6cnx_connection_t * connection)
{
  _httpd_context_t *httpd_context = (_httpd_context_t *) srv_context;
  char *ret = NULL;

  if (connection)
    {
      ret = _mod_httpd_error (httpd_context, connection);
    }

  return ret;
}

lw6sys_module_pedigree_t *
mod_httpd_get_pedigree ()
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree =
    (lw6sys_module_pedigree_t *)
    LW6SYS_CALLOC (sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "httpd";
      module_pedigree->category = "srv";
      module_pedigree->name = _x_ ("HTTPD");
      module_pedigree->readme =
	_x_
	("Custom HTTP server, allows HTTP clients to connect, and also exposes game status to standard web browsers.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright =
	"Copyright (C)  2010  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

lw6srv_backend_t *
mod_httpd_create_backend ()
{
  lw6srv_backend_t *backend;

  backend = LW6SYS_MALLOC (sizeof (lw6srv_backend_t));
  if (backend)
    {
      memset (backend, 0, sizeof (lw6srv_backend_t));

      backend->init = _init;
      backend->quit = _quit;
      backend->analyse_tcp = _analyse_tcp;
      backend->analyse_udp = _analyse_udp;
      backend->process_oob = _process_oob;
      backend->open = _open;
      backend->feed_with_tcp = _feed_with_tcp;
      backend->feed_with_udp = _feed_with_udp;
      backend->close = _close;
      backend->send = _send;
      backend->poll = _poll;
      backend->is_alive = _is_alive;
      backend->repr = _repr;
      backend->error = _error;
    }

  return backend;
}
