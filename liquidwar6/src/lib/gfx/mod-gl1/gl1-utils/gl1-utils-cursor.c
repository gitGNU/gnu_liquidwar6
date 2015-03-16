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

#define _DESC_CURSOR_FG_BG "cursor_fg_bg"
#define _DESC_CURSOR_COLOR "cursor_color"

mod_gl1_utils_bitmap_t *
mod_gl1_utils_cursor_create_fg_bg (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
				   const lw6map_level_t * level)
{
  mod_gl1_utils_bitmap_t *bitmap = NULL;
  SDL_Surface *surface = NULL;
  int x, y;

  surface = mod_gl1_utils_create_surface (sys_context, utils_context, LW6MAP_CURSOR_TEXTURE_SIZE, LW6MAP_CURSOR_TEXTURE_SIZE);
  if (surface)
    {
      for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE; ++y)
	{
	  for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE; ++x)
	    {
	      mod_gl1_utils_putpixel (sys_context, surface, x, y,
				      lw6sys_color_8_to_irgba (lw6map_cursor_texture_layer_get (sys_context, &(level->cursor_texture.fg_bg_layer), x, y)));
	    }
	}
      bitmap = mod_gl1_utils_surface2bitmap (sys_context, utils_context, surface, _DESC_CURSOR_FG_BG);
      if (bitmap)
	{
	  mod_gl1_utils_bitmap_colorize (sys_context, utils_context, bitmap, look->style.colorize_cursor, &look->style.color_set.view_color_cursor);
	}
      else
	{
	  mod_gl1_utils_delete_surface (sys_context, utils_context, surface);
	  surface = NULL;
	}
    }

  return bitmap;
}

mod_gl1_utils_bitmap_t
  * mod_gl1_utils_cursor_create_color (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				       utils_context, const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_cursor_t * cursor)
{
  mod_gl1_utils_bitmap_t *bitmap = NULL;
  SDL_Surface *surface = NULL;
  int x, y;
  lw6map_color_couple_t color_couple;

  surface = mod_gl1_utils_create_surface (sys_context, utils_context, LW6MAP_CURSOR_TEXTURE_SIZE, LW6MAP_CURSOR_TEXTURE_SIZE);
  if (surface)
    {
      for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE; ++y)
	{
	  for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE; ++x)
	    {
	      mod_gl1_utils_putpixel (sys_context, surface, x, y,
				      lw6sys_color_8_to_irgba (lw6map_cursor_texture_layer_get (sys_context, &(level->cursor_texture.color_layer), x, y)));
	    }
	}
      bitmap = mod_gl1_utils_surface2bitmap (sys_context, utils_context, surface, _DESC_CURSOR_COLOR);
      if (bitmap)
	{
	  if (cursor->team_color >= 0)
	    {
	      color_couple.fg = look->style.color_set.team_colors[cursor->team_color];
	    }
	  else
	    {
	      color_couple.fg = look->style.color_set.team_color_dead;
	    }
	  color_couple.bg = look->style.color_set.team_color_dead;
	  mod_gl1_utils_bitmap_colorize (sys_context, utils_context, bitmap, 1, &color_couple);
	}
      else
	{
	  mod_gl1_utils_delete_surface (sys_context, utils_context, surface);
	  surface = NULL;
	}
    }

  return bitmap;
}
