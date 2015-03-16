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
#include "gl1-bubbles.h"
#include "gl1-bubbles-internal.h"

#define CONST_FILE "background/bubbles/gl1-bubbles-const.xml"

#define IMAGE_DIR "background/bubbles/image/"

#define IMAGE_BACKGROUND "fumes.png"
#define IMAGE_BUBBLE "bubble.png"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_gl1_background_bubbles_const_data_t *const_data;

  const_data = (_mod_gl1_background_bubbles_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "nb-bubbles", &const_data->nb_bubbles);
    }

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "yspeed", &const_data->yspeed);
      lw6cfg_read_xml_float (sys_context, key, value, "bubble-yspeed", &const_data->bubble_yspeed);
      lw6cfg_read_xml_float (sys_context, key, value, "bubble-size-min", &const_data->bubble_size_min);
      lw6cfg_read_xml_float (sys_context, key, value, "bubble-size-max", &const_data->bubble_size_max);
    }
}

static int
_load_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, utils_context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) &(bubbles_context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

static void
_unload_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context)
{
  memset (&bubbles_context->const_data, 0, sizeof (_mod_gl1_background_bubbles_const_data_t));
}

/*
 * Loads images from disk.
 */
static int
_load_bitmaps (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading bitmaps"));

  ret =
    ((bubbles_context->bitmap_data.background =
      mod_gl1_utils_bitmap_load (sys_context, utils_context,
				 IMAGE_DIR IMAGE_BACKGROUND)) != NULL)
    && ((bubbles_context->bitmap_data.bubble = mod_gl1_utils_bitmap_load (sys_context, utils_context, IMAGE_DIR IMAGE_BUBBLE)) != NULL);

  if (bubbles_context->bitmap_data.background)
    {
      mod_gl1_utils_bitmap_set_wrap (sys_context, utils_context, bubbles_context->bitmap_data.background, GL_REPEAT);
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't load bitmaps, expect serious problems"));
    }

  return ret;
}

/*
 * Free memory.
 */
static void
_unload_bitmaps (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context)
{
  mod_gl1_utils_bitmap_free (sys_context, utils_context, bubbles_context->bitmap_data.background);
  mod_gl1_utils_bitmap_free (sys_context, utils_context, bubbles_context->bitmap_data.bubble);

  memset (&bubbles_context->bitmap_data, 0, sizeof (_mod_gl1_background_bubbles_bitmap_data_t));
}

/*
 * Putting all the load/unload functions together
 */
int
_mod_gl1_background_bubbles_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				       _mod_gl1_background_bubbles_context_t * bubbles_context)
{
  return _load_consts (sys_context, utils_context, bubbles_context) && _load_bitmaps (sys_context, utils_context, bubbles_context);
}

void
_mod_gl1_background_bubbles_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					 _mod_gl1_background_bubbles_context_t * bubbles_context)
{
  _unload_bitmaps (sys_context, utils_context, bubbles_context);
  _unload_consts (sys_context, utils_context, bubbles_context);
}
