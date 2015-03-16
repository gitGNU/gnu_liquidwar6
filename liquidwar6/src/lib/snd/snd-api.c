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

#include "snd.h"

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("snd backend function \"%s\" is not defined"), func_name);
}

/**
 * lw6snd_play_fx
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 * @fx_id: sound fx id
 *
 * Plays a sound fx.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6snd_play_fx (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, int fx_id)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->play_fx)
    {
      ret = backend->play_fx (sys_context, backend->snd_context, fx_id);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6snd_is_music_file
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 * @map_dir: map directory, to search additionnal files
 * @music_path: config entry containing multiple paths
 * @music_file: relative/local name of a music file
 *
 * Tells wether a file is a valid music file, typicallly based on file
 * existence and extension. Not bullet proof, file might actually not
 * be loadable, but chances are 99%.
 *
 * Return value: 1 if music file, 0 if not
 */
int
lw6snd_is_music_file (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, char *map_dir, char *music_path, char *music_file)
{
  int ret = 0;
  char *found_path;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->is_music_file)
    {
      found_path = lw6sys_find_in_dir_and_path (sys_context, map_dir, music_path, music_file);
      if (found_path)
	{
	  ret = backend->is_music_file (sys_context, backend->snd_context, found_path);
	  LW6SYS_FREE (sys_context, found_path);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6snd_play_music_file
 *
 * @backend: sound backend to use
 * @map_dir: map directory, to search additionnal files
 * @music_path: config entry containing multiple paths
 * @music_file: relative/local name of a music file
 *
 * Plays a music file.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6snd_play_music_file (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, char *map_dir, char *music_path, char *music_file)
{
  int ret = 0;
  char *found_path = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->play_music_file)
    {
      /*
       * If music file is empty, we just return 0 without
       * even asking for the actual backend.
       */
      if (strlen (music_file) > 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trying song \"%s\" with map_dir=\"%s\" and music_path=\"%s\""), music_file, map_dir, music_path);
	  found_path = lw6sys_find_in_dir_and_path (sys_context, map_dir, music_path, music_file);
	  if (found_path)
	    {
	      ret = backend->play_music_file (sys_context, backend->snd_context, found_path);
	      LW6SYS_FREE (sys_context, found_path);
	    }
	}
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6snd_play_music_random
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 * @map_dir: map directory, to search additionnal files
 * @music_path: config entry containing multiple paths
 * @music_filter: string filter, must be present
 * @music_exclude: string filter, must not be present
 *
 * Plays a random music file. The filter and exclude mecanisms
 * are not complete regex filters, only a quick and dirty feature
 * which should still help in some cases, such as sorting musics
 * for the menus and for the rest.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6snd_play_music_random (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, char *music_path, char *music_filter, char *music_exclude)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->play_music_random)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("picking a random song in \"%s\" using filter \"%s\" and exclude \"%s\""), music_path, music_filter, music_exclude);
      ret = backend->play_music_random (sys_context, backend->snd_context, music_path, music_filter, music_exclude);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6snd_stop_music
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 *
 * Stops the music.
 *
 * Return value: none.
 */
void
lw6snd_stop_music (lw6sys_context_t * sys_context, lw6snd_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->stop_music)
    {
      backend->stop_music (sys_context, backend->snd_context);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6snd_init
 *
 * @sys_context: global system context
 * @backend: the graphical backend to use
 * @fx_volume: sound fx volume
 * @water_volume: water sounds volume
 * @music_volume: music volume
 *
 * Sets up the sound backend for good, initializing a playback engine
 * ready to play sounds and set to defaults.
 * This call can typically fail if there's no device available, if
 * the user doesn't have enough rights to access the hardware, and so on.
 *
 * Return value: 1 on success, 0 if not
 */
int
lw6snd_init (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float fx_volume, float water_volume, float music_volume)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->init)
    {
      backend->snd_context = backend->init (sys_context, backend->argc, backend->argv, fx_volume, water_volume, music_volume);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->snd_context ? 1 : 0;
}

/**
 * lw6snd_set_fx_volume
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 * @volume: sound fx volume
 *
 * Changes sound fx volume.
 *
 * Return value: none.
 */
void
lw6snd_set_fx_volume (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float volume)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->set_fx_volume)
    {
      backend->set_fx_volume (sys_context, backend->snd_context, volume);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6snd_set_water_volume
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 * @volume: water sounds volume
 *
 * Changes water sounds volume.
 *
 * Return value: none.
 */
void
lw6snd_set_water_volume (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float volume)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->set_water_volume)
    {
      backend->set_water_volume (sys_context, backend->snd_context, volume);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6snd_set_music_volume
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 * @volume: music volume
 *
 * Changes music volume.
 *
 * Return value: none.
 */
void
lw6snd_set_music_volume (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float volume)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->set_music_volume)
    {
      backend->set_music_volume (sys_context, backend->snd_context, volume);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6snd_poll
 *
 * @sys_context: global system context
 * @backend: sound backend to use
 *
 * Polling function, must be called on a regular basis.
 *
 * Return value: none.
 */
void
lw6snd_poll (lw6sys_context_t * sys_context, lw6snd_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->poll)
    {
      if (backend->snd_context)
	{
	  backend->poll (sys_context, backend->snd_context);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6snd_quit
 *
 * @sys_context: global system context
 * @backend: the backend to quit
 *
 * Uninitializes the backend, that is, releases resources, stops playback.
 *
 * Return value: none.
 */
void
lw6snd_quit (lw6sys_context_t * sys_context, lw6snd_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->quit)
    {
      /*
       * It's important to check that backend is not NULL for
       * quit can *really* be called several times on the same backend
       */
      if (backend->snd_context)
	{
	  backend->quit (sys_context, backend->snd_context);
	  backend->snd_context = NULL;
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6snd_repr
 *
 * @sys_context: global system context
 * @backend: the backend to represent
 *
 * Returns a readable version of the backend object.
 *
 * Return value: a newly allocated pointer.
 */
char *
lw6snd_repr (lw6sys_context_t * sys_context, const lw6snd_backend_t * backend)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (sys_context, backend->snd_context, backend->id);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
