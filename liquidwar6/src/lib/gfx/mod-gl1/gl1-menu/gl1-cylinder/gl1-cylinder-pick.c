/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#include "../../mod-gl1.h"
#include "gl1-cylinder-internal.h"

#define FEEDBACK_BUFFER_SIZE 4096

#define _PASS_THROUGH_DEFAULT -1
#define _PASS_THROUGH_PREV -2
#define _PASS_THROUGH_NEXT -3
#define _PASS_THROUGH_ESC -4
#define _PASS_THROUGH_SELECTED -5

static void
draw_button_corners (mod_gl1_utils_context_t * utils_context,
		     _mod_gl1_menu_cylinder_context_t * cylinder_context, int i, int n, float relative_text_width, int pass_through)
{

  if (i >= 0)
    {
      _mod_gl1_menu_cylinder_draw_cylinder_corners (utils_context, cylinder_context, i, n, relative_text_width, pass_through);
    }
  else
    {
      float screen_ratio;
      float draw_esc_offset;
      float draw_esc_radius;
      float draw_esc_cyl_height;
      float draw_esc_cyl_height_offset;
      float draw_esc_rotate;

      screen_ratio = ((float) utils_context->sdl_context.video_mode.width) / ((float) utils_context->sdl_context.video_mode.height);

      draw_esc_offset = cylinder_context->const_data.esc_offset;
      draw_esc_radius = cylinder_context->const_data.esc_radius;
      draw_esc_cyl_height = relative_text_width * cylinder_context->const_data.esc_cyl_height * screen_ratio;
      draw_esc_cyl_height_offset = cylinder_context->const_data.esc_cyl_height_offset * screen_ratio;
      draw_esc_rotate = cylinder_context->const_data.esc_rotate;

      _mod_gl1_menu_cylinder_draw_fixed_cylinder_corners (utils_context,
							  cylinder_context,
							  draw_esc_offset,
							  draw_esc_radius,
							  draw_esc_cyl_height, draw_esc_cyl_height_offset, draw_esc_rotate, relative_text_width, pass_through);
    }
}

static void
draw_spheres_corners (mod_gl1_utils_context_t * utils_context,
		      _mod_gl1_menu_cylinder_context_t * cylinder_context, int i, int n, int nb_spheres, int pass_through)
{
  int sphere_i;

  for (sphere_i = 0; sphere_i < nb_spheres; ++sphere_i)
    {
      _mod_gl1_menu_cylinder_draw_sphere_corners (utils_context, cylinder_context, i, n, sphere_i, nb_spheres, pass_through);
    }
}

void
_mod_gl1_menu_cylinder_pick_item (mod_gl1_utils_context_t * utils_context,
				  _mod_gl1_menu_cylinder_context_t *
				  cylinder_context,
				  const lw6gui_look_t * look, int *position, int *scroll, int *esc, lw6gui_menu_t * menu, int screen_x, int screen_y)
{
  //  http://users.polytech.unice.fr/~buffa/cours/synthese_image/DOCS/Tutoriaux/glGameDeveloppers/view.cgi-V=tutorial_glfeedback&S=3.htm

  GLfloat feedback_buffer[FEEDBACK_BUFFER_SIZE];
  GLfloat *ptr;
  int i, n;
  float x, y, x_min, x_max, y_min, y_max;
  lw6gui_menuitem_t *menuitem;
  mod_gl1_utils_bitmap_t *button_bitmap = NULL;
  float relative_text_width;
  int ret = _PASS_THROUGH_DEFAULT;

  lw6gui_menu_update_display_range (sys_context, menu, cylinder_context->const_data.max_displayed_items);

  memset (feedback_buffer, 0, sizeof (GLfloat) * FEEDBACK_BUFFER_SIZE);

  glFeedbackBuffer (FEEDBACK_BUFFER_SIZE, GL_2D, (float *) feedback_buffer);

  mod_gl1_utils_set_render_mode_3d_feedback (sys_context, utils_context);
  glRenderMode (GL_FEEDBACK);	// enter feedback mode

  n = menu->nb_items_displayed + 2;

  if (menu->first_item_displayed > 0)
    {
      draw_spheres_corners (utils_context, cylinder_context, 0, n, cylinder_context->const_data.nb_spheres, _PASS_THROUGH_PREV);
    }
  if (menu->first_item_displayed + menu->nb_items_displayed < menu->nb_items)
    {
      draw_spheres_corners (utils_context, cylinder_context, n - 1, n, cylinder_context->const_data.nb_spheres, _PASS_THROUGH_NEXT);
    }

  for (i = 0; i < menu->nb_items_displayed; ++i)
    {
      menuitem = menu->items[i + menu->first_item_displayed];
      button_bitmap = mod_gl1_utils_get_button_from_menucache (utils_context, look, menuitem);
      if (button_bitmap)
	{
	  //
	  // If button_bitmap doesn't exist, well, we won't die,
	  // we just suppose the button is very large. It practise
	  // it shouldn't happen for we've drawn them just before.
	  //
	  relative_text_width = ((float) button_bitmap->surface->w) / ((float) MOD_GL1_UTILS_MENU_TEXTURE_W);
	}
      else
	{
	  relative_text_width = 1.0f;
	}
      draw_button_corners (utils_context, cylinder_context, i + 1, n, relative_text_width, i + menu->first_item_displayed);
    }

  if (menu->esc_item->enabled)
    {
      button_bitmap = mod_gl1_utils_get_button_from_menucache (utils_context, look, menu->esc_item);
      if (button_bitmap)
	{
	  //
	  // If button_bitmap doesn't exist, well, we won't die,
	  // we just suppose the button is very large. It practise
	  // it shouldn't happen for we've drawn them just before.
	  //
	  relative_text_width = ((float) button_bitmap->surface->w) / ((float) MOD_GL1_UTILS_MENU_TEXTURE_W);
	}
      else
	{
	  relative_text_width = 1.0f;
	}
      draw_button_corners (utils_context, cylinder_context, -1, n, relative_text_width, _PASS_THROUGH_ESC);
    }

  glRenderMode (GL_RENDER);	// back to normal mode

  ptr = feedback_buffer;

  while ((*ptr) == GL_PASS_THROUGH_TOKEN && (ptr < feedback_buffer + FEEDBACK_BUFFER_SIZE))
    {
      ptr++;
      if (ptr < feedback_buffer + FEEDBACK_BUFFER_SIZE)
	{
	  x_min = utils_context->sdl_context.video_mode.width + 1;
	  x_max = -1;
	  y_min = utils_context->sdl_context.video_mode.height + 1;
	  y_max = -1;
	  i = (int) (*ptr++);
	  while ((*ptr) == GL_POINT_TOKEN && ptr < feedback_buffer + FEEDBACK_BUFFER_SIZE - 3)
	    {
	      ptr++;
	      x = *(ptr++);
	      y = (utils_context->sdl_context.video_mode.height - *(ptr++));
	      if (x < x_min)
		{
		  x_min = x;
		}
	      if (x > x_max)
		{
		  x_max = x;
		}
	      if (y < y_min)
		{
		  y_min = y;
		}
	      if (y > y_max)
		{
		  y_max = y;
		}
	    }
	  if (x_min <= screen_x && screen_x <= x_max && y_min <= screen_y && screen_y <= y_max)
	    {
	      ret = i;
	    }
	}
    }

  switch (ret)
    {
    case _PASS_THROUGH_PREV:
      (*position) = -1;
      (*scroll) = -1;
      (*esc) = 0;
      break;
    case _PASS_THROUGH_NEXT:
      (*position) = -1;
      (*scroll) = +1;
      (*esc) = 0;
      break;
    case _PASS_THROUGH_ESC:
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

void
_mod_gl1_menu_cylinder_get_cylinder_right_point (mod_gl1_utils_context_t *
						 utils_context,
						 _mod_gl1_menu_cylinder_context_t
						 * cylinder_context, int i, int n, float relative_text_width, float *right_point_x, float *right_point_y)
{
  GLfloat feedback_buffer[FEEDBACK_BUFFER_SIZE];
  GLfloat *ptr;
  float x, y, x_min = 0, x_max = 0, y_min = 0, y_max = 0;

  memset (feedback_buffer, 0, sizeof (GLfloat) * FEEDBACK_BUFFER_SIZE);

  glFeedbackBuffer (FEEDBACK_BUFFER_SIZE, GL_2D, (float *) feedback_buffer);

  mod_gl1_utils_set_render_mode_3d_feedback (sys_context, utils_context);
  glRenderMode (GL_FEEDBACK);	// enter feedback mode

  _mod_gl1_menu_cylinder_draw_cylinder_corners (utils_context, cylinder_context, i, n, relative_text_width, _PASS_THROUGH_SELECTED);

  glRenderMode (GL_RENDER);	// back to normal mode

  ptr = feedback_buffer;

  while ((*ptr) == GL_PASS_THROUGH_TOKEN && (ptr < feedback_buffer + FEEDBACK_BUFFER_SIZE))
    {
      ptr++;
      if (ptr < feedback_buffer + FEEDBACK_BUFFER_SIZE)
	{
	  x_min = utils_context->sdl_context.video_mode.width + 1;
	  x_max = -1;
	  y_min = utils_context->sdl_context.video_mode.height + 1;
	  y_max = -1;
	  ptr++;		// skipping passthrough value (_PASS_THROUGH_SELECTED)
	  while ((*ptr) == GL_POINT_TOKEN && ptr < feedback_buffer + FEEDBACK_BUFFER_SIZE - 3)
	    {
	      ptr++;
	      x = *(ptr++);
	      y = (utils_context->sdl_context.video_mode.height - *(ptr++));
	      if (x < x_min)
		{
		  x_min = x;
		}
	      if (x > x_max)
		{
		  x_max = x;
		}
	      if (y < y_min)
		{
		  y_min = y;
		}
	      if (y > y_max)
		{
		  y_max = y;
		}
	    }
	}
    }

  (*right_point_x) = x_max;
  (*right_point_y) = (y_min + y_max) / 2;
}
