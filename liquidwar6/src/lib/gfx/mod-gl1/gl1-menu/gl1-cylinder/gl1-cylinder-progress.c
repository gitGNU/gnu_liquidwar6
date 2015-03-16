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

#include "../../mod-gl1.h"
#include "gl1-cylinder-internal.h"

/*
 * OpenGL wizardry, to prepare view parameters.
 */
static void
_prepare_view (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context)
{
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

static void
_draw_cylinder_with_color (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			   _mod_gl1_menu_cylinder_context_t * cylinder_context, const lw6gui_look_t * look, float progress)
{
  lw6sys_color_8_t done;
  lw6sys_color_8_t todo;
  SDL_Color sdl_todo;
  u_int32_t i_done;
  u_int32_t i_todo;
  SDL_Surface *surface = NULL;
  GLuint texture;
  int limit;
  float offset;
  float radius;
  float cyl_height;

  done = look->style.color_set.menu_color_default.fg;
  todo = look->style.color_set.menu_color_default.bg;
  sdl_todo = mod_gl1_utils_color_8_to_sdl (sys_context, todo);
  i_done = lw6sys_color_8_to_irgba (done);
  i_todo = lw6sys_color_8_to_irgba (todo);

  surface = mod_gl1_utils_create_surface (sys_context, utils_context, MOD_GL1_UTILS_MENU_TEXTURE_W, MOD_GL1_UTILS_MENU_TEXTURE_H);
  if (surface)
    {
      mod_gl1_utils_clear_surface_with_color (sys_context, surface, sdl_todo);
      limit = progress * (MOD_GL1_UTILS_MENU_TEXTURE_W - 1);
      limit = lw6sys_imin (limit, MOD_GL1_UTILS_MENU_TEXTURE_W - 1);
      limit = lw6sys_imax (limit, 0);
      mod_gl1_utils_draw_rectfill (sys_context, surface, 0, 0, limit, MOD_GL1_UTILS_MENU_TEXTURE_H - 1, i_done);
      mod_gl1_utils_draw_rectfill (sys_context, surface, limit, 0, MOD_GL1_UTILS_MENU_TEXTURE_W - 1, MOD_GL1_UTILS_MENU_TEXTURE_H - 1, i_todo);
      texture = mod_gl1_utils_surface2texture (sys_context, utils_context, surface, 0);
      if (texture)
	{
	  glEnable (GL_TEXTURE_2D);
	  glColor3f (1.0f, 1.0f, 1.0f);
	  glBindTexture (GL_TEXTURE_2D, texture);

	  glMatrixMode (GL_TEXTURE);
	  glPushMatrix ();
	  glLoadIdentity ();
	  glScalef (1.0f, -1.0f, 1.0f);
	  glTranslatef (0.0f, -0.25f + cylinder_context->const_data.texture_offset, 0.0f);
	  glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);

	  offset = cylinder_context->const_data.progress_offset;
	  radius = cylinder_context->const_data.progress_radius;
	  cyl_height =
	    cylinder_context->const_data.progress_cyl_height *
	    ((float) utils_context->sdl_context.video_mode.width) / ((float) utils_context->sdl_context.video_mode.height);

	  _mod_gl1_menu_cylinder_draw_fixed_cylinder (sys_context, utils_context, cylinder_context, GL_RENDER, offset, radius, cyl_height, 0.0f, 0.0f);

	  glMatrixMode (GL_TEXTURE);
	  glPopMatrix ();
	  mod_gl1_utils_schedule_delete_texture (sys_context, utils_context, texture);
	}

      mod_gl1_utils_delete_surface (sys_context, utils_context, surface);
    }
}

void
_mod_gl1_menu_cylinder_draw_progress (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				      _mod_gl1_menu_cylinder_context_t * cylinder_context, const lw6gui_look_t * look, float progress)
{
  mod_gl1_utils_set_render_mode_3d_menu (sys_context, utils_context);
  _prepare_view (sys_context, utils_context, cylinder_context);
  _draw_cylinder_with_color (sys_context, utils_context, cylinder_context, look, progress);
}
