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

#include "gui.h"

/**
 * lw6gui_mouse_register_move
 *
 * @sys_context: global system context
 * @mouse: the mouse object to work on
 * @screen_pos_x: the x position on screen
 * @screen_pos_y: the y position on screen
 * @timestamp: current timestamp
 *
 * Registers a mouse move event.
 *
 * Return value: note.
 */
void
lw6gui_mouse_register_move (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, int screen_pos_x, int screen_pos_y, int64_t timestamp)
{
  if (mouse->screen_pointer.pos_x != screen_pos_x || mouse->screen_pointer.pos_y != screen_pos_y)
    {
      int dt;

      dt = (int64_t) (timestamp - mouse->last_moved);
      if (dt > 0)
	{
	  mouse->screen_pointer.speed_x = ((screen_pos_x - mouse->screen_pointer.pos_x) * 1000) / dt;
	  mouse->screen_pointer.speed_y = ((screen_pos_y - mouse->screen_pointer.pos_y) * 1000) / dt;
	}
      mouse->screen_pointer.pos_x = screen_pos_x;
      mouse->screen_pointer.pos_y = screen_pos_y;
      mouse->moved = 1;
      mouse->last_moved = timestamp;
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("mouse pos=%d,%d speed=%d,%d"),
		  mouse->screen_pointer.pos_x, mouse->screen_pointer.pos_y, mouse->screen_pointer.speed_x, mouse->screen_pointer.speed_y);
      /*
         if (mouse->drag_mode==LW6GUI_DRAG_MODE_OFF) {
         mouse->screen_drag_start = mouse->screen_pointer;
         }
       */
    }
}

/**
 * lw6gui_mouse_poll_move
 *
 * @sys_context: global system context
 * @mouse: the mouse object to poll
 * @screen_pos_x: pointer to the x position (can be NULL), will be updated even if no move
 * @screen_pos_y: pointer to the y position (can be NULL), will be updated even if no move
 *
 * Asks wether the mouse has moved or not.
 *
 * Return value: 1 if mouse was moved since last call, 0 if not.
 */
int
lw6gui_mouse_poll_move (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, int *screen_pos_x, int *screen_pos_y)
{
  int ret = 0;

  if (mouse->moved)
    {
      mouse->moved = 0;
      ret = 1;
    }

  if (screen_pos_x)
    {
      (*screen_pos_x) = mouse->screen_pointer.pos_x;
    }
  if (screen_pos_y)
    {
      (*screen_pos_y) = mouse->screen_pointer.pos_y;
    }

  return ret;
}

/**
 * lw6gui_mouse_update_repeat
 *
 * @mouse: the mouse to update
 * @repeat_settings: the repeat settings (delay + interval)
 * @timestamp: the current ticks (milliseconds)
 *
 * Updates the repeat informations for a mouse, must be called
 * regularly, as often as possible.
 *
 * Return value: none.
 */
void
lw6gui_mouse_update_repeat (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, lw6gui_repeat_settings_t * repeat_settings, int64_t timestamp)
{
  lw6gui_button_update_repeat (sys_context, &(mouse->button_left), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(mouse->button_right), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(mouse->button_middle), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(mouse->wheel_up), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(mouse->wheel_down), repeat_settings, timestamp, 0);
}

/**
 * lw6gui_mouse_sync
 *
 * @sys_context: global system context
 * @dst: the target mouse object
 * @src: the source mouse object
 *
 * Synchronizes two mouse objects. This is typically used to pass data from
 * one thread to another. Will handle "mouse move" attribute and clear it
 * in src if needed while setting it in dst.
 *
 * Return value: 1 if success, O if failure.
 */
int
lw6gui_mouse_sync (lw6sys_context_t * sys_context, lw6gui_mouse_t * dst, lw6gui_mouse_t * src)
{
  int ret = 1;

  dst->moved = dst->moved || src->moved;
  src->moved = 0;		// yes, src is cleared
  dst->last_moved = src->last_moved;
  dst->screen_pointer = src->screen_pointer;
  dst->map_pointer = src->map_pointer;
  dst->screen_drag_start = src->screen_drag_start;
  dst->drag_mode = src->drag_mode;
  dst->menu_position = src->menu_position;
  dst->menu_scroll = src->menu_scroll;
  dst->menu_esc = src->menu_esc;

  ret = lw6gui_button_sync (sys_context, &(dst->button_left), &(src->button_left)) &&
    lw6gui_button_sync (sys_context, &(dst->button_right), &(src->button_right)) &&
    lw6gui_button_sync (sys_context, &(dst->button_middle), &(src->button_middle)) &&
    lw6gui_button_sync (sys_context, &(dst->wheel_up), &(src->wheel_up)) && lw6gui_button_sync (sys_context, &(dst->wheel_down), &(src->wheel_down));

  return ret;
}

/**
 * lw6gui_mouse_drag_begin
 *
 * @sys_context: global system context
 * @mouse: mouse struct to update
 *
 * To be called when one wants to start recording a drag session,
 * typically when left button is pressed.
 *
 * Return value: none.
 */
void
lw6gui_mouse_drag_begin (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse)
{
  mouse->drag_mode = LW6GUI_DRAG_MODE_ON;
  mouse->screen_drag_start = mouse->screen_pointer;
}

/**
 * lw6gui_mouse_drag_end
 *
 * @sys_context: global system context
 * @mouse: mouse struct to update
 *
 * To be called when one wants to stop recording a drag session,
 * typically when left button is released.
 *
 * Return value: none.
 */
void
lw6gui_mouse_drag_end (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse)
{
  mouse->drag_mode = LW6GUI_DRAG_MODE_DONE;
}

/**
 * lw6gui_mouse_drag_pop
 *
 * @sys_context: global system context
 * @mouse: mouse struct to query
 * @delta_x: x movement (on screen, out param can be NULL)
 * @delta_y: y movement (on screen, out param can be NULL)
 * @pos_x: x pos (on screen, out param can be NULL)
 * @pos_y: y pos (on screen, out param can be NULL)
 * @speed_x: x speed (on screen, out param can be NULL)
 * @speed_y: y speed (on screen, out param can be NULL)
 *
 * To be called when one wants to stop recording a drag session,
 * typically when left button is released.
 *
 * Return value: none.
 */
int
lw6gui_mouse_drag_pop (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, int *delta_x, int *delta_y, int *pos_x, int *pos_y, int *speed_x, int *speed_y)
{
  int ret = 0;
  int dx = 0;
  int dy = 0;
  int px = 0;
  int py = 0;
  int sx = 0;
  int sy = 0;

  px = mouse->screen_pointer.pos_x;
  py = mouse->screen_pointer.pos_y;
  if (mouse->drag_mode == LW6GUI_DRAG_MODE_ON || mouse->drag_mode == LW6GUI_DRAG_MODE_DONE)
    {
      dx = mouse->screen_pointer.pos_x - mouse->screen_drag_start.pos_x;
      dy = mouse->screen_pointer.pos_y - mouse->screen_drag_start.pos_y;
      sx = mouse->screen_pointer.speed_x;
      sy = mouse->screen_pointer.speed_y;
      if (mouse->drag_mode == LW6GUI_DRAG_MODE_DONE)
	{
	  ret = 1;
	  mouse->drag_mode = LW6GUI_DRAG_MODE_OFF;
	}
    }

  if (delta_x)
    {
      (*delta_x) = dx;
    }
  if (delta_y)
    {
      (*delta_y) = dy;
    }
  if (pos_x)
    {
      (*pos_x) = px;
    }
  if (pos_y)
    {
      (*pos_y) = py;
    }
  if (speed_x)
    {
      (*speed_x) = sx;
    }
  if (speed_y)
    {
      (*speed_y) = sy;
    }

  return ret;
}
