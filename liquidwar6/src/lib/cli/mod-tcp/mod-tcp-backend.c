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

#include "../cli.h"
#include "mod-tcp.h"
#include "mod-tcp-internal.h"

/**
 * mod_tcp_is_GPL_compatible
 *
 * Defined to tell mod_tcp is compatible with GNU General Public License.
 * Of course it is. This function does nothing, but the fact it's declared
 * makes its GPL compatibility obvious. Having this declared is required.
 *
 * Return value: none
 */
void
mod_tcp_is_GPL_compatible ()
{
/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
}

/**
 * mod_tcp_is_dlclose_safe
 *
 * Defined to tell mod_tcp has no dlclose issue, once can safely call
 * lt_dlclose on it when done with it, without risking any segfault.
 * Some other LW6 modules/shared libraries do have this problem.
 *
 * Return value: none
 */
void
mod_tcp_is_dlclose_safe ()
{
}

static void *
_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6cnx_properties_t * properties)
{
  _mod_tcp_context_t *tcp_context = _mod_tcp_init (sys_context, argc, argv, properties);

  return (void *) tcp_context;
}

static void
_quit (lw6sys_context_t * sys_context, void *cli_context)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;

  if (tcp_context)
    {
      _mod_tcp_quit (sys_context, tcp_context);
    }
}

static int
_process_oob (lw6sys_context_t * sys_context, void *cli_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;
  int ret = 0;

  if (tcp_context)
    {
      ret = _mod_tcp_process_oob (sys_context, tcp_context, node_info, oob_data);
    }

  return ret;
}

static lw6cnx_connection_t *
_open (lw6sys_context_t * sys_context, void *cli_context, const char *local_url, const char *remote_url,
       const char *remote_ip, int remote_port, const char *password, u_int64_t local_id, u_int64_t remote_id, int dns_ok, int network_reliability)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;
  lw6cnx_connection_t *ret = NULL;

  if (tcp_context)
    {
      ret =
	_mod_tcp_open (sys_context, tcp_context, local_url, remote_url, remote_ip, remote_port, password, local_id, remote_id, dns_ok, network_reliability);
    }

  return ret;
}

static void
_close (lw6sys_context_t * sys_context, void *cli_context, lw6cnx_connection_t * connection)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;

  if (tcp_context)
    {
      _mod_tcp_close (sys_context, tcp_context, connection);
    }
}

static int
_send (lw6sys_context_t * sys_context, void *cli_context, lw6cnx_connection_t * connection, int64_t now,
       u_int32_t physical_ticket_sig, u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;
  int ret = 0;

  if (tcp_context)
    {
      ret = _mod_tcp_send (sys_context, tcp_context, connection, now, physical_ticket_sig, logical_ticket_sig, logical_from_id, logical_to_id, message);
    }

  return ret;
}

static int
_can_send (lw6sys_context_t * sys_context, void *cli_context, lw6cnx_connection_t * connection)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;
  int ret = 0;

  if (tcp_context)
    {
      ret = _mod_tcp_can_send (sys_context, tcp_context, connection);
    }

  return ret;
}

static void
_poll (lw6sys_context_t * sys_context, void *cli_context, lw6cnx_connection_t * connection)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;

  if (tcp_context)
    {
      _mod_tcp_poll (sys_context, tcp_context, connection);
    }
}

static char *
_repr (lw6sys_context_t * sys_context, void *cli_context, lw6cnx_connection_t * connection)
{
  _mod_tcp_context_t *tcp_context = (_mod_tcp_context_t *) cli_context;
  char *ret = NULL;

  if (tcp_context)
    {
      ret = _mod_tcp_repr (sys_context, tcp_context, connection);
    }

  return ret;
}

/**
 * mod_tcp_get_pedigree
 *
 * @sys_context: global system context
 *
 * Returns the pedigree for mod-tcp, giving details about the module,
 * including name, description, licence, date/time of compilation.
 *
 * Return value: dynamically allocated object.
 */
lw6sys_module_pedigree_t *
mod_tcp_get_pedigree (lw6sys_context_t * sys_context)
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree = (lw6sys_module_pedigree_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = MOD_TCP_BACKEND_ID;
      module_pedigree->category = "cli";
      module_pedigree->name = _("TCP");
      module_pedigree->readme = _x_ ("A simple network client which uses raw TCP/IP sockets. The most reliable way to communicate.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright = "Copyright (C)  2015  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

/**
 * mod_tcp_create_backend
 *
 * @sys_context: global system context
 *
 * Creates a mod-tcp backend.
 *
 * Return value: backend pointer.
 */
lw6cli_backend_t *
mod_tcp_create_backend (lw6sys_context_t * sys_context)
{
  lw6cli_backend_t *backend;

  backend = LW6SYS_MALLOC (sys_context, sizeof (lw6cli_backend_t));
  if (backend)
    {
      memset (backend, 0, sizeof (lw6cli_backend_t));

      backend->init = _init;
      backend->quit = _quit;
      backend->process_oob = _process_oob;
      backend->open = _open;
      backend->close = _close;
      backend->send = _send;
      backend->can_send = _can_send;
      backend->poll = _poll;
      backend->repr = _repr;
    }

  return backend;
}
