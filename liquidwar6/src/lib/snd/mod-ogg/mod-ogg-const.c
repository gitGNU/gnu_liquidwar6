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

#include "../../cfg/cfg.h"
#include "mod-ogg-internal.h"

#define CONST_FILE "ogg-const.xml"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_ogg_const_data_t *const_data;

  const_data = (_mod_ogg_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "frequency", &const_data->frequency);
      lw6cfg_read_xml_int (sys_context, key, value, "bits", &const_data->bits);
      lw6cfg_read_xml_int (sys_context, key, value, "channels", &const_data->channels);
      lw6cfg_read_xml_int (sys_context, key, value, "chunksize", &const_data->chunksize);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "water-pan1", &const_data->water_pan1);
      lw6cfg_read_xml_float (sys_context, key, value, "water-pan2", &const_data->water_pan2);
    }

  if (!strcmp (element, "string"))
    {
      lw6cfg_read_xml_string (sys_context, key, value, "file-splash", &const_data->file_splash);
      lw6cfg_read_xml_string (sys_context, key, value, "file-valid", &const_data->file_valid);
      lw6cfg_read_xml_string (sys_context, key, value, "file-select", &const_data->file_select);
      lw6cfg_read_xml_string (sys_context, key, value, "file-no", &const_data->file_no);
      lw6cfg_read_xml_string (sys_context, key, value, "file-start", &const_data->file_start);
      lw6cfg_read_xml_string (sys_context, key, value, "file-death", &const_data->file_death);
      lw6cfg_read_xml_string (sys_context, key, value, "file-bell", &const_data->file_bell);
      lw6cfg_read_xml_string (sys_context, key, value, "file-score", &const_data->file_score);
      lw6cfg_read_xml_string (sys_context, key, value, "file-hello", &const_data->file_hello);
      lw6cfg_read_xml_string (sys_context, key, value, "file-goodbye", &const_data->file_goodbye);
      lw6cfg_read_xml_string (sys_context, key, value, "file-water1", &const_data->file_water1);
      lw6cfg_read_xml_string (sys_context, key, value, "file-water2", &const_data->file_water2);
      lw6cfg_read_xml_string (sys_context, key, value, "file-water3", &const_data->file_water3);
      lw6cfg_read_xml_string (sys_context, key, value, "file-water4", &const_data->file_water4);
      lw6cfg_read_xml_string (sys_context, key, value, "file-water5", &const_data->file_water5);
      lw6cfg_read_xml_string (sys_context, key, value, "file-water6", &const_data->file_water6);
    }
}

/*
 * Loads constants.
 */
int
_mod_ogg_load_consts (lw6sys_context_t * sys_context, _mod_ogg_context_t * context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) &(context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

/*
 * Unload constants, free memory
 */
void
_mod_ogg_unload_consts (lw6sys_context_t * sys_context, _mod_ogg_context_t * context)
{
  if (context->const_data.file_splash)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_splash);
    }
  if (context->const_data.file_valid)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_valid);
    }
  if (context->const_data.file_select)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_select);
    }
  if (context->const_data.file_no)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_no);
    }
  if (context->const_data.file_start)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_start);
    }
  if (context->const_data.file_death)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_death);
    }
  if (context->const_data.file_bell)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_bell);
    }
  if (context->const_data.file_score)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_score);
    }
  if (context->const_data.file_hello)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_hello);
    }
  if (context->const_data.file_goodbye)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_goodbye);
    }
  if (context->const_data.file_water1)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_water1);
    }
  if (context->const_data.file_water2)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_water2);
    }
  if (context->const_data.file_water3)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_water3);
    }
  if (context->const_data.file_water4)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_water4);
    }
  if (context->const_data.file_water5)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_water5);
    }
  if (context->const_data.file_water6)
    {
      LW6SYS_FREE (sys_context, context->const_data.file_water6);
    }
  memset (&context->const_data, 0, sizeof (_mod_ogg_const_data_t));
}
