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

#include "../../../gfx.h"
#include "gl1-flat.h"
#include "gl1-flat-internal.h"

void
_mod_gl1_view_flat_viewport_update_preview (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					    utils_context,
					    _mod_gl1_view_flat_context_t * flat_context, const lw6gui_look_t * look, const lw6map_level_t * level)
{
  int center_x = 0;
  int center_y = 0;
  float global_zoom = 1.0f;

  center_x = level->body.shape.w / 2;
  center_y = level->body.shape.h / 2;

  lw6gui_smoother_immediate_force (sys_context, &(utils_context->smoothers.map_center_x), center_x);
  lw6gui_smoother_immediate_force (sys_context, &(utils_context->smoothers.map_center_y), center_y);

  global_zoom = look->dynamic_zoom * look->style.zoom;
  lw6gui_smoother_set_target (sys_context, &(utils_context->smoothers.global_zoom), global_zoom,
			      _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context)));

  mod_gl1_utils_smoothers_update (sys_context, utils_context);

  lw6gui_viewport_init (sys_context, &(flat_context->viewport),
			utils_context->sdl_context.video_mode.width,
			utils_context->sdl_context.video_mode.height,
			utils_context->sdl_context.viewport.x1,
			utils_context->sdl_context.viewport.y1,
			utils_context->sdl_context.viewport.x2,
			utils_context->sdl_context.viewport.y2,
			center_x,
			center_y,
			level->body.shape.w,
			level->body.shape.h,
			level->param.rules.x_polarity,
			level->param.rules.y_polarity,
			look->style.x_wrap,
			look->style.y_wrap, look->style.keep_ratio, utils_context->smoothed.global_zoom, flat_context->const_data.scroll_limit, 0);
}

void
_mod_gl1_view_flat_viewport_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				    utils_context,
				    _mod_gl1_view_flat_context_t *
				    flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  float global_zoom = 1.0f;
  float map_mouse_x = 0.0f;
  float map_mouse_y = 0.0f;
  lw6gui_viewport_t test;
  int main_cursor_x = 0;
  int main_cursor_y = 0;
  lw6sys_whd_t shape;
  int mouse_controlled = 0;
  int dx = 0, dy = 0, px = 0, py = 0, sx = 0, sy = 0;
  int delta_x = 0, delta_y = 0;
  float center_x = 0.0f, center_y = 0.0f;
  int drag_pop = 0;
  float x, y, z;

  lw6ker_game_state_get_shape (sys_context, game_state, &shape);

  main_cursor_x = shape.w / 2;
  main_cursor_y = shape.h / 2;

  if (local_cursors)
    {
      if (!lw6pil_local_cursors_get_main_info (sys_context, local_cursors, NULL, &main_cursor_x, &main_cursor_y, &mouse_controlled))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no main cursor"));
	}
    }

  x = main_cursor_x;
  y = main_cursor_y;
  z = 0;
  /*
   * Normally fix_10 is called by calling, but just in case.
   */
  lw6pil_coords_fix_x10 (sys_context, &(game_state->game_struct->rules), &shape, &x, &y, &z);
  main_cursor_x = x;
  main_cursor_y = y;

  if (mouse_controlled)
    {
      drag_pop = lw6gui_mouse_drag_pop (sys_context, &(utils_context->sdl_context.input.mouse), &dx, &dy, &px, &py, &sx, &sy);
      delta_x = px - (flat_context->viewport.drawable.x1 + flat_context->viewport.drawable.x2) / 2.0f;
      delta_y = py - (flat_context->viewport.drawable.y1 + flat_context->viewport.drawable.y2) / 2.0f;
      lw6gui_viewport_calc_drag (sys_context, &(flat_context->viewport), &center_x, &center_y, main_cursor_x, main_cursor_y, delta_x, delta_y);
      if (dx || dy)
	{
	  lw6gui_smoother_immediate_force (sys_context, &(utils_context->smoothers.map_center_x), center_x);
	  lw6gui_smoother_immediate_force (sys_context, &(utils_context->smoothers.map_center_y), center_y);
	}
      if (drag_pop)
	{
	  flat_context->viewport.speed_x = sx;
	  flat_context->viewport.speed_y = sy;
	}
    }
  else
    {
      center_x = main_cursor_x;
      center_y = main_cursor_y;
      lw6gui_smoother_set_target (sys_context, &(utils_context->smoothers.map_center_x), center_x,
				  _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context)));
      lw6gui_smoother_set_target (sys_context, &(utils_context->smoothers.map_center_y), center_y,
				  _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context)));
      lw6gui_smoother_fix_overflow (sys_context, &(utils_context->smoothers.map_center_x), shape.w * LW6PIL_COORDS_X2);
      lw6gui_smoother_fix_overflow (sys_context, &(utils_context->smoothers.map_center_y), shape.h * LW6PIL_COORDS_X2);
    }

  global_zoom = look->dynamic_zoom * look->style.zoom;
  lw6gui_smoother_set_target (sys_context, &(utils_context->smoothers.global_zoom), global_zoom,
			      _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context)));

  mod_gl1_utils_smoothers_update (sys_context, utils_context);

  lw6gui_viewport_init (sys_context, &test,
			utils_context->sdl_context.video_mode.width,
			utils_context->sdl_context.video_mode.height,
			utils_context->sdl_context.viewport.x1,
			utils_context->sdl_context.viewport.y1,
			utils_context->sdl_context.viewport.x2,
			utils_context->sdl_context.viewport.y2,
			utils_context->smoothed.map_center_x,
			utils_context->smoothed.map_center_y,
			shape.w,
			shape.h,
			game_state->game_struct->rules.x_polarity,
			game_state->game_struct->rules.y_polarity,
			look->style.x_wrap,
			look->style.y_wrap, look->style.keep_ratio, utils_context->smoothed.global_zoom, flat_context->const_data.scroll_limit, 1);

  flat_context->viewport = test;

  lw6gui_viewport_screen_to_map (sys_context, &(flat_context->viewport), &map_mouse_x, &map_mouse_y, utils_context->sdl_context.input.mouse.screen_pointer.pos_x, utils_context->sdl_context.input.mouse.screen_pointer.pos_y, 0);	// was 1

  utils_context->sdl_context.input.mouse.map_pointer.pos_x = map_mouse_x;
  utils_context->sdl_context.input.mouse.map_pointer.pos_y = map_mouse_y;
}
