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

#define FX_DIR "fx"

int
_mod_ogg_play_fx (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, int fx_id)
{
  int ret = 0;
  int channel = -1;

  if (fx_id >= 0 && fx_id < LW6SND_NB_FX)
    {
      if (ogg_context->fx.fx[fx_id])
	{
	  channel = Mix_GroupAvailable (_MOD_OGG_CHANNEL_GROUP_FX);
	  if (channel < 0)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("no free channel for fx, trying to use the oldest"));
	      channel = Mix_GroupOldest (_MOD_OGG_CHANNEL_GROUP_FX);
	    }
	  if (channel < 0)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to find channel for fx, using default"));
	      channel = _MOD_OGG_CHANNEL_FX0;
	    }
	  if (channel >= _MOD_OGG_CHANNEL_FX0 && channel < ogg_context->mixer.nb_channels)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("play sound fx %d on channel %d"), fx_id, channel);
	      Mix_PlayChannel (channel, ogg_context->fx.fx[fx_id], 0);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("not enough channels (%d) to play fx"), ogg_context->mixer.nb_channels);
	    }
	  ogg_context->mixer.last_channel = channel;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to play fx %d, was not correctly loaded"), fx_id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to play fx %d, index out of range"), fx_id);
    }

  ret = 1;

  return ret;
}

static Mix_Chunk *
_load_fx (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *file)
{
  char *path1 = NULL;
  char *path2 = NULL;
  Mix_Chunk *ret = NULL;

  if (file)
    {
      path1 = lw6sys_path_concat (sys_context, FX_DIR, file);
      if (path1)
	{
	  path2 = lw6sys_path_concat (sys_context, ogg_context->path.data_dir, path1);
	  if (path2)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading fx \"%s\""), path2);
	      ret = Mix_LoadWAV (path2);
	      if (!ret)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to load fx \"%s\" Mix_GetError returned \"%s\""), path2, Mix_GetError ());
		}
	      LW6SYS_FREE (sys_context, path2);
	    }
	  LW6SYS_FREE (sys_context, path1);
	}
    }

  return ret;
}

int
_mod_ogg_load_fx (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context)
{
  int ret = 0;
  int i;

  ogg_context->fx.fx[LW6SND_FX_SPLASH] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_splash);
  ogg_context->fx.fx[LW6SND_FX_VALID] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_valid);
  ogg_context->fx.fx[LW6SND_FX_SELECT] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_select);
  ogg_context->fx.fx[LW6SND_FX_NO] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_no);
  ogg_context->fx.fx[LW6SND_FX_START] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_start);
  ogg_context->fx.fx[LW6SND_FX_DEATH] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_death);
  ogg_context->fx.fx[LW6SND_FX_BELL] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_bell);
  ogg_context->fx.fx[LW6SND_FX_SCORE] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_score);
  ogg_context->fx.fx[LW6SND_FX_HELLO] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_hello);
  ogg_context->fx.fx[LW6SND_FX_GOODBYE] = _load_fx (sys_context, ogg_context, ogg_context->const_data.file_goodbye);

  ret = 1;
  for (i = 0; i < LW6SND_NB_FX; ++i)
    {
      if (!ogg_context->fx.fx[i])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to load fx %d"), i);
	  ret = 0;
	}
    }

  return ret;
}

void
_mod_ogg_unload_fx (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context)
{
  int i;

  for (i = 0; i < LW6SND_NB_FX; ++i)
    {
      if (ogg_context->fx.fx[i])
	{
	  Mix_FreeChunk (ogg_context->fx.fx[i]);
	}
      ogg_context->fx.fx[i] = NULL;
    }
}
