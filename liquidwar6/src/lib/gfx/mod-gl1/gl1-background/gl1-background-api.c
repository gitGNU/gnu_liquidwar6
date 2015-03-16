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

#include "gl1-background.h"

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("background backend function \"%s\" is not defined"), func_name);
}

int
mod_gl1_background_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_background_backend_t * backend)
{
  if (backend->init)
    {
      backend->background_context = backend->init (sys_context, utils_context);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return backend->background_context ? 1 : 0;
}

void
mod_gl1_background_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_background_backend_t * backend)
{
  if (backend->quit)
    {
      backend->quit (sys_context, utils_context, backend->background_context);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}

void
mod_gl1_background_display_background (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_background_backend_t * backend,
				       const lw6gui_look_t * look)
{
  if (backend->display_background)
    {
      backend->display_background (sys_context, utils_context, backend->background_context, look);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}
