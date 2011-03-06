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

#include "../../gfx.h"
#include "gl-utils.h"

static void
_display_bitmap (mod_gl_utils_context_t * utils_context,
		 mod_gl_utils_bitmap_t * bitmap)
{
  mod_gl_utils_bitmap_display (utils_context, bitmap,
			       utils_context->smoothed.drawable.x1,
			       utils_context->smoothed.drawable.y1,
			       utils_context->smoothed.drawable.x2,
			       utils_context->smoothed.drawable.y2);
}

static void
display_map_zones (mod_gl_utils_context_t * utils_context,
		   lw6gui_look_t * look, lw6map_level_t * level,
		   lw6ker_map_struct_t * map_struct)
{
  mod_gl_utils_bitmap_t *bitmap = NULL;
  SDL_Surface *surface = NULL;

  surface = mod_gl_utils_create_zones_surface (utils_context, map_struct);
  if (surface)
    {
      bitmap = mod_gl_utils_surface2bitmap (utils_context, surface);

      if (bitmap)
	{
	  _display_bitmap (utils_context, bitmap);

	  mod_gl_utils_bitmap_free (utils_context, bitmap);
	}
      else
	{
	  mod_gl_utils_delete_surface (utils_context, surface);
	}
    }
}

void
mod_gl_utils_display_zones (mod_gl_utils_context_t * utils_context,
			    lw6gui_look_t * look,
			    lw6ker_game_struct_t * game_struct)
{
  if (utils_context && look && game_struct)
    {
      //_mod_gl_set_render_mode_2d (context);
      mod_gl_utils_set_render_mode_2d_blend (utils_context);

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      display_map_zones (utils_context, look, game_struct->level,
			 &(game_struct->map_struct));
    }
}

static void
display_map_gradient (mod_gl_utils_context_t * utils_context,
		      lw6gui_look_t * look,
		      lw6map_level_t * level,
		      lw6ker_map_state_t * map_state, int team_id,
		      int layer_id)
{
  mod_gl_utils_bitmap_t *bitmap = NULL;
  SDL_Surface *surface = NULL;

  surface =
    mod_gl_utils_create_gradient_surface (utils_context, map_state, team_id,
					  layer_id);
  if (surface)
    {
      bitmap = mod_gl_utils_surface2bitmap (utils_context, surface);

      if (bitmap)
	{
	  _display_bitmap (utils_context, bitmap);

	  mod_gl_utils_bitmap_free (utils_context, bitmap);
	}
      else
	{
	  mod_gl_utils_delete_surface (utils_context, surface);
	}
    }
}

void
mod_gl_utils_display_gradient (mod_gl_utils_context_t * utils_context,
			       lw6gui_look_t * look,
			       lw6ker_game_state_t * game_state,
			       int team_id, int layer_id)
{
  if (utils_context && game_state)
    {
      mod_gl_utils_set_render_mode_2d_blend (utils_context);

      glColor3f (1.0, 1.0, 1.0);
      glEnable (GL_TEXTURE_2D);	// for texture

      display_map_gradient (utils_context, look,
			    game_state->game_struct->level,
			    &(game_state->map_state), team_id, layer_id);
    }
}
