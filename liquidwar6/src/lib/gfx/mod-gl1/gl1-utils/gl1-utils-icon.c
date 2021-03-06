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

#include <time.h>

#include "../../gfx.h"
#include "gl1-utils.h"

#define ICON_DIR "utils/icon/"

#ifdef LW6_MS_WINDOWS
#define ICON_PNG "liquidwar6-icon-32x32.png"
#else
#define ICON_PNG "liquidwar6-icon-128x128.png"
#endif

int
mod_gl1_utils_icon_set (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  int ret = 0;

  utils_context->icon.bitmap = mod_gl1_utils_bitmap_load (sys_context, utils_context, ICON_DIR ICON_PNG);
  if (utils_context->icon.bitmap)
    {
      SDL_WM_SetIcon (utils_context->icon.bitmap->surface, NULL);
      ret = 1;
    }

  return ret;
}

void
mod_gl1_utils_icon_unset (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  if (utils_context->icon.bitmap)
    {
      mod_gl1_utils_bitmap_free (sys_context, utils_context, utils_context->icon.bitmap);
    }
}
