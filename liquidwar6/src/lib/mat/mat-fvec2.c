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
 * Normalizes a vector
 *
 * @fvec2: the vector to normalize.
 *
 * Normalizes a vector, that is, make its length be 1.
 *
 * Return value: none.
 */
void
lw6mat_fvec2_norm (lw6mat_fvec2_t * fvec2)
{
  float len = lw6mat_fvec2_len (fvec2);

  if (len)
    {
      fvec2->p.x /= len;
      fvec2->p.y /= len;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("trying to normalize vector zero"));
    }
}
