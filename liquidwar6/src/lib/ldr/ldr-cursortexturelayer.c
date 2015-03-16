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

#include "ldr.h"
#include "ldr-internal.h"

static int
_read_image (lw6sys_context_t * sys_context, lw6map_cursor_texture_layer_t * cursor_texture_layer, _lw6ldr_image_rgba_t * image)
{
  int ret = 0;

  if (image->w == LW6MAP_CURSOR_TEXTURE_SIZE && image->h == LW6MAP_CURSOR_TEXTURE_SIZE)
    {
      int row, col;
      lw6sys_color_8_t value;

      for (row = 0; row < LW6MAP_CURSOR_TEXTURE_SIZE; ++row)
	{
	  for (col = 0; col < LW6MAP_CURSOR_TEXTURE_SIZE; ++col)
	    {
	      value.r = image->data[row][col * 4];
	      value.g = image->data[row][col * 4 + 1];
	      value.b = image->data[row][col * 4 + 2];
	      value.a = image->data[row][col * 4 + 3];

	      lw6map_cursor_texture_layer_set (sys_context, cursor_texture_layer, col, row, value);
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("bad cursor texture layer size %dx%d, should be %dx%d"), image->w, image->h, LW6MAP_CURSOR_TEXTURE_SIZE, LW6MAP_CURSOR_TEXTURE_SIZE);
    }

  _lw6ldr_rgba_clear (sys_context, image);

  return ret;
}

int
_lw6ldr_cursor_texture_layer_read_png (lw6sys_context_t * sys_context, lw6map_cursor_texture_layer_t * cursor_texture_layer, const char *png_file)
{
  int ret = 0;
  _lw6ldr_image_rgba_t image;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading PNG cursor texture layer \"%s\""), png_file);

  memset (&image, 0, sizeof (_lw6ldr_image_rgba_t));

  if (_lw6ldr_rgba_read_png (sys_context, &image, png_file, NULL))
    {
      ret = _read_image (sys_context, cursor_texture_layer, &image);
    }

  return ret;
}
