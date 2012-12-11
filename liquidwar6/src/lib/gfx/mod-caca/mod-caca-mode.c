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

#include <errno.h>

#include "mod-caca-internal.h"

static caca_display_t *
_set_with_driver (const char *driver)
{
  caca_display_t *ret = NULL;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("trying libcaca driver \"%s\""), driver);
  ret = caca_create_display_with_driver (NULL, driver);
  if (ret == NULL)
    {
      switch (errno)
	{
	case ENOMEM:
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("libcaca driver \"%s\" failed to create display (memory problem)"),
		      driver);
	  break;
	case ENODEV:
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("libcaca driver \"%s\" failed to create display (device problem)"),
		      driver);
	  break;
	default:
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("libcaca driver \"%s\" failed to create display (unknown problem)"),
		      driver);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("set libcaca driver \"%s\""), driver);
    }

  return ret;
}

/*
 * Initialize display.
 */
int
_mod_caca_set_video_mode (_mod_caca_context_t * caca_context,
			  lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  caca_context->display =
    _set_with_driver (caca_context->const_data.video_mode_default);
  if (caca_context == NULL)
    {
      caca_context->display =
	_set_with_driver (caca_context->const_data.video_mode_fallback);
    }

  if (caca_context->display)
    {
      caca_context->canvas = caca_get_canvas (caca_context->display);
    }

  if (caca_context->display && caca_context->canvas)
    {
      caca_set_display_title (caca_context->display,
			      lw6sys_build_get_package_string ());
      caca_context->video_mode.width =
	caca_get_canvas_width (caca_context->canvas);
      caca_context->video_mode.height =
	caca_get_canvas_height (caca_context->canvas);
      caca_context->video_mode.fullscreen = 1;
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("libcaca mode with canvas=%dx%d OK"),
		  caca_context->video_mode.width,
		  caca_context->video_mode.height);
      ret = 1;
    }

  return ret;
}


/*
 * Called whenever window resize is asked for.
 */
int
_mod_caca_resize_video_mode (_mod_caca_context_t * caca_context,
			     lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  /*
   * Todo -> update video mode on a resize
   */

  return ret;
}

int
_mod_caca_get_video_mode (_mod_caca_context_t * caca_context,
			  lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  if (caca_context && video_mode)
    {
      caca_context->video_mode = *video_mode;
    }

  return ret;
}

int
_mod_caca_set_resize_callback (_mod_caca_context_t * caca_context,
			       lw6gui_resize_callback_func_t resize_callback)
{
  int ret = 0;

  caca_context->resize_callback = resize_callback;
  ret = 1;

  return ret;
}

void
_mod_caca_call_resize_callback (_mod_caca_context_t * caca_context)
{
  if (caca_context->resize_callback)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("calling resize callback"));
      caca_context->resize_callback (&(caca_context->video_mode));
    }
}
