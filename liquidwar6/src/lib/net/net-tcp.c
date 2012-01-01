/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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
_lw6net_delay_msec_to_timeval (struct timeval *tv, int delay_msec)
{
  memset (tv, 0, sizeof (struct timeval));

  tv->tv_sec = delay_msec / 1000;
  tv->tv_usec = delay_msec % 1000;
}

/**
 * lw6net_tcp_listen
 *
 * @ip: IP address to bind to
 * @port: IP port to listen on
 *
 * Listens in TCP on a given port. 
 *
 * Return value: >=0 on success, -1 on failure.
 */
int
lw6net_tcp_listen (char *ip, int port)
{
  int backlog;
  int sock = -1;

  backlog = _lw6net_global_context->const_data.listen_backlog;

  sock = _lw6net_socket_bind (ip, port, SOCK_STREAM);

  if (sock >= 0)
    {
      if (listen (sock, backlog) < 0)
	{
	  lw6net_last_error ();
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("listen() failed"));

	  lw6net_socket_close (sock);
	  sock = -1;
	}
    }

  return sock;
}

/**
 * lw6net_tcp_accept
 *
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
lw6net_tcp_accept (char **incoming_ip,
		   int *incoming_port, int listening_sock, int delay_msec)
{
  int new_sock = -1;
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
      _lw6net_delay_msec_to_timeval (&tv, delay_msec);

      res = select (listening_sock + 1, &read, NULL, NULL, &tv);
      if (res >= 1)
	{
	  new_sock =
	    accept (listening_sock, (struct sockaddr *) &name, &namelen);
	  if (new_sock >= 0)
	    {
	      li.l_onoff = 0;
	      li.l_linger = 0;

	      if (setsockopt (new_sock, SOL_SOCKET, SO_KEEPALIVE,
			      (char *) &enable, sizeof (int)))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("setsockopt(SO_KEEPALIVE) failed"));
		  lw6net_last_error ();
		}
	      if (setsockopt (new_sock, SOL_SOCKET, SO_OOBINLINE,
			      (char *) &disable, sizeof (int)))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("setsockopt(SO_OOBINLINE) failed"));
		  lw6net_last_error ();
		}
	      if (setsockopt (new_sock, SOL_SOCKET, SO_LINGER,
			      (char *) &li, sizeof (struct linger)))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("setsockopt(SO_LINGER) failed"));
		  lw6net_last_error ();
		}

	      //fcntl (new_sock, F_SETFL, O_NONBLOCK, 0);

	      if (incoming_ip)
		{
		  (*incoming_ip) = _lw6net_inet_ntoa (name.sin_addr);
		  if (*incoming_ip)
		    {
		      _lw6net_counters_register_socket (&
							(_lw6net_global_context->
							 counters));
		      accepted = 1;
		    }
		}
	      if (incoming_port)
		{
		  (*incoming_port) = (int) ntohs (name.sin_port);
		}

	      if (incoming_ip && incoming_port && *incoming_ip
		  && (*incoming_port) >= 0)
		{
		  lw6sys_log (LW6SYS_LOG_INFO,
			      _x_ ("socket %d accepted from %s:%d"), new_sock,
			      *incoming_ip, *incoming_port);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("accept() failed"));
	      lw6net_last_error ();
	    }

	}
    }

  if (new_sock >= 0 && !accepted)
    {
      if (incoming_ip)
	{
	  if (*incoming_ip)
	    {
	      LW6SYS_FREE (*incoming_ip);
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
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("close() failed"));
	  lw6net_last_error ();
	}
      new_sock = -1;
    }

  return new_sock;
}

/**
 * lw6net_tcp_connect
 *
 * @ip: address to connect to
 * @port: port to connect to
 * @delay_msec: delay before we consider it's too late
 *
 * Tries to connect on a given socket.
 *
 * Return value: socket (>=0) on success, else -1
 */
int
lw6net_tcp_connect (char *ip, int port, int delay_msec)
{
  int sock = -1;
  int connect_ret = 0;
  int connect_async = 0;
  int select_ret = 0;
  int connected = 0;
  struct sockaddr_in name;
  /*
     int enable = 1;
     int disable = 0;
     struct linger li; 
   */
  fd_set write;
  struct timeval tv;
  int64_t origin = 0;
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif

  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock >= 0)
    {
      if (lw6net_socket_set_blocking_mode (sock, 0))
	{
	  name.sin_family = AF_INET;
	  name.sin_addr.s_addr = INADDR_ANY;
	  name.sin_port = 0;
	  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) >= 0)
	    {
	      name.sin_family = AF_INET;
	      if (_lw6net_inet_aton (&name.sin_addr, ip))
		{
		  name.sin_port = htons (port);
		  connect_ret =
		    connect (sock, (struct sockaddr *) &name, sizeof (name));
		  if (connect_ret >= 0)
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
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
			  lw6sys_log (LW6SYS_LOG_INFO,
				      _x_
				      ("can't connect on \"%s:%d\", host unreachable or down"),
				      ip, port);
			  break;
			default:
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("connect on \"%s:%d\" failed, did not return WSAEINPROGRESS or WSAEWOULDBLOCK (code=%d)"),
				      ip, port, winerr);
			  lw6net_last_error ();
			}
#else
		      switch (errno)
			{
			  /*
			   * Fixed 35104 (warning on windows build) by specifically
			   * adding both INPROGRESS and WOULDBLOCK in acceptable values.
			   */
			case EINPROGRESS:
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
			  lw6sys_log (LW6SYS_LOG_INFO,
				      _x_
				      ("can't connect on \"%s:%d\", host unreachable or down"),
				      ip, port);
			  break;
			default:
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("connect on \"%s:%d\" failed, did not return EINPROGRESS or EWOULDBLOCK (code=%d)"),
				      ip, port, errno);
			  lw6net_last_error ();
			}
#endif
		      if (connect_async)
			{
			  FD_ZERO (&write);
			  FD_SET (sock, &write);
			  _lw6net_delay_msec_to_timeval (&tv, delay_msec);

			  origin = lw6sys_get_timestamp ();
			  while (!connected
				 && (lw6sys_get_timestamp () <
				     (origin + delay_msec)))
			    {
			      select_ret =
				select (sock + 1, NULL, &write, NULL, &tv);

			      if (select_ret > 0)
				{
				  if (FD_ISSET (sock, &write))
				    {
				      connected = 1;
				    }
				}
			    }
			}
		    }

		  if (connected)
		    {
		      /*
		         // Added this code copied/paste from accept.
		         // don't know if it's usefull
		         // see also https://savannah.gnu.org/bugs/?34060

		         li.l_onoff = 0;
		         li.l_linger = 0;
		         if (setsockopt (sock, SOL_SOCKET, SO_KEEPALIVE,
		         (char *) &enable, sizeof (int)))
		         {
		         lw6sys_log (LW6SYS_LOG_WARNING,
		         _x_
		         ("setsockopt(SO_KEEPALIVE) failed"));
		         lw6net_last_error ();
		         }
		         if (setsockopt (sock, SOL_SOCKET, SO_OOBINLINE,
		         (char *) &disable, sizeof (int)))
		         {
		         lw6sys_log (LW6SYS_LOG_WARNING,
		         _x_
		         ("setsockopt(SO_OOBINLINE) failed"));
		         lw6net_last_error ();
		         }
		         if (setsockopt (sock, SOL_SOCKET, SO_LINGER,
		         (char *) &li, sizeof (struct linger)))
		         {
		         lw6sys_log (LW6SYS_LOG_WARNING,
		         _x_ ("setsockopt(SO_LINGER) failed"));
		         lw6net_last_error ();
		         }
		       */

		      lw6sys_log (LW6SYS_LOG_INFO,
				  _x_ ("socket %d connected on %s:%d"), sock,
				  ip, port);

		      _lw6net_counters_register_socket (&
							(_lw6net_global_context->
							 counters));
		    }
		  else
		    {
		      /*
		       * Here, no peculiar warning, connect() is definitely
		       * the kind of function which *will* fail very
		       * often, we certainly don't want this to fill our
		       * log...
		       */
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bind() failed"));
	      lw6net_last_error ();
	    }
	}
    }

  if (lw6net_socket_is_valid (sock) && !connected)
    {
      if (close (sock))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("close() failed"));
	  lw6net_last_error ();
	}
      sock = -1;
    }

  return sock;
}

/**
 * lw6net_tcp_send
 *
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @delay_msec: delay after which we give up
 * @loop: accept to do several calls if needed
 *
 * Will send data, possibly looping until all is send, and waiting for
 * a maximum time of delay_msec.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6net_tcp_send (int sock, char *buf, int len, int delay_msec, int loop)
{
  int ret = 0;
  fd_set write;
  struct timeval tv;
  int select_ret;
  int total_sent = 0;
  int sent = 0;
  int chunk_size;
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif

  if (lw6net_socket_is_valid (sock))
    {
      ret = 1;

      chunk_size = _lw6net_global_context->const_data.chunk_size;
      total_sent = 0;
      while (total_sent != len && ret)
	{
	  FD_ZERO (&write);
	  FD_SET (sock, &write);
	  _lw6net_delay_msec_to_timeval (&tv, delay_msec);

	  select_ret = select (sock + 1, NULL, &write, NULL, &tv);

	  switch (select_ret)
	    {
	    case -1:
#ifdef LW6_MS_WINDOWS
	      winerr = WSAGetLastError ();
	      if (winerr != WSAEINTR && winerr != WSAENOBUFS)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("select() failed, code=%d"), winerr);
		  lw6net_last_error ();
		  ret = 0;
		}
#else
	      if (errno != EINTR && errno != ENOBUFS)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("select() failed"));
		  lw6net_last_error ();
		  ret = 0;
		}
#endif
	      break;
	    case 1:
	      if (FD_ISSET (sock, &write))
		{
		  sent = send (sock,
			       buf + total_sent,
			       lw6sys_min (len - total_sent, chunk_size), 0);
		  if (sent > 0 && sent <= len - total_sent)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("%d bytes sent on TCP socket %d"),
				  sent, sock);
		      total_sent += sent;
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _x_
				  ("can't send data on TCP socket %d (%d bytes put)"),
				  sock, sent);
		      ret = 0;
		    }
		}
	      break;
	    default:
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("can't send data on socket %d (select returned %d)"),
			  sock, select_ret);
	      ret = 0;
	    }

	  if ((!loop) && (total_sent != len))
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_ ("can't send data on socket %d (%d/%d)"), sock,
			  total_sent, len);
	      ret = 0;
	    }
	}

      if (total_sent > 0)
	{
	  _lw6net_counters_register_send (&
					  (_lw6net_global_context->counters),
					  total_sent);
	  _lw6net_log_tcp_send (&(_lw6net_global_context->log),
				buf, total_sent);
	}
    }

  return ret;
}

/**
 * lw6net_tcp_peek
 * 
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @delay_msec: maximum time to wait
 *
 * Tells wether data is available. Will actually fill the buffer
 * with the data, but not remove it from the fifo list.
 *
 * Return value: number of bytes available, 0 when nothing
 */
int
lw6net_tcp_peek (int sock, char *buf, int len, int delay_msec)
{
  fd_set read;
  struct timeval tv;
  int select_ret;
  char *buf2;
  int available = 0;

  if (sock >= 0)
    {
      if (buf)
	{
	  buf2 = buf;
	}
      else
	{
	  buf2 = (char *) LW6SYS_MALLOC (len);
	}
      if (buf2)
	{
	  FD_ZERO (&read);
	  FD_SET (sock, &read);
	  _lw6net_delay_msec_to_timeval (&tv, delay_msec);

	  select_ret = select (sock + 1, &read, NULL, NULL, &tv);

	  if (select_ret > 0)
	    {
	      if (FD_ISSET (sock, &read))
		{
		  available = recv (sock, buf2, len, MSG_PEEK);
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_ ("%d bytes available on TCP socket %d"),
			      available, sock);
		}
	    }

	  if (!buf)
	    {
	      LW6SYS_FREE (buf2);
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
 * @sock: socket to use
 * @buf: data buffer
 * @len: data buffer length
 * @delay_msec: maximum time to wait
 * @loop: wether to loop or not
 *
 * If data is available, put it in buffer. If needed, will
 * loop until @delay_msec is elapsed. Data is removed from queue.
 *
 * Return value: number of bytes received, 0 when nothing
 */
int
lw6net_tcp_recv (int sock, char *buf, int len, int delay_msec, int loop)
{
  int ret = 0;
  fd_set read;
  struct timeval tv;
  int select_ret;
  int total_received = 0;
  int received;
  int chunk_size;

  if (lw6net_socket_is_valid (sock))
    {
      ret = 1;
      chunk_size = _lw6net_global_context->const_data.chunk_size;
      memset (buf, 0, len);
      total_received = 0;
      while (total_received != len && ret)
	{
	  FD_ZERO (&read);
	  FD_SET (sock, &read);
	  _lw6net_delay_msec_to_timeval (&tv, delay_msec);

	  select_ret = select (sock + 1, &read, NULL, NULL, &tv);

	  switch (select_ret)
	    {
	    case -1:
	      if (errno != EINTR)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("error receiving data on socket %d (select error %d)"),
			      sock, errno);
		  lw6net_last_error ();
		  ret = 0;
		}
	      break;
	    case 1:
	      if (FD_ISSET (sock, &read))
		{
		  received = recv (sock,
				   buf + total_received,
				   lw6sys_min (len - total_received,
					       chunk_size), 0);
		  if (received > 0 && received <= len - total_received)
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("%d bytes received on TCP socket %d"),
				  received, sock);
		      total_received += received;
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_
				  ("can't recv data on socket %d (got %d bytes)"),
				  sock, received);
		      lw6net_last_error ();
		      ret = 0;
		    }
		}
	      break;
	    default:
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_
			  ("can't recv data on socket %d (select returned %d)"),
			  sock, select_ret);
	      lw6net_last_error ();
	      ret = 0;
	    }

	  if ((!loop) && (total_received != len))
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("can't recv data on socket %d (%d/%d)"), sock,
			  total_received, len);
	      lw6net_last_error ();
	      ret = 0;
	    }
	}

      if (total_received > 0)
	{
	  _lw6net_counters_register_recv (&
					  (_lw6net_global_context->counters),
					  total_received);
	  _lw6net_log_tcp_recv (&(_lw6net_global_context->log),
				buf, total_received);
	}
    }

  return ret;
}

/**
 * lw6net_tcp_is_alive
 *
 * @sock: socket to test
 *
 * Tells wether a socket is alive and able to send data. This function
 * will attempt a write to test if it's really usable.
 *
 * Return value: 1 if alive, 0 if not.
 */
int
lw6net_tcp_is_alive (int sock)
{
  int ret = 0;
  fd_set write;
  struct timeval tv;
  int select_ret;
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif

  if (sock >= 0)
    {
      ret = 1;

      FD_ZERO (&write);
      FD_SET (sock, &write);
      tv.tv_sec = 0;
      tv.tv_usec = 0;

      select_ret = select (sock + 1, NULL, &write, NULL, &tv);

      switch (select_ret)
	{
	case -1:
#ifdef LW6_MS_WINDOWS
	  winerr = WSAGetLastError ();
	  if (winerr != WSAEINTR && winerr != WSAENOBUFS)
#else
	  if (errno != EINTR && errno != ENOBUFS)
#endif
	    {
	      // socket is closed...
	      ret = 0;
	    }
	  break;
	case 1:
	  if (!FD_ISSET (sock, &write))
	    {
	      // socket is closed
	      ret = 0;
	    }
	  break;
	default:
	  // socket is closed
	  ret = 0;
	}
    }

  return ret;
}
