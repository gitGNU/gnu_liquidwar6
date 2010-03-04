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

int
mod_gl_utils_bitmap_array_init (mod_gl_utils_context_t *
				utils_context,
				mod_gl_utils_bitmap_array_t * bitmap_array,
				int w, int h, int tile_size)
{
  int ret = 0;
  int n_x, n_y;
  mod_gl_utils_bitmap_t *bitmap;

  if (mod_gl_utils_rect_array_init
      (utils_context, &(bitmap_array->layout), w, h, tile_size))
    {
      bitmap_array->bitmaps =
	(mod_gl_utils_bitmap_t **) LW6SYS_CALLOC (bitmap_array->layout.n_w *
						  bitmap_array->layout.n_h *
						  sizeof
						  (mod_gl_utils_bitmap_t *));

      if (bitmap_array->bitmaps)
	{
	  ret = 1;
	  for (n_y = 0; n_y < bitmap_array->layout.n_h; ++n_y)
	    {
	      for (n_x = 0; n_x < bitmap_array->layout.n_w; ++n_x)
		{
		  bitmap =
		    mod_gl_utils_bitmap_new (utils_context,
					     bitmap_array->layout.w[n_x],
					     bitmap_array->layout.h[n_y]);
		  if (bitmap)
		    {
		      /*
		       * Call to refresh before the first update
		       * (conversion from surface to texture)
		       * happens when bitmap has never been displayed
		       * so it might still be 0.
		       */
		      mod_gl_utils_bitmap_refresh (utils_context, bitmap);
		      mod_gl_utils_bitmap_array_set (bitmap_array,
						     n_x, n_y, bitmap);
		    }
		  else
		    {
		      ret = 0;
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _
				  ("bitmap %d,%d of bitmap array couldn't be created, array is broken"),
				  n_x, n_y);
		    }
		}
	    }
	}
    }
  else
    {
      mod_gl_utils_rect_array_clear (utils_context, &(bitmap_array->layout));
      if (bitmap_array->bitmaps)
	{
	  LW6SYS_FREE (bitmap_array->bitmaps);
	  bitmap_array->bitmaps = NULL;
	}
    }

  return ret;
}

int
mod_gl_utils_bitmap_array_init_from_surface (mod_gl_utils_context_t *
					     utils_context,
					     mod_gl_utils_bitmap_array_t *
					     bitmap_array,
					     SDL_Surface * surface,
					     int tile_size)
{
  int ret = 0;
  int n_x, n_y;
  mod_gl_utils_bitmap_t *bitmap;
  SDL_Surface *sub_surface;
  SDL_Rect area;
  SDL_Rect sub_area;
  SDL_Rect border_area;
  SDL_Rect border_sub_area;

  if (mod_gl_utils_bitmap_array_init
      (utils_context, bitmap_array, surface->w, surface->h, tile_size))
    {
      ret = 1;
      for (n_y = 0; n_y < bitmap_array->layout.n_h; ++n_y)
	{
	  for (n_x = 0; n_x < bitmap_array->layout.n_w; ++n_x)
	    {
	      area.x = bitmap_array->layout.x0[n_x];
	      area.y = bitmap_array->layout.y0[n_y];
	      area.w = bitmap_array->layout.w[n_x];
	      area.h = bitmap_array->layout.h[n_y];

	      sub_area.x = 0;
	      sub_area.y = 0;
	      sub_area.w = bitmap_array->layout.w[n_x];
	      sub_area.h = bitmap_array->layout.h[n_y];

	      bitmap = mod_gl_utils_bitmap_array_get (bitmap_array, n_x, n_y);
	      if (bitmap)
		{
		  sub_surface = bitmap->surface;
		  if (sub_surface)
		    {
		      SDL_SetAlpha (surface, 0, SDL_ALPHA_OPAQUE);
		      SDL_SetAlpha (sub_surface, 0, SDL_ALPHA_OPAQUE);
		      mod_gl_utils_clear_surface (sub_surface);
		      SDL_BlitSurface (surface, &area, sub_surface,
				       &sub_area);
		      if (area.x == -1)
			{
			  border_area = area;
			  border_area.x = 0;
			  border_area.w = 1;
			  border_sub_area = sub_area;
			  border_sub_area.w = 1;
			  SDL_BlitSurface (surface, &border_area, sub_surface,
					   &border_sub_area);
			}
		      if (area.y == -1)
			{
			  border_area = area;
			  border_area.y = 0;
			  border_area.h = 1;
			  border_sub_area = sub_area;
			  border_sub_area.h = 1;
			  SDL_BlitSurface (surface, &border_area, sub_surface,
					   &border_sub_area);
			}
		      if (area.x + area.w == surface->w + 1)
			{
			  border_area = area;
			  border_area.x = surface->w - 1;
			  border_area.w = 1;
			  border_sub_area = sub_area;
			  border_sub_area.x = sub_area.w - 1;
			  border_sub_area.w = 1;
			}
		      if (area.y + area.h == surface->h + 1)
			{
			  border_area = area;
			  border_area.y = surface->h - 1;
			  border_area.h = 1;
			  border_sub_area = sub_area;
			  border_sub_area.y = sub_area.h - 1;
			  border_sub_area.h = 1;
			}
		      if (area.x < -1 || area.y < -1
			  || area.x + area.w > surface->w + 1
			  || area.y + area.h > surface->h + 1)
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _
				      ("unsupported ayout area.x=%d area.y=%d area.w=%d area.h=%d surface->w=%d surface->h=%d"),
				      area.x, area.y, area.w, area.h,
				      surface->w, surface->h);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _("sub_surface is NULL"));
		      ret = 0;
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING, _("bitmap is NULL"));
		  ret = 0;
		}
	    }
	}
    }

  return ret;
}

int
mod_gl_utils_bitmap_array_init_from_map (mod_gl_utils_context_t *
					 utils_context,
					 mod_gl_utils_bitmap_array_t *
					 bitmap_array,
					 lw6map_level_t * level,
					 int tile_size)
{
  int ret = 0;
  SDL_Surface *surface;

  surface = mod_gl_utils_map2surface (utils_context, level);
  if (surface)
    {
      ret = mod_gl_utils_bitmap_array_init_from_surface (utils_context,
							 bitmap_array,
							 surface, tile_size);
      mod_gl_utils_delete_surface (utils_context, surface);
    }

  return ret;
}

int
mod_gl_utils_bitmap_array_update (mod_gl_utils_context_t *
				  utils_context,
				  mod_gl_utils_bitmap_array_t *
				  bitmap_array, GLint wrap, GLint filter)
{
  int ret = 1;
  int n_x, n_y;
  mod_gl_utils_bitmap_t *bitmap;

  for (n_y = 0; n_y < bitmap_array->layout.n_h; ++n_y)
    {
      for (n_x = 0; n_x < bitmap_array->layout.n_w; ++n_x)
	{
	  bitmap = mod_gl_utils_bitmap_array_get (bitmap_array, n_x, n_y);
	  if (bitmap)
	    {
	      ret = mod_gl_utils_bitmap_set_wrap (utils_context, bitmap, wrap)
		&& ret;
	      ret =
		mod_gl_utils_bitmap_set_filter (utils_context, bitmap, filter)
		&& ret;
	      mod_gl_utils_texture_update (utils_context, bitmap->texture,
					   bitmap->surface);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _("NULL bitmap in bitmap_array at pos %d,%d"), n_x,
			  n_y);
	      ret = 0;
	    }
	}
    }

  return ret;
}

void
mod_gl_utils_bitmap_array_clear (mod_gl_utils_context_t * utils_context,
				 mod_gl_utils_bitmap_array_t * bitmap_array)
{
  int i;

  if (bitmap_array->bitmaps != NULL)
    {
      for (i = 0; i < bitmap_array->layout.n_w * bitmap_array->layout.n_h;
	   ++i)
	{
	  mod_gl_utils_bitmap_free (utils_context, bitmap_array->bitmaps[i]);
	}
      LW6SYS_FREE (bitmap_array->bitmaps);
      bitmap_array->bitmaps = NULL;
    }
  mod_gl_utils_rect_array_clear (utils_context, &(bitmap_array->layout));
}

/* 
 * This will actually delete the structure immediately and
 * not pospone its deletion as the name "schedule_delete"
 * could suggest. Still, the bitmaps themselves will remain
 * usable 'till next redraw.
 */
void
sdl_utils_schedule_delete_bitmap_array (mod_gl_utils_context_t *
					utils_context,
					mod_gl_utils_bitmap_array_t *
					bitmap_array)
{
  int i;

  if (bitmap_array->bitmaps != NULL)
    {
      for (i = 0; i < bitmap_array->layout.n_w * bitmap_array->layout.n_h;
	   ++i)
	{
	  mod_gl_utils_bitmap_free (utils_context, bitmap_array->bitmaps[i]);
	}
      LW6SYS_FREE (bitmap_array->bitmaps);
      bitmap_array->bitmaps = NULL;
    }
  mod_gl_utils_rect_array_clear (utils_context, &(bitmap_array->layout));
}

int
mod_gl_utils_bitmap_array_set (mod_gl_utils_bitmap_array_t *
			       bitmap_array, int n_x, int n_y,
			       mod_gl_utils_bitmap_t * bitmap)
{
  int ret = 0;

  if (n_x >= 0 && n_x < bitmap_array->layout.n_w && n_y >= 0
      && n_y < bitmap_array->layout.n_h)
    {
      bitmap_array->bitmaps[n_y * bitmap_array->layout.n_w + n_x] = bitmap;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("bitmap_array_set parameters out of range n_x=%d n_y=%d n_w=%d n_h=%d"),
		  n_x, n_y, bitmap_array->layout.n_w,
		  bitmap_array->layout.n_h);
    }

  return ret;
}

mod_gl_utils_bitmap_t *
mod_gl_utils_bitmap_array_get (mod_gl_utils_bitmap_array_t *
			       bitmap_array, int n_x, int n_y)
{
  mod_gl_utils_bitmap_t *ret = NULL;

  if (n_x >= 0 && n_x < bitmap_array->layout.n_w && n_y >= 0
      && n_y < bitmap_array->layout.n_h)
    {
      ret = bitmap_array->bitmaps[n_y * bitmap_array->layout.n_w + n_x];
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("bitmap_array_get parameters out of range n_x=%d n_y=%d n_w=%d n_h=%d"),
		  n_x, n_y, bitmap_array->layout.n_w,
		  bitmap_array->layout.n_h);
    }

  return ret;
}
