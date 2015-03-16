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
#endif // HAVE_CONFIG_H

#include "ker.h"
#include "ker-internal.h"

static int
_push_place_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_place_struct_t * place_struct)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, place_struct->act_incr);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, place_struct->health_correction);

  return ret;
}

static int
_push_zone_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_zone_struct_t * zone_struct)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_push_xyz (sys_context, hexa_serializer, zone_struct->pos);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, zone_struct->size);
  for (i = 0; i < LW6KER_NB_DIRS; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, zone_struct->link[i]);
    }
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, zone_struct->one_way_dir_odd);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, zone_struct->one_way_dir_even);

  return ret;
}

static int
_push_slot_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_slot_struct_t * slot_struct)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, slot_struct->zone_id);

  return ret;
}

static int
_push_map_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_map_struct_t * map_struct)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_push_whd (sys_context, hexa_serializer, map_struct->shape);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, map_struct->nb_places);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, map_struct->nb_zones);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, map_struct->nb_slots);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, map_struct->room_for_armies);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, map_struct->max_zone_size);

  for (i = 0; i < map_struct->nb_places; ++i)
    {
      ret = ret && _push_place_struct (sys_context, hexa_serializer, &(map_struct->places[i]));
    }
  for (i = 0; i < map_struct->nb_zones; ++i)
    {
      ret = ret && _push_zone_struct (sys_context, hexa_serializer, &(map_struct->zones[i]));
    }
  for (i = 0; i < map_struct->nb_slots; ++i)
    {
      ret = ret && _push_slot_struct (sys_context, hexa_serializer, &(map_struct->slots[i]));
    }

  return ret;
}

char *
_lw6ker_game_struct_to_hexa (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct)
{
  char *ret = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer = NULL;
  int ok = 1;

  hexa_serializer = lw6sys_hexa_serializer_new (sys_context, NULL);
  if (hexa_serializer)
    {
      ok = ok && _push_map_struct (sys_context, hexa_serializer, &(game_struct->map_struct));
      if (ok)
	{
	  ret = lw6sys_hexa_serializer_as_string (sys_context, hexa_serializer);
	}
      lw6sys_hexa_serializer_free (sys_context, hexa_serializer);
    }

  return ret;
}

/**
 * lw6ker_game_struct_to_hexa
 *
 * @sys_context: global system context
 * @game_struct: the game struct to convert
 *
 * Converts a map to something that is later readable by @lw6ker_game_struct_from_hexa
 * to reproduce the exact same map. Just a serializer.
 *
 * Return value: a newly allocated pointer, NULL if conversion failed.
 */
char *
lw6ker_game_struct_to_hexa (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  char *ret = NULL;

  ret = _lw6ker_game_struct_to_hexa (sys_context, (const _lw6ker_game_struct_t *) game_struct);

  return ret;
}

static int
_pop_place_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_place_struct_t * place_struct)
{
  int ret = 1;
  int16_t tmp = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &tmp);
  place_struct->act_incr = tmp;
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &tmp);
  place_struct->health_correction = tmp;

  return ret;
}

static int
_pop_zone_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_zone_struct_t * zone_struct)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_pop_xyz (sys_context, hexa_serializer, &(zone_struct->pos));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(zone_struct->size));
  for (i = 0; i < LW6KER_NB_DIRS; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(zone_struct->link[i]));
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &(zone_struct->one_way_dir_odd));
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &(zone_struct->one_way_dir_even));

  return ret;
}

static int
_pop_slot_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_slot_struct_t * slot_struct)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(slot_struct->zone_id));

  return ret;
}

static int
_pop_map_struct (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_map_struct_t * map_struct)
{
  int ret = 1;
  int i = 0;
  lw6sys_whd_t shape_min = { LW6MAP_MIN_BODY_WIDTH, LW6MAP_MIN_BODY_HEIGHT, LW6MAP_MIN_BODY_DEPTH };
  lw6sys_whd_t shape_max = { LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_BODY_DEPTH };
  int surface = 0;
  int volume = 0;

  ret = ret && lw6sys_hexa_serializer_pop_whd (sys_context, hexa_serializer, &(map_struct->shape));
  if (ret && !lw6sys_shape_check_min_max_whd (sys_context, &(map_struct->shape), &shape_min, &shape_max))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("map_struct shape out of range (%dx%dx%d)"), map_struct->shape.w, map_struct->shape.h, map_struct->shape.d);
      ret = 0;
    }

  surface = lw6sys_shape_surface_wh (sys_context, &(map_struct->shape));
  volume = lw6sys_shape_volume_whd (sys_context, &(map_struct->shape));

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(map_struct->nb_places));
  if (ret && map_struct->nb_places != surface)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad nb_places=%d for surface=%d"), map_struct->nb_places, surface);
      ret = 0;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(map_struct->nb_zones));
  if (ret && map_struct->nb_zones > volume)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("nb_zones=%d too big for volume=%d"), map_struct->nb_zones, volume);
      ret = 0;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(map_struct->nb_slots));
  if (ret && map_struct->nb_slots != volume)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad nb_slots=%d for map_struct with volume=%d"), map_struct->nb_slots, volume);
      ret = 0;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(map_struct->room_for_armies));
  if (ret && map_struct->room_for_armies > map_struct->nb_slots)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("room_for_armies=%d too big for nb_slots=%d"), map_struct->room_for_armies, map_struct->nb_slots);
      ret = 0;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(map_struct->max_zone_size));
  if (ret && (map_struct->max_zone_size < LW6MAP_RULES_MIN_MAX_ZONE_SIZE || map_struct->max_zone_size > LW6MAP_RULES_MAX_MAX_ZONE_SIZE))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad max_zone_size=%d"), map_struct->max_zone_size);
      ret = 0;
    }

  if (ret)
    {
      map_struct->places = (_lw6ker_place_struct_t *) LW6SYS_CALLOC (sys_context, map_struct->nb_places * sizeof (_lw6ker_place_struct_t));
      if (ret && map_struct->places)
	{
	  for (i = 0; i < map_struct->nb_places; ++i)
	    {
	      ret = ret && _pop_place_struct (sys_context, hexa_serializer, &(map_struct->places[i]));
	    }
	}
      else
	{
	  ret = 0;
	}
    }
  if (ret)
    {
      map_struct->zones = (_lw6ker_zone_struct_t *) LW6SYS_CALLOC (sys_context, map_struct->nb_zones * sizeof (_lw6ker_zone_struct_t));
      if (ret && map_struct->nb_zones)
	{
	  for (i = 0; i < map_struct->nb_zones; ++i)
	    {
	      ret = ret && _pop_zone_struct (sys_context, hexa_serializer, &(map_struct->zones[i]));
	    }
	}
      else
	{
	  ret = 0;
	}
    }
  if (ret)
    {
      map_struct->slots = (_lw6ker_slot_struct_t *) LW6SYS_CALLOC (sys_context, map_struct->nb_slots * sizeof (_lw6ker_slot_struct_t));
      if (ret && map_struct->slots)
	{
	  for (i = 0; i < map_struct->nb_slots; ++i)
	    {
	      ret = ret && _pop_slot_struct (sys_context, hexa_serializer, &(map_struct->slots[i]));
	    }
	}
      else
	{
	  ret = 0;
	}
    }

  if (ret)
    {
      if (!_lw6ker_map_struct_sanity_check (sys_context, map_struct))
	{
	  ret = 0;
	}
    }

  if (!ret)
    {
      _lw6ker_map_struct_clear (sys_context, map_struct);
    }

  return ret;
}

_lw6ker_game_struct_t *
_lw6ker_game_struct_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const lw6map_level_t * level)
{
  _lw6ker_game_struct_t *game_struct = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer;
  int ok = 1;

  hexa_serializer = lw6sys_hexa_serializer_new (sys_context, hexa);
  if (hexa_serializer)
    {
      lw6sys_hexa_serializer_rewind (sys_context, hexa_serializer);

      game_struct = (_lw6ker_game_struct_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6ker_game_struct_t));

      if (game_struct)
	{
	  _lw6ker_game_struct_set_id (sys_context, game_struct);
	  _lw6ker_game_struct_point_to (sys_context, game_struct, level);
	  lw6map_rules_copy (sys_context, &(game_struct->rules), &(level->param.rules));
	  ok = ok && lw6map_rules_sanity_check (sys_context, &(game_struct->rules));
	  ok = ok && _pop_map_struct (sys_context, hexa_serializer, &(game_struct->map_struct));

	  if (lw6sys_shape_is_same (sys_context, &(game_struct->map_struct.shape), &(level->body.shape)))
	    {
	      if (!lw6sys_hexa_serializer_eof (sys_context, hexa_serializer))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("expected EOF in serialized game_struct"));
		  ok = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("shape mismatch game_struct=%dx%dx%d level=%dx%dx%d"),
			  game_struct->map_struct.shape.w,
			  game_struct->map_struct.shape.h, game_struct->map_struct.shape.d, level->body.shape.w, level->body.shape.h, level->body.shape.d);
	      ok = 0;
	    }
	}
      lw6sys_hexa_serializer_free (sys_context, hexa_serializer);
    }

  if (!ok)
    {
      if (game_struct)
	{
	  _lw6ker_game_struct_free (sys_context, game_struct);
	  game_struct = NULL;
	}
    }

  return game_struct;
}

/**
 * lw6ker_game_struct_from_hexa
 *
 * @sys_context: global system context
 * @hexa: an hexadecimal ASCII string, created by @lw6ker_game_struct_to_hexa
 * @level: the level this game_struct is bounded to
 *
 * Constructs a game struct from an hexadecimal string generated
 * by @lw6ker_game_struct_to_hexa. Just an un-serializer.
 *
 * Return value: a new map, might be NULL if string isn't correct.
 */
lw6ker_game_struct_t *
lw6ker_game_struct_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const lw6map_level_t * level)
{
  lw6ker_game_struct_t *game_struct = NULL;

  game_struct = (lw6ker_game_struct_t *) _lw6ker_game_struct_from_hexa (sys_context, hexa, level);

  return game_struct;
}

static int
_push_fighter (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6ker_fighter_t * fighter)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, fighter->team_color);
  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, fighter->last_direction);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, fighter->health);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, fighter->act_counter);
  ret = ret && lw6sys_hexa_serializer_push_xyz (sys_context, hexa_serializer, fighter->pos);

  return ret;
}

static int
_push_armies (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_armies_t * armies)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, armies->max_fighters);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, armies->active_fighters);
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, armies->fighters_per_team[i]);
    }
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, armies->frags[i]);
    }
  for (i = 0; i < armies->max_fighters; ++i)
    {
      ret = ret && _push_fighter (sys_context, hexa_serializer, &(armies->fighters[i]));
    }

  return ret;
}

static int
_push_zone_state (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_zone_state_t * zone)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, zone->potential);
  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, zone->direction_to_cursor);
  ret = ret && lw6sys_hexa_serializer_push_xyz (sys_context, hexa_serializer, zone->closest_cursor_pos);

  return ret;
}

static int
_push_team (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_team_t * team)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->active);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->has_been_active);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->respawn_round);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->offline);
  for (i = 0; i < team->map_struct->nb_zones; ++i)
    {
      ret = ret && _push_zone_state (sys_context, hexa_serializer, &(team->gradient[i]));
    }
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->cursor_ref_pot);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->last_spread_dir);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->charge);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->weapon_id);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->weapon_first_round);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, team->weapon_last_round);

  return ret;
}

static int
_push_cursor (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6ker_cursor_t * cursor)
{
  int ret = 1;


  ret = ret && lw6sys_hexa_serializer_push_int64 (sys_context, hexa_serializer, cursor->node_id);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, cursor->cursor_id);
  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, cursor->letter);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, cursor->enabled);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, cursor->team_color);
  ret = ret && lw6sys_hexa_serializer_push_xyz (sys_context, hexa_serializer, cursor->pos);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, cursor->fire);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, cursor->fire2);
  ret = ret && lw6sys_hexa_serializer_push_xyz (sys_context, hexa_serializer, cursor->apply_pos);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, cursor->pot_offset);

  return ret;
}

static int
_push_cursor_array (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_cursor_array_t * cursor_array)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, cursor_array->nb_cursors);
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      ret = ret && _push_cursor (sys_context, hexa_serializer, &(cursor_array->cursors[i]));
    }

  return ret;
}

static int
_push_slot_state (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_slot_state_t * slot)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, slot->fighter_id);

  return ret;
}

static int
_push_map_state (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_map_state_t * map_state)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_push_whd (sys_context, hexa_serializer, map_state->shape);
  ret = ret && _push_armies (sys_context, hexa_serializer, &(map_state->armies));
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, map_state->max_nb_teams);
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      ret = ret && _push_team (sys_context, hexa_serializer, &(map_state->teams[i]));
    }
  ret = ret && _push_cursor_array (sys_context, hexa_serializer, &(map_state->cursor_array));
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, map_state->nb_slots);
  for (i = 0; i < map_state->nb_slots; ++i)
    {
      ret = ret && _push_slot_state (sys_context, hexa_serializer, &(map_state->slots[i]));
    }

  return ret;
}

static int
_push_node (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_node_t * node)
{
  int ret = 1;


  ret = ret && lw6sys_hexa_serializer_push_int64 (sys_context, hexa_serializer, node->node_id);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, node->enabled);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, node->last_command_round);

  return ret;
}

static int
_push_node_array (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_node_array_t * node_array)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, node_array->nb_nodes);
  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      ret = ret && _push_node (sys_context, hexa_serializer, &(node_array->nodes[i]));
    }

  return ret;
}

static int
_push_history (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const _lw6ker_history_t * history)
{
  int ret = 1;
  int i = 0, j = 0;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, history->nb_entries);
  for (i = 0; i < LW6KER_HISTORY_SIZE; ++i)
    {
      for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	{
	  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, history->nb_fighters[i][j]);
	}
    }

  return ret;
}

char *
_lw6ker_game_state_to_hexa (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  char *ret = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer = NULL;
  int ok = 1;

  hexa_serializer = lw6sys_hexa_serializer_new (sys_context, NULL);
  if (hexa_serializer)
    {
      ok = ok && _push_map_state (sys_context, hexa_serializer, &(game_state->map_state));
      ok = ok && _push_node_array (sys_context, hexa_serializer, &(game_state->node_array));
      ok = ok && _push_history (sys_context, hexa_serializer, &(game_state->global_history));
      ok = ok && _push_history (sys_context, hexa_serializer, &(game_state->latest_history));
      ok = ok && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, game_state->moves);
      ok = ok && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, game_state->spreads);
      ok = ok && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, game_state->rounds);
      ok = ok && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, game_state->total_rounds);
      ok = ok && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, game_state->max_reached_teams);
      ok = ok && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, game_state->over);

      if (ok)
	{
	  ret = lw6sys_hexa_serializer_as_string (sys_context, hexa_serializer);
	}
      lw6sys_hexa_serializer_free (sys_context, hexa_serializer);
    }

  return ret;
}

/**
 * lw6ker_game_state_to_hexa
 *
 * @sys_context: global system context
 * @game_state: the game state to convert
 *
 * Converts a map to something that is later readable by @lw6ker_game_state_from_hexa
 * to reproduce the exact same map. Just a serializer.
 *
 * Return value: a newly allocated pointer, NULL if conversion failed.
 */
char *
lw6ker_game_state_to_hexa (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  char *ret = NULL;

  ret = _lw6ker_game_state_to_hexa (sys_context, (const _lw6ker_game_state_t *) game_state);

  return ret;
}

static int
_pop_fighter (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6ker_fighter_t * fighter)
{
  int ret = 1;
  int8_t tmp8 = 0;
  int16_t tmp16 = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &tmp8);
  fighter->team_color = tmp8;
  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &tmp8);
  fighter->last_direction = tmp8;
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &tmp16);
  fighter->health = tmp16;
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &tmp16);
  fighter->act_counter = tmp16;
  ret = ret && lw6sys_hexa_serializer_pop_xyz (sys_context, hexa_serializer, &(fighter->pos));

  return ret;
}

static int
_pop_armies (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_armies_t * armies)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(armies->max_fighters));
  if (ret && armies->max_fighters > LW6MAP_MAX_BODY_VOLUME)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("max_fighters is too great (%d>%d)"), armies->max_fighters, LW6MAP_MAX_BODY_VOLUME);
      ret = 0;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(armies->active_fighters));
  if (ret && armies->active_fighters > armies->max_fighters)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("active_fighters is too great (%d>%d)"), armies->active_fighters, armies->max_fighters);
      ret = 0;
    }
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(armies->fighters_per_team[i]));
      if (ret && armies->fighters_per_team[i] > armies->active_fighters)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("fighters_per_team[%d] is too great (%d>%d)"), i, armies->fighters_per_team[i], armies->active_fighters);
	  ret = 0;
	}
    }
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(armies->frags[i]));
    }
  if (ret)
    {
      armies->fighters = (lw6ker_fighter_t *) LW6SYS_CALLOC (sys_context, armies->max_fighters * sizeof (lw6ker_fighter_t));
      if (ret && armies->fighters)
	{
	  for (i = 0; i < armies->max_fighters; ++i)
	    {
	      ret = ret && _pop_fighter (sys_context, hexa_serializer, &(armies->fighters[i]));
	    }
	}
    }

  return ret;
}

static int
_pop_zone_state (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_zone_state_t * zone)
{
  int ret = 1;
  int8_t tmp8 = 0;
  int32_t tmp32 = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &tmp32);
  zone->potential = tmp32;
  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &tmp8);
  zone->direction_to_cursor = tmp8;
  ret = ret && lw6sys_hexa_serializer_pop_xyz (sys_context, hexa_serializer, &(zone->closest_cursor_pos));

  return ret;
}

static int
_pop_team (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_team_t * team)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->active));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->has_been_active));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->respawn_round));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->offline));
  if (ret)
    {
      team->gradient = (_lw6ker_zone_state_t *) LW6SYS_CALLOC (sys_context, team->map_struct->nb_zones * sizeof (_lw6ker_zone_state_t));
      if (ret && team->gradient)
	{
	  for (i = 0; i < team->map_struct->nb_zones; ++i)
	    {
	      ret = ret && _pop_zone_state (sys_context, hexa_serializer, &(team->gradient[i]));
	    }
	}
      else
	{
	  ret = 0;
	}
    }
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->cursor_ref_pot));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->last_spread_dir));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->charge));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->weapon_id));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->weapon_first_round));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(team->weapon_last_round));

  return ret;
}

static int
_pop_cursor (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6ker_cursor_t * cursor)
{
  int ret = 1;
  int64_t tmp64 = 0LL;
  int16_t tmp16 = 0;
  int8_t tmp8 = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int64 (sys_context, hexa_serializer, &tmp64);
  cursor->node_id = tmp64;
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &tmp16);
  cursor->cursor_id = tmp16;
  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &tmp8);
  cursor->letter = tmp8;
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(cursor->enabled));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(cursor->team_color));
  ret = ret && lw6sys_hexa_serializer_pop_xyz (sys_context, hexa_serializer, &(cursor->pos));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(cursor->fire));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(cursor->fire2));
  ret = ret && lw6sys_hexa_serializer_pop_xyz (sys_context, hexa_serializer, &(cursor->apply_pos));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(cursor->pot_offset));

  return ret;
}

static int
_pop_cursor_array (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_cursor_array_t * cursor_array)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(cursor_array->nb_cursors));
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      ret = ret && _pop_cursor (sys_context, hexa_serializer, &(cursor_array->cursors[i]));
    }

  return ret;
}

static int
_pop_slot_state (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_slot_state_t * slot)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(slot->fighter_id));

  return ret;
}

static int
_pop_map_state (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_map_state_t * map_state)
{
  int ret = 1;
  int i = 0;
  lw6sys_whd_t shape_min = { LW6MAP_MIN_BODY_WIDTH, LW6MAP_MIN_BODY_HEIGHT, LW6MAP_MIN_BODY_DEPTH };
  lw6sys_whd_t shape_max = { LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_BODY_DEPTH };
  int surface = 0;
  int volume = 0;

  ret = ret && lw6sys_hexa_serializer_pop_whd (sys_context, hexa_serializer, &(map_state->shape));
  if (ret && !lw6sys_shape_check_min_max_whd (sys_context, &(map_state->shape), &shape_min, &shape_max))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("map_state shape out of range (%dx%dx%d)"), map_state->shape.w, map_state->shape.h, map_state->shape.d);
      ret = 0;
    }

  surface = lw6sys_shape_surface_wh (sys_context, &(map_state->shape));
  volume = lw6sys_shape_volume_whd (sys_context, &(map_state->shape));

  if (ret)
    {
      map_state->shape_surface = surface;
    }

  ret = ret && _pop_armies (sys_context, hexa_serializer, &(map_state->armies));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(map_state->max_nb_teams));
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      ret = ret && _pop_team (sys_context, hexa_serializer, &(map_state->teams[i]));
    }
  ret = ret && _pop_cursor_array (sys_context, hexa_serializer, &(map_state->cursor_array));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(map_state->nb_slots));
  if (ret && map_state->nb_slots != volume)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad nb_slots=%d for map_state with volume=%d"), map_state->nb_slots, volume);
      ret = 0;
    }
  if (ret && map_state->nb_slots != map_state->map_struct->nb_slots)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("map_state->nb_slots=%d but map_state->map_struct->nb_slots=%d"), map_state->nb_slots, map_state->map_struct->nb_slots);
      ret = 0;
    }
  if (ret)
    {
      map_state->slots = (_lw6ker_slot_state_t *) LW6SYS_CALLOC (sys_context, map_state->nb_slots * sizeof (_lw6ker_slot_state_t));
      if (ret && map_state->slots)
	{
	  for (i = 0; i < map_state->map_struct->nb_slots; ++i)
	    {
	      ret = ret && _pop_slot_state (sys_context, hexa_serializer, &(map_state->slots[i]));
	    }
	}
    }

  if (ret)
    {
      if (!_lw6ker_map_state_sanity_check (sys_context, map_state))
	{
	  ret = 0;
	}
    }

  if (!ret)
    {
      _lw6ker_map_state_clear (sys_context, map_state);
    }

  return ret;
}

static int
_pop_node (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_node_t * node)
{
  int ret = 1;
  int64_t tmp64 = 0LL;

  ret = ret && lw6sys_hexa_serializer_pop_int64 (sys_context, hexa_serializer, &tmp64);
  node->node_id = tmp64;
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(node->enabled));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(node->last_command_round));

  return ret;
}

static int
_pop_node_array (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_node_array_t * node_array)
{
  int ret = 1;
  int i = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(node_array->nb_nodes));
  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      ret = ret && _pop_node (sys_context, hexa_serializer, &(node_array->nodes[i]));
    }

  return ret;
}

static int
_pop_history (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, _lw6ker_history_t * history)
{
  int ret = 1;
  int i = 0, j = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(history->nb_entries));
  for (i = 0; i < LW6KER_HISTORY_SIZE; ++i)
    {
      for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	{
	  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(history->nb_fighters[i][j]));
	}
    }

  return ret;
}

_lw6ker_game_state_t *
_lw6ker_game_state_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const _lw6ker_game_struct_t * game_struct)
{
  _lw6ker_game_state_t *game_state = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer;
  int ok = 1;
  int32_t tmp32 = 0;
  int8_t tmp8 = 0;

  hexa_serializer = lw6sys_hexa_serializer_new (sys_context, hexa);
  if (hexa_serializer)
    {
      lw6sys_hexa_serializer_rewind (sys_context, hexa_serializer);

      game_state = (_lw6ker_game_state_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6ker_game_state_t));

      if (game_state)
	{
	  _lw6ker_game_state_set_id (sys_context, game_state);
	  _lw6ker_game_state_point_to (sys_context, game_state, game_struct);
	  game_state->map_state.map_struct = &(game_struct->map_struct);
	  ok = ok && _pop_map_state (sys_context, hexa_serializer, &(game_state->map_state));
	  ok = ok && _pop_node_array (sys_context, hexa_serializer, &(game_state->node_array));
	  ok = ok && _pop_history (sys_context, hexa_serializer, &(game_state->global_history));
	  ok = ok && _pop_history (sys_context, hexa_serializer, &(game_state->latest_history));
	  ok = ok && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &tmp32);
	  game_state->moves = tmp32;
	  ok = ok && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &tmp32);
	  game_state->spreads = tmp32;
	  ok = ok && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &tmp32);
	  game_state->rounds = tmp32;
	  ok = ok && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &tmp32);
	  game_state->total_rounds = tmp32;
	  ok = ok && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &tmp8);
	  game_state->max_reached_teams = tmp8;
	  ok = ok && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &tmp8);
	  game_state->over = tmp8;

	  if (lw6sys_shape_is_same (sys_context, &(game_state->map_state.shape), &(game_struct->map_struct.shape)))
	    {
	      if (!lw6sys_hexa_serializer_eof (sys_context, hexa_serializer))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("expected EOF in serialized game_state"));
		  ok = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("shape mismatch map_state=%dx%dx%d map_struct=%dx%dx%d"),
			  game_state->map_state.shape.w,
			  game_state->map_state.shape.h,
			  game_state->map_state.shape.d, game_struct->map_struct.shape.w, game_struct->map_struct.shape.h, game_struct->map_struct.shape.d);
	      ok = 0;
	    }
	}
      lw6sys_hexa_serializer_free (sys_context, hexa_serializer);
    }

  if (!ok)
    {
      if (game_state)
	{
	  _lw6ker_game_state_free (sys_context, game_state);
	  game_state = NULL;
	}
    }

  return game_state;
}

/**
 * lw6ker_game_state_from_hexa
 *
 * @sys_context: global system context
 * @hexa: an hexadecimal ASCII string, created by @lw6ker_game_state_to_hexa
 * @game_struct: the game_struct this game_state is bounded to
 *
 * Constructs a game state from an hexadecimal string generated
 * by @lw6ker_game_state_to_hexa. Just an un-serializer.
 *
 * Return value: a new map, might be NULL if string isn't correct.
 */
lw6ker_game_state_t *
lw6ker_game_state_from_hexa (lw6sys_context_t * sys_context, const char *hexa, const lw6ker_game_struct_t * game_struct)
{
  lw6ker_game_state_t *game_state = NULL;

  game_state = (lw6ker_game_state_t *) _lw6ker_game_state_from_hexa (sys_context, hexa, (_lw6ker_game_struct_t *) game_struct);

  return game_state;
}
