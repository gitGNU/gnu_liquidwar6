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

#include "../srv.h"
#include "mod-httpd-internal.h"

int
_mod_httpd_analyse_tcp (_httpd_context_t * httpd_context,
			lw6srv_tcp_accepter_t * tcp_accepter)
{
  int ret = 0;
  char *pos = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("trying to recognize httpd protocol"));

  if (!lw6net_tcp_is_alive (tcp_accepter->sock))
    {
      ret |= LW6SRV_ANALYSE_DEAD;
      lw6net_socket_close (tcp_accepter->sock);
      tcp_accepter->sock = -1;
    }

  if (strlen (tcp_accepter->first_line) >=
      _MOD_HTTPD_PROTOCOL_UNDERSTANDABLE_SIZE
      || strchr (tcp_accepter->first_line, '\n'))
    {
      if (!strncmp
	  (tcp_accepter->first_line, _MOD_HTTPD_PROTOCOL_GET_STRING,
	   _MOD_HTTPD_PROTOCOL_GET_SIZE)
	  || !strncmp (tcp_accepter->first_line,
		       _MOD_HTTPD_PROTOCOL_POST_STRING,
		       _MOD_HTTPD_PROTOCOL_POST_SIZE)
	  || !strncmp (tcp_accepter->first_line,
		       _MOD_HTTPD_PROTOCOL_HEAD_STRING,
		       _MOD_HTTPD_PROTOCOL_HEAD_SIZE))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("recognized httpd protocol \"%s\""),
		      tcp_accepter->first_line);
	  ret |= LW6SRV_ANALYSE_UNDERSTANDABLE;

	  pos = tcp_accepter->first_line;
	  while ((*pos) && !lw6sys_chr_is_space (*pos))
	    {
	      pos++;
	    }
	  while (lw6sys_chr_is_space (*pos))
	    {
	      pos++;
	    }
	  if (!strncmp
	      (pos, _MOD_HTTPD_PROTOCOL_LW6_STRING,
	       _MOD_HTTPD_PROTOCOL_LW6_SIZE))
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("httpd LW6 message \"%s\""),
			  pos);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("out of band httpd \"%s\""),
			  pos);
	      ret |= LW6SRV_ANALYSE_OOB;
	    }
	}
    }

  /*
   * Here, maybe we didn't recognize HTTPD protocol, but since we're
   * in timeout, we decide so send an error 500 answer anyway.
   */
  if (lw6net_tcp_is_alive (tcp_accepter->sock)
      && !_mod_httpd_timeout_ok (httpd_context,
				 tcp_accepter->creation_timestamp))
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _
		  ("timeout on receive (first_line=\"%s\") so sending request to http handler, which will probably return error 500"),
		  tcp_accepter->first_line);
      ret |= (LW6SRV_ANALYSE_UNDERSTANDABLE | LW6SRV_ANALYSE_OOB);
    }

  return ret;
}

int
_mod_httpd_analyse_udp (_httpd_context_t * httpd_context,
			lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  /*
   * UDP is never handled by mod_httpd, return always false
   */

  return ret;
}

lw6srv_connection_t *
_mod_httpd_accept_tcp (_httpd_context_t * httpd_context,
		       lw6srv_tcp_accepter_t * tcp_accepter, char *password)
{
  lw6srv_connection_t *ret = NULL;

  // todo

  return ret;
}

lw6srv_connection_t *
_mod_httpd_new_udp (_httpd_context_t * httpd_context,
		    lw6srv_udp_buffer_t * udp_buffer, char *password)
{
  lw6srv_connection_t *ret = NULL;

  /*
   * UDP is never handled by mod_httpd, return always NULL
   */

  return ret;
}

int
_mod_httpd_is_associated_with_udp (_httpd_context_t * httpd_context,
				   lw6srv_connection_t * connection,
				   lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  /*
   * UDP is never handled by mod_httpd, return always false
   */

  return ret;
}

int
_mod_httpd_update_with_udp (_httpd_context_t * httpd_context,
			    lw6srv_connection_t * connection,
			    lw6srv_udp_buffer_t * udp_buffer)
{
  int ret = 0;

  /*
   * UDP is never handled by mod_httpd, return always false
   */

  return ret;
}
