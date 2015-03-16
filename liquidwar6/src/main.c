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

#include "lib/liquidwar6.h"

#ifdef LW6_MAC_OS_X
#include <SDL/SDL.h>
#endif // LW6_MAX_OS_X

/*
 * On Mac OS X, SDL/SDL.h will redefine main, override it, whatever,
 * anyway it needs it to be "int argc, char **argv" so *without* the
 * const attribute on argv. On the other hand, using const is nice
 * on other platforms since, in reality, it *is* const. So the
 * workarround is to have the #define below.
 */
#ifdef LW6_MAC_OS_X
int
main (int argc, char *argv[])
#else // LW6_MAX_OS_X
int
main (int argc, const char *argv[])
#endif				// LW6_MAC_OS_X
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_MAIN_BEGIN (sys_context);
  LW6HLP_MAIN_BEGIN (sys_context);

  ret = lw6_main (sys_context, argc, (const char **) argv);

  LW6HLP_MAIN_END (sys_context);
  LW6SYS_MAIN_END (sys_context);

  return (ret ? LW6SYS_EXIT_OK : LW6SYS_EXIT_ERROR);
}

/*
 * Dummy includes because libtool does not seem to rebuild the
 * main binary when necessary.
 */
#include "lib/sys/sys-build-stamp.h"
#include "lib/sys/sys-build-md5sum.h"
