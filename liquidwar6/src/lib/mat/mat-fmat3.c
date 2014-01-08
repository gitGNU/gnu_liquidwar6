/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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
 * lw6mat_fmat3_id
 *
 * @fmat3: the matrix to initialize.
 *
 * Loads the matrix with the identity matrix, that is, zero everywhere
 * but one on the main diag.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_id (lw6mat_fmat3_t * fmat3)
{
  int i;

  lw6mat_fmat3_zero (fmat3);

  for (i = 0; i < LW6MAT_MAT3_M_SIZE; ++i)
    {
      fmat3->m[i][i] = LW6MAT_F_1;
    }
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
lw6mat_fmat3_is_same (const lw6mat_fmat3_t * fmat3_a,
		      const lw6mat_fmat3_t * fmat3_b)
{
  return (!memcmp
	  ((void *) fmat3_a, (void *) fmat3_b, sizeof (lw6mat_fmat3_t)));
}

/**
 * lw6mat_fmat3_trans
 *
 * @fmat3: the matrix to transpose
 *
 * Transposes the matrix, that is, inverts rows and columns.
 *
 * Return value: none.
 */
void
lw6mat_fmat3_trans (lw6mat_fmat3_t * fmat3)
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
    fmat3->m[0][0] * fmat3->m[1][2] * fmat3->m[2][1] -
    fmat3->m[0][1] * fmat3->m[1][0] * fmat3->m[2][2] -
    fmat3->m[0][2] * fmat3->m[1][1] * fmat3->m[2][0];
}

/**
 * lw6mat_fmat3_scale
 *
 * @fmat3: matrix to modify
 * @f: scale factor
 *
 * Scales the matrix by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_fmat3_scale (lw6mat_fmat3_t * fmat3, float f)
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
lw6mat_fmat3_inv (lw6mat_fmat3_t * fmat3_dst,
		  const lw6mat_fmat3_t * fmat3_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (fmat3_dst == fmat3_src)
    {
      lw6mat_fmat3_t fmat3_tmp = *fmat3_src;

      return lw6mat_fmat3_inv (fmat3_dst, &fmat3_tmp);
    }
  else
    {
      float det = lw6mat_fmat3_det (fmat3_src);

      if (det)
	{
	  /*
	   * Wooo I'm so lazy, got this one from :
	   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/
	   */
	  fmat3_dst->m[0][0] =
	    (fmat3_src->m[1][1] * fmat3_src->m[2][2] -
	     fmat3_src->m[1][2] * fmat3_src->m[2][1]) / det;
	  fmat3_dst->m[0][1] =
	    (fmat3_src->m[0][2] * fmat3_src->m[2][1] -
	     fmat3_src->m[0][1] * fmat3_src->m[2][2]) / det;
	  fmat3_dst->m[0][2] =
	    (fmat3_src->m[0][1] * fmat3_src->m[1][2] -
	     fmat3_src->m[0][2] * fmat3_src->m[1][1]) / det;
	  fmat3_dst->m[1][0] =
	    (fmat3_src->m[1][2] * fmat3_src->m[2][0] -
	     fmat3_src->m[1][0] * fmat3_src->m[2][2]) / det;
	  fmat3_dst->m[1][1] =
	    (fmat3_src->m[0][0] * fmat3_src->m[2][2] -
	     fmat3_src->m[0][2] * fmat3_src->m[2][0]) / det;
	  fmat3_dst->m[1][2] =
	    (fmat3_src->m[0][2] * fmat3_src->m[1][0] -
	     fmat3_src->m[0][0] * fmat3_src->m[1][2]) / det;
	  fmat3_dst->m[2][0] =
	    (fmat3_src->m[1][0] * fmat3_src->m[2][1] -
	     fmat3_src->m[1][1] * fmat3_src->m[2][0]) / det;
	  fmat3_dst->m[2][1] =
	    (fmat3_src->m[0][1] * fmat3_src->m[2][0] -
	     fmat3_src->m[0][0] * fmat3_src->m[2][1]) / det;
	  fmat3_dst->m[2][2] =
	    (fmat3_src->m[0][0] * fmat3_src->m[1][1] -
	     fmat3_src->m[0][1] * fmat3_src->m[1][0]) / det;

	  return 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_
		      ("trying to invert non-invertible fmat3 matrix, determinant is 0"));
	  /*
	   * Putting identity in result, yes it's not optimal it's CPU
	   * waste but in case matrix was not invertible, it the
	   * problem will really show afterwards, and the result can
	   * always be inverted again.
	   */
	  lw6mat_fmat3_id (fmat3_dst);

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
lw6mat_fmat3_mul_fmat3 (lw6mat_fmat3_t * fmat3,
			const lw6mat_fmat3_t * fmat3_a,
			const lw6mat_fmat3_t * fmat3_b)
{
  int i, j;

  for (i = 0; i < LW6MAT_MAT3_M_SIZE; ++i)
    {
      for (j = 0; j < LW6MAT_MAT3_M_SIZE; ++j)
	{
	  fmat3->m[i][j] = fmat3_a->m[0][j] * fmat3_b->m[i][0]
	    + fmat3_a->m[1][j] * fmat3_b->m[i][1]
	    + fmat3_a->m[2][j] * fmat3_b->m[i][2];
	}
    }
}

/**
 * lw6mat_fmat3_mul_fvec3
 *
 * @fmat3: the result matrix
 * @fmat3_a: the 1st matrix to multiply, on the left
 * @fmat3_b: the 2nd matrix to multiply, on the right
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
lw6mat_fmat3_mul_fvec3 (lw6mat_fvec3_t * fvec3_dst,
			const lw6mat_fmat3_t * fmat3,
			const lw6mat_fvec3_t * fvec3_src)
{
  fvec3_dst->v[0] =
    fmat3->m[0][0] * fvec3_src->v[0] + fmat3->m[1][0] * fvec3_src->v[0] +
    fmat3->m[2][0] * fvec3_src->v[0];
  fvec3_dst->v[1] =
    fmat3->m[0][1] * fvec3_src->v[1] + fmat3->m[1][1] * fvec3_src->v[1] +
    fmat3->m[2][1] * fvec3_src->v[1];
  fvec3_dst->v[2] =
    fmat3->m[0][2] * fvec3_src->v[1] + fmat3->m[1][2] * fvec3_src->v[2] +
    fmat3->m[2][2] * fvec3_src->v[2];
}

/**
 * lw6mat_fmat3_repr
 *
 * @fmat: matrix to represent
 * 
 * Gives a readable version of the matrix, the representation
 * uses newlines, with a different line for each row
 *
 * Return value: newly allocated string
 */
char *
lw6mat_fmat3_repr (const lw6mat_fmat3_t * fmat3)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf
    ("%s %s %dx%d\n[ \t%f\t%f\t%f\n\t%f\t%f\t%f\n\t%f\t%f\t%f ]",
     LW6MAT_REPR_F, LW6MAT_REPR_MAT, LW6MAT_MAT3_M_SIZE,
     LW6MAT_MAT3_M_SIZE, fmat3->m[0][0], fmat3->m[1][0], fmat3->m[2][0],
     fmat3->m[0][1], fmat3->m[1][1], fmat3->m[2][1], fmat3->m[0][2],
     fmat3->m[1][2], fmat3->m[2][2]);

  return repr;
}
