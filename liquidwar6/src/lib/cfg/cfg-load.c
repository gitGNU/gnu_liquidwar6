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

#include "cfg.h"
#include "cfg-internal.h"


static void
load_callback (void *callback_data, const char *element, const char *key,
	       const char *value)
{
  _lw6cfg_context_t *cfg_context;

  cfg_context = (_lw6cfg_context_t *) callback_data;

  if (lw6cfg_must_be_saved (key))
    {
      lw6sys_hash_set (cfg_context->options, key, lw6sys_str_copy (value));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("option \"%s\" not loaded"), key);
    }
}

int
_lw6cfg_load (_lw6cfg_context_t * cfg_context, const char *filename)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("loading config from \"%s\""), filename);

  if (lw6sys_file_exists (filename))
    {
      ret =
	lw6cfg_read_key_value_xml_file (filename, load_callback,
					(void *) cfg_context);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_INFO,
		  _x_ ("config file \"%s\" doesn't exist, using defaults"),
		  filename);
    }

  _lw6cfg_merge_env (cfg_context);
  _lw6cfg_parse_command_line (cfg_context);

  return ret;
}

/**
 * lw6cfg_load
 *
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
lw6cfg_load (void *cfg_context, const char *filename)
{
  return _lw6cfg_load ((_lw6cfg_context_t *) cfg_context, filename);
}
