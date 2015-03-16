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

#include "gl1-hud.h"

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("hud backend function \"%s\" is not defined"), func_name);
}

int
mod_gl1_hud_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_hud_backend_t * backend)
{
  if (backend->init)
    {
      backend->hud_context = backend->init (sys_context, utils_context);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return backend->hud_context ? 1 : 0;
}

void
mod_gl1_hud_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_hud_backend_t * backend)
{
  if (backend->quit)
    {
      backend->quit (sys_context, utils_context, backend->hud_context);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}

void
mod_gl1_hud_display_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			 mod_gl1_hud_backend_t * backend,
			 const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  if (backend->display_hud)
    {
      backend->display_hud (sys_context, utils_context, backend->hud_context, look, game_state, local_cursors);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}

void
mod_gl1_hud_display_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			   mod_gl1_hud_backend_t * backend,
			   const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  if (backend->display_score)
    {
      backend->display_score (sys_context, utils_context, backend->hud_context, look, game_state, local_cursors);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}
