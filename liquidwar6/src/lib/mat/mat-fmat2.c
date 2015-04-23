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
 * lw6mat_fmat2_zero
 *
 * @fmat2: the matrix to initialize.
 *
 * Fills the matrix with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_zero (lw6mat_fmat2_t * fmat2)
{
  memset (fmat2, 0, sizeof (lw6mat_fmat2_t));
}

/**
 * lw6mat_fmat2_identity
 *
 * @fmat2: the matrix to initialize.
 *
 * Loads the matrix with the identity matrix, that is, zero everywhere
 * but one on the main diag.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_identity (lw6mat_fmat2_t * fmat2)
{
  fmat2->m[0][0] = LW6MAT_F_1;
  fmat2->m[0][1] = LW6MAT_F_0;
  fmat2->m[1][0] = LW6MAT_F_0;
  fmat2->m[1][1] = LW6MAT_F_1;
}

/**
 * lw6mat_fmat2_translation
 *
 * @fmat2: the matrix to initialize.
 * @f: value which defines the translation.
 *
 * Loads the matrix with a translation transformation matrix.
 * By multiplicating by this matrix, a translation is done.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_translation (lw6mat_fmat2_t * fmat2, float f)
{
  lw6mat_fmat2_identity (fmat2);
  fmat2->m[1][0] = f;
}

/**
 * lw6mat_fmat2_scale
 *
 * @fmat2: the matrix to initialize.
 * @f: value used to scale matrix.
 *
 * Loads the matrix with a scale matrix. By multiplicating by this
 * matrix, a scaling is done.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_scale (lw6mat_fmat2_t * fmat2, float f)
{
  lw6mat_fmat2_identity (fmat2);

  fmat2->m[0][0] = f;
}

/**
 * lw6mat_fmat2_is_same
 *
 * @fmat2_a: 1st matrix to compare
 * @fmat2_b: 2nd matrix to compare
 *
 * Compares two matrix, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_fmat2_is_same (const lw6mat_fmat2_t * fmat2_a, const lw6mat_fmat2_t * fmat2_b)
{
  return (!memcmp ((void *) fmat2_a, (void *) fmat2_b, sizeof (lw6mat_fmat2_t)));
}

/**
 * lw6mat_fmat2_transpose
 *
 * @fmat2: the matrix to transpose
 *
 * Transposes the matrix, that is, inverts rows and columns.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_transpose (lw6mat_fmat2_t * fmat2)
{
  float tmp;

  tmp = fmat2->m[1][0];
  fmat2->m[1][0] = fmat2->m[0][1];
  fmat2->m[0][1] = tmp;
}

/**
 * lw6mat_fmat2_det
 *
 * @fmat2: the matrix used to calculate the determinant
 *
 * Calulates the determinant of the matrix.
 *
 * Return value: the determinant.
 */
float
lw6mat_fmat2_det (const lw6mat_fmat2_t * fmat2)
{
  /*
   * Wooo I'm so lazy, got this one from :
   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/twoD/
   */
  return fmat2->m[0][0] * fmat2->m[1][1] - fmat2->m[0][1] * fmat2->m[1][0];
}

/**
 * lw6mat_fmat2_mul_scale
 *
 * @fmat2: matrix to modify
 * @f: scale factor
 *
 * Scales the matrix by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_fmat2_mul_scale (lw6mat_fmat2_t * fmat2, float f)
{
  int i;

  for (i = 0; i < LW6MAT_MAT2_V_SIZE_X_SIZE; ++i)
    {
      fmat2->v[i] *= f;
    }
}

/**
 * lw6mat_fmat2_inv
 *
 * @sys_context: global system context
 * @fmat2_dst: the matrix inverted
 * @fmat2_src: the matrix to invert
 *
 * Inverts a matrix. Probably not the fastest implementation, but
 * should work in all cases. Use hardware accelerated API such as
 * OpenGL on dedicated hardware if you want power.
 *
 * Return value: 1 if inverted, 0 if error, typically if determinant was 0, matrix
 * can not be inverted.
 */
int
lw6mat_fmat2_inv (lw6sys_context_t * sys_context, lw6mat_fmat2_t * fmat2_dst, const lw6mat_fmat2_t * fmat2_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fmat2_dst == fmat2_src)
    {
      lw6mat_fmat2_t fmat2_tmp = *fmat2_src;

      return lw6mat_fmat2_inv (sys_context, fmat2_dst, &fmat2_tmp);
    }
  else
    {
      float det = lw6mat_fmat2_det (fmat2_src);

      if (!lw6mat_is_similar_f (det, LW6MAT_F_0))
	{
	  /*
	   * Wooo I'm so lazy, got this one from :
	   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/twoD/
	   */
	  fmat2_dst->m[0][0] = fmat2_src->m[1][1] / det;
	  fmat2_dst->m[0][1] = -fmat2_src->m[0][1] / det;
	  fmat2_dst->m[1][0] = -fmat2_src->m[1][0] / det;
	  fmat2_dst->m[1][1] = fmat2_src->m[0][0] / det;

	  return 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to invert non-invertible fmat2 matrix, determinant is 0"));

	  return 0;
	}
    }
}

/**
 * lw6mat_fmat2_mul_fmat2
 *
 * @fmat2: the result matrix
 * @fmat2_a: the 1st matrix to multiply, on the left
 * @fmat2_b: the 2nd matrix to multiply, on the right
 *
 * Classic matrix multiplication.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_mul_fmat2 (lw6mat_fmat2_t * fmat2, const lw6mat_fmat2_t * fmat2_a, const lw6mat_fmat2_t * fmat2_b)
{
  /*
   * In case src(s) and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fmat2 == fmat2_a)
    {
      lw6mat_fmat2_t fmat2_tmp = *fmat2_a;

      lw6mat_fmat2_mul_fmat2 (fmat2, &fmat2_tmp, fmat2_b);
    }
  else if (fmat2 == fmat2_b)
    {
      lw6mat_fmat2_t fmat2_tmp = *fmat2_b;

      lw6mat_fmat2_mul_fmat2 (fmat2, fmat2_a, &fmat2_tmp);
    }
  else
    {
      int i, j;

      for (i = 0; i < LW6MAT_MAT2_M_SIZE; ++i)
	{
	  for (j = 0; j < LW6MAT_MAT2_M_SIZE; ++j)
	    {
	      fmat2->m[i][j] = fmat2_a->m[0][j] * fmat2_b->m[i][0] + fmat2_a->m[1][j] * fmat2_b->m[i][1];
	    }
	}
    }
}

/**
 * lw6mat_fmat2_mul_fvec2
 *
 * @fvec2_dst: the result vector
 * @fmat2_a: the matrix to use
 * @fvec2_src: the source vector
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
lw6mat_fmat2_mul_fvec2 (lw6mat_fvec2_t * fvec2_dst, const lw6mat_fmat2_t * fmat2, const lw6mat_fvec2_t * fvec2_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fvec2_dst == fvec2_src)
    {
      lw6mat_fvec2_t fvec2_tmp = *fvec2_src;

      lw6mat_fmat2_mul_fvec2 (fvec2_dst, fmat2, &fvec2_tmp);
    }
  else
    {
      fvec2_dst->v[0] = fmat2->m[0][0] * fvec2_src->v[0] + fmat2->m[1][0] * fvec2_src->v[1];
      fvec2_dst->v[1] = fmat2->m[0][1] * fvec2_src->v[0] + fmat2->m[1][1] * fvec2_src->v[1];
    }
}

/**
 * lw6mat_fmat2_repr
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
lw6mat_fmat2_repr (lw6sys_context_t * sys_context, const lw6mat_fmat2_t * fmat2)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf (sys_context, "%s %s %dx%d\n[ \t%f\t%f\n\t%f\t%f ]", LW6MAT_REPR_F,
			LW6MAT_REPR_MAT, LW6MAT_MAT2_M_SIZE, LW6MAT_MAT2_M_SIZE, fmat2->m[0][0], fmat2->m[1][0], fmat2->m[0][1], fmat2->m[1][1]);

  return repr;
}
