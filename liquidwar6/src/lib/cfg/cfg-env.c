/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

static int
must_be_stored (char *key)
{
  int ret = 1;

  /*
   * We exclude some keys, for which it doesn't make sense
   * to be stored:
   * - reset, because it's useless and brings confusion to store it
   * - map-path, because values in it should be added, not replaced
   */
  if ((!strcmp (key, LW6DEF_RESET)) || (!strcmp (key, LW6DEF_MAP_PATH)))
    {
      ret = 0;
    }

  return ret;
}

static void
merge_func (void *func_data, void *data)
{
  _lw6cfg_context_t *cfg_context = (_lw6cfg_context_t *) func_data;
  char *keyword = (char *) data;
  char *value = NULL;
  char *value_converted = NULL;
  char *env = NULL;

  if (must_be_stored (keyword) && strlen (keyword) > 0
      && lw6sys_env_exists (keyword))
    {
      value = lw6sys_getenv (keyword);
      if (value)
	{
	  value_converted = lw6cfg_format_guess_type (keyword, value);
	  if (value_converted)
	    {
	      env = lw6sys_keyword_as_env (keyword);
	      if (env)
		{
		  lw6sys_log (LW6SYS_LOG_INFO,
			      _
			      ("key \"%s\" set to \"%s\" by environment variable \"%s\""),
			      keyword, value_converted, env);
		  LW6SYS_FREE (env);
		}
	      lw6sys_hash_set (cfg_context->options, keyword,
			       value_converted);
	      // no need to free value_converted now
	    }
	  LW6SYS_FREE (value);
	}
    }
}

int
_lw6cfg_merge_env (_lw6cfg_context_t * cfg_context)
{
  int ret = 0;
  lw6sys_list_t *keywords;

  lw6sys_log (LW6SYS_LOG_INFO, _("merging environment variables"));
  keywords = lw6hlp_list ();
  if (keywords)
    {
      lw6sys_list_map (keywords, &merge_func, (void *) cfg_context);
      lw6sys_list_free (keywords);
    }

  return ret;
}

/**
 * lw6cfg_merge_env
 *
 * @cfg_context: a context returned by @lw6cfg_init
 *
 * Overwrites any existing vale in the config with environment
 * variables prefixed by LW6_.
 *
 * Return value: 1 if successfull, 0 if error.
 */
int
lw6cfg_merge_env (void *cfg_context)
{
  return _lw6cfg_merge_env ((_lw6cfg_context_t *) cfg_context);
}
