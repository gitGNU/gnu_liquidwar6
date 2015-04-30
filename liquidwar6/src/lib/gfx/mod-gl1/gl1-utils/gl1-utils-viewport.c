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

void
mod_gl1_utils_viewport_drawable_max (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  lw6gui_zone_init_xywh (sys_context, &(utils_context->sdl_context.viewport), 0.0f, 0.0f,
			 utils_context->sdl_context.video_mode.width, utils_context->sdl_context.video_mode.height);
}

void
mod_gl1_utils_viewport_screen_to_gl (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, float *gl_x, float *gl_y, int screen_x,
				     int screen_y)
{
  if (utils_context->sdl_context.video_mode.width > 0 && utils_context->sdl_context.video_mode.height > 0)
    {
      *gl_x = (2.0f * ((float) screen_x) / ((float) utils_context->sdl_context.video_mode.width)) - 1.0f;
      *gl_y = 1.0f - (2.0f * ((float) screen_y) / ((float) utils_context->sdl_context.video_mode.height));
    }
  else
    {
      *gl_x = -1.0f;
      *gl_y = -1.0f;
    }
}

void
mod_gl1_utils_viewport_gl_to_screen (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int *screen_x, int *screen_y, float gl_x,
				     float gl_y)
{
  if (utils_context->sdl_context.video_mode.width > 0 && utils_context->sdl_context.video_mode.height > 0)
    {
      *screen_x = (int) (((gl_x + 1.0f) / 2.0f) * ((float) utils_context->sdl_context.video_mode.width));
      *screen_y = (int) (((1.0f - gl_y) / 2.0f) * ((float) utils_context->sdl_context.video_mode.height));
    }
  else
    {
      *screen_x = 0;
      *screen_y = 0;
    }
}
