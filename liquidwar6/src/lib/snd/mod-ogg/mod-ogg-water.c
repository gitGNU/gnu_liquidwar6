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

#define WATER_DIR "water"

void
_mod_ogg_poll_water (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context)
{
  int channel = 0;
  int water_id = 0;

  for (channel = _MOD_OGG_CHANNEL_WATER1; channel <= _MOD_OGG_CHANNEL_WATER2; ++channel)
    {
      if (Mix_Playing (channel))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("OK, channel %d playing"), channel);
	}
      else
	{
	  water_id = lw6sys_random (sys_context, _MOD_OGG_NB_WATER);
	  if (ogg_context->water.water[water_id])
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("play water %d on channel %d"), water_id, channel);
	      Mix_PlayChannel (channel, ogg_context->water.water[water_id], 0);
	    }
	}
    }
}

static Mix_Chunk *
_load_water (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *file)
{
  char *path1 = NULL;
  char *path2 = NULL;
  Mix_Chunk *ret = NULL;

  if (file)
    {
      path1 = lw6sys_path_concat (sys_context, WATER_DIR, file);
      if (path1)
	{
	  path2 = lw6sys_path_concat (sys_context, ogg_context->path.data_dir, path1);
	  if (path2)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading water \"%s\""), path2);
	      ret = Mix_LoadWAV (path2);
	      if (!ret)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to load water \"%s\" Mix_GetError returned \"%s\""), path2, Mix_GetError ());
		}
	      LW6SYS_FREE (sys_context, path2);
	    }
	  LW6SYS_FREE (sys_context, path1);
	}
    }

  return ret;
}

int
_mod_ogg_load_water (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context)
{
  int ret = 0;
  int i;

  ogg_context->water.water[_MOD_OGG_WATER1] = _load_water (sys_context, ogg_context, ogg_context->const_data.file_water1);
  ogg_context->water.water[_MOD_OGG_WATER2] = _load_water (sys_context, ogg_context, ogg_context->const_data.file_water2);
  ogg_context->water.water[_MOD_OGG_WATER3] = _load_water (sys_context, ogg_context, ogg_context->const_data.file_water3);
  ogg_context->water.water[_MOD_OGG_WATER4] = _load_water (sys_context, ogg_context, ogg_context->const_data.file_water4);
  ogg_context->water.water[_MOD_OGG_WATER5] = _load_water (sys_context, ogg_context, ogg_context->const_data.file_water5);
  ogg_context->water.water[_MOD_OGG_WATER6] = _load_water (sys_context, ogg_context, ogg_context->const_data.file_water6);

  ret = 1;
  for (i = 0; i < _MOD_OGG_NB_WATER; ++i)
    {
      if (!ogg_context->water.water[i])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to load water %d"), i);
	  ret = 0;
	}
    }

  return ret;
}

void
_mod_ogg_unload_water (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context)
{
  int i;

  for (i = 0; i < _MOD_OGG_NB_WATER; ++i)
    {
      if (ogg_context->water.water[i])
	{
	  Mix_FreeChunk (ogg_context->water.water[i]);
	}
      ogg_context->water.water[i] = NULL;
    }
}
