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

#include "hlp.h"
#include "hlp-internal.h"

static const char *
_about (lw6sys_context_t * sys_context, lw6hlp_type_t * type, const char **default_value, int *min_value, int *max_value, const char *keyword)
{
  const char *ret = NULL;
  _lw6hlp_reference_entry_t *entry = NULL;

  entry = _lw6hlp_reference_get_entry (sys_context, keyword);
  if (entry)
    {
      if (type)
	{
	  *type = entry->type;
	}
      if (default_value)
	{
	  *default_value = entry->default_value;
	}
      if (min_value)
	{
	  *min_value = entry->min_value;
	}
      if (max_value)
	{
	  *max_value = entry->max_value;
	}
      ret = entry->about;
    }

  return ret;
}

/**
 * lw6hlp_is_documented
 *
 * @sys_context: global system context
 * @keyword: the keyword we want to check out
 *
 * Checks wether a given keyword is documented or not.
 *
 * Return value: 1 if documented, 0 if not.
 */
int
lw6hlp_is_documented (lw6sys_context_t * sys_context, const char *keyword)
{
  int ret = 0;

  ret = (_about (sys_context, NULL, NULL, NULL, NULL, keyword) != NULL);

  return ret;
}

/**
 * lw6hlp_about
 *
 * @sys_context: global system context
 * @type: the type of the data associated to the keyword, will be written
 * @default_value: the default value for the keyword, will be written
 * @min_value: the min value for the keyword, will be written
 * @max_value: the max value for the keyword, will be written
 * @keyword: the keyword we want help about
 *
 * Returns the documentation string associated to a keyword. The keyword
 * might be a command-line option, a Guile function, an XML file entry.
 * Raises a warning if the keyword is undocumented, but never returns
 * NULL, you can use the returned value without checking it. String
 * is localized if a translation is available. It's safe to call this
 * function with type or other parameters being NULL.
 *
 * Return value: a help string, never NULL, must not be freed.
 *   Additionnally, type will be updated.
 */
const char *
lw6hlp_about (lw6sys_context_t * sys_context, lw6hlp_type_t * type, const char **default_value, int *min_value, int *max_value, const char *keyword)
{
  const char *ret = NULL;

  ret = _about (sys_context, type, default_value, min_value, max_value, keyword);

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("keyword '%s' is not documented"), keyword);
      if (type)
	{
	  *type = LW6HLP_TYPE_VOID;
	}
      if (default_value)
	{
	  *default_value = NULL;
	}
      if (min_value)
	{
	  *min_value = 0;
	}
      if (max_value)
	{
	  *max_value = 0;
	}
      ret = _x_ ("Not documented.");
    }

  return ret;
}

/**
 * lw6hlp_get_type
 *
 * @sys_context: global system context
 * @keyword: the keyword we want the type of
 *
 * Returns the type of a keyword. Calls lw6hlp_about internally.
 *
 * Return value: the type, might be LW6HLP_TYPE_VOID.
 */
lw6hlp_type_t
lw6hlp_get_type (lw6sys_context_t * sys_context, const char *keyword)
{
  lw6hlp_type_t ret = LW6HLP_TYPE_VOID;

  lw6hlp_about (sys_context, &ret, NULL, NULL, NULL, keyword);

  return ret;
}

/**
 * lw6hlp_get_default_value
 *
 * @sys_context: global system context
 * @keyword: the keyword we want the default for
 *
 * Returns the default value for a keyword. Note that it can be NULL!
 * The returned value is always a string, it's suitable to store in
 * the config file, it's the value a user would pass on a command line,
 * the one he wants documented.
 *
 * Return value: a pointer, which can be NULL, must not be freed.
 */
const char *
lw6hlp_get_default_value (lw6sys_context_t * sys_context, const char *keyword)
{
  const char *ret = NULL;

  lw6hlp_about (sys_context, NULL, &ret, NULL, NULL, keyword);

  return ret;
}

/**
 * lw6hlp_get_min_value
 *
 * @sys_context: global system context
 * @keyword: the keyword we want the min for
 *
 * Returns the min value for a keyword. Wether this is relevant for a
 * given keyword does not affect the fact that you can call this function.
 * A min and max of zero means min and max make no sense.
 *
 * Return value: the value (integer)
 */
int
lw6hlp_get_min_value (lw6sys_context_t * sys_context, const char *keyword)
{
  int ret = 0;

  lw6hlp_about (sys_context, NULL, NULL, &ret, NULL, keyword);

  return ret;
}

/**
 * lw6hlp_get_max_value
 *
 * @sys_context: global system context
 * @keyword: the keyword we want the max for
 *
 * Returns the max value for a keyword. Wether this is relevant for a
 * given keyword does not affect the fact that you can call this function.
 * A min and max of zero means min and max make no sense.
 *
 * Return value: the value (integer)
 */
int
lw6hlp_get_max_value (lw6sys_context_t * sys_context, const char *keyword)
{
  int ret = 0;

  lw6hlp_about (sys_context, NULL, NULL, NULL, &ret, keyword);

  return ret;
}
