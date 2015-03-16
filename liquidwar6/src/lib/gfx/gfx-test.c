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

#include "gfx-internal.h"

#define _TEST_WIDTH 640
#define _TEST_HEIGHT 480
#define _TEST_FULLSCREEN 0
#define _TEST_SLEEP 1.00
#define _TEST_DURATION_SETUP 100
#define _TEST_DURATION_SPLASH 500
#define _TEST_DURATION_BACKGROUND 500
#define _TEST_DURATION_MENU 500
#define _TEST_DURATION_VIEW 500
#define _TEST_DURATION_HUD 10000
#define _TEST_MENU_TITLE "My menu"
#define _TEST_MENU_HELP "This is a sample menu"
#define _TEST_MENU_POPUP ""
#define _TEST_MENU_ESC "Esc"
#define _TEST_MENU_ENABLE_ESC 0
#define _TEST_MENU_ALLOW_SCROLL 0
#define _TEST_MENU_COLOR 1
#define _TEST_MENU_NB_MENUITEMS 5
#define _TEST_MENU_SELECTED_MENUITEM 1
#define _TEST_MENU_COLORED_MENUITEM 3
#define _TEST_MENU_MAX_DISPLAYED_ITEMS 3
#define _TEST_DURATION_EVENTS 10000
#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

#define _TEST_BACKGROUND_STYLE_1 "bubbles"
#define _TEST_BACKGROUND_STYLE_2 "void"

//#define _TEST_HUD_STYLE_1 "floating"
//#define _TEST_HUD_STYLE_2 "tactical"

typedef struct _lw6gfx_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
  lw6gfx_backend_t *backend;
} _lw6gfx_test_data_t;

static _lw6gfx_test_data_t _test_data = { 0, NULL, NULL };

#if MOD_GL1 || MOD_GLES2 || MOD_SOFT
static void
_test_sdl ()
{
  int ret = 1;
#ifndef LW6_ALLINONE
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };
#endif
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6gfx_sdl_funcs_t funcs;
    void *handle = NULL;

    memset (&funcs, 0, sizeof (_lw6gfx_sdl_funcs_t));

#ifndef LW6_ALLINONE
    handle = lw6dyn_dlopen_shared (sys_context, argc, argv, "gfx", "sdl");
    if (!handle)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to load shared SDL code"));
	ret = 0;
      }
#endif

    if (ret)
      {
	if (_lw6gfx_sdl_bind_funcs (sys_context, &funcs, handle))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("binded SDL funcs"));
	    _lw6gfx_sdl_unbind_funcs (sys_context, &funcs);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't bind SDL funcs"));
	    ret = 0;
	  }
      }

#ifndef LW6_ALLINONE
    if (handle)
      {
	lw6dyn_dlclose_shared (sys_context, handle);
	handle = NULL;
      }
#endif
  }

  LW6SYS_TEST_FUNCTION_END;
}
#endif // MOD_GL1 || MOD_GLES2 || MOD_SOFT

#if MOD_GL1 || MOD_GLES2 || MOD_SOFT || MOD_CACA
static void
_test_resolution ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gfx_backend_t *backend = _test_data.backend;
    lw6gui_fullscreen_modes_t modes;

    if (lw6gfx_get_fullscreen_modes (sys_context, backend, &modes))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("low resolution is %dx%d"), modes.low.width, modes.low.height);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("standard resolution is %dx%d"), modes.standard.width, modes.standard.height);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("high resolution is %dx%d"), modes.high.width, modes.high.height);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get fullscreen modes"));
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_resize_callback (lw6sys_context_t * sys_context, lw6gui_video_mode_t * video_mode)
{
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
	      _x_ ("resize callback called %dx%d fullscreen=%d"), video_mode->width, video_mode->height, video_mode->fullscreen);
}

static int
_call_init (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend)
{
  int ret = 1;

  lw6gui_video_mode_t video_mode;
  int64_t ticks;
  char *repr;

  video_mode.width = _TEST_WIDTH;
  video_mode.height = _TEST_HEIGHT;
  video_mode.fullscreen = _TEST_FULLSCREEN;

  ret = ret && lw6gfx_init (sys_context, backend, &video_mode, _resize_callback);
  if (ret)
    {
      repr = lw6gfx_repr (sys_context, backend);
      if (repr)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("display repr is \"%s\""), repr);
	  LW6SYS_FREE (sys_context, repr);
	}
      ticks = lw6sys_get_uptime (sys_context);
      while (lw6sys_get_uptime (sys_context) < ticks + _TEST_DURATION_SETUP)
	{
	  lw6sys_sleep (sys_context, _TEST_SLEEP);
	}
    }

  return ret;
}

static void
_test_splash ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gfx_backend_t *backend = _test_data.backend;
    lw6gui_look_t *look = NULL;
    int64_t ticks;

    look = lw6gui_look_new (sys_context, NULL);
    if (look)
      {
	ticks = lw6sys_get_uptime (sys_context);
	while (lw6sys_get_uptime (sys_context) < ticks + _TEST_DURATION_SPLASH)
	  {
	    if (!lw6gfx_display
		(sys_context, backend, LW6GUI_DISPLAY_BACKGROUND | LW6GUI_DISPLAY_SPLASH, look, NULL, NULL, NULL, 0, NULL, 0.0f, 0, 0, NULL, 0, 0, 0, 0))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display error"));
		ret = 0;
	      }
	    lw6sys_sleep (sys_context, _TEST_SLEEP);
	  }
	lw6gui_look_free (sys_context, look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_background ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gfx_backend_t *backend = _test_data.backend;
    int64_t ticks;
    lw6gui_look_t *look = NULL;
    char *styles[] = { _TEST_BACKGROUND_STYLE_1, _TEST_BACKGROUND_STYLE_2, NULL };
    int i;

    look = lw6gui_look_new (sys_context, NULL);
    if (look)
      {
	for (i = 0; styles[i]; ++i)
	  {
	    lw6gui_look_set (sys_context, look, LW6DEF_BACKGROUND_STYLE, styles[i]);
	    ticks = lw6sys_get_uptime (sys_context);
	    while (lw6sys_get_uptime (sys_context) < ticks + _TEST_DURATION_BACKGROUND)
	      {
		if (!lw6gfx_display (sys_context, backend, LW6GUI_DISPLAY_BACKGROUND, look, NULL, NULL, NULL, 0, NULL, 0.0f, 0, 0, NULL, 0, 0, 0, 0))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display error"));
		    ret = 0;
		  }
		lw6sys_sleep (sys_context, _TEST_SLEEP);
	      }
	  }
	lw6gui_look_free (sys_context, look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_menu ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gfx_backend_t *backend = _test_data.backend;
    lw6gui_menuitem_t *menuitem = NULL;
    lw6gui_menu_t *menu = NULL;
    int i;
    char *menuitem_label;
    char *menuitem_tooltip;
    int64_t ticks;
    float progress;

    lw6gui_look_t *look = NULL;

    look = lw6gui_look_new (sys_context, NULL);
    if (look)
      {
	menu = lw6gui_menu_new (sys_context, _TEST_MENU_TITLE, _TEST_MENU_HELP, _TEST_MENU_POPUP, _TEST_MENU_ESC, _TEST_MENU_ENABLE_ESC);
	if (menu)
	  {
	    for (i = 0; i < _TEST_MENU_NB_MENUITEMS; ++i)
	      {
		menuitem_label = lw6sys_new_sprintf (sys_context, _x_ ("Test menuitem %d"), i);
		if (menuitem_label)
		  {
		    menuitem_tooltip =
		      lw6sys_new_sprintf (sys_context, _x_ ("Tooltip for menuitem %d\nExplaining how it works\nIt works as follows:\n...it works!"), i);
		    if (menuitem_tooltip)
		      {
			menuitem =
			  lw6gui_menuitem_new (sys_context, menuitem_label, menuitem_tooltip, _TEST_MENU_COLOR, 1, 0, i == _TEST_MENU_COLORED_MENUITEM);
			if (menuitem)
			  {
			    lw6gui_menu_append (sys_context, menu, menuitem, lw6sys_get_uptime (sys_context));
			  }
			LW6SYS_FREE (sys_context, menuitem_tooltip);
		      }
		    LW6SYS_FREE (sys_context, menuitem_label);
		  }
	      }
	    lw6gui_menu_select (sys_context, menu, _TEST_MENU_SELECTED_MENUITEM, _TEST_MENU_ALLOW_SCROLL, 0);
	    ticks = lw6sys_get_uptime (sys_context);
	    while (lw6sys_get_uptime (sys_context) < ticks + _TEST_DURATION_MENU)
	      {
		progress = ((float) (lw6sys_get_uptime (sys_context) - ticks)) / ((float) _TEST_DURATION_MENU);
		if (!lw6gfx_display (sys_context, backend,
				     LW6GUI_DISPLAY_BACKGROUND |
				     LW6GUI_DISPLAY_MENU | LW6GUI_DISPLAY_META
				     | LW6GUI_DISPLAY_PROGRESS, look, NULL, NULL, NULL, 0, menu, progress, 0, 0, NULL, 0, 0, 0, 0))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display error"));
		    ret = 0;
		  }
		lw6sys_sleep (sys_context, _TEST_SLEEP);
	      }
	    lw6gui_menu_free (sys_context, menu);	// should free the menuitem
	  }
	lw6gui_look_free (sys_context, look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_view ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gfx_backend_t *backend = _test_data.backend;
    lw6gui_look_t *look = NULL;
    int64_t ticks;
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;

    look = lw6gui_look_new (sys_context, NULL);
    if (look)
      {
	level = lw6map_builtin_defaults (sys_context);
	if (level)
	  {
	    game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	    if (game_struct)
	      {
		game_state = lw6ker_game_state_new (sys_context, game_struct, NULL);
		if (game_state)
		  {
		    ticks = lw6sys_get_uptime (sys_context);
		    while (lw6sys_get_uptime (sys_context) < ticks + _TEST_DURATION_VIEW)
		      {
			if (!lw6gfx_display (sys_context, backend,
					     LW6GUI_DISPLAY_BACKGROUND |
					     LW6GUI_DISPLAY_MAP |
					     LW6GUI_DISPLAY_FIGHTERS, look, level, game_struct, game_state, NULL, NULL, 0.0f, 0.0f, 0.0f, NULL, 0, 0, 0, 0))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display error"));
			    ret = 0;
			  }
			lw6sys_sleep (sys_context, _TEST_SLEEP);
		      }
		    lw6ker_game_state_free (sys_context, game_state);
		  }
		lw6ker_game_struct_free (sys_context, game_struct);
	      }
	    lw6map_free (sys_context, level);
	  }
	lw6gui_look_free (sys_context, look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_hud ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gfx_backend_t *backend = _test_data.backend;
    lw6gui_look_t *look = NULL;
    int64_t ticks;
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6pil_local_cursors_t local_cursors;
    u_int64_t node_id;

    look = lw6gui_look_new (sys_context, NULL);
    if (look)
      {
	level = lw6map_builtin_defaults (sys_context);
	level->param.rules.total_time = LW6MAP_RULES_MIN_TOTAL_TIME;
	if (level)
	  {
	    game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	    if (game_struct)
	      {
		game_state = lw6ker_game_state_new (sys_context, game_struct, NULL);
		if (game_state)
		  {
		    lw6pil_local_cursors_reset (sys_context, &local_cursors);
		    node_id = lw6sys_generate_id_64 (sys_context);
		    lw6ker_game_state_register_node (sys_context, game_state, node_id);
		    lw6ker_game_state_add_cursor (sys_context, game_state, node_id, lw6sys_generate_id_16 (sys_context), LW6MAP_TEAM_COLOR_RED);
		    lw6ker_game_state_add_cursor (sys_context, game_state, node_id, lw6sys_generate_id_16 (sys_context), LW6MAP_TEAM_COLOR_GREEN);
		    lw6ker_game_state_add_cursor (sys_context, game_state, node_id, lw6sys_generate_id_16 (sys_context), LW6MAP_TEAM_COLOR_BLUE);
		    while (lw6ker_game_state_get_time_left (sys_context, game_state) > 0)
		      {
			lw6ker_game_state_do_round (sys_context, game_state);
		      }

		    ticks = lw6sys_get_uptime (sys_context);
		    while (lw6sys_get_uptime (sys_context) < ticks + _TEST_DURATION_HUD)
		      {
			if (!lw6gfx_display (sys_context, backend,
					     LW6GUI_DISPLAY_BACKGROUND |
					     LW6GUI_DISPLAY_HUD |
					     LW6GUI_DISPLAY_SCORE, look,
					     level, game_struct, game_state, &local_cursors, NULL, 0.0f, 0.0f, 0.0f, NULL, 0, 0, 0, 0))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display error"));
			    ret = 0;
			  }
			lw6sys_sleep (sys_context, _TEST_SLEEP);
		      }
		    lw6ker_game_state_free (sys_context, game_state);
		  }
		lw6ker_game_struct_free (sys_context, game_struct);
	      }
	    lw6map_free (sys_context, level);
	  }
	lw6gui_look_free (sys_context, look);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_events ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6gfx_backend_t *backend = _test_data.backend;
    lw6gui_look_t *look = NULL;
    lw6gui_input_t *input = NULL;
    lw6gui_keypress_t *keypress = NULL;
    char *repr = NULL;
    int64_t ticks = 0;

    look = lw6gui_look_new (sys_context, NULL);
    if (look)
      {
	ticks = lw6sys_get_uptime (sys_context);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("now for %d seconds you can move mouse, touch keyboard, punch joystick"), _TEST_DURATION_EVENTS / 1000);
	input = lw6gfx_pump_events (sys_context, backend);
	while (lw6sys_get_uptime (sys_context) < ticks + _TEST_DURATION_EVENTS && !lw6sys_signal_poll_quit (sys_context))
	  {
	    if (!lw6gfx_display (sys_context, backend, LW6GUI_DISPLAY_BACKGROUND, look, NULL, NULL, NULL, 0, NULL, 0.0f, 0, 0, NULL, 0, 0, 0, 0))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("display error"));
		ret = 0;
	      }
	    lw6sys_sleep (sys_context, _TEST_SLEEP);
	    input = lw6gfx_pump_events (sys_context, backend);
	    if (lw6gui_button_pop_press (sys_context, &(input->mouse.button_left)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("left mouse button pressed at %dx%d"), input->mouse.screen_pointer.pos_x, input->mouse.screen_pointer.pos_y);
	      }
	    if (lw6gui_button_pop_press (sys_context, &(input->mouse.button_right)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("right mouse button pressed at %d,%d"), input->mouse.screen_pointer.pos_x, input->mouse.screen_pointer.pos_y);
	      }
	    if (lw6gui_button_pop_press (sys_context, &(input->mouse.wheel_up)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("mouse wheel up"));
	      }
	    if (lw6gui_button_pop_press (sys_context, &(input->mouse.wheel_down)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("mouse wheel down"));
	      }
	    keypress = lw6gui_keyboard_pop_keypress (sys_context, &(input->keyboard));
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
	    if (lw6sys_signal_poll_quit (sys_context))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("a QUIT event was detected"));
	      }
	  }
	lw6gui_look_free (sys_context, look);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}


static void
_call_quit (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend)
{
  lw6gfx_quit (sys_context, backend);
}
#endif // MOD_GL1 || MOD_GLES2 || MOD_SOFT || MOD_CACA

#if MOD_GL1 || MOD_GLES2 || MOD_SOFT
static int
_setup_init_sdl ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libgfx-sdl CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit_sdl ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libgfx-sdl CUnit test suite"));

  return CUE_SUCCESS;
}
#endif // MOD_GL1 || MOD_GLES2 || MOD_SOFT

#ifdef MOD_GL1
static int
_setup_init_gl1 ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libgfx-gl1 CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6gfx_create_backend (sys_context, argc, argv, "gl1");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6gfx_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_gl1 ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libgfx-gl1 CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6gfx_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}
#endif // MOD_GL1

#ifdef MOD_GLES2
static int
_setup_init_gles2 ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libgfx-gles2 CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6gfx_create_backend (sys_context, argc, argv, "gles2");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6gfx_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_gles2 ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libgfx-gles2 CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6gfx_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}
#endif // MOD_GLES2

#ifdef MOD_SOFT
static int
_setup_init_soft ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libgfx-soft CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6gfx_create_backend (sys_context, argc, argv, "soft");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6gfx_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_soft ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libgfx-soft CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6gfx_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}
#endif // MOD_SOFT

#ifdef MOD_CACA
static int
_setup_init_caca ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libgfx-caca CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6gfx_create_backend (sys_context, argc, argv, "caca");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6gfx_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_caca ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libgfx-caca CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6gfx_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}
#endif // MOD_CACA

/**
 * lw6gfx_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libgfx module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gfx_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
#if MOD_GL1 || MOD_GLES2 || MOD_SOFT || MOD_CACA
  CU_Suite *suite = NULL;
#endif // MOD_GL1 || MOD_GLES2 || MOD_SOFT || MOD_CACA

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
      lw6cns_test_register (sys_context, mode);
      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
    }

  if (mode & LW6SYS_TEST_MODE_FULL_TEST)
    {
#if MOD_GL1 || MOD_GLES2 || MOD_SOFT
      suite = CU_add_suite ("lw6gfx-sdl", _setup_init_sdl, _setup_quit_sdl);
      if (suite)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_sdl);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
	  ret = 0;
	}
#endif // MOD_GL1 || MOD_GLES2 || MOD_SOFT

#ifdef MOD_GL1
      suite = CU_add_suite ("lw6gfx-gl1", _setup_init_gl1, _setup_quit_gl1);
      if (suite)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_resolution);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_splash);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_background);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_menu);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_view);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_hud);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_events);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
	  ret = 0;
	}
#endif // MOD_GL1

#ifdef MOD_GLES2
      suite = CU_add_suite ("lw6gfx-gles2", _setup_init_gles2, _setup_quit_gles2);
      if (suite)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_resolution);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_splash);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_background);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_menu);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_view);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_hud);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_events);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
	  ret = 0;
	}
#endif // MOD_GLES2

#ifdef MOD_SOFT
      suite = CU_add_suite ("lw6gfx-soft", _setup_init_soft, _setup_quit_soft);
      if (suite)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_resolution);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_splash);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_background);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_menu);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_view);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_hud);
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_events);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
	  ret = 0;
	}
#endif // MOD_SOFT

#ifdef MOD_CACA
      if (!(mode & LW6SYS_TEST_MODE_INTERACTIVE))
	{
	  suite = CU_add_suite ("lw6gfx-caca", _setup_init_caca, _setup_quit_caca);
	  if (suite)
	    {
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_resolution);
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_splash);
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_background);
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_menu);
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_view);
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_hud);
	      LW6SYS_CUNIT_ADD_TEST (suite, _test_events);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
	      ret = 0;
	    }
	}
#endif // MOD_CACA
    }

  return ret;
}

/**
 * lw6gfx_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @gfx module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gfx_test_run (lw6sys_context_t * sys_context, int mode)
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
