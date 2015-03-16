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

#include "vox.h"
#include "vox-internal.h"

_lw6vox_renderer_t *
_lw6vox_renderer_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state)
{
  _lw6vox_renderer_t *ret = NULL;

  ret = (_lw6vox_renderer_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6vox_renderer_t));
  if (ret)
    {
      // ok
    }

  return ret;
}

/**
 * lw6vox_renderer_new
 *
 * @sys_context: global system context
 * @game_state: the game state to use
 *
 * Creates a voxel rendering object (todo, not implemented yet).
 *
 * Return value: renderer object
 */
lw6vox_renderer_t *
lw6vox_renderer_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state)
{
  return (lw6vox_renderer_t *) _lw6vox_renderer_new (sys_context, game_state);
}

void
_lw6vox_renderer_free (lw6sys_context_t * sys_context, _lw6vox_renderer_t * renderer)
{
  LW6SYS_FREE (sys_context, renderer);
}

/**
 * lw6vox_renderer_free
 *
 * @sys_context: global system context
 * @renderer: the renderer object
 *
 * Frees a voxel rendering object (todo, not implemented yet).
 *
 * Return value: none
 */
void
lw6vox_renderer_free (lw6sys_context_t * sys_context, lw6vox_renderer_t * renderer)
{
  _lw6vox_renderer_free (sys_context, (_lw6vox_renderer_t *) renderer);
}
