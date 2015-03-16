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

#define SUB "gfx/gl1"
#define CAPTURE "gl1-capture"
#define BITMAP "gl1-bitmap"

int
mod_gl1_utils_path_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int argc, const char *argv[])
{
  char *data_root_dir = NULL;
  char *user_dir = NULL;
  int ret = 0;

  data_root_dir = lw6sys_get_data_dir (sys_context, argc, argv);
  if (data_root_dir)
    {
      utils_context->path.data_dir = lw6sys_path_concat (sys_context, data_root_dir, SUB);
      LW6SYS_FREE (sys_context, data_root_dir);
    }

  user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
  if (user_dir)
    {
      utils_context->path.capture_dir = lw6sys_path_concat (sys_context, user_dir, CAPTURE);
      utils_context->path.bitmap_dir = lw6sys_path_concat (sys_context, user_dir, BITMAP);
      mod_gl1_utils_path_update (sys_context, utils_context);
      LW6SYS_FREE (sys_context, user_dir);
    }

  ret = (utils_context->path.data_dir && utils_context->path.capture_dir && utils_context->path.bitmap_dir);

  return ret;
}

int
mod_gl1_utils_path_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  int ret = 0;
  char *frame_str = NULL;

  if (utils_context->path.bitmap_dir)
    {
      if (utils_context->path.bitmap_frame_dir)
	{
	  LW6SYS_FREE (sys_context, utils_context->path.bitmap_frame_dir);
	}
      frame_str = lw6sys_itoa (sys_context, utils_context->counter.nb_frames);
      if (frame_str)
	{
	  utils_context->path.bitmap_frame_dir = lw6sys_path_concat (sys_context, utils_context->path.bitmap_dir, frame_str);
	  LW6SYS_FREE (sys_context, frame_str);
	  ret = 1;
	}
    }

  return ret;
}

void
mod_gl1_utils_path_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  if (utils_context->path.data_dir)
    {
      LW6SYS_FREE (sys_context, utils_context->path.data_dir);
    }
  if (utils_context->path.capture_dir)
    {
      LW6SYS_FREE (sys_context, utils_context->path.capture_dir);
    }
  if (utils_context->path.bitmap_dir)
    {
      LW6SYS_FREE (sys_context, utils_context->path.bitmap_dir);
    }
  if (utils_context->path.bitmap_frame_dir)
    {
      LW6SYS_FREE (sys_context, utils_context->path.bitmap_frame_dir);
    }
}
