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
 * lw6mat_dvec2_zero
 *
 * @dvec2: the vector to initialize.
 *
 * Fills the vector with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_dvec2_zero (lw6mat_dvec2_t * dvec2)
{
  memset (dvec2, 0, sizeof (lw6mat_dvec2_t));
}

/**
 * lw6mat_dvec2_is_same
 *
 * @dvec2_a: 1st vector to compare
 * @dvec2_b: 2nd vector to compare
 *
 * Compares two vectors, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_dvec2_is_same (const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b)
{
  return (!memcmp ((void *) dvec2_a, (void *) dvec2_b, sizeof (lw6mat_dvec2_t)));
}

/**
 * lw6mat_dvec2_len_sq
 *
 * @dvec2: the vector to query.
 *
 * Returns the square of a vector length. To get the real length
 * one should then apply a square root but at this stage one has
 * at least an idea about vector length, and this information is
 * enough to compare them.
 *
 * Return value: sigma(coord*coord)
 */
double
lw6mat_dvec2_len_sq (const lw6mat_dvec2_t * dvec2)
{
  return dvec2->p.x * dvec2->p.x + dvec2->p.y * dvec2->p.y;
}

/**
 * lw6mat_dvec2_len_sq
 *
 * @dvec2: the vector to query.
 *
 * Returns the size/length of a vector, this is the distance
 * of the point from origin, not the number of elements.
 *
 * Return value: the length of the vector.
 */
double
lw6mat_dvec2_len (const lw6mat_dvec2_t * dvec2)
{
  return sqrt (lw6mat_dvec2_len_sq (dvec2));
}

/**
 * lw6mat_dvec2_normalize
 *
 * @sys_context: global system context
 * @dvec2: the vector to normalize.
 *
 * Normalizes a vector, that is, make its length be 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to normalize vector zero.
 */
int
lw6mat_dvec2_normalize (lw6sys_context_t * sys_context, lw6mat_dvec2_t * dvec2)
{
  double len = lw6mat_dvec2_len (dvec2);

  if (!lw6mat_is_similar_d (len, LW6MAT_D_0))
    {
      dvec2->p.x /= len;
      dvec2->p.y /= len;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to normalize vector zero"));

      return 0;
    }
}

/**
 * lw6mat_dvec2_homogeneous
 *
 * Homogeneouss a vector
 *
 * @sys_context: global system context
 * @dvec2: the vector to homogeneous.
 *
 * Transforms a vector into homegeneous coords, that is, scales it so
 * that its last member is 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to operate on vector zero.
 */
int
lw6mat_dvec2_homogeneous (lw6sys_context_t * sys_context, lw6mat_dvec2_t * dvec2)
{
  double d = dvec2->v[1];

  if (d != 0.0f)
    {
      dvec2->p.x /= d;
      dvec2->v[1] = LW6MAT_D_1;

      return 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to make homogeneous a vector with zero last member"));

      return 0;
    }
}

/**
 * lw6mat_dvec2_neg
 *
 * @dvec2: vector to modify
 *
 * Calcs the opposite vector, by making a negation on all its members
 *
 * Return value: none
 */
void
lw6mat_dvec2_neg (lw6mat_dvec2_t * dvec2)
{
  dvec2->p.x = -dvec2->p.x;
  dvec2->p.y = -dvec2->p.y;
}

/**
 * lw6mat_dvec2_add
 *
 * @dvec2: result vector
 * @dvec2_a: 1st vector to add
 * @dvec2_b: 2nd vector to add
 *
 * Adds two vectors.
 *
 * Return value: none
 */
void
lw6mat_dvec2_add (lw6mat_dvec2_t * dvec2, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b)
{
  dvec2->p.x = dvec2_a->p.x + dvec2_b->p.x;
  dvec2->p.y = dvec2_a->p.y + dvec2_b->p.y;
}

/**
 * lw6mat_dvec2_sub
 *
 * @dvec2: result vector
 * @dvec2_a: 1st vector
 * @dvec2_b: 2nd vector, will be substracted to 1st vector
 *
 * Substracts vector b from vector a.
 *
 * Return value: none
 */
void
lw6mat_dvec2_sub (lw6mat_dvec2_t * dvec2, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b)
{
  dvec2->p.x = dvec2_a->p.x - dvec2_b->p.x;
  dvec2->p.y = dvec2_a->p.y - dvec2_b->p.y;
}

/**
 * lw6mat_dvec2_dot
 *
 * @dvec2_a: 1st vector
 * @dvec2_b: 2nd vector
 *
 * Calculates the dot AKA scalar product of the two vectors.
 *
 * Return value: none
 */
double
lw6mat_dvec2_dot (const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b)
{
  return dvec2_a->p.x * dvec2_b->p.x + dvec2_a->p.y * dvec2_b->p.y;
}

/**
 * lw6mat_dvec2_cross
 *
 * @dvec3: result vector
 * @dvec2_a: 1st vector
 * @dvec2_b: 2nd vector
 *
 * Calculates the cross AKA vectorial product of the two vectors.
 * Since cross product only really makes sense in 3D, this function
 * will interpret the 2D vectors as 3D vectors with z set t zero,
 * that is, a vector in the xy plane.
 *
 * Return value: none
 */
void
lw6mat_dvec2_cross (lw6mat_dvec3_t * dvec3, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b)
{
  dvec3->p.x = LW6MAT_F_0;
  dvec3->p.y = LW6MAT_F_0;
  dvec3->p.z = dvec2_a->p.x * dvec2_b->p.y - dvec2_a->p.y * dvec2_b->p.x;
}

/**
 * lw6mat_dvec2_mul_scale
 *
 * @dvec2: vector to modify
 * @f: scale factor
 *
 * Scales the vector by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_dvec2_mul_scale (lw6mat_dvec2_t * dvec2, double f)
{
  dvec2->p.x *= f;
  dvec2->p.y *= f;
}

/**
 * lw6mat_dvec2_mul_dvec2
 *
 * @dmat2: result matrix
 * @dvec2_a: 1st row vector
 * @dvec2_b: 2nd column vector
 *
 * Multiplication of a row vector by a column vector to give a matrix.
 *
 * Return value: none
 */
void
lw6mat_dvec2_mul_dvec2 (lw6mat_dmat2_t * dmat2, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b)
{
  int i, j;

  for (i = 0; i < LW6MAT_VEC2_V_SIZE; ++i)
    {
      for (j = 0; j < LW6MAT_VEC2_V_SIZE; ++j)
	{
	  dmat2->m[i][j] = dvec2_a->v[i] * dvec2_b->v[j];
	}
    }
}

/**
 * lw6mat_dvec2_repr
 *
 * @sys_context: global system context
 * @dvec: vector to represent
 *
 * Gives a readable version of the vector
 *
 * Return value: newly allocated string
 */
char *
lw6mat_dvec2_repr (lw6sys_context_t * sys_context, const lw6mat_dvec2_t * dvec2)
{
  char *repr = NULL;

  repr = lw6sys_new_sprintf (sys_context, "%s %s %d [ %f %f ]", LW6MAT_REPR_D, LW6MAT_REPR_VEC, LW6MAT_VEC2_V_SIZE, dvec2->v[0], dvec2->v[1]);

  return repr;
}
