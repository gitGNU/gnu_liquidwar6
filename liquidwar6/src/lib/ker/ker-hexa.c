/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "ker.h"
#include "ker-internal.h"

static int
push_place (lw6sys_hexa_serializer_t * hexa_serializer,
	    _lw6ker_place_struct_t * place_struct)
{
  int ret = 1;

  ret = ret
    && lw6sys_hexa_serializer_push_int16 (hexa_serializer,
					  place_struct->act_incr);
  ret = ret
    && lw6sys_hexa_serializer_push_int16 (hexa_serializer,
					  place_struct->health_correction);

  return ret;
}

static int
push_zone (lw6sys_hexa_serializer_t * hexa_serializer,
	   _lw6ker_zone_struct_t * zone_struct)
{
  int ret = 1;
  int i = 0;

  ret = ret
    && lw6sys_hexa_serializer_push_xyz (hexa_serializer, zone_struct->pos);
  ret = ret
    && lw6sys_hexa_serializer_push_int32 (hexa_serializer, zone_struct->size);
  for (i = 0; i < LW6KER_NB_DIRS; ++i)
    {
      ret = ret
	&& lw6sys_hexa_serializer_push_int32 (hexa_serializer,
					      zone_struct->link[i]);
    }
  ret = ret
    && lw6sys_hexa_serializer_push_int16 (hexa_serializer,
					  zone_struct->one_way_dir_odd);
  ret = ret
    && lw6sys_hexa_serializer_push_int16 (hexa_serializer,
					  zone_struct->one_way_dir_even);

  return ret;
}

static int
push_slot (lw6sys_hexa_serializer_t * hexa_serializer,
	   _lw6ker_slot_struct_t * slot_struct)
{
  int ret = 1;

  ret = ret
    && lw6sys_hexa_serializer_push_int32 (hexa_serializer,
					  slot_struct->zone_id);

  return ret;
}

static int
push_map_struct (lw6sys_hexa_serializer_t * hexa_serializer,
		 _lw6ker_map_struct_t * map_struct)
{
  int ret = 1;
  int i = 0;

  ret = ret
    && lw6sys_hexa_serializer_push_whd (hexa_serializer, map_struct->shape);
  ret = ret
    && lw6sys_hexa_serializer_push_int32 (hexa_serializer,
					  map_struct->nb_places);
  ret = ret
    && lw6sys_hexa_serializer_push_int32 (hexa_serializer,
					  map_struct->nb_zones);
  ret = ret
    && lw6sys_hexa_serializer_push_int32 (hexa_serializer,
					  map_struct->nb_slots);
  ret = ret
    && lw6sys_hexa_serializer_push_int32 (hexa_serializer,
					  map_struct->room_for_armies);
  ret = ret
    && lw6sys_hexa_serializer_push_int32 (hexa_serializer,
					  map_struct->max_zone_size);

  for (i = 0; i < map_struct->nb_places; ++i)
    {
      ret = ret && push_place (hexa_serializer, &(map_struct->places[i]));
    }
  for (i = 0; i < map_struct->nb_zones; ++i)
    {
      ret = ret && push_zone (hexa_serializer, &(map_struct->zones[i]));
    }
  for (i = 0; i < map_struct->nb_slots; ++i)
    {
      ret = ret && push_slot (hexa_serializer, &(map_struct->slots[i]));
    }

  return ret;
}

char *
_lw6ker_game_struct_to_hexa (_lw6ker_game_struct_t * game_struct)
{
  char *ret = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer = NULL;
  int ok = 1;

  hexa_serializer = lw6sys_hexa_serializer_new (NULL);
  if (hexa_serializer)
    {
      ok = ok
	&& push_map_struct (hexa_serializer, &(game_struct->map_struct));
      if (ok)
	{
	  ret = lw6sys_hexa_serializer_as_string (hexa_serializer);
	}
      lw6sys_hexa_serializer_free (hexa_serializer);
    }

  return ret;
}

/**
 * lw6ker_game_struct_to_hexa
 *
 * @map: the map to convert
 *
 * Converts a map to something that is later readable by @lw6ker_game_struct_from_hexa
 * to reproduce the exact same map. Just a serializer.
 *
 * Return value: a newly allocated pointer, NULL if conversion failed.
 */
char *
lw6ker_game_struct_to_hexa (lw6ker_game_struct_t * game_struct)
{
  char *ret = NULL;

  ret = _lw6ker_game_struct_to_hexa ((_lw6ker_game_struct_t *) game_struct);

  return ret;
}

static int
pop_place (lw6sys_hexa_serializer_t * hexa_serializer,
	   _lw6ker_place_struct_t * place_struct)
{
  int ret = 1;
  int16_t tmp = 0;

  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &tmp);
  place_struct->act_incr = tmp;
  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &tmp);
  place_struct->health_correction = tmp;

  return ret;
}

static int
pop_zone (lw6sys_hexa_serializer_t * hexa_serializer,
	  _lw6ker_zone_struct_t * zone_struct)
{
  int ret = 1;
  int i = 0;

  ret = ret
    && lw6sys_hexa_serializer_pop_xyz (hexa_serializer, &(zone_struct->pos));
  ret = ret
    && lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					 &(zone_struct->size));
  for (i = 0; i < LW6KER_NB_DIRS; ++i)
    {
      ret = ret
	&& lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					     &(zone_struct->link[i]));
    }
  ret = ret
    && lw6sys_hexa_serializer_pop_int16 (hexa_serializer,
					 &(zone_struct->one_way_dir_odd));
  ret = ret
    && lw6sys_hexa_serializer_pop_int16 (hexa_serializer,
					 &(zone_struct->one_way_dir_even));

  return ret;
}

static int
pop_slot (lw6sys_hexa_serializer_t * hexa_serializer,
	  _lw6ker_slot_struct_t * slot_struct)
{
  int ret = 1;

  ret = ret
    && lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					 &(slot_struct->zone_id));

  return ret;
}

static int
pop_map_struct (lw6sys_hexa_serializer_t * hexa_serializer,
		_lw6ker_map_struct_t * map_struct)
{
  int ret = 1;
  int i = 0;
  lw6sys_whd_t shape_min =
    { LW6MAP_MIN_BODY_WIDTH, LW6MAP_MIN_BODY_HEIGHT, LW6MAP_MIN_BODY_DEPTH };
  lw6sys_whd_t shape_max =
    { LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_BODY_DEPTH };
  int surface = 0;
  int volume = 0;

  ret = ret
    && lw6sys_hexa_serializer_pop_whd (hexa_serializer, &(map_struct->shape));
  if (!lw6sys_shape_check_min_max_whd
      (&(map_struct->shape), &shape_min, &shape_max))
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("map_struct shape out of range (%dx%dx%d)"),
		  map_struct->shape.w, map_struct->shape.h,
		  map_struct->shape.d);
      ret = 0;
    }

  surface = lw6sys_shape_surface_wh (&(map_struct->shape));
  volume = lw6sys_shape_volume_whd (&(map_struct->shape));

  ret = ret
    && lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					 &(map_struct->nb_places));
  if (ret && map_struct->nb_places != surface)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bad nb_places=%d for surface=%d"),
		  map_struct->nb_places, surface);
      ret = 0;
    }
  ret = ret
    && lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					 &(map_struct->nb_zones));
  if (ret && map_struct->nb_zones > volume)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("nb_zones=%d too big for volume=%d"),
		  map_struct->nb_zones, volume);
      ret = 0;
    }
  ret = ret
    && lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					 &(map_struct->nb_slots));
  if (ret && map_struct->nb_slots != volume)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bad nb_slots=%d for volume=%d"),
		  map_struct->nb_slots, volume);
      ret = 0;
    }
  ret = ret
    && lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					 &(map_struct->room_for_armies));
  if (ret && map_struct->room_for_armies > map_struct->nb_slots)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("room_for_armies=%d too big for nb_slots=%d"),
		  map_struct->room_for_armies, map_struct->nb_slots);
      ret = 0;
    }
  ret = ret
    && lw6sys_hexa_serializer_pop_int32 (hexa_serializer,
					 &(map_struct->max_zone_size));
  if (ret
      && (map_struct->max_zone_size < LW6MAP_RULES_MIN_MAX_ZONE_SIZE
	  || map_struct->max_zone_size > LW6MAP_RULES_MAX_MAX_ZONE_SIZE))
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bad max_zone_size=%d"),
		  map_struct->max_zone_size);
      ret = 0;
    }

  // TODO: add checks so that impossible map structs can't be poped

  if (ret)
    {
      map_struct->places =
	(_lw6ker_place_struct_t *) LW6SYS_CALLOC (map_struct->nb_places *
						  sizeof
						  (_lw6ker_place_struct_t));
      for (i = 0; i < map_struct->nb_places; ++i)
	{
	  ret = ret && pop_place (hexa_serializer, &(map_struct->places[i]));
	}
    }
  if (ret)
    {
      map_struct->zones =
	(_lw6ker_zone_struct_t *) LW6SYS_CALLOC (map_struct->nb_zones *
						 sizeof
						 (_lw6ker_zone_struct_t));
      for (i = 0; i < map_struct->nb_zones; ++i)
	{
	  ret = ret && pop_zone (hexa_serializer, &(map_struct->zones[i]));
	}
    }
  if (ret)
    {
      map_struct->slots =
	(_lw6ker_slot_struct_t *) LW6SYS_CALLOC (map_struct->nb_slots *
						 sizeof
						 (_lw6ker_slot_struct_t));
      for (i = 0; i < map_struct->nb_slots; ++i)
	{
	  ret = ret && pop_slot (hexa_serializer, &(map_struct->slots[i]));
	}
    }

  if (ret)
    {
      if (!_lw6ker_map_struct_sanity_check (map_struct))
	{
	  ret = 0;
	}
    }

  if (!ret)
    {
      _lw6ker_map_struct_clear (map_struct);
    }

  return ret;
}

_lw6ker_game_struct_t *
_lw6ker_game_struct_from_hexa (const char *hexa, lw6map_level_t * level)
{
  _lw6ker_game_struct_t *game_struct = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer;
  int ok = 1;

  hexa_serializer = lw6sys_hexa_serializer_new (hexa);
  if (hexa_serializer)
    {
      lw6sys_hexa_serializer_rewind (hexa_serializer);

      game_struct =
	(_lw6ker_game_struct_t *)
	LW6SYS_CALLOC (sizeof (_lw6ker_game_struct_t));

      if (game_struct)
	{
	  _lw6ker_game_struct_set_id (game_struct);
	  game_struct->level = level;
	  lw6map_rules_copy (&(game_struct->rules), &(level->param.rules));
	  ok = ok
	    && pop_map_struct (hexa_serializer, &(game_struct->map_struct));

	  if (lw6sys_shape_is_same
	      (&(game_struct->map_struct.shape), &(level->body.shape)))
	    {
	      if (!lw6sys_hexa_serializer_eof (hexa_serializer))
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("expected EOF in serialized game_struct"));
		  ok = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_
			  ("shape mismatch game_struct=%dx%dx%d level=%dx%dx%d"),
			  game_struct->map_struct.shape.w,
			  game_struct->map_struct.shape.h,
			  game_struct->map_struct.shape.d,
			  level->body.shape.w, level->body.shape.h,
			  level->body.shape.d);
	      ok = 0;
	    }
	}
      lw6sys_hexa_serializer_free (hexa_serializer);
    }

  if (!ok)
    {
      if (game_struct)
	{
	  _lw6ker_game_struct_free (game_struct);
	  game_struct = NULL;
	}
    }

  return game_struct;
}

/**
 * lw6ker_game_struct_from_hexa
 *
 * @hexa: an hexadecimal ASCII string, created by @lw6ker_game_struct_to_hexa
 * @level: the level this game_struct is bounded to
 *
 * Constructs a game struct from an hexadecimal string generated
 * by @lw6ker_game_struct_to_hexa. Just an un-serializer.
 *
 * Return value: a new map, might be NULL if string isn't correct.
 */
lw6ker_game_struct_t *
lw6ker_game_struct_from_hexa (const char *hexa, lw6map_level_t * level)
{
  lw6ker_game_struct_t *game_struct = NULL;

  game_struct =
    (lw6ker_game_struct_t *) _lw6ker_game_struct_from_hexa (hexa, level);

  return game_struct;
}
