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

#include <string.h>

#include "map.h"

/**
 * lw6map_cursor_texture_layer_set
 *
 * @sys_context: global system context
 * @cursor_texture_layer: the cursor texture_layer to change
 * @x: x coord
 * @y: y coord
 * @color: the color
 *
 * Sets a pixel in the cursor texture_layer.
 *
 * Return value: none
 */
void
lw6map_cursor_texture_layer_set (lw6sys_context_t * sys_context, lw6map_cursor_texture_layer_t * cursor_texture_layer, int x, int y, lw6sys_color_8_t color)
{
  cursor_texture_layer->data[y][x] = color;
}

/**
 * lw6map_cursor_texture_layer_get
 *
 * @sys_context: global system context
 * @cursor_texture_layer: the cursor texture_layer to query
 * @x: x coord
 * @y: y coord
 *
 * Gets a pixel in the cursor texture_layer.
 *
 * Return value: the color
 */
lw6sys_color_8_t
lw6map_cursor_texture_layer_get (lw6sys_context_t * sys_context, const lw6map_cursor_texture_layer_t * cursor_texture_layer, int x, int y)
{
  return cursor_texture_layer->data[y][x];
}
