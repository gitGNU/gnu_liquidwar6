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

#include "sys.h"

/**
 * lw6sys_atoi
 *
 * @sys_context: global system context
 * @str: string to convert
 *
 * Just a plain wrapper on @atoi, it's here for API consistency.
 * Will check if str is NULL (and in this case return 0).
 *
 * Return value: an integer.
 */
int
lw6sys_atoi (lw6sys_context_t * sys_context, const char *str)
{
  return (str != NULL) ? atoi (str) : 0;
}

/**
 * lw6sys_atoll
 *
 * @sys_context: global system context
 * @str: string to convert
 *
 * Wrapper on @atoll, it's here for API consistency.
 * Will check if str is NULL (and in this case return 0).
 *
 * Return value: a 64-bit integer.
 */
int64_t
lw6sys_atoll (lw6sys_context_t * sys_context, const char *str)
{
  return (str != NULL) ? atoll (str) : 0L;
}

/**
 * lw6sys_atob
 *
 * @sys_context: global system context
 * @str: string to convert
 *
 * Transform a string into a boolean value. Accepts "0"/"1" in input,
 * but also y/n, yes/no, true/false, on/off.
 * Will check if str is NULL (and in this case return 0).
 *
 * Return value: an integer, 0 or 1.
 */
int
lw6sys_atob (lw6sys_context_t * sys_context, const char *str)
{
  int ret = 0;

  if (str != NULL)
    {
      ret = (strcasecmp (str, "true") == 0
	     || strcasecmp (str, "y") == 0
	     || strcasecmp (str, "yes") == 0 || strcasecmp (str, "on") == 0 || strlen (str) == 0 || lw6sys_atoi (sys_context, str) > 0);
    }

  return ret;
}

/**
 * lw6sys_atof
 *
 * @sys_context: global system context
 * @str: string to convert
 *
 * A wrapper on @atof, makes sure the locale used is C (default)
 * and won't change the decimal separator whatsoever. Usefull for
 * serialization for instance.
 * Will check if str is NULL (and in this case return 0).
 *
 * Return value: a float.
 */
float
lw6sys_atof (lw6sys_context_t * sys_context, const char *str)
{
  float ret = 0.0f;
  char *locale;
  char *old_locale;

  if (str != NULL)
    {
      locale = setlocale (LC_ALL, NULL);
      if (locale)
	{
	  /*
	   * We do need to make a copy in a separate buffer,
	   * otherwise the content pointed by *locale
	   * might change dynamically when calling setlocale
	   */
	  old_locale = lw6sys_str_copy (sys_context, locale);

	  setlocale (LC_ALL, "C");

	  ret = atof (str);

	  setlocale (LC_ALL, old_locale);
	  if (old_locale)
	    {
	      LW6SYS_FREE (sys_context, old_locale);
	    }
	}
    }

  return ret;
}

/**
 * lw6sys_itoa
 *
 * @sys_context: global system context
 * @value: the integer to convert
 *
 * Converts an integer to a string, the advantage of this function
 * is it allocates memory, and does the dirty job.
 *
 * Return value: a newly allocated pointer, must be freed, may be NULL.
 */
char *
lw6sys_itoa (lw6sys_context_t * sys_context, int value)
{
  return lw6sys_new_sprintf (sys_context, "%d", value);
}

/**
 * lw6sys_lltoa
 *
 * @value: the integer to convert
 *
 * Converts a 64-bit integer to a string, the advantage of this function
 * is it allocates memory, and does the dirty job.
 *
 * Return value: a newly allocated pointer, must be freed, may be NULL.
 */
char *
lw6sys_lltoa (lw6sys_context_t * sys_context, int64_t value)
{
  return lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d", (long long) value);
}

/**
 * lw6sys_btoa
 *
 * @sys_context: global system context
 * @value: the boolean to convert
 *
 * Converts a boolean to a string, the advantage of this function
 * is it allocates memory, and does the dirty job.
 *
 * Return value: a newly allocated pointer, must be freed, may be NULL.
 */
char *
lw6sys_btoa (lw6sys_context_t * sys_context, int value)
{
  char *ret = NULL;

  ret = value ? lw6sys_new_sprintf (sys_context, "%s", "true") : lw6sys_new_sprintf (sys_context, "%s", "false");

  return ret;
}

/**
 * lw6sys_ftoa
 *
 * @sys_context: global system context
 * @value: the float to convert
 *
 * Converts a float to a string, the advantage of this function
 * is it allocates memory, and does the dirty job.
 *
 * Return value: a newly allocated pointer, must be freed, may be NULL.
 */
char *
lw6sys_ftoa (lw6sys_context_t * sys_context, float value)
{
  char *ret = NULL;
  char *locale;
  char *old_locale;

  locale = setlocale (LC_ALL, NULL);
  if (locale)
    {
      /*
       * We do need to make a copy in a separate buffer,
       * otherwise the content pointed by *locale
       * might change dynamically when calling setlocale
       */
      old_locale = lw6sys_str_copy (sys_context, locale);

      setlocale (LC_ALL, "C");

      ret = lw6sys_new_sprintf (sys_context, "%f", value);

      setlocale (LC_ALL, old_locale);
      if (old_locale)
	{
	  LW6SYS_FREE (sys_context, old_locale);
	}
    }

  return ret;
}
