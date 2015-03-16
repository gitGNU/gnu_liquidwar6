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
 * lw6gui_joystick_check_index
 *
 * @sys_context: global system context
 * @i: index to check
 *
 * Checks wether the index is correct. Does not mean the joystick exists, it's
 * just to avoid out of bounds errors.
 *
 * Return value: 1 if within range, 0 if not.
 */
int
lw6gui_joystick_check_index (lw6sys_context_t * sys_context, int i)
{
  int ret = 0;

  if (i >= 0 && i < LW6GUI_NB_JOYSTICKS)
    {
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("joystick index %d out of range"), i);
    }

  return ret;
}

/**
 * lw6gui_joystick_update_axis_x
 *
 * @sys_context: global system context
 * @joystick: joystick to update
 * @x: x-axis position, as returned by the driver
 * @limit: the limit, under this, buttons are considered unpressed.
 * @timestamp: current ticks (timestamp in ms)
 *
 * Updates the x axis of a joystick, this will convert an information of analog
 * type such as "joystick is here" to a pad-like information such as "pressed
 * in that direction".
 *
 * Return value: 1 if within range, 0 if not.
 */
void
lw6gui_joystick_update_axis_x (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, int x, int limit, int64_t timestamp)
{
  if (x < -limit)
    {
      if (!lw6gui_button_is_pressed (sys_context, &(joystick->pad_left)))
	{
	  lw6gui_button_register_down (sys_context, &(joystick->pad_left), timestamp);
	}
    }
  else
    {
      lw6gui_button_register_up (sys_context, &(joystick->pad_left));
    }

  if (x > limit)
    {
      if (!lw6gui_button_is_pressed (sys_context, &(joystick->pad_right)))
	{
	  lw6gui_button_register_down (sys_context, &(joystick->pad_right), timestamp);
	}
    }
  else
    {
      lw6gui_button_register_up (sys_context, &(joystick->pad_right));
    }
}

/**
 * lw6gui_joystick_update_axis_x
 *
 * @sys_context: global system context
 * @joystick: joystick to update
 * @x: y-axis position, as returned by the driver
 * @limit: the limit, under this, buttons are considered unpressed.
 * @timestamp: current ticks (timestamp in ms)
 *
 * Updates the y axis of a joystick, this will convert an information of analog
 * type such as "joystick is here" to a pad-like information such as "pressed
 * in that direction".
 *
 * Return value: 1 if within range, 0 if not.
 */
void
lw6gui_joystick_update_axis_y (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, int y, int limit, int64_t timestamp)
{
  if (y < -limit)
    {
      if (!lw6gui_button_is_pressed (sys_context, &(joystick->pad_up)))
	{
	  lw6gui_button_register_down (sys_context, &(joystick->pad_up), timestamp);
	}
    }
  else
    {
      lw6gui_button_register_up (sys_context, &(joystick->pad_up));
    }

  if (y > limit)
    {
      if (!lw6gui_button_is_pressed (sys_context, &(joystick->pad_down)))
	{
	  lw6gui_button_register_down (sys_context, &(joystick->pad_down), timestamp);
	}
    }
  else
    {
      lw6gui_button_register_up (sys_context, &(joystick->pad_down));
    }
}

/**
 * lw6gui_joystick_update_repeat
 *
 * @sys_context: global system context
 * @joystick: the joystick to update
 * @repeat_settings: the repeat settings (delay + interval)
 * @timestamp: the current ticks (milliseconds)
 *
 * Updates the repeat informations for a joystick, must be called
 * regularly, as often as possible.
 *
 * Return value: none.
 */
void
lw6gui_joystick_update_repeat (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, lw6gui_repeat_settings_t * repeat_settings, int64_t timestamp)
{
  lw6gui_button_update_repeat (sys_context, &(joystick->pad_up), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->pad_down), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->pad_left), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->pad_right), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->button_a), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->button_b), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->button_c), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->button_d), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->button_e), repeat_settings, timestamp, 0);
  lw6gui_button_update_repeat (sys_context, &(joystick->button_f), repeat_settings, timestamp, 0);
}

/**
 * lw6gui_joystick_sync
 *
 * @sys_context: global system context
 * @dst: the target joystick object
 * @src: the source joystick object
 *
 * Synchronizes two joystick objects. This is typically used to pass data from
 * one thread to another.
 *
 * Return value: 1 if success, O if failure.
 */
int
lw6gui_joystick_sync (lw6sys_context_t * sys_context, lw6gui_joystick_t * dst, lw6gui_joystick_t * src)
{
  int ret = 1;

  ret = lw6gui_button_sync (sys_context, &(dst->pad_up), &(src->pad_up)) &&
    lw6gui_button_sync (sys_context, &(dst->pad_down), &(src->pad_down)) &&
    lw6gui_button_sync (sys_context, &(dst->pad_left), &(src->pad_left)) &&
    lw6gui_button_sync (sys_context, &(dst->pad_right), &(src->pad_right)) &&
    lw6gui_button_sync (sys_context, &(dst->button_a), &(src->button_a)) &&
    lw6gui_button_sync (sys_context, &(dst->button_b), &(src->button_b)) &&
    lw6gui_button_sync (sys_context, &(dst->button_c), &(src->button_c)) &&
    lw6gui_button_sync (sys_context, &(dst->button_d), &(src->button_d)) &&
    lw6gui_button_sync (sys_context, &(dst->button_e), &(src->button_e)) && lw6gui_button_sync (sys_context, &(dst->button_f), &(src->button_f));

  return ret;
}

/**
 * lw6gui_joystick_get_move_pad
 *
 * @sys_context: global system context
 * @joystick: the joystick to query
 * @move_pad: the structure which will contain the results
 *
 * Returns the state of the joystick in a uniform, non-device specific structure
 * containing only the up/down/left/right information.
 *
 * Return value: none, the value are stored in @move_pad.
 */
void
lw6gui_joystick_get_move_pad (lw6sys_context_t * sys_context, const lw6gui_joystick_t * joystick, lw6gui_move_pad_t * move_pad)
{
  move_pad->up = lw6gui_button_is_pressed (sys_context, &(joystick->pad_up));
  move_pad->down = lw6gui_button_is_pressed (sys_context, &(joystick->pad_down));
  move_pad->left = lw6gui_button_is_pressed (sys_context, &(joystick->pad_left));
  move_pad->right = lw6gui_button_is_pressed (sys_context, &(joystick->pad_right));
}
