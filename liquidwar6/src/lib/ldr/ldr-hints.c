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

#include <dirent.h>

#include "ldr.h"
#include "ldr-internal.h"

typedef struct hints_update_data_s
{
  lw6ldr_hints_t *hints;
  lw6sys_assoc_t *values;
}
hints_update_data_t;

/**
 * lw6ldr_hints_defaults
 *
 * @sys_context: global system context
 * @hints: data to initialize
 *
 * Set the hints struct to its defaults.
 *
 * Return value: none.
 */
void
lw6ldr_hints_defaults (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints)
{
  hints->resample = LW6LDR_HINTS_DEFAULT_RESAMPLE;
  hints->min_map_width = LW6LDR_HINTS_DEFAULT_MIN_MAP_WIDTH;
  hints->max_map_width = LW6LDR_HINTS_DEFAULT_MAX_MAP_WIDTH;
  hints->min_map_height = LW6LDR_HINTS_DEFAULT_MIN_MAP_HEIGHT;
  hints->max_map_height = LW6LDR_HINTS_DEFAULT_MAX_MAP_HEIGHT;
  hints->min_map_surface = LW6LDR_HINTS_DEFAULT_MIN_MAP_SURFACE;
  hints->max_map_surface = LW6LDR_HINTS_DEFAULT_MAX_MAP_SURFACE;
  hints->fighter_scale = LW6LDR_HINTS_DEFAULT_FIGHTER_SCALE;
  hints->downsize_using_fighter_scale = LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_FIGHTER_SCALE;
  hints->upsize_using_fighter_scale = LW6LDR_HINTS_DEFAULT_UPSIZE_USING_FIGHTER_SCALE;
  hints->downsize_using_bench_value = LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_BENCH_VALUE;
  hints->upsize_using_bench_value = LW6LDR_HINTS_DEFAULT_UPSIZE_USING_BENCH_VALUE;
  hints->guess_colors = LW6LDR_HINTS_DEFAULT_GUESS_COLORS;
  hints->background_color_auto = LW6LDR_HINTS_DEFAULT_BACKGROUND_COLOR_AUTO;
  hints->hud_color_auto = LW6LDR_HINTS_DEFAULT_HUD_COLOR_AUTO;
  hints->menu_color_auto = LW6LDR_HINTS_DEFAULT_MENU_COLOR_AUTO;
  hints->view_color_auto = LW6LDR_HINTS_DEFAULT_VIEW_COLOR_AUTO;
  hints->system_color_auto = LW6LDR_HINTS_DEFAULT_SYSTEM_COLOR_AUTO;
  hints->wall_grease = LW6LDR_HINTS_DEFAULT_WALL_GREASE;
  hints->guess_moves_per_sec = LW6LDR_HINTS_DEFAULT_GUESS_MOVES_PER_SEC;
  hints->speed = LW6LDR_HINTS_DEFAULT_SPEED;
}

/**
 * lw6ldr_hints_zero
 *
 * @sys_context: global system context
 * @hints: data to initialize
 *
 * Zeros the hints struct, this is not the same as setting to defaults.
 *
 * Return value: none.
 */
void
lw6ldr_hints_zero (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints)
{
  memset (hints, 0, sizeof (lw6ldr_hints_t));
}

/**
 * lw6ldr_hints_clear
 *
 * @sys_context: global system context
 * @hints: data to initialize
 *
 * Clears the hints struct, this is not the same as setting to defaults.
 *
 * Return value: none.
 */
void
lw6ldr_hints_clear (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints)
{
  lw6ldr_hints_zero (sys_context, hints);
}

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  lw6ldr_hints_t *hints_data;

  hints_data = (lw6ldr_hints_t *) callback_data;

  lw6ldr_hints_set (sys_context, hints_data, key, value);
}

/**
 * lw6ldr_hints_read
 *
 * @sys_context: global system context
 * @param: the hints struct to fill with values (read/write parameter)
 * @dirname: the directory of the map
 *
 * Read the hints (hints.xml) of a map. Pointer to hints must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_hints_read (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints, const char *dirname)
{
  int ret = 0;
  char *buf = NULL;

  buf = lw6sys_path_concat (sys_context, dirname, _LW6LDR_FILE_HINTS_XML);

  if (buf)
    {
      if (lw6sys_file_exists (sys_context, buf))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading hints \"%s\""), buf);
	  ret = lw6cfg_read_key_value_xml_file (sys_context, buf, _read_callback, (void *) hints);
	}
      else
	{
	  // using defaults
	  ret = 1;
	}

      LW6SYS_FREE (sys_context, buf);
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to read map hints"));
    }

  return ret;
}

/**
 * lw6ldr_hints_set
 *
 * @sys_context: global system context
 * @hints: the hints to modify
 * @key: the key to modify
 * @value: the value to affect to the key, as a string
 *
 * Sets one single parameter in a hints structure. Value must
 * always be passed as a string, will be converted to the right
 * type automatically when storing it in the structure.
 *
 * Return value: 1 if success, 0 if failed. Note that while 0 really
 * means there's a problem, some affectations can fail and return 1,
 * needs to be worked on.
 */
int
lw6ldr_hints_set (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints, const char *key, const char *value)
{
  int ret = 1;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (sys_context, key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_RESAMPLE, formatted_key))
	{
	  hints->resample = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MIN_MAP_WIDTH, formatted_key))
	{
	  hints->min_map_width = lw6sys_atoi (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MAX_MAP_WIDTH, formatted_key))
	{
	  hints->max_map_width = lw6sys_atoi (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MIN_MAP_HEIGHT, formatted_key))
	{
	  hints->min_map_height = lw6sys_atoi (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MAX_MAP_HEIGHT, formatted_key))
	{
	  hints->max_map_height = lw6sys_atoi (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MIN_MAP_SURFACE, formatted_key))
	{
	  hints->min_map_surface = lw6sys_atoi (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MAX_MAP_SURFACE, formatted_key))
	{
	  hints->max_map_surface = lw6sys_atoi (sys_context, value);
	}
      else if (!strcmp (LW6DEF_FIGHTER_SCALE, formatted_key))
	{
	  hints->fighter_scale = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_DOWNSIZE_USING_FIGHTER_SCALE, formatted_key))
	{
	  hints->downsize_using_fighter_scale = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_UPSIZE_USING_FIGHTER_SCALE, formatted_key))
	{
	  hints->upsize_using_fighter_scale = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_DOWNSIZE_USING_BENCH_VALUE, formatted_key))
	{
	  hints->downsize_using_bench_value = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_UPSIZE_USING_BENCH_VALUE, formatted_key))
	{
	  hints->upsize_using_bench_value = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_GUESS_COLORS, formatted_key))
	{
	  hints->guess_colors = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_AUTO, formatted_key))
	{
	  hints->background_color_auto = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_AUTO, formatted_key))
	{
	  hints->hud_color_auto = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_AUTO, formatted_key))
	{
	  hints->menu_color_auto = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_AUTO, formatted_key))
	{
	  hints->view_color_auto = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_SYSTEM_COLOR_AUTO, formatted_key))
	{
	  hints->system_color_auto = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_WALL_GREASE, formatted_key))
	{
	  hints->wall_grease = lw6sys_atoi (sys_context, value);
	}
      else if (!strcmp (LW6DEF_GUESS_MOVES_PER_SEC, formatted_key))
	{
	  hints->guess_moves_per_sec = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_SPEED, formatted_key))
	{
	  hints->speed = lw6sys_atof (sys_context, value);
	}
      else
	{
	  ret = 0;
	}
      LW6SYS_FREE (sys_context, formatted_key);
    }
  else
    {
      ret = 0;
    }

  return ret;
}

/**
 * lw6ldr_hints_get
 *
 * @sys_context: global system context
 * @hints: the hints to modify
 * @key: the key to modify
 *
 * Gets one single parameter in a hints structure. Value is
 * converted as a string.
 *
 * Return value: dynamically allocated string, NULL on error.
 */
char *
lw6ldr_hints_get (lw6sys_context_t * sys_context, const lw6ldr_hints_t * hints, const char *key)
{
  char *ret = NULL;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (sys_context, key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_RESAMPLE, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->resample);
	}
      else if (!strcmp (LW6DEF_MIN_MAP_WIDTH, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, hints->min_map_width);
	}
      else if (!strcmp (LW6DEF_MAX_MAP_WIDTH, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, hints->max_map_width);
	}
      else if (!strcmp (LW6DEF_MIN_MAP_HEIGHT, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, hints->min_map_height);
	}
      else if (!strcmp (LW6DEF_MAX_MAP_HEIGHT, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, hints->max_map_height);
	}
      else if (!strcmp (LW6DEF_MIN_MAP_SURFACE, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, hints->min_map_surface);
	}
      else if (!strcmp (LW6DEF_MAX_MAP_SURFACE, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, hints->max_map_surface);
	}
      else if (!strcmp (LW6DEF_FIGHTER_SCALE, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, hints->fighter_scale);
	}
      else if (!strcmp (LW6DEF_DOWNSIZE_USING_FIGHTER_SCALE, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->downsize_using_fighter_scale);
	}
      else if (!strcmp (LW6DEF_UPSIZE_USING_FIGHTER_SCALE, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->upsize_using_fighter_scale);
	}
      else if (!strcmp (LW6DEF_DOWNSIZE_USING_BENCH_VALUE, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->downsize_using_bench_value);
	}
      else if (!strcmp (LW6DEF_UPSIZE_USING_BENCH_VALUE, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->upsize_using_bench_value);
	}
      else if (!strcmp (LW6DEF_GUESS_COLORS, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->guess_colors);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_AUTO, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->background_color_auto);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_AUTO, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->hud_color_auto);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_AUTO, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->menu_color_auto);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_AUTO, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->view_color_auto);
	}
      else if (!strcmp (LW6DEF_SYSTEM_COLOR_AUTO, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->system_color_auto);
	}
      else if (!strcmp (LW6DEF_WALL_GREASE, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, hints->wall_grease);
	}
      else if (!strcmp (LW6DEF_GUESS_MOVES_PER_SEC, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, hints->guess_moves_per_sec);
	}
      else if (!strcmp (LW6DEF_SPEED, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, hints->speed);
	}
      LW6SYS_FREE (sys_context, formatted_key);
    }

  return ret;
}

/**
 * lw6ldr_hints_get_default
 *
 * @sys_context: global system context
 * @key: the key we want informations about.
 *
 * Gets the default value for a given hints key.
 *
 * Return value: dynamically allocated string, NULL on error.
 */
char *
lw6ldr_hints_get_default (lw6sys_context_t * sys_context, const char *key)
{
  lw6ldr_hints_t *hints;
  char *ret = NULL;

  hints = (lw6ldr_hints_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6ldr_hints_t));
  if (hints)
    {
      lw6ldr_hints_defaults (sys_context, hints);
      ret = lw6ldr_hints_get (sys_context, hints, key);
      lw6ldr_hints_clear (sys_context, hints);
      LW6SYS_FREE (sys_context, hints);
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get default value for hints parameter \"%s\""), key);
    }

  return ret;
}

static void
_hints_update_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  hints_update_data_t *update_data;
  char *key;
  char *value;

  key = (char *) data;
  update_data = (hints_update_data_t *) func_data;

  if (lw6sys_assoc_has_key (sys_context, update_data->values, key))
    {
      value = lw6sys_assoc_get (sys_context, update_data->values, key);
      lw6ldr_hints_set (sys_context, update_data->hints, key, value);
    }
}

/**
 * lw6ldr_hints_update
 *
 * @sys_context: global system context
 * @hints: the hints struct to fill with values (read/write parameter)
 * @values: an assoc containing strings with the new values
 *
 * Overrides hints with values. Pointer to hints must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values. Not all parameters need be defined in values. It can
 * even be NULL. The idea is just that if something is defined in values,
 * it will override the existing hints.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_hints_update (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints, lw6sys_assoc_t * values)
{
  int ret = 0;
  lw6sys_list_t *list;
  hints_update_data_t data;

  if (values)
    {
      list = lw6hlp_list_map_hints (sys_context);
      if (list)
	{
	  data.hints = hints;
	  data.values = values;
	  lw6sys_list_map (sys_context, list, _hints_update_callback, &data);
	  lw6sys_list_free (sys_context, list);
	  ret = 1;
	}
    }
  else
    {
      // NULL values is allowed
      ret = 1;
    }

  return ret;
}
