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

typedef struct _sort_item_s
{
  lw6sys_context_t *sys_context;
  lw6sys_sort_callback_func_t func;
  void *func_data;
  void *data;
} _sort_item_t;

static int
_sort_callback (const _sort_item_t * item_a, const _sort_item_t * item_b)
{
  return item_a->func (item_a->sys_context, item_a->func_data, item_a->data, item_b->data);
}

/**
 * lw6sys_sort_int_callback
 *
 * @sys_context: global system context
 * @func_data: function specific data
 * @ptr_a: pointer to an int item
 * @ptr_b: pointer to an int item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of integers.
 *
 * Return value: -1 if @ptr_a < @ptr_b , 0 if @ptr_a == @ptr_b,
 *   1 if @ptr_a > @ptr_b
 */
int
lw6sys_sort_int_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;
  int value_a = *((int *) ptr_a);
  int value_b = *((int *) ptr_b);

  if (value_a < value_b)
    {
      ret = -1;
    }
  else if (value_a > value_b)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_sort_int_desc_callback
 *
 * @sys_context: global system context
 * @func_data: function specific data
 * @ptr_a: pointer to an int item
 * @ptr_b: pointer to an int item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of integers. This one will sort in reverse mode.
 *
 * Return value: 1 if @ptr_a < @ptr_b , 0 if @ptr_a == @ptr_b,
 *   -1 if @ptr_a > @ptr_b
 */
int
lw6sys_sort_int_desc_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;
  int value_a = *((int *) ptr_a);
  int value_b = *((int *) ptr_b);

  if (value_a < value_b)
    {
      ret = 1;
    }
  else if (value_a > value_b)
    {
      ret = -1;
    }

  return ret;
}

/**
 * lw6sys_sort_float_callback
 *
 * @sys_context: global system context
 * @func_data: function specific data
 * @ptr_a: pointer to a float item
 * @ptr_b: pointer to a float item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of floating point numbers.
 *
 * Return value: -1 if @ptr_a < @ptr_b , 0 if @ptr_a == @ptr_b,
 *   1 if @ptr_a > @ptr_b
 */
int
lw6sys_sort_float_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;
  float value_a = *((float *) ptr_a);
  float value_b = *((float *) ptr_b);

  if (value_a < value_b)
    {
      ret = -1;
    }
  else if (value_a > value_b)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_sort_float_desc_callback
 *
 * @sys_context: global system context
 * @func_data: function specific data
 * @ptr_a: pointer to a float item
 * @ptr_b: pointer to a float item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of floating point numbers. This one will
 * sort in reverse mode.
 *
 * Return value: 1 if @ptr_a < @ptr_b , 0 if @ptr_a == @ptr_b,
 *   -1 if @ptr_a > @ptr_b
 */
int
lw6sys_sort_float_desc_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;
  float value_a = *((float *) ptr_a);
  float value_b = *((float *) ptr_b);

  if (value_a < value_b)
    {
      ret = 1;
    }
  else if (value_a > value_b)
    {
      ret = -1;
    }

  return ret;
}

/**
 * lw6sys_sort_str_callback
 *
 * @sys_context: global system context
 * @func_data: function specific data
 * @ptr_a: pointer to a string item
 * @ptr_b: pointer to a string item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of 0-terminated strings.
 *
 * Return value: -1 if @ptr_a < @ptr_b , 0 if @ptr_a == @ptr_b,
 *   1 if @ptr_a > @ptr_b
 */
int
lw6sys_sort_str_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;

  ret = strcmp ((const char *) ptr_a, (const char *) ptr_b);

  return ret;
}

/**
 * lw6sys_sort_str_desc_callback
 *
 * @sys_context: global system context
 * @func_data: function specific data
 * @ptr_a: pointer to a string item
 * @ptr_b: pointer to a string item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of 0-terminated strings. This one will sort
 * in reverse mode.
 *
 * Return value: 1 if @ptr_a < @ptr_b , 0 if @ptr_a == @ptr_b,
 *   -1 if @ptr_a > @ptr_b
 */
int
lw6sys_sort_str_desc_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;

  ret = -strcmp ((const char *) ptr_a, (const char *) ptr_b);

  return ret;
}

/**
 * lw6sys_sort
 *
 * @sys_context: global system context
 * @list: the list to sort, might be modified by the function
 * @sort_func: the callback function used to sort
 * @func_data: function specific data
 *
 * A general sorting function. Internally, will use the glibc @qsort
 * function, but this one is adapted to the LW6 specific data
 * structures, more exactly, the @lw6sys_list structure. Several
 * default sort callbacks are defined, but one is free to use
 * any callback, provided it has the right prototype.
 */
void
lw6sys_sort (lw6sys_context_t * sys_context, lw6sys_list_t ** list, lw6sys_sort_callback_func_t sort_func, void *func_data)
{
  int length, i;
  _sort_item_t *array;
  lw6sys_list_t *pos;

  length = lw6sys_list_length (sys_context, *list);
  if (length > 0)
    {
      array = (_sort_item_t *) LW6SYS_CALLOC (sys_context, length * sizeof (_sort_item_t));
      if (array)
	{
	  for (pos = (*list), i = 0; pos->next_item != NULL; pos = lw6sys_list_next (sys_context, pos), i++)
	    {
	      array[i].sys_context = sys_context;
	      array[i].func = sort_func;
	      array[i].func_data = func_data;
	      array[i].data = pos->data;
	    }
	  if (i != length)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("sort error, i=%d, length=%d"), i, length);
	    }

	  // do the sort using a function that knows how to do the job
	  qsort (array, length, sizeof (_sort_item_t), (void *) _sort_callback);

	  for (pos = (*list), i = 0; pos->next_item != NULL; pos = lw6sys_list_next (sys_context, pos), i++)
	    {
	      pos->data = array[i].data;
	    }
	  if (i != length)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("sort error, i=%d, length=%d"), i, length);
	    }

	  LW6SYS_FREE (sys_context, array);
	}
    }
}
