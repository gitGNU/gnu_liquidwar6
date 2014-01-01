/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#include "gui.h"

int
lw6gui_rect_array_init (lw6gui_rect_array_t * rect_array, int w,
			int h, int tile_size, int border_size)
{
  int ret = 0;
  int nb_tiles_w, nb_tiles_h;
  int tile_spacing;

  memset (rect_array, 0, sizeof (lw6gui_rect_array_t));

  tile_size = lw6gui_power_of_two_le (tile_size);
  // tile_size = lw6sys_imin (tile_size, utils_context->caps.max_texture_size); to be reported in GL module
  tile_size = lw6sys_imin (tile_size, LW6GUI_MAX_TILE_SIZE);
  tile_size = lw6sys_imax (tile_size, LW6GUI_MIN_TILE_SIZE);

  border_size = lw6sys_imin (border_size, (tile_size / 2) - 1);
  border_size = lw6sys_imin (border_size, LW6GUI_MAX_BORDER_SIZE);
  border_size = lw6sys_imax (border_size, LW6GUI_MIN_BORDER_SIZE);

  rect_array->source.w = w;
  rect_array->source.h = h;
  rect_array->tile_size = tile_size;
  rect_array->border_size = border_size;

  tile_spacing = tile_size - 2 * border_size;

  nb_tiles_w = ((w - 1) / tile_spacing) + 1;
  nb_tiles_h = ((h - 1) / tile_spacing) + 1;

  rect_array->tile_spacing = tile_spacing;
  rect_array->nb_tiles_w = nb_tiles_w;
  rect_array->nb_tiles_h = nb_tiles_h;
  rect_array->nb_tiles = (rect_array->nb_tiles_w * rect_array->nb_tiles_h);
  lw6gui_rect_init_xywh (&(rect_array->limits), -rect_array->border_size,
			 -rect_array->border_size,
			 nb_tiles_w * tile_spacing + 2 * border_size,
			 nb_tiles_h * tile_spacing + 2 * border_size);

  ret = (rect_array->source.w > 0 && rect_array->source.h > 0
	 && rect_array->tile_size > 0 && rect_array->border_size > 0);

  return ret;
}

int
lw6gui_rect_array_get_tile_by_source_xy (lw6gui_rect_array_t * rect_array,
					 lw6gui_rect_t * rect, int *i,
					 int source_x, int source_y)
{
  int tile_x, tile_y;
  int ret = 0;
  int x1, y1;

  if (source_x >= rect_array->limits.x1 && source_x < rect_array->limits.x2
      && source_y >= rect_array->limits.y1
      && source_y < rect_array->limits.y2)
    {
      tile_x = source_x / rect_array->tile_spacing;
      tile_y = source_y / rect_array->tile_spacing;
      (*i) = tile_y * rect_array->nb_tiles_w + tile_x;
      x1 = tile_x * rect_array->tile_spacing - rect_array->border_size;
      y1 = tile_y * rect_array->tile_spacing - rect_array->border_size;
      lw6gui_rect_init_xywh (rect, x1, y1, rect_array->tile_size,
			     rect_array->tile_size);
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("pos %d,%d out of range"),
		  source_x, source_y);
    }

  return ret;
}

int
lw6gui_rect_array_get_tile_by_i (lw6gui_rect_array_t * rect_array,
				 lw6gui_rect_t * rect, int i)
{
  int tile_x, tile_y;
  int ret = 0;
  int x1, y1;

  if (i >= 0 && i < rect_array->nb_tiles)
    {
      tile_y = i / rect_array->nb_tiles_w;
      tile_x = i % rect_array->nb_tiles_w;
      x1 = tile_x * rect_array->tile_spacing - rect_array->border_size;
      y1 = tile_y * rect_array->tile_spacing - rect_array->border_size;
      lw6gui_rect_init_xywh (rect, x1, y1, rect_array->tile_size,
			     rect_array->tile_size);
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("tile index %d out of range"), i);
    }

  return ret;
}

int
lw6gui_rect_array_get_tile_and_quad (lw6gui_rect_array_t * rect_array,
				     lw6gui_rect_t * rect, int *i,
				     lw6gui_quad_t * quad,
				     lw6gui_quad_t * source_quad,
				     int x_polarity, int y_polarity)
{
  float x_center;
  float y_center;
  float x_center_fixed;
  float y_center_fixed;
  int x_flip = 0;
  int y_flip = 0;
  int ret = 0;

  x_center_fixed = x_center =
    (source_quad->p1.x + source_quad->p2.x + source_quad->p3.x +
     source_quad->p4.x) / 4.0f;
  y_center_fixed = y_center =
    (source_quad->p1.y + source_quad->p2.y + source_quad->p3.y +
     source_quad->p4.y) / 4.0f;

  lw6gui_coords_fix_xy_float (&x_center_fixed, &y_center_fixed, &x_flip,
			      &y_flip, rect_array->source.w,
			      rect_array->source.h, x_polarity, y_polarity);

  if (lw6gui_rect_array_get_tile_by_source_xy
      (rect_array, rect, i, x_center_fixed, y_center_fixed))
    {
      quad->p1.x =
	x_center_fixed + ((float) x_flip) * (source_quad->p1.x - x_center);
      quad->p2.x =
	x_center_fixed + ((float) x_flip) * (source_quad->p2.x - x_center);
      quad->p3.x =
	x_center_fixed + ((float) x_flip) * (source_quad->p3.x - x_center);
      quad->p4.x =
	x_center_fixed + ((float) x_flip) * (source_quad->p4.x - x_center);
      quad->p1.y =
	y_center_fixed + ((float) y_flip) * (source_quad->p1.y - y_center);
      quad->p2.y =
	y_center_fixed + ((float) y_flip) * (source_quad->p2.y - y_center);
      quad->p3.y =
	y_center_fixed + ((float) y_flip) * (source_quad->p3.y - y_center);
      quad->p4.y =
	y_center_fixed + ((float) y_flip) * (source_quad->p4.y - y_center);
      if (lw6gui_quad_is_inside_rect (quad, rect))
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("quad is outside rect (%f,%f),(%f,%f),(%f,%f),(%f,%f) vs (%d,%d),(%d,%d) source was (%f,%f),(%f,%f),(%f,%f),(%f,%f)"),
		      quad->p1.x, quad->p1.y, quad->p2.x, quad->p2.y,
		      quad->p3.x, quad->p3.y, quad->p4.x, quad->p4.y,
		      rect->x1, rect->y1, rect->x2, rect->y2,
		      source_quad->p1.x, source_quad->p1.y, source_quad->p2.x,
		      source_quad->p2.y, source_quad->p3.x, source_quad->p3.y,
		      source_quad->p4.x, source_quad->p4.y);
	}
    }
  else
    {
      memset (quad, 0, sizeof (lw6gui_quad_t));
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("couldn't find tile in rect_array of %dx%d for quad (%f,%f),(%f,%f),(%f,%f),(%f,%f)"),
		  rect_array->source.w, rect_array->source.h,
		  source_quad->p1.x, source_quad->p1.y, source_quad->p2.x,
		  source_quad->p2.y, source_quad->p3.x, source_quad->p3.y,
		  source_quad->p4.x, source_quad->p4.y);
    }

  return ret;
}
