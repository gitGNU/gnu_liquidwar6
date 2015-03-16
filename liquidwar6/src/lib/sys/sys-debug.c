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
#include "sys-internal.h"

/**
 * lw6sys_debug_get
 *
 * @sys_context: global system context
 *
 * Gets the debug mode.
 *
 * @Return value: 1 if debug mode set, 0 if not.
 */
int
lw6sys_debug_get (lw6sys_context_t * sys_context)
{
  int ret = 0;
  _lw6sys_context_t *_sys_context = (_lw6sys_context_t *) sys_context;

  ret = _sys_context->global.debug ? 1 : 0;

  return ret;
}

/**
 * lw6sys_debug_set
 *
 * @sys_context: global system context
 * @mode: the debug mode, 1 if set, 0 if not.
 *
 * Sets the debug mode.
 */
void
lw6sys_debug_set (lw6sys_context_t * sys_context, int mode)
{
  _lw6sys_context_t *_sys_context = (_lw6sys_context_t *) sys_context;

  _sys_context->global.debug = mode ? 1 : 0;
}
