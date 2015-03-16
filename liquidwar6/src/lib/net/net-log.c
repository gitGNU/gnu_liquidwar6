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

#include "net.h"
#include "net-internal.h"

#define _NET_DIR "net"
#define _TCP_RECV_FILE "tcp-recv.log"
#define _TCP_SEND_FILE "tcp-send.log"
#define _UDP_RECV_FILE "udp-recv.log"
#define _UDP_SEND_FILE "udp-send.log"

int
_lw6net_log_init (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6net_log_t * log, int net_log)
{
  int ret = 0;
  char *user_dir;
  char *net_dir;

  user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
  if (user_dir)
    {
      net_dir = lw6sys_path_concat (sys_context, user_dir, _NET_DIR);
      if (net_dir)
	{
	  /*
	   * We create dir only if log is activated,
	   * if log isn't will still check if exists
	   * afterwards and clean files if needed, just
	   * to avoid old traces to fill your HD.
	   */
	  if (net_log && !lw6sys_dir_exists (sys_context, net_dir))
	    {
	      lw6sys_create_dir (sys_context, net_dir);
	    }
	  if (lw6sys_dir_exists (sys_context, net_dir))
	    {
	      log->tcp_recv_filename = lw6sys_path_concat (sys_context, net_dir, _TCP_RECV_FILE);
	      if (log->tcp_recv_filename)
		{
		  if (lw6sys_file_exists (sys_context, log->tcp_recv_filename))
		    {
		      lw6sys_clear_file (sys_context, log->tcp_recv_filename);
		    }
		  if (!net_log)
		    {
		      LW6SYS_FREE (sys_context, log->tcp_recv_filename);
		      log->tcp_recv_filename = NULL;
		    }
		}
	      log->tcp_send_filename = lw6sys_path_concat (sys_context, net_dir, _TCP_SEND_FILE);
	      if (log->tcp_send_filename)
		{
		  if (lw6sys_file_exists (sys_context, log->tcp_send_filename))
		    {
		      lw6sys_clear_file (sys_context, log->tcp_send_filename);
		    }
		  if (!net_log)
		    {
		      LW6SYS_FREE (sys_context, log->tcp_send_filename);
		      log->tcp_send_filename = NULL;
		    }
		}
	      log->udp_recv_filename = lw6sys_path_concat (sys_context, net_dir, _UDP_RECV_FILE);
	      if (log->udp_recv_filename)
		{
		  if (lw6sys_file_exists (sys_context, log->udp_recv_filename))
		    {
		      lw6sys_clear_file (sys_context, log->udp_recv_filename);
		    }
		  if (!net_log)
		    {
		      LW6SYS_FREE (sys_context, log->udp_recv_filename);
		      log->udp_recv_filename = NULL;
		    }
		}
	      log->udp_send_filename = lw6sys_path_concat (sys_context, net_dir, _UDP_SEND_FILE);
	      if (log->udp_send_filename)
		{
		  if (lw6sys_file_exists (sys_context, log->udp_send_filename))
		    {
		      lw6sys_clear_file (sys_context, log->udp_send_filename);
		    }
		  if (!net_log)
		    {
		      LW6SYS_FREE (sys_context, log->udp_send_filename);
		      log->udp_send_filename = NULL;
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, net_dir);
	}
      LW6SYS_FREE (sys_context, user_dir);
    }
  if (net_log)
    {
      ret = (log->tcp_recv_filename && log->tcp_send_filename && log->udp_recv_filename && log->udp_send_filename);
    }
  else
    {
      // ok, no need to initialize
      ret = 1;
    }

  return ret;
}

void
_lw6net_log_quit (lw6sys_context_t * sys_context, _lw6net_log_t * log)
{
  if (log->tcp_recv_filename)
    {
      LW6SYS_FREE (sys_context, log->tcp_recv_filename);
    }
  if (log->tcp_send_filename)
    {
      LW6SYS_FREE (sys_context, log->tcp_send_filename);
    }
  if (log->udp_recv_filename)
    {
      LW6SYS_FREE (sys_context, log->udp_recv_filename);
    }
  if (log->udp_send_filename)
    {
      LW6SYS_FREE (sys_context, log->udp_send_filename);
    }
  memset (log, 0, sizeof (_lw6net_log_t));
}

static int
_log (lw6sys_context_t * sys_context, const char *filename, const char *buf, int len)
{
  int ret = 0;
  char *buf_base64 = NULL;

  if (filename)
    {
      FILE *f = NULL;

      f = fopen (filename, "ab");
      if (f)
	{
	  if (lw6sys_str_is_bin (sys_context, buf, len))
	    {
	      buf_base64 = lw6glb_base64_encode_bin (sys_context, buf, len);
	    }

	  if (buf_base64)
	    {
	      fprintf (f, "%s\n", buf_base64);
	      LW6SYS_FREE (sys_context, buf_base64);
	    }
	  else
	    {
	      if (fwrite (buf, sizeof (char), len, f) == len)
		{
		  ret = 1;
		}
	    }
	  fclose (f);
	}
    }
  else
    {
      ret = 1;
    }

  return ret;
}

int
_lw6net_log_tcp_recv (lw6sys_context_t * sys_context, _lw6net_log_t * log, const char *buf, int len)
{
  return _log (sys_context, log->tcp_recv_filename, buf, len);
}

int
_lw6net_log_tcp_send (lw6sys_context_t * sys_context, _lw6net_log_t * log, const char *buf, int len)
{
  return _log (sys_context, log->tcp_send_filename, buf, len);
}

int
_lw6net_log_udp_recv (lw6sys_context_t * sys_context, _lw6net_log_t * log, const char *buf, int len)
{
  return _log (sys_context, log->udp_recv_filename, buf, len);
}

int
_lw6net_log_udp_send (lw6sys_context_t * sys_context, _lw6net_log_t * log, const char *buf, int len)
{
  return _log (sys_context, log->udp_send_filename, buf, len);
}
