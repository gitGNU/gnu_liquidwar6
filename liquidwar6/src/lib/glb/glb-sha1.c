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

#include "glb.h"
#include "glb-internal.h"

#include "gnulib-hmac.h"

#define _SHA1_EMPTY ""
#define _SHA1_SIZE 20
#define _SHA1_80_SIZE 10

/**
 * lw6glb_sha1_hmac_80_bin
 *
 * @sys_context: global system context
 * @key: the key buffer
 * @key_size
 * @buf: the data to analyse
 * @buf_size: the size of data to analyse
 *
 * Calculates an SHA-1 sum of buffer, using key to seed calc.
 *
 * Return value: newly allocated string, containing 20 chars checksum.
 */
char *
lw6glb_sha1_hmac_80_bin (lw6sys_context_t * sys_context, const char *key, int key_size, const char *buf, int buf_size)
{
  char *ret = NULL;
  char sha1_bin[_SHA1_SIZE];

  memset (sha1_bin, 0, _SHA1_SIZE);
  if (!hmac_sha1 ((void *) key, key_size, (void *) buf, buf_size, sha1_bin))
    {
      /*
       * We only convert the 10 first bytes, this way we reveal
       * less information when sending password hashes on the net.
       */
      ret = lw6sys_hexa_buf_to_str (sys_context, sha1_bin, _SHA1_80_SIZE);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error calculating SHA-1 sum for %d bytes"), buf_size);
    }

  return ret;
}

/**
 * lw6glb_sha1_hmac_80_str
 *
 * @sys_context: global system context
 * @key: a key (string)
 * @str: the string to calculate the checksum for
 *
 * Calculates an SHA-1 sum of a string, using key to seed calc.
 *
 * Return value: newly allocated string, containing 20 chars checksum.
 */
char *
lw6glb_sha1_hmac_80_str (lw6sys_context_t * sys_context, const char *key, const char *str)
{
  char *ret = NULL;

  if (!key)
    {
      key = _SHA1_EMPTY;
    }
  if (!str)
    {
      str = _SHA1_EMPTY;
    }
  ret = lw6glb_sha1_hmac_80_bin (sys_context, key, strlen (key), str, strlen (str));

  return ret;
}

/**
 * lw6glb_sha1_hmac_32_bin
 *
 * @sys_context: global system context
 * @key: the key buffer
 * @key_size: the key buffer size
 * @buf: the data to analyse
 * @buf_size: the size of data to analyse
 *
 * Calculates an SHA-1 sum of buffer, using key to seed calc.
 *
 * Return value: a 32-bit unsigned integer
 */
u_int32_t
lw6glb_sha1_hmac_32_bin (lw6sys_context_t * sys_context, const char *key, int key_size, const char *buf, int buf_size)
{
  u_int32_t ret = 0;
  char sha1_bin[_SHA1_SIZE];

  memset (sha1_bin, 0, _SHA1_SIZE);
  if (!hmac_sha1 ((void *) key, key_size, (void *) buf, buf_size, sha1_bin))
    {
      /*
       * We only convert the 10 first bytes, this way we reveal
       * less information when sending password hashes on the net.
       */
      ret = (u_int32_t) lw6sys_unserialize_int32 (sys_context, (unsigned char *) sha1_bin);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error calculating SHA-1 sum for %d bytes"), buf_size);
    }

  return ret;
}

/**
 * lw6glb_sha1_hmac_32_str
 *
 * @sys_context: global system context
 * @key: a key (string)
 * @str: the string to calculate the checksum for
 *
 * Calculates an SHA-1 sum of a string, using key to seed calc.
 *
 * Return value: a 32-bit unsigned integer
 */
u_int32_t
lw6glb_sha1_hmac_32_str (lw6sys_context_t * sys_context, const char *key, const char *str)
{
  u_int32_t ret = 0;

  if (!key)
    {
      key = _SHA1_EMPTY;
    }
  if (!str)
    {
      str = _SHA1_EMPTY;
    }
  ret = lw6glb_sha1_hmac_32_bin (sys_context, key, strlen (key), str, strlen (str));

  return ret;
}
