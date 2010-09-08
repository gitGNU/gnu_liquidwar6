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

  ret = lw6glb_base64_encode_bin_prefix (buf, size, NULL);

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

  ret = lw6glb_base64_decode_bin_prefix (size, base64_str, NULL);

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

  ret = lw6glb_base64_encode_str_prefix (str, NULL);

  return ret;
}

/**
 * lw6glb_base64_decode_str
 *
 * @str: the string to decode
 *
 * Decodes a string from base64.
 *
 * Return value: newly allocated string, NULL on error.
 */
char *
lw6glb_base64_decode_str (char *str)
{
  char *ret = NULL;

  ret = lw6glb_base64_decode_str_prefix (str, NULL);

  return ret;
}

/**
 * lw6glb_base64_encode_bin_prefix
 *
 * @buf: the data to encode
 * @size: the size of data to encode
 * @prefix: a prefix string
 * 
 * Encodes data into base64. Memory allocation is done
 * automatically.  The encoded string will
 * be prefixed with @prefix.
 *
 * Return value: newly allocated string.
 */
char *
lw6glb_base64_encode_bin_prefix (char *buf, int size, char *prefix)
{
  char *ret = NULL;
  int out_len = 0;
  int prefix_len = 0;

  if (prefix)
    {
      prefix_len = strlen (prefix);
    }

  out_len = BASE64_LENGTH (size);
  ret = (char *) LW6SYS_MALLOC (prefix_len + out_len + 1);
  if (ret)
    {
      if (prefix && prefix_len > 0)
	{
	  memcpy (ret, prefix, prefix_len);
	}
      base64_encode (buf, size, ret + prefix_len, out_len);
      ret[prefix_len + out_len] = '\0';
    }

  return ret;
}

/**
 * lw6glb_base64_decode_bin_prefix
 *
 * @size: the size of the decoded data
 * @base64_str: the string to decode
 * @prefix: a prefix string
 * 
 * Decodes data from base64. Memory allocation is done
 * automatically. Note that this function only works for
 * strings, other data might not be handled correctly.
 * The encoded is expected
 * to start with prefix @prefix and then contain base64 data.
 *
 * Return value: newly allocated pointer, NULL on error.
 */
char *
lw6glb_base64_decode_bin_prefix (int *size, char *base64_str, char *prefix)
{
  char *ret = NULL;
  int in_len = 0;
  size_t out_len = 0;
  int decode_ret = 0;
  int prefix_len = 0;
  int c = '\0';

  if (prefix)
    {
      prefix_len = strlen (prefix);
    }
  if (prefix && prefix_len > 0)
    {
      if (lw6sys_str_starts_with (base64_str, prefix))
	{
	  base64_str += prefix_len;
	}
      else
	{
	  base64_str = NULL;
	}
    }

  if (base64_str)
    {
      /*
       * We calculate strlen manually else program can sometimes pathetically
       * fail on stupid trailing "\n" or such.
       */
      in_len = 0;
      while (((c = base64_str[in_len]) != '\0') && !lw6sys_chr_is_space (c)
	     && !lw6sys_chr_is_eol (c))
	{
	  in_len++;
	}
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
			  _x_ ("base64 decode failed, decode_ret=%d"),
			  decode_ret);
	      LW6SYS_FREE (ret);
	      ret = NULL;
	    }
	}
    }

  return ret;
}

/**
 * lw6glb_base64_encode_str_prefix
 *
 * @str: the string to encode
 * @prefix: a prefix string
 *
 * Encodes a string into base64. The encoded string will
 * be prefixed with @prefix.
 *
 * Return value: newly allocated string.
 */
char *
lw6glb_base64_encode_str_prefix (char *str, char *prefix)
{
  char *ret = NULL;

  ret = lw6glb_base64_encode_bin_prefix (str, strlen (str), prefix);

  return ret;
}

/**
 * lw6glb_base64_decode_str_prefix
 *
 * @str: the string to decode
 * @prefix: a prefix string
 *
 * Decodes a string from base64. The encoded is expected
 * to start with prefix @prefix and then contain base64 data.
 *
 * Return value: newly allocated string, NULL on error.
 */
char *
lw6glb_base64_decode_str_prefix (char *str, char *prefix)
{
  char *ret = NULL;
  int size = 0;

  ret = lw6glb_base64_decode_bin_prefix (&size, str, prefix);

  return ret;
}
