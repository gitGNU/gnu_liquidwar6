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

static void
_set_map_rules (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context)
{
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

static void
_set_fighters_rules (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context)
{
  // glTexParameteri useless herem set in mod_gl1_utils_texture_update
  /*
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   */
}

/*
  static void
  _set_cursor_rules (mod_gl1_utils_context_t * utils_context,
  _mod_gl1_view_flat_context_t * flat_context)
  {
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
*/

static void
_display_bitmap_array (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		       _mod_gl1_view_flat_context_t * flat_context,
		       mod_gl1_utils_bitmap_array_t * bitmap_array, const lw6gui_look_t * look, int x_polarity, int y_polarity)
{
  float wave_length;
  float step_for_waves;
  float step_for_border_x;
  float step_for_border_y;
  float step_x = 0.0f;
  float step_y = 0.0f;
  float x, y;
  lw6gui_rect_t rect;
  int i;
  lw6gui_quad_t quad;
  lw6gui_quad_t source_quad;
  mod_gl1_utils_bitmap_t *bitmap = NULL;
  float map_x1, map_y1, map_x2, map_y2;
  float texture_x1, texture_y1, texture_x2, texture_y2;
  int use_waves = 0;
  int waves_cycle = 0;
  float waves_cycle_offset = 0.0f;
  float waves_scale_x = 0.0f;
  float waves_scale_y = 0.0f;
  mod_gl1_utils_bitmap_t *last_bitmap = NULL;

  if (bitmap_array->layout.source.w > 0 && bitmap_array->layout.source.h > 0 && flat_context->viewport.map_main.w > 0 && flat_context->viewport.map_main.h > 0)
    {
      /*
       * Now *before* we draw (generating textures between begin & end
       * isn't correct, we bind all textures, so that they are
       * generated if needed.
       */
      for (i = 0; i < bitmap_array->layout.nb_tiles; ++i)
	{
	  bitmap = mod_gl1_utils_bitmap_array_get (sys_context, bitmap_array, i);
	  mod_gl1_utils_bitmap_bind (sys_context, utils_context, bitmap);
	}

      mod_gl1_utils_display_quad_begin (sys_context, utils_context);
      /*
       * there's a *2 on the denom because we add w and h on the first
       * member, want we want is nb_waves on an average *per side*
       */
      wave_length = (flat_context->viewport.map_visible.w + flat_context->viewport.map_visible.h) / ((float) (flat_context->const_data.nb_waves * 2));
      use_waves = look->style.waves;
      if (use_waves)
	{
	  step_for_waves = wave_length / ((float) flat_context->const_data.vertices_per_wave);
	  waves_cycle = _lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context));
	  waves_cycle_offset = ((float) (waves_cycle)) / ((float) flat_context->const_data.waves_period);
	  waves_scale_x = (flat_context->const_data.waves_amplitude * wave_length * flat_context->viewport.map_shape.w) / flat_context->viewport.map_main.w;
	  waves_scale_y = (flat_context->const_data.waves_amplitude * wave_length * flat_context->viewport.map_shape.h) / flat_context->viewport.map_main.h;
	}
      else
	{
	  step_for_waves = (float) (flat_context->viewport.map_visible.w + flat_context->viewport.map_visible.h);
	}
      step_for_border_x = bitmap_array->layout.border_size * flat_context->viewport.map_main.w / ((float) bitmap_array->layout.source.w);
      step_for_border_y = bitmap_array->layout.border_size * flat_context->viewport.map_main.h / ((float) bitmap_array->layout.source.h);
      step_x = (step_for_waves < step_for_border_x) ? step_for_waves : step_for_border_x;
      step_y = (step_for_waves < step_for_border_y) ? step_for_waves : step_for_border_y;
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("step_for_waves=%f step_for_border_x=%f step_for_border_y=%f step_x=%f step_y=%f"),
		  step_for_waves, step_for_border_x, step_for_border_y, step_x, step_y);

      for (y = flat_context->viewport.map_visible.y1; y < flat_context->viewport.map_visible.y2 - step_y / 2.0f; y += step_y)
	{
	  for (x = flat_context->viewport.map_visible.x1; x < flat_context->viewport.map_visible.x2 - step_x / 2.0f; x += step_x)
	    {
	      lw6gui_viewport_screen_to_map (sys_context, &(flat_context->viewport), &map_x1, &map_y1, x, y, 0);
	      lw6gui_viewport_screen_to_map (sys_context, &(flat_context->viewport), &map_x2, &map_y2, x + step_x, y + step_y, 0);
	      if (use_waves)
		{
		  map_x1 += waves_scale_x * cos ((waves_cycle_offset + (x / wave_length)) * M_PI * 2.0f);
		  map_y1 += waves_scale_y * sin ((waves_cycle_offset + (y / wave_length)) * M_PI * 2.0f);
		  map_x2 += waves_scale_x * cos ((waves_cycle_offset + ((x + step_x) / wave_length)) * M_PI * 2.0f);
		  map_y2 += waves_scale_y * sin ((waves_cycle_offset + ((y + step_y) / wave_length)) * M_PI * 2.0f);
		}
	      map_x1 *= ((float) bitmap_array->layout.source.w) / ((float) flat_context->viewport.map_shape.w);
	      map_y1 *= ((float) bitmap_array->layout.source.h) / ((float) flat_context->viewport.map_shape.h);
	      map_x2 *= ((float) bitmap_array->layout.source.w) / ((float) flat_context->viewport.map_shape.w);
	      map_y2 *= ((float) bitmap_array->layout.source.h) / ((float) flat_context->viewport.map_shape.h);
	      source_quad.p1.x = map_x1;
	      source_quad.p1.y = map_y1;
	      source_quad.p2.x = map_x2;
	      source_quad.p2.y = map_y1;
	      source_quad.p3.x = map_x2;
	      source_quad.p3.y = map_y2;
	      source_quad.p4.x = map_x1;
	      source_quad.p4.y = map_y2;
	      if (lw6gui_rect_array_get_tile_and_quad (sys_context, &(bitmap_array->layout), &rect, &i, &quad, &source_quad, x_polarity, y_polarity))
		{
		  bitmap = mod_gl1_utils_bitmap_array_get (sys_context, bitmap_array, i);
		  texture_x1 = (quad.p1.x - ((float) rect.x1)) / ((float) bitmap_array->layout.tile_size);
		  texture_y1 = (quad.p1.y - ((float) rect.y1)) / ((float) bitmap_array->layout.tile_size);
		  texture_x2 = (quad.p3.x - ((float) rect.x1)) / ((float) bitmap_array->layout.tile_size);
		  texture_y2 = (quad.p3.y - ((float) rect.y1)) / ((float) bitmap_array->layout.tile_size);
		  if (last_bitmap == bitmap)
		    {
		      mod_gl1_utils_bitmap_bind_no_gen (sys_context, utils_context, bitmap);
		    }
		  else
		    {
		      mod_gl1_utils_display_quad_end (sys_context, utils_context);
		      mod_gl1_utils_bitmap_bind (sys_context, utils_context, bitmap);
		      last_bitmap = bitmap;
		      mod_gl1_utils_display_quad_begin (sys_context, utils_context);
		    }
		  if (bitmap)
		    {
		      mod_gl1_utils_display_quad_do (sys_context, utils_context,
						     bitmap->texture, x, y, x + step_x, y + step_y, texture_x1, texture_y1, texture_x2, texture_y2);
		    }
		}
	    }
	}
      mod_gl1_utils_display_quad_end (sys_context, utils_context);
    }
}

static void
_display_map_preview (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		      _mod_gl1_view_flat_context_t * flat_context, const lw6gui_look_t * look, const lw6map_level_t * level)
{
  _set_map_rules (sys_context, utils_context, flat_context);
  _display_bitmap_array (sys_context, utils_context, flat_context,
			 &(flat_context->game_context.map.map_bitmap_array), look, level->param.rules.x_polarity, level->param.rules.y_polarity);
}

void
_mod_gl1_view_flat_display_preview (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				    _mod_gl1_view_flat_context_t * flat_context, const lw6gui_look_t * look, const lw6map_level_t * level)
{
  if (utils_context && flat_context && level && level->texture.data)
    {
      _mod_gl1_view_flat_game_context_update_map (sys_context, utils_context, flat_context, &(flat_context->game_context.map), look, level);

      if (level->texture.has_alpha)
	{
	  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);
	}
      else
	{
	  mod_gl1_utils_set_render_mode_2d (sys_context, utils_context);
	}

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl1_view_flat_viewport_update_preview (sys_context, utils_context, flat_context, look, level);
      _display_map_preview (sys_context, utils_context, flat_context, look, level);
    }
}

void
mod_gl1_view_flat_display_preview (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *flat_context, const lw6gui_look_t * look,
				   const lw6map_level_t * level)
{
  _mod_gl1_view_flat_display_preview (sys_context, utils_context, (_mod_gl1_view_flat_context_t *) flat_context, look, level);
}

/*
 * Game_state is not passed as args but retrived
 * from flat_context struct, not to give the illusion that
 * any game_state could be displayed. One *needs* to call
 * game_context_init first.
 */
static void
_display_armies (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context,
		 const lw6gui_look_t * look)
{
  lw6sys_whd_t shape;
  const lw6ker_game_state_t *game_state;

  game_state = flat_context->game_context.armies.game_state;
  lw6ker_game_state_get_shape (sys_context, game_state, &shape);

  mod_gl1_utils_update_game_bitmap_array (sys_context, utils_context, &(flat_context->game_context.armies.armies_bitmap_array), game_state, look);
  _set_fighters_rules (sys_context, utils_context, flat_context);
  _display_bitmap_array (sys_context, utils_context, flat_context,
			 &(flat_context->game_context.armies.armies_bitmap_array), look,
			 game_state->game_struct->rules.x_polarity, game_state->game_struct->rules.y_polarity);
}

/*
 * Game_state is not passed as args but retrived
 * from flat_context struct, not to give the illusion that
 * any game_state could be displayed. One *needs* to call
 * game_context_init first.
 */
static void
_display_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
	      _mod_gl1_view_flat_context_t *
	      flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display map"));
  _display_map_preview (sys_context, utils_context, flat_context, look, game_state->game_struct->level);
}

void
_mod_gl1_view_flat_display_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				_mod_gl1_view_flat_context_t *
				flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  if (utils_context && flat_context && game_state)
    {
      lw6map_level_t *level;

      level = game_state->game_struct->level;

      _mod_gl1_view_flat_game_context_update (sys_context, utils_context, flat_context, &flat_context->game_context, look, level, game_state);

      if (level->texture.has_alpha)
	{
	  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);
	}
      else
	{
	  mod_gl1_utils_set_render_mode_2d (sys_context, utils_context);
	}

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl1_view_flat_viewport_update (sys_context, utils_context, flat_context, look, game_state, local_cursors);

      _display_map (sys_context, utils_context, flat_context, look, game_state, local_cursors);
    }
}

void
mod_gl1_view_flat_display_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			       void *flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl1_view_flat_display_map (sys_context, utils_context, (_mod_gl1_view_flat_context_t *) flat_context, look, game_state, local_cursors);
}

/*
 * Game_state is not passed as args but retrived
 * from flat_context struct, not to give the illusion that
 * any game_state could be displayed. One *needs* to call
 * game_context_init first.
 */
static void
_display_fighters (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		   _mod_gl1_view_flat_context_t *
		   flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display fighters"));
  _display_armies (sys_context, utils_context, flat_context, look);
}

void
_mod_gl1_view_flat_display_fighters (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				     _mod_gl1_view_flat_context_t *
				     flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  if (utils_context && flat_context && game_state)
    {
      lw6map_level_t *level;

      level = game_state->game_struct->level;

      _mod_gl1_view_flat_game_context_update (sys_context, utils_context, flat_context, &flat_context->game_context, look, level, game_state);

      mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl1_view_flat_viewport_update (sys_context, utils_context, flat_context, look, game_state, local_cursors);
      _display_fighters (sys_context, utils_context, flat_context, look, game_state, local_cursors);
    }
}

void
mod_gl1_view_flat_display_fighters (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				    void *flat_context,
				    const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl1_view_flat_display_fighters (sys_context, utils_context, (_mod_gl1_view_flat_context_t *) flat_context, look, game_state, local_cursors);
}

static void
_display_cursor_bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			_mod_gl1_view_flat_context_t * flat_context, float x, float y, float w, float h, mod_gl1_utils_bitmap_t * bitmap)
{
  mod_gl1_utils_bitmap_bind (sys_context, utils_context, bitmap);
  mod_gl1_utils_bitmap_display (sys_context, utils_context, bitmap, x - w / 2.0f, y - h / 2.0f, x + w / 2.0f, y + h / 2.0f);
}

static void
_display_cursor (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		 _mod_gl1_view_flat_context_t * flat_context,
		 const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursor_t * local_cursor, int i, int x, int y, int even_odd)
{

  float cursor_x = 0.0f;
  float cursor_y = 0.0f;
  float cursor_w = 0.0f;
  float cursor_h = 0.0f;
  float cursor_avg = 0.0f;
  float text_x1 = 0.0f;
  float text_y1 = 0.0f;
  float text_x2 = 0.0f;
  float text_y2 = 0.0f;
  float text_dw = 0.0f;
  float text_dh = 0.0f;

  if (local_cursor && local_cursor->is_main && local_cursor->mouse_controlled)
    {
      x = local_cursor->x;
      y = local_cursor->y;
    }

  lw6gui_viewport_map_to_screen (sys_context, &(flat_context->viewport), &cursor_x, &cursor_y, x, y, 0);

  if (local_cursor)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("cursor %x is local"), local_cursor->cursor_id);
      if (local_cursor->mouse_controlled)
	{
	  cursor_x = utils_context->sdl_context.input.mouse.screen_pointer.pos_x;
	  cursor_y = utils_context->sdl_context.input.mouse.screen_pointer.pos_y;
	}
      else
	{
	  lw6gui_viewport_map_to_screen (sys_context, &(flat_context->viewport), &cursor_x, &cursor_y, local_cursor->x, local_cursor->y, 0);
	}

      cursor_x = lw6sys_imax (cursor_x, flat_context->viewport.map_visible.x1);
      cursor_x = lw6sys_imin (cursor_x, flat_context->viewport.map_visible.x2);
      cursor_y = lw6sys_imax (cursor_y, flat_context->viewport.map_visible.y1);
      cursor_y = lw6sys_imin (cursor_y, flat_context->viewport.map_visible.y2);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("displaying non-local cursor"));
    }

  if (cursor_x >= flat_context->viewport.map_visible.x1 &&
      cursor_y >= flat_context->viewport.map_visible.y1 &&
      cursor_x <= flat_context->viewport.map_visible.x2 && cursor_y <= flat_context->viewport.map_visible.y2)
    {
      cursor_avg =
	(utils_context->sdl_context.video_mode.width +
	 utils_context->sdl_context.video_mode.height) * flat_context->const_data.cursor_size * look->style.cursor_size;
      if (lw6ker_game_state_get_charge_per1000 (sys_context, game_state, flat_context->cursors_context.cursor[i].team_color) >= 1000)
	{
	  cursor_w = cursor_h =
	    (utils_context->sdl_context.video_mode.width +
	     utils_context->sdl_context.video_mode.height) *
	    lw6sys_math_heartbeat (sys_context, _lw6gfx_sdl_timer_get_uptime
				   (sys_context, &(utils_context->sdl_context)),
				   flat_context->const_data.cursor_heartbeat_period,
				   flat_context->const_data.cursor_size_heartbeat_min,
				   flat_context->const_data.cursor_size_heartbeat_max) * look->style.cursor_size;
	}
      else
	{
	  cursor_w = cursor_h = cursor_avg;
	}
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display cursor %d %0.1f , %0.1f - %0.1f x %0.1f"), i, cursor_x, cursor_y, cursor_w, cursor_h);
      if (even_odd || !look->style.blink_cursor)
	{
	  _display_cursor_bitmap (sys_context, utils_context, flat_context, cursor_x, cursor_y, cursor_w, cursor_h,
				  flat_context->cursors_context.bitmap_fg_bg);
	  _display_cursor_bitmap (sys_context, utils_context, flat_context, cursor_x, cursor_y, cursor_w, cursor_h,
				  flat_context->cursors_context.cursor[i].bitmap_color);
	}
      if (flat_context->cursors_context.cursor[i].shaded_text_letter->texture_h > 0)
	{
	  text_y1 = cursor_y + (flat_context->const_data.cursor_relative_text_y1 - 0.5) * cursor_avg;
	  text_y2 = cursor_y + (flat_context->const_data.cursor_relative_text_y2 - 0.5) * cursor_avg;
	  text_x1 = cursor_x + (flat_context->const_data.cursor_relative_text_x1 - 0.5) * cursor_avg;
	  text_x2 =
	    text_x1 +
	    ((text_y2 -
	      text_y1) *
	     (flat_context->cursors_context.cursor[i].shaded_text_letter->texture_w)) / flat_context->cursors_context.cursor[i].shaded_text_letter->texture_h;
	  text_dw = flat_context->const_data.cursor_relative_text_dw * cursor_avg;
	  text_dh = flat_context->const_data.cursor_relative_text_dh * cursor_avg;

	  mod_gl1_utils_shaded_text_display (sys_context, utils_context,
					     flat_context->cursors_context.cursor[i].shaded_text_letter, text_x1, text_y1, text_x2, text_y2, text_dw, text_dh);
	}
    }
}

static void
_display_cursors (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		  _mod_gl1_view_flat_context_t *
		  flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  int i;
  lw6ker_cursor_t cursor;
  lw6pil_local_cursor_t *local_cursor;
  int blink_state;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display cursors"));
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      lw6ker_game_state_get_cursor_by_index (sys_context, game_state, &cursor, i);
      if (cursor.enabled)
	{
	  local_cursor = lw6pil_local_cursors_get_cursor (sys_context, local_cursors, cursor.cursor_id);
	  blink_state =
	    lw6sys_math_blink (sys_context, _lw6gfx_sdl_timer_get_uptime (sys_context, &(utils_context->sdl_context)),
			       flat_context->const_data.cursor_blink_period);
	  _display_cursor (sys_context, utils_context, flat_context, look, game_state, local_cursor, i, cursor.pos.x, cursor.pos.y, blink_state);
	}
    }
}

void
_mod_gl1_view_flat_display_cursors (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				    _mod_gl1_view_flat_context_t *
				    flat_context, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  if (utils_context && flat_context && game_state)
    {
      _mod_gl1_view_flat_cursors_context_update (sys_context, utils_context, &flat_context->cursors_context, &flat_context->const_data, look, game_state);

      mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      _mod_gl1_view_flat_viewport_update (sys_context, utils_context, flat_context, look, game_state, local_cursors);

      _display_cursors (sys_context, utils_context, flat_context, look, game_state, local_cursors);
    }
}

void
mod_gl1_view_flat_display_cursors (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				   utils_context,
				   void *flat_context,
				   const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl1_view_flat_display_cursors (sys_context, utils_context, (_mod_gl1_view_flat_context_t *) flat_context, look, game_state, local_cursors);
}
