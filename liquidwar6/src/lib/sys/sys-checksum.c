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

#include <string.h>

#include "sys.h"

#define CHECKSUM_POLY 0x04c11db7

/**
 * lw6sys_checksum
 *
 * @sys_context: global system context
 * @data: the data to process
 * @len: the length, in bytes, of the data to process
 *
 * Creates a checksum from a byte array. This could be mapped on
 * any standard CRC-32 and/or MD5 algorithm, but licence issues
 * for those are such a headache that for the sake of simplicity,
 * it's wrapped here. In LW6 context, we do not really really
 * fear any attack for these checksums are used internally to
 * track bugs and check, for instance, that two game states are
 * actually the same on two distant computers in a network game.
 * Data encryption and security of network links is another debate.
 * Additionnally, this function returns an integer, easier to
 * handle in standard C than any malloc'ed stuff.
 *
 * Return value: the checksum, as an integer.
 */
u_int32_t
lw6sys_checksum (lw6sys_context_t * sys_context, unsigned char *data, int len)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update (sys_context, &ret, data, len);

  return ret;
}

/**
 * lw6sys_checksum_str
 *
 * @sys_context: global system context
 * @value: the string to process
 *
 * Creates a checksum from a string. This is a convenience
 * function to save the programmer the hassle of calling strlen
 * before any checksum calculation.
 *
 * Return value: the checksum, as an integer.
 */
u_int32_t
lw6sys_checksum_str (lw6sys_context_t * sys_context, const char *value)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update_str (sys_context, &ret, value);

  return ret;
}

/**
 * lw6sys_checksum_int32
 *
 * @sys_context: global system context
 * @value: the integer to process
 *
 * Creates a checksum from an integer. This is a convenience
 * function to save the programmer the hassle of passing a pointer
 * to the integer with the size of it each time there's a checksum
 * to do. Additionnally, with this one you can pass an int8 or an int16,
 * and function will work just the same indenpendantly of endianness.
 *
 * Return value: the checksum, as an integer.
 */
u_int32_t
lw6sys_checksum_int32 (lw6sys_context_t * sys_context, u_int32_t value)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update_int32 (sys_context, &ret, value);

  return ret;
}

/**
 * lw6sys_checksum_int64
 *
 * @value: the integer to process
 *
 * Creates a checksum from an integer. This is a convenience
 * function to save the programmer the hassle of passing a pointer
 * to the integer with the size of it each time there's a checksum
 * to do. This function handles 64-bit long long integers..
 *
 * Return value: the checksum, as an integer.
 */
u_int32_t
lw6sys_checksum_int64 (lw6sys_context_t * sys_context, u_int64_t value)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update_int64 (sys_context, &ret, value);

  return ret;
}

/**
 * lw6sys_checksum_whd
 *
 * @sys_context: global system context
 * @whd: a pointer to the wh struct to be processed
 *
 * Creates a checksum from the given structure. Convenience
 * function to save the hassle of passing a pointer to and the size
 * of the @lw6sys_wh_t struct each time, knowing that there are very often
 * checksums calculated on it. Also avoids endianess issues.
 *
 * Return value: the checksum, as an integer.
 */
u_int32_t
lw6sys_checksum_whd (lw6sys_context_t * sys_context, lw6sys_whd_t * whd)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update_whd (sys_context, &ret, whd);

  return ret;
}

/**
 * lw6sys_checksum_xyz
 *
 * @sys_context: global system context
 * @xyz: a pointer to the xy struct to be processed
 *
 * Creates a checksum from the given structure. Convenience
 * function to save the hassle of passing a pointer to and the size
 * of the @lw6sys_xy_t struct each time, knowing that there are very often
 * checksums calculated on it. Also avoids endianess issues.
 *
 * Return value: the checksum, as an integer.
 */
u_int32_t
lw6sys_checksum_xyz (lw6sys_context_t * sys_context, lw6sys_xyz_t * xyz)
{
  u_int32_t ret = 0;

  lw6sys_checksum_update_xyz (sys_context, &ret, xyz);

  return ret;
}

/**
 * lw6sys_checksum_update
 *
 * @sys_context: global system context
 * @checksum: a pointer to the previous checksum
 * @data: the data to process
 * @len: the length, in bytes, of the data to process
 *
 * Creates a checksum from the given data. The difference
 * with @lw6sys_checksum is that this one updates an existing
 * checksum, thus enabling the programmer to call it sequentially
 * and get a global checksum on different sources.
 *
 * Return value: none.
 */
void
lw6sys_checksum_update (lw6sys_context_t * sys_context, u_int32_t * checksum, unsigned char *data, int len)
{
  /*
   * Algorithm copied from Linux kernel source (lib/crc32.c).
   * The code used to be public domain, was GPL'ed, and is however
   * way less than 15 lines long... We don't care wether it's a real
   * bullet proof CRC32, all we want is consistent calculus on
   * every platform, hopefully this code fits.
   */
  int i;

  while (len--)
    {
      (*checksum) ^= ((u_int32_t) (*data++)) << 24;
      for (i = 0; i < 8; i++)
	(*checksum) = ((*checksum) << 1) ^ (((*checksum) & 0x80000000) ? CHECKSUM_POLY : 0);
    }
}

/**
 * lw6sys_checksum_update_str
 *
 * @sys_context: global system context
 * @checksum: a pointer to the previous checksum
 * @value: the string to process
 *
 * Creates a checksum from the given string. The difference
 * with @lw6sys_checksum_str is that this one updates an existing
 * checksum, thus enabling the programmer to call it sequentially
 * and get a global checksum on different sources.
 *
 * Return value: none.
 */
void
lw6sys_checksum_update_str (lw6sys_context_t * sys_context, u_int32_t * checksum, const char *value)
{
  int len = 0;

  len = strlen (value);
  if (len > 0)
    {
      lw6sys_checksum_update (sys_context, checksum, (unsigned char *) value, len);
    }
}

/**
 * lw6sys_checksum_update_int32
 *
 * @sys_context: global system context
 * @checksum: a pointer to the previous checksum
 * @value: the integer to process
 *
 * Creates a checksum from the given integer. The difference
 * with @lw6sys_checksum_int32 is that this one updates an existing
 * checksum, thus enabling the programmer to call it sequentially
 * and get a global checksum on different sources.
 *
 * Return value: none.
 */
void
lw6sys_checksum_update_int32 (lw6sys_context_t * sys_context, u_int32_t * checksum, int32_t value)
{
  unsigned char buffer[LW6SYS_SIZEOF_INT32];

  lw6sys_serialize_int32 (sys_context, buffer, value);
  lw6sys_checksum_update (sys_context, checksum, buffer, LW6SYS_SIZEOF_INT32);
}

/**
 * lw6sys_checksum_update_int64
 *
 * @sys_context: global system context
 * @checksum: a pointer to the previous checksum
 * @value: the integer to process
 *
 * Creates a checksum from the given integer. The difference
 * with @lw6sys_checksum_int64 is that this one updates an existing
 * checksum, thus enabling the programmer to call it sequentially
 * and get a global checksum on different sources.
 *
 * Return value: none.
 */
void
lw6sys_checksum_update_int64 (lw6sys_context_t * sys_context, u_int32_t * checksum, int64_t value)
{
  unsigned char buffer[LW6SYS_SIZEOF_INT64];

  lw6sys_serialize_int64 (sys_context, buffer, value);
  lw6sys_checksum_update (sys_context, checksum, buffer, LW6SYS_SIZEOF_INT64);
}

/**
 * lw6sys_checksum_update_whd
 *
 * @sys_context: global system context
 * @checksum: a pointer to the previous checksum
 * @whd: a pointer to the wh struct to be processed
 *
 * Creates a checksum from the given structure. The difference
 * with @lw6sys_checksum_whd is that this one updates an existing
 * checksum, thus enabling the programmer to call it sequentially
 * and get a global checksum on different sources.
 *
 * Return value: none.
 */
void
lw6sys_checksum_update_whd (lw6sys_context_t * sys_context, u_int32_t * checksum, const lw6sys_whd_t * whd)
{
  lw6sys_checksum_update_int32 (sys_context, checksum, whd->w);
  lw6sys_checksum_update_int32 (sys_context, checksum, whd->h);
  lw6sys_checksum_update_int32 (sys_context, checksum, whd->d);
}

/**
 * lw6sys_checksum_update_xyz
 *
 * @sys_context: global system context
 * @checksum: a pointer to the previous checksum
 * @xyz: a pointer to the xy struct to be processed
 *
 * Creates a checksum from the given structure. The difference
 * with @lw6sys_checksum_xyz is that this one updates an existing
 * checksum, thus enabling the programmer to call it sequentially
 * and get a global checksum on different sources.
 *
 * Return value: none.
 */
void
lw6sys_checksum_update_xyz (lw6sys_context_t * sys_context, u_int32_t * checksum, const lw6sys_xyz_t * xyz)
{
  lw6sys_checksum_update_int32 (sys_context, checksum, xyz->x);
  lw6sys_checksum_update_int32 (sys_context, checksum, xyz->y);
  lw6sys_checksum_update_int32 (sys_context, checksum, xyz->z);
}
