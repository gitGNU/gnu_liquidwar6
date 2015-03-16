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

#include "msg.h"

/**
 * lw6msg_meta_array_zero
 *
 * @sys_context: global system context
 * @meta_array: meta meta_array (list of nodes) to modify
 *
 * Fills the meta meta_array with zeroes, emptying all nodes.
 *
 * Return value: none
 */
void
lw6msg_meta_array_zero (lw6sys_context_t * sys_context, lw6msg_meta_array_t * meta_array)
{
  memset ((void *) meta_array, 0, sizeof (lw6msg_meta_array_t));
}

/**
 * lw6msg_meta_array_find
 *
 * @sys_context: global system context
 * @meta_array: meta meta_array (list of nodes) to modify
 * @node_id: the ID (64-bit) of the node to seartch
 *
 * Registers a node in the meta_array.
 *
 * Return value: 1 if registered, 0 if not (possible error: no place left)
 */
int
lw6msg_meta_array_find (lw6sys_context_t * sys_context, const lw6msg_meta_array_t * meta_array, u_int64_t node_id)
{
  int ret = -1;
  int i = 0;

  for (i = 0; i < LW6MSG_NB_META_ARRAY_ITEMS && ret < 0; ++i)
    {
      if (meta_array->items[i].node_id == node_id)
	{
	  ret = i;
	}
    }

  return ret;
}

/**
 * lw6msg_meta_array_exists
 *
 * @sys_context: global system context
 * @meta_array: meta meta_array (list of nodes) to modify
 * @node_id: the ID (64-bit) of the node to test
 *
 * Registers a node in the meta_array.
 *
 * Return value: 1 if registered, 0 if not (possible error: no place left)
 */
int
lw6msg_meta_array_exists (lw6sys_context_t * sys_context, const lw6msg_meta_array_t * meta_array, u_int64_t node_id)
{
  return (lw6msg_meta_array_find (sys_context, meta_array, node_id) >= 0) ? 1 : 0;
}

/**
 * lw6msg_meta_array_set
 *
 * @sys_context: global system context
 * @meta_array: meta meta_array (list of nodes) to modify
 * @node_id: the ID (64-bit) of the node to add
 * @serial_0: base serialfor the node to add
 * @seq_0: base seq for for node to add
 *
 * Registers a node in the meta_array.
 *
 * Return value: 1 if registered, 0 if not (possible error: no place left)
 */
int
lw6msg_meta_array_set (lw6sys_context_t * sys_context, lw6msg_meta_array_t * meta_array, u_int64_t node_id, int serial_0, int64_t seq_0)
{
  int ret = 0;
  int i = -1;

  /*
   * Searching wether node is already registered
   */
  i = lw6msg_meta_array_find (sys_context, meta_array, node_id);
  if (i >= 0)
    {
      meta_array->items[i].serial_0 = serial_0;
      meta_array->items[i].seq_0 = seq_0;
      ret = 1;
    }
  else
    {
      /*
       * Searching for a free slot
       */
      i = lw6msg_meta_array_find (sys_context, meta_array, 0LL);
      if (i >= 0)
	{
	  meta_array->items[i].node_id = node_id;
	  meta_array->items[i].serial_0 = serial_0;
	  meta_array->items[i].seq_0 = seq_0;
	  ret = 1;
	}
    }

  return ret;
}

/**
 * lw6msg_meta_array_unset
 *
 * @sys_context: global system context
 * @meta_array: meta meta_array (list of nodes) to modify
 * @node_id: the ID (64-bit) of the node to remove
 *
 * Unregisters a node in the meta_array.
 *
 * Return value: 1 if node existed, 0 if it was not here
 */
int
lw6msg_meta_array_unset (lw6sys_context_t * sys_context, lw6msg_meta_array_t * meta_array, u_int64_t node_id)
{
  int ret = 0;
  int i = -1;

  /*
   * Searching wether node is already registered
   */
  i = lw6msg_meta_array_find (sys_context, meta_array, node_id);
  if (i >= 0)
    {
      memset (&(meta_array->items[i]), 0, sizeof (lw6msg_meta_array_item_t));
      ret = 1;
    }

  return ret;
}

/**
 * lw6msg_meta_str2array
 *
 * @sys_context: global system context
 * @meta_array: meta meta_array (list of nodes) to get (out param)
 * @str: meta string (list of nodes) to be put in the meta_array
 *
 * Transforms a string describing the nodes and their id/serial/seq
 * into a more usable C structure.
 *
 * Return value: 1 if parseable and success, 0 if not.
 */
int
lw6msg_meta_str2array (lw6sys_context_t * sys_context, lw6msg_meta_array_t * meta_array, const char *str)
{
  int ret = 0;
  const char *pos = NULL;
  char *seek = NULL;
  int still_ok = 1;
  u_int64_t node_id = 0LL;
  int serial_0 = 0;
  int64_t seq_0 = 0LL;

  pos = str;
  while (still_ok)
    {
      if (lw6msg_word_first_id_64 (sys_context, &node_id, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_int_32_ge0 (sys_context, &serial_0, &seek, pos))
	    {
	      pos = seek;
	      if (lw6msg_word_first_int_64_gt0 (sys_context, &seq_0, &seek, pos))
		{
		  pos = seek;
		  if (lw6msg_meta_array_set (sys_context, meta_array, node_id, serial_0, seq_0))
		    {
		      ret = 1;
		    }
		  else
		    {
		      still_ok = 0;
		    }
		}
	      else
		{
		  still_ok = 0;
		  ret = 0;
		}
	    }
	  else
	    {
	      still_ok = 0;
	      ret = 0;
	    }
	}
      else
	{
	  /*
	   * Not finding an id in the first place is not a problem
	   * in itself -> it problably means "end of list". So
	   * we don't explicitely set ret=0
	   */
	  still_ok = 0;
	}
    }

  return ret;
}

/**
 * lw6msg_meta_array2str
 *
 * @sys_context: global system context
 * @meta_array: meta meta_array (list of nodes) to transform as a string
 *
 * Transforms a C struct describing the nodes and their id/serial/seq
 * into a string transmittable on the network.
 *
 * Return value: dynamically allocated string
 */
char *
lw6msg_meta_array2str (lw6sys_context_t * sys_context, const lw6msg_meta_array_t * meta_array)
{
  char *str = NULL;
  char *tmp = NULL;
  int i = 0;

  for (i = 0; i < LW6MSG_NB_META_ARRAY_ITEMS; ++i)
    {
      if (meta_array->items[i].node_id > 0)
	{
	  if (str)
	    {
	      tmp =
		lw6sys_new_sprintf (sys_context, "%s %" LW6SYS_PRINTF_LL "x %d %"
				    LW6SYS_PRINTF_LL "d", str,
				    (long long) meta_array->items[i].node_id, meta_array->items[i].serial_0, (long long) meta_array->items[i].seq_0);
	      if (tmp)
		{
		  LW6SYS_FREE (sys_context, str);
		  str = tmp;
		}
	    }
	  else
	    {
	      str =
		lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "x %d %"
				    LW6SYS_PRINTF_LL "d",
				    (long long) meta_array->items[i].node_id, meta_array->items[i].serial_0, (long long) meta_array->items[i].seq_0);
	    }
	}
    }

  return str;
}
