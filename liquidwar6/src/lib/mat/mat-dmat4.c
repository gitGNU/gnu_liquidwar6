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

#include "mat.h"

/**
 * lw6mat_dmat4_zero
 *
 * @dmat4: the matrix to initialize.
 *
 * Fills the matrix with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_zero (lw6mat_dmat4_t * dmat4)
{
  memset (dmat4, 0, sizeof (lw6mat_dmat4_t));
}

/**
 * lw6mat_dmat4_identity
 *
 * @dmat4: the matrix to initialize.
 *
 * Loads the matrix with the identity matrix, that is, zero everywhere
 * but one on the main diag.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_identity (lw6mat_dmat4_t * dmat4)
{
  int i;

  lw6mat_dmat4_zero (dmat4);

  for (i = 0; i < LW6MAT_MAT4_M_SIZE; ++i)
    {
      dmat4->m[i][i] = LW6MAT_D_1;
    }
}

/**
 * lw6mat_dmat4_translation
 *
 * @dmat4: the matrix to initialize.
 * @dvec3: vector which defines the translation.
 *
 * Loads the matrix with a translation transformation matrix.
 * By multiplicating by this matrix, a translation is done.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_translation (lw6mat_dmat4_t * dmat4, const lw6mat_dvec3_t * dvec3)
{
  int i;

  lw6mat_dmat4_identity (dmat4);

  for (i = 0; i < LW6MAT_VEC3_V_SIZE; ++i)
    {
      dmat4->m[LW6MAT_VEC3_V_SIZE][i] = dvec3->v[i];
    }
}

/**
 * lw6mat_dmat4_scale
 *
 * @dmat4: the matrix to initialize.
 * @dvec3: value used to scale matrix.
 *
 * Loads the matrix with a scale matrix. By multiplicating by this
 * matrix, a scaling is done.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_scale (lw6mat_dmat4_t * dmat4, const lw6mat_dvec3_t * dvec3)
{
  int i;

  lw6mat_dmat4_identity (dmat4);

  for (i = 0; i < LW6MAT_VEC3_V_SIZE; ++i)
    {
      dmat4->m[i][i] = dvec3->v[i];
    }
}

/**
 * lw6mat_dmat4_rot_x
 *
 * @dmat4: the matrix to initialize.
 * @r: value used to for the rotation, angle in radians.
 *
 * Loads the matrix with a rotation matrix. By multiplicating by this
 * matrix, a rotation is done, over the X axis.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_rot_x (lw6mat_dmat4_t * dmat4, double r)
{
  float c = cos (r);
  float s = sin (r);

  lw6mat_dmat4_identity (dmat4);

  dmat4->m[1][1] = c;
  dmat4->m[1][2] = s;
  dmat4->m[2][1] = -s;
  dmat4->m[2][2] = c;
}

/**
 * lw6mat_dmat4_rot_y
 *
 * @dmat4: the matrix to initialize.
 * @r: value used to for the rotation, angle in radians.
 *
 * Loads the matrix with a rotation matrix. By multiplicating by this
 * matrix, a rotation is done, over the Y axis.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_rot_y (lw6mat_dmat4_t * dmat4, double r)
{
  float c = cos (r);
  float s = sin (r);

  lw6mat_dmat4_identity (dmat4);

  dmat4->m[2][2] = c;
  dmat4->m[2][0] = s;
  dmat4->m[0][2] = -s;
  dmat4->m[0][0] = c;
}

/**
 * lw6mat_dmat4_rot_z
 *
 * @dmat4: the matrix to initialize.
 * @r: value used to for the rotation, angle in radians.
 *
 * Loads the matrix with a rotation matrix. By multiplicating by this
 * matrix, a rotation is done, over the Z axis.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_rot_z (lw6mat_dmat4_t * dmat4, double r)
{
  float c = cos (r);
  float s = sin (r);

  lw6mat_dmat4_identity (dmat4);

  dmat4->m[0][0] = c;
  dmat4->m[0][1] = s;
  dmat4->m[1][0] = -s;
  dmat4->m[1][1] = c;
}

/**
 * lw6mat_dmat4_ortho
 *
 * @dmat4: the matrix to initialize.
 * @left: left plane coordinate
 * @right: right plane coordinate
 * @bottom: bottom plane coordinate
 * @top: top plane coordinate
 * @nearval: near plane coordinate
 * @farval: far plane coordinate
 *
 * Loads the matrix with an orthogonal projection matrix.
 * Does it the way glOrtho would, see
 * https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
 * for details.
 * Note: use -nearVal and -farVal to initialize.
 * It's a little akward, if you expect to pass vectors with positions
 * ranging from nearVal to farVal then you need to pass -nearVal and
 * -farVal to this function. This is probably due to the fact that
 * with a right-handed basis and X,Y set up "as usual", then Z is negative
 * when going farther and farther. This tweak allows farVal to yield
 * +1 and nearVal -1. We keep this function as is here, as this is the
 * way OpenGL functions seem to work.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_ortho (lw6mat_dmat4_t * dmat4, double left, double right, double bottom, double top, double nearval, double farval)
{
  double dx = right - left;
  double dy = top - bottom;
  double dz = farval - nearval;

  lw6mat_dmat4_identity (dmat4);

  if (dx != 0.0f)
    {
      dmat4->m[0][0] = 2.0f / dx;
      dmat4->m[3][0] = -(right + left) / dx;
    }
  if (dy != 0.0f)
    {
      dmat4->m[1][1] = 2.0f / dy;
      dmat4->m[3][1] = -(top + bottom) / dy;
    }
  if (dz != 0.0f)
    {
      dmat4->m[2][2] = -2.0f / dz;
      dmat4->m[3][2] = -(nearval + farval) / dz;
    }
}

/**
 * lw6mat_dmat4_perspective
 *
 * @dmat4: the matrix to initialize.
 * @fovy: vertical field of view (degrees, not radians)
 * @aspect: x/y ratio
 * @znear: near plane coordinate (use -znear to initialize)
 * @zfar: far plane coordinate (use -zfar to initialize)
 *
 * Loads the matrix with a projection matrix.
 * Does it the way gluPerspective would, see
 * https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
 * for details.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_perspective (lw6mat_dmat4_t * dmat4, double fovy, double aspect, double znear, double zfar)
{
  double radfovy2 = lw6sys_math_deg2rad (fovy) / 2.0;
  double s = sin (radfovy2);
  double dz = znear - zfar;

  lw6mat_dmat4_zero (dmat4);

  if (s != 0.0f)
    {
      double f = cos (radfovy2) / s;

      if (aspect != 0.0f)
	{
	  dmat4->m[0][0] = f / aspect;
	  dmat4->m[1][1] = f;
	}
    }

  if (dz != 0.0f)
    {
      dmat4->m[2][2] = (znear + zfar) / dz;
      dmat4->m[3][2] = 2.0f * znear * zfar / dz;
    }

  dmat4->m[2][3] = -LW6MAT_D_1;
}

/**
 * lw6mat_dmat4_is_same
 *
 * @dmat4_a: 1st matrix to compare
 * @dmat4_b: 2nd matrix to compare
 *
 * Compares two matrix, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_dmat4_is_same (const lw6mat_dmat4_t * dmat4_a, const lw6mat_dmat4_t * dmat4_b)
{
  return (!memcmp ((void *) dmat4_a, (void *) dmat4_b, sizeof (lw6mat_dmat4_t)));
}

/**
 * lw6mat_dmat4_transpose
 *
 * @dmat4: the matrix to transpose
 *
 * Transposes the matrix, that is, inverts rows and columns.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_transpose (lw6mat_dmat4_t * dmat4)
{
  int i, j;
  double tmp;

  for (i = 1; i < LW6MAT_MAT4_M_SIZE; ++i)
    {
      for (j = 0; j < i; ++j)
	{
	  tmp = dmat4->m[i][j];
	  dmat4->m[i][j] = dmat4->m[j][i];
	  dmat4->m[j][i] = tmp;
	}
    }
}

/**
 * lw6mat_dmat4_det
 *
 * @dmat4: the matrix used to calculate the determinant
 *
 * Calulates the determinant of the matrix.
 *
 * Return value: the determinant.
 */
double
lw6mat_dmat4_det (const lw6mat_dmat4_t * dmat4)
{
  /*
   * Wooo I'm so lazy, got this one from :
   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
   */
  return dmat4->m[0][3] * dmat4->m[1][2] * dmat4->m[2][1] * dmat4->m[3][0] -
    dmat4->m[0][2] * dmat4->m[1][3] * dmat4->m[2][1] * dmat4->m[3][0] -
    dmat4->m[0][3] * dmat4->m[1][1] * dmat4->m[2][2] * dmat4->m[3][0] +
    dmat4->m[0][1] * dmat4->m[1][3] * dmat4->m[2][2] * dmat4->m[3][0] +
    dmat4->m[0][2] * dmat4->m[1][1] * dmat4->m[2][3] * dmat4->m[3][0] -
    dmat4->m[0][1] * dmat4->m[1][2] * dmat4->m[2][3] * dmat4->m[3][0] -
    dmat4->m[0][3] * dmat4->m[1][2] * dmat4->m[2][0] * dmat4->m[3][1] +
    dmat4->m[0][2] * dmat4->m[1][3] * dmat4->m[2][0] * dmat4->m[3][1] +
    dmat4->m[0][3] * dmat4->m[1][0] * dmat4->m[2][2] * dmat4->m[3][1] -
    dmat4->m[0][0] * dmat4->m[1][3] * dmat4->m[2][2] * dmat4->m[3][1] -
    dmat4->m[0][2] * dmat4->m[1][0] * dmat4->m[2][3] * dmat4->m[3][1] +
    dmat4->m[0][0] * dmat4->m[1][2] * dmat4->m[2][3] * dmat4->m[3][1] +
    dmat4->m[0][3] * dmat4->m[1][1] * dmat4->m[2][0] * dmat4->m[3][2] -
    dmat4->m[0][1] * dmat4->m[1][3] * dmat4->m[2][0] * dmat4->m[3][2] -
    dmat4->m[0][3] * dmat4->m[1][0] * dmat4->m[2][1] * dmat4->m[3][2] +
    dmat4->m[0][0] * dmat4->m[1][3] * dmat4->m[2][1] * dmat4->m[3][2] +
    dmat4->m[0][1] * dmat4->m[1][0] * dmat4->m[2][3] * dmat4->m[3][2] -
    dmat4->m[0][0] * dmat4->m[1][1] * dmat4->m[2][3] * dmat4->m[3][2] -
    dmat4->m[0][2] * dmat4->m[1][1] * dmat4->m[2][0] * dmat4->m[3][3] +
    dmat4->m[0][1] * dmat4->m[1][2] * dmat4->m[2][0] * dmat4->m[3][3] +
    dmat4->m[0][2] * dmat4->m[1][0] * dmat4->m[2][1] * dmat4->m[3][3] -
    dmat4->m[0][0] * dmat4->m[1][2] * dmat4->m[2][1] * dmat4->m[3][3] -
    dmat4->m[0][1] * dmat4->m[1][0] * dmat4->m[2][2] * dmat4->m[3][3] + dmat4->m[0][0] * dmat4->m[1][1] * dmat4->m[2][2] * dmat4->m[3][3];
}

/**
 * lw6mat_dmat4_mul_scale
 *
 * @dmat4: matrix to modify
 * @f: scale factor
 *
 * Scales the matrix by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_dmat4_mul_scale (lw6mat_dmat4_t * dmat4, double f)
{
  int i;

  for (i = 0; i < LW6MAT_MAT4_V_SIZE_X_SIZE; ++i)
    {
      dmat4->v[i] *= f;
    }
}

/**
 * lw6mat_dmat4_inv
 *
 * @sys_context: global system context
 * @dmat4_dst: the matrix inverted
 * @dmat4_src: the matrix to invert
 *
 * Inverts a matrix. Probably not the fastest implementation, but
 * should work in all cases. Use hardware accelerated API such as
 * OpenGL on dedicated hardware if you want power.
 *
 * Return value: 1 if inverted, 0 if error, typically if determinant was 0, matrix
 * can not be inverted.
 */
int
lw6mat_dmat4_inv (lw6sys_context_t * sys_context, lw6mat_dmat4_t * dmat4_dst, const lw6mat_dmat4_t * dmat4_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dmat4_dst == dmat4_src)
    {
      lw6mat_dmat4_t dmat4_tmp = *dmat4_src;

      return lw6mat_dmat4_inv (sys_context, dmat4_dst, &dmat4_tmp);
    }
  else
    {
      double det = lw6mat_dmat4_det (dmat4_src);

      if (!lw6mat_is_similar_d (det, LW6MAT_D_0))
	{
	  /*
	   * Wooo I'm so lazy, got this one from :
	   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
	   */

	  dmat4_dst->m[0][0] =
	    (dmat4_src->m[1][2] * dmat4_src->m[2][3] * dmat4_src->m[3][1] -
	     dmat4_src->m[1][3] * dmat4_src->m[2][2] * dmat4_src->m[3][1] +
	     dmat4_src->m[1][3] * dmat4_src->m[2][1] * dmat4_src->m[3][2] -
	     dmat4_src->m[1][1] * dmat4_src->m[2][3] * dmat4_src->m[3][2] -
	     dmat4_src->m[1][2] * dmat4_src->m[2][1] * dmat4_src->m[3][3] + dmat4_src->m[1][1] * dmat4_src->m[2][2] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[0][1] =
	    (dmat4_src->m[0][3] * dmat4_src->m[2][2] * dmat4_src->m[3][1] -
	     dmat4_src->m[0][2] * dmat4_src->m[2][3] * dmat4_src->m[3][1] -
	     dmat4_src->m[0][3] * dmat4_src->m[2][1] * dmat4_src->m[3][2] +
	     dmat4_src->m[0][1] * dmat4_src->m[2][3] * dmat4_src->m[3][2] +
	     dmat4_src->m[0][2] * dmat4_src->m[2][1] * dmat4_src->m[3][3] - dmat4_src->m[0][1] * dmat4_src->m[2][2] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[0][2] =
	    (dmat4_src->m[0][2] * dmat4_src->m[1][3] * dmat4_src->m[3][1] -
	     dmat4_src->m[0][3] * dmat4_src->m[1][2] * dmat4_src->m[3][1] +
	     dmat4_src->m[0][3] * dmat4_src->m[1][1] * dmat4_src->m[3][2] -
	     dmat4_src->m[0][1] * dmat4_src->m[1][3] * dmat4_src->m[3][2] -
	     dmat4_src->m[0][2] * dmat4_src->m[1][1] * dmat4_src->m[3][3] + dmat4_src->m[0][1] * dmat4_src->m[1][2] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[0][3] =
	    (dmat4_src->m[0][3] * dmat4_src->m[1][2] * dmat4_src->m[2][1] -
	     dmat4_src->m[0][2] * dmat4_src->m[1][3] * dmat4_src->m[2][1] -
	     dmat4_src->m[0][3] * dmat4_src->m[1][1] * dmat4_src->m[2][2] +
	     dmat4_src->m[0][1] * dmat4_src->m[1][3] * dmat4_src->m[2][2] +
	     dmat4_src->m[0][2] * dmat4_src->m[1][1] * dmat4_src->m[2][3] - dmat4_src->m[0][1] * dmat4_src->m[1][2] * dmat4_src->m[2][3]) / det;
	  dmat4_dst->m[1][0] =
	    (dmat4_src->m[1][3] * dmat4_src->m[2][2] * dmat4_src->m[3][0] -
	     dmat4_src->m[1][2] * dmat4_src->m[2][3] * dmat4_src->m[3][0] -
	     dmat4_src->m[1][3] * dmat4_src->m[2][0] * dmat4_src->m[3][2] +
	     dmat4_src->m[1][0] * dmat4_src->m[2][3] * dmat4_src->m[3][2] +
	     dmat4_src->m[1][2] * dmat4_src->m[2][0] * dmat4_src->m[3][3] - dmat4_src->m[1][0] * dmat4_src->m[2][2] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[1][1] =
	    (dmat4_src->m[0][2] * dmat4_src->m[2][3] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][3] * dmat4_src->m[2][2] * dmat4_src->m[3][0] +
	     dmat4_src->m[0][3] * dmat4_src->m[2][0] * dmat4_src->m[3][2] -
	     dmat4_src->m[0][0] * dmat4_src->m[2][3] * dmat4_src->m[3][2] -
	     dmat4_src->m[0][2] * dmat4_src->m[2][0] * dmat4_src->m[3][3] + dmat4_src->m[0][0] * dmat4_src->m[2][2] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[1][2] =
	    (dmat4_src->m[0][3] * dmat4_src->m[1][2] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][2] * dmat4_src->m[1][3] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][3] * dmat4_src->m[1][0] * dmat4_src->m[3][2] +
	     dmat4_src->m[0][0] * dmat4_src->m[1][3] * dmat4_src->m[3][2] +
	     dmat4_src->m[0][2] * dmat4_src->m[1][0] * dmat4_src->m[3][3] - dmat4_src->m[0][0] * dmat4_src->m[1][2] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[1][3] =
	    (dmat4_src->m[0][2] * dmat4_src->m[1][3] * dmat4_src->m[2][0] -
	     dmat4_src->m[0][3] * dmat4_src->m[1][2] * dmat4_src->m[2][0] +
	     dmat4_src->m[0][3] * dmat4_src->m[1][0] * dmat4_src->m[2][2] -
	     dmat4_src->m[0][0] * dmat4_src->m[1][3] * dmat4_src->m[2][2] -
	     dmat4_src->m[0][2] * dmat4_src->m[1][0] * dmat4_src->m[2][3] + dmat4_src->m[0][0] * dmat4_src->m[1][2] * dmat4_src->m[2][3]) / det;
	  dmat4_dst->m[2][0] =
	    (dmat4_src->m[1][1] * dmat4_src->m[2][3] * dmat4_src->m[3][0] -
	     dmat4_src->m[1][3] * dmat4_src->m[2][1] * dmat4_src->m[3][0] +
	     dmat4_src->m[1][3] * dmat4_src->m[2][0] * dmat4_src->m[3][1] -
	     dmat4_src->m[1][0] * dmat4_src->m[2][3] * dmat4_src->m[3][1] -
	     dmat4_src->m[1][1] * dmat4_src->m[2][0] * dmat4_src->m[3][3] + dmat4_src->m[1][0] * dmat4_src->m[2][1] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[2][1] =
	    (dmat4_src->m[0][3] * dmat4_src->m[2][1] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][1] * dmat4_src->m[2][3] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][3] * dmat4_src->m[2][0] * dmat4_src->m[3][1] +
	     dmat4_src->m[0][0] * dmat4_src->m[2][3] * dmat4_src->m[3][1] +
	     dmat4_src->m[0][1] * dmat4_src->m[2][0] * dmat4_src->m[3][3] - dmat4_src->m[0][0] * dmat4_src->m[2][1] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[2][2] =
	    (dmat4_src->m[0][1] * dmat4_src->m[1][3] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][3] * dmat4_src->m[1][1] * dmat4_src->m[3][0] +
	     dmat4_src->m[0][3] * dmat4_src->m[1][0] * dmat4_src->m[3][1] -
	     dmat4_src->m[0][0] * dmat4_src->m[1][3] * dmat4_src->m[3][1] -
	     dmat4_src->m[0][1] * dmat4_src->m[1][0] * dmat4_src->m[3][3] + dmat4_src->m[0][0] * dmat4_src->m[1][1] * dmat4_src->m[3][3]) / det;
	  dmat4_dst->m[2][3] =
	    (dmat4_src->m[0][3] * dmat4_src->m[1][1] * dmat4_src->m[2][0] -
	     dmat4_src->m[0][1] * dmat4_src->m[1][3] * dmat4_src->m[2][0] -
	     dmat4_src->m[0][3] * dmat4_src->m[1][0] * dmat4_src->m[2][1] +
	     dmat4_src->m[0][0] * dmat4_src->m[1][3] * dmat4_src->m[2][1] +
	     dmat4_src->m[0][1] * dmat4_src->m[1][0] * dmat4_src->m[2][3] - dmat4_src->m[0][0] * dmat4_src->m[1][1] * dmat4_src->m[2][3]) / det;
	  dmat4_dst->m[3][0] =
	    (dmat4_src->m[1][2] * dmat4_src->m[2][1] * dmat4_src->m[3][0] -
	     dmat4_src->m[1][1] * dmat4_src->m[2][2] * dmat4_src->m[3][0] -
	     dmat4_src->m[1][2] * dmat4_src->m[2][0] * dmat4_src->m[3][1] +
	     dmat4_src->m[1][0] * dmat4_src->m[2][2] * dmat4_src->m[3][1] +
	     dmat4_src->m[1][1] * dmat4_src->m[2][0] * dmat4_src->m[3][2] - dmat4_src->m[1][0] * dmat4_src->m[2][1] * dmat4_src->m[3][2]) / det;
	  dmat4_dst->m[3][1] =
	    (dmat4_src->m[0][1] * dmat4_src->m[2][2] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][2] * dmat4_src->m[2][1] * dmat4_src->m[3][0] +
	     dmat4_src->m[0][2] * dmat4_src->m[2][0] * dmat4_src->m[3][1] -
	     dmat4_src->m[0][0] * dmat4_src->m[2][2] * dmat4_src->m[3][1] -
	     dmat4_src->m[0][1] * dmat4_src->m[2][0] * dmat4_src->m[3][2] + dmat4_src->m[0][0] * dmat4_src->m[2][1] * dmat4_src->m[3][2]) / det;
	  dmat4_dst->m[3][2] =
	    (dmat4_src->m[0][2] * dmat4_src->m[1][1] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][1] * dmat4_src->m[1][2] * dmat4_src->m[3][0] -
	     dmat4_src->m[0][2] * dmat4_src->m[1][0] * dmat4_src->m[3][1] +
	     dmat4_src->m[0][0] * dmat4_src->m[1][2] * dmat4_src->m[3][1] +
	     dmat4_src->m[0][1] * dmat4_src->m[1][0] * dmat4_src->m[3][2] - dmat4_src->m[0][0] * dmat4_src->m[1][1] * dmat4_src->m[3][2]) / det;
	  dmat4_dst->m[3][3] =
	    (dmat4_src->m[0][1] * dmat4_src->m[1][2] * dmat4_src->m[2][0] -
	     dmat4_src->m[0][2] * dmat4_src->m[1][1] * dmat4_src->m[2][0] +
	     dmat4_src->m[0][2] * dmat4_src->m[1][0] * dmat4_src->m[2][1] -
	     dmat4_src->m[0][0] * dmat4_src->m[1][2] * dmat4_src->m[2][1] -
	     dmat4_src->m[0][1] * dmat4_src->m[1][0] * dmat4_src->m[2][2] + dmat4_src->m[0][0] * dmat4_src->m[1][1] * dmat4_src->m[2][2]) / det;

	  return 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to invert non-invertible dmat4 matrix, determinant is 0"));

	  return 0;
	}
    }
}

/**
 * lw6mat_dmat4_mul_dmat4
 *
 * @dmat4: the result matrix
 * @dmat4_a: the 1st matrix to multiply, on the left
 * @dmat4_b: the 2nd matrix to multiply, on the right
 *
 * Classic matrix multiplication.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_mul_dmat4 (lw6mat_dmat4_t * dmat4, const lw6mat_dmat4_t * dmat4_a, const lw6mat_dmat4_t * dmat4_b)
{
  /*
   * In case src(s) and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dmat4 == dmat4_a)
    {
      lw6mat_dmat4_t dmat4_tmp = *dmat4_a;

      lw6mat_dmat4_mul_dmat4 (dmat4, &dmat4_tmp, dmat4_b);
    }
  else if (dmat4 == dmat4_b)
    {
      lw6mat_dmat4_t dmat4_tmp = *dmat4_b;

      lw6mat_dmat4_mul_dmat4 (dmat4, dmat4_a, &dmat4_tmp);
    }
  else
    {
      int i, j;

      for (i = 0; i < LW6MAT_MAT4_M_SIZE; ++i)
	{
	  for (j = 0; j < LW6MAT_MAT4_M_SIZE; ++j)
	    {
	      dmat4->m[i][j] = dmat4_a->m[0][j] * dmat4_b->m[i][0]
		+ dmat4_a->m[1][j] * dmat4_b->m[i][1] + dmat4_a->m[2][j] * dmat4_b->m[i][2] + dmat4_a->m[3][j] * dmat4_b->m[i][3];
	    }
	}
    }
}

/**
 * lw6mat_dmat4_mul_dvec4
 *
 * @dvec4_dst: the result vector
 * @dmat4: the matrix to use
 * @dvec4_src: the source vector
 *
 * Multiplication of matrix by vector. The result is a
 * vector, the convention used is that of OpenGL, matrix are
 * column major and vector are columns, that is, should you
 * do it on a paper, vector is placed vertically, on the right of
 * matrix. The other multiplication is not implemented, transposing
 * the matrix will do it the other way if you wish.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_mul_dvec4 (lw6mat_dvec4_t * dvec4_dst, const lw6mat_dmat4_t * dmat4, const lw6mat_dvec4_t * dvec4_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dvec4_dst == dvec4_src)
    {
      lw6mat_dvec4_t dvec4_tmp = *dvec4_src;

      lw6mat_dmat4_mul_dvec4 (dvec4_dst, dmat4, &dvec4_tmp);
    }
  else
    {
      dvec4_dst->v[0] =
	dmat4->m[0][0] * dvec4_src->v[0] + dmat4->m[1][0] * dvec4_src->v[1] + dmat4->m[2][0] * dvec4_src->v[2] + dmat4->m[3][0] * dvec4_src->v[3];
      dvec4_dst->v[1] =
	dmat4->m[0][1] * dvec4_src->v[0] + dmat4->m[1][1] * dvec4_src->v[1] + dmat4->m[2][1] * dvec4_src->v[2] + dmat4->m[3][1] * dvec4_src->v[3];
      dvec4_dst->v[2] =
	dmat4->m[0][2] * dvec4_src->v[0] + dmat4->m[1][2] * dvec4_src->v[1] + dmat4->m[2][2] * dvec4_src->v[2] + dmat4->m[3][2] * dvec4_src->v[3];
      dvec4_dst->v[3] =
	dmat4->m[0][3] * dvec4_src->v[0] + dmat4->m[1][3] * dvec4_src->v[1] + dmat4->m[2][3] * dvec4_src->v[2] + dmat4->m[3][3] * dvec4_src->v[3];
    }
}

/**
 * lw6mat_dmat4_mul_dvec3
 *
 * @dvec3_dst: the result vector
 * @dmat4: the matrix to use
 * @dvec3_src: the source vector
 *
 * Multiplication of matrix by vector. The result is a
 * vector, the convention used is that of OpenGL, matrix are
 * column major and vector are columns, that is, should you
 * do it on a paper, vector is placed vertically, on the right of
 * matrix. The other multiplication is not implemented, transposing
 * the matrix will do it the other way if you wish.
 * The vector, here, is smaller than the matrix, the last element
 * is supposed to be 1, this is how one implements translation through
 * multiplication.
 *
 * Return value: none.
 */
void
lw6mat_dmat4_mul_dvec3 (lw6mat_dvec3_t * dvec3_dst, const lw6mat_dmat4_t * dmat4, const lw6mat_dvec3_t * dvec3_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dvec3_dst == dvec3_src)
    {
      lw6mat_dvec3_t dvec3_tmp = *dvec3_src;

      lw6mat_dmat4_mul_dvec3 (dvec3_dst, dmat4, &dvec3_tmp);
    }
  else
    {
      dvec3_dst->v[0] = dmat4->m[0][0] * dvec3_src->v[0] + dmat4->m[1][0] * dvec3_src->v[1] + dmat4->m[2][0] * dvec3_src->v[2] + dmat4->m[3][0];
      dvec3_dst->v[1] = dmat4->m[0][1] * dvec3_src->v[0] + dmat4->m[1][1] * dvec3_src->v[1] + dmat4->m[2][1] * dvec3_src->v[2] + dmat4->m[3][1];
      dvec3_dst->v[2] = dmat4->m[0][2] * dvec3_src->v[0] + dmat4->m[1][2] * dvec3_src->v[1] + dmat4->m[2][2] * dvec3_src->v[2] + dmat4->m[3][2];
    }
}

/**
 * lw6mat_dmat4_repr
 *
 * @sys_context: global system context
 * @dmat: matrix to represent
 *
 * Gives a readable version of the matrix, the representation
 * uses newlines, with a different line for each row
 *
 * Return value: newly allocated string
 */
char *
lw6mat_dmat4_repr (lw6sys_context_t * sys_context, const lw6mat_dmat4_t * dmat4)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf
    (sys_context, "%s %s %dx%d\n[ \t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f ]",
     LW6MAT_REPR_D, LW6MAT_REPR_MAT, LW6MAT_MAT4_M_SIZE,
     LW6MAT_MAT4_M_SIZE, dmat4->m[0][0], dmat4->m[1][0], dmat4->m[2][0],
     dmat4->m[3][0], dmat4->m[0][1], dmat4->m[1][1], dmat4->m[2][1],
     dmat4->m[3][1], dmat4->m[0][2], dmat4->m[1][2], dmat4->m[2][2], dmat4->m[3][2], dmat4->m[0][3], dmat4->m[1][3], dmat4->m[2][3], dmat4->m[3][3]);

  return repr;
}
