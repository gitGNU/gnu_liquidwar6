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
#endif // HAVE_CONFIG_H

#include <errno.h>

#include "mod-caca-internal.h"

static void
key_down (lw6gui_keyboard_t * keyboard, caca_event_t* event,
	  _mod_caca_const_data_t * const_data, int64_t timestamp)
{
  int sym = 0;

  sym = caca_get_event_key_ch(event);

  lw6gui_keyboard_register_key_down (keyboard, sym,
  				     caca_get_event_key_utf32(event),
  				     NULL, timestamp);

  if (sym == const_data->keysym1_up ||
      sym == const_data->keysym2_up ||
      sym == const_data->keysym3_up || sym == const_data->keysym4_up)
    {
      lw6gui_button_register_down (&(keyboard->arrow_up), timestamp);
    }
  if (sym == const_data->keysym1_down ||
      sym == const_data->keysym2_down ||
      sym == const_data->keysym3_down || sym == const_data->keysym4_down)
    {
      lw6gui_button_register_down (&(keyboard->arrow_down), timestamp);
    }
  if (sym == const_data->keysym1_left ||
      sym == const_data->keysym2_left ||
      sym == const_data->keysym3_left || sym == const_data->keysym4_left)
    {
      lw6gui_button_register_down (&(keyboard->arrow_left), timestamp);
    }
  if (sym == const_data->keysym1_right ||
      sym == const_data->keysym2_right ||
      sym == const_data->keysym3_right || sym == const_data->keysym4_right)
    {
      lw6gui_button_register_down (&(keyboard->arrow_right), timestamp);
    }
  if (sym == const_data->keysym1_enter || sym == const_data->keysym2_enter)
    {
      lw6gui_button_register_down (&(keyboard->key_enter), timestamp);
    }
  if (sym == const_data->keysym1_esc || sym == const_data->keysym2_esc)
    {
      lw6gui_button_register_down (&(keyboard->key_esc), timestamp);
    }
  if (sym == const_data->keysym1_ctrl || sym == const_data->keysym2_ctrl)
    {
      lw6gui_button_register_down (&(keyboard->key_ctrl), timestamp);
    }
  if (sym == const_data->keysym1_alt || sym == const_data->keysym2_alt)
    {
      lw6gui_button_register_down (&(keyboard->key_alt), timestamp);
    }
  if (sym == const_data->keysym1_pgup || sym == const_data->keysym2_pgup)
    {
      lw6gui_button_register_down (&(keyboard->key_pgup), timestamp);
    }
  if (sym == const_data->keysym1_pgdown || sym == const_data->keysym2_pgdown)
    {
      lw6gui_button_register_down (&(keyboard->key_pgdown), timestamp);
    }
}

static void
key_up (lw6gui_keyboard_t * keyboard, caca_event_t * event,
	_mod_caca_const_data_t* const_data)
{
  int sym = 0;

  sym = caca_get_event_key_ch(event);

  lw6gui_keyboard_register_key_up (keyboard, sym);

  if (sym == const_data->keysym1_up ||
      sym == const_data->keysym2_up ||
      sym == const_data->keysym3_up || sym == const_data->keysym4_up)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_up)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_up)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym3_up)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym4_up))
	{
	  lw6gui_button_register_up (&(keyboard->arrow_up));
	}
    }
  if (sym == const_data->keysym1_down ||
      sym == const_data->keysym2_down ||
      sym == const_data->keysym3_down || sym == const_data->keysym4_down)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_down)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_down)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym3_down)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym4_down))
	{
	  lw6gui_button_register_up (&(keyboard->arrow_down));
	}
    }
  if (sym == const_data->keysym1_left ||
      sym == const_data->keysym2_left ||
      sym == const_data->keysym3_left || sym == const_data->keysym4_left)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_left)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_left)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym3_left)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym4_left))
	{
	  lw6gui_button_register_up (&(keyboard->arrow_left));
	}
    }
  if (sym == const_data->keysym1_right ||
      sym == const_data->keysym2_right ||
      sym == const_data->keysym3_right || sym == const_data->keysym4_right)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_right)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_right)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym3_right)
	  && !lw6gui_keyboard_is_pressed (keyboard,
					  const_data->keysym4_right))
	{
	  lw6gui_button_register_up (&(keyboard->arrow_right));
	}
    }
  if (sym == const_data->keysym1_enter || sym == const_data->keysym2_enter)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_enter)
	  && !lw6gui_keyboard_is_pressed (keyboard,
					  const_data->keysym2_enter))
	{
	  lw6gui_button_register_up (&(keyboard->key_enter));
	}
    }
  if (sym == const_data->keysym1_esc || sym == const_data->keysym2_esc)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_esc)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_esc))
	{
	  lw6gui_button_register_up (&(keyboard->key_esc));
	}
    }
  if (sym == const_data->keysym1_ctrl || sym == const_data->keysym2_ctrl)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_ctrl)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_ctrl))
	{
	  lw6gui_button_register_up (&(keyboard->key_ctrl));
	}
    }
  if (sym == const_data->keysym1_alt || sym == const_data->keysym2_alt)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_alt)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_alt))
	{
	  lw6gui_button_register_up (&(keyboard->key_alt));
	}
    }
  if (sym == const_data->keysym1_pgup || sym == const_data->keysym2_pgup)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_pgup)
	  && !lw6gui_keyboard_is_pressed (keyboard, const_data->keysym2_pgup))
	{
	  lw6gui_button_register_up (&(keyboard->key_pgup));
	}
    }
  if (sym == const_data->keysym1_pgdown || sym == const_data->keysym2_pgdown)
    {
      if (!lw6gui_keyboard_is_pressed (keyboard, const_data->keysym1_pgdown)
	  && !lw6gui_keyboard_is_pressed (keyboard,
					  const_data->keysym2_pgdown))
	{
	  lw6gui_button_register_up (&(keyboard->key_pgdown));
	}
    }
}

lw6gui_input_t *
_mod_caca_pump_events (_mod_caca_context_t * caca_context)
{
  lw6gui_input_t *input = &(caca_context->input);
  caca_event_t event;
  _mod_caca_const_data_t *const_data = &(caca_context->const_data);
  int64_t timestamp = 0LL;

  if (caca_context)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_ ("pumping libcaca events timeout %d microseconds"),
		  caca_context->const_data.event_timeout_microseconds);

      memset(&event, 0, sizeof(event));
      // todo : pump them for good!

      while (caca_get_event(caca_context->display,
			    CACA_EVENT_KEY_PRESS | CACA_EVENT_KEY_RELEASE,
			    &event, caca_context->const_data.event_timeout_microseconds))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("libcaca event type=%s ascii=%d\n"),
			   (caca_get_event_type(&event) == CACA_EVENT_KEY_PRESS ? "CACA_EVENT_KEY_PRESS" : "CACA_EVENT_KEY_RELEASE"),
			   caca_get_event_key_ch(&event)
		      );
	  switch (caca_get_event_type(&event))
	    {
	    case CACA_EVENT_KEY_PRESS:
	      lw6gui_input_register_change (input);
	      key_down (&(input->keyboard), &event, const_data, timestamp);
	      if (caca_get_event_key_ch(&event) == const_data->keysym_quit)
		{
		  lw6sys_signal_send_quit ();
		}
	      break ;
	    case CACA_EVENT_KEY_RELEASE:
	      lw6gui_input_register_change (input);
	      key_up (&(input->keyboard), &event, const_data);
	      break ;
	    default:
	      break ;
	    }
	}
    }

  return input;
}
