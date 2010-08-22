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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "net.h"
#include "net-internal.h"

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
	  lw6sys_log (LW6SYS_LOG_WARNING, _("listen() failed"));

	  lw6net_socket_close (sock);
	  sock = -1;
	}
    }

  return sock;
}

int
lw6net_tcp_accept (char **incoming_ip,
		   int *incoming_port, int listening_sock, float delay)
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
      tv.tv_sec = (int) delay;
      tv.tv_usec = (int) (delay / 1000000.0f);

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
			      _("setsockopt(SO_KEEPALIVE) failed"));
		  lw6net_last_error ();
		}
	      if (setsockopt (new_sock, SOL_SOCKET, SO_OOBINLINE,
			      (char *) &disable, sizeof (int)))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("setsockopt(SO_OOBINLINE) failed"));
		  lw6net_last_error ();
		}
	      if (setsockopt (new_sock, SOL_SOCKET, SO_LINGER,
			      (char *) &li, sizeof (struct linger)))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("setsockopt(SO_LINGER) failed"));
		  lw6net_last_error ();
		}

	      //fcntl (new_sock, F_SETFL, O_NONBLOCK, 0);

	      if (incoming_ip)
		{
		  (*incoming_ip) = _lw6net_inet_ntoa (name.sin_addr);
		  if (*incoming_ip)
		    {
		      _lw6net_global_context->socket_counters.open_counter++;
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
			      _("socket %d accepted from %s:%d"), new_sock,
			      *incoming_ip, *incoming_port);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING, _("accept() failed"));
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
	  lw6sys_log (LW6SYS_LOG_WARNING, _("close() failed"));
	  lw6net_last_error ();
	}
      new_sock = -1;
    }

  return new_sock;
}

int
lw6net_tcp_connect (char *ip, int port)
{
  int sock = -1;
  int connected = 0;
  struct sockaddr_in name;
  int enable = 1;
  int disable = 0;
  struct linger li;

  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock >= 0)
    {
      name.sin_family = AF_INET;
      name.sin_addr.s_addr = INADDR_ANY;
      name.sin_port = 0;
      if (bind (sock, (struct sockaddr *) &name, sizeof (name)) >= 0)
	{
	  name.sin_family = AF_INET;
#ifdef LW6_MS_WINDOWS
	  if ((name.sin_addr.s_addr = inet_addr (ip)) != 0)
#else
	  if (inet_aton (ip, &name.sin_addr) != 0)
#endif
	    {
	      name.sin_port = htons (port);
	      if (connect (sock, (struct sockaddr *) &name, sizeof (name)) >=
		  0)
		{
		  /*
		   * Added this code copied/paste from accept.
		   * don't know if it's usefull
		   */
		  li.l_onoff = 0;
		  li.l_linger = 0;
		  if (setsockopt (sock, SOL_SOCKET, SO_KEEPALIVE,
				  (char *) &enable, sizeof (int)))
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _("setsockopt(SO_KEEPALIVE) failed"));
		      lw6net_last_error ();
		    }
		  if (setsockopt (sock, SOL_SOCKET, SO_OOBINLINE,
				  (char *) &disable, sizeof (int)))
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _("setsockopt(SO_OOBINLINE) failed"));
		      lw6net_last_error ();
		    }
		  if (setsockopt (sock, SOL_SOCKET, SO_LINGER,
				  (char *) &li, sizeof (struct linger)))
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _("setsockopt(SO_LINGER) failed"));
		      lw6net_last_error ();
		    }

		  //fcntl (sock, F_SETFL, O_NONBLOCK, 0);

		  lw6sys_log (LW6SYS_LOG_INFO,
			      _("socket %d connected on %s:%d"), sock, ip,
			      port);

		  _lw6net_global_context->socket_counters.open_counter++;
		  connected = 1;
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
	  else
	    {
#ifdef LW6_MS_WINDOWS
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _("inet_addr() failed, ip=\"%s\""), ip);
	      lw6net_last_error ();
#else
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _("inet_aton() failed, ip=\"%s\""), ip);
	      lw6net_last_error ();
#endif
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("bind() failed"));
	  lw6net_last_error ();
	}
    }

  if (sock >= 0 && !connected)
    {
      if (close (sock))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("close() failed"));
	  lw6net_last_error ();
	}
      sock = -1;
    }

  return sock;
}

static void
async_connect_callback_func (void *callback_data)
{
  char *state = NULL;
  _lw6net_socket_async_connect_data_t *async_connect_data;

  async_connect_data = (_lw6net_socket_async_connect_data_t *) callback_data;

  async_connect_data->sock =
    lw6net_tcp_connect (async_connect_data->ip, async_connect_data->port);

  if (async_connect_data->sock >= 0)
    {
      state = _("connected");
    }
  else
    {
      state = _("connexion failed");
    }

  lw6sys_log (LW6SYS_LOG_INFO,
	      _("async connect on %s:%d has terminated, %s"),
	      async_connect_data->ip, async_connect_data->port, state);
}

static void
async_connect_callback_join (void *callback_data)
{
  _lw6net_socket_async_connect_data_t *async_connect_data;

  async_connect_data = (_lw6net_socket_async_connect_data_t *) callback_data;

  /*
   * It's important to have this here, nowhere else but at the very end
   * of the connect thread can we close the socket, it ever "got connected"
   * after we waited "long enough".
   */
  if (async_connect_data->close && async_connect_data->sock >= 0)
    {
      lw6net_socket_close (async_connect_data->sock);
    }

  LW6SYS_FREE (async_connect_data);
}

void *
lw6net_tcp_async_connect_init (char *ip, int port)
{
  void *ret = NULL;
  _lw6net_socket_async_connect_data_t *async_connect_data = NULL;

  async_connect_data =
    LW6SYS_CALLOC (sizeof (_lw6net_socket_async_connect_data_t));
  if (async_connect_data)
    {
      strncpy (async_connect_data->ip, ip, _LW6NET_IP_SIZE - 1);
      async_connect_data->ip[_LW6NET_IP_SIZE - 1] = '\0';
      async_connect_data->port = port;
      async_connect_data->sock = -1;
      async_connect_data->close = 1;
      // all other values set to 0 (calloc)
      ret =
	lw6sys_thread_create (&async_connect_callback_func,
			      &async_connect_callback_join,
			      (void *) async_connect_data);
      if (ret)
	{
	  // ok
	  _lw6net_thread_register (ret);
	}
      else
	{
	  LW6SYS_FREE (async_connect_data);
	}
    }

  return ret;
}

int
lw6net_tcp_async_connect_get (int *sock, void *handler)
{
  int ret = 0;
  _lw6net_socket_async_connect_data_t *async_connect_data;

  if (lw6sys_thread_is_callback_done (handler))
    {
      async_connect_data =
	(_lw6net_socket_async_connect_data_t *)
	lw6sys_thread_get_data (handler);
      (*sock) = async_connect_data->sock;
      async_connect_data->close = 0;
      ret = 1;
    }

  return ret;
}

void
lw6net_tcp_async_connect_exit (void *handler)
{
  /*
   * Unregister should automatically call
   * lw6sys_thread_join(handler); but the advantage is that
   * this way, at the end of the program, we can join all threads
   * together and garbage collect things "properly".
   */
  _lw6net_thread_unregister (handler);
}

int
lw6net_tcp_send (int sock, char *buf, int len, float delay, int loop)
{
  int ret = 0;
  fd_set write;
  struct timeval tv;
  int select_ret;
  int total_sent;
  int sent = 0;
  int chunk_size;
#ifdef LW6_MS_WINDOWS
  int winerr = 0;
#endif

  if (sock >= 0)
    {
      ret = 1;

      chunk_size = _lw6net_global_context->const_data.chunk_size;
      total_sent = 0;
      while (total_sent != len && ret)
	{
	  FD_ZERO (&write);
	  FD_SET (sock, &write);
	  tv.tv_sec = (int) delay;
	  tv.tv_usec = (int) ((delay - tv.tv_sec) * 1000000.0f);

	  select_ret = select (sock + 1, NULL, &write, NULL, &tv);

	  switch (select_ret)
	    {
	    case -1:
#ifdef LW6_MS_WINDOWS
	      winerr = WSAGetLastError ();
	      if (winerr != WSAEINTR && winerr != WSAENOBUFS)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("select() failed, code=%d"), winerr);
		  lw6net_last_error ();
		  ret = 0;
		}
#else
	      if (errno != EINTR && errno != ENOBUFS)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING, _("select() failed"));
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
				  _("%d bytes sent on TCP socket %d"), sent,
				  sock);
		      total_sent += sent;
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _
				  ("can't send data on TCP socket %d (%d bytes put)"),
				  sock, sent);
		      ret = 0;
		    }
		}
	      break;
	    default:
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _
			  ("can't send data on socket %d (select returned %d)"),
			  sock, select_ret);
	      ret = 0;
	    }

	  if ((!loop) && (total_sent != len))
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _("can't send data on socket %d (%d/%d)"), sock,
			  total_sent, len);
	      ret = 0;
	    }
	}
    }

  return ret;
}

int
lw6net_tcp_peek (int sock, char *buf, int len, float delay)
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
	  tv.tv_sec = (int) delay;
	  tv.tv_usec = (int) ((delay - tv.tv_sec) * 1000000.0f);

	  select_ret = select (sock + 1, &read, NULL, NULL, &tv);

	  if (select_ret > 0)
	    {
	      if (FD_ISSET (sock, &read))
		{
		  available = recv (sock, buf2, len, MSG_PEEK);
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _("%d bytes available on TCP socket %d"),
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

int
lw6net_tcp_recv (int sock, char *buf, int len, float delay, int loop)
{
  int ret = 0;
  fd_set read;
  struct timeval tv;
  int select_ret;
  int total_received;
  int received;
  int chunk_size;

  if (sock >= 0)
    {
      ret = 1;
      chunk_size = _lw6net_global_context->const_data.chunk_size;
      memset (buf, 0, len);
      total_received = 0;
      while (total_received != len && ret)
	{
	  FD_ZERO (&read);
	  FD_SET (sock, &read);
	  tv.tv_sec = (int) delay;
	  tv.tv_usec = (int) ((delay - tv.tv_sec) * 1000000.0f);

	  select_ret = select (sock + 1, &read, NULL, NULL, &tv);

	  switch (select_ret)
	    {
	    case -1:
	      if (errno != EINTR)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _
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
				  _("%d bytes received on TCP socket %d"),
				  received, sock);
		      total_received += received;
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _
				  ("can't recv data on socket %d (got %d bytes)"),
				  sock, received);
		      lw6net_last_error ();
		      ret = 0;
		    }
		}
	      break;
	    default:
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _
			  ("can't recv data on socket %d (select returned %d)"),
			  sock, select_ret);
	      lw6net_last_error ();
	      ret = 0;
	    }

	  if ((!loop) && (total_received != len))
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _("can't recv data on socket %d (%d/%d)"), sock,
			  total_received, len);
	      lw6net_last_error ();
	      ret = 0;
	    }
	}
    }

  return ret;
}

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
