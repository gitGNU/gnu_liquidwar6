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
#include <expat.h>

#include "../../gfx.h"
#include "gl1-utils.h"

#define FONT_FILE "utils/font/FreeSansBold.ttf"

/*
 * Dummy wrapper to handle cast & errors.
 */
SDL_Surface *
mod_gl1_utils_load_image (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *filename)
{
  SDL_Surface *ret = NULL;
  char *image_file = NULL;

  image_file = lw6sys_path_concat (sys_context, utils_context->path.data_dir, filename);

  if (image_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading image \"%s\""), image_file);
      ret = IMG_Load (image_file);
      if (!ret)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("SDL_image error \"%s\" while loading image \"%s\""), IMG_GetError (), image_file);
	}
      LW6SYS_FREE (sys_context, image_file);
    }

  if (ret != NULL)
    {
      utils_context->surface_counter.new_counter++;
    }
  else
    {
      /*
       * Carefull not to use buf in error message.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("unable to load image \"%s\""), filename);
    }

  return ret;
}

/*
 * Dummy wrapper to handle cast & errors.
 */
void
mod_gl1_utils_unload_image (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * image)
{
  mod_gl1_utils_delete_surface (sys_context, utils_context, image);
}

/*
 * Dummy wrapper to handle cast & errors.
 */
static TTF_Font *
_load_font (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *filename, int size)
{
  TTF_Font *ret = NULL;
  char *font_file;

  font_file = lw6sys_path_concat (sys_context, utils_context->path.data_dir, filename);
  if (font_file)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading font \"%s\", size %d"), font_file, size);
      ret = TTF_OpenFont (font_file, size);
      LW6SYS_FREE (sys_context, font_file);
    }

  if (ret == NULL)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("unable to load font \"%s\""), filename);
    }

  return ret;
}

/*
 * Dummy wrapper to handle cast & errors.
 */
static void
_unload_font (lw6sys_context_t * sys_context, TTF_Font * font)
{
  if (font != NULL)
    {
      TTF_CloseFont (font);
    }
}

/*
 * Loads fonts
 */
int
mod_gl1_utils_load_fonts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading fonts"));

  ret =
    ((utils_context->font_data.menu =
      _load_font (sys_context, utils_context, FONT_FILE,
		  utils_context->const_data.menu_font_size)) != NULL)
    &&
    ((utils_context->font_data.hud =
      _load_font (sys_context, utils_context, FONT_FILE,
		  utils_context->const_data.hud_font_size)) != NULL)
    &&
    ((utils_context->font_data.cursor =
      _load_font (sys_context, utils_context, FONT_FILE,
		  utils_context->const_data.cursor_font_size)) != NULL)
    && ((utils_context->font_data.system = _load_font (sys_context, utils_context, FONT_FILE, utils_context->const_data.system_font_size)) != NULL);

  if (!ret)
    {
      /*
       * If we can't load fonts, we simply exit the game...
       */
      exit (1);
    }

  return ret;
}

/*
 * Unload fonts, free memory
 */
void
mod_gl1_utils_unload_fonts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  _unload_font (sys_context, utils_context->font_data.system);
  _unload_font (sys_context, utils_context->font_data.cursor);
  _unload_font (sys_context, utils_context->font_data.hud);
  _unload_font (sys_context, utils_context->font_data.menu);

  memset (&utils_context->font_data, 0, sizeof (mod_gl1_utils_font_data_t));
}

/*
 * Putting all the load/unload functions together
 */
int
mod_gl1_utils_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  int ret = 0;
  lw6gui_look_t *look = NULL;

  ret = ((utils_context->texture_data.to_delete = lw6sys_list_new (sys_context, NULL)) != NULL) && mod_gl1_utils_load_fonts (sys_context, utils_context);

  look = lw6gui_look_new (sys_context, NULL);
  if (look)
    {
      ret = mod_gl1_utils_cache_update (sys_context, utils_context, look) && ret;
      lw6gui_look_free (sys_context, look);
    }

  return ret;
}

void
mod_gl1_utils_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  mod_gl1_utils_clear_menucache (sys_context, utils_context);
  mod_gl1_utils_unload_fonts (sys_context, utils_context);
  mod_gl1_utils_texture_1x1_clear (sys_context, utils_context);

  mod_gl1_utils_delete_scheduled_textures (sys_context, utils_context);
  if (utils_context->texture_data.to_delete)
    {
      LW6SYS_FREE (sys_context, utils_context->texture_data.to_delete);
    }
  memset (&utils_context->texture_data, 0, sizeof (mod_gl1_utils_texture_data_t));
}
