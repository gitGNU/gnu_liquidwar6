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
#include "gl1-tactical.h"
#include "gl1-tactical-internal.h"

#define CONST_FILE "hud/tactical/const.xml"

#define IMAGE_DIR "hud/tactical/image/"

#define IMAGE_FRAME "frame.png"

static void
read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_gl1_hud_tactical_const_data_t *const_data;

  const_data = (_mod_gl1_hud_tactical_const_data_t *) callback_data;

  if (!strcmp (element, "float"))
    {
      lw6cfg_read_xml_float (sys_context, key, value, "viewport-x", &const_data->viewport_x);
      lw6cfg_read_xml_float (sys_context, key, value, "viewport-y", &const_data->viewport_y);
      lw6cfg_read_xml_float (sys_context, key, value, "viewport-w", &const_data->viewport_w);
      lw6cfg_read_xml_float (sys_context, key, value, "viewport-h", &const_data->viewport_h);
    }
}

static int
_load_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_tactical_context_t * tactical_context)
{
  int ret = 0;
  char *const_file = NULL;

  const_file = lw6sys_path_concat (sys_context, utils_context->path.data_dir, CONST_FILE);

  if (const_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

      ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, read_callback, (void *) &(tactical_context->const_data));

      LW6SYS_FREE (sys_context, const_file);
    }

  return ret;
}

static void
_unload_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_tactical_context_t * tactical_context)
{
  memset (&tactical_context->const_data, 0, sizeof (_mod_gl1_hud_tactical_const_data_t));
}

/*
 * Loads bitmaps from disk.
 */
static int
_load_bitmaps (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_tactical_context_t * tactical_context)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading bitmaps"));

  ret = ((tactical_context->bitmap_data.frame = mod_gl1_utils_bitmap_load (sys_context, utils_context, IMAGE_DIR IMAGE_FRAME)) != NULL);

  if (!ret)
    {
      /*
       * If we can't load bitmaps, we simply exit the game...
       */
      exit (1);
    }

  return ret;
}

/*
 * Free memory.
 */
static void
_unload_bitmaps (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_tactical_context_t * tactical_context)
{
  mod_gl1_utils_bitmap_free (sys_context, utils_context, tactical_context->bitmap_data.frame);

  memset (&tactical_context->bitmap_data, 0, sizeof (_mod_gl1_hud_tactical_bitmap_data_t));
}

/*
 * Putting all the load/unload functions together
 */
int
_mod_gl1_hud_tactical_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_tactical_context_t * tactical_context)
{
  return _load_consts (sys_context, utils_context, tactical_context) && _load_bitmaps (sys_context, utils_context, tactical_context);
}

void
_mod_gl1_hud_tactical_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_tactical_context_t * tactical_context)
{
  _unload_consts (sys_context, utils_context, tactical_context);
  _unload_bitmaps (sys_context, utils_context, tactical_context);
}
