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

#define DISPLAY_CYCLES 4

/*
 * Very slow, we don't care, for debugging only.
 */
SDL_Surface *
mod_gl_utils_create_gradient_surface (mod_gl_utils_context_t * utils_context,
				      lw6ker_map_state_t * map_state,
				      int team_id, int layer_id)
{
  SDL_Surface *gradient_surface;
  lw6sys_color_f_t color;

  if (layer_id < 0 || layer_id >= map_state->shape.d)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("layer %d out of range"), layer_id);
    }

  gradient_surface =
    mod_gl_utils_create_surface (utils_context,
				 map_state->map_struct->shape.w,
				 map_state->map_struct->shape.h);
  if (gradient_surface)
    {
      int i;
      float grey;
      lw6ker_zone_struct_t *zone;
      lw6ker_team_t *team;
      int pot_min = 0;
      int pot_max = 0;

      team = &map_state->teams[team_id];
      if (map_state->map_struct->nb_zones > 0)
	{
	  pot_min = team->gradient[0].potential;
	  pot_max = team->gradient[0].potential;
	}

      for (i = 1; i < map_state->map_struct->nb_zones; ++i)
	{
	  pot_min = lw6sys_min (pot_min, team->gradient[i].potential);
	  pot_max = lw6sys_max (pot_max, team->gradient[i].potential);
	}

      pot_max = lw6sys_max (pot_min + 1, pot_max);

      for (i = 0; i < map_state->map_struct->nb_zones; ++i)
	{
	  zone = &map_state->map_struct->zones[i];
	  if (zone->pos.z == layer_id)
	    {
	      grey =
		((float) (team->gradient[i].potential - pot_min)) /
		((float) (pot_max - pot_min));
	      grey *= DISPLAY_CYCLES;
	      grey = fmod (grey, 1.0f);

	      color.r = color.g = color.b = grey;
	      color.a = utils_context->const_data.gradient_opacity;

	      mod_gl_utils_draw_rectfill (gradient_surface, zone->pos.x,
					  zone->pos.y,
					  zone->pos.x + zone->size - 1,
					  zone->pos.y + zone->size - 1,
					  lw6sys_color_f_to_i (&color));
	    }
	}
    }

  return (gradient_surface);
}
