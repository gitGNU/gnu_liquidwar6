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

#ifndef LIQUIDWAR6GFX_MOD_GL1_MENU_H
#define LIQUIDWAR6GFX_MOD_GL1_MENU_H

#include <ltdl.h>

#include "../../../sys/sys.h"
#include "../../../map/map.h"
#include "../mod-gl1.h"
#include "../gl1-utils/gl1-utils.h"

typedef struct mod_gl1_menu_backend_s
{
  void *menu_context;
  char *backend_name;

  void *(*init) (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
  void (*quit) (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *menu_context);
  void (*display_menu) (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *menu_context, const lw6gui_look_t * look,
			lw6gui_menu_t * menu);
  void (*display_progress) (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *menu_context, const lw6gui_look_t * look,
			    float progress);
  void (*display_meta) (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *menu_context, const lw6gui_look_t * look,
			lw6gui_menu_t * menu);
}
mod_gl1_menu_backend_t;

/* gl1-menu-api.c */
extern int mod_gl1_menu_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_menu_backend_t * backend);
extern void mod_gl1_menu_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_menu_backend_t * backend);
extern void mod_gl1_menu_display_menu (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				       utils_context, mod_gl1_menu_backend_t * backend, const lw6gui_look_t * look, lw6gui_menu_t * menu);
extern void mod_gl1_menu_display_progress (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					   utils_context, mod_gl1_menu_backend_t * backend, const lw6gui_look_t * look, float progress);
extern void mod_gl1_menu_display_meta (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				       utils_context, mod_gl1_menu_backend_t * backend, const lw6gui_look_t * look, lw6gui_menu_t * menu);

/* gl1-menu-backend.c */
extern mod_gl1_menu_backend_t *mod_gl1_menu_create_backend (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *name);
extern void mod_gl1_menu_destroy_backend (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_menu_backend_t * backend);

#endif // LIQUIDWAR6GFX_MOD_GL1_MENU_H
