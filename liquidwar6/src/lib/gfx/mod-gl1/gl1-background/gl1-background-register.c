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

#include <string.h>

#include "../mod-gl1.h"

#include "gl1-bubbles/gl1-bubbles.h"
#include "gl1-void/gl1-void.h"

mod_gl1_background_backend_t *
mod_gl1_background_create_backend (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *name)
{
  mod_gl1_background_backend_t *(*init_func) (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context) = NULL;
  mod_gl1_background_backend_t *backend = NULL;

  if (strcmp (name, "bubbles") == 0)
    {
      init_func = mod_gl1_background_bubbles_create_backend;
    }
  else if (strcmp (name, "void") == 0)
    {
      init_func = mod_gl1_background_void_create_backend;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("background backend \"%s\" does not exists"), name);
    }

  if (init_func)
    {
      backend = init_func (sys_context, utils_context);
      if (backend)
	{
	  backend->backend_name = lw6sys_str_copy (sys_context, name);
	}
    }

  return backend;
}

void
mod_gl1_background_destroy_backend (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_background_backend_t * backend)
{
  if (backend)
    {
      if (backend->backend_name)
	{
	  LW6SYS_FREE (sys_context, backend->backend_name);
	}
      LW6SYS_FREE (sys_context, backend);
    }
}
