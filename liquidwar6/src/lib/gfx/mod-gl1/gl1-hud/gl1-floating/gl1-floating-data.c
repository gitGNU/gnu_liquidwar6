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
#include "gl1-floating.h"
#include "gl1-floating-internal.h"

#define CONST_FILE "hud/floating/gl1-floating-const.xml"

#define IMAGE_DIR "hud/floating/image/"

#define IMAGE_GAUGE_FRAME "gauge-frame.png"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_gl1_hud_floating_const_data_t *const_data;

  const_data = (_mod_gl1_hud_floating_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "gauge-heartbeat-period", &const_data->gauge_heartbeat_period);
      lw6cfg_read_xml_int (sys_context, key, value, "gauge-slices", &const_data->gauge_slices);
      lw6cfg_read_xml_int (sys_context, key, value, "gauge-loops", &const_data->gauge_loops);
      lw6cfg_read_xml_int (sys_context, key, value, "score-pie-rotation-period", &const_data->score_pie_rotation_period);
      lw6cfg_read_xml_int (sys_context, key, value, "score-pie-heartbeat-period", &const_data->score_pie_heartbeat_period);
      lw6cfg_read_xml_int (sys_context, key, value, "score-pie-slices", &const_data->score_pie_slices);
      lw6cfg_read_xml_int (sys_context, key, value, "score-pie-loops", &const_data->score_pie_loops);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "clock-y1", &const_data->clock_y1);
      lw6cfg_read_xml_float (sys_context, key, value, "clock-x2", &const_data->clock_x2);
      lw6cfg_read_xml_float (sys_context, key, value, "clock-size", &const_data->clock_size);
      lw6cfg_read_xml_float (sys_context, key, value, "clock-dw", &const_data->clock_dw);
      lw6cfg_read_xml_float (sys_context, key, value, "clock-dh", &const_data->clock_dh);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-bg-inner", &const_data->gauge_relative_bg_inner);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-bg-outer", &const_data->gauge_relative_bg_outer);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-fg-inner", &const_data->gauge_relative_fg_inner);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-fg-outer", &const_data->gauge_relative_fg_outer);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-text-size", &const_data->gauge_relative_text_size);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-text-dw", &const_data->gauge_relative_text_dw);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-text-dh", &const_data->gauge_relative_text_dh);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-percent-only-y1", &const_data->gauge_relative_percent_only_y1);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-percent-y1", &const_data->gauge_relative_percent_y1);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-frags-y1", &const_data->gauge_relative_frags_y1);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-heartbeat-min", &const_data->gauge_relative_heartbeat_min);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-relative-heartbeat-max", &const_data->gauge_relative_heartbeat_max);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-start", &const_data->gauge_start);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-sweep", &const_data->gauge_sweep);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-min-size", &const_data->gauge_min_size);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-max-size", &const_data->gauge_max_size);
      lw6cfg_read_xml_float (sys_context, key, value, "gauge-max-health-angle", &const_data->gauge_max_health_angle);
      lw6cfg_read_xml_float (sys_context, key, value, "weapon-ambiance-alpha1", &const_data->weapon_ambiance_alpha1);
      lw6cfg_read_xml_float (sys_context, key, value, "weapon-ambiance-alpha2", &const_data->weapon_ambiance_alpha2);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-heartbeat-factor1", &const_data->score_pie_heartbeat_factor1);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-heartbeat-factor2", &const_data->score_pie_heartbeat_factor2);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-inner", &const_data->score_pie_inner);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-outer", &const_data->score_pie_outer);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-offset", &const_data->score_pie_offset);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-text-radius-min", &const_data->score_pie_text_radius_min);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-text-radius-max", &const_data->score_pie_text_radius_max);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-text-size", &const_data->score_pie_text_size);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-text-dw", &const_data->score_pie_text_dw);
      lw6cfg_read_xml_float (sys_context, key, value, "score-pie-text-dh", &const_data->score_pie_text_dh);
    }
}

static int
_load_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, utils_context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) &(floating_context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

static void
_unload_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  memset (&floating_context->const_data, 0, sizeof (_mod_gl1_hud_floating_const_data_t));
}

static int
_load_images (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading images"));

  ret = ((floating_context->gauges.gauge_frame = mod_gl1_utils_bitmap_load (sys_context, utils_context, IMAGE_DIR IMAGE_GAUGE_FRAME)) != NULL);
  return ret;
}

/*
 * Free memory.
 */
static void
_unload_images (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  if (floating_context->gauges.gauge_frame)
    {
      mod_gl1_utils_bitmap_free (sys_context, utils_context, floating_context->gauges.gauge_frame);
      floating_context->gauges.gauge_frame = NULL;
    }
}

/*
 * Putting all the load/unload functions together
 */
int
_mod_gl1_hud_floating_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 1;

  ret = ret && _load_consts (sys_context, utils_context, floating_context);
  ret = ret && _load_images (sys_context, utils_context, floating_context);

  return ret;
}

void
_mod_gl1_hud_floating_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  _unload_consts (sys_context, utils_context, floating_context);
  _unload_images (sys_context, utils_context, floating_context);
}
