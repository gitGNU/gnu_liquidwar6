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
#endif

#include "cnx.h"

/**
 * lw6cnx_backlog_zero
 *
 * @backlog: struct to initialize
 *
 * Fills the struct with zeros, ignoring what was in there before
 *
 * Return value: none.
 */
void
lw6cnx_backlog_zero (lw6cnx_backlog_t * backlog)
{
  memset (backlog, 0, sizeof (lw6cnx_backlog_t));
}

/**
 * lw6cnx_backlog_reset
 *
 * @backlog: struct to reset
 *
 * Resets the structure, freeing any ressource it was holding.
 *
 * Return value: none.
 */
void
lw6cnx_backlog_reset (lw6cnx_backlog_t * backlog)
{
  if (backlog->list)
    {
      lw6sys_list_free (backlog->list);
    }
  lw6cnx_backlog_zero (backlog);
}

/**
 * lw6cnx_backlog_push
 *
 * @backlog: struct to add stuff to
 * @str: string to add
 *
 * Adds a string to the backlog. Be careful, the function will not duplicate
 * the string but assume it's been dynamically allocated, and will free it
 * when it's done with it.
 *
 * Return value: none.
 */
void
lw6cnx_backlog_push (lw6cnx_backlog_t * backlog, char *str)
{
  int pushed = 0;

  if (!(backlog->list))
    {
      backlog->list = lw6sys_list_new (lw6sys_free_callback);
    }
  if (backlog->list)
    {
      /*
       * We use a push_front, this will cause messages to arrive
       * in the wrong order *but* this is generally not a big problem
       * since when we get to the point the backlog fills itself,
       * it usually means that we are sending big messages splitted
       * in many atoms and those atoms need to all be here for the
       * message to be available.
       */
      lw6sys_list_push_front (&(backlog->list), str);
      if (backlog->list)
	{
	  pushed = 1;
	}
    }

  if (!pushed)
    {
      LW6SYS_FREE (str);
    }
}

/**
 * lw6cnx_backlog_filter
 *
 * @backlog: struct to filter
 * @func: function used for filter
 * @func_data: additionnal data passed to function
 *
 * Filters the internal backlog list, the filter function, as standard list
 * filter funcs, should return 1 if the record is to be kept, and 0 if it
 * must be deleted, typically because it finally has been sent.
 *
 * Return value: none.
 */
void
lw6cnx_backlog_filter (lw6cnx_backlog_t * backlog,
		       lw6cnx_backlog_filter_func_t func,
		       lw6cnx_backlog_filter_data_t * func_data)
{
  if (backlog->list)
    {
      lw6sys_list_filter (&(backlog->list), (lw6sys_list_filter_func_t) func,
			  (void *) func_data);
    }
}
