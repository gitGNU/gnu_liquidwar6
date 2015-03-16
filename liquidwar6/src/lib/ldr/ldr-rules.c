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

#include <string.h>

#include "ldr.h"
#include "../cfg/cfg.h"
#include "ldr-internal.h"

typedef struct rules_update_data_s
{
  lw6map_rules_t *rules;
  lw6sys_assoc_t *values;
}
rules_update_data_t;

/*
 * One could wonder why this is not in the "opt" module. Basically,
 * doing so would tie the "opt" module to some XML library, and since
 * the "ker" module requires it, we would end up with "ker" requiring
 * an XML library, which is certainly not want we want. Actually
 * "opt" is only here to avoid "ker" depending directly on "map" and
 * its dependencies.
 */

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  lw6map_rules_t *rules_data;
  int int_value;

  rules_data = (lw6map_rules_t *) callback_data;

  if (!strcmp (element, LW6CFG_XML_INT))
    {
      lw6cfg_read_xml_int (sys_context, key, value, key, &int_value);
      lw6map_rules_set_int (sys_context, rules_data, key, int_value);
    }

  if (!strcmp (element, LW6CFG_XML_BOOL))
    {
      lw6cfg_read_xml_bool (sys_context, key, value, key, &int_value);
      lw6map_rules_set_bool (sys_context, rules_data, key, int_value);
    }

  if (!strcmp (element, LW6CFG_XML_FLOAT))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map rules should not use floats, XML entry %s=%s is not correct"), key, value);
    }

  if (!strcmp (element, LW6CFG_XML_STRING))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map rules should not use strings, XML entry %s=%s is not correct"), key, value);
    }
}

/**
 * lw6ldr_rules_read
 *
 * @sys_context: global system context
 * @param: the rules struct to fill with values (read/write parameter)
 * @dirname: the directory of the map
 *
 * Read the rules (rules.xml) of a map. Pointer to rules must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_rules_read (lw6sys_context_t * sys_context, lw6map_rules_t * rules, const char *dirname)
{
  int ret = 0;
  char *buf = NULL;

  buf = lw6sys_path_concat (sys_context, dirname, _LW6LDR_FILE_RULES_XML);

  if (buf)
    {
      if (lw6sys_file_exists (sys_context, buf))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading rules \"%s\""), buf);
	  ret = lw6cfg_read_key_value_xml_file (sys_context, buf, _read_callback, (void *) rules);
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
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to read map rules"));
    }

  return ret;
}

static void
_rules_update_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  rules_update_data_t *update_data;
  char *key;
  char *value;
  char *element;
  lw6hlp_type_t type = LW6HLP_TYPE_VOID;

  key = (char *) data;
  update_data = (rules_update_data_t *) func_data;

  if (lw6sys_assoc_has_key (sys_context, update_data->values, key))
    {
      lw6hlp_about (sys_context, &type, NULL, NULL, NULL, key);
      value = lw6sys_assoc_get (sys_context, update_data->values, key);
      element = lw6cfg_xml_element (sys_context, type);
      _read_callback (sys_context, update_data->rules, element, key, value);
    }
}

/**
 * lw6ldr_rules_update
 *
 * @sys_context: global system context
 * @rules: the rules struct to fill with values (read/write parameter)
 * @values: an assoc containing strings with the new values
 *
 * Overrides rules with values. Pointer to rules must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values. Not all parameters need be defined in values. It can
 * even be NULL. The idea is just that if something is defined in values,
 * it will override the existing rules.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_rules_update (lw6sys_context_t * sys_context, lw6map_rules_t * rules, lw6sys_assoc_t * values)
{
  int ret = 0;
  lw6sys_list_t *list;
  rules_update_data_t data;

  if (values)
    {
      list = lw6hlp_list_map_rules (sys_context);
      if (list)
	{
	  data.rules = rules;
	  data.values = values;
	  lw6sys_list_map (sys_context, list, _rules_update_callback, &data);
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
