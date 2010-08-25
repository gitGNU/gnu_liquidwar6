/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "dat.h"

/**
 * lw6dat_matrix_new
 *
 * @line_0_serial: serial number for line 0.
 *
 * Creates a new matrix object.
 *
 * Return value: newly allocated object.
 */
lw6dat_matrix_t *
lw6dat_matrix_new (int line_0_serial)
{
  lw6dat_matrix_t *matrix = NULL;

  matrix = (lw6dat_matrix_t *) LW6SYS_CALLOC (sizeof (lw6dat_matrix_t));
  if (matrix)
    {
      matrix->line_0_serial = line_0_serial;
    }

  return matrix;
}

/**
 * lw6dat_matrix_free
 *
 * @matrix: the matrix to free
 *
 * Deletes a matrix object
 *
 * Return value: none.
 */
void
lw6dat_matrix_free (lw6dat_matrix_t * matrix)
{
  LW6SYS_FREE (matrix);
}
