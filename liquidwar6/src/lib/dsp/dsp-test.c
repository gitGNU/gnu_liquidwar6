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

#include "dsp.h"
#include "dsp-internal.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"
#define _TEST_SLEEP_LONG 1.0f
#define _TEST_SLEEP_SHORT 0.2f
#define _TEST_WIDTH1 640
#define _TEST_HEIGHT1 480
#define _TEST_FULLSCREEN1 0
#define _TEST_WIDTH2 800
#define _TEST_HEIGHT2 600
#define _TEST_FULLSCREEN2 0
#define _TEST_REPEAT_DELAY 200
#define _TEST_REPEAT_INTERVAL 20
#define _TEST_DOUBLE_CLICK_DELAY 400
#define _TEST_AUTO_RELEASE_DELAY 300
#define _TEST_FRAMES_PER_SEC 60
#define _TEST_DISPLAY_BACKGROUND 1
#define _TEST_DISPLAY_MENU 1
#define _TEST_DISPLAY_HUD 1
#define _TEST_DISPLAY_VIEW 1
#define _TEST_MENU_TITLE "This is a title"
#define _TEST_MENU_HELP "This is a help, it's quite long so it's very likely it should be displayed on several lines, anyways, who knows, you can't garantee that"
#define _TEST_MENU_POPUP ""
#define _TEST_MENU_ESC "<-"
#define _TEST_MENU_ENABLE_ESC 1
#define _TEST_MENU_ALLOW_SCROLL 0
#define _TEST_NB_MENUITEMS1 5
#define _TEST_NB_MENUITEMS2 15
#define _TEST_MENUITEM_LABEL_FORMAT "Item %d"
#define _TEST_MENUITEM_TOOLTIP_FORMAT "Item %d"
#define _TEST_MENUITEM_COLORED_INDEX 2
#define _TEST_MAP "subflower/"
#define _TEST_NB_ROUNDS 250
#define _TEST_NODE_ID 0x1234123412341234LL
#define _TEST_CURSOR1_ID 0x1234
#define _TEST_CURSOR2_ID 0x2345
#define _TEST_COLOR1 LW6MAP_TEAM_COLOR_RED
#define _TEST_COLOR2 LW6MAP_TEAM_COLOR_GREEN
#define _TEST_INPUT_TICKS 3000
#define _TEST_LOG_TIMEOUT 5

typedef struct _lw6dsp_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
  lw6dsp_backend_t *backend;
} _lw6dsp_test_data_t;

static _lw6dsp_test_data_t _test_data = { 0, NULL, NULL };

static void
_sleep_and_report (lw6sys_context_t * sys_context, lw6dsp_backend_t * display, float delay)
{
  lw6sys_sleep (sys_context, delay);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
	      _x_
	      ("last_frame_rendering_time=%d, instant_fps=%d, average_fps=%d"),
	      lw6dsp_get_last_frame_rendering_time (sys_context, display), lw6dsp_get_instant_fps (sys_context, display), lw6dsp_get_average_fps (sys_context,
																		  display));
}

static lw6gui_menu_t *
_menu_new (lw6sys_context_t * sys_context, int n)
{
  lw6gui_menu_t *ret = NULL;
  lw6gui_menuitem_t *item = NULL;
  char *label;
  char *tooltip;
  int i;

  ret = lw6gui_menu_new (sys_context, _TEST_MENU_TITLE, _TEST_MENU_HELP, _TEST_MENU_POPUP, _TEST_MENU_ESC, _TEST_MENU_ENABLE_ESC);
  if (ret)
    {
      for (i = 0; i < n; ++i)
	{
	  label = lw6sys_new_sprintf (sys_context, _TEST_MENUITEM_LABEL_FORMAT, i + 1);
	  if (label)
	    {
	      tooltip = lw6sys_new_sprintf (sys_context, _TEST_MENUITEM_TOOLTIP_FORMAT, i + 1);
	      if (tooltip)
		{
		  item = lw6gui_menuitem_new (sys_context, label, tooltip, i + 1, 1, i == 0, i == _TEST_MENUITEM_COLORED_INDEX);
		  if (item)
		    {
		      lw6gui_menu_append (sys_context, ret, item, 0);
		    }
		  LW6SYS_FREE (sys_context, tooltip);
		}
	      LW6SYS_FREE (sys_context, label);
	    }
	}
    }

  return ret;
}

static lw6map_level_t *
_level_new (lw6sys_context_t * sys_context, char *name, int display_width, int display_height, lw6sys_progress_t * progress)
{
  lw6map_level_t *ret = NULL;
  char *map_path = NULL;
  char *user_dir = NULL;
  const int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_progress_begin (sys_context, progress);
  map_path = lw6cfg_unified_get_map_path (sys_context, argc, argv);
  if (map_path)
    {
      user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
      if (user_dir)
	{
	  ret =
	    lw6ldr_read_relative (sys_context, map_path, _TEST_MAP, NULL, NULL,
				  display_width, display_height, LW6LDR_DEFAULT_BENCH_VALUE, LW6LDR_DEFAULT_MAGIC_NUMBER, user_dir, progress);
	  LW6SYS_FREE (sys_context, user_dir);
	}
      LW6SYS_FREE (sys_context, map_path);
    }
  lw6sys_progress_begin (sys_context, progress);

  return ret;
}

static lw6ker_game_struct_t *
_game_struct_new (lw6sys_context_t * sys_context, lw6map_level_t * level, lw6sys_progress_t * progress)
{
  lw6ker_game_struct_t *ret = NULL;

  lw6sys_progress_begin (sys_context, progress);
  ret = lw6ker_game_struct_new (sys_context, level, progress);
  lw6sys_progress_begin (sys_context, progress);

  return ret;
}

static lw6ker_game_state_t *
_game_state_new (lw6sys_context_t * sys_context, lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress)
{
  lw6ker_game_state_t *ret = NULL;

  lw6sys_progress_begin (sys_context, progress);
  ret = lw6ker_game_state_new (sys_context, game_struct, progress);
  if (ret)
    {
      lw6ker_game_state_register_node (sys_context, ret, _TEST_NODE_ID);
      lw6ker_game_state_add_cursor (sys_context, ret, _TEST_NODE_ID, _TEST_CURSOR1_ID, _TEST_COLOR1);
      lw6ker_game_state_add_cursor (sys_context, ret, _TEST_NODE_ID, _TEST_CURSOR2_ID, _TEST_COLOR2);
    }
  lw6sys_progress_begin (sys_context, progress);

  return ret;
}

static lw6pil_pilot_t *
_pilot_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, lw6sys_progress_t * progress)
{
  lw6pil_pilot_t *ret = NULL;

  lw6sys_progress_begin (sys_context, progress);
  ret = lw6pil_pilot_new (sys_context, game_state, lw6pil_seq_random_0 (sys_context), lw6sys_get_timestamp (sys_context), progress);
  if (ret)
    {
      // ok
    }
  lw6sys_progress_begin (sys_context, progress);

  return ret;
}

static void
_test_display ()
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6dsp_backend_t *display = _test_data.backend;
    lw6gui_keypress_t *keypress = NULL;
    lw6dsp_param_t param;
    lw6gui_menu_t *menu = NULL;
    lw6gui_video_mode_t video_mode;
    char *repr = NULL;
    float progress_value = 0.0f;
    lw6sys_progress_t progress;
    int i;
    int ticks;
    int nb_frames = 0;
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6pil_pilot_t *pilot = NULL;

    lw6sys_progress_default (sys_context, &progress, &progress_value);
    lw6sys_progress_begin (sys_context, &progress);
    repr = lw6dsp_repr (sys_context, display);
    if (repr)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("display repr is \"%s\""), repr);
	LW6SYS_FREE (sys_context, repr);
      }
    lw6dsp_param_zero (sys_context, &param);
    param.video_mode.width = _TEST_WIDTH1;
    param.video_mode.height = _TEST_HEIGHT1;
    param.video_mode.fullscreen = _TEST_FULLSCREEN1;
    param.misc.target_fps = _TEST_FRAMES_PER_SEC;
    param.misc.mask =
      LW6GUI_DISPLAY_BACKGROUND | LW6GUI_DISPLAY_PREVIEW |
      LW6GUI_DISPLAY_MAP | LW6GUI_DISPLAY_FIGHTERS |
      LW6GUI_DISPLAY_CURSORS | LW6GUI_DISPLAY_MENU | LW6GUI_DISPLAY_META | LW6GUI_DISPLAY_LOG | LW6GUI_DISPLAY_FPS;
    param.look = lw6gui_look_new (sys_context, NULL);
    if (param.look)
      {
	if (lw6dsp_init (sys_context, display, &param, NULL))
	  {
	    repr = lw6dsp_repr (sys_context, display);
	    if (repr)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("display repr is \"%s\""), repr);
		LW6SYS_FREE (sys_context, repr);
	      }
	    lw6dsp_get_video_mode (sys_context, display, &video_mode);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("video mode is %dx%d fullscreen=%d"), video_mode.width, video_mode.height, video_mode.fullscreen);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("displaying background"));
	    _sleep_and_report (sys_context, display, _TEST_SLEEP_LONG);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("changing resolution"));
	    param.video_mode.width = _TEST_WIDTH2;
	    param.video_mode.height = _TEST_HEIGHT2;
	    param.video_mode.fullscreen = _TEST_FULLSCREEN2;
	    param.misc.repeat_settings.delay = _TEST_REPEAT_DELAY;
	    param.misc.repeat_settings.interval = _TEST_REPEAT_INTERVAL;
	    param.misc.repeat_settings.double_click_delay = _TEST_DOUBLE_CLICK_DELAY;
	    param.misc.repeat_settings.auto_release_delay = _TEST_AUTO_RELEASE_DELAY;
	    param.misc.log_timeout = _TEST_LOG_TIMEOUT;
	    lw6dsp_update (sys_context, display, &param);
	    _sleep_and_report (sys_context, display, _TEST_SLEEP_LONG);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("displaying menu"));
	    param.menu = _menu_new (sys_context, _TEST_NB_MENUITEMS1);
	    if (param.menu)
	      {
		lw6dsp_update (sys_context, display, &param);
		_sleep_and_report (sys_context, display, _TEST_SLEEP_SHORT);
		while (param.menu->nb_items > 0)
		  {
		    lw6gui_menu_remove (sys_context, param.menu, 0, 0);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("displaying menu with %d items"), param.menu->nb_items);
		    lw6dsp_update (sys_context, display, &param);
		    _sleep_and_report (sys_context, display, _TEST_SLEEP_SHORT);
		  }
		lw6gui_menu_free (sys_context, param.menu);
		param.menu = _menu_new (sys_context, _TEST_NB_MENUITEMS2);
		if (param.menu)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("displaying big menu (%d items)"), param.menu->nb_items);
		    lw6dsp_update (sys_context, display, &param);
		    _sleep_and_report (sys_context, display, _TEST_SLEEP_SHORT);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("nb items displayed is %d"), param.menu->nb_items_displayed);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("scrolling to middle of big menu (%d)"), param.menu->nb_items / 2);
		    lw6gui_menu_select (sys_context, param.menu, param.menu->nb_items / 2, _TEST_MENU_ALLOW_SCROLL, 0);
		    lw6dsp_update (sys_context, display, &param);
		    _sleep_and_report (sys_context, display, _TEST_SLEEP_SHORT);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("nb items displayed is %d"), param.menu->nb_items_displayed);
		    menu = param.menu;
		    param.menu = NULL;
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("big menu disappears"));
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("this warning should appear for %d seconds on the screen"), _TEST_LOG_TIMEOUT);
		    lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("this error too"));
		    lw6dsp_update (sys_context, display, &param);
		    _sleep_and_report (sys_context, display, _TEST_SLEEP_SHORT);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("big menu is back again"));
		    param.menu = menu;
		    lw6dsp_update (sys_context, display, &param);
		    _sleep_and_report (sys_context, display, _TEST_SLEEP_SHORT);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("loading a map"));
		    param.misc.progress = &progress_value;
		    lw6dsp_update (sys_context, display, &param);
		    level = _level_new (sys_context, _TEST_MAP, _TEST_WIDTH2, _TEST_HEIGHT2, &progress);
		    if (level)
		      {
			param.level = level;
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("displaying a map"));
			lw6dsp_update (sys_context, display, &param);
			_sleep_and_report (sys_context, display, _TEST_SLEEP_LONG);

			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("build struct"));
			game_struct = _game_struct_new (sys_context, param.level, &progress);
			if (game_struct)
			  {
			    param.game_struct = game_struct;
			    lw6dsp_update (sys_context, display, &param);
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("build state"));
			    game_state = _game_state_new (sys_context, param.game_struct, &progress);
			    if (game_state)
			      {
				param.game_state = game_state;
				lw6dsp_update (sys_context, display, &param);
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("displaying a game"));
				_sleep_and_report (sys_context, display, _TEST_SLEEP_LONG);
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("start play with game state"));
				for (i = 0; i < _TEST_NB_ROUNDS; ++i)
				  {
				    lw6ker_game_state_do_round (sys_context, game_state);
				    lw6dsp_update (sys_context, display, &param);
				  }
				lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("stop play with game state"));
				pilot = _pilot_new (sys_context, game_state, &progress);
				if (pilot)
				  {
				    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("start play with pilot"));
				    _sleep_and_report (sys_context, display, _TEST_SLEEP_LONG);
				    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("stop play with pilot"));
				    param.pilot = NULL;
				    lw6dsp_update (sys_context, display, &param);
				    lw6pil_pilot_free (sys_context, pilot);
				    pilot = NULL;
				  }
				param.game_state = NULL;
				lw6dsp_update (sys_context, display, &param);
				lw6ker_game_state_free (sys_context, game_state);
				game_state = NULL;
				ret = 1;
			      }
			    param.game_struct = NULL;
			    lw6dsp_update (sys_context, display, &param);
			    lw6ker_game_struct_free (sys_context, game_struct);
			    game_struct = NULL;
			  }
			param.level = NULL;
			lw6dsp_update (sys_context, display, &param);
			lw6map_free (sys_context, level);
			level = NULL;
		      }
		    lw6gui_menu_free (sys_context, param.menu);
		    param.menu = NULL;
		  }
	      }
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("checking what happens when replicated objects are freed in the param (menu, for instance)"));
	    _sleep_and_report (sys_context, display, _TEST_SLEEP_SHORT);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("now applying the params, nothing should be displayed but the background"));
	    lw6dsp_update (sys_context, display, &param);
	    _sleep_and_report (sys_context, display, _TEST_SLEEP_LONG);

	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_
			("pumping events, try to press random keys while having the focus on the game window, you should get one log message per keypress, test will end after %d seconds"),
			(int) (_TEST_INPUT_TICKS / LW6SYS_TICKS_PER_SEC));
	    ticks = lw6sys_get_uptime (sys_context);
	    while ((!lw6sys_signal_poll_quit (sys_context)) && lw6sys_get_uptime (sys_context) < ticks + _TEST_INPUT_TICKS)
	      {
		/*
		 * Note, there's no sleep in this sloop, should be CPU greedy,
		 * it's interesting to see how display reacts.
		 */
		lw6dsp_update (sys_context, display, &param);
		keypress = lw6gui_keyboard_pop_keypress (sys_context, &(display->input->keyboard));
		if (keypress)
		  {
		    repr = lw6gui_keypress_repr (sys_context, keypress);
		    if (repr)
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s"), repr);
			LW6SYS_FREE (sys_context, repr);
		      }
		    lw6gui_keypress_free (sys_context, keypress);
		  }
	      }
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("done with events"));

	    nb_frames = lw6dsp_get_nb_frames (sys_context, display);

	    lw6dsp_quit (sys_context, display);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to start display"));
	  }
	lw6gui_look_free (sys_context, param.look);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("frames=%d"), nb_frames);
    lw6dsp_destroy_backend (sys_context, display);
    _test_data.backend = display = NULL;
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init_gl1 ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libdsp-gl1 CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6dsp_create_backend (sys_context, argc, argv, "gl1");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_gl1 ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libdsp-gl1 CUnit test suite"));

  if (_test_data.backend)
    {
      lw6dsp_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_init_gles2 ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libdsp-gles2 CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6dsp_create_backend (sys_context, argc, argv, "gles2");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_gles2 ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libdsp-gles2 CUnit test suite"));

  if (_test_data.backend)
    {
      lw6dsp_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_init_soft ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libdsp-soft CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6dsp_create_backend (sys_context, argc, argv, "soft");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_soft ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libdsp-soft CUnit test suite"));

  if (_test_data.backend)
    {
      lw6dsp_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_init_caca ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libdsp-caca CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6dsp_create_backend (sys_context, argc, argv, "caca");
      if (_test_data.backend)
	{
	  ret = CUE_SUCCESS;
	}
    }

  return ret;
}

static int
_setup_quit_caca ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libdsp-caca CUnit test suite"));

  if (_test_data.backend)
    {
      lw6dsp_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}

/**
 * lw6dsp_test_register
 *
  * @sys_context: global system context
* @mode: test mode (bitmask)
 *
 * Registers all tests for the libdsp module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dsp_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };
  CU_Suite *suite = NULL;
  lw6sys_assoc_t *gfx_backends = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6cfg_test_register (sys_context, mode);
      lw6map_test_register (sys_context, mode);
      lw6ker_test_register (sys_context, mode);
      lw6pil_test_register (sys_context, mode);
      lw6gui_test_register (sys_context, mode);
      lw6vox_test_register (sys_context, mode);
      lw6gfx_test_register (sys_context, mode);
      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
    }

  if (mode & LW6SYS_TEST_MODE_FULL_TEST)
    {
      gfx_backends = lw6gfx_get_backends (sys_context, argc, argv);
      if (gfx_backends)
	{
	  if (lw6sys_assoc_has_key (sys_context, gfx_backends, "gl1"))
	    {
	      suite = CU_add_suite ("lw6dsp-gl1", _setup_init_gl1, _setup_quit_gl1);
	      if (suite)
		{
		  LW6SYS_CUNIT_ADD_TEST (suite, _test_display);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
		  ret = 0;
		}
	    }

	  if (lw6sys_assoc_has_key (sys_context, gfx_backends, "gles2"))
	    {
	      suite = CU_add_suite ("lw6dsp-gles2", _setup_init_gles2, _setup_quit_gles2);
	      if (suite)
		{
		  LW6SYS_CUNIT_ADD_TEST (suite, _test_display);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
		  ret = 0;
		}
	    }

	  if (lw6sys_assoc_has_key (sys_context, gfx_backends, "soft"))
	    {
	      suite = CU_add_suite ("lw6dsp-soft", _setup_init_soft, _setup_quit_soft);
	      if (suite)
		{
		  LW6SYS_CUNIT_ADD_TEST (suite, _test_display);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
		  ret = 0;
		}
	    }

	  if (lw6sys_assoc_has_key (sys_context, gfx_backends, "caca"))
	    {
	      if (!(mode & LW6SYS_TEST_MODE_INTERACTIVE))
		{
		  suite = CU_add_suite ("lw6dsp-caca", _setup_init_caca, _setup_quit_caca);
		  if (suite)
		    {
		      LW6SYS_CUNIT_ADD_TEST (suite, _test_display);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
		      ret = 0;
		    }
		}
	    }

	  lw6sys_assoc_free (sys_context, gfx_backends);
	}
    }

  return ret;
}

/**
 * lw6dsp_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @dsp module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dsp_test_run (lw6sys_context_t * sys_context, int mode)
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
