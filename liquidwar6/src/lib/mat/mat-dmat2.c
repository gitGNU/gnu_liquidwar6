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
 * lw6mat_dmat2_zero
 *
 * @dmat2: the matrix to initialize.
 *
 * Fills the matrix with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_dmat2_zero (lw6mat_dmat2_t * dmat2)
{
  memset (dmat2, 0, sizeof (lw6mat_dmat2_t));
}

/**
 * lw6mat_dmat2_id
 *
 * @dmat2: the matrix to initialize.
 *
 * Loads the matrix with the identity matrix, that is, zero everywhere
 * but one on the main diag.
 *
 * Return value: none.
 */
void
lw6mat_dmat2_id (lw6mat_dmat2_t * dmat2)
{
  dmat2->m[0][0] = LW6MAT_D_1;
  dmat2->m[0][1] = LW6MAT_D_0;
  dmat2->m[1][0] = LW6MAT_D_0;
  dmat2->m[1][1] = LW6MAT_D_1;
}

/**
 * lw6mat_dmat2_is_same
 *
 * @dmat2_a: 1st matrix to compare
 * @dmat2_b: 2nd matrix to compare
 *
 * Compares two matrix, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_dmat2_is_same (const lw6mat_dmat2_t * dmat2_a,
		      const lw6mat_dmat2_t * dmat2_b)
{
  return (!memcmp
	  ((void *) dmat2_a, (void *) dmat2_b, sizeof (lw6mat_dmat2_t)));
}

/**
 * lw6mat_dmat2_trans
 *
 * @dmat2: the matrix to transpose
 *
 * Transposes the matrix, that is, inverts rows and columns.
 *
 * Return value: none.
 */
void
lw6mat_dmat2_trans (lw6mat_dmat2_t * dmat2)
{
  double tmp;

  tmp = dmat2->m[1][0];
  dmat2->m[1][0] = dmat2->m[0][1];
  dmat2->m[0][1] = tmp;
}

/**
 * lw6mat_dmat2_det
 *
 * @dmat2: the matrix used to calculate the determinant
 *
 * Calulates the determinant of the matrix.
 *
 * Return value: the determinant.
 */
double
lw6mat_dmat2_det (const lw6mat_dmat2_t * dmat2)
{
  /*
   * Wooo I'm so lazy, got this one from :
   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/twoD/
   */
  return dmat2->m[0][0] * dmat2->m[1][1] - dmat2->m[0][1] * dmat2->m[1][0];
}

/**
 * lw6mat_dmat2_scale
 *
 * @dmat2: matrix to modify
 * @f: scale factor
 *
 * Scales the matrix by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_dmat2_scale (lw6mat_dmat2_t * dmat2, double f)
{
  int i;

  for (i = 0; i < LW6MAT_MAT2_V_SIZE_X_SIZE; ++i)
    {
      dmat2->v[i] *= f;
    }
}

/**
 * lw6mat_dmat2_inv
 *
 * @dmat2_dst: the matrix inverted
 * @dmat2_src: the matrix to invert
 *
 * Inverts a matrix. Probably not the fastest implementation, but
 * should work in all cases. Use hardware accelerated API such as
 * OpenGL on dedicated hardware if you want power.
 *
 * Return value: 1 if inverted, 0 if error, typically if determinant was 0, matrix
 * can not be inverted.
 */
int
lw6mat_dmat2_inv (lw6mat_dmat2_t * dmat2_dst,
		  const lw6mat_dmat2_t * dmat2_src)
{
  /*
   * In case src and dst or the same, recursively call this
   * with a tmp pivot to avoid wrecking source while writing
   * destination.
   */
  if (dmat2_dst == dmat2_src)
    {
      lw6mat_dmat2_t dmat2_tmp = *dmat2_src;

      return lw6mat_dmat2_inv (dmat2_dst, &dmat2_tmp);
    }
  else
    {
      double det = lw6mat_dmat2_det (dmat2_src);

      if (det)
	{
	  /*
	   * Wooo I'm so lazy, got this one from :
	   * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/twoD/
	   */
	  dmat2_dst->m[0][0] = dmat2_src->m[1][1] / det;
	  dmat2_dst->m[0][1] = -dmat2_src->m[0][1] / det;
	  dmat2_dst->m[1][0] = -dmat2_src->m[1][0] / det;
	  dmat2_dst->m[1][1] = dmat2_src->m[0][0] / det;

	  return 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_
		      ("trying to invert non-invertible dmat2 matrix, determinant is 0"));
	  /*
	   * Putting identity in result, yes it's not optimal it's CPU
	   * waste but in case matrix was not invertible, it the
	   * problem will really show afterwards, and the result can
	   * always be inverted again.
	   */
	  lw6mat_dmat2_id (dmat2_dst);

	  return 0;
	}
    }
}

/**
 * lw6mat_dmat2_mul_dmat2
 *
 * @dmat2: the result matrix
 * @dmat2_a: the 1st matrix to multiply, on the left
 * @dmat2_b: the 2nd matrix to multiply, on the right
 *
 * Classic matrix multiplication.
 *
 * Return value: none.
 */
void
lw6mat_dmat2_mul_dmat2 (lw6mat_dmat2_t * dmat2,
			const lw6mat_dmat2_t * dmat2_a,
			const lw6mat_dmat2_t * dmat2_b)
{
  int i, j;

  for (i = 0; i < LW6MAT_MAT2_M_SIZE; ++i)
    {
      for (j = 0; j < LW6MAT_MAT2_M_SIZE; ++j)
	{
	  dmat2->m[i][j] = dmat2_a->m[0][j] * dmat2_b->m[i][0]
	    + dmat2_a->m[1][j] * dmat2_b->m[i][1];
	}
    }
}

/**
 * lw6mat_dmat2_repr
 *
 * @dmat: matrix to represent
 * 
 * Gives a readable version of the matrix, the representation
 * uses newlines, with a different line for each row
 *
 * Return value: newly allocated string
 */
char *
lw6mat_dmat2_repr (const lw6mat_dmat2_t * dmat2)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf ("%s %s %dx%d\n[ \t%f\t%f\n\t%f\t%f ]", LW6MAT_REPR_D,
			LW6MAT_REPR_MAT, LW6MAT_MAT2_M_SIZE,
			LW6MAT_MAT2_M_SIZE, dmat2->m[0][0], dmat2->m[1][0],
			dmat2->m[0][1], dmat2->m[1][1]);

  return repr;
}
