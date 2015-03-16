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
#endif // HAVE_CONFIG_H

#include "sys.h"

/**
 * lw6sys_cache_new:
 *
 * @sys_context: global system context
 * @free_func: optional callback used to free memory when stored
 *   date is a pointer. Can be NULL when one stores non dynamically
 *   allocated data, such as an integer or a static array.
 * @size: the estimated size of the cache table. This is required because,
 *   internally, the object uses a hash. Note that this is
 *   an estimation only. You could theorically fit 1000000 objects
 *   in a 3-sized cache. Problem -> this is inefficient, you'd better
 *   use an assoc or a bigger cache. If you store 3 elements in a
 *   1000000-sized cache, you'll waste memory. It might be wise to
 *   use a prime number as the estimated size. 421 is prime ;)
 *
 * Creates an empty cache. There's a difference between NULL and an
 * empty cache.
 *
 * Return value: a pointer to the newly allocated cache table.
 * Must be freed with @lw6sys_cache_free.
 */
lw6sys_cache_t *
lw6sys_cache_new (lw6sys_context_t * sys_context, lw6sys_free_func_t free_func, int size, int delay_msec)
{
  lw6sys_cache_t *ret = NULL;

  ret = (lw6sys_cache_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_cache_t));
  if (ret)
    {
      ret->delay_msec = delay_msec;
      ret->real_free_func = free_func;
      ret->data = lw6sys_hash_new (sys_context, lw6sys_cache_free_callback, size);
      if (!(ret->data))
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6sys_cache_free:
 *
 * @sys_context: global system context
 * @cache: the cache to be freed.
 *
 * The function will cascade  delete all elements, using (if not NULL...)
 * the callback passed when first creating the cache.
 *
 * Return value: void
 */
void
lw6sys_cache_free (lw6sys_context_t * sys_context, lw6sys_cache_t * cache)
{
  if (cache)
    {
      lw6sys_hash_free (sys_context, cache->data);
      LW6SYS_FREE (sys_context, cache);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL cache"));
    }
}

/**
 * lw6sys_cache_free_callback:
 *
 * @sys_context: global system context
 * @data: data to free, this is normally an cache item
 *
 * This is a wrapper, used as the actual free callback
 * for the internal hash. What it does is that it just
 * runs the real free callback (the one given by the
 * user) on the member value. This indirection is required
 * since we use the intermediate item object to store
 * the timestamp along with the key and data.
 *
 * Return value: none.
 */
void
lw6sys_cache_free_callback (lw6sys_context_t * sys_context, void *data)
{
  lw6sys_cache_item_t *item = (lw6sys_cache_item_t *) data;

  if (item)
    {
      if (item->real_free_func && item->value)
	{
	  item->real_free_func (sys_context, item->value);
	}
      LW6SYS_FREE (sys_context, item);
    }
}

/**
 * lw6sys_cache_has_key:
 *
 * @sys_context: global system context
 * @cache: the cache to test
 * @key: the key to search
 *
 * Tells wether the key is present or not. If key is here but
 * too old (expired) then will return 0 and key will be deleted
 * on the fly.
 *
 * Return value: non-zero if there's an entry with the
 *   corresponding key.
 */
int
lw6sys_cache_has_key (lw6sys_context_t * sys_context, lw6sys_cache_t * cache, const char *key)
{
  int ret = 0;
  lw6sys_cache_item_t *item = NULL;

  item = lw6sys_hash_get (sys_context, cache->data, key);
  /*
   * Testing if item is NOT NULL is equivalent to testing wether
   * key exists as in a cache, the hash always stores real data,
   * even for a NULL pointer it will store an item with a value
   * member set to NULL, but the hash data is NOT NULL indeed.
   */
  if (item)
    {
      if (item->expiration_timestamp >= lw6sys_get_timestamp (sys_context))
	{
	  ret = 1;
	}
      else
	{
	  /*
	   * We delete the key on the fly, it's obsolete, completely
	   * removing it will better performance in the long run.
	   */
	  lw6sys_cache_unset (sys_context, cache, key);
	}
    }

  return ret;
}

/**
 * lw6sys_cache_get:
 *
 * @sys_context: global system context
 * @cache: the cache to query
 * @key: the key of which we want the value
 *
 * Gets the value corresponding to a given key. Note that the
 * value might be NULL, even if the key exists. If the key
 * has expired, NULL will be returned, and the entry deleted
 * on the fly.
 *
 * Return value: a void pointer to the data contained
 *   in the cache. Note that the pointer on the actual data
 *   is returned, that is, if it's static data, you must not
 *   try to free it... As long as memory management is concerned,
 *   destroying the cache will actually free the data if needed.
 */
void *
lw6sys_cache_get (lw6sys_context_t * sys_context, lw6sys_cache_t * cache, const char *key)
{
  void *ret = NULL;
  lw6sys_cache_item_t *item = NULL;

  item = lw6sys_hash_get (sys_context, cache->data, key);
  if (item)
    {
      if (item->expiration_timestamp >= lw6sys_get_timestamp (sys_context))
	{
	  ret = item->value;
	}
      else
	{
	  /*
	   * We delete the key on the fly, it's obsolete, completely
	   * removing it will better performance in the long run.
	   */
	  lw6sys_cache_unset (sys_context, cache, key);
	}
    }

  return ret;
}

/**
 * lw6sys_cache_set:
 *
 * @cache: the cache to modify
 * @key: the key we want to updated
 * @value: the new value
 *
 * Sets a value in a cache table. The key pointer need
 * not be persistent, it can be freed after affectation. In
 * fact a new string will be created internally. This is not
 * true for the value, it's hard to find way to copy "any object".
 * So if you want a cache of strings, key can
 * disappear after calling this function, but not value. The
 * function passed as free_func when creating the cache will
 * be used to free stuff whenever needed (unset or free).
 *
 * Return value: void
 */
void
lw6sys_cache_set (lw6sys_context_t * sys_context, lw6sys_cache_t * cache, const char *key, void *value)
{
  lw6sys_cache_item_t *item = NULL;

  item = (lw6sys_cache_item_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_cache_item_t));

  if (item)
    {
      item->expiration_timestamp = lw6sys_get_timestamp (sys_context) + cache->delay_msec;
      item->real_free_func = cache->real_free_func;
      item->value = value;
      lw6sys_hash_set (sys_context, cache->data, key, item);
    }
}

/**
 * lw6sys_cache_unset:
 *
 * @sys_context: global system context
 * @cache: the cache concerned
 * @key: the key to unset
 *
 * Clears an entry in a cache table. The callback passed when
 * creating the cache will be called if needed, to free the data
 * automatically.
 *
 * Return value: void
 */
void
lw6sys_cache_unset (lw6sys_context_t * sys_context, lw6sys_cache_t * cache, const char *key)
{
  lw6sys_hash_unset (sys_context, cache->data, key);
}
