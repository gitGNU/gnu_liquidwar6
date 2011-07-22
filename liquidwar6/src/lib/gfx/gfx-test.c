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

#include "gfx.h"

#define TEST_WIDTH 640
#define TEST_HEIGHT 480
#define TEST_FULLSCREEN 0
#define TEST_SLEEP 0.01
#define TEST_DURATION_SETUP 100
#define TEST_DURATION_SPLASH 500
#define TEST_DURATION_BACKGROUND 500
#define TEST_DURATION_MENU 500
#define TEST_DURATION_VIEW 500
#define TEST_DURATION_HUD 10000
#define TEST_MENU_LABEL "My menu"
#define TEST_MENU_ESC "Esc"
#define TEST_MENU_ENABLE_ESC 0
#define TEST_MENU_ALLOW_SCROLL 0
#define TEST_MENU_COLOR 1
#define TEST_MENU_NB_MENUITEMS 5
#define TEST_MENU_SELECTED_MENUITEM 1
#define TEST_MENU_COLORED_MENUITEM 3
#define TEST_MENU_MAX_DISPLAYED_ITEMS 3
#define TEST_DURATION_EVENTS 10000
#define TEST_ARGC 1
#define TEST_ARGV0 "prog"

#define _TEST_BACKGROUND_STYLE_1 "bubbles"
#define _TEST_BACKGROUND_STYLE_2 "void"

//#define _TEST_HUD_STYLE_1 "floating"
//#define _TEST_HUD_STYLE_2 "tactical"

static int
test_resolution (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_fullscreen_modes_t modes;

    if (lw6gfx_get_fullscreen_modes (backend, &modes))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("low resolution is %dx%d"),
		    modes.low.width, modes.low.height);
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("standard resolution is %dx%d"),
		    modes.standard.width, modes.standard.height);
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("high resolution is %dx%d"),
		    modes.high.width, modes.high.height);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("unable to get fullscreen modes"));
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
resize_callback (lw6gui_video_mode_t * video_mode)
{
  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _x_ ("resize callback called %dx%d fullscreen=%d"),
	      video_mode->width, video_mode->height, video_mode->fullscreen);
}

static int
test_init (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_video_mode_t video_mode;
    int64_t ticks;
    char *repr;

    video_mode.width = TEST_WIDTH;
    video_mode.height = TEST_HEIGHT;
    video_mode.fullscreen = TEST_FULLSCREEN;

    ret = ret && lw6gfx_init (backend, &video_mode, resize_callback);
    if (ret)
      {
	repr = lw6gfx_repr (backend);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("display repr is \"%s\""),
			repr);
	    LW6SYS_FREE (repr);
	  }
	ticks = lw6sys_get_uptime ();
	while (lw6sys_get_uptime () < ticks + TEST_DURATION_SETUP)
	  {
	    lw6sys_sleep (TEST_SLEEP);
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_splash (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_look_t *look = NULL;
    int64_t ticks;

    look = lw6gui_look_new (NULL);
    if (look)
      {
	ticks = lw6sys_get_uptime ();
	while (lw6sys_get_uptime () < ticks + TEST_DURATION_SPLASH)
	  {
	    if (!lw6gfx_display (backend,
				 LW6GUI_DISPLAY_BACKGROUND |
				 LW6GUI_DISPLAY_SPLASH, look, NULL, NULL,
				 NULL, 0, NULL, 0.0f, 0, 0, NULL, 0, 0, 0, 0))
	      {
		lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("display error"));
		ret = 0;
	      }
	    lw6sys_sleep (TEST_SLEEP);
	  }
	lw6gui_look_free (look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_background (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int64_t ticks;
    lw6gui_look_t *look = NULL;
    char *styles[] =
      { _TEST_BACKGROUND_STYLE_1, _TEST_BACKGROUND_STYLE_2, NULL };
    int i;

    look = lw6gui_look_new (NULL);
    if (look)
      {
	for (i = 0; styles[i]; ++i)
	  {
	    lw6gui_look_set (look, LW6DEF_BACKGROUND_STYLE, styles[i]);
	    ticks = lw6sys_get_uptime ();
	    while (lw6sys_get_uptime () < ticks + TEST_DURATION_BACKGROUND)
	      {
		if (!lw6gfx_display (backend,
				     LW6GUI_DISPLAY_BACKGROUND,
				     look,
				     NULL,
				     NULL, NULL, 0, NULL, 0.0f, 0, 0, NULL, 0,
				     0, 0, 0))
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("display error"));
		    ret = 0;
		  }
		lw6sys_sleep (TEST_SLEEP);
	      }
	  }
	lw6gui_look_free (look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_menu (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_menuitem_t *menuitem = NULL;
    lw6gui_menu_t *menu = NULL;
    int i;
    char *menuitem_label;
    int64_t ticks;
    float progress;

    lw6gui_look_t *look = NULL;

    look = lw6gui_look_new (NULL);
    if (look)
      {
	menu =
	  lw6gui_menu_new (TEST_MENU_LABEL, TEST_MENU_ESC,
			   TEST_MENU_ENABLE_ESC);
	if (menu)
	  {
	    for (i = 0; i < TEST_MENU_NB_MENUITEMS; ++i)
	      {
		menuitem_label =
		  lw6sys_new_sprintf (_x_ ("Test menuitem %d"), i);
		if (menuitem_label)
		  {
		    menuitem =
		      lw6gui_menuitem_new (menuitem_label, TEST_MENU_COLOR, 1,
					   0,
					   i == TEST_MENU_COLORED_MENUITEM);
		    if (menuitem)
		      {
			lw6gui_menu_append (menu, menuitem,
					    lw6sys_get_uptime ());
		      }
		    LW6SYS_FREE (menuitem_label);
		  }
	      }
	    lw6gui_menu_select (menu, TEST_MENU_SELECTED_MENUITEM,
				TEST_MENU_ALLOW_SCROLL, 0);
	    ticks = lw6sys_get_uptime ();
	    while (lw6sys_get_uptime () < ticks + TEST_DURATION_MENU)
	      {
		progress = ((float)
			    (lw6sys_get_uptime () -
			     ticks)) / ((float) TEST_DURATION_MENU);
		if (!lw6gfx_display (backend,
				     LW6GUI_DISPLAY_BACKGROUND |
				     LW6GUI_DISPLAY_MENU |
				     LW6GUI_DISPLAY_PROGRESS, look, NULL,
				     NULL, NULL, 0, menu, progress, 0, 0,
				     NULL, 0, 0, 0, 0))
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("display error"));
		    ret = 0;
		  }
		lw6sys_sleep (TEST_SLEEP);
	      }
	    lw6gui_menu_free (menu);	// should free the menuitem
	  }
	lw6gui_look_free (look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_view (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_look_t *look = NULL;
    int64_t ticks;
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;

    look = lw6gui_look_new (NULL);
    if (look)
      {
	level = lw6map_builtin_defaults ();
	if (level)
	  {
	    game_struct = lw6ker_game_struct_new (level, NULL);
	    if (game_struct)
	      {
		game_state = lw6ker_game_state_new (game_struct, NULL);
		if (game_state)
		  {
		    ticks = lw6sys_get_uptime ();
		    while (lw6sys_get_uptime () < ticks + TEST_DURATION_VIEW)
		      {
			if (!lw6gfx_display (backend,
					     LW6GUI_DISPLAY_BACKGROUND |
					     LW6GUI_DISPLAY_MAP |
					     LW6GUI_DISPLAY_FIGHTERS, look,
					     level, game_struct, game_state,
					     NULL, NULL, 0.0f, 0.0f, 0.0f,
					     NULL, 0, 0, 0, 0))
			  {
			    lw6sys_log (LW6SYS_LOG_WARNING,
					_x_ ("display error"));
			    ret = 0;
			  }
			lw6sys_sleep (TEST_SLEEP);
		      }
		    lw6ker_game_state_free (game_state);
		  }
		lw6ker_game_struct_free (game_struct);
	      }
	    lw6map_free (level);
	  }
	lw6gui_look_free (look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_hud (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_look_t *look = NULL;
    int64_t ticks;
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6pil_local_cursors_t local_cursors;
    u_int64_t node_id;

    look = lw6gui_look_new (NULL);
    if (look)
      {
	level = lw6map_builtin_defaults ();
	level->param.rules.total_time = LW6MAP_RULES_MIN_TOTAL_TIME;
	if (level)
	  {
	    game_struct = lw6ker_game_struct_new (level, NULL);
	    if (game_struct)
	      {
		game_state = lw6ker_game_state_new (game_struct, NULL);
		if (game_state)
		  {
		    lw6pil_local_cursors_reset (&local_cursors);
		    node_id = lw6sys_generate_id_64 ();
		    lw6ker_game_state_register_node (game_state, node_id);
		    lw6ker_game_state_add_cursor (game_state, node_id,
						  lw6sys_generate_id_16 (),
						  LW6MAP_TEAM_COLOR_RED);
		    lw6ker_game_state_add_cursor (game_state, node_id,
						  lw6sys_generate_id_16 (),
						  LW6MAP_TEAM_COLOR_GREEN);
		    lw6ker_game_state_add_cursor (game_state, node_id,
						  lw6sys_generate_id_16 (),
						  LW6MAP_TEAM_COLOR_BLUE);
		    while (lw6ker_game_state_get_time_left (game_state) > 0)
		      {
			lw6ker_game_state_do_round (game_state);
		      }

		    ticks = lw6sys_get_uptime ();
		    while (lw6sys_get_uptime () < ticks + TEST_DURATION_HUD)
		      {
			if (!lw6gfx_display (backend,
					     LW6GUI_DISPLAY_BACKGROUND |
					     LW6GUI_DISPLAY_HUD |
					     LW6GUI_DISPLAY_SCORE, look,
					     level, game_struct, game_state,
					     &local_cursors, NULL, 0.0f, 0.0f,
					     0.0f, NULL, 0, 0, 0, 0))
			  {
			    lw6sys_log (LW6SYS_LOG_WARNING,
					_x_ ("display error"));
			    ret = 0;
			  }
			lw6sys_sleep (TEST_SLEEP);
		      }
		    lw6ker_game_state_free (game_state);
		  }
		lw6ker_game_struct_free (game_struct);
	      }
	    lw6map_free (level);
	  }
	lw6gui_look_free (look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_events (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gui_look_t *look = NULL;
    lw6gui_input_t *input = NULL;
    lw6gui_keypress_t *keypress = NULL;
    char *repr = NULL;
    int64_t ticks = 0;

    look = lw6gui_look_new (NULL);
    if (look)
      {
	ticks = lw6sys_get_uptime ();
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("now for %d seconds you can move mouse, touch keyboard, punch joystick"),
		    TEST_DURATION_EVENTS / 1000);
	input = lw6gfx_pump_events (backend);
	while (lw6sys_get_uptime () < ticks + TEST_DURATION_EVENTS
	       && !lw6sys_signal_poll_quit ())
	  {
	    if (!lw6gfx_display (backend,
				 LW6GUI_DISPLAY_BACKGROUND, look, NULL,
				 NULL, NULL, 0, NULL, 0.0f, 0, 0, NULL, 0, 0,
				 0, 0))
	      {
		lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("display error"));
		ret = 0;
	      }
	    lw6sys_sleep (TEST_SLEEP);
	    input = lw6gfx_pump_events (backend);
	    if (lw6gui_button_pop_press (&(input->mouse.button_left)))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("left mouse button pressed at %dx%d"),
			    input->mouse.x, input->mouse.y);
	      }
	    if (lw6gui_button_pop_press (&(input->mouse.button_right)))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("right mouse button pressed at %d,%d"),
			    input->mouse.x, input->mouse.y);
	      }
	    if (lw6gui_button_pop_press (&(input->mouse.wheel_up)))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("mouse wheel up"));
	      }
	    if (lw6gui_button_pop_press (&(input->mouse.wheel_down)))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("mouse wheel down"));
	      }
	    keypress = lw6gui_keyboard_pop_keypress (&(input->keyboard));
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
	    if (lw6sys_signal_poll_quit ())
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("a QUIT event was detected"));
	      }
	  }
	lw6gui_look_free (look);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}


static int
test_quit (lw6gfx_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  lw6gfx_quit (backend);

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6gfx_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @gfx module test suite. In check-only mode, this
 * function won't test many things, for it requires a graphical
 * mode to be available to perform the complete test.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gfx_test (int mode)
{
  int ret = 0;
  lw6gfx_backend_t *backend = NULL;
  int argc = TEST_ARGC;
  char *argv[TEST_ARGC] = { TEST_ARGV0 };

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6cfg_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
      lw6pil_test (mode);
      lw6gui_test (mode);
    }

  if (mode)
    {
#ifdef MOD_GL
      backend = lw6gfx_create_backend (argc, argv, "gl");
      if (backend)
	{
	  ret = (test_init (backend) && test_resolution (backend)
		 && test_splash (backend)
		 && test_background (backend) && test_menu (backend)
		 && test_view (backend)
		 && test_hud (backend)
		 && test_events (backend) && test_quit (backend)) || ret;
	  lw6gfx_destroy_backend (backend);
	}
#endif
    }
  else
    {
      ret = 1;
    }

  return ret;
}
