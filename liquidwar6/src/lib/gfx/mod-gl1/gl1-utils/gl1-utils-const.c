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

#include <string.h>

#include "../../gfx.h"
#include "../../../cfg/cfg.h"
#include "gl1-utils.h"

#define CONST_FILE "utils/gl1-utils-const.xml"

static void
read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  mod_gl1_utils_const_data_t *const_data;

  const_data = (mod_gl1_utils_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "menu-font-size", &const_data->menu_font_size);
      lw6cfg_read_xml_int (sys_context, key, value, "hud-font-size", &const_data->hud_font_size);
      lw6cfg_read_xml_int (sys_context, key, value, "cursor-font-size", &const_data->cursor_font_size);
      lw6cfg_read_xml_int (sys_context, key, value, "system-font-size", &const_data->system_font_size);
      lw6cfg_read_xml_int (sys_context, key, value, "system-font-dh", &const_data->system_font_dh);
      lw6cfg_read_xml_int (sys_context, key, value, "system-font-dw", &const_data->system_font_dw);
      lw6cfg_read_xml_int (sys_context, key, value, "smoother-global-zoom-duration", &const_data->smoother_global_zoom_duration);
      lw6cfg_read_xml_int (sys_context, key, value, "smoother-center-duration", &const_data->smoother_center_duration);
      lw6cfg_read_xml_int (sys_context, key, value, "smoother-drawable-duration", &const_data->smoother_drawable_duration);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "persp-fovy", &const_data->persp_fovy);
      lw6cfg_read_xml_float (sys_context, key, value, "persp-znear", &const_data->persp_znear);
      lw6cfg_read_xml_float (sys_context, key, value, "persp-zfar", &const_data->persp_zfar);
      lw6cfg_read_xml_float (sys_context, key, value, "system-font-hcoef", &const_data->system_font_hcoef);
      lw6cfg_read_xml_float (sys_context, key, value, "gradient-opacity", &const_data->gradient_opacity);
      lw6cfg_read_xml_float (sys_context, key, value, "zones-opacity", &const_data->zones_opacity);
      lw6cfg_read_xml_float (sys_context, key, value, "mainlight-r", &const_data->mainlight_r);
      lw6cfg_read_xml_float (sys_context, key, value, "mainlight-g", &const_data->mainlight_g);
      lw6cfg_read_xml_float (sys_context, key, value, "mainlight-b", &const_data->mainlight_b);
      lw6cfg_read_xml_float (sys_context, key, value, "mainlight-a", &const_data->mainlight_a);
      lw6cfg_read_xml_float (sys_context, key, value, "mainlight-x", &const_data->mainlight_x);
      lw6cfg_read_xml_float (sys_context, key, value, "mainlight-y", &const_data->mainlight_y);
      lw6cfg_read_xml_float (sys_context, key, value, "mainlight-z", &const_data->mainlight_z);
    }
}

/*
 * Loads constants.
 */
int
mod_gl1_utils_load_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, read_callback, (void *) &(context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

/*
 * Unload constants, free memory
 */
void
mod_gl1_utils_unload_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context)
{
  memset (&context->const_data, 0, sizeof (mod_gl1_utils_const_data_t));
}
