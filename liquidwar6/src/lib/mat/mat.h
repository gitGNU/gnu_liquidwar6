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
#define LW6MAT_D_0 0.0f
#define LW6MAT_X_0 0
#define LW6MAT_F_1 1.0f
#define LW6MAT_I_1 1
#define LW6MAT_D_1 1.0f
#define LW6MAT_X_1 65536

#define LW6MAT_SIMILAR_F_DIFF 0.001f
#define LW6MAT_SIMILAR_I_DIFF 0
#define LW6MAT_SIMILAR_D_DIFF 0.00001f
#define LW6MAT_SIMILAR_X_DIFF 4096
#define LW6MAT_SIMILAR_F_SCALE 0.0001f
#define LW6MAT_SIMILAR_I_SCALE 0
#define LW6MAT_SIMILAR_D_SCALE 0.0000001f
#define LW6MAT_SIMILAR_X_SCALE 256

#define LW6MAT_VEC2_V_SIZE 2
#define LW6MAT_VEC3_V_SIZE 3
#define LW6MAT_VEC4_V_SIZE 4

#define LW6MAT_MAT2_M_SIZE 2
#define LW6MAT_MAT2_V_SIZE_X_SIZE 4
#define LW6MAT_MAT3_M_SIZE 3
#define LW6MAT_MAT3_V_SIZE_X_SIZE 9
#define LW6MAT_MAT4_M_SIZE 4
#define LW6MAT_MAT4_V_SIZE_X_SIZE 16

#define LW6MAT_REPR_VEC "vector"
#define LW6MAT_REPR_MAT "matrix"
#define LW6MAT_REPR_F "float"
#define LW6MAT_REPR_I "integer"
#define LW6MAT_REPR_D "double"
#define LW6MAT_REPR_X "fixed point"

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
  float v[LW6MAT_VEC2_V_SIZE];
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
  float v[LW6MAT_VEC3_V_SIZE];
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
  float v[LW6MAT_VEC4_V_SIZE];
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
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  float m[LW6MAT_MAT2_M_SIZE][LW6MAT_MAT2_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*4+j.
   */
  float v[LW6MAT_MAT2_V_SIZE_X_SIZE];
} lw6mat_fmat2_t;

/**
 * Float 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  float m[LW6MAT_MAT3_M_SIZE][LW6MAT_MAT3_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*3+j.
   */
  float v[LW6MAT_MAT3_V_SIZE_X_SIZE];
} lw6mat_fmat3_t;

/**
 * Float 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  float m[LW6MAT_MAT4_M_SIZE][LW6MAT_MAT4_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*4+j.
   */
  float v[LW6MAT_MAT4_V_SIZE_X_SIZE];
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
  int32_t v[LW6MAT_VEC2_V_SIZE];
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
  int32_t v[LW6MAT_VEC3_V_SIZE];
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
  int32_t v[LW6MAT_VEC4_V_SIZE];
} lw6mat_ivec4_t;

/**
 * Integer 2x2 matrix (AKA 2D rectangle).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  int32_t m[LW6MAT_MAT2_M_SIZE][LW6MAT_MAT2_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*2+j.
   */
  int32_t v[LW6MAT_MAT2_V_SIZE_X_SIZE];
} lw6mat_imat2_t;

/**
 * Integer 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  int32_t m[LW6MAT_MAT3_M_SIZE][LW6MAT_MAT3_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*3+j.
   */
  int32_t v[LW6MAT_MAT3_V_SIZE_X_SIZE];
} lw6mat_imat3_t;

/**
 * Integer 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  int32_t m[LW6MAT_MAT4_M_SIZE][LW6MAT_MAT4_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*4+j.
   */
  int32_t v[LW6MAT_MAT4_V_SIZE_X_SIZE];
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
  double v[LW6MAT_VEC2_V_SIZE];
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
  double v[LW6MAT_VEC3_V_SIZE];
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
  double v[LW6MAT_VEC4_V_SIZE];
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
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  double m[LW6MAT_MAT2_M_SIZE][LW6MAT_MAT2_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*2+j.
   */
  double v[LW6MAT_MAT2_V_SIZE_X_SIZE];
} lw6mat_dmat2_t;

/**
 * Double 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  double m[LW6MAT_MAT3_M_SIZE][LW6MAT_MAT3_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*3+j.
   */
  double v[LW6MAT_MAT3_V_SIZE_X_SIZE];
} lw6mat_dmat3_t;

/**
 * Double 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  double m[LW6MAT_MAT4_M_SIZE][LW6MAT_MAT4_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*4+j.
   */
  double v[LW6MAT_MAT4_V_SIZE_X_SIZE];
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
  int32_t v[LW6MAT_VEC2_V_SIZE];
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
  int32_t v[LW6MAT_VEC3_V_SIZE];
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
  int32_t v[LW6MAT_VEC4_V_SIZE];
} lw6mat_xvec4_t;

/**
 * Fixed Point 2x2 matrix (AKA 2D rectangle).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  int32_t m[LW6MAT_MAT2_M_SIZE][LW6MAT_MAT2_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*3+j.
   */
  int32_t v[LW6MAT_MAT2_V_SIZE_X_SIZE];
} lw6mat_xmat2_t;

/**
 * Fixed Point 3x3 matrix (AKA 3D triangle).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  int32_t m[LW6MAT_MAT3_M_SIZE][LW6MAT_MAT3_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*3+j.
   */
  int32_t v[LW6MAT_MAT3_V_SIZE_X_SIZE];
} lw6mat_xmat3_t;

/**
 * Fixed Point 4x4 matrix (AKA 3D transformation/composition matrix).
 */
typedef union
{
  /**
   * Accessor with 2 dimensions array index.
   * The convention is column-major mode as done in OpenGL
   * so that m[i][j] is element at column i and row j.
   * Beware, this is not the most natural order for a
   * C programmer.
   */
  int32_t m[LW6MAT_MAT4_M_SIZE][LW6MAT_MAT4_M_SIZE];
  /**
   * Accessor with flat array index. To access element
   * a column i and row j, use i*4+j.
   */
  int32_t v[LW6MAT_MAT4_V_SIZE_X_SIZE];
} lw6mat_xmat4_t;

/*
 * Matrix index calculus. While you should probably
 * not use this (use the built-in m[][] accessor instead
 * they are provided to explicitly show memory layout.
 */
static inline int __attribute__ ((const)) lw6mat_mat2_v_index (int i_column, int j_row)
{
  return i_column * LW6MAT_MAT2_M_SIZE + j_row;
}

static inline int __attribute__ ((const)) lw6mat_mat3_v_index (int i_column, int j_row)
{
  return i_column * LW6MAT_MAT3_M_SIZE + j_row;
}

static inline int __attribute__ ((const)) lw6mat_mat4_v_index (int i_column, int j_row)
{
  return i_column * LW6MAT_MAT4_M_SIZE + j_row;
}

/*
 * Conversions.
 */

static inline int32_t __attribute__ ((const)) lw6mat_ftoi (float f)
{
  /*
   * Calling round() gives a better conversion
   * than a plain cast, which does an implicit floor()
   */
  return (int32_t) round (f);
}

static inline double __attribute__ ((const)) lw6mat_ftod (float f)
{
  return (double) f;
}

static inline int32_t __attribute__ ((const)) lw6mat_ftox (float f)
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

static inline float __attribute__ ((const)) lw6mat_itof (int32_t i)
{
  return (float) i;
}

static inline double __attribute__ ((const)) lw6mat_itod (int32_t i)
{
  return (double) i;
}

static inline int32_t __attribute__ ((const)) lw6mat_itox (int32_t i)
{
  return i << LW6MAT_X_SHIFT_I;
}

static inline float __attribute__ ((const)) lw6mat_dtof (double d)
{
  return (float) d;
}

static inline int32_t __attribute__ ((const)) lw6mat_dtoi (double d)
{
  /*
   * Calling round() gives a better conversion
   * than a plain cast, which does an implicit floor()
   */
  return (int32_t) round (d);
}

static inline int32_t __attribute__ ((const)) lw6mat_dtox (double d)
{
  return (int32_t) round (d * LW6MAT_X_MUL_F);
}

static inline float __attribute__ ((const)) lw6mat_xtof (int32_t x)
{
  return ((float) x) / LW6MAT_X_MUL_F;
}

static inline int32_t __attribute__ ((const)) lw6mat_xtoi (int32_t x)
{
  /*
   * We add 0.5 to the number before truncating
   * its left part, this simulates a round()
   * and provides more consistent conversions.
   */
  return (x + LW6MAT_X_ROUND_I) >> LW6MAT_X_SHIFT_I;
}

static inline double __attribute__ ((const)) lw6mat_xtod (int32_t x)
{
  return ((double) x) / LW6MAT_X_MUL_F;
}

/*
 * Specific fixed-point routines
 */

static inline int32_t __attribute__ ((const)) lw6mat_x_mul_32 (int32_t a, int32_t b)
{
  /*
   * Multiplying two fixed-point numbers, does a rough approximation
   * which will make it behave OK when numbers are close to 1, but
   * numbers which are two small won't behave the right way.
   */
  return ((a + LW6MAT_X_ROUND_HALF_I) >> LW6MAT_X_SHIFT_HALF_I) * ((b + LW6MAT_X_ROUND_HALF_I) >> LW6MAT_X_SHIFT_HALF_I);
}

static inline int32_t __attribute__ ((const)) lw6mat_x_div_32 (int32_t a, int32_t b)
{
  /*
   * Dividing two fixed-point numbers, does a rough approximation
   * which will make it behave OK when numbers are close to 1, but
   * if a is too big or b is too small, won't behave the right way.
   */
  return ((a << LW6MAT_X_SHIFT_HALF_I) / ((b + LW6MAT_X_ROUND_HALF_I) >> LW6MAT_X_SHIFT_HALF_I));
}

static inline int32_t __attribute__ ((const)) lw6mat_x_inv_32 (int32_t a)
{
  /*
   * Inverse of a fixed-point number, does a rough approximation
   * which will make it behave OK when numbers are close to 1, but
   * if a is too big, won't behave the right way.
   */
  return ((LW6MAT_X_1 << LW6MAT_X_SHIFT_HALF_I) / ((a + LW6MAT_X_ROUND_HALF_I) >> LW6MAT_X_SHIFT_HALF_I));
}

static inline int32_t __attribute__ ((const)) lw6mat_x_mul_64 (int32_t a, int32_t b)
{
  /*
   * Multiplying two fixed point numbers, will give the exact result
   * apart from out-of-range errors, but does it at the cost of a 64-bit
   * conversion and intermediate calc, which is slow.
   */
  return ((int32_t) (((((int64_t) a) * ((int64_t) b)) + LW6MAT_X_ROUND_I) >> LW6MAT_X_SHIFT_I));
}

static inline int32_t __attribute__ ((const)) lw6mat_x_div_64 (int32_t a, int32_t b)
{
  /*
   * Dividing two fixed point numbers, will give the exact result
   * apart from out-of-range errors, but does it at the cost of a 64-bit
   * conversion and intermediate calc, which is slow.
   */
  return ((int32_t) ((((int64_t) a) << LW6MAT_X_SHIFT_I) / ((int64_t) b)));
}

static inline int32_t __attribute__ ((const)) lw6mat_x_inv_64 (int32_t a)
{
  /*
   * Inverse of a fixed point numbers, will give the exact result
   * apart from out-of-range errors, but does it at the cost of a 64-bit
   * conversion and intermediate calc, which is slow.
   */
  return ((int32_t) ((((int64_t) LW6MAT_X_1) << LW6MAT_X_SHIFT_I) / ((int64_t) a)));
}

/* mat-fvec2.c */
extern void lw6mat_fvec2_zero (lw6mat_fvec2_t * fvec2);
extern int lw6mat_fvec2_is_same (const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b);
extern float lw6mat_fvec2_len_sq (const lw6mat_fvec2_t * fvec2);
extern float lw6mat_fvec2_len (const lw6mat_fvec2_t * fvec2);
extern int lw6mat_fvec2_normalize (lw6sys_context_t * sys_context, lw6mat_fvec2_t * fvec2);
extern int lw6mat_fvec2_homogeneous (lw6sys_context_t * sys_context, lw6mat_fvec2_t * fvec2);
extern void lw6mat_fvec2_neg (lw6mat_fvec2_t * fvec2);
extern void lw6mat_fvec2_add (lw6mat_fvec2_t * fvec2, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b);
extern void lw6mat_fvec2_sub (lw6mat_fvec2_t * fvec2, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b);
extern float lw6mat_fvec2_dot (const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b);
extern void lw6mat_fvec2_cross (lw6mat_fvec3_t * fvec3, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b);
extern void lw6mat_fvec2_mul_scale (lw6mat_fvec2_t * fvec2, float f);
extern void lw6mat_fvec2_mul_fvec2 (lw6mat_fmat2_t * fmat2, const lw6mat_fvec2_t * fvec2_a, const lw6mat_fvec2_t * fvec2_b);
extern char *lw6mat_fvec2_repr (lw6sys_context_t * sys_context, const lw6mat_fvec2_t * fvec2);

/* mat-fvec3.c */
extern void lw6mat_fvec3_zero (lw6mat_fvec3_t * fvec3);
extern int lw6mat_fvec3_is_same (const lw6mat_fvec3_t * fvec3_a, const lw6mat_fvec3_t * fvec3_b);
extern float lw6mat_fvec3_len_sq (const lw6mat_fvec3_t * fvec3);
extern float lw6mat_fvec3_len (const lw6mat_fvec3_t * fvec3);
extern int lw6mat_fvec3_normalize (lw6sys_context_t * sys_context, lw6mat_fvec3_t * fvec3);
extern int lw6mat_fvec3_homogeneous (lw6sys_context_t * sys_context, lw6mat_fvec3_t * fvec3);
extern void lw6mat_fvec3_neg (lw6mat_fvec3_t * fvec3);
extern void lw6mat_fvec3_add (lw6mat_fvec3_t * fvec3, const lw6mat_fvec3_t * fvec3_a, const lw6mat_fvec3_t * fvec3_b);
extern void lw6mat_fvec3_sub (lw6mat_fvec3_t * fvec3, const lw6mat_fvec3_t * fvec3_a, const lw6mat_fvec3_t * fvec3_b);
extern float lw6mat_fvec3_dot (const lw6mat_fvec3_t * fvec3_a, const lw6mat_fvec3_t * fvec3_b);
extern void lw6mat_fvec3_cross (lw6mat_fvec3_t * fvec3, const lw6mat_fvec3_t * fvec3_a, const lw6mat_fvec3_t * fvec3_b);
extern void lw6mat_fvec3_mul_scale (lw6mat_fvec3_t * fvec3, float f);
extern void lw6mat_fvec3_mul_fvec3 (lw6mat_fmat3_t * fmat3, const lw6mat_fvec3_t * fvec3_a, const lw6mat_fvec3_t * fvec3_b);
extern char *lw6mat_fvec3_repr (lw6sys_context_t * sys_context, const lw6mat_fvec3_t * fvec3);

/* mat-fvec4.c */
extern void lw6mat_fvec4_zero (lw6mat_fvec4_t * fvec4);
extern int lw6mat_fvec4_is_same (const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b);
extern float lw6mat_fvec4_len_sq (const lw6mat_fvec4_t * fvec4);
extern float lw6mat_fvec4_len (const lw6mat_fvec4_t * fvec4);
extern int lw6mat_fvec4_normalize (lw6sys_context_t * sys_context, lw6mat_fvec4_t * fvec4);
extern int lw6mat_fvec4_homogeneous (lw6sys_context_t * sys_context, lw6mat_fvec4_t * fvec4);
extern void lw6mat_fvec4_neg (lw6mat_fvec4_t * fvec4);
extern void lw6mat_fvec4_add (lw6mat_fvec4_t * fvec4, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b);
extern void lw6mat_fvec4_sub (lw6mat_fvec4_t * fvec4, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b);
extern float lw6mat_fvec4_dot (const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b);
extern void lw6mat_fvec4_cross (lw6mat_fvec3_t * fvec3, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b);
extern void lw6mat_fvec4_mul_scale (lw6mat_fvec4_t * fvec4, float f);
extern void lw6mat_fvec4_mul_fvec4 (lw6mat_fmat4_t * fmat4, const lw6mat_fvec4_t * fvec4_a, const lw6mat_fvec4_t * fvec4_b);
extern char *lw6mat_fvec4_repr (lw6sys_context_t * sys_context, const lw6mat_fvec4_t * fvec4);

/* mat-ivec2.c */
extern void lw6mat_ivec2_zero (lw6mat_ivec2_t * ivec2);
extern int lw6mat_ivec2_is_same (const lw6mat_ivec2_t * ivec2_a, const lw6mat_ivec2_t * ivec2_b);
extern int32_t lw6mat_ivec2_len_sq (const lw6mat_ivec2_t * ivec2);
extern int32_t lw6mat_ivec2_len (const lw6mat_ivec2_t * ivec2);
extern int lw6mat_ivec2_normalize (lw6sys_context_t * sys_context, lw6mat_ivec2_t * ivec2);
extern void lw6mat_ivec2_neg (lw6mat_ivec2_t * ivec2);
extern void lw6mat_ivec2_add (lw6mat_ivec2_t * ivec2, const lw6mat_ivec2_t * ivec2_a, const lw6mat_ivec2_t * ivec2_b);
extern void lw6mat_ivec2_sub (lw6mat_ivec2_t * ivec2, const lw6mat_ivec2_t * ivec2_a, const lw6mat_ivec2_t * ivec2_b);
extern int32_t lw6mat_ivec2_dot (const lw6mat_ivec2_t * ivec2_a, const lw6mat_ivec2_t * ivec2_b);
extern void lw6mat_ivec2_cross (lw6mat_ivec3_t * ivec3, const lw6mat_ivec2_t * ivec2_a, const lw6mat_ivec2_t * ivec2_b);
extern void lw6mat_ivec2_mul_scale (lw6mat_ivec2_t * ivec2, int32_t f);
extern char *lw6mat_ivec2_repr (lw6sys_context_t * sys_context, const lw6mat_ivec2_t * ivec2);

/* mat-ivec3.c */
extern void lw6mat_ivec3_zero (lw6mat_ivec3_t * ivec3);
extern int lw6mat_ivec3_is_same (const lw6mat_ivec3_t * ivec3_a, const lw6mat_ivec3_t * ivec3_b);
extern int32_t lw6mat_ivec3_len_sq (const lw6mat_ivec3_t * ivec3);
extern int32_t lw6mat_ivec3_len (const lw6mat_ivec3_t * ivec3);
extern int lw6mat_ivec3_normalize (lw6sys_context_t * sys_context, lw6mat_ivec3_t * ivec3);
extern void lw6mat_ivec3_neg (lw6mat_ivec3_t * ivec3);
extern void lw6mat_ivec3_add (lw6mat_ivec3_t * ivec3, const lw6mat_ivec3_t * ivec3_a, const lw6mat_ivec3_t * ivec3_b);
extern void lw6mat_ivec3_sub (lw6mat_ivec3_t * ivec3, const lw6mat_ivec3_t * ivec3_a, const lw6mat_ivec3_t * ivec3_b);
extern int32_t lw6mat_ivec3_dot (const lw6mat_ivec3_t * ivec3_a, const lw6mat_ivec3_t * ivec3_b);
extern void lw6mat_ivec3_cross (lw6mat_ivec3_t * ivec3, const lw6mat_ivec3_t * ivec3_a, const lw6mat_ivec3_t * ivec3_b);
extern void lw6mat_ivec3_mul_scale (lw6mat_ivec3_t * ivec3, int32_t f);
extern char *lw6mat_ivec3_repr (lw6sys_context_t * sys_context, const lw6mat_ivec3_t * ivec3);

/* mat-ivec4.c */
extern void lw6mat_ivec4_zero (lw6mat_ivec4_t * ivec4);
extern int lw6mat_ivec4_is_same (const lw6mat_ivec4_t * ivec4_a, const lw6mat_ivec4_t * ivec4_b);
extern int32_t lw6mat_ivec4_len_sq (const lw6mat_ivec4_t * ivec4);
extern int32_t lw6mat_ivec4_len (const lw6mat_ivec4_t * ivec4);
extern int lw6mat_ivec4_normalize (lw6sys_context_t * sys_context, lw6mat_ivec4_t * ivec4);
extern void lw6mat_ivec4_neg (lw6mat_ivec4_t * ivec4);
extern void lw6mat_ivec4_add (lw6mat_ivec4_t * ivec4, const lw6mat_ivec4_t * ivec4_a, const lw6mat_ivec4_t * ivec4_b);
extern void lw6mat_ivec4_sub (lw6mat_ivec4_t * ivec4, const lw6mat_ivec4_t * ivec4_a, const lw6mat_ivec4_t * ivec4_b);
extern int32_t lw6mat_ivec4_dot (const lw6mat_ivec4_t * ivec4_a, const lw6mat_ivec4_t * ivec4_b);
extern void lw6mat_ivec4_cross (lw6mat_ivec3_t * ivec3, const lw6mat_ivec4_t * ivec4_a, const lw6mat_ivec4_t * ivec4_b);
extern void lw6mat_ivec4_mul_scale (lw6mat_ivec4_t * ivec4, int32_t f);
extern char *lw6mat_ivec4_repr (lw6sys_context_t * sys_context, const lw6mat_ivec3_t * ivec4);

/* mat-dvec2.c */
extern void lw6mat_dvec2_zero (lw6mat_dvec2_t * dvec2);
extern int lw6mat_dvec2_is_same (const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b);
extern double lw6mat_dvec2_len_sq (const lw6mat_dvec2_t * dvec2);
extern double lw6mat_dvec2_len (const lw6mat_dvec2_t * dvec2);
extern int lw6mat_dvec2_normalize (lw6sys_context_t * sys_context, lw6mat_dvec2_t * dvec2);
extern int lw6mat_dvec2_homogeneous (lw6sys_context_t * sys_context, lw6mat_dvec2_t * dvec2);
extern void lw6mat_dvec2_neg (lw6mat_dvec2_t * dvec2);
extern void lw6mat_dvec2_add (lw6mat_dvec2_t * dvec2, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b);
extern void lw6mat_dvec2_sub (lw6mat_dvec2_t * dvec2, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b);
extern double lw6mat_dvec2_dot (const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b);
extern void lw6mat_dvec2_cross (lw6mat_dvec3_t * dvec3, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b);
extern void lw6mat_dvec2_mul_scale (lw6mat_dvec2_t * dvec2, double f);
extern void lw6mat_dvec2_mul_dvec2 (lw6mat_dmat2_t * dmat2, const lw6mat_dvec2_t * dvec2_a, const lw6mat_dvec2_t * dvec2_b);
extern char *lw6mat_dvec2_repr (lw6sys_context_t * sys_context, const lw6mat_dvec2_t * dvec2);

/* mat-dvec3.c */
extern void lw6mat_dvec3_zero (lw6mat_dvec3_t * dvec3);
extern int lw6mat_dvec3_is_same (const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b);
extern double lw6mat_dvec3_len_sq (const lw6mat_dvec3_t * dvec3);
extern double lw6mat_dvec3_len (const lw6mat_dvec3_t * dvec3);
extern int lw6mat_dvec3_normalize (lw6sys_context_t * sys_context, lw6mat_dvec3_t * dvec3);
extern int lw6mat_dvec3_homogeneous (lw6sys_context_t * sys_context, lw6mat_dvec3_t * dvec3);
extern void lw6mat_dvec3_neg (lw6mat_dvec3_t * dvec3);
extern void lw6mat_dvec3_add (lw6mat_dvec3_t * dvec3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b);
extern void lw6mat_dvec3_sub (lw6mat_dvec3_t * dvec3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b);
extern double lw6mat_dvec3_dot (const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b);
extern void lw6mat_dvec3_cross (lw6mat_dvec3_t * dvec3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b);
extern void lw6mat_dvec3_mul_scale (lw6mat_dvec3_t * dvec3, double f);
extern void lw6mat_dvec3_mul_dvec3 (lw6mat_dmat3_t * dmat3, const lw6mat_dvec3_t * dvec3_a, const lw6mat_dvec3_t * dvec3_b);
extern char *lw6mat_dvec3_repr (lw6sys_context_t * sys_context, const lw6mat_dvec3_t * dvec3);

/* mat-dvec4.c */
extern void lw6mat_dvec4_zero (lw6mat_dvec4_t * dvec4);
extern int lw6mat_dvec4_is_same (const lw6mat_dvec4_t * dvec4_a, const lw6mat_dvec4_t * dvec4_b);
extern double lw6mat_dvec4_len_sq (const lw6mat_dvec4_t * dvec4);
extern double lw6mat_dvec4_len (const lw6mat_dvec4_t * dvec4);
extern int lw6mat_dvec4_normalize (lw6sys_context_t * sys_context, lw6mat_dvec4_t * dvec4);
extern int lw6mat_dvec4_homogeneous (lw6sys_context_t * sys_context, lw6mat_dvec4_t * dvec4);
extern void lw6mat_dvec4_neg (lw6mat_dvec4_t * dvec4);
extern void lw6mat_dvec4_add (lw6mat_dvec4_t * dvec4, const lw6mat_dvec4_t * dvec4_a, const lw6mat_dvec4_t * dvec4_b);
extern void lw6mat_dvec4_sub (lw6mat_dvec4_t * dvec4, const lw6mat_dvec4_t * dvec4_a, const lw6mat_dvec4_t * dvec4_b);
extern double lw6mat_dvec4_dot (const lw6mat_dvec4_t * dvec4_a, const lw6mat_dvec4_t * dvec4_b);
extern void lw6mat_dvec4_cross (lw6mat_dvec3_t * dvec3, const lw6mat_dvec4_t * dvec4_a, const lw6mat_dvec4_t * dvec4_b);
extern void lw6mat_dvec4_mul_scale (lw6mat_dvec4_t * dvec4, double f);
extern void lw6mat_dvec4_mul_dvec4 (lw6mat_dmat4_t * dmat4, const lw6mat_dvec4_t * dvec4_a, const lw6mat_dvec4_t * dvec4_b);
extern char *lw6mat_dvec4_repr (lw6sys_context_t * sys_context, const lw6mat_dvec4_t * dvec4);

/* mat-xvec2.c */
extern void lw6mat_xvec2_zero (lw6mat_xvec2_t * xvec2);
extern int lw6mat_xvec2_is_same (const lw6mat_xvec2_t * xvec2_a, const lw6mat_xvec2_t * xvec2_b);
extern int32_t lw6mat_xvec2_len_sq (const lw6mat_xvec2_t * xvec2);
extern int32_t lw6mat_xvec2_len (const lw6mat_xvec2_t * xvec2);
extern int lw6mat_xvec2_normalize (lw6sys_context_t * sys_context, lw6mat_xvec2_t * xvec2);
extern void lw6mat_xvec2_neg (lw6mat_xvec2_t * xvec2);
extern void lw6mat_xvec2_add (lw6mat_xvec2_t * xvec2, const lw6mat_xvec2_t * xvec2_a, const lw6mat_xvec2_t * xvec2_b);
extern void lw6mat_xvec2_sub (lw6mat_xvec2_t * xvec2, const lw6mat_xvec2_t * xvec2_a, const lw6mat_xvec2_t * xvec2_b);
extern int32_t lw6mat_xvec2_dot (const lw6mat_xvec2_t * xvec2_a, const lw6mat_xvec2_t * xvec2_b);
extern void lw6mat_xvec2_cross (lw6mat_xvec3_t * xvec3, const lw6mat_xvec2_t * xvec2_a, const lw6mat_xvec2_t * xvec2_b);
extern void lw6mat_xvec2_mul_scale (lw6mat_xvec2_t * xvec2, int32_t f);
extern char *lw6mat_xvec2_repr (lw6sys_context_t * sys_context, const lw6mat_xvec2_t * xvec2);

/* mat-xvec3.c */
extern void lw6mat_xvec3_zero (lw6mat_xvec3_t * xvec3);
extern int lw6mat_xvec3_is_same (const lw6mat_xvec3_t * xvec3_a, const lw6mat_xvec3_t * xvec3_b);
extern int32_t lw6mat_xvec3_len_sq (const lw6mat_xvec3_t * xvec3);
extern int32_t lw6mat_xvec3_len (const lw6mat_xvec3_t * xvec3);
extern int lw6mat_xvec3_normalize (lw6sys_context_t * sys_context, lw6mat_xvec3_t * xvec3);
extern void lw6mat_xvec3_neg (lw6mat_xvec3_t * xvec3);
extern void lw6mat_xvec3_add (lw6mat_xvec3_t * xvec3, const lw6mat_xvec3_t * xvec3_a, const lw6mat_xvec3_t * xvec3_b);
extern void lw6mat_xvec3_sub (lw6mat_xvec3_t * xvec3, const lw6mat_xvec3_t * xvec3_a, const lw6mat_xvec3_t * xvec3_b);
extern int32_t lw6mat_xvec3_dot (const lw6mat_xvec3_t * xvec3_a, const lw6mat_xvec3_t * xvec3_b);
extern void lw6mat_xvec3_cross (lw6mat_xvec3_t * xvec3, const lw6mat_xvec3_t * xvec3_a, const lw6mat_xvec3_t * xvec3_b);
extern void lw6mat_xvec3_mul_scale (lw6mat_xvec3_t * xvec3, int32_t f);
extern char *lw6mat_xvec3_repr (lw6sys_context_t * sys_context, const lw6mat_xvec3_t * xvec3);

/* mat-xvec4.c */
extern void lw6mat_xvec4_zero (lw6mat_xvec4_t * xvec4);
extern int lw6mat_xvec4_is_same (const lw6mat_xvec4_t * xvec4_a, const lw6mat_xvec4_t * xvec4_b);
extern int32_t lw6mat_xvec4_len_sq (const lw6mat_xvec4_t * xvec4);
extern int32_t lw6mat_xvec4_len (const lw6mat_xvec4_t * xvec4);
extern int lw6mat_xvec4_normalize (lw6sys_context_t * sys_context, lw6mat_xvec4_t * xvec4);
extern void lw6mat_xvec4_neg (lw6mat_xvec4_t * xvec4);
extern void lw6mat_xvec4_add (lw6mat_xvec4_t * xvec4, const lw6mat_xvec4_t * xvec4_a, const lw6mat_xvec4_t * xvec4_b);
extern void lw6mat_xvec4_sub (lw6mat_xvec4_t * xvec4, const lw6mat_xvec4_t * xvec4_a, const lw6mat_xvec4_t * xvec4_b);
extern int32_t lw6mat_xvec4_dot (const lw6mat_xvec4_t * xvec4_a, const lw6mat_xvec4_t * xvec4_b);
extern void lw6mat_xvec4_cross (lw6mat_xvec3_t * xvec3, const lw6mat_xvec4_t * xvec4_a, const lw6mat_xvec4_t * xvec4_b);
extern void lw6mat_xvec4_mul_scale (lw6mat_xvec4_t * xvec4, int32_t f);
extern char *lw6mat_xvec4_repr (lw6sys_context_t * sys_context, const lw6mat_xvec4_t * xvec4);

/* mat-fmat2.c */
extern void lw6mat_fmat2_zero (lw6mat_fmat2_t * fmat2);
extern void lw6mat_fmat2_identity (lw6mat_fmat2_t * fmat2);
extern void lw6mat_fmat2_translation (lw6mat_fmat2_t * fmat2, float f);
extern void lw6mat_fmat2_scale (lw6mat_fmat2_t * fmat2, float f);
extern int lw6mat_fmat2_is_same (const lw6mat_fmat2_t * fmat2_a, const lw6mat_fmat2_t * fmat2_b);
extern void lw6mat_fmat2_transpose (lw6mat_fmat2_t * fmat2);
extern float lw6mat_fmat2_det (const lw6mat_fmat2_t * fmat2);
extern void lw6mat_fmat2_mul_scale (lw6mat_fmat2_t * fmat2, float f);
extern int lw6mat_fmat2_inv (lw6sys_context_t * sys_context, lw6mat_fmat2_t * fmat2_dst, const lw6mat_fmat2_t * fmat2_src);
extern void lw6mat_fmat2_mul_fmat2 (lw6mat_fmat2_t * fmat2, const lw6mat_fmat2_t * fmat2_a, const lw6mat_fmat2_t * fmat2_b);
extern void lw6mat_fmat2_mul_fvec2 (lw6mat_fvec2_t * fvec2_dst, const lw6mat_fmat2_t * fmat2, const lw6mat_fvec2_t * fvec2_src);
extern char *lw6mat_fmat2_repr (lw6sys_context_t * sys_context, const lw6mat_fmat2_t * fmat2);

/* mat-fmat3.c */
extern void lw6mat_fmat3_zero (lw6mat_fmat3_t * fmat3);
extern void lw6mat_fmat3_identity (lw6mat_fmat3_t * fmat3);
extern void lw6mat_fmat3_translation (lw6mat_fmat3_t * fmat3, const lw6mat_fvec2_t * vec2);
extern void lw6mat_fmat3_scale (lw6mat_fmat3_t * fmat3, const lw6mat_fvec2_t * vec2);
extern void lw6mat_fmat3_rot (lw6mat_fmat3_t * fmat3, float r);
extern int lw6mat_fmat3_is_same (const lw6mat_fmat3_t * fmat3_a, const lw6mat_fmat3_t * fmat3_b);
extern void lw6mat_fmat3_transpose (lw6mat_fmat3_t * fmat3);
extern float lw6mat_fmat3_det (const lw6mat_fmat3_t * fmat3);
extern void lw6mat_fmat3_mul_scale (lw6mat_fmat3_t * fmat3, float f);
extern int lw6mat_fmat3_inv (lw6sys_context_t * sys_context, lw6mat_fmat3_t * fmat3_dst, const lw6mat_fmat3_t * fmat3_src);
extern void lw6mat_fmat3_mul_fmat3 (lw6mat_fmat3_t * fmat3, const lw6mat_fmat3_t * fmat3_a, const lw6mat_fmat3_t * fmat3_b);
extern void lw6mat_fmat3_mul_fvec3 (lw6mat_fvec3_t * fvec3_dst, const lw6mat_fmat3_t * fmat3, const lw6mat_fvec3_t * fvec3_src);
extern void lw6mat_fmat3_mul_fvec2 (lw6mat_fvec2_t * fvec2_dst, const lw6mat_fmat3_t * fmat3, const lw6mat_fvec2_t * fvec2_src);
extern char *lw6mat_fmat3_repr (lw6sys_context_t * sys_context, const lw6mat_fmat3_t * fmat3);

/* mat-fmat4.c */
extern void lw6mat_fmat4_zero (lw6mat_fmat4_t * fmat4);
extern void lw6mat_fmat4_identity (lw6mat_fmat4_t * fmat4);
extern void lw6mat_fmat4_translation (lw6mat_fmat4_t * fmat4, const lw6mat_fvec3_t * vec3);
extern void lw6mat_fmat4_scale (lw6mat_fmat4_t * fmat4, const lw6mat_fvec3_t * vec3);
extern void lw6mat_fmat4_rot_x (lw6mat_fmat4_t * fmat4, float r);
extern void lw6mat_fmat4_rot_y (lw6mat_fmat4_t * fmat4, float r);
extern void lw6mat_fmat4_rot_z (lw6mat_fmat4_t * fmat4, float r);
extern void lw6mat_fmat4_ortho (lw6mat_fmat4_t * fmat4, float left, float right, float bottom, float top, float nearval, float farval);
extern void lw6mat_fmat4_perspective (lw6mat_fmat4_t * fmat4, float fovy, float aspect, float znear, float zfar);
extern int lw6mat_fmat4_is_same (const lw6mat_fmat4_t * fmat4_a, const lw6mat_fmat4_t * fmat4_b);
extern void lw6mat_fmat4_transpose (lw6mat_fmat4_t * fmat4);
extern float lw6mat_fmat4_det (const lw6mat_fmat4_t * fmat4);
extern void lw6mat_fmat4_mul_scale (lw6mat_fmat4_t * fmat4, float f);
extern int lw6mat_fmat4_inv (lw6sys_context_t * sys_context, lw6mat_fmat4_t * fmat4_dst, const lw6mat_fmat4_t * fmat4_src);
extern void lw6mat_fmat4_mul_fmat4 (lw6mat_fmat4_t * fmat4, const lw6mat_fmat4_t * fmat4_a, const lw6mat_fmat4_t * fmat4_b);
extern void lw6mat_fmat4_mul_fvec4 (lw6mat_fvec4_t * fvec4_dst, const lw6mat_fmat4_t * fmat4, const lw6mat_fvec4_t * fvec4_src);
extern void lw6mat_fmat4_mul_fvec3 (lw6mat_fvec3_t * fvec3_dst, const lw6mat_fmat4_t * fmat4, const lw6mat_fvec3_t * fvec3_src);
extern char *lw6mat_fmat4_repr (lw6sys_context_t * sys_context, const lw6mat_fmat4_t * fmat4);

/* mat-dmat2.c */
extern void lw6mat_dmat2_zero (lw6mat_dmat2_t * dmat2);
extern void lw6mat_dmat2_identity (lw6mat_dmat2_t * dmat2);
extern void lw6mat_dmat2_translation (lw6mat_dmat2_t * dmat2, double d);
extern void lw6mat_dmat2_scale (lw6mat_dmat2_t * dmat2, double d);
extern int lw6mat_dmat2_is_same (const lw6mat_dmat2_t * dmat2_a, const lw6mat_dmat2_t * dmat2_b);
extern void lw6mat_dmat2_transpose (lw6mat_dmat2_t * dmat2);
extern double lw6mat_dmat2_det (const lw6mat_dmat2_t * dmat2);
extern void lw6mat_dmat2_mul_scale (lw6mat_dmat2_t * dmat2, double f);
extern int lw6mat_dmat2_inv (lw6sys_context_t * sys_context, lw6mat_dmat2_t * dmat2_dst, const lw6mat_dmat2_t * dmat2_src);
extern void lw6mat_dmat2_mul_dmat2 (lw6mat_dmat2_t * dmat2, const lw6mat_dmat2_t * dmat2_a, const lw6mat_dmat2_t * dmat2_b);
extern void lw6mat_dmat2_mul_dvec2 (lw6mat_dvec2_t * dvec2_dst, const lw6mat_dmat2_t * dmat2, const lw6mat_dvec2_t * dvec2_src);
extern char *lw6mat_dmat2_repr (lw6sys_context_t * sys_context, const lw6mat_dmat2_t * dmat2);

/* mat-dmat3.c */
extern void lw6mat_dmat3_zero (lw6mat_dmat3_t * dmat3);
extern void lw6mat_dmat3_identity (lw6mat_dmat3_t * dmat3);
extern void lw6mat_dmat3_translation (lw6mat_dmat3_t * dmat3, const lw6mat_dvec2_t * dvec2);
extern void lw6mat_dmat3_scale (lw6mat_dmat3_t * dmat3, const lw6mat_dvec2_t * dev2);
extern void lw6mat_dmat3_rot (lw6mat_dmat3_t * dmat3, double r);
extern int lw6mat_dmat3_is_same (const lw6mat_dmat3_t * dmat3_a, const lw6mat_dmat3_t * dmat3_b);
extern void lw6mat_dmat3_transpose (lw6mat_dmat3_t * dmat3);
extern double lw6mat_dmat3_det (const lw6mat_dmat3_t * dmat3);
extern void lw6mat_dmat3_mul_scale (lw6mat_dmat3_t * dmat3, double f);
extern int lw6mat_dmat3_inv (lw6sys_context_t * sys_context, lw6mat_dmat3_t * dmat3_dst, const lw6mat_dmat3_t * dmat3_src);
extern void lw6mat_dmat3_mul_dmat3 (lw6mat_dmat3_t * dmat3, const lw6mat_dmat3_t * dmat3_a, const lw6mat_dmat3_t * dmat3_b);
extern void lw6mat_dmat3_mul_dvec3 (lw6mat_dvec3_t * dvec3_dst, const lw6mat_dmat3_t * dmat3, const lw6mat_dvec3_t * dvec3_src);
extern void lw6mat_dmat3_mul_dvec2 (lw6mat_dvec2_t * dvec2_dst, const lw6mat_dmat3_t * fmat3, const lw6mat_dvec2_t * dvec2_src);
extern char *lw6mat_dmat3_repr (lw6sys_context_t * sys_context, const lw6mat_dmat3_t * dmat3);

/* mat-dmat4.c */
extern void lw6mat_dmat4_zero (lw6mat_dmat4_t * dmat4);
extern void lw6mat_dmat4_identity (lw6mat_dmat4_t * dmat4);
extern void lw6mat_dmat4_translation (lw6mat_dmat4_t * dmat4, const lw6mat_dvec3_t * dvec3);
extern void lw6mat_dmat4_scale (lw6mat_dmat4_t * dmat4, const lw6mat_dvec3_t * dvec3);
extern void lw6mat_dmat4_rot_x (lw6mat_dmat4_t * dmat4, double r);
extern void lw6mat_dmat4_rot_y (lw6mat_dmat4_t * dmat4, double r);
extern void lw6mat_dmat4_rot_z (lw6mat_dmat4_t * dmat4, double r);
extern void lw6mat_dmat4_ortho (lw6mat_dmat4_t * dmat4, double left, double right, double bottom, double top, double nearval, double farval);
extern void lw6mat_dmat4_perspective (lw6mat_dmat4_t * dmat4, double fovy, double aspect, double znear, double zfar);
extern int lw6mat_dmat4_is_same (const lw6mat_dmat4_t * dmat4_a, const lw6mat_dmat4_t * dmat4_b);
extern void lw6mat_dmat4_transpose (lw6mat_dmat4_t * dmat4);
extern double lw6mat_dmat4_det (const lw6mat_dmat4_t * dmat4);
extern void lw6mat_dmat4_mul_scale (lw6mat_dmat4_t * dmat4, double f);
extern int lw6mat_dmat4_inv (lw6sys_context_t * sys_context, lw6mat_dmat4_t * dmat4_dst, const lw6mat_dmat4_t * dmat4_src);
extern void lw6mat_dmat4_mul_dmat4 (lw6mat_dmat4_t * dmat4, const lw6mat_dmat4_t * dmat4_a, const lw6mat_dmat4_t * dmat4_b);
extern void lw6mat_dmat4_mul_dvec4 (lw6mat_dvec4_t * dvec4_dst, const lw6mat_dmat4_t * dmat4, const lw6mat_dvec4_t * dvec4_src);
extern void lw6mat_dmat4_mul_dvec3 (lw6mat_dvec3_t * dvec3_dst, const lw6mat_dmat4_t * dmat4, const lw6mat_dvec3_t * dvec3_src);
extern char *lw6mat_dmat4_repr (lw6sys_context_t * sys_context, const lw6mat_dmat4_t * dmat4);

/* mat-similar.c */
extern int lw6mat_is_similar_f (float f_a, float f_b) __attribute__ ((const));
extern int lw6mat_is_similar_i (int32_t i_a, int32_t i_b) __attribute__ ((const));
extern int lw6mat_is_similar_d (double d_a, double d_b) __attribute__ ((const));
extern int lw6mat_is_similar_x (int32_t x_a, int32_t x_b) __attribute__ ((const));

/* mat-test.c */
extern int lw6mat_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6mat_test_run (lw6sys_context_t * sys_context, int mode);

#endif
