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

/**
 * lw6sys_arg_match
 *
 * @sys_context: global system context
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
lw6sys_arg_match (lw6sys_context_t * sys_context, const char *keyword, const char *argv_string)
{
  int ret = 0;
  char *buf = NULL;
  int i;
  char *match_exact_formats[MATCH_EXACT_FORMAT_SIZE] = { MATCH_EXACT_FORMAT_1, MATCH_EXACT_FORMAT_2, MATCH_EXACT_FORMAT_3,
    MATCH_EXACT_FORMAT_4
  };
  char *match_lazy_formats[MATCH_LAZY_FORMAT_SIZE] = { MATCH_LAZY_FORMAT_1, MATCH_LAZY_FORMAT_2, MATCH_LAZY_FORMAT_3,
    MATCH_LAZY_FORMAT_4
  };

  if (strlen (keyword) > 0)
    {
      for (i = 0; i < MATCH_EXACT_FORMAT_SIZE && !ret; ++i)
	{
	  buf = lw6sys_new_sprintf (sys_context, match_exact_formats[i], keyword);
	  if (buf)
	    {
	      if (strcasecmp (buf, argv_string) == 0)
		{
		  ret = 1;
		}
	      LW6SYS_FREE (sys_context, buf);
	    }
	}

      for (i = 0; i < MATCH_LAZY_FORMAT_SIZE && !ret; ++i)
	{
	  buf = lw6sys_new_sprintf (sys_context, match_lazy_formats[i], keyword);
	  if (buf)
	    {
	      if (strncasecmp (buf, argv_string, strlen (buf)) == 0)
		{
		  ret = 1;
		}
	      LW6SYS_FREE (sys_context, buf);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to match empty command-line argument"));
    }

  return ret;
}

/**
 * lw6sys_arg_exists
 *
 * @sys_context: global system context
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
lw6sys_arg_exists (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *keyword)
{
  int i;
  int ret = 0;

  for (i = 1; i < argc; ++i)
    {
      if (lw6sys_arg_match (sys_context, keyword, argv[i]))
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
lw6sys_arg_get_value (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *keyword)
{
  int i;
  char *equal = NULL;
  char *ret = NULL;

  for (i = 1; i < argc && !ret; ++i)
    {
      if (!ret && lw6sys_arg_match (sys_context, keyword, argv[i]))
	{
	  equal = strchr (argv[i], '=');
	  if (equal)
	    {
	      ret = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, equal + 1));
	    }
	}
    }

  return ret;
}

/**
 * lw6sys_arg_get_value_with_env
 *
 * @sys_context: global system context
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
lw6sys_arg_get_value_with_env (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *keyword)
{
  char *ret = NULL;

  ret = lw6sys_arg_get_value (sys_context, argc, argv, keyword);

  if (ret == NULL)
    {
      ret = lw6sys_getenv_prefixed (sys_context, keyword);
    }

  return ret;
}

/**
 * lw6sys_arg_test_mode
 *
 * @sys_context: global system context
 * @argc: argc as passed to main
 * @argv: argv as passed to main
 *
 * Chooses between the two test modes "check" or "test" and also
 * reports wether one should run "batch" or "interactive" tests.
 * This is done by using the bit mask defined in
 * LW6SYS_TEST_MODE_FULL_TEST and LW6SYS_TEST_MODE_INTERACTIVE.
 *
 * Return value: a bit mask one can pass to test functions
 */
int
lw6sys_arg_test_mode (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  int ret = 0;
  int syntax_ok = 0;
  int mode = 0;

  if (argc == 1)
    {
      syntax_ok = 1;
    }
  if (argc >= 2)
    {
      mode = lw6sys_atoi (sys_context, argv[1]);
      ret = mode & (LW6SYS_TEST_MODE_FULL_TEST | LW6SYS_TEST_MODE_INTERACTIVE | LW6SYS_TEST_MODE_DEBUG);
      if (ret == mode)
	{
	  syntax_ok = 1;
	}
    }
  if (!syntax_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong syntax (should be \"<program> [0-%d]\"), test suite will be run anyways with mode=%d"),
		  LW6SYS_TEST_MODE_FULL_TEST | LW6SYS_TEST_MODE_INTERACTIVE | LW6SYS_TEST_MODE_DEBUG, ret);
    }

  lw6sys_debug_set (sys_context, ((ret & LW6SYS_TEST_MODE_DEBUG) != 0));

  return ret;
}
