/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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
 * lw6gui_input_init
 *
 * @input: the input struct to initialise
 *
 * Initialises an input structure, don't use twice, it won't free a previous init.
 *
 * Return value: a pointer to the newly allocated object.
 */
int
lw6gui_input_init (lw6gui_input_t * input)
{
  int ret = 0;

  memset (input, 0, sizeof (lw6gui_input_t));

  input->keyboard.queue =
    lw6sys_list_new ((lw6sys_free_func_t) lw6gui_keypress_free);
  if (input->keyboard.queue)
    {
      ret = 1;
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("new input queue created"));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to initialize input"));
    }

  return ret;
}

/**
 * lw6gui_input_quit
 *
 * @input: the input struct to uninitialise
 *
 * Unitialises an input structure, need to call it to free event queue.
 *
 * Return value: a pointer to the newly allocated object.
 */
void
lw6gui_input_quit (lw6gui_input_t * input)
{
  if (input->keyboard.queue)
    {
      lw6sys_list_free (input->keyboard.queue);
    }
  memset (input, 0, sizeof (lw6gui_input_t));
}

/**
 * lw6gui_input_new
 *
 * Creates an input structure, which can be used to handle input state & buffer.
 *
 * Return value: a pointer to the newly allocated object.
 */
lw6gui_input_t *
lw6gui_input_new ()
{
  lw6gui_input_t *ret = NULL;

  ret = (lw6gui_input_t *) LW6SYS_CALLOC (sizeof (lw6gui_input_t));
  if (ret)
    {
      if (!lw6gui_input_init (ret))
	{
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6gui_input_free
 *
 * @input: the input object to free.
 *
 * Deletes an input structure.
 *
 * Return value: none.
 */
void
lw6gui_input_free (lw6gui_input_t * input)
{
  if (input)
    {
      lw6gui_input_quit (input);
      LW6SYS_FREE (input);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("trying to free NULL input"));
    }
}

/**
 * lw6gui_input_reset
 *
 * @input: the input struct to reset
 *
 * Resets an input structure. Must have been initialized before. It will empty
 * all queues and mark everything as unpressed.
 *
 * Return value: 1 on success, 0 if failure.
 */
int
lw6gui_input_reset (lw6gui_input_t * input)
{
  int ret = 0;

  lw6gui_input_quit (input);
  ret = lw6gui_input_init (input);

  return ret;
}

/**
 * lw6gui_input_update_repeat
 *
 * @input: the input to update
 * @repeat_settings: the repeat settings (delay + interval)
 * @timestamp: the current ticks (milliseconds)
 *
 * Updates the repeat informations for an input, must be called
 * regularly, as often as possible.
 *
 * Return value: none.
 */
void
lw6gui_input_update_repeat (lw6gui_input_t * input,
			    lw6gui_repeat_settings_t * repeat_settings,
			    int64_t timestamp)
{
  int i;

  lw6gui_keyboard_update_repeat (&(input->keyboard), repeat_settings,
				 timestamp);
  lw6gui_mouse_update_repeat (&(input->mouse), repeat_settings, timestamp);
  for (i = 0; i < LW6GUI_NB_JOYSTICKS; ++i)
    {
      lw6gui_joystick_update_repeat (&(input->joysticks[i]), repeat_settings,
				     timestamp);
    }
}

/**
 * lw6gui_input_register_change
 *
 * @input: the input to update
 *
 * Tells an input object that one of its descendants has been
 * modified. This will affect the return value of @lw6gui_input_need_sync
 *
 * Return value: none.
 */
void
lw6gui_input_register_change (lw6gui_input_t * input)
{
  input->need_sync = 1;
}

/**
 * lw6gui_input_need_sync
 *
 * @input: the input to test
 *
 * Tests wether an input object contains was modified and
 * needs synchronisation.
 *
 * Return value: 1 if sync is need, 0 if not.
 */
int
lw6gui_input_need_sync (lw6gui_input_t * input)
{
  int ret = 0;

  ret = input->need_sync;

  return ret;
}

/**
 * lw6gui_input_sync
 *
 * @dst: the target input object
 * @src: the source input object
 *
 * Synchronizes two input objects. This is typically used to pass data from
 * one thread to another. This is not a copy, it will brute-force copy the
 * static data such as mouse position, but anything like a queue will be
 * treated in a "empty source and fill target with data" scheme. So source
 * will be affected by this, the key buffer will be emptied, and so on.
 * And if there are key in the target buffer, they won't be overwritten
 * but kept in front of the FIFO list.
 *
 * Return value: 1 if success, O if failure.
 */
int
lw6gui_input_sync (lw6gui_input_t * dst, lw6gui_input_t * src)
{
  int ret = 1;
  int i = 0;

  dst->need_sync = src->need_sync;
  src->need_sync = 0;		// we've been synced, need to reset the flag
  ret = lw6gui_keyboard_sync (&(dst->keyboard), &(src->keyboard)) && ret;
  ret = lw6gui_mouse_sync (&(dst->mouse), &(src->mouse)) && ret;
  for (i = 0; i < LW6GUI_NB_JOYSTICKS; ++i)
    {
      ret = lw6gui_joystick_sync (&(dst->joysticks[i]), &(src->joysticks[i]))
	&& ret;
    }

  return ret;
}
