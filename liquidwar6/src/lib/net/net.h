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

#ifndef LIQUIDWAR6NET_H
#define LIQUIDWAR6NET_H

#include "../sys/sys.h"
#include "../cfg/cfg.h"

#define LW6NET_ADDRESS_ANY "0.0.0.0"
#define LW6NET_ADDRESS_BROADCAST "255.255.255.255"
#define LW6NET_ADDRESS_LOOPBACK "127.0.0.1"
#define LW6NET_DEFAULT_PORT 8056
#define LW6NET_HTTP_PORT 80
#define LW6NET_MAX_PACKET_SIZE 2000
#define LW6NET_SOCKET_INVALID -1

// net-error.c
extern int lw6net_last_error ();

/* net-dns.c */
extern int lw6net_dns_is_ip (char *ip);
extern char *lw6net_dns_gethostbyname (char *name);
extern int lw6net_dns_lock ();
extern int lw6net_dns_unlock ();

/* net-if.c */
extern char *lw6net_if_guess_local ();
extern char *lw6net_if_guess_public_url (char *bind_ip, int bind_port);

/*
 * In line.c
 */
extern char *lw6net_recv_line_tcp (int sock);
extern int lw6net_send_line_tcp (int sock, char *line);
extern char *lw6net_recv_line_udp (int sock,
				   char **incoming_ip, int *incoming_port);
extern lw6sys_list_t *lw6net_recv_lines_udp (int sock,
					     char **incoming_ip,
					     int *incoming_port);
extern int lw6net_send_line_udp (int sock, char *line, char *ip, int port);

/*
 * In setup.c
 */
extern int lw6net_init (int argc, char *argv[]);
extern void lw6net_quit ();

/* net-socket.c */
extern int lw6net_socket_set_blocking_mode (int sock, int mode);
extern int lw6net_socket_is_valid (int sock);
extern void lw6net_socket_close (int sock);

/*
 * In tcp.c
 */
extern int lw6net_tcp_listen (char *ip, int port);
extern int lw6net_tcp_accept (char **incoming_ip,
			      int *incoming_port, int listening_sock,
			      int delay_msec);
extern int lw6net_tcp_connect (char *ip, int port, int delay_msec);
extern int lw6net_tcp_send (int sock, char *buf,
			    int len, int delay_msec, int loop);
extern int lw6net_tcp_peek (int sock, char *buf, int len, int delay_msec);
extern int lw6net_tcp_recv (int sock, char *buf,
			    int len, int delay_msec, int loop);
extern int lw6net_tcp_is_alive (int sock);

/* net-test.c */
extern int lw6net_test (int mode);

/*
 * In udp.c
 */
extern int lw6net_udp_client ();
extern int lw6net_udp_server (char *ip, int port);
extern int lw6net_udp_send (int sock, char *buf, int len, char *ip, int port);
extern int lw6net_udp_peek (int sock, char *buf,
			    int len, char **incoming_ip, int *incoming_port);
extern int lw6net_udp_recv (int sock, char *buf, int len,
			    char **incoming_ip, int *incoming_port);

#endif
