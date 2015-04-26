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

#include <math.h>

#include "sys.h"
#include "sys-internal.h"

#define _HEARTBEAT_HIGH1 0.1
#define _HEARTBEAT_LOW1 0.25
#define _HEARTBEAT_HIGH2 0.35
#define _HEARTBEAT_LOW2 0.90

/**
 * lw6sys_math_poly_wy1y2s1
 *
 * @sys_context: global system context
 * @y: the return value (position, may be NULL)
 * @s: the return value (speed, may be NULL)
 * @x: the x parameter, the value to iterate on
 * @w: the width, that is, the x value after which output is constant
 * @y1: the initial value, when v is s1 and x=0
 * @y2: the target value, when v=0 and x>=w
 * @s1: the initial speed, that is dy/dx at x=0
 *
 * A function which can be used to implement smooth moving. It will
 * extrapolate, for values of x>=0, an y position with a continuous
 * derivate (dy/dx is continuous, function is 2nd order polynom) and
 * which ends up at x=w with a constant value, that is dy/dx=v=0.
 * Typically an item set with an initial speed of v with this function
 *
 */
void
lw6sys_math_poly_wy1y2s1 (lw6sys_context_t * sys_context, float *y, float *s, float x, float w, float y1, float y2, float s1)
{
  float a = 0.0f;
  float b = 0.0f;
  float c = 0.0f;
  float d = 0.0f;
  float tmp_y = 0.0f;
  float tmp_s = 0.0f;

  /*
   * Problem follows:
   *
   * y = a*x^3 + b*x^2 + c*x + d (y' = 3*a*x^2 + 2*b*x + c)
   *
   * y1 = a*0^3 + b*0^2 + c*0 + d                 # L1 : y1 cond
   * s1 = 3*0^2 + 2*b*0 + c                       # L2 : s1 cond
   * y2 = a*w^3 + b*w^2 + c*w + d                 # L3 : y2 cond
   * 0 = 3*a*w^2 + 2*b*w + c                      # L4 : vf cond
   *
   * d = y1                                       # L1
   * c = s1                                       # L2
   * y2 = a*w^3 + b*w^2 + s1*w + y1               # L3
   * - 3*y2/w = 2*b*w - 3*b*w + c - 3*c - 3*d/w   # L4 - 3*L3/w
   *
   * d = y1                                       # L1
   * c = s1                                       # L2
   * a*w^3 = y2 - b*w^2 - s1*w - y1               # L3
   * b*w = 3*y2/w - 2*s1 - 3*y1/w                 # L4
   *
   * d = y1                                       # L1
   * c = s1                                       # L2
   * a = (y2-y1)/w^3 - b/w - s1/w^2               # L3
   * b = 3*(y2-y1)/w^2 - 2*s1/w                   # L4
   *
   * d = y1                                              # L1
   * c = s1                                              # L2
   * a = (y2-y1)/w^3 - 3*(y2-y1)/w^3 + 2*s1/w^2 - s1/w^2 # L3
   * b = 3*(y2-y1)/w^2 - 2*s1/w                          # L4
   *
   * d = y1                                       # L1
   * c = s1                                       # L2
   * a = -2*(y2-y1)/w^3 + s1/w^2                  # L3
   * b = 3*(y2-y1)/w^2 - 2*s1/w                   # L4
   */
  if (w > 0.0f && x >= 0.0f)
    {
      if (x >= w)
	{
	  tmp_y = y2;
	  tmp_s = 0.0f;
	}
      else
	{
	  a = -2.0f * (y2 - y1) / (w * w * w) + s1 / (w * w);
	  b = 3.0f * (y2 - y1) / (w * w) - 2 * s1 / w;
	  c = s1;
	  d = y1;
	  tmp_y = a * x * x * x + b * x * x + c * x + d;
	  tmp_s = 3 * a * x * x + 2 * b * x + c;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't extrapolate with x=%f and w=%f"), x, w);
      tmp_y = y2;
      tmp_s = 0.0f;
    }
  if (y)
    {
      (*y) = tmp_y;
    }
  if (s)
    {
      (*s) = tmp_s;
    }
}

/**
 * lw6sys_math_angle_360
 *
 * @sys_context: global system context
 * @x: x coordinate
 * @y: y coordinate
 *
 * This is a wrapper over the standard @atan function which will
 * handle internally the special x == 0 case and the various positive/negative
 * values of @x and @y.
 *
 * Return value: the angle, in degrees
 */
float
lw6sys_math_angle_360 (lw6sys_context_t * sys_context, int x, int y)
{
  float ret = 0.0f;

  if (x == 0)
    {
      ret = (y == 0) ? 0.0f : (y > 0) ? 90.0f : 270.0f;
    }
  else
    {
      if (x > 0)
	{
	  if (y >= 0)
	    {
	      ret = lw6sys_math_rad2deg (atanf (((float) y) / ((float) x)));
	    }
	  else
	    {
	      ret = 360.0f - lw6sys_math_rad2deg (atanf (((float) -y) / ((float) x)));
	    }
	}
      else
	{
	  if (y >= 0)
	    {
	      ret = 180.0f - lw6sys_math_rad2deg (atanf (((float) y) / ((float) -x)));
	    }
	  else
	    {
	      ret = 180.0f + lw6sys_math_rad2deg (atanf (((float) -y) / ((float) -x)));
	    }
	}
    }

  return ret;
}

/**
 * lw6sys_math_heartbeat
 *
 * @sys_context: global system context
 * @x: the parameter (typically a timestamp)
 * @period: the period (typically something like 1000 milliseconds)
 * @y1: the low value (heart at rest)
 * @y2: the high value (when bumping)
 *
 * A heartbeat function, typically usefull to power up eye-candy,
 * but it could do something else.
 *
 */
float
lw6sys_math_heartbeat (lw6sys_context_t * sys_context, int64_t x, int period, float y1, float y2)
{
  float ret = y1;
  int t;
  float high1;
  float low1;
  float high2;
  float low2;

  if (period > 0)
    {
      if (x >= 0)
	{
	  t = x % (int64_t) period;
	  high1 = _HEARTBEAT_HIGH1 * period;
	  low1 = _HEARTBEAT_LOW1 * period;
	  high2 = _HEARTBEAT_HIGH2 * period;
	  low2 = _HEARTBEAT_LOW2 * period;

	  if (t < high1)
	    {
	      lw6sys_math_poly_wy1y2s1 (sys_context, &ret, NULL, t, high1, y1, y2, 0.0f);
	    }
	  else if (t < low1)
	    {
	      lw6sys_math_poly_wy1y2s1 (sys_context, &ret, NULL, t - high1, low1 - high1, y2, y1, 0.0f);
	    }
	  else if (t < high2)
	    {
	      lw6sys_math_poly_wy1y2s1 (sys_context, &ret, NULL, t - low1, high2 - low1, y1, y2, 0.0f);
	    }
	  else if (t < low2)
	    {
	      lw6sys_math_poly_wy1y2s1 (sys_context, &ret, NULL, t - high2, low2 - high2, y2, y1, 0.0f);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't use period %d"), period);
    }

  return ret;
}

/**
 * lw6sys_math_blink
 *
 * @sys_context: global system context
 * @x: the parameter (typically a timestamp)
 * @period: the period (typically something like 1000 milliseconds)
 *
 * This function will alternatively return 1 or 0, usefull
 * to handle blinking for instance.
 *
 * Return value: 0 or 1
 */
int
lw6sys_math_blink (lw6sys_context_t * sys_context, int64_t x, int period)
{
  int ret = 0;

  if (period > 0)
    {
      if (x >= 0)
	{
	  ret = 2 * (x % (int64_t) period) / period;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't use period %d"), period);
    }

  return ret;
}


/**
 * lw6sys_math_lin2log
 *
 * @sys_context: global system context
 * @lin_value: value on a linear scale
 * @base: the base to use, 10 for decibel-like scale
 *
 * Converts a linar-scale value to a logarithmic one. The scale is done
 * so that base in linear mode is base in scaled mode, and it uses a
 * log-base conversion, so that with a base 10 it behaves the way
 * the decibel sound-volume unit works.
 *
 * Return value: value on a logarithmic scale.
 */
float
lw6sys_math_lin2log (lw6sys_context_t * sys_context, int lin_value, int base)
{
  base = lw6sys_fmax (2.0f, base);

  return base * logf (lw6sys_imax (1, lin_value)) / logf (base);
}

/**
 * lw6sys_math_log2lin
 *
 * @sys_context: global system context
 * @lin_value: value on a linear scale
 * @base: the base to use, 10 for decibel-like scale
 *
 * Converts a linar-scale value to a logarithmic one. The scale is done
 * so that 10 in linear mode is 10 in scaled mode, and it uses a
 * log-base conversion, so that with a base 10 it behaves the way
 * the decibel sound-volume unit works.
 *
 * Return value: value on a linear scale.
 */
int
lw6sys_math_log2lin (lw6sys_context_t * sys_context, float log_value, int base)
{
  base = lw6sys_fmax (2.0f, base);

  return lw6sys_imax (1, (int) roundf (expf (log_value * logf (base) / base)));
}

/**
 * lw6sys_math_deg2rad
 *
 * @deg: angle in degrees
 *
 * Converts an angle from degrees to radians.
 *
 * Return value: angle in radians.
 */
float
lw6sys_math_deg2rad (float deg)
{
  return deg * M_PI / 180.0f;
}

/**
 * lw6sys_math_rad2deg
 *
 * @rad: angle in radians
 *
 * Converts an angle from radians to degrees.
 *
 * Return value: angle in degrees.
 */
float
lw6sys_math_rad2deg (float rad)
{
  return rad * 180.0f / M_PI;
}
