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
#endif // HAVE_CONFIG_H

#include "p2p.h"
#include "p2p-internal.h"

_lw6p2p_srv_oob_callback_data_t *
_lw6p2p_srv_oob_callback_data_new (lw6sys_context_t * sys_context, lw6srv_backend_t * backend,
				   lw6nod_info_t * node_info, const char *remote_ip, int remote_port, int sock, const char *first_line)
{
  _lw6p2p_srv_oob_callback_data_t *ret = NULL;

  ret = (_lw6p2p_srv_oob_callback_data_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6p2p_srv_oob_callback_data_t));
  if (ret)
    {
      ret->backend = backend;
      ret->node_info = node_info;
      ret->srv_oob = lw6srv_oob_new (sys_context, remote_ip, remote_port, sock, first_line);
      if (!ret->srv_oob)
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_lw6p2p_srv_oob_callback_data_free (lw6sys_context_t * sys_context, _lw6p2p_srv_oob_callback_data_t * srv_oob)
{
  if (srv_oob)
    {
      if (srv_oob->srv_oob)
	{
	  lw6srv_oob_free (sys_context, srv_oob->srv_oob);
	}
      LW6SYS_FREE (sys_context, srv_oob);
    }
}

int
_lw6p2p_srv_oob_filter (lw6sys_context_t * sys_context, _lw6p2p_srv_oob_callback_data_t * srv_oob)
{
  int ret = 1;
  lw6sys_thread_handler_t *thread;

  if (srv_oob && srv_oob->srv_oob)
    {
      thread = srv_oob->srv_oob->thread;
      if (thread)
	{
	  if (lw6sys_thread_is_callback_done (sys_context, thread))
	    {
	      /*
	       * We don't need to join the thread, it will
	       * be done when deleting the srv_oob object.
	       */
	      ret = 0;
	    }
	}
      else
	{
	  ret = 0;		// no thread, we exit
	}
    }

  return ret;
}

void
_lw6p2p_srv_oob_callback (lw6sys_context_t * sys_context, void *callback_data)
{
  _lw6p2p_srv_oob_callback_data_t *srv_oob = (_lw6p2p_srv_oob_callback_data_t *) callback_data;
  int ret = 0;

  ret = lw6srv_process_oob (sys_context, srv_oob->backend, srv_oob->node_info, &(srv_oob->srv_oob->data));

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("_srv_oob_callback done ret=%d"), ret);
}
