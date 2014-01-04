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
#define LW6MAT_X_SHIFT_HALF_I 8
#define LW6MAT_X_ROUND_HALF_I 127

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
  /// Accessor with texture-like name.
  struct
  {
    float s;
    float t;
  } t;
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
  /// Accessor with color-like name.
  struct
  {
    float r;
    float g;
    float b;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    float s;
    float t;
    float p;
  } t;
  /// Accessor with array index.
  float v[3];
  /// Accessor with smaller-sized vector, only 2 dimensions
  lw6mat_fvec2_t v2;
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
  /// Accessor with color-like name.
  struct
  {
    float r;
    float g;
    float b;
    float a;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    float s;
    float t;
    float p;
    float q;
  } t;
  /// Accessor with array index.
  float v[4];
  /// Accessor with smaller-sized vector, only 2 dimensions
  lw6mat_fvec2_t v2;
  /// Accessor with smaller-sized vector, only 3 dimensions
  lw6mat_fvec3_t v3;
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
  /// Accessor with texture-like name.
  struct
  {
    int32_t s;
    int32_t t;
  } t;
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
  /// Accessor with color-like name.
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    int32_t s;
    int32_t t;
    int32_t p;
  } t;
  /// Accessor with array index.
  int32_t v[3];
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
  /// Accessor with color-like name.
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
    int32_t a;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    int32_t s;
    int32_t t;
    int32_t p;
    int32_t q;
  } t;
  /// Accessor with array index.
  int32_t v[4];
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
 * Double API.
 */

/**
 * Double vector with 2 elements (AKA 2D point).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    double x;
    double y;
  } p;
  /// Accessor with texture-like name.
  struct
  {
    double s;
    double t;
  } t;
  /// Accessor with array index.
  double v[2];
} lw6mat_dvec2_t;

/**
 * Double vector with 3 elements (AKA 3D point).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    double x;
    double y;
    double z;
  } p;
  /// Accessor with color-like name.
  struct
  {
    double r;
    double g;
    double b;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    double s;
    double t;
    double p;
  } t;
  /// Accessor with array index.
  double v[3];
  /// Accessor with smaller-sized vector, only 2 dimensions
  lw6mat_dvec2_t v2;
} lw6mat_dvec3_t;

/**
 * Double vector with 4 elements (AKA quaternion).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    double x;
    double y;
    double z;
    double w;
  } p;
  /// Accessor with color-like name.
  struct
  {
    double r;
    double g;
    double b;
    double a;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    double s;
    double t;
    double p;
    double q;
  } t;
  /// Accessor with array index.
  double v[4];
  /// Accessor with smaller-sized vector, only 2 dimensions
  lw6mat_dvec2_t v2;
  /// Accessor with smaller-sized vector, only 3 dimensions
  lw6mat_dvec3_t v3;
} lw6mat_dvec4_t;

/**
 * Double 2x2 matrix (AKA 2D rectangle).
 */
typedef union
{
  /// Accessor with array index.
  double m[2][2];
} lw6mat_dmat2_t;

/**
 * Double 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /// Accessor with array index.
  double m[3][3];
} lw6mat_dmat3_t;

/**
 * Double 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /// Accessor with array index.
  double m[4][4];
} lw6mat_dmat4_t;

/*
 * Fixed Point API.
 */

/**
 * Fixed Point vector with 2 elements (AKA 2D point).
 */
typedef union
{
  /// Accessor with named/point coords.
  struct
  {
    int32_t x;
    int32_t y;
  } p;
  /// Accessor with texture-like name.
  struct
  {
    int32_t s;
    int32_t t;
  } t;
  /// Accessor with array index.
  int32_t v[2];
} lw6mat_xvec2_t;

/**
 * Fixed Point vector with 3 elements (AKA 3D point).
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
  /// Accessor with color-like name.
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    int32_t s;
    int32_t t;
    int32_t p;
  } t;
  /// Accessor with array index.
  int32_t v[3];
} lw6mat_xvec3_t;

/**
 * Fixed Point vector with 4 elements (AKA quaternion).
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
  /// Accessor with color-like name.
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
    int32_t a;
  } c;
  /// Accessor with texture-like name.
  struct
  {
    int32_t s;
    int32_t t;
    int32_t p;
    int32_t q;
  } t;
  /// Accessor with array index.
  int32_t v[4];
} lw6mat_xvec4_t;

/**
 * Fixed Point 2x2 matrix (AKA 2D rectangle).
 */
typedef union
{
  /// Accessor with array index.
  int32_t m[2][2];
} lw6mat_xmat2_t;

/**
 * Fixed Point 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /// Accessor with array index.
  int32_t m[3][3];
} lw6mat_xmat3_t;

/**
 * Fixed Point 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /// Accessor with array index.
  int32_t m[4][4];
} lw6mat_xmat4_t;

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
  return (int32_t) round (d * LW6MAT_X_MUL_F);
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

/*
 * Specific fixed-point routines
 */

static inline int32_t
lw6mat_x_mul_32 (int32_t a, int32_t b)
{
  /*
   * Multiplying two fixed-point numbers, does a rough approximation
   * which will make it behave OK when numbers are close to 1, but
   * numbers which are two small won't behave the right way.
   */
  return ((a + LW6MAT_X_ROUND_HALF_I) >> LW6MAT_X_SHIFT_HALF_I) * ((b +
								    LW6MAT_X_ROUND_HALF_I)
								   >>
								   LW6MAT_X_SHIFT_HALF_I);
}

static inline int32_t
lw6mat_x_div_32 (int32_t a, int32_t b)
{
  /*
   * Dividing two fixed-point numbers, does a rough approximation
   * which will make it behave OK when numbers are close to 1, but
   * if a is too big or b is too small, won't behave the right way.
   */
  return ((a << LW6MAT_X_SHIFT_HALF_I) /
	  ((b + LW6MAT_X_ROUND_HALF_I) >> LW6MAT_X_SHIFT_HALF_I));
}

static inline int32_t
lw6mat_x_inv_32 (int32_t a)
{
  /*
   * Inverse of a fixed-point number, does a rough approximation
   * which will make it behave OK when numbers are close to 1, but
   * if a is too big, won't behave the right way.
   */
  return ((LW6MAT_X_1 << LW6MAT_X_SHIFT_HALF_I) /
	  ((a + LW6MAT_X_ROUND_HALF_I) >> LW6MAT_X_SHIFT_HALF_I));
}

static inline int32_t
lw6mat_x_mul_64 (int32_t a, int32_t b)
{
  /*
   * Multiplying two fixed point numbers, will give the exact result
   * apart from out-of-range errors, but does it at the cost of a 64-bit
   * conversion and intermediate calc, which is slow.
   */
  return ((int32_t)
	  (((((int64_t) a) * ((int64_t) b)) +
	    LW6MAT_X_ROUND_I) >> LW6MAT_X_SHIFT_I));
}

static inline int32_t
lw6mat_x_div_64 (int32_t a, int32_t b)
{
  /*
   * Dividing two fixed point numbers, will give the exact result
   * apart from out-of-range errors, but does it at the cost of a 64-bit
   * conversion and intermediate calc, which is slow.
   */
  return ((int32_t) ((((int64_t) a) << LW6MAT_X_SHIFT_I) / ((int64_t) b)));
}

static inline int32_t
lw6mat_x_inv_64 (int32_t a)
{
  /*
   * Inverse of a fixed point numbers, will give the exact result
   * apart from out-of-range errors, but does it at the cost of a 64-bit
   * conversion and intermediate calc, which is slow.
   */
  return ((int32_t)
	  ((((int64_t) LW6MAT_X_1) << LW6MAT_X_SHIFT_I) / ((int64_t) a)));
}

/* mat-fvec2.c */
extern void lw6mat_fvec2_zero (lw6mat_fvec2_t * fvec2);
extern int lw6mat_fvec2_is_same (const lw6mat_fvec2_t * fvec2_a,
				 const lw6mat_fvec2_t * fvec2_b);
extern float lw6mat_fvec2_len_sq (const lw6mat_fvec2_t * fvec2);
extern float lw6mat_fvec2_len (const lw6mat_fvec2_t * fvec2);
extern int lw6mat_fvec2_norm (lw6mat_fvec2_t * fvec2);
extern void lw6mat_fvec2_neg (lw6mat_fvec2_t * fvec2);
extern void lw6mat_fvec2_add (lw6mat_fvec2_t * fvec2,
			      const lw6mat_fvec2_t * fvec2_a,
			      const lw6mat_fvec2_t * fvec2_b);
extern void lw6mat_fvec2_sub (lw6mat_fvec2_t * fvec2,
			      const lw6mat_fvec2_t * fvec2_a,
			      const lw6mat_fvec2_t * fvec2_b);
extern float lw6mat_fvec2_dot (const lw6mat_fvec2_t * fvec2_a,
			       const lw6mat_fvec2_t * fvec2_b);
extern void lw6mat_fvec2_cross (lw6mat_fvec3_t * fvec3,
				const lw6mat_fvec2_t * fvec2_a,
				const lw6mat_fvec2_t * fvec2_b);
extern void lw6mat_fvec2_scale (lw6mat_fvec2_t * fvec2, float f);

/* mat-fvec3.c */
extern void lw6mat_fvec3_zero (lw6mat_fvec3_t * fvec3);
extern int lw6mat_fvec3_is_same (const lw6mat_fvec3_t * fvec3_a,
				 const lw6mat_fvec3_t * fvec3_b);
extern float lw6mat_fvec3_len_sq (const lw6mat_fvec3_t * fvec3);
extern float lw6mat_fvec3_len (const lw6mat_fvec3_t * fvec3);
extern int lw6mat_fvec3_norm (lw6mat_fvec3_t * fvec3);
extern void lw6mat_fvec3_neg (lw6mat_fvec3_t * fvec3);
extern void lw6mat_fvec3_add (lw6mat_fvec3_t * fvec3,
			      const lw6mat_fvec3_t * fvec3_a,
			      const lw6mat_fvec3_t * fvec3_b);
extern void lw6mat_fvec3_sub (lw6mat_fvec3_t * fvec3,
			      const lw6mat_fvec3_t * fvec3_a,
			      const lw6mat_fvec3_t * fvec3_b);
extern float lw6mat_fvec3_dot (const lw6mat_fvec3_t * fvec3_a,
			       const lw6mat_fvec3_t * fvec3_b);
extern void lw6mat_fvec3_cross (lw6mat_fvec3_t * fvec3,
				const lw6mat_fvec3_t * fvec3_a,
				const lw6mat_fvec3_t * fvec3_b);
extern void lw6mat_fvec3_scale (lw6mat_fvec3_t * fvec3, float f);

/* mat-fvec4.c */
extern void lw6mat_fvec4_zero (lw6mat_fvec4_t * fvec4);
extern int lw6mat_fvec4_is_same (const lw6mat_fvec4_t * fvec4_a,
				 const lw6mat_fvec4_t * fvec4_b);
extern float lw6mat_fvec4_len_sq (const lw6mat_fvec4_t * fvec4);
extern float lw6mat_fvec4_len (const lw6mat_fvec4_t * fvec4);
extern int lw6mat_fvec4_norm (lw6mat_fvec4_t * fvec4);
extern void lw6mat_fvec4_neg (lw6mat_fvec4_t * fvec4);
extern void lw6mat_fvec4_add (lw6mat_fvec4_t * fvec4,
			      const lw6mat_fvec4_t * fvec4_a,
			      const lw6mat_fvec4_t * fvec4_b);
extern void lw6mat_fvec4_sub (lw6mat_fvec4_t * fvec4,
			      const lw6mat_fvec4_t * fvec4_a,
			      const lw6mat_fvec4_t * fvec4_b);
extern float lw6mat_fvec4_dot (const lw6mat_fvec4_t * fvec4_a,
			       const lw6mat_fvec4_t * fvec4_b);
extern void lw6mat_fvec4_cross (lw6mat_fvec3_t * fvec3,
				const lw6mat_fvec4_t * fvec4_a,
				const lw6mat_fvec4_t * fvec4_b);
extern void lw6mat_fvec4_scale (lw6mat_fvec4_t * fvec4, float f);

/* mat-ivec2.c */
extern void lw6mat_ivec2_zero (lw6mat_ivec2_t * ivec2);
extern int lw6mat_ivec2_is_same (const lw6mat_ivec2_t * ivec2_a,
				 const lw6mat_ivec2_t * ivec2_b);
extern int32_t lw6mat_ivec2_len_sq (const lw6mat_ivec2_t * ivec2);
extern int32_t lw6mat_ivec2_len (const lw6mat_ivec2_t * ivec2);
extern int lw6mat_ivec2_norm (lw6mat_ivec2_t * ivec2);
extern void lw6mat_ivec2_neg (lw6mat_ivec2_t * ivec2);
extern void lw6mat_ivec2_add (lw6mat_ivec2_t * ivec2,
			      const lw6mat_ivec2_t * ivec2_a,
			      const lw6mat_ivec2_t * ivec2_b);
extern void lw6mat_ivec2_sub (lw6mat_ivec2_t * ivec2,
			      const lw6mat_ivec2_t * ivec2_a,
			      const lw6mat_ivec2_t * ivec2_b);
extern int32_t lw6mat_ivec2_dot (const lw6mat_ivec2_t * ivec2_a,
				 const lw6mat_ivec2_t * ivec2_b);
extern void lw6mat_ivec2_cross (lw6mat_ivec3_t * ivec3,
				const lw6mat_ivec2_t * ivec2_a,
				const lw6mat_ivec2_t * ivec2_b);
extern void lw6mat_ivec2_scale (lw6mat_ivec2_t * ivec2, int32_t f);

/* mat-ivec3.c */
extern void lw6mat_ivec3_zero (lw6mat_ivec3_t * ivec3);
extern int lw6mat_ivec3_is_same (const lw6mat_ivec3_t * ivec3_a,
				 const lw6mat_ivec3_t * ivec3_b);
extern int32_t lw6mat_ivec3_len_sq (const lw6mat_ivec3_t * ivec3);
extern int32_t lw6mat_ivec3_len (const lw6mat_ivec3_t * ivec3);
extern int lw6mat_ivec3_norm (lw6mat_ivec3_t * ivec3);
extern void lw6mat_ivec3_neg (lw6mat_ivec3_t * ivec3);
extern void lw6mat_ivec3_add (lw6mat_ivec3_t * ivec3,
			      const lw6mat_ivec3_t * ivec3_a,
			      const lw6mat_ivec3_t * ivec3_b);
extern void lw6mat_ivec3_sub (lw6mat_ivec3_t * ivec3,
			      const lw6mat_ivec3_t * ivec3_a,
			      const lw6mat_ivec3_t * ivec3_b);
extern int32_t lw6mat_ivec3_dot (const lw6mat_ivec3_t * ivec3_a,
				 const lw6mat_ivec3_t * ivec3_b);
extern void lw6mat_ivec3_cross (lw6mat_ivec3_t * ivec3,
				const lw6mat_ivec3_t * ivec3_a,
				const lw6mat_ivec3_t * ivec3_b);
extern void lw6mat_ivec3_scale (lw6mat_ivec3_t * ivec3, int32_t f);

/* mat-ivec4.c */
extern void lw6mat_ivec4_zero (lw6mat_ivec4_t * ivec4);
extern int lw6mat_ivec4_is_same (const lw6mat_ivec4_t * ivec4_a,
				 const lw6mat_ivec4_t * ivec4_b);
extern int32_t lw6mat_ivec4_len_sq (const lw6mat_ivec4_t * ivec4);
extern int32_t lw6mat_ivec4_len (const lw6mat_ivec4_t * ivec4);
extern int lw6mat_ivec4_norm (lw6mat_ivec4_t * ivec4);
extern void lw6mat_ivec4_neg (lw6mat_ivec4_t * ivec4);
extern void lw6mat_ivec4_add (lw6mat_ivec4_t * ivec4,
			      const lw6mat_ivec4_t * ivec4_a,
			      const lw6mat_ivec4_t * ivec4_b);
extern void lw6mat_ivec4_sub (lw6mat_ivec4_t * ivec4,
			      const lw6mat_ivec4_t * ivec4_a,
			      const lw6mat_ivec4_t * ivec4_b);
extern int32_t lw6mat_ivec4_dot (const lw6mat_ivec4_t * ivec4_a,
				 const lw6mat_ivec4_t * ivec4_b);
extern void lw6mat_ivec4_cross (lw6mat_ivec3_t * ivec3,
				const lw6mat_ivec4_t * ivec4_a,
				const lw6mat_ivec4_t * ivec4_b);
extern void lw6mat_ivec4_scale (lw6mat_ivec4_t * ivec4, int32_t f);

/* mat-dvec2.c */
extern void lw6mat_dvec2_zero (lw6mat_dvec2_t * dvec2);
extern int lw6mat_dvec2_is_same (const lw6mat_dvec2_t * dvec2_a,
				 const lw6mat_dvec2_t * dvec2_b);
extern double lw6mat_dvec2_len_sq (const lw6mat_dvec2_t * dvec2);
extern double lw6mat_dvec2_len (const lw6mat_dvec2_t * dvec2);
extern int lw6mat_dvec2_norm (lw6mat_dvec2_t * dvec2);
extern void lw6mat_dvec2_neg (lw6mat_dvec2_t * dvec2);
extern void lw6mat_dvec2_add (lw6mat_dvec2_t * dvec2,
			      const lw6mat_dvec2_t * dvec2_a,
			      const lw6mat_dvec2_t * dvec2_b);
extern void lw6mat_dvec2_sub (lw6mat_dvec2_t * dvec2,
			      const lw6mat_dvec2_t * dvec2_a,
			      const lw6mat_dvec2_t * dvec2_b);
extern double lw6mat_dvec2_dot (const lw6mat_dvec2_t * dvec2_a,
				const lw6mat_dvec2_t * dvec2_b);
extern void lw6mat_dvec2_cross (lw6mat_dvec3_t * dvec3,
				const lw6mat_dvec2_t * dvec2_a,
				const lw6mat_dvec2_t * dvec2_b);
extern void lw6mat_dvec2_scale (lw6mat_dvec2_t * dvec2, double f);

/* mat-dvec3.c */
extern void lw6mat_dvec3_zero (lw6mat_dvec3_t * dvec3);
extern int lw6mat_dvec3_is_same (const lw6mat_dvec3_t * dvec3_a,
				 const lw6mat_dvec3_t * dvec3_b);
extern double lw6mat_dvec3_len_sq (const lw6mat_dvec3_t * dvec3);
extern double lw6mat_dvec3_len (const lw6mat_dvec3_t * dvec3);
extern int lw6mat_dvec3_norm (lw6mat_dvec3_t * dvec3);
extern void lw6mat_dvec3_neg (lw6mat_dvec3_t * dvec3);
extern void lw6mat_dvec3_add (lw6mat_dvec3_t * dvec3,
			      const lw6mat_dvec3_t * dvec3_a,
			      const lw6mat_dvec3_t * dvec3_b);
extern void lw6mat_dvec3_sub (lw6mat_dvec3_t * dvec3,
			      const lw6mat_dvec3_t * dvec3_a,
			      const lw6mat_dvec3_t * dvec3_b);
extern double lw6mat_dvec3_dot (const lw6mat_dvec3_t * dvec3_a,
				const lw6mat_dvec3_t * dvec3_b);
extern void lw6mat_dvec3_cross (lw6mat_dvec3_t * dvec3,
				const lw6mat_dvec3_t * dvec3_a,
				const lw6mat_dvec3_t * dvec3_b);
extern void lw6mat_dvec3_scale (lw6mat_dvec3_t * dvec3, double f);

/* mat-dvec4.c */
extern void lw6mat_dvec4_zero (lw6mat_dvec4_t * dvec4);
extern int lw6mat_dvec4_is_same (const lw6mat_dvec4_t * dvec4_a,
				 const lw6mat_dvec4_t * dvec4_b);
extern double lw6mat_dvec4_len_sq (const lw6mat_dvec4_t * dvec4);
extern double lw6mat_dvec4_len (const lw6mat_dvec4_t * dvec4);
extern int lw6mat_dvec4_norm (lw6mat_dvec4_t * dvec4);
extern void lw6mat_dvec4_neg (lw6mat_dvec4_t * dvec4);
extern void lw6mat_dvec4_add (lw6mat_dvec4_t * dvec4,
			      const lw6mat_dvec4_t * dvec4_a,
			      const lw6mat_dvec4_t * dvec4_b);
extern void lw6mat_dvec4_sub (lw6mat_dvec4_t * dvec4,
			      const lw6mat_dvec4_t * dvec4_a,
			      const lw6mat_dvec4_t * dvec4_b);
extern double lw6mat_dvec4_dot (const lw6mat_dvec4_t * dvec4_a,
				const lw6mat_dvec4_t * dvec4_b);
extern void lw6mat_dvec4_cross (lw6mat_dvec3_t * dvec3,
				const lw6mat_dvec4_t * dvec4_a,
				const lw6mat_dvec4_t * dvec4_b);
extern void lw6mat_dvec4_scale (lw6mat_dvec4_t * dvec4, double f);

/* mat-xvec2.c */
extern void lw6mat_xvec2_zero (lw6mat_xvec2_t * xvec2);
extern int lw6mat_xvec2_is_same (const lw6mat_xvec2_t * xvec2_a,
				 const lw6mat_xvec2_t * xvec2_b);
extern int32_t lw6mat_xvec2_len_sq (const lw6mat_xvec2_t * xvec2);
extern int32_t lw6mat_xvec2_len (const lw6mat_xvec2_t * xvec2);
extern int lw6mat_xvec2_norm (lw6mat_xvec2_t * xvec2);
extern void lw6mat_xvec2_neg (lw6mat_xvec2_t * xvec2);
extern void lw6mat_xvec2_add (lw6mat_xvec2_t * xvec2,
			      const lw6mat_xvec2_t * xvec2_a,
			      const lw6mat_xvec2_t * xvec2_b);
extern void lw6mat_xvec2_sub (lw6mat_xvec2_t * xvec2,
			      const lw6mat_xvec2_t * xvec2_a,
			      const lw6mat_xvec2_t * xvec2_b);
extern int32_t lw6mat_xvec2_dot (const lw6mat_xvec2_t * xvec2_a,
				 const lw6mat_xvec2_t * xvec2_b);
extern void lw6mat_xvec2_cross (lw6mat_xvec3_t * xvec3,
				const lw6mat_xvec2_t * xvec2_a,
				const lw6mat_xvec2_t * xvec2_b);
extern void lw6mat_xvec2_scale (lw6mat_xvec2_t * xvec2, int32_t f);

/* mat-xvec3.c */
extern void lw6mat_xvec3_zero (lw6mat_xvec3_t * xvec3);
extern int lw6mat_xvec3_is_same (const lw6mat_xvec3_t * xvec3_a,
				 const lw6mat_xvec3_t * xvec3_b);
extern int32_t lw6mat_xvec3_len_sq (const lw6mat_xvec3_t * xvec3);
extern int32_t lw6mat_xvec3_len (const lw6mat_xvec3_t * xvec3);
extern int lw6mat_xvec3_norm (lw6mat_xvec3_t * xvec3);
extern void lw6mat_xvec3_neg (lw6mat_xvec3_t * xvec3);
extern void lw6mat_xvec3_add (lw6mat_xvec3_t * xvec3,
			      const lw6mat_xvec3_t * xvec3_a,
			      const lw6mat_xvec3_t * xvec3_b);
extern void lw6mat_xvec3_sub (lw6mat_xvec3_t * xvec3,
			      const lw6mat_xvec3_t * xvec3_a,
			      const lw6mat_xvec3_t * xvec3_b);
extern int32_t lw6mat_xvec3_dot (const lw6mat_xvec3_t * xvec3_a,
				 const lw6mat_xvec3_t * xvec3_b);
extern void lw6mat_xvec3_cross (lw6mat_xvec3_t * xvec3,
				const lw6mat_xvec3_t * xvec3_a,
				const lw6mat_xvec3_t * xvec3_b);
extern void lw6mat_xvec3_scale (lw6mat_xvec3_t * xvec3, int32_t f);

/* mat-xvec4.c */
extern void lw6mat_xvec4_zero (lw6mat_xvec4_t * xvec4);
extern int lw6mat_xvec4_is_same (const lw6mat_xvec4_t * xvec4_a,
				 const lw6mat_xvec4_t * xvec4_b);
extern int32_t lw6mat_xvec4_len_sq (const lw6mat_xvec4_t * xvec4);
extern int32_t lw6mat_xvec4_len (const lw6mat_xvec4_t * xvec4);
extern int lw6mat_xvec4_norm (lw6mat_xvec4_t * xvec4);
extern void lw6mat_xvec4_neg (lw6mat_xvec4_t * xvec4);
extern void lw6mat_xvec4_add (lw6mat_xvec4_t * xvec4,
			      const lw6mat_xvec4_t * xvec4_a,
			      const lw6mat_xvec4_t * xvec4_b);
extern void lw6mat_xvec4_sub (lw6mat_xvec4_t * xvec4,
			      const lw6mat_xvec4_t * xvec4_a,
			      const lw6mat_xvec4_t * xvec4_b);
extern int32_t lw6mat_xvec4_dot (const lw6mat_xvec4_t * xvec4_a,
				 const lw6mat_xvec4_t * xvec4_b);
extern void lw6mat_xvec4_cross (lw6mat_xvec3_t * xvec3,
				const lw6mat_xvec4_t * xvec4_a,
				const lw6mat_xvec4_t * xvec4_b);
extern void lw6mat_xvec4_scale (lw6mat_xvec4_t * xvec4, int32_t f);

/* mat-test.c */
extern int lw6mat_test_register (int mode);
extern int lw6mat_test_run (int mode);

#endif
