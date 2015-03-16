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
#include "gl1-bubbles-internal.h"

static void
_display_background (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context,
		     const lw6gui_look_t * look)
{
  mod_gl1_utils_set_render_mode_2d (sys_context, utils_context);

  mod_gl1_utils_bitmap_colorize (sys_context, utils_context, bubbles_context->bitmap_data.background, look->style.colorize,
				 &(look->style.color_set.background_color_root));
  mod_gl1_utils_bitmap_bind (sys_context, utils_context, bubbles_context->bitmap_data.background);

  glMatrixMode (GL_TEXTURE);
  glPushMatrix ();
  glLoadIdentity ();
  glTranslatef (0.0f,
		bubbles_context->const_data.yspeed *
		look->style.animation_speed * _lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) / 1000.0f, 0.0f);

  glBegin (GL_QUADS);
  glTexCoord2d (0.0f, 0.0f);
  glVertex3f (0.0f, 0.0f, 0.0f);	// top left
  glTexCoord2d (1.0f, 0.0f);
  glVertex3f (utils_context->sdl_context.video_mode.width, 0.0f, 0.0f);	// top right
  glTexCoord2d (1.0f, 1.0f);
  glVertex3f (utils_context->sdl_context.video_mode.width, utils_context->sdl_context.video_mode.height, 0.0f);	// bottom right
  glTexCoord2d (0.0f, 1.0f);
  glVertex3f (0.0f, utils_context->sdl_context.video_mode.height, 0.0f);	// bottom left
  glEnd ();

  glMatrixMode (GL_TEXTURE);
  glPopMatrix ();
}

static void
_display_bubble (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		 _mod_gl1_background_bubbles_context_t * bubbles_context, _mod_gl1_background_bubbles_bubble_t * bubble, const lw6gui_look_t * look)
{
  int dt;
  float y;
  float x_px, y_px, size2_px;

  if (!bubble->active)
    {
      bubble->active = 1;
      bubble->t0 = _lw6gfx_sdl_timer_get_uptime (sys_context, &(utils_context->sdl_context));
      bubble->size = lw6sys_random_float (sys_context, bubbles_context->const_data.bubble_size_min, bubbles_context->const_data.bubble_size_max);
      bubble->x = lw6sys_random_float (sys_context, 0.0f, 1.0f);
    }

  dt = _lw6gfx_sdl_timer_get_uptime (sys_context, &(utils_context->sdl_context)) - bubble->t0;
  x_px = bubble->x * utils_context->sdl_context.video_mode.width;
  y = 1.0f + (bubble->size / 2.0f) - (bubble->size * bubbles_context->const_data.bubble_yspeed * look->style.animation_speed * dt / 1000.0f);
  y_px = y * utils_context->sdl_context.video_mode.height;
  size2_px = (utils_context->sdl_context.video_mode.height * bubble->size) / 2.0f;

  glMatrixMode (GL_TEXTURE);
  glPushMatrix ();
  glLoadIdentity ();

  glTexCoord2d (0.0f, 0.0f);
  glVertex3f (x_px - size2_px, y_px - size2_px, 0.0f);	// top left
  glTexCoord2d (1.0f, 0.0f);
  glVertex3f (x_px + size2_px, y_px - size2_px, 0.0f);	// top right
  glTexCoord2d (1.0f, 1.0f);
  glVertex3f (x_px + size2_px, y_px + size2_px, 0.0f);	// bottom right
  glTexCoord2d (0.0f, 1.0f);
  glVertex3f (x_px - size2_px, y_px + size2_px, 0.0f);	// bottom left

  glMatrixMode (GL_TEXTURE);
  glPopMatrix ();

  if (y < (-bubble->size / 2.0f))
    {
      bubble->active = 0;
    }
}

static void
_display_bubbles (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context,
		  const lw6gui_look_t * look)
{
  int i;
  int nb_bubbles;

  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

  mod_gl1_utils_bitmap_colorize (sys_context, utils_context, bubbles_context->bitmap_data.bubble, look->style.colorize,
				 &(look->style.color_set.background_color_stuff));
  mod_gl1_utils_bitmap_bind (sys_context, utils_context, bubbles_context->bitmap_data.bubble);

  nb_bubbles = bubbles_context->const_data.nb_bubbles * look->style.animation_density;

  nb_bubbles = lw6sys_imin (nb_bubbles, _MOD_GL1_BACKGROUND_BUBBLES_MAX_NB_BUBBLES);

  glBegin (GL_QUADS);
  for (i = 0; i < nb_bubbles; ++i)
    {
      _display_bubble (sys_context, utils_context, bubbles_context, &(bubbles_context->state.bubbles[i]), look);
    }
  glEnd ();
}

void
_mod_gl1_background_bubbles_display_background (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context, const lw6gui_look_t * look)
{
  _display_background (sys_context, utils_context, bubbles_context, look);
  _display_bubbles (sys_context, utils_context, bubbles_context, look);
}

void
mod_gl1_background_bubbles_display_background (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *bubbles_context,
					       const lw6gui_look_t * look)
{
  _mod_gl1_background_bubbles_display_background (sys_context, utils_context, bubbles_context, look);
}
