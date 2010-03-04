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

#ifndef LIQUIDWAR6CLI_HTTP_INTERNAL_H
#define LIQUIDWAR6CLI_HTTP_INTERNAL_H

#include "../cli.h"

typedef struct _http_context_s
{
  int dummy_http;
}
_http_context_t;

/*
 * In setup.c
 */
extern _http_context_t *_mod_http_init ();
extern void _mod_http_quit (_http_context_t * http_context);

/*
 * In handshake.c
 */
extern int _mod_http_can_handle (_http_context_t * http_context,
				 char *server_url);
extern lw6cli_connection_t *_mod_http_connect (_http_context_t * http_context,
					       char *server_url,
					       char *client_url,
					       char *password);

/*
 * In state.c
 */
extern void _mod_http_close (_http_context_t * http_context,
			     lw6cli_connection_t * connection);
extern int _mod_http_is_alive (_http_context_t * http_context,
			       lw6cli_connection_t * connection);

/*
 * In message.c
 */
extern int _mod_http_send (_http_context_t * http_context,
			   lw6cli_connection_t * connection, char *message);
extern char *_mod_http_recv (_http_context_t * http_context,
			     lw6cli_connection_t * connection);

/*
 * In info.c
 */
extern char *_mod_http_repr (_http_context_t * http_context,
			     lw6cli_connection_t * connection);
extern char *_mod_http_error (_http_context_t * http_context,
			      lw6cli_connection_t * connection);

#endif
