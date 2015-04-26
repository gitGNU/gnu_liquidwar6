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
 * lw6mat_fvec4_zero
 *
 * @fvec4: the vector to initialize.
 *
 * Fills the vector with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_fvec4_zero (lw6mat_fvec4_t * fvec4)
{
  memset (fvec4, 0, sizeof (lw6mat_fvec4_t));
}

/**
 * lw6mat_fvec4_is_same
 *
 * @fvec4_a: 1st vector to compare
 * @fvec4_b: 2nd vector to compare
 *
 * Compares two vectors, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_fvec4_is_same (const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b)
{
  return (!memcmp ((void *) fvec4_a, (void *) fvec4_b, sizeof (lw6mat_fvec4_t)));
}

/**
 * lw6mat_fvec4_len_sq
 *
 * @fvec4: the vector to query.
 *
 * Returns the square of a vector length. To get the real length
 * one should then apply a square root but at this stage one has
 * at least an idea about vector length, and this information is
 * enough to compare them.
 *
 * Return value: sigma(coord*coord)
 */
float
lw6mat_fvec4_len_sq (const lw6mat_fvec4_t * fvec4)
{
  return fvec4->p.x * fvec4->p.x + fvec4->p.y * fvec4->p.y + fvec4->p.z * fvec4->p.z + fvec4->p.w * fvec4->p.w;
}

/**
 * lw6mat_fvec4_len_sq
 *
 * @fvec4: the vector to query.
 *
 * Returns the size/length of a vector, this is the distance
 * of the point from origin, not the number of elements.
 *
 * Return value: the length of the vector.
 */
float
lw6mat_fvec4_len (const lw6mat_fvec4_t * fvec4)
{
  return sqrt (lw6mat_fvec4_len_sq (fvec4));
}

/**
 * lw6mat_fvec4_normalize
 *
 * @sys_context: global system context
 * @fvec4: the vector to normalize.
 *
 * Normalizes a vector, that is, make its length be 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to normalize vector zero.
 */
int
lw6mat_fvec4_normalize (lw6sys_context_t * sys_context, lw6mat_fvec4_t * fvec4)
{
  float len = lw6mat_fvec4_len (fvec4);

  if (!lw6mat_is_similar_f (len, LW6MAT_F_0))
    {
      fvec4->p.x /= len;
      fvec4->p.y /= len;
      fvec4->p.z /= len;
      fvec4->p.w /= len;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to normalize vector zero"));

      return 0;
    }
}

/**
 * lw6mat_fvec4_homogeneous
 *
 * Homogeneouss a vector
 *
 * @sys_context: global system context
 * @fvec4: the vector to homogeneous.
 *
 * Transforms a vector into homegeneous coords, that is, scales it so
 * that its last member is 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to operate on vector zero.
 */
int
lw6mat_fvec4_homogeneous (lw6sys_context_t * sys_context, lw6mat_fvec4_t * fvec4)
{
  float f = fvec4->v[3];

  if (f != 0.0f)
    {
      fvec4->p.x /= f;
      fvec4->p.y /= f;
      fvec4->p.z /= f;
      fvec4->v[3] = LW6MAT_F_1;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to make homogeneous a vector with zero last member"));

      return 0;
    }
}

/**
 * lw6mat_fvec4_neg
 *
 * @fvec4: vector to modify
 *
 * Calcs the opposite vector, by making a negation on all its members
 *
 * Return value: none
 */
void
lw6mat_fvec4_neg (lw6mat_fvec4_t * fvec4)
{
  fvec4->p.x = -fvec4->p.x;
  fvec4->p.y = -fvec4->p.y;
  fvec4->p.z = -fvec4->p.z;
  fvec4->p.w = -fvec4->p.w;
}

/**
 * lw6mat_fvec4_add
 *
 * @fvec4: result vector
 * @fvec4_a: 1st vector to add
 * @fvec4_b: 2nd vector to add
 *
 * Adds two vectors.
 *
 * Return value: none
 */
void
lw6mat_fvec4_add (lw6mat_fvec4_t * fvec4, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b)
{
  fvec4->p.x = fvec4_a->p.x + fvec4_b->p.x;
  fvec4->p.y = fvec4_a->p.y + fvec4_b->p.y;
  fvec4->p.z = fvec4_a->p.z + fvec4_b->p.z;
  fvec4->p.w = fvec4_a->p.w + fvec4_b->p.w;
}

/**
 * lw6mat_fvec4_sub
 *
 * @fvec4: result vector
 * @fvec4_a: 1st vector
 * @fvec4_b: 2nd vector, will be substracted to 1st vector
 *
 * Substracts vector b from vector a.
 *
 * Return value: none
 */
void
lw6mat_fvec4_sub (lw6mat_fvec4_t * fvec4, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b)
{
  fvec4->p.x = fvec4_a->p.x - fvec4_b->p.x;
  fvec4->p.y = fvec4_a->p.y - fvec4_b->p.y;
  fvec4->p.z = fvec4_a->p.z - fvec4_b->p.z;
  fvec4->p.w = fvec4_a->p.w - fvec4_b->p.w;
}

/**
 * lw6mat_fvec4_dot
 *
 * @fvec4_a: 1st vector
 * @fvec4_b: 2nd vector
 *
 * Calculates the dot AKA scalar product of the two vectors.
 *
 * Return value: none
 */
float
lw6mat_fvec4_dot (const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b)
{
  return fvec4_a->p.x * fvec4_b->p.x + fvec4_a->p.y * fvec4_b->p.y + fvec4_a->p.z * fvec4_b->p.z + fvec4_a->p.w * fvec4_b->p.w;
}

/**
 * lw6mat_fvec4_cross
 *
 * @fvec3: result vector
 * @fvec4_a: 1st vector
 * @fvec4_b: 2nd vector
 *
 * Calculates the cross AKA vectorial product of the two vectors.
 * Since cross product only really makes sense in 3D, this function
 * will interpret the 4D vectors as 3D vectors only, ignoring
 * the last value.
 *
 * Return value: none
 */
void
lw6mat_fvec4_cross (lw6mat_fvec3_t * fvec3, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b)
{
  fvec3->p.x = fvec4_a->p.y * fvec4_b->p.z - fvec4_a->p.z * fvec4_b->p.y;
  fvec3->p.y = fvec4_a->p.z * fvec4_b->p.x - fvec4_a->p.x * fvec4_b->p.z;
  fvec3->p.z = fvec4_a->p.x * fvec4_b->p.y - fvec4_a->p.y * fvec4_b->p.x;
}

/**
 * lw6mat_fvec4_mul_scale
 *
 * @fvec4: vector to modify
 * @f: scale factor
 *
 * Scales the vector by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_fvec4_mul_scale (lw6mat_fvec4_t * fvec4, float f)
{
  fvec4->p.x *= f;
  fvec4->p.y *= f;
  fvec4->p.z *= f;
  fvec4->p.w *= f;
}

/**
 * lw6mat_fvec4_mul_fvec4
 *
 * @fmat4: result matrix
 * @fvec4_a: 1st row vector
 * @fvec4_b: 4nd column vector
 *
 * Multiplication of a row vector by a column vector to give a matrix.
 *
 * Return value: none
 */
void
lw6mat_fvec4_mul_fvec4 (lw6mat_fmat4_t * fmat4, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b)
{
  int i, j;

  for (i = 0; i < LW6MAT_VEC4_V_SIZE; ++i)
    {
      for (j = 0; j < LW6MAT_VEC4_V_SIZE; ++j)
	{
	  fmat4->m[i][j] = fvec4_a->v[i] * fvec4_b->v[j];
	}
    }
}

/**
 * lw6mat_fvec4_repr
 *
 * @sys_context: global system context
 * @fvec: vector to represent
 *
 * Gives a readable version of the vector
 *
 * Return value: newly allocated string
 */
char *
lw6mat_fvec4_repr (lw6sys_context_t * sys_context, const lw6mat_fvec4_t * fvec4)
{
  char *repr = NULL;

  repr =
    lw6sys_new_sprintf (sys_context, "%s %s %d [ %f %f %f %f ]", LW6MAT_REPR_F, LW6MAT_REPR_VEC, LW6MAT_VEC4_V_SIZE, fvec4->v[0], fvec4->v[1], fvec4->v[2],
			fvec4->v[3]);

  return repr;
}
