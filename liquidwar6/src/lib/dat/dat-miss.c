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
lw6dat_miss_new (lw6sys_context_t * sys_context, u_int64_t from_id, int serial_min, int serial_max)
{
  lw6dat_miss_t *miss = NULL;

  miss = (lw6dat_miss_t *) LW6SYS_MALLOC (sys_context, sizeof (lw6dat_miss_t));
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
lw6dat_miss_free (lw6sys_context_t * sys_context, lw6dat_miss_t * miss)
{
  if (miss)
    {
      LW6SYS_FREE (sys_context, miss);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL miss"));
    }
}

/**
 * lw6dat_miss_sync
 *
 * @dst: target object
 * @src: source object
 *
 * Writes the contents of src to dst
 *
 * Return value: none.
 */
void
lw6dat_miss_sync (lw6sys_context_t * sys_context, lw6dat_miss_t * dst, lw6dat_miss_t * src)
{
  if (dst && src)
    {
      memcpy (dst, src, sizeof (lw6dat_miss_t));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to sync NULL miss dst=%p src=%p"), dst, src);
    }
}

/**
 * lw6dat_miss_is_same
 *
 * @a: first element to compare 
 * @b: second element to compare
 *
 * Compares two miss objects, returning true if they are the same.
 *
 * Return value: 1 if same, else 0.
 */
int
lw6dat_miss_is_same (lw6sys_context_t * sys_context, lw6dat_miss_t * a, lw6dat_miss_t * b)
{
  int ret = 0;

  if (a && b)
    {
      ret = !memcmp (a, b, sizeof (lw6dat_miss_t));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to compare NULL miss a=%p b=%p"), a, b);
    }

  return ret;
}

/**
 * lw6dat_miss_is_included
 *
 * @a: first element to compare 
 * @b: second element to compare
 *
 * Compares two miss objects, returning true if a is included in b.
 *
 * Return value: 1 if included, else 0.
 */
int
lw6dat_miss_is_included (lw6sys_context_t * sys_context, lw6dat_miss_t * a, lw6dat_miss_t * b)
{
  int ret = 0;

  if (a && b)
    {
      ret = (a->from_id == b->from_id) && (a->serial_min >= b->serial_min) && (a->serial_max <= b->serial_max) && (a->serial_min <= a->serial_max);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to compare NULL miss a=%p b=%p"), a, b);
    }

  return ret;
}

/**
 * lw6dat_miss_overlaps
 *
 * @a: first element to compare 
 * @b: second element to compare
 *
 * Compares two miss objects, returning true if a overlaps with b.
 *
 * Return value: 1 if included, else 0.
 */
int
lw6dat_miss_overlaps (lw6sys_context_t * sys_context, lw6dat_miss_t * a, lw6dat_miss_t * b)
{
  int ret = 0;

  if (a && b)
    {
      ret = (a->from_id == b->from_id) && (a->serial_min <= a->serial_max)
	&& (b->serial_min <= b->serial_max) && ((a->serial_max >= b->serial_min) || (a->serial_min <= b->serial_max));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to compare NULL miss a=%p b=%p"), a, b);
    }

  return ret;
}
