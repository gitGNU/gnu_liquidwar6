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

#include "mod-caca-internal.h"

#define CONST_FILE "caca-const.xml"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_caca_const_data_t *const_data;

  const_data = (_mod_caca_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "event-timeout-microseconds", &const_data->event_timeout_microseconds);
      lw6cfg_read_xml_int (sys_context, key, value, "canvas-create-width", &const_data->canvas_create_width);
      lw6cfg_read_xml_int (sys_context, key, value, "canvas-create-height", &const_data->canvas_create_height);

      /* KEYS */
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-up", &const_data->keysym1_up);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-up", &const_data->keysym2_up);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym3-up", &const_data->keysym3_up);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym4-up", &const_data->keysym4_up);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-down", &const_data->keysym1_down);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-down", &const_data->keysym2_down);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym3-down", &const_data->keysym3_down);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym4-down", &const_data->keysym4_down);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-left", &const_data->keysym1_left);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-left", &const_data->keysym2_left);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym3-left", &const_data->keysym3_left);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym4-left", &const_data->keysym4_left);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-right", &const_data->keysym1_right);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-right", &const_data->keysym2_right);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym3-right", &const_data->keysym3_right);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym4-right", &const_data->keysym4_right);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-enter", &const_data->keysym1_enter);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-enter", &const_data->keysym2_enter);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-esc", &const_data->keysym1_esc);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-esc", &const_data->keysym2_esc);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-ctrl", &const_data->keysym1_ctrl);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-ctrl", &const_data->keysym2_ctrl);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-alt", &const_data->keysym1_alt);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-alt", &const_data->keysym2_alt);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-pgup", &const_data->keysym1_pgup);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-pgup", &const_data->keysym2_pgup);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym1-pgdown", &const_data->keysym1_pgdown);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym2-pgdown", &const_data->keysym2_pgdown);
      lw6cfg_read_xml_int (sys_context, key, value, "keysym-quit", &const_data->keysym_quit);
    }
  if (!strcmp (element, "color"))
    {
      lw6cfg_read_xml_color (sys_context, key, value, "color-bg", &const_data->bg_color);
      lw6cfg_read_xml_color (sys_context, key, value, "color-fg", &const_data->fg_color);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color0", &const_data->team_color[0]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color1", &const_data->team_color[1]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color2", &const_data->team_color[2]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color3", &const_data->team_color[3]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color4", &const_data->team_color[4]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color5", &const_data->team_color[5]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color6", &const_data->team_color[6]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color7", &const_data->team_color[7]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color8", &const_data->team_color[8]);
      lw6cfg_read_xml_color (sys_context, key, value, "team_color9", &const_data->team_color[9]);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "quit-sleep", &const_data->quit_sleep);
    }

  if (!strcmp (element, "string"))
    {
      lw6cfg_read_xml_string (sys_context, key, value, "video-driver-1", &(const_data->video_driver[0]));
      lw6cfg_read_xml_string (sys_context, key, value, "video-driver-2", &(const_data->video_driver[1]));
      lw6cfg_read_xml_string (sys_context, key, value, "video-driver-3", &(const_data->video_driver[2]));
      lw6cfg_read_xml_string (sys_context, key, value, "video-driver-4", &(const_data->video_driver[3]));
    }
}

/*
 * Loads constants.
 */
int
_mod_caca_load_consts (lw6sys_context_t * sys_context, _mod_caca_context_t * context)
{
  int ret = 0;
  char *const_file = NULL;
  int i = 0;

  const_file = lw6sys_path_concat (sys_context, context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) &(context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  for (i = 0; i < _MOD_CACA_NB_VIDEO_DRIVERS; ++i)
    {
      if (!context->const_data.video_driver[i])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad caca consts, \"video-driver-%d\" (i=%d) not found"), i + 1, i);
	  ret = 0;
	}
    }

  return ret;
}

/*
 * Unload constants, free memory
 */
void
_mod_caca_unload_consts (lw6sys_context_t * sys_context, _mod_caca_context_t * context)
{
  int i = 0;

  for (i = 0; i < _MOD_CACA_NB_VIDEO_DRIVERS; ++i)
    {
      if (context->const_data.video_driver[i])
	{
	  LW6SYS_FREE (sys_context, context->const_data.video_driver[i]);
	}
    }

  memset (&context->const_data, 0, sizeof (_mod_caca_const_data_t));
}
