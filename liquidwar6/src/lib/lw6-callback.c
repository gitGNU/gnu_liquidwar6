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

#include "liquidwar6.h"

/**
 * lw6_resize_callback
 *
 * @sys_context: global system context
 * @video_mode: the new video mode
 *
 * This callback is here because gfx needs to update the config
 * when the screen is resized. But... we did not want to make
 * gfx depend on cfg "directly". It's cleaner to pass parameters
 * with Scheme, in the long run, it should make things easier.
 * So this callback is the solution. Another side effect is that
 * this way there's a tangible obvious trace of this updating
 * of config status by the gfx module. Seeing it sticking out
 * like a thumb isn't a bad thing.
 *
 * Return value: none
 */
void
lw6_resize_callback (lw6sys_context_t * sys_context, lw6gui_video_mode_t * video_mode)
{
  if (lw6_global.cfg_context)
    {
      lw6cfg_set_option_int (sys_context, lw6_global.cfg_context, LW6DEF_WIDTH, video_mode->width);
      lw6cfg_set_option_int (sys_context, lw6_global.cfg_context, LW6DEF_HEIGHT, video_mode->height);
      lw6cfg_set_option_bool (sys_context, lw6_global.cfg_context, LW6DEF_FULLSCREEN, video_mode->fullscreen);
    }
}
