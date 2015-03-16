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
#endif // HAVE_CONFIG_H

#include "sys.h"
#include "sys-internal.h"

#ifdef LW6_OPENMP
#include <omp.h>
#endif // LW6_OPENMP

/**
 * lw6sys_openmp_get_num_procs
 *
 * @sys_context: global system context
 *
 * Wrapper on @omp_get_num_procs the advantage of this is that
 * it's always defined, wether OpenMP supported is compiled in
 * or not, will returned 1 if no OpenMP support.
 *
 * Return value: number of procs
 */
int
lw6sys_openmp_get_num_procs (lw6sys_context_t * sys_context)
{
  int ret = 1;

#ifdef LW6_OPENMP
  ret = omp_get_num_procs ();
#endif // LW6_OPENMP

  return ret;
}
