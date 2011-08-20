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

#include "../../mod-gl.h"
#include "gl-cylinder-internal.h"

#define SELECT_BUFFER_SIZE 4096

#define NAME_DEFAULT -1
#define NAME_PREV -2
#define NAME_NEXT -3
#define NAME_ESC -4

static void
draw_button_shape (mod_gl_utils_context_t * utils_context,
		   _mod_gl_menu_cylinder_context_t * cylinder_context,
		   int i, int n, float relative_text_width, int name)
{
  glLoadName (name);
  if (i >= 0)
    {
      _mod_gl_menu_cylinder_draw_cylinder (utils_context, cylinder_context,
					   GL_SELECT, i, n,
					   relative_text_width);
    }
  else
    {
      _mod_gl_menu_cylinder_draw_fixed_cylinder (utils_context,
						 cylinder_context, GL_SELECT,
						 cylinder_context->const_data.
						 esc_offset,
						 cylinder_context->const_data.
						 esc_radius,
						 cylinder_context->const_data.
						 esc_cyl_height *
						 ((float) utils_context->
						  video_mode.width) /
						 ((float) utils_context->
						  video_mode.height),
						 cylinder_context->const_data.
						 esc_cyl_height_offset *
						 ((float) utils_context->
						  video_mode.width) /
						 ((float) utils_context->
						  video_mode.height),
						 cylinder_context->const_data.
						 esc_rotate);
    }
}

static void
draw_spheres_shape (mod_gl_utils_context_t * utils_context,
		    _mod_gl_menu_cylinder_context_t * cylinder_context, int i,
		    int n, int nb_spheres, int name)
{
  int sphere_i;

  glLoadName (name);

  for (sphere_i = 0; sphere_i < nb_spheres; ++sphere_i)
    {
      _mod_gl_menu_cylinder_draw_sphere (utils_context, cylinder_context,
					 GL_SELECT, i, n, sphere_i,
					 nb_spheres);
    }
}

static int
process_select_hits (GLint select_hits,
		     GLuint select_buffer[SELECT_BUFFER_SIZE], int x, int y)
{
  int ret = NAME_DEFAULT;
  unsigned int i, j;
  GLuint names, *ptr;

  /*
   * We decide to walk all the array and keep the last value.
   * Note that we could look the first one and code could look simpler,
   * but for debugging purposes, it's easier to keep a track of
   * how to get all the values, with the lool & all fields.
   */
  ptr = (GLuint *) select_buffer;
  for (i = 0; i < select_hits; i++)
    {				// for each hit
      names = *ptr;
      ptr++;			// names
      ptr++;			// z1
      ptr++;			// z2
      for (j = 0; j < names; j++)
	{			// for each name
	  ret = (*ptr);
	  ptr++;		// name
	}
    }

  return ret;
}

void
_mod_gl_menu_cylinder_pick_item (mod_gl_utils_context_t * utils_context,
				 _mod_gl_menu_cylinder_context_t *
				 cylinder_context, lw6gui_look_t * look,
				 int *position, int *scroll, int *esc,
				 lw6gui_menu_t * menu, int screen_x,
				 int screen_y)
{
  int ret = NAME_DEFAULT;
  int i, n;
  lw6gui_menuitem_t *menuitem;
  GLuint select_buffer[SELECT_BUFFER_SIZE];
  GLint select_hits;
  mod_gl_utils_bitmap_t *button_bitmap = NULL;
  float relative_text_width = 1.0f;

  lw6gui_menu_update_display_range (menu,
				    cylinder_context->
				    const_data.max_displayed_items);

  glSelectBuffer (SELECT_BUFFER_SIZE, select_buffer);
  glRenderMode (GL_SELECT);
  glInitNames ();
  glPushName (NAME_DEFAULT);

  mod_gl_utils_set_render_mode_3d_pick (utils_context, screen_x, screen_y);

  n = menu->nb_items_displayed + 2;
  if (menu->first_item_displayed > 0)
    {
      draw_spheres_shape (utils_context, cylinder_context, 0, n,
			  cylinder_context->const_data.nb_spheres, NAME_PREV);
    }
  if (menu->first_item_displayed + menu->nb_items_displayed < menu->nb_items)
    {
      draw_spheres_shape (utils_context, cylinder_context, n - 1, n,
			  cylinder_context->const_data.nb_spheres, NAME_NEXT);
    }
  for (i = 0; i < menu->nb_items_displayed; ++i)
    {
      menuitem = menu->items[i + menu->first_item_displayed];
      button_bitmap =
	mod_gl_utils_get_button_from_menucache (utils_context, look,
						menuitem);
      if (button_bitmap)
	{
	  /*
	   * If button_bitmap doesn't exist, well, we won't die,
	   * we just suppose the button is very large. It practise
	   * it shouldn't happen for we've drawn them just before.
	   */
	  relative_text_width =
	    ((float) button_bitmap->surface->w) /
	    ((float) MOD_GL_UTILS_MENU_TEXTURE_W);
	}
      else
	{
	  relative_text_width = 1.0f;
	}
      draw_button_shape (utils_context,
			 cylinder_context,
			 i + 1, n, relative_text_width,
			 i + menu->first_item_displayed);
    }
  if (menu->esc_item->enabled)
    {
      button_bitmap =
	mod_gl_utils_get_button_from_menucache (utils_context, look,
						menu->esc_item);
      if (button_bitmap)
	{
	  /*
	   * If button_bitmap doesn't exist, well, we won't die,
	   * we just suppose the button is very large. It practise
	   * it shouldn't happen for we've drawn them just before.
	   */
	  relative_text_width =
	    ((float) button_bitmap->surface->w) /
	    ((float) MOD_GL_UTILS_MENU_TEXTURE_W);
	}
      else
	{
	  relative_text_width = 1.0f;
	}
      draw_button_shape (utils_context, cylinder_context, -1, n,
			 relative_text_width, NAME_ESC);
    }

  select_hits = glRenderMode (GL_RENDER);
  ret = process_select_hits (select_hits, select_buffer, screen_x, screen_y);

  switch (ret)
    {
    case NAME_PREV:
      (*position) = -1;
      (*scroll) = -1;
      (*esc) = 0;
      break;
    case NAME_NEXT:
      (*position) = -1;
      (*scroll) = +1;
      (*esc) = 0;
      break;
    case NAME_ESC:
      (*position) = -1;
      (*scroll) = 0;
      (*esc) = 1;
      break;
    default:
      (*position) = ret;
      (*scroll) = 0;
      (*esc) = 0;
    }
}
