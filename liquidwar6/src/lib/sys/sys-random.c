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

#define RANDOM_FLOAT_PRECISION 1000000

/**
 * lw6sys_random:
 *
 * @sys_context: global system context
 * @range: the high limit for random generated numbers. If you want
 *         random numbers between 0 and 5, set this to 6.
 *
 * Wrapper over standard random function. This one is thread safe.
 * This idea is not to provide cryptographic-proof random numbers,
 * rather generate sequences which are random enough to generate
 * unique server ids and such things. The function is initialized
 * on its first call, and results depend on timestamp, host name,
 * user name, and memory available.
 *
 * @Return value: a pseudo-random number
 */
u_int32_t
lw6sys_random (lw6sys_context_t * sys_context, u_int32_t range)
{
  static u_int32_t dirty_seed = 0;
  static u_int32_t alternate_seed = 0;
  u_int32_t tmp_seed;
  u_int32_t ret = 0;
  char *str = NULL;

  if (!alternate_seed)
    {
      str = lw6sys_get_hostname (sys_context);
      if (str)
	{
	  lw6sys_checksum_update_str (sys_context, &alternate_seed, str);
	  LW6SYS_FREE (sys_context, str);
	}
      str = lw6sys_get_username (sys_context);
      if (str)
	{
	  lw6sys_checksum_update_str (sys_context, &alternate_seed, str);
	  LW6SYS_FREE (sys_context, str);
	}
      lw6sys_checksum_update_int32 (sys_context, &alternate_seed, lw6sys_megabytes_available (sys_context));
      lw6sys_checksum_update_int32 (sys_context, &alternate_seed, lw6sys_get_timestamp (sys_context));
      while (!alternate_seed)
	{
	  lw6sys_checksum_update_int32 (sys_context, &alternate_seed, lw6sys_get_timestamp (sys_context));
	}
    }

  tmp_seed = dirty_seed;
  lw6sys_checksum_update_int32 (sys_context, &tmp_seed, alternate_seed);
  lw6sys_checksum_update_int32 (sys_context, &tmp_seed, lw6sys_get_timestamp (sys_context));
  while (tmp_seed == dirty_seed)
    {
      lw6sys_checksum_update_int32 (sys_context, &tmp_seed, lw6sys_get_timestamp (sys_context));
    }
#ifdef LW6_MS_WINDOWS
  /*
   * rand_r not available on Microsoft proprietary platform. Rather
   * than calling rand() which is not said to be thread-safe, we rely
   * on a dummy checksum, since dirty_seed is incremented at each
   * call, it should still give a (poor...) illusion of random numbers.
   *
   * Note that fancy functions CryptAcquireContext / CryptGenRandom exist
   * but require professionnal / server versions of MS Windows.
   */
  if (range > 0)
    {
      ret = tmp_seed % range;
    }
#else
  ret = (((u_int64_t) rand_r (&tmp_seed)) * ((u_int64_t) range)) / (((u_int64_t) RAND_MAX) + 1);
#endif
  dirty_seed = tmp_seed;

  return ret;
}

/**
 * lw6sys_random_float:
 *
 * @sys_context: global system context
 * @min: the min value, as a float
 * @max: the max value, as a float
 *
 * Returns a random float number between min & max. Can be equal to
 * min or max.
 *
 * @Return value: a pseudo-random number
 */
float
lw6sys_random_float (lw6sys_context_t * sys_context, float min, float max)
{
  float ret = 0.0f;

  ret = ((((float) (lw6sys_random (sys_context, RANDOM_FLOAT_PRECISION)) * (max - min))) / (float) RANDOM_FLOAT_PRECISION) + min;

  return ret;
}
