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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "net.h"
#include "net-internal.h"

void
_lw6net_delay_msec_to_timeval (lw6sys_context_t * sys_context, struct timeval *tv, int delay_msec)
{
  memset (tv, 0, sizeof (struct timeval));

  tv->tv_sec = delay_msec / 1000;
  tv->tv_usec = delay_msec % 1000;
}

/**
 * lw6net_tcp_listen
 *
 * @sys_context: global system context
 * @ip: IP address to bind to
 * @port: IP port to listen on
 *
 * Listens in TCP on a given port.
 *
 * Return value: >=0 on success, -1 on failure.
 */
int
lw6net_tcp_listen (lw6sys_context_t * sys_context, const char *ip, int port)
{
  int backlog;
  int sock = LW6NET_SOCKET_INVALID;

  backlog = _lw6net_global_context->const_data.listen_backlog;

  sock = _lw6net_socket_bind (sys_context, ip, port, SOCK_STREAM);

  if (lw6net_socket_is_valid (sys_context, sock))
    {
      if (listen (sock, backlog) < 0)
	{
	  lw6net_last_error (sys_context);
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("listen() failed"));

	  lw6net_socket_close (sys_context, &sock);
	}
    }

  return sock;
}

/**
 * lw6net_tcp_accept
 *
 * @sys_context: global system context
 * @incoming_ip: address of remote peer (out param, dynamically allocated)
 * @incoming_port: port of remote peer (out param)
 * @listening_sock: socket to listen on
 * @delay_msec: delay, in msec, after which we stop accepting
 *
 * Accepts for a connexion on the given socket.
 *
 * Return value: the new socket (>=0) if accepted, else -1
 */
int
lw6net_tcp_accept (lw6sys_context_t * sys_context, char **incoming_ip, int *incoming_port, int listening_sock, int delay_msec)
{
  int new_sock = LW6NET_SOCKET_INVALID;
  int accepted = 0;
  struct sockaddr_in name;
  socklen_t namelen = sizeof (struct sockaddr_in);
  fd_set read;
  struct timeval tv;
  int res;
  int enable = 1;
  int disable = 0;
  struct linger li;

  memset (&name, 0, sizeof (name));

  if (incoming_ip)
    {
      *incoming_ip = NULL;
    }
  if (incoming_port)
    {
      *incoming_port = 0;
    }

  if (listening_sock >= 0)
    {
      FD_ZERO (&read);
      FD_SET (listening_sock, &read);
      _lw6net_delay_msec_to_timeval (sys_context, &tv, delay_msec);

      res = select (listening_sock + 1, &read, NULL, NULL, &tv);
      if (res >= 1)
	{
	  new_sock = accept (listening_sock, (struct sockaddr *) &name, &namelen);
	  if (new_sock >= 0)
	    {
	      li.l_onoff = 0;
	      li.l_linger = 0;

	      if (setsockopt (new_sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &enable, sizeof (int)))
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_KEEPALIVE) failed"));
		}
	      /*
	       * SO_NOSIGPIPE not defined on GNU/Linux, in that case
	       * we use MSG_NOSIGNAL on send calls
	       */
#ifdef SO_NOSIGPIPE
	      if (setsockopt (new_sock, SOL_SOCKET, SO_NOSIGPIPE, (char *) &enable, sizeof (int)))
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_NOSIGPIPE) failed"));
		}
#endif // SO_NOSIGPIPE
	      if (setsockopt (new_sock, SOL_SOCKET, SO_OOBINLINE, (char *) &disable, sizeof (int)))
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_OOBINLINE) failed"));
		}
	      if (setsockopt (new_sock, SOL_SOCKET, SO_LINGER, (char *) &li, sizeof (struct linger)))
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_LINGER) failed"));
		}

	      //fcntl (new_sock, F_SETFL, O_NONBLOCK, 0);

	      if (incoming_ip)
		{
		  (*incoming_ip) = _lw6net_inet_ntoa (sys_context, name.sin_addr);
		  if (*incoming_ip)
		    {
		      _lw6net_counters_register_socket (sys_context, &(_lw6net_global_context->counters));
		      accepted = 1;
		    }
		}
	      if (incoming_port)
		{
		  (*incoming_port) = (int) ntohs (name.sin_port);
		}

	      if (incoming_ip && incoming_port && *incoming_ip && (*incoming_port) >= 0)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("socket %d accepted from %s:%d"), new_sock, *incoming_ip, *incoming_port);
		}
	    }
	  else
	    {
	      lw6net_last_error (sys_context);
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("accept() failed"));
	    }

	}
    }

  if (new_sock >= 0 && !accepted)
    {
      if (incoming_ip)
	{
	  if (*incoming_ip)
	    {
	      LW6SYS_FREE (sys_context, *incoming_ip);
	    }
	  (*incoming_ip) = NULL;
	}
      if (incoming_port)
	{
	  (*incoming_port) = 0;
	}
#ifdef LW6_MS_WINDOWS
      if (closesocket (new_sock))
#else
      if (close (new_sock))
#endif
	{
	  lw6net_last_error (sys_context);
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("close() failed"));
	}
      new_sock = LW6NET_SOCKET_INVALID;
    }

  return new_sock;
}

/**
 * lw6net_tcp_connect
 *
 * @sys_context: global system context
 * @ip: address to connect to
 * @port: port to connect to
 * @delay_msec: delay before we consider it's too late
 *
 * Tries to connect on a given socket.
 *
 * Return value: socket (>=0) on success, else -1
 */
int
lw6net_tcp_connect (lw6sys_context_t * sys_context, const char *ip, int port, int delay_msec)
{
  int sock = LW6NET_SOCKET_INVALID;
  int connect_ret = 0;
  int connect_async = 0;
  int select_ret = 0;
  int connected = 0;
  int refused = 0;
  struct sockaddr_in name;
#ifdef SO_NOSIGPIPE
  int enable = 1;
#endif // SO_NOSIGPIPE
  fd_set write;
  struct timeval tv;
  int64_t origin = 0;
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#else
  struct sockaddr getpeername_address;
  socklen_t getpeername_address_len = sizeof (struct sockaddr);
#endif

  if (lw6net_is_connectable (sys_context, ip, port))
    {
      sock = socket (AF_INET, SOCK_STREAM, 0);
      if (sock >= 0)
	{
	  if (lw6net_socket_set_blocking_mode (sys_context, sock, 0))
	    {
	      name.sin_family = AF_INET;
	      name.sin_addr.s_addr = INADDR_ANY;
	      name.sin_port = 0;
	      if (bind (sock, (struct sockaddr *) &name, sizeof (name)) >= 0)
		{
		  name.sin_family = AF_INET;
		  if (_lw6net_inet_aton (sys_context, &name.sin_addr, ip))
		    {
		      name.sin_port = htons (port);
		      connect_ret = connect (sock, (struct sockaddr *) &name, sizeof (name));
		      if (connect_ret >= 0)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				      _x_
				      ("connect on \"%s:%d\" returned with a successfull code %d, this is very strange since we should be in non-blocking mode"),
				      ip, port, connect_ret);
			}
		      else
			{
#ifdef LW6_MS_WINDOWS
			  winerr = WSAGetLastError ();
			  switch (winerr)
			    {
			      /*
			       * Fixed 35104 (warning on windows build) by specifically
			       * adding both INPROGRESS and WOULDBLOCK in acceptable values.
			       */
			    case WSAEINTR:
			    case WSAEINPROGRESS:
			    case WSAEWOULDBLOCK:
			      connect_async = 1;
			      break;
			      /*
			       * UNREACH is not reported as an error, only an info,
			       * it's very common not to be able to reach host, it
			       * can be down or you can just be playing locally.
			       */
			    case WSAEHOSTUNREACH:
			    case WSAEHOSTDOWN:
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't connect on \"%s:%d\", host unreachable or down"), ip, port);
			      break;
			    default:
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					  _x_ ("connect on \"%s:%d\" failed, did not return WSAEINPROGRESS or WSAEWOULDBLOCK (code=%d)"), ip, port, winerr);
			    }
#else
			  switch (errno)
			    {
			      /*
			       * Fixed 35104 (warning on windows build) by specifically
			       * adding both INPROGRESS and WOULDBLOCK in acceptable values.
			       */
			    case EINTR:
			    case EINPROGRESS:
			      /*
			       * Here we could test for EAGAIN too but on most systems
			       * EAGAIN and EWOULDBLOCK are the same (ie glibc)
			       * so this would yield a compilation warning
			       */
			    case EWOULDBLOCK:
			      connect_async = 1;
			      break;
			      /*
			       * UNREACH is not reported as an error, only an info,
			       * it's very common not to be able to reach host, it
			       * can be down or you can just be playing locally.
			       */
			    case EHOSTUNREACH:
			    case EHOSTDOWN:
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't connect on \"%s:%d\", host unreachable or down"), ip, port);
			      break;
			    default:
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
					  _x_ ("connect on \"%s:%d\" failed, did not return EINPROGRESS or EWOULDBLOCK (code=%d)"), ip, port, errno);
			    }
#endif
			  if (connect_async)
			    {
			      FD_ZERO (&write);
			      FD_SET (sock, &write);
			      _lw6net_delay_msec_to_timeval (sys_context, &tv, delay_msec);

			      origin = lw6sys_get_timestamp (sys_context);
			      while (!connected && !refused && (lw6sys_get_timestamp (sys_context) < (origin + delay_msec)))
				{
				  select_ret = select (sock + 1, NULL, &write, NULL, &tv);

				  if (select_ret > 0)
				    {
				      if (FD_ISSET (sock, &write))
					{
#ifdef LW6_MS_WINDOWS
					  connected = 1;
#else
					  /*
					   * This is a mess, logically, a select
					   * on write should yield an error if
					   * not connected, but this is not the
					   * case. Following advices on
					   * http://cr.yp.to/docs/connect.html
					   * we use getpeername, which should
					   * (at least on GNU/Linux) tell
					   * something is wrong.
					   */
					  memset (&getpeername_address, 0, getpeername_address_len);
					  if (!getpeername (sock, &getpeername_address, &getpeername_address_len))
					    {
					      /*
					       * SO_NOSIGPIPE not defined on GNU/Linux, in that case
					       * we use MSG_NOSIGNAL on send calls
					       */
#ifdef SO_NOSIGPIPE
					      if (setsockopt (sock, SOL_SOCKET, SO_NOSIGPIPE, (char *) &enable, sizeof (int)))
						{
						  lw6net_last_error (sys_context);
						  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setsockopt(SO_NOSIGPIPE) failed"));
						}
#endif // SO_NOSIGPIPE

					      connected = 1;
					    }
					  else
					    {
					      lw6sys_log (sys_context,
							  LW6SYS_LOG_INFO,
							  _x_ ("async connect on \"%s:%d\" failed, getpeername didn't return 0 as expected"), ip, port);
					      refused = 1;
					    }
#endif
					}
				      else
					{
					  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("async connect on \"%s:%d\" failed, FD_ISSET returned 0"), ip, port);
					  refused = 1;
					}
				    }
				}
			    }
			}
		    }
		}
	      else
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bind() failed"));
		}
	    }
	}
      if (connected)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("socket %d connected on %s:%d"), sock, ip, port);

	  _lw6net_counters_register_socket (sys_context, &(_lw6net_global_context->counters));
	}
      else
	{
	  /*
	   * Here, no peculiar warning, connect() is definitely
	   * the kind of function which *will* fail very
	   * often, we certainly don't want this to fill our
	   * log...
	   */
	  lw6net_set_connectable (sys_context, ip, port, 0);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("destination %s:%d already marked as not connectable, not even trying to connect to it"), ip, port);
    }

  if (lw6net_socket_is_valid (sys_context, sock) && !connected)
    {
      if (close (sock))
	{
	  lw6net_last_error (sys_context);
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("close() failed"));
	}
      sock = LW6NET_SOCKET_INVALID;
    }

  return sock;
}

/**
 * lw6net_tcp_send
 *
 * @sys_context: global system context
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @delay_msec: delay after which we give up
 * @loop: accept to do several calls if needed
 *
 * Will send data, possibly looping until all is send, and waiting for
 * a maximum time of delay_msec. If the send reveals a socket closed
 * by peer or other serious problem, socket is closed and sock set to -1.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6net_tcp_send (lw6sys_context_t * sys_context, int *sock, const char *buf, int len, int delay_msec, int loop)
{
  int ret = 0;
  fd_set write;
  struct timeval tv;
  int select_ret;
  int total_sent = 0;
  int sent = 0;
  int chunk_size;
#ifdef MSG_NOSIGNAL
  /*
   * Defining MSG_NOSIGNAL is important because in a heavy
   * multithreaded context, this signal is harder to handle
   * than a proper "send failed with -1", and threads do
   * tend to generate this signal. We also use SO_NOSIGPIPE
   * on socket creation but some platforms, including GNU/Linux,
   * do not support it.
   */
  int flags = MSG_NOSIGNAL;
#else
  int flags = 0;
#endif
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif
  int try_again = 0;
  int64_t limit_timestamp = 0LL;

  if (lw6net_socket_is_valid (sys_context, *sock))
    {
      ret = 1;

      chunk_size = _lw6net_global_context->const_data.chunk_size;
      total_sent = 0;
      while (total_sent != len && ret)
	{
	  try_again = 0;

	  FD_ZERO (&write);
	  FD_SET (*sock, &write);
	  _lw6net_delay_msec_to_timeval (sys_context, &tv, delay_msec);

	  select_ret = select ((*sock) + 1, NULL, &write, NULL, &tv);

	  if (select_ret < 0)
	    {
#ifdef LW6_MS_WINDOWS
	      winerr = WSAGetLastError ();
	      if (winerr != WSAEINTR && winerr != WSAENOBUFS)
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("error sending data on socket %d select() failed, code=%d"), *sock, winerr);
		  lw6net_socket_close (sys_context, sock);
		  ret = 0;
		}
#else
	      if (errno != EINTR && errno != ENOBUFS)
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("error sending data on socket %d select() failed"), *sock);
		  lw6net_socket_close (sys_context, sock);
		  ret = 0;
		}
#endif
	      if (ret)
		{
		  /*
		   * Got interrupted by something which is not really an error,
		   * try again, even if not looping.
		   */
		  try_again = 1;
		}
	    }
	  else
	    {
	      if (FD_ISSET (*sock, &write))
		{
		  sent = send (*sock, buf + total_sent, lw6sys_imin (len - total_sent, chunk_size), flags);
		  if (sent > 0 && sent <= len - total_sent)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%d bytes sent on TCP socket %d"), sent, *sock);
		      total_sent += sent;
		    }
		  else
		    {
		      if (sent < 0)
			{
#ifdef LW6_MS_WINDOWS
			  winerr = WSAGetLastError ();
			  if (winerr != WSAEINTR && winerr != WSAENOBUFS && winerr != WSAEWOULDBLOCK)
			    {
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't send data on socket %d send() failed, code=%d"), *sock, winerr);
			      lw6net_socket_close (sys_context, sock);
			      ret = 0;
			    }
#else
			  if (errno != EINTR && errno != ENOBUFS && errno != EAGAIN && errno != EWOULDBLOCK)
			    {
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't send data on socket %d send() failed"), *sock);
			      lw6net_socket_close (sys_context, sock);
			      ret = 0;
			    }
#endif
			  if (ret)
			    {
			      /*
			       * Got interrupted by something which is not really an error,
			       * try again, even if not looping.
			       */
			      try_again = 1;

			    }
			}
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("socket %d is not writeable, it's very likely that TCP system buffer is full"), *sock);
		}
	    }

	  if ((!loop) && (!try_again) && (total_sent != len))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't send data on socket %d (%d/%d)"), *sock, total_sent, len);
	      ret = 0;
	    }

	  /*
	   * Now we only use this get_timestamp foolishness
	   * if we are heading towards a second try, for the
	   * first one, the select timeout is likley to be
	   * enough. But in looping mode, we need some barrier,
	   * because if buffer is full, select will just wait
	   * forever, while never reporting the socket as bad...
	   */
	  if (total_sent != len && ret)
	    {
	      if (limit_timestamp == 0LL)
		{
		  limit_timestamp = lw6sys_get_timestamp (sys_context) + delay_msec;
		}
	      else
		{
		  if (lw6sys_get_timestamp (sys_context) > limit_timestamp)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("timeout on socket %d, could only send %d bytes out of %d in %d msec"), *sock, total_sent, len, delay_msec);
		      ret = 0;
		    }
		}
	    }
	}

      if (total_sent > 0)
	{
	  _lw6net_counters_register_send (sys_context, &(_lw6net_global_context->counters), total_sent);
	  _lw6net_log_tcp_send (sys_context, &(_lw6net_global_context->log), buf, total_sent);
	}
    }

  return ret;
}

/**
 * lw6net_tcp_peek
 *
 * @sys_context: global system context
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @delay_msec: maximum time to wait
 *
 * Tells wether data is available. Will actually fill the buffer
 * with the data, but not remove it from the fifo list.
 * If the peel reveals a socket closed
 * by peer or other serious problem, socket is closed and sock set to -1.
 *
 * Return value: number of bytes available, 0 when nothing
 */
int
lw6net_tcp_peek (lw6sys_context_t * sys_context, int *sock, char *buf, int len, int delay_msec)
{
  fd_set read;
  struct timeval tv;
  int select_ret;
  char *buf2;
  int available = 0;
#ifdef MSG_NOSIGNAL
  /*
   * Defining MSG_NOSIGNAL is important because in a heavy
   * multithreaded context, this signal is harder to handle
   * than a proper "send failed with -1", and threads do
   * tend to generate this signal. We also use SO_NOSIGPIPE
   * on socket creation but some platforms, including GNU/Linux,
   * do not support it.
   */
  int flags = MSG_PEEK | MSG_NOSIGNAL;
#else
  int flags = MSG_PEEK;
#endif
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif

  if (*sock >= 0)
    {
      if (buf)
	{
	  buf2 = buf;
	}
      else
	{
	  buf2 = (char *) LW6SYS_MALLOC (sys_context, len);
	}
      if (buf2)
	{
	  FD_ZERO (&read);
	  FD_SET (*sock, &read);
	  _lw6net_delay_msec_to_timeval (sys_context, &tv, delay_msec);

	  select_ret = select ((*sock) + 1, &read, NULL, NULL, &tv);

	  if (select_ret < 0)
	    {
#ifdef LW6_MS_WINDOWS
	      winerr = WSAGetLastError ();
	      if (winerr != WSAEINTR)
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("error peeking data on socket %d select() failed, code=%d"), *sock, winerr);
		}
#else
	      if (errno != EINTR)
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("error peeking data on socket %d select() failed"), *sock);
		}
#endif
	    }
	  else
	    {
	      if (FD_ISSET (*sock, &read))
		{
		  available = recv (*sock, buf2, len, flags);
		  if (available >= 0)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%d bytes available on TCP socket %d"), available, *sock);
		    }
		  else
		    {
		      if (available < 0)
			{
#ifdef LW6_MS_WINDOWS
			  winerr = WSAGetLastError ();
			  if (winerr != WSAEINTR && winerr != WSAEWOULDBLOCK)
			    {
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't peek data on socket %d, code=%d"), *sock, winerr);
			      lw6net_socket_close (sys_context, sock);
			    }
#else
			  if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
			    {
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't peek data on socket %d"), *sock);
			      lw6net_socket_close (sys_context, sock);
			    }
#endif
			}
		    }
		}
	    }

	  if (!buf)
	    {
	      LW6SYS_FREE (sys_context, buf2);
	    }
	}
    }

  if (available < 0)
    {
      available = 0;
    }

  return available;
}

/**
 * lw6net_tcp_recv
 *
 * @sys_context: global system context
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @delay_msec: maximum time to wait
 * @loop: wether to loop or not
 *
 * If data is available, put it in buffer. If needed, will
 * loop until @delay_msec is elapsed. Data is removed from queue.
 * If the peel reveals a socket closed
 * by peer or other serious problem, socket is closed and sock set to -1.
 *
 * Return value: number of bytes received, 0 when nothing
 */
int
lw6net_tcp_recv (lw6sys_context_t * sys_context, int *sock, char *buf, int len, int delay_msec, int loop)
{
  int ret = 0;
  fd_set read;
  struct timeval tv;
  int select_ret;
  int total_received = 0;
  int received;
  int chunk_size;
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif
#ifdef MSG_NOSIGNAL
  /*
   * Defining MSG_NOSIGNAL is important because in a heavy
   * multithreaded context, this signal is harder to handle
   * than a proper "send failed with -1", and threads do
   * tend to generate this signal. We also use SO_NOSIGPIPE
   * on socket creation but some platforms, including GNU/Linux,
   * do not support it.
   */
  int flags = MSG_NOSIGNAL;
#else
  int flags = 0;
#endif

  if (lw6net_socket_is_valid (sys_context, *sock))
    {
      ret = 1;
      chunk_size = _lw6net_global_context->const_data.chunk_size;
      memset (buf, 0, len);
      total_received = 0;
      while (total_received != len && ret)
	{
	  FD_ZERO (&read);
	  FD_SET (*sock, &read);
	  _lw6net_delay_msec_to_timeval (sys_context, &tv, delay_msec);

	  select_ret = select ((*sock) + 1, &read, NULL, NULL, &tv);

	  if (select_ret < 0)
	    {
#ifdef LW6_MS_WINDOWS
	      winerr = WSAGetLastError ();
	      if (winerr != WSAEINTR)
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("error receiving data on socket %d select() failed, code=%d"), *sock, winerr);
		  ret = 0;
		}
#else
	      if (errno != EINTR)
		{
		  lw6net_last_error (sys_context);
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("error receiving data on socket %d select() failed"), *sock);
		  ret = 0;
		}
#endif
	    }
	  else
	    {
	      if (FD_ISSET (*sock, &read))
		{
		  received = recv (*sock, buf + total_received, lw6sys_imin (len - total_received, chunk_size), flags);
		  if (received > 0 && received <= len - total_received)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%d bytes received on TCP socket %d"), received, *sock);
		      total_received += received;
		    }
		  else
		    {
		      if (received < 0)
			{
#ifdef LW6_MS_WINDOWS
			  winerr = WSAGetLastError ();
			  if (winerr != WSAEINTR && winerr != WSAEWOULDBLOCK)
			    {
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't recv data on socket %d, code=%d"), *sock, winerr);
			      lw6net_socket_close (sys_context, sock);
			      ret = 0;
			    }
#else
			  if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
			    {
			      lw6net_last_error (sys_context);
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't recv data on socket %d"), *sock);
			      lw6net_socket_close (sys_context, sock);
			      ret = 0;
			    }
#endif
			}
		    }
		}
	      break;
	    }

	  if ((!loop) && (total_received != len))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't recv data on socket %d (%d/%d)"), *sock, total_received, len);
	      ret = 0;
	    }
	}

      if (total_received > 0)
	{
	  _lw6net_counters_register_recv (sys_context, &(_lw6net_global_context->counters), total_received);
	  _lw6net_log_tcp_recv (sys_context, &(_lw6net_global_context->log), buf, total_received);
	}
    }

  return ret;
}

/**
 * lw6net_tcp_is_alive
 *
 * @sys_context: global system context
 * @sock: socket to test
 *
 * Tells wether a socket is alive and able to send data. This function
 * will attempt a write to test if it's really usable. If not, will
 * close in on the fly.
 *
 * Return value: 1 if alive, 0 if not.
 */
int
lw6net_tcp_is_alive (lw6sys_context_t * sys_context, int *sock)
{
  int ret = 0;
  fd_set write;
  struct timeval tv;
  int select_ret;
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif

  if (lw6net_socket_is_valid (sys_context, *sock))
    {
      ret = 1;

      FD_ZERO (&write);
      FD_SET (*sock, &write);
      tv.tv_sec = 0;
      tv.tv_usec = 0;

      select_ret = select ((*sock) + 1, NULL, &write, NULL, &tv);

      if (select_ret < 0)
	{
#ifdef LW6_MS_WINDOWS
	  winerr = WSAGetLastError ();
	  if (winerr != WSAEINTR && winerr != WSAENOBUFS)
#else
	  if (errno != EINTR && errno != ENOBUFS)
#endif
	    {
	      // socket is closed...
	      lw6net_last_error (sys_context);
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("closing socket %d because select failed"), *sock);
	      lw6net_socket_close (sys_context, sock);
	      ret = 0;
	    }
	}
      else
	{
	  if (FD_ISSET (*sock, &write))
	    {
	      // OK, looks like we can send data on it
	    }
	  else
	    {
	      lw6net_last_error (sys_context);
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("socket %d is not writeable, it's very likely that TCP system buffer is full"), *sock);
	      /*
	       * Here, we don't mark the "alive" flag (ret) as
	       * not alive, as a filled buffer is not a good enough
	       * reason to close a socket and/or consider the
	       * socket is broken. Only, send operations will return
	       * 0 and not work.
	       */
	    }
	}
    }

  return ret;
}
