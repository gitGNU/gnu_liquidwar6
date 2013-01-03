/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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
read_callback (void *callback_data, const char *element, const char *key,
	       const char *value)
{
  _mod_caca_const_data_t *const_data;

  const_data = (_mod_caca_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (key, value, "event-timeout-microseconds",
			   &const_data->event_timeout_microseconds);
      lw6cfg_read_xml_int (key, value, "canvas-create-width",
			   &const_data->canvas_create_width);
      lw6cfg_read_xml_int (key, value, "canvas-create-height",
			   &const_data->canvas_create_height);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (key, value, "quit-sleep",
			     &const_data->quit_sleep);
    }

  if (!strcmp (element, "string"))
    {
      lw6cfg_read_xml_string (key, value, "video-mode-default",
			      &const_data->video_mode_default);
      lw6cfg_read_xml_string (key, value, "video-mode-fallback",
			      &const_data->video_mode_fallback);
    }
}

/*
 * Loads constants.
 */
int
_mod_caca_load_consts (_mod_caca_context_t * context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret =
	lw6cfg_read_key_value_xml_file (const_file, read_callback,
					(void *) &(context->const_data));

      LW6SYS_FREE (const_file);
    }

  if (context->const_data.video_mode_default
      && context->const_data.video_mode_fallback)
    {
      // OK
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bad caca consts"));
      ret = 0;
    }

  return ret;
}

/*
 * Unload constants, free memory
 */
void
_mod_caca_unload_consts (_mod_caca_context_t * context)
{
  if (context->const_data.video_mode_default)
    {
      LW6SYS_FREE (context->const_data.video_mode_default);
    }
  if (context->const_data.video_mode_fallback)
    {
      LW6SYS_FREE (context->const_data.video_mode_fallback);
    }
  memset (&context->const_data, 0, sizeof (_mod_caca_const_data_t));
}
