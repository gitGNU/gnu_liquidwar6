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

static void
_set_map_rules (mod_gl_utils_context_t * utils_context,
		_mod_gl_view_flat_context_t * flat_context)
{
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		   GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

static void
_set_fighters_rules (mod_gl_utils_context_t * utils_context,
		     _mod_gl_view_flat_context_t * flat_context)
{
  // glTexParameteri useless herem set in mod_gl_utils_texture_update
  /*
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   */
}

/*
static void
_set_cursor_rules (mod_gl_utils_context_t * utils_context,
		   _mod_gl_view_flat_context_t * flat_context)
{
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
*/

static void
_display_bitmap_array (mod_gl_utils_context_t * utils_context,
		       _mod_gl_view_flat_context_t * flat_context,
		       int bitmap_array_w,
		       int bitmap_array_h,
		       int shape_w,
		       int shape_h,
		       mod_gl_utils_bitmap_array_t * bitmap_array,
		       lw6gui_look_t * look)
{
  float x1, y1, x2, y2, w, h, coef_x, coef_y;
  float texture_x1, texture_x2, texture_y1, texture_y2;
  float texture_w, texture_h, layout_w, layout_h;
  int n_x, n_y;
  int view_x, view_y, view_w, view_h;
  float min_x, min_y, max_x, max_y;
  mod_gl_utils_bitmap_t *bitmap = NULL;

  x1 = flat_context->viewport.map_main.x1;
  y1 = flat_context->viewport.map_main.y1;
  x2 = flat_context->viewport.map_main.x2;
  y2 = flat_context->viewport.map_main.y2;

  w = x2 - x1;
  h = y2 - y1;
  coef_x = w / bitmap_array_w;
  coef_y = h / bitmap_array_h;

  view_x = flat_context->viewport.drawable.x1;
  view_y = flat_context->viewport.drawable.y1;
  view_w = flat_context->viewport.drawable.w;
  view_h = flat_context->viewport.drawable.h;

  min_x = view_x;
  min_y = view_y;
  max_x = view_x + view_w;
  max_y = view_y + view_h;

  if (bitmap_array->layout.n_w > 0)
    {
      for (n_x = 0; n_x < bitmap_array->layout.n_w; ++n_x)
	{
	  bitmap_array->layout.screen_x[n_x] =
	    x1 + (bitmap_array->layout.x0[n_x] + 1) * coef_x;
	}
      bitmap_array->layout.screen_x[bitmap_array->layout.n_w] =
	bitmap_array->layout.screen_x[bitmap_array->layout.n_w - 1] +
	(bitmap_array->layout.w[bitmap_array->layout.n_w - 1] - 2) * coef_x;
    }

  if (bitmap_array->layout.n_h > 0)
    {
      for (n_y = 0; n_y < bitmap_array->layout.n_h; ++n_y)
	{
	  bitmap_array->layout.screen_y[n_y] =
	    y1 + (bitmap_array->layout.y0[n_y] + 1) * coef_y;
	}
      bitmap_array->layout.screen_y[bitmap_array->layout.n_h] =
	bitmap_array->layout.screen_y[bitmap_array->layout.n_h - 1] +
	(bitmap_array->layout.h[bitmap_array->layout.n_h - 1] - 2) * coef_y;
    }

  for (n_y = 0; n_y < bitmap_array->layout.n_h; ++n_y)
    {
      for (n_x = 0; n_x < bitmap_array->layout.n_w; ++n_x)
	{
	  if (bitmap_array->layout.screen_x[n_x] > max_x ||
	      bitmap_array->layout.screen_y[n_y] > max_y ||
	      bitmap_array->layout.screen_x[n_x + 1] < min_x
	      || bitmap_array->layout.screen_y[n_y + 1] < min_y)
	    {
	      // skip texture, it's outside our screen...
	    }
	  else
	    {
	      layout_w = bitmap_array->layout.w[n_x];
	      layout_h = bitmap_array->layout.h[n_y];
	      texture_w = mod_gl_utils_power_of_two_ge (layout_w);
	      texture_h = mod_gl_utils_power_of_two_ge (layout_h);
	      texture_x1 = 1.0f / texture_w;
	      texture_y1 = 1.0f / texture_h;
	      texture_x2 = (layout_w - 1.0f) / texture_w;
	      texture_y2 = (layout_h - 1.0f) / texture_h;
	      bitmap = mod_gl_utils_bitmap_array_get (bitmap_array, n_x, n_y);
	      mod_gl_utils_bitmap_bind (utils_context, bitmap);
	      mod_gl_utils_display_texture (utils_context,
					    bitmap->texture,
					    bitmap_array->layout.
					    screen_x[n_x],
					    bitmap_array->layout.
					    screen_y[n_y],
					    bitmap_array->layout.
					    screen_x[n_x + 1],
					    bitmap_array->layout.
					    screen_y[n_y + 1], texture_x1,
					    texture_y1, texture_x2,
					    texture_y2);
	    }
	}
    }
}

static void
_display_map_preview (mod_gl_utils_context_t * utils_context,
		      _mod_gl_view_flat_context_t * flat_context,
		      lw6gui_look_t * look, lw6map_level_t * level)
{
  _set_map_rules (utils_context, flat_context);
  _display_bitmap_array (utils_context, flat_context,
			 flat_context->game_context.map.level->texture.w,
			 flat_context->game_context.map.level->texture.h,
			 flat_context->game_context.map.level->body.shape.w,
			 flat_context->game_context.map.level->body.shape.h,
			 &(flat_context->game_context.map.map_bitmap_array),
			 look);
}

void
_mod_gl_view_flat_display_preview (mod_gl_utils_context_t * utils_context,
				   _mod_gl_view_flat_context_t *
				   flat_context, lw6gui_look_t * look,
				   lw6map_level_t * level)
{
  if (utils_context && flat_context && level && level->texture.data)
    {
      _mod_gl_view_flat_game_context_update_map (utils_context,
						 flat_context,
						 &(flat_context->
						   game_context.map), look,
						 level);

      if (level->texture.has_alpha)
	{
	  mod_gl_utils_set_render_mode_2d_blend (utils_context);
	}
      else
	{
	  mod_gl_utils_set_render_mode_2d (utils_context);
	}

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl_view_flat_viewport_update_preview (utils_context,
						 flat_context, look, level);
      _display_map_preview (utils_context, flat_context, look, level);
    }
}

void
mod_gl_view_flat_display_preview (mod_gl_utils_context_t * utils_context,
				  void *flat_context, lw6gui_look_t * look,
				  lw6map_level_t * level)
{
  _mod_gl_view_flat_display_preview (utils_context,
				     (_mod_gl_view_flat_context_t *)
				     flat_context, look, level);
}

/*
 * Game_state is not passed as args but retrived
 * from flat_context struct, not to give the illusion that
 * any game_state could be displayed. One *needs* to call
 * game_context_init first.
 */
static void
_display_armies (mod_gl_utils_context_t * utils_context,
		 _mod_gl_view_flat_context_t *
		 flat_context, lw6gui_look_t * look)
{
  lw6sys_whd_t shape;

  lw6ker_game_state_get_shape (flat_context->game_context.armies.game_state,
			       &shape);

  mod_gl_utils_update_game_bitmap_array (utils_context,
					 &(flat_context->game_context.armies.
					   armies_bitmap_array),
					 flat_context->game_context.
					 armies.game_state, look);
  _set_fighters_rules (utils_context, flat_context);
  _display_bitmap_array (utils_context, flat_context,
			 shape.w,
			 shape.h,
			 shape.w,
			 shape.h,
			 &(flat_context->game_context.armies.
			   armies_bitmap_array), look);
}

/*
 * Game_state is not passed as args but retrived
 * from flat_context struct, not to give the illusion that
 * any game_state could be displayed. One *needs* to call
 * game_context_init first.
 */
static void
_display_map (mod_gl_utils_context_t * utils_context,
	      _mod_gl_view_flat_context_t *
	      flat_context, lw6gui_look_t * look,
	      lw6ker_game_state_t * game_state,
	      lw6pil_local_cursors_t * local_cursors)
{
  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display map"));
  _display_map_preview (utils_context, flat_context, look,
			game_state->game_struct->level);
}

void
_mod_gl_view_flat_display_map (mod_gl_utils_context_t * utils_context,
			       _mod_gl_view_flat_context_t *
			       flat_context,
			       lw6gui_look_t * look,
			       lw6ker_game_state_t * game_state,
			       lw6pil_local_cursors_t * local_cursors)
{
  if (utils_context && flat_context && game_state)
    {
      lw6map_level_t *level;

      level = game_state->game_struct->level;

      _mod_gl_view_flat_game_context_update (utils_context,
					     flat_context,
					     &flat_context->game_context,
					     look, level, game_state);

      if (level->texture.has_alpha)
	{
	  mod_gl_utils_set_render_mode_2d_blend (utils_context);
	}
      else
	{
	  mod_gl_utils_set_render_mode_2d (utils_context);
	}

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl_view_flat_viewport_update (utils_context,
					 flat_context, look, game_state,
					 local_cursors);

      _display_map (utils_context, flat_context, look, game_state,
		    local_cursors);
    }
}

void
mod_gl_view_flat_display_map (mod_gl_utils_context_t * utils_context,
			      void *flat_context,
			      lw6gui_look_t * look,
			      lw6ker_game_state_t * game_state,
			      lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl_view_flat_display_map (utils_context,
				 (_mod_gl_view_flat_context_t *)
				 flat_context, look, game_state,
				 local_cursors);
}

/*
 * Game_state is not passed as args but retrived
 * from flat_context struct, not to give the illusion that
 * any game_state could be displayed. One *needs* to call
 * game_context_init first.
 */
static void
_display_fighters (mod_gl_utils_context_t * utils_context,
		   _mod_gl_view_flat_context_t *
		   flat_context, lw6gui_look_t * look,
		   lw6ker_game_state_t * game_state,
		   lw6pil_local_cursors_t * local_cursors)
{
  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display fighters"));
  _display_armies (utils_context, flat_context, look);
}

void
_mod_gl_view_flat_display_fighters (mod_gl_utils_context_t * utils_context,
				    _mod_gl_view_flat_context_t *
				    flat_context,
				    lw6gui_look_t * look,
				    lw6ker_game_state_t * game_state,
				    lw6pil_local_cursors_t * local_cursors)
{
  if (utils_context && flat_context && game_state)
    {
      lw6map_level_t *level;

      level = game_state->game_struct->level;

      _mod_gl_view_flat_game_context_update (utils_context,
					     flat_context,
					     &flat_context->game_context,
					     look, level, game_state);

      mod_gl_utils_set_render_mode_2d_blend (utils_context);

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl_view_flat_viewport_update (utils_context,
					 flat_context, look, game_state,
					 local_cursors);
      _display_fighters (utils_context, flat_context, look, game_state,
			 local_cursors);
    }
}

void
mod_gl_view_flat_display_fighters (mod_gl_utils_context_t * utils_context,
				   void *flat_context,
				   lw6gui_look_t * look,
				   lw6ker_game_state_t * game_state,
				   lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl_view_flat_display_fighters (utils_context,
				      (_mod_gl_view_flat_context_t *)
				      flat_context, look, game_state,
				      local_cursors);
}

static void
_display_cursor_bitmap (mod_gl_utils_context_t * utils_context,
			_mod_gl_view_flat_context_t * flat_context,
			float x, float y, float w, float h,
			mod_gl_utils_bitmap_t * bitmap)
{
  mod_gl_utils_bitmap_bind (utils_context, bitmap);
  mod_gl_utils_bitmap_display (utils_context,
			       bitmap, x - w / 2.0f, y - h / 2.0f,
			       x + w / 2.0f, y + h / 2.0f);
}

static void
_display_cursor (mod_gl_utils_context_t * utils_context,
		 _mod_gl_view_flat_context_t * flat_context,
		 lw6gui_look_t * look, lw6pil_local_cursor_t * local_cursor,
		 int i, int x, int y, int even_odd)
{

  float cursor_x = 0.0f;
  float cursor_y = 0.0f;
  float cursor_w = 0.0f;
  float cursor_h = 0.0f;

  if (local_cursor && local_cursor->is_main && local_cursor->mouse_controlled)
    {
      x = local_cursor->x;
      y = local_cursor->y;
    }

  lw6gui_viewport_map_to_screen (&(flat_context->viewport), &cursor_x,
				 &cursor_y, x, y, 0);

  if (local_cursor)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("cursor %x is local"),
		  local_cursor->cursor_id);
      if (local_cursor->mouse_controlled)
	{
	  cursor_x = utils_context->input.mouse.x;
	  cursor_y = utils_context->input.mouse.y;
	}
      else
	{
	  lw6gui_viewport_map_to_screen (&(flat_context->viewport), &cursor_x,
					 &cursor_y, local_cursor->x,
					 local_cursor->y, 0);
	}

      cursor_x = lw6sys_max (cursor_x, flat_context->viewport.map_visible.x1);
      cursor_x = lw6sys_min (cursor_x, flat_context->viewport.map_visible.x2);
      cursor_y = lw6sys_max (cursor_y, flat_context->viewport.map_visible.y1);
      cursor_y = lw6sys_min (cursor_y, flat_context->viewport.map_visible.y2);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("displaying non-local cursor"));
    }

  if (cursor_x >= flat_context->viewport.map_visible.x1 &&
      cursor_y >= flat_context->viewport.map_visible.y1 &&
      cursor_x <= flat_context->viewport.map_visible.x2 &&
      cursor_y <= flat_context->viewport.map_visible.y2)
    {
      cursor_w = cursor_h =
	(utils_context->video_mode.width +
	 utils_context->video_mode.height) *
	lw6sys_math_heartbeat (mod_gl_utils_timer_get_uptime (utils_context),
			       flat_context->const_data.
			       cursor_heartbeat_period,
			       flat_context->const_data.cursor_size_min,
			       flat_context->const_data.cursor_size_max) *
	look->style.cursor_size;
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("display cursor %d %0.1f , %0.1f - %0.1f x %0.1f"), i,
		  cursor_x, cursor_y, cursor_w, cursor_h);
      _display_cursor_bitmap (utils_context, flat_context, cursor_x, cursor_y,
			      cursor_w, cursor_h,
			      even_odd ? flat_context->cursors_context.
			      cursor[i].bitmap_even : flat_context->
			      cursors_context.cursor[i].bitmap_odd);
    }
}

static void
_display_cursors (mod_gl_utils_context_t * utils_context,
		  _mod_gl_view_flat_context_t *
		  flat_context,
		  lw6gui_look_t * look, lw6ker_game_state_t * game_state,
		  lw6pil_local_cursors_t * local_cursors)
{
  int i;
  lw6ker_cursor_t *cursor;
  lw6ker_cursor_array_t cursor_array;
  lw6pil_local_cursor_t *local_cursor;
  int blink_state;

  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display cursors"));
  lw6ker_game_state_get_cursor_array (game_state, &cursor_array);
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      cursor = &(cursor_array.cursors[i]);
      if (cursor->enabled)
	{
	  local_cursor =
	    lw6pil_local_cursors_get_cursor (local_cursors,
					     cursor->cursor_id);
	  blink_state =
	    lw6sys_math_blink (mod_gl_utils_timer_get_uptime (utils_context),
			       flat_context->const_data.cursor_blink_period);
	  _display_cursor (utils_context, flat_context, look, local_cursor, i,
			   cursor->pos.x, cursor->pos.y, blink_state);
	}
    }
}

void
_mod_gl_view_flat_display_cursors (mod_gl_utils_context_t * utils_context,
				   _mod_gl_view_flat_context_t *
				   flat_context,
				   lw6gui_look_t * look,
				   lw6ker_game_state_t * game_state,
				   lw6pil_local_cursors_t * local_cursors)
{
  if (utils_context && flat_context && game_state)
    {
      _mod_gl_view_flat_cursors_context_update (utils_context,
						&flat_context->
						cursors_context,
						&flat_context->const_data,
						look, game_state);

      mod_gl_utils_set_render_mode_2d_blend (utils_context);

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl_view_flat_viewport_update (utils_context,
					 flat_context, look, game_state,
					 local_cursors);

      _display_cursors (utils_context, flat_context, look, game_state,
			local_cursors);
    }
}

void
mod_gl_view_flat_display_cursors (mod_gl_utils_context_t *
				  utils_context,
				  void *flat_context,
				  lw6gui_look_t * look,
				  lw6ker_game_state_t * game_state,
				  lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl_view_flat_display_cursors (utils_context,
				     (_mod_gl_view_flat_context_t *)
				     flat_context, look, game_state,
				     local_cursors);
}
