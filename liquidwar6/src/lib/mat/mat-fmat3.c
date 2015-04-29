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
 * lw6mat_fmat3_zero
 *
 * @fmat3: the matrix to initialize.
 *
 * Fills the matrix with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_zero (lw6mat_fmat3_t * fmat3)
{
  memset (fmat3, 0, sizeof (lw6mat_fmat3_t));
}

/**
 * lw6mat_fmat3_identity
 *
 * @fmat3: the matrix to initialize.
 *
 * Loads the matrix with the identity matrix, that is, zero everywhere
 * but one on the main diag.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_identity (lw6mat_fmat3_t * fmat3)
{
  int i;

  lw6mat_fmat3_zero (fmat3);

  for (i = 0; i < LW6MAT_MAT3_M_SIZE; ++i)
    {
      fmat3->m[i][i] = LW6MAT_F_1;
    }
}

/**
 * lw6mat_fmat3_translation
 *
 * @fmat3: the matrix to initialize.
 * @fvec2: vector which defines the translation.
 *
 * Loads the matrix with a translation transformation matrix.
 * By multiplicating by this matrix, a translation is done.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_translation (lw6mat_fmat3_t * fmat3, const lw6mat_fvec2_t * fvec2)
{
  int i;

  lw6mat_fmat3_identity (fmat3);

  for (i = 0; i < LW6MAT_VEC2_V_SIZE; ++i)
    {
      fmat3->m[LW6MAT_VEC2_V_SIZE][i] = fvec2->v[i];
    }
}

/**
 * lw6mat_fmat3_scale
 *
 * @fmat3: the matrix to initialize.
 * @fvec2: value used to scale matrix.
 *
 * Loads the matrix with a scale matrix. By multiplicating by this
 * matrix, a scaling is done.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_scale (lw6mat_fmat3_t * fmat3, const lw6mat_fvec2_t * fvec2)
{
  int i;

  lw6mat_fmat3_identity (fmat3);

  for (i = 0; i < LW6MAT_VEC2_V_SIZE; ++i)
    {
      fmat3->m[i][i] = fvec2->v[i];
    }
}

/**
 * lw6mat_fmat3_rot
 *
 * @fmat3: the matrix to initialize.
 * @r: value used to for the rotation, angle in radians.
 *
 * Loads the matrix with a rotation matrix. By multiplicating by this
 * matrix, a rotation is done, over a virtual Z axis such as Z is the
 * cross product of X and Y.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_rot (lw6mat_fmat3_t * fmat3, float r)
{
  float c = cos (r);
  float s = sin (r);

  lw6mat_fmat3_identity (fmat3);

  fmat3->m[0][0] = c;
  fmat3->m[0][1] = s;
  fmat3->m[1][0] = -s;
  fmat3->m[1][1] = c;
}

/**
 * lw6mat_fmat3_is_same
 *
 * @fmat3_a: 1st matrix to compare
 * @fmat3_b: 2nd matrix to compare
 *
 * Compares two matrix, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_fmat3_is_same (const lw6mat_fmat3_t * fmat3_a, const lw6mat_fmat3_t * fmat3_b)
{
  return (!memcmp ((void *) fmat3_a, (void *) fmat3_b, sizeof (lw6mat_fmat3_t)));
}

/**
 * lw6mat_fmat3_transpose
 *
 * @fmat3: the matrix to transpose
 *
 * Transposes the matrix, that is, inverts rows and columns.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_transpose (lw6mat_fmat3_t * fmat3)
{
  int i, j;
  float tmp;

  for (i = 1; i < LW6MAT_MAT3_M_SIZE; ++i)
    {
      for (j = 0; j < i; ++j)
	{
	  tmp = fmat3->m[i][j];
	  fmat3->m[i][j] = fmat3->m[j][i];
	  fmat3->m[j][i] = tmp;
	}
    }
}

/**
 * lw6mat_fmat3_det
 *
 * @fmat3: the matrix used to calculate the determinant
 *
 * Calulates the determinant of the matrix.
 *
 * Return value: the determinant.
 */
float
lw6mat_fmat3_det (const lw6mat_fmat3_t * fmat3)
{
  /*
   * Wooo I'm so lazy, got this one from :
   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/
   */
  return fmat3->m[0][0] * fmat3->m[1][1] * fmat3->m[2][2] +
    fmat3->m[0][1] * fmat3->m[1][2] * fmat3->m[2][0] +
    fmat3->m[0][2] * fmat3->m[1][0] * fmat3->m[2][1] -
    fmat3->m[0][0] * fmat3->m[1][2] * fmat3->m[2][1] - fmat3->m[0][1] * fmat3->m[1][0] * fmat3->m[2][2] - fmat3->m[0][2] * fmat3->m[1][1] * fmat3->m[2][0];
}

/**
 * lw6mat_fmat3_mul_scale
 *
 * @fmat3: matrix to modify
 * @f: scale factor
 *
 * Scales the matrix by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_fmat3_mul_scale (lw6mat_fmat3_t * fmat3, float f)
{
  int i;

  for (i = 0; i < LW6MAT_MAT3_V_SIZE_X_SIZE; ++i)
    {
      fmat3->v[i] *= f;
    }
}

/**
 * lw6mat_fmat3_inv
 *
 * @sys_context: global system context
 * @fmat3_dst: the matrix inverted
 * @fmat3_src: the matrix to invert
 *
 * Inverts a matrix. Probably not the fastest implementation, but
 * should work in all cases. Use hardware accelerated API such as
 * OpenGL on dedicated hardware if you want power.
 *
 * Return value: 1 if inverted, 0 if error, typically if determinant was 0, matrix
 * can not be inverted.
 */
int
lw6mat_fmat3_inv (lw6sys_context_t * sys_context, lw6mat_fmat3_t * fmat3_dst, const lw6mat_fmat3_t * fmat3_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fmat3_dst == fmat3_src)
    {
      lw6mat_fmat3_t fmat3_tmp = *fmat3_src;

      return lw6mat_fmat3_inv (sys_context, fmat3_dst, &fmat3_tmp);
    }
  else
    {
      float det = lw6mat_fmat3_det (fmat3_src);

      if (!lw6mat_is_similar_f (det, LW6MAT_F_0))
	{
	  /*
	   * Wooo I'm so lazy, got this one from :
	   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/
	   */
	  fmat3_dst->m[0][0] = (fmat3_src->m[1][1] * fmat3_src->m[2][2] - fmat3_src->m[1][2] * fmat3_src->m[2][1]) / det;
	  fmat3_dst->m[0][1] = (fmat3_src->m[0][2] * fmat3_src->m[2][1] - fmat3_src->m[0][1] * fmat3_src->m[2][2]) / det;
	  fmat3_dst->m[0][2] = (fmat3_src->m[0][1] * fmat3_src->m[1][2] - fmat3_src->m[0][2] * fmat3_src->m[1][1]) / det;
	  fmat3_dst->m[1][0] = (fmat3_src->m[1][2] * fmat3_src->m[2][0] - fmat3_src->m[1][0] * fmat3_src->m[2][2]) / det;
	  fmat3_dst->m[1][1] = (fmat3_src->m[0][0] * fmat3_src->m[2][2] - fmat3_src->m[0][2] * fmat3_src->m[2][0]) / det;
	  fmat3_dst->m[1][2] = (fmat3_src->m[0][2] * fmat3_src->m[1][0] - fmat3_src->m[0][0] * fmat3_src->m[1][2]) / det;
	  fmat3_dst->m[2][0] = (fmat3_src->m[1][0] * fmat3_src->m[2][1] - fmat3_src->m[1][1] * fmat3_src->m[2][0]) / det;
	  fmat3_dst->m[2][1] = (fmat3_src->m[0][1] * fmat3_src->m[2][0] - fmat3_src->m[0][0] * fmat3_src->m[2][1]) / det;
	  fmat3_dst->m[2][2] = (fmat3_src->m[0][0] * fmat3_src->m[1][1] - fmat3_src->m[0][1] * fmat3_src->m[1][0]) / det;

	  return 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to invert non-invertible fmat3 matrix, determinant is 0"));

	  return 0;
	}
    }
}

/**
 * lw6mat_fmat3_mul_fmat3
 *
 * @fmat3: the result matrix
 * @fmat3_a: the 1st matrix to multiply, on the left
 * @fmat3_b: the 2nd matrix to multiply, on the right
 *
 * Classic matrix multiplication.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_mul_fmat3 (lw6mat_fmat3_t * fmat3, const lw6mat_fmat3_t * fmat3_a, const lw6mat_fmat3_t * fmat3_b)
{
  /*
   * In case src(s) and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fmat3 == fmat3_a)
    {
      lw6mat_fmat3_t fmat3_tmp = *fmat3_a;

      lw6mat_fmat3_mul_fmat3 (fmat3, &fmat3_tmp, fmat3_b);
    }
  else if (fmat3 == fmat3_b)
    {
      lw6mat_fmat3_t fmat3_tmp = *fmat3_b;

      lw6mat_fmat3_mul_fmat3 (fmat3, fmat3_a, &fmat3_tmp);
    }
  else
    {
      int i, j;

      for (i = 0; i < LW6MAT_MAT3_M_SIZE; ++i)
	{
	  for (j = 0; j < LW6MAT_MAT3_M_SIZE; ++j)
	    {
	      fmat3->m[i][j] = fmat3_a->m[0][j] * fmat3_b->m[i][0] + fmat3_a->m[1][j] * fmat3_b->m[i][1] + fmat3_a->m[2][j] * fmat3_b->m[i][2];
	    }
	}
    }
}

/**
 * lw6mat_fmat3_mul_fvec3
 *
 * @fvec3_dst: the result vector
 * @fmat3_a: the matrix to use
 * @fvec3_src: the source vector
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
lw6mat_fmat3_mul_fvec3 (lw6mat_fvec3_t * fvec3_dst, const lw6mat_fmat3_t * fmat3, const lw6mat_fvec3_t * fvec3_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fvec3_dst == fvec3_src)
    {
      lw6mat_fvec3_t fvec3_tmp = *fvec3_src;

      lw6mat_fmat3_mul_fvec3 (fvec3_dst, fmat3, &fvec3_tmp);
    }
  else
    {
      fvec3_dst->v[0] = fmat3->m[0][0] * fvec3_src->v[0] + fmat3->m[1][0] * fvec3_src->v[1] + fmat3->m[2][0] * fvec3_src->v[2];
      fvec3_dst->v[1] = fmat3->m[0][1] * fvec3_src->v[0] + fmat3->m[1][1] * fvec3_src->v[1] + fmat3->m[2][1] * fvec3_src->v[2];
      fvec3_dst->v[2] = fmat3->m[0][2] * fvec3_src->v[0] + fmat3->m[1][2] * fvec3_src->v[1] + fmat3->m[2][2] * fvec3_src->v[2];
    }
}

/**
 * lw6mat_fmat3_mul_fvec2
 *
 * @fvec2_dst: the result vector
 * @fmat3_a: the matrix to use
 * @fmat2_src: the source vector
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
lw6mat_fmat3_mul_fvec2 (lw6mat_fvec2_t * fvec2_dst, const lw6mat_fmat3_t * fmat3, const lw6mat_fvec2_t * fvec2_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fvec2_dst == fvec2_src)
    {
      lw6mat_fvec2_t fvec2_tmp = *fvec2_src;

      lw6mat_fmat3_mul_fvec2 (fvec2_dst, fmat3, &fvec2_tmp);
    }
  else
    {
      fvec2_dst->v[0] = fmat3->m[0][0] * fvec2_src->v[0] + fmat3->m[1][0] * fvec2_src->v[1] + fmat3->m[2][0];
      fvec2_dst->v[1] = fmat3->m[0][1] * fvec2_src->v[0] + fmat3->m[1][1] * fvec2_src->v[1] + fmat3->m[2][1];
    }
}

/**
 * lw6mat_fmat3_repr
 *
 * @sys_context: global system context
 * @fmat: matrix to represent
 *
 * Gives a readable version of the matrix, the representation
 * uses newlines, with a different line for each row
 *
 * Return value: newly allocated string
 */
char *
lw6mat_fmat3_repr (lw6sys_context_t * sys_context, const lw6mat_fmat3_t * fmat3)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf
    (sys_context, "%s %s %dx%d\n[ \t%f\t%f\t%f\n\t%f\t%f\t%f\n\t%f\t%f\t%f ]",
     LW6MAT_REPR_F, LW6MAT_REPR_MAT, LW6MAT_MAT3_M_SIZE,
     LW6MAT_MAT3_M_SIZE, fmat3->m[0][0], fmat3->m[1][0], fmat3->m[2][0],
     fmat3->m[0][1], fmat3->m[1][1], fmat3->m[2][1], fmat3->m[0][2], fmat3->m[1][2], fmat3->m[2][2]);

  return repr;
}
