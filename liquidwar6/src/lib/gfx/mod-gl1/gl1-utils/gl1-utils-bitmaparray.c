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

#define _BITMAP_ARRAY_DESC "bitmap_array"

int
mod_gl1_utils_bitmap_array_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				 utils_context, mod_gl1_utils_bitmap_array_t * bitmap_array, int w, int h, int tile_size, int border_size)
{
  int ret = 0;
  int i;
  mod_gl1_utils_bitmap_t *bitmap;

  tile_size = lw6sys_imin (tile_size, utils_context->caps.max_texture_size);
  if (lw6gui_rect_array_init (sys_context, &(bitmap_array->layout), w, h, tile_size, border_size))
    {
      bitmap_array->bitmaps = (mod_gl1_utils_bitmap_t **) LW6SYS_CALLOC (sys_context, bitmap_array->layout.nb_tiles * sizeof (mod_gl1_utils_bitmap_t *));

      if (bitmap_array->bitmaps)
	{
	  ret = 1;
	  for (i = 0; i < bitmap_array->layout.nb_tiles; ++i)
	    {
	      bitmap =
		mod_gl1_utils_bitmap_new (sys_context, utils_context, bitmap_array->layout.tile_size, bitmap_array->layout.tile_size, _BITMAP_ARRAY_DESC);
	      if (bitmap)
		{
		  /*
		   * Call to refresh before the first update
		   * (conversion from surface to texture)
		   * happens when bitmap has never been displayed
		   * so it might still be 0.
		   */
		  mod_gl1_utils_bitmap_refresh (sys_context, utils_context, bitmap);
		  mod_gl1_utils_bitmap_array_set (sys_context, bitmap_array, i, bitmap);
		}
	      else
		{
		  ret = 0;
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bitmap %d of bitmap array couldn't be created, array is broken"), i);
		}
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init rect_array"));
      if (bitmap_array->bitmaps)
	{
	  LW6SYS_FREE (sys_context, bitmap_array->bitmaps);
	  bitmap_array->bitmaps = NULL;
	}
    }

  return ret;
}

int
mod_gl1_utils_bitmap_array_init_from_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					      utils_context,
					      mod_gl1_utils_bitmap_array_t *
					      bitmap_array, SDL_Surface * surface, int tile_size, int border_size, int x_polarity, int y_polarity)
{
  int ret = 0;
  int i;
  int x, y;
  mod_gl1_utils_bitmap_t *bitmap;
  SDL_Surface *sub_surface;
  lw6gui_rect_t rect;
  lw6map_rules_t rules;
  lw6sys_whd_t shape;
  int x_get, y_get, x_put, y_put;

  if (mod_gl1_utils_bitmap_array_init (sys_context, utils_context, bitmap_array, surface->w, surface->h, tile_size, border_size))
    {
      ret = 1;
      lw6map_rules_defaults (sys_context, &rules);
      rules.x_polarity = x_polarity;
      rules.y_polarity = y_polarity;
      shape.w = surface->w;
      shape.h = surface->h;
      for (i = 0; i < bitmap_array->layout.nb_tiles; ++i)
	{
	  bitmap = mod_gl1_utils_bitmap_array_get (sys_context, bitmap_array, i);
	  if (bitmap)
	    {
	      sub_surface = bitmap->surface;
	      if (sub_surface)
		{
		  SDL_SetAlpha (surface, 0, SDL_ALPHA_OPAQUE);
		  SDL_SetAlpha (sub_surface, 0, SDL_ALPHA_OPAQUE);
		  mod_gl1_utils_clear_surface (sys_context, sub_surface);
		  if (lw6gui_rect_array_get_tile_by_i (sys_context, &(bitmap_array->layout), &rect, i))
		    {
		      /*
		       * Previous version of the code used blits but it's really
		       * annoying to handle with all the wrap/polarity stuff,
		       * someday could rewrite this to use blits in the middle and
		       * this ugly get/put only on the edges
		       */
		      for (y = rect.y1; y < rect.y2; ++y)
			{
			  for (x = rect.x1; x < rect.x2; ++x)
			    {
			      x_get = x;
			      y_get = y;
			      lw6map_coords_fix_xy (&rules, &shape, &x_get, &y_get);
			      x_put = x - rect.x1;
			      y_put = y - rect.y1;
			      mod_gl1_utils_putpixel (sys_context, sub_surface, x_put, y_put, mod_gl1_utils_getpixel (sys_context, surface, x_get, y_get));
			    }
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistent layout"));
		      ret = 0;
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("sub_surface is NULL"));
		  ret = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bitmap is NULL"));
	      ret = 0;
	    }
	}
    }

  return ret;
}

int
mod_gl1_utils_bitmap_array_init_from_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					  utils_context,
					  mod_gl1_utils_bitmap_array_t *
					  bitmap_array, const lw6map_level_t * level, int tile_size, int border_size, int x_polarity, int y_polarity)
{
  int ret = 0;
  SDL_Surface *surface;

  surface = mod_gl1_utils_map2surface (sys_context, utils_context, level);
  if (surface)
    {
      ret = mod_gl1_utils_bitmap_array_init_from_surface (sys_context, utils_context, bitmap_array, surface, tile_size, border_size, x_polarity, y_polarity);
      mod_gl1_utils_delete_surface (sys_context, utils_context, surface);
    }

  return ret;
}

int
mod_gl1_utils_bitmap_array_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_array_t * bitmap_array,
				   GLint wrap, GLint filter)
{
  int ret = 1;
  int i;
  mod_gl1_utils_bitmap_t *bitmap;

  for (i = 0; i < bitmap_array->layout.nb_tiles; ++i)
    {
      bitmap = mod_gl1_utils_bitmap_array_get (sys_context, bitmap_array, i);
      if (bitmap)
	{
	  ret = mod_gl1_utils_bitmap_set_wrap (sys_context, utils_context, bitmap, wrap) && ret;
	  ret = mod_gl1_utils_bitmap_set_filter (sys_context, utils_context, bitmap, filter) && ret;
	  mod_gl1_utils_texture_update (sys_context, utils_context, bitmap->texture, bitmap->surface);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("NULL bitmap in bitmap_array at index %d"), i);
	  ret = 0;
	}
    }

  return ret;
}

void
mod_gl1_utils_bitmap_array_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_array_t * bitmap_array)
{
  int i;

  if (bitmap_array->bitmaps != NULL)
    {
      for (i = 0; i < bitmap_array->layout.nb_tiles; ++i)
	{
	  mod_gl1_utils_bitmap_free (sys_context, utils_context, bitmap_array->bitmaps[i]);
	}
      LW6SYS_FREE (sys_context, bitmap_array->bitmaps);
      bitmap_array->bitmaps = NULL;
    }
}

/*
 * This will actually delete the structure immediately and
 * not pospone its deletion as the name "schedule_delete"
 * could suggest. Still, the bitmaps themselves will remain
 * usable 'till next redraw.
 */
void
mod_gl1_utils_schedule_delete_bitmap_array (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					    mod_gl1_utils_bitmap_array_t * bitmap_array)
{
  int i;

  if (bitmap_array->bitmaps != NULL)
    {
      for (i = 0; i < bitmap_array->layout.nb_tiles; ++i)
	{
	  mod_gl1_utils_bitmap_free (sys_context, utils_context, bitmap_array->bitmaps[i]);
	}
      LW6SYS_FREE (sys_context, bitmap_array->bitmaps);
      bitmap_array->bitmaps = NULL;
    }
}

int
mod_gl1_utils_bitmap_array_set (lw6sys_context_t * sys_context, mod_gl1_utils_bitmap_array_t * bitmap_array, int i, mod_gl1_utils_bitmap_t * bitmap)
{
  int ret = 0;

  if (i >= 0 && i < bitmap_array->layout.nb_tiles)
    {
      bitmap_array->bitmaps[i] = bitmap;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bitmap_array_set parameters out of range i=%d nb_tiles=%d"), i, bitmap_array->layout.nb_tiles);
    }

  return ret;
}

mod_gl1_utils_bitmap_t *
mod_gl1_utils_bitmap_array_get (lw6sys_context_t * sys_context, mod_gl1_utils_bitmap_array_t * bitmap_array, int i)
{
  mod_gl1_utils_bitmap_t *ret = NULL;

  if (i >= 0 && i < bitmap_array->layout.nb_tiles)
    {
      ret = bitmap_array->bitmaps[i];
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bitmap_array_get parameters out of range i=%d nb_tiles=%d"), i, bitmap_array->layout.nb_tiles);
      /*
       * By default we still return something, display might be a little
       * inconsistent but this is not as bad as a segfault. BTW, most of the
       * time the problem is an out of range due to rounding errors on the edges
       * so what is to be drawn is usually very small.
       */
      if (bitmap_array->layout.nb_tiles > 0)
	{
	  ret = bitmap_array->bitmaps[0];
	}
    }

  return ret;
}
