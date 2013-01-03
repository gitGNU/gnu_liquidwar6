/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#include "dsp.h"
#include "dsp-internal.h"

#define TEST_ARGC 1
#define TEST_ARGV0 "prog"
#define TEST_SLEEP_LONG 1.0f
#define TEST_SLEEP_SHORT 0.2f
#define TEST_WIDTH1 640
#define TEST_HEIGHT1 480
#define TEST_FULLSCREEN1 0
#define TEST_WIDTH2 800
#define TEST_HEIGHT2 600
#define TEST_FULLSCREEN2 0
#define TEST_REPEAT_DELAY 200
#define TEST_REPEAT_INTERVAL 20
#define TEST_DOUBLE_CLICK_DELAY 400
#define TEST_FRAMES_PER_SEC 60
#define TEST_DISPLAY_BACKGROUND 1
#define TEST_DISPLAY_MENU 1
#define TEST_DISPLAY_HUD 1
#define TEST_DISPLAY_VIEW 1
#define TEST_MENU_TITLE "This is a title"
#define TEST_MENU_HELP "This is a help, it's quite long so it's very likely it should be displayed on several lines, anyways, who knows, you can't garantee that"
#define TEST_MENU_POPUP ""
#define TEST_MENU_ESC "<-"
#define TEST_MENU_ENABLE_ESC 1
#define TEST_MENU_ALLOW_SCROLL 0
#define TEST_NB_MENUITEMS1 5
#define TEST_NB_MENUITEMS2 15
#define TEST_MENUITEM_LABEL_FORMAT "Item %d"
#define TEST_MENUITEM_TOOLTIP_FORMAT "Item %d"
#define TEST_MENUITEM_COLORED_INDEX 2
#define TEST_MAP "subflower/"
#define TEST_NB_ROUNDS 250
#define TEST_NODE_ID 0x1234123412341234LL
#define TEST_CURSOR1_ID 0x1234
#define TEST_CURSOR2_ID 0x2345
#define TEST_COLOR1 LW6MAP_TEAM_COLOR_RED
#define TEST_COLOR2 LW6MAP_TEAM_COLOR_GREEN
#define TEST_INPUT_TICKS 3000
#define TEST_LOG_TIMEOUT 5

static void
sleep_and_report (lw6dsp_backend_t * display, float delay)
{
  lw6sys_sleep (delay);
  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _x_
	      ("last_frame_rendering_time=%d, instant_fps=%d, average_fps=%d"),
	      lw6dsp_get_last_frame_rendering_time (display),
	      lw6dsp_get_instant_fps (display),
	      lw6dsp_get_average_fps (display));
}

static lw6gui_menu_t *
menu_new (int n)
{
  lw6gui_menu_t *ret = NULL;
  lw6gui_menuitem_t *item = NULL;
  char *label;
  char *tooltip;
  int i;

  ret =
    lw6gui_menu_new (TEST_MENU_TITLE, TEST_MENU_HELP, TEST_MENU_POPUP,
		     TEST_MENU_ESC, TEST_MENU_ENABLE_ESC);
  if (ret)
    {
      for (i = 0; i < n; ++i)
	{
	  label = lw6sys_new_sprintf (TEST_MENUITEM_LABEL_FORMAT, i + 1);
	  if (label)
	    {
	      tooltip =
		lw6sys_new_sprintf (TEST_MENUITEM_TOOLTIP_FORMAT, i + 1);
	      if (tooltip)
		{
		  item =
		    lw6gui_menuitem_new (label, tooltip, i + 1, 1, i == 0,
					 i == TEST_MENUITEM_COLORED_INDEX);
		  if (item)
		    {
		      lw6gui_menu_append (ret, item, 0);
		    }
		  LW6SYS_FREE (tooltip);
		}
	      LW6SYS_FREE (label);
	    }
	}
    }

  return ret;
}

static lw6map_level_t *
level_new (char *name, int display_width, int display_height,
	   lw6sys_progress_t * progress)
{
  lw6map_level_t *ret = NULL;
  char *map_path = NULL;
  char *user_dir = NULL;
  const int argc = TEST_ARGC;
  const char *argv[TEST_ARGC] = { TEST_ARGV0 };

  lw6sys_progress_begin (progress);
  map_path = lw6cfg_unified_get_map_path (argc, argv);
  if (map_path)
    {
      user_dir = lw6sys_get_user_dir (argc, argv);
      if (user_dir)
	{
	  ret =
	    lw6ldr_read_relative (map_path, TEST_MAP, NULL, NULL,
				  display_width, display_height,
				  LW6LDR_DEFAULT_BENCH_VALUE,
				  LW6LDR_DEFAULT_MAGIC_NUMBER, user_dir,
				  progress);
	  LW6SYS_FREE (user_dir);
	}
      LW6SYS_FREE (map_path);
    }
  lw6sys_progress_begin (progress);

  return ret;
}

static lw6ker_game_struct_t *
game_struct_new (lw6map_level_t * level, lw6sys_progress_t * progress)
{
  lw6ker_game_struct_t *ret = NULL;

  lw6sys_progress_begin (progress);
  ret = lw6ker_game_struct_new (level, progress);
  lw6sys_progress_begin (progress);

  return ret;
}

static lw6ker_game_state_t *
game_state_new (lw6ker_game_struct_t * game_struct,
		lw6sys_progress_t * progress)
{
  lw6ker_game_state_t *ret = NULL;

  lw6sys_progress_begin (progress);
  ret = lw6ker_game_state_new (game_struct, progress);
  if (ret)
    {
      lw6ker_game_state_register_node (ret, TEST_NODE_ID);
      lw6ker_game_state_add_cursor (ret, TEST_NODE_ID,
				    TEST_CURSOR1_ID, TEST_COLOR1);
      lw6ker_game_state_add_cursor (ret, TEST_NODE_ID,
				    TEST_CURSOR2_ID, TEST_COLOR2);
    }
  lw6sys_progress_begin (progress);

  return ret;
}

static lw6pil_pilot_t *
pilot_new (lw6ker_game_state_t * game_state, lw6sys_progress_t * progress)
{
  lw6pil_pilot_t *ret = NULL;

  lw6sys_progress_begin (progress);
  ret =
    lw6pil_pilot_new (game_state, lw6pil_seq_random_0 (),
		      lw6sys_get_timestamp (), progress);
  if (ret)
    {
      // ok
    }
  lw6sys_progress_begin (progress);

  return ret;
}

static int
test_display (const char *gfx_backend_name)
{
  int ret = 0;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = TEST_ARGC;
    const char *argv[TEST_ARGC] = { TEST_ARGV0 };
    lw6dsp_backend_t *display = NULL;
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

    lw6sys_progress_default (&progress, &progress_value);
    lw6sys_progress_begin (&progress);
    display = lw6dsp_create_backend (argc, argv, gfx_backend_name);
    if (display)
      {
	repr = lw6dsp_repr (display);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("display repr is \"%s\""),
			repr);
	    LW6SYS_FREE (repr);
	  }
	lw6dsp_param_zero (&param);
	param.video_mode.width = TEST_WIDTH1;
	param.video_mode.height = TEST_HEIGHT1;
	param.video_mode.fullscreen = TEST_FULLSCREEN1;
	param.misc.target_fps = TEST_FRAMES_PER_SEC;
	param.misc.mask =
	  LW6GUI_DISPLAY_BACKGROUND | LW6GUI_DISPLAY_PREVIEW |
	  LW6GUI_DISPLAY_MAP | LW6GUI_DISPLAY_FIGHTERS |
	  LW6GUI_DISPLAY_CURSORS | LW6GUI_DISPLAY_MENU | LW6GUI_DISPLAY_META |
	  LW6GUI_DISPLAY_LOG | LW6GUI_DISPLAY_FPS;
	param.look = lw6gui_look_new (NULL);
	if (param.look)
	  {
	    if (lw6dsp_init (display, &param, NULL))
	      {
		repr = lw6dsp_repr (display);
		if (repr)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("display repr is \"%s\""), repr);
		    LW6SYS_FREE (repr);
		  }
		lw6dsp_get_video_mode (display, &video_mode);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("video mode is %dx%d fullscreen=%d"),
			    video_mode.width, video_mode.height,
			    video_mode.fullscreen);
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("displaying background"));
		sleep_and_report (display, TEST_SLEEP_LONG);
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("changing resolution"));
		param.video_mode.width = TEST_WIDTH2;
		param.video_mode.height = TEST_HEIGHT2;
		param.video_mode.fullscreen = TEST_FULLSCREEN2;
		param.misc.repeat_settings.delay = TEST_REPEAT_DELAY;
		param.misc.repeat_settings.interval = TEST_REPEAT_INTERVAL;
		param.misc.repeat_settings.double_click_delay =
		  TEST_DOUBLE_CLICK_DELAY;
		param.misc.log_timeout = TEST_LOG_TIMEOUT;
		lw6dsp_update (display, &param);
		sleep_and_report (display, TEST_SLEEP_LONG);
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("displaying menu"));
		param.menu = menu_new (TEST_NB_MENUITEMS1);
		if (param.menu)
		  {
		    lw6dsp_update (display, &param);
		    sleep_and_report (display, TEST_SLEEP_SHORT);
		    while (param.menu->nb_items > 0)
		      {
			lw6gui_menu_remove (param.menu, 0, 0);
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("displaying menu with %d items"),
				    param.menu->nb_items);
			lw6dsp_update (display, &param);
			sleep_and_report (display, TEST_SLEEP_SHORT);
		      }
		    lw6gui_menu_free (param.menu);
		    param.menu = menu_new (TEST_NB_MENUITEMS2);
		    if (param.menu)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("displaying big menu (%d items)"),
				    param.menu->nb_items);
			lw6dsp_update (display, &param);
			sleep_and_report (display, TEST_SLEEP_SHORT);
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("nb items displayed is %d"),
				    param.menu->nb_items_displayed);
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("scrolling to middle of big menu (%d)"),
				    param.menu->nb_items / 2);
			lw6gui_menu_select (param.menu,
					    param.menu->nb_items / 2,
					    TEST_MENU_ALLOW_SCROLL, 0);
			lw6dsp_update (display, &param);
			sleep_and_report (display, TEST_SLEEP_SHORT);
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("nb items displayed is %d"),
				    param.menu->nb_items_displayed);
			menu = param.menu;
			param.menu = NULL;
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("big menu disappears"));
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("this warning should appear for %d seconds on the screen"),
				    TEST_LOG_TIMEOUT);
			lw6sys_log (LW6SYS_LOG_ERROR, _("this error too"));
			lw6dsp_update (display, &param);
			sleep_and_report (display, TEST_SLEEP_SHORT);
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("big menu is back again"));
			param.menu = menu;
			lw6dsp_update (display, &param);
			sleep_and_report (display, TEST_SLEEP_SHORT);
			lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("loading a map"));
			param.misc.progress = &progress_value;
			lw6dsp_update (display, &param);
			level =
			  level_new (TEST_MAP, TEST_WIDTH2, TEST_HEIGHT2,
				     &progress);
			if (level)
			  {
			    param.level = level;
			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_ ("displaying a map"));
			    lw6dsp_update (display, &param);
			    sleep_and_report (display, TEST_SLEEP_LONG);

			    lw6sys_log (LW6SYS_LOG_NOTICE,
					_x_ ("build struct"));
			    game_struct =
			      game_struct_new (param.level, &progress);
			    if (game_struct)
			      {
				param.game_struct = game_struct;
				lw6dsp_update (display, &param);
				lw6sys_log (LW6SYS_LOG_NOTICE,
					    _x_ ("build state"));
				game_state =
				  game_state_new (param.game_struct,
						  &progress);
				if (game_state)
				  {
				    param.game_state = game_state;
				    lw6dsp_update (display, &param);
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_ ("displaying a game"));
				    sleep_and_report (display,
						      TEST_SLEEP_LONG);
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_
						("start play with game state"));
				    for (i = 0; i < TEST_NB_ROUNDS; ++i)
				      {
					lw6ker_game_state_do_round
					  (game_state);
					lw6dsp_update (display, &param);
				      }
				    lw6sys_log (LW6SYS_LOG_NOTICE,
						_x_
						("stop play with game state"));
				    pilot = pilot_new (game_state, &progress);
				    if (pilot)
				      {
					lw6sys_log (LW6SYS_LOG_NOTICE,
						    _x_
						    ("start play with pilot"));
					sleep_and_report (display,
							  TEST_SLEEP_LONG);
					lw6sys_log (LW6SYS_LOG_NOTICE,
						    _x_
						    ("stop play with pilot"));
					param.pilot = NULL;
					lw6dsp_update (display, &param);
					lw6pil_pilot_free (pilot);
					pilot = NULL;
				      }
				    param.game_state = NULL;
				    lw6dsp_update (display, &param);
				    lw6ker_game_state_free (game_state);
				    game_state = NULL;
				    ret = 1;
				  }
				param.game_struct = NULL;
				lw6dsp_update (display, &param);
				lw6ker_game_struct_free (game_struct);
				game_struct = NULL;
			      }
			    param.level = NULL;
			    lw6dsp_update (display, &param);
			    lw6map_free (level);
			    level = NULL;
			  }
			lw6gui_menu_free (param.menu);
			param.menu = NULL;
		      }
		  }
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("checking what happens when replicated objects are freed in the param (menu, for instance)"));
		sleep_and_report (display, TEST_SLEEP_SHORT);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("now applying the params, nothing should be displayed but the background"));
		lw6dsp_update (display, &param);
		sleep_and_report (display, TEST_SLEEP_LONG);

		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("pumping events, try to press random keys while having the focus on the game window, you should get one log message per keypress, test will end after %d seconds"),
			    (int) (TEST_INPUT_TICKS / LW6SYS_TICKS_PER_SEC));
		ticks = lw6sys_get_uptime ();
		while ((!lw6sys_signal_poll_quit ())
		       && lw6sys_get_uptime () < ticks + TEST_INPUT_TICKS)
		  {
		    /*
		     * Note, there's no sleep in this sloop, should be CPU greedy,
		     * it's interesting to see how display reacts.
		     */
		    lw6dsp_update (display, &param);
		    keypress =
		      lw6gui_keyboard_pop_keypress (&
						    (display->input->
						     keyboard));
		    if (keypress)
		      {
			repr = lw6gui_keypress_repr (keypress);
			if (repr)
			  {
			    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("%s"), repr);
			    LW6SYS_FREE (repr);
			  }
			lw6gui_keypress_free (keypress);
		      }
		  }
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("done with events"));

		nb_frames = lw6dsp_get_nb_frames (display);

		lw6dsp_quit (display);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("unable to start display"));
	      }
	    lw6gui_look_free (param.look);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("frames=%d"), nb_frames);
	lw6dsp_destroy_backend (display);
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

static void
test_callback_func (void *func_data, const char *key, void *value)
{
  int *ret = NULL;

  ret = (int *) func_data;
  (*ret) = test_display (key) || (*ret);
}

/**
 * lw6dsp_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the test suite for the dsp module. In check (0) mode, won't
 * test much to avoid failure because of missing graphical environment.
 *
 * Return value: 1 if OK, 0 if error.
 */
int
lw6dsp_test (int mode)
{
  int ret = 0;
  lw6sys_assoc_t *gfx_backends = NULL;
  const int argc = TEST_ARGC;
  const char *argv[TEST_ARGC] = { TEST_ARGV0 };

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6cfg_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
      lw6gui_test (mode);
      lw6vox_test (mode);
      lw6gfx_test (mode);
    }

  gfx_backends = lw6gfx_get_backends (argc, argv);
  if (gfx_backends)
    {
      if (mode)
	{
	  lw6sys_assoc_map (gfx_backends, test_callback_func, &ret);
	}
      else
	{
	  ret = 1;
	}
      lw6sys_assoc_free (gfx_backends);
    }

  return ret;
}
