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
  lw6mat_fmat3_zero (fmat3);
  fmat3->m[0][0] = LW6MAT_F_1;
  fmat3->m[1][1] = LW6MAT_F_1;
  fmat3->m[2][2] = LW6MAT_F_1;
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
  int i, j, tmp;

  for (i = 1; i < 3; ++i)
    {
      for (j = 0; j < i - 1; ++j)
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
  fmat3->m[0][0] *= f;
  fmat3->m[0][1] *= f;
  fmat3->m[0][2] *= f;
  fmat3->m[1][0] *= f;
  fmat3->m[1][1] *= f;
  fmat3->m[1][2] *= f;
  fmat3->m[2][0] *= f;
  fmat3->m[2][1] *= f;
  fmat3->m[2][2] *= f;
}

/**
 * lw6mat_fmat3_inv
 *
 * @fmat3: the matrix to invert
 *
 * Inverts a matrix. Probably not the fastest implementation, but
 * should work in all cases. Use hardware accelerated API such as
 * OpenGL on dedicated hardware if you want power.
 *
 * Return value: 1 if inverted, 0 if error, typically if determinant was 0, matrix
 * can not be inverted.
 */
int
lw6mat_fmat3_inv (lw6mat_fmat3_t * fmat3)
{
  float det = lw6mat_fmat3_det (fmat3);

  if (det)
    {
      lw6mat_fmat3_t orig;

      orig = (*fmat3);

      /*
       * Wooo I'm so lazy, got this one from :
       * http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/
       */
      fmat3->m[0][0] =
	(orig.m[1][1] * orig.m[2][2] - orig.m[1][2] * orig.m[2][1]) / det;
      fmat3->m[0][1] =
	(orig.m[0][2] * orig.m[2][1] - orig.m[0][1] * orig.m[2][2]) / det;
      fmat3->m[0][2] =
	(orig.m[0][1] * orig.m[1][2] - orig.m[0][2] * orig.m[1][1]) / det;
      fmat3->m[1][0] =
	(orig.m[1][2] * orig.m[2][0] - orig.m[1][0] * orig.m[2][2]) / det;
      fmat3->m[1][1] =
	(orig.m[0][0] * orig.m[2][2] - orig.m[0][2] * orig.m[2][0]) / det;
      fmat3->m[1][2] =
	(orig.m[0][2] * orig.m[1][0] - orig.m[0][0] * orig.m[1][2]) / det;
      fmat3->m[2][0] =
	(orig.m[1][0] * orig.m[2][1] - orig.m[1][1] * orig.m[2][0]) / det;
      fmat3->m[2][1] =
	(orig.m[0][1] * orig.m[2][0] - orig.m[0][0] * orig.m[2][1]) / det;
      fmat3->m[2][2] =
	(orig.m[0][0] * orig.m[1][1] - orig.m[0][1] * orig.m[1][0]) / det;

      return 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_INFO,
		  _x_
		  ("trying to invert non-invertible fmat3 matrix, determinant is 0"));
      return 0;
    }
}
