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

#include "ldr.h"
#include "ldr-internal.h"

/**
 * lw6ldr_cursor_texture_read
 *
 * @sys_context: global system context
 * @cursor_texture: the cursor texture (out param)
 * @dirname: the directory we load the data form (map dir)
 *
 * Reads the cursor texture information, if not available, will use defaults
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ldr_cursor_texture_read (lw6sys_context_t * sys_context, lw6map_cursor_texture_t * cursor_texture, const char *dirname)
{
  int ret = 1;
  int found = 0;
  char *cursor_dot_png;
  char *cursor_color_dot_png;

  lw6map_cursor_texture_clear (sys_context, cursor_texture);

  cursor_dot_png = lw6sys_path_concat (sys_context, dirname, _LW6LDR_FILE_CURSOR_PNG);
  if (cursor_dot_png)
    {
      if (lw6sys_file_exists (sys_context, cursor_dot_png))
	{
	  found = 1;
	  ret = ret && _lw6ldr_cursor_texture_layer_read_png (sys_context, &(cursor_texture->fg_bg_layer), cursor_dot_png);
	}
      LW6SYS_FREE (sys_context, cursor_dot_png);
    }

  cursor_color_dot_png = lw6sys_path_concat (sys_context, dirname, _LW6LDR_FILE_CURSOR_COLOR_PNG);
  if (cursor_color_dot_png)
    {
      if (lw6sys_file_exists (sys_context, cursor_color_dot_png))
	{
	  found = 1;
	  ret = ret && _lw6ldr_cursor_texture_layer_read_png (sys_context, &(cursor_texture->color_layer), cursor_color_dot_png);
	}
      LW6SYS_FREE (sys_context, cursor_color_dot_png);
    }

  if (!found)
    {
      lw6map_cursor_texture_builtin (sys_context, cursor_texture);
    }

  return ret;
}
