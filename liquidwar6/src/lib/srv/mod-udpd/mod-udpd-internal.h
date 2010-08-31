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

#ifndef LIQUIDWAR6SRV_UDPD_INTERNAL_H
#define LIQUIDWAR6SRV_UDPD_INTERNAL_H

#include "../srv.h"

typedef struct _udpd_consts_s
{
  int error_timeout;
}
_udpd_consts_t;

typedef struct _udpd_data_s
{
  _udpd_consts_t consts;
}
_udpd_data_t;

typedef struct _udpd_context_s
{
  _udpd_data_t data;
}
_udpd_context_t;

/* mod-udpd-data.c */
extern int _mod_udpd_load_data (_udpd_data_t * udpd_data, char *data_dir);
extern void _mod_udpd_unload_data (_udpd_data_t * udpd_data);

/*
 * In setup.c
 */
extern _udpd_context_t *_mod_udpd_init (int argc, char *argv[],
					lw6srv_listener_t * listener);
extern void _mod_udpd_quit (_udpd_context_t * udpd_context);

/*
 * In handshake.c
 */
extern int _mod_udpd_analyse_tcp (_udpd_context_t * udpd_context,
				  lw6srv_tcp_accepter_t * tcp_accepter,
				  u_int64_t * remote_id);
extern int _mod_udpd_analyse_udp (_udpd_context_t * udpd_context,
				  lw6srv_udp_buffer_t * udp_buffer,
				  u_int64_t * remote_id);
extern lw6srv_connection_t *_mod_udpd_accept_tcp (_udpd_context_t *
						  udpd_context,
						  lw6srv_tcp_accepter_t *
						  tcp_accepter,
						  char *password);
extern lw6srv_connection_t *_mod_udpd_new_udp (_udpd_context_t * udpd_context,
					       lw6srv_udp_buffer_t *
					       udp_buffer, char *password);
extern int _mod_udpd_is_associated_with_udp (_udpd_context_t * udpd_context,
					     lw6srv_connection_t * connection,
					     lw6srv_udp_buffer_t *
					     udp_buffer);
extern int _mod_udpd_update_with_udp (_udpd_context_t * udpd_context,
				      lw6srv_connection_t * connection,
				      lw6srv_udp_buffer_t * udp_buffer);

/*
 * In state.c
 */
extern void _mod_udpd_close (_udpd_context_t * udpd_context,
			     lw6srv_connection_t * connection);
extern int _mod_udpd_is_alive (_udpd_context_t * udpd_context,
			       lw6srv_connection_t * connection);
extern int _mod_udpd_timeout_ok (_udpd_context_t * udpd_context,
				 int64_t origin_timestamp);

/*
 * In message.c
 */
extern int _mod_udpd_send (_udpd_context_t * udpd_context,
			   lw6srv_connection_t * connection, char *message);
extern char *_mod_udpd_recv (_udpd_context_t * udpd_context,
			     lw6srv_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_udpd_repr (_udpd_context_t * udpd_context,
			     lw6srv_connection_t * connection);
extern char *_mod_udpd_error (_udpd_context_t * udpd_context,
			      lw6srv_connection_t * connection);

/* mod-udpd-oob.c */
extern int _mod_udpd_process_oob (_udpd_context_t * udpd_context,
				  lw6nod_info_t * node_info,
				  lw6srv_oob_data_t * oob_data);
extern int _mod_udpd_oob_should_continue (_udpd_context_t * udpd_context,
					  lw6srv_oob_data_t * oob_data);

#endif
