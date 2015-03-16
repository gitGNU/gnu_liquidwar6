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

#include "../../gfx.h"
#include "gl1-utils.h"


int
mod_gl1_utils_cache_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look)
{
  int ret = 1;

  if (!lw6map_color_set_is_same (sys_context, &(utils_context->cache.color_set), &(look->style.color_set)))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("updating cache"));
      ret = ret && mod_gl1_utils_texture_1x1_update (sys_context, utils_context, look);
      mod_gl1_utils_clear_menucache (sys_context, utils_context);

      utils_context->cache.color_set = look->style.color_set;
    }

  return ret;
}
