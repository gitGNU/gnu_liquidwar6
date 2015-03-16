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
#include <unistd.h>

#include "sys.h"

#ifdef LW6_MS_WINDOWS
#include <winbase.h>
#include <lmcons.h>
#else
#include <sys/utsname.h>
#endif

#define DEFAULT_USERNAME "player"
#define DEFAULT_HOSTNAME "computer"

#ifdef LW6_MS_WINDOWS
#define ENV_SEPARATOR_CHAR ';'
#define ENV_SEPARATOR_STR ";"
#else
#define ENV_SEPARATOR_CHAR ':'
#define ENV_SEPARATOR_STR ":"
#endif

#ifdef LW6_MS_WINDOWS
#define GETENV_SIZE 2048
#endif

/**
 * lw6sys_env_separator_char
 *
 * Gets the ENV separator, that is, for instance, the character
 * used to separate paths in environment variables. Typically,
 * this would be ":" on GNU and ";" on Microsft platforms.
 *
 * Return value: the ascii character code.
 */
char
lw6sys_env_separator_char ()
{
  return ENV_SEPARATOR_CHAR;
}

/**
 * lw6sys_env_separator_str
 *
 * Gets the ENV separator, that is, for instance, the character
 * used to separate paths in environment variables. Typically,
 * this would be ":" on GNU and ";" on Microsft platforms.
 *
 * Return value: a pointer to a single 0-terminated character string
 *   which contains the character. Must not be freed.
 */
char *
lw6sys_env_separator_str ()
{
  return ENV_SEPARATOR_STR;
}

/**
 * lw6sys_env_concat
 *
 * @sys_context: global system context
 * @value1: the left part to be concatenated
 * @value2: the right part to be concatenated
 *
 * Concatenates two values and puts the ENV separator, as returned
 * by @lw6sys_env_separator_char between them.
 *
 * Return value: the concatenated string, must be freed.
 */
char *
lw6sys_env_concat (lw6sys_context_t * sys_context, const char *value1, const char *value2)
{
  char *ret = NULL;

  if (!strlen (value1))
    {
      ret = lw6sys_str_copy (sys_context, value2);
    }
  else if (!strlen (value2))
    {
      ret = lw6sys_str_copy (sys_context, value1);
    }
  else
    {
      ret = lw6sys_new_sprintf (sys_context, "%s%s%s", value1, lw6sys_env_separator_str (), value2);
    }

  return ret;
}

/**
 * lw6sys_env_exists_prefixed
 *
 * @sys_context: global system context
 * @keyword: the keyword to be searched in the environment variables.
 *
 * Searches environment variables for the given keyword. The keyword
 * will be fixed so that all dashes "-" characters are replaced
 * by underscores "_" characters. Characters will be changed to
 * uppercase. Any non alphanumeric character will be replaced
 * by "_". Finally, an "LW6_" prefix will be added. That is to say,
 * calling this function with "my-param" will search for
 * the "LW6_MY_PARAM" environment variable.
 *
 * Return value: 1 if the environment variable exists, 0 if not.
 */
int
lw6sys_env_exists_prefixed (lw6sys_context_t * sys_context, const char *keyword)
{
  int ret = 0;
  char *value = NULL;

  value = lw6sys_getenv_prefixed (sys_context, keyword);
  if (value)
    {
      ret = 1;
      LW6SYS_FREE (sys_context, value);
    }

  return ret;
}

/**
 * lw6sys_getenv
 *
 * @sys_context: global system context
 * @key: the environment variable to get.
 *
 * Searches environment variables for the given value.
 * This is a wrapper over the standard C getenv, the
 * difference is it will return a dynamically allocated
 * pointer, and on some platforms will query specific
 * OS functions.
 *
 * Return value: the value for the given keyword. May be NULL. Must be freed.
 */
char *
lw6sys_getenv (lw6sys_context_t * sys_context, const char *key)
{
#ifdef LW6_MS_WINDOWS
  char value[GETENV_SIZE + 1];
#else
  char *value = NULL;
#endif
  char *ret = NULL;

#ifdef LW6_MS_WINDOWS
  memset (value, 0, GETENV_SIZE + 1);
  if (!GetEnvironmentVariable (key, value, GETENV_SIZE))
    {
      if (GetLastError () == ERROR_ENVVAR_NOT_FOUND)
	{
	  // OK, not found, ret stays NULL
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("GetEnvironmentVariable failed"));
	}
    }
  else
    {
      ret = lw6sys_str_copy (sys_context, value);
    }
#else
  value = getenv (key);
  if (value)
    {
      ret = lw6sys_str_copy (sys_context, value);
    }
#endif

  return ret;
}

/**
 * lw6sys_getenv_prefixed
 *
 * @sys_context: global system context
 * @keyword: the keyword to be searched in the environment variables.
 *
 * Searches environment variables for the given value. The keyword
 * will be fixed so that all dashes "-" characters are replaced
 * by underscores "_" characters. Characters will be changed to
 * uppercase. Any non alphanumeric character will be replaced
 * by "_". Finally, an "LW6_" prefix will be added. That is to say,
 * calling this function with "my-param" will search for
 * the "LW6_MY_PARAM" environment variable.
 *
 * Return value: the value for the given keyword. May be NULL. Must be freed.
 */
char *
lw6sys_getenv_prefixed (lw6sys_context_t * sys_context, const char *keyword)
{
  char *keyword_prefixed = NULL;
  char *ret = NULL;

  keyword_prefixed = lw6sys_keyword_as_env (sys_context, keyword);
  if (keyword_prefixed)
    {
      ret = lw6sys_getenv (sys_context, keyword_prefixed);
      LW6SYS_FREE (sys_context, keyword_prefixed);
    }

  return ret;
}

/**
 * lw6sys_setenv
 *
 * @keyword: the environment variable to set
 * @value: the value of the environment variable to set
 *
 * Sets the environment variable to a given value. If value
 * is NULL, variable is unset. Note that unlike lw6sys_getenv_prefixed,
 * this function does not transform the keyword into "LW6_..."
 * before setting the value, so it's your responsability to
 * call "lw6sys_keyword_as_env" if needed.
 *
 * Return value: 1 if success, 0 if failed
 */
int
lw6sys_setenv (lw6sys_context_t * sys_context, const char *keyword, const char *value)
{
  int ret = 0;

#ifdef LW6_MS_WINDOWS
  {
    /*
     * For some reason (tired of getting into MS specifics...) it's
     * required to use putenv *and* SetEnvironmentVariable. Putenv
     * is required for instance when setting GUILE_LOAD_PATH. In doubt,
     * we do both, the last one being the MS dedicated function,
     * which should overwrite the previous in case of conflict and/or
     * handle special characters such as '=' in a more consistent way.
     */
    char *putenv_str = NULL;

    putenv_str = lw6sys_new_sprintf ("%s=%s", keyword, value);
    if (putenv_str)
      {
	putenv (putenv_str);
	LW6SYS_FREE (sys_context, putenv_str);
      }
  }
  ret = SetEnvironmentVariable (keyword, value) ? 1 : 0;
  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("SetEnvironmentVariable failed"));
    }
#else
  if (value)
    {
      ret = setenv (keyword, value, 1) ? 0 : 1;
    }
  else
    {
      ret = unsetenv (keyword);
    }
#endif

  return ret;
}

/**
 * lw6sys_setenv_prefixed
 *
 * @sys_context: global system context
 * @keyword: the keyword to be searched in the environment variables.
 * @value: the value of the environment variable to set
 *
 * Sets the environment variable to the given value. The keyword
 * will be fixed so that all dashes "-" characters are replaced
 * by underscores "_" characters. Characters will be changed to
 * uppercase. Any non alphanumeric character will be replaced
 * by "_". Finally, an "LW6_" prefix will be added. That is to say,
 * calling this function with "my-param" will set
 * the "LW6_MY_PARAM" environment variable.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_setenv_prefixed (lw6sys_context_t * sys_context, const char *keyword, const char *value)
{
  char *keyword_prefixed = NULL;
  int ret = 0;

  keyword_prefixed = lw6sys_keyword_as_env (sys_context, keyword);
  if (keyword_prefixed)
    {
      ret = lw6sys_setenv (sys_context, keyword_prefixed, value);
      LW6SYS_FREE (sys_context, keyword_prefixed);
    }

  return ret;
}

/**
 * lw6sys_env_split
 *
 * @sys_context: global system context
 * @value: the value, a list of item separated by... the separator
 *
 * Splits the environment value into a list of strings containing
 * each element. All strings are dynamically allocated, but they
 * will be freed automatically when the list is freed.
 *
 * Return value: a list of strings.
 */
lw6sys_list_t *
lw6sys_env_split (lw6sys_context_t * sys_context, const char *value)
{
  lw6sys_list_t *ret = NULL;
  char *copy = NULL;
  char *sep = NULL;
  char *pos = NULL;
  char *item = NULL;

  ret = lw6sys_list_new (sys_context, lw6sys_free_callback);
  if (ret)
    {
      /*
       * We make a copy for we're going to modify it
       */
      copy = lw6sys_str_copy (sys_context, value);
      if (copy)
	{
	  pos = copy;
	  while ((sep = strchr (pos, lw6sys_env_separator_char ())) != NULL)
	    {
	      sep[0] = '\0';
	      if (ret)
		{
		  item = lw6sys_str_copy (sys_context, pos);
		  if (item)
		    {
		      lw6sys_lifo_push (sys_context, &ret, item);
		      // do not free, will be done when list is destroyed
		    }
		}
	      pos = sep + 1;
	    }
	  if (ret)
	    {
	      item = lw6sys_str_copy (sys_context, pos);
	      if (item)
		{
		  lw6sys_lifo_push (sys_context, &ret, item);
		}
	    }
	  LW6SYS_FREE (sys_context, copy);
	}
    }

  return ret;
}

/**
 * lw6sys_get_home
 *
 * @sys_context: global system context
 *
 * Gets the home directory of the user. Used internally to calculate
 * the @user-dir value. Note that Liquid War 6, by default, never
 * stores files under '$HOME', instead it put things in '$HOME/.liquidwar6',
 * that is 'user-dir'. If the environment variable 'HOME' is not set,
 * will return '.'.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_get_home (lw6sys_context_t * sys_context)
{
  char *ret = NULL;
  char *home = NULL;

#ifdef LW6_MS_WINDOWS
  home = getenv ("USERPROFILE");
#else
  home = getenv ("HOME");
#endif
  if (!home)
    {
      home = ".";
    }

  ret = lw6sys_str_copy (sys_context, home);

  return ret;
}

/**
 * lw6sys_get_username
 *
 * @sys_context: global system context
 *
 * Gets the name of the current user. Difference with the standard
 * function @getlogin is that this function will returned a dynamically
 * allocated pointer, and provide a default value if it's undefined.
 * Also, if will look at the content of the 'LOGNAME' environment
 * variable if needed, and will even provide a default value.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_get_username (lw6sys_context_t * sys_context)
{
  char *ret = NULL;
#ifdef LW6_MS_WINDOWS
  DWORD size;
#else
  char *login = NULL;
#endif

#ifdef LW6_MS_WINDOWS
  ret = LW6SYS_CALLOC (sys_context, UNLEN + 2);
  if (ret)
    {
      size = UNLEN + 1;
      if (GetUserName (ret, &size) && size > 0)
	{
	  // OK
	}
      else
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = lw6sys_str_copy (sys_context, DEFAULT_USERNAME);
	}
    }
#else
  login = getlogin ();
  if (!login)
    {
      login = getenv ("LOGNAME");
    }
  if (!login)
    {
      login = DEFAULT_USERNAME;
    }

  ret = lw6sys_str_copy (sys_context, login);
#endif

  return ret;
}

/**
 * lw6sys_get_hostname
 *
 * @sys_context: global system context
 *
 * Gets the name of the current host. The name of the computer.
 * Might not work perfectly, this function is just used to
 * provide default values for player names and such things.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_get_hostname (lw6sys_context_t * sys_context)
{
  char *ret = NULL;
#ifdef LW6_MS_WINDOWS
  DWORD size;
#else
  struct utsname buf;
#endif

#ifdef LW6_MS_WINDOWS
  ret = LW6SYS_CALLOC (sys_context, MAX_COMPUTERNAME_LENGTH + 2);
  if (ret)
    {
      size = MAX_COMPUTERNAME_LENGTH + 1;
      if (GetComputerName (ret, &size) && size > 0)
	{
	  // OK
	}
      else
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = lw6sys_str_copy (sys_context, DEFAULT_HOSTNAME);
	}
    }
#else
  memset (&buf, 0, sizeof (buf));
  if ((!uname (&buf)) && strlen (buf.nodename) > 0)
    {
      ret = lw6sys_str_copy (sys_context, buf.nodename);
    }
  else
    {
      ret = lw6sys_str_copy (sys_context, DEFAULT_HOSTNAME);
    }
#endif

  return ret;
}
