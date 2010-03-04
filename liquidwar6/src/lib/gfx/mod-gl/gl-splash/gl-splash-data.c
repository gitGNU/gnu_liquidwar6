/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "gl-splash.h"
#include "gl-splash-internal.h"

#define CONST_FILE "splash/gl-splash-const.xml"
#define IMAGE_DIR "splash/image/"
#define IMAGE_GNU_HEAD "gnu-head.jpeg"
#define _MAP_DEPTH 1
#define _MAP_NOISE_PERCENT 0
#define _GAME_COLOR1 LW6MAP_TEAM_COLOR_RED
#define _GAME_COLOR2 LW6MAP_TEAM_COLOR_GREEN
#define _GAME_COLOR3 LW6MAP_TEAM_COLOR_BLUE
#define _GAME_COLOR4 LW6MAP_TEAM_COLOR_YELLOW

static void
_read_callback (void *callback_data, char *element, char *key, char *value)
{
  _mod_gl_splash_const_data_t *const_data;

  const_data = (_mod_gl_splash_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (key, value, "text-period",
			   &const_data->text_period);
      lw6cfg_read_xml_int (key, value, "slices", &const_data->slices);
      lw6cfg_read_xml_int (key, value, "stacks", &const_data->stacks);
      lw6cfg_read_xml_int (key, value, "planet-day-period",
			   &const_data->planet_day_period);
      lw6cfg_read_xml_int (key, value, "satellite-year-period",
			   &const_data->satellite_year_period);
      lw6cfg_read_xml_int (key, value, "satellite-day-period",
			   &const_data->satellite_day_period);
      lw6cfg_read_xml_int (key, value, "cursors-spin-radius",
			   &const_data->cursors_spin_radius);
      lw6cfg_read_xml_int (key, value, "cursors-spin-period",
			   &const_data->cursors_spin_period);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (key, value, "text-center-x",
			     &const_data->text_center_x);
      lw6cfg_read_xml_float (key, value, "text-center-y",
			     &const_data->text_center_y);
      lw6cfg_read_xml_float (key, value, "text-size", &const_data->text_size);
      lw6cfg_read_xml_float (key, value, "text-dw", &const_data->text_dw);
      lw6cfg_read_xml_float (key, value, "text-dh", &const_data->text_dh);
      lw6cfg_read_xml_float (key, value, "rotate-x", &const_data->rotate_x);
      lw6cfg_read_xml_float (key, value, "rotate-y", &const_data->rotate_y);
      lw6cfg_read_xml_float (key, value, "rotate-z", &const_data->rotate_z);
      lw6cfg_read_xml_float (key, value, "planet-x", &const_data->planet_x);
      lw6cfg_read_xml_float (key, value, "planet-y", &const_data->planet_y);
      lw6cfg_read_xml_float (key, value, "planet-z", &const_data->planet_z);
      lw6cfg_read_xml_float (key, value, "planet-radius",
			     &const_data->planet_radius);
      lw6cfg_read_xml_float (key, value, "satellite-radius",
			     &const_data->satellite_radius);
      lw6cfg_read_xml_float (key, value, "satellite-distance",
			     &const_data->satellite_distance);
      lw6cfg_read_xml_float (key, value, "cursors-center-speed",
			     &const_data->cursors_center_speed);
    }

  if (!strcmp (element, "color"))
    {
      lw6cfg_read_xml_color (key, value, "root-bg", &const_data->root_bg);
      lw6cfg_read_xml_color (key, value, "map-fg", &const_data->map_color.fg);
      lw6cfg_read_xml_color (key, value, "map-bg", &const_data->map_color.bg);
      lw6cfg_read_xml_color (key, value, "text-fg",
			     &const_data->text_color.fg);
      lw6cfg_read_xml_color (key, value, "text-bg",
			     &const_data->text_color.bg);
    }
}

static int
_load_consts (mod_gl_utils_context_t * utils_context,
	      _mod_gl_splash_context_t * splash_context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (utils_context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("reading \"%s\""), const_file);

      ret =
	lw6cfg_read_key_value_xml_file (const_file, _read_callback,
					(void *)
					&(splash_context->const_data));

      LW6SYS_FREE (const_file);
    }

  return ret;
}

static void
_unload_consts (mod_gl_utils_context_t * utils_context,
		_mod_gl_splash_context_t * splash_context)
{
  memset (&splash_context->const_data, 0,
	  sizeof (_mod_gl_splash_const_data_t));
}

static int
_load_bitmaps (mod_gl_utils_context_t * utils_context,
	       _mod_gl_splash_context_t * splash_context)
{
  int ret = 0;

  splash_context->bitmap_data.satellite =
    mod_gl_utils_bitmap_load (utils_context, IMAGE_DIR IMAGE_GNU_HEAD);
  if (splash_context->bitmap_data.satellite)
    {
      mod_gl_utils_bitmap_set_wrap (utils_context,
				    splash_context->bitmap_data.satellite,
				    GL_REPEAT);
    }
  ret = (splash_context->bitmap_data.satellite != NULL);

  return ret;
}

static void
_unload_bitmaps (mod_gl_utils_context_t * utils_context,
		 _mod_gl_splash_context_t * splash_context)
{
  if (splash_context->bitmap_data.satellite)
    {
      mod_gl_utils_bitmap_free (utils_context,
				splash_context->bitmap_data.satellite);
    }
  memset (&splash_context->bitmap_data, 0,
	  sizeof (_mod_gl_splash_bitmap_data_t));
}

static int
_load_game (mod_gl_utils_context_t * utils_context,
	    _mod_gl_splash_context_t * splash_context)
{
  int ret = 0;

  splash_context->game.sphere = gluNewQuadric ();
  splash_context->game.level =
    lw6map_builtin_custom (LW6MAP_BUILTIN_DEFAULT_WIDTH,
			   LW6MAP_BUILTIN_DEFAULT_HEIGHT,
			   _MAP_DEPTH, _MAP_NOISE_PERCENT);
  if (splash_context->game.level)
    {
      splash_context->game.level->param.rules.total_time =
	LW6MAP_RULES_MAX_TOTAL_TIME;
      splash_context->game.game_struct =
	lw6ker_game_struct_new (splash_context->game.level, NULL);
      if (splash_context->game.game_struct)
	{
	  splash_context->game.game_state =
	    lw6ker_game_state_new (splash_context->game.game_struct, NULL);
	  if (splash_context->game.game_state)
	    {
	      lw6ker_game_state_register_server (splash_context->game.
						 game_state,
						 _MOD_GL_SPLASH_GAME_SERVER_ID);
	      lw6ker_game_state_add_cursor (splash_context->game.game_state,
					    _MOD_GL_SPLASH_GAME_SERVER_ID,
					    _MOD_GL_SPLASH_GAME_CURSOR1_ID,
					    _GAME_COLOR1);
	      lw6ker_game_state_add_cursor (splash_context->game.game_state,
					    _MOD_GL_SPLASH_GAME_SERVER_ID,
					    _MOD_GL_SPLASH_GAME_CURSOR2_ID,
					    _GAME_COLOR2);
	      lw6ker_game_state_add_cursor (splash_context->game.game_state,
					    _MOD_GL_SPLASH_GAME_SERVER_ID,
					    _MOD_GL_SPLASH_GAME_CURSOR3_ID,
					    _GAME_COLOR3);
	      lw6ker_game_state_add_cursor (splash_context->game.game_state,
					    _MOD_GL_SPLASH_GAME_SERVER_ID,
					    _MOD_GL_SPLASH_GAME_CURSOR4_ID,
					    _GAME_COLOR4);
	      splash_context->game.bitmap =
		mod_gl_utils_bitmap_new (utils_context,
					 splash_context->game.game_struct->
					 map_struct.shape.w,
					 splash_context->game.game_struct->
					 map_struct.shape.h);
	      if (splash_context->game.bitmap)
		{
		  mod_gl_utils_bitmap_set_wrap (utils_context,
						splash_context->game.bitmap,
						GL_REPEAT);
		}
	    }
	}
    }

  ret = (splash_context->game.sphere != 0)
    && (splash_context->game.level != NULL)
    && (splash_context->game.game_struct != NULL)
    && (splash_context->game.game_state != NULL)
    && (splash_context->game.bitmap != NULL);

  return ret;
}

static void
_unload_game (mod_gl_utils_context_t * utils_context,
	      _mod_gl_splash_context_t * splash_context)
{
  if (splash_context->game.bitmap)
    {
      mod_gl_utils_bitmap_free (utils_context, splash_context->game.bitmap);
    }
  if (splash_context->game.game_state)
    {
      lw6ker_game_state_free (splash_context->game.game_state);
    }
  if (splash_context->game.game_struct)
    {
      lw6ker_game_struct_free (splash_context->game.game_struct);
    }
  if (splash_context->game.level)
    {
      lw6map_free (splash_context->game.level);
    }
  if (splash_context->game.sphere)
    {
      gluDeleteQuadric (splash_context->game.sphere);
    }
  memset (&splash_context->game, 0, sizeof (_mod_gl_splash_game_t));
}

int
_mod_gl_splash_load_data (mod_gl_utils_context_t * utils_context,
			  _mod_gl_splash_context_t * splash_context)
{
  int ret = 0;

  ret = _load_consts (utils_context, splash_context)
    && _load_bitmaps (utils_context, splash_context)
    && _load_game (utils_context, splash_context);

  return ret;
}

void
_mod_gl_splash_unload_data (mod_gl_utils_context_t * utils_context,
			    _mod_gl_splash_context_t * splash_context)
{
  _unload_game (utils_context, splash_context);
  _unload_bitmaps (utils_context, splash_context);
  _unload_consts (utils_context, splash_context);
}
