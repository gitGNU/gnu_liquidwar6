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

#include "msg.h"

/**
 * lw6msg_utils_parse_key_value_to_ptr
 *
 * @sys_context: global system context
 * @key: will contain the key detected
 * @value: will contain the value detected
 * @line: the line to analyse
 *
 * Analyses a trivial "KEY value" line and returns the key and the value in
 * the passed pointers.
 *
 * Return value: 1 if line OK (and in this case @key and @value are set), 0 if not.
 */
int
lw6msg_utils_parse_key_value_to_ptr (lw6sys_context_t * sys_context, char **key, char **value, const char *line)
{
  int ret = 0;
  char *seek = NULL;
  char *pos = NULL;
  char *copy = NULL;
  char seek_c = '\0';
  int found_space = 0;

  (*key) = NULL;
  (*value) = NULL;

  copy = lw6sys_str_copy (sys_context, line);
  if (copy)
    {
      seek = pos = copy;
      while (lw6sys_chr_is_space (*seek))
	{
	  seek++;
	}
      pos = seek;
      while (lw6sys_chr_is_alnum (*seek) || ((*seek) == '_') || ((*seek) == '-'))
	{
	  seek++;
	}
      seek_c = *seek;
      (*seek) = '\0';
      (*key) = lw6sys_str_copy (sys_context, pos);
      (*seek) = seek_c;
      pos = seek;
      while (lw6sys_chr_is_space (*seek) || ((*seek) == ':') || ((*seek) == '='))
	{
	  found_space = 1;
	  seek++;
	}
      pos = seek;
      while ((*seek) && !lw6sys_chr_is_eol (*seek))
	{
	  seek++;
	}
      seek_c = *seek;
      (*seek) = '\0';
      (*value) = lw6sys_str_copy (sys_context, pos);

      if (*key && *value && strlen (*key) > 0)
	{
	  if (found_space || strlen (*value) == 0)
	    {
	      ret = 1;
	    }
	}

      if (!ret)
	{
	  if (*key)
	    {
	      LW6SYS_FREE (sys_context, *key);
	      (*key) = NULL;
	    }
	  if (*value)
	    {
	      LW6SYS_FREE (sys_context, *value);
	      (*value) = NULL;
	    }
	}

      LW6SYS_FREE (sys_context, copy);
    }

  return ret;
}

/**
 * lw6msg_utils_parse_key_value_to_assoc
 *
 * @sys_context: global system context
 * @assoc: an assoc object which will contain the result
 * @line: the line to analyse
 *
 * Analyses a trivial "KEY value" line and sets the @assoc parameter according
 * to detected values. Note that @assoc must be set to contain string, and
 * free them automatically with @lw6sys_free_callback for instance.
 *
 * Return value: 1 if line OK (and in this case @assoc is updated), 0 if not.
 */
int
lw6msg_utils_parse_key_value_to_assoc (lw6sys_context_t * sys_context, lw6sys_assoc_t ** assoc, const char *line)
{
  int ret = 0;
  char *key = NULL;
  char *value = NULL;

  ret = lw6msg_utils_parse_key_value_to_ptr (sys_context, &key, &value, line);
  if (ret)
    {
      if (key && value)
	{
	  lw6sys_assoc_set (sys_context, assoc, key, value);
	  if ((*assoc) == NULL)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("assoc_set failed, assoc is now NULL"));
	    }
	  LW6SYS_FREE (sys_context, key);
	}
    }

  return ret;
}

/**
 * lw6msg_utils_get_assoc_str_with_default
 *
 * @sys_context: global system context
 * @assoc: the string assoc to query
 * @key: the key to find in the assoc
 * @default_value: the default value to return
 *
 * Queries a string assoc for a given value, and if not available, returns
 * default value. Not that default value (nor the assoc value) is copied,
 * so you must take care all remain valid until usage of returned value
 * is over.
 *
 * Return value: a string, must not be freed.
 */
char *
lw6msg_utils_get_assoc_str_with_default (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, const char *key, const char *default_value)
{
  char *ret = NULL;

  ret = lw6sys_assoc_get (sys_context, assoc, key);
  if (!ret)
    {
      ret = (char *) default_value;
    }

  return ret;
}

/**
 * lw6msg_utils_get_assoc_int_with_default
 *
 * @sys_context: global system context
 * @assoc: the string assoc to query
 * @key: the key to find in the assoc
 * @default_value: the default value to return
 *
 * Queries a string assoc for a given value, and if not available, returns
 * default value. Not that default value (nor the assoc value) is copied,
 * so you must take care all remain valid until usage of returned value
 * is over. This one will returned an int converted with @lw6sys_atoi.
 *
 * Return value: a string, must not be freed.
 */
int
lw6msg_utils_get_assoc_int_with_default (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, const char *key, int default_value)
{
  int ret = 0;
  char *tmp = NULL;

  tmp = lw6sys_assoc_get (sys_context, assoc, key);
  if (tmp)
    {
      ret = lw6sys_atoi (sys_context, tmp);
    }
  else
    {
      ret = default_value;
    }

  return ret;
}
