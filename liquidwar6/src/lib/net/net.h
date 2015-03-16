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

#ifndef LIQUIDWAR6NET_H
#define LIQUIDWAR6NET_H

#include "../sys/sys.h"
#include "../glb/glb.h"
#include "../hlp/hlp.h"
#include "../cfg/cfg.h"

#define LW6NET_ADDRESS_ANY "0.0.0.0"
#define LW6NET_ADDRESS_BROADCAST "255.255.255.255"
#define LW6NET_ADDRESS_LOOPBACK "127.0.0.1"
#define LW6NET_DEFAULT_PORT 8056
#define LW6NET_HTTP_PORT 80

/*
 * http://www.faqs.org/rfcs/rfc1191.html
 * http://www.dslreports.com/faq/695
 * 1460 TCP Data size (MSS) when MTU is 1500 and not using PPPoE.
 * 1452 TCP Data size (MSS) when MTU is 1492 and using PPPoE.
 * 576 Typically recommended as the MTU for dial-up type applications, leaving 536 bytes of TCP data.
 *
 * Here we don't define the real MTU but the real data
 * one can send in a row with a given MTU.
 */
#define LW6NET_DIALUP_MTU 536
#define LW6NET_PPPOE_MTU 1452
#define LW6NET_ETHERNET_MTU 1460

#define LW6NET_MAX_CHUNK_SIZE 4095
#define LW6NET_MAX_LINE_SIZE 4093

#define LW6NET_SOCKET_INVALID -1
#define LW6NET_UDP_MINIMAL_BUF_SIZE 1

/* net-connectable.c */
extern int lw6net_is_connectable (lw6sys_context_t * sys_context, const char *ip, int port);
extern void lw6net_set_connectable (lw6sys_context_t * sys_context, const char *ip, int port, int status);

/* net-counters.c */
extern int lw6net_counters_get_sent_kbytes (lw6sys_context_t * sys_context);
extern int lw6net_counters_get_received_kbytes (lw6sys_context_t * sys_context);

/* net-dns.c */
extern int lw6net_dns_is_ip (lw6sys_context_t * sys_context, const char *ip);
extern char *lw6net_dns_gethostbyname (lw6sys_context_t * sys_context, const char *name);
extern int lw6net_dns_lock (lw6sys_context_t * sys_context);
extern int lw6net_dns_unlock (lw6sys_context_t * sys_context);

/* net-error.c */
extern int lw6net_last_error (lw6sys_context_t * sys_context);

/* net-if.c */
extern char *lw6net_if_guess_local (lw6sys_context_t * sys_context);
extern char *lw6net_if_guess_public_url (lw6sys_context_t * sys_context, const char *bind_ip, int bind_port);

/* net-line.c */
extern char *lw6net_recv_line_tcp (lw6sys_context_t * sys_context, int *sock);
extern int lw6net_send_line_tcp (lw6sys_context_t * sys_context, int *sock, const char *line);
extern char *lw6net_recv_line_udp (lw6sys_context_t * sys_context, int sock, char **incoming_ip, int *incoming_port);
extern lw6sys_list_t *lw6net_recv_lines_udp (lw6sys_context_t * sys_context, int sock, char **incoming_ip, int *incoming_port);
extern int lw6net_send_line_udp (lw6sys_context_t * sys_context, int sock, const char *line, const char *ip, int port);

/* net-setup.c */
extern int lw6net_init (lw6sys_context_t * sys_context, int argc, const char *argv[], int net_log);
extern void lw6net_quit (lw6sys_context_t * sys_context);

/* net-socket.c */
extern int lw6net_socket_set_blocking_mode (lw6sys_context_t * sys_context, int sock, int mode);
extern int lw6net_socket_is_valid (lw6sys_context_t * sys_context, int sock);
extern void lw6net_socket_close (lw6sys_context_t * sys_context, int *sock);

/* net-tcp.c */
extern int lw6net_tcp_listen (lw6sys_context_t * sys_context, const char *ip, int port);
extern int lw6net_tcp_accept (lw6sys_context_t * sys_context, char **incoming_ip, int *incoming_port, int listening_sock, int delay_msec);
extern int lw6net_tcp_connect (lw6sys_context_t * sys_context, const char *ip, int port, int delay_msec);
extern int lw6net_tcp_send (lw6sys_context_t * sys_context, int *sock, const char *buf, int len, int delay_msec, int loop);
extern int lw6net_tcp_peek (lw6sys_context_t * sys_context, int *sock, char *buf, int len, int delay_msec);
extern int lw6net_tcp_recv (lw6sys_context_t * sys_context, int *sock, char *buf, int len, int delay_msec, int loop);
extern int lw6net_tcp_is_alive (lw6sys_context_t * sys_context, int *sock);

/* net-test.c */
extern int lw6net_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6net_test_run (lw6sys_context_t * sys_context, int mode);

/* net-udp.c */
extern int lw6net_udp_client (lw6sys_context_t * sys_context);
extern int lw6net_udp_server (lw6sys_context_t * sys_context, const char *ip, int port);
extern int lw6net_udp_send (lw6sys_context_t * sys_context, int sock, const char *buf, int len, const char *ip, int port);
extern int lw6net_udp_peek (lw6sys_context_t * sys_context, int sock, char *buf, int len, char **incoming_ip, int *incoming_port);
extern int lw6net_udp_recv (lw6sys_context_t * sys_context, int sock, char *buf, int len, char **incoming_ip, int *incoming_port);

#endif
