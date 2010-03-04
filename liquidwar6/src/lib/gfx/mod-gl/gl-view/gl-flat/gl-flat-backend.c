/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "../../../gfx.h"
#include "gl-flat.h"
#include "gl-flat-internal.h"

mod_gl_view_backend_t *
mod_gl_view_flat_create_backend (mod_gl_utils_context_t * utils_context)
{
  mod_gl_view_backend_t *backend;

  backend =
    (mod_gl_view_backend_t *) LW6SYS_MALLOC (sizeof (mod_gl_view_backend_t));
  if (backend)
    {
      memset (backend, 0, sizeof (mod_gl_view_backend_t));

      backend->display_preview = mod_gl_view_flat_display_preview;
      backend->display_map = mod_gl_view_flat_display_map;
      backend->display_fighters = mod_gl_view_flat_display_fighters;
      backend->display_cursors = mod_gl_view_flat_display_cursors;
      backend->init = mod_gl_view_flat_init;
      backend->quit = mod_gl_view_flat_quit;
    }

  return backend;
}
