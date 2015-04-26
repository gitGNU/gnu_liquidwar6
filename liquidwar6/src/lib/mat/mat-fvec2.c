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

#include <math.h>

/**
 * lw6mat_fvec2_zero
 *
 * @fvec2: the vector to initialize.
 *
 * Fills the vector with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_fvec2_zero (lw6mat_fvec2_t * fvec2)
{
  memset (fvec2, 0, sizeof (lw6mat_fvec2_t));
}

/**
 * lw6mat_fvec2_is_same
 *
 * @fvec2_a: 1st vector to compare
 * @fvec2_b: 2nd vector to compare
 *
 * Compares two vectors, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_fvec2_is_same (const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b)
{
  return (!memcmp ((void *) fvec2_a, (void *) fvec2_b, sizeof (lw6mat_fvec2_t)));
}

/**
 * lw6mat_fvec2_len_sq
 *
 * @fvec2: the vector to query.
 *
 * Returns the square of a vector length. To get the real length
 * one should then apply a square root but at this stage one has
 * at least an idea about vector length, and this information is
 * enough to compare them.
 *
 * Return value: sigma(coord*coord)
 */
float
lw6mat_fvec2_len_sq (const lw6mat_fvec2_t * fvec2)
{
  return fvec2->p.x * fvec2->p.x + fvec2->p.y * fvec2->p.y;
}

/**
 * lw6mat_fvec2_len_sq
 *
 * @fvec2: the vector to query.
 *
 * Returns the size/length of a vector, this is the distance
 * of the point from origin, not the number of elements.
 *
 * Return value: the length of the vector.
 */
float
lw6mat_fvec2_len (const lw6mat_fvec2_t * fvec2)
{
  return sqrt (lw6mat_fvec2_len_sq (fvec2));
}

/**
 * lw6mat_fvec2_normalize
 *
 * @sys_context: global system context
 * @fvec2: the vector to normalize.
 *
 * Normalizes a vector, that is, make its length be 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to normalize vector zero.
 */
int
lw6mat_fvec2_normalize (lw6sys_context_t * sys_context, lw6mat_fvec2_t * fvec2)
{
  float len = lw6mat_fvec2_len (fvec2);

  if (!lw6mat_is_similar_f (len, LW6MAT_F_0))
    {
      fvec2->p.x /= len;
      fvec2->p.y /= len;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to normalize vector zero"));

      return 0;
    }
}

/**
 * lw6mat_fvec2_homogeneous
 *
 * Homogeneouss a vector
 *
 * @sys_context: global system context
 * @fvec2: the vector to homogeneous.
 *
 * Transforms a vector into homegeneous coords, that is, scales it so
 * that its last member is 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to operate on vector zero.
 */
int
lw6mat_fvec2_homogeneous (lw6sys_context_t * sys_context, lw6mat_fvec2_t * fvec2)
{
  float f = fvec2->v[1];

  if (f != 0.0f)
    {
      fvec2->p.x /= f;
      fvec2->v[1] = LW6MAT_F_1;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to make homogeneous a vector with zero last member"));

      return 0;
    }
}

/**
 * lw6mat_fvec2_neg
 *
 * @fvec2: vector to modify
 *
 * Calcs the opposite vector, by making a negation on all its members
 *
 * Return value: none
 */
void
lw6mat_fvec2_neg (lw6mat_fvec2_t * fvec2)
{
  fvec2->p.x = -fvec2->p.x;
  fvec2->p.y = -fvec2->p.y;
}

/**
 * lw6mat_fvec2_add
 *
 * @fvec2: result vector
 * @fvec2_a: 1st vector to add
 * @fvec2_b: 2nd vector to add
 *
 * Adds two vectors.
 *
 * Return value: none
 */
void
lw6mat_fvec2_add (lw6mat_fvec2_t * fvec2, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b)
{
  fvec2->p.x = fvec2_a->p.x + fvec2_b->p.x;
  fvec2->p.y = fvec2_a->p.y + fvec2_b->p.y;
}

/**
 * lw6mat_fvec2_sub
 *
 * @fvec2: result vector
 * @fvec2_a: 1st vector
 * @fvec2_b: 2nd vector, will be substracted to 1st vector
 *
 * Substracts vector b from vector a.
 *
 * Return value: none
 */
void
lw6mat_fvec2_sub (lw6mat_fvec2_t * fvec2, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b)
{
  fvec2->p.x = fvec2_a->p.x - fvec2_b->p.x;
  fvec2->p.y = fvec2_a->p.y - fvec2_b->p.y;
}

/**
 * lw6mat_fvec2_dot
 *
 * @fvec2_a: 1st vector
 * @fvec2_b: 2nd vector
 *
 * Calculates the dot AKA scalar product of the two vectors.
 *
 * Return value: none
 */
float
lw6mat_fvec2_dot (const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b)
{
  return fvec2_a->p.x * fvec2_b->p.x + fvec2_a->p.y * fvec2_b->p.y;
}

/**
 * lw6mat_fvec2_cross
 *
 * @fvec3: result vector
 * @fvec2_a: 1st vector
 * @fvec2_b: 2nd vector
 *
 * Calculates the cross AKA vectorial product of the two vectors.
 * Since cross product only really makes sense in 3D, this function
 * will interpret the 2D vectors as 3D vectors with z set t zero,
 * that is, a vector in the xy plane.
 *
 * Return value: none
 */
void
lw6mat_fvec2_cross (lw6mat_fvec3_t * fvec3, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b)
{
  fvec3->p.x = LW6MAT_F_0;
  fvec3->p.y = LW6MAT_F_0;
  fvec3->p.z = fvec2_a->p.x * fvec2_b->p.y - fvec2_a->p.y * fvec2_b->p.x;
}

/**
 * lw6mat_fvec2_mul_scale
 *
 * @fvec2: vector to modify
 * @f: scale factor
 *
 * Scales the vector by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_fvec2_mul_scale (lw6mat_fvec2_t * fvec2, float f)
{
  fvec2->p.x *= f;
  fvec2->p.y *= f;
}

/**
 * lw6mat_fvec2_mul_fvec2
 *
 * @fmat2: result matrix
 * @fvec2_a: 1st row vector
 * @fvec2_b: 2nd column vector
 *
 * Multiplication of a row vector by a column vector to give a matrix.
 *
 * Return value: none
 */
void
lw6mat_fvec2_mul_fvec2 (lw6mat_fmat2_t * fmat2, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b)
{
  int i, j;

  for (i = 0; i < LW6MAT_VEC2_V_SIZE; ++i)
    {
      for (j = 0; j < LW6MAT_VEC2_V_SIZE; ++j)
	{
	  fmat2->m[i][j] = fvec2_a->v[i] * fvec2_b->v[j];
	}
    }
}

/**
 * lw6mat_fvec2_repr
 *
 * @sys_context: global system context
 * @fvec: vector to represent
 *
 * Gives a readable version of the vector
 *
 * Return value: newly allocated string
 */
char *
lw6mat_fvec2_repr (lw6sys_context_t * sys_context, const lw6mat_fvec2_t * fvec2)
{
  char *repr = NULL;

  repr = lw6sys_new_sprintf (sys_context, "%s %s %d [ %f %f ]", LW6MAT_REPR_F, LW6MAT_REPR_VEC, LW6MAT_VEC2_V_SIZE, fvec2->v[0], fvec2->v[1]);

  return repr;
}
