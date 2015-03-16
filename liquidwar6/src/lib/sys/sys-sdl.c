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
 * lw6sys_sdl_register:
 *
 * @sys_context: global system context
 *
 * Function used to avoid initializing SDL several times in a program.
 * AFAIK Allegro has a @was_init function, but SDL doesn't. With this
 * function - which every LW6 sub-module should use - one can know
 * globally, for the whole program, wether SDL has been initialized
 * or not. Note that this function uses the global system context,
 * and can therefore be buggy when used in multithreaded / reentrant
 * mode. So in some cases, that is, with two different contexts, SDL
 * could be called twice. This is a limitation of current SDL implementations,
 * should it have a per-thread / per-handler context, the problem would
 * be solved. Fundamentally, the idea is that SDL does have a global static
 * state, you've been warned.
 *
 * @Return value: 1 if SDL has never been initialized, 0 if it has
 *   already been set up.
 */
int
lw6sys_sdl_register (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  int ret = (global->sdl_registered_use <= 0);

  global->sdl_registered_use++;

  return ret;
}

/**
 * lw6sys_sdl_unregister:
 *
 * @sys_context: global system context
 *
 * Call this whenever you are done with SDL and exit it, so that
 * the @lw6sys_sdl_register function works correctly.
 *
 * Return value: 1 if SDL needs to be unregistered, that is, if
 *   it has already been initialized, else 0.
 */
int
lw6sys_sdl_unregister (lw6sys_context_t * sys_context)
{
  int ret = 0;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (global->sdl_registered_use > 0)
    {
      global->sdl_registered_use--;
    }

  ret = (global->sdl_registered_use <= 0);

  return ret;
}
