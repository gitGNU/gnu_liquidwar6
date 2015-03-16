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

#include "cfg.h"
#include "cfg-internal.h"


static void
_load_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _lw6cfg_context_t *cfg_context;

  cfg_context = (_lw6cfg_context_t *) callback_data;

  if (lw6cfg_must_be_saved (sys_context, key))
    {
      if (lw6sys_spinlock_lock (sys_context, cfg_context->spinlock))
	{
	  lw6sys_hash_set (sys_context, cfg_context->options, key, lw6sys_str_copy (sys_context, value));
	  lw6sys_spinlock_unlock (sys_context, cfg_context->spinlock);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("option \"%s\" not loaded"), key);
    }
}

int
_lw6cfg_load (lw6sys_context_t * sys_context, _lw6cfg_context_t * cfg_context, const char *filename)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading config from \"%s\""), filename);

  if (lw6sys_file_exists (sys_context, filename))
    {
      ret = lw6cfg_read_key_value_xml_file (sys_context, filename, _load_callback, (void *) cfg_context);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("config file \"%s\" doesn't exist, using defaults"), filename);
    }

  _lw6cfg_merge_env (sys_context, cfg_context);
  _lw6cfg_parse_command_line (sys_context, cfg_context);

  return ret;
}

/**
 * lw6cfg_load
 *
 * @sys_context: global system context
 * @cfg_context: a context returned by @lw6cfg_init
 * @filename: a file path, absolute or relative
 *
 * Loads the given config file, and stores its values into the current
 * context. Parameters which are both in the config file and given
 * as command line parameters, will be taken from the command-line.
 *
 * Return value: 1 if successfull, 0 if error.
 */
int
lw6cfg_load (lw6sys_context_t * sys_context, void *cfg_context, const char *filename)
{
  return _lw6cfg_load (sys_context, (_lw6cfg_context_t *) cfg_context, filename);
}
