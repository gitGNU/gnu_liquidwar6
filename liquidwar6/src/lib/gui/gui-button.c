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
 * lw6gui_button_register_down
 *
 * @sys_context: global system context
 * @button: the button to update
 * @timestamp: the current ticks (milliseconds)
 *
 * Registers a "down" (press) event on a button.
 *
 * Return value: none.
 */
void
lw6gui_button_register_down (lw6sys_context_t * sys_context, lw6gui_button_t * button, int64_t timestamp)
{
  if (timestamp <= 0LL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("using negative or zero timestamp (%" LW6SYS_PRINTF_LL "d)"), (long long) timestamp);
    }

  button->is_pressed = 1;
  button->press_queue++;
  button->last_press = timestamp;
  button->last_repeat = 0;

  if (button->double_click_t1 <= 0 || button->double_click_t2 <= 0 || button->double_click_t3 <= 0)
    {
      button->double_click_t1 = button->double_click_t2;
      button->double_click_t2 = button->double_click_t3;
      button->double_click_t3 = timestamp;
    }
}

/**
 * lw6gui_button_register_up
 *
 * @sys_context: global system context
 * @button: the button to update
 *
 * Registers a "up" (release) event on a button.
 *
 * Return value: none.
 */
void
lw6gui_button_register_up (lw6sys_context_t * sys_context, lw6gui_button_t * button)
{
  button->is_pressed = 0;
  button->last_press = 0;
  button->last_repeat = 0;
}

/**
 * lw6gui_button_is_pressed
 *
 * @sys_context: global system context
 * @button: the button to query
 *
 * Tells wether a button is pressed or not.
 *
 * Return value: 1 if pressed, 0 if not.
 */
int
lw6gui_button_is_pressed (lw6sys_context_t * sys_context, const lw6gui_button_t * button)
{
  int ret = 0;

  ret = button->is_pressed;

  return ret;
}

/**
 * lw6gui_button_pop_press
 *
 * @sys_context: global system context
 * @button: the button to query
 *
 * Tells how many times the button has been pressed. Typical usage: the
 * button is pressed, released, pressed, released several times. Then,
 * after all this, you want to know how many times it has been pressed.
 * Querying its state with @lw6gui_button_is_pressed won't tell you much
 * but this @pop_press function will return 1 for each press there's been.
 *
 * Return value: 1 if there's a press event in the queue, 0 if empty.
 */
int
lw6gui_button_pop_press (lw6sys_context_t * sys_context, lw6gui_button_t * button)
{
  int ret = 0;

  if (button->press_queue > 0)
    {
      ret = 1;
      button->press_queue--;
    }

  return ret;
}

/**
 * lw6gui_button_pop_simple_click
 *
 * @sys_context: global system context
 * @button: the button to query
 *
 * Tells how many times the button has been simpleclicked. This is different
 * from a simple press, in fact, there's a delay, we must wait until the
 * double-click delay is elapsed to make sure this is a simple click...
 * Designed for use with mouse to differentiate fire and alternate fire.
 *
 * Return value: 1 if there's a simpleclick event in the queue, 0 if empty.
 */
int
lw6gui_button_pop_simple_click (lw6sys_context_t * sys_context, lw6gui_button_t * button)
{
  int ret = 0;

  if (button->simple_click_queue > 0)
    {
      ret = 1;
      button->simple_click_queue--;
    }

  return ret;
}

/**
 * lw6gui_button_pop_double_click
 *
 * @sys_context: global system context
 * @button: the button to query
 *
 * Tells how many times the button has been doubleclicked. Typical usage: the
 * button is doubleclicked, released, doubleclicked, released several times. Then,
 * after all this, you want to know how many times it has been doubleclicked.
 *
 * Return value: 1 if there's a doubleclick event in the queue, 0 if empty.
 */
int
lw6gui_button_pop_double_click (lw6sys_context_t * sys_context, lw6gui_button_t * button)
{
  int ret = 0;

  if (button->double_click_queue > 0)
    {
      ret = 1;
      button->double_click_queue--;
    }

  return ret;
}

/**
 * lw6gui_button_pop_triple_click
 *
 * @sys_context: global system context
 * @button: the button to query
 *
 * Tells how many times the button has been tripleclicked. Typical usage: the
 * button is tripleclicked, released, tripleclicked, released several times. Then,
 * after all this, you want to know how many times it has been tripleclicked.
 *
 * Return value: 1 if there's a tripleclick event in the queue, 0 if empty.
 */
int
lw6gui_button_pop_triple_click (lw6sys_context_t * sys_context, lw6gui_button_t * button)
{
  int ret = 0;

  if (button->triple_click_queue > 0)
    {
      ret = 1;
      button->triple_click_queue--;
    }

  return ret;
}

/**
 * lw6gui_button_update_repeat
 *
 * @sys_context: global system context
 * @button: the button to update
 * @repeat_settings: the repeat settings
 * @timestamp: the current ticks (milliseconds)
 # @auto_release_enabled: wether to use the auto_release feature
 *
 * Updates the repeat informations for a button, must be called
 * regularly, as often as possible.
 *
 * Return value: none.
 */
void
lw6gui_button_update_repeat (lw6sys_context_t * sys_context, lw6gui_button_t * button, lw6gui_repeat_settings_t * repeat_settings, int64_t timestamp,
			     int auto_release_enabled)
{
  if (auto_release_enabled && button->is_pressed && button->last_press <= timestamp - repeat_settings->auto_release_delay)
    {
      /*
       * Before anything else, register a fake button_up if we're
       * in auto_release mode and it's relevant.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("button auto release button->last_press=%"
		       LW6SYS_PRINTF_LL "d timestamp=%" LW6SYS_PRINTF_LL "d"), (long long) button->last_press, (long long) timestamp);
      lw6gui_button_register_up (sys_context, button);
    }

  if (button->is_pressed)
    {
      if ((repeat_settings->delay > 0 && button->last_repeat == 0
	   && button->last_press <= timestamp - repeat_settings->delay)
	  || (repeat_settings->interval > 0 && button->last_repeat != 0 && button->last_repeat <= timestamp - repeat_settings->interval))
	{
	  (button->press_queue)++;
	  button->last_repeat = timestamp;
	}
    }

  if (button->double_click_t1 > 0 && button->double_click_t2 > 0
      && button->double_click_t3 > 0
      && (button->double_click_t3 - button->double_click_t2) <
      repeat_settings->double_click_delay && (button->double_click_t2 - button->double_click_t1) < repeat_settings->double_click_delay)
    {
      /*
       * We don't "++" here, no buffering of triple clicks
       */
      button->triple_click_queue = 1;
      button->double_click_t1 = 0;
      button->double_click_t2 = 0;
      button->double_click_t3 = 0;
    }
  if (button->double_click_t2 > 0 && button->double_click_t3 > 0
      && (button->double_click_t3 - button->double_click_t2) <
      repeat_settings->double_click_delay && timestamp - button->double_click_t3 >= repeat_settings->double_click_delay)
    {
      /*
       * We don't "++" here, no buffering of double clicks
       */
      button->double_click_queue = 1;
      button->double_click_t1 = 0;
      button->double_click_t2 = 0;
      button->double_click_t3 = 0;
    }
  if (button->double_click_t3 > 0 && timestamp - button->double_click_t3 >= repeat_settings->double_click_delay)
    {
      /*
       * We don't "++" here, no buffering of simple clicks
       */
      button->simple_click_queue = 1;
      button->double_click_t1 = 0;
      button->double_click_t2 = 0;
      button->double_click_t3 = 0;
    }
}

/**
 * lw6gui_button_sync
 *
 * @sys_context: global system context
 * @dst: the target button object
 * @src: the source button object
 *
 * Synchronizes two button objects. This is typically used to pass data from
 * one thread to another. This is not a simple copy, it will handle data
 * such as "when was it pressed last" it an intelligent manner, popping src
 * data to put it in dst, and clearing src.
 *
 * Return value: 1 if success, O if failure.
 */
int
lw6gui_button_sync (lw6sys_context_t * sys_context, lw6gui_button_t * dst, lw6gui_button_t * src)
{
  int ret = 1;

  dst->press_queue += src->press_queue;
  src->press_queue = 0;		// clear src queue, it's been transfered to dst
  dst->simple_click_queue |= src->simple_click_queue;
  src->simple_click_queue = 0;	// clear src queue, it's been transfered to dst
  dst->double_click_queue |= src->double_click_queue;
  src->double_click_queue = 0;	// clear src queue, it's been transfered to dst
  dst->triple_click_queue |= src->triple_click_queue;
  src->triple_click_queue = 0;	// clear src queue, it's been transfered to dst
  dst->is_pressed = src->is_pressed;
  dst->last_press = src->last_press;
  dst->last_repeat = src->last_repeat;
  dst->double_click_t1 = src->double_click_t1;
  dst->double_click_t2 = src->double_click_t2;
  dst->double_click_t3 = src->double_click_t3;

  return ret;
}
