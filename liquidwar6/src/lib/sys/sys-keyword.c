/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#include <string.h>
#include <ctype.h>

#include "sys.h"

#define ENV_PREFIX "LW6_"
#define ENV_MASK ENV_PREFIX "%s"
#define ARG_MASK "--%s"

static int
is_sep (char c)
{
  return (lw6sys_chr_is_space (c) || c == '-' || c == '_' || c == '/');
}

static int
is_c (char c)
{
  return (lw6sys_chr_is_alnum (c) || (c == '?'));
}

static int
is_equal (char c)
{
  return (c == '=');
}

static void
skip_prefix (const char *keyword, int *pos)
{
  /*
   * Only 2 separators at beginning, more leads to confusion...
   */
  if (is_sep (keyword[*pos]))
    {
      (*pos)++;
    }
  if (is_sep (keyword[*pos]))
    {
      (*pos)++;
    }

  if (strncasecmp (keyword + (*pos), ENV_PREFIX, strlen (ENV_PREFIX)) == 0)
    {
      (*pos) += strlen (ENV_PREFIX);
    }
}


/**
 * lw6sys_keyword_as_key
 *
 * @keyword: the keyword to transform
 *
 * Transforms a keyword into a "key", that is, removes all heading dashes,
 * switches to lowercase, and other stuff. This is used internally
 * to match options and config file parameters, for instance.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_keyword_as_key (const char *keyword)
{
  char *ret = NULL;
  int pos_i = 0;
  char *pos_p = NULL;

  skip_prefix (keyword, &pos_i);

  ret = lw6sys_str_copy (keyword + pos_i);
  if (ret)
    {
      pos_p = ret;
      while (pos_p[0])
	{
	  if (is_c (pos_p[0]))
	    {
	      pos_p[0] = tolower (pos_p[0]);
	    }
	  else if (is_sep (pos_p[0]))
	    {
	      pos_p[0] = '-';
	    }
	  else if (is_equal (pos_p[0]))
	    {
	      pos_p[0] = '\0';
	    }
	  else
	    {
	      lw6sys_log_critical (_x_
				   ("weird char '%c' in keyword \"%s\""),
				   pos_p[0], keyword);
	    }
	  pos_p++;
	}
    }

  return ret;
}

/**
 * lw6sys_keyword_as_arg
 *
 * @keyword: the keyword to transform
 *
 * Transforms a keyword into a command-line parameter to be matched.
 * Does the same as @lw6sys_keyword_as_key, and adds a "--" prefix.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_keyword_as_arg (const char *keyword)
{
  char *ret = NULL;
  char *key = NULL;

  key = lw6sys_keyword_as_key (keyword);
  if (key)
    {
      ret = lw6sys_new_sprintf (ARG_MASK, key);
      LW6SYS_FREE (key);
    }

  return ret;
}

/**
 * lw6sys_keyword_as_env
 *
 * @keyword: the keyword to transform
 *
 * Transforms a keyword into the corresponding environment variable name.
 * It will uppercase the name, replace "-" by "_", and add a "LW6_"
 * prefix. "my-param" will become "LW6_MY_PARAM".
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_keyword_as_env (const char *keyword)
{
  char *ret = NULL;
  int pos_i = 0;
  char *pos_p = NULL;

  skip_prefix (keyword, &pos_i);

  ret = lw6sys_new_sprintf (ENV_MASK, keyword + pos_i);
  if (ret)
    {
      pos_p = ret;
      while (pos_p[0])
	{
	  if (is_c (pos_p[0]))
	    {
	      pos_p[0] = toupper (pos_p[0]);
	    }
	  else if (is_sep (pos_p[0]))
	    {
	      pos_p[0] = '_';
	    }
	  else if (is_equal (pos_p[0]))
	    {
	      pos_p[0] = '\0';
	    }
	  else
	    {
	      lw6sys_log_critical (_x_
				   ("weird char '%c' in keyword \"%s\""),
				   pos_p[0], keyword);
	    }
	  pos_p++;
	}
    }

  return ret;
}

/**
 * lw6sys_keyword_as_xml
 *
 * @keyword: the keyword to transform
 *
 * Transforms a keyword into the corresponding config file entry.
 * In practice, just the same as @lw6sys_keyword_as_key.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_keyword_as_xml (const char *keyword)
{
  char *ret = NULL;

  ret = lw6sys_keyword_as_key (keyword);

  return ret;
}
