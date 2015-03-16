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
#endif // HAVE_CONFIG_H

#include "mod-soft-internal.h"

int
_mod_soft_get_fullscreen_modes (lw6sys_context_t * sys_context, _mod_soft_context_t * soft_context, lw6gui_fullscreen_modes_t * modes)
{
  SDL_Rect **sdl_modes = NULL;
  int i = 0;
  int ret = 0;

  modes->low.width = LW6GUI_DEFAULT_LOW_WIDTH;
  modes->low.height = LW6GUI_DEFAULT_LOW_HEIGHT;
  modes->low.fullscreen = 1;
  modes->low.width = LW6GUI_DEFAULT_STANDARD_WIDTH;
  modes->low.height = LW6GUI_DEFAULT_STANDARD_HEIGHT;
  modes->low.fullscreen = 1;
  modes->high.width = LW6GUI_DEFAULT_HIGH_WIDTH;
  modes->high.height = LW6GUI_DEFAULT_HIGH_HEIGHT;
  modes->high.fullscreen = 1;

  sdl_modes = SDL_ListModes (NULL, SDL_OPENGL | SDL_FULLSCREEN);
  if (sdl_modes)
    {
      for (i = 0; sdl_modes[i]; ++i)
	{
	  modes->low.width = sdl_modes[i]->w;
	  modes->low.height = sdl_modes[i]->h;
	}

      if (sdl_modes[0])
	{
	  modes->high.width = sdl_modes[0]->w;
	  modes->high.height = sdl_modes[0]->h;
	}

      ret = 1;
    }

  modes->low.width = lw6sys_imin (modes->low.width, modes->high.width / 2);
  modes->low.height = lw6sys_imin (modes->low.height, modes->high.height / 2);

  modes->standard.width = (modes->low.width + modes->high.width) / 2;
  modes->standard.height = (modes->low.height + modes->high.height) / 2;

  lw6gui_video_mode_sync_ratio (sys_context, &(modes->standard), &(modes->high));

  _mod_soft_find_closest_resolution (sys_context, soft_context, &(modes->standard.width), &(modes->standard.height), modes->standard.width,
				     modes->standard.height);

  return ret;
}

void
_mod_soft_find_closest_resolution (lw6sys_context_t * sys_context, _mod_soft_context_t * soft_context, int *closest_width, int *closest_height,
				   int wished_width, int wished_height)
{
  SDL_Rect **sdl_modes = NULL;
  int i = 0;
  lw6sys_list_t *mode_list;
  lw6gui_video_mode_t *video_mode;
  lw6gui_video_mode_t closest;
  lw6gui_video_mode_t wished;

  (*closest_width) = wished_width;
  (*closest_height) = wished_height;

  sdl_modes = SDL_ListModes (NULL, SDL_OPENGL | SDL_FULLSCREEN);
  if (sdl_modes)
    {
      mode_list = lw6sys_list_new (sys_context, lw6sys_free_callback);
      if (mode_list)
	{
	  for (i = 0; sdl_modes[i]; ++i)
	    {
	      video_mode = (lw6gui_video_mode_t *) LW6SYS_MALLOC (sys_context, sizeof (lw6gui_video_mode_t));
	      if (video_mode)
		{
		  video_mode->width = sdl_modes[i]->w;
		  video_mode->height = sdl_modes[i]->h;
		  video_mode->fullscreen = LW6GUI_DEFAULT_FULLSCREEN;
		  lw6sys_lifo_push (sys_context, &mode_list, video_mode);
		}
	    }
	  wished.width = wished_width;
	  wished.height = wished_height;
	  wished.fullscreen = LW6GUI_DEFAULT_FULLSCREEN;

	  lw6gui_video_mode_find_closest (sys_context, &closest, &wished, mode_list);

	  (*closest_width) = closest.width;
	  (*closest_height) = closest.height;

	  lw6sys_list_free (sys_context, mode_list);
	}
    }
}
