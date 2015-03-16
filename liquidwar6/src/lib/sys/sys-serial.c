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

/**
 * lw6sys_serialize_int64
 *
 * @sys_context: global system context
 * @data: pointer to the data, must contain at least 8 bytes of writable space
 * @value: the integer to serialize
 *
 * Serializes a 64-bit integer in a byte buffer. Result is not dependant
 * on machine endianess. Typically used for checksums or high-level
 * serializations.
 */
void
lw6sys_serialize_int64 (lw6sys_context_t * sys_context, unsigned char *data, int64_t value)
{
  data[0] = value >> 56;
  data[1] = (value >> 48) & 0xFF;
  data[2] = (value >> 40) & 0xFF;
  data[3] = (value >> 32) & 0xFF;
  data[4] = (value >> 24) & 0xFF;
  data[5] = (value >> 16) & 0xFF;
  data[6] = (value >> 8) & 0xFF;
  data[7] = value & 0xFF;
}

/**
 * lw6sys_unserialize_int64
 *
 * @sys_context: global system context
 * @data: pointer to the data, must contain at least 8 bytes
 * @value: the integer to unserialize
 *
 * Recovers a 64-bit integer from a byte buffer created,
 * for instance, with @lw6sys_serialize_int64.
 */
int64_t
lw6sys_unserialize_int64 (lw6sys_context_t * sys_context, unsigned char *data)
{
  int64_t ret;

  ret =
    (((int64_t) data[0]) << 56) + (((int64_t) data[1]) << 48) +
    (((int64_t) data[2]) << 40) + (((int64_t) data[3]) << 32) +
    (((int64_t) data[4]) << 24) + (((int64_t) data[5]) << 16) + (((int64_t) data[6]) << 8) + (int64_t) data[7];

  return ret;
}

/**
 * lw6sys_serialize_int32
 *
 * @sys_context: global system context
 * @data: pointer to the data, must contain at least 4 bytes of writable space
 * @value: the integer to serialize
 *
 * Serializes a 32-bit integer in a byte buffer. Result is not dependant
 * on machine endianess. Typically used for checksums or high-level
 * serializations.
 */
void
lw6sys_serialize_int32 (lw6sys_context_t * sys_context, unsigned char *data, int32_t value)
{
  data[0] = value >> 24;
  data[1] = (value >> 16) & 0xFF;
  data[2] = (value >> 8) & 0xFF;
  data[3] = value & 0xFF;
}

/**
 * lw6sys_unserialize_int32
 *
 * @sys_context: global system context
 * @data: pointer to the data, must contain at least 4 bytes
 * @value: the integer to unserialize
 *
 * Recovers a 32-bit integer from a byte buffer created,
 * for instance, with @lw6sys_serialize_int32.
 */
int32_t
lw6sys_unserialize_int32 (lw6sys_context_t * sys_context, unsigned char *data)
{
  int32_t ret;

  ret = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];

  return ret;
}

/**
 * lw6sys_serialize_int16
 *
 * @sys_context: global system context
 * @data: pointer to the data, must contain at least 2 bytes of writable space
 * @value: the integer to serialize
 *
 * Serializes a 16-bit integer in a byte buffer. Result is not dependant
 * on machine endianess. Typically used for checksums or high-level
 * serializations.
 */
void
lw6sys_serialize_int16 (lw6sys_context_t * sys_context, unsigned char *data, int16_t value)
{
  data[0] = value >> 8;
  data[1] = value & 0xFF;
}

/**
 * lw6sys_unserialize_int16
 *
 * @sys_context: global system context
 * @data: pointer to the data, must contain at least 2 bytes
 * @value: the integer to unserialize
 *
 * Recovers a 16-bit integer from a byte buffer created,
 * for instance, with @lw6sys_serialize_int16.
 */
int16_t
lw6sys_unserialize_int16 (lw6sys_context_t * sys_context, unsigned char *data)
{
  int16_t ret;

  ret = (data[0] << 8) + data[1];

  return ret;
}
