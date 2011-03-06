/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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
_mod_ogg_init (int argc, char *argv[], float sound_volume, float music_volume)
{
  _mod_ogg_context_t *snd_context = NULL;
  int sdl_ok = 1;
  int ok = 0;
  SDL_version version;

  lw6sys_log (LW6SYS_LOG_INFO,
	      _x_ ("ogg init volume=%01.2f/%01.2f"), sound_volume,
	      music_volume);

  snd_context =
    (_mod_ogg_context_t *) LW6SYS_CALLOC (sizeof (_mod_ogg_context_t));
  if (snd_context)
    {
      if (_mod_ogg_path_init (snd_context, argc, argv))
	{
	  memset (&version, 0, sizeof (SDL_version));
	  SDL_VERSION (&version);
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("SDL header version when compiled %u.%u.%u"),
		      version.major, version.minor, version.patch);
	  version = *SDL_Linked_Version ();
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("SDL linked version now at runtime %u.%u.%u"),
		      version.major, version.minor, version.patch);

	  _mod_ogg_load_consts (snd_context);

	  if (lw6sys_sdl_register ())
	    {
	      sdl_ok = !SDL_Init (SDL_INIT_EVENTTHREAD);
	    }

	  if (!SDL_WasInit (SDL_INIT_EVENTTHREAD))
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("unable to start SDL event thread, using poll() function"));
	    }

	  sdl_ok = sdl_ok && (SDL_WasInit (SDL_INIT_AUDIO)
			      || !SDL_InitSubSystem (SDL_INIT_AUDIO));

	  if (sdl_ok)
	    {
	      if (!Mix_OpenAudio
		  (snd_context->const_data.frequency,
		   (snd_context->const_data.bits ==
		    8) ? AUDIO_U8 : AUDIO_S16SYS,
		   snd_context->const_data.channels,
		   snd_context->const_data.chunksize))
		{
		  _mod_ogg_set_sound_volume (snd_context, sound_volume);
		  _mod_ogg_set_music_volume (snd_context, music_volume);
		  if (_mod_ogg_load_sounds (snd_context))
		    {
		      ok = 1;
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_ ("SDL_mixer init error: \"%s\""),
			      Mix_GetError ());
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("SDL init error: \"%s\""), SDL_GetError ());
	    }
	}
    }

  if (!ok)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to init mod_ogg"));
      _mod_ogg_quit (snd_context);
      snd_context = NULL;
    }

  return snd_context;
}

void
_mod_ogg_quit (_mod_ogg_context_t * snd_context)
{
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("ogg quit"));

  _mod_ogg_stop_music (snd_context);

  Mix_CloseAudio ();

  _mod_ogg_unload_sounds (snd_context);
  _mod_ogg_unload_consts (snd_context);

  SDL_QuitSubSystem (SDL_INIT_AUDIO);

  if (lw6sys_sdl_unregister ())
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("SDL Quit"));
      SDL_Quit ();
    }

  _mod_ogg_path_quit (snd_context);

  LW6SYS_FREE (snd_context);
}
