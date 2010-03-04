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
mod_gl_utils_rect_array_init (mod_gl_utils_context_t *
			      utils_context,
			      mod_gl_utils_rect_array_t * rect_array, int w,
			      int h, int tile_size)
{
  int ret = 0;
  int n_w, n_h, n_x, n_y;
  int tile_spacing;

  tile_size = mod_gl_utils_power_of_two_le (tile_size);
  tile_size = lw6sys_min (tile_size, utils_context->caps.max_texture_size);
  tile_size = lw6sys_max (tile_size, 3);
  tile_spacing = tile_size - 2;

  n_w = ((w - 1) / tile_spacing) + 1;
  n_h = ((h - 1) / tile_spacing) + 1;

  rect_array->n_w = n_w;
  rect_array->n_h = n_h;
  rect_array->x0 = (int *) LW6SYS_CALLOC (n_w * sizeof (int));
  rect_array->y0 = (int *) LW6SYS_CALLOC (n_h * sizeof (int));
  rect_array->w = (int *) LW6SYS_CALLOC (n_w * sizeof (int));
  rect_array->h = (int *) LW6SYS_CALLOC (n_h * sizeof (int));
  rect_array->screen_x = (float *) LW6SYS_CALLOC ((n_w + 1) * sizeof (float));
  rect_array->screen_y = (float *) LW6SYS_CALLOC ((n_h + 1) * sizeof (float));

  if (rect_array->x0 && rect_array->y0 && rect_array->w && rect_array->h
      && rect_array->screen_x && rect_array->screen_y && n_w > 0 && n_h > 0)
    {
      rect_array->x0[0] = -1;
      for (n_x = 0; n_x < n_w - 1; ++n_x)
	{
	  rect_array->w[n_x] = tile_size;
	  rect_array->x0[n_x + 1] = rect_array->x0[n_x] + tile_spacing;
	}
      rect_array->w[n_w - 1] = w + 1 - rect_array->x0[n_w - 1];

      rect_array->y0[0] = -1;
      for (n_y = 0; n_y < n_h - 1; ++n_y)
	{
	  rect_array->h[n_y] = tile_size;
	  rect_array->y0[n_y + 1] = rect_array->y0[n_y] + tile_spacing;
	}
      rect_array->h[n_h - 1] = h + 1 - rect_array->y0[n_h - 1];

      if (rect_array->w[n_w - 1] > 0 && rect_array->w[n_w - 1] <= tile_size &&
	  rect_array->h[n_h - 1] > 0 && rect_array->h[n_h - 1] <= tile_size)
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("rect_array dimensions are incorrect, w=%d, h=%d, tile_size=%d, last_w=%d, last_h=%d"),
		      w, h, tile_size, rect_array->w[n_w - 1],
		      rect_array->h[n_h - 1]);
	}
    }
  else
    {
      mod_gl_utils_rect_array_clear (utils_context, rect_array);
    }

  return ret;
}

void
mod_gl_utils_rect_array_clear (mod_gl_utils_context_t *
			       utils_context,
			       mod_gl_utils_rect_array_t * rect_array)
{
  if (rect_array->x0)
    {
      LW6SYS_FREE (rect_array->x0);
    }
  if (rect_array->y0)
    {
      LW6SYS_FREE (rect_array->y0);
    }
  if (rect_array->w)
    {
      LW6SYS_FREE (rect_array->w);
    }
  if (rect_array->h)
    {
      LW6SYS_FREE (rect_array->h);
    }
  if (rect_array->screen_x)
    {
      LW6SYS_FREE (rect_array->screen_x);
    }
  if (rect_array->screen_y)
    {
      LW6SYS_FREE (rect_array->screen_y);
    }
  memset (rect_array, 0, sizeof (mod_gl_utils_rect_array_t));
}
