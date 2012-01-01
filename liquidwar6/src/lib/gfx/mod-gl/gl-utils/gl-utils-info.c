/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include <time.h>

#include "../../gfx.h"
#include "gl-utils.h"

void
mod_gl_utils_display_log (mod_gl_utils_context_t * context,
			  lw6gui_look_t * look, char **log_list)
{
  mod_gl_utils_draw_system_text_bottom_left (context, look, log_list);
}

void
mod_gl_utils_display_fps (mod_gl_utils_context_t * context,
			  lw6gui_look_t * look, float fps)
{
  char *list_fps[2];

  if (fps >= 0)
    {
      list_fps[0] = lw6sys_itoa (fps);
      list_fps[1] = NULL;

      if (list_fps[0])
	{
	  mod_gl_utils_draw_system_text_top_left (context, look, list_fps);
	  LW6SYS_FREE (list_fps[0]);
	}
    }
}

void
mod_gl_utils_display_mps (mod_gl_utils_context_t * context,
			  lw6gui_look_t * look, float mps, int target_mps)
{
  char *list_mps[3];

  if (mps >= 0.0f)
    {
      list_mps[0] = " ";
      list_mps[1] = lw6sys_new_sprintf ("%d/%d", (int) mps, target_mps);
      list_mps[2] = NULL;

      if (list_mps[0])
	{
	  mod_gl_utils_draw_system_text_top_left (context, look, list_mps);
	  LW6SYS_FREE (list_mps[1]);
	}
    }
}

void
mod_gl_utils_display_url (mod_gl_utils_context_t * context,
			  lw6gui_look_t * look, const char *url)
{
  char *list_url[2];

  if (url)
    {
      /*
       * Cast else we'd get a const warning.
       */
      list_url[0] = (char *) url;
      list_url[1] = NULL;
      mod_gl_utils_draw_system_text_bottom_right (context, look, list_url);
    }
}
