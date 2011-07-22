/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "sys.h"

#define MATCH_EXACT_FORMAT_SIZE 4
#define MATCH_EXACT_FORMAT_1 "%s"
#define MATCH_EXACT_FORMAT_2 "-%s"
#define MATCH_EXACT_FORMAT_3 "--%s"
#define MATCH_EXACT_FORMAT_4 "/%s"
#define MATCH_LAZY_FORMAT_SIZE 4
#define MATCH_LAZY_FORMAT_1 "%s="
#define MATCH_LAZY_FORMAT_2 "-%s="
#define MATCH_LAZY_FORMAT_3 "--%s="
#define MATCH_LAZY_FORMAT_4 "/%s="

#define _ARG_CHECK "0"
#define _ARG_TEST "1"

/**
 * lw6sys_arg_match
 *
 * @keyword: the option to match, without the prefix "-" or "--"
 * @argv_string: the argv value, for instance argv[1]
 *
 * This is an utility function which allow the program to handle
 * options in a uniform manner. Key comparison is insensitive, that is,
 * --option and --OPTION are equivalent. Besides, -option and --OPTION
 * are equivalent too. Liquid War 6 documentation mentions options in
 * lowercase with a double dash (--option) by default, but it's a fact,
 * the program supports variants. This is just for convenience, the
 * philosophy behind this behavior is "be as permissive as possible
 * when interpreting input, and as strict as possible when generating
 * output". In fact, it's even said that Liquid War 6 will accept
 * the argument without any prefix dash as being valid... This is
 * to say running "liquidwar6 --option" is the same as running
 * "liquidwar6 option". But, this is a secret 8-)
 *
 * Return value: non zero if it matches, 0 if it doesn't.
 */
int
lw6sys_arg_match (char *keyword, char *argv_string)
{
  int ret = 0;
  char *buf = NULL;
  int i;
  char *match_exact_formats[MATCH_EXACT_FORMAT_SIZE] =
    { MATCH_EXACT_FORMAT_1, MATCH_EXACT_FORMAT_2, MATCH_EXACT_FORMAT_3,
    MATCH_EXACT_FORMAT_4
  };
  char *match_lazy_formats[MATCH_LAZY_FORMAT_SIZE] =
    { MATCH_LAZY_FORMAT_1, MATCH_LAZY_FORMAT_2, MATCH_LAZY_FORMAT_3,
    MATCH_LAZY_FORMAT_4
  };

  if (strlen (keyword) > 0)
    {
      for (i = 0; i < MATCH_EXACT_FORMAT_SIZE && !ret; ++i)
	{
	  buf = lw6sys_new_sprintf (match_exact_formats[i], keyword);
	  if (buf)
	    {
	      if (strcasecmp (buf, argv_string) == 0)
		{
		  ret = 1;
		}
	      LW6SYS_FREE (buf);
	    }
	}

      for (i = 0; i < MATCH_LAZY_FORMAT_SIZE && !ret; ++i)
	{
	  buf = lw6sys_new_sprintf (match_lazy_formats[i], keyword);
	  if (buf)
	    {
	      if (strncasecmp (buf, argv_string, strlen (buf)) == 0)
		{
		  ret = 1;
		}
	      LW6SYS_FREE (buf);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("trying to match empty command-line argument"));
    }

  return ret;
}

/**
 * lw6sys_arg_exists
 *
 * @argc: the number of arguments, as passed to @main
 * @argv: an array of arguments, as passed to @main
 * @keyword: the keyword to match
 *
 * Parses all command-line arguments, searching for one
 * precise "--key[=...]" entry.
 *
 * Return value: 1 if key is present, 0 if not.
 */
int
lw6sys_arg_exists (int argc, char *argv[], char *keyword)
{
  int i;
  int ret = 0;

  for (i = 1; i < argc; ++i)
    {
      if (lw6sys_arg_match (keyword, argv[i]))
	{
	  ret = 1;
	}
    }

  return ret;
}

/**
 * lw6sys_arg_get_value
 *
 * @argc: the number of arguments, as passed to @main
 * @argv: an array of arguments, as passed to @main
 * @keyword: the keyword to match
 *
 * Parses all command-line arguments, searching for one
 * precise "--key=value" pair, and returns the value.
 *
 * Return value: a pointer to the value. May be NULL. Must be freed. 
 */
char *
lw6sys_arg_get_value (int argc, char *argv[], char *keyword)
{
  int i;
  char *equal = NULL;
  char *ret = NULL;

  for (i = 1; i < argc && !ret; ++i)
    {
      if (!ret && lw6sys_arg_match (keyword, argv[i]))
	{
	  equal = strchr (argv[i], '=');
	  if (equal)
	    {
	      ret = lw6sys_str_copy (equal + 1);
	    }
	  else
	    {
	      ret = lw6sys_str_copy ("");
	    }
	}
    }

  return ret;
}

/**
 * lw6sys_arg_get_value_with_env
 *
 * @argc: the number of arguments, as passed to @main
 * @argv: an array of arguments, as passed to @main
 * @keyword: the keyword to match
 *
 * Parses all command-line arguments, searching for one
 * precise "--key=value" pair, and returns the value.
 * If a corresponding environment variable is available, but
 * no command-line parameter was passed, the environment variable
 * is intepreted. Such environment variables are uppercased,
 * prefixed by "LW6_" and "_" replaces "-". The
 * environment variable will be overriden if the command-line
 * parameter is present.
 *
 * Return value: a pointer to the value. May be NULL. Must be freed. 
 */
char *
lw6sys_arg_get_value_with_env (int argc, char *argv[], char *keyword)
{
  char *ret = NULL;

  ret = lw6sys_arg_get_value (argc, argv, keyword);

  if (ret == NULL)
    {
      ret = lw6sys_getenv_prefixed (keyword);
    }

  return ret;
}

/**
 * lw6sys_arg_test_mode
 *
 * @argc: argc as passed to main
 * @argv: argv as passed to main
 *
 * Chooses between the two test modes "check" or "test". Check (value 0)
 * is a lighter test which should never fail even if some special
 * hardware or environment is missing. Test (value 1) is a more complete
 * test which does things which *can* require some special conditions.
 * Function will log and be verbose is syntax is not correct.
 *
 * Return value: 1 if complete test must be run, 0 is only check
 */
int
lw6sys_arg_test_mode (int argc, char *argv[])
{
  int ret = 0;
  int syntax_ok = 0;

  if (argc == 1)
    {
      syntax_ok = 1;
    }
  if (argc >= 2)
    {
      if (!strcmp (argv[1], _ARG_CHECK))
	{
	  syntax_ok = 1;
	}
      else if (!strcmp (argv[1], _ARG_TEST))
	{
	  ret = 1;
	  syntax_ok = 1;
	}
    }
  if (!syntax_ok)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("wrong syntax (should be \"<program> {0|1}\"), test suite will be run anyways"));
    }

  return ret;
}
