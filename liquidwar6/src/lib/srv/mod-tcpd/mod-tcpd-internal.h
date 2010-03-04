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

#ifndef LIQUIDWAR6SRV_TCPD_INTERNAL_H
#define LIQUIDWAR6SRV_TCPD_INTERNAL_H

#include "../srv.h"

typedef struct _tcpd_context_s
{
  int dummy_tcpd;
}
_tcpd_context_t;

/*
 * In setup.c
 */
extern _tcpd_context_t *_mod_tcpd_init ();
extern void _mod_tcpd_quit (_tcpd_context_t * tcpd_context);

/*
 * In handshake.c
 */
extern int _mod_tcpd_can_handle_tcp (_tcpd_context_t * tcpd_context,
				     lw6srv_tcp_accepter_t * tcp_accepter);
extern int _mod_tcpd_can_handle_udp (_tcpd_context_t * tcpd_context,
				     lw6srv_udp_buffer_t * udp_buffer);
extern lw6srv_connection_t *_mod_tcpd_accept_tcp (_tcpd_context_t *
						  tcpd_context,
						  lw6srv_tcp_accepter_t *
						  tcp_accepter,
						  char *password);
extern lw6srv_connection_t *_mod_tcpd_new_udp (_tcpd_context_t * tcpd_context,
					       lw6srv_udp_buffer_t *
					       udp_buffer, char *password);
extern int _mod_tcpd_is_associated_with_udp (_tcpd_context_t * tcpd_context,
					     lw6srv_connection_t * connection,
					     lw6srv_udp_buffer_t *
					     udp_buffer);
extern int _mod_tcpd_update_with_udp (_tcpd_context_t * tcpd_context,
				      lw6srv_connection_t * connection,
				      lw6srv_udp_buffer_t * udp_buffer);

/*
 * In state.c
 */
extern void _mod_tcpd_close (_tcpd_context_t * tcpd_context,
			     lw6srv_connection_t * connection);
extern int _mod_tcpd_is_alive (_tcpd_context_t * tcpd_context,
			       lw6srv_connection_t * connection);

/*
 * In message.c
 */
extern int _mod_tcpd_send (_tcpd_context_t * tcpd_context,
			   lw6srv_connection_t * connection, char *message);
extern char *_mod_tcpd_recv (_tcpd_context_t * tcpd_context,
			     lw6srv_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_tcpd_repr (_tcpd_context_t * tcpd_context,
			     lw6srv_connection_t * connection);
extern char *_mod_tcpd_error (_tcpd_context_t * tcpd_context,
			      lw6srv_connection_t * connection);

#endif
