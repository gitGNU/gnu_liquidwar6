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
#include <math.h>

#include "map.h"

#define _TEXTURE_DD 1
#define _COLOR_DMIN 24
#define _COLOR_DMAX 28
#define _COLOR_FULL 192
#define _COLOR_NONE 0

/**
 * lw6map_cursor_texture_clear
 *
 * @sys_context: global system context
 * @cursor_texture: the cursor texture to clear
 *
 * Clears a cursor texture (set it all transparent).
 *
 * Return value: none
 */
void
lw6map_cursor_texture_clear (lw6sys_context_t * sys_context, lw6map_cursor_texture_t * cursor_texture)
{
  memset (cursor_texture, 0, sizeof (lw6map_cursor_texture_t));
}

/**
 * lw6map_cursor_texture_clear
 *
 * @sys_context: global system context
 * @cursor_texture: the cursor texture to clear
 *
 * Sets a cursor texture to the builtin defauts, that is a circle
 * that is black on the outside and gets white/transparent in the
 * middle.
 *
 * Return value: none
 */
void
lw6map_cursor_texture_builtin (lw6sys_context_t * sys_context, lw6map_cursor_texture_t * cursor_texture)
{
  int x, y, d;
  float c;
  lw6sys_color_8_t color = { 0, 0, 0, 0 };

  lw6map_cursor_texture_clear (sys_context, cursor_texture);

  c = (LW6MAP_CURSOR_TEXTURE_SIZE - 1) / 2;
  for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE; ++y)
    {
      for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE; ++x)
	{
	  d = sqrt ((y - c) * (y - c) + (x - c) * (x - c)) + _TEXTURE_DD;
	  if (d <= c)
	    {
	      color.a = (255 * d) / c;
	      color.r = color.g = color.b = 255 - color.a;
	    }
	  else
	    {
	      color.r = color.g = color.b = color.a = 0;
	    }
	  lw6map_cursor_texture_layer_set (sys_context, &(cursor_texture->fg_bg_layer), x, y, color);
	  color.a = (d >= _COLOR_DMIN && d <= _COLOR_DMAX) ? _COLOR_FULL : _COLOR_NONE;
	  color.r = color.g = color.b = 255;
	  lw6map_cursor_texture_layer_set (sys_context, &(cursor_texture->color_layer), x, y, color);
	}
    }
}
