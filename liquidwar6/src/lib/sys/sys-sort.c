/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

/**
 * lw6sys_sort_int_callback
 *
 * @list_a: pointer to a list of int item
 * @list_b: pointer to a list of int item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of integers.
 *
 * Return value: -1 if @list_a < @list_b , 0 if @list_a == @list_b,
 *   1 if @list_a > @list_b
 */
int
lw6sys_sort_int_callback (const lw6sys_list_t ** list_a,
			  const lw6sys_list_t ** list_b)
{
  int ret = 0;
  int a, b;

  a = *((int *) ((*list_a)->data));
  b = *((int *) ((*list_b)->data));

  if (a < b)
    {
      ret = -1;
    }
  else if (a > b)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_sort_int_desc_callback
 *
 * @list_a: pointer to a list of int item
 * @list_b: pointer to a list of int item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of integers. This one will sort in reverse mode.
 *
 * Return value: 1 if @list_a < @list_b , 0 if @list_a == @list_b,
 *   -1 if @list_a > @list_b
 */
int
lw6sys_sort_int_desc_callback (const lw6sys_list_t ** list_a,
			       const lw6sys_list_t ** list_b)
{
  int ret = 0;
  int a, b;

  a = *((int *) ((*list_a)->data));
  b = *((int *) ((*list_b)->data));

  if (a < b)
    {
      ret = 1;
    }
  else if (a > b)
    {
      ret = -1;
    }

  return ret;
}

/**
 * lw6sys_sort_float_callback
 *
 * @list_a: pointer to a list of float item
 * @list_b: pointer to a list of float item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of floating point numbers.
 *
 * Return value: -1 if @list_a < @list_b , 0 if @list_a == @list_b,
 *   1 if @list_a > @list_b
 */
int
lw6sys_sort_float_callback (const lw6sys_list_t ** list_a,
			    const lw6sys_list_t ** list_b)
{
  int ret = 0;
  float a, b;

  a = *((float *) ((*list_a)->data));
  b = *((float *) ((*list_b)->data));

  if (a < b)
    {
      ret = -1;
    }
  else if (a > b)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6sys_sort_float_desc_callback
 *
 * @list_a: pointer to a list of float item
 * @list_b: pointer to a list of float item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of floating point numbers. This one will
 * sort in reverse mode.
 *
 * Return value: 1 if @list_a < @list_b , 0 if @list_a == @list_b,
 *   -1 if @list_a > @list_b
 */
int
lw6sys_sort_float_desc_callback (const lw6sys_list_t ** list_a,
				 const lw6sys_list_t ** list_b)
{
  int ret = 0;
  float a, b;

  a = *((float *) ((*list_a)->data));
  b = *((float *) ((*list_b)->data));

  if (a < b)
    {
      ret = 1;
    }
  else if (a > b)
    {
      ret = -1;
    }

  return ret;
}

/**
 * lw6sys_sort_str_callback
 *
 * @list_a: pointer to a list of string item
 * @list_b: pointer to a list of string item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of 0-terminated strings.
 *
 * Return value: -1 if @list_a < @list_b , 0 if @list_a == @list_b,
 *   1 if @list_a > @list_b
 */
int
lw6sys_sort_str_callback (const lw6sys_list_t ** list_a,
			  const lw6sys_list_t ** list_b)
{
  int ret = 0;

  ret = strcmp ((char *) ((*list_a)->data), (char *) ((*list_b)->data));

  return ret;
}

/**
 * lw6sys_sort_str_desc_callback
 *
 * @list_a: pointer to a list of string item
 * @list_b: pointer to a list of string item
 *
 * A typicall sort callback function, can be passed to @lw6sys_sort
 * to sort a list of 0-terminated strings. This one will sort
 * in reverse mode.
 *
 * Return value: 1 if @list_a < @list_b , 0 if @list_a == @list_b,
 *   -1 if @list_a > @list_b
 */
int
lw6sys_sort_str_desc_callback (const lw6sys_list_t ** list_a,
			       const lw6sys_list_t ** list_b)
{
  int ret = 0;

  ret = -strcmp ((char *) ((*list_a)->data), (char *) ((*list_b)->data));

  return ret;
}

/**
 * lw6sys_sort
 *
 * @list: the list to sort, might be modified by the function
 * @sort_func: the callback function used to sort
 *
 * A general sorting function. Internally, will use the glibc @qsort
 * function, but this one is adapted to the LW6 specific data
 * structures, more exactly, the @lw6sys_list structure. Several
 * default sort callbacks are defined, but one is free to use
 * any callback, provided it has the right prototype.
 */
void
lw6sys_sort (lw6sys_list_t ** list, lw6sys_sort_callback_func_t sort_func)
{
  int length, i;
  lw6sys_list_t **array;
  lw6sys_list_t *pos;
  lw6sys_list_t *end;

  length = lw6sys_list_length (*list);
  if (length > 0)
    {
      array =
	(lw6sys_list_t **) LW6SYS_CALLOC (length * sizeof (lw6sys_list_t *));
      if (array)
	{
	  for (pos = (*list), i = 0; pos->next_item != NULL;
	       pos = lw6sys_list_next (pos), i++)
	    {
	      array[i] = pos;
	    }
	  if (i != length)
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("sort error, i=%d, length=%d"), i, length);
	    }
	  end = pos;

	  // do the sort using a function that knows how to do the job
	  qsort (array, length, sizeof (lw6sys_list_t *), (void *) sort_func);

	  for (i = 0; i < length - 1; ++i)
	    {
	      array[i]->next_item = array[i + 1];
	    }
	  array[length - 1]->next_item = end;

	  (*list) = array[0];

	  LW6SYS_FREE (array);
	}
    }
}
