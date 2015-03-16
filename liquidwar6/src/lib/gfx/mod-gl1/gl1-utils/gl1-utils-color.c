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

SDL_Color
mod_gl1_utils_color_8_to_sdl (lw6sys_context_t * sys_context, lw6sys_color_8_t color_8)
{
  SDL_Color ret;

  ret.r = color_8.r;
  ret.g = color_8.g;
  ret.b = color_8.b;
  ret.unused = 0;

  return ret;
}

SDL_Color
mod_gl1_utils_color_f_to_sdl (lw6sys_context_t * sys_context, const lw6sys_color_f_t * color_f)
{
  lw6sys_color_8_t color_8;
  SDL_Color ret;

  color_8 = lw6sys_color_f_to_8 (color_f);
  ret = mod_gl1_utils_color_8_to_sdl (sys_context, color_8);

  return ret;
}

static Uint32
_prepare_shaded_color_for_fighter (lw6sys_context_t * sys_context, lw6sys_color_8_t dead_color, lw6sys_color_8_t team_color, int j)
{
  Uint32 ret = 0;
  lw6sys_color_8_t color;

  color = lw6sys_color_ponderate (sys_context, dead_color, team_color, ((float) j) / ((float) MOD_GL1_SHADES_FOR_FIGHTERS_SCALE));
  color.a = 0xFF;
  ret = lw6sys_color_8_to_irgba (color);

  return ret;
}

void
mod_gl1_utils_update_team_color_map (lw6sys_context_t * sys_context, mod_gl1_utils_team_color_map_t * team_color_map, const lw6map_style_t * map_style)
{
  int i, j;
  Uint32 test;
  lw6sys_color_8_t team_color;
  lw6sys_color_8_t dead_color;

  dead_color = map_style->color_set.team_color_dead;
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      team_color = map_style->color_set.team_colors[i];
      test = _prepare_shaded_color_for_fighter (sys_context, dead_color, team_color, MOD_GL1_SHADES_FOR_FIGHTERS_SCALE);
      if (test != team_color_map->team_colors[i][MOD_GL1_SHADES_FOR_FIGHTERS_SCALE])
	{
	  lw6sys_color_8_to_f (&(team_color_map->team_colors_f[i]), team_color);
	  for (j = 0; j < MOD_GL1_SHADES_FOR_FIGHTERS_SCALE + 1; ++j)
	    {
	      team_color_map->team_colors[i][j] = _prepare_shaded_color_for_fighter (sys_context, dead_color, team_color, j);
	    }
	}
    }
}

int
mod_gl1_utils_team_color_map_is_same (lw6sys_context_t * sys_context, const mod_gl1_utils_team_color_map_t * team_color_map_a,
				      const mod_gl1_utils_team_color_map_t * team_color_map_b)
{
  int ret = 0;

  ret = !memcmp (team_color_map_a, team_color_map_b, sizeof (mod_gl1_utils_team_color_map_t));

  return ret;
}

void
mod_gl1_utils_force_color32_alpha (lw6sys_context_t * sys_context, Uint32 * color, float alpha)
{
  unsigned char *ptr = (unsigned char *) color;

  ptr[3] = (unsigned char) (alpha * 255.0f);
}

Uint32
mod_gl1_utils_get_shaded_color_for_fighter_f (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int team_id, float health)
{
  Uint32 ret;
  float f;

  f = health * (float) MOD_GL1_SHADES_FOR_FIGHTERS_SCALE;
  ret = utils_context->team_color_map.team_colors[team_id][(int) f];

  return ret;
}
