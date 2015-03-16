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

#ifndef LIQUIDWAR6VOX_H
#define LIQUIDWAR6VOX_H

#include "../sys/sys.h"
#include "../hlp/hlp.h"
#include "../cfg/cfg.h"
#include "../map/map.h"
#include "../ker/ker.h"
#include "../mat/mat.h"
#include "../gui/gui.h"

/**
 * Voxel renderer object, not implemented yet.
 */
typedef struct lw6vox_renderer_s
{
  /// Todo...
  int dummy;
} lw6vox_renderer_t;

/* vox-renderer.c */
extern lw6vox_renderer_t *lw6vox_renderer_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state);
extern void lw6vox_renderer_free (lw6sys_context_t * sys_context, lw6vox_renderer_t * renderer);

/* vox-test.c */
extern int lw6vox_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6vox_test_run (lw6sys_context_t * sys_context, int mode);

#endif
