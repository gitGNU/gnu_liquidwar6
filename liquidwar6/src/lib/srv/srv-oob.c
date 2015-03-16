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

#include "srv.h"

/**
 * lw6srv_oob_new
 *
 * @sys_context: global system context
 * @remote_ip: remote IP address
 * @remote_port: remote port
 * @sock: the socket handler (either TCP or UDP)
 * @first_line: the first line of data (can be NULL)
 *
 * Create a new OOB structure, copying required objects.
 * We need to make copies for this is for usage in a
 * separate thread. The thread member is not set
 * here since the right way to do things is first to
 * set up data then to fire the thread.
 *
 * Return value: new object
 */
lw6srv_oob_t *
lw6srv_oob_new (lw6sys_context_t * sys_context, const char *remote_ip, int remote_port, int sock, const char *first_line)
{
  lw6srv_oob_t *oob = NULL;

  oob = (lw6srv_oob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6srv_oob_t));
  if (oob)
    {
      oob->data.creation_timestamp = lw6sys_get_timestamp (sys_context);
      oob->data.do_not_finish = 0;
      oob->data.remote_ip = lw6sys_str_copy (sys_context, remote_ip);
      oob->data.remote_port = remote_port;
      oob->data.sock = sock;
      if (first_line)
	{
	  oob->data.first_line = lw6sys_str_copy (sys_context, first_line);
	}
    }

  if (oob)
    {
      if (!oob->data.remote_ip)
	{
	  lw6srv_oob_free (sys_context, oob);
	  oob = NULL;
	}
    }

  return oob;
}

/**
 * lw6srv_oob_free
 *
 * @sys_context: global system context
 * @oob: the object to free
 *
 * Frees an OOB structure.
 *
 * Return value: none
 */
void
lw6srv_oob_free (lw6sys_context_t * sys_context, lw6srv_oob_t * oob)
{
  if (oob)
    {
      if (oob->thread)
	{
	  /*
	   * Normally do_not_finish is already set,
	   * or it should, for it's better to set
	   * this flag on all objects then free
	   * them later.
	   */
	  oob->data.do_not_finish = 1;
	  if (oob->data.remote_ip)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joining srv OOB thread serving \"%s:%d\""), oob->data.remote_ip, oob->data.remote_port);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joining srv OOB thread"));
	    }
	  lw6sys_thread_join (sys_context, oob->thread);
	  if (oob->data.remote_ip)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joined srv OOB thread serving \"%s:%d\""), oob->data.remote_ip, oob->data.remote_port);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joined srv OOB thread"));
	    }
	}

      if (oob->data.remote_ip)
	{
	  LW6SYS_FREE (sys_context, oob->data.remote_ip);
	}
      if (oob->data.first_line)
	{
	  LW6SYS_FREE (sys_context, oob->data.first_line);
	}
      LW6SYS_FREE (sys_context, oob);
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("oob freed"));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL oob"));
    }
}
