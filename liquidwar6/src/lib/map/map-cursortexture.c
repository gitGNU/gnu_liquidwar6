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

#include <string.h>
#include <math.h>

#include "map.h"

#define _TEXTURE_DD 1
#define _COLOR_ALPHA_DMIN 24
#define _COLOR_ALPHA_DMAX 28
#define _COLOR_ALPHA_FULL 192
#define _COLOR_ALPHA_NONE 0

/**
 * lw6map_cursor_texture_set
 *
 * @cursor_texture: the cursor texture to change
 * @x: x coord
 * @y: y coord
 * @color: the color
 *
 * Sets a pixel in the cursor texture.
 *
 * Return value: none
 */
void
lw6map_cursor_texture_set (lw6map_cursor_texture_t * cursor_texture, int x,
			   int y, lw6sys_color_8_t color)
{
  cursor_texture->data[y][x] = color;
}

/**
 * lw6map_cursor_texture_get
 *
 * @cursor_texture: the cursor texture to query
 * @x: x coord
 * @y: y coord
 *
 * Gets a pixel in the cursor texture.
 *
 * Return value: the color
 */
lw6sys_color_8_t
lw6map_cursor_texture_get (lw6map_cursor_texture_t * cursor_texture, int x,
			   int y)
{
  return cursor_texture->data[y][x];
}

/**
 * lw6map_cursor_texture_set_color_alpha
 *
 * @cursor_texture: the cursor texture to change
 * @x: x coord
 * @y: y coord
 * @color_alpha: the alpha channel value
 *
 * Sets the color_alpha channel of the cursor texture, the color alpha channel
 * is set by the cursor-color.png file, it enables colorization with team color
 * at run-time.
 *
 * Return value: none.
 */
void
lw6map_cursor_texture_set_color_alpha (lw6map_cursor_texture_t *
				       cursor_texture, int x, int y,
				       u_int8_t color_alpha)
{
  cursor_texture->color_alpha_data[y][x] = color_alpha;
}

/**
 * lw6map_cursor_texture_get_color_alpha
 *
 * @cursor_texture: the cursor texture to query
 * @x: x coord
 * @y: y coord
 *
 * Gets the color_alpha channel of the cursor texture, the color alpha channel
 * is set by the cursor-color.png file, it enables colorization with team color
 * at run-time.
 *
 * Return value: the color
 */
u_int8_t
lw6map_cursor_texture_get_color_alpha (lw6map_cursor_texture_t *
				       cursor_texture, int x, int y)
{
  return cursor_texture->color_alpha_data[y][x];
}

/**
 * lw6map_cursor_texture_clear
 *
 * @cursor_texture: the cursor texture to clear
 *
 * Clears a cursor texture (set it all transparent).
 *
 * Return value: none
 */
void
lw6map_cursor_texture_clear (lw6map_cursor_texture_t * cursor_texture)
{
  memset (cursor_texture, 0, sizeof (lw6map_cursor_texture_t));
}

/**
 * lw6map_cursor_texture_clear
 *
 * @cursor_texture: the cursor texture to clear
 *
 * Sets a cursor texture to the builtin defauts, that is a circle
 * that is black on the outside and gets white/transparent in the
 * middle.
 *
 * Return value: none
 */
void
lw6map_cursor_texture_builtin (lw6map_cursor_texture_t * cursor_texture)
{
  int x, y, d;
  float c;
  lw6sys_color_8_t color = { 0, 0, 0, 0 };
  u_int8_t color_alpha = 0;

  lw6map_cursor_texture_clear (cursor_texture);

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
	  color_alpha = (d >= _COLOR_ALPHA_DMIN
			 && d <=
			 _COLOR_ALPHA_DMAX) ? _COLOR_ALPHA_FULL :
	    _COLOR_ALPHA_NONE;
	  lw6map_cursor_texture_set (cursor_texture, x, y, color);
	  lw6map_cursor_texture_set_color_alpha (cursor_texture, x, y,
						 color_alpha);
	}
    }
}
