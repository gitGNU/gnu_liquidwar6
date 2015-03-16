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

#include "map.h"

/*
 * This is used to stamp maps as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

/**
 * lw6map_new
 *
 * @sys_context: global system context
 *
 * Creates a new empty map. This object is perfectly unusable as is,
 * since it has a 0x0 size, and many things set to "NULL". Still, it's
 * used internally and is the canonical way to create the object, it
 * ensures later calls that set up default parameters, for instance,
 * will succeed.
 *
 * Return value: a newly allocated pointer.
 */
lw6map_level_t *
lw6map_new (lw6sys_context_t * sys_context)
{
  lw6map_level_t *level = NULL;

  level = (lw6map_level_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_level_t));

  if (level)
    {
      level->id = 0;
      while (!level->id)
	{
	  level->id = ++seq_id;
	}
    }

  return level;
}

/**
 * lw6map_builtin_defaults
 *
 * @sys_context: global system context
 *
 * Creates a map, set to defaults. This is usefull mostly for testing.
 * This builtin map has walls, paths, it's playable.
 *
 * Return value: a newly allocated map.
 */
lw6map_level_t *
lw6map_builtin_defaults (lw6sys_context_t * sys_context)
{
  return lw6map_builtin_custom (sys_context, LW6MAP_BUILTIN_DEFAULT_WIDTH,
				LW6MAP_BUILTIN_DEFAULT_HEIGHT, LW6MAP_BUILTIN_DEFAULT_DEPTH, LW6MAP_BUILTIN_DEFAULT_NOISE_PERCENT);
}

/**
 * lw6map_builtin_scale
 *
 * @sys_context: global system context
 * @percent_factor: how big the map should be, 100 is defaults 200 is double.
 *
 * Creates a map, set to defaults. This is usefull mostly for testing.
 * This builtin map has walls, paths, it's playable, additionnally
 * it's scalable, that's to say one can make it bigger if needed,
 * using a percent factor.
 *
 * Return value: a newly allocated map.
 */
lw6map_level_t *
lw6map_builtin_scale (lw6sys_context_t * sys_context, int percent_factor)
{
  int width = 0;
  int height = 0;
  int depth = 0;

  width = lw6sys_imax (LW6MAP_MIN_BODY_WIDTH, lw6sys_imin (LW6MAP_MAX_BODY_WIDTH, (LW6MAP_BUILTIN_DEFAULT_WIDTH * percent_factor) / 100));
  height = lw6sys_imax (LW6MAP_MIN_BODY_HEIGHT, lw6sys_imin (LW6MAP_MAX_BODY_HEIGHT, (LW6MAP_BUILTIN_DEFAULT_HEIGHT * percent_factor) / 100));
  depth = lw6sys_imax (LW6MAP_MIN_BODY_DEPTH, lw6sys_imin (LW6MAP_MAX_BODY_DEPTH, (LW6MAP_BUILTIN_DEFAULT_DEPTH * percent_factor) / 100));

  return lw6map_builtin_custom (sys_context, width, height, depth, LW6MAP_BUILTIN_DEFAULT_NOISE_PERCENT);
}

/**
 * lw6map_builtin_custom
 *
 * @sys_context: global system context
 * @w: the width of the map
 * @h: the height of the map
 * @d: the depth (number of layers) of the map
 * @noise_percent: percentage of noise to use for metalayers
 *
 * Creates a map, set to defaults. This is usefull mostly for testing.
 * This one, unlike @lw6map_builtin_defaults  will let you give a width,
 * height and a depth.
 *
 * Return value: a newly allocated map.
 */
lw6map_level_t *
lw6map_builtin_custom (lw6sys_context_t * sys_context, int w, int h, int d, int noise_percent)
{
  lw6map_level_t *level = NULL;
  lw6map_color_couple_t color;

  color.fg = LW6SYS_COLOR_8_WHITE;
  color.bg = LW6SYS_COLOR_8_BLACK;

  level = (lw6map_level_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_level_t));
  if (level)
    {
      level->id = 0;
      while (!level->id)
	{
	  level->id = ++seq_id;
	}

      lw6map_param_defaults (sys_context, &level->param);
      /*
       * Default map is a world map with polarity enabled on the X axis
       */
      level->param.rules.x_polarity = 1;
      /*
       * Enable all weapons, this is usefull when testing, we want the
       * complete, full-featured algorithm with all bell and whistles
       * enabled to be tested.
       */
      level->param.rules.exp = LW6MAP_RULES_MAX_EXP;
      level->metadata.vanilla_exp = LW6MAP_RULES_MAX_EXP;
      /*
       * Now proceed with standard construction
       */
      lw6map_body_builtin_custom (sys_context, &level->body, w, h, d, noise_percent, &level->param.rules);
      lw6map_texture_from_body (sys_context, &level->texture, &level->body, &color);
      lw6map_cursor_texture_builtin (sys_context, &level->cursor_texture);
      lw6map_metadata_defaults (sys_context, &level->metadata);
    }

  return level;
}

/**
 * lw6map_free
 *
 * @sys_context: global system context
 * @map: the map to free
 *
 * Frees a map and releases all its internal ressources.
 *
 * Return value: none.
 */
void
lw6map_free (lw6sys_context_t * sys_context, lw6map_level_t * level)
{
  if (level)
    {
      lw6map_texture_clear (sys_context, &level->texture);
      lw6map_body_clear (sys_context, &level->body);
      lw6map_param_clear (sys_context, &level->param);
      lw6map_local_info_clear (sys_context, &level->local_info);
      lw6map_metadata_clear (sys_context, &level->metadata);

      LW6SYS_FREE (sys_context, level);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL map"));
    }
}

/**
 * lw6map_memory_footprint
 *
 * @sys_context: global system context
 * @map: the map to query
 *
 * Reports how many bytes the map needs, in memory. Note that this is
 * not contiguous memory, it involves a bunch of pointers, and possibly
 * much more...
 */
int
lw6map_memory_footprint (lw6sys_context_t * sys_context, const lw6map_level_t * level)
{
  int memory_footprint = 0;

  if (level)
    {
      memory_footprint += sizeof (lw6map_level_t);
      memory_footprint += strlen (level->metadata.title) + 1;
      memory_footprint += strlen (level->metadata.author) + 1;
      memory_footprint += strlen (level->metadata.description) + 1;
      memory_footprint += strlen (level->metadata.license) + 1;
      memory_footprint += level->body.shape.w * level->body.shape.h * level->body.shape.d * sizeof (u_int8_t);
      memory_footprint += level->texture.w * level->texture.h * sizeof (lw6sys_color_8_t);

    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling memory_footprint on NULL map"));
    }

  return memory_footprint;
}

/**
 * lw6map_repr
 *
 * @sys_context: global system context
 * @map: the map to describe
 *
 * Returns a string describing the map. This is a very short description,
 * use it for logs, and to debug stuff. By no means it's a complete exhaustive
 * description. Still, the string returned should be unique.
 *
 * Return value: a dynamically allocated string.
 */
char *
lw6map_repr (lw6sys_context_t * sys_context, const lw6map_level_t * level)
{
  char *ret = NULL;

  if (level)
    {
      const char *title = NULL;

      title = lw6map_get_title (sys_context, level);
      ret = lw6sys_new_sprintf (sys_context, "%u \"%s\" (%dx%dx%d)", level->id, title, level->body.shape.w, level->body.shape.h, level->body.shape.d);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't generate string id for NULL level"));
    }

  return ret;
}

/**
 * lw6map_is_same
 *
 * @sys_context: global system context
 * @level_a: the first level to compare
 * @level_b: the other level to compare
 *
 * Compares two level structs, the idea is to compare the content, not
 * only the pointers and level ids.
 *
 * Return value: 1 if they're the same, 0 if not.
 */
int
lw6map_is_same (lw6sys_context_t * sys_context, const lw6map_level_t * level_a, const lw6map_level_t * level_b)
{
  int ret = 1;

  if (((!level_a) && level_b) || (level_a && (!level_b)))
    {
      ret = 0;
    }
  if (level_a && level_b)
    {
      /*
       * We don't compare the whole map (performance...) but assume if
       * the metadata is the same, all the parameters are the same,
       * the shape is the same, and the body checksum is the same ->
       * it's the same map
       */
      ret = ret && lw6map_metadata_is_same (sys_context, &(level_a->metadata), &(level_b->metadata));
      ret = ret && lw6sys_shape_is_same (sys_context, &(level_a->body.shape), &(level_b->body.shape));
      ret = ret && lw6map_param_is_same (sys_context, &(level_a->param), &(level_b->param));

      ret = ret && (level_a->body.checksum == level_b->body.checksum);
    }

  return ret;
}

/**
 * lw6map_get_title
 *
 * @sys_context: global system context
 * @level: level to get informations about
 *
 * Gives the map name. This is just a simple utility/wrapper function
 * which is meant to be exported to Guile scripts.
 *
 * Return value: static string, must not be freed, can't be NULL
 */
const char *
lw6map_get_title (lw6sys_context_t * sys_context, const lw6map_level_t * level)
{
  const char *ret = NULL;

  ret = level->metadata.title;
  if (!ret)
    {
      ret = _x_ ("No title");
    }

  return ret;
}

/**
 * lw6map_get_max_nb_colors
 *
 * @level: level to get informations about
 *
 * Gives the max number of colors (AKA teams) that can fit
 * on this map. This is just a simple utility/wrapper function
 * which is meant to be exported to Guile scripts.
 *
 * Return value: number of colors, taken from rules
 */
int
lw6map_get_max_nb_colors (lw6sys_context_t * sys_context, const lw6map_level_t * level)
{
  return level->param.rules.max_nb_teams;
}

/**
 * lw6map_get_max_nb_cursors
 *
 * @sys_context: global system context
 * @level: level to get informations about
 *
 * Gives the max number of cursors that can fit
 * on this map. This is just a simple utility/wrapper function
 * which is meant to be exported to Guile scripts.
 *
 * Return value: number of cursors, taken from rules
 */
int
lw6map_get_max_nb_cursors (lw6sys_context_t * sys_context, const lw6map_level_t * level)
{
  return level->param.rules.max_nb_cursors;
}

/**
 * lw6map_get_max_nb_nodes
 *
 * @sys_context: global system context
 * @level: level to get informations about
 *
 * Gives the max number of nodes that can fit
 * on this map. This is just a simple utility/wrapper function
 * which is meant to be exported to Guile scripts.
 *
 * Return value: number of nodes, taken from rules
 */
int
lw6map_get_max_nb_nodes (lw6sys_context_t * sys_context, const lw6map_level_t * level)
{
  return level->param.rules.max_nb_nodes;
}
