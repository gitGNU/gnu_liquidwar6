/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6MAT_H
#define LIQUIDWAR6MAT_H

#include "../sys/sys.h"

#include <math.h>

#define LW6MAT_X_MUL_F 65536.0f
#define LW6MAT_X_SHIFT_I 16
#define LW6MAT_X_ROUND_I 32767

#define LW6MAT_F_0 0.0f
#define LW6MAT_I_0 0
#define LW6MAT_D_0 0.0d
#define LW6MAT_X_0 0
#define LW6MAT_F_1 1.0f
#define LW6MAT_I_1 1
#define LW6MAT_D_1 1.0d
#define LW6MAT_X_1 65536

/*
 * Float API.
 */

/**
 * Float vector with 2 elements (AKA 2D point).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    float x;
    float y;
  } p;
  /// Accessor with array index.
  float v[2];
} lw6mat_fvec2_t;

/**
 * Float vector with 3 elements (AKA 3D point).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    float x;
    float y;
    float z;
  } p;
  /// Accessor with array index.
  float v[3];
  /// Accessor with color-like name.
  struct
  {
    float r;
    float g;
    float b;
  } c;
} lw6mat_fvec3_t;

/**
 * Float vector with 4 elements (AKA quaternion).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    float x;
    float y;
    float z;
    float w;
  } p;
  /// Accessor with array index.
  float v[4];
  /// Accessor with color-like name.
  struct
  {
    float r;
    float g;
    float b;
    float a;
  } c;
} lw6mat_fvec4_t;

/**
 * Float 2x2 matrix (AKA 2D rectangle).
 */
typedef union
{
  /// Accessor with array index.
  float m[2][2];
} lw6mat_fmat2_t;

/**
 * Float 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /// Accessor with array index.
  float m[3][3];
} lw6mat_fmat3_t;

/**
 * Float 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /// Accessor with array index.
  float m[4][4];
} lw6mat_fmat4_t;

/*
 * Integer API.
 */

/**
 * Integer vector with 2 elements (AKA 2D point).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    int32_t x;
    int32_t y;
  } p;
  /// Accessor with array index.
  int32_t v[2];
} lw6mat_ivec2_t;

/**
 * Integer vector with 3 elements (AKA 3D point).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    int32_t x;
    int32_t y;
    int32_t z;
  } p;
  /// Accessor with array index.
  int32_t v[3];
  /// Accessor with color-like name.
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
  } c;
} lw6mat_ivec3_t;

/**
 * Integer vector with 4 elements (AKA quaternion).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
  } p;
  /// Accessor with array index.
  int32_t v[4];
  /// Accessor with color-like name.
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
    int32_t a;
  } c;
} lw6mat_ivec4_t;

/**
 * Integer 2x2 matrix (AKA 2D rectangle).
 */
typedef union
{
  /// Accessor with array index.
  int32_t m[2][2];
} lw6mat_imat2_t;

/**
 * Integer 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /// Accessor with array index.
  int32_t m[3][3];
} lw6mat_imat3_t;

/**
 * Integer 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /// Accessor with array index.
  int32_t m[4][4];
} lw6mat_imat4_t;

/*
 * Conversions.
 */

static inline int32_t
lw6mat_ftoi (float f)
{
  /*
   * Calling round() gives a better conversion
   * than a plain cast, which does an implicit floor()
   */
  return (int32_t) round (f);
}

static inline double
lw6mat_ftod (float f)
{
  return (double) f;
}

static inline int32_t
lw6mat_ftox (float f)
{
  /*
   * Calling round() gives a better conversion
   * than a plain cast, which does an implicit floor()
   * Wether it's really usefull here is questionnable,
   * but not doing it causes some weird "65535 errors".
   * Which are ugly. And break tests.
   */
  return (int32_t) round (f * LW6MAT_X_MUL_F);
}

static inline float
lw6mat_itof (int32_t i)
{
  return (float) i;
}

static inline double
lw6mat_itod (int32_t i)
{
  return (double) i;
}

static inline int32_t
lw6mat_itox (int32_t i)
{
  return i << LW6MAT_X_SHIFT_I;
}

static inline float
lw6mat_dtof (double d)
{
  return (float) d;
}

static inline int32_t
lw6mat_dtoi (double d)
{
  /*
   * Calling round() gives a better conversion
   * than a plain cast, which does an implicit floor()
   */
  return (int32_t) round (d);
}

static inline int32_t
lw6mat_dtox (double d)
{
  return (int32_t) (d * LW6MAT_X_MUL_F);
}

static inline float
lw6mat_xtof (int32_t x)
{
  return ((float) x) / LW6MAT_X_MUL_F;
}

static inline int32_t
lw6mat_xtoi (int32_t x)
{
  /*
   * We add 0.5 to the number before truncating
   * its left part, this simulates a round()
   * and provides more consistent conversions.
   */
  return (x + LW6MAT_X_ROUND_I) >> LW6MAT_X_SHIFT_I;
}

static inline double
lw6mat_xtod (int32_t x)
{
  return ((double) x) / LW6MAT_X_MUL_F;
}

/* mat-fvec2.c */
extern void lw6mat_fvec2_zero (lw6mat_fvec2_t * fvec2);
extern float lw6mat_fvec2_len_sq (const lw6mat_fvec2_t * fvec2);
extern float lw6mat_fvec2_len (const lw6mat_fvec2_t * fvec2);
extern void lw6mat_fvec2_norm (lw6mat_fvec2_t * fvec2);

/* mat-fvec3.c */
extern void lw6mat_fvec3_zero (lw6mat_fvec3_t * fvec3);
extern float lw6mat_fvec3_len_sq (const lw6mat_fvec3_t * fvec3);
extern float lw6mat_fvec3_len (const lw6mat_fvec3_t * fvec3);
extern void lw6mat_fvec3_norm (lw6mat_fvec3_t * fvec3);

/* mat-fvec4.c */
extern void lw6mat_fvec4_zero (lw6mat_fvec4_t * fvec4);
extern float lw6mat_fvec4_len_sq (const lw6mat_fvec4_t * fvec4);
extern float lw6mat_fvec4_len (const lw6mat_fvec4_t * fvec4);
extern void lw6mat_fvec4_norm (lw6mat_fvec4_t * fvec4);

/* mat-test.c */
extern int lw6mat_test_register (int mode);
extern int lw6mat_test_run (int mode);

#endif
