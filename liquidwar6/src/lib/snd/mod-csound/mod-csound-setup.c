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

#include "../snd.h"
#include "mod-csound-internal.h"

_mod_csound_context_t *
_mod_csound_init (lw6sys_context_t * sys_context, int argc, const char *argv[], float fx_volume, float water_volume, float music_volume)
{
  _mod_csound_context_t *csound_context = NULL;
  int ok = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("csound init volume=%01.2f/%01.2f"), fx_volume, music_volume);

  csound_context = (_mod_csound_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_csound_context_t));
  if (csound_context)
    {
      if (_mod_csound_path_init (sys_context, csound_context, argc, argv))
	{
	  _mod_csound_set_fx_volume (sys_context, csound_context, fx_volume);
	  _mod_csound_set_water_volume (sys_context, csound_context, water_volume);
	  _mod_csound_set_music_volume (sys_context, csound_context, music_volume);

	  ok = 1;
	}
    }

  if (!ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init csound module"));
      _mod_csound_quit (sys_context, csound_context);
    }

  return csound_context;
}

void
_mod_csound_poll (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("csound poll"));
}

void
_mod_csound_quit (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("csound quit"));
  _mod_csound_path_quit (sys_context, csound_context);
  LW6SYS_FREE (sys_context, csound_context);
}
