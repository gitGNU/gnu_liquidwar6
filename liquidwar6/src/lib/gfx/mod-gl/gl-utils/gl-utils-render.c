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

#include "../../gfx.h"
#include "gl-utils.h"

#define PICK_WIDTH 3.0f
#define PICK_HEIGHT 3.0f

static void
basic_cleanup ()
{
  glEnable (GL_TEXTURE_2D);
  glColor3f (1.0f, 1.0f, 1.0f);
}

/*
 * Prepares OpenGL buffer for a new draw.
 */
void
mod_gl_utils_prepare_buffer (mod_gl_utils_context_t * utils_context,
			     lw6gui_look_t * look)
{
  lw6sys_color_f_t bg_color;

  mod_gl_utils_sync_viewport (utils_context);
  lw6sys_color_8_to_f (&bg_color,
		       look->style.color_set.background_color_root.bg);
  glClearColor (bg_color.r, bg_color.g, bg_color.b, bg_color.a);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mod_gl_utils_update_team_color_map (&(utils_context->team_color_map),
				      &(look->style));
  if (utils_context->render_param.gfx_quality != look->gfx_quality)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("set rendering gfx_quality to %d"),
		  look->gfx_quality);
      utils_context->render_param.gfx_quality = look->gfx_quality;
      mod_gl_utils_timer_set_bitmap_refresh (utils_context);
    }
}

void
mod_gl_prepare_buffer (void *utils_context, lw6gui_look_t * look)
{
  mod_gl_utils_prepare_buffer ((mod_gl_utils_context_t *) utils_context,
			       look);
}

/*
 * Liquid War 6 uses a double buffer, this function swaps them.
 */
void
mod_gl_utils_swap_buffers (mod_gl_utils_context_t * utils_context)
{
  /*
   * glFinish suspected to be a performance killer, and anyways
   * SwapBuffers should call it.
   */
  //glFinish ();
  SDL_GL_SwapBuffers ();
  mod_gl_utils_delete_scheduled_textures (utils_context);
}

void
mod_gl_swap_buffers (void *utils_context)
{
  mod_gl_utils_swap_buffers ((mod_gl_utils_context_t *) utils_context);
}

void
mod_gl_utils_set_render_mode_2d (mod_gl_utils_context_t * utils_context)
{
  basic_cleanup ();
  if (utils_context->render_param.mode != MOD_GL_UTILS_RENDER_2D)
    {
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();

      gluOrtho2D (0.0f,		// left
		  (GLfloat) utils_context->video_mode.width,	// right
		  (GLfloat) utils_context->video_mode.height,	// bottom
		  0.0f);	// top

      glDisable (GL_DEPTH_TEST);
      glDisable (GL_DEPTH);
      glDisable (GL_BLEND);
      glDisable (GL_LIGHTING);

      utils_context->render_param.mode = MOD_GL_UTILS_RENDER_2D;
    }
}

void
mod_gl_utils_set_render_mode_2d_blend (mod_gl_utils_context_t * utils_context)
{
  basic_cleanup ();
  if (utils_context->render_param.mode != MOD_GL_UTILS_RENDER_2D_BLEND)
    {
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();

      gluOrtho2D (0.0f,		// left
		  (GLfloat) utils_context->video_mode.width,	// right
		  (GLfloat) utils_context->video_mode.height,	// bottom
		  0.0f);	// top

      glDisable (GL_DEPTH_TEST);
      glDisable (GL_DEPTH);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable (GL_BLEND);
      glDisable (GL_LIGHTING);

      utils_context->render_param.mode = MOD_GL_UTILS_RENDER_2D_BLEND;
    }
}

void
mod_gl_utils_set_render_mode_3d_map (mod_gl_utils_context_t * utils_context)
{
  basic_cleanup ();
  if (utils_context->render_param.mode != MOD_GL_UTILS_RENDER_3D_MAP)
    {
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      gluPerspective (utils_context->const_data.persp_fovy,
		      ((float) utils_context->video_mode.width) /
		      ((float) utils_context->video_mode.height),
		      utils_context->const_data.persp_znear,
		      utils_context->const_data.persp_zfar);
      glEnable (GL_DEPTH_TEST);
      glEnable (GL_DEPTH);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable (GL_BLEND);
      mod_gl_utils_main_light (utils_context);

      utils_context->render_param.mode = MOD_GL_UTILS_RENDER_3D_MAP;
    }
}

void
mod_gl_utils_set_render_mode_3d_menu (mod_gl_utils_context_t * utils_context)
{
  basic_cleanup ();
  if (utils_context->render_param.mode != MOD_GL_UTILS_RENDER_3D_MENU)
    {
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      gluPerspective (utils_context->const_data.persp_fovy,
		      ((float) utils_context->video_mode.width) /
		      ((float) utils_context->video_mode.height),
		      utils_context->const_data.persp_znear,
		      utils_context->const_data.persp_zfar);
      glEnable (GL_DEPTH_TEST);
      glEnable (GL_DEPTH);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable (GL_BLEND);
      mod_gl_utils_main_light (utils_context);

      utils_context->render_param.mode = MOD_GL_UTILS_RENDER_3D_MENU;
    }
}

void
mod_gl_utils_set_render_mode_3d_pick (mod_gl_utils_context_t * utils_context,
				      int x, int y)
{
  GLint viewport[4];

  basic_cleanup ();
  if (utils_context->render_param.mode != MOD_GL_UTILS_RENDER_3D_PICK)
    {
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();

      glGetIntegerv (GL_VIEWPORT, viewport);
      gluPickMatrix (x, viewport[3] - y, PICK_WIDTH, PICK_HEIGHT, viewport);

      gluPerspective (utils_context->const_data.persp_fovy,
		      ((float) utils_context->video_mode.width) /
		      ((float) utils_context->video_mode.height),
		      utils_context->const_data.persp_znear,
		      utils_context->const_data.persp_zfar);
      glDisable (GL_DEPTH_TEST);
      glDisable (GL_DEPTH);
      glDisable (GL_BLEND);
      glDisable (GL_LIGHTING);

      utils_context->render_param.mode = MOD_GL_UTILS_RENDER_3D_PICK;
    }
}
