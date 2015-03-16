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

#ifndef LIQUIDWAR6CLI_UDP_INTERNAL_H
#define LIQUIDWAR6CLI_UDP_INTERNAL_H

#include "../cli.h"
#include "mod-udp.h"

typedef struct _udp_consts_s
{
  int global_timeout;
  int broadcast_timeout;
  int ping_alter_base;
  int ping_alter_percent;
}
_udp_consts_t;

typedef struct _udp_data_s
{
  _udp_consts_t consts;
}
_udp_data_t;

typedef struct _udp_context_s
{
  _udp_data_t data;
}
_mod_udp_context_t;

typedef struct _udp_specific_data_s
{
  int sock;
}
_udp_specific_data_t;

/* mod-udp-data.c */
extern int _mod_udp_load_data (lw6sys_context_t * sys_context, _udp_data_t * udp_data, const char *data_dir);
extern void _mod_udp_unload_data (lw6sys_context_t * sys_context, _udp_data_t * udp_data);

/* mod-udp-setup.c */
extern _mod_udp_context_t *_mod_udp_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6cnx_properties_t * properties);
extern void _mod_udp_quit (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context);

/* mod-udp-state.c */
extern lw6cnx_connection_t *_mod_udp_open (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context,
					   const char *local_url,
					   const char *remote_url,
					   const char *remote_ip,
					   int remote_port,
					   const char *password, u_int64_t local_id, u_int64_t remote_id, int dns_ok, int network_reliability);
extern void _mod_udp_close (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6cnx_connection_t * connection);
extern int _mod_udp_timeout_ok (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, int64_t origin_timestamp, int broadcast);

/* mod-udp-message.c */
extern int _mod_udp_send (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context,
			  lw6cnx_connection_t * connection,
			  int64_t now,
			  u_int32_t physical_ticket_sig,
			  u_int32_t logical_ticket_sig, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *message);
extern int _mod_udp_can_send (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6cnx_connection_t * connection);
extern void _mod_udp_poll (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6cnx_connection_t * connection);

/* mod-udp-info.c */
extern char *_mod_udp_repr (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6cnx_connection_t * connection);

/* mod-udp-oob.c */
extern int _mod_udp_process_oob (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6nod_info_t * node_info, lw6cli_oob_data_t * oob_data);
extern int _mod_udp_oob_should_continue (lw6sys_context_t * sys_context, _mod_udp_context_t * udp_context, lw6cli_oob_data_t * oob_data, int broadcast);

#endif // LIQUIDWAR6CLI_UDP_INTERNAL_H
