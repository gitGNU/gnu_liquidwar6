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

#include "msg.h"

#include <zlib.h>

#define _MSG_LEN_DUMP_MAX 4096
#define _IN_LEN_DUMP_MAX 4096

/*
 *  http://www.zlib.net/zlib_how.html
 */
static int
_encode_buffer_len (lw6sys_context_t * sys_context, int in_len)
{
  int out_len = 0;
  /*
   * "the destination buffer, which must be at least 0.1% larger
   * than sourceLen plus 12 bytes"
   *
   * By security, we use 1% and 24 bytes...
   */
  out_len = in_len + (in_len / 100) + 24;

  return out_len;
}

static int
_z_encode (lw6sys_context_t * sys_context, char *out_buf, int *out_buf_len, const char *msg, int msg_len)
{
  int ret = 0;
  uLongf _out_buf_len = (*out_buf_len);

  ret = compress ((Bytef *) out_buf, &_out_buf_len, (Bytef *) msg, msg_len);
  (*out_buf_len) = _out_buf_len;

  return ret;
}

static int
_z_decode (lw6sys_context_t * sys_context, char *out_buf, int *out_buf_len, const char *msg, int msg_len)
{
  int ret = 0;
  uLongf _out_buf_len = (*out_buf_len);

  ret = uncompress ((Bytef *) out_buf, &_out_buf_len, (Bytef *) msg, msg_len);
  (*out_buf_len) = _out_buf_len;

  return ret;
}

/**
 * lw6msg_z_encode
 *
 * @sys_context: global system context
 * @msg: message to encode
 * @limit: if under this limit (length in bytes), do not encode, return as is
 *
 * Z-encode a message, by "Z-encoding" we mean pass the string through
 * 1) zlib then 2) base64 encoding, this way we get a string without any
 * blank and/or special character, and of reasonnable length. There's
 * an optional limit *not* to encode anything, just when we know there
 * are no special characters to escape and string is small, it's useless
 * to fire this big artillery.
 *
 * Return value: newly allocated string, 0 terminated, NULL on error.
 */
char *
lw6msg_z_encode (lw6sys_context_t * sys_context, const char *msg, int limit)
{
  char *ret = NULL;
  int in_len = 0;
  int out_len = 0;
  int z_ret = Z_BUF_ERROR;
  char *out_buf;

  in_len = strlen (msg);
  if (in_len > limit)
    {
      out_len = _encode_buffer_len (sys_context, in_len);
      out_buf = (char *) LW6SYS_MALLOC (sys_context, out_len);
      if (out_buf)
	{
	  z_ret = _z_encode (sys_context, out_buf, &out_len, msg, in_len);
	  if (z_ret == Z_OK)
	    {
	      ret = lw6glb_base64_encode_bin_prefix (sys_context, out_buf, out_len, LW6MSG_Z_PREFIX);
	    }
	  else
	    {
	      switch (z_ret)
		{
		case Z_BUF_ERROR:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, buffer is too small (in_len=%d out_len=%d)"), in_len, out_len);
		  break;
		case Z_MEM_ERROR:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, not enough memory (in_len=%d out_len=%d)"), in_len, out_len);
		  break;
		default:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, ret=%d"), z_ret);
		  break;
		}
	    }
	  LW6SYS_FREE (sys_context, out_buf);
	}
    }
  else
    {
      ret = lw6sys_str_copy (sys_context, msg);
    }

  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("z-encode \"%s\" -> \"%s\""), msg, ret);
    }

  return ret;
}

/**
 * lw6msg_z_decode
 *
 * @sys_context: global system context
 * @msg: message to decode
 *
 * Z-decode a message, by "Z-encoding" we mean pass the string through
 * 1) zlib then 2) base64 encoding, this way we get a string without any
 * blank and/or special character, and of reasonnable length. This decode
 * string does it the reverse way, un64-encode the string then uncompress
 * it back to a readable string.
 *
 * Return value: newly allocated string, 0 terminated, NULL on error.
 */
char *
lw6msg_z_decode (lw6sys_context_t * sys_context, const char *msg)
{
  char *ret = NULL;
  int in_len = 0;
  int out_len = 0;
  int z_ret = Z_BUF_ERROR;
  char *in_buf = NULL;
  int out_alloc_len = 0;

  if (lw6sys_str_starts_with_no_case (sys_context, msg, LW6MSG_Z_PREFIX))
    {
      in_buf = lw6glb_base64_decode_bin_prefix (sys_context, &in_len, msg, LW6MSG_Z_PREFIX);
      if (in_buf)
	{
	  out_alloc_len = 1;	// too small, on purpose, to check auto-extend works
	  z_ret = Z_BUF_ERROR;
	  while (z_ret == Z_BUF_ERROR && !ret)
	    {
	      out_len = out_alloc_len;
	      ret = (char *) LW6SYS_MALLOC (sys_context, out_alloc_len);
	      if (ret)
		{
		  z_ret = _z_decode (sys_context, ret, &out_len, in_buf, in_len);
		  switch (z_ret)
		    {
		    case Z_OK:
		      /*
		       * Very important, we check we have the room for '\0'
		       */
		      if (out_len < out_alloc_len)
			{
			  // OK
			  ret[out_len] = '\0';
			}
		      else
			{
			  out_alloc_len++;
			  z_ret = Z_BUF_ERROR;
			}
		      break;
		    case Z_DATA_ERROR:
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("corrupted zlib data"));
		      break;
		    case Z_BUF_ERROR:
		      out_alloc_len += in_len + 1;
		      break;
		    }
		}
	      else
		{
		  z_ret = Z_MEM_ERROR;
		}
	      if (z_ret != Z_OK)
		{
		  if (ret)
		    {
		      LW6SYS_FREE (sys_context, ret);
		      ret = NULL;
		    }
		}
	    }
	  if (ret)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("z-decode \"%s\" -> \"%s\""), msg, ret);
	    }
	  else
	    {
	      switch (z_ret)
		{
		case Z_BUF_ERROR:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, buffer is too small (in_len=%d out_len=%d)"), in_len, out_len);
		  break;
		case Z_MEM_ERROR:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, not enough memory (in_len=%d out_len=%d)"), in_len, out_len);
		  break;
		case Z_DATA_ERROR:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, data error (in_len=%d out_len=%d)"), in_len, out_len);
		  break;
		case Z_STREAM_ERROR:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, stream error (in_len=%d out_len=%d)"), in_len, out_len);
		  break;
		default:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zlib error, ret=%d"), z_ret);
		  break;
		}
/*
* We don't log "very" long messages, this would
* make log file unusable in case of map dumps
*/
	      if (strlen (msg) < _MSG_LEN_DUMP_MAX)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("z-decode %d bytes msg was \"%s\""), (int) strlen (msg), msg);
		}
	      if (in_len < _IN_LEN_DUMP_MAX)
		{
		  char *in_hexa_str = NULL;

		  in_hexa_str = lw6sys_hexa_buf_to_str (sys_context, in_buf, in_len);
		  if (in_hexa_str)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("z-decode %d bytes in_hexa_str was \"%s\""), in_len, in_hexa_str);
		      LW6SYS_FREE (sys_context, in_hexa_str);
		    }
		}
	    }
	  LW6SYS_FREE (sys_context, in_buf);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to decode \"%s\" as base64 prefixed by \"%s\""), msg, LW6MSG_Z_PREFIX);
	}
    }
  else
    {
      /*
       * We consider it wasn't z-encoded, keep it the same
       */
      ret = lw6sys_str_copy (sys_context, msg);
    }

  return ret;
}
