/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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
  return fvec4->p.x * fvec4->p.x + fvec4->p.y * fvec4->p.y +
    fvec4->p.z * fvec4->p.z + fvec4->p.w * fvec4->p.w;
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
 * Normalizes a vector
 *
 * @fvec4: the vector to normalize.
 *
 * Normalizes a vector, that is, make its length be 1.
 *
 * Return value: none.
 */
void
lw6mat_fvec4_norm (lw6mat_fvec4_t * fvec4)
{
  float len = lw6mat_fvec4_len (fvec4);

  if (len)
    {
      fvec4->p.x /= len;
      fvec4->p.y /= len;
      fvec4->p.z /= len;
      fvec4->p.w /= len;
    }
}
