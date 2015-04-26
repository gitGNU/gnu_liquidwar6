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
 * lw6mat_dvec3_zero
 *
 * @dvec3: the vector to initialize.
 *
 * Fills the vector with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_dvec3_zero (lw6mat_dvec3_t * dvec3)
{
  memset (dvec3, 0, sizeof (lw6mat_dvec3_t));
}

/**
 * lw6mat_dvec3_is_same
 *
 * @dvec3_a: 1st vector to compare
 * @dvec3_b: 2nd vector to compare
 *
 * Compares two vectors, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_dvec3_is_same (const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b)
{
  return (!memcmp ((void *) dvec3_a, (void *) dvec3_b, sizeof (lw6mat_dvec3_t)));
}

/**
 * lw6mat_dvec3_len_sq
 *
 * @dvec3: the vector to query.
 *
 * Returns the square of a vector length. To get the real length
 * one should then apply a square root but at this stage one has
 * at least an idea about vector length, and this information is
 * enough to compare them.
 *
 * Return value: sigma(coord*coord)
 */
double
lw6mat_dvec3_len_sq (const lw6mat_dvec3_t * dvec3)
{
  return dvec3->p.x * dvec3->p.x + dvec3->p.y * dvec3->p.y + dvec3->p.z * dvec3->p.z;
}

/**
 * lw6mat_dvec3_len_sq
 *
 * @dvec3: the vector to query.
 *
 * Returns the size/length of a vector, this is the distance
 * of the point from origin, not the number of elements.
 *
 * Return value: the length of the vector.
 */
double
lw6mat_dvec3_len (const lw6mat_dvec3_t * dvec3)
{
  return sqrt (lw6mat_dvec3_len_sq (dvec3));
}

/**
 * lw6mat_dvec3_normalize
 *
 * @sys_context: global system context
 * @dvec3: the vector to normalize.
 *
 * Normalizes a vector, that is, make its length be 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to normalize vector zero.
 */
int
lw6mat_dvec3_normalize (lw6sys_context_t * sys_context, lw6mat_dvec3_t * dvec3)
{
  double len = lw6mat_dvec3_len (dvec3);

  if (!lw6mat_is_similar_d (len, LW6MAT_D_0))
    {
      dvec3->p.x /= len;
      dvec3->p.y /= len;
      dvec3->p.z /= len;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to normalize vector zero"));

      return 0;
    }
}

/**
 * lw6mat_dvec3_homogeneous
 *
 * Homogeneouss a vector
 *
 * @sys_context: global system context
 * @dvec3: the vector to homogeneous.
 *
 * Transforms a vector into homegeneous coords, that is, scales it so
 * that its last member is 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to operate on vector zero.
 */
int
lw6mat_dvec3_homogeneous (lw6sys_context_t * sys_context, lw6mat_dvec3_t * dvec3)
{
  double d = dvec3->v[2];

  if (d != 0.0f)
    {
      dvec3->p.x /= d;
      dvec3->p.y /= d;
      dvec3->v[2] = LW6MAT_D_1;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to make homogeneous a vector with zero last member"));

      return 0;
    }
}

/**
 * lw6mat_dvec3_neg
 *
 * @dvec3: vector to modify
 *
 * Calcs the opposite vector, by making a negation on all its members
 *
 * Return value: none
 */
void
lw6mat_dvec3_neg (lw6mat_dvec3_t * dvec3)
{
  dvec3->p.x = -dvec3->p.x;
  dvec3->p.y = -dvec3->p.y;
  dvec3->p.z = -dvec3->p.z;
}

/**
 * lw6mat_dvec3_add
 *
 * @dvec3: result vector
 * @dvec3_a: 1st vector to add
 * @dvec3_b: 2nd vector to add
 *
 * Adds two vectors.
 *
 * Return value: none
 */
void
lw6mat_dvec3_add (lw6mat_dvec3_t * dvec3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b)
{
  dvec3->p.x = dvec3_a->p.x + dvec3_b->p.x;
  dvec3->p.y = dvec3_a->p.y + dvec3_b->p.y;
  dvec3->p.z = dvec3_a->p.z + dvec3_b->p.z;
}

/**
 * lw6mat_dvec3_sub
 *
 * @dvec3: result vector
 * @dvec3_a: 1st vector
 * @dvec3_b: 2nd vector, will be substracted to 1st vector
 *
 * Substracts vector b from vector a.
 *
 * Return value: none
 */
void
lw6mat_dvec3_sub (lw6mat_dvec3_t * dvec3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b)
{
  dvec3->p.x = dvec3_a->p.x - dvec3_b->p.x;
  dvec3->p.y = dvec3_a->p.y - dvec3_b->p.y;
  dvec3->p.z = dvec3_a->p.z - dvec3_b->p.z;
}

/**
 * lw6mat_dvec3_dot
 *
 * @dvec3_a: 1st vector
 * @dvec3_b: 2nd vector
 *
 * Calculates the dot AKA scalar product of the two vectors.
 *
 * Return value: none
 */
double
lw6mat_dvec3_dot (const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b)
{
  return dvec3_a->p.x * dvec3_b->p.x + dvec3_a->p.y * dvec3_b->p.y + dvec3_a->p.z * dvec3_b->p.z;
}

/**
 * lw6mat_dvec3_cross
 *
 * @dvec3: result vector
 * @dvec3_a: 1st vector
 * @dvec3_b: 2nd vector
 *
 * Calculates the cross AKA vectorial product of the two vectors.
 *
 * Return value: none
 */
void
lw6mat_dvec3_cross (lw6mat_dvec3_t * dvec3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b)
{
  dvec3->p.x = dvec3_a->p.y * dvec3_b->p.z - dvec3_a->p.z * dvec3_b->p.y;
  dvec3->p.y = dvec3_a->p.z * dvec3_b->p.x - dvec3_a->p.x * dvec3_b->p.z;
  dvec3->p.z = dvec3_a->p.x * dvec3_b->p.y - dvec3_a->p.y * dvec3_b->p.x;
}

/**
 * lw6mat_dvec3_mul_scale
 *
 * @dvec3: vector to modify
 * @f: scale factor
 *
 * Scales the vector by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_dvec3_mul_scale (lw6mat_dvec3_t * dvec3, double f)
{
  dvec3->p.x *= f;
  dvec3->p.y *= f;
  dvec3->p.z *= f;
}

/**
 * lw6mat_dvec3_mul_dvec3
 *
 * @dmat3: result matrix
 * @dvec3_a: 1st row vector
 * @dvec3_b: 3nd column vector
 *
 * Multiplication of a row vector by a column vector to give a matrix.
 *
 * Return value: none
 */
void
lw6mat_dvec3_mul_dvec3 (lw6mat_dmat3_t * dmat3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b)
{
  int i, j;

  for (i = 0; i < LW6MAT_VEC3_V_SIZE; ++i)
    {
      for (j = 0; j < LW6MAT_VEC3_V_SIZE; ++j)
	{
	  dmat3->m[i][j] = dvec3_a->v[i] * dvec3_b->v[j];
	}
    }
}

/**
 * lw6mat_dvec3_repr
 *
 * @sys_context: global system context
 * @dvec: vector to represent
 *
 * Gives a readable version of the vector
 *
 * Return value: newly allocated string
 */
char *
lw6mat_dvec3_repr (lw6sys_context_t * sys_context, const lw6mat_dvec3_t * dvec3)
{
  char *repr = NULL;

  repr = lw6sys_new_sprintf (sys_context, "%s %s %d [ %f %f %f ]", LW6MAT_REPR_D, LW6MAT_REPR_VEC, LW6MAT_VEC3_V_SIZE, dvec3->v[0], dvec3->v[1], dvec3->v[2]);

  return repr;
}
