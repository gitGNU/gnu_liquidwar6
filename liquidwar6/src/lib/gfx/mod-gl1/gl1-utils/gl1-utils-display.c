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

#include "../../gfx.h"
#include "gl1-utils.h"

void
mod_gl1_utils_display_texture_full (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				    GLuint texture, float x1, float y1, float x2, float y2, int texture_w, int texture_h)
{
  float scale_w, scale_h;

  if (texture_w > 0 && texture_h > 0)
    {
      scale_w = mod_gl1_utils_texture_scale (sys_context, texture_w);
      scale_h = mod_gl1_utils_texture_scale (sys_context, texture_h);

      glBindTexture (GL_TEXTURE_2D, texture);

      glMatrixMode (GL_TEXTURE);
      glPushMatrix ();
      glLoadIdentity ();
      glBegin (GL_QUADS);
      glTexCoord2d (0.0f, 0.0f);
      glVertex3f (x1, y1, 0.0f);	// top left
      glTexCoord2d (scale_w, 0.0f);
      glVertex3f (x2, y1, 0.0f);	// top right
      glTexCoord2d (scale_w, scale_h);
      glVertex3f (x2, y2, 0.0f);	// bottom right
      glTexCoord2d (0.0f, scale_h);
      glVertex3f (x1, y2, 0.0f);	// bottom left
      glEnd ();

      glMatrixMode (GL_TEXTURE);
      glPopMatrix ();
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid texture size %dx%d"), texture_w, texture_h);
    }
}

void
mod_gl1_utils_display_quad_begin (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  glMatrixMode (GL_TEXTURE);
  glPushMatrix ();
  glLoadIdentity ();
  glBegin (GL_QUADS);
}

void
mod_gl1_utils_display_quad_end (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  glEnd ();
  glMatrixMode (GL_TEXTURE);
  glPopMatrix ();
}

void
mod_gl1_utils_display_quad_do (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			       GLuint texture, float x1, float y1, float x2, float y2, float texture_x1, float texture_y1, float texture_x2, float texture_y2)
{
  glBindTexture (GL_TEXTURE_2D, texture);

  glTexCoord2d (texture_x1, texture_y1);
  glVertex3f (x1, y1, 0.0f);	// top left
  glTexCoord2d (texture_x2, texture_y1);
  glVertex3f (x2, y1, 0.0f);	// top right
  glTexCoord2d (texture_x2, texture_y2);
  glVertex3f (x2, y2, 0.0f);	// bottom right
  glTexCoord2d (texture_x1, texture_y2);
  glVertex3f (x1, y2, 0.0f);	// bottom left
}

void
mod_gl1_utils_display_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
			       GLuint texture, float x1, float y1, float x2, float y2, float texture_x1, float texture_y1, float texture_x2, float texture_y2)
{
  mod_gl1_utils_display_quad_begin (sys_context, utils_context);
  mod_gl1_utils_display_quad_do (sys_context, utils_context, texture, x1, y1, x2, y2, texture_x1, texture_y1, texture_x2, texture_y2);
  ;
  mod_gl1_utils_display_quad_end (sys_context, utils_context);
}

void
mod_gl1_utils_display_texture_with_filter (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					   utils_context, GLuint texture,
					   float x1, float y1, float x2,
					   float y2, float texture_x1, float texture_y1, float texture_x2, float texture_y2, int pixelize)
{
  glBindTexture (GL_TEXTURE_2D, texture);
  if (pixelize)
    {
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
  else
    {
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  glMatrixMode (GL_TEXTURE);
  glPushMatrix ();
  glLoadIdentity ();
  glBegin (GL_QUADS);
  glTexCoord2d (texture_x1, texture_y1);
  glVertex3f (x1, y1, 0.0f);	// top left
  glTexCoord2d (texture_x2, texture_y1);
  glVertex3f (x2, y1, 0.0f);	// top right
  glTexCoord2d (texture_x2, texture_y2);
  glVertex3f (x2, y2, 0.0f);	// bottom right
  glTexCoord2d (texture_x1, texture_y2);
  glVertex3f (x1, y2, 0.0f);	// bottom left
  glEnd ();

  glMatrixMode (GL_TEXTURE);
  glPopMatrix ();
}
