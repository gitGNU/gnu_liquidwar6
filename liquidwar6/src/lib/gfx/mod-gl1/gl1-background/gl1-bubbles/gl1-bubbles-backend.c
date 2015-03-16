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

#include "../../../gfx.h"
#include "gl1-bubbles.h"
#include "gl1-bubbles-internal.h"

mod_gl1_background_backend_t *
mod_gl1_background_bubbles_create_backend (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  mod_gl1_background_backend_t *backend;

  backend = (mod_gl1_background_backend_t *) LW6SYS_MALLOC (sys_context, sizeof (mod_gl1_background_backend_t));
  if (backend)
    {
      memset (backend, 0, sizeof (mod_gl1_background_backend_t));

      backend->display_background = mod_gl1_background_bubbles_display_background;
      backend->init = mod_gl1_background_bubbles_init;
      backend->quit = mod_gl1_background_bubbles_quit;
    }

  return backend;
}
