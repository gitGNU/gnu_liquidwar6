/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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
#include "../../mod-gl-internal.h"
#include "gl-cylinder-internal.h"

float
_mod_gl_menu_cylinder_get_cylinder_y (mod_gl_utils_context_t * utils_context,
				      _mod_gl_menu_cylinder_context_t *
				      cylinder_context, int i, int n)
{
  float y;

  y = -cylinder_context->const_data.between1 * (i - (n - 1) / 2.0f) / n;

  return y;
}

/*
 * Display a horizontal  cylinder.
 */
void
_mod_gl_menu_cylinder_draw_cylinder (mod_gl_utils_context_t * utils_context,
				     _mod_gl_menu_cylinder_context_t *
				     cylinder_context, GLenum mode, int i,
				     int n, float relative_text_width)
{
  GLUquadricObj *cyl;
  int32_t dt;

  cyl = gluNewQuadric ();
  if (cyl != NULL)
    {
      float radius;
      float cyl_height;
      int slices;
      float y;
      float dy;

      gluQuadricTexture (cyl, (mode == GL_RENDER) ? GL_TRUE : GL_FALSE);

      radius = cylinder_context->const_data.radius1 / n;
      cyl_height = relative_text_width *
	cylinder_context->const_data.cyl_height *
	((float) utils_context->video_mode.width) /
	((float) utils_context->video_mode.height);
      y =
	_mod_gl_menu_cylinder_get_cylinder_y (utils_context, cylinder_context,
					      i, n);
      dt =
	mod_gl_utils_timer_get_cycle (utils_context) %
	cylinder_context->const_data.oscil_period;
      dy =
	(cylinder_context->const_data.oscil_range1 / n) * sin (2.0f * M_PI *
							       ((dt /
								 ((float)
								  cylinder_context->
								  const_data.
								  oscil_period))
								+
								(((float) i) /
								 ((float)
								  n))));
      slices = cylinder_context->const_data.slices1 / n;
      if (slices < cylinder_context->const_data.slices_min)
	{
	  slices = cylinder_context->const_data.slices_min;
	}

      glMatrixMode (GL_MODELVIEW);
      glPushMatrix ();
      glLoadIdentity ();
      glTranslatef (-cyl_height / 2.0f, 0.0f, -1.0f);
      glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
      glTranslatef (0.0f, y + dy, 0.0f);

      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("%s.gluCylinder begin %d"),
		  __FUNCTION__, mode);
      gluCylinder (cyl, radius, radius, cyl_height, slices,
		   cylinder_context->const_data.stacks);
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("%s.gluCylinder end"), __FUNCTION__);

      glMatrixMode (GL_MODELVIEW);
      glPopMatrix ();

      gluDeleteQuadric (cyl);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create quadric"));
    }
}

void
_mod_gl_menu_cylinder_draw_cylinder_corners (mod_gl_utils_context_t *
					     utils_context,
					     _mod_gl_menu_cylinder_context_t *
					     cylinder_context, int i, int n,
					     float relative_text_width,
					     int pass_through)
{
  int32_t dt;

  float radius;
  float cyl_height;
  float y;
  float dy;

  radius = cylinder_context->const_data.radius1 / n;
  cyl_height = relative_text_width *
    cylinder_context->const_data.cyl_height *
    ((float) utils_context->video_mode.width) /
    ((float) utils_context->video_mode.height);
  y =
    _mod_gl_menu_cylinder_get_cylinder_y (utils_context, cylinder_context,
					  i, n);
  dt =
    mod_gl_utils_timer_get_cycle (utils_context) %
    cylinder_context->const_data.oscil_period;
  dy =
    (cylinder_context->const_data.oscil_range1 / n) * sin (2.0f * M_PI *
							   ((dt /
							     ((float)
							      cylinder_context->
							      const_data.
							      oscil_period)) +
							    (((float) i) /
							     ((float) n))));
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();
  glTranslatef (-cyl_height / 2.0f, 0.0f, -1.0f);
  glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef (0.0f, y + dy, 0.0f);

  glPassThrough ((float) pass_through);
  glBegin (GL_POINTS);
  glVertex3f (0.0f, radius, 0.0f);
  glVertex3f (0.0f, radius, cyl_height);
  glVertex3f (0.0f, -radius, cyl_height);
  glVertex3f (0.0f, -radius, 0.0f);
  glVertex3f (0.0f, radius, cyl_height * 0.33);
  glVertex3f (0.0f, radius, cyl_height * 0.67);
  glVertex3f (0.0f, -radius, cyl_height * 0.67);
  glVertex3f (0.0f, -radius, cyl_height * 0.33);
  glEnd ();

  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
}

void
_mod_gl_menu_cylinder_draw_sphere (mod_gl_utils_context_t * utils_context,
				   _mod_gl_menu_cylinder_context_t *
				   cylinder_context, GLenum mode, int i,
				   int n, int sphere_i, int nb_spheres)
{
  GLUquadricObj *cyl;

  cyl = gluNewQuadric ();
  if (cyl != NULL)
    {
      float radius;
      int slices;
      int stacks;
      float y;
      float x;

      gluQuadricTexture (cyl, (mode == GL_RENDER) ? GL_TRUE : GL_FALSE);

      radius = cylinder_context->const_data.sphere_radius1 / n;
      y = -cylinder_context->const_data.between1 * (i - (n - 1) / 2.0f) / n;
      x =
	(cylinder_context->const_data.sphere_between1 *
	 (sphere_i + 0.5f - nb_spheres / 2.0f)) / n;
      slices = cylinder_context->const_data.sphere_slices1 / n;
      if (slices < cylinder_context->const_data.sphere_slices_min)
	{
	  slices = cylinder_context->const_data.sphere_slices_min;
	}
      stacks = slices;
      glMatrixMode (GL_MODELVIEW);
      glPushMatrix ();
      glLoadIdentity ();
      glTranslatef (x, y, -1.0f);

      gluSphere (cyl, radius, slices, stacks);

      glMatrixMode (GL_MODELVIEW);
      glPopMatrix ();

      gluDeleteQuadric (cyl);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create quadric"));
    }
}

void
_mod_gl_menu_cylinder_draw_sphere_corners (mod_gl_utils_context_t *
					   utils_context,
					   _mod_gl_menu_cylinder_context_t *
					   cylinder_context, int i, int n,
					   int sphere_i, int nb_spheres,
					   int pass_through)
{
  float radius;
  float y;
  float x;

  radius = cylinder_context->const_data.sphere_radius1 / n;
  y = -cylinder_context->const_data.between1 * (i - (n - 1) / 2.0f) / n;
  x =
    (cylinder_context->const_data.sphere_between1 *
     (sphere_i + 0.5f - nb_spheres / 2.0f)) / n;
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();
  glTranslatef (x, y, -1.0f);

  glPassThrough ((float) pass_through);
  glBegin (GL_POINTS);

  glVertex3f (-radius, -radius, 0.0f);
  glVertex3f (radius, -radius, 0.0f);
  glVertex3f (radius, radius, 0.0f);
  glVertex3f (-radius, radius, 0.0f);

  glEnd ();

  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
}

void
_mod_gl_menu_cylinder_draw_fixed_cylinder (mod_gl_utils_context_t *
					   utils_context,
					   _mod_gl_menu_cylinder_context_t *
					   cylinder_context, GLenum mode,
					   float offset, float radius,
					   float cyl_height,
					   float cyl_height_offset,
					   float rotate)
{
  GLUquadricObj *cyl;

  cyl = gluNewQuadric ();
  if (cyl != NULL)
    {
      int slices;
      int stacks;

      gluQuadricTexture (cyl, (mode == GL_RENDER) ? GL_TRUE : GL_FALSE);

      slices = cylinder_context->const_data.slices_min;
      stacks = cylinder_context->const_data.stacks;

      glMatrixMode (GL_MODELVIEW);
      glPushMatrix ();
      glLoadIdentity ();
      glTranslatef (cyl_height_offset - cyl_height / 2.0f, 0.0f, -1.0f);
      glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
      glTranslatef (0.0f, offset, 0.0f);

      glTranslatef (-0.0f, 0.0f, cyl_height / 2.0f);
      glRotatef (rotate, 1.0f, 0.0f, 0.0f);
      glTranslatef (0.0f, 0.0f, -cyl_height / 2.0f);

      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("%s.gluCylinder begin %d"),
		  __FUNCTION__, mode);
      gluCylinder (cyl, radius, radius, cyl_height, slices, stacks);
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("%s.gluCylinder end"), __FUNCTION__);

      glMatrixMode (GL_MODELVIEW);
      glPopMatrix ();

      gluDeleteQuadric (cyl);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create quadric"));
    }
}

void
_mod_gl_menu_cylinder_draw_fixed_cylinder_corners (mod_gl_utils_context_t *
						   utils_context,
						   _mod_gl_menu_cylinder_context_t
						   * cylinder_context,
						   float offset, float radius,
						   float cyl_height,
						   float cyl_height_offset,
						   float rotate,
						   float relative_text_width,
						   int pass_through)
{
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();
  glTranslatef (cyl_height_offset - cyl_height / 2.0f, 0.0f, -1.0f);
  glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef (0.0f, offset, 0.0f);

  glTranslatef (-0.0f, 0.0f, cyl_height / 2.0f);
  glRotatef (rotate, 1.0f, 0.0f, 0.0f);
  glTranslatef (0.0f, 0.0f, -cyl_height / 2.0f);

  glPassThrough ((float) pass_through);
  glBegin (GL_POINTS);

  glVertex3f (0.0f, radius, 0.0f);
  glVertex3f (0.0f, radius, cyl_height);
  glVertex3f (0.0f, -radius, cyl_height);
  glVertex3f (0.0f, -radius, 0.0f);
  glVertex3f (0.0f, radius, cyl_height * 0.33);
  glVertex3f (0.0f, radius, cyl_height * 0.67);
  glVertex3f (0.0f, -radius, cyl_height * 0.67);
  glVertex3f (0.0f, -radius, cyl_height * 0.33);

  glEnd ();

  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
}
