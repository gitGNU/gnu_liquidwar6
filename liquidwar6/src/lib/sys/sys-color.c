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

#include <math.h>

#include "sys.h"

#define FLOAT2CHAR_FACTOR 255.0f
#define XYZ_CONE_Z 2.0f

lw6sys_color_f_t LW6SYS_COLOR_F_WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
lw6sys_color_f_t LW6SYS_COLOR_F_BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
lw6sys_color_f_t LW6SYS_COLOR_F_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
lw6sys_color_f_t LW6SYS_COLOR_F_GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
lw6sys_color_f_t LW6SYS_COLOR_F_BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
lw6sys_color_8_t LW6SYS_COLOR_8_WHITE = { 255, 255, 255, 255 };
lw6sys_color_8_t LW6SYS_COLOR_8_BLACK = { 0, 0, 0, 255 };
lw6sys_color_8_t LW6SYS_COLOR_8_RED = { 255, 0, 0, 255 };
lw6sys_color_8_t LW6SYS_COLOR_8_GREEN = { 0, 255, 0, 255 };
lw6sys_color_8_t LW6SYS_COLOR_8_BLUE = { 0, 0, 255, 255 };
lw6sys_color_hsv_t LW6SYS_COLOR_HSV_WHITE = { 0.0f, 0.0f, 1.0f, 1.0f };
lw6sys_color_hsv_t LW6SYS_COLOR_HSV_BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
lw6sys_color_hsv_t LW6SYS_COLOR_HSV_RED = { 0.0f, 1.0f, 1.0f, 1.0f };
lw6sys_color_hsv_t LW6SYS_COLOR_HSV_GREEN = { 120.0f, 1.0f, 1.0f, 1.0f };
lw6sys_color_hsv_t LW6SYS_COLOR_HSV_BLUE = { 240.0f, 1.0f, 1.0f, 1.0f };

static inline u_int8_t
float2char (float comp)
{
  return comp * FLOAT2CHAR_FACTOR;
}

static inline float
char2float (u_int8_t comp)
{
  return comp / FLOAT2CHAR_FACTOR;
}

/**
 * lw6sys_color_float2char
 *
 * @f: the value to convert, from 0.0f to 1.0f
 *
 * Converts a floating point value between 0.0f and 1.0f to its
 * 8-bit equivalent between 0 and 255. Usefull in color conversion.
 *
 * Return value: an integer between 0 and 255.
 */
u_int8_t
lw6sys_color_float2char (float f)
{
  return float2char (f);
}

/**
 * lw6sys_color_float2char
 *
 * @i: the value to convert, from 0 to 255
 *
 * Converts an 8-bit value between 0 and 255 to its floating-point
 * equivalent between 0.0f and 1.0f. Usefull in color conversion.
 *
 * Return value: a float between 0.0f and 1.0f.
 */
float
lw6sys_color_char2float (u_int8_t i)
{
  return char2float (i);
}

/**
 * lw6sys_color_f_to_8
 *
 * @color_f: the color to convert
 *
 * Converts a color from floating point format to the
 * integer "0 to 255" common format.
 * All fields (RGBA) are converted.
 *
 * Return value: the color in 8-bit format.
 */
lw6sys_color_8_t
lw6sys_color_f_to_8 (const lw6sys_color_f_t * color_f)
{
  lw6sys_color_8_t ret;

  ret.r = float2char (color_f->r);
  ret.g = float2char (color_f->g);
  ret.b = float2char (color_f->b);
  ret.a = float2char (color_f->a);

  return ret;
}

/**
 * lw6sys_color_8_to_f
 *
 * @color_f: the converted color (pointer must point to writable memory)
 * @color_8: the color to convert
 *
 * Converts a color from the integer "0 to 255" common format
 * to floating point format.
 * All fields (RGBA) are converted.
 *
 * Return value: none.
 */
void
lw6sys_color_8_to_f (lw6sys_color_f_t * color_f, lw6sys_color_8_t color_8)
{
  color_f->r = char2float (color_8.r);
  color_f->g = char2float (color_8.g);
  color_f->b = char2float (color_8.b);
  color_f->a = char2float (color_8.a);
}

/**
 * lw6sys_color_f_to_irgba
 *
 * @color_f: the color to convert
 *
 * Converts a color from floating point format to a single
 * integer, where all fields (RGBA) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_f_to_irgba (const lw6sys_color_f_t * color_f)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = float2char (color_f->r);
  p[1] = float2char (color_f->g);
  p[2] = float2char (color_f->b);
  p[3] = float2char (color_f->a);

  return ret;
}

/**
 * lw6sys_color_f_to_ibgra
 *
 * @color_f: the color to convert
 *
 * Converts a color from floating point format to a single
 * integer, where all fields (BGRA) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_f_to_ibgra (const lw6sys_color_f_t * color_f)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = float2char (color_f->b);
  p[1] = float2char (color_f->g);
  p[2] = float2char (color_f->r);
  p[3] = float2char (color_f->a);

  return ret;
}

/**
 * lw6sys_color_f_to_iargb
 *
 * @color_f: the color to convert
 *
 * Converts a color from floating point format to a single
 * integer, where all fields (ARGB) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_f_to_iargb (const lw6sys_color_f_t * color_f)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = float2char (color_f->a);
  p[1] = float2char (color_f->r);
  p[2] = float2char (color_f->g);
  p[3] = float2char (color_f->b);

  return ret;
}

/**
 * lw6sys_color_f_to_iabgr
 *
 * @color_f: the color to convert
 *
 * Converts a color from floating point format to a single
 * integer, where all fields (ABGR) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_f_to_iabgr (const lw6sys_color_f_t * color_f)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = float2char (color_f->a);
  p[1] = float2char (color_f->b);
  p[2] = float2char (color_f->g);
  p[3] = float2char (color_f->r);

  return ret;
}

/**
 * lw6sys_color_8_to_irgba
 *
 * @color_8: the color to convert
 *
 * Converts a color from common "0 to 255" structured format to a single
 * integer, where all fields (RGBA) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_8_to_irgba (lw6sys_color_8_t color_8)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = color_8.r;
  p[1] = color_8.g;
  p[2] = color_8.b;
  p[3] = color_8.a;

  return ret;
}

/**
 * lw6sys_color_8_to_ibgra
 *
 * @color_8: the color to convert
 *
 * Converts a color from common "0 to 255" structured format to a single
 * integer, where all fields (BGRA) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_8_to_ibgra (lw6sys_color_8_t color_8)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = color_8.b;
  p[1] = color_8.g;
  p[2] = color_8.r;
  p[3] = color_8.a;

  return ret;
}

/**
 * lw6sys_color_8_to_iargb
 *
 * @color_8: the color to convert
 *
 * Converts a color from common "0 to 255" structured format to a single
 * integer, where all fields (ARGB) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_8_to_iargb (lw6sys_color_8_t color_8)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = color_8.a;
  p[1] = color_8.r;
  p[2] = color_8.g;
  p[3] = color_8.b;

  return ret;
}

/**
 * lw6sys_color_8_to_iabgr
 *
 * @color_8: the color to convert
 *
 * Converts a color from common "0 to 255" structured format to a single
 * integer, where all fields (ABGR) are serialized. This
 * serialization is endianess independant. Could be used
 * directly by low-level libraries such as SDL.
 *
 * Return value: the color serialized in an integer.
 */
u_int32_t
lw6sys_color_8_to_iabgr (lw6sys_color_8_t color_8)
{
  u_int32_t ret;

  u_int8_t *p = (u_int8_t *) & ret;

  p[0] = color_8.a;
  p[1] = color_8.b;
  p[2] = color_8.g;
  p[3] = color_8.r;

  return ret;
}

/**
 * lw6sys_color_irgba_to_f
 *
 * @color_f: the converted color (point must point to writable memory)
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (RGBA) to
 * a floating point structure.
 *
 * Return value: none.
 */
void
lw6sys_color_irgba_to_f (lw6sys_color_f_t * color_f, u_int32_t color_i)
{
  u_int8_t *p = (u_int8_t *) & color_i;

  color_f->r = char2float (p[0]);
  color_f->g = char2float (p[1]);
  color_f->b = char2float (p[2]);
  color_f->a = char2float (p[3]);
}

/**
 * lw6sys_color_ibgra_to_f
 *
 * @color_f: the converted color (point must point to writable memory)
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (BGRA) to
 * a floating point structure.
 *
 * Return value: none.
 */
void
lw6sys_color_ibgra_to_f (lw6sys_color_f_t * color_f, u_int32_t color_i)
{
  u_int8_t *p = (u_int8_t *) & color_i;

  color_f->b = char2float (p[0]);
  color_f->g = char2float (p[1]);
  color_f->r = char2float (p[2]);
  color_f->a = char2float (p[3]);
}

/**
 * lw6sys_color_iargb_to_f
 *
 * @color_f: the converted color (point must point to writable memory)
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (ARGB) to
 * a floating point structure.
 *
 * Return value: none.
 */
void
lw6sys_color_iargb_to_f (lw6sys_color_f_t * color_f, u_int32_t color_i)
{
  u_int8_t *p = (u_int8_t *) & color_i;

  color_f->a = char2float (p[0]);
  color_f->r = char2float (p[1]);
  color_f->g = char2float (p[2]);
  color_f->b = char2float (p[3]);
}

/**
 * lw6sys_color_iabgr_to_f
 *
 * @color_f: the converted color (point must point to writable memory)
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (ABGR) to
 * a floating point structure.
 *
 * Return value: none.
 */
void
lw6sys_color_iabgr_to_f (lw6sys_color_f_t * color_f, u_int32_t color_i)
{
  u_int8_t *p = (u_int8_t *) & color_i;

  color_f->a = char2float (p[0]);
  color_f->b = char2float (p[1]);
  color_f->g = char2float (p[2]);
  color_f->r = char2float (p[3]);
}

/**
 * lw6sys_color_irgba_to_8
 *
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (RGBA) to
 * a "0 to 255" based structure.
 *
 * Return value: the converted color (structure).
 */
lw6sys_color_8_t
lw6sys_color_irgba_to_8 (u_int32_t color_i)
{
  lw6sys_color_8_t ret;
  u_int8_t *p = (u_int8_t *) & color_i;

  ret.r = p[0];
  ret.g = p[1];
  ret.b = p[2];
  ret.a = p[3];

  return ret;
}

/**
 * lw6sys_color_ibgra_to_8
 *
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (BGRA) to
 * a "0 to 255" based structure.
 *
 * Return value: the converted color (structure).
 */
lw6sys_color_8_t
lw6sys_color_ibgra_to_8 (u_int32_t color_i)
{
  lw6sys_color_8_t ret;
  u_int8_t *p = (u_int8_t *) & color_i;

  ret.b = p[0];
  ret.g = p[1];
  ret.r = p[2];
  ret.a = p[3];

  return ret;
}

/**
 * lw6sys_color_iargb_to_8
 *
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (ARGB) to
 * a "0 to 255" based structure.
 *
 * Return value: the converted color (structure).
 */
lw6sys_color_8_t
lw6sys_color_iargb_to_8 (u_int32_t color_i)
{
  lw6sys_color_8_t ret;
  u_int8_t *p = (u_int8_t *) & color_i;

  ret.a = p[0];
  ret.r = p[1];
  ret.g = p[2];
  ret.b = p[3];

  return ret;
}

/**
 * lw6sys_color_iabgr_to_8
 *
 * @color_i: the color to convert
 *
 * Converts a color from a serialized integer format (ABGR) to
 * a "0 to 255" based structure.
 *
 * Return value: the converted color (structure).
 */
lw6sys_color_8_t
lw6sys_color_iabgr_to_8 (u_int32_t color_i)
{
  lw6sys_color_8_t ret;
  u_int8_t *p = (u_int8_t *) & color_i;

  ret.a = p[0];
  ret.b = p[1];
  ret.g = p[2];
  ret.r = p[3];

  return ret;
}

/**
 * lw6sys_color_a_to_8
 *
 * @sys_context: global system context
 * @ascii: the color to convert
 *
 * Converts a color from a human readable string to
 * a "0 to 255" based structure. The string must be
 * of the form "#RRGGBBAA" or "#RGB", in a general
 * manner any HTML-valid value should work.
 *
 * Return value: the converted color (structure).
 */
lw6sys_color_8_t
lw6sys_color_a_to_8 (lw6sys_context_t * sys_context, const char *ascii)
{
  lw6sys_color_8_t ret;

  ret.r = 0x00;
  ret.g = 0x00;
  ret.b = 0x00;
  ret.a = 0xFF;

  if (ascii[0] == '#')
    {
      char buf[3];
      u_int8_t c;

      memset (buf, 0, 3);
      switch (strlen (ascii + 1))
	{
	case 4:
	  buf[0] = ascii[4];
	  c = strtol (buf, NULL, 16);
	  ret.a = c + (c << 4);
	  // Important *NOT* to put a break here.
	case 3:
	  buf[0] = ascii[1];
	  c = strtol (buf, NULL, 16);
	  ret.r = c + (c << 4);
	  buf[0] = ascii[2];
	  c = strtol (buf, NULL, 16);
	  ret.g = c + (c << 4);
	  buf[0] = ascii[3];
	  c = strtol (buf, NULL, 16);
	  ret.b = c + (c << 4);
	  break;
	case 8:
	  buf[0] = ascii[7];
	  buf[1] = ascii[8];
	  ret.a = strtol (buf, NULL, 16);
	  // Important *NOT* to put a break here.
	case 6:
	  buf[0] = ascii[1];
	  buf[1] = ascii[2];
	  ret.r = strtol (buf, NULL, 16);
	  buf[0] = ascii[3];
	  buf[1] = ascii[4];
	  ret.g = strtol (buf, NULL, 16);
	  buf[0] = ascii[5];
	  buf[1] = ascii[6];
	  ret.b = strtol (buf, NULL, 16);
	  break;
	default:
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("invalid color value \"%s\", color format must be \"#RGB\", \"#RGBA\", \"#RRGGBB\" or \"#RRGGBBAA\""), ascii);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid color value \"%s\", color value must start with \"#\""), ascii);
    }

  return ret;
}

/**
 * lw6sys_color_a_to_f
 *
 * @sys_context: global system context
 * @color_f: the converted color (pointer must point to writable memory)
 * @ascii: the color to convert
 *
 * Converts a color from a human readable string to
 * a float based structure. The string must be
 * of the form "#RRGGBBAA" or "#RGB", in a general
 * manner any HTML-valid value should work.
 *
 * Return value: none.
 */
void
lw6sys_color_a_to_f (lw6sys_context_t * sys_context, lw6sys_color_f_t * color_f, const char *ascii)
{
  lw6sys_color_8_t color_8;

  color_8 = lw6sys_color_a_to_8 (sys_context, ascii);
  lw6sys_color_8_to_f (color_f, color_8);
}

/**
 * lw6sys_color_8_to_a
 *
 * @sys_context: global system context
 * @color_8: the color to convert
 *
 * Converts a color from a "0 - 255" integer based structure
 * to its readable form "#RRGGBBAA". If alpha is 255 (0xFF), that is,
 * if it's opaque, then the "AA" part is ommitted.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_color_8_to_a (lw6sys_context_t * sys_context, lw6sys_color_8_t color_8)
{
  char *ret = NULL;

  if (color_8.a == 0xFF)
    {
      ret = lw6sys_new_sprintf (sys_context, "#%02x%02x%02x", color_8.r, color_8.g, color_8.b);
    }
  else
    {
      ret = lw6sys_new_sprintf (sys_context, "#%02x%02x%02x%02x", color_8.r, color_8.g, color_8.b, color_8.a);
    }

  return ret;
}

/**
 * lw6sys_color_rgb_to_hsv
 *
 * @sys_context: global system context
 * @color_hsv: the target color, in HSV format
 * @color_8: the source color, in RGB 256 format
 *
 * Converts from HSV to RGB. Usefull for color manipulation,
 * since most colors are stored in RGB but HSV is convenient
 * for transformation. Alpha layer is kept as is.
 *
 * Return value: none.
 */
void
lw6sys_color_rgb_to_hsv (lw6sys_context_t * sys_context, lw6sys_color_hsv_t * color_hsv, lw6sys_color_8_t color_8)
{
  // inspired from Allegro src/color.c
  int delta;

  //ASSERT(r >= 0 && r <= 255);
  //ASSERT(g >= 0 && g <= 255);
  //ASSERT(b >= 0 && b <= 255);

  if (color_8.r > color_8.g)
    {
      if (color_8.b > color_8.r)
	{
	  /* b>r>g */
	  delta = color_8.b - color_8.g;
	  color_hsv->h = 240.0f + ((color_8.r - color_8.g) * 60) / (float) delta;
	  color_hsv->s = (float) delta / (float) color_8.b;
	  color_hsv->v = (float) color_8.b * (1.0f / 255.0f);
	}
      else
	{
	  /* r>g and r>b */
	  delta = color_8.r - lw6sys_imin (color_8.g, color_8.b);
	  color_hsv->h = ((color_8.g - color_8.b) * 60) / (float) delta;
	  if (color_hsv->h < 0.0f)
	    color_hsv->h += 360.0f;
	  color_hsv->s = (float) delta / (float) color_8.r;
	  color_hsv->v = (float) color_8.r * (1.0f / 255.0f);
	}
    }
  else
    {
      if (color_8.b > color_8.g)
	{
	  /* b>g>=r */
	  delta = color_8.b - color_8.r;
	  color_hsv->h = 240.0f + ((color_8.r - color_8.g) * 60) / (float) delta;
	  color_hsv->s = (float) delta / (float) color_8.b;
	  color_hsv->v = (float) color_8.b * (1.0f / 255.0f);
	}
      else
	{
	  /* g>=b and g>=r */
	  delta = color_8.g - lw6sys_imin (color_8.r, color_8.b);
	  if (delta == 0)
	    {
	      color_hsv->h = 0.0f;
	      if (color_8.g == 0)
		color_hsv->s = color_hsv->v = 0.0f;
	      else
		{
		  color_hsv->s = (float) delta / (float) color_8.g;
		  color_hsv->v = (float) color_8.g * (1.0f / 255.0f);
		}
	    }
	  else
	    {
	      color_hsv->h = 120.0f + ((color_8.b - color_8.r) * 60) / (float) delta;
	      color_hsv->s = (float) delta / (float) color_8.g;
	      color_hsv->v = (float) color_8.g * (1.0f / 255.0f);
	    }
	}
    }

  color_hsv->a = char2float (color_8.a);
}

/**
 * lw6sys_color_hsv_to_rgb
 *
 * @sys_context: global system context
 * @color_hsv: the source color, in HSV format
 *
 * Converts from RGB to HSV. Usefull to make colors transformed in HSV
 * format usable again by all display routines, which consume RGB.
 * Alpha layer is kept as is.
 *
 * Return value: the RGB color.
 */
lw6sys_color_8_t
lw6sys_color_hsv_to_rgb (lw6sys_context_t * sys_context, const lw6sys_color_hsv_t * color_hsv)
{
  // inspired from Allegro src/color.c
  lw6sys_color_8_t color_8 = LW6SYS_COLOR_8_BLACK;
  float h, s, v;
  float f, x, y, z;
  int i;

  //ASSERT(s >= 0 && s <= 1);
  //ASSERT(v >= 0 && v <= 1);

  h = color_hsv->h;
  s = color_hsv->s;
  v = color_hsv->v;

  v *= 255.0f;

  if (s == 0.0f)
    {				/* ok since we don't divide by s, and faster */
      color_8.r = color_8.g = color_8.b = v + 0.5f;
    }
  else
    {
      h = fmod (h, 360.0f) / 60.0f;
      if (h < 0.0f)
	h += 6.0f;

      i = (int) h;
      f = h - i;
      x = v * s;
      y = x * f;
      v += 0.5f;		/* round to the nearest integer below */
      z = v - x;

      switch (i)
	{

	case 6:
	case 0:
	  color_8.r = v;
	  color_8.g = z + y;
	  color_8.b = z;
	  break;

	case 1:
	  color_8.r = v - y;
	  color_8.g = v;
	  color_8.b = z;
	  break;

	case 2:
	  color_8.r = z;
	  color_8.g = v;
	  color_8.b = z + y;
	  break;

	case 3:
	  color_8.r = z;
	  color_8.g = v - y;
	  color_8.b = v;
	  break;

	case 4:
	  color_8.r = z + y;
	  color_8.g = z;
	  color_8.b = v;
	  break;

	case 5:
	  color_8.r = v;
	  color_8.g = z;
	  color_8.b = v - y;
	  break;
	}
    }

  color_8.a = float2char (color_hsv->a);

  return color_8;
}

/**
 * lw6sys_color_hsv_invert
 *
 * @sys_context: global system context
 * @color_hsv: the source color, in HSV format
 * @invert_h: wether to invert the hue
 * @invert_s: wether to invert the saturation
 * @invert_v: wether to invert the value
 *
 * Inverts an HSV color, calling it with 1,0,0 the color
 * will become a color with opposite hue but same saturation
 * and same value.
 *
 * Return value: none.
 */
void
lw6sys_color_hsv_invert (lw6sys_context_t * sys_context, lw6sys_color_hsv_t * color_hsv, int invert_h, int invert_s, int invert_v)
{
  if (invert_h)
    {
      color_hsv->h += 180.0f;
      if (color_hsv->h >= 360.0f)
	{
	  color_hsv->h -= 360.0f;
	}
    }
  if (invert_s)
    {
      color_hsv->s = 1.0f - color_hsv->s;
    }
  if (invert_v)
    {
      color_hsv->v = 1.0f - color_hsv->v;
    }
}

/**
 * lw6sys_color_is_grey
 *
 * @color: the color to test
 *
 * Tells wether a color is pure grey or not. This is interesting
 * for such colors have no hue and sometimes need special handling.
 *
 * Return value: 1 if grey, 0 if colored
 */
int
lw6sys_color_is_grey (lw6sys_color_8_t color)
{
  int ret;

  ret = (color.r == color.g) && (color.g == color.b);

  return ret;
}

/**
 * lw6sys_color_average
 *
 * @sys_context: global system context
 * @size: number of the color array (number of items)
 * @colors: the colors to compute
 *
 * Tries to find out the "average" color from an array of colors.
 * The algorithm is far from perfect, but should output a color which
 * reflects the colors passed in.
 *
 * Return value: the (inexact) average color.
 */
lw6sys_color_8_t
lw6sys_color_average (lw6sys_context_t * sys_context, int size, const lw6sys_color_8_t * colors)
{
  int i;
  int64_t sum_r = 0;
  int64_t sum_g = 0;
  int64_t sum_b = 0;
  int64_t sum_a = 0;
  double sum_s = 0.0f;
  double sum_v = 0.0f;
  lw6sys_color_8_t color_8 = LW6SYS_COLOR_8_BLACK;
  lw6sys_color_hsv_t color_hsv = LW6SYS_COLOR_HSV_BLACK;

  if (size > 0)
    {
      for (i = 0; i < size; ++i)
	{
	  lw6sys_color_rgb_to_hsv (sys_context, &color_hsv, colors[i]);
	  sum_s += color_hsv.s;
	  sum_v += color_hsv.v;
	  sum_r += colors[i].r;
	  sum_g += colors[i].g;
	  sum_b += colors[i].b;
	  sum_a += colors[i].a;
	}
      color_8.r = sum_r / size;
      color_8.g = sum_g / size;
      color_8.b = sum_b / size;
      color_8.a = sum_a / size;
      lw6sys_color_rgb_to_hsv (sys_context, &color_hsv, color_8);	// set hue & alpha (h,a)
      color_hsv.s = sum_s / size;
      color_hsv.v = sum_v / size;
      color_8 = lw6sys_color_hsv_to_rgb (sys_context, &color_hsv);
    }

  return color_8;
}

/**
 * lw6sys_color_ponderate
 *
 * @sys_context: global system context
 * @color1: first color
 * @color2: second color
 * @coeff: the ponderation coefficient
 *
 * Tries to find a color between the two colors passed as an argument.
 * The coefficient can be used, to set the relative weight of each color.
 * Using 0 will return color1, 1 will return color2 and 0.5 will make
 * an average between the two colors. Any value between 0 and 1 can be
 * used.
 *
 * Return value: the (inexact) ponderated color.
 */
lw6sys_color_8_t
lw6sys_color_ponderate (lw6sys_context_t * sys_context, lw6sys_color_8_t color1, lw6sys_color_8_t color2, float coeff)
{
  lw6sys_color_8_t ret = LW6SYS_COLOR_8_BLACK;
  lw6sys_color_hsv_t color1_hsv;
  lw6sys_color_hsv_t color2_hsv;
  lw6sys_color_hsv_t ret_hsv = LW6SYS_COLOR_HSV_BLACK;
  float one_minus_coeff;

  if (coeff <= 0.0f)
    {
      ret = color1;
    }
  else if (coeff >= 1.0f)
    {
      ret = color2;
    }
  else
    {
      one_minus_coeff = 1.0f - coeff;
      if (lw6sys_color_is_grey (color1) || lw6sys_color_is_grey (color2))
	{
	  ret.r = one_minus_coeff * color1.r + coeff * color2.r;
	  ret.g = one_minus_coeff * color1.g + coeff * color2.g;
	  ret.b = one_minus_coeff * color1.b + coeff * color2.b;
	  ret.a = one_minus_coeff * color1.a + coeff * color2.a;
	}
      else
	{
	  lw6sys_color_rgb_to_hsv (sys_context, &color1_hsv, color1);
	  lw6sys_color_rgb_to_hsv (sys_context, &color2_hsv, color2);
	  if (color2_hsv.h - color1_hsv.h < 180.0f)
	    {
	      color2_hsv.h += 360.0f;
	    }
	  if (color2_hsv.h - color1_hsv.h > 180.0f)
	    {
	      color1_hsv.h += 360.0f;
	    }
	  ret_hsv.h = one_minus_coeff * color1_hsv.h + coeff * color2_hsv.h;
	  ret_hsv.s = one_minus_coeff * color1_hsv.s + coeff * color2_hsv.s;
	  ret_hsv.v = one_minus_coeff * color1_hsv.v + coeff * color2_hsv.v;
	  ret_hsv.a = one_minus_coeff * color1_hsv.a + coeff * color2_hsv.a;
	  ret = lw6sys_color_hsv_to_rgb (sys_context, &ret_hsv);
	}
    }

  return ret;
}

static void
color_hsv_to_xyz (lw6sys_context_t * sys_context, float *x, float *y, float *z, lw6sys_color_hsv_t * color_hsv)
{
  float angle;

  angle = M_PI * color_hsv->h / 180.0f;
  (*x) = cos (angle) * color_hsv->s * color_hsv->v;
  (*y) = sin (angle) * color_hsv->s * color_hsv->v;
  (*z) = XYZ_CONE_Z * color_hsv->v;
}

static void
color_8_to_xyz (lw6sys_context_t * sys_context, float *x, float *y, float *z, lw6sys_color_8_t color_8)
{
  lw6sys_color_hsv_t color_hsv = LW6SYS_COLOR_HSV_BLACK;

  lw6sys_color_rgb_to_hsv (sys_context, &color_hsv, color_8);
  color_hsv_to_xyz (sys_context, x, y, z, &color_hsv);
}

/**
 * lw6sys_color_distance
 *
 * @sys_context: global system context
 * @color1: first color
 * @color2: second color
 *
 * Calculates the distance between two colors. The unit is arbitrary, a big
 * value means "colors are different", 0 means they are the same. A distance
 * of 1 corresponds to colors which have barely anything in common, but the
 * result can still be greater than 1. Alpha layer is not taken in account.
 *
 * Return value: the distance.
 */
float
lw6sys_color_distance (lw6sys_context_t * sys_context, lw6sys_color_8_t color1, lw6sys_color_8_t color2)
{
  float x1 = 0.0f;
  float y1 = 0.0f;
  float z1 = 0.0f;
  float x2 = 0.0f;
  float y2 = 0.0f;
  float z2 = 0.0f;
  float distance = 0.0f;

  color_8_to_xyz (sys_context, &x1, &y1, &z1, color1);
  color_8_to_xyz (sys_context, &x2, &y2, &z2, color2);
  distance = sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));

  return distance;
}

/**
 * lw6sys_color_is_same
 *
 * @sys_context: global system context
 * @color1: the first color to compare
 * @color2: the second color to compare
 *
 * Compares two colors.
 *
 * Return value: 1 if they are the same, 0 if not.
 */
int
lw6sys_color_is_same (lw6sys_context_t * sys_context, lw6sys_color_8_t color1, lw6sys_color_8_t color2)
{
  int ret = 0;

  ret = memcmp (&color1, &color2, sizeof (lw6sys_color_8_t)) ? 0 : 1;

  return ret;
}

/**
 * lw6sys_color_8_solid
 *
 * @color: the color to modify
 *
 * Make a color "solid" that is make it not transparent at all.
 *
 * Return value: none.
 */
void
lw6sys_color_8_solid (lw6sys_color_8_t * color)
{
  color->a = 0xFF;
}

/**
 * lw6sys_color_f_solid
 *
 * @color: the color to modify
 *
 * Make a color "solid" that is make it not transparent at all.
 *
 * Return value: none.
 */
void
lw6sys_color_f_solid (lw6sys_color_f_t * color)
{
  color->a = 1.0f;
}
