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
#include <ncurses.h>

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
  const char **d = NULL;
  const char *driver_id = NULL;
  const char *driver_desc = NULL;
  _mod_caca_context_t *caca_context = NULL;

  caca_context =
    (_mod_caca_context_t *) LW6SYS_CALLOC (sizeof (_mod_caca_context_t));
  if (caca_context)
    {
      caca_context->version = (const char *) caca_get_version ();
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("libcaca version %s"),
		  caca_context->version);
      caca_context->driver_list =
	(const char **) caca_get_display_driver_list ();
      d = caca_context->driver_list;
      if (d)
	{
	  while ((driver_id = d[0]) != NULL && (driver_desc = d[1]) != NULL)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("libcaca driver available id=\"%s\" desc=\"%s\""),
			  d[0], d[1]);
	      d += 2;
	    }
	}

      if (_mod_caca_path_init (&(caca_context->path), argc, argv))
	{
	  if (_mod_caca_load_consts (caca_context))
	    {
	      lw6gui_input_init (&(caca_context->input));

	      if (_mod_caca_set_resize_callback
		  (caca_context, resize_callback))
		{
		  /*
		   * Disabling console output, it could interfere with libcaca
		   * output and wreck everything...
		   */
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("disabling console output (libcaca)"));
		  lw6sys_log_console_enable (0);

		  if (_mod_caca_set_video_mode (caca_context, video_mode))
		    {
		      // OK
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
      else
	{
	  lw6sys_log (LW6SYS_LOG_ERROR, _("unable to init paths"));
	  _mod_caca_quit (caca_context);
	  caca_context = NULL;
	}
    }

  /*
   * Depending on global success, might be interesting to
   * re-enable console output, should have been done by _mod_caca_quit, 
   * but, just in case...
   */
  lw6sys_log_console_enable (caca_context == NULL);

  return caca_context;
}

/*
 * Ends-up all SDL stuff.
 */
void
_mod_caca_quit (_mod_caca_context_t * caca_context)
{
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("quit libcaca"));

  if (caca_context->display)
    {
      if (caca_context->canvas)
	{
	  caca_clear_canvas (caca_context->canvas);
	}
      caca_refresh_display (caca_context->display);
      caca_free_display (caca_context->display);
      lw6sys_snooze ();
      caca_context->display = NULL;
    }

  lw6sys_log_console_enable (1);
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("console output enabled (libcaca)"));

  lw6gui_input_quit (&(caca_context->input));

  _mod_caca_unload_consts (caca_context);
  _mod_caca_path_quit (&(caca_context->path));

  LW6SYS_FREE (caca_context);
}
