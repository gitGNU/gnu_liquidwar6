/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#define _SHA1_KEY ""
#define _SHA1_SIZE 20
#define _SHA1_80_SIZE 10

/**
 * lw6glb_sha1_hmac_80_bin
 *
 * @key: a key (string)
 * @buf: the data to analyse
 * @size: the size of data to analyse
 * 
 * Calculates an SHA-1 sum of buffer, using key to seed calc.
 *
 * Return value: newly allocated string, containing 20 chars checksum.
 */
char *
lw6glb_sha1_hmac_80_bin (char *key, char *buf, int size)
{
  char *ret = NULL;
  char sha1_bin[_SHA1_SIZE];

  memset (sha1_bin, 0, _SHA1_SIZE);
  if (!key)
    {
      key = _SHA1_KEY;
    }
  if (!hmac_sha1 (key, strlen (key), (void *) buf, size, sha1_bin))
    {
      /*
       * We only convert the 10 first bytes, this way we reveal
       * less information when sending password hashes on the net.
       */
      ret = lw6sys_hexa_buf_to_str (sha1_bin, _SHA1_80_SIZE);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("error calculating SHA-1 sum for %d bytes"), size);
    }

  return ret;
}

/**
 * lw6glb_sha1_hmac_80_str
 *
 * @key: a key (string)
 * @str: the string to calculate the checksum for 
 * 
 * Calculates an SHA-1 sum of a string, using key to seed calc.
 *
 * Return value: newly allocated string, containing 20 chars checksum.
 */
char *
lw6glb_sha1_hmac_80_str (char *key, char *str)
{
  char *ret = NULL;

  ret = lw6glb_sha1_hmac_80_bin (key, str, strlen (str));

  return ret;
}
