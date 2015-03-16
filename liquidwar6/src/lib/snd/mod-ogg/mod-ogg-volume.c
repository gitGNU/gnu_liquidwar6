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
#include "mod-ogg-internal.h"

extern void
_mod_ogg_set_fx_volume (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, float volume)
{
  int i = 0;

  ogg_context->volume.fx = volume;

  for (i = _MOD_OGG_CHANNEL_FX0; i < ogg_context->mixer.nb_channels; ++i)
    {
      Mix_Volume (i, volume * MIX_MAX_VOLUME);
    }
}

extern void
_mod_ogg_set_water_volume (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, float volume)
{
  ogg_context->volume.water = volume;

  if (ogg_context->mixer.nb_channels > _MOD_OGG_CHANNEL_WATER2)
    {
      Mix_Volume (_MOD_OGG_CHANNEL_WATER1, volume * MIX_MAX_VOLUME);
      Mix_Volume (_MOD_OGG_CHANNEL_WATER2, volume * MIX_MAX_VOLUME);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("not even channels (%d) to handle water"), ogg_context->mixer.nb_channels);
    }
}

extern void
_mod_ogg_set_music_volume (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, float volume)
{
  ogg_context->volume.music = volume;

  Mix_VolumeMusic (volume * MIX_MAX_VOLUME);
}
