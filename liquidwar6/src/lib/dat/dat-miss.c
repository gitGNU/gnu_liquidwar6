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

#include "dat-internal.h"

/**
 * lw6dat_miss_new
 *
 * @from_id: who needs to resend the message
 * @serial_min: serial range begin (included)
 * @serial_max: serial range end (included)
 *
 * Creates a miss structure, simply allocates memory and
 * pumps values into the right fields.
 *
 * Return value: new dynamically allocated object
 */
lw6dat_miss_t *
lw6dat_miss_new (u_int64_t from_id, int serial_min, int serial_max)
{
  lw6dat_miss_t *miss = NULL;

  miss = (lw6dat_miss_t *) LW6SYS_MALLOC (sizeof (lw6dat_miss_t));
  if (miss)
    {
      miss->from_id = from_id;
      miss->serial_min = serial_min;
      miss->serial_max = serial_max;
    }

  return miss;
}

/**
 * lw6dat_miss_free
 *
 * @miss: structure to free
 *
 * Frees a miss structure, simply unallocates memory.
 *
 * Return value: none.
 */
void
lw6dat_miss_free (lw6dat_miss_t * miss)
{
  if (miss)
    {
      LW6SYS_FREE (miss);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("trying to free NULL miss"));
    }
}
