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

_mod_ogg_context_t *
_mod_ogg_init (lw6sys_context_t * sys_context, int argc, const char *argv[], float fx_volume, float water_volume, float music_volume)
{
  _mod_ogg_context_t *snd_context = NULL;
  int sdl_ok = 1;
  int ok = 0;
  SDL_version version;
  int pan1 = 0;
  int pan2 = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("ogg init volume=%01.2f/%01.2f/%01.2f"), fx_volume, water_volume, music_volume);

  snd_context = (_mod_ogg_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_ogg_context_t));
  if (snd_context)
    {
      if (_mod_ogg_path_init (sys_context, snd_context, argc, argv))
	{
	  memset (&version, 0, sizeof (SDL_version));
	  SDL_VERSION (&version);
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL header version when compiled %u.%u.%u"), version.major, version.minor, version.patch);
	  version = *SDL_Linked_Version ();
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL linked version now at runtime %u.%u.%u"), version.major, version.minor, version.patch);

	  _mod_ogg_load_consts (sys_context, snd_context);

	  if (lw6sys_sdl_register (sys_context))
	    {
	      sdl_ok = !SDL_Init (SDL_INIT_EVENTTHREAD);
	    }

	  if (!SDL_WasInit (SDL_INIT_EVENTTHREAD))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to start SDL event thread, using poll() function"));
	    }

	  sdl_ok = sdl_ok && (SDL_WasInit (SDL_INIT_AUDIO) || !SDL_InitSubSystem (SDL_INIT_AUDIO));

	  if (sdl_ok)
	    {
	      if (!Mix_OpenAudio
		  (snd_context->const_data.frequency,
		   (snd_context->const_data.bits == 8) ? AUDIO_U8 : AUDIO_S16SYS, snd_context->const_data.channels, snd_context->const_data.chunksize))
		{
		  snd_context->mixer.nb_channels = Mix_AllocateChannels (-1);
		  if (snd_context->mixer.nb_channels > _MOD_OGG_CHANNEL_FX0)
		    {
		      Mix_GroupChannel (_MOD_OGG_CHANNEL_WATER1, _MOD_OGG_CHANNEL_GROUP_WATER);
		      Mix_GroupChannel (_MOD_OGG_CHANNEL_WATER2, _MOD_OGG_CHANNEL_GROUP_WATER);
		      Mix_GroupChannels (_MOD_OGG_CHANNEL_FX0, snd_context->mixer.nb_channels - 1, _MOD_OGG_CHANNEL_GROUP_FX);
		      /*
		       * For some reason panning does not use MIX_MAX_VOLUME
		       */
		      pan1 = snd_context->const_data.water_pan1 * 255.0f;
		      pan2 = snd_context->const_data.water_pan2 * 255.0f;

		      if (!Mix_SetPanning (_MOD_OGG_CHANNEL_WATER1, pan1, pan2))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				      _x_ ("unable to set panning on channel %d (%s)"), _MOD_OGG_CHANNEL_WATER1, Mix_GetError ());
			}
		      if (!Mix_SetPanning (_MOD_OGG_CHANNEL_WATER2, pan2, pan1))
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				      _x_ ("unable to set panning on channel %d (%s)"), _MOD_OGG_CHANNEL_WATER2, Mix_GetError ());
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				  _x_ ("not enough channels (%d) to handle both water and sound fx"), snd_context->mixer.nb_channels);
		    }

		  _mod_ogg_set_fx_volume (sys_context, snd_context, fx_volume);
		  _mod_ogg_set_water_volume (sys_context, snd_context, water_volume);
		  _mod_ogg_set_music_volume (sys_context, snd_context, music_volume);
		  if (_mod_ogg_load_fx (sys_context, snd_context) && _mod_ogg_load_water (sys_context, snd_context))
		    {
		      ok = 1;
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("SDL_mixer init error: \"%s\""), Mix_GetError ());
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("SDL init error: \"%s\""), SDL_GetError ());
	    }
	}
    }

  if (!ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init mod_ogg"));
      _mod_ogg_quit (sys_context, snd_context);
      snd_context = NULL;
    }

  return snd_context;
}

void
_mod_ogg_poll (lw6sys_context_t * sys_context, _mod_ogg_context_t * snd_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("ogg poll"));

  _mod_ogg_poll_water (sys_context, snd_context);
}

void
_mod_ogg_quit (lw6sys_context_t * sys_context, _mod_ogg_context_t * snd_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("ogg quit"));

  _mod_ogg_stop_music (sys_context, snd_context);

  lw6sys_idle (sys_context);
  Mix_HaltChannel (-1);
  lw6sys_idle (sys_context);

  lw6sys_idle (sys_context);
  SDL_PauseAudio (1);
  lw6sys_idle (sys_context);
  Mix_CloseAudio ();
  lw6sys_idle (sys_context);

  _mod_ogg_unload_fx (sys_context, snd_context);
  _mod_ogg_unload_water (sys_context, snd_context);
  _mod_ogg_unload_consts (sys_context, snd_context);

  SDL_QuitSubSystem (SDL_INIT_AUDIO);

  if (lw6sys_sdl_unregister (sys_context))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("SDL Quit"));
      SDL_Quit ();
    }

  _mod_ogg_path_quit (sys_context, snd_context);

  LW6SYS_FREE (sys_context, snd_context);
}
