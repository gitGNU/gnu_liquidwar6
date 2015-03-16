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

/**
 * lw6mat_is_similar_f
 *
 * @f_a: 1st value to compare
 * @f_b: 2nd value to compare
 *
 * Compares two value, and returns true if they look the same.
 * This similarity is based on a percentage of difference for
 * big enough values, and for very small values, they are just
 * considered equal whatever happens. This is far from perfect
 * but the purpose is usually just to track blunders in matrix
 * code.
 *
 * Return value: 1 if similar, 0 if not.
 */
int
lw6mat_is_similar_f (float f_a, float f_b)
{
  float diff = fabsf (f_a - f_b);
  float scale = lw6sys_fmax (fabsf (f_a), fabsf (f_b));

  return (scale <= LW6MAT_SIMILAR_F_SCALE) ? 1 : ((diff / scale) <= LW6MAT_SIMILAR_F_DIFF);
}

/**
 * lw6mat_is_similar_i
 *
 * @i_a: 1st value to compare
 * @i_b: 2nd value to compare
 *
 * Compares two value, and returns true if they look the same.
 * This similarity is based on a percentage of difference for
 * big enough values, and for very small values, they are just
 * considered equal whatever happens. This is far from perfect
 * but the purpose is usually just to track blunders in matrix
 * code.
 *
 * Return value: 1 if similar, 0 if not.
 */
int
lw6mat_is_similar_i (int32_t i_a, int32_t i_b)
{
  int32_t diff = abs (i_a - i_b);
  int32_t scale = lw6sys_imax (abs (i_a), abs (i_b));

  return (scale <= LW6MAT_SIMILAR_I_SCALE) ? 1 : (diff <= LW6MAT_SIMILAR_I_DIFF);
}

/**
 * lw6mat_is_similar_d
 *
 * @d_a: 1st value to compare
 * @d_b: 2nd value to compare
 *
 * Compares two value, and returns true if they look the same.
 * This similarity is based on a percentage of difference for
 * big enough values, and for very small values, they are just
 * considered equal whatever happens. This is far from perfect
 * but the purpose is usually just to track blunders in matrix
 * code.
 *
 * Return value: 1 if similar, 0 if not.
 */
int
lw6mat_is_similar_d (double d_a, double d_b)
{
  double diff = fabs (d_a - d_b);
  double scale = lw6sys_dmax (fabs (d_a), fabs (d_b));

  return (scale <= LW6MAT_SIMILAR_D_SCALE) ? 1 : ((diff / scale) <= LW6MAT_SIMILAR_D_DIFF);
}

/**
 * lw6mat_is_similar_x
 *
 * @x_a: 1st value to compare
 * @x_b: 2nd value to compare
 *
 * Compares two value, and returns true if they look the same.
 * This similarity is based on a percentage of difference for
 * big enough values, and for very small values, they are just
 * considered equal whatever happens. This is far from perfect
 * but the purpose is usually just to track blunders in matrix
 * code.
 *
 * Return value: 1 if similar, 0 if not.
 */
int
lw6mat_is_similar_x (int32_t x_a, int32_t x_b)
{
  int32_t diff = abs (x_a - x_b);
  int32_t scale = lw6sys_imax (abs (x_a), abs (x_b));

  return (scale <= LW6MAT_SIMILAR_X_SCALE) ? 1 : (lw6mat_x_div_32 (diff, scale) <= LW6MAT_SIMILAR_X_DIFF);
}
