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

#include "mod-caca-internal.h"

/*
 * Initialize display.
 */
int
_mod_caca_set_video_mode (_mod_caca_context_t * caca_context,
			  lw6gui_video_mode_t * video_mode)
{
  int ret = 0;

  /*
   * Todo -> set up video mode
   */

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
