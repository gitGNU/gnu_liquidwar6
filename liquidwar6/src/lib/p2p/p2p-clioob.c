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

#include "p2p.h"
#include "p2p-internal.h"

_lw6p2p_cli_oob_callback_data_t *
_lw6p2p_cli_oob_callback_data_new (lw6cli_backend_t * backend,
				   lw6nod_info_t * node_info,
				   char *public_url)
{
  _lw6p2p_cli_oob_callback_data_t *ret = NULL;

  ret =
    (_lw6p2p_cli_oob_callback_data_t *)
    LW6SYS_CALLOC (sizeof (_lw6p2p_cli_oob_callback_data_t));
  if (ret)
    {
      ret->backend = backend;
      ret->node_info = node_info;
      ret->cli_oob = lw6cli_oob_new (public_url);
      if (!ret->cli_oob)
	{
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_lw6p2p_cli_oob_callback_data_free (_lw6p2p_cli_oob_callback_data_t * cli_oob)
{
  if (cli_oob)
    {
      if (cli_oob->cli_oob)
	{
	  lw6cli_oob_free (cli_oob->cli_oob);
	}
      LW6SYS_FREE (cli_oob);
    }
}

int
_lw6p2p_cli_oob_filter (_lw6p2p_cli_oob_callback_data_t * cli_oob)
{
  int ret = 1;
  void *thread;

  if (cli_oob && cli_oob->cli_oob)
    {
      thread = cli_oob->cli_oob->thread;
      if (thread)
	{
	  if (lw6sys_thread_is_callback_done (thread))
	    {
	      /*
	       * We don't need to join the thread, it will
	       * be done when deleting the cli_oob object.
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
_lw6p2p_cli_oob_callback (void *callback_data)
{
  _lw6p2p_cli_oob_callback_data_t *cli_oob =
    (_lw6p2p_cli_oob_callback_data_t *) callback_data;
  int ret = 0;

  ret =
    lw6cli_process_oob (cli_oob->backend, cli_oob->node_info,
			&(cli_oob->cli_oob->data));

  lw6sys_log (LW6SYS_LOG_DEBUG, _("_cli_oob_callback done ret=%d"), ret);
}
