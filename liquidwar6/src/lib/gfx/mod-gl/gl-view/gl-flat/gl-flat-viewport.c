/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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
#include "gl-flat.h"
#include "gl-flat-internal.h"

void
_mod_gl_view_flat_viewport_update_preview (mod_gl_utils_context_t *
					   utils_context,
					   _mod_gl_view_flat_context_t *
					   flat_context,
					   lw6gui_look_t * look,
					   lw6map_level_t * level)
{
  int center_x = 0;
  int center_y = 0;
  float global_zoom = 1.0f;

  center_x = level->body.shape.w / 2;
  center_y = level->body.shape.h / 2;

  /*
     lw6gui_smoother_set_target (&(utils_context->smoothers.center_x), center_x,
     utils_context->timer.timestamp);
     lw6gui_smoother_set_target (&(utils_context->smoothers.center_y), center_y,
     utils_context->timer.timestamp);
   */

  global_zoom = look->dynamic_zoom * look->style.zoom;
  lw6gui_smoother_set_target (&(utils_context->smoothers.global_zoom),
			      global_zoom, utils_context->timer.timestamp);

  mod_gl_utils_smoothers_update (utils_context);

  lw6gui_viewport_init (&(flat_context->viewport),
			utils_context->video_mode.width,
			utils_context->video_mode.height,
			utils_context->viewport.x1,
			utils_context->viewport.y1,
			utils_context->viewport.x2,
			utils_context->viewport.y2,
			level->body.shape.w / 2,
			level->body.shape.h / 2,
			level->body.shape.w,
			level->body.shape.h,
			level->param.rules.x_polarity,
			level->param.rules.y_polarity,
			look->style.x_wrap,
			look->style.y_wrap,
			look->style.keep_ratio,
			utils_context->smoothed.global_zoom,
			flat_context->const_data.scroll_limit, 0);
}

void
_mod_gl_view_flat_viewport_update (mod_gl_utils_context_t *
				   utils_context,
				   _mod_gl_view_flat_context_t *
				   flat_context,
				   lw6gui_look_t * look,
				   lw6ker_game_state_t *
				   game_state,
				   lw6pil_local_cursors_t * local_cursors)
{
  float global_zoom = 1.0f;
  float mouse_x;
  float mouse_y;
  lw6gui_viewport_t test;
  float test_cursor_x = 0.0f;
  float test_cursor_y = 0.0f;
  int main_cursor_x = 0;
  int main_cursor_y = 0;
  float dw, dh;
  lw6sys_whd_t shape;
  int mouse_controlled = 0;
  int dx = 0, dy = 0, sx = 0, sy = 0;

  lw6ker_game_state_get_shape (game_state, &shape);

  main_cursor_x = shape.w / 2;
  main_cursor_y = shape.h / 2;

  if (local_cursors)
    {
      if (!lw6pil_local_cursors_get_main_info
	  (local_cursors, NULL, &main_cursor_x, &main_cursor_y,
	   &mouse_controlled))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("no main cursor"));
	}
    }
  /*
     if (mouse_controlled)
     {
     TMP("mouse controlled");
     }
     else
     {
     TMP("center");
     }
   */

  if (mouse_controlled)
    {
      if (lw6gui_mouse_drag_pop
	  (&(utils_context->input.mouse), &dx, &dy, &sx, &sy))
	{
	  TMP2 ("drag! %d %d", dx, dy);
	}
      else
	{
	  if (dx || dy)
	    {
	      TMP2 ("%d %d", dx, dy);
	    }
	}
    }

  global_zoom = look->dynamic_zoom * look->style.zoom;
  lw6gui_smoother_set_target (&(utils_context->smoothers.global_zoom),
			      global_zoom, utils_context->timer.timestamp);

  mod_gl_utils_smoothers_update (utils_context);

  lw6gui_viewport_init (&test,
			utils_context->video_mode.width,
			utils_context->video_mode.height,
			utils_context->viewport.x1,
			utils_context->viewport.y1,
			utils_context->viewport.x2,
			utils_context->viewport.y2,
			main_cursor_x,
			main_cursor_y,
			shape.w,
			shape.h,
			game_state->game_struct->rules.x_polarity,
			game_state->game_struct->rules.y_polarity,
			look->style.x_wrap,
			look->style.y_wrap,
			look->style.keep_ratio,
			utils_context->smoothed.global_zoom,
			flat_context->const_data.scroll_limit, 1);

  flat_context->viewport = test;

  /*
     lw6gui_viewport_map_to_screen (&test, &test_cursor_x, &test_cursor_y,
     main_cursor_x, main_cursor_y, 0);
     dw = test.map_main_clipped.w * flat_context->const_data.scroll_limit;
     dh = test.map_main_clipped.h * flat_context->const_data.scroll_limit;

     if (test_cursor_x < test.map_visible.x1 + dw
     || test_cursor_x > test.map_visible.x2 - dw)
     {
     lw6gui_smoother_set_target (&(utils_context->smoothers.center_x),
     main_cursor_x,
     utils_context->timer.timestamp);
     }
     if (test_cursor_y < test.map_visible.y1 + dh
     || test_cursor_y > test.map_visible.y2 - dh)
     {
     lw6gui_smoother_set_target (&(utils_context->smoothers.center_y),
     main_cursor_y,
     utils_context->timer.timestamp);
     }

     mod_gl_utils_smoothers_update (utils_context);

     lw6gui_viewport_init (&(flat_context->viewport),
     utils_context->video_mode.width,
     utils_context->video_mode.height,
     utils_context->smoothed.drawable.x1,
     utils_context->smoothed.drawable.y1,
     utils_context->smoothed.drawable.x2,
     utils_context->smoothed.drawable.y2,
     utils_context->smoothed.center_x,
     utils_context->smoothed.center_y,
     shape.w,
     shape.h,
     game_state->game_struct->rules.x_polarity,
     game_state->game_struct->rules.y_polarity,
     look->style.x_wrap,
     look->style.y_wrap,
     look->style.keep_ratio,
     utils_context->smoothed.global_zoom,
     flat_context->const_data.scroll_limit, 1);
   */
  lw6gui_viewport_screen_to_map (&(flat_context->viewport), &mouse_x, &mouse_y, utils_context->input.mouse.screen_pointer.pos_x, utils_context->input.mouse.screen_pointer.pos_y, 0);	// was 1

  utils_context->input.mouse.map_pointer.pos_x = mouse_x;
  utils_context->input.mouse.map_pointer.pos_y = mouse_y;
}
