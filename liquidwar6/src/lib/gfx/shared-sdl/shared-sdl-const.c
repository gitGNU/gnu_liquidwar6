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

#define CONST_FILE "sdl-const.xml"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _lw6gfx_sdl_const_data_t *const_data;

  const_data = (_lw6gfx_sdl_const_data_t *) callback_data;

  if (!strcmp (element, "bool"))
    {
      lw6cfg_read_xml_bool (sys_context, key, value, "joystick-all-buttons", &const_data->joystick_all_buttons);
    }

  if (!strcmp (element, "int"))
    {
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
      lw6cfg_read_xml_int (sys_context, key, value, "joystick-limit", &const_data->joystick_limit);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick1-index", &const_data->joystick1_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick1-button-a-index", &const_data->joystick1_button_a_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick1-button-b-index", &const_data->joystick1_button_b_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick1-button-c-index", &const_data->joystick1_button_c_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick1-button-d-index", &const_data->joystick1_button_d_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick1-button-e-index", &const_data->joystick1_button_e_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick1-button-f-index", &const_data->joystick1_button_f_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick2-index", &const_data->joystick2_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick2-button-a-index", &const_data->joystick2_button_a_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick2-button-b-index", &const_data->joystick2_button_b_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick2-button-c-index", &const_data->joystick2_button_c_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick2-button-d-index", &const_data->joystick2_button_d_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick2-button-e-index", &const_data->joystick2_button_e_index);
      lw6cfg_read_xml_int (sys_context, key, value, "joystick2-button-f-index", &const_data->joystick2_button_f_index);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "warp-x", &const_data->warp_x);
      lw6cfg_read_xml_float (sys_context, key, value, "warp-y", &const_data->warp_y);
      lw6cfg_read_xml_float (sys_context, key, value, "mode-sleep", &const_data->mode_sleep);
      lw6cfg_read_xml_float (sys_context, key, value, "quit-sleep", &const_data->quit_sleep);
    }
}

/*
 * Loads constants.
 */
int
shared_sdl_load_consts (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, sdl_context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) &(sdl_context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

/*
 * Unload constants, free memory
 */
void
shared_sdl_unload_consts (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context)
{
  memset (&sdl_context->const_data, 0, sizeof (_lw6gfx_sdl_const_data_t));
}
