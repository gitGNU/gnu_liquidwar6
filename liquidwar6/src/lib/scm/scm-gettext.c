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

#include "scm.h"

/**
 * lw6scm_gettext
 *
 * @sys_context: global system context
 * @string: SCM object to convert
 *
 * For a GNU gettext-like behavior of scheme code,
 * exported with a name such as _ then calling function
 * _ from Guile will just do the same as _ in C.
 *
 * Return value: SCM value, the translated string
 */
SCM
lw6scm_gettext (lw6sys_context_t * sys_context, SCM string)
{
  char *c_string;
  SCM ret = SCM_BOOL_F;

  SCM_ASSERT (scm_is_string (string), string, SCM_ARG1, __FUNCTION__);

  c_string = lw6scm_utils_to_0str (sys_context, string);
  if (c_string)
    {
      ret = scm_from_locale_string (gettext (c_string));
      LW6SYS_FREE (sys_context, c_string);
    }

  return ret;
}
