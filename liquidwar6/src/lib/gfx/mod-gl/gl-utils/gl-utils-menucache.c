/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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
#include "gl-utils.h"

/*
 * Creates a unique string key for a given menu.
 * Pointer must be freed.
 */
static char *
create_button_key (lw6gui_look_t * look, lw6gui_menuitem_t * menuitem)
{
  char *key;
  int c1, c2;

  c1 = lw6sys_checksum ((unsigned char *) look, sizeof (lw6gui_look_t));
  c2 =
    lw6sys_checksum ((unsigned char *) menuitem, sizeof (lw6gui_menuitem_t));
  key = lw6sys_new_sprintf ("%d %d", c1, c2);

  return key;
}

/*
 * Store a button bitmap in the menucache. Returns != 0 if successfull.
 */
int
mod_gl_utils_store_button_in_menucache (mod_gl_utils_context_t * context,
					lw6gui_look_t * look,
					lw6gui_menuitem_t * menuitem,
					mod_gl_utils_bitmap_t * bitmap)
{
  mod_gl_utils_menucache_array_t *menucache_array;
  int ret = 0;
  int i;
  char *key;
  char *old_key;
  mod_gl_utils_bitmap_t *old_bitmap;

  menucache_array = &(context->menucache_array);

  i = menucache_array->last_stored_item =
    (menucache_array->last_stored_item +
     1) % MOD_GL_UTILS_MENUCACHE_ARRAY_SIZE;

  if ((old_key = menucache_array->item_array[i].key) != NULL)
    {
      LW6SYS_FREE (old_key);
      menucache_array->item_array[i].key = NULL;
    }
  if ((old_bitmap = menucache_array->item_array[i].bitmap) != NULL)
    {
      mod_gl_utils_bitmap_free (context, old_bitmap);
      menucache_array->item_array[i].bitmap = NULL;
    }

  key = create_button_key (look, menuitem);
  if (key != NULL)
    {
      ret = 1;
      menucache_array->item_array[i].key = key;
      menucache_array->item_array[i].bitmap = bitmap;
    }

  return ret;
}

/*
 * Gets a button bitmap from the menucache. Returns non NULL if successfull.
 */
mod_gl_utils_bitmap_t *
mod_gl_utils_get_button_from_menucache (mod_gl_utils_context_t * context,
					lw6gui_look_t * look,
					lw6gui_menuitem_t * menuitem)
{
  mod_gl_utils_menucache_array_t *menucache_array;
  mod_gl_utils_bitmap_t *bitmap = NULL;
  int i;
  char *key;

  menucache_array = &(context->menucache_array);

  key = create_button_key (look, menuitem);

  if (key != NULL)
    {
      for (i = 0; i < MOD_GL_UTILS_MENUCACHE_ARRAY_SIZE; ++i)
	{
	  if (menucache_array->item_array[i].key != NULL &&
	      menucache_array->item_array[i].bitmap != NULL)
	    {
	      if (strcmp (key, menucache_array->item_array[i].key) == 0)
		{
		  bitmap = menucache_array->item_array[i].bitmap;
		  break;
		}
	    }
	}
      LW6SYS_FREE (key);
    }

  return bitmap;
}

/*
 * Clear the cache (usefull when quiting the program).
 */
void
mod_gl_utils_clear_menucache (mod_gl_utils_context_t * context)
{
  mod_gl_utils_menucache_array_t *menucache_array;
  int i;
  char *key;
  mod_gl_utils_bitmap_t *bitmap;

  menucache_array = &(context->menucache_array);

  for (i = 0; i < MOD_GL_UTILS_MENUCACHE_ARRAY_SIZE; ++i)
    {
      if ((key = menucache_array->item_array[i].key) != NULL)
	{
	  LW6SYS_FREE (key);
	}
      if ((bitmap = menucache_array->item_array[i].bitmap) != NULL)
	{
	  mod_gl_utils_bitmap_free (context, bitmap);
	}
    }
  if (menucache_array->tooltip_str)
    {
      LW6SYS_FREE (menucache_array->tooltip_str);
    }
  if (menucache_array->tooltip_bitmap)
    {
      mod_gl_utils_bitmap_free (context, menucache_array->tooltip_bitmap);
    }
  if (menucache_array->help_str)
    {
      LW6SYS_FREE (menucache_array->help_str);
    }
  if (menucache_array->help_bitmap)
    {
      mod_gl_utils_bitmap_free (context, menucache_array->help_bitmap);
    }

  memset (menucache_array, 0, sizeof (mod_gl_utils_menucache_array_t));
}
