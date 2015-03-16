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

#include <string.h>

#include "net.h"
#include "net-internal.h"

#define CHAR_CR '\x0d'
#define CHAR_LF '\x0a'
#define CHAR_0 '\0'
#define TRAIL_SIZE 2

#ifdef LW6_MS_WINDOWS
/*
 * Using CR/LF instead of LF only under MS-Windows is clearly
 * *not* mandatory but... it has a "je-ne-sais-quoi" to keep
 * using despite everyone else does use LF only with success,
 * so it's kept here for nothing but style ;)
 */
static char trail[TRAIL_SIZE + 1] = { CHAR_CR, CHAR_LF, CHAR_0 };
#else
static char trail[TRAIL_SIZE + 1] = { CHAR_LF, CHAR_0, CHAR_0 };
#endif

/**
 * lw6net_recv_line_tcp:
 *
 * @sys_context: global system context
 * @sock: the socket descriptor
 *
 * Receives a line terminated by LF ("\n", chr(10)) or
 * CR/LF ("\r\n", chr(10)chr(13)) on a TCP socket, that is,
 * stream oriented. If there's no complete line
 * available, function returns immediately with NULL. Same
 * if socket is closed, broken, whatever. Only if there's
 * something consistent will the function return non-NULL.
 * Socket descriptor is closed on the fly on connection problem.
 *
 * Return value: a dynamically allocated string with the
 *   content received. The tailing (CR)/LF is stripped.
 */
char *
lw6net_recv_line_tcp (lw6sys_context_t * sys_context, int *sock)
{
  char *ret = NULL;
  int line_size = 0;
  int line_delay;
  int wanted_size;
  int available_size;
  int trail_size;
  char *pos_lf;
  char line_buf[LW6NET_MAX_LINE_SIZE + TRAIL_SIZE + 1];

  if (lw6net_socket_is_valid (sys_context, *sock))
    {
      line_size = _lw6net_global_context->const_data.line_size;
      line_delay = _lw6net_global_context->const_data.line_delay_msec;
      memset (line_buf, 0, line_size + TRAIL_SIZE + 1);
      available_size = lw6net_tcp_peek (sys_context, sock, line_buf, line_size + TRAIL_SIZE, 0.0f);
      if (available_size > 0)
	{
	  pos_lf = strchr (line_buf, CHAR_LF);
	  if (pos_lf)
	    {
	      trail_size = (pos_lf > line_buf && pos_lf[-1] == CHAR_CR) ? TRAIL_SIZE : TRAIL_SIZE - 1;
	      wanted_size = (pos_lf + 1 - line_buf);
	      pos_lf[1 - trail_size] = CHAR_0;
	      ret = lw6sys_str_copy (sys_context, line_buf);
	      if (ret)
		{
		  lw6sys_str_cleanup (sys_context, ret);
		}

	      // remove data from queue
	      lw6net_tcp_recv (sys_context, sock, line_buf, wanted_size, line_delay, 0);
	    }
	}
    }

  return ret;
}

/**
 * lw6net_send_line_tcp:
 *
 * @sys_context: global system context
 * @sock: the socket descriptor
 * @line: the line to be sent, without the "\n" at the end
 *
 * Sends a line terminated by LF ("\n", chr(10)) on a TCP
 * socket, that is, stream oriented. The "\n" is automatically
 * added, do not bother sending it.
 * Socket descriptor is closed on the fly on connection problem.
 *
 * Return value: non-zero if success
 */
int
lw6net_send_line_tcp (lw6sys_context_t * sys_context, int *sock, const char *line)
{
  int ret = 0;
  int line_size = 0;
  int line_delay;
  int wanted_size = 0;
  char *trailed_line = NULL;

  if (lw6net_socket_is_valid (sys_context, *sock) && line)
    {
      /*
       * If sock is not reported as alive, we don't even waste
       * the time to try and send data, this could really slow
       * down things on polling loops.
       */
      if (lw6net_tcp_is_alive (sys_context, sock))
	{
	  trailed_line = lw6sys_str_concat (sys_context, line, trail);
	  if (trailed_line)
	    {
	      line_size = _lw6net_global_context->const_data.line_size;
	      line_delay = _lw6net_global_context->const_data.line_delay_msec;
	      wanted_size = strlen (trailed_line);
	      if (wanted_size > line_size)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("stripping line \"%s\" of size %d, limit is %d"), line, wanted_size, line_size);
		  wanted_size = line_size;
		}

	      ret = lw6net_tcp_send (sys_context, sock, trailed_line, wanted_size, line_delay, 0);
	      LW6SYS_FREE (sys_context, trailed_line);
	    }
	}
    }

  return ret;
}

/**
 * lw6net_recv_line_udp:
 *
 * @sys_context: global system context
 * @sock: the socket descriptor
 * @incoming_ip: the IP address of the sender (returned)
 * @incoming_port: the IP port of the sender (returned)
 *
 * Receives a line terminated by LF ("\n", chr(10)) or
 * CR/LF ("\r\n", chr(10)chr(13)) on a UDP socket, that is,
 * datagram oriented. If there's no complete line
 * available, function returns immediately with NULL. Same
 * if socket is closed, broken, whatever. Only if there's
 * something consistent will the function return non-NULL.
 * By-value parameters allow the caller to know where the
 * data come from.
 *
 * Return value: a dynamically allocated string with the
 *   content received. The tailing (CR)/LF is stripped.
 */
char *
lw6net_recv_line_udp (lw6sys_context_t * sys_context, int sock, char **incoming_ip, int *incoming_port)
{
  char *ret = NULL;
  int line_size = 0;
  int available_size;
  int trail_size;
  char *pos_lf;
  /*
   * This is the very function that needs line_buf to be stack based
   * and not malloced from the heap. The idea is that this function
   * can be called by many threads in polling loops (think of broadcast)
   * and allocating memory for this causes bottlenecks in bazooka functions.
   */
  char line_buf[LW6NET_MAX_LINE_SIZE + TRAIL_SIZE + 1];

  if (lw6net_socket_is_valid (sys_context, sock))
    {
      line_size = lw6sys_imin (_lw6net_global_context->const_data.line_size, LW6NET_PPPOE_MTU - TRAIL_SIZE);
      memset (line_buf, 0, line_size + TRAIL_SIZE + 1);
      available_size = lw6net_udp_peek (sys_context, sock, line_buf, line_size + TRAIL_SIZE, incoming_ip, incoming_port);
      if (incoming_ip && (*incoming_ip))
	{
	  LW6SYS_FREE (sys_context, *incoming_ip);
	}
      if (available_size > 0)
	{
	  pos_lf = strchr (line_buf, CHAR_LF);
	  if (pos_lf)
	    {
	      trail_size = (pos_lf > line_buf && pos_lf[-1] == CHAR_CR) ? TRAIL_SIZE : TRAIL_SIZE - 1;
	      pos_lf[1 - trail_size] = CHAR_0;
	      ret = lw6sys_str_copy (sys_context, line_buf);
	      if (ret)
		{
		  lw6sys_str_cleanup (sys_context, ret);
		}
	    }
	  // remove data from queue
	  lw6net_udp_recv (sys_context, sock, line_buf, available_size, incoming_ip, incoming_port);
	}
    }

  return ret;
}

/**
 * lw6net_recv_lines_udp:
 *
 * @sys_context: global system context
 * @sock: the socket descriptor
 * @incoming_ip: the IP address of the sender (returned)
 * @incoming_port: the IP port of the sender (returned)
 *
 * Receives several lines terminated by LF ("\n", chr(10)) or
 * CR/LF ("\r\n", chr(10)chr(13)) on a UDP socket, that is,
 * datagram oriented. If there's no complete line
 * available, function returns immediately with NULL. Same
 * if socket is closed, broken, whatever. Only if there's
 * something consistent will the function return non-NULL.
 * By-value parameters allow the caller to know where the
 * data come from. This variant of @lw6net_recv_line_tcp will
 * return a list of lines, this is mandatory since in UDP
 * we can't call recv several times.
 *
 * Return value: a list of dynamically allocated strings.
 *   The tailing (CR)/LF is stripped from strings.
 */
lw6sys_list_t *
lw6net_recv_lines_udp (lw6sys_context_t * sys_context, int sock, char **incoming_ip, int *incoming_port)
{
  lw6sys_list_t *ret = NULL;
  char *line = NULL;
  int line_size = 0;
  int available_size;
  int trail_size;
  char *pos_lf;
  char line_buf[LW6NET_MAX_LINE_SIZE + TRAIL_SIZE + 1];
  char *seek = NULL;
  int no_lf_at_very_end = 0;

  if (lw6net_socket_is_valid (sys_context, sock))
    {
      ret = lw6sys_list_new (sys_context, lw6sys_free_callback);
      if (ret)
	{
	  line_size = lw6sys_imin (_lw6net_global_context->const_data.line_size, LW6NET_PPPOE_MTU - TRAIL_SIZE);
	  memset (line_buf, 0, line_size + TRAIL_SIZE + 1);
	  available_size = lw6net_udp_peek (sys_context, sock, line_buf, line_size + TRAIL_SIZE, incoming_ip, incoming_port);
	  if (incoming_ip && (*incoming_ip))
	    {
	      LW6SYS_FREE (sys_context, *incoming_ip);
	    }
	  if (available_size > 0)
	    {
	      seek = line_buf;
	      while (*seek)
		{
		  pos_lf = strchr (seek, CHAR_LF);
		  if (pos_lf)
		    {
		      trail_size = (pos_lf > seek && pos_lf[-1] == CHAR_CR) ? TRAIL_SIZE : TRAIL_SIZE - 1;
		      pos_lf[1 - trail_size] = CHAR_0;
		      line = lw6sys_str_copy (sys_context, seek);
		      if (line)
			{
			  lw6sys_str_cleanup (sys_context, line);
			  if (ret)
			    {
			      lw6sys_list_push_back (sys_context, &ret, line);
			    }
			}
		      seek = pos_lf + 1;
		    }
		  else
		    {
		      no_lf_at_very_end = 1;
		      seek += strlen (seek);
		    }
		}
	      // remove data from queue
	      lw6net_udp_recv (sys_context, sock, line_buf, available_size, incoming_ip, incoming_port);
	    }
	}
    }

  if (ret && lw6sys_list_is_empty (sys_context, ret))
    {
      lw6sys_list_free (sys_context, ret);
      ret = NULL;
    }

  if (no_lf_at_very_end)
    {
      if (ret)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no lf at the very end of the sequence of lines, message was probably truncated"));
	  lw6sys_list_free (sys_context, ret);
	  ret = NULL;
	}
    }

  if (!ret)
    {
      if (incoming_ip && (*incoming_ip))
	{
	  LW6SYS_FREE (sys_context, *incoming_ip);
	  (*incoming_ip) = NULL;
	}
    }

  return ret;
}

/**
 * lw6net_send_line_udp:
 *
 * @sys_context: global system context
 * @sock: the socket descriptor
 * @line: the line to be sent, without the "\n" at the end
 * @ip: the IP address of the target
 * @port: the IP port of the target
 *
 * Sends a line terminated by LF ("\n", chr(10)) on a UDP
 * socket, that is, datagram oriented. The "\n" is automatically
 * added, do not bother sending it.
 *
 * Return value: the number of bytes sent, 0 if failure
 */
int
lw6net_send_line_udp (lw6sys_context_t * sys_context, int sock, const char *line, const char *ip, int port)
{
  int ret = 0;
  int line_size = 0;
  int wanted_size = 0;
  char *copied_line;
  char *trailed_line;

  if (lw6net_socket_is_valid (sys_context, sock) && line)
    {
      line_size = lw6sys_imin (_lw6net_global_context->const_data.line_size, LW6NET_PPPOE_MTU - TRAIL_SIZE);
      copied_line = lw6sys_str_copy (sys_context, line);
      if (copied_line)
	{
	  wanted_size = strlen (copied_line);
	  if (wanted_size > line_size)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("stripping line \"%s\" of size %d, limit is %d"), copied_line, wanted_size, line_size);
	      lw6sys_str_truncate (sys_context, copied_line, line_size);
	    }
	  trailed_line = lw6sys_str_concat (sys_context, copied_line, trail);
	  if (trailed_line)
	    {
	      ret = lw6net_udp_send (sys_context, sock, trailed_line, strlen (trailed_line), ip, port);
	      LW6SYS_FREE (sys_context, trailed_line);
	    }
	  LW6SYS_FREE (sys_context, copied_line);
	}
    }

  return ret;
}
