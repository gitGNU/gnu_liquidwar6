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

#include <math.h>

#include "gui.h"

/**
 * lw6gui_smoother_init
 *
 * @sys_context: global system context
 * @smoother: the structure to initialize
 * @value: the value to use for now
 * @duration: the duration of a standard move, in ticks (msec)
 *
 * Initializes a smoother object, with a default value. The important point
 * is the duration which will condition all the behavior of the object.
 *
 * Return value: none.
 */
void
lw6gui_smoother_init (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, float value, int duration)
{
  memset (smoother, 0, sizeof (lw6gui_smoother_t));

  lw6gui_smoother_immediate_force (sys_context, smoother, value);
  smoother->duration = lw6sys_imax (1, duration);
}

/**
 * lw6gui_smoother_immediate_force
 *
 * @sys_context: global system context
 * @smoother: the structure to use
 * @value: the target value
 *
 * Forces a smoother object to immediately point on a value.
 *
 * Return value: none.
 */
void
lw6gui_smoother_immediate_force (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, float value)
{
  smoother->s1 = 0.0f;
  smoother->y1 = value;
  smoother->y2 = value;
  smoother->t1 = 0L;
}

/**
 * lw6gui_smoother_set_target
 *
 * @sys_context: global system context
 * @smoother: the structure to use
 * @value: the target value
 * @now: the current timestamp
 *
 * Sets a new target, will automatically calculate current speed to smooth
 * the next returned values.
 *
 * Return value: none.
 */
void
lw6gui_smoother_set_target (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, float value, int64_t now)
{
  float y = 0.0f;
  float s = 0.0f;

  if (smoother->y2 != value)
    {
      if (smoother->t1 > 0)
	{
	  lw6sys_math_poly_wy1y2s1 (sys_context, &y, &s, now - smoother->t1, smoother->duration, smoother->y1, smoother->y2, smoother->s1);
	  smoother->y1 = y;
	  smoother->s1 = s;
	}
      else
	{
	  smoother->y1 = smoother->y2;
	  smoother->s1 = 0.0f;
	}
      smoother->y2 = value;
      smoother->t1 = now;
    }
}

/**
 * lw6gui_smoother_get_value
 *
 * @sys_context: global system context
 * @smoother: the structure to use
 * @now: the current timestamp
 *
 * Returns the current value of the smoother.
 *
 * Return value: a float.
 */
float
lw6gui_smoother_get_value (lw6sys_context_t * sys_context, const lw6gui_smoother_t * smoother, int64_t now)
{
  float ret = 0.0f;

  if (smoother->t1 > 0)
    {
      lw6sys_math_poly_wy1y2s1 (sys_context, &ret, NULL, now - smoother->t1, smoother->duration, smoother->y1, smoother->y2, smoother->s1);
    }
  else
    {
      ret = smoother->y2;
    }

  return ret;
}

/**
 * lw6gui_smoother_fix_overflow
 *
 * @sys_context: global system context
 * @smoother: object to modify
 * @step: step size, typically twice the map size
 *
 * Companion function of @lw6pil_coords_fix_x10, this one will fix
 * a smoother target to avoid crazy scrolls when cursor is on a map
 * edge.
 *
 * Return value: none.
 */
void
lw6gui_smoother_fix_overflow (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, int step)
{
  float delta = 0.0f;
  int changed = 0;
  float value;
  float closest;

  value = smoother->y1;
  closest = smoother->y2;

  if (value > closest)
    {
      delta = fabs (value - closest);
      while (delta > fabs ((value - step) - closest))
	{
	  value -= step;
	  delta = fabs (value - closest);
	  changed = 1;
	}
    }
  if (value < closest)
    {
      delta = fabs (value - closest);
      while (delta > fabs ((value + step) - closest))
	{
	  value += step;
	  delta = fabs (value - closest);
	  changed = 1;
	}
    }
  if (changed)
    {
      smoother->y1 = value;
      lw6gui_smoother_set_target (sys_context, smoother, closest, smoother->t1);
    }
}
