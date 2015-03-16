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

#include <math.h>

#include "../../gfx.h"
#include "gl1-utils.h"

/*
 * Very slow, we don't care, for debugging only.
 */
SDL_Surface *
mod_gl1_utils_create_zones_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6ker_game_struct_t * game_struct)
{
  SDL_Surface *zones_surface;
  lw6sys_color_f_t color;
  lw6sys_whd_t shape;

  lw6ker_game_struct_get_shape (sys_context, game_struct, &shape);

  zones_surface = mod_gl1_utils_create_surface (sys_context, utils_context, shape.w, shape.h);
  if (zones_surface)
    {
      int i;
      float grey;
      int log_base;
      int nb_zones, max_zone_size;
      lw6sys_xyz_t zone_pos;
      int zone_size;

      lw6ker_game_struct_get_zones_info (sys_context, game_struct, &nb_zones, &max_zone_size);

      log_base = max_zone_size;
      if (log_base <= 1)
	{
	  log_base = 2;
	}

      for (i = 0; i < nb_zones; ++i)
	{
	  lw6ker_game_struct_get_zone_info (sys_context, game_struct, i, &zone_pos, &zone_size);
	  grey = log (zone_size) / log (log_base);

	  color.r = color.g = color.b = grey;
	  color.a = utils_context->const_data.gradient_opacity;

	  mod_gl1_utils_draw_rectfill (sys_context, zones_surface, zone_pos.x, zone_pos.y,
				       zone_pos.x + zone_size - 1, zone_pos.y + zone_size - 1, lw6sys_color_f_to_irgba (&color));
	}
    }

  return (zones_surface);
}
