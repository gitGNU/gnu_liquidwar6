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

/*
 * Setup main light (top left).
 */
void
mod_gl1_utils_main_light (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  GLfloat mainlight_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat mainlight_position[4] = { 1.0f, 1.0f, 1.0f, 0.0f };

  mainlight_specular[0] = utils_context->const_data.mainlight_r;
  mainlight_specular[1] = utils_context->const_data.mainlight_g;
  mainlight_specular[2] = utils_context->const_data.mainlight_b;
  mainlight_specular[3] = utils_context->const_data.mainlight_a;
  mainlight_position[0] = utils_context->const_data.mainlight_x;
  mainlight_position[1] = utils_context->const_data.mainlight_y;
  mainlight_position[2] = utils_context->const_data.mainlight_z;

  glEnable (GL_LIGHTING);
  glLightfv (GL_LIGHT0, GL_POSITION, mainlight_position);
  glLightfv (GL_LIGHT0, GL_SPECULAR, mainlight_specular);
  glEnable (GL_LIGHT0);
}
