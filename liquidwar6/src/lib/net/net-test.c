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

#include <CUnit/CUnit.h>

#include "net.h"
#include "net-internal.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

#define _TEST_NET_LOG 1
#define _TEST_HOST_IP "127.0.0.1"
#define _TEST_HOST_OK "localhost"
#define _TEST_HOST_KO "now-if-this-is-a valid HOST there's a serious PROBLEM!!!!"
#define _TEST_UNREACHABLE_IP "209.85.135.99"
#define _TEST_UNREACHABLE_PORT 8000
#define _TEST_TCP_CONNECT_DELAY 1000
#define _TEST_TCP_ACCEPT_DELAY 1000
#define _TEST_TCP_STREAM_DELAY 100
#define _TEST_TCP_MASSIVE_N 100000
#define _TEST_TCP_MASSIVE_STEP 1000
#define _TEST_UDP_DELAY 100
#define _TEST_BUF1_STR "foo"
#define _TEST_BUF2_STR "what's the difference between a chicken and a house?"
#define _TEST_LINE1 "foo"
#define _TEST_LINE2 "a\tb\tc"
#define _TEST_LINE3 "azerty azerty azerty azerty azerty azerty azerty azerty azerty azerty azerty azerty"
#define _TEST_LINES_OK "there\nare\n4\nlines\n"
#define _TEST_LINES_KO "this\nis\ntruncated"
/*
 * When testing, using the port following the default one,
 * to avoid the rather common bug: LW is actually running,
 * and test fails...
 */
#define _TEST_PORT (LW6NET_DEFAULT_PORT+2)

typedef struct _lw6net_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6net_test_data_t;

static _lw6net_test_data_t _test_data = { 0, NULL };

static void
_test_address ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    struct in_addr in;
    char *str = NULL;

    ret = 0;
    if (_lw6net_inet_aton (sys_context, &in, _TEST_HOST_IP))
      {
	str = _lw6net_inet_ntoa (sys_context, in);
	if (str)
	  {
	    if (lw6sys_str_is_same (sys_context, _TEST_HOST_IP, str))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("\"%s\" converted to/from in_addr OK"), str);
		ret = 1;
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("\"%s\" transformed in \"%s\" by aton + ntoa"), _TEST_HOST_IP, str);
	      }
	    LW6SYS_FREE (sys_context, str);
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_dns ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *ip = NULL;

    if (lw6net_dns_is_ip (sys_context, _TEST_HOST_IP) && !lw6net_dns_is_ip (sys_context, _TEST_HOST_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("trivial IP parsing reports \"%s\" is an IP but not \"%s\""), _TEST_HOST_IP, _TEST_HOST_OK);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("trivial IP parsing failed to report \"%s\" as an IP and \"%s\" as not"), _TEST_HOST_IP, _TEST_HOST_OK);
	ret = 0;
      }
    ip = lw6net_dns_gethostbyname (sys_context, _TEST_HOST_IP);
    if (ip)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("gethostbyname on \"%s\" returns \"%s\""), _TEST_HOST_IP, ip);
	LW6SYS_FREE (sys_context, ip);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("gethostbyname on \"%s\" failed"), _TEST_HOST_IP);
	ret = 0;
      }
    ip = lw6net_dns_gethostbyname (sys_context, _TEST_HOST_OK);
    if (ip)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("gethostbyname on \"%s\" returns \"%s\""), _TEST_HOST_OK, ip);
	LW6SYS_FREE (sys_context, ip);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("gethostbyname on \"%s\" failed"), _TEST_HOST_OK);
	ret = 0;
      }
    ip = lw6net_dns_gethostbyname (sys_context, _TEST_HOST_KO);
    if (ip)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("gethostbyname on \"%s\" returns \"%s\", this is... surprising"), _TEST_HOST_KO, ip);
	LW6SYS_FREE (sys_context, ip);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("gethostbyname on \"%s\" returns NULL, this is fine"), _TEST_HOST_KO);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in if.c
 */
static void
_test_if ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *ip = NULL;
    char *public_url = NULL;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("trying to guess local interface IP"));
    ip = lw6net_if_guess_local (sys_context);
    if (ip)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("local interface IP is \"%s\""), ip);
	LW6SYS_FREE (sys_context, ip);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("unable to guess local interface, this is a problem only if machine *really* has no network available"));
      }
    public_url = lw6net_if_guess_public_url (sys_context, LW6NET_ADDRESS_ANY, _TEST_PORT);
    if (public_url)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("public URL on default port would be \"%s\""), public_url);
	LW6SYS_FREE (sys_context, public_url);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess public URL"));
	ret = 0;
      }
    public_url = lw6net_if_guess_public_url (sys_context, _TEST_UNREACHABLE_IP, LW6NET_HTTP_PORT);
    if (public_url)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("public URL on http port with IP %s would be \"%s\""), _TEST_UNREACHABLE_IP, public_url);
	LW6SYS_FREE (sys_context, public_url);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess public URL"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_prepare_2_tcp_socks (lw6sys_context_t * sys_context, int *sock1, int *sock2)
{
  int ret = 0;
  int listening_sock = -1;
  int connect_sock = -1;
  int accept_sock = -1;
  char *accept_ip = NULL;
  int accept_port = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("listening on port %d (TCP)"), _TEST_PORT);
  listening_sock = lw6net_tcp_listen (sys_context, LW6NET_ADDRESS_ANY, _TEST_PORT);
  if (listening_sock >= 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("TCP socket %d listening on port %d"), listening_sock, _TEST_PORT);

      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("trying to connect on localhost %s:%d"), LW6NET_ADDRESS_LOOPBACK, _TEST_PORT);
      connect_sock = lw6net_tcp_connect (sys_context, LW6NET_ADDRESS_LOOPBACK, _TEST_PORT, _TEST_TCP_CONNECT_DELAY);
      if (lw6net_socket_is_valid (sys_context, connect_sock))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("TCP socket %d connected on %s:%d"), connect_sock, LW6NET_ADDRESS_LOOPBACK, _TEST_PORT);
	  accept_sock = lw6net_tcp_accept (sys_context, &accept_ip, &accept_port, listening_sock, _TEST_TCP_ACCEPT_DELAY);
	  if (lw6net_socket_is_valid (sys_context, accept_sock))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("TCP socket %d accepted %s:%d"), accept_sock, accept_ip, accept_port);
	    }
	  if (accept_ip)
	    {
	      LW6SYS_FREE (sys_context, accept_ip);
	    }
	}
    }
  if (lw6net_socket_is_valid (sys_context, accept_sock) && lw6net_socket_is_valid (sys_context, connect_sock))
    {
      (*sock1) = accept_sock;
      (*sock2) = connect_sock;
      ret = 1;
    }
  else
    {
      if (lw6net_socket_is_valid (sys_context, accept_sock))
	{
	  lw6net_socket_close (sys_context, &accept_sock);
	}
      if (lw6net_socket_is_valid (sys_context, connect_sock))
	{
	  lw6net_socket_close (sys_context, &connect_sock);
	}
      (*sock1) = (*sock2) = LW6NET_SOCKET_INVALID;
    }
  if (lw6net_socket_is_valid (sys_context, listening_sock))
    {
      lw6net_socket_close (sys_context, &listening_sock);
    }

  return ret;
}

/*
 * Testing functions in tcp.c
 */
static void
_test_tcp ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int sock1 = -1;
    int sock2 = -1;
    int ret_tmp1 = 0;
    int ret_tmp2 = 0;
    char *buf1_send = NULL;
    char *buf1_recv = NULL;
    char *buf2_send = NULL;
    char *buf2_recv = NULL;
    int size = 0;
    int received = 0;
    int i = 0;
    int j = 0;
    int sent_i = 0;
    int received_i = 0;
    int send_failed = 0;
    int64_t massive_begin_timestamp = 0LL;
    int64_t massive_end_timestamp = 0LL;
    int massive_duration = 0;
    int bytes_per_sec = 0;
    int internal_tcp_buffer_size = 0;

    sock1 = lw6net_tcp_connect (sys_context, _TEST_UNREACHABLE_IP, _TEST_UNREACHABLE_PORT, _TEST_TCP_CONNECT_DELAY);
    if (!lw6net_socket_is_valid (sys_context, sock1))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to connect on \"%s:%d\", this is fine"), _TEST_UNREACHABLE_IP, _TEST_UNREACHABLE_PORT);
	sock1 = lw6net_tcp_connect (sys_context, _TEST_UNREACHABLE_IP, _TEST_UNREACHABLE_PORT, _TEST_TCP_CONNECT_DELAY);
	if (!lw6net_socket_is_valid (sys_context, sock1))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_
			("unable to connect on \"%s:%d\", this is fine, this second test was just here to trigger the code that skips destinations marked as not connectable"),
			_TEST_UNREACHABLE_IP, _TEST_UNREACHABLE_PORT);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("was able to connect on \"%s:%d\" the second time this is (very!) strange..."), _TEST_UNREACHABLE_IP, _TEST_UNREACHABLE_PORT);
	    lw6net_socket_close (sys_context, &sock1);
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("was able to connect on \"%s:%d\", this is strange..."), _TEST_UNREACHABLE_IP, _TEST_UNREACHABLE_PORT);
	lw6net_socket_close (sys_context, &sock1);
	ret = 0;
      }

    if (_prepare_2_tcp_socks (sys_context, &sock1, &sock2))
      {

	size = strlen (_TEST_BUF1_STR) + 1;
	buf1_send = lw6sys_str_copy (sys_context, _TEST_BUF1_STR);
	buf1_recv = LW6SYS_CALLOC (sys_context, strlen (_TEST_BUF1_STR) + 1);

	if (buf1_send && buf1_recv)
	  {
	    if (lw6net_tcp_send (sys_context, &sock1, buf1_send, size, _TEST_TCP_STREAM_DELAY, 1))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sent \"%s\" on TCP socket %d"), buf1_send, sock1);
		received = lw6net_tcp_peek (sys_context, &sock2, NULL, size, _TEST_TCP_STREAM_DELAY);
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d bytes available on TCP socket %d"), received, sock2);
		if (received == size && lw6net_tcp_recv (sys_context, &sock2, buf1_recv, size, _TEST_TCP_STREAM_DELAY, 1))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("received \"%s\" on TCP socket %d"), buf1_recv, sock2);
		    ret_tmp1 = 1;
		  }
	      }
	  }
	/*
	 * Now proceeding with a massive data test
	 */
	size = strlen (_TEST_BUF2_STR) + 1;
	buf2_send = lw6sys_str_copy (sys_context, _TEST_BUF2_STR);
	buf2_recv = LW6SYS_CALLOC (sys_context, strlen (_TEST_BUF2_STR) + 1);
	if (buf2_send && buf2_recv)
	  {
	    massive_begin_timestamp = lw6sys_get_timestamp (sys_context);
	    for (j = 0;
		 j < _TEST_TCP_MASSIVE_N && sent_i < _TEST_TCP_MASSIVE_N
		 && lw6net_tcp_is_alive (sys_context, &sock1) && received_i < _TEST_TCP_MASSIVE_N && lw6net_tcp_is_alive (sys_context, &sock2); ++j)
	      {
		send_failed = 0;
		for (i = 0; i < _TEST_TCP_MASSIVE_N && sent_i < _TEST_TCP_MASSIVE_N && !send_failed && lw6net_tcp_is_alive (sys_context, &sock1); ++i)
		  {
		    if (!(sent_i % _TEST_TCP_MASSIVE_STEP))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("TCP massive data test, send sent_i=%d"), sent_i);
		      }
		    if (lw6net_tcp_send (sys_context, &sock1, buf2_send, size, _TEST_TCP_STREAM_DELAY, 1))
		      {
			sent_i++;
		      }
		    else
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to send \"%s\" on TCP socket, massive data (sent_i=%d)"), buf2_send, sent_i);
			if (internal_tcp_buffer_size == 0)
			  {
			    /*
			     * Raw estimation of internal TCP buffer size, we round
			     * it to the closest thousand, there's no point in
			     * doing more...
			     */
			    internal_tcp_buffer_size = (sent_i * size) / 1000 * 1000;
			  }
			send_failed = 1;
		      }
		  }

		for (i = 0; i < _TEST_TCP_MASSIVE_N && received_i < sent_i && lw6net_tcp_is_alive (sys_context, &sock2); ++i)
		  {
		    {
		      if (!(received_i % _TEST_TCP_MASSIVE_STEP))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("TCP massive data test, recv received_i=%d"), received_i);
			}
		      received = lw6net_tcp_peek (sys_context, &sock2, NULL, size, _TEST_TCP_STREAM_DELAY);
		      if (received == size && lw6net_tcp_recv (sys_context, &sock2, buf2_recv, size, _TEST_TCP_STREAM_DELAY, 1))
			{
			  received_i++;
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				      _x_ ("unable to recv %d bytes on TCP socket, massive data (received_i=%d)"), size, received_i);
			}
		    }
		  }
	      }
	    massive_end_timestamp = lw6sys_get_timestamp (sys_context);
	    massive_duration = massive_end_timestamp - massive_begin_timestamp;
	    if (massive_duration)
	      {
		/*
		 * It's important to multiplicate by LW6SYS_TICKS_PER_SEC after
		 * the first value is computed, because a) it avoids out of range
		 * error on int32 and b) it rounds to the closest thousand, and
		 * this way one does not have the illusion the result is very precise.
		 * And it's good, because it's not.
		 */
		bytes_per_sec = ((_TEST_TCP_MASSIVE_N * size) / massive_duration) * LW6SYS_TICKS_PER_SEC;
	      }
	    if (sent_i == _TEST_TCP_MASSIVE_N && received_i == _TEST_TCP_MASSIVE_N)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("OK, was able to send and receive %d bytes splitted in %d messages"), _TEST_TCP_MASSIVE_N * size, _TEST_TCP_MASSIVE_N);
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("exchange took %d msec, bytes_per_sec=%d"), massive_duration, bytes_per_sec);
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("raw estimation of internal TCP buffer size is %d bytes"), internal_tcp_buffer_size);

		ret_tmp2 = 1;
	      }
	    else
	      {
		if (sent_i != _TEST_TCP_MASSIVE_N)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("only sent %d messages out of %d"), sent_i, _TEST_TCP_MASSIVE_N);
		  }
		if (received_i != _TEST_TCP_MASSIVE_N)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("only received %d messages out of %d"), received_i, _TEST_TCP_MASSIVE_N);
		  }
	      }
	  }
      }
    if ((!ret_tmp1) || (!ret_tmp2))
      {
	ret = 0;
      }

    if (buf1_send)
      {
	LW6SYS_FREE (sys_context, buf1_send);
      }
    if (buf1_recv)
      {
	LW6SYS_FREE (sys_context, buf1_recv);
      }
    if (buf2_send)
      {
	LW6SYS_FREE (sys_context, buf2_send);
      }
    if (buf2_recv)
      {
	LW6SYS_FREE (sys_context, buf2_recv);
      }
    if (sock1 >= 0)
      {
	lw6net_socket_close (sys_context, &sock1);
      }
    if (sock2 >= 0)
      {
	lw6net_socket_close (sys_context, &sock2);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_prepare_2_udp_socks (lw6sys_context_t * sys_context, int *sock1, int *sock2)
{
  int ret = 0;
  int server_sock = -1;
  int client_sock = -1;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("listening on port %d (UDP)"), _TEST_PORT);
  server_sock = lw6net_udp_server (sys_context, LW6NET_ADDRESS_ANY, _TEST_PORT);
  if (server_sock >= 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("UDP socket %d listening on port %d"), server_sock, _TEST_PORT);
      client_sock = lw6net_udp_client (sys_context);
    }
  if (server_sock >= 0 && client_sock >= 0)
    {
      (*sock1) = server_sock;
      (*sock2) = client_sock;
      ret = 1;
    }
  else
    {
      if (server_sock >= 0)
	{
	  lw6net_socket_close (sys_context, &server_sock);
	}
      if (client_sock >= 0)
	{
	  lw6net_socket_close (sys_context, &client_sock);
	}
      (*sock1) = (*sock2) = LW6NET_SOCKET_INVALID;
    }

  return ret;
}

/*
 * Testing functions in udp.c
 */
static void
_test_udp ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int sock1 = -1;
    int sock2 = -1;
    int ret_tmp = 0;
    char *buf1_send = NULL;
    char *buf1_recv = NULL;
    int size;
    char *incoming_ip = NULL;
    int incoming_port = 0;

    if (_prepare_2_udp_socks (sys_context, &sock1, &sock2))
      {
	size = strlen (_TEST_BUF1_STR) + 1;
	buf1_send = lw6sys_str_copy (sys_context, _TEST_BUF1_STR);
	buf1_recv = LW6SYS_CALLOC (sys_context, strlen (_TEST_BUF1_STR) + 1);

	if (buf1_send && buf1_recv)
	  {
	    if (lw6net_udp_send (sys_context, sock2, buf1_send, size, LW6NET_ADDRESS_LOOPBACK, _TEST_PORT) == size)
	      {
		lw6sys_delay (sys_context, _TEST_UDP_DELAY);
		if (lw6net_udp_peek (sys_context, sock1, buf1_recv, size, &incoming_ip, &incoming_port) == size)
		  {
		    if (incoming_ip)
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d bytes available on socket %d (%s:%d)"), size, sock1, incoming_ip, incoming_port);
			LW6SYS_FREE (sys_context, incoming_ip);
		      }
		    if (lw6net_udp_recv (sys_context, sock1, buf1_recv, size, &incoming_ip, &incoming_port) == size)
		      {
			if (incoming_ip)
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					_x_ ("received \"%s\" on socket %d (%s:%d)"), buf1_recv, sock1, incoming_ip, incoming_port);
			    LW6SYS_FREE (sys_context, incoming_ip);
			  }
			ret_tmp = 1;
		      }
		  }
	      }
	  }
      }
    if (buf1_send)
      {
	LW6SYS_FREE (sys_context, buf1_send);
      }
    if (buf1_recv)
      {
	LW6SYS_FREE (sys_context, buf1_recv);
      }
    if (sock1 >= 0)
      {
	lw6net_socket_close (sys_context, &sock1);
      }
    if (sock2 >= 0)
      {
	lw6net_socket_close (sys_context, &sock2);
      }

    if (!ret_tmp)
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

void
_udp_lines_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  char *line = (char *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("line=\"%s\""), line);
}

/*
 * Testing functions in line.c
 */
static void
_test_line ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int sock1 = -1;
    int sock2 = -1;
    int ret_tmp = 0;
    int i;
    char *line;

    if (_prepare_2_tcp_socks (sys_context, &sock1, &sock2))
      {
	if (lw6net_send_line_tcp (sys_context, &sock1, _TEST_LINE1) && lw6net_send_line_tcp (sys_context, &sock1, _TEST_LINE2)
	    && lw6net_send_line_tcp (sys_context, &sock1, _TEST_LINE3))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    ret_tmp = 1;
	    for (i = 0; i < 4; ++i)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("receiving line %d"), i + 1);
		line = lw6net_recv_line_tcp (sys_context, &sock2);
		if (line)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("received line \"%s\""), line);
		    LW6SYS_FREE (sys_context, line);
		  }
		else
		  {
		    if (i < 3)
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("problem receiving line %d"), i + 1);
			ret_tmp = 0;
		      }
		  }
	      }
	  }
      }
    if (lw6net_socket_is_valid (sys_context, sock1))
      {
	lw6net_socket_close (sys_context, &sock1);
      }
    if (lw6net_socket_is_valid (sys_context, sock2))
      {
	lw6net_socket_close (sys_context, &sock2);
      }

    if (!ret_tmp)
      {
	ret = 0;
      }
  }

  {
    int sock1 = -1;
    int sock2 = -1;
    int ret_tmp = 0;
    int i;
    char *line;
    char *incoming_ip;
    int incoming_port;
    lw6sys_list_t *list = NULL;

    if (_prepare_2_udp_socks (sys_context, &sock1, &sock2))
      {
	if (lw6net_send_line_udp
	    (sys_context, sock2, _TEST_LINE1, LW6NET_ADDRESS_LOOPBACK,
	     _TEST_PORT)
	    && lw6net_send_line_udp (sys_context, sock2, _TEST_LINE2,
				     LW6NET_ADDRESS_LOOPBACK, _TEST_PORT)
	    && lw6net_send_line_udp (sys_context, sock2, _TEST_LINE3, LW6NET_ADDRESS_LOOPBACK, _TEST_PORT))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    lw6sys_idle (sys_context);
	    ret_tmp = 1;
	    for (i = 0; i < 4; ++i)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("receiving line %d"), i + 1);
		line = lw6net_recv_line_udp (sys_context, sock1, &incoming_ip, &incoming_port);
		if (line)
		  {
		    if (incoming_ip)
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("received line \"%s\" from %s:%d"), line, incoming_ip, incoming_port);
			LW6SYS_FREE (sys_context, incoming_ip);
		      }
		    LW6SYS_FREE (sys_context, line);
		  }
		else
		  {
		    if (i < 3)
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("problem receiving line %d"), i + 1);
			ret_tmp = 0;
		      }
		  }
	      }
	  }
	if (lw6net_udp_send (sys_context, sock2, _TEST_LINES_OK, strlen (_TEST_LINES_OK), LW6NET_ADDRESS_LOOPBACK, _TEST_PORT))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    lw6sys_idle (sys_context);
	    list = lw6net_recv_lines_udp (sys_context, sock1, &incoming_ip, &incoming_port);
	    if (list)
	      {
		lw6sys_list_map (sys_context, list, _udp_lines_callback, NULL);
		lw6sys_list_free (sys_context, list);
		LW6SYS_FREE (sys_context, incoming_ip);
	      }
	  }
	if (lw6net_udp_send (sys_context, sock2, _TEST_LINES_OK, strlen (_TEST_LINES_KO), LW6NET_ADDRESS_LOOPBACK, _TEST_PORT))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    lw6sys_idle (sys_context);
	    list = lw6net_recv_lines_udp (sys_context, sock1, &incoming_ip, &incoming_port);
	    if (list)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("received lines, when it should have returned an error"));
		ret_tmp = 0;
		lw6sys_list_free (sys_context, list);
		LW6SYS_FREE (sys_context, incoming_ip);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("truncated lines handled correctly"));
	      }
	  }
      }
    if (lw6net_socket_is_valid (sys_context, sock1))
      {
	lw6net_socket_close (sys_context, &sock1);
      }
    if (lw6net_socket_is_valid (sys_context, sock2))
      {
	lw6net_socket_close (sys_context, &sock2);
      }

    if (!ret_tmp)
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  const int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libnet CUnit test suite"));

  if (lw6net_init (sys_context, argc, argv, _TEST_NET_LOG))
    {
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_quit ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libnet CUnit test suite"));

  lw6net_quit (sys_context);
  ret = CUE_SUCCESS;

  return ret;
}

/**
 * lw6net_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libnet module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6net_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6cfg_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6net", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_address);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dns);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_if);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_tcp);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_udp);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_line);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6net_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @net module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6net_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
