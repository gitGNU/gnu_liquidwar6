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

#include "snd.h"

static void
_warning (const char *func_name)
{
  lw6sys_log (LW6SYS_LOG_WARNING,
	      _x_ ("snd backend function \"%s\" is not defined"), func_name);
}

int
lw6snd_play_fx (lw6snd_backend_t * backend, int fx_id)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->play_fx)
    {
      ret = backend->play_fx (backend->snd_context, fx_id);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

int
lw6snd_is_music_file (lw6snd_backend_t * backend, char *map_dir,
		      char *music_path, char *music_file)
{
  int ret = 0;
  char *found_path;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->is_music_file)
    {
      found_path =
	lw6sys_find_in_dir_and_path (map_dir, music_path, music_file);
      if (found_path)
	{
	  ret = backend->is_music_file (backend->snd_context, found_path);
	  LW6SYS_FREE (found_path);
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

int
lw6snd_play_music_file (lw6snd_backend_t * backend, char *map_dir,
			char *music_path, char *music_file)
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
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("trying song \"%s\" with map_dir=\"%s\" and music_path=\"%d\""),
		      music_file, map_dir, music_path);
	  found_path =
	    lw6sys_find_in_dir_and_path (map_dir, music_path, music_file);
	  if (found_path)
	    {
	      ret =
		backend->play_music_file (backend->snd_context, found_path);
	      LW6SYS_FREE (found_path);
	    }
	}
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

int
lw6snd_play_music_random (lw6snd_backend_t * backend, char *music_path,
			  char *music_filter, char *music_exclude)
{
  int ret = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->play_music_random)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_
		  ("picking a random song in \"%s\" using filter \"%s\" and exclude \"%s\""),
		  music_path, music_filter, music_exclude);
      ret =
	backend->play_music_random (backend->snd_context, music_path,
				    music_filter, music_exclude);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

void
lw6snd_stop_music (lw6snd_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->stop_music)
    {
      backend->stop_music (backend->snd_context);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6snd_init (lw6snd_backend_t * backend, float fx_volume,
	     float music_volume)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->init)
    {
      backend->snd_context =
	backend->init (backend->argc, backend->argv, fx_volume,
		       music_volume);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->snd_context ? 1 : 0;
}

void
lw6snd_set_fx_volume (lw6snd_backend_t * backend, float volume)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->set_fx_volume)
    {
      backend->set_fx_volume (backend->snd_context, volume);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

void
lw6snd_set_music_volume (lw6snd_backend_t * backend, float volume)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->set_music_volume)
    {
      backend->set_music_volume (backend->snd_context, volume);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

void
lw6snd_quit (lw6snd_backend_t * backend)
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
	  backend->quit (backend->snd_context);
	  backend->snd_context = NULL;
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

char *
lw6snd_repr (lw6snd_backend_t * backend)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (backend->snd_context, backend->id);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
