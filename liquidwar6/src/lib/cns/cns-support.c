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

#include "cns.h"

#define _ENV_TERM_KEY "TERM"
#define _ENV_TERM_ALLOWED_XTERM "xterm"
#define _ENV_TERM_ALLOWED_LINUX "linux"

/**
 * lw6cns_console_support
 *
 * @sys_context: global system context
 *
 * Tells wether console is supported.
 *
 * Return value: 1 if console can be enabled, 0 if not
 */
int
lw6cns_console_support (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifdef LW6_CONSOLE
  ret = 1;
#endif

  return ret;
}

/**
 * lw6cns_term_support
 *
 * @sys_context: global system context
 *
 * Tells wether program is likely to have proper term (xterm, Linux console) support.
 *
 * Return value: 1 if has validated TERM support, 0 if not
 */
int
lw6cns_term_support (lw6sys_context_t * sys_context)
{
  int ret = 0;
  char *env_term = NULL;

  /*
   * We're sort of paranoid, if TERM is not a well-known proven-to-work
   * entry, then simply disable console support.
   */
  env_term = lw6sys_getenv (sys_context, _ENV_TERM_KEY);
  if (env_term)
    {
      if (lw6sys_str_is_same (sys_context, env_term, _ENV_TERM_ALLOWED_XTERM) || lw6sys_str_is_same (sys_context, env_term, _ENV_TERM_ALLOWED_LINUX))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("term support should be fine, env variable \"%s\" is \"%s\""), _ENV_TERM_KEY, env_term);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_
		      ("term support probably broken, env variable \"%s\" is \"%s\" which is neither \"%s\" or \"%s\""),
		      _ENV_TERM_KEY, env_term, _ENV_TERM_ALLOWED_XTERM, _ENV_TERM_ALLOWED_LINUX);
	}

      LW6SYS_FREE (sys_context, env_term);
      env_term = NULL;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("term support probably broken, env variable \"%s\" is not set"), _ENV_TERM_KEY);
    }

  return ret;
}
