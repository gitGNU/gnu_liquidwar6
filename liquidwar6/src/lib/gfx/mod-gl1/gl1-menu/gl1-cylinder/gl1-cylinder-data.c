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

#include "../../../gfx.h"
#include "../../../../cfg/cfg.h"
#include "gl1-cylinder.h"
#include "gl1-cylinder-internal.h"

#define CONST_FILE "menu/cylinder/gl1-cylinder-const.xml"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_gl1_menu_cylinder_const_data_t *const_data;

  const_data = (_mod_gl1_menu_cylinder_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "max-displayed-items", &const_data->max_displayed_items);
      lw6cfg_read_xml_int (sys_context, key, value, "slices1", &const_data->slices1);
      lw6cfg_read_xml_int (sys_context, key, value, "slices-min", &const_data->slices_min);
      lw6cfg_read_xml_int (sys_context, key, value, "stacks", &const_data->stacks);
      lw6cfg_read_xml_int (sys_context, key, value, "oscil-period", &const_data->oscil_period);
      lw6cfg_read_xml_int (sys_context, key, value, "nb-spheres", &const_data->nb_spheres);
      lw6cfg_read_xml_int (sys_context, key, value, "sphere-slices1", &const_data->sphere_slices1);
      lw6cfg_read_xml_int (sys_context, key, value, "sphere-slices-min", &const_data->sphere_slices_min);
      lw6cfg_read_xml_int (sys_context, key, value, "sphere-blink-period", &const_data->sphere_blink_period);
      lw6cfg_read_xml_int (sys_context, key, value, "tooltip-max-width", &const_data->tooltip_max_width);
      lw6cfg_read_xml_int (sys_context, key, value, "tooltip-max-height", &const_data->tooltip_max_height);
      lw6cfg_read_xml_int (sys_context, key, value, "tooltip-border-size", &const_data->tooltip_border_size);
      lw6cfg_read_xml_int (sys_context, key, value, "tooltip-margin-size", &const_data->tooltip_margin_size);
      lw6cfg_read_xml_int (sys_context, key, value, "tooltip-reformat-width", &const_data->tooltip_reformat_width);
      lw6cfg_read_xml_int (sys_context, key, value, "help-max-width", &const_data->help_max_width);
      lw6cfg_read_xml_int (sys_context, key, value, "help-max-height", &const_data->help_max_height);
      lw6cfg_read_xml_int (sys_context, key, value, "help-border-size", &const_data->help_border_size);
      lw6cfg_read_xml_int (sys_context, key, value, "help-margin-size", &const_data->help_margin_size);
      lw6cfg_read_xml_int (sys_context, key, value, "help-reformat-width", &const_data->help_reformat_width);
      lw6cfg_read_xml_int (sys_context, key, value, "breadcrumbs-max-width", &const_data->breadcrumbs_max_width);
      lw6cfg_read_xml_int (sys_context, key, value, "breadcrumbs-max-height", &const_data->breadcrumbs_max_height);
      lw6cfg_read_xml_int (sys_context, key, value, "breadcrumbs-border-size", &const_data->breadcrumbs_border_size);
      lw6cfg_read_xml_int (sys_context, key, value, "breadcrumbs-margin-size", &const_data->breadcrumbs_margin_size);
      lw6cfg_read_xml_int (sys_context, key, value, "breadcrumbs-reformat-width", &const_data->breadcrumbs_reformat_width);
      lw6cfg_read_xml_int (sys_context, key, value, "popup-max-width", &const_data->popup_max_width);
      lw6cfg_read_xml_int (sys_context, key, value, "popup-max-height", &const_data->popup_max_height);
      lw6cfg_read_xml_int (sys_context, key, value, "popup-border-size", &const_data->popup_border_size);
      lw6cfg_read_xml_int (sys_context, key, value, "popup-margin-size", &const_data->popup_margin_size);
      lw6cfg_read_xml_int (sys_context, key, value, "popup-reformat-width", &const_data->popup_reformat_width);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "radius1", &const_data->radius1);
      lw6cfg_read_xml_float (sys_context, key, value, "between1", &const_data->between1);
      lw6cfg_read_xml_float (sys_context, key, value, "cyl-height", &const_data->cyl_height);
      lw6cfg_read_xml_float (sys_context, key, value, "oscil-range1", &const_data->oscil_range1);
      lw6cfg_read_xml_float (sys_context, key, value, "texture-offset", &const_data->texture_offset);
      lw6cfg_read_xml_float (sys_context, key, value, "sphere-radius1", &const_data->sphere_radius1);
      lw6cfg_read_xml_float (sys_context, key, value, "sphere-between1", &const_data->sphere_between1);
      lw6cfg_read_xml_float (sys_context, key, value, "progress-offset", &const_data->progress_offset);
      lw6cfg_read_xml_float (sys_context, key, value, "progress-radius", &const_data->progress_radius);
      lw6cfg_read_xml_float (sys_context, key, value, "progress-cyl-height", &const_data->progress_cyl_height);
      lw6cfg_read_xml_float (sys_context, key, value, "esc-offset", &const_data->esc_offset);
      lw6cfg_read_xml_float (sys_context, key, value, "esc-radius", &const_data->esc_radius);
      lw6cfg_read_xml_float (sys_context, key, value, "esc-cyl-height", &const_data->esc_cyl_height);
      lw6cfg_read_xml_float (sys_context, key, value, "esc-cyl-height-offset", &const_data->esc_cyl_height_offset);
      lw6cfg_read_xml_float (sys_context, key, value, "esc-rotate", &const_data->esc_rotate);
      lw6cfg_read_xml_float (sys_context, key, value, "tooltip-alpha-bg", &const_data->tooltip_alpha_bg);
      lw6cfg_read_xml_float (sys_context, key, value, "tooltip-relative-size", &const_data->tooltip_relative_size);
      lw6cfg_read_xml_float (sys_context, key, value, "help-alpha-bg", &const_data->help_alpha_bg);
      lw6cfg_read_xml_float (sys_context, key, value, "help-relative-size", &const_data->help_relative_size);
      lw6cfg_read_xml_float (sys_context, key, value, "breadcrumbs-alpha-bg", &const_data->breadcrumbs_alpha_bg);
      lw6cfg_read_xml_float (sys_context, key, value, "breadcrumbs-relative-size", &const_data->breadcrumbs_relative_size);
      lw6cfg_read_xml_float (sys_context, key, value, "popup-alpha-bg", &const_data->popup_alpha_bg);
      lw6cfg_read_xml_float (sys_context, key, value, "popup-relative-size", &const_data->popup_relative_size);
    }
}

static int
_load_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, utils_context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) &(cylinder_context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

static void
_unload_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context)
{
  memset (&cylinder_context->const_data, 0, sizeof (_mod_gl1_menu_cylinder_const_data_t));
}

/*
 * Putting all the load/unload functions together
 */
int
_mod_gl1_menu_cylinder_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context)
{
  return _load_consts (sys_context, utils_context, cylinder_context);
}

int
mod_gl1_menu_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *cylinder_context)
{
  return _mod_gl1_menu_cylinder_load_data (sys_context, utils_context, (_mod_gl1_menu_cylinder_context_t *) cylinder_context);
}

void
_mod_gl1_menu_cylinder_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				    _mod_gl1_menu_cylinder_context_t * cylinder_context)
{
  _unload_consts (sys_context, utils_context, cylinder_context);
}

void
mod_gl1_menu_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *cylinder_context)
{
  _mod_gl1_menu_cylinder_unload_data (sys_context, utils_context, (_mod_gl1_menu_cylinder_context_t *) cylinder_context);
}
