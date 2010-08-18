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

/**
 * lw6glb_base64_encode_bin
 *
 * @buf: the data to encode
 * @size: the size of data to encode
 * 
 * Encodes data into base64. Memory allocation is done
 * automatically.
 *
 * Return value: newly allocated string.
 */
char *
lw6glb_base64_encode_bin (char *buf, int size)
{
  char *ret = NULL;
  int out_len = 0;

  out_len = BASE64_LENGTH (size);
  ret = (char *) LW6SYS_CALLOC (out_len + 1);
  if (ret)
    {
      base64_encode (buf, size, ret, out_len);
    }

  return ret;
}

/**
 * lw6glb_base64_decode_bin
 *
 * @size: the size of the decoded data
 * @base64_str: the string to decode
 * 
 * Decodes data from base64. Memory allocation is done
 * automatically. Note that this function only works for
 * strings, other data might not be handled correctly.
 *
 * Return value: newly allocated pointer, NULL on error.
 */
char *
lw6glb_base64_decode_bin (int *size, char *base64_str)
{
  char *ret = NULL;
  int in_len = 0;
  size_t out_len = 0;
  int decode_ret = 0;

  in_len = strlen (base64_str);
  out_len = in_len;
  ret = (char *) LW6SYS_CALLOC (out_len + 1);
  if (ret)
    {
      decode_ret = base64_decode (base64_str, in_len, ret, &out_len);
      if (decode_ret == 1)
	{
	  (*size) = out_len;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("base64 decode failed, decode_ret=%d"), decode_ret);
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6glb_base64_encode_str
 *
 * @str: the string to encode
 *
 * Encodes a string into base64.
 *
 * Return value: newly allocated string.
 */
char *
lw6glb_base64_encode_str (char *str)
{
  char *ret = NULL;

  ret = lw6glb_base64_encode_bin (str, strlen (str));

  return ret;
}

/**
 * lw6glb_base64_decode_str
 *
 * @str: the string to decode
 *
 * Deodes a string from base64.
 *
 * Return value: newly allocated string, NULL on error.
 */
char *
lw6glb_base64_decode_str (char *str)
{
  char *ret = NULL;
  int size = 0;

  ret = lw6glb_base64_decode_bin (&size, str);

  return ret;
}
