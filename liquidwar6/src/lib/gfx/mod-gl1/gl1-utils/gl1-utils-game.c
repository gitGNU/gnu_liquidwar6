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

#include "../../gfx.h"
#include "gl1-utils.h"

/*
 * This is where the game map with actual information "there are
 * fighters here" is done.
 */
static void
_update_game_surface_raw (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			  SDL_Surface * surface,
			  const lw6ker_game_state_t * game_state, const lw6map_color_couple_t * map_color, int invert_y, int x0, int y0, int w, int h)
{
  int x, y, x1, y1, ry;
  int fighter_id;
  const lw6ker_fighter_t *fighter;
  Uint32 color;
  Uint32 bg;
  Uint32 fg;
  int team_color;
  lw6map_rules_t *rules = NULL;
  lw6sys_whd_t shape = { 0, 0, 0 };
  int bytes_per_pixel;

  lw6ker_game_state_get_shape (sys_context, game_state, &shape);
  rules = &(game_state->game_struct->rules);

  x1 = x0 + w - 1;
  y1 = y0 + h - 1;
  lw6map_coords_fix_xy (rules, &shape, &x0, &y0);
  lw6map_coords_fix_xy (rules, &shape, &x1, &y1);
  x1++;
  y1++;
  bg = lw6sys_color_8_to_irgba (map_color->bg);
  fg = lw6sys_color_8_to_irgba (map_color->fg);
  bytes_per_pixel = surface->format->BytesPerPixel;
  if (x1 - x0 <= surface->w && y1 - y0 <= surface->h)
    {
      for (y = y0; y < y1; ++y)
	{
	  ry = invert_y ? y1 - (y - y0) - 1 : y;
	  for (x = x0; x < x1; ++x)
	    {
	      fighter_id = -1;
	      fighter_id = lw6ker_game_state_get_fighter_id (sys_context, game_state, x, ry, 0);
	      if (fighter_id >= 0)
		{
		  fighter = lw6ker_game_state_get_fighter_ro_by_id (sys_context, game_state, fighter_id);
		  team_color = fighter->team_color;
		  if (team_color >= 0)
		    {
		      /*
		       * We need to cache & check team_color for in dirty read
		       * mode, team_color could be negative even if fighter was
		       * reported active just before.
		       */
		      color = mod_gl1_utils_get_shaded_color_for_fighter (utils_context, team_color, fighter->health);
		    }
		}
	      else
		{
		  if (lw6ker_game_struct_is_bg (sys_context, game_state->game_struct, x, ry, 0))
		    {
		      color = bg;
		    }
		  else
		    {
		      color = fg;
		    }
		}
	      if (bytes_per_pixel == 4)
		{
		  mod_gl1_utils_putpixel_4_bytes_per_pixel (surface, x - x0, y - y0, color);
		}
	      else
		{
		  mod_gl1_utils_putpixel (sys_context, surface, x - x0, y - y0, color);
		}
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("wrong surface dimension x0=%d x1=%d surface->w=%d y0=%d y1=%d surface->h=%d"), x0, x1, surface->w, y0, y1, surface->h);
    }
}

static void
_update_game_bitmap_raw (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			 mod_gl1_utils_bitmap_t * bitmap,
			 const lw6ker_game_state_t * game_state, const lw6map_color_couple_t * map_color, int invert_y, int x0, int y0, int w, int h)
{
  _update_game_surface_raw (sys_context, utils_context, bitmap->surface, game_state, map_color, invert_y, x0, y0, w, h);
}

void
mod_gl1_utils_update_game_bitmap_raw (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				      mod_gl1_utils_bitmap_t * bitmap,
				      const lw6ker_game_state_t * game_state, const lw6map_color_couple_t * map_color, int invert_y)
{
  if (bitmap->surface)
    {
      _update_game_bitmap_raw (sys_context, utils_context, bitmap, game_state, map_color,
			       invert_y, 0, 0, lw6ker_game_state_get_w (sys_context, game_state), lw6ker_game_state_get_h (sys_context, game_state));
      if (bitmap->texture)
	{
	  /*
	   * Using this instead of creating a new texture from a surface using
	   * glTexImage2D (surface2texture) is important, for performance.
	   * This is called at each display, we don't want a new texture to
	   * be created each time: it's always the same size and represents
	   * pretty much the same thing/layer -> where fighters are!
	   */
	  mod_gl1_utils_texture_update (sys_context, utils_context, bitmap->texture, bitmap->surface);
	}
    }
}

/*
 * This is where the game map with actual information "there are
 * fighters here" is done.
 */
static void
_update_game_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		      SDL_Surface * surface, const lw6ker_game_state_t * game_state, const lw6gui_look_t * look, int x0, int y0, int w, int h)
{
  int x, y, z, x1, y1, d;
  int safe_x = 0, safe_y = 0;
  int fighter_id, hidden;
  const lw6ker_fighter_t *fighter;
  Uint32 color;
  float hidden_layer_alpha;
  int team_color;
  lw6map_rules_t *rules = NULL;
  lw6sys_whd_t shape = { 0, 0, 0 };
  int bytes_per_pixel;

  lw6ker_game_state_get_shape (sys_context, game_state, &shape);
  rules = &(game_state->game_struct->rules);

  hidden_layer_alpha = look->style.hidden_layer_alpha;
  x1 = x0 + w;
  y1 = y0 + h;
  d = shape.d;
  bytes_per_pixel = surface->format->BytesPerPixel;
  if (x1 - x0 <= surface->w && y1 - y0 <= surface->h)
    {
      for (y = y0; y < y1; ++y)
	{
	  for (x = x0; x < x1; ++x)
	    {
	      safe_x = x;
	      safe_y = y;
	      lw6map_coords_fix_xy (rules, &shape, &safe_x, &safe_y);
	      fighter_id = -1;
	      hidden = 0;
	      for (z = 0; z < d && fighter_id < 0; ++z)
		{
		  fighter_id = lw6ker_game_state_get_fighter_id (sys_context, game_state, safe_x, safe_y, z);
		  /*
		   * It's important to test d>1 else a call
		   * to get_zone_id is done when it's not needed,
		   * and this function does cost CPU. When d==1 hidden
		   * just makes no sense.
		   */
		  if (d > 1 && fighter_id < 0)
		    {
		      hidden = lw6ker_game_struct_is_fg (sys_context, game_state->game_struct, safe_x, safe_y, z);
		    }
		}
	      if (fighter_id >= 0)
		{
		  fighter = lw6ker_game_state_get_fighter_ro_by_id (sys_context, game_state, fighter_id);
		  team_color = fighter->team_color;
		  if (team_color >= 0)
		    {
		      /*
		       * We need to cache & check team_color for in dirty read
		       * mode, team_color could be negative even if fighter was
		       * reported active just before.
		       */
		      color = mod_gl1_utils_get_shaded_color_for_fighter (utils_context, team_color, fighter->health);
		      if (hidden)
			{
			  mod_gl1_utils_force_color32_alpha (sys_context, &color, hidden_layer_alpha);
			}
		    }
		  if (bytes_per_pixel == 4)
		    {
		      mod_gl1_utils_putpixel_4_bytes_per_pixel (surface, x - x0, y - y0, color);
		    }
		  else
		    {
		      mod_gl1_utils_putpixel (sys_context, surface, x - x0, y - y0, color);
		    }
		}
	      else
		{
		  if (bytes_per_pixel == 4)
		    {
		      mod_gl1_utils_putpixel_4_bytes_per_pixel (surface, x - x0, y - y0, 0);
		    }
		  else
		    {
		      mod_gl1_utils_putpixel (sys_context, surface, x - x0, y - y0, 0);
		    }
		}
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("wrong surface dimension x0=%d x1=%d surface->w=%d y0=%d y1=%d surface->h=%d"), x0, x1, surface->w, y0, y1, surface->h);
    }
}

static void
_update_game_bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		     mod_gl1_utils_bitmap_t * bitmap, const lw6ker_game_state_t * game_state, const lw6gui_look_t * look, int x0, int y0, int w, int h)
{
  _update_game_surface (sys_context, utils_context, bitmap->surface, game_state, look, x0, y0, w, h);
}

void
mod_gl1_utils_update_game_bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				  mod_gl1_utils_bitmap_t * bitmap, const lw6ker_game_state_t * game_state, const lw6gui_look_t * look)
{
  if (bitmap->surface)
    {
      _update_game_bitmap (sys_context, utils_context, bitmap, game_state, look,
			   0, 0, lw6ker_game_state_get_w (sys_context, game_state), lw6ker_game_state_get_h (sys_context, game_state));
      if (bitmap->texture)
	{
	  /*
	   * Using this instead of creating a new texture from a surface using
	   * glTexImage2D (surface2texture) is important, for performance.
	   * This is called at each display, we don't want a new texture to
	   * be created each time: it's always the same size and represents
	   * pretty much the same thing/layer -> where fighters are!
	   */
	  mod_gl1_utils_texture_update (sys_context, utils_context, bitmap->texture, bitmap->surface);
	}
    }
}

void
mod_gl1_utils_update_game_bitmap_array (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					utils_context,
					mod_gl1_utils_bitmap_array_t * bitmap_array, const lw6ker_game_state_t * game_state, const lw6gui_look_t * look)
{
  int i;
  GLint wrap = 0;
  GLint filter = 0;
  mod_gl1_utils_bitmap_t *bitmap = NULL;

  if (utils_context->last_action.game_bitmap_array_update_id !=
      game_state->id || utils_context->last_action.game_bitmap_array_update_rounds != lw6ker_game_state_get_rounds (sys_context, game_state))
    {
      wrap = GL_CLAMP_TO_EDGE;
      if (look->style.pixelize)
	{
	  filter = GL_NEAREST;
	}
      else
	{
	  filter = GL_LINEAR;
	}

      utils_context->last_action.game_bitmap_array_update_id = game_state->id;
      utils_context->last_action.game_bitmap_array_update_rounds = lw6ker_game_state_get_rounds (sys_context, game_state);

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("updating %d game tiles"), bitmap_array->layout.nb_tiles);
      /*
       * Typically parallelizable function. Doesn't matter how tiles are updated,
       * and in which order.
       */
#ifdef LW6_OPENMP
#pragma omp parallel for
#endif // LW6_OPENMP
      for (i = 0; i < bitmap_array->layout.nb_tiles; ++i)
	{
	  /*
	   * rect defined locally inside the loop because of
	   * parallelizing, each thread must have a copy.
	   */
	  lw6gui_rect_t rect;

	  if (lw6gui_rect_array_get_tile_by_i (sys_context, &(bitmap_array->layout), &rect, i))
	    {
	      bitmap = mod_gl1_utils_bitmap_array_get (sys_context, bitmap_array, i);
	      if (bitmap)
		{
		  _update_game_bitmap (sys_context, utils_context, bitmap, game_state, look, rect.x1, rect.y1, rect.w, rect.h);
		}
	    }
	}

      /*
       * Now render from surfaces to textures.
       */
      mod_gl1_utils_bitmap_array_update (sys_context, utils_context, bitmap_array, wrap, filter);
    }
  else
    {
      // same as before, no need to update
    }
}
