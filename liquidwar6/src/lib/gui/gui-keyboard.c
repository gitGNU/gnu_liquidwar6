/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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
 * lw6gui_keyboard_check_keysym
 *
 * @keysym: the keysym to check
 *
 * Tells wether the keysym is valid or not.
 *
 * Return value: 1 if valid, 0 if not
 */
int
lw6gui_keyboard_check_keysym (int keysym)
{
  int ret = 0;

  if (keysym >= 0 && keysym < LW6GUI_NB_KEYS)
    {
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("keysym %d out of range"), keysym);
    }

  return ret;
}

/**
 * lw6gui_keyboard_pop_keypress
 *
 * @keyboard: the keyboard structure which stores keyboard state
 *
 * Pops (in FIFO mode) a keypress stored in the keyboard buffer. You must
 * free the obtained keypress object after you're done with it.
 *
 * Return value: a newly allocated pointer, or NULL if no keypress pending.
 */
lw6gui_keypress_t *
lw6gui_keyboard_pop_keypress (lw6gui_keyboard_t * keyboard)
{
  lw6gui_keypress_t *ret = NULL;

  if (!lw6sys_list_is_empty (keyboard->queue))
    {
      ret = (lw6gui_keypress_t *) lw6sys_list_pop_front (&(keyboard->queue));
    }

  return ret;
}

/**
 * lw6gui_keyboard_is_pressed
 *
 * @keyboard: the keyboard structure which stores keyboard state
 *
 * Tells wether a key is pressed or not. The function will test out of
 * bound values.
 *
 * Return value: 1 if pressed, 0 if not.
 */
int
lw6gui_keyboard_is_pressed (lw6gui_keyboard_t * keyboard, int keysym)
{
  int ret = 0;

  if (lw6gui_keyboard_check_keysym (keysym))
    {
      ret = lw6gui_button_is_pressed (&(keyboard->keys_state[keysym]));
    }

  return ret;
}

/**
 * lw6gui_keyboard_register_key_down
 *
 * @keyboard: the keyboard structure which will store the keypress
 * @keysym: the keysym for the keypress
 * @unicode: the ASCII/unicode code for the keypress
 * @label: the label for the keypress
 * @timestamp: the current ticks (timestamp in ms)
 *
 * Registers a keypress event, that is, puts it in the event queue. This
 * function does not take an @lw6gui_keypress_t structure but separated
 * args, this is because it will construct the object internally. You
 * may free @label after calling this, an internal copy will be done.
 * This function will also maintain the array of key states up to date.
 *
 * Return value: 1 if success, O if failure.
 */
int
lw6gui_keyboard_register_key_down (lw6gui_keyboard_t * keyboard, int keysym,
				   int unicode, char *label,
				   int64_t timestamp)
{
  int ret = 0;
  lw6gui_keypress_t *keypress;

  if (lw6gui_keyboard_check_keysym (keysym))
    {
      lw6gui_button_register_down (&(keyboard->keys_state[keysym]),
				   timestamp);
      keypress = lw6gui_keypress_new (keysym, unicode, label);
      if (keypress)
	{
	  lw6sys_list_push_back (&(keyboard->queue), (void *) keypress);
	  ret = 1;
	}
    }

  return ret;
}

/**
 * lw6gui_keyboard_register_key_up
 *
 * @keyboard: the keyboard structure which will store the keypress
 * @keysym: the keysym for the keypress
 *
 * Registers a key release event.
 *
 * Return value: 1 if success, O if failure.
 */
int
lw6gui_keyboard_register_key_up (lw6gui_keyboard_t * keyboard, int keysym)
{
  int ret = 0;

  if (lw6gui_keyboard_check_keysym (keysym))
    {
      lw6gui_button_register_up (&(keyboard->keys_state[keysym]));
      ret = 1;
    }

  return ret;
}

/**
 * lw6gui_keyboard_update_repeat
 *
 * @keyboard: the keyboard to update
 * @repeat_settings: the repeat settings (delay + interval)
 * @timestamp: the current ticks (milliseconds)
 *
 * Updates the repeat informations for a keyboard, must be called
 * regularly, as often as possible.
 *
 * Return value: none.
 */
void
lw6gui_keyboard_update_repeat (lw6gui_keyboard_t * keyboard,
			       lw6gui_repeat_settings_t * repeat_settings,
			       int64_t timestamp)
{
  int i = 0;

  lw6gui_button_update_repeat (&(keyboard->arrow_up), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->arrow_down), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->arrow_left), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->arrow_right), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->key_enter), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->key_esc), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->key_ctrl), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->key_alt), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->key_pgup), repeat_settings,
			       timestamp);
  lw6gui_button_update_repeat (&(keyboard->key_pgdown), repeat_settings,
			       timestamp);

  for (i = 0; i < LW6GUI_NB_KEYS; ++i)
    {
      lw6gui_button_update_repeat (&(keyboard->keys_state[i]),
				   repeat_settings, timestamp);
    }
}

/**
 * lw6gui_keyboard_sync
 *
 * @dst: the target keyboard object
 * @src: the source keyboard object
 *
 * Synchronizes two keyboard objects. This is typically used to pass data from
 * one thread to another. Will pop the src queue to fill the dst queue.
 *
 * Return value: 1 if success, O if failure.
 */
int
lw6gui_keyboard_sync (lw6gui_keyboard_t * dst, lw6gui_keyboard_t * src)
{
  int ret = 1;
  int i;
  lw6gui_keypress_t *keypress;

  ret = lw6gui_button_sync (&(dst->arrow_up), &(src->arrow_up)) &&
    lw6gui_button_sync (&(dst->arrow_down), &(src->arrow_down)) &&
    lw6gui_button_sync (&(dst->arrow_left), &(src->arrow_left)) &&
    lw6gui_button_sync (&(dst->arrow_right), &(src->arrow_right)) &&
    lw6gui_button_sync (&(dst->key_enter), &(src->key_enter)) &&
    lw6gui_button_sync (&(dst->key_esc), &(src->key_esc)) &&
    lw6gui_button_sync (&(dst->key_ctrl), &(src->key_ctrl)) &&
    lw6gui_button_sync (&(dst->key_alt), &(src->key_alt)) &&
    lw6gui_button_sync (&(dst->key_pgup), &(src->key_pgup)) &&
    lw6gui_button_sync (&(dst->key_pgdown), &(src->key_pgdown));

  /*
   * We use raw list functions to pop/push objects to avoid
   * re-construction the object
   */
  while (!lw6sys_list_is_empty (src->queue))
    {
      keypress = (lw6gui_keypress_t *) lw6sys_list_pop_front (&(src->queue));
      if (keypress)
	{
	  lw6sys_list_push_back (&(dst->queue), (void *) keypress);
	}
    }

  for (i = 0; i < LW6GUI_NB_KEYS; ++i)
    {
      ret = lw6gui_button_sync (&(dst->keys_state[i]), &(src->keys_state[i]))
	&& ret;
    }

  return ret;
}

/**
 * lw6gui_keyboard_get_move_pad
 *
 * @keyboard: the keyboard to query
 * @move_pad: the structure which will contain the results
 *
 * Returns the state of the keyboard in a uniform, non-device specific structure
 * containing only the up/down/left/right information.
 *
 * Return value: none, the value are stored in @move_pad.
 */
void
lw6gui_keyboard_get_move_pad (lw6gui_keyboard_t * keyboard,
			      lw6gui_move_pad_t * move_pad)
{
  move_pad->up = lw6gui_button_is_pressed (&(keyboard->arrow_up));
  move_pad->down = lw6gui_button_is_pressed (&(keyboard->arrow_down));
  move_pad->left = lw6gui_button_is_pressed (&(keyboard->arrow_left));
  move_pad->right = lw6gui_button_is_pressed (&(keyboard->arrow_right));
}
