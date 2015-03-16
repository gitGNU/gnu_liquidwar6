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

#include "shared-sdl-internal.h"

static void
_log_event (lw6sys_context_t * sys_context, SDL_Event * event)
{
  switch (event->type)
    {
    case SDL_ACTIVEEVENT:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL event type=SDL_ACTIVEEVENT gain=%d state=%d"), (int) event->active.gain, (int) event->active.state);
      break;
    case SDL_KEYDOWN:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_KEYDOWN state=%d scancode=%d sym=%d mod=%d unicode=%d"),
		  (int) event->key.state,
		  (int) event->key.keysym.scancode, (int) event->key.keysym.sym, (int) event->key.keysym.mod, (int) event->key.keysym.unicode);
      break;
    case SDL_KEYUP:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_KEYUP state=%d scancode=%d sym=%d mod=%d unicode=%d"),
		  (int) event->key.state,
		  (int) event->key.keysym.scancode, (int) event->key.keysym.sym, (int) event->key.keysym.mod, (int) event->key.keysym.unicode);
      break;
    case SDL_MOUSEMOTION:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_MOUSEMOTION state=%d x=%d y=%d xrel=%d yrel=%d"),
		  (int) event->motion.state, (int) event->motion.x, (int) event->motion.y, (int) event->motion.xrel, (int) event->motion.yrel);
      break;
    case SDL_MOUSEBUTTONDOWN:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_MOUSEBUTTONDOWN button=%d state=%d x=%d y=%d"),
		  (int) event->button.button, (int) event->button.state, (int) event->motion.x, (int) event->motion.y);
      break;
    case SDL_MOUSEBUTTONUP:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_MOUSEBUTTONUP button=%d state=%d x=%d y=%d"),
		  (int) event->button.button, (int) event->button.state, (int) event->motion.x, (int) event->motion.y);
      break;
    case SDL_JOYAXISMOTION:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("SDL event type=SDL_JOYAXISMOTION which=%d axis=%d value=%d"), (int) event->jaxis.which, event->jaxis.axis, (int) event->jaxis.value);
      break;
    case SDL_JOYBALLMOTION:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_JOYBALLMOTION which=%d ball=%d xrel=%d yrel=%d"),
		  (int) event->jball.which, (int) event->jball.ball, (int) event->jball.xrel, (int) event->jball.yrel);
      break;
    case SDL_JOYHATMOTION:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("SDL event type=SDL_JOYHATMOTION which=%d hat=%d value=%d"), (int) event->jhat.which, (int) event->jhat.hat, (int) event->jhat.value);
      break;
    case SDL_JOYBUTTONDOWN:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_JOYBUTTONDOWN which=%d button=%d state=%d"),
		  (int) event->jbutton.which, (int) event->jbutton.button, (int) event->jbutton.state);
      break;
    case SDL_JOYBUTTONUP:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("SDL event type=SDL_JOYBUTTONUP which=%d button=%d state=%d"),
		  (int) event->jbutton.which, (int) event->jbutton.button, (int) event->jbutton.state);
      break;
    case SDL_QUIT:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL event type=SDL_QUIT"));
      break;
    case SDL_SYSWMEVENT:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL event type=SDL_SYSWMEVENT"));
      break;
    case SDL_VIDEORESIZE:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL event type=SDL_VIDEORESIZE w=%d h=%d"), (int) event->resize.w, (int) event->resize.h);
      break;
    case SDL_VIDEOEXPOSE:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL event type=SDL_VIDEOEXPOSE"));
      break;
    case SDL_USEREVENT:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("SDL event type=SDL_USEREVENT code=%d"), (int) event->user.code);
      break;
    default:
// should never get there, unless API changes...
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("SDL event with unknown type=%d"), event->type);
    }
}

static void
_key_down (lw6sys_context_t * sys_context, lw6gui_keyboard_t * keyboard, SDL_Event * event, _lw6gfx_sdl_const_data_t * const_data, int64_t timestamp)
{
  int sym = 0;

  sym = event->key.keysym.sym;

  lw6gui_keyboard_register_key_down (sys_context, keyboard, sym, event->key.keysym.unicode, SDL_GetKeyName (sym), timestamp);

  if (sym == const_data->keysym1_up || sym == const_data->keysym2_up || sym == const_data->keysym3_up || sym == const_data->keysym4_up)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->arrow_up), timestamp);
    }
  if (sym == const_data->keysym1_down || sym == const_data->keysym2_down || sym == const_data->keysym3_down || sym == const_data->keysym4_down)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->arrow_down), timestamp);
    }
  if (sym == const_data->keysym1_left || sym == const_data->keysym2_left || sym == const_data->keysym3_left || sym == const_data->keysym4_left)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->arrow_left), timestamp);
    }
  if (sym == const_data->keysym1_right || sym == const_data->keysym2_right || sym == const_data->keysym3_right || sym == const_data->keysym4_right)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->arrow_right), timestamp);
    }
  if (sym == const_data->keysym1_enter || sym == const_data->keysym2_enter)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->key_enter), timestamp);
    }
  if (sym == const_data->keysym1_esc || sym == const_data->keysym2_esc)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->key_esc), timestamp);
    }
  if (sym == const_data->keysym1_ctrl || sym == const_data->keysym2_ctrl)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->key_ctrl), timestamp);
    }
  if (sym == const_data->keysym1_alt || sym == const_data->keysym2_alt)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->key_alt), timestamp);
    }
  if (sym == const_data->keysym1_pgup || sym == const_data->keysym2_pgup)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->key_pgup), timestamp);
    }
  if (sym == const_data->keysym1_pgdown || sym == const_data->keysym2_pgdown)
    {
      lw6gui_button_register_down (sys_context, &(keyboard->key_pgdown), timestamp);
    }
}

static void
_key_up (lw6sys_context_t * sys_context, lw6gui_keyboard_t * keyboard, SDL_Event * event, _lw6gfx_sdl_const_data_t * const_data)
{
  int sym = 0;

  sym = event->key.keysym.sym;

  lw6gui_keyboard_register_key_up (sys_context, keyboard, sym);

  if (sym == const_data->keysym1_up || sym == const_data->keysym2_up || sym == const_data->keysym3_up || sym == const_data->keysym4_up)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_up)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_up)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym3_up)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym4_up))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->arrow_up));
	}
    }
  if (sym == const_data->keysym1_down || sym == const_data->keysym2_down || sym == const_data->keysym3_down || sym == const_data->keysym4_down)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_down)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_down)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym3_down)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym4_down))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->arrow_down));
	}
    }
  if (sym == const_data->keysym1_left || sym == const_data->keysym2_left || sym == const_data->keysym3_left || sym == const_data->keysym4_left)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_left)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_left)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym3_left)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym4_left))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->arrow_left));
	}
    }
  if (sym == const_data->keysym1_right || sym == const_data->keysym2_right || sym == const_data->keysym3_right || sym == const_data->keysym4_right)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_right)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_right)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym3_right)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym4_right))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->arrow_right));
	}
    }
  if (sym == const_data->keysym1_enter || sym == const_data->keysym2_enter)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_enter)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_enter))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->key_enter));
	}
    }
  if (sym == const_data->keysym1_esc || sym == const_data->keysym2_esc)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_esc)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_esc))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->key_esc));
	}
    }
  if (sym == const_data->keysym1_ctrl || sym == const_data->keysym2_ctrl)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_ctrl)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_ctrl))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->key_ctrl));
	}
    }
  if (sym == const_data->keysym1_alt || sym == const_data->keysym2_alt)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_alt)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_alt))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->key_alt));
	}
    }
  if (sym == const_data->keysym1_pgup || sym == const_data->keysym2_pgup)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_pgup)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_pgup))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->key_pgup));
	}
    }
  if (sym == const_data->keysym1_pgdown || sym == const_data->keysym2_pgdown)
    {
      if (!lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym1_pgdown)
	  && !lw6gui_keyboard_is_pressed (sys_context, keyboard, const_data->keysym2_pgdown))
	{
	  lw6gui_button_register_up (sys_context, &(keyboard->key_pgdown));
	}
    }
}

static void
_mouse_move (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, SDL_Event * event, int64_t timestamp)
{
  lw6gui_mouse_register_move (sys_context, mouse, event->button.x, event->button.y, timestamp);
}

static void
_mouse_button_down (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, SDL_Event * event, int64_t timestamp)
{
  switch (event->button.button)
    {
    case SDL_BUTTON_LEFT:
      lw6gui_button_register_down (sys_context, &(mouse->button_left), timestamp);
      lw6gui_mouse_drag_begin (sys_context, mouse);
      break;
    case SDL_BUTTON_RIGHT:
      lw6gui_button_register_down (sys_context, &(mouse->button_right), timestamp);
      break;
    case SDL_BUTTON_MIDDLE:
      lw6gui_button_register_down (sys_context, &(mouse->button_middle), timestamp);
      break;
    case SDL_BUTTON_WHEELUP:
      lw6gui_button_register_down (sys_context, &(mouse->wheel_up), timestamp);
      /*
       * WHEELUP & WHEELDOWN are not ordinary buttons, they never
       * stay pressed so we unpress them right on and only the
       * poll()-like functions which query if it has been pressed
       * will have some effects.
       */
      lw6gui_button_register_up (sys_context, &(mouse->wheel_up));
      lw6gui_button_register_up (sys_context, &(mouse->wheel_down));
      break;
    case SDL_BUTTON_WHEELDOWN:
      lw6gui_button_register_down (sys_context, &(mouse->wheel_down), timestamp);
      /*
       * WHEELUP & WHEELDOWN are not ordinary buttons, they never
       * stay pressed so we unpress them right on and only the
       * poll()-like functions which query if it has been pressed
       * will have some effects.
       */
      lw6gui_button_register_up (sys_context, &(mouse->wheel_up));
      lw6gui_button_register_up (sys_context, &(mouse->wheel_down));
      break;
    }
}

static void
_mouse_button_up (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, SDL_Event * event)
{
  switch (event->button.button)
    {
    case SDL_BUTTON_LEFT:
      lw6gui_button_register_up (sys_context, &(mouse->button_left));
      lw6gui_mouse_drag_end (sys_context, mouse);
      break;
    case SDL_BUTTON_RIGHT:
      lw6gui_button_register_up (sys_context, &(mouse->button_right));
      break;
    case SDL_BUTTON_MIDDLE:
      lw6gui_button_register_up (sys_context, &(mouse->button_middle));
      break;
    case SDL_BUTTON_WHEELUP:
      lw6gui_button_register_up (sys_context, &(mouse->wheel_up));
      break;
    case SDL_BUTTON_WHEELDOWN:
      lw6gui_button_register_up (sys_context, &(mouse->wheel_down));
      break;
    }
}

static int
_joystick_index (lw6sys_context_t * sys_context, SDL_Event * event, _lw6gfx_sdl_const_data_t * const_data)
{
  int ret = -1;

  if (event->jaxis.which == const_data->joystick1_index)
    {
      ret = LW6GUI_JOYSTICK1_ID;
    }
  if (event->jaxis.which == const_data->joystick2_index)
    {
      ret = LW6GUI_JOYSTICK2_ID;
    }

  return ret;
}

static int
_joystick_button_index (lw6sys_context_t * sys_context, SDL_Event * event, _lw6gfx_sdl_const_data_t * const_data, int i)
{
  int ret = -1;

  switch (i)
    {
    case LW6GUI_JOYSTICK1_ID:
      if (event->jbutton.button == const_data->joystick1_button_a_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_A;
	}
      if (event->jbutton.button == const_data->joystick1_button_b_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_B;
	}
      if (event->jbutton.button == const_data->joystick1_button_c_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_C;
	}
      if (event->jbutton.button == const_data->joystick1_button_d_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_D;
	}
      if (event->jbutton.button == const_data->joystick1_button_e_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_E;
	}
      if (event->jbutton.button == const_data->joystick1_button_f_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_F;
	}
      break;
    case LW6GUI_JOYSTICK2_ID:
      if (event->jbutton.button == const_data->joystick2_button_a_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_A;
	}
      if (event->jbutton.button == const_data->joystick2_button_b_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_B;
	}
      if (event->jbutton.button == const_data->joystick2_button_c_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_C;
	}
      if (event->jbutton.button == const_data->joystick2_button_d_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_D;
	}
      if (event->jbutton.button == const_data->joystick2_button_e_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_E;
	}
      if (event->jbutton.button == const_data->joystick2_button_f_index)
	{
	  ret = LW6GUI_JOYSTICK_BUTTON_ID_F;
	}
      break;
    }

  /*
   * If button is outside our range we still take it in account but with
   * a non-modifiable binding.
   */
  if (ret < 0 && const_data->joystick_all_buttons)
    {
      ret = event->jbutton.button % LW6GUI_NB_JOYSTICK_BUTTONS;
    }

  return ret;
}

static void
_joystick_move (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, SDL_Event * event, int limit, int64_t timestamp)
{
  switch (event->jaxis.axis)
    {
    case 0:
      lw6gui_joystick_update_axis_x (sys_context, joystick, event->jaxis.value, limit, timestamp);
      break;
    case 1:
      lw6gui_joystick_update_axis_y (sys_context, joystick, event->jaxis.value, limit, timestamp);
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("joystick %d axis %d with value %d ignored"), event->jaxis.which, event->jaxis.axis, event->jaxis.value);
      break;
    }
}

static void
_joystick_button_down (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, int b, SDL_Event * event, int64_t timestamp)
{
  switch (b)
    {
    case LW6GUI_JOYSTICK_BUTTON_ID_A:
      lw6gui_button_register_down (sys_context, &(joystick->button_a), timestamp);
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_B:
      lw6gui_button_register_down (sys_context, &(joystick->button_b), timestamp);
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_C:
      lw6gui_button_register_down (sys_context, &(joystick->button_c), timestamp);
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_D:
      lw6gui_button_register_down (sys_context, &(joystick->button_d), timestamp);
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_E:
      lw6gui_button_register_down (sys_context, &(joystick->button_e), timestamp);
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_F:
      lw6gui_button_register_down (sys_context, &(joystick->button_f), timestamp);
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("joystick %d button %d ignored"), event->jbutton.which, event->jbutton.button);
      break;
    }
}

static void
_joystick_button_up (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, int b, SDL_Event * event)
{
  switch (b)
    {
    case LW6GUI_JOYSTICK_BUTTON_ID_A:
      lw6gui_button_register_up (sys_context, &(joystick->button_a));
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_B:
      lw6gui_button_register_up (sys_context, &(joystick->button_b));
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_C:
      lw6gui_button_register_up (sys_context, &(joystick->button_c));
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_D:
      lw6gui_button_register_up (sys_context, &(joystick->button_d));
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_E:
      lw6gui_button_register_up (sys_context, &(joystick->button_e));
      break;
    case LW6GUI_JOYSTICK_BUTTON_ID_F:
      lw6gui_button_register_up (sys_context, &(joystick->button_f));
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("joystick %d button %d ignored"), event->jbutton.which, event->jbutton.button);
      break;
    }
}

/*
 * Internal poll function.
 */
lw6gui_input_t *
shared_sdl_pump_events (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context, _lw6gfx_sdl_event_callback_t event_callback_func,
			void *event_callback_data)
{
  SDL_Event event;
  int64_t timestamp = 0LL;
  lw6gui_input_t *input = &(sdl_context->input);
  _lw6gfx_sdl_const_data_t *const_data = &(sdl_context->const_data);
  int i, b;

  timestamp = shared_sdl_timer_get_timestamp (sys_context, &(sdl_context->timer));
  memset (&event, 0, sizeof (SDL_Event));
  while (SDL_PollEvent (&event))
    {
      _log_event (sys_context, &event);
      switch (event.type)
	{
	case SDL_KEYDOWN:
	  lw6gui_input_register_change (sys_context, input);
	  _key_down (sys_context, &(input->keyboard), &event, const_data, timestamp);
	  if (event.key.keysym.sym == const_data->keysym_quit)
	    {
	      lw6sys_signal_send_quit (sys_context);
	    }
	  break;
	case SDL_KEYUP:
	  lw6gui_input_register_change (sys_context, input);
	  _key_up (sys_context, &(input->keyboard), &event, const_data);
	  break;
	case SDL_QUIT:
	  lw6gui_input_register_change (sys_context, input);
	  lw6sys_signal_send_quit (sys_context);
	  break;
	case SDL_MOUSEMOTION:
	  lw6gui_input_register_change (sys_context, input);
	  _mouse_move (sys_context, &(input->mouse), &event, timestamp);
	  break;
	case SDL_MOUSEBUTTONDOWN:
	  lw6gui_input_register_change (sys_context, input);
	  _mouse_move (sys_context, &(input->mouse), &event, timestamp);
	  _mouse_button_down (sys_context, &(input->mouse), &event, timestamp);
	  break;
	case SDL_MOUSEBUTTONUP:
	  lw6gui_input_register_change (sys_context, input);
	  _mouse_move (sys_context, &(input->mouse), &event, timestamp);
	  _mouse_button_up (sys_context, &(input->mouse), &event);
	  break;
	case SDL_JOYAXISMOTION:
	  lw6gui_input_register_change (sys_context, input);
	  i = _joystick_index (sys_context, &event, const_data);
	  if (lw6gui_joystick_check_index (sys_context, i))
	    {
	      _joystick_move (sys_context, &(input->joysticks[i]), &event, const_data->joystick_limit, timestamp);
	    }
	  break;
	case SDL_JOYBUTTONDOWN:
	  lw6gui_input_register_change (sys_context, input);
	  i = _joystick_index (sys_context, &event, const_data);
	  if (lw6gui_joystick_check_index (sys_context, i))
	    {
	      b = _joystick_button_index (sys_context, &event, const_data, i);
	      _joystick_button_down (sys_context, &(input->joysticks[i]), b, &event, timestamp);
	    }
	  break;
	case SDL_JOYBUTTONUP:
	  lw6gui_input_register_change (sys_context, input);
	  i = _joystick_index (sys_context, &event, const_data);
	  if (lw6gui_joystick_check_index (sys_context, i))
	    {
	      b = _joystick_button_index (sys_context, &event, const_data, i);
	      _joystick_button_up (sys_context, &(input->joysticks[i]), b, &event);
	    }
	  break;
	}
    }

  /*
   * Event callback is here to implement specific functions, which
   * might be backend specific. Typically, this will be SDL_VIDEORESIZE
   * and SDL_VIDEOEXPOSE events, which can require specific treatments
   * which depend on how SDL is set up (OpenGL corner cases).
   */
  if (event_callback_func)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("event_callback defined, calling it with data=%p"), event_callback_data);
      event_callback_func (sys_context, event_callback_data, (void *) &event);
    }

  return input;
}
