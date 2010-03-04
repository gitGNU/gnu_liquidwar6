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

#include <string.h>

#include "../../gfx.h"
#include "../../../cfg/cfg.h"
#include "gl-utils.h"

#define CONST_FILE "utils/gl-utils-const.xml"

static void
read_callback (void *callback_data, char *element, char *key, char *value)
{
  mod_gl_utils_const_data_t *const_data;

  const_data = (mod_gl_utils_const_data_t *) callback_data;

  if (!strcmp (element, "bool"))
    {
      lw6cfg_read_xml_bool (key, value, "joystick-all-buttons",
			    &const_data->joystick_all_buttons);
    }

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (key, value, "menu-font-size",
			   &const_data->menu_font_size);
      lw6cfg_read_xml_int (key, value, "hud-font-size",
			   &const_data->hud_font_size);
      lw6cfg_read_xml_int (key, value, "cursor-font-size",
			   &const_data->cursor_font_size);
      lw6cfg_read_xml_int (key, value, "system-font-size",
			   &const_data->system_font_size);
      lw6cfg_read_xml_int (key, value, "system-font-dw",
			   &const_data->system_font_dw);
      lw6cfg_read_xml_int (key, value, "system-font-dh",
			   &const_data->system_font_dh);
      lw6cfg_read_xml_int (key, value, "keysym1-up", &const_data->keysym1_up);
      lw6cfg_read_xml_int (key, value, "keysym2-up", &const_data->keysym2_up);
      lw6cfg_read_xml_int (key, value, "keysym3-up", &const_data->keysym3_up);
      lw6cfg_read_xml_int (key, value, "keysym4-up", &const_data->keysym4_up);
      lw6cfg_read_xml_int (key, value, "keysym1-down",
			   &const_data->keysym1_down);
      lw6cfg_read_xml_int (key, value, "keysym2-down",
			   &const_data->keysym2_down);
      lw6cfg_read_xml_int (key, value, "keysym3-down",
			   &const_data->keysym3_down);
      lw6cfg_read_xml_int (key, value, "keysym4-down",
			   &const_data->keysym4_down);
      lw6cfg_read_xml_int (key, value, "keysym1-left",
			   &const_data->keysym1_left);
      lw6cfg_read_xml_int (key, value, "keysym2-left",
			   &const_data->keysym2_left);
      lw6cfg_read_xml_int (key, value, "keysym3-left",
			   &const_data->keysym3_left);
      lw6cfg_read_xml_int (key, value, "keysym4-left",
			   &const_data->keysym4_left);
      lw6cfg_read_xml_int (key, value, "keysym1-right",
			   &const_data->keysym1_right);
      lw6cfg_read_xml_int (key, value, "keysym2-right",
			   &const_data->keysym2_right);
      lw6cfg_read_xml_int (key, value, "keysym3-right",
			   &const_data->keysym3_right);
      lw6cfg_read_xml_int (key, value, "keysym4-right",
			   &const_data->keysym4_right);
      lw6cfg_read_xml_int (key, value, "keysym1-enter",
			   &const_data->keysym1_enter);
      lw6cfg_read_xml_int (key, value, "keysym2-enter",
			   &const_data->keysym2_enter);
      lw6cfg_read_xml_int (key, value, "keysym1-esc",
			   &const_data->keysym1_esc);
      lw6cfg_read_xml_int (key, value, "keysym2-esc",
			   &const_data->keysym2_esc);
      lw6cfg_read_xml_int (key, value, "keysym1-pgup",
			   &const_data->keysym1_pgup);
      lw6cfg_read_xml_int (key, value, "keysym2-pgup",
			   &const_data->keysym2_pgup);
      lw6cfg_read_xml_int (key, value, "keysym1-pgdown",
			   &const_data->keysym1_pgdown);
      lw6cfg_read_xml_int (key, value, "keysym2-pgdown",
			   &const_data->keysym2_pgdown);
      lw6cfg_read_xml_int (key, value, "keysym-quit",
			   &const_data->keysym_quit);
      lw6cfg_read_xml_int (key, value, "joystick-limit",
			   &const_data->joystick_limit);
      lw6cfg_read_xml_int (key, value, "joystick1-index",
			   &const_data->joystick1_index);
      lw6cfg_read_xml_int (key, value, "joystick1-button-a-index",
			   &const_data->joystick1_button_a_index);
      lw6cfg_read_xml_int (key, value, "joystick1-button-b-index",
			   &const_data->joystick1_button_b_index);
      lw6cfg_read_xml_int (key, value, "joystick1-button-c-index",
			   &const_data->joystick1_button_c_index);
      lw6cfg_read_xml_int (key, value, "joystick1-button-d-index",
			   &const_data->joystick1_button_d_index);
      lw6cfg_read_xml_int (key, value, "joystick2-index",
			   &const_data->joystick2_index);
      lw6cfg_read_xml_int (key, value, "joystick2-button-a-index",
			   &const_data->joystick2_button_a_index);
      lw6cfg_read_xml_int (key, value, "joystick2-button-b-index",
			   &const_data->joystick2_button_b_index);
      lw6cfg_read_xml_int (key, value, "joystick2-button-c-index",
			   &const_data->joystick2_button_c_index);
      lw6cfg_read_xml_int (key, value, "joystick2-button-d-index",
			   &const_data->joystick2_button_d_index);
      lw6cfg_read_xml_int (key, value, "smoother-global-zoom-duration",
			   &const_data->smoother_global_zoom_duration);
      lw6cfg_read_xml_int (key, value, "smoother-center-duration",
			   &const_data->smoother_center_duration);
      lw6cfg_read_xml_int (key, value, "smoother-drawable-duration",
			   &const_data->smoother_drawable_duration);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (key, value, "warp-x", &const_data->warp_x);
      lw6cfg_read_xml_float (key, value, "warp-y", &const_data->warp_y);
      lw6cfg_read_xml_float (key, value, "mode-sleep",
			     &const_data->mode_sleep);
      lw6cfg_read_xml_float (key, value, "quit-sleep",
			     &const_data->quit_sleep);
      lw6cfg_read_xml_float (key, value, "persp-fovy",
			     &const_data->persp_fovy);
      lw6cfg_read_xml_float (key, value, "persp-znear",
			     &const_data->persp_znear);
      lw6cfg_read_xml_float (key, value, "persp-zfar",
			     &const_data->persp_zfar);
      lw6cfg_read_xml_float (key, value, "system-font-hcoef",
			     &const_data->system_font_hcoef);
      lw6cfg_read_xml_float (key, value, "gradient-opacity",
			     &const_data->gradient_opacity);
      lw6cfg_read_xml_float (key, value, "zones-opacity",
			     &const_data->zones_opacity);
      lw6cfg_read_xml_float (key, value, "mainlight-r",
			     &const_data->mainlight_r);
      lw6cfg_read_xml_float (key, value, "mainlight-g",
			     &const_data->mainlight_g);
      lw6cfg_read_xml_float (key, value, "mainlight-b",
			     &const_data->mainlight_b);
      lw6cfg_read_xml_float (key, value, "mainlight-a",
			     &const_data->mainlight_a);
      lw6cfg_read_xml_float (key, value, "mainlight-x",
			     &const_data->mainlight_x);
      lw6cfg_read_xml_float (key, value, "mainlight-y",
			     &const_data->mainlight_y);
      lw6cfg_read_xml_float (key, value, "mainlight-z",
			     &const_data->mainlight_z);
    }
}

/*
 * Loads constants.
 */
int
mod_gl_utils_load_consts (mod_gl_utils_context_t * context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("reading \"%s\""), const_file);

      ret =
	lw6cfg_read_key_value_xml_file (const_file, read_callback,
					(void *) &(context->const_data));

      LW6SYS_FREE (const_file);
    }

  return ret;
}

/*
 * Unload constants, free memory
 */
void
mod_gl_utils_unload_consts (mod_gl_utils_context_t * context)
{
  memset (&context->const_data, 0, sizeof (mod_gl_utils_const_data_t));
}
