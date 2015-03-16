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

#include "pil.h"

static lw6pil_local_cursor_t *
_get_cursor (lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id, int create)
{
  lw6pil_local_cursor_t *ret = NULL;
  int i;

  for (i = 0; i < local_cursors->nb_cursors; ++i)
    {
      if (local_cursors->cursors[i].cursor_id == cursor_id)
	{
	  ret = &(local_cursors->cursors[i]);
	  break;
	}
    }
  if (create && (!ret) && (i < LW6MAP_MAX_NB_CURSORS) && (local_cursors->nb_cursors < LW6MAP_MAX_NB_CURSORS))
    {
      ret = &(local_cursors->cursors[i]);
      ret->cursor_id = cursor_id;
      ret->x = 0;
      ret->y = 0;
      ret->mouse_controlled = 0;
      local_cursors->nb_cursors++;
    }

  return ret;
}

/**
 * lw6pil_local_cursors_reset
 *
 * @sys_context: global system context
 * @local_cursors: the structure to reset
 *
 * Resets a local cursors struct. Note that this need not be called very often,
 * in fact the local cursors can cope with "dead" cursors easily. In practise,
 * in a local game, there are only 4 of them, great maximum.
 *
 * Return value: none.
 */
void
lw6pil_local_cursors_reset (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors)
{
  memset (local_cursors, 0, sizeof (lw6pil_local_cursors_t));
}

/**
 * lw6pil_local_cursors_get_cursor
 *
 * @sys_context: global system context
 * @local_cursors: the structure to query
 * @cursor_id: the id of the cursor to query
 *
 * Returns a pointer on the cursor with the given id.
 *
 * Return value: a pointer (must *not* be freed) which is NULL is cursor
 * does not exist.
 */
lw6pil_local_cursor_t *
lw6pil_local_cursors_get_cursor (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id)
{
  return _get_cursor (local_cursors, cursor_id, 0);
}

/**
 * lw6pil_local_cursors_get_info
 *
 * @local_cursors: the structure to query
 * @x: a pointer to the x position, may be NULL
 * @y: a pointer to the y position, may be NULL
 * @mouse_controlled: a pointer to the mouse_controlled flag, may be NULL
 * @cursor_id: the id of the cursor to query
 *
 * Gets the x,y position of the cursor, and tells if it's mouse controlled.
 *
 * Return value: 1 on success (cursor exists), 0 on failure (no such cursor).
 */
int
lw6pil_local_cursors_get_info (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, int *x, int *y, int *mouse_controlled,
			       u_int16_t cursor_id)
{
  int ret = 0;
  lw6pil_local_cursor_t *cursor;

  cursor = _get_cursor (local_cursors, cursor_id, 0);
  if (cursor)
    {
      if (x)
	{
	  *x = cursor->x;
	}
      if (y)
	{
	  *y = cursor->y;
	}
      if (mouse_controlled)
	{
	  *mouse_controlled = cursor->mouse_controlled;
	}
      ret = 1;
    }

  return ret;
}

/**
 * lw6pil_local_cursors_set_xy
 *
 * @sys_context: global system context
 * @local_cursors: the structure to modify
 * @cursor_id: the id of the cursor to modify
 * @x: the x position
 * @y: the y position
 *
 * Sets the position of a cursor in the local cursors struct. If cursor does
 * not exists, it's appended to the list.
 *
 * Return value: 1 on success (cursor exists), 0 on failure (no such cursor).
 */
int
lw6pil_local_cursors_set_xy (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id, int x, int y)
{
  int ret = 0;
  lw6pil_local_cursor_t *cursor;

  cursor = _get_cursor (local_cursors, cursor_id, 1);
  if (cursor)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("set local cursor %x to %d,%d"), cursor_id, x, y);
      cursor->x = x;
      cursor->y = y;
      ret = 1;
    }

  return ret;
}

/**
 * lw6pil_local_cursors_set_mouse_controlled
 *
 * @sys_context: global system context
 * @local_cursors: the structure to modify
 * @cursor_id: the id of the cursor to modify
 * @mouse_controlled: the mouse_controlled attribute
 *
 * Sets which cursor is mouse controlled. If mouse_controlled is 1,
 * the flag is set for this cursor and cleared for all others. If set
 * to 0, only this cursor's flag is cleared.
 *
 * Return value: 1 on success (cursor exists), 0 on failure (no such cursor).
 */
int
lw6pil_local_cursors_set_mouse_controlled (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id, int mouse_controlled)
{
  int ret = 0;
  int i;

  for (i = 0; i < local_cursors->nb_cursors; ++i)
    {
      if (local_cursors->cursors[i].cursor_id == cursor_id)
	{
	  local_cursors->cursors[i].mouse_controlled = mouse_controlled;
	  ret = 1;
	}
      else
	{
	  if (mouse_controlled)
	    {
	      local_cursors->cursors[i].mouse_controlled = 0;
	    }
	}
    }

  return ret;
}

/**
 * lw6pil_local_cursors_get_main_info
 *
 * @sys_context: global system context
 * @local_cursors: the structure to query
 * @cursor_id: the id of the main cursor, may be NULL
 * @x: a pointer to the x position, may be NULL
 * @y: a pointer to the y position, may be NULL
 * @mouse_controlled: a pointer to the mouse_controlled flag, may be NULL
 *
 * Gets the x,y position of the main cursor, and tells if it's mouse controlled.
 *
 * Return value: 1 on success (cursor exists), 0 on failure (no such cursor).
 */
int
lw6pil_local_cursors_get_main_info (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t * cursor_id, int *x, int *y,
				    int *mouse_controlled)
{
  int ret = 0;
  int i = local_cursors->main_i;

  if (lw6sys_check_id_16 (sys_context, local_cursors->main_cursor_id) && local_cursors->cursors[i].is_main)
    {
      if (cursor_id)
	{
	  *cursor_id = local_cursors->cursors[i].cursor_id;
	}
      if (x)
	{
	  *x = local_cursors->cursors[i].x;
	}
      if (y)
	{
	  *y = local_cursors->cursors[i].y;
	}
      if (mouse_controlled)
	{
	  *mouse_controlled = local_cursors->cursors[i].mouse_controlled;
	}
      ret = 1;
    }

  return ret;
}

/**
 * lw6pil_local_cursors_set_main
 *
 * @sys_context: global system context
 * @local_cursors: the structure to modify
 * @cursor_id: the id of the cursor to be labelled as main cursor
 *
 * Sets the main cursor attribute, the main cursor is later used,
 * for instance, to decide how to display the map (centered on it,
 * for instance).
 *
 * Return value: 1 on success (cursor exists), 0 on failure (no such cursor).
 */
int
lw6pil_local_cursors_set_main (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, u_int16_t cursor_id)
{
  int ret = 0;
  int i;

  for (i = 0; i < local_cursors->nb_cursors; ++i)
    {
      if (local_cursors->cursors[i].cursor_id == cursor_id)
	{
	  local_cursors->main_i = i;
	  local_cursors->main_cursor_id = cursor_id;
	  local_cursors->cursors[i].is_main = 1;
	  ret = 1;
	}
      else
	{
	  local_cursors->cursors[i].is_main = 0;
	}
    }

  return ret;
}
