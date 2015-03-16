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

typedef struct list_dup_callback_data_s
{
  lw6sys_list_t *list;
  lw6sys_dup_func_t dup_func;
} list_dup_callback_data_t;

/**
 * lw6sys_list_new
 *
 * @sys_context: global system context
 * @free_func: a callback which will be called on data when freeing the list
 *
 * Creates an empty list. There's a difference between NULL and an
 * empty list. The empty list would (in Scheme) be '() whereas
 * NULL corresponds to undefined "is not a list and will generate
 * errors if you ever call list functions on it".
 *
 * Return value: a pointer to the created object, may be NULL.
 */
lw6sys_list_t *
lw6sys_list_new (lw6sys_context_t * sys_context, lw6sys_free_func_t free_func)
{
  lw6sys_list_t *ret = NULL;

  ret = LW6SYS_CALLOC (sys_context, sizeof (lw6sys_list_t));
  if (ret)
    {
      ret->free_func = free_func;
    }

  return ret;
}

/**
 * lw6sys_list_free
 *
 * @sys_context: global system context
 * @list: the list to delete.
 *
 * Delete a list, this will cascade delete all the following
 * items in the list.
 *
 * Return value: none.
 */
void
lw6sys_list_free (lw6sys_context_t * sys_context, lw6sys_list_t * list)
{
  if (list)
    {
      /*
       * Keep a copy of next_item for we are about to
       * free the pointer to it.
       */
      lw6sys_list_t *next_item = list->next_item;

      /*
       * It's legal to have free_func or data set to NULL,
       * we simply need to avoid the core dump, but one can
       * legitimately not need any peculiar free function,
       * or desire to store a NULL content in a valid list.
       */
      if (list->free_func && list->data)
	{
	  list->free_func (sys_context, list->data);
	}

      LW6SYS_FREE (sys_context, list);

      if (next_item)
	{
	  /*
	   * This should be the last call of the function.
	   * Hopefully the compiler will find this out and
	   * optimize and *not* generate hudge stacks with
	   * return addresses which are of no use. At least
	   * the compiler *could* do it 8-) Recursion recursion...
	   */
	  lw6sys_list_free (sys_context, (lw6sys_list_t *) next_item);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL list"));
    }
}

/**
 * lw6sys_list_next
 *
 * @sys_context: global system context
 * @list: the current position in the list
 *
 * It's safer to call this rather than dig right into
 * the internals of the list.
 *
 * Return value: a new position in the list, may be NULL.
 */
lw6sys_list_t *
lw6sys_list_next (lw6sys_context_t * sys_context, lw6sys_list_t * list)
{
  lw6sys_list_t *ret = NULL;

  if (list)
    {
      ret = list->next_item;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling next on NULL list"));
    }

  return ret;
}

/**
 * lw6sys_list_is_empty
 *
 * @sys_context: global system context
 * @list: the list we want informations about
 *
 * Checks wether the list is empty or not. Note that being empty
 * and being NULL is not the same. An empty list is a valid pointer
 * on a list where there's no item, a NULL pointer is not a list at all.
 * Do *NOT* call this function on NULL.
 *
 * Return value: 1 if empty, 0 if there is at list one item.
 */
int
lw6sys_list_is_empty (lw6sys_context_t * sys_context, const lw6sys_list_t * list)
{
  int empty = 1;

  if (list)
    {
      empty = (list->next_item == NULL);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling is_empty on NULL list"));
    }

  return empty;
}

/**
 * lw6sys_list_length
 *
 * @sys_context: global system context
 * @list: the list we want informations about
 *
 * Calculates the length of the list. This is a performance killer
 * for lists are inadapted to this. But it can still be usefull.
 *
 * Return value: the number of elements, 0 is none (empty list).
 */
int
lw6sys_list_length (lw6sys_context_t * sys_context, const lw6sys_list_t * list)
{
  int ret = 0;

  if (list)
    {
      while (list->next_item)
	{
	  ret++;
	  list = (lw6sys_list_t *) list->next_item;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling length on NULL list"));
    }

  return ret;
}

/**
 * lw6sys_list_map
 *
 * @sys_context: global system context
 * @list: the list where elements will be taken
 * @func: the function which will be executed
 * @func_data: additionnal data to be passed to @func
 *
 * Executes a function on all list items.
 * The @func_data parameter allows you to pass extra values to
 * the function, such as a file handler or any variable which
 * can not be inferred from list item values, and you of course
 * do not want to make global... Not as convenient as a
 * real "for each" construct as can be found in any modern
 * langage, but does the job. No return value, if you really
 * want one, pass a structure in @func_data and modify something
 * in it on success, failure, whatever.
 *
 * Return value: none.
 */
void
lw6sys_list_map (lw6sys_context_t * sys_context, lw6sys_list_t * list, lw6sys_list_callback_func_t func, void *func_data)
{
  if (list)
    {
      while (list)
	{
	  if (list->next_item)
	    {
	      func (sys_context, func_data, list->data);
	    }
	  list = list->next_item;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling map on NULL list"));
    }
}

/**
 * lw6sys_list_filter
 *
 * @sys_context: global system context
 * @list: the list where elements will be taken
 * @func: the function which will be executed
 * @func_data: additionnal data to be passed to @func
 *
 * Executes a function on all list items and keeps only those
 * for which the function returned non zero (true).
 * The @func_data parameter allows you to pass extra values to
 * the function, such as a file handler or any variable which
 * can not be inferred from list item values, and you of course
 * do not want to make global...
 *
 * Return value: none.
 */
void
lw6sys_list_filter (lw6sys_context_t * sys_context, lw6sys_list_t ** list, lw6sys_list_filter_func_t func, void *func_data)
{
  lw6sys_list_t *tmp_item = NULL;
  lw6sys_list_t *cur_item = NULL;
  lw6sys_list_t *prev_item = NULL;

  cur_item = (*list);
  if (cur_item)
    {
      while (cur_item)
	{
	  if (cur_item->next_item)
	    {
	      if (func (sys_context, func_data, cur_item->data))
		{
		  prev_item = cur_item;
		  cur_item = cur_item->next_item;
		}
	      else
		{
		  tmp_item = cur_item;
		  if (cur_item->free_func && cur_item->data)
		    {
		      cur_item->free_func (sys_context, cur_item->data);
		    }
		  cur_item = cur_item->next_item;
		  if (prev_item)
		    {
		      prev_item->next_item = cur_item;
		    }
		  else
		    {
		      (*list) = cur_item;
		    }
		  LW6SYS_FREE (sys_context, tmp_item);
		}
	    }
	  else
	    {
	      cur_item = NULL;
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling filter on NULL list"));
    }
}

/**
 * lw6sys_list_push_front
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 * @data: the data to be pushed
 *
 * Pushes data on the list. The @free_func function is copied
 * from the previous element. The pointer on the list is
 * changed "in place" (in/out). Note that if there's a @malloc
 * problem it might end-up being NULL... This should be rare
 * but it *can* happen. You cannot push something else than a
 * pointer, pushing an int is a very bad idea. Push a pointer
 * on the integer, and ensure it's always there, or @malloc it
 * and pass @lw6sys_free_callback when creating the list.
 * If you think you can cast an integer into a pointer, think
 * 64-bit machines...
 *
 * Return value: none.
 */
void
lw6sys_list_push_front (lw6sys_context_t * sys_context, lw6sys_list_t ** list, void *data)
{
  lw6sys_list_t *new_list = NULL;

  if (list && *list)
    {
      new_list = LW6SYS_MALLOC (sys_context, sizeof (lw6sys_list_t));
      if (new_list)
	{
	  new_list->next_item = (*list);
	  new_list->data = data;
	  new_list->free_func = (*list)->free_func;
	  (*list) = new_list;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling push_front on NULL list"));
    }
}

/**
 * lw6sys_list_pop_front
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Pops data from the list, the returned value is what
 * was passed to list_push. The pointer on the list is
 * changed "in place" (in/out). When data is popped,
 * that needs some freeing (i.e. free_func was not NULL
 * when creating the list) then it's the responsibility
 * of the caller to free it when popping it. One popped
 * it's not freed, but it's out of the list scope. Of
 * course the lw6sys_list_t is freed, but not the data.
 * If you happen to store non-NULL data in your list, you
 * can call this function without bothering calling @lw6sys_list_is_empty
 * and assume that when you get NULL, there's no data left.
 * At this stage, the list won't exist anymore BTW, you won't
 * even need to free it. The idea is: popping a list which
 * has no elements left (empty list) destroys the list and returns NULL.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_list_pop_front (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  void *data = NULL;

  if (list && *list)
    {
      lw6sys_list_t *new_list;
      data = (*list)->data;
      new_list = (*list)->next_item;

      /*
       * Here we do *NOT* call free_func, or else the
       * value returned would be freed before it is even
       * returned to the caller!!!
       */
      LW6SYS_FREE (sys_context, *list);

      (*list) = new_list;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling pop_front on NULL list"));
    }

  return data;
}

/**
 * lw6sys_list_push_back
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 * @data: the data to be pushed
 *
 * Pushes data on the list. The @free_func function is copied
 * from the previous element. The pointer on the list is
 * changed "in place" (in/out). Note that if there's a @malloc
 * problem it might end-up being NULL... This should be rare
 * but it *can* happen. You cannot push something else than a
 * pointer, pushing an int is a very bad idea. Push a pointer
 * on the integer, and ensure it's always there, or @malloc it
 * and pass @lw6sys_free_callback when creating the list.
 * If you think you can cast an integer into a pointer, think
 * 64-bit machines...
 *
 * Return value: none.
 */
void
lw6sys_list_push_back (lw6sys_context_t * sys_context, lw6sys_list_t ** list, void *data)
{
  lw6sys_list_t *end = NULL;
  lw6sys_list_t *new_list = NULL;

  if (list && *list)
    {
      end = (*list);
      while (end && end->next_item)
	{
	  end = end->next_item;
	}

      if (end && !end->next_item)
	{
	  new_list = LW6SYS_CALLOC (sys_context, sizeof (lw6sys_list_t));
	  if (new_list)
	    {
	      end->next_item = new_list;
	      end->data = data;
	      new_list->free_func = end->free_func;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("list push_back bug"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling push_back on NULL list"));
    }
}

/**
 * lw6sys_list_pop_back
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Pops data from the list, the returned value is what
 * was passed to list_push. The pointer on the list is
 * changed "in place" (in/out). When data is popped,
 * that needs some freeing (i.e. free_func was not NULL
 * when creating the list) then it's the responsibility
 * of the caller to free it when popping it. One popped
 * it's not freed, but it's out of the list scope. Of
 * course the lw6sys_list_t is freed, but not the data.
 * If you happen to store non-NULL data in your list, you
 * can call this function without bothering calling @lw6sys_list_is_empty
 * and assume that when you get NULL, there's no data left.
 * At this stage, the list won't exist anymore BTW, you won't
 * even need to free it. The idea is: popping a list which
 * has no elements left (empty list) destroys the list and returns NULL.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_list_pop_back (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  void *data = NULL;
  lw6sys_list_t *end = NULL;
  lw6sys_list_t *before_end = NULL;
  lw6sys_list_t *tmp = NULL;

  if (list && *list)
    {
      end = (*list);
      before_end = end;
      while (end && end->next_item && ((lw6sys_list_t *) (end->next_item))->next_item)
	{
	  before_end = end;
	  end = end->next_item;
	}
      data = end->data;
      if (((*list)->next_item) == NULL)
	{
	  LW6SYS_FREE (sys_context, *list);
	  (*list) = NULL;
	}
      else
	{
	  if (((lw6sys_list_t *) ((*list)->next_item))->next_item == NULL)
	    {
	      tmp = (*list);
	      (*list) = (*list)->next_item;
	      LW6SYS_FREE (sys_context, tmp);
	    }
	  else
	    {
	      tmp = end->next_item;
	      //end->next_item = ((lw6sys_list_t *) (end->next_item))->next_item;
	      before_end->next_item = tmp;
	      /*
	       * Here we do *NOT* call free_func, or else the
	       * value returned would be freed before it is even
	       * returned to the caller!!!
	       */
	      LW6SYS_FREE (sys_context, end);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling pop_back on NULL list"));
    }

  return data;
}

/**
 * lw6sys_lifo_push
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Pops data to a list, in last-in first-out mode (AKA LIFO).
 * This is equivalent t @lw6sys_list_push_front.
 *
 * Return value: none.
 */
void
lw6sys_lifo_push (lw6sys_context_t * sys_context, lw6sys_list_t ** list, void *data)
{
  lw6sys_list_push_front (sys_context, list, data);
}

/**
 * lw6sys_lifo_pop
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Pops the first element of a list, in last-in first-out mode (AKA LIFO).
 * This is equivalent to @lw6sys_list_pop_front.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_lifo_pop (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  return lw6sys_list_pop_front (sys_context, list);
}

/**
 * lw6sys_fifo_push
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Pops data to a list, in first-in first-out mode (AKA FIFO).
 * This is equivalent t @lw6sys_list_push_front.
 *
 * Return value: none.
 */
void
lw6sys_fifo_push (lw6sys_context_t * sys_context, lw6sys_list_t ** list, void *data)
{
  lw6sys_list_push_front (sys_context, list, data);
}

/**
 * lw6sys_fifo_pop
 *
 * @sys_context: global system context
 * @list: a pointer to the list (pointer on pointer, read/write value)
 *
 * Pops the first element of a list, in last-in first-out mode (AKA FIFO).
 * This is equivalent to @lw6sys_list_pop_back.
 * It can be quite time-consuming on big lists.
 *
 * Return value: a pointer on the popped data, whatever you pushed.
 */
void *
lw6sys_fifo_pop (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  return lw6sys_list_pop_back (sys_context, list);
}

static void
list_dup_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  list_dup_callback_data_t *list_dup_callback_data = (list_dup_callback_data_t *) func_data;
  void *new_data;

  if (list_dup_callback_data->dup_func)
    {
      new_data = list_dup_callback_data->dup_func (sys_context, data);
    }
  else
    {
      new_data = data;
    }

  // push_back is slower but this way we preserve order
  lw6sys_list_push_back (sys_context, &(list_dup_callback_data->list), new_data);
}

/**
 * lw6sys_list_dup
 *
 * @sys_context: global system context
 * @list: the list to duplicate, can be NULL
 * @dup_func: the function which will be called to duplicate data
 *
 * Duplicates a list. All data will be copied so that if the first
 * list is deleted, the duplicated one is fine. Addtionnally, dup_func
 * will be called to filter all data, and possibly allocated new
 * pointers if needed, for instance. If dup_func is NULL, then data
 * values will simply be copied. This is likely to be usefull when
 * data is not dynamically allocated.
 *
 * Returned value: a newly allocated list.
 */
lw6sys_list_t *
lw6sys_list_dup (lw6sys_context_t * sys_context, lw6sys_list_t * list, lw6sys_dup_func_t dup_func)
{
  list_dup_callback_data_t list_dup_callback_data;
  lw6sys_list_t *ret = NULL;

  if (list)
    {
      list_dup_callback_data.list = lw6sys_list_new (sys_context, list->free_func);
      list_dup_callback_data.dup_func = dup_func;

      lw6sys_list_map (sys_context, list, list_dup_callback, &list_dup_callback_data);

      ret = list_dup_callback_data.list;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling dup on NULL list"));
    }

  return ret;
}
