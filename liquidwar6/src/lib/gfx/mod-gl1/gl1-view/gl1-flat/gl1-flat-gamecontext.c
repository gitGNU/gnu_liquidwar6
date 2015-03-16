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

//#include <GL/glu.h>

#include "../../../gfx.h"
#include "gl1-flat.h"
#include "gl1-flat-internal.h"

int
_mod_gl1_view_flat_game_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				      utils_context,
				      _mod_gl1_view_flat_context_t *
				      flat_context,
				      _mod_gl1_view_flat_game_context_t *
				      game_context, const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_game_state_t * game_state)
{
  int ret = 0;

  ret =
    _mod_gl1_view_flat_game_context_init_map (sys_context, utils_context, flat_context,
					      &(game_context->map), look,
					      level)
    && _mod_gl1_view_flat_game_context_init_armies (sys_context, utils_context, flat_context, &(game_context->armies), look, game_state);

  return ret;
}

int
_mod_gl1_view_flat_game_context_init_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					  utils_context,
					  _mod_gl1_view_flat_context_t *
					  flat_context,
					  _mod_gl1_view_flat_game_context_map_t * game_context_map, const lw6gui_look_t * look, const lw6map_level_t * level)
{
  int ret = 0;
  GLint wrap = 0;
  GLint filter = 0;

  ret = 1;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("init map"));
  memset (&(game_context_map->map_bitmap_array), 0, sizeof (mod_gl1_utils_bitmap_array_t));

  ret = ret && mod_gl1_utils_bitmap_array_init_from_map (sys_context, utils_context,
							 &
							 (game_context_map->map_bitmap_array),
							 level,
							 flat_context->const_data.tile_size_map,
							 flat_context->const_data.border_size_map,
							 level->param.rules.x_polarity, level->param.rules.y_polarity);
  if (ret)
    {
      wrap = GL_CLAMP_TO_EDGE;
      if (look->style.pixelize)
	{
	  filter = GL_NEAREST;
	}
      else
	{
	  // should be GL_LINEAR on low-end hardware, make this an option
	  filter = GL_LINEAR_MIPMAP_LINEAR;
	}

      ret = mod_gl1_utils_bitmap_array_update (sys_context, utils_context, &(game_context_map->map_bitmap_array), wrap, filter);
    }

  game_context_map->level = level;
  game_context_map->level_id = level->id;

  if (!ret)
    {

      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init game_context (map) for flat backend"));
    }

  return ret;
}

int
_mod_gl1_view_flat_game_context_init_armies (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					     utils_context,
					     _mod_gl1_view_flat_context_t *
					     flat_context,
					     _mod_gl1_view_flat_game_context_armies_t
					     * game_context_armies, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state)
{
  int ret = 0;
  lw6ker_game_struct_t *game_struct = NULL;

  ret = 1;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("init armies"));

  game_struct = game_state->game_struct;

  ret = ret
    && mod_gl1_utils_bitmap_array_init (sys_context, utils_context,
					&
					(game_context_armies->armies_bitmap_array),
					lw6ker_game_struct_get_w
					(sys_context, game_struct),
					lw6ker_game_struct_get_h
					(sys_context, game_struct), flat_context->const_data.tile_size_armies, flat_context->const_data.border_size_armies);

  game_context_armies->game_state = game_state;
  game_context_armies->game_struct_id = game_struct->id;

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init game_context (armies) for flat backend"));
    }

  return ret;
}

void
_mod_gl1_view_flat_game_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				       utils_context, _mod_gl1_view_flat_context_t * flat_context, _mod_gl1_view_flat_game_context_t * game_context)
{
  _mod_gl1_view_flat_game_context_clear_map (sys_context, utils_context, flat_context, &(game_context->map));
  _mod_gl1_view_flat_game_context_clear_armies (sys_context, utils_context, flat_context, &(game_context->armies));
}

void
_mod_gl1_view_flat_game_context_clear_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					   utils_context,
					   _mod_gl1_view_flat_context_t * flat_context, _mod_gl1_view_flat_game_context_map_t * game_context_map)
{
  mod_gl1_utils_bitmap_array_clear (sys_context, utils_context, &(game_context_map->map_bitmap_array));
  memset (game_context_map, 0, sizeof (_mod_gl1_view_flat_game_context_map_t));
}

void
_mod_gl1_view_flat_game_context_clear_armies (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					      utils_context,
					      _mod_gl1_view_flat_context_t * flat_context, _mod_gl1_view_flat_game_context_armies_t * game_context_armies)
{
  mod_gl1_utils_bitmap_array_clear (sys_context, utils_context, &(game_context_armies->armies_bitmap_array));

  memset (game_context_armies, 0, sizeof (_mod_gl1_view_flat_game_context_armies_t));
}

int
_mod_gl1_view_flat_game_context_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					utils_context,
					_mod_gl1_view_flat_context_t *
					flat_context,
					_mod_gl1_view_flat_game_context_t *
					game_context, const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_game_state_t * game_state)
{
  int ret = 0;

  ret =
    _mod_gl1_view_flat_game_context_update_map (sys_context, utils_context, flat_context,
						&(game_context->map), look,
						level)
    && _mod_gl1_view_flat_game_context_update_armies (sys_context, utils_context, flat_context, &(game_context->armies), look, game_state);

  return ret;
}

int
_mod_gl1_view_flat_game_context_update_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					    utils_context,
					    _mod_gl1_view_flat_context_t *
					    flat_context,
					    _mod_gl1_view_flat_game_context_map_t * game_context_map, const lw6gui_look_t * look, const lw6map_level_t * level)
{
  int ret = 0;

  if (game_context_map->level_id == level->id && game_context_map->level == level)
    {
      ret = 1;			// nothing to do, up to date
    }
  else
    {
      _mod_gl1_view_flat_game_context_clear_map (sys_context, utils_context, flat_context, game_context_map);
      ret = _mod_gl1_view_flat_game_context_init_map (sys_context, utils_context, flat_context, game_context_map, look, level);
    }

  return ret;
}

int
_mod_gl1_view_flat_game_context_update_armies (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					       utils_context,
					       _mod_gl1_view_flat_context_t *
					       flat_context,
					       _mod_gl1_view_flat_game_context_armies_t
					       * game_context_armies, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state)
{
  int ret = 0;

  /*
   * We rely on game_struct comparison to decide wether to
   * re-initialize context. This is because some modules such
   * as pil might pass different pointers during the same game
   * session (for instance in some cases "reference" and in some
   * cases "draft". The initialisation depends on the game_struct
   * anyway.
   */
  if (game_context_armies->game_struct_id == game_state->game_struct->id)
    {
      if (game_context_armies->game_state != game_state)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_
		      ("game_state is changed while game_struct remains the same, there's nothing wrong with that but not synchronizing the game_state in this special case has caused a historical, hard to track bug, so it's worth noting it"));
	  game_context_armies->game_state = game_state;
	}
      ret = 1;			// nothing to do, up to date
    }
  else
    {
      _mod_gl1_view_flat_game_context_clear_armies (sys_context, utils_context, flat_context, game_context_armies);
      ret = _mod_gl1_view_flat_game_context_init_armies (sys_context, utils_context, flat_context, game_context_armies, look, game_state);
    }

  return ret;
}
