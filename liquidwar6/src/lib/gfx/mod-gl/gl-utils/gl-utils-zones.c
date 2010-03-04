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

#include <math.h>

#include "../../gfx.h"
#include "gl-utils.h"

/*
 * Very slow, we don't care, for debugging only.
 */
SDL_Surface *
mod_gl_utils_create_zones_surface (mod_gl_utils_context_t * utils_context,
				   lw6ker_map_struct_t * map_struct)
{
  SDL_Surface *zones_surface;
  lw6sys_color_f_t color;

  zones_surface =
    mod_gl_utils_create_surface (utils_context, map_struct->shape.w,
				 map_struct->shape.h);
  if (zones_surface)
    {
      int i;
      float grey;
      lw6ker_zone_struct_t *zone;
      int log_base;

      log_base = map_struct->max_zone_size;
      if (log_base <= 1)
	{
	  log_base = 2;
	}

      for (i = 0; i < map_struct->nb_zones; ++i)
	{
	  zone = &map_struct->zones[i];
	  grey = log (zone->size) / log (log_base);

	  color.r = color.g = color.b = grey;
	  color.a = utils_context->const_data.gradient_opacity;

	  mod_gl_utils_draw_rectfill (zones_surface, zone->pos.x, zone->pos.y,
				      zone->pos.x + zone->size - 1,
				      zone->pos.y + zone->size - 1,
				      lw6sys_color_f_to_i (&color));
	}
    }

  return (zones_surface);
}
