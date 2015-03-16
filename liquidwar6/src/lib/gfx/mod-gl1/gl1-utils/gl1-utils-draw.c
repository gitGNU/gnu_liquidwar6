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

#include "../../gfx.h"
#include "gl1-utils.h"

void
mod_gl1_utils_draw_rectfill (lw6sys_context_t * sys_context, SDL_Surface * surface, int x1, int y1, int x2, int y2, Uint32 color)
{
  /*
   * The slowest implementation ever, however optimizing
   * this is of little interest, it's only used in very
   * rare/debugging functions. For now...
   */

  int x, y;

  for (x = x1; x < x2; ++x)
    {
      for (y = y1; y < y2; ++y)
	{
	  mod_gl1_utils_putpixel (sys_context, surface, x, y, color);
	}
    }
}

void
mod_gl1_utils_draw_set_alpha_for_color (lw6sys_context_t * sys_context, SDL_Surface * surface, float alpha, Uint32 color)
{
  int x, y, w, h;
  Uint32 pixel_color;
  Uint32 alpha_color;
  lw6sys_color_f_t color_f_tmp;

  //alpha_8=lw6sys_color_float2char(alpha);
  lw6sys_color_irgba_to_f (&color_f_tmp, color);
  color_f_tmp.a = alpha;
  alpha_color = lw6sys_color_f_to_irgba (&color_f_tmp);
  w = surface->w;
  h = surface->h;
  for (x = 0; x < w; ++x)
    {
      for (y = 0; y < h; ++y)
	{
	  pixel_color = mod_gl1_utils_getpixel (sys_context, surface, x, y);
	  if (pixel_color == color)
	    {
	      mod_gl1_utils_putpixel (sys_context, surface, x, y, alpha_color);
	    }
	}
    }
}
