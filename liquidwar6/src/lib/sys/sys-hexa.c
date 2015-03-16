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

#define RESIZE_FACTOR 1.5f
#define RESIZE_PLUS 1024
#define HEXA_0 "00"

/**
 * lw6sys_hexa_serializer_new
 *
 * @sys_context: global system context
 * @hexa_string: an initialization string, can be NULL.
 *
 * Creates an hexa serializer object. It can be initialized or not,
 * if an initialization string is provided it must of course be
 * valid hexadecimal ascii code, and all serialized content will
 * simply be appended to it.
 *
 * Return value: a newly allocated object.
 */
lw6sys_hexa_serializer_t *
lw6sys_hexa_serializer_new (lw6sys_context_t * sys_context, const char *hexa_string)
{
  lw6sys_hexa_serializer_t *hexa_serializer = NULL;

  hexa_serializer = (lw6sys_hexa_serializer_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_hexa_serializer_t));
  if (hexa_serializer)
    {
      if (!hexa_string)
	{
	  hexa_string = "";
	}
      hexa_serializer->pos = strlen (hexa_string);
      hexa_serializer->buf_size = hexa_serializer->pos + 1;
      hexa_serializer->buf = lw6sys_str_copy (sys_context, hexa_string);
    }

  if (!hexa_serializer->buf)
    {
      LW6SYS_FREE (sys_context, hexa_serializer);
      hexa_serializer = NULL;
    }

  return hexa_serializer;
}

/**
 * lw6sys_hexa_serializer_free
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 *
 * Frees an hexa serializer object.
 *
 * Return value: none.
 */
void
lw6sys_hexa_serializer_free (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer)
{
  if (hexa_serializer->buf)
    {
      LW6SYS_FREE (sys_context, hexa_serializer->buf);
    }
  LW6SYS_FREE (sys_context, hexa_serializer);
}

/**
 * lw6sys_hexa_serializer_rewind
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 *
 * Rewinds the serializer pointer, that is, make it point to start.
 * Usefull before calling pop functions, when one wants to be
 * sure to get the first object.
 *
 * Return value: none.
 */
void
lw6sys_hexa_serializer_rewind (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer)
{
  hexa_serializer->pos = 0;
}

/**
 * lw6sys_hexa_serializer_eof
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 *
 * Tests wether we're at EOF. Usefull when one wants to know
 * if there's still some data or if all objects have been correctly
 * popped.
 *
 * Return value: 1 if at end of file, 0 if not.
 */
int
lw6sys_hexa_serializer_eof (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer)
{
  int eof = 0;
  int len = 0;

  if (hexa_serializer->buf)
    {
      len = strlen (hexa_serializer->buf);
    }
  if (hexa_serializer->pos == len)
    {
      eof = 1;
    }
  else if (hexa_serializer->pos > len)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("hexa serializer pos past end (%d/%d)"), hexa_serializer->pos, len);
    }

  return eof;
}

/**
 * lw6sys_hexa_serializer_as_string
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 *
 * Exports the current content of the serializer as a string.
 * String can then safely be sent on the network, for instance.
 * String is copied from internal value, so it's safe to use
 * it after serializer has been freed or modified.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_hexa_serializer_as_string (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer)
{
  char *hexa_string = NULL;

  hexa_string = lw6sys_str_copy (sys_context, hexa_serializer->buf);

  return hexa_string;
}

static int
check_size (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int required)
{
  int ret = 0;

  if (hexa_serializer->pos + required + 1 <= hexa_serializer->buf_size)
    {
      ret = 1;
    }

  return ret;
}

static int
resize (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int required)
{
  int ret = 0;
  int new_size = 0;

  if (!check_size (sys_context, hexa_serializer, required))
    {
      new_size = (hexa_serializer->buf_size * RESIZE_FACTOR) + RESIZE_PLUS + required;
      hexa_serializer->buf = LW6SYS_REALLOC (sys_context, hexa_serializer->buf, new_size);
      if (hexa_serializer->buf)
	{
	  hexa_serializer->buf_size = new_size;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't resize hexa_serializer (new_size=%d), expect serious trouble"), new_size);
	}
    }
  else
    {
      // nothing to do...
      ret = 1;
    }

  return ret;
}

static int
push_raw (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, u_int8_t * buf, int size)
{
  int ret = 0;
  int i;

  if (resize (sys_context, hexa_serializer, size * 2))
    {
      for (i = 0; i < size; ++i, hexa_serializer->pos += 2)
	{
	  snprintf (hexa_serializer->buf + hexa_serializer->pos, 3, "%02x", (int) buf[i]);
	}
      if (hexa_serializer->pos < hexa_serializer->buf_size && !(hexa_serializer->pos & 0x01))
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("inconsistent hexa_serializer after push, pos=%d, buf_size=%d"), hexa_serializer->pos, hexa_serializer->buf_size);
	}
    }

  return ret;
}

static int
pop_raw (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, u_int8_t * buf, int size)
{
  int ret = 0;
  int i;
  int j;
  char hexa2[3] = { 0, 0, 0 };	// third 0 is *very* important

  if (check_size (sys_context, hexa_serializer, size * 2))
    {
      for (i = 0; i < size; ++i, hexa_serializer->pos += 2)
	{
	  j = 0;
	  memcpy (hexa2, hexa_serializer->buf + hexa_serializer->pos, 2);
	  /*
	   * Performing sscanf on long strings is a performance killer
	   * for sscanf calls strlen internally to get the length
	   * of the string...
	   */
	  sscanf (hexa2, "%02x", &j);
	  buf[i] = (u_int8_t) j;
	}
      if (hexa_serializer->pos < hexa_serializer->buf_size && !(hexa_serializer->pos & 0x01))
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("inconsistent hexa_serializer after pop, pos=%d, buf_size=%d"), hexa_serializer->pos, hexa_serializer->buf_size);
	}
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int64
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a 64 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int64 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int64_t value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int64_t)];

  lw6sys_serialize_int64 (sys_context, buffer, value);
  ret = push_raw (sys_context, hexa_serializer, buffer, sizeof (int64_t));

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int32
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a 32 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int32 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int32_t value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int32_t)];

  lw6sys_serialize_int32 (sys_context, buffer, value);
  ret = push_raw (sys_context, hexa_serializer, buffer, sizeof (int32_t));

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int16
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a 16 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int16 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int16_t value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int16_t)];

  lw6sys_serialize_int16 (sys_context, buffer, value);
  ret = push_raw (sys_context, hexa_serializer, buffer, sizeof (int16_t));

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int8
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes an 8 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int8 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int8_t value)
{
  int ret = 0;

  ret = push_raw (sys_context, hexa_serializer, (u_int8_t *) & value, 1);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_float
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a floating point value in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_float (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, float value)
{
  int ret = 0;
  char *str = NULL;

  str = lw6sys_ftoa (sys_context, value);
  if (str)
    {
      ret = lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, str);
      LW6SYS_FREE (sys_context, str);
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_str
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a string in the serializer object. Note that the string
 * is not directly copied in the serializer, instead all its characters
 * are converted to their ASCII equivalent, then appended.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_str (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const char *value)
{
  int ret = 0;

  ret = push_raw (sys_context, hexa_serializer, (u_int8_t *) value, strlen (value) + 1);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_xyz
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a lw6sys_xyz_t structure in the serializer object.
 * Calling this avoids calling push for 2 integers separately.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_xyz (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6sys_xyz_t value)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, value.x);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, value.y);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, value.z);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_whd
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a lw6sys_whd_t structure in the serializer object.
 * Calling this avoids calling push for 2 integers separately.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_whd (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6sys_whd_t value)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, value.w);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, value.h);
  ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, value.d);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_color
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a color structure in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_color (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6sys_color_8_t value)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, value.r);
  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, value.g);
  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, value.b);
  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, value.a);

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int64
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a 64 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int64 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int64_t * value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int64_t)];

  if (check_size (sys_context, hexa_serializer, sizeof (int64_t) * 2))
    {
      if (pop_raw (sys_context, hexa_serializer, buffer, sizeof (int64_t)))
	{
	  (*value) = lw6sys_unserialize_int64 (sys_context, buffer);
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to pop int64 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int32
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a 32 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int32 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int32_t * value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int32_t)];

  if (check_size (sys_context, hexa_serializer, sizeof (int32_t) * 2))
    {
      if (pop_raw (sys_context, hexa_serializer, buffer, sizeof (int32_t)))
	{
	  (*value) = lw6sys_unserialize_int32 (sys_context, buffer);
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to pop int32 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int16
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a 16 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int16 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int16_t * value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int16_t)];

  if (check_size (sys_context, hexa_serializer, sizeof (int16_t) * 2))
    {
      if (pop_raw (sys_context, hexa_serializer, buffer, sizeof (int16_t)))
	{
	  (*value) = lw6sys_unserialize_int16 (sys_context, buffer);
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to pop int16 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int8
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops an 8 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int8 (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, int8_t * value)
{
  int ret = 0;

  if (check_size (sys_context, hexa_serializer, 2))
    {
      if (pop_raw (sys_context, hexa_serializer, (u_int8_t *) value, 1))
	{
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to pop int8 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_float
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a floating point value from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_float (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, float *value)
{
  int ret = 0;
  char *str = NULL;

  if (lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &str))
    {
      if (str)
	{
	  (*value) = lw6sys_atof (sys_context, str);
	  LW6SYS_FREE (sys_context, str);
	  ret = 1;
	}
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_str
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a string from the serializer object. The returned value
 * is a newly allocated pointer, which must be freed, you don't
 * need to provide a buffer, just a valid pointer on a NULL pointer.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_str (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, char **value)
{
  int ret = 0;
  char *end_zero = NULL;
  int offset_zero;
  int len;
  u_int8_t *buf = NULL;
  int pos;
  int i;

  for (pos = hexa_serializer->pos; (pos < hexa_serializer->buf_size - 2) && (!end_zero); pos += 2)
    {
      if (hexa_serializer->buf[pos] == '0' && hexa_serializer->buf[pos + 1] == '0')
	{
	  end_zero = (char *) (hexa_serializer->buf + pos);
	}
    }
  if (end_zero)
    {
      offset_zero = (end_zero - hexa_serializer->buf) - hexa_serializer->pos;
      len = offset_zero / 2;
      buf = (u_int8_t *) LW6SYS_CALLOC (sys_context, len + 1);
      if (buf)
	{
	  ret = 1;
	  for (i = 0; i < len + 1; ++i)
	    {
	      ret = ret && pop_raw (sys_context, hexa_serializer, buf + i, 1);
	    }
	  buf[len] = '\0';	// just to be sure

	  if (ret && value)
	    {
	      (*value) = (char *) buf;
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, buf);
	    }
	  buf = NULL;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find \"%s\""), HEXA_0);
    }

  if (buf && !ret)
    {
      LW6SYS_FREE (sys_context, buf);
      buf = NULL;
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_xyz
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a lw6sys_xyz_t structure from the serializer object.
 * Avoids calling two integer pops.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_xyz (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6sys_xyz_t * value)
{
  int ret = 1;
  int16_t s;

  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &s);
  if (ret)
    {
      value->x = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &s);
  if (ret)
    {
      value->y = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &s);
  if (ret)
    {
      value->z = s;
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_whd
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a lw6sys_whd_t structure from the serializer object.
 * Avoids calling two integer pops.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_whd (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6sys_whd_t * value)
{
  int ret = 1;
  int16_t s;

  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &s);
  if (ret)
    {
      value->w = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &s);
  if (ret)
    {
      value->h = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &s);
  if (ret)
    {
      value->d = s;
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_color
 *
 * @sys_context: global system context
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a color from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_color (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6sys_color_8_t * value)
{
  int ret = 1;
  int8_t c;

  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &c);
  if (ret)
    {
      value->r = c;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &c);
  if (ret)
    {
      value->g = c;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &c);
  if (ret)
    {
      value->b = c;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &c);
  if (ret)
    {
      value->a = c;
    }

  return ret;
}

/**
 * lw6sys_hexa_str_to_buf
 *
 * @sys_context: global system context
 * @buf: binary buffer to convert
 * @size: binary buffer length
 * @str: the source string
 *
 * Converts the stringified hexa representation of a string to
 * its source binary buffer. Buffer must be exactly @strlen(str)/2
 *
 * Return value: 1 on success
 */
int
lw6sys_hexa_str_to_buf (lw6sys_context_t * sys_context, void *buf, int size, const char *str)
{
  char *buffer = (char *) buf;
  int ret = 0;
  int i = 0, j = 0;
  char hexa2[3] = { 0, 0, 0 };	// third 0 is *very* important

  if (strlen (str) == 2 * size)
    {
      for (i = 0; i < size; ++i)
	{
	  memcpy (hexa2, str + 2 * i, 2);
	  sscanf (hexa2, "%02x", &j);
	  buffer[i] = (u_int8_t) j;
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("incorrect string (bad size) \"%s\" expected %d but got %d"), str, 2 * size, (int) strlen (str));
    }

  return ret;
}

/**
 * lw6sys_hexa_buf_to_str
 *
 * @sys_context: global system context
 * @buf: the buffer to stringify
 * @size: the length of the buffer
 *
 * Transforms a binary buffer into its hexa representation.
 *
 * Return value: newly allocated string.
 */
char *
lw6sys_hexa_buf_to_str (lw6sys_context_t * sys_context, void *buf, int size)
{
  char *buffer = (char *) buf;
  char *str = NULL;
  int i = 0;
  u_int8_t c = 0;

  str = (char *) LW6SYS_MALLOC (sys_context, 2 * size + 1);
  if (str)
    {
      for (i = 0; i < size; ++i)
	{
	  c = buffer[i];
	  snprintf (str + 2 * i, 3, "%02x", (u_int32_t) c);
	}
      str[2 * size] = '\0';
    }

  return str;
}

/**
 * lw6sys_hexa_str_to_ptr
 *
 * @sys_context: global system context
 * @str: the string containing an hexa representation of pointer
 *
 * Transforms a string into a pointer, this is typically used
 * to store pointers in temporary agnostic storage such as
 * a database. Beware not to use that to exchange data with
 * other computers and/or use it for persistent data. This
 * is a high-risk function as it lets you do real dirty stuff
 * but it really does save time compared to using a key returned
 * by the database engine and then search this key in a user-space
 * hash table. Direct pointer access is definitely faster.
 *
 * Return value: the pointer, or NULL is str is invalid.
 */
void *
lw6sys_hexa_str_to_ptr (lw6sys_context_t * sys_context, const char *str)
{
  void *ptr = NULL;

  if (!lw6sys_hexa_str_to_buf (sys_context, &ptr, sizeof (void *), str))
    {
      ptr = NULL;
    }

  return ptr;
}

/**
 * lw6sys_hexa_ptr_to_str
 *
 * @sys_context: global system context
 * @ptr: pointer to convert into string representation
 *
 * Transforms a pointer into a string, this is typically used
 * to store pointers in temporary agnostic storage such as
 * a database. Beware not to use that to exchange data with
 * other computers and/or use it for persistent data. This
 * is a high-risk function as it lets you do real dirty stuff
 * but it really does save time compared to using a key returned
 * by the database engine and then search this key in a user-space
 * hash table. Direct pointer access is definitely faster.
 *
 * Return value: the string, can be NULL on errror, must be freed.
 */
char *
lw6sys_hexa_ptr_to_str (lw6sys_context_t * sys_context, void *ptr)
{
  char *str = NULL;

  str = lw6sys_hexa_buf_to_str (sys_context, &ptr, sizeof (void *));

  return str;
}
