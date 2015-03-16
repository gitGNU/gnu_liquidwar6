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

typedef struct assoc_dup_callback_data_s
{
  lw6sys_assoc_t *assoc;
  lw6sys_dup_func_t dup_func;
} assoc_dup_callback_data_t;

/**
 * lw6sys_assoc_new:
 *
 * @sys_context: global system context
 * @free_func: optional callback used to free memory when stored
 *   date is a pointer. Can be NULL when one stores non dynamically
 *   allocated data, such as an integer or a static array.
 *
 * Creates an empty assoc. There's a difference between NULL and an
 * empty assoc. The empty assoc would (in Scheme) be '() whereas
 * NULL corresponds to undefined "is not a assoc and will generate
 * errors if you ever call assoc functions on it". Such created
 * assoc are not performant hash tables but slowish "strcmp me for
 * each key" associative arrays, the key being a "char *" string
 * and the value a "void *" pointer.
 *
 * Return value: a pointer to the newly allocated associative
 *   array. Must be freed with @lw6sys_assoc_free.
 */
lw6sys_assoc_t *
lw6sys_assoc_new (lw6sys_context_t * sys_context, lw6sys_free_func_t free_func)
{
  lw6sys_assoc_t *ret = NULL;

  ret = LW6SYS_MALLOC (sys_context, sizeof (lw6sys_assoc_t));
  if (ret)
    {
      memset (ret, 0, sizeof (lw6sys_assoc_t));
      ret->free_func = free_func;
    }

  return ret;
}

/**
 * lw6sys_assoc_free:
 *
 * @sys_context: global system context
 * @assoc: the assoc to be freed.
 *
 * The function will cascade  delete all elements, using (if not NULL...)
 * the callback passed when first creating the assoc.
 *
 * Return value: void
 */
void
lw6sys_assoc_free (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc)
{
  if (assoc)
    {
      /*
       * Keep a copy of next_item for we are about to
       * free the pointer to it.
       */
      lw6sys_assoc_t *next_item = assoc->next_item;

      if (assoc->key)
	{
	  LW6SYS_FREE (sys_context, assoc->key);
	}

      /*
       * It's legal to have free_func or value set to NULL,
       * we simply need to avoid the core dump, but one can
       * legitimately not need any peculiar free function,
       * or desire to store a NULL content in a valid assoc.
       */
      if (assoc->free_func && assoc->value)
	{
	  assoc->free_func (sys_context, assoc->value);
	}

      LW6SYS_FREE (sys_context, assoc);

      if (next_item)
	{
	  /*
	   * This should be the last call of the function.
	   * Hopefully the compiler will find this out and
	   * optimize and *not* generate hudge stacks with
	   * return addresses which are of no use. At least
	   * the compiler *could* do it 8-) Recursion recursion...
	   */
	  lw6sys_assoc_free (sys_context, (lw6sys_assoc_t *) next_item);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL assoc"));
    }
}

/**
 * lw6sys_assoc_has_key:
 *
 * @sys_context: global system context
 * @assoc: the assoc to test
 * @key: the key to search
 *
 * Not a very fast function, since on a "big" assoc, strcmp will be
 * called internally until the key is found.
 *
 * Return value: non-zero if there's an entry with the
 *   corresponding key.
 */
int
lw6sys_assoc_has_key (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, const char *key)
{
  int exists = 0;

  if (assoc)
    {
      while (assoc && !exists)
	{
	  if (assoc->key && strcmp (assoc->key, key) == 0)
	    {
	      exists = 1;
	    }
	  assoc = assoc->next_item;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling has_key on NULL assoc"));
    }

  return exists;
}

/**
 * lw6sys_assoc_get:
 *
 * @sys_context: global system context
 * @assoc: the assoc to query
 * @key: the key of which we want the value
 *
 * Return value: a void pointer to the data contained
 *   in the assoc. Note that the pointer on the actual data
 *   is returned, that is, if it's static data, you must not
 *   try to free it... As long as memory management is concerned,
 *   destroying the assoc will actually free the data if needed.
 */
void *
lw6sys_assoc_get (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, const char *key)
{
  void *value = NULL;

  if (assoc)
    {
      while (assoc)
	{
	  if (assoc->key && strcmp (assoc->key, key) == 0)
	    {
	      value = assoc->value;
	    }
	  assoc = assoc->next_item;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling get on NULL assoc"));
    }

  return value;
}

/**
 * lw6sys_assoc_set:
 *
 * @sys_context: global system context
 * @assoc: the assoc to modify
 * @key: the key we want to updated
 * @value: the new value
 *
 * Sets a value in an associative array. The key pointer need
 * not be persistent, it can be freed after affectation. In
 * fact a new string will be created internally. This is not
 * true for the value, it's hard to find way to copy "any object".
 * So if you want an associative array of strings, key can
 * disappear after calling this function, but not value. The
 * function passed as free_func when creating the assoc will
 * be used to free stuff whenever needed (unset or free).
 *
 * Return value: void
 */
void
lw6sys_assoc_set (lw6sys_context_t * sys_context, lw6sys_assoc_t ** assoc, const char *key, void *value)
{
  int exists = 0;

  if (assoc && *assoc)
    {
      lw6sys_assoc_t *search = *assoc;
      while (search)
	{
	  if (search->key && strcmp (search->key, key) == 0)
	    {
	      exists = 1;
	      if (search->free_func && search->value)
		{
		  search->free_func (sys_context, search->value);
		}
	      search->value = value;
	    }
	  search = search->next_item;
	}
      if (!exists)
	{
	  lw6sys_assoc_t *new_assoc = NULL;

	  new_assoc = LW6SYS_MALLOC (sys_context, sizeof (lw6sys_assoc_t));
	  if (new_assoc)
	    {
	      new_assoc->next_item = (*assoc);
	      new_assoc->key = lw6sys_str_copy (sys_context, key);
	      new_assoc->value = value;
	      new_assoc->free_func = (*assoc)->free_func;
	      (*assoc) = new_assoc;
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling set on NULL assoc"));
    }
}

/**
 * lw6sys_assoc_unset:
 *
 * @sys_context: global system context
 * @assoc: the assoc concerned
 * @key: the key to unset
 *
 * Clears an entry in an associative array. The callback passed when
 * creating the assoc will be called if needed, to free the data
 * automatically.
 *
 * Return value: void
 */
void
lw6sys_assoc_unset (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, const char *key)
{
  if (assoc)
    {
      while (assoc)
	{
	  if (assoc->key && strcmp (assoc->key, key) == 0)
	    {
	      if (assoc->key)
		{
		  LW6SYS_FREE (sys_context, assoc->key);
		}
	      if (assoc->free_func && assoc->value)
		{
		  assoc->free_func (sys_context, assoc->value);
		}
	      if (assoc->next_item)
		{
		  lw6sys_assoc_t *to_free;

		  to_free = assoc->next_item;
		  memcpy (assoc, assoc->next_item, sizeof (lw6sys_assoc_t));
		  LW6SYS_FREE (sys_context, to_free);
		}
	      else
		{
		  memset (assoc, 0, sizeof (lw6sys_assoc_t));
		}
	    }
	  assoc = assoc->next_item;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling unset on NULL assoc"));
    }
}

/**
 * lw6sys_assoc_keys:
 *
 * @sys_context: global system context
 * @assoc: the assoc to work on
 *
 * Returns a list containing all the keys of the assoc. The
 * list must be free with lw6sys_list_free by the caller.
 * This list copies all the keys of the assoc, so it is
 * safe to use it once the assoc is deleted. However the
 * keys will of course be of little interest in this case.
 * But the program won't segfault.
 *
 * Return value: the list of keys.
 */
lw6sys_list_t *
lw6sys_assoc_keys (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc)
{
  lw6sys_list_t *keys = NULL;

  if (assoc)
    {
      keys = lw6sys_list_new (sys_context, lw6sys_free_callback);
      if (keys)
	{
	  while (assoc)
	    {
	      if (assoc->key && keys)
		{
		  lw6sys_lifo_push (sys_context, &keys, lw6sys_str_copy (sys_context, assoc->key));
		}
	      assoc = assoc->next_item;
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling keys on NULL assoc"));
    }

  return keys;
}

/**
 * lw6sys_assoc_map:
 *
 * @sys_context: global system context
 * @assoc: the assoc to work on
 * @func: a callback to call on each entry
 * @func_data: a pointer on some data which will be passed to the callback
 *
 * Executes a function on all assoc items.
 * The func_data parameter allows you to pass extra values to
 * the function, such as a file handler or any variable which
 * can not be inferred from list item values, and you of course
 * do not want to make global...
 *
 * Return value: void
 */
void
lw6sys_assoc_map (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, lw6sys_assoc_callback_func_t func, void *func_data)
{
  if (assoc)
    {
      while (assoc)
	{
	  /*
	   * To check if the assoc item is defined we call
	   * assoc->next_item which is false only at the end of
	   * the list. Other indicators such as key are "not as
	   * good".
	   */
	  if (assoc->next_item)
	    {
	      if (assoc->key)
		{
		  func (sys_context, func_data, assoc->key, assoc->value);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("assoc has a NULL key"));
		}
	    }
	  assoc = assoc->next_item;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling map on NULL assoc"));
    }
}

/**
 * lw6sys_assoc_sort_and_map:
 *
 * @sys_context: global system context
 * @assoc: the assoc to work on
 * @func: a callback to call on each entry, may be NULL
 * @func_data: a pointer on some data which will be passed to the callback
 *
 * Executes a function on all assoc items, like @lw6sys_assoc_sort_and_map
 * but befor doing so, sorts all entries in alphabetical order.
 *
 * Return value: void
 */
void
lw6sys_assoc_sort_and_map (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, lw6sys_assoc_callback_func_t func, void *func_data)
{
  lw6sys_list_t *keys;
  lw6sys_list_t *current_key;
  char *str_key;

  if (assoc)
    {
      keys = lw6sys_assoc_keys (sys_context, assoc);
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
			  func (sys_context, func_data, str_key, lw6sys_assoc_get (sys_context, assoc, str_key));
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
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling sort and map on NULL assoc"));
    }
}

static void
assoc_dup_callback (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  assoc_dup_callback_data_t *assoc_dup_callback_data = (assoc_dup_callback_data_t *) func_data;
  void *new_value;

  if (assoc_dup_callback_data->dup_func)
    {
      new_value = assoc_dup_callback_data->dup_func (sys_context, value);
    }
  else
    {
      new_value = value;
    }

  lw6sys_assoc_set (sys_context, &(assoc_dup_callback_data->assoc), key, new_value);
}

/**
 * lw6sys_assoc_dup
 *
 * @sys_context: global system context
 * @assoc: the assoc to duplicate, can be NULL
 * @dup_func: the function which will be called to duplicate data
 *
 * Duplicates an assoc. All keys will be copied so that if the first
 * assoc is deleted, the duplicated one is fine. Additionnaly, dup_func
 * will be called with all data fields. If dup_func is NULL, then data
 * values will simply be copied. This is likely to be usefull when
 * data is not dynamically allocated.
 *
 * Returned value: a newly allocated assoc.
 */
lw6sys_assoc_t *
lw6sys_assoc_dup (lw6sys_context_t * sys_context, lw6sys_assoc_t * assoc, lw6sys_dup_func_t dup_func)
{
  assoc_dup_callback_data_t assoc_dup_callback_data;
  lw6sys_assoc_t *ret = NULL;

  if (assoc)
    {
      assoc_dup_callback_data.assoc = lw6sys_assoc_new (sys_context, assoc->free_func);
      assoc_dup_callback_data.dup_func = dup_func;

      lw6sys_assoc_map (sys_context, assoc, assoc_dup_callback, &assoc_dup_callback_data);

      ret = assoc_dup_callback_data.assoc;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling dup on NULL assoc"));
    }

  return ret;
}
