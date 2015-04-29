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
 * lw6mat_dmat3_zero
 *
 * @dmat3: the matrix to initialize.
 *
 * Fills the matrix with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_dmat3_zero (lw6mat_dmat3_t * dmat3)
{
  memset (dmat3, 0, sizeof (lw6mat_dmat3_t));
}

/**
 * lw6mat_dmat3_identity
 *
 * @dmat3: the matrix to initialize.
 *
 * Loads the matrix with the identity matrix, that is, zero everywhere
 * but one on the main diag.
 *
 * Return value: none.
 */
void
lw6mat_dmat3_identity (lw6mat_dmat3_t * dmat3)
{
  int i;

  lw6mat_dmat3_zero (dmat3);

  for (i = 0; i < LW6MAT_MAT3_M_SIZE; ++i)
    {
      dmat3->m[i][i] = LW6MAT_D_1;
    }
}

/**
 * lw6mat_dmat3_translation
 *
 * @dmat3: the matrix to initialize.
 * @dvec2: vector which defines the translation.
 *
 * Loads the matrix with a translation transformation matrix.
 * By multiplicating by this matrix, a translation is done.
 *
 * Return value: none.
 */
void
lw6mat_dmat3_translation (lw6mat_dmat3_t * dmat3, const lw6mat_dvec2_t * dvec2)
{
  int i;

  lw6mat_dmat3_identity (dmat3);

  for (i = 0; i < LW6MAT_VEC2_V_SIZE; ++i)
    {
      dmat3->m[LW6MAT_VEC2_V_SIZE][i] = dvec2->v[i];
    }
}

/**
 * lw6mat_dmat3_scale
 *
 * @dmat3: the matrix to initialize.
 * @dvec2: value used to scale matrix.
 *
 * Loads the matrix with a scale matrix. By multiplicating by this
 * matrix, a scaling is done.
 *
 * Return value: none.
 */
void
lw6mat_dmat3_scale (lw6mat_dmat3_t * dmat3, const lw6mat_dvec2_t * dvec2)
{
  int i;

  lw6mat_dmat3_identity (dmat3);

  for (i = 0; i < LW6MAT_VEC2_V_SIZE; ++i)
    {
      dmat3->m[i][i] = dvec2->v[i];
    }
}

/**
 * lw6mat_dmat3_rot
 *
 * @dmat3: the matrix to initialize.
 * @r: value used to for the rotation, angle in radians.
 *
 * Loads the matrix with a rotation matrix. By multiplicating by this
 * matrix, a rotation is done, over a virtual Z axis such as Z is the
 * cross product of X and Y.
 *
 * Return value: none.
 */
void
lw6mat_dmat3_rot (lw6mat_dmat3_t * dmat3, double r)
{
  float c = cos (r);
  float s = sin (r);

  lw6mat_dmat3_identity (dmat3);

  dmat3->m[0][0] = c;
  dmat3->m[0][1] = s;
  dmat3->m[1][0] = -s;
  dmat3->m[1][1] = c;
}

/**
 * lw6mat_dmat3_is_same
 *
 * @dmat3_a: 1st matrix to compare
 * @dmat3_b: 2nd matrix to compare
 *
 * Compares two matrix, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_dmat3_is_same (const lw6mat_dmat3_t * dmat3_a, const lw6mat_dmat3_t * dmat3_b)
{
  return (!memcmp ((void *) dmat3_a, (void *) dmat3_b, sizeof (lw6mat_dmat3_t)));
}

/**
 * lw6mat_dmat3_transpose
 *
 * @dmat3: the matrix to transpose
 *
 * Transposes the matrix, that is, inverts rows and columns.
 *
 * Return value: none.
 */
void
lw6mat_dmat3_transpose (lw6mat_dmat3_t * dmat3)
{
  int i, j;
  double tmp;

  for (i = 1; i < LW6MAT_MAT3_M_SIZE; ++i)
    {
      for (j = 0; j < i; ++j)
	{
	  tmp = dmat3->m[i][j];
	  dmat3->m[i][j] = dmat3->m[j][i];
	  dmat3->m[j][i] = tmp;
	}
    }
}

/**
 * lw6mat_dmat3_det
 *
 * @dmat3: the matrix used to calculate the determinant
 *
 * Calulates the determinant of the matrix.
 *
 * Return value: the determinant.
 */
double
lw6mat_dmat3_det (const lw6mat_dmat3_t * dmat3)
{
  /*
   * Wooo I'm so lazy, got this one from :
   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/
   */
  return dmat3->m[0][0] * dmat3->m[1][1] * dmat3->m[2][2] +
    dmat3->m[0][1] * dmat3->m[1][2] * dmat3->m[2][0] +
    dmat3->m[0][2] * dmat3->m[1][0] * dmat3->m[2][1] -
    dmat3->m[0][0] * dmat3->m[1][2] * dmat3->m[2][1] - dmat3->m[0][1] * dmat3->m[1][0] * dmat3->m[2][2] - dmat3->m[0][2] * dmat3->m[1][1] * dmat3->m[2][0];
}

/**
 * lw6mat_dmat3_mul_scale
 *
 * @dmat3: matrix to modify
 * @f: scale factor
 *
 * Scales the matrix by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_dmat3_mul_scale (lw6mat_dmat3_t * dmat3, double f)
{
  int i;

  for (i = 0; i < LW6MAT_MAT3_V_SIZE_X_SIZE; ++i)
    {
      dmat3->v[i] *= f;
    }
}

/**
 * lw6mat_dmat3_inv
 *
 * @sys_context: global system context
 * @dmat3_dst: the matrix inverted
 * @dmat3_src: the matrix to invert
 *
 * Inverts a matrix. Probably not the fastest implementation, but
 * should work in all cases. Use hardware accelerated API such as
 * OpenGL on dedicated hardware if you want power.
 *
 * Return value: 1 if inverted, 0 if error, typically if determinant was 0, matrix
 * can not be inverted.
 */
int
lw6mat_dmat3_inv (lw6sys_context_t * sys_context, lw6mat_dmat3_t * dmat3_dst, const lw6mat_dmat3_t * dmat3_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dmat3_dst == dmat3_src)
    {
      lw6mat_dmat3_t dmat3_tmp = *dmat3_src;

      return lw6mat_dmat3_inv (sys_context, dmat3_dst, &dmat3_tmp);
    }
  else
    {
      double det = lw6mat_dmat3_det (dmat3_src);

      if (!lw6mat_is_similar_d (det, LW6MAT_D_0))
	{
	  /*
	   * Wooo I'm so lazy, got this one from :
	   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/
	   */
	  dmat3_dst->m[0][0] = (dmat3_src->m[1][1] * dmat3_src->m[2][2] - dmat3_src->m[1][2] * dmat3_src->m[2][1]) / det;
	  dmat3_dst->m[0][1] = (dmat3_src->m[0][2] * dmat3_src->m[2][1] - dmat3_src->m[0][1] * dmat3_src->m[2][2]) / det;
	  dmat3_dst->m[0][2] = (dmat3_src->m[0][1] * dmat3_src->m[1][2] - dmat3_src->m[0][2] * dmat3_src->m[1][1]) / det;
	  dmat3_dst->m[1][0] = (dmat3_src->m[1][2] * dmat3_src->m[2][0] - dmat3_src->m[1][0] * dmat3_src->m[2][2]) / det;
	  dmat3_dst->m[1][1] = (dmat3_src->m[0][0] * dmat3_src->m[2][2] - dmat3_src->m[0][2] * dmat3_src->m[2][0]) / det;
	  dmat3_dst->m[1][2] = (dmat3_src->m[0][2] * dmat3_src->m[1][0] - dmat3_src->m[0][0] * dmat3_src->m[1][2]) / det;
	  dmat3_dst->m[2][0] = (dmat3_src->m[1][0] * dmat3_src->m[2][1] - dmat3_src->m[1][1] * dmat3_src->m[2][0]) / det;
	  dmat3_dst->m[2][1] = (dmat3_src->m[0][1] * dmat3_src->m[2][0] - dmat3_src->m[0][0] * dmat3_src->m[2][1]) / det;
	  dmat3_dst->m[2][2] = (dmat3_src->m[0][0] * dmat3_src->m[1][1] - dmat3_src->m[0][1] * dmat3_src->m[1][0]) / det;

	  return 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to invert non-invertible dmat3 matrix, determinant is 0"));

	  return 0;
	}
    }
}

/**
 * lw6mat_dmat3_mul_dmat3
 *
 * @dmat3: the result matrix
 * @dmat3_a: the 1st matrix to multiply, on the left
 * @dmat3_b: the 2nd matrix to multiply, on the right
 *
 * Classic matrix multiplication.
 *
 * Return value: none.
 */
void
lw6mat_dmat3_mul_dmat3 (lw6mat_dmat3_t * dmat3, const lw6mat_dmat3_t * dmat3_a, const lw6mat_dmat3_t * dmat3_b)
{
  /*
   * In case src(s) and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dmat3 == dmat3_a)
    {
      lw6mat_dmat3_t dmat3_tmp = *dmat3_a;

      lw6mat_dmat3_mul_dmat3 (dmat3, &dmat3_tmp, dmat3_b);
    }
  else if (dmat3 == dmat3_b)
    {
      lw6mat_dmat3_t dmat3_tmp = *dmat3_b;

      lw6mat_dmat3_mul_dmat3 (dmat3, dmat3_a, &dmat3_tmp);
    }
  else
    {
      int i, j;

      for (i = 0; i < LW6MAT_MAT3_M_SIZE; ++i)
	{
	  for (j = 0; j < LW6MAT_MAT3_M_SIZE; ++j)
	    {
	      dmat3->m[i][j] = dmat3_a->m[0][j] * dmat3_b->m[i][0] + dmat3_a->m[1][j] * dmat3_b->m[i][1] + dmat3_a->m[2][j] * dmat3_b->m[i][2];
	    }
	}
    }
}

/**
 * lw6mat_dmat3_mul_dvec3
 *
 * @dvec3_dst: the result vector
 * @dmat3_a: the matrix to use
 * @dvec3_src: the source vector
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
lw6mat_dmat3_mul_dvec3 (lw6mat_dvec3_t * dvec3_dst, const lw6mat_dmat3_t * dmat3, const lw6mat_dvec3_t * dvec3_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dvec3_dst == dvec3_src)
    {
      lw6mat_dvec3_t dvec3_tmp = *dvec3_src;

      lw6mat_dmat3_mul_dvec3 (dvec3_dst, dmat3, &dvec3_tmp);
    }
  else
    {
      dvec3_dst->v[0] = dmat3->m[0][0] * dvec3_src->v[0] + dmat3->m[1][0] * dvec3_src->v[1] + dmat3->m[2][0] * dvec3_src->v[2];
      dvec3_dst->v[1] = dmat3->m[0][1] * dvec3_src->v[0] + dmat3->m[1][1] * dvec3_src->v[1] + dmat3->m[2][1] * dvec3_src->v[2];
      dvec3_dst->v[2] = dmat3->m[0][2] * dvec3_src->v[0] + dmat3->m[1][2] * dvec3_src->v[1] + dmat3->m[2][2] * dvec3_src->v[2];
    }
}

/**
 * lw6mat_dmat3_mul_dvec2
 *
 * @dvec2_dst: the result vector
 * @dmat3_a: the matrix to use
 * @dmat2_src: the source vector
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
lw6mat_dmat3_mul_dvec2 (lw6mat_dvec2_t * dvec2_dst, const lw6mat_dmat3_t * dmat3, const lw6mat_dvec2_t * dvec2_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dvec2_dst == dvec2_src)
    {
      lw6mat_dvec2_t dvec2_tmp = *dvec2_src;

      lw6mat_dmat3_mul_dvec2 (dvec2_dst, dmat3, &dvec2_tmp);
    }
  else
    {
      dvec2_dst->v[0] = dmat3->m[0][0] * dvec2_src->v[0] + dmat3->m[1][0] * dvec2_src->v[1] + dmat3->m[2][0];
      dvec2_dst->v[1] = dmat3->m[0][1] * dvec2_src->v[0] + dmat3->m[1][1] * dvec2_src->v[1] + dmat3->m[2][1];
    }
}

/**
 * lw6mat_dmat3_repr
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
lw6mat_dmat3_repr (lw6sys_context_t * sys_context, const lw6mat_dmat3_t * dmat3)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf
    (sys_context, "%s %s %dx%d\n[ \t%f\t%f\t%f\n\t%f\t%f\t%f\n\t%f\t%f\t%f ]",
     LW6MAT_REPR_D, LW6MAT_REPR_MAT, LW6MAT_MAT3_M_SIZE,
     LW6MAT_MAT3_M_SIZE, dmat3->m[0][0], dmat3->m[1][0], dmat3->m[2][0],
     dmat3->m[0][1], dmat3->m[1][1], dmat3->m[2][1], dmat3->m[0][2], dmat3->m[1][2], dmat3->m[2][2]);

  return repr;
}
