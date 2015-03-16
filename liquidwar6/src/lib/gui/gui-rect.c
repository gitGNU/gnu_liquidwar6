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

#include "gui.h"

/**
 * lw6gui_rect_init_x1y1x2y2
 *
 * @sys_context: global system context
 * @rect: the structure to initialize
 * @x1: x for top left corner
 * @y1: y for top left corner
 * @x2: x for bottom right corner
 * @y2: y for bottom right corner
 *
 * Initializes a rect structure, will calculate w & h.
 *
 * Return value: none.
 */
void
lw6gui_rect_init_x1y1x2y2 (lw6sys_context_t * sys_context, lw6gui_rect_t * rect, int x1, int y1, int x2, int y2)
{
  rect->x1 = x1;
  rect->y1 = y1;
  rect->x2 = x2;
  rect->y2 = y2;
  rect->w = x2 - x1;
  rect->h = y2 - y1;
}

/**
 * lw6gui_rect_init_xywh
 *
 * @sys_context: global system context
 * @rect: the structure to initialize
 * @x: x for top left corner
 * @y: y for top left corner
 * @w: width
 * @h: height
 *
 * Initializes a rect structure, will calculate x2 & y2.
 *
 * Return value: none.
 */
void
lw6gui_rect_init_xywh (lw6sys_context_t * sys_context, lw6gui_rect_t * rect, int x, int y, int w, int h)
{
  rect->x1 = x;
  rect->y1 = y;
  rect->w = w;
  rect->h = h;
  rect->x2 = x + w;
  rect->y2 = y + h;
}

/**
 * lw6gui_rect_clip
 *
 * @sys_context: global system context
 * @dst: the structure which will contain the result
 * @src: the source rect
 * @clip: the clipping rect (boundaries)
 *
 * Clips a rect (think of rectangle clips).
 *
 * Return value: none.
 */
void
lw6gui_rect_clip (lw6sys_context_t * sys_context, lw6gui_rect_t * dst, const lw6gui_rect_t * src, const lw6gui_rect_t * clip)
{
  int max_x1 = 0;
  int max_y1 = 0;
  int min_x2 = 0;
  int min_y2 = 0;

  max_x1 = lw6sys_imax (src->x1, clip->x1);
  max_y1 = lw6sys_imax (src->y1, clip->y1);
  min_x2 = lw6sys_imin (src->x2, clip->x2);
  min_y2 = lw6sys_imin (src->y2, clip->y2);

  lw6gui_rect_init_x1y1x2y2 (sys_context, dst, max_x1, max_y1, min_x2, min_y2);
}
