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

#include "scm.h"

/**
 * lw6scm_c_define_gsubr
 *
 * @name: name of the function when called from guile
 * @req: required parameters
 * @opt: optional parameters
 * @rst: ? should RTFM to find that out
 * @fcn: the function itself (pointer on the C executable code)
 *
 * Wrapper on @scm_c_define_gsubr, one of the value of this function is that
 * it does check wether it's documented before registering it. So if you
 * try to register something not documented, it will fire a warning, which
 * is a very nice code-quality tool.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6scm_c_define_gsubr (char *name, int req, int opt, int rst,
		       lw6scm_func_t fcn)
{
  int ret = 0;

  if (lw6hlp_is_documented (name))
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("registering Guile function \"%s\""),
		  name);
      scm_c_define_gsubr ((const char *) name, req, opt, rst, fcn);
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("Guile function \"%s\" not documented, won't register it"),
		  name);
    }

  return ret;
}

/**
 * lw6scm_c_primitive_load
 *
 * @filename: file to execute
 *
 * Loads and executes a script. Will add a log message while doing it.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6scm_c_primitive_load (char *filename)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("loading \"%s\""), filename);

  if (lw6sys_file_exists (filename))
    {
      scm_c_primitive_load ((const char *) filename);
      // todo, check that it does really work
      ret = 1;
    }
  else
    {
      /*
       * Well, actually, in the calling code, the
       * check for file existence is also done
       * so this is just "in case".
       */
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to read script file \"%s\""), filename);
    }

  return ret;
}
