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
 * lw6mat_fmat2_id
 *
 * @fmat2: the matrix to initialize.
 *
 * Loads the matrix with the identity matrix, that is, zero everywhere
 * but one on the main diag.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_id (lw6mat_fmat2_t * fmat2)
{
  fmat2->m[0][0] = LW6MAT_F_1;
  fmat2->m[0][1] = LW6MAT_F_0;
  fmat2->m[1][0] = LW6MAT_F_0;
  fmat2->m[1][1] = LW6MAT_F_1;
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
lw6mat_fmat2_is_same (const lw6mat_fmat2_t * fmat2_a,
		      const lw6mat_fmat2_t * fmat2_b)
{
  return (!memcmp
	  ((void *) fmat2_a, (void *) fmat2_b, sizeof (lw6mat_fmat2_t)));
}

/**
 * lw6mat_fmat2_trans
 *
 * @fmat2: the matrix to transpose
 *
 * Transposes the matrix, that is, inverts rows and columns.
 *
 * Return value: none.
 */
void
lw6mat_fmat2_trans (lw6mat_fmat2_t * fmat2)
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
 * lw6mat_fmat2_scale
 *
 * @fmat2: matrix to modify
 * @f: scale factor
 *
 * Scales the matrix by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_fmat2_scale (lw6mat_fmat2_t * fmat2, float f)
{
  int i;

  for (i = 0; i < LW6MAT_MAT2_V_SIZE_4; ++i)
    {
      fmat2->v[i] *= f;
    }
}

/**
 * lw6mat_fmat2_inv
 *
 * @fmat2: the matrix to invert
 *
 * Inverts a matrix. Probably not the fastest implementation, but
 * should work in all cases. Use hardware accelerated API such as
 * OpenGL on dedicated hardware if you want power.
 *
 * Return value: 1 if inverted, 0 if error, typically if determinant was 0, matrix
 * can not be inverted.
 */
int
lw6mat_fmat2_inv (lw6mat_fmat2_t * fmat2)
{
  float det = lw6mat_fmat2_det (fmat2);

  if (det)
    {
      lw6mat_fmat2_t orig;

      orig = (*fmat2);

      /*
       * Wooo I'm so lazy, got this one from :
       * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/twoD/
       */
      fmat2->m[0][0] = orig.m[1][1] / det;
      fmat2->m[0][1] = -orig.m[0][1] / det;
      fmat2->m[1][0] = -orig.m[1][0] / det;
      fmat2->m[1][1] = orig.m[0][0] / det;

      return 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_INFO,
		  _x_
		  ("trying to invert non-invertible fmat2 matrix, determinant is 0"));
      return 0;
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
lw6mat_fmat2_mul_fmat2 (lw6mat_fmat2_t * fmat2,
			const lw6mat_fmat2_t * fmat2_a,
			const lw6mat_fmat2_t * fmat2_b)
{
  int i, j;

  for (i = 0; i < LW6MAT_MAT2_M_SIZE_2; ++i)
    {
      for (j = 0; j < LW6MAT_MAT2_M_SIZE_2; ++j)
	{
	  fmat2->m[i][j] = fmat2_a->m[0][j] * fmat2_b->m[i][0]
	    + fmat2_a->m[1][j] * fmat2_b->m[i][1];
	}
    }
}

/**
 * lw6mat_fmat2_repr
 *
 * @fmat: matrix to represent
 * 
 * Gives a readable version of the matrix, the representation
 * uses newlines, with a different line for each row
 *
 * Return value: newly allocated string
 */
char *
lw6mat_fmat2_repr (const lw6mat_fmat2_t * fmat2)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf ("%s %s %dx%d\n[ \t%f\t%f\n\t%f\t%f ]", LW6MAT_REPR_F,
			LW6MAT_REPR_MAT, LW6MAT_MAT2_M_SIZE_2,
			LW6MAT_MAT2_M_SIZE_2, fmat2->m[0][0], fmat2->m[1][0],
			fmat2->m[0][1], fmat2->m[1][1]);

  return repr;
}
