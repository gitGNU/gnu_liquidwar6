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

#include "sys.h"

/**
 * lw6sys_true:
 *
 * Function which returns always true, that is, something different
 * than 0.
 *
 * @Return value: 1
 */
int
lw6sys_true ()
{
  return 1;
}

/**
 * lw6sys_false:
 *
 * Function which returns always false, that is, 0. This can seem
 * totally useless but it does have some utility. It's used for instance
 * to "fool" the compiler and force it to compile and link functions in
 * binaries, so that, afterwards, dynamically loaded .so files can find
 * in the main binary some functions which would otherwise be stripped
 * during the final link.
 *
 * @Return value: 0
 */
int
lw6sys_false ()
{
  return 0;
}
