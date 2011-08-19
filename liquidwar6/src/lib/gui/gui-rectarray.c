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

#include "gui.h"

int
lw6gui_rect_array_init (
			      lw6gui_rect_array_t * rect_array, int w,
			      int h, int tile_size, int border_size)
{
  int ret = 0;
  int nb_tiles_w, nb_tiles_h, n_x, n_y;
  int tile_spacing;

  tile_size = lw6gui_power_of_two_le (tile_size);
  tile_size = lw6sys_min (tile_size, utils_context->caps.max_texture_size);
  tile_size=lw6sys_min(tile_size,LW6GUI_MAX_TILE_SIZE);
  tile_size=lw6sys_max(tile_size,LW6GUI_MIN_TILE_SIZE);

  border_size=lw6sys_min(border_size,(tile_size/2)-1);
  border_size=lw6sys_min(border_size,LW6GUI_MAX_BORDER_SIZE);
  border_size=lw6sys_max(border_size,LW6GUI_MIN_BORDER_SIZE);

  rect_array->source.w=w;
  rect_array->source.h=h;
  rect_array->tile_size=tile_size;
  rect_array->border_size=border_size;

  tile_spacing = tile_size - 2*border_size;

  nb_tiles_w = ((w - 1) / tile_spacing) + 1;
  nb_tiles_h = ((h - 1) / tile_spacing) + 1;

  rect_array->tile_spacing=tile_spacing;
  rect_array->nb_tiles_w = nb_tiles_w;
  rect_array->nb_tiles_h = nb_tiles_h;
  rect_array->nb_tiles=(rect_array->nb_tiles_w*rect_array->nb_tiles_h);
  rect_array->min_x=-border_size;
  rect_array->min_y=-border_size;
  rect_array->max_x=rect_array->source.w+border_size;
  rect_array->max_y=rect_array->source.h+border_size;

  return ret;
}

int lw6gui_rect_get_tile_by_source_xy(lw6gui_rect_array_t *rect_array, lw6gui_rect_t *rect, int *i, int source_x, int source_y)
{
  int tile_x,tile_y;
  int ret=0;
  int x1,y1;

  if (source_x>=rect_array->min_x && source_x<rect_array->max_x && source_y>=rect_array->min_y && source_y<rect_array->max_y) {
    tile_x=source_x/rect_array->tile_spacing;
    tile_y=source_y/rect_array->tile_spacing;
    (*i)=tile_y*rect_array->nb_tiles_w+tile_x;
    x1=tile_x*rect_array->tile_spacing-border_size;
    y1=tile_y*rect_array->tile_spacing-border_size;
    lw6gui_rect_init_xywh(rect,x1,y1,rect_array->tile_size,rect_array->tile_size);
    ret=1;
  } else {
    lw6sys_log(LW6SYS_LOG_WARNING,_x_("pos %d,%d out of range"),x,y);
  }

  return ret;
}

int lw6gui_rect_get_tile_by_i(lw6gui_rect_array_t *rect_array, lw6gui_rect_t *rect, int i)
{
  int tile_x,tile_y;
  int ret=0;
  int x1,y1;

  if (i>=0 && i<rect_array->nb_tiles) {
    tile_y=i/rect_array->nb_tiles_w;
    tile_x=i%rect_array->nb_tiles_w;
    x1=tile_x*rect_array->tile_spacing-border_size;
    y1=tile_y*rect_array->tile_spacing-border_size;
    lw6gui_rect_init_xywh(rect,x1,y1,rect_array->tile_size,rect_array->tile_size);
    ret=1;
  } else {
    lw6sys_log(LW6SYS_LOG_WARNING,_x_("tile index %d out of range"),i);
  }

  return ret;
}

int lw6gui_rect_get_zone_for_source_zone(lw6gui_context_t *rect_array, int *i, lw6gui_zone_t *zone, lw6gui_zone_t *source_zone,int x_polarity, int y_polarity)
{
  float x_center;
  float y_center;
  float x_center_fixed;
  float y_center_fixed;
  float dx,dy;
  lw6gui_rect_t *rect;
  int ret=0;

  x_center_fixed=x_center=(source_zone->x1+source_zone->x2)/2;
  y_center_fixed=y_center=(source_zone->y1+source_zone->y2)/2;
  shape=
    lw6map_coords_fix_xy (rules,&(rect_array->source),&x_center_fixed,&y_center_fixed);
  
  if (lw6gui_rect_get_tile_by_source_xy( rect_array,&rect,i,x_center_fixed,y_center_fixed)) {
    dx=x_center_fixed-x_center;
    lw6gui_zone_init_x1y1x2y2(zone,source_zone->x1+,
  }
}

void
lw6gui_rect_array_clear (lw6gui_context_t *
			       utils_context,
			       lw6gui_rect_array_t * rect_array)
{
  memset (rect_array, 0, sizeof (lw6gui_rect_array_t));
}

int _get_nb_tiles_w(lw6gui_rect_array_t * rect_array)
{
  int ret=0;

re
}

