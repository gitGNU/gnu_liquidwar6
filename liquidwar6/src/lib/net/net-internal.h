/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6NET_INTERNAL_H
#define LIQUIDWAR6NET_INTERNAL_H

#include "net.h"

#include <pthread.h>
#ifdef LW6_MS_WINDOWS
#include <winsock.h>
typedef u_int32_t in_addr_t;
typedef int socklen_t;
#else // LW6_MS_WINDOWS
#include <sys/types.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#endif // LW6_MS_WINDOWS

#define _LW6NET_IP_SIZE 20	// only 15 required but...

typedef struct _lw6net_const_data_s
{
  int listen_backlog;
  int chunk_size;
  int line_size;
  int line_delay_msec;
  int dns_cache_hash_size;
  int dns_cache_delay_sec;
  int connectable_cache_hash_size;
  int connectable_cache_delay_sec;
}
_lw6net_const_data_t;

typedef struct _lw6net_counters_s
{
  lw6sys_spinlock_t *spinlock;
  int open_counter;
  int close_counter;
  int64_t sent_bytes;
  int64_t received_bytes;
}
_lw6net_counters_t;

typedef struct _lw6net_log_s
{
  char *tcp_recv_filename;
  char *tcp_send_filename;
  char *udp_recv_filename;
  char *udp_send_filename;
}
_lw6net_log_t;

typedef struct _lw6net_dns_s
{
  lw6sys_mutex_t *dns_gethostbyname_mutex;
  lw6sys_mutex_t *dns_cache_mutex;
  lw6sys_cache_t *dns_cache;
}
_lw6net_dns_t;

typedef struct _lw6net_connectable_s
{
  lw6sys_mutex_t *connectable_cache_mutex;
  lw6sys_cache_t *connectable_cache;
}
_lw6net_connectable_t;

typedef struct _lw6net_context_s
{
  _lw6net_const_data_t const_data;
  _lw6net_counters_t counters;
  _lw6net_log_t log;
  _lw6net_dns_t dns;
  _lw6net_connectable_t connectable;
}
_lw6net_context_t;

typedef struct _lw6net_socket_async_connect_data_s
{
  char ip[_LW6NET_IP_SIZE];
  int port;
  int sock;
  int close;
}
_lw6net_socket_async_connect_data_t;

extern _lw6net_context_t *_lw6net_global_context;

/* net-address.c */
extern char *_lw6net_inet_ntoa (struct in_addr in);
extern int _lw6net_inet_aton (struct in_addr *in, const char *ip);

/* net-connectable.c */
extern int _lw6net_connectable_init (_lw6net_connectable_t * connectable, int connectable_cache_hash_size, int connectable_cache_delay_sec);
extern void _lw6net_connectable_quit (_lw6net_connectable_t * connectable);

/* net-const.c */
extern int _lw6net_const_init (int argc, const char *argv[], _lw6net_const_data_t * const_data);
extern void _lw6net_const_quit (_lw6net_const_data_t * const_data);

/* net-counters.c */
extern int _lw6net_counters_init (int argc, const char *argv[], _lw6net_counters_t * counters);
extern void _lw6net_counters_quit (_lw6net_counters_t * counters);
extern void _lw6net_counters_register_socket (_lw6net_counters_t * counters);
extern void _lw6net_counters_unregister_socket (_lw6net_counters_t * counters);
extern void _lw6net_counters_register_send (_lw6net_counters_t * counters, int bytes);
extern void _lw6net_counters_register_recv (_lw6net_counters_t * counters, int bytes);

/* net-dns.c */
extern int _lw6net_dns_init (_lw6net_dns_t * dns, int dns_cache_hash_size, int dns_cache_delay_sec);
extern void _lw6net_dns_quit (_lw6net_dns_t * dns);

/* net-error.c */
#ifdef LW6_MS_WINDOWS
extern char *_lw6net_wsa_str (int wsa_int);
#endif

/* net-log.c */
extern int _lw6net_log_init (int argc, const char *argv[], _lw6net_log_t * log, int net_log);
extern void _lw6net_log_quit (_lw6net_log_t * log);
extern int _lw6net_log_tcp_recv (_lw6net_log_t * log, const char *buf, int len);
extern int _lw6net_log_tcp_send (_lw6net_log_t * log, const char *buf, int len);
extern int _lw6net_log_udp_recv (_lw6net_log_t * log, const char *buf, int len);
extern int _lw6net_log_udp_send (_lw6net_log_t * log, const char *buf, int len);

/* net-socket.c */
extern int _lw6net_socket_bind (const char *ip, int port, int protocol);

/* net-tcp.c */
extern void _lw6net_delay_msec_to_timeval (struct timeval *tv, int delay_msec);

#endif
