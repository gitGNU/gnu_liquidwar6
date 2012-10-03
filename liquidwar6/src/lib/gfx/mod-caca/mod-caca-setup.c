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
#endif // HAVE_CONFIG_H

#include <time.h>

#include "../gfx.h"
#include "mod-caca-internal.h"

/*
 * Low-level SDL initialisation.
 */
_mod_caca_context_t *
_mod_caca_init (int argc, const char *argv[],
		lw6gui_video_mode_t * video_mode,
		lw6gui_resize_callback_func_t resize_callback)
{
  _mod_caca_context_t *caca_context = NULL;

  caca_context =
    (_mod_caca_context_t *) LW6SYS_CALLOC (sizeof (_mod_caca_context_t));
  if (caca_context)
    {
      /*
       * Todo: init library
       */
      if (_mod_caca_load_consts (caca_context))
	{
	  if (_mod_caca_set_resize_callback (caca_context, resize_callback))
	    {
	      if (_mod_caca_set_video_mode (caca_context, video_mode))
		{
		  // todo
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_ERROR,
			      _("unable to set video mode"));
		  _mod_caca_quit (caca_context);
		  caca_context = NULL;
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_ERROR,
			  _("unable to set resize callback"));
	      _mod_caca_quit (caca_context);
	      caca_context = NULL;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_ERROR, _("unable to load consts"));
	  _mod_caca_quit (caca_context);
	  caca_context = NULL;
	}
    }

  return caca_context;
}

/*
 * Ends-up all SDL stuff.
 */
void
_mod_caca_quit (_mod_caca_context_t * caca_context)
{
  _mod_caca_unload_consts (caca_context);

  LW6SYS_FREE (caca_context);
}
