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

#include <string.h>

#include "../../gfx.h"
#include "gl1-utils.h"

void
mod_gl1_utils_smoothers_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  lw6gui_smoother_init (sys_context, &(utils_context->smoothers.global_zoom), 1.0f, utils_context->const_data.smoother_global_zoom_duration);

  lw6gui_smoother_init (sys_context, &(utils_context->smoothers.map_center_x), 0, utils_context->const_data.smoother_center_duration);
  lw6gui_smoother_init (sys_context, &(utils_context->smoothers.map_center_y), 0, utils_context->const_data.smoother_center_duration);
  /*
     lw6gui_smoother_init (sys_context,&(utils_context->smoothers.drawable_x1), 0,
     utils_context->const_data.smoother_drawable_duration);
     lw6gui_smoother_init (sys_context,&(utils_context->smoothers.drawable_y1), 0,
     utils_context->const_data.smoother_drawable_duration);
     lw6gui_smoother_init (sys_context,&(utils_context->smoothers.drawable_x2), 0,
     utils_context->const_data.smoother_drawable_duration);
     lw6gui_smoother_init (sys_context,&(utils_context->smoothers.drawable_y2), 0,
     utils_context->const_data.smoother_drawable_duration);
   */
  mod_gl1_utils_smoothers_reset_drawable (sys_context, utils_context);
}

void
mod_gl1_utils_smoothers_reset_drawable (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  /*
     lw6gui_smoother_immediate_force (sys_context,&(utils_context->smoothers.drawable_x1),
     0);
     lw6gui_smoother_immediate_force (sys_context,&(utils_context->smoothers.drawable_y1),
     0);
     lw6gui_smoother_immediate_force (sys_context,&(utils_context->smoothers.drawable_x2),
     utils_context->video_mode.width);
     lw6gui_smoother_immediate_force (sys_context,&(utils_context->smoothers.drawable_y2),
     utils_context->video_mode.height);
   */
}

void
mod_gl1_utils_smoothers_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  //float drawable_x1, drawable_y1, drawable_x2, drawable_y2;
  utils_context->smoothed.global_zoom =
    lw6gui_smoother_get_value (sys_context, &(utils_context->smoothers.global_zoom),
			       _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context)));
  utils_context->smoothed.map_center_x =
    lw6gui_smoother_get_value (sys_context, &(utils_context->smoothers.map_center_x),
			       _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context)));
  utils_context->smoothed.map_center_y =
    lw6gui_smoother_get_value (sys_context, &(utils_context->smoothers.map_center_y),
			       _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context)));
  /*
     drawable_x1 =
     lw6gui_smoother_get_value (sys_context,&(utils_context->smoothers.drawable_x1),
     utils_context->timer.timestamp);
     drawable_x2 =
     lw6gui_smoother_get_value (sys_context,&(utils_context->smoothers.drawable_x2),
     utils_context->timer.timestamp);
     drawable_y1 =
     lw6gui_smoother_get_value (sys_context,&(utils_context->smoothers.drawable_y1),
     utils_context->timer.timestamp);
     drawable_y2 =
     lw6gui_smoother_get_value (sys_context,&(utils_context->smoothers.drawable_y2),
     utils_context->timer.timestamp);
     lw6gui_zone_init_x1y1x2y2 (sys_context,&(utils_context->smoothed.drawable), drawable_x1,
     drawable_y1, drawable_x2, drawable_y2);
   */
}
