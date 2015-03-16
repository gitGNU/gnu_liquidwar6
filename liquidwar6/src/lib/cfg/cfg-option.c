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

int
_lw6cfg_option_exists (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key)
{
  int ret = 0;

  if (lw6sys_spinlock_lock (sys_context, _cfg_context->spinlock))
    {
      ret = lw6sys_hash_has_key (sys_context, _cfg_context->options, key);
      lw6sys_spinlock_unlock (sys_context, _cfg_context->spinlock);
    }

  return ret;
}

/**
 * lw6cfg_option_exists
 *
 * @sys_context: global system context
 * @cfg_context: context to query
 * @key: key to search
 *
 * Returns wether a key exists within context or not.
 *
 * Return value: 1 if exists, 0 if not
 */
int
lw6cfg_option_exists (lw6sys_context_t * sys_context, void *cfg_context, const char *key)
{
  return _lw6cfg_option_exists (sys_context, (_lw6cfg_context_t *) cfg_context, key);
}

char *
_lw6cfg_get_option (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key)
{
  char *ret = "";

  if (lw6sys_spinlock_lock (sys_context, _cfg_context->spinlock))
    {
      ret = (char *) lw6sys_hash_get (sys_context, _cfg_context->options, key);
      if (ret)
	{
	  ret = lw6sys_str_copy (sys_context, ret);
	}
      lw6sys_spinlock_unlock (sys_context, _cfg_context->spinlock);
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("option \"%s\" is not set"), key);
    }

  return ret;
}

/**
 * lw6cfg_get_option
 *
 * @sys_context: global system context
 * @cfg_context: context to query
 * @key: key to search
 *
 * Returns the current value for a given query, the returned value is always
 * a string, typically the string one would pass on the command line or
 * set in a config file
 *
 * Return value: pointer to string, must be freed.
 */
char *
lw6cfg_get_option (lw6sys_context_t * sys_context, void *cfg_context, const char *key)
{
  return _lw6cfg_get_option (sys_context, (_lw6cfg_context_t *) cfg_context, key);
}

void
_lw6cfg_set_option (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key, const char *value)
{
  char *value_converted = NULL;

  value_converted = lw6cfg_format_guess_type (sys_context, key, value);
  if (value_converted)
    {
      if (lw6sys_spinlock_lock (sys_context, _cfg_context->spinlock))
	{
	  lw6sys_hash_set (sys_context, _cfg_context->options, key, (void *) value_converted);
	  lw6sys_spinlock_unlock (sys_context, _cfg_context->spinlock);
	}
    }
}

/**
 * lw6cfg_set_option
 *
 * @sys_context: global system context
 * @cfg_context: context to modify
 * @key: key to search and change
 * @value: new value
 *
 * Sets a given key to the specified value, this is a string only
 * function, pass the value you would pass on the command line or
 * set in a config file.
 *
 * Return value: none
 */
void
lw6cfg_set_option (lw6sys_context_t * sys_context, void *cfg_context, const char *key, const char *value)
{
  _lw6cfg_set_option (sys_context, (_lw6cfg_context_t *) cfg_context, key, value);
}

int
_lw6cfg_get_option_int (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key)
{
  int ret = 0;
  char *str = _lw6cfg_get_option (sys_context, _cfg_context, key);

  if (str)
    {
      ret = lw6sys_atoi (sys_context, str);
      LW6SYS_FREE (sys_context, str);
    }

  return ret;
}

/**
 * lw6cfg_get_option_int
 *
 * @context: context to query
 * @key: key to search
 *
 * Returns an option as an integer. Note that this function does
 * not know wether the parameter should really be an integer or not,
 * so you can call it even on non-integer values, but of course
 * results will have no real meaning.
 *
 * Return value: option value converted to int
 */
int
lw6cfg_get_option_int (lw6sys_context_t * sys_context, void *cfg_context, const char *key)
{
  return _lw6cfg_get_option_int (sys_context, (_lw6cfg_context_t *) cfg_context, key);
}

void
_lw6cfg_set_option_int (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key, int value)
{
  char *str;

  str = lw6sys_itoa (sys_context, value);
  if (str)
    {
      _lw6cfg_set_option (sys_context, _cfg_context, key, str);
      LW6SYS_FREE (sys_context, str);
    }
}

/**
 * lw6cfg_set_option_int
 *
 * @sys_context: global system context
 * @cfg_context: context to modify
 * @key: key to search and change
 * @value: new value
 *
 * Set a config option to an integer value. Note that this function
 * does not know wether the parameter should really be an integer or not,
 * so you can call it even on non-integer values, at your own risk.
 *
 * Return value: none.
 */
void
lw6cfg_set_option_int (lw6sys_context_t * sys_context, void *cfg_context, const char *key, int value)
{
  _lw6cfg_set_option_int (sys_context, (_lw6cfg_context_t *) cfg_context, key, value);
}

int
_lw6cfg_get_option_bool (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key)
{
  int ret = 0;
  char *str = _lw6cfg_get_option (sys_context, _cfg_context, key);

  if (str)
    {
      ret = lw6sys_atoi (sys_context, str) ? 1 : 0;
      LW6SYS_FREE (sys_context, str);
    }

  return ret;
}

/**
 * lw6cfg_get_option_bool
 *
 * @sys_context: global system context
 * @cfg_context: context to query
 * @key: key to search
 *
 * Returns an option as a boolean. Note that this function does
 * not know wether the parameter should really be a boolean or not,
 * so you can call it even on non-boolean values, but of course
 * results will have no real meaning.
 *
 * Return value: option value converted to boolean
 */
int
lw6cfg_get_option_bool (lw6sys_context_t * sys_context, void *cfg_context, const char *key)
{
  return _lw6cfg_get_option_bool (sys_context, (_lw6cfg_context_t *) cfg_context, key);
}

void
_lw6cfg_set_option_bool (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key, int value)
{
  char *str;

  str = lw6sys_itoa (sys_context, value ? 1 : 0);
  if (str)
    {
      _lw6cfg_set_option (sys_context, _cfg_context, key, str);
      LW6SYS_FREE (sys_context, str);
    }
}

/**
 * lw6cfg_set_option_bool
 *
 * @sys_context: global system context
 * @cfg_context: context to modify
 * @key: key to search and change
 * @value: new value
 *
 * Set a config option to a boolean value. Note that this function
 * does not know wether the parameter should really be a boolean or not,
 * so you can call it even on non-boolean values, at your own risk.
 *
 * Return value: none.
 */
void
lw6cfg_set_option_bool (lw6sys_context_t * sys_context, void *cfg_context, const char *key, int value)
{
  _lw6cfg_set_option_bool (sys_context, (_lw6cfg_context_t *) cfg_context, key, value);
}
