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

static u_int16_t
generate_4_digit (lw6sys_context_t * sys_context)
{
  u_int16_t ret = 0;

  ret = ((u_int16_t) lw6sys_random (sys_context, 0x6fff)) + 0x1000;

  return ret;
}

/**
 * lw6sys_generate_id_16:
 *
 * @sys_context: global system context
 *
 * Long 16-bit ID generator, calls the random function internally.
 * As usual, those are not perfect random numbers, however the function
 * implementation emphasizes more on 'real randomness' rather than
 * relying on performance. Generating twice the same number should be
 * fairly rare.
 *
 * @Return value: a pseudo-random number, 16-bit long, can not be 0.
 */
u_int16_t
lw6sys_generate_id_16 (lw6sys_context_t * sys_context)
{
  u_int16_t ret;

  ret = generate_4_digit (sys_context);

  if (!ret)
    {
      ret = lw6sys_generate_id_16 (sys_context);
    }

  return ret;
}

/**
 * lw6sys_generate_id_32:
 *
 * @sys_context: global system context
 *
 * Long 32-bit ID generator, calls the random function internally.
 * As usual, those are not perfect random numbers, however the function
 * implementation emphasizes more on 'real randomness' rather than
 * relying on performance. Generating twice the same number should be
 * fairly rare.
 *
 * @Return value: a pseudo-random number, 32-bit long, can not be 0.
 */
u_int32_t
lw6sys_generate_id_32 (lw6sys_context_t * sys_context)
{
  u_int32_t ret;

  ret = (((u_int32_t) generate_4_digit (sys_context)) << 16) | ((u_int32_t) generate_4_digit (sys_context));

  if (!ret)
    {
      ret = lw6sys_generate_id_32 (sys_context);
    }

  return ret;
}

/**
 * lw6sys_generate_id_64:
 *
 * @sys_context: global system context
 *
 * Long 64-bit ID generator, calls the random function internally.
 * As usual, those are not perfect random numbers, however the function
 * implementation emphasizes more on 'real randomness' rather than
 * relying on performance. Generating twice the same number should be
 * fairly rare.
 *
 * @Return value: a pseudo-random number, 64-bit long, can not be 0.
 */
u_int64_t
lw6sys_generate_id_64 (lw6sys_context_t * sys_context)
{
  u_int64_t ret;

  ret =
    (((u_int64_t) generate_4_digit (sys_context)) << 48) |
    (((u_int64_t) generate_4_digit (sys_context)) << 32) | (((u_int64_t) generate_4_digit (sys_context)) << 16) | ((u_int64_t) generate_4_digit (sys_context));

  if (!ret)
    {
      ret = lw6sys_generate_id_64 (sys_context);
    }

  return ret;
}

/**
 * lw6sys_check_id_16
 *
 * @sys_context: global system context
 * @id_16: the id to check
 *
 * Checks wether the given id is a valid 16-bit id.
 *
 * Return value: 1 if OK, 0 if not a valid id.
 */
int
lw6sys_check_id_16 (lw6sys_context_t * sys_context, u_int16_t id_16)
{
  int ret = 0;

  ret = (id_16 >= 0x1000 && id_16 <= 0x7fff) || (id_16 == LW6SYS_BROADCAST_ID_16);

  return ret;
}

/**
 * lw6sys_check_id_32
 *
 * @sys_context: global system context
 * @id_32: the id to check
 *
 * Checks wether the given id is a valid 32-bit id.
 *
 * Return value: 1 if OK, 0 if not a valid id.
 */
int
lw6sys_check_id_32 (lw6sys_context_t * sys_context, u_int32_t id_32)
{
  int ret = 0;

  ret = (id_32 >= 0x10001000 && id_32 <= 0x7fff7fff) || (id_32 == LW6SYS_BROADCAST_ID_32);

  return ret;
}

/**
 * lw6sys_check_id_64
 *
 * @sys_context: global system context
 * @id_64: the id to check
 *
 * Checks wether the given id is a valid 64-bit id.
 *
 * Return value: 1 if OK, 0 if not a valid id.
 */
int
lw6sys_check_id_64 (lw6sys_context_t * sys_context, u_int64_t id_64)
{
  int ret = 0;

  ret = (id_64 >= 0x1000100010001000LL && id_64 <= 0x7fff7fff7fff7fffLL) || (id_64 == LW6SYS_BROADCAST_ID_64);

  return ret;
}

/**
 * lw6sys_check_id
 *
 * @sys_context: global system context
 * @id: the id to check
 *
 * Checks wether the given id is a valid id (16, 32 or 64-bit).
 *
 * Return value: 1 if OK, 0 if not a valid id.
 */
int
lw6sys_check_id (lw6sys_context_t * sys_context, u_int64_t id)
{
  int ret = 0;

  ret = lw6sys_check_id_16 (sys_context, id) || lw6sys_check_id_32 (sys_context, id) || lw6sys_check_id_64 (sys_context, id);

  return ret;
}

/**
 * lw6sys_id_ltoa
 *
 * @sys_context: global system context
 * @id: the id to convert
 *
 * Transform an id into its string representation. Error checking is
 * done, if the id is invalid, returned value is NULL.
 * All ids (16, 32 and 64-bit) are handled.
 *
 * Return value: a newly allocated string, might be NULL.
 */
char *
lw6sys_id_ltoa (lw6sys_context_t * sys_context, u_int64_t id)
{
  char *ret = NULL;

  if (lw6sys_check_id (sys_context, id))
    {
      ret = lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "x", (long long) id);
    }

  return ret;
}

/**
 * lw6sys_id_atol
 *
 * @sys_context: global system context
 * @id: the id to convert
 *
 * Transform an id into a long integer. Error checking is
 * done, if the id is invalid, returned value is 0.
 * All ids (16, 32 and 64-bit) are handled.
 *
 * Return value: the id as a long integer, 0 if incorrect source id.
 */
u_int64_t
lw6sys_id_atol (lw6sys_context_t * sys_context, char *id)
{
  u_int64_t ret = 0LL;
  int len = 0;
  char *scan_format_u_int64 = "%" LW6SYS_PRINTF_LL "x";	// defined here to avoid GCC warning

  len = strlen (id);

  if (len == LW6SYS_LEN_ID_16 || len == LW6SYS_LEN_ID_32 || len == LW6SYS_LEN_ID_64)
    {
      if (!sscanf (id, scan_format_u_int64, &ret))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("id \"%s\" is not valid (sscanf error)"), id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("id \"%s\" is not valid (bad length)"), id);
    }

  if (!lw6sys_check_id (sys_context, ret))
    {
      ret = 0;
    }

  return ret;
}
