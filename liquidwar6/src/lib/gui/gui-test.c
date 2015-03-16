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

#include <CUnit/CUnit.h>

#include "gui.h"

#define _TEST_LOOK_KEY1 "dynamic-zoom"
#define _TEST_LOOK_VALUE1 "1.23"
#define _TEST_LOOK_KEY2 LW6DEF_BACKGROUND_STYLE
#define _TEST_LOOK_VALUE2 "void"
#define _TEST_LOOK_ZOOM_STEP 1.1
#define _TEST_MENUITEM_LABEL1 "My Label"
#define _TEST_MENUITEM_LABEL2 "My Label 2"
#define _TEST_MENUITEM_LABEL3 "The third label"
#define _TEST_MENUITEM_LABEL4 "From outer space"
#define _TEST_MENUITEM_TOOLTIP1 "Tooltip explaining what my label means"
#define _TEST_MENUITEM_TOOLTIP2 "Tooltip explaining what my label 2 means,\nand does it on 2 lines"
#define _TEST_MENUITEM_TOOLTIP3 "The third label is the best label of all, at least this is what this (rather long) tooltip says"
#define _TEST_MENUITEM_TOOLTIP4 NULL
#define _TEST_MENUITEM_VALUE1 42
#define _TEST_MENUITEM_VALUE2 421
#define _TEST_MENUITEM_VALUE3 -1
#define _TEST_MENUITEM_VALUE4 65535
#define _TEST_MENUITEM_NOW 1234
#define _TEST_MENU_TITLE1 "My Title"
#define _TEST_MENU_TITLE2 "My Title 2"
#define _TEST_MENU_HELP1 "Chapi chapo this is the first help"
#define _TEST_MENU_HELP2 "2nd help, things change, now let's face it, this is a long string are there few chances it won't be cut somewhere when displayed"
#define _TEST_MENU_POPUP1 NULL
#define _TEST_MENU_POPUP2 "Be carefull\nt...this is important!\nYou're likely to be eaten by a grue ;)"
#define _TEST_MENU_ESC "Esc"
#define _TEST_MENU_ENABLE_ESC 1
#define _TEST_MENU_ALLOW_SCROLL1 0
#define _TEST_MENU_ALLOW_SCROLL2 0
#define _TEST_MENU_BREADCRUMBS1 "foo"
#define _TEST_MENU_BREADCRUMBS2 "bar"
#define _TEST_MAX_DISPLAYED_ITEMS 2
#define _TEST_VIDEO_MODE_WISHED_WIDTH 640
#define _TEST_VIDEO_MODE_WISHED_HEIGHT 480
#define _TEST_VIDEO_MODE_WISHED_FULLSCREEN 1
#define _TEST_VIDEO_MODE_AVAILABLE_1_WIDTH 400
#define _TEST_VIDEO_MODE_AVAILABLE_1_HEIGHT 300
#define _TEST_VIDEO_MODE_AVAILABLE_2_WIDTH 700
#define _TEST_VIDEO_MODE_AVAILABLE_2_HEIGHT 525
#define _TEST_VIDEO_MODE_AVAILABLE_3_WIDTH 1600
#define _TEST_VIDEO_MODE_AVAILABLE_3_HEIGHT 1200
#define _TEST_VIDEO_MODE_RATIO_DST_WIDTH 100
#define _TEST_VIDEO_MODE_RATIO_DST_HEIGHT 100
#define _TEST_VIDEO_MODE_RATIO_SRC_WIDTH 2000
#define _TEST_VIDEO_MODE_RATIO_SRC_HEIGHT 500
#define _TEST_KEYSYM 123
#define _TEST_UNICODE 65
#define _TEST_LABEL "fake A letter"
#define _TEST_REPEAT_DELAY 100
#define _TEST_REPEAT_INTERVAL 10
#define _TEST_REPEAT_DOUBLE_CLICK_DELAY 250
#define _TEST_REPEAT_AUTO_RELEASE_DELAY 50
#define _TEST_MOUSE_X 12
#define _TEST_MOUSE_Y 34
#define _TEST_JOYSTICK_LIMIT 20
#define _TEST_JOYSTICK_X -30
#define _TEST_JOYSTICK_Y 50
#define _TEST_DST_X0 100.0
#define _TEST_DST_Y0 200.0
#define _TEST_DST_W 30.0
#define _TEST_DST_H 40.0
#define _TEST_SRC_X 1.0
#define _TEST_SRC_Y 2.0
#define _TEST_SRC_X0 10.0
#define _TEST_SRC_Y0 20.0
#define _TEST_SRC_W 100.0
#define _TEST_SRC_H 50.0
#define _TEST_RECT1_X1 3.0f
#define _TEST_RECT1_Y1 2.0f
#define _TEST_RECT1_X2 30.0f
#define _TEST_RECT1_Y2 20.0f
#define _TEST_RECT2_X -1.0f
#define _TEST_RECT2_Y -1.0f
#define _TEST_RECT2_W 10.0f
#define _TEST_RECT2_H 10.0f
#define _TEST_RECT_ARRAY_W 1333
#define _TEST_RECT_ARRAY_H 666
#define _TEST_RECT_ARRAY_TILE_SIZE 123
#define _TEST_RECT_ARRAY_BORDER_SIZE 7
#define _TEST_RECT_ARRAY_SOURCE_X 500
#define _TEST_RECT_ARRAY_SOURCE_Y 250
#define _TEST_RECT_ARRAY_TILE_I 19
#define _TEST_RECT_ARRAY_QUAD_P1X -10.0f
#define _TEST_RECT_ARRAY_QUAD_P1Y -5.0f
#define _TEST_RECT_ARRAY_QUAD_P2X 1.0f
#define _TEST_RECT_ARRAY_QUAD_P2Y -4.0f
#define _TEST_RECT_ARRAY_QUAD_P3X -8.0f
#define _TEST_RECT_ARRAY_QUAD_P3Y 2.0f
#define _TEST_RECT_ARRAY_QUAD_P4X -1.0f
#define _TEST_RECT_ARRAY_QUAD_P4Y 3.0f
#define _TEST_ZONE1_X1 3.0f
#define _TEST_ZONE1_Y1 2.0f
#define _TEST_ZONE1_X2 30.0f
#define _TEST_ZONE1_Y2 20.0f
#define _TEST_ZONE2_X -1.0f
#define _TEST_ZONE2_Y -1.0f
#define _TEST_ZONE2_W 10.0f
#define _TEST_ZONE2_H 10.0f
#define _TEST_POWER_OF_TWO 37
#define _TEST_VIEWPORT_SCREEN_W 800
#define _TEST_VIEWPORT_SCREEN_H 600
#define _TEST_VIEWPORT_DRAWABLE_X1 5
#define _TEST_VIEWPORT_DRAWABLE_Y1 10
#define _TEST_VIEWPORT_DRAWABLE_X2 700
#define _TEST_VIEWPORT_DRAWABLE_Y2 500
#define _TEST_VIEWPORT_CENTER_X 50
#define _TEST_VIEWPORT_CENTER_Y 60
#define _TEST_VIEWPORT_MAP_W 120
#define _TEST_VIEWPORT_MAP_H 80
#define _TEST_VIEWPORT_X_POLARITY 1
#define _TEST_VIEWPORT_Y_POLARITY 0
#define _TEST_VIEWPORT_X_WRAP 1
#define _TEST_VIEWPORT_Y_WRAP 1
#define _TEST_VIEWPORT_KEEP_RATIO 1
#define _TEST_VIEWPORT_SCROLL_LIMIT 0.5f
#define _TEST_VIEWPORT_USE_OLD_CENTER 0
#define _TEST_VIEWPORT_GLOBAL_ZOOM_MIN 0.1f
#define _TEST_VIEWPORT_GLOBAL_ZOOM_MAX 10.0f
#define _TEST_VIEWPORT_GLOBAL_ZOOM_STEP 9.9f
#define _TEST_VIEWPORT_NB_RANDOM_TRIES 10
#define _TEST_VIEWPORT_CLIP 1
#define _TEST_VIEWPORT_WRAP 1
#define _TEST_SMOOTHER_DURATION 500
#define _TEST_SMOOTHER_DELAY 60
#define _TEST_SMOOTHER_N 10
#define _TEST_SMOOTHER_VALUE1 4.0f
#define _TEST_SMOOTHER_VALUE2 100.0f
#define _TEST_SMOOTHER_VALUE3 -1000.0f
#define _TEST_SMOOTHER_VALUE4 421.0f
#define _TEST_SMOOTHER_STEP 100
#define _TEST_TIMESTAMP 1000000000000LL
#define _TEST_GEOMETRY_POINT_MIN 1
#define _TEST_GEOMETRY_POINT_RANGE 10
#define _TEST_GEOMETRY_RECT_X1 2
#define _TEST_GEOMETRY_RECT_Y1 6
#define _TEST_GEOMETRY_RECT_X2 5
#define _TEST_GEOMETRY_RECT_Y2 9

typedef struct _lw6gui_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6gui_test_data_t;

static _lw6gui_test_data_t _test_data = { 0, NULL };

/*
 * Testing button
 */
static void
_test_button ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int64_t timestamp = _TEST_TIMESTAMP;
    lw6gui_button_t button;
    lw6gui_repeat_settings_t repeat_settings;

    repeat_settings.delay = _TEST_REPEAT_DELAY;
    repeat_settings.interval = _TEST_REPEAT_INTERVAL;
    repeat_settings.double_click_delay = _TEST_REPEAT_DOUBLE_CLICK_DELAY;
    repeat_settings.auto_release_delay = _TEST_REPEAT_AUTO_RELEASE_DELAY;

    memset (&button, 0, sizeof (lw6gui_button_t));

    lw6gui_button_register_down (sys_context, &button, timestamp++);
    lw6gui_button_register_up (sys_context, &button);
    if (!lw6gui_button_is_pressed (sys_context, &button))
      {
	if (lw6gui_button_pop_press (sys_context, &button))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("button press buffer works"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("button press buffer problem"));
	    ret = 0;
	  }
      }
    else
      {
	ret = 0;
      }
    if (ret)
      {
	ret = 0;
	lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 0);
	if (!lw6gui_button_pop_double_click (sys_context, &button) && !lw6gui_button_pop_triple_click (sys_context, &button))
	  {
	    lw6gui_button_register_down (sys_context, &button, timestamp++);
	    lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 0);
	    timestamp += 2 * _TEST_REPEAT_DOUBLE_CLICK_DELAY;
	    lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 0);
	    if (lw6gui_button_pop_double_click (sys_context, &button))
	      {
		if (!lw6gui_button_pop_double_click (sys_context, &button))
		  {
		    timestamp += 2 * _TEST_REPEAT_DOUBLE_CLICK_DELAY;
		    lw6gui_button_register_down (sys_context, &button, timestamp);
		    lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 0);
		    timestamp += 2 * _TEST_REPEAT_DOUBLE_CLICK_DELAY;
		    lw6gui_button_register_down (sys_context, &button, timestamp);
		    lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 0);
		    if (!lw6gui_button_pop_double_click (sys_context, &button))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("double click works"));
			ret = 1;
		      }
		  }
	      }
	  }
	if (!ret)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("double click problem"));
	  }
      }
    if (ret)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing auto_release mode"));
	lw6gui_button_register_up (sys_context, &button);
	lw6gui_button_register_down (sys_context, &button, timestamp);
	lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 1);
	if (lw6gui_button_is_pressed (sys_context, &button))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, button pressed at t=%" LW6SYS_PRINTF_LL "d just after press"), (long long) timestamp);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("Problem, button not pressed at t=%" LW6SYS_PRINTF_LL "d just after press"), (long long) timestamp);
	    ret = 0;
	  }
	timestamp += _TEST_REPEAT_AUTO_RELEASE_DELAY / 2;
	lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 1);
	if (lw6gui_button_is_pressed (sys_context, &button))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("OK, button still pressed at t=%" LW6SYS_PRINTF_LL "d \"half delay\" after press"), (long long) timestamp);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("Problem, button not pressed at t=%" LW6SYS_PRINTF_LL "d \"half delay\" after press"), (long long) timestamp);
	    ret = 0;
	  }
	timestamp += _TEST_REPEAT_AUTO_RELEASE_DELAY;
	lw6gui_button_update_repeat (sys_context, &button, &repeat_settings, timestamp, 1);
	if (!lw6gui_button_is_pressed (sys_context, &button))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("OK, button now not pressed at t=%" LW6SYS_PRINTF_LL "d \"1 1/2 delay\" after press"), (long long) timestamp);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("Problem, button still pressed at t=%" LW6SYS_PRINTF_LL "d \"1 1/2 delay\" after press"), (long long) timestamp);
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing coord
 */
static void
_test_coord ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    float x = 0.0f;
    float y = 0.0f;

    if (lw6gui_coord_calc_xy
	(sys_context, &x, &y, _TEST_DST_X0, _TEST_DST_Y0, _TEST_DST_W, _TEST_DST_H, _TEST_SRC_X, _TEST_SRC_Y, _TEST_SRC_X0, _TEST_SRC_Y0, _TEST_SRC_W,
	 _TEST_SRC_H))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_
		    ("calc_xy returned %f,%f (%f,%f/%fx%f) with source %f,%f (%f,%f/%fx%f)"),
		    x, y, _TEST_DST_X0, _TEST_DST_Y0, _TEST_DST_W,
		    _TEST_DST_H, _TEST_SRC_X, _TEST_SRC_Y, _TEST_SRC_X0, _TEST_SRC_Y0, _TEST_SRC_W, _TEST_SRC_H);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing input
 */
static void
_test_input ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_input_t *src = NULL;
    lw6gui_input_t *dst = NULL;
    lw6gui_keypress_t *keypress = NULL;
    lw6gui_repeat_settings_t repeat_settings;

    src = lw6gui_input_new (sys_context);
    if (src)
      {
	lw6gui_input_reset (sys_context, src);
	lw6gui_input_register_change (sys_context, src);
	dst = lw6gui_input_new (sys_context);
	if (dst)
	  {
	    if (lw6gui_keyboard_register_key_down (sys_context, &(src->keyboard), _TEST_KEYSYM, _TEST_UNICODE, _TEST_LABEL, 0))
	      {
		repeat_settings.delay = _TEST_REPEAT_DELAY;
		repeat_settings.interval = _TEST_REPEAT_INTERVAL;
		repeat_settings.double_click_delay = _TEST_REPEAT_DOUBLE_CLICK_DELAY;
		lw6gui_input_update_repeat (sys_context, src, &repeat_settings, 0);
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("need_sync (before sync) dst=%d src=%d"), lw6gui_input_need_sync (sys_context, dst), lw6gui_input_need_sync (sys_context,
																	      src));
		if (lw6gui_input_sync (sys_context, dst, src))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_ ("need_sync (after sync) dst=%d src=%d"), lw6gui_input_need_sync (sys_context, dst), lw6gui_input_need_sync (sys_context,
																		 src));
		    keypress = lw6gui_keyboard_pop_keypress (sys_context, &(dst->keyboard));
		    if (keypress)
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keypress synchronisation is OK"));
			lw6gui_keypress_free (sys_context, keypress);
		      }
		    else
		      {
			ret = 0;
		      }
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("synchronisation error"));
		    ret = 0;
		  }
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6gui_input_free (sys_context, dst);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6gui_input_free (sys_context, src);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing joystick
 */
static void
_test_joystick ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_joystick_t joystick;
    lw6gui_move_pad_t move_pad;

    memset (&joystick, 0, sizeof (lw6gui_joystick_t));
    memset (&move_pad, 0, sizeof (lw6gui_move_pad_t));

    if (!lw6gui_joystick_check_index (sys_context, 0) || lw6gui_joystick_check_index (sys_context, -1)
	|| lw6gui_joystick_check_index (sys_context, LW6GUI_NB_KEYS))
      {
	ret = 0;
      }
    lw6gui_joystick_update_axis_x (sys_context, &joystick, _TEST_JOYSTICK_X, _TEST_JOYSTICK_LIMIT, 0);
    lw6gui_joystick_update_axis_y (sys_context, &joystick, _TEST_JOYSTICK_Y, _TEST_JOYSTICK_LIMIT, 0);
    lw6gui_joystick_get_move_pad (sys_context, &joystick, &move_pad);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("joystick up=%d, down=%d, left=%d, right=%d"), move_pad.up, move_pad.down, move_pad.left, move_pad.right);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing keyboard
 */
static void
_test_keyboard ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_input_t *input = NULL;
    lw6gui_keypress_t *keypress = NULL;
    char *repr;
    lw6gui_move_pad_t move_pad;

    memset (&move_pad, 0, sizeof (lw6gui_move_pad_t));

    if (!lw6gui_keyboard_check_keysym (sys_context, 0) || lw6gui_keyboard_check_keysym (sys_context, -1)
	|| lw6gui_keyboard_check_keysym (sys_context, LW6GUI_NB_KEYS))
      {
	ret = 0;
      }

    input = lw6gui_input_new (sys_context);
    if (input)
      {
	if (lw6gui_keyboard_register_key_down (sys_context, &(input->keyboard), _TEST_KEYSYM, _TEST_UNICODE, _TEST_LABEL, 0))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("key %d pressed=%d"), _TEST_KEYSYM, lw6gui_keyboard_is_pressed (sys_context, &(input->keyboard), _TEST_KEYSYM));
	    if (lw6gui_keyboard_register_key_up (sys_context, &(input->keyboard), _TEST_KEYSYM))
	      {
		keypress = lw6gui_keyboard_pop_keypress (sys_context, &(input->keyboard));
		if (keypress)
		  {
		    lw6gui_keyboard_get_move_pad (sys_context, &(input->keyboard), &move_pad);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_ ("keyboard up=%d, down=%d, left=%d, right=%d"), move_pad.up, move_pad.down, move_pad.left, move_pad.right);
		    repr = lw6gui_keypress_repr (sys_context, keypress);
		    if (repr)
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keysym %d popped"), keypress->keysym);
			LW6SYS_FREE (sys_context, repr);
		      }
		    else
		      {
			ret = 0;
		      }
		    if (lw6gui_keyboard_pop_keypress (sys_context, &(input->keyboard)))
		      {
			ret = 0;
		      }
		    lw6gui_keypress_free (sys_context, keypress);
		  }
		else
		  {
		    ret = 0;
		  }
	      }
	    else
	      {
		ret = 0;
	      }
	  }
	else
	  {
	    ret = 0;
	  }
	lw6gui_input_free (sys_context, input);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing keypress
 */
static void
_test_keypress ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_keypress_t *keypress = NULL;
    char *repr = NULL;

    keypress = lw6gui_keypress_new (sys_context, _TEST_KEYSYM, _TEST_UNICODE, _TEST_LABEL);
    if (keypress)
      {
	repr = lw6gui_keypress_repr (sys_context, keypress);
	if (repr)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keypress repr is \"%s\""), repr);
	    LW6SYS_FREE (sys_context, repr);
	  }
	lw6gui_keypress_free (sys_context, keypress);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing look
 */
static void
_test_look ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_style_t style;
    lw6gui_look_t *look = NULL;
    lw6gui_look_t *look2 = NULL;
    char *repr = NULL;
    char *value = NULL;

    lw6map_style_zero (sys_context, &style);
    lw6map_style_defaults (sys_context, &style);
    look = lw6gui_look_new (sys_context, &style);
    if (look)
      {
	repr = lw6gui_look_repr (sys_context, look);
	if (repr)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("look repr is %s"), repr);
	    LW6SYS_FREE (sys_context, repr);
	  }
	if (!lw6gui_look_is_same (sys_context, look, look))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("look comparison failed"));
	    ret = 0;
	  }
	if (lw6gui_look_set (sys_context, look, _TEST_LOOK_KEY1, _TEST_LOOK_VALUE1))
	  {
	    value = lw6gui_look_get (sys_context, look, _TEST_LOOK_KEY1);
	    if (value)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("look value for key \"%s\" is \"%s\""), _TEST_LOOK_KEY1, value);
		LW6SYS_FREE (sys_context, value);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get look key \"%s\""), _TEST_LOOK_KEY1);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to set look key \"%s\" to \"%s\""), _TEST_LOOK_KEY1, _TEST_LOOK_VALUE1);
	    ret = 0;
	  }
	if (lw6gui_look_set (sys_context, look, _TEST_LOOK_KEY2, _TEST_LOOK_VALUE2))
	  {
	    value = lw6gui_look_get (sys_context, look, _TEST_LOOK_KEY2);
	    if (value)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("look value for key \"%s\" is \"%s\""), _TEST_LOOK_KEY2, value);
		LW6SYS_FREE (sys_context, value);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get look key \"%s\""), _TEST_LOOK_KEY2);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to set look key \"%s\" to \"%s\""), _TEST_LOOK_KEY2, _TEST_LOOK_VALUE2);
	    ret = 0;
	  }
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("zoom is %f"), look->dynamic_zoom);
	lw6gui_look_zoom_in (sys_context, look, _TEST_LOOK_ZOOM_STEP);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("after zoom in zoom is %f"), look->dynamic_zoom);
	lw6gui_look_zoom_out (sys_context, look, _TEST_LOOK_ZOOM_STEP);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("after zoom out zoom is %f"), look->dynamic_zoom);
	look2 = lw6gui_look_dup (sys_context, look);
	if (look2)
	  {
	    repr = lw6gui_look_repr (sys_context, look2);
	    if (repr)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("duplicated, look2 repr is %s"), repr);
		LW6SYS_FREE (sys_context, repr);
	      }
	    lw6gui_look_free (sys_context, look2);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate look"));
	    ret = 0;
	  }
	lw6gui_look_free (sys_context, look);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create look"));
	ret = 0;
      }
    lw6map_style_clear (sys_context, &style);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing menuitemm
 */
static void
_test_menuitem ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_menuitem_t *menuitem = NULL;
    lw6gui_menuitem_t *menuitem2 = NULL;
    char *repr;

    menuitem = lw6gui_menuitem_new (sys_context, _TEST_MENUITEM_LABEL1, _TEST_MENUITEM_TOOLTIP1, _TEST_MENUITEM_VALUE1, 1, 0, 0);
    if (menuitem)
      {
	lw6gui_menuitem_set_label (sys_context, menuitem, _TEST_MENUITEM_LABEL2, _TEST_MENUITEM_NOW);
	lw6gui_menuitem_set_tooltip (sys_context, menuitem, _TEST_MENUITEM_TOOLTIP2, _TEST_MENUITEM_NOW);
	lw6gui_menuitem_set_value (sys_context, menuitem, _TEST_MENUITEM_VALUE2, _TEST_MENUITEM_NOW);
	lw6gui_menuitem_select (sys_context, menuitem, 1, _TEST_MENUITEM_NOW);
	lw6gui_menuitem_select (sys_context, menuitem, 0, _TEST_MENUITEM_NOW);
	repr = lw6gui_menuitem_repr (sys_context, menuitem);
	if (repr)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("menuitem repr is %s"), repr);
	    LW6SYS_FREE (sys_context, repr);
	  }
	else
	  {
	    ret = 0;
	  }
	if (!lw6gui_menuitem_is_same (sys_context, menuitem, menuitem))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menuitem comparison failed"));
	  }
	menuitem2 = lw6gui_menuitem_dup (sys_context, menuitem);
	if (menuitem2)
	  {
	    if (lw6gui_menuitem_is_same (sys_context, menuitem, menuitem2))
	      {
		lw6gui_menuitem_sync (sys_context, menuitem2, menuitem);
		repr = lw6gui_menuitem_repr (sys_context, menuitem2);
		if (repr)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("menuitem duplicate repr is %s"), repr);
		    LW6SYS_FREE (sys_context, repr);
		  }
		else
		  {
		    ret = 0;
		  }
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6gui_menuitem_free (sys_context, menuitem2);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6gui_menuitem_free (sys_context, menuitem);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing menu
 */
static void
_test_menu ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_menu_t *menu = NULL;
    char *repr;

    menu = lw6gui_menu_new (sys_context, _TEST_MENU_TITLE1, _TEST_MENU_HELP1, _TEST_MENU_POPUP1, _TEST_MENU_ESC, _TEST_MENU_ENABLE_ESC);
    if (menu)
      {
	lw6gui_menu_set_title (sys_context, menu, _TEST_MENU_TITLE2);
	lw6gui_menu_set_help (sys_context, menu, _TEST_MENU_HELP2);
	lw6gui_menu_set_popup (sys_context, menu, _TEST_MENU_POPUP2);
	lw6gui_menu_close_popup (sys_context, menu);
	if (!lw6gui_menu_has_popup (sys_context, menu))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("no popup after close, OK"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("popup reported to exist after being closed"));
	    ret = 0;
	  }
	lw6gui_menu_set_popup (sys_context, menu, _TEST_MENU_POPUP2);
	if (lw6gui_menu_has_popup (sys_context, menu))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("popup defined, OK"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no popup reported when it's just been set"));
	    ret = 0;
	  }
	repr = lw6gui_menu_repr (sys_context, menu);
	if (repr)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("menu repr is %s"), repr);
	    LW6SYS_FREE (sys_context, repr);
	  }
	else
	  {
	    ret = 0;
	  }
	if (!lw6gui_menu_is_same (sys_context, menu, menu))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menu comparison failed"));
	  }
	lw6gui_menu_free (sys_context, menu);
      }
    else
      {
	ret = 0;
      }
  }

  {
    lw6gui_menu_t *menu = NULL;
    lw6gui_menu_t *menu2 = NULL;
    lw6gui_menuitem_t *menuitem1 = NULL;
    lw6gui_menuitem_t *menuitem2 = NULL;
    lw6gui_menuitem_t *menuitem3 = NULL;
    char *repr;
    int menuitem_id;
    lw6sys_list_t *breadcrumbs = NULL;

    breadcrumbs = lw6sys_list_new (sys_context, lw6sys_free_callback);
    if (breadcrumbs)
      {
	lw6sys_list_push_back (sys_context, &breadcrumbs, lw6sys_str_copy (sys_context, _TEST_MENU_BREADCRUMBS1));
	lw6sys_list_push_back (sys_context, &breadcrumbs, lw6sys_str_copy (sys_context, _TEST_MENU_BREADCRUMBS2));
      }
    if (breadcrumbs)
      {
	menu = lw6gui_menu_new (sys_context, _TEST_MENU_TITLE1, _TEST_MENU_HELP1, _TEST_MENU_POPUP1, _TEST_MENU_ESC, _TEST_MENU_ENABLE_ESC);
	if (menu)
	  {
	    lw6gui_menu_set_title (sys_context, menu, _TEST_MENU_TITLE2);
	    lw6gui_menu_set_breadcrumbs (sys_context, menu, breadcrumbs);
	    menuitem1 = lw6gui_menuitem_new (sys_context, _TEST_MENUITEM_LABEL1, _TEST_MENUITEM_TOOLTIP1, _TEST_MENUITEM_VALUE1, 1, 0, 0);
	    if (menuitem1)
	      {
		menuitem2 = lw6gui_menuitem_new (sys_context, _TEST_MENUITEM_LABEL2, _TEST_MENUITEM_TOOLTIP2, _TEST_MENUITEM_VALUE2, 1, 0, 0);
		if (menuitem2)
		  {
		    menuitem3 = lw6gui_menuitem_new (sys_context, _TEST_MENUITEM_LABEL3, _TEST_MENUITEM_TOOLTIP3, _TEST_MENUITEM_VALUE3, 1, 0, 0);
		    if (menuitem3)
		      {
			lw6gui_menu_append (sys_context, menu, menuitem1, 0);
			lw6gui_menu_insert (sys_context, menu, menuitem2, 0, 0);
			lw6gui_menu_insert (sys_context, menu, menuitem3, 1, 0);
			lw6gui_menu_select (sys_context, menu, 2, _TEST_MENU_ALLOW_SCROLL1, 0);
			lw6gui_menu_remove (sys_context, menu, 0, 0);
			lw6gui_menu_select (sys_context, menu, 0, _TEST_MENU_ALLOW_SCROLL2, 0);
			menuitem_id =
			  lw6gui_menu_append_for_id_use (sys_context, menu, _TEST_MENUITEM_LABEL4, _TEST_MENUITEM_TOOLTIP4, _TEST_MENUITEM_VALUE4, 1, 0, 0, 0);
			if (menuitem_id)
			  {
			    lw6gui_menu_sync_using_id (sys_context, menu, menuitem_id, _TEST_MENUITEM_LABEL1, _TEST_MENUITEM_TOOLTIP1, _TEST_MENUITEM_VALUE1,
						       1, 0, 0, 0);
			    lw6gui_menu_remove_using_id (sys_context, menu, menuitem_id, 0);
			  }
			lw6gui_menu_update_display_range (sys_context, menu, _TEST_MAX_DISPLAYED_ITEMS);
			lw6gui_menu_center (sys_context, menu, 1, _TEST_MAX_DISPLAYED_ITEMS);
			repr = lw6gui_menu_repr (sys_context, menu);
			if (repr)
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("menu repr is %s"), repr);
			    LW6SYS_FREE (sys_context, repr);
			  }
			else
			  {
			    ret = 0;
			  }
			menu2 = lw6gui_menu_dup (sys_context, menu);
			if (menu2)
			  {
			    if (lw6gui_menu_is_same (sys_context, menu, menu2))
			      {
				lw6gui_menu_sync (sys_context, menu2, menu);
				repr = lw6gui_menu_repr (sys_context, menu2);
				if (repr)
				  {
				    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("menu duplicate repr is %s"), repr);
				    LW6SYS_FREE (sys_context, repr);
				  }
				else
				  {
				    ret = 0;
				  }
			      }
			    else
			      {
				ret = 0;
			      }
			    lw6gui_menu_free (sys_context, menu2);
			  }
			else
			  {
			    ret = 0;
			  }
		      }
		    else
		      {
			ret = 0;
		      }
		  }
		else
		  {
		    ret = 0;
		  }
	      }
	    else
	      {
		ret = 0;
	      }
	    if (menu->nb_items > 0)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d items in menu, will try to remove them all"), menu->nb_items);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("%d items in menu, there should be more"), menu->nb_items);
		ret = 0;
	      }
	    lw6gui_menu_remove_all (sys_context, menu, lw6sys_get_timestamp (sys_context));
	    if (menu->nb_items == 0)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("no more items in menu, fine"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("%d items in menu, there should be none after complete removal"), menu->nb_items);
		ret = 0;
	      }
	    lw6gui_menu_free (sys_context, menu);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6sys_list_free (sys_context, breadcrumbs);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing mouse
 */
static void
_test_mouse ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_mouse_t mouse;
    int x = 0, y = 0;
    int dx = 0, dy = 0, px = 0, py = 0, sx = 0, sy = 0;

    memset (&mouse, 0, sizeof (lw6gui_mouse_t));

    lw6gui_mouse_register_move (sys_context, &mouse, _TEST_MOUSE_X, _TEST_MOUSE_Y, 0);
    lw6gui_mouse_drag_begin (sys_context, &mouse);
    lw6gui_mouse_drag_end (sys_context, &mouse);
    lw6gui_mouse_drag_pop (sys_context, &mouse, &dx, &dy, &px, &py, &sx, &sy);

    if (lw6gui_mouse_poll_move (sys_context, &mouse, &x, &y))
      {
	if (x == _TEST_MOUSE_X && y == _TEST_MOUSE_Y)
	  {
	    // ok
	  }
	else
	  {
	    ret = 0;
	  }
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_log_rect (lw6sys_context_t * sys_context, lw6gui_rect_t * rect, char *comment)
{
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s x1=%d y1=%d x2=%d y2=%d w=%d h=%d"), comment, rect->x1, rect->y1, rect->x2, rect->y2, rect->w, rect->h);
}

/*
 * Testing poweroftwo
 */
static void
_test_power_of_two ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("input=%d power_of_two_le=%d power_of_two_ge=%d"),
		_TEST_POWER_OF_TWO, lw6gui_power_of_two_le (sys_context, _TEST_POWER_OF_TWO), lw6gui_power_of_two_ge (sys_context, _TEST_POWER_OF_TWO));
  }
  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing rect
 */
static void
_test_rect ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_rect_t rect1;
    lw6gui_rect_t rect2;
    lw6gui_rect_t rect3;

    lw6gui_rect_init_x1y1x2y2 (sys_context, &rect1, _TEST_RECT1_X1, _TEST_RECT1_Y1, _TEST_RECT1_X2, _TEST_RECT1_Y2);
    _log_rect (sys_context, &rect1, _x_ ("rect1"));
    lw6gui_rect_init_x1y1x2y2 (sys_context, &rect1, _TEST_RECT2_X, _TEST_RECT2_Y, _TEST_RECT2_W, _TEST_RECT2_H);
    _log_rect (sys_context, &rect2, _x_ ("rect2"));
    lw6gui_rect_clip (sys_context, &rect3, &rect1, &rect2);
    _log_rect (sys_context, &rect3, _x_ ("rect3"));
  }
  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing rect array
 */
static void
_test_rect_array ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_rect_array_t rect_array;
    lw6gui_rect_t rect;
    int i = 0;
    int x_polarity, y_polarity;
    lw6gui_quad_t quad;
    lw6gui_quad_t source_quad;

    lw6gui_rect_array_init (sys_context, &rect_array, _TEST_RECT_ARRAY_W, _TEST_RECT_ARRAY_H, _TEST_RECT_ARRAY_TILE_SIZE, _TEST_RECT_ARRAY_BORDER_SIZE);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_
		("rect_array source=%dx%d limits=(%d,%d),(%d,%d) tile_size=%d border_size=%d tile_spacing=%d nb_tiles_w=%d nb_tiles_h=%d nb_tiles=%d"),
		rect_array.source.w, rect_array.source.h,
		rect_array.limits.x1, rect_array.limits.y1,
		rect_array.limits.x2, rect_array.limits.y2,
		rect_array.tile_size, rect_array.border_size, rect_array.tile_spacing, rect_array.nb_tiles_w, rect_array.nb_tiles_h, rect_array.nb_tiles);
    if (lw6gui_rect_array_get_tile_by_source_xy (sys_context, &rect_array, &rect, &i, _TEST_RECT_ARRAY_SOURCE_X, _TEST_RECT_ARRAY_SOURCE_Y))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_
		    ("tile for source %d,%d is index %d corners=(%d,%d),(%d,%d)"),
		    _TEST_RECT_ARRAY_SOURCE_X, _TEST_RECT_ARRAY_SOURCE_Y, i, rect.x1, rect.y1, rect.x2, rect.y2);
	if (lw6gui_rect_array_get_tile_by_i (sys_context, &rect_array, &rect, _TEST_RECT_ARRAY_TILE_I))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("tile i=%d corners=(%d,%d),(%d,%d)"), _TEST_RECT_ARRAY_TILE_I, rect.x1, rect.y1, rect.x2, rect.y2);
	    source_quad.p1.x = _TEST_RECT_ARRAY_QUAD_P1X;
	    source_quad.p1.y = _TEST_RECT_ARRAY_QUAD_P1Y;
	    source_quad.p2.x = _TEST_RECT_ARRAY_QUAD_P2X;
	    source_quad.p2.y = _TEST_RECT_ARRAY_QUAD_P2Y;
	    source_quad.p3.x = _TEST_RECT_ARRAY_QUAD_P3X;
	    source_quad.p3.y = _TEST_RECT_ARRAY_QUAD_P3Y;
	    source_quad.p4.x = _TEST_RECT_ARRAY_QUAD_P4X;
	    source_quad.p4.y = _TEST_RECT_ARRAY_QUAD_P4Y;
	    for (y_polarity = LW6MAP_RULES_MIN_Y_POLARITY; y_polarity <= LW6MAP_RULES_MAX_Y_POLARITY; ++y_polarity)
	      {
		for (x_polarity = LW6MAP_RULES_MIN_X_POLARITY; x_polarity <= LW6MAP_RULES_MAX_X_POLARITY; ++x_polarity)
		  {
		    if (lw6gui_rect_array_get_tile_and_quad (sys_context, &rect_array, &rect, &i, &quad, &source_quad, x_polarity, y_polarity))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				    _x_
				    ("polarity x=%d y=%d tile corners=(%d,%d),(%d,%d) quad=(%f,%f),(%f,%f),(%f,%f),(%f,%f)"),
				    x_polarity, y_polarity, rect.x1, rect.y1,
				    rect.x2, rect.y2, quad.p1.x, quad.p1.y, quad.p2.x, quad.p2.y, quad.p3.x, quad.p3.y, quad.p4.x, quad.p4.y);
		      }
		    else
		      {
			ret = 0;
		      }
		  }
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find tile for i=%d"), _TEST_RECT_ARRAY_TILE_I);
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find tile for source %d,%d"), _TEST_RECT_ARRAY_SOURCE_X, _TEST_RECT_ARRAY_SOURCE_Y);
	ret = 0;
      }
    /*
       #define _TEST_RECT_ARRAY_SOURCE_X 500
       #define _TEST_RECT_ARRAY_SOURCE_Y 250
       #define _TEST_RECT_ARRAY_TILE_I 19
       #define _TEST_RECT_ARRAY_QUAD_P1X 2000.0f
       #define _TEST_RECT_ARRAY_QUAD_P1Y 3000.0f
       #define _TEST_RECT_ARRAY_QUAD_P2X 2100.0f
       #define _TEST_RECT_ARRAY_QUAD_P2Y 3100.0f
       #define _TEST_RECT_ARRAY_QUAD_P3X 2050.0f
       #define _TEST_RECT_ARRAY_QUAD_P3Y 3100.0f
       #define _TEST_RECT_ARRAY_QUAD_P4X 2050.0f
       #define _TEST_RECT_ARRAY_QUAD_P4Y 2100.0f
       #define _TEST_RECT_ARRAY_X_POLARITY -1
       #define _TEST_RECT_ARRAY_Y_POLARITY 1
     */
  }
  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing video more
 */
static void
_test_smoother ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_smoother_t smoother;
    int i;

    lw6gui_smoother_init (sys_context, &smoother, _TEST_SMOOTHER_VALUE1, _TEST_SMOOTHER_DURATION);
    lw6sys_delay (sys_context, _TEST_SMOOTHER_DELAY);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("smoother init %f"),
		lw6gui_smoother_get_value (sys_context, &smoother, lw6sys_get_timestamp (sys_context)));
    lw6gui_smoother_immediate_force (sys_context, &smoother, _TEST_SMOOTHER_VALUE2);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("smoother force %f"),
		lw6gui_smoother_get_value (sys_context, &smoother, lw6sys_get_timestamp (sys_context)));
    lw6gui_smoother_set_target (sys_context, &smoother, _TEST_SMOOTHER_VALUE3, lw6sys_get_timestamp (sys_context));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("smoother set target %f"), _TEST_SMOOTHER_VALUE3);
    lw6sys_delay (sys_context, _TEST_SMOOTHER_DELAY);
    lw6gui_smoother_set_target (sys_context, &smoother, _TEST_SMOOTHER_VALUE4, lw6sys_get_timestamp (sys_context));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("smoother set target %f"), _TEST_SMOOTHER_VALUE4);
    for (i = 0; i < _TEST_SMOOTHER_N; i++)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("smoother value is %f"),
		    lw6gui_smoother_get_value (sys_context, &smoother, lw6sys_get_timestamp (sys_context)));
	lw6sys_delay (sys_context, _TEST_SMOOTHER_DELAY);
      }
    lw6gui_smoother_fix_overflow (sys_context, &smoother, _TEST_SMOOTHER_STEP);
  }
  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing video more
 */
static void
_test_video_mode ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_list_t *available = NULL;
    lw6gui_video_mode_t wished = { _TEST_VIDEO_MODE_WISHED_WIDTH, _TEST_VIDEO_MODE_WISHED_HEIGHT,
      _TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t closest;
    lw6gui_video_mode_t available_1 = { _TEST_VIDEO_MODE_AVAILABLE_1_WIDTH,
      _TEST_VIDEO_MODE_AVAILABLE_1_HEIGHT, _TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t available_2 = { _TEST_VIDEO_MODE_AVAILABLE_2_WIDTH,
      _TEST_VIDEO_MODE_AVAILABLE_2_HEIGHT, _TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t available_3 = { _TEST_VIDEO_MODE_AVAILABLE_3_WIDTH,
      _TEST_VIDEO_MODE_AVAILABLE_3_HEIGHT, _TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t ratio_dst = { _TEST_VIDEO_MODE_RATIO_DST_WIDTH, _TEST_VIDEO_MODE_RATIO_DST_HEIGHT,
      0
    };
    lw6gui_video_mode_t ratio_src = { _TEST_VIDEO_MODE_RATIO_SRC_WIDTH, _TEST_VIDEO_MODE_RATIO_SRC_HEIGHT,
      0
    };

    available = lw6sys_list_new (sys_context, NULL);

    if (available)
      {
	lw6sys_lifo_push (sys_context, &available, &available_1);
	lw6sys_lifo_push (sys_context, &available, &available_2);
	lw6sys_lifo_push (sys_context, &available, &available_3);

	lw6gui_video_mode_find_closest (sys_context, &closest, &wished, available);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("closest video_mode to %dx%d is %dx%d"), wished.width, wished.height, closest.width, closest.height);

	if (lw6gui_video_mode_is_same (sys_context, &available_1, &available_1))
	  {
	    if (!lw6gui_video_mode_is_same (sys_context, &available_2, &available_3))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("video mode comparison works"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("can't compare %dx%d with %dx%d"), available_2.width, available_2.height, available_3.width, available_3.height);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("error comparing %dx%d with itself"), available_1.width, available_1.height);
	    ret = 0;
	  }

	lw6sys_list_free (sys_context, available);
      }
    else
      {
	ret = 0;
      }

    if (lw6gui_video_mode_sync_ratio (sys_context, &ratio_dst, &ratio_src))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("applying ratio %dx%d to %dx%d gives %dx%d"),
		    _TEST_VIDEO_MODE_RATIO_SRC_WIDTH,
		    _TEST_VIDEO_MODE_RATIO_SRC_HEIGHT, _TEST_VIDEO_MODE_RATIO_DST_WIDTH, _TEST_VIDEO_MODE_RATIO_DST_HEIGHT, ratio_dst.width, ratio_dst.height);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_log_zone (lw6sys_context_t * sys_context, lw6gui_zone_t * zone, char *comment)
{
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s x1=%f y1=%f x2=%f y2=%f w=%f h=%f"), comment, zone->x1, zone->y1, zone->x2, zone->y2, zone->w, zone->h);
}

/*
 * Testing viewport
 */
static void
_test_viewport ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_viewport_t viewport;
    int i;
    float map_x = 0.0f;
    float map_y = 0.0f;
    float screen_x = 0.0f;
    float screen_y = 0.0f;
    float global_zoom = 0.0f;

    for (global_zoom = _TEST_VIEWPORT_GLOBAL_ZOOM_MIN; global_zoom < _TEST_VIEWPORT_GLOBAL_ZOOM_MAX; global_zoom *= _TEST_VIEWPORT_GLOBAL_ZOOM_STEP)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing viewport with zoom=%f"), global_zoom);
	lw6gui_viewport_init (sys_context, &viewport, _TEST_VIEWPORT_SCREEN_W,
			      _TEST_VIEWPORT_SCREEN_H,
			      _TEST_VIEWPORT_DRAWABLE_X1,
			      _TEST_VIEWPORT_DRAWABLE_Y1,
			      _TEST_VIEWPORT_DRAWABLE_X2,
			      _TEST_VIEWPORT_DRAWABLE_Y2,
			      _TEST_VIEWPORT_CENTER_X,
			      _TEST_VIEWPORT_CENTER_Y, _TEST_VIEWPORT_MAP_W,
			      _TEST_VIEWPORT_MAP_H, _TEST_VIEWPORT_X_POLARITY,
			      _TEST_VIEWPORT_Y_POLARITY,
			      _TEST_VIEWPORT_X_WRAP, _TEST_VIEWPORT_Y_WRAP,
			      _TEST_VIEWPORT_KEEP_RATIO, global_zoom, _TEST_VIEWPORT_SCROLL_LIMIT, _TEST_VIEWPORT_USE_OLD_CENTER);
	_log_zone (sys_context, &viewport.drawable, _x_ ("drawable"));
	_log_zone (sys_context, &viewport.map_main, _x_ ("map_main"));
	_log_zone (sys_context, &viewport.map_main_clipped, _x_ ("map_main_clipped"));
	_log_zone (sys_context, &viewport.map_visible, _x_ ("visible"));
	for (i = 0; i < _TEST_VIEWPORT_NB_RANDOM_TRIES; ++i)
	  {
	    map_x = lw6sys_random_float (sys_context, 0.0f, _TEST_VIEWPORT_MAP_W);
	    map_y = lw6sys_random_float (sys_context, 0.0f, _TEST_VIEWPORT_MAP_H);
	    lw6gui_viewport_map_to_screen (sys_context, &viewport, &screen_x, &screen_y, map_x, map_y, _TEST_VIEWPORT_CLIP);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map ( %f , %f ) -> screen ( %f , %f )"), map_x, map_y, screen_x, screen_y);
	  }
	for (i = 0; i < _TEST_VIEWPORT_NB_RANDOM_TRIES; ++i)
	  {
	    screen_x = lw6sys_random_float (sys_context, 0.0f, _TEST_VIEWPORT_SCREEN_W);
	    screen_y = lw6sys_random_float (sys_context, 0.0f, _TEST_VIEWPORT_SCREEN_H);
	    lw6gui_viewport_screen_to_map (sys_context, &viewport, &map_x, &map_y, screen_x, screen_y, _TEST_VIEWPORT_WRAP);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("screen ( %f , %f ) -> map ( %f , %f )"), screen_x, screen_y, map_x, map_y);
	  }
	lw6gui_viewport_calc_drag (sys_context, &viewport, &map_x, &map_y, map_x, map_y, screen_x, screen_y);
      }
  }
  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing zone
 */
static void
_test_zone ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_zone_t zone1;
    lw6gui_zone_t zone2;
    lw6gui_zone_t zone3;

    lw6gui_zone_init_x1y1x2y2 (sys_context, &zone1, _TEST_ZONE1_X1, _TEST_ZONE1_Y1, _TEST_ZONE1_X2, _TEST_ZONE1_Y2);
    _log_zone (sys_context, &zone1, _x_ ("zone1"));
    lw6gui_zone_init_x1y1x2y2 (sys_context, &zone1, _TEST_ZONE2_X, _TEST_ZONE2_Y, _TEST_ZONE2_W, _TEST_ZONE2_H);
    _log_zone (sys_context, &zone2, _x_ ("zone2"));
    lw6gui_zone_clip (sys_context, &zone3, &zone1, &zone2);
    _log_zone (sys_context, &zone3, _x_ ("zone3"));
  }
  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing point, segment, triangle, quad
 */
static void
_test_geometry ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_point_t point;
    lw6gui_segment_t segment;
    lw6gui_triangle_t triangle;
    lw6gui_quad_t quad;
    lw6gui_rect_t rect;

    point.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    point.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    segment.p1.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    segment.p1.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    segment.p2.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    segment.p2.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    triangle.p1.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    triangle.p1.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    triangle.p2.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    triangle.p2.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    triangle.p3.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    triangle.p3.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p1.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p1.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p2.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p2.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p3.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p3.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p4.x = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    quad.p4.y = _TEST_GEOMETRY_POINT_MIN + lw6sys_random (sys_context, _TEST_GEOMETRY_POINT_RANGE);
    lw6gui_rect_init_x1y1x2y2 (sys_context, &rect, _TEST_GEOMETRY_RECT_X1, _TEST_GEOMETRY_RECT_Y1, _TEST_GEOMETRY_RECT_X2, _TEST_GEOMETRY_RECT_Y2);

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_
		("point_is_inside=%d segment_is_inside=%d triangle_is_inside=%d quad_is_inside=%d"),
		lw6gui_point_is_inside_rect (sys_context, point, &rect),
		lw6gui_segment_is_inside_rect (sys_context, &segment, &rect), lw6gui_triangle_is_inside_rect (sys_context, &triangle, &rect),
		lw6gui_quad_is_inside_rect (sys_context, &quad, &rect));
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libgui CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libgui CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6gui_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libgui module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gui_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6hlp_test_register (sys_context, mode);
      lw6cfg_test_register (sys_context, mode);
      lw6map_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6gui", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_button);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_coord);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_input);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_joystick);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_keyboard);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_keypress);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_look);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_menuitem);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_menu);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_mouse);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_power_of_two);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_rect);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_rect_array);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_smoother);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_video_mode);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_viewport);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_zone);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_geometry);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6gui_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @gui module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gui_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
