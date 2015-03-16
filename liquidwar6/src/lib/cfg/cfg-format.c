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

#include "cfg.h"
#include "cfg-internal.h"

/**
 * lw6cfg_format
 *
 * @sys_context: global system context
 * @key: the key of the value to format
 * @value: the value to format
 * @type: the type of the value to format
 *
 * Formats, converts, a given value to its cannonical representation.
 * Booleans will be converted to true/false, strings containing integers
 * will be stripped from junk, and so on. This is a performance killer
 * but will ensure everything is correct.
 *
 * Return value: a newly allocated string, containing the same as the
 * input, but reformatted the pedantic way.
 */
char *
lw6cfg_format (lw6sys_context_t * sys_context, const char *key, const char *value, lw6hlp_type_t type)
{
  char *ret = NULL;

  if (value)
    {
      switch (type)
	{
	case LW6HLP_TYPE_INT:
	  ret = lw6sys_itoa (sys_context, lw6sys_atoi (sys_context, value));
	  break;
	case LW6HLP_TYPE_FLOAT:
	  ret = lw6sys_ftoa (sys_context, lw6sys_atof (sys_context, value));
	  break;
	case LW6HLP_TYPE_BOOL:
	  ret = lw6sys_btoa (sys_context, lw6sys_atob (sys_context, value));
	  break;
	case LW6HLP_TYPE_COLOR:
	  ret = lw6sys_color_8_to_a (sys_context, lw6sys_color_a_to_8 (sys_context, value));
	  break;
	default:
	  ret = lw6sys_str_copy (sys_context, value);
	  break;
	}
    }

  return ret;
}

/**
 * lw6cfg_format_guess_type
 *
 * @sys_context: global system context
 * @key: the key of the value to format
 * @value: the value to format
 *
 * Formats, converts, a given value to its cannonical representation.
 * Booleans will be converted to true/false, strings containing integers
 * will be stripped from junk, and so on. This is a performance killer
 * but will ensure everything is correct. This function will automatically
 * guess the type of the value from its description in the help system.
 *
 * Return value: a newly allocated string, containing the same as the
 * input, but reformatted the pedantic way.
 */
char *
lw6cfg_format_guess_type (lw6sys_context_t * sys_context, const char *key, const char *value)
{
  char *ret = NULL;
  lw6hlp_type_t type;

  type = lw6hlp_get_type (sys_context, key);
  ret = lw6cfg_format (sys_context, key, value, type);

  return ret;
}
