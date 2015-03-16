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

static char *_MUSIC_EXTS[] = { ".ogg", ".mp3", ".wav", ".mid", ".midi", ".mod", ".xm", ".s3m", NULL };

int
_mod_ogg_is_music_file (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *music_file)
{
  int ret = 0;
  int i = 0;
  char *music_file_tmp = NULL;

  if (music_file && strlen (music_file) > 0)
    {
      /*
       * We copy this for we're going to modify it in place
       */
      music_file_tmp = lw6sys_str_copy (sys_context, music_file);
      if (music_file_tmp)
	{
	  lw6sys_str_tolower (sys_context, music_file_tmp);
	  if ((ogg_context->music.filter == NULL) || (strlen (ogg_context->music.filter) == 0) || (strstr (music_file_tmp, ogg_context->music.filter)))
	    {
	      if ((ogg_context->music.exclude == NULL) || (strlen (ogg_context->music.exclude) == 0) || (!strstr (music_file_tmp, ogg_context->music.exclude)))
		{
		  while (_MUSIC_EXTS[i])
		    {
		      if (strstr (music_file_tmp, _MUSIC_EXTS[i]))
			{
			  ret = 1;
			}
		      i++;
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, music_file_tmp);
	}
      if (ret)
	{
	  if (lw6sys_file_exists (sys_context, music_file))
	    {
	      ret = 1;
	    }
	  else
	    {
	      ret = 0;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("file \"%s\" is not a music file, it can't be opened"), music_file);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("file \"%s\" is not a music file, extension is unknown"), music_file);
	}
    }

  return ret;
}

int
_mod_ogg_play_music_file (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *music_file)
{
  int ret = 0;

  _mod_ogg_stop_music (sys_context, ogg_context);

  if (!ogg_context->music.music)
    {
      ogg_context->music.music = Mix_LoadMUS (music_file);
      if (ogg_context->music.music)
	{
	  if (Mix_PlayMusic (ogg_context->music.music, -1) == 0)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("playing music file \"%s\""), music_file);
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't play \%s\", SDL_mixer says \"%s\""), music_file, Mix_GetError ());
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't load \%s\", SDL_mixer says \"%s\""), music_file, Mix_GetError ());
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't play music \"%s\", one seems already active"), music_file);
    }

  return ret;
}

static int
_is_music_callback_func (lw6sys_context_t * sys_context, void *func_data, char *file)
{
  int ret = 0;
  _mod_ogg_context_t *ogg_context = (_mod_ogg_context_t *) func_data;

  ret = _mod_ogg_is_music_file (sys_context, ogg_context, file);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("found file \"%s\" is_music_file=%d"), file, ret);

  return ret;
}

int
_mod_ogg_play_music_random (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *music_path, char *music_filter, char *music_exclude)
{
  int ret = 0;
  lw6sys_list_t *list = NULL;
  char *music_file = NULL;
  char *path = NULL;
  int i = 0;
  int j = 0;
  int n = 0;

  if (music_filter && strlen (music_filter) > 0)
    {
      ogg_context->music.filter = lw6sys_str_copy (sys_context, music_filter);
      lw6sys_str_tolower (sys_context, ogg_context->music.filter);
    }
  if (music_exclude && strlen (music_exclude) > 0)
    {
      ogg_context->music.exclude = lw6sys_str_copy (sys_context, music_exclude);
      lw6sys_str_tolower (sys_context, ogg_context->music.exclude);
    }

  list = lw6sys_path_list (sys_context, music_path, _is_music_callback_func, (void *) ogg_context, &n);
  if (list)
    {
      j = lw6sys_random (sys_context, n);
      while (list && ((path = ((char *) lw6sys_list_pop_front (sys_context, &list))) != NULL))
	{
	  if ((i == j) && (!music_file))
	    {
	      music_file = path;
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, path);
	    }
	  i++;
	}
      if (music_file)
	{
	  ret = _mod_ogg_play_music_file (sys_context, ogg_context, music_file);
	  LW6SYS_FREE (sys_context, music_file);
	}
      // list is freed at this stage
    }

  if (ogg_context->music.filter)
    {
      LW6SYS_FREE (sys_context, ogg_context->music.filter);
      ogg_context->music.filter = NULL;
    }
  if (ogg_context->music.exclude)
    {
      LW6SYS_FREE (sys_context, ogg_context->music.exclude);
      ogg_context->music.exclude = NULL;
    }

  return ret;
}

void
_mod_ogg_stop_music (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context)
{
  if (ogg_context->music.music)
    {
      Mix_FreeMusic (ogg_context->music.music);
      ogg_context->music.music = NULL;
    }
}
