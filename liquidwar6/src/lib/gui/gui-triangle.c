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
 * lw6gui_triangle_is_inside_rect
 *
 * @sys_context: global system context
 * @triangle: triangle to test
 * @rect: rectangle in which triangle is supposed to be
 *
 * Tests wether a triangle is inside a rectangle, this is typically used to know
 * if a triangle is inside the right texture or if we're outside.
 *
 * Return value: 1 if OK, 0 if outside
 */
int
lw6gui_triangle_is_inside_rect (lw6sys_context_t * sys_context, const lw6gui_triangle_t * triangle, const lw6gui_rect_t * rect)
{
  return (lw6gui_point_is_inside_rect (sys_context, triangle->p1, rect)
	  && lw6gui_point_is_inside_rect (sys_context, triangle->p2, rect) && lw6gui_point_is_inside_rect (sys_context, triangle->p3, rect));
}
