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

#include <string.h>

#include "sys.h"

static int
_lock_ro (lw6sys_context_t * sys_context, const lw6sys_list_r_t * list_r)
{
  int ret = 0;

  if (list_r->list)
    {
      ret = LW6SYS_MUTEX_LOCK (sys_context, list_r->mutex);
    }

  return ret;
}

static int
_lock_rw (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r)
{
  int ret = 0;

  if (!(list_r->list))
    {
      /*
       * If we're likely to modify the list, one needs to provide
       * a non-null member, else list functions could fail.
       * Standard list will return NULL after popping some time,
       * we must cope with that.
       */
      list_r->list = lw6sys_list_new (sys_context, list_r->free_func);
    }

  if (list_r->list)
    {
      ret = _lock_ro (sys_context, list_r);
    }

  return ret;
}

static void
_unlock (lw6sys_context_t * sys_context, const lw6sys_list_r_t * list_r)
{
  LW6SYS_MUTEX_UNLOCK (sys_context, list_r->mutex);
}

/**
 * lw6sys_list_r_new
 *
 * @sys_context: global system context
 * @free_func: a callback which will be called on data when freeing the list
 *
 * Creates an empty reentrant list. This is different from a regular
 * list in the sense that here the object is a holder with both a mutex
 * and the list itself.
 *
 * Return value: a pointer to the created object, may be NULL.
 */
lw6sys_list_r_t *
lw6sys_list_r_new (lw6sys_context_t * sys_context, lw6sys_free_func_t free_func)
{
  lw6sys_list_r_t *ret = NULL;

  ret = LW6SYS_MALLOC (sys_context, sizeof (lw6sys_list_r_t));
  if (ret)
    {
      ret->mutex = LW6SYS_MUTEX_CREATE (sys_context);
      ret->list = lw6sys_list_new (sys_context, free_func);
      ret->free_func = free_func;
      if ((!(ret->mutex)) || (!(ret->list)))
	{
	  if (ret->mutex)
	    {
	      LW6SYS_MUTEX_DESTROY (sys_context, ret->mutex);
	      ret->mutex = NULL;
	    }
	  if (ret->list)
	    {
	      lw6sys_list_free (sys_context, ret->list);
	      ret->list = NULL;
	    }
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6sys_list_r_free
 *
 * @sys_context: global system context
 * @list_r: the list to delete.
 *
 * Delete a reentrant list, this will cascade delete all the
 * items in the list.
 *
 * Return value: none.
 */
void
lw6sys_list_r_free (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r)
{
  if (list_r)
    {
      if (list_r->mutex)
	{
	  LW6SYS_MUTEX_DESTROY (sys_context, list_r->mutex);
	  list_r->mutex = NULL;
	}
      if (list_r->list)
	{
	  lw6sys_list_free (sys_context, list_r->list);
	  list_r->list = NULL;
	}
      LW6SYS_FREE (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL list_r"));
    }
}

/**
 * lw6sys_list_r_is_empty
 *
 * @sys_context: global system context
 * @list_r: the list we want informations about
 *
 * Checks wether the reentrant list is empty or not. Note there's a slight
 * difference with basic lists, here it can't / should not be NULL, as
 * the list_r is really a list container.
 *
 * Return value: 1 if empty, 0 if there is at list one item.
 */
int
lw6sys_list_r_is_empty (lw6sys_context_t * sys_context, const lw6sys_list_r_t * list_r)
{
  int ret = 0;

  if (_lock_ro (sys_context, list_r))
    {
      if (list_r->list)
	{
	  /*
	   * Call is_empty only if member is not NULL,
	   * else consider there's no data, period.
	   */
	  ret = lw6sys_list_is_empty (sys_context, list_r->list);
	}
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }

  return ret;
}

/**
 * lw6sys_list_r_length
 *
 * @sys_context: global system context
 * @list_r: the list we want informations about
 *
 * Calculates the length of the reentrant list. This is a performance killer
 * for lists are inadapted to this. But it can still be usefull.
 *
 * Return value: the number of elements, 0 is none (empty list).
 */
int
lw6sys_list_r_length (lw6sys_context_t * sys_context, const lw6sys_list_r_t * list_r)
{
  int ret = 0;

  if (_lock_ro (sys_context, list_r))
    {
      if (list_r->list)
	{
	  /*
	   * Call list_lenght only if member is not NULL,
	   * else consider there's no data, period.
	   */
	  ret = lw6sys_list_length (sys_context, list_r->list);
	}
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }

  return ret;
}

/**
 * lw6sys_list_r_map
 *
 * @sys_context: global system context
 * @list_r: the list where elements will be taken
 * @func: the function which will be executed
 * @func_data: additionnal data to be passed to @func
 *
 * Executes a function on all reentrant list items.
 * This is a wrapper on @lw6sys_list_map.
 *
 * Return value: none.
 */
void
lw6sys_list_r_map (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, lw6sys_list_callback_func_t func, void *func_data)
{
  if (_lock_rw (sys_context, list_r))
    {
      lw6sys_list_map (sys_context, list_r->list, func, func_data);
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }
}

/**
 * lw6sys_list_r_filter
 *
 * @sys_context: global system context
 * @list_r: the list where elements will be taken
 * @func: the function which will be executed
 * @func_data: additionnal data to be passed to @func
 *
 * Executes a function on all reentrant list items.
 * Ths is a wrapper on @lw6sys_list_filter.
 *
 * Return value: none.
 */
void
lw6sys_list_r_filter (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, lw6sys_list_filter_func_t func, void *func_data)
{
  if (_lock_rw (sys_context, list_r))
    {
      lw6sys_list_filter (sys_context, &(list_r->list), func, func_data);
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }
}

/**
 * lw6sys_list_r_push_front
 *
 * @sys_context: global system context
 * @list_r: a pointer to the list (pointer on pointer, read/write value)
 * @data: the data to be pushed
 *
 * Wapper on lw6sys_list_push_front, reentrant version.
 *
 * Return value: none.
 */
void
lw6sys_list_r_push_front (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, void *data)
{
  if (_lock_rw (sys_context, list_r))
    {
      lw6sys_list_push_front (sys_context, &(list_r->list), data);
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }
}

/**
 * lw6sys_list_r_pop_front
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Wrapper on lw6sys_list_pop_front, reentrant version.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_list_r_pop_front (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r)
{
  void *ret = NULL;

  if (_lock_rw (sys_context, list_r))
    {
      ret = lw6sys_list_pop_front (sys_context, &(list_r->list));
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }
  return ret;
}

/**
 * lw6sys_list_r_push_back
 *
 * @sys_context: global system context
 * @list_r: a pointer to the list (pointer on pointer, read/write value)
 * @data: the data to be pushed
 *
 * Wapper on lw6sys_list_push_back, reentrant version.
 *
 * Return value: none.
 */
void
lw6sys_list_r_push_back (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, void *data)
{
  if (_lock_rw (sys_context, list_r))
    {
      lw6sys_list_push_back (sys_context, &(list_r->list), data);
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }
}

/**
 * lw6sys_list_r_pop_back
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Wrapper on lw6sys_list_pop_back, reentrant version.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_list_r_pop_back (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r)
{
  void *ret = NULL;

  if (_lock_rw (sys_context, list_r))
    {
      ret = lw6sys_list_pop_back (sys_context, &(list_r->list));
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }

  return ret;
}

/**
 * lw6sys_lifo_r_push
 *
 * @sys_context: global system context
 * @list_r: a pointer to the list (pointer on pointer, read/write value)
 * @data: the data to be pushed
 *
 * Wapper on lw6sys_lifo_r_push_, reentrant version.
 *
 * Return value: none.
 */
void
lw6sys_lifo_r_push (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, void *data)
{
  if (_lock_rw (sys_context, list_r))
    {
      lw6sys_lifo_push (sys_context, &(list_r->list), data);
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }
}

/**
 * lw6sys_lifo_r_pop
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Wrapper on lw6sys_lifo_r_pop, reentrant version.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_lifo_r_pop (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r)
{
  void *ret = NULL;

  if (_lock_rw (sys_context, list_r))
    {
      ret = lw6sys_lifo_pop (sys_context, &(list_r->list));
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }

  return ret;
}

/**
 * lw6sys_fifo_r_push
 *
 * @sys_context: global system context
 * @list_r: a pointer to the list (pointer on pointer, read/write value)
 * @data: the data to be pushed
 *
 * Wapper on lw6sys_fifo_r_push, reentrant version.
 *
 * Return value: none.
 */
void
lw6sys_fifo_r_push (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, void *data)
{
  if (_lock_rw (sys_context, list_r))
    {
      lw6sys_fifo_push (sys_context, &(list_r->list), data);
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }
}

/**
 * lw6sys_fifo_r_pop
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Wrapper on lw6sys_fifo_r_pop, reentrant version.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_fifo_r_pop (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r)
{
  void *ret = NULL;

  if (_lock_rw (sys_context, list_r))
    {
      ret = lw6sys_fifo_pop (sys_context, &(list_r->list));
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }

  return ret;
}

/**
 * lw6sys_list_r_dup
 *
 * @sys_context: global system context
 * @list_r: the reentrant list to duplicate
 * @dup_func: the function which will be called to duplicate data
 *
 * Duplicates a reentrant list.
 * This is a wrapper on @lw6sys_list_dup, but is reentrant
 * and works on a list_r type.
 *
 * Returned value: a newly allocated reentrant list.
 */
lw6sys_list_r_t *
lw6sys_list_r_dup (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, lw6sys_dup_func_t dup_func)
{
  lw6sys_list_r_t *ret = NULL;

  if (_lock_rw (sys_context, list_r))
    {
      ret = LW6SYS_MALLOC (sys_context, sizeof (lw6sys_list_r_t));
      if (ret)
	{
	  ret->mutex = LW6SYS_MUTEX_CREATE (sys_context);
	  ret->list = lw6sys_list_dup (sys_context, list_r->list, dup_func);
	  ret->free_func = list_r->free_func;

	  if ((!(ret->mutex)) || (!(ret->list)))
	    {
	      if (ret->mutex)
		{
		  LW6SYS_MUTEX_DESTROY (sys_context, ret->mutex);
		  ret->mutex = NULL;
		}
	      if (ret->list)
		{
		  lw6sys_list_free (sys_context, ret->list);
		  ret->list = NULL;
		}
	      LW6SYS_FREE (sys_context, ret);
	      ret = NULL;
	    }
	}
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }

  return ret;
}

/**
 * lw6sys_list_r_transfer_to
 *
 * @sys_context: global system context
 * @list_r: the reentrant list to transfer
 *
 * Transfers the contents of a reentrant list to a regular list.
 * Basically, this locks the list, extracts informations from it,
 * then releases the lock and leaves the list_r empty.
 * This is convenient in multithreaded contexts, typical pattern
 * is a thread that pushes items, then another thread does massive
 * transfers and processes each item separately with local pops
 * on the regular list. This limits the amount of locking.
 *
 * Returned value: a standard list, must be freed.
 */
lw6sys_list_t *
lw6sys_list_r_transfer_to (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r)
{
  lw6sys_list_t *ret = NULL;

  if (_lock_rw (sys_context, list_r))
    {
      ret = list_r->list;
      list_r->list = lw6sys_list_new (sys_context, list_r->free_func);
      _unlock (sys_context, list_r);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
    }

  return ret;
}

/**
 * lw6sys_list_r_transfer_from
 *
 * @sys_context: global system context
 * @list_r: the reentrant list where to put data
 * @list: the list to get data from
 *
 * Transfers the contents of a regular list to a reentrant list.
 * Basically, this locks the list, then appends all contents
 * from the source standard list, then releases the lock.
 * The source list pointer is set to an empty list, if
 * it's NULL it means an error happened.
 * This is convenient in multithreaded contexts, typical pattern
 * is a thread that pushes items in a bulked mode, this has
 * the advantage of less lock/unlock, plus the side effect
 * of having more atomicity, one can indeed garantee that a 
 * whole packet of items are sent at once. There's no
 * garantee on the order, you'd better sort them afterwards
 * if order does matter, by default the function does what
 * is fastest/easiest to do.
 *
 * Returned value: none, but check if *list is not null.
 */
void
lw6sys_list_r_transfer_from (lw6sys_context_t * sys_context, lw6sys_list_r_t * list_r, lw6sys_list_t ** list)
{
  lw6sys_list_t *end1 = NULL;
  lw6sys_list_t *end2 = NULL;
  lw6sys_list_t *tmp = NULL;

  if (list && *list && lw6sys_list_length (sys_context, *list) > 0)
    {
      if (_lock_rw (sys_context, list_r))
	{
	  end1 = end2 = list_r->list;

	  while (end2 && end2->next_item)
	    {
	      end1 = end2;
	      end2 = (lw6sys_list_t *) end2->next_item;
	    }

	  if (end1 && end2 && end1 != end2)
	    {
	      end1->next_item = *list;
	      lw6sys_list_free (sys_context, end2);
	      end2 = NULL;
	      (*list) = lw6sys_list_new (sys_context, list_r->free_func);
	    }
	  else
	    {
	      /*
	       * Looks like list_r has size 0 here -> just swap!
	       */
	      tmp = *list;
	      (*list) = list_r->list;
	      list_r->list = tmp;
	    }

	  _unlock (sys_context, list_r);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire lock on list_r"));
	}
    }
}
