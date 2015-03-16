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

#define DISPLAY_CYCLES 4

/*
 * Very slow, we don't care, for debugging only.
 */
SDL_Surface *
mod_gl1_utils_create_gradient_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6ker_game_state_t * game_state,
				       int team_id, int layer_id)
{
  SDL_Surface *gradient_surface;
  lw6sys_color_f_t color;
  lw6sys_whd_t shape;

  lw6ker_game_state_get_shape (sys_context, game_state, &shape);

  if (layer_id < 0 || layer_id >= shape.d)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("layer %d out of range"), layer_id);
    }

  gradient_surface = mod_gl1_utils_create_surface (sys_context, utils_context, shape.w, shape.h);
  if (gradient_surface)
    {
      int i;
      float grey;
      int pot_min = 0;
      int pot_max = 0;
      int nb_zones;
      int potential;
      lw6sys_xyz_t zone_pos;
      int zone_size;

      lw6ker_game_struct_get_zones_info (sys_context, game_state->game_struct, &nb_zones, NULL);
      if (nb_zones > 0)
	{
	  pot_min = lw6ker_game_state_get_zone_potential (sys_context, game_state, 0, team_id);
	  pot_max = lw6ker_game_state_get_zone_potential (sys_context, game_state, 0, team_id);
	}

      for (i = 1; i < nb_zones; ++i)
	{
	  pot_min = lw6sys_imin (pot_min, lw6ker_game_state_get_zone_potential (sys_context, game_state, i, team_id));
	  pot_max = lw6sys_imax (pot_max, lw6ker_game_state_get_zone_potential (sys_context, game_state, i, team_id));
	}

      pot_max = lw6sys_imax (pot_min + 1, pot_max);

      for (i = 0; i < nb_zones; ++i)
	{
	  lw6ker_game_struct_get_zone_info (sys_context, game_state->game_struct, i, &zone_pos, &zone_size);
	  if (zone_pos.z == layer_id)
	    {
	      potential = lw6ker_game_state_get_zone_potential (sys_context, game_state, i, team_id);
	      grey = ((float) (potential - pot_min)) / ((float) (pot_max - pot_min));
	      grey *= DISPLAY_CYCLES;
	      grey = fmod (grey, 1.0f);

	      color.r = color.g = color.b = grey;
	      color.a = utils_context->const_data.gradient_opacity;

	      mod_gl1_utils_draw_rectfill (sys_context, gradient_surface, zone_pos.x,
					   zone_pos.y, zone_pos.x + zone_size - 1, zone_pos.y + zone_size - 1, lw6sys_color_f_to_irgba (&color));
	    }
	}
    }

  return (gradient_surface);
}
