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

#include <string.h>

#include "sys.h"

#define RESIZE_FACTOR 1.5f
#define RESIZE_PLUS 1024
#define HEXA_0 "00"

/**
 * lw6sys_hexa_serializer_new
 *
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
lw6sys_hexa_serializer_new (char *hexa_string)
{
  lw6sys_hexa_serializer_t *hexa_serializer = NULL;

  hexa_serializer =
    (lw6sys_hexa_serializer_t *)
    LW6SYS_CALLOC (sizeof (lw6sys_hexa_serializer_t));
  if (hexa_serializer)
    {
      if (!hexa_string)
	{
	  hexa_string = "";
	}
      hexa_serializer->pos = strlen (hexa_string);
      hexa_serializer->buf_size = hexa_serializer->pos + 1;
      hexa_serializer->buf = lw6sys_str_copy (hexa_string);
    }

  if (!hexa_serializer->buf)
    {
      LW6SYS_FREE (hexa_serializer);
      hexa_serializer = NULL;
    }

  return hexa_serializer;
}

/**
 * lw6sys_hexa_serializer_free
 *
 * @hexa_serializer: an hexa serializer object
 *
 * Frees an hexa serializer object.
 *
 * Return value: none.
 */
void
lw6sys_hexa_serializer_free (lw6sys_hexa_serializer_t * hexa_serializer)
{
  if (hexa_serializer->buf)
    {
      LW6SYS_FREE (hexa_serializer->buf);
    }
  LW6SYS_FREE (hexa_serializer);
}

/**
 * lw6sys_hexa_serializer_rewind
 *
 * @hexa_serializer: an hexa serializer object
 *
 * Rewinds the serializer pointer, that is, make it point to start.
 * Usefull before calling pop functions, when one wants to be
 * sure to get the first object.
 *
 * Return value: none.
 */
void
lw6sys_hexa_serializer_rewind (lw6sys_hexa_serializer_t * hexa_serializer)
{
  hexa_serializer->pos = 0;
}

/**
 * lw6sys_hexa_serializer_eof
 *
 * @hexa_serializer: an hexa serializer object
 *
 * Tests wether we're at EOF. Usefull when one wants to know
 * if there's still some data or if all objects have been correctly
 * popped.
 *
 * Return value: 1 if at end of file, 0 if not.
 */
int
lw6sys_hexa_serializer_eof (lw6sys_hexa_serializer_t * hexa_serializer)
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
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("hexa serializer pos past end (%d/%d)"),
		  hexa_serializer->pos, len);
    }

  return eof;
}

/**
 * lw6sys_hexa_serializer_as_string
 *
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
lw6sys_hexa_serializer_as_string (lw6sys_hexa_serializer_t * hexa_serializer)
{
  char *hexa_string = NULL;

  hexa_string = lw6sys_str_copy (hexa_serializer->buf);

  return hexa_string;
}

static int
check_size (lw6sys_hexa_serializer_t * hexa_serializer, int required)
{
  int ret = 0;

  if (hexa_serializer->pos + required + 1 <= hexa_serializer->buf_size)
    {
      ret = 1;
    }

  return ret;
}

static int
resize (lw6sys_hexa_serializer_t * hexa_serializer, int required)
{
  int ret = 0;
  int new_size = 0;

  if (!check_size (hexa_serializer, required))
    {
      new_size =
	(hexa_serializer->buf_size * RESIZE_FACTOR) + RESIZE_PLUS + required;
      hexa_serializer->buf = LW6SYS_REALLOC (hexa_serializer->buf, new_size);
      if (hexa_serializer->buf)
	{
	  hexa_serializer->buf_size = new_size;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("can't resize hexa_serializer (new_size=%d), expect serious trouble"),
		      new_size);
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
push_raw (lw6sys_hexa_serializer_t * hexa_serializer, u_int8_t * buf,
	  int size)
{
  int ret = 0;
  int i;

  if (resize (hexa_serializer, size * 2))
    {
      for (i = 0; i < size; ++i, hexa_serializer->pos += 2)
	{
	  snprintf (hexa_serializer->buf + hexa_serializer->pos, 3, "%02x",
		    (int) buf[i]);
	}
      if (hexa_serializer->pos < hexa_serializer->buf_size &&
	  !(hexa_serializer->pos & 0x01))
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("inconsistent hexa_serializer after push, pos=%d, buf_size=%d"),
		      hexa_serializer->pos, hexa_serializer->buf_size);
	}
    }

  return ret;
}

static int
pop_raw (lw6sys_hexa_serializer_t * hexa_serializer, u_int8_t * buf, int size)
{
  int ret = 0;
  int i;
  int j;
  char hexa2[3] = { 0, 0, 0 };	// third 0 is *very* important

  if (check_size (hexa_serializer, size * 2))
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
      if (hexa_serializer->pos < hexa_serializer->buf_size &&
	  !(hexa_serializer->pos & 0x01))
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("inconsistent hexa_serializer after pop, pos=%d, buf_size=%d"),
		      hexa_serializer->pos, hexa_serializer->buf_size);
	}
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int64
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a 64 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int64 (lw6sys_hexa_serializer_t * hexa_serializer,
				   int64_t value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int64_t)];

  lw6sys_serialize_int64 (buffer, value);
  ret = push_raw (hexa_serializer, buffer, sizeof (int64_t));

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int32
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a 32 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int32 (lw6sys_hexa_serializer_t * hexa_serializer,
				   int32_t value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int32_t)];

  lw6sys_serialize_int32 (buffer, value);
  ret = push_raw (hexa_serializer, buffer, sizeof (int32_t));

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int16
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a 16 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int16 (lw6sys_hexa_serializer_t * hexa_serializer,
				   int16_t value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int16_t)];

  lw6sys_serialize_int16 (buffer, value);
  ret = push_raw (hexa_serializer, buffer, sizeof (int16_t));

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_int8
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes an 8 bit integer in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_int8 (lw6sys_hexa_serializer_t * hexa_serializer,
				  int8_t value)
{
  int ret = 0;

  ret = push_raw (hexa_serializer, (u_int8_t *) & value, 1);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_float
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a floating point value in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_float (lw6sys_hexa_serializer_t * hexa_serializer,
				   float value)
{
  int ret = 0;
  char *str = NULL;

  str = lw6sys_ftoa (value);
  if (str)
    {
      ret = lw6sys_hexa_serializer_push_str (hexa_serializer, str);
      LW6SYS_FREE (str);
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_str
 *
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
lw6sys_hexa_serializer_push_str (lw6sys_hexa_serializer_t * hexa_serializer,
				 char *value)
{
  int ret = 0;

  ret = push_raw (hexa_serializer, (u_int8_t *) value, strlen (value) + 1);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_xyz
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a lw6sys_xyz_t structure in the serializer object.
 * Calling this avoids calling push for 2 integers separately.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_xyz (lw6sys_hexa_serializer_t *
				 hexa_serializer, lw6sys_xyz_t value)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int16 (hexa_serializer, value.x);
  ret = ret && lw6sys_hexa_serializer_push_int16 (hexa_serializer, value.y);
  ret = ret && lw6sys_hexa_serializer_push_int16 (hexa_serializer, value.z);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_whd
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a lw6sys_whd_t structure in the serializer object.
 * Calling this avoids calling push for 2 integers separately.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_whd (lw6sys_hexa_serializer_t *
				 hexa_serializer, lw6sys_whd_t value)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int16 (hexa_serializer, value.w);
  ret = ret && lw6sys_hexa_serializer_push_int16 (hexa_serializer, value.h);
  ret = ret && lw6sys_hexa_serializer_push_int16 (hexa_serializer, value.d);

  return ret;
}

/**
 * lw6sys_hexa_serializer_push_color
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to push
 *
 * Pushes a color structure in the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_push_color (lw6sys_hexa_serializer_t *
				   hexa_serializer, lw6sys_color_8_t value)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_int8 (hexa_serializer, value.r);
  ret = ret && lw6sys_hexa_serializer_push_int8 (hexa_serializer, value.g);
  ret = ret && lw6sys_hexa_serializer_push_int8 (hexa_serializer, value.b);
  ret = ret && lw6sys_hexa_serializer_push_int8 (hexa_serializer, value.a);

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int64
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a 64 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int64 (lw6sys_hexa_serializer_t * hexa_serializer,
				  int64_t * value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int64_t)];

  if (check_size (hexa_serializer, sizeof (int64_t) * 2))
    {
      if (pop_raw (hexa_serializer, buffer, sizeof (int64_t)))
	{
	  (*value) = lw6sys_unserialize_int64 (buffer);
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to pop int64 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int32
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a 32 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int32 (lw6sys_hexa_serializer_t * hexa_serializer,
				  int32_t * value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int32_t)];

  if (check_size (hexa_serializer, sizeof (int32_t) * 2))
    {
      if (pop_raw (hexa_serializer, buffer, sizeof (int32_t)))
	{
	  (*value) = lw6sys_unserialize_int32 (buffer);
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to pop int32 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int16
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a 16 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int16 (lw6sys_hexa_serializer_t * hexa_serializer,
				  int16_t * value)
{
  int ret = 0;
  u_int8_t buffer[sizeof (int16_t)];

  if (check_size (hexa_serializer, sizeof (int16_t) * 2))
    {
      if (pop_raw (hexa_serializer, buffer, sizeof (int16_t)))
	{
	  (*value) = lw6sys_unserialize_int16 (buffer);
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to pop int16 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_int8
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops an 8 bit integer from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_int8 (lw6sys_hexa_serializer_t * hexa_serializer,
				 int8_t * value)
{
  int ret = 0;

  if (check_size (hexa_serializer, 2))
    {
      if (pop_raw (hexa_serializer, (u_int8_t *) value, 1))
	{
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to pop int8 from hexa_serializer"));
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_float
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a floating point value from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_float (lw6sys_hexa_serializer_t * hexa_serializer,
				  float *value)
{
  int ret = 0;
  char *str = NULL;

  if (lw6sys_hexa_serializer_pop_str (hexa_serializer, &str))
    {
      if (str)
	{
	  (*value) = lw6sys_atof (str);
	  LW6SYS_FREE (str);
	  ret = 1;
	}
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_str
 *
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
lw6sys_hexa_serializer_pop_str (lw6sys_hexa_serializer_t * hexa_serializer,
				char **value)
{
  int ret = 0;
  char *end_zero = NULL;
  int offset_zero;
  int len;
  u_int8_t *buf = NULL;
  int pos;
  int i;

  for (pos = hexa_serializer->pos;
       (pos < hexa_serializer->buf_size - 2) && (!end_zero); pos += 2)
    {
      if (hexa_serializer->buf[pos] == '0'
	  && hexa_serializer->buf[pos + 1] == '0')
	{
	  end_zero = (char *) (hexa_serializer->buf + pos);
	}
    }
  if (end_zero)
    {
      offset_zero = (end_zero - hexa_serializer->buf) - hexa_serializer->pos;
      len = offset_zero / 2;
      buf = (u_int8_t *) LW6SYS_CALLOC (len + 1);
      if (buf)
	{
	  ret = 1;
	  for (i = 0; i < len + 1; ++i)
	    {
	      ret = ret && pop_raw (hexa_serializer, buf + i, 1);
	    }
	  buf[len] = '\0';	// just to be sure

	  if (ret && value)
	    {
	      (*value) = (char *) buf;
	    }
	  else
	    {
	      LW6SYS_FREE (buf);
	    }
	  buf = NULL;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("unable to find \"%s\""), HEXA_0);
    }

  if (buf && !ret)
    {
      LW6SYS_FREE (buf);
      buf = NULL;
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_xyz
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a lw6sys_xyz_t structure from the serializer object.
 * Avoids calling two integer pops.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_xyz (lw6sys_hexa_serializer_t *
				hexa_serializer, lw6sys_xyz_t * value)
{
  int ret = 1;
  int16_t s;

  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &s);
  if (ret)
    {
      value->x = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &s);
  if (ret)
    {
      value->y = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &s);
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
lw6sys_hexa_serializer_pop_whd (lw6sys_hexa_serializer_t *
				hexa_serializer, lw6sys_whd_t * value)
{
  int ret = 1;
  int16_t s;

  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &s);
  if (ret)
    {
      value->w = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &s);
  if (ret)
    {
      value->h = s;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &s);
  if (ret)
    {
      value->d = s;
    }

  return ret;
}

/**
 * lw6sys_hexa_serializer_pop_color
 *
 * @hexa_serializer: an hexa serializer object
 * @value: value to pop (returned value)
 *
 * Pops a color from the serializer object.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6sys_hexa_serializer_pop_color (lw6sys_hexa_serializer_t *
				  hexa_serializer, lw6sys_color_8_t * value)
{
  int ret = 1;
  int8_t c;

  ret = ret && lw6sys_hexa_serializer_pop_int8 (hexa_serializer, &c);
  if (ret)
    {
      value->r = c;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int8 (hexa_serializer, &c);
  if (ret)
    {
      value->g = c;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int8 (hexa_serializer, &c);
  if (ret)
    {
      value->b = c;
    }
  ret = ret && lw6sys_hexa_serializer_pop_int8 (hexa_serializer, &c);
  if (ret)
    {
      value->a = c;
    }

  return ret;
}
