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

#define TEST_LOOK_KEY1 "dynamic-zoom"
#define TEST_LOOK_VALUE1 "1.23"
#define TEST_LOOK_KEY2 LW6DEF_BACKGROUND_STYLE
#define TEST_LOOK_VALUE2 "void"
#define TEST_LOOK_ZOOM_STEP 1.1
#define TEST_MENUITEM_LABEL1 "My Label"
#define TEST_MENUITEM_LABEL2 "My Label 2"
#define TEST_MENUITEM_LABEL3 "The third label"
#define TEST_MENUITEM_LABEL4 "From outer space"
#define TEST_MENUITEM_TOOLTIP1 "Tooltip explaining what my label means"
#define TEST_MENUITEM_TOOLTIP2 "Tooltip explaining what my label 2 means,\nand does it on 2 lines"
#define TEST_MENUITEM_TOOLTIP3 "The third label is the best label of all, at least this is what this (rather long) tooltip says"
#define TEST_MENUITEM_TOOLTIP4 NULL
#define TEST_MENUITEM_VALUE1 42
#define TEST_MENUITEM_VALUE2 421
#define TEST_MENUITEM_VALUE3 -1
#define TEST_MENUITEM_VALUE4 65535
#define TEST_MENUITEM_NOW 1234
#define TEST_MENU_TITLE1 "My Title"
#define TEST_MENU_TITLE2 "My Title 2"
#define TEST_MENU_HELP1 "Chapi chapo this is the first help"
#define TEST_MENU_HELP2 "2nd help, things change, now let's face it, this is a long string are there few chances it won't be cut somewhere when displayed"
#define TEST_MENU_POPUP1 NULL
#define TEST_MENU_POPUP2 "Be carefull\nt...this is important!\nYou're likely to be eaten by a grue ;)"
#define TEST_MENU_ESC "Esc"
#define TEST_MENU_ENABLE_ESC 1
#define TEST_MENU_ALLOW_SCROLL1 0
#define TEST_MENU_ALLOW_SCROLL2 0
#define TEST_MENU_BREADCRUMBS1 "foo"
#define TEST_MENU_BREADCRUMBS2 "bar"
#define TEST_MAX_DISPLAYED_ITEMS 2
#define TEST_VIDEO_MODE_WISHED_WIDTH 640
#define TEST_VIDEO_MODE_WISHED_HEIGHT 480
#define TEST_VIDEO_MODE_WISHED_FULLSCREEN 1
#define TEST_VIDEO_MODE_AVAILABLE_1_WIDTH 400
#define TEST_VIDEO_MODE_AVAILABLE_1_HEIGHT 300
#define TEST_VIDEO_MODE_AVAILABLE_2_WIDTH 700
#define TEST_VIDEO_MODE_AVAILABLE_2_HEIGHT 525
#define TEST_VIDEO_MODE_AVAILABLE_3_WIDTH 1600
#define TEST_VIDEO_MODE_AVAILABLE_3_HEIGHT 1200
#define TEST_VIDEO_MODE_RATIO_DST_WIDTH 100
#define TEST_VIDEO_MODE_RATIO_DST_HEIGHT 100
#define TEST_VIDEO_MODE_RATIO_SRC_WIDTH 2000
#define TEST_VIDEO_MODE_RATIO_SRC_HEIGHT 500
#define TEST_KEYSYM 123
#define TEST_UNICODE 65
#define TEST_LABEL "fake A letter"
#define TEST_REPEAT_DELAY 100
#define TEST_REPEAT_INTERVAL 10
#define TEST_REPEAT_DOUBLE_CLICK_DELAY 250
#define TEST_MOUSE_X 12
#define TEST_MOUSE_Y 34
#define TEST_JOYSTICK_LIMIT 20
#define TEST_JOYSTICK_X -30
#define TEST_JOYSTICK_Y 50
#define TEST_DST_X0 100.0
#define TEST_DST_Y0 200.0
#define TEST_DST_W 30.0
#define TEST_DST_H 40.0
#define TEST_SRC_X 1.0
#define TEST_SRC_Y 2.0
#define TEST_SRC_X0 10.0
#define TEST_SRC_Y0 20.0
#define TEST_SRC_W 100.0
#define TEST_SRC_H 50.0
#define TEST_RECT1_X1 3.0f
#define TEST_RECT1_Y1 2.0f
#define TEST_RECT1_X2 30.0f
#define TEST_RECT1_Y2 20.0f
#define TEST_RECT2_X -1.0f
#define TEST_RECT2_Y -1.0f
#define TEST_RECT2_W 10.0f
#define TEST_RECT2_H 10.0f
#define TEST_RECT_ARRAY_W 1333
#define TEST_RECT_ARRAY_H 666
#define TEST_RECT_ARRAY_TILE_SIZE 123
#define TEST_RECT_ARRAY_BORDER_SIZE 7
#define TEST_RECT_ARRAY_SOURCE_X 500
#define TEST_RECT_ARRAY_SOURCE_Y 250
#define TEST_RECT_ARRAY_TILE_I 19
#define TEST_RECT_ARRAY_QUAD_P1X -10.0f
#define TEST_RECT_ARRAY_QUAD_P1Y -5.0f
#define TEST_RECT_ARRAY_QUAD_P2X 1.0f
#define TEST_RECT_ARRAY_QUAD_P2Y -4.0f
#define TEST_RECT_ARRAY_QUAD_P3X -8.0f
#define TEST_RECT_ARRAY_QUAD_P3Y 2.0f
#define TEST_RECT_ARRAY_QUAD_P4X -1.0f
#define TEST_RECT_ARRAY_QUAD_P4Y 3.0f
#define TEST_ZONE1_X1 3.0f
#define TEST_ZONE1_Y1 2.0f
#define TEST_ZONE1_X2 30.0f
#define TEST_ZONE1_Y2 20.0f
#define TEST_ZONE2_X -1.0f
#define TEST_ZONE2_Y -1.0f
#define TEST_ZONE2_W 10.0f
#define TEST_ZONE2_H 10.0f
#define TEST_POWER_OF_TWO 37
#define TEST_VIEWPORT_SCREEN_W 800
#define TEST_VIEWPORT_SCREEN_H 600
#define TEST_VIEWPORT_DRAWABLE_X1 5
#define TEST_VIEWPORT_DRAWABLE_Y1 10
#define TEST_VIEWPORT_DRAWABLE_X2 700
#define TEST_VIEWPORT_DRAWABLE_Y2 500
#define TEST_VIEWPORT_CENTER_X 50
#define TEST_VIEWPORT_CENTER_Y 60
#define TEST_VIEWPORT_MAP_W 120
#define TEST_VIEWPORT_MAP_H 80
#define TEST_VIEWPORT_X_POLARITY 1
#define TEST_VIEWPORT_Y_POLARITY 0
#define TEST_VIEWPORT_X_WRAP 1
#define TEST_VIEWPORT_Y_WRAP 1
#define TEST_VIEWPORT_KEEP_RATIO 1
#define TEST_VIEWPORT_SCROLL_LIMIT 0.5f
#define TEST_VIEWPORT_USE_OLD_CENTER 0
#define TEST_VIEWPORT_GLOBAL_ZOOM_MIN 0.1f
#define TEST_VIEWPORT_GLOBAL_ZOOM_MAX 10.0f
#define TEST_VIEWPORT_GLOBAL_ZOOM_STEP 9.9f
#define TEST_VIEWPORT_NB_RANDOM_TRIES 10
#define TEST_VIEWPORT_CLIP 1
#define TEST_VIEWPORT_WRAP 1
#define TEST_SMOOTHER_DURATION 500
#define TEST_SMOOTHER_DELAY 60
#define TEST_SMOOTHER_N 10
#define TEST_SMOOTHER_VALUE1 4.0f
#define TEST_SMOOTHER_VALUE2 100.0f
#define TEST_SMOOTHER_VALUE3 -1000.0f
#define TEST_SMOOTHER_VALUE4 421.0f
#define TEST_SMOOTHER_STEP 100
#define TEST_TIMESTAMP 1000000000000LL
#define TEST_GEOMETRY_POINT_MIN 1
#define TEST_GEOMETRY_POINT_RANGE 10
#define TEST_GEOMETRY_RECT_X1 2
#define TEST_GEOMETRY_RECT_Y1 6
#define TEST_GEOMETRY_RECT_X2 5
#define TEST_GEOMETRY_RECT_Y2 9

/*
 * Testing button
 */
static int
test_button ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int64_t timestamp = TEST_TIMESTAMP;
    lw6gui_button_t button;
    lw6gui_repeat_settings_t repeat_settings;

    repeat_settings.delay = TEST_REPEAT_DELAY;
    repeat_settings.interval = TEST_REPEAT_INTERVAL;
    repeat_settings.double_click_delay = TEST_REPEAT_DOUBLE_CLICK_DELAY;

    memset (&button, 0, sizeof (lw6gui_button_t));

    lw6gui_button_register_down (&button, timestamp++);
    lw6gui_button_register_up (&button);
    if (!lw6gui_button_is_pressed (&button))
      {
	if (lw6gui_button_pop_press (&button))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("button press buffer works"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("button press buffer problem"));
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
	lw6gui_button_update_repeat (&button, &repeat_settings, timestamp);
	if (!lw6gui_button_pop_double_click (&button)
	    && !lw6gui_button_pop_triple_click (&button))
	  {
	    lw6gui_button_register_down (&button, timestamp++);
	    lw6gui_button_update_repeat (&button, &repeat_settings,
					 timestamp);
	    timestamp += 2 * TEST_REPEAT_DOUBLE_CLICK_DELAY;
	    lw6gui_button_update_repeat (&button, &repeat_settings,
					 timestamp);
	    if (lw6gui_button_pop_double_click (&button))
	      {
		if (!lw6gui_button_pop_double_click (&button))
		  {
		    timestamp += 2 * TEST_REPEAT_DOUBLE_CLICK_DELAY;
		    lw6gui_button_register_down (&button, timestamp);
		    lw6gui_button_update_repeat (&button, &repeat_settings,
						 timestamp);
		    timestamp += 2 * TEST_REPEAT_DOUBLE_CLICK_DELAY;
		    lw6gui_button_register_down (&button, timestamp);
		    lw6gui_button_update_repeat (&button, &repeat_settings,
						 timestamp);
		    if (!lw6gui_button_pop_double_click (&button))
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("double click works"));
			ret = 1;
		      }
		  }
	      }
	  }
	if (!ret)
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("double click problem"));
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing coord
 */
static int
test_coord ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    float x = 0.0f;
    float y = 0.0f;

    if (lw6gui_coord_calc_xy
	(&x, &y, TEST_DST_X0, TEST_DST_Y0, TEST_DST_W, TEST_DST_H, TEST_SRC_X,
	 TEST_SRC_Y, TEST_SRC_X0, TEST_SRC_Y0, TEST_SRC_W, TEST_SRC_H))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("calc_xy returned %f,%f (%f,%f/%fx%f) with source %f,%f (%f,%f/%fx%f)"),
		    x, y, TEST_DST_X0, TEST_DST_Y0, TEST_DST_W, TEST_DST_H,
		    TEST_SRC_X, TEST_SRC_Y, TEST_SRC_X0, TEST_SRC_Y0,
		    TEST_SRC_W, TEST_SRC_H);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing input
 */
static int
test_input ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_input_t *src = NULL;
    lw6gui_input_t *dst = NULL;
    lw6gui_keypress_t *keypress = NULL;
    lw6gui_repeat_settings_t repeat_settings;

    src = lw6gui_input_new ();
    if (src)
      {
	lw6gui_input_reset (src);
	lw6gui_input_register_change (src);
	dst = lw6gui_input_new ();
	if (dst)
	  {
	    if (lw6gui_keyboard_register_key_down
		(&(src->keyboard), TEST_KEYSYM, TEST_UNICODE, TEST_LABEL, 0))
	      {
		repeat_settings.delay = TEST_REPEAT_DELAY;
		repeat_settings.interval = TEST_REPEAT_INTERVAL;
		repeat_settings.double_click_delay =
		  TEST_REPEAT_DOUBLE_CLICK_DELAY;
		lw6gui_input_update_repeat (src, &repeat_settings, 0);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("need_sync (before sync) dst=%d src=%d"),
			    lw6gui_input_need_sync (dst),
			    lw6gui_input_need_sync (src));
		if (lw6gui_input_sync (dst, src))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("need_sync (after sync) dst=%d src=%d"),
				lw6gui_input_need_sync (dst),
				lw6gui_input_need_sync (src));
		    keypress =
		      lw6gui_keyboard_pop_keypress (&(dst->keyboard));
		    if (keypress)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("keypress synchronisation is OK"));
			lw6gui_keypress_free (keypress);
		      }
		    else
		      {
			ret = 0;
		      }
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_ ("synchronisation error"));
		    ret = 0;
		  }
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6gui_input_free (dst);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6gui_input_free (src);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing joystick
 */
static int
test_joystick ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_joystick_t joystick;
    lw6gui_move_pad_t move_pad;

    memset (&joystick, 0, sizeof (lw6gui_joystick_t));
    memset (&move_pad, 0, sizeof (lw6gui_move_pad_t));

    if (!lw6gui_joystick_check_index (0) || lw6gui_joystick_check_index (-1)
	|| lw6gui_joystick_check_index (LW6GUI_NB_KEYS))
      {
	ret = 0;
      }
    lw6gui_joystick_update_axis_x (&joystick, TEST_JOYSTICK_X,
				   TEST_JOYSTICK_LIMIT, 0);
    lw6gui_joystick_update_axis_y (&joystick, TEST_JOYSTICK_Y,
				   TEST_JOYSTICK_LIMIT, 0);
    lw6gui_joystick_get_move_pad (&joystick, &move_pad);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("joystick up=%d, down=%d, left=%d, right=%d"),
		move_pad.up, move_pad.down, move_pad.left, move_pad.right);
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing keyboard
 */
static int
test_keyboard ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_input_t *input = NULL;
    lw6gui_keypress_t *keypress = NULL;
    char *repr;
    lw6gui_move_pad_t move_pad;

    memset (&move_pad, 0, sizeof (lw6gui_move_pad_t));

    if (!lw6gui_keyboard_check_keysym (0) || lw6gui_keyboard_check_keysym (-1)
	|| lw6gui_keyboard_check_keysym (LW6GUI_NB_KEYS))
      {
	ret = 0;
      }

    input = lw6gui_input_new ();
    if (input)
      {
	if (lw6gui_keyboard_register_key_down
	    (&(input->keyboard), TEST_KEYSYM, TEST_UNICODE, TEST_LABEL, 0))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("key %d pressed=%d"),
			TEST_KEYSYM,
			lw6gui_keyboard_is_pressed (&(input->keyboard),
						    TEST_KEYSYM));
	    if (lw6gui_keyboard_register_key_up
		(&(input->keyboard), TEST_KEYSYM))
	      {
		keypress = lw6gui_keyboard_pop_keypress (&(input->keyboard));
		if (keypress)
		  {
		    lw6gui_keyboard_get_move_pad (&(input->keyboard),
						  &move_pad);
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("keyboard up=%d, down=%d, left=%d, right=%d"),
				move_pad.up, move_pad.down, move_pad.left,
				move_pad.right);
		    repr = lw6gui_keypress_repr (keypress);
		    if (repr)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("keysym %d popped"),
				    keypress->keysym);
			LW6SYS_FREE (repr);
		      }
		    else
		      {
			ret = 0;
		      }
		    if (lw6gui_keyboard_pop_keypress (&(input->keyboard)))
		      {
			ret = 0;
		      }
		    lw6gui_keypress_free (keypress);
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
	lw6gui_input_free (input);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing keypress
 */
static int
test_keypress ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_keypress_t *keypress = NULL;
    char *repr = NULL;

    keypress = lw6gui_keypress_new (TEST_KEYSYM, TEST_UNICODE, TEST_LABEL);
    if (keypress)
      {
	repr = lw6gui_keypress_repr (keypress);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("keypress repr is \"%s\""),
			repr);
	    LW6SYS_FREE (repr);
	  }
	lw6gui_keypress_free (keypress);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * Testing look
 */
static int
test_look ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_style_t style;
    lw6gui_look_t *look = NULL;
    lw6gui_look_t *look2 = NULL;
    char *repr = NULL;
    char *value = NULL;

    lw6map_style_zero (&style);
    lw6map_style_defaults (&style);
    look = lw6gui_look_new (&style);
    if (look)
      {
	repr = lw6gui_look_repr (look);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("look repr is %s"), repr);
	    LW6SYS_FREE (repr);
	  }
	if (!lw6gui_look_is_same (look, look))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("look comparison failed"));
	    ret = 0;
	  }
	if (lw6gui_look_set (look, TEST_LOOK_KEY1, TEST_LOOK_VALUE1))
	  {
	    value = lw6gui_look_get (look, TEST_LOOK_KEY1);
	    if (value)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("look value for key \"%s\" is \"%s\""),
			    TEST_LOOK_KEY1, value);
		LW6SYS_FREE (value);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("unable to get look key \"%s\""),
			    TEST_LOOK_KEY1);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("unable to set look key \"%s\" to \"%s\""),
			TEST_LOOK_KEY1, TEST_LOOK_VALUE1);
	    ret = 0;
	  }
	if (lw6gui_look_set (look, TEST_LOOK_KEY2, TEST_LOOK_VALUE2))
	  {
	    value = lw6gui_look_get (look, TEST_LOOK_KEY2);
	    if (value)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("look value for key \"%s\" is \"%s\""),
			    TEST_LOOK_KEY2, value);
		LW6SYS_FREE (value);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("unable to get look key \"%s\""),
			    TEST_LOOK_KEY2);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("unable to set look key \"%s\" to \"%s\""),
			TEST_LOOK_KEY2, TEST_LOOK_VALUE2);
	    ret = 0;
	  }
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("zoom is %f"),
		    look->dynamic_zoom);
	lw6gui_look_zoom_in (look, TEST_LOOK_ZOOM_STEP);
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("after zoom in zoom is %f"),
		    look->dynamic_zoom);
	lw6gui_look_zoom_out (look, TEST_LOOK_ZOOM_STEP);
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("after zoom out zoom is %f"),
		    look->dynamic_zoom);
	look2 = lw6gui_look_dup (look);
	if (look2)
	  {
	    repr = lw6gui_look_repr (look2);
	    if (repr)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("duplicated, look2 repr is %s"), repr);
		LW6SYS_FREE (repr);
	      }
	    lw6gui_look_free (look2);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to duplicate look"));
	    ret = 0;
	  }
	lw6gui_look_free (look);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create look"));
	ret = 0;
      }
    lw6map_style_clear (&style);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing menuitemm
 */
static int
test_menuitem ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_menuitem_t *menuitem = NULL;
    lw6gui_menuitem_t *menuitem2 = NULL;
    char *repr;

    menuitem =
      lw6gui_menuitem_new (TEST_MENUITEM_LABEL1, TEST_MENUITEM_TOOLTIP1,
			   TEST_MENUITEM_VALUE1, 1, 0, 0);
    if (menuitem)
      {
	lw6gui_menuitem_set_label (menuitem, TEST_MENUITEM_LABEL2,
				   TEST_MENUITEM_NOW);
	lw6gui_menuitem_set_tooltip (menuitem, TEST_MENUITEM_TOOLTIP2,
				     TEST_MENUITEM_NOW);
	lw6gui_menuitem_set_value (menuitem, TEST_MENUITEM_VALUE2,
				   TEST_MENUITEM_NOW);
	lw6gui_menuitem_select (menuitem, 1, TEST_MENUITEM_NOW);
	lw6gui_menuitem_select (menuitem, 0, TEST_MENUITEM_NOW);
	repr = lw6gui_menuitem_repr (menuitem);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("menuitem repr is %s"), repr);
	    LW6SYS_FREE (repr);
	  }
	else
	  {
	    ret = 0;
	  }
	if (!lw6gui_menuitem_is_same (menuitem, menuitem))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("menuitem comparison failed"));
	  }
	menuitem2 = lw6gui_menuitem_dup (menuitem);
	if (menuitem2)
	  {
	    if (lw6gui_menuitem_is_same (menuitem, menuitem2))
	      {
		lw6gui_menuitem_sync (menuitem2, menuitem);
		repr = lw6gui_menuitem_repr (menuitem2);
		if (repr)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("menuitem duplicate repr is %s"), repr);
		    LW6SYS_FREE (repr);
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
	    lw6gui_menuitem_free (menuitem2);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6gui_menuitem_free (menuitem);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing menu
 */
static int
test_menu ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_menu_t *menu = NULL;
    char *repr;

    menu =
      lw6gui_menu_new (TEST_MENU_TITLE1, TEST_MENU_HELP1, TEST_MENU_POPUP1,
		       TEST_MENU_ESC, TEST_MENU_ENABLE_ESC);
    if (menu)
      {
	lw6gui_menu_set_title (menu, TEST_MENU_TITLE2);
	lw6gui_menu_set_help (menu, TEST_MENU_HELP2);
	lw6gui_menu_set_popup (menu, TEST_MENU_POPUP2);
	lw6gui_menu_close_popup (menu);
	if (!lw6gui_menu_has_popup (menu))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("no popup after close, OK"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("popup reported to exist after being closed"));
	    ret = 0;
	  }
	lw6gui_menu_set_popup (menu, TEST_MENU_POPUP2);
	if (lw6gui_menu_has_popup (menu))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("popup defined, OK"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("no popup reported when it's just been set"));
	    ret = 0;
	  }
	repr = lw6gui_menu_repr (menu);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("menu repr is %s"), repr);
	    LW6SYS_FREE (repr);
	  }
	else
	  {
	    ret = 0;
	  }
	if (!lw6gui_menu_is_same (menu, menu))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("menu comparison failed"));
	  }
	lw6gui_menu_free (menu);
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

    breadcrumbs = lw6sys_list_new (lw6sys_free_callback);
    if (breadcrumbs)
      {
	lw6sys_list_push_back (&breadcrumbs,
			       lw6sys_str_copy (TEST_MENU_BREADCRUMBS1));
	lw6sys_list_push_back (&breadcrumbs,
			       lw6sys_str_copy (TEST_MENU_BREADCRUMBS2));
      }
    if (breadcrumbs)
      {
	menu =
	  lw6gui_menu_new (TEST_MENU_TITLE1, TEST_MENU_HELP1,
			   TEST_MENU_POPUP1, TEST_MENU_ESC,
			   TEST_MENU_ENABLE_ESC);
	if (menu)
	  {
	    lw6gui_menu_set_title (menu, TEST_MENU_TITLE2);
	    lw6gui_menu_set_breadcrumbs (menu, breadcrumbs);
	    menuitem1 =
	      lw6gui_menuitem_new (TEST_MENUITEM_LABEL1,
				   TEST_MENUITEM_TOOLTIP1,
				   TEST_MENUITEM_VALUE1, 1, 0, 0);
	    if (menuitem1)
	      {
		menuitem2 =
		  lw6gui_menuitem_new (TEST_MENUITEM_LABEL2,
				       TEST_MENUITEM_TOOLTIP2,
				       TEST_MENUITEM_VALUE2, 1, 0, 0);
		if (menuitem2)
		  {
		    menuitem3 =
		      lw6gui_menuitem_new (TEST_MENUITEM_LABEL3,
					   TEST_MENUITEM_TOOLTIP3,
					   TEST_MENUITEM_VALUE3, 1, 0, 0);
		    if (menuitem3)
		      {
			lw6gui_menu_append (menu, menuitem1, 0);
			lw6gui_menu_insert (menu, menuitem2, 0, 0);
			lw6gui_menu_insert (menu, menuitem3, 1, 0);
			lw6gui_menu_select (menu, 2, TEST_MENU_ALLOW_SCROLL1,
					    0);
			lw6gui_menu_remove (menu, 0, 0);
			lw6gui_menu_select (menu, 0, TEST_MENU_ALLOW_SCROLL2,
					    0);
			menuitem_id =
			  lw6gui_menu_append_for_id_use (menu,
							 TEST_MENUITEM_LABEL4,
							 TEST_MENUITEM_TOOLTIP4,
							 TEST_MENUITEM_VALUE4,
							 1, 0, 0, 0);
			if (menuitem_id)
			  {
			    lw6gui_menu_sync_using_id (menu, menuitem_id,
						       TEST_MENUITEM_LABEL1,
						       TEST_MENUITEM_VALUE1,
						       1, 0, 0, 0);
			    lw6gui_menu_remove_using_id (menu, menuitem_id,
							 0);
			  }
			lw6gui_menu_update_display_range (menu,
							  TEST_MAX_DISPLAYED_ITEMS);
			lw6gui_menu_center (menu, 1,
					    TEST_MAX_DISPLAYED_ITEMS);
			repr = lw6gui_menu_repr (menu);
			if (repr)
			  {
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_ ("menu repr is %s"), repr);
			    LW6SYS_FREE (repr);
			  }
			else
			  {
			    ret = 0;
			  }
			menu2 = lw6gui_menu_dup (menu);
			if (menu2)
			  {
			    if (lw6gui_menu_is_same (menu, menu2))
			      {
				lw6gui_menu_sync (menu2, menu);
				repr = lw6gui_menu_repr (menu2);
				if (repr)
				  {
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_
						("menu duplicate repr is %s"),
						repr);
				    LW6SYS_FREE (repr);
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
			    lw6gui_menu_free (menu2);
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
	    lw6gui_menu_free (menu);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6sys_list_free (breadcrumbs);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing mouse
 */
static int
test_mouse ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_mouse_t mouse;
    int x = 0, y = 0;
    int dx = 0, dy = 0, px = 0, py = 0, sx = 0, sy = 0;

    memset (&mouse, 0, sizeof (lw6gui_mouse_t));

    lw6gui_mouse_register_move (&mouse, TEST_MOUSE_X, TEST_MOUSE_Y, 0);
    lw6gui_mouse_drag_begin (&mouse);
    lw6gui_mouse_drag_end (&mouse);
    lw6gui_mouse_drag_pop (&mouse, &dx, &dy, &px, &py, &sx, &sy);

    if (lw6gui_mouse_poll_move (&mouse, &x, &y))
      {
	if (x == TEST_MOUSE_X && y == TEST_MOUSE_Y)
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

  return ret;
}

static void
log_rect (lw6gui_rect_t * rect, char *comment)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("%s x1=%d y1=%d x2=%d y2=%d w=%d h=%d"),
	      comment, rect->x1, rect->y1, rect->x2, rect->y2, rect->w,
	      rect->h);
}

/*
 * Testing poweroftwo
 */
static int
test_power_of_two ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("input=%d power_of_two_le=%d power_of_two_ge=%d"),
		TEST_POWER_OF_TWO, lw6gui_power_of_two_le (TEST_POWER_OF_TWO),
		lw6gui_power_of_two_ge (TEST_POWER_OF_TWO));
  }
  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing rect
 */
static int
test_rect ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_rect_t rect1;
    lw6gui_rect_t rect2;
    lw6gui_rect_t rect3;

    lw6gui_rect_init_x1y1x2y2 (&rect1, TEST_RECT1_X1, TEST_RECT1_Y1,
			       TEST_RECT1_X2, TEST_RECT1_Y2);
    log_rect (&rect1, _x_ ("rect1"));
    lw6gui_rect_init_x1y1x2y2 (&rect1, TEST_RECT2_X, TEST_RECT2_Y,
			       TEST_RECT2_W, TEST_RECT2_H);
    log_rect (&rect2, _x_ ("rect2"));
    lw6gui_rect_clip (&rect3, &rect1, &rect2);
    log_rect (&rect3, _x_ ("rect3"));
  }
  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing rect array
 */
static int
test_rect_array ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_rect_array_t rect_array;
    lw6gui_rect_t rect;
    int i = 0;
    int x_polarity, y_polarity;
    lw6gui_quad_t quad;
    lw6gui_quad_t source_quad;

    lw6gui_rect_array_init (&rect_array, TEST_RECT_ARRAY_W, TEST_RECT_ARRAY_H,
			    TEST_RECT_ARRAY_TILE_SIZE,
			    TEST_RECT_ARRAY_BORDER_SIZE);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("rect_array source=%dx%d limits=(%d,%d),(%d,%d) tile_size=%d border_size=%d tile_spacing=%d nb_tiles_w=%d nb_tiles_h=%d nb_tiles=%d"),
		rect_array.source.w, rect_array.source.h,
		rect_array.limits.x1, rect_array.limits.y1,
		rect_array.limits.x2, rect_array.limits.y2,
		rect_array.tile_size, rect_array.border_size,
		rect_array.tile_spacing, rect_array.nb_tiles_w,
		rect_array.nb_tiles_h, rect_array.nb_tiles);
    if (lw6gui_rect_array_get_tile_by_source_xy
	(&rect_array, &rect, &i, TEST_RECT_ARRAY_SOURCE_X,
	 TEST_RECT_ARRAY_SOURCE_Y))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("tile for source %d,%d is index %d corners=(%d,%d),(%d,%d)"),
		    TEST_RECT_ARRAY_SOURCE_X, TEST_RECT_ARRAY_SOURCE_Y, i,
		    rect.x1, rect.y1, rect.x2, rect.y2);
	if (lw6gui_rect_array_get_tile_by_i
	    (&rect_array, &rect, TEST_RECT_ARRAY_TILE_I))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("tile i=%d corners=(%d,%d),(%d,%d)"),
			TEST_RECT_ARRAY_TILE_I, rect.x1, rect.y1, rect.x2,
			rect.y2);
	    source_quad.p1.x = TEST_RECT_ARRAY_QUAD_P1X;
	    source_quad.p1.y = TEST_RECT_ARRAY_QUAD_P1Y;
	    source_quad.p2.x = TEST_RECT_ARRAY_QUAD_P2X;
	    source_quad.p2.y = TEST_RECT_ARRAY_QUAD_P2Y;
	    source_quad.p3.x = TEST_RECT_ARRAY_QUAD_P3X;
	    source_quad.p3.y = TEST_RECT_ARRAY_QUAD_P3Y;
	    source_quad.p4.x = TEST_RECT_ARRAY_QUAD_P4X;
	    source_quad.p4.y = TEST_RECT_ARRAY_QUAD_P4Y;
	    for (y_polarity = LW6MAP_RULES_MIN_Y_POLARITY;
		 y_polarity <= LW6MAP_RULES_MAX_Y_POLARITY; ++y_polarity)
	      {
		for (x_polarity = LW6MAP_RULES_MIN_X_POLARITY;
		     x_polarity <= LW6MAP_RULES_MAX_X_POLARITY; ++x_polarity)
		  {
		    if (lw6gui_rect_array_get_tile_and_quad
			(&rect_array, &rect, &i, &quad, &source_quad,
			 x_polarity, y_polarity))
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("polarity x=%d y=%d tile corners=(%d,%d),(%d,%d) quad=(%f,%f),(%f,%f),(%f,%f),(%f,%f)"),
				    x_polarity, y_polarity, rect.x1, rect.y1,
				    rect.x2, rect.y2, quad.p1.x, quad.p1.y,
				    quad.p2.x, quad.p2.y, quad.p3.x,
				    quad.p3.y, quad.p4.x, quad.p4.y);
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
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("unable to find tile for i=%d"),
			TEST_RECT_ARRAY_TILE_I);
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("unable to find tile for source %d,%d"),
		    TEST_RECT_ARRAY_SOURCE_X, TEST_RECT_ARRAY_SOURCE_Y);
	ret = 0;
      }
    /*
       #define TEST_RECT_ARRAY_SOURCE_X 500
       #define TEST_RECT_ARRAY_SOURCE_Y 250
       #define TEST_RECT_ARRAY_TILE_I 19
       #define TEST_RECT_ARRAY_QUAD_P1X 2000.0f
       #define TEST_RECT_ARRAY_QUAD_P1Y 3000.0f
       #define TEST_RECT_ARRAY_QUAD_P2X 2100.0f
       #define TEST_RECT_ARRAY_QUAD_P2Y 3100.0f
       #define TEST_RECT_ARRAY_QUAD_P3X 2050.0f
       #define TEST_RECT_ARRAY_QUAD_P3Y 3100.0f
       #define TEST_RECT_ARRAY_QUAD_P4X 2050.0f
       #define TEST_RECT_ARRAY_QUAD_P4Y 2100.0f
       #define TEST_RECT_ARRAY_X_POLARITY -1
       #define TEST_RECT_ARRAY_Y_POLARITY 1
     */
  }
  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing video more
 */
static int
test_smoother ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_smoother_t smoother;
    int i;

    lw6gui_smoother_init (&smoother, TEST_SMOOTHER_VALUE1,
			  TEST_SMOOTHER_DURATION);
    lw6sys_delay (TEST_SMOOTHER_DELAY);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("smoother init %f"),
		lw6gui_smoother_get_value (&smoother,
					   lw6sys_get_timestamp ()));
    lw6gui_smoother_immediate_force (&smoother, TEST_SMOOTHER_VALUE2);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("smoother force %f"),
		lw6gui_smoother_get_value (&smoother,
					   lw6sys_get_timestamp ()));
    lw6gui_smoother_set_target (&smoother, TEST_SMOOTHER_VALUE3,
				lw6sys_get_timestamp ());
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("smoother set target %f"),
		TEST_SMOOTHER_VALUE3);
    lw6sys_delay (TEST_SMOOTHER_DELAY);
    lw6gui_smoother_set_target (&smoother, TEST_SMOOTHER_VALUE4,
				lw6sys_get_timestamp ());
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("smoother set target %f"),
		TEST_SMOOTHER_VALUE4);
    for (i = 0; i < TEST_SMOOTHER_N; i++)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("smoother value is %f"),
		    lw6gui_smoother_get_value (&smoother,
					       lw6sys_get_timestamp ()));
	lw6sys_delay (TEST_SMOOTHER_DELAY);
      }
    lw6gui_smoother_fix_overflow (&smoother, TEST_SMOOTHER_STEP);
  }
  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing video more
 */
static int
test_video_mode ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_list_t *available = NULL;
    lw6gui_video_mode_t wished =
      { TEST_VIDEO_MODE_WISHED_WIDTH, TEST_VIDEO_MODE_WISHED_HEIGHT,
      TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t closest;
    lw6gui_video_mode_t available_1 = { TEST_VIDEO_MODE_AVAILABLE_1_WIDTH,
      TEST_VIDEO_MODE_AVAILABLE_1_HEIGHT, TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t available_2 = { TEST_VIDEO_MODE_AVAILABLE_2_WIDTH,
      TEST_VIDEO_MODE_AVAILABLE_2_HEIGHT, TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t available_3 = { TEST_VIDEO_MODE_AVAILABLE_3_WIDTH,
      TEST_VIDEO_MODE_AVAILABLE_3_HEIGHT, TEST_VIDEO_MODE_WISHED_FULLSCREEN
    };
    lw6gui_video_mode_t ratio_dst =
      { TEST_VIDEO_MODE_RATIO_DST_WIDTH, TEST_VIDEO_MODE_RATIO_DST_HEIGHT,
      0
    };
    lw6gui_video_mode_t ratio_src =
      { TEST_VIDEO_MODE_RATIO_SRC_WIDTH, TEST_VIDEO_MODE_RATIO_SRC_HEIGHT,
      0
    };

    available = lw6sys_list_new (NULL);

    if (available)
      {
	lw6sys_lifo_push (&available, &available_1);
	lw6sys_lifo_push (&available, &available_2);
	lw6sys_lifo_push (&available, &available_3);

	lw6gui_video_mode_find_closest (&closest, &wished, available);
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("closest video_mode to %dx%d is %dx%d"),
		    wished.width, wished.height, closest.width,
		    closest.height);

	if (lw6gui_video_mode_is_same (&available_1, &available_1))
	  {
	    if (!lw6gui_video_mode_is_same (&available_2, &available_3))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("video mode comparison works"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("can't compare %dx%d with %dx%d"),
			    available_2.width, available_2.height,
			    available_3.width, available_3.height);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("error comparing %dx%d with itself"),
			available_1.width, available_1.height);
	    ret = 0;
	  }

	lw6sys_list_free (available);
      }
    else
      {
	ret = 0;
      }

    if (lw6gui_video_mode_sync_ratio (&ratio_dst, &ratio_src))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("applying ratio %dx%d to %dx%d gives %dx%d"),
		    TEST_VIDEO_MODE_RATIO_SRC_WIDTH,
		    TEST_VIDEO_MODE_RATIO_SRC_HEIGHT,
		    TEST_VIDEO_MODE_RATIO_DST_WIDTH,
		    TEST_VIDEO_MODE_RATIO_DST_HEIGHT, ratio_dst.width,
		    ratio_dst.height);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
log_zone (lw6gui_zone_t * zone, char *comment)
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("%s x1=%f y1=%f x2=%f y2=%f w=%f h=%f"),
	      comment, zone->x1, zone->y1, zone->x2, zone->y2, zone->w,
	      zone->h);
}

/*
 * Testing viewport
 */
static int
test_viewport ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_viewport_t viewport;
    int i;
    float map_x = 0.0f;
    float map_y = 0.0f;
    float screen_x = 0.0f;
    float screen_y = 0.0f;
    float global_zoom = 0.0f;

    for (global_zoom = TEST_VIEWPORT_GLOBAL_ZOOM_MIN;
	 global_zoom < TEST_VIEWPORT_GLOBAL_ZOOM_MAX;
	 global_zoom *= TEST_VIEWPORT_GLOBAL_ZOOM_STEP)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("testing viewport with zoom=%f"),
		    global_zoom);
	lw6gui_viewport_init (&viewport, TEST_VIEWPORT_SCREEN_W,
			      TEST_VIEWPORT_SCREEN_H,
			      TEST_VIEWPORT_DRAWABLE_X1,
			      TEST_VIEWPORT_DRAWABLE_Y1,
			      TEST_VIEWPORT_DRAWABLE_X2,
			      TEST_VIEWPORT_DRAWABLE_Y2,
			      TEST_VIEWPORT_CENTER_X, TEST_VIEWPORT_CENTER_Y,
			      TEST_VIEWPORT_MAP_W, TEST_VIEWPORT_MAP_H,
			      TEST_VIEWPORT_X_POLARITY,
			      TEST_VIEWPORT_Y_POLARITY,
			      TEST_VIEWPORT_X_WRAP,
			      TEST_VIEWPORT_Y_WRAP,
			      TEST_VIEWPORT_KEEP_RATIO, global_zoom,
			      TEST_VIEWPORT_SCROLL_LIMIT,
			      TEST_VIEWPORT_USE_OLD_CENTER);
	log_zone (&viewport.drawable, _x_ ("drawable"));
	log_zone (&viewport.map_main, _x_ ("map_main"));
	log_zone (&viewport.map_main_clipped, _x_ ("map_main_clipped"));
	log_zone (&viewport.map_visible, _x_ ("visible"));
	for (i = 0; i < TEST_VIEWPORT_NB_RANDOM_TRIES; ++i)
	  {
	    map_x = lw6sys_random_float (0.0f, TEST_VIEWPORT_MAP_W);
	    map_y = lw6sys_random_float (0.0f, TEST_VIEWPORT_MAP_H);
	    lw6gui_viewport_map_to_screen (&viewport, &screen_x, &screen_y,
					   map_x, map_y, TEST_VIEWPORT_CLIP);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("map ( %f , %f ) -> screen ( %f , %f )"), map_x,
			map_y, screen_x, screen_y);
	  }
	for (i = 0; i < TEST_VIEWPORT_NB_RANDOM_TRIES; ++i)
	  {
	    screen_x = lw6sys_random_float (0.0f, TEST_VIEWPORT_SCREEN_W);
	    screen_y = lw6sys_random_float (0.0f, TEST_VIEWPORT_SCREEN_H);
	    lw6gui_viewport_screen_to_map (&viewport, &map_x, &map_y,
					   screen_x, screen_y,
					   TEST_VIEWPORT_WRAP);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("screen ( %f , %f ) -> map ( %f , %f )"),
			screen_x, screen_y, map_x, map_y);
	  }
	lw6gui_viewport_calc_drag (&viewport, &map_x, &map_y, map_x, map_y,
				   screen_x, screen_y);
      }
  }
  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing zone
 */
static int
test_zone ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_zone_t zone1;
    lw6gui_zone_t zone2;
    lw6gui_zone_t zone3;

    lw6gui_zone_init_x1y1x2y2 (&zone1, TEST_ZONE1_X1, TEST_ZONE1_Y1,
			       TEST_ZONE1_X2, TEST_ZONE1_Y2);
    log_zone (&zone1, _x_ ("zone1"));
    lw6gui_zone_init_x1y1x2y2 (&zone1, TEST_ZONE2_X, TEST_ZONE2_Y,
			       TEST_ZONE2_W, TEST_ZONE2_H);
    log_zone (&zone2, _x_ ("zone2"));
    lw6gui_zone_clip (&zone3, &zone1, &zone2);
    log_zone (&zone3, _x_ ("zone3"));
  }
  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing point, segment, triangle, quad
 */
static int
test_geometry ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_point_t point;
    lw6gui_segment_t segment;
    lw6gui_triangle_t triangle;
    lw6gui_quad_t quad;
    lw6gui_rect_t rect;

    point.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    point.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    segment.p1.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    segment.p1.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    segment.p2.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    segment.p2.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    triangle.p1.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    triangle.p1.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    triangle.p2.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    triangle.p2.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    triangle.p3.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    triangle.p3.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p1.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p1.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p2.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p2.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p3.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p3.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p4.x =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    quad.p4.y =
      TEST_GEOMETRY_POINT_MIN + lw6sys_random (TEST_GEOMETRY_POINT_RANGE);
    lw6gui_rect_init_x1y1x2y2 (&rect, TEST_GEOMETRY_RECT_X1,
			       TEST_GEOMETRY_RECT_Y1, TEST_GEOMETRY_RECT_X2,
			       TEST_GEOMETRY_RECT_Y2);

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("point_is_inside=%d segment_is_inside=%d triangle_is_inside=%d quad_is_inside=%d"),
		lw6gui_point_is_inside_rect (point, &rect),
		lw6gui_segment_is_inside_rect (&segment, &rect),
		lw6gui_triangle_is_inside_rect (&triangle, &rect),
		lw6gui_quad_is_inside_rect (&quad, &rect));
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6gui_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Run tests in the gui module.
 * 
 * Return value: 1 if successfull, 0 if failed.
 */
int
lw6gui_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6hlp_test (mode);
      lw6cfg_test (mode);
      lw6map_test (mode);
    }

  ret = test_button () && test_coord () && test_input () && test_joystick ()
    && test_keyboard () && test_keypress ()
    && test_look () && test_menuitem () && test_menu () && test_mouse ()
    && test_power_of_two () && test_rect () && test_rect_array ()
    && test_smoother () && test_video_mode () && test_viewport ()
    && test_zone () && test_geometry ();

  return ret;
}
