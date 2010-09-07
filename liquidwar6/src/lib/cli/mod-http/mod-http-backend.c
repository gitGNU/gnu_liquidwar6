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
#include "mod-http.h"
#include "mod-http-internal.h"

/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
void
mod_http_is_GPL_compatible ()
{
}

static void *
_init (int argc, char *argv[])
{
  _http_context_t *http_context = _mod_http_init (argc, argv);

  return (void *) http_context;
}

static void
_quit (void *cli_context)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;

  if (http_context)
    {
      _mod_http_quit (http_context);
    }
}

static int
_process_oob (void *cli_context, lw6nod_info_t * node_info,
	      lw6cli_oob_data_t * oob_data)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;
  int ret = 0;

  if (http_context)
    {
      ret = _mod_http_process_oob (http_context, node_info, oob_data);
    }

  return ret;
}

static lw6cnx_connection_t *
_open (void *cli_context, char *local_url, char *remote_url, char *remote_ip,
       int remote_port, char *password, u_int64_t local_id,
       u_int64_t remote_id, lw6cnx_recv_callback_t recv_callback_func,
       void *recv_callback_data)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;
  lw6cnx_connection_t *ret = NULL;

  if (http_context)
    {
      ret =
	_mod_http_open (http_context, local_url, remote_url, remote_ip,
			remote_port, password, local_id, remote_id,
			recv_callback_func, recv_callback_data);
    }

  return ret;
}

static void
_close (void *cli_context, lw6cnx_connection_t * connection)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;

  if (http_context)
    {
      _mod_http_close (http_context, connection);
    }
}

static int
_send (void *cli_context, lw6cnx_connection_t * connection,
       u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig,
       u_int64_t logical_from_id, u_int64_t logical_to_id, char *message)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;
  int ret = 0;

  if (http_context)
    {
      ret =
	_mod_http_send (http_context, connection, physical_ticket_sig,
			logical_ticket_sig, logical_from_id, logical_to_id,
			message);
    }

  return ret;
}

static void
_poll (void *cli_context, lw6cnx_connection_t * connection)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;

  if (http_context)
    {
      _mod_http_poll (http_context, connection);
    }
}

static int
_is_alive (void *cli_context, lw6cnx_connection_t * connection)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;
  int ret = 0;

  if (http_context)
    {
      ret = _mod_http_is_alive (http_context, connection);
    }

  return ret;
}

static char *
_repr (void *cli_context, lw6cnx_connection_t * connection)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;
  char *ret = NULL;

  if (http_context)
    {
      ret = _mod_http_repr (http_context, connection);
    }

  return ret;
}

static char *
_error (void *cli_context, lw6cnx_connection_t * connection)
{
  _http_context_t *http_context = (_http_context_t *) cli_context;
  char *ret = NULL;

  if (http_context)
    {
      ret = _mod_http_error (http_context, connection);
    }

  return ret;
}

lw6sys_module_pedigree_t *
mod_http_get_pedigree ()
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree =
    (lw6sys_module_pedigree_t *)
    LW6SYS_CALLOC (sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "http";
      module_pedigree->category = "cli";
      module_pedigree->name = _("HTTP (Curl)");
      module_pedigree->readme =
	_
	("A network client which uses Curl to encapsulate the LW6 protocol into HTTP requests.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright =
	"Copyright (C)  2009  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

lw6cli_backend_t *
mod_http_create_backend ()
{
  lw6cli_backend_t *backend;

  backend = LW6SYS_MALLOC (sizeof (lw6cli_backend_t));
  if (backend)
    {
      memset (backend, 0, sizeof (lw6cli_backend_t));

      backend->init = _init;
      backend->quit = _quit;
      backend->process_oob = _process_oob;
      backend->open = _open;
      backend->close = _close;
      backend->send = _send;
      backend->poll = _poll;
      backend->is_alive = _is_alive;
      backend->repr = _repr;
      backend->error = _error;
    }

  return backend;
}
