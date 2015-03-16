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

#include "gl1-utils.h"

/*
 * Backend specific functions
 */
void
mod_gl1_utils_pump_event_callback (lw6sys_context_t * sys_context, void *data, void *event)
{
  lw6gui_video_mode_t video_mode;
  SDL_Event *sdl_event = (SDL_Event *) event;
  mod_gl1_utils_context_t *utils_context = (mod_gl1_utils_context_t *) data;

  switch (sdl_event->type)
    {
    case SDL_VIDEORESIZE:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL_VIDEORESIZE mod_gl1 specific code triggered"));
      mod_gl1_utils_get_video_mode (sys_context, utils_context, &video_mode);
      video_mode.width = sdl_event->resize.w;
      video_mode.height = sdl_event->resize.h;
      mod_gl1_utils_resize_video_mode (sys_context, utils_context, &video_mode);
      mod_gl1_utils_timer_set_bitmap_refresh (sys_context, utils_context);
      break;
    case SDL_VIDEOEXPOSE:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL_VIDEOEXPOSE mod_gl1 specific code triggered"));
      mod_gl1_utils_sync_mode (sys_context, utils_context, 0);
      mod_gl1_utils_timer_set_bitmap_refresh (sys_context, utils_context);
      break;
    }
}
