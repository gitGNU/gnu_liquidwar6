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

int
mod_gl1_utils_joystick_init (lw6sys_context_t * sys_context, mod_gl1_utils_joysticks_info_t * joysticks_info)
{
  int ret = 0;
  int i;

  joysticks_info->driver_nb_joysticks = SDL_NumJoysticks ();
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("%d joystick(s) initialized"), joysticks_info->driver_nb_joysticks);
  for (i = 0; i < joysticks_info->driver_nb_joysticks; ++i)
    {
      if (i < LW6GUI_NB_JOYSTICKS)
	{
	  joysticks_info->sdl_joysticks[i] = SDL_JoystickOpen (i);
	  if (joysticks_info->sdl_joysticks[i])
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joystick %d \"%s\" enabled"), i, SDL_JoystickName (i));
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to initialize joystick %d \"%s\" enabled"), i, SDL_JoystickName (i));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		      _x_ ("joystick %d \"%s\" ignored, only %d joysticks supported"), i, SDL_JoystickName (i), LW6GUI_NB_JOYSTICKS);
	}
    }

  if (joysticks_info->driver_nb_joysticks > 0)
    {
      SDL_JoystickEventState (SDL_ENABLE);
    }

  return ret;
}

void
mod_gl1_utils_joystick_quit (lw6sys_context_t * sys_context, mod_gl1_utils_joysticks_info_t * joysticks_info)
{
  int i;

  for (i = 0; i < joysticks_info->driver_nb_joysticks && i < LW6GUI_NB_JOYSTICKS; ++i)
    {
      if (joysticks_info->sdl_joysticks[i])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joystick %d \"%s\" disabled"), i, SDL_JoystickName (i));
	  SDL_JoystickClose (joysticks_info->sdl_joysticks[i]);
	}
    }
}
