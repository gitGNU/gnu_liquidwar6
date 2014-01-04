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

#include <math.h>

/**
 * lw6mat_fvec3_zero
 *
 * @fvec3: the vector to initialize.
 *
 * Fills the vector with zeros, regardless of what was there before.
 * Internally, does a memset the only advantage is that this function
 * should use the right sizeof and therefore avoids typo errors.
 *
 * Return value: none.
 */
void
lw6mat_fvec3_zero (lw6mat_fvec3_t * fvec3)
{
  memset (fvec3, 0, sizeof (lw6mat_fvec3_t));
}

/**
 * lw6mat_fvec3_is_same
 *
 * @fvec3_a: 1st vector to compare
 * @fvec3_b: 2nd vector to compare
 *
 * Compares two vectors, returns true if they are equal.
 *
 * Return value: 1 if equal, 0 if different.
 */
int
lw6mat_fvec3_is_same (const lw6mat_fvec3_t * fvec3_a,
		      const lw6mat_fvec3_t * fvec3_b)
{
  return (!memcmp
	  ((void *) fvec3_a, (void *) fvec3_b, sizeof (lw6mat_fvec3_t)));
}

/**
 * lw6mat_fvec3_len_sq
 *
 * @fvec3: the vector to query.
 *
 * Returns the square of a vector length. To get the real length
 * one should then apply a square root but at this stage one has
 * at least an idea about vector length, and this information is
 * enough to compare them.
 *
 * Return value: sigma(coord*coord)
 */
float
lw6mat_fvec3_len_sq (const lw6mat_fvec3_t * fvec3)
{
  return fvec3->p.x * fvec3->p.x + fvec3->p.y * fvec3->p.y +
    fvec3->p.z * fvec3->p.z;
}

/**
 * lw6mat_fvec3_len_sq
 *
 * @fvec3: the vector to query.
 *
 * Returns the size/length of a vector, this is the distance
 * of the point from origin, not the number of elements.
 *
 * Return value: the length of the vector.
 */
float
lw6mat_fvec3_len (const lw6mat_fvec3_t * fvec3)
{
  return sqrt (lw6mat_fvec3_len_sq (fvec3));
}

/**
 * Normalizes a vector
 *
 * @fvec3: the vector to normalize.
 *
 * Normalizes a vector, that is, make its length be 1.
 *
 * Return value: 1 if OK, 0 if error, such as trying to normalize vector zero.
 */
int
lw6mat_fvec3_norm (lw6mat_fvec3_t * fvec3)
{
  float len = lw6mat_fvec3_len (fvec3);

  if (len)
    {
      fvec3->p.x /= len;
      fvec3->p.y /= len;
      fvec3->p.z /= len;

      return 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("trying to normalize vector zero"));

      return 0;
    }
}

/**
 * lw6mat_fvec3_neg
 *
 * @fvec3: vector to modify
 *
 * Calcs the opposite vector, by making a negation on all its members
 *
 * Return value: none
 */
void
lw6mat_fvec3_neg (lw6mat_fvec3_t * fvec3)
{
  fvec3->p.x = -fvec3->p.x;
  fvec3->p.y = -fvec3->p.y;
  fvec3->p.z = -fvec3->p.z;
}

/**
 * lw6mat_fvec3_add
 *
 * @fvec3: result vector
 * @fvec3_a: 1st vector to add
 * @fvec3_b: 2nd vector to add
 *
 * Adds two vectors.
 *
 * Return value: none
 */
void
lw6mat_fvec3_add (lw6mat_fvec3_t * fvec3, const lw6mat_fvec3_t * fvec3_a,
		  const lw6mat_fvec3_t * fvec3_b)
{
  fvec3->p.x = fvec3_a->p.x + fvec3_b->p.x;
  fvec3->p.y = fvec3_a->p.y + fvec3_b->p.y;
  fvec3->p.z = fvec3_a->p.z + fvec3_b->p.z;
}

/**
 * lw6mat_fvec3_sub
 *
 * @fvec3: result vector
 * @fvec3_a: 1st vector
 * @fvec3_b: 2nd vector, will be substracted to 1st vector
 *
 * Substracts vector b from vector a.
 *
 * Return value: none
 */
void
lw6mat_fvec3_sub (lw6mat_fvec3_t * fvec3, const lw6mat_fvec3_t * fvec3_a,
		  const lw6mat_fvec3_t * fvec3_b)
{
  fvec3->p.x = fvec3_a->p.x - fvec3_b->p.x;
  fvec3->p.y = fvec3_a->p.y - fvec3_b->p.y;
  fvec3->p.z = fvec3_a->p.z - fvec3_b->p.z;
}

/**
 * lw6mat_fvec3_dot
 *
 * @fvec3_a: 1st vector
 * @fvec3_b: 2nd vector
 *
 * Calculates the dot AKA scalar product of the two vectors.
 *
 * Return value: none
 */
float
lw6mat_fvec3_dot (const lw6mat_fvec3_t * fvec3_a,
		  const lw6mat_fvec3_t * fvec3_b)
{
  return fvec3_a->p.x * fvec3_b->p.x + fvec3_a->p.y * fvec3_b->p.y +
    fvec3_a->p.z * fvec3_b->p.z;
}

/**
 * lw6mat_fvec3_cross
 *
 * @fvec3: result vector
 * @fvec3_a: 1st vector
 * @fvec3_b: 2nd vector
 *
 * Calculates the cross AKA vectorial product of the two vectors.
 *
 * Return value: none
 */
void
lw6mat_fvec3_cross (lw6mat_fvec3_t * fvec3, const lw6mat_fvec3_t * fvec3_a,
		    const lw6mat_fvec3_t * fvec3_b)
{
  fvec3->p.x = fvec3_a->p.y * fvec3_b->p.z - fvec3_a->p.z * fvec3_b->p.y;
  fvec3->p.y = fvec3_a->p.z * fvec3_b->p.x - fvec3_a->p.x * fvec3_b->p.z;
  fvec3->p.z = fvec3_a->p.x * fvec3_b->p.y - fvec3_a->p.y * fvec3_b->p.x;
}

/**
 * lw6mat_fvec3_scale
 *
 * @fvec3: vector to modify
 * @f: scale factor
 *
 * Scales the vector by multiplying all its members by a scalar value.
 *
 * Return value: none
 */
void
lw6mat_fvec3_scale (lw6mat_fvec3_t * fvec3, float f)
{
  fvec3->p.x *= f;
  fvec3->p.y *= f;
  fvec3->p.z *= f;
}
