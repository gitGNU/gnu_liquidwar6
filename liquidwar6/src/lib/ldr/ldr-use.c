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

typedef struct use_update_data_s
{
  lw6ldr_use_t *use;
  lw6sys_assoc_t *values;
}
use_update_data_t;

/**
 * lw6ldr_use_defaults
 *
 * @sys_context: global system context
 * @use: struct to initialize
 *
 * Sets the use structure to its defaults, this structure
 * being used to now wether we should use texture, cursor textures,
 * rules, hints, style, teams and music.
 *
 * Return value: none.
 */
void
lw6ldr_use_defaults (lw6sys_context_t * sys_context, lw6ldr_use_t * use)
{
  use->use_texture = LW6LDR_USE_DEFAULT_USE_TEXTURE;
  use->use_cursor_texture = LW6LDR_USE_DEFAULT_USE_CURSOR_TEXTURE;
  use->use_rules_xml = LW6LDR_USE_DEFAULT_USE_RULES_XML;
  use->use_hints_xml = LW6LDR_USE_DEFAULT_USE_HINTS_XML;
  use->use_style_xml = LW6LDR_USE_DEFAULT_USE_STYLE_XML;
  use->use_teams_xml = LW6LDR_USE_DEFAULT_USE_TEAMS_XML;
  use->use_music_file = LW6LDR_USE_DEFAULT_USE_MUSIC_FILE;
}

/**
 * lw6ldr_use_clear
 *
 * @sys_context: global system context
 * @use: struct to clear
 *
 * Clears the use structure, set it to the use nothing mode.
 *
 * Return value: none.
 */
void
lw6ldr_use_clear (lw6sys_context_t * sys_context, lw6ldr_use_t * use)
{
  memset (use, 0, sizeof (lw6ldr_use_t));
}

/**
 * lw6ldr_use_set
 *
 * @sys_context: global system context
 * @use: struct to modify
 * @key: key to change (as a string)
 * @value: value to set (as a string)
 *
 * Sets a key to the given value, OK all fields are integer,
 * this is just a convenient function to be called in more
 * general functions which are fed with those string pointers,
 * typically coming from an XML file.
 *
 * Return value: 1 on success, 0 on failure (key not found).
 */
int
lw6ldr_use_set (lw6sys_context_t * sys_context, lw6ldr_use_t * use, const char *key, const char *value)
{
  int ret = 1;

  if (lw6sys_arg_match (sys_context, LW6DEF_USE_TEXTURE, key))
    {
      use->use_texture = lw6sys_atob (sys_context, value);
    }
  else if (lw6sys_arg_match (sys_context, LW6DEF_USE_CURSOR_TEXTURE, key))
    {
      use->use_cursor_texture = lw6sys_atob (sys_context, value);
    }
  else if (lw6sys_arg_match (sys_context, LW6DEF_USE_RULES_XML, key))
    {
      use->use_rules_xml = lw6sys_atob (sys_context, value);
    }
  else if (lw6sys_arg_match (sys_context, LW6DEF_USE_HINTS_XML, key))
    {
      use->use_hints_xml = lw6sys_atob (sys_context, value);
    }
  else if (lw6sys_arg_match (sys_context, LW6DEF_USE_STYLE_XML, key))
    {
      use->use_style_xml = lw6sys_atob (sys_context, value);
    }
  else if (lw6sys_arg_match (sys_context, LW6DEF_USE_TEAMS_XML, key))
    {
      use->use_teams_xml = lw6sys_atob (sys_context, value);
    }
  else if (lw6sys_arg_match (sys_context, LW6DEF_USE_MUSIC_FILE, key))
    {
      use->use_music_file = lw6sys_atob (sys_context, value);
    }
  else
    {
      ret = 0;
    }

  return ret;
}

static void
_use_update_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  use_update_data_t *update_data;
  char *key;
  char *value;

  key = (char *) data;
  update_data = (use_update_data_t *) func_data;

  if (lw6sys_assoc_has_key (sys_context, update_data->values, key))
    {
      value = lw6sys_assoc_get (sys_context, update_data->values, key);
      lw6ldr_use_set (sys_context, update_data->use, key, value);
    }
}

/**
 * lw6ldr_use_update
 *
 * @sys_context: global system context
 * @use: the use struct to fill with values (read/write parameter)
 * @values: an assoc containing strings with the new values
 *
 * Overrides use with values. Pointer to use must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values. Not all parameters need be defined in values. It can
 * even be NULL. The idea is just that if something is defined in values,
 * it will override the existing use.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_use_update (lw6sys_context_t * sys_context, lw6ldr_use_t * use, lw6sys_assoc_t * values)
{
  int ret = 0;
  lw6sys_list_t *list;
  use_update_data_t data;

  if (values)
    {
      list = lw6hlp_list_map (sys_context);	// contains use switches
      if (list)
	{
	  data.use = use;
	  data.values = values;
	  lw6sys_list_map (sys_context, list, _use_update_callback, &data);
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
