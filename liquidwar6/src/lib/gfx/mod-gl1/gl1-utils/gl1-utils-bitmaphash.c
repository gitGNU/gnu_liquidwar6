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

#include "gl1-utils.h"

#define _BITMAP_HASH_APPROX_NB_ENTRIES 421
#define _FILE_MASK "%s-%u.bmp"
#define _DUMP_EVERY_N_FRAMES 100

lw6sys_hash_t *
mod_gl1_utils_bitmap_hash_init (lw6sys_context_t * sys_context)
{
  lw6sys_hash_t *ret = NULL;

  /*
   * Note, the free_func here is NULL, this is normal,
   * the hash is just here to keep track of the various
   * bitmaps, not to actually store them.
   */
  ret = lw6sys_hash_new (sys_context, NULL, _BITMAP_HASH_APPROX_NB_ENTRIES);

  return ret;
}

void
mod_gl1_utils_bitmap_hash_quit (lw6sys_context_t * sys_context, lw6sys_hash_t * bitmap_hash)
{
  lw6sys_list_t *keys;

  keys = lw6sys_hash_keys (sys_context, bitmap_hash);
  if (keys)
    {
      if (lw6sys_list_is_empty (sys_context, keys))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bitmap hash is empty on quit, this is fine"));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bitmap hash still has keys on quit, memory leak suspected"));
	}
      lw6sys_list_free (sys_context, keys);
    }
  lw6sys_hash_free (sys_context, bitmap_hash);
}

static char *
_get_key (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  char *key = NULL;

  if (bitmap->desc)
    {
      key = lw6sys_new_sprintf (sys_context, _FILE_MASK, bitmap->desc, bitmap->id);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bitmap has no desc"));
    }

  return key;
}

int
mod_gl1_utils_bitmap_hash_register (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  int ret = 0;
  char *key = NULL;

  if (utils_context->bitmap_hash)
    {
      key = _get_key (sys_context, utils_context, bitmap);
      if (key)
	{
	  if (!lw6sys_hash_has_key (sys_context, utils_context->bitmap_hash, key))
	    {
	      ret = 1;
	      lw6sys_hash_set (sys_context, utils_context->bitmap_hash, key, bitmap);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bitmap hash already has key \"%s\""), key);
	    }
	  LW6SYS_FREE (sys_context, key);
	}
    }

  return ret;
}

int
mod_gl1_utils_bitmap_hash_unregister (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  int ret = 0;
  char *key = NULL;

  if (utils_context->bitmap_hash)
    {
      key = _get_key (sys_context, utils_context, bitmap);
      if (key)
	{
	  if (lw6sys_hash_has_key (sys_context, utils_context->bitmap_hash, key))
	    {
	      ret = 1;
	      lw6sys_hash_unset (sys_context, utils_context->bitmap_hash, key);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bitmap hash has no key \"%s\""), key);
	    }
	  LW6SYS_FREE (sys_context, key);
	}
    }

  return ret;
}

static void
_refresh_callback (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  mod_gl1_utils_context_t *utils_context = (mod_gl1_utils_context_t *) func_data;
  mod_gl1_utils_bitmap_t *bitmap = (mod_gl1_utils_bitmap_t *) value;

  mod_gl1_utils_bitmap_refresh_force (sys_context, utils_context, bitmap);
}

int
mod_gl1_utils_bitmap_hash_refresh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  int ret = 1;

  if (utils_context->bitmap_hash)
    {
      lw6sys_hash_map (sys_context, utils_context->bitmap_hash, _refresh_callback, (void *) utils_context);
    }

  return ret;
}

static void
_dump2disk_callback (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  mod_gl1_utils_context_t *utils_context = (mod_gl1_utils_context_t *) func_data;
  mod_gl1_utils_bitmap_t *bitmap = (mod_gl1_utils_bitmap_t *) value;
  char *full_path = NULL;

  if (bitmap->surface)
    {
      full_path = lw6sys_path_concat (sys_context, utils_context->path.bitmap_frame_dir, key);
      if (full_path)
	{
	  if (SDL_SaveBMP (bitmap->surface, full_path) == 0)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("wrote bitmap \"%s\""), full_path);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to write bitmap \"%s\" to disk"), full_path);
	    }
	  LW6SYS_FREE (sys_context, full_path);
	}
    }
}

int
mod_gl1_utils_bitmap_hash_dump2disk (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int force)
{
  int ret = 0;
  int frames = 0;

  frames = utils_context->counter.nb_frames;
  if (force || !(frames % _DUMP_EVERY_N_FRAMES))
    {
      if (utils_context->bitmap_hash)
	{
	  mod_gl1_utils_path_update (sys_context, utils_context);
	  if (!lw6sys_dir_exists (sys_context, utils_context->path.bitmap_dir))
	    {
	      lw6sys_create_dir (sys_context, utils_context->path.bitmap_dir);
	    }
	  if (lw6sys_dir_exists (sys_context, utils_context->path.bitmap_dir))
	    {
	      if (!lw6sys_dir_exists (sys_context, utils_context->path.bitmap_frame_dir))
		{
		  lw6sys_create_dir (sys_context, utils_context->path.bitmap_frame_dir);
		}
	      if (lw6sys_dir_exists (sys_context, utils_context->path.bitmap_frame_dir))
		{
		  lw6sys_hash_map (sys_context, utils_context->bitmap_hash, _dump2disk_callback, (void *) utils_context);
		  ret = 1;
		}
	    }
	  if (!ret)
	    {
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to dump bitmaps to disk in \"%s\""), utils_context->path.bitmap_frame_dir);
	      }
	    }
	}
    }
  else
    {
      ret = 1;			// nothing to do
    }

  return ret;
}
