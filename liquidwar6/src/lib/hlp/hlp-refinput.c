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

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_input (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_MOUSE_SENSITIVITY,
			  _x_
			  ("Mouse sensitivity, 1.0 is the default, 0.1 is slow, 10 is reponsive. This is used for moving the cursor during the game only, the option has no impact on menu navigation."),
			  1.0, 0, 0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_CURSOR_SENSITIVITY,
			  _x_
			  ("Keyboard and joystick sensitivity while moving the cursor. 1.0 is the default, 0.1 is slow, 10 is reponsive. This is used for moving the cursor during the game only, the option has no impact on menu navigation."),
			  1.0, 0, 0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_MAX_CURSOR_SPEED,
			  _x_
			  ("Maximum cursor speed when cursor is controlled with keyboard or joystick joystick 1. Consider using cursor-sensitivity too."),
			  10.0, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_REPEAT_DELAY, _x_ ("Time, in milliseconds, before key repeat will start, use 0 to disable."), 500, 0,
			0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_REPEAT_INTERVAL,
			_x_ ("Time, in milliseconds, between two repeats, once repeat has started, use 0 to disable."), 100, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_DOUBLE_CLICK_DELAY,
			_x_ ("Time, in milliseconds, determining wether two consecutive clicks make a double-click or not."), 333, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_AUTO_RELEASE_DELAY,
			_x_
			("Time, in milliseconds, before which a key is automatically released. This might or might not be used by the graphics backend, it's typically used by backends which don't always handle key releases events the right way, that is, don't fire them. Libcaca is a good example of such a case."),
			250, 0, 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_DOUBLE_CLICK,
			 _x_
			 ("Wether to use double-click feature, mostly usefull if running on a system that has only one button (such as a tablet-PC or anything with a tactile screen), if your mouse has three buttons, disabling this might avoid some confusion. Basically, if enabled, double-click is equivalent to right-click (fire) and triple-click is equivalent to middle-click (alternate fire)."),
			 0);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_UP, _x_ ("Custom keycode to be used as the UP key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 101) ; SDLK_e");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_DOWN, _x_ ("Guile custom code associated to the DOWN key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 100) ; SDLK_d");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_LEFT, _x_ ("Guile custom code associated to the LEFT key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 99) ; SDLK_c");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_RIGHT, _x_ ("Guile custom code associated to the RIGHT key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 118) ; SDLK_v");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_ENTER, _x_ ("Guile custom code associated to the ENTER key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 103) ; SDLK_g");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_ESC, _x_ ("Guile custom code associated to the ESC key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 102) ; SDLK_f");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_CTRL, _x_ ("Guile custom code associated to the CTRL key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 98) ; SDLK_b");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_ALT, _x_ ("Guile custom code associated to the ALT key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 110) ; SDLK_n");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_PGUP, _x_ ("Guile custom code associated to the PGUP key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 119) ; SDLK_w");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CUSTOM_PGDOWN, _x_ ("Guile custom code associated to the PGDOWN key equivalent."),
			"(c-lw6gui-keyboard-is-pressed 115) ; SDLK_s");
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_CLICK_TO_FOCUS,
			 _x_
			 ("If set to true, you'll need to click with the mouse to select a menuitem or move the cursor in the game. If not, some actions will be taken automatically without the need to click."),
			 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_ESC_BUTTON,
			 _x_
			 ("Decides wether to display an 'esc' (escape) button in the interface. This is usefull for people who control the game with the mouse only, and have a single buttons, or on a touchscreen."),
			 1);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_ZOOM_STEP,
			  _x_ ("A value, strictly greater than 1, which will be used when zooming. The greater it is, the more sensible the zoom is."), 1.1, 0,
			  0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_ZOOM_STICK_DELAY, _x_ ("How long, in msec, the zoom will stick to its default value."), 1000, 0, 0);

  ret = 1;
  return ret;
}
