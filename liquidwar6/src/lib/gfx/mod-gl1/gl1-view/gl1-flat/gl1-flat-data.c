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

#include "../../../gfx.h"
#include "../../../../cfg/cfg.h"
#include "gl1-flat.h"
#include "gl1-flat-internal.h"

#define CONST_FILE "view/flat/gl1-flat-const.xml"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_gl1_view_flat_const_data_t *const_data;

  const_data = (_mod_gl1_view_flat_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "scroll-min-offset", &const_data->scroll_min_offset);
      lw6cfg_read_xml_int (sys_context, key, value, "cursor-blink-period", &const_data->cursor_blink_period);
      lw6cfg_read_xml_int (sys_context, key, value, "cursor-heartbeat-period", &const_data->cursor_heartbeat_period);
      lw6cfg_read_xml_int (sys_context, key, value, "tile-size-map", &const_data->tile_size_map);
      lw6cfg_read_xml_int (sys_context, key, value, "tile-size-armies", &const_data->tile_size_armies);
      lw6cfg_read_xml_int (sys_context, key, value, "border-size-map", &const_data->border_size_map);
      lw6cfg_read_xml_int (sys_context, key, value, "border-size-armies", &const_data->border_size_armies);
      lw6cfg_read_xml_int (sys_context, key, value, "nb-waves", &const_data->nb_waves);
      lw6cfg_read_xml_int (sys_context, key, value, "vertices-per-wave", &const_data->vertices_per_wave);
      lw6cfg_read_xml_int (sys_context, key, value, "waves-period", &const_data->waves_period);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-size", &const_data->cursor_size);
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-size-heartbeat-min", &const_data->cursor_size_heartbeat_min);
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-size-heartbeat-max", &const_data->cursor_size_heartbeat_max);
      lw6cfg_read_xml_float (sys_context, key, value, "scroll-limit", &const_data->scroll_limit);
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-relative-text-y1", &const_data->cursor_relative_text_y1);
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-relative-text-y2", &const_data->cursor_relative_text_y2);
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-relative-text-x1", &const_data->cursor_relative_text_x1);
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-relative-text-dw", &const_data->cursor_relative_text_dw);
      lw6cfg_read_xml_float (sys_context, key, value, "cursor-relative-text-dh", &const_data->cursor_relative_text_dh);
      lw6cfg_read_xml_float (sys_context, key, value, "waves-amplitude", &const_data->waves_amplitude);
    }
}

static int
_load_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, utils_context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) &(flat_context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

static void
_unload_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context)
{
  memset (&flat_context->const_data, 0, sizeof (_mod_gl1_view_flat_const_data_t));
}


/*
 * Putting all the load/unload functions together
 */
int
_mod_gl1_view_flat_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context)
{
  return _load_consts (sys_context, utils_context, flat_context);
}

int
mod_gl1_view_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *flat_context)
{
  return _mod_gl1_view_flat_load_data (sys_context, utils_context, (_mod_gl1_view_flat_context_t *) flat_context);
}

void
_mod_gl1_view_flat_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context)
{
  _unload_consts (sys_context, utils_context, flat_context);
}

void
mod_gl1_view_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *flat_context)
{
  _mod_gl1_view_flat_unload_data (sys_context, utils_context, (_mod_gl1_view_flat_context_t *) flat_context);
}
