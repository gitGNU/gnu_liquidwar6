/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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
 * lw6msg_meta_array_clear
 *
 * @array: meta array (list of nodes) to modify
 *
 * Clears the meta array, emptying all nodes.
 *
 * Return value: none
 */
void
lw6msg_meta_array_clear (lw6msg_meta_array_t * array)
{
  memset (&array, 0, sizeof (lw6msg_meta_array_t));
}

/**
 * lw6msg_meta_array_find
 *
 * @array: meta array (list of nodes) to modify
 * @node_id: the ID (64-bit) of the node to seartch
 *
 * Registers a node in the array.
 *
 * Return value: 1 if registered, 0 if not (possible error: no place left)
 */
int
lw6msg_meta_array_find (const lw6msg_meta_array_t * array, u_int64_t node_id)
{
  int ret = -1;
  int i = 0;

  for (i = 0; i < LW6MSG_NB_META_ARRAY_ITEMS && ret < 0; ++i)
    {
      if (array->items[i].node_id == node_id)
	{
	  ret = i;
	}
    }

  return ret;
}

/**
 * lw6msg_meta_array_exists
 *
 * @array: meta array (list of nodes) to modify
 * @node_id: the ID (64-bit) of the node to test
 *
 * Registers a node in the array.
 *
 * Return value: 1 if registered, 0 if not (possible error: no place left)
 */
int
lw6msg_meta_array_exists (const lw6msg_meta_array_t * array,
			  u_int64_t node_id)
{
  return (lw6msg_meta_array_find (array, node_id) >= 0);
}

/**
 * lw6msg_meta_array_set
 *
 * @array: meta array (list of nodes) to modify
 * @node_id: the ID (64-bit) of the node to add
 * @serial_0: base serialfor the node to add
 * @seq_0: base seq for for node to add
 *
 * Registers a node in the array.
 *
 * Return value: 1 if registered, 0 if not (possible error: no place left)
 */
int
lw6msg_meta_array_set (lw6msg_meta_array_t * array, u_int64_t node_id,
		       int serial_0, int64_t seq_0)
{
  int ret = 0;

  return ret;
}

/**
 * lw6msg_meta_str2array
 *
 * @array: meta array (list of nodes) to get (out param)
 * @str: meta string (list of nodes) to be put in the array
 *
 * Transforms a string describing the nodes and their id/serial/seq
 * into a more usable C structure.
 *
 * Return value: 1 if parseable and success, 0 if not.
 */
int
lw6msg_meta_str2array (lw6msg_meta_array_t * array, const char *str)
{
  int ret = 0;

  return ret;
}

/**
 * lw6msg_meta_array2str
 *
 * @array: meta array (list of nodes) to transform as a string
 *
 * Transforms a C struct describing the nodes and their id/serial/seq
 * into a string transmittable on the network.
 *
 * Return value: dynamically allocated string
 */
char *
lw6msg_meta_array2str (const lw6msg_meta_array_t * array)
{
  char *str = NULL;

  return str;
}
