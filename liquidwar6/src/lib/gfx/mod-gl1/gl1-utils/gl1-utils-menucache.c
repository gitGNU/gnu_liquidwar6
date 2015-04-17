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
#include "gl1-utils.h"

/*
 * Creates a unique string key for a given menu.
 * Pointer must be freed.
 */
static char *
_create_button_key (lw6sys_context_t * sys_context, const lw6gui_look_t * look, const lw6gui_menuitem_t * menuitem)
{
  char *key;
  int c1 = 0;
  int c2 = 0;

  if (look)
    {
      c1 = lw6sys_checksum (sys_context, (unsigned char *) look, sizeof (lw6gui_look_t));
    }
  if (menuitem)
    {
      c2 = lw6sys_checksum (sys_context, (unsigned char *) menuitem, sizeof (lw6gui_menuitem_t));
    }
  key = lw6sys_new_sprintf (sys_context, "%d %d", c1, c2);

  return key;
}

/*
 * Store a button bitmap in the menucache. Returns != 0 if successfull.
 */
int
mod_gl1_utils_store_button_in_menucache (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context,
					 const lw6gui_look_t * look, const lw6gui_menuitem_t * menuitem, mod_gl1_utils_bitmap_t * bitmap)
{
  mod_gl1_utils_menucache_array_t *menucache_array;
  int ret = 0;
  int i;
  char *key;
  char *old_key;
  mod_gl1_utils_bitmap_t *old_bitmap;

  menucache_array = &(context->menucache_array);

  i = menucache_array->last_stored_item = (menucache_array->last_stored_item + 1) % MOD_GL1_UTILS_MENUCACHE_ARRAY_SIZE;

  if ((old_key = menucache_array->item_array[i].key) != NULL)
    {
      LW6SYS_FREE (sys_context, old_key);
      menucache_array->item_array[i].key = NULL;
    }
  if ((old_bitmap = menucache_array->item_array[i].bitmap) != NULL)
    {
      mod_gl1_utils_bitmap_free (sys_context, context, old_bitmap);
      menucache_array->item_array[i].bitmap = NULL;
    }

  key = _create_button_key (sys_context, look, menuitem);
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
mod_gl1_utils_bitmap_t *
mod_gl1_utils_get_button_from_menucache (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context, const lw6gui_look_t * look,
					 const lw6gui_menuitem_t * menuitem)
{
  mod_gl1_utils_menucache_array_t *menucache_array;
  mod_gl1_utils_bitmap_t *bitmap = NULL;
  int i;
  char *key;

  menucache_array = &(context->menucache_array);

  key = _create_button_key (sys_context, look, menuitem);

  if (key != NULL)
    {
      for (i = 0; i < MOD_GL1_UTILS_MENUCACHE_ARRAY_SIZE; ++i)
	{
	  if (menucache_array->item_array[i].key != NULL && menucache_array->item_array[i].bitmap != NULL)
	    {
	      if (strcmp (key, menucache_array->item_array[i].key) == 0)
		{
		  bitmap = menucache_array->item_array[i].bitmap;
		  break;
		}
	    }
	}
      LW6SYS_FREE (sys_context, key);
    }

  return bitmap;
}

/*
 * Clear the cache (usefull when quiting the program).
 */
void
mod_gl1_utils_clear_menucache (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context)
{
  mod_gl1_utils_menucache_array_t *menucache_array;
  int i;
  char *key;
  mod_gl1_utils_bitmap_t *bitmap;

  menucache_array = &(context->menucache_array);

  for (i = 0; i < MOD_GL1_UTILS_MENUCACHE_ARRAY_SIZE; ++i)
    {
      if ((key = menucache_array->item_array[i].key) != NULL)
	{
	  LW6SYS_FREE (sys_context, key);
	}
      if ((bitmap = menucache_array->item_array[i].bitmap) != NULL)
	{
	  mod_gl1_utils_bitmap_free (sys_context, context, bitmap);
	}
    }
  if (menucache_array->tooltip_str)
    {
      LW6SYS_FREE (sys_context, menucache_array->tooltip_str);
    }
  if (menucache_array->tooltip_bitmap)
    {
      mod_gl1_utils_bitmap_free (sys_context, context, menucache_array->tooltip_bitmap);
    }
  if (menucache_array->help_str)
    {
      LW6SYS_FREE (sys_context, menucache_array->help_str);
    }
  if (menucache_array->help_bitmap)
    {
      mod_gl1_utils_bitmap_free (sys_context, context, menucache_array->help_bitmap);
    }
  if (menucache_array->breadcrumbs_str)
    {
      LW6SYS_FREE (sys_context, menucache_array->breadcrumbs_str);
    }
  if (menucache_array->breadcrumbs_bitmap)
    {
      mod_gl1_utils_bitmap_free (sys_context, context, menucache_array->breadcrumbs_bitmap);
    }
  if (menucache_array->popup_str)
    {
      LW6SYS_FREE (sys_context, menucache_array->popup_str);
    }
  if (menucache_array->popup_bitmap)
    {
      mod_gl1_utils_bitmap_free (sys_context, context, menucache_array->popup_bitmap);
    }

  memset (menucache_array, 0, sizeof (mod_gl1_utils_menucache_array_t));
}
