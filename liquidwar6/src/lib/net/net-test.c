/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#include "net.h"
#include "net-internal.h"

#define TEST_ARGC 1
#define TEST_ARGV0 "prog"

#define TEST_NET_LOG 1
#define TEST_HOST_IP "127.0.0.1"
#define TEST_HOST_OK "localhost"
#define TEST_HOST_KO "now-if-this-is-a valid HOST there's a serious PROBLEM!!!!"
#define TEST_UNREACHABLE_IP "209.85.135.99"
#define TEST_UNREACHABLE_PORT 8000
#define TEST_TCP_CONNECT_DELAY 1000
#define TEST_TCP_ACCEPT_DELAY 1000
#define TEST_TCP_STREAM_DELAY 100
#define _TEST_TCP_MASSIVE_N 100000
#define _TEST_TCP_MASSIVE_STEP 1000
#define TEST_UDP_DELAY 100
#define TEST_BUF1_STR "foo"
#define TEST_BUF2_STR "what's the difference between a chicken and a house?"
#define TEST_LINE1 "foo"
#define TEST_LINE2 "a\tb\tc"
#define TEST_LINE3 "azerty azerty azerty azerty azerty azerty azerty azerty azerty azerty azerty azerty"
#define TEST_LINES_OK "there\nare\n4\nlines\n"
#define TEST_LINES_KO "this\nis\ntruncated"
/*
 * When testing, using the port following the default one,
 * to avoid the rather common bug: LW is actually running,
 * and test fails...
 */
#define TEST_PORT (LW6NET_DEFAULT_PORT+1)

static int
test_address ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    struct in_addr in;
    char *str = NULL;

    ret = 0;
    if (_lw6net_inet_aton (&in, TEST_HOST_IP))
      {
	str = _lw6net_inet_ntoa (in);
	if (str)
	  {
	    if (lw6sys_str_is_same (TEST_HOST_IP, str))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("\"%s\" converted to/from in_addr OK"), str);
		ret = 1;
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("\"%s\" transformed in \"%s\" by aton + ntoa"),
			    TEST_HOST_IP, str);
	      }
	    LW6SYS_FREE (str);
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_dns ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *ip = NULL;

    if (lw6net_dns_is_ip (TEST_HOST_IP) && !lw6net_dns_is_ip (TEST_HOST_OK))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("trivial IP parsing reports \"%s\" is an IP but not \"%s\""),
		    TEST_HOST_IP, TEST_HOST_OK);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("trivial IP parsing failed to report \"%s\" as an IP and \"%s\" as not"),
		    TEST_HOST_IP, TEST_HOST_OK);
	ret = 0;
      }
    ip = lw6net_dns_gethostbyname (TEST_HOST_IP);
    if (ip)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("gethostbyname on \"%s\" returns \"%s\""),
		    TEST_HOST_IP, ip);
	LW6SYS_FREE (ip);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("gethostbyname on \"%s\" failed"), TEST_HOST_IP);
	ret = 0;
      }
    ip = lw6net_dns_gethostbyname (TEST_HOST_OK);
    if (ip)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("gethostbyname on \"%s\" returns \"%s\""),
		    TEST_HOST_OK, ip);
	LW6SYS_FREE (ip);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("gethostbyname on \"%s\" failed"), TEST_HOST_OK);
	ret = 0;
      }
    ip = lw6net_dns_gethostbyname (TEST_HOST_KO);
    if (ip)
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("gethostbyname on \"%s\" returns \"%s\", this is... surprising"),
		    TEST_HOST_KO, ip);
	LW6SYS_FREE (ip);
	ret = 0;
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("gethostbyname on \"%s\" returns NULL, this is fine"),
		    TEST_HOST_KO);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in if.c
 */
static int
test_if ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *ip = NULL;
    char *public_url = NULL;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("trying to guess local interface IP"));
    ip = lw6net_if_guess_local ();
    if (ip)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("local interface IP is \"%s\""),
		    ip);
	LW6SYS_FREE (ip);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("unable to guess local interface, this is a problem only if machine *really* has no network available"));
      }
    public_url = lw6net_if_guess_public_url (LW6NET_ADDRESS_ANY, TEST_PORT);
    if (public_url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("public URL on default port would be \"%s\""),
		    public_url);
	LW6SYS_FREE (public_url);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to guess public URL"));
	ret = 0;
      }
    public_url =
      lw6net_if_guess_public_url (TEST_UNREACHABLE_IP, LW6NET_HTTP_PORT);
    if (public_url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("public URL on http port with IP %s would be \"%s\""),
		    TEST_UNREACHABLE_IP, public_url);
	LW6SYS_FREE (public_url);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to guess public URL"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
prepare_2_tcp_socks (int *sock1, int *sock2)
{
  int ret = 0;
  int listening_sock = -1;
  int connect_sock = -1;
  int accept_sock = -1;
  char *accept_ip = NULL;
  int accept_port = 0;

  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("listening on port %d (TCP)"),
	      TEST_PORT);
  listening_sock = lw6net_tcp_listen (LW6NET_ADDRESS_ANY, TEST_PORT);
  if (listening_sock >= 0)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_ ("TCP socket %d listening on port %d"), listening_sock,
		  TEST_PORT);

      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_ ("trying to connect on localhost %s:%d"),
		  LW6NET_ADDRESS_LOOPBACK, TEST_PORT);
      connect_sock =
	lw6net_tcp_connect (LW6NET_ADDRESS_LOOPBACK,
			    TEST_PORT, TEST_TCP_CONNECT_DELAY);
      if (lw6net_socket_is_valid (connect_sock))
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _x_ ("TCP socket %d connected on %s:%d"),
		      connect_sock, LW6NET_ADDRESS_LOOPBACK, TEST_PORT);
	  accept_sock =
	    lw6net_tcp_accept (&accept_ip,
			       &accept_port, listening_sock,
			       TEST_TCP_ACCEPT_DELAY);
	  if (lw6net_socket_is_valid (accept_sock))
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _x_ ("TCP socket %d accepted %s:%d"),
			  accept_sock, accept_ip, accept_port);
	    }
	  if (accept_ip)
	    {
	      LW6SYS_FREE (accept_ip);
	    }
	}
    }
  if (lw6net_socket_is_valid (accept_sock)
      && lw6net_socket_is_valid (connect_sock))
    {
      (*sock1) = accept_sock;
      (*sock2) = connect_sock;
      ret = 1;
    }
  else
    {
      if (lw6net_socket_is_valid (accept_sock))
	{
	  lw6net_socket_close (&accept_sock);
	}
      if (lw6net_socket_is_valid (connect_sock))
	{
	  lw6net_socket_close (&connect_sock);
	}
      (*sock1) = (*sock2) = LW6NET_SOCKET_INVALID;
    }
  if (lw6net_socket_is_valid (listening_sock))
    {
      lw6net_socket_close (&listening_sock);
    }

  return ret;
}

/*
 * Testing functions in tcp.c
 */
static int
test_tcp ()
{
  int ret = 1;
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

    sock1 =
      lw6net_tcp_connect (TEST_UNREACHABLE_IP, TEST_UNREACHABLE_PORT,
			  TEST_TCP_CONNECT_DELAY);
    if (!lw6net_socket_is_valid (sock1))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("unable to connect on \"%s:%d\", this is fine"),
		    TEST_UNREACHABLE_IP, TEST_UNREACHABLE_PORT);
	sock1 =
	  lw6net_tcp_connect (TEST_UNREACHABLE_IP, TEST_UNREACHABLE_PORT,
			      TEST_TCP_CONNECT_DELAY);
	if (!lw6net_socket_is_valid (sock1))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("unable to connect on \"%s:%d\", this is fine, this second test was just here to trigger the code that skips destinations marked as not connectable"),
			TEST_UNREACHABLE_IP, TEST_UNREACHABLE_PORT);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("was able to connect on \"%s:%d\" the second time this is (very!) strange..."),
			TEST_UNREACHABLE_IP, TEST_UNREACHABLE_PORT);
	    lw6net_socket_close (&sock1);
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("was able to connect on \"%s:%d\", this is strange..."),
		    TEST_UNREACHABLE_IP, TEST_UNREACHABLE_PORT);
	lw6net_socket_close (&sock1);
	ret = 0;
      }

    if (prepare_2_tcp_socks (&sock1, &sock2))
      {

	size = strlen (TEST_BUF1_STR) + 1;
	buf1_send = lw6sys_str_copy (TEST_BUF1_STR);
	buf1_recv = LW6SYS_CALLOC (strlen (TEST_BUF1_STR) + 1);

	if (buf1_send && buf1_recv)
	  {
	    if (lw6net_tcp_send
		(&sock1, buf1_send, size, TEST_TCP_STREAM_DELAY, 1))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("sent \"%s\" on TCP socket %d"),
			    buf1_send, sock1);
		received =
		  lw6net_tcp_peek (&sock2, NULL, size, TEST_TCP_STREAM_DELAY);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("%d bytes available on TCP socket %d"),
			    received, sock2);
		if (received == size
		    && lw6net_tcp_recv (&sock2,
					buf1_recv, size,
					TEST_TCP_STREAM_DELAY, 1))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("received \"%s\" on TCP socket %d"),
				buf1_recv, sock2);
		    ret_tmp1 = 1;
		  }
	      }
	  }
	/*
	 * Now proceeding with a massive data test
	 */
	size = strlen (TEST_BUF2_STR) + 1;
	buf2_send = lw6sys_str_copy (TEST_BUF2_STR);
	buf2_recv = LW6SYS_CALLOC (strlen (TEST_BUF2_STR) + 1);
	if (buf2_send && buf2_recv)
	  {
	    massive_begin_timestamp = lw6sys_get_timestamp ();
	    for (j = 0;
		 j < _TEST_TCP_MASSIVE_N && sent_i < _TEST_TCP_MASSIVE_N
		 && lw6net_tcp_is_alive (&sock1)
		 && received_i < _TEST_TCP_MASSIVE_N
		 && lw6net_tcp_is_alive (&sock2); ++j)
	      {
		send_failed = 0;
		for (i = 0;
		     i < _TEST_TCP_MASSIVE_N && sent_i < _TEST_TCP_MASSIVE_N
		     && !send_failed && lw6net_tcp_is_alive (&sock1); ++i)
		  {
		    if (!(sent_i % _TEST_TCP_MASSIVE_STEP))
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("TCP massive data test, send sent_i=%d"),
				    sent_i);
		      }
		    if (lw6net_tcp_send
			(&sock1, buf2_send, size, TEST_TCP_STREAM_DELAY, 1))
		      {
			sent_i++;
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("unable to send \"%s\" on TCP socket, massive data (sent_i=%d)"),
				    buf2_send, sent_i);
			if (internal_tcp_buffer_size == 0)
			  {
			    /*
			     * Raw estimation of internal TCP buffer size, we round
			     * it to the closest thousand, there's no point in
			     * doing more...
			     */
			    internal_tcp_buffer_size =
			      (sent_i * size) / 1000 * 1000;
			  }
			send_failed = 1;
		      }
		  }

		for (i = 0;
		     i < _TEST_TCP_MASSIVE_N && received_i < sent_i
		     && lw6net_tcp_is_alive (&sock2); ++i)
		  {
		    {
		      if (!(received_i % _TEST_TCP_MASSIVE_STEP))
			{
			  lw6sys_log (LW6SYS_LOG_NOTICE,
				      _x_
				      ("TCP massive data test, recv received_i=%d"),
				      received_i);
			}
		      received =
			lw6net_tcp_peek (&sock2, NULL, size,
					 TEST_TCP_STREAM_DELAY);
		      if (received == size
			  && lw6net_tcp_recv (&sock2, buf2_recv, size,
					      TEST_TCP_STREAM_DELAY, 1))
			{
			  received_i++;
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("unable to recv %d bytes on TCP socket, massive data (received_i=%d)"),
				      size, received_i);
			}
		    }
		  }
	      }
	    massive_end_timestamp = lw6sys_get_timestamp ();
	    massive_duration =
	      massive_end_timestamp - massive_begin_timestamp;
	    if (massive_duration)
	      {
		/*
		 * It's important to multiplicate by LW6SYS_TICKS_PER_SEC after
		 * the first value is computed, because a) it avoids out of range
		 * error on int32 and b) it rounds to the closest thousand, and
		 * this way one does not have the illusion the result is very precise.
		 * And it's good, because it's not.
		 */
		bytes_per_sec =
		  ((_TEST_TCP_MASSIVE_N * size) / massive_duration) *
		  LW6SYS_TICKS_PER_SEC;
	      }
	    if (sent_i == _TEST_TCP_MASSIVE_N
		&& received_i == _TEST_TCP_MASSIVE_N)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("OK, was able to send and receive %d bytes splitted in %d messages"),
			    _TEST_TCP_MASSIVE_N * size, _TEST_TCP_MASSIVE_N);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("exchange took %d msec, bytes_per_sec=%d"),
			    massive_duration, bytes_per_sec);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("raw estimation of internal TCP buffer size is %d bytes"),
			    internal_tcp_buffer_size);

		ret_tmp2 = 1;
	      }
	    else
	      {
		if (sent_i != _TEST_TCP_MASSIVE_N)
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_ ("only sent %d messages out of %d"),
				sent_i, _TEST_TCP_MASSIVE_N);
		  }
		if (received_i != _TEST_TCP_MASSIVE_N)
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_ ("only received %d messages out of %d"),
				received_i, _TEST_TCP_MASSIVE_N);
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
	LW6SYS_FREE (buf1_send);
      }
    if (buf1_recv)
      {
	LW6SYS_FREE (buf1_recv);
      }
    if (buf2_send)
      {
	LW6SYS_FREE (buf2_send);
      }
    if (buf2_recv)
      {
	LW6SYS_FREE (buf2_recv);
      }
    if (sock1 >= 0)
      {
	lw6net_socket_close (&sock1);
      }
    if (sock2 >= 0)
      {
	lw6net_socket_close (&sock2);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
prepare_2_udp_socks (int *sock1, int *sock2)
{
  int ret = 0;
  int server_sock = -1;
  int client_sock = -1;

  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("listening on port %d (UDP)"),
	      TEST_PORT);
  server_sock = lw6net_udp_server (LW6NET_ADDRESS_ANY, TEST_PORT);
  if (server_sock >= 0)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_ ("UDP socket %d listening on port %d"), server_sock,
		  TEST_PORT);
      client_sock = lw6net_udp_client ();
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
	  lw6net_socket_close (&server_sock);
	}
      if (client_sock >= 0)
	{
	  lw6net_socket_close (&client_sock);
	}
      (*sock1) = (*sock2) = LW6NET_SOCKET_INVALID;
    }

  return ret;
}

/*
 * Testing functions in udp.c
 */
static int
test_udp ()
{
  int ret = 1;
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

    if (prepare_2_udp_socks (&sock1, &sock2))
      {
	size = strlen (TEST_BUF1_STR) + 1;
	buf1_send = lw6sys_str_copy (TEST_BUF1_STR);
	buf1_recv = LW6SYS_CALLOC (strlen (TEST_BUF1_STR) + 1);

	if (buf1_send && buf1_recv)
	  {
	    if (lw6net_udp_send
		(sock2, buf1_send, size,
		 LW6NET_ADDRESS_LOOPBACK, TEST_PORT) == size)
	      {
		lw6sys_delay (TEST_UDP_DELAY);
		if (lw6net_udp_peek
		    (sock1, buf1_recv, size,
		     &incoming_ip, &incoming_port) == size)
		  {
		    if (incoming_ip)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("%d bytes available on socket %d (%s:%d)"),
				    size, sock1, incoming_ip, incoming_port);
			LW6SYS_FREE (incoming_ip);
		      }
		    if (lw6net_udp_recv
			(sock1, buf1_recv, size,
			 &incoming_ip, &incoming_port) == size)
		      {
			if (incoming_ip)
			  {
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_
					("received \"%s\" on socket %d (%s:%d)"),
					buf1_recv, sock1, incoming_ip,
					incoming_port);
			    LW6SYS_FREE (incoming_ip);
			  }
			ret_tmp = 1;
		      }
		  }
	      }
	  }
      }
    if (buf1_send)
      {
	LW6SYS_FREE (buf1_send);
      }
    if (buf1_recv)
      {
	LW6SYS_FREE (buf1_recv);
      }
    if (sock1 >= 0)
      {
	lw6net_socket_close (&sock1);
      }
    if (sock2 >= 0)
      {
	lw6net_socket_close (&sock2);
      }

    if (!ret_tmp)
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

void
_udp_lines_callback (void *func_data, void *data)
{
  char *line = (char *) data;

  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("line=\"%s\""), line);
}

/*
 * Testing functions in line.c
 */
static int
test_line ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int sock1 = -1;
    int sock2 = -1;
    int ret_tmp = 0;
    int i;
    char *line;

    if (prepare_2_tcp_socks (&sock1, &sock2))
      {
	if (lw6net_send_line_tcp (&sock1, TEST_LINE1) &&
	    lw6net_send_line_tcp (&sock1, TEST_LINE2) &&
	    lw6net_send_line_tcp (&sock1, TEST_LINE3))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    ret_tmp = 1;
	    for (i = 0; i < 4; ++i)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("receiving line %d"),
			    i + 1);
		line = lw6net_recv_line_tcp (&sock2);
		if (line)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("received line \"%s\""), line);
		    LW6SYS_FREE (line);
		  }
		else
		  {
		    if (i < 3)
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_ ("problem receiving line %d"), i + 1);
			ret_tmp = 0;
		      }
		  }
	      }
	  }
      }
    if (lw6net_socket_is_valid (sock1))
      {
	lw6net_socket_close (&sock1);
      }
    if (lw6net_socket_is_valid (sock2))
      {
	lw6net_socket_close (&sock2);
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

    if (prepare_2_udp_socks (&sock1, &sock2))
      {
	if (lw6net_send_line_udp
	    (sock2, TEST_LINE1, LW6NET_ADDRESS_LOOPBACK,
	     TEST_PORT)
	    && lw6net_send_line_udp (sock2, TEST_LINE2,
				     LW6NET_ADDRESS_LOOPBACK,
				     TEST_PORT)
	    && lw6net_send_line_udp (sock2, TEST_LINE3,
				     LW6NET_ADDRESS_LOOPBACK, TEST_PORT))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    lw6sys_idle ();
	    ret_tmp = 1;
	    for (i = 0; i < 4; ++i)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("receiving line %d"),
			    i + 1);
		line =
		  lw6net_recv_line_udp (sock1, &incoming_ip, &incoming_port);
		if (line)
		  {
		    if (incoming_ip)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("received line \"%s\" from %s:%d"),
				    line, incoming_ip, incoming_port);
			LW6SYS_FREE (incoming_ip);
		      }
		    LW6SYS_FREE (line);
		  }
		else
		  {
		    if (i < 3)
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_ ("problem receiving line %d"), i + 1);
			ret_tmp = 0;
		      }
		  }
	      }
	  }
	if (lw6net_udp_send
	    (sock2, TEST_LINES_OK, strlen (TEST_LINES_OK),
	     LW6NET_ADDRESS_LOOPBACK, TEST_PORT))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    lw6sys_idle ();
	    list =
	      lw6net_recv_lines_udp (sock1, &incoming_ip, &incoming_port);
	    if (list)
	      {
		lw6sys_list_map (list, _udp_lines_callback, NULL);
		lw6sys_list_free (list);
		LW6SYS_FREE (incoming_ip);
	      }
	  }
	if (lw6net_udp_send
	    (sock2, TEST_LINES_OK, strlen (TEST_LINES_KO),
	     LW6NET_ADDRESS_LOOPBACK, TEST_PORT))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("lines sent"));
	    lw6sys_idle ();
	    list =
	      lw6net_recv_lines_udp (sock1, &incoming_ip, &incoming_port);
	    if (list)
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("received lines, when it should have returned an error"));
		ret_tmp = 0;
		lw6sys_list_free (list);
		LW6SYS_FREE (incoming_ip);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("truncated lines handled correctly"));
	      }
	  }
      }
    if (lw6net_socket_is_valid (sock1))
      {
	lw6net_socket_close (&sock1);
      }
    if (lw6net_socket_is_valid (sock2))
      {
	lw6net_socket_close (&sock2);
      }

    if (!ret_tmp)
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6net_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @net module test suite. This one could fail if some
 * sockets are already bound, for instance. It's still run even
 * in check-only (mode=0) mode.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6net_test (int mode)
{
  int ret = 0;
  const int argc = TEST_ARGC;
  const char *argv[TEST_ARGC] = { TEST_ARGV0 };

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6cfg_test (mode);
    }

  if (lw6net_init (argc, argv, TEST_NET_LOG))
    {
      ret = test_address ()
	&& test_dns () && test_if () && test_tcp () && test_udp ()
	&& test_line ();

      lw6net_quit ();
    }

  return ret;
}
