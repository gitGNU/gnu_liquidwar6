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

#ifndef LIQUIDWAR6NET_INTERNAL_H
#define LIQUIDWAR6NET_INTERNAL_H

#include "net.h"

#include <pthread.h>
#ifdef LW6_MS_WINDOWS
#include <winsock.h>
typedef u_int32_t in_addr_t;
typedef int socklen_t;
#else
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/signal.h>
#endif

#define _LW6NET_IP_SIZE 20	// only 15 required but...

typedef struct _lw6net_const_data_s
{
  int listen_backlog;
  int chunk_size;
  int line_size;
  float line_delay;
  /*  char *httpd_log;
     int listen_backlog;
     int chunk_size;
     int line_size;
     float protocol_delay;
     float line_delay;
     float http_delay;
     char *http_status;
     char *http_status_200;
     char *http_status_404;
     char *http_status_500;
     char *http_header_content_type;
     char *http_header_content_length;
     char *http_header_last_modified;
     char *http_header_connection;
     char *http_header_server;
     char *http_header_x_powered_by;
     char *content_type_txt;
     char *content_type_html;
     char *content_type_jpeg;
     char *content_type_ico;
     char *content_type_css;
     char *http_request_pattern;
     char *uri_root;
     char *uri_index;
     char *uri_robots;
     char *uri_favicon;
     char *uri_more;
     char *uri_style;
     char *uri_gnu;
     char *uri_gpl;
     char *uri_screenshot;
     char *uri_status;
     char *uri_list;
     char *screenshot_filename;
     int screenshot_refresh_delay;
     int screenshot_width;
     int screenshot_height;
     int screenshot_quality;
     char *template_pattern;
     char *template_format_version;
     char *template_format_compiled;
   */
}
_lw6net_const_data_t;

typedef struct _lw6net_socket_counters_s
{
  int open_counter;
  int close_counter;
}
_lw6net_socket_counters_t;

typedef struct _lw6net_context_s
{
  _lw6net_const_data_t const_data;
  _lw6net_socket_counters_t socket_counters;
  lw6sys_assoc_t *threads;
  int server_sock;
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

/*
 * In address.c
 */
extern char *_lw6net_inet_ntoa (struct in_addr in);

/*
 * In const.c
 */
extern int _lw6net_const_init (int argc, char *argv[]);
extern void _lw6net_const_quit ();

// net-error.c
#ifdef LW6_MS_WINDOWS
extern char *_lw6net_wsa_str (int wsa_int);
#endif

/*
 * In socket.c
 */
extern int _lw6net_socket_init ();
extern void _lw6net_socket_quit ();
extern int _lw6net_socket_bind (char *ip, int port, int protocol);

/*
 * In thread.c
 */
extern int _lw6net_thread_init ();
extern void _lw6net_thread_quit ();
extern int _lw6net_thread_register (void *handler);
extern int _lw6net_thread_unregister (void *handler);
extern int _lw6net_thread_vacuum ();

#endif
