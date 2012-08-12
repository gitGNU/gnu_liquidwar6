/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "sys.h"

lw6sys_cache_t *
lw6sys_cache_new (lw6sys_free_func_t free_func, int size, int delay_msec)
{
  lw6sys_cache_t *ret = NULL;

  ret = (lw6sys_cache_t *) LW6SYS_CALLOC (sizeof (lw6sys_cache_t));
  if (ret)
    {
      ret->delay_msec = delay_msec;
      ret->real_free_func = free_func;
      ret->data = lw6sys_hash_new (lw6sys_cache_free_callback, size);
      if (!(ret->data))
	{
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
lw6sys_cache_free (lw6sys_cache_t * cache)
{
  if (cache)
    {
      lw6sys_hash_free (cache->data);
      LW6SYS_FREE (cache);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("trying to free NULL cache"));
    }
}

void
lw6sys_cache_free_callback (void *data)
{
  lw6sys_cache_item_t *item = (lw6sys_cache_item_t *) data;

  if (item)
    {
      if (item->real_free_func && item->value)
	{
	  item->real_free_func (item->value);
	}
      LW6SYS_FREE (item);
    }
}

int
lw6sys_cache_has_key (lw6sys_cache_t * cache, const char *key)
{
  int ret = 0;
  lw6sys_cache_item_t *item = NULL;

  item = lw6sys_hash_get (cache->data, key);
  /*
   * Testing if item is NOT NULL is equivalent to testing wether
   * key exists as in a cache, the hash always stores real data,
   * even for a NULL pointer it will store an item with a value
   * member set to NULL, but the hash data is NOT NULL indeed.
   */
  if (item)
    {
      if (item->expiration_timestamp >= lw6sys_get_timestamp ())
	{
	  ret = 1;
	}
      else
	{
	  /*
	   * We delete the key on the fly, it's obsolete, completely
	   * removing it will better performance in the long run.
	   */
	  lw6sys_cache_unset (cache, key);
	}
    }

  return ret;
}

void *
lw6sys_cache_get (lw6sys_cache_t * cache, const char *key)
{
  void *ret = NULL;
  lw6sys_cache_item_t *item = NULL;

  item = lw6sys_hash_get (cache->data, key);
  if (item)
    {
      if (item->expiration_timestamp >= lw6sys_get_timestamp ())
	{
	  ret = item->value;
	}
      else
	{
	  /*
	   * We delete the key on the fly, it's obsolete, completely
	   * removing it will better performance in the long run.
	   */
	  lw6sys_cache_unset (cache, key);
	}
    }

  return ret;
}

void
lw6sys_cache_set (lw6sys_cache_t * cache, const char *key, void *value)
{
  lw6sys_cache_item_t *item = NULL;

  item = (lw6sys_cache_item_t *) LW6SYS_CALLOC (sizeof (lw6sys_cache_item_t));

  if (item)
    {
      item->expiration_timestamp =
	lw6sys_get_timestamp () + cache->delay_msec;
      item->real_free_func = cache->real_free_func;
      item->value = value;
      lw6sys_hash_set (cache->data, key, item);
    }
}

void
lw6sys_cache_unset (lw6sys_cache_t * cache, const char *key)
{
  // todo...
}

lw6sys_list_t *
lw6sys_cache_keys (lw6sys_cache_t * cache)
{
  lw6sys_list_t *ret = NULL;

  // todo...

  return ret;
}

lw6sys_cache_t *
lw6sys_cache_dup (lw6sys_cache_t * cache, lw6sys_dup_func_t dup_func)
{
  lw6sys_cache_t *ret = NULL;

  // todo...

  return ret;
}
