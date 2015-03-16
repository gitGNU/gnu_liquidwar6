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

#include "pil.h"
#include "pil-internal.h"

/**
 * lw6pil_seq_random_0
 *
 * @sys_context: global system context
 *
 * Gets a pseudo-random start seq, why do we use this? Just to make
 * sure even in non-network situations, seq are always very high and
 * random, this way this is one less bug to check in networked context.
 *
 * Return value: random integer value, always greater than INT_MAX
 */
int64_t
lw6pil_seq_random_0 (lw6sys_context_t * sys_context)
{
  int64_t ret = 0L;

  ret = _LW6PIL_MIN_SEQ_0 + lw6sys_random (sys_context, _LW6PIL_RANGE_SEQ_0);

  return ret;
}
