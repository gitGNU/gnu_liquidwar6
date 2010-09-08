/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

mod_gl_utils_shaded_text_t *
mod_gl_utils_shaded_text_new (mod_gl_utils_context_t * utils_context,
			      TTF_Font * font, char *text,
			      lw6map_color_couple_t * color)
{
  mod_gl_utils_shaded_text_t *shaded_text;

  shaded_text =
    (mod_gl_utils_shaded_text_t *)
    LW6SYS_CALLOC (sizeof (mod_gl_utils_shaded_text_t));
  if (shaded_text)
    {
      shaded_text->font = font;
      if (!mod_gl_utils_shaded_text_update
	  (utils_context, shaded_text, text, color))
	{
	  LW6SYS_FREE (shaded_text);
	  shaded_text = NULL;
	}
    }
  return shaded_text;
}

int
mod_gl_utils_shaded_text_update (mod_gl_utils_context_t * utils_context,
				 mod_gl_utils_shaded_text_t * shaded_text,
				 char *text, lw6map_color_couple_t * color)
{
  int ret = 0;
  int change = 0;
  SDL_Color color_bg;
  SDL_Color color_fg;
  SDL_Surface *surface_bg = NULL;
  SDL_Surface *surface_fg = NULL;

  if ((!shaded_text->text)
      || ((shaded_text->text && strcmp (text, shaded_text->text) != 0)))
    {
      if (shaded_text->text)
	{
	  LW6SYS_FREE (shaded_text->text);
	}
      shaded_text->text = lw6sys_str_copy (text);
      change = (shaded_text->text != NULL);
    }
  if (!lw6map_color_is_same (color, &(shaded_text->color)))
    {
      shaded_text->color = *color;
      change = 1;
    }
  if (change && shaded_text->text)
    {
      if (shaded_text->bg)
	{
	  mod_gl_utils_bitmap_free (utils_context, shaded_text->bg);
	}
      if (shaded_text->fg)
	{
	  mod_gl_utils_bitmap_free (utils_context, shaded_text->fg);
	}
      color_bg = mod_gl_utils_color_8_to_sdl (shaded_text->color.bg);
      color_fg = mod_gl_utils_color_8_to_sdl (shaded_text->color.fg);
      surface_bg =
	mod_gl_utils_blended_text_surface (utils_context,
					   shaded_text->font,
					   color_bg, shaded_text->text);
      surface_fg =
	mod_gl_utils_blended_text_surface (utils_context,
					   shaded_text->font,
					   color_fg, shaded_text->text);
      if (surface_bg)
	{
	  shaded_text->bg =
	    mod_gl_utils_surface2bitmap (utils_context, surface_bg);
	  if (shaded_text->bg)
	    {
	      shaded_text->texture_w = surface_bg->w;
	      shaded_text->texture_h = surface_bg->h;
	    }
	  else
	    {
	      mod_gl_utils_delete_surface (utils_context, surface_bg);
	    }
	}
      if (surface_fg)
	{
	  shaded_text->fg =
	    mod_gl_utils_surface2bitmap (utils_context, surface_fg);
	  if (shaded_text->fg)
	    {
	      shaded_text->texture_w = surface_fg->w;
	      shaded_text->texture_h = surface_fg->h;
	    }
	  else
	    {
	      mod_gl_utils_delete_surface (utils_context, surface_fg);
	    }
	}
      ret = (shaded_text->bg && shaded_text->fg);
    }

  return ret;
}

int
mod_gl_utils_shaded_text_display (mod_gl_utils_context_t * utils_context,
				  mod_gl_utils_shaded_text_t * shaded_text,
				  float x1, float y1, float x2, float y2,
				  float dw, float dh)
{
  int ret = 1;

  if (shaded_text->bg)
    {
      ret = ret
	&& mod_gl_utils_bitmap_display (utils_context, shaded_text->bg,
					x1 + dw, y1 + dh, x2 + dw, y2 + dh);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't display, shaded_text->bg=NULL"));
      ret = 0;
    }
  if (shaded_text->fg)
    {
      ret = ret
	&& mod_gl_utils_bitmap_display (utils_context, shaded_text->fg, x1,
					y1, x2, y2);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't display, shaded_text->fg=NULL"));
      ret = 0;
    }

  return ret;
}

void
mod_gl_utils_shaded_text_free (mod_gl_utils_context_t * utils_context,
			       mod_gl_utils_shaded_text_t * shaded_text)
{
  if (shaded_text)
    {
      if (shaded_text->text)
	{
	  LW6SYS_FREE (shaded_text->text);
	}
      if (shaded_text->bg)
	{
	  mod_gl_utils_bitmap_free (utils_context, shaded_text->bg);
	}
      if (shaded_text->fg)
	{
	  mod_gl_utils_bitmap_free (utils_context, shaded_text->fg);
	}
      LW6SYS_FREE (shaded_text);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("trying to free NULL shaded text"));
    }
}
