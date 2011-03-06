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

#include "cfg.h"
#include "cfg-internal.h"

int
_lw6cfg_option_exists (_lw6cfg_context_t * context, char *key)
{
  int ret = 0;

  ret = lw6sys_hash_has_key (context->options, key);

  return ret;
}

int
lw6cfg_option_exists (void *context, char *key)
{
  return _lw6cfg_option_exists ((_lw6cfg_context_t *) context, key);
}

char *
_lw6cfg_get_option (_lw6cfg_context_t * context, char *key)
{
  char *ret = "";

  ret = (char *) lw6sys_hash_get (context->options, key);

  if (!ret)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("option \"%s\" is not set"), key);
    }

  return ret;
}

char *
lw6cfg_get_option (void *context, char *key)
{
  return _lw6cfg_get_option ((_lw6cfg_context_t *) context, key);
}

void
_lw6cfg_set_option (_lw6cfg_context_t * context, char *key, char *value)
{
  char *value_converted = NULL;

  value_converted = lw6cfg_format_guess_type (key, value);
  if (value_converted)
    {
      lw6sys_hash_set (context->options, key, (void *) value_converted);
    }
}

void
lw6cfg_set_option (void *context, char *key, char *value)
{
  _lw6cfg_set_option ((_lw6cfg_context_t *) context, key, value);
}

int
_lw6cfg_get_option_int (_lw6cfg_context_t * context, char *key)
{
  int ret = 0;
  char *str = _lw6cfg_get_option (context, key);

  if (str)
    {
      ret = lw6sys_atoi (str);
    }

  return ret;
}

int
lw6cfg_get_option_int (void *context, char *key)
{
  return _lw6cfg_get_option_int ((_lw6cfg_context_t *) context, key);
}

void
_lw6cfg_set_option_int (_lw6cfg_context_t * context, char *key, int value)
{
  char *str;

  str = lw6sys_itoa (value);
  if (str)
    {
      _lw6cfg_set_option (context, key, str);
      LW6SYS_FREE (str);
    }
}

void
lw6cfg_set_option_int (void *context, char *key, int value)
{
  _lw6cfg_set_option_int ((_lw6cfg_context_t *) context, key, value);
}

int
_lw6cfg_get_option_bool (_lw6cfg_context_t * context, char *key)
{
  int ret = 0;
  char *str = _lw6cfg_get_option (context, key);

  if (str)
    {
      ret = lw6sys_atoi (str) ? 1 : 0;
    }

  return ret;
}

int
lw6cfg_get_option_bool (void *context, char *key)
{
  return _lw6cfg_get_option_bool ((_lw6cfg_context_t *) context, key);
}

void
_lw6cfg_set_option_bool (_lw6cfg_context_t * context, char *key, int value)
{
  char *str;

  str = lw6sys_itoa (value ? 1 : 0);
  if (str)
    {
      _lw6cfg_set_option (context, key, str);
      LW6SYS_FREE (str);
    }
}

void
lw6cfg_set_option_bool (void *context, char *key, int value)
{
  _lw6cfg_set_option_bool ((_lw6cfg_context_t *) context, key, value);
}
