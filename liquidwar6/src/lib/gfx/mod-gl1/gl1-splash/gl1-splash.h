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

#ifndef LIQUIDWAR6GFX_MOD_GL1_SPLASH_H
#define LIQUIDWAR6GFX_MOD_GL1_SPLASH_H

#include "../../gfx.h"
#include "../gl1-utils/gl1-utils.h"


/* gl1-splash-display.c */
extern void mod_gl1_splash_display (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *splash_context);
extern void mod_gl1_splash_patch_system_color (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *splash_context,
					       lw6map_color_couple_t * system_color);

/* gl1-splash-setup.c */
extern void *mod_gl1_splash_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_splash_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *splash_context);

#endif // LIQUIDWAR6GFX_MOD_GL1_SPLASH_H
