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

/*
 * Hash smaller than 2 in size do not
 * make any sense, assoc is more efficient for
 * that matter...
 */
#define _HASH_SIZE_MIN 2

typedef struct hash_dup_callback_data_s
{
  lw6sys_hash_t *hash;
  lw6sys_dup_func_t dup_func;
} hash_dup_callback_data_t;

/**
 * lw6sys_hash_new:
 *
 * @sys_context: global system context
 * @free_func: optional callback used to free memory when stored
 *   date is a pointer. Can be NULL when one stores non dynamically
 *   allocated data, such as an integer or a static array.
 * @size: the estimated size of the hash table. Note that this is
 *   an estimation only. You could theorically fit 1000000 objects
 *   in a 3-sized hash. Problem -> this is inefficient, you'd better
 *   use an assoc or a bigger hash. If you store 3 elements in a
 *   1000000-sized hash, you'll waste memory. It might be wise to
 *   use a prime number as the estimated size. 421 is prime ;)
 *
 * Creates an empty hash. There's a difference between NULL and an
 * empty hash.
 *
 * Return value: a pointer to the newly allocated hash table.
 * Must be freed with @lw6sys_hash_free.
 */
lw6sys_hash_t *
lw6sys_hash_new (lw6sys_context_t * sys_context, lw6sys_free_func_t free_func, int size)
{
  lw6sys_hash_t *ret = NULL;
  int i;
  int null_entry = 0;

  if (size >= _HASH_SIZE_MIN)
    {
      ret = LW6SYS_MALLOC (sys_context, sizeof (lw6sys_hash_t));
      if (ret)
	{
	  ret->free_func = free_func;
	  ret->size = size;
	  ret->entries = (lw6sys_assoc_t **) LW6SYS_MALLOC (sys_context, size * sizeof (lw6sys_assoc_t *));
	  if (ret->entries)
	    {
	      for (i = 0; i < ret->size; ++i)
		{
		  ret->entries[i] = lw6sys_assoc_new (sys_context, free_func);
		  if (!ret->entries[i])
		    {
		      null_entry = 1;
		    }
		}
	      if (null_entry)
		{
		  for (i = 0; i < ret->size; ++i)
		    {
		      if (ret->entries[i])
			{
			  lw6sys_assoc_free (sys_context, ret->entries[i]);
			}
		    }
		  LW6SYS_FREE (sys_context, ret);
		  ret = NULL;
		}
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, ret);
	      ret = NULL;
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create hash with size=%d"), size);
    }

  return ret;
}

/**
 * lw6sys_hash_free:
 *
 * @sys_context: global system context
 * @hash: the hash to be freed.
 *
 * The function will cascade  delete all elements, using (if not NULL...)
 * the callback passed when first creating the hash.
 *
 * Return value: void
 */
void
lw6sys_hash_free (lw6sys_context_t * sys_context, lw6sys_hash_t * hash)
{
  int i;

  if (hash)
    {
      if (hash->entries)
	{
	  for (i = 0; i < hash->size; ++i)
	    {
	      lw6sys_assoc_free (sys_context, hash->entries[i]);
	    }
	  LW6SYS_FREE (sys_context, hash->entries);
	}
      LW6SYS_FREE (sys_context, hash);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL hash"));
    }
}

/**
 * lw6sys_hash_has_key:
 *
 * @sys_context: global system context
 * @hash: the hash to test
 * @key: the key to search
 *
 * Tells wether the key is present or not.
 *
 * Return value: non-zero if there's an entry with the
 *   corresponding key.
 */
int
lw6sys_hash_has_key (lw6sys_context_t * sys_context, lw6sys_hash_t * hash, const char *key)
{
  int exists = 0;
  int index = 0;

  if (hash)
    {
      index = lw6sys_checksum_str (sys_context, key) % hash->size;
      exists = lw6sys_assoc_has_key (sys_context, hash->entries[index], key);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling has_key on NULL hash"));
    }

  return exists;
}

/**
 * lw6sys_hash_get:
 *
 * @sys_context: global system context
 * @hash: the hash to query
 * @key: the key of which we want the value
 *
 * Gets the value corresponding to a given key. Not that the
 * value can be NULL, even if the key exitsts.
 *
 * Return value: a void pointer to the data contained
 *   in the hash. Note that the pointer on the actual data
 *   is returned, that is, if it's static data, you must not
 *   try to free it... As long as memory management is concerned,
 *   destroying the hash will actually free the data if needed.
 */
void *
lw6sys_hash_get (lw6sys_context_t * sys_context, lw6sys_hash_t * hash, const char *key)
{
  void *value = NULL;
  int index = 0;

  if (hash)
    {
      index = lw6sys_checksum_str (sys_context, key) % hash->size;
      value = lw6sys_assoc_get (sys_context, hash->entries[index], key);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling get on NULL hash"));
    }

  return value;
}

/**
 * lw6sys_hash_set:
 *
 * @sys_context: global system context
 * @hash: the hash to modify
 * @key: the key we want to updated
 * @value: the new value
 *
 * Sets a value in a hash table. The key pointer need
 * not be persistent, it can be freed after affectation. In
 * fact a new string will be created internally. This is not
 * true for the value, it's hard to find way to copy "any object".
 * So if you want a hash table of strings, key can
 * disappear after calling this function, but not value. The
 * function passed as free_func when creating the hash will
 * be used to free stuff whenever needed (unset or free).
 *
 * Return value: void
 */
void
lw6sys_hash_set (lw6sys_context_t * sys_context, lw6sys_hash_t * hash, const char *key, void *value)
{
  int index = 0;

  if (hash)
    {
      index = lw6sys_checksum_str (sys_context, key) % hash->size;
      lw6sys_assoc_set (sys_context, &(hash->entries[index]), key, value);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling set on NULL hash"));
    }
}

/**
 * lw6sys_hash_unset:
 *
 * @sys_context: global system context
 * @hash: the hash concerned
 * @key: the key to unset
 *
 * Clears an entry in a hash table. The callback passed when
 * creating the hash will be called if needed, to free the data
 * automatically.
 *
 * Return value: void
 */
void
lw6sys_hash_unset (lw6sys_context_t * sys_context, lw6sys_hash_t * hash, const char *key)
{
  int index = 0;

  if (hash)
    {
      index = lw6sys_checksum_str (sys_context, key) % hash->size;
      lw6sys_assoc_unset (sys_context, hash->entries[index], key);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling unset on NULL hash"));
    }
}

/**
 * lw6sys_hash_keys:
 *
 * @sys_context: global system context
 * @hash: the hash to work on
 *
 * Returns a list containing all the keys of the hash. The
 * list must be free with lw6sys_list_free by the caller.
 * This list copies all the keys of the hash, so it is
 * safe to use it once the hash is deleted. However the
 * keys will sometimes be of little interest in this case.
 * But the program won't segfault.
 *
 * Return value: the list of keys.
 */
lw6sys_list_t *
lw6sys_hash_keys (lw6sys_context_t * sys_context, lw6sys_hash_t * hash)
{
  lw6sys_list_t *keys = NULL;
  lw6sys_assoc_t *assoc = NULL;
  int i;

  if (hash)
    {
      keys = lw6sys_list_new (sys_context, lw6sys_free_callback);
      if (keys)
	{
	  for (i = 0; i < hash->size; ++i)
	    {
	      assoc = hash->entries[i];
	      while (assoc && keys)
		{
		  if (assoc->key)
		    {
		      lw6sys_lifo_push (sys_context, &keys, lw6sys_str_copy (sys_context, assoc->key));
		    }
		  assoc = assoc->next_item;
		}
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling keys on NULL hash"));
    }

  return keys;
}

/**
 * lw6sys_hash_map:
 *
 * @sys_context: global system context
 * @hash: the hash to work on
 * @func: a callback to call on each entry
 * @func_data: a pointer on some data which will be passed to the callback
 *
 * Executes a function on all hash items.
 * The func_data parameter allows you to pass extra values to
 * the function, such as a file handler or any variable which
 * can not be inferred from list item values, and you of course
 * do not want to make global...
 *
 * Return value: void
 */
void
lw6sys_hash_map (lw6sys_context_t * sys_context, lw6sys_hash_t * hash, lw6sys_assoc_callback_func_t func, void *func_data)
{
  int i;

  if (hash)
    {
      for (i = 0; i < hash->size; ++i)
	{
	  lw6sys_assoc_map (sys_context, hash->entries[i], func, func_data);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling map on NULL hash"));
    }
}

/**
 * lw6sys_hash_sort_and_map:
 *
 * @sys_context: global system context
 * @hash: the hash to work on
 * @func: a callback to call on each entry, may be NULL
 * @func_data: a pointer on some data which will be passed to the callback
 *
 * Executes a function on all hash items, like @lw6sys_hash_sort_and_map
 * but befor doing so, sorts all entries in alphabetical order.
 *
 * Return value: void
 */
void
lw6sys_hash_sort_and_map (lw6sys_context_t * sys_context, lw6sys_hash_t * hash, lw6sys_assoc_callback_func_t func, void *func_data)
{
  lw6sys_list_t *keys = NULL;
  lw6sys_list_t *current_key;
  char *str_key;

  if (hash)
    {
      keys = lw6sys_hash_keys (sys_context, hash);
      if (keys)
	{
	  lw6sys_sort (sys_context, &keys, lw6sys_sort_str_callback, NULL);
	  if (keys)
	    {
	      current_key = keys;
	      while (current_key)
		{
		  str_key = (char *) current_key->data;
		  if (str_key)
		    {
		      if (func)
			{
			  func (sys_context, func_data, str_key, lw6sys_hash_get (sys_context, hash, str_key));
			}
		    }
		  current_key = lw6sys_list_next (sys_context, current_key);
		}
	    }
	}
      if (keys)
	{
	  lw6sys_list_free (sys_context, keys);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling sort and map on NULL hash"));
    }
}

/**
 * lw6sys_hash_dup
 *
 * @sys_context: global system context
 * @hash: the hash to duplicate, can be NULL
 * @dup_func: the function which will be called to duplicate data
 *
 * Duplicates an hash. All keys will be copied so that if the first
 * hash is deleted, the duplicated one is fine. Additionnaly, dup_func
 * will be called with all data fields. If dup_func is NULL, then data
 * values will simply be copied. This is likely to be usefull when
 * data is not dynamically allocated.
 *
 * Returned value: a newly allocated hash.
 */
lw6sys_hash_t *
lw6sys_hash_dup (lw6sys_context_t * sys_context, lw6sys_hash_t * hash, lw6sys_dup_func_t dup_func)
{
  lw6sys_hash_t *ret = NULL;
  int i;
  int null_entry = 0;

  if (hash)
    {
      ret = (lw6sys_hash_t *) LW6SYS_MALLOC (sys_context, sizeof (lw6sys_hash_t));
      if (ret)
	{
	  ret->free_func = hash->free_func;
	  ret->size = hash->size;
	  ret->entries = (lw6sys_assoc_t **) LW6SYS_MALLOC (sys_context, hash->size * sizeof (lw6sys_assoc_t *));
	  if (ret->entries)
	    {
	      for (i = 0; i < hash->size; ++i)
		{
		  ret->entries[i] = lw6sys_assoc_dup (sys_context, hash->entries[i], dup_func);
		  if (!ret->entries[i])
		    {
		      null_entry = 1;
		    }
		}
	      if (null_entry)
		{
		  for (i = 0; i < ret->size; ++i)
		    {
		      if (ret->entries[i])
			{
			  lw6sys_assoc_free (sys_context, ret->entries[i]);
			}
		    }
		  LW6SYS_FREE (sys_context, ret);
		  ret = NULL;
		}
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, ret);
	      ret = NULL;
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling dup on NULL hash"));
    }

  return ret;
}
