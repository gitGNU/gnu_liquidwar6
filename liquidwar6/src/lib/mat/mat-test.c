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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <CUnit/CUnit.h>

#include "mat.h"

#define _TEST_SIZEOF_INT32 4
#define _TEST_SIZEOF_FLOAT 4
#define _TEST_SIZEOF_DOUBLE 8

#define _TEST_FTOI_IN 2.9f
#define _TEST_FTOI_OUT 3
#define _TEST_FTOD_IN 2.0f
#define _TEST_FTOD_OUT 2.0f
#define _TEST_FTOX_IN 256.0f
#define _TEST_FTOX_OUT 16777216
#define _TEST_ITOF_IN 2
#define _TEST_ITOF_OUT 2.0f
#define _TEST_ITOD_IN -4
#define _TEST_ITOD_OUT -4.0f
#define _TEST_ITOX_IN -1
#define _TEST_ITOX_OUT -65536
#define _TEST_DTOF_IN 16.125f
#define _TEST_DTOF_OUT 16.125f
#define _TEST_DTOI_IN -2.9f
#define _TEST_DTOI_OUT -3
#define _TEST_DTOX_IN -64.0f
#define _TEST_DTOX_OUT -4194304
#define _TEST_XTOF_IN 131072
#define _TEST_XTOF_OUT 2.0f
#define _TEST_XTOI_IN -131072
#define _TEST_XTOI_OUT -2
#define _TEST_XTOD_IN -32768
#define _TEST_XTOD_OUT -0.5f

#define _TEST_X_IN_N 8
#define _TEST_X_IN_1 2
#define _TEST_X_IN_2 1000
#define _TEST_X_IN_3 -4000
#define _TEST_X_IN_4 20000
#define _TEST_X_IN_5 -100000
#define _TEST_X_IN_6 7000000
#define _TEST_X_IN_7 -30000000
#define _TEST_X_IN_8 500000000
#define _TEST_X_LIMIT_IN_F 100.0f
#define _TEST_X_LIMIT_OUT_F 10000.0f
#define _TEST_X_EQUAL_MUL_32_THRESHOLD 500
#define _TEST_X_EQUAL_DIV_32_THRESHOLD 200000
#define _TEST_X_EQUAL_INV_32_THRESHOLD 200000
#define _TEST_X_EQUAL_MUL_64_THRESHOLD 10
#define _TEST_X_EQUAL_DIV_64_THRESHOLD 10
#define _TEST_X_EQUAL_INV_64_THRESHOLD 10

#define _TEST_FVEC_X1 5.0f
#define _TEST_FVEC_Y1 -10.0f
#define _TEST_FVEC_Z1 15.0f
#define _TEST_FVEC_W1 -20.0f
#define _TEST_FVEC_X2 1.0f
#define _TEST_FVEC_Y2 4.0f
#define _TEST_FVEC_Z2 9.0f
#define _TEST_FVEC_W2 16.0f

#define _TEST_FVEC2_LEN_SQ 8192000
#define _TEST_FVEC2_LEN 732715
#define _TEST_FVEC2_DOT -2293760
#define _TEST_FVEC2_CROSS_X 0
#define _TEST_FVEC2_CROSS_Y 0
#define _TEST_FVEC2_CROSS_Z 1966080

#define _TEST_FVEC3_LEN_SQ 22937600
#define _TEST_FVEC3_LEN 1226066
#define _TEST_FVEC3_DOT 6553600
#define _TEST_FVEC3_CROSS_X -9830400
#define _TEST_FVEC3_CROSS_Y -1966080
#define _TEST_FVEC3_CROSS_Z 1966080

#define _TEST_FVEC4_LEN_SQ 49152000
#define _TEST_FVEC4_LEN 1794777
#define _TEST_FVEC4_DOT -14417920
#define _TEST_FVEC4_CROSS_X -9830400
#define _TEST_FVEC4_CROSS_Y -1966080
#define _TEST_FVEC4_CROSS_Z 1966080

#define _TEST_DVEC_X1 5.0f
#define _TEST_DVEC_Y1 -10.0f
#define _TEST_DVEC_Z1 15.0f
#define _TEST_DVEC_W1 -20.0f
#define _TEST_DVEC_X2 1.0f
#define _TEST_DVEC_Y2 4.0f
#define _TEST_DVEC_Z2 9.0f
#define _TEST_DVEC_W2 16.0f

#define _TEST_DVEC2_LEN_SQ 8192000
#define _TEST_DVEC2_LEN 732715
#define _TEST_DVEC2_DOT -2293760
#define _TEST_DVEC2_CROSS_X 0
#define _TEST_DVEC2_CROSS_Y 0
#define _TEST_DVEC2_CROSS_Z 1966080

#define _TEST_DVEC3_LEN_SQ 22937600
#define _TEST_DVEC3_LEN 1226066
#define _TEST_DVEC3_DOT 6553600
#define _TEST_DVEC3_CROSS_X -9830400
#define _TEST_DVEC3_CROSS_Y -1966080
#define _TEST_DVEC3_CROSS_Z 1966080

#define _TEST_DVEC4_LEN_SQ 49152000
#define _TEST_DVEC4_LEN 1794777
#define _TEST_DVEC4_DOT -14417920
#define _TEST_DVEC4_CROSS_X -9830400
#define _TEST_DVEC4_CROSS_Y -1966080
#define _TEST_DVEC4_CROSS_Z 1966080

#define _TEST_FMAT_RANDOM_RANGE 1.5f
#define _TEST_FMAT_RANDOM_ROUND 0.01f
#define _TEST_FMAT_DET_LIMIT 100.0f
#define _TEST_FMAT_INVMUL_TOLERANCE 0.001f
#define _TEST_FMAT_SCALE 4.0f
#define _TEST_FMAT_SCALE_TOLERANCE 1.0f

#define _TEST_DMAT_RANDOM_RANGE 2.0f
#define _TEST_DMAT_RANDOM_ROUND 0.01f
#define _TEST_DMAT_DET_LIMIT 100.0f
#define _TEST_DMAT_INVMUL_TOLERANCE 0.0001f
#define _TEST_DMAT_SCALE 4.0f
#define _TEST_DMAT_SCALE_TOLERANCE 1.0f

#define _TEST_MAT_DET_0_NB_TRIES 100

typedef struct _lw6mat_test_data_s
{
  int ret;
} _lw6mat_test_data_t;

static _lw6mat_test_data_t _test_data = { 0 };

/*
 * Testing functions in header (fixed point arithmetic)
 */
static void
_test_sizeof ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int s = 0;

    s = sizeof (int32_t);
    if (s == _TEST_SIZEOF_INT32)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("sizeof(int32_t) is %d, OK"), s);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("sizeof(int32_t) is %d, what the hell is going on?"),
		    s);
	ret = 0;
      }
    s = sizeof (float);
    if (s == _TEST_SIZEOF_FLOAT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("sizeof(float) is %d, OK"), s);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("sizeof(float) is %d, what the hell is going on?"),
		    s);
	ret = 0;
      }
    s = sizeof (double);
    if (s == _TEST_SIZEOF_DOUBLE)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("sizeof(double) is %d, OK"), s);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("sizeof(double) is %d, what the hell is going on?"),
		    s);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in header (conversions)
 */
static void
_test_convert ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    float f = 0;
    int32_t i = 0;
    double d = 0;
    int32_t x = 0;

    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("Verifying conversions"));

    i = lw6mat_ftoi (_TEST_FTOI_IN);
    if (i == _TEST_FTOI_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("ftoi OK, output is %d"), i);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("ftoi failed, output is %d, expecting %d"), i,
		    _TEST_FTOI_OUT);
	ret = 0;
      }

    d = lw6mat_ftod (_TEST_FTOD_IN);
    if (d == _TEST_FTOD_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("ftod OK, output is %f"),
		    (float) d);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("ftod failed, output is %f, expecting %f"),
		    (float) d, _TEST_FTOD_OUT);
	ret = 0;
      }

    x = lw6mat_ftox (_TEST_FTOX_IN);
    if (x == _TEST_FTOX_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("ftox OK, output is %d"), x);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("ftox failed, output is %d, expecting %d"), x,
		    _TEST_FTOX_OUT);
	ret = 0;
      }

    f = lw6mat_itof (_TEST_ITOF_IN);
    if (f == _TEST_ITOF_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("itof OK, output is %f"), f);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("itof failed, output is %f, expecting %f"), f,
		    _TEST_ITOF_OUT);
	ret = 0;
      }

    d = lw6mat_itod (_TEST_ITOD_IN);
    if (d == _TEST_ITOD_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("itod OK, output is %f"),
		    (float) d);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("itod failed, output is %f, expecting %f"),
		    (float) d, _TEST_ITOD_OUT);
	ret = 0;
      }

    x = lw6mat_itox (_TEST_ITOX_IN);
    if (x == _TEST_ITOX_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("itox OK, output is %d"), x);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("itox failed, output is %d, expecting %d"), x,
		    _TEST_ITOX_OUT);
	ret = 0;
      }

    f = lw6mat_dtof (_TEST_DTOF_IN);
    if (f == _TEST_DTOF_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dtof OK, output is %f"), f);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dtof failed, output is %f, expecting %f"), f,
		    _TEST_DTOF_OUT);
	ret = 0;
      }

    i = lw6mat_dtoi (_TEST_DTOI_IN);
    if (i == _TEST_DTOI_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dtoi OK, output is %d"), i);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dtoi failed, output is %d, expecting %d"), i,
		    _TEST_DTOI_OUT);
	ret = 0;
      }

    x = lw6mat_dtox (_TEST_DTOX_IN);
    if (x == _TEST_DTOX_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dtox OK, output is %d"), x);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dtox failed, output is %d, expecting %d"), x,
		    _TEST_DTOX_OUT);
	ret = 0;
      }

    f = lw6mat_xtof (_TEST_XTOF_IN);
    if (f == _TEST_XTOF_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("xtof OK, output is %f"), f);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("xtof failed, output is %f, expecting %f"), f,
		    _TEST_XTOF_OUT);
	ret = 0;
      }

    i = lw6mat_xtoi (_TEST_XTOI_IN);
    if (i == _TEST_XTOI_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("xtoi OK, output is %d"), i);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("xtoi failed, output is %d, expecting %d"), i,
		    _TEST_XTOI_OUT);
	ret = 0;
      }

    d = lw6mat_xtod (_TEST_XTOD_IN);
    if (d == _TEST_XTOD_OUT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("xtod OK, output is %f"),
		    (float) d);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("xtod failed, output is %f, expecting %f"),
		    (float) d, _TEST_XTOD_OUT);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_f_is_within_limit (float f, float limit_f)
{
  return (((f > -limit_f) && (f < -1.0 / limit_f))
	  || ((f > 1.0f / limit_f) && (f < limit_f)));
}

/*
 * Testing functions in header (fixed point arithmetic)
 */
static void
_test_x ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i = 0;
    int j = 0;
    int32_t x = 0;
    float f = 0.0f;
    //int64_t a=0LL;
    //int64_t b=0LL;
    //int64_t c=0LL;
    int x_in[_TEST_X_IN_N] =
      { _TEST_X_IN_1, _TEST_X_IN_2, _TEST_X_IN_3, _TEST_X_IN_4, _TEST_X_IN_5,
      _TEST_X_IN_6, _TEST_X_IN_7, _TEST_X_IN_8
    };

    for (i = 0; i < _TEST_X_IN_N; ++i)
      {
	for (j = 0; j < _TEST_X_IN_N; ++j)
	  {
	    f = lw6mat_xtof (x_in[i]) * lw6mat_xtof (x_in[j]);
	    if (_f_is_within_limit (f, _TEST_X_LIMIT_OUT_F))
	      {
		if (_f_is_within_limit
		    (lw6mat_xtof (x_in[i]), _TEST_X_LIMIT_IN_F)
		    && _f_is_within_limit (lw6mat_xtof (x_in[j]),
					   _TEST_X_LIMIT_IN_F))
		  {
		    x = lw6mat_x_mul_32 (x_in[i], x_in[j]);
		    if (abs (x - lw6mat_ftox (f)) <=
			_TEST_X_EQUAL_MUL_32_THRESHOLD)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("mul32 %f * %f -> %f OK (fixed point) (%d ~= %d)"),
				    lw6mat_xtof (x_in[i]),
				    lw6mat_xtof (x_in[j]), lw6mat_xtof (x), x,
				    lw6mat_ftox (f));
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("mul32 %f * %f -> %f problem (fixed point) was expecting %f (%d != %d)"),
				    lw6mat_xtof (x_in[i]),
				    lw6mat_xtof (x_in[j]), lw6mat_xtof (x), f,
				    x, lw6mat_ftox (f));
			ret = 0;
		      }
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("mul32 not testing out-of-range %f * %f"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]));
		  }
		x = lw6mat_x_mul_64 (x_in[i], x_in[j]);
		if (abs (x - lw6mat_ftox (f)) <=
		    _TEST_X_EQUAL_MUL_64_THRESHOLD)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("mul64 %f * %f -> %f OK (fixed point) (%d ~= %d)"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]),
				lw6mat_xtof (x), x, lw6mat_ftox (f));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("mul64 %f * %f -> %f problem (fixed point) was expecting %f (%d != %d)"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]),
				lw6mat_xtof (x), f, x, lw6mat_ftox (f));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("mul32/64 not testing out-of-range %f * %f"),
			    lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]));
	      }
	    f = lw6mat_xtof (x_in[i]) / lw6mat_xtof (x_in[j]);
	    if (_f_is_within_limit (f, _TEST_X_LIMIT_OUT_F))
	      {
		if (_f_is_within_limit
		    (lw6mat_xtof (x_in[i]), _TEST_X_LIMIT_IN_F)
		    && _f_is_within_limit (lw6mat_xtof (x_in[j]),
					   _TEST_X_LIMIT_IN_F))
		  {
		    x = lw6mat_x_div_32 (x_in[i], x_in[j]);
		    if (abs (x - lw6mat_ftox (f)) <=
			_TEST_X_EQUAL_DIV_32_THRESHOLD)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("div32 %f / %f -> %f OK (fixed point) (%d ~= %d)"),
				    lw6mat_xtof (x_in[i]),
				    lw6mat_xtof (x_in[j]), lw6mat_xtof (x), x,
				    lw6mat_ftox (f));
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("div32 %f / %f -> %f problem (fixed point) was expecting %f (%d != %d)"),
				    lw6mat_xtof (x_in[i]),
				    lw6mat_xtof (x_in[j]), lw6mat_xtof (x), f,
				    x, lw6mat_ftox (f));
			ret = 0;
		      }
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("div32 not testing out-of-range %f / %f"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]));
		  }
		x = lw6mat_x_div_64 (x_in[i], x_in[j]);
		if (abs (x - lw6mat_ftox (f)) <=
		    _TEST_X_EQUAL_DIV_64_THRESHOLD)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("div64 %f / %f -> %f OK (fixed point) (%d ~= %d)"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]),
				lw6mat_xtof (x), x, lw6mat_ftox (f));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("div64 %f / %f -> %f problem (fixed point) was expecting %f (%d != %d)"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]),
				lw6mat_xtof (x), f, x, lw6mat_ftox (f));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("div32/64 not testing out-of-range %f / %f"),
			    lw6mat_xtof (x_in[i]), lw6mat_xtof (x_in[j]));
	      }
	  }
	f = LW6MAT_F_1 / lw6mat_xtof (x_in[i]);
	if (_f_is_within_limit (f, _TEST_X_LIMIT_OUT_F))
	  {
	    if (_f_is_within_limit
		(lw6mat_xtof (x_in[i]), _TEST_X_LIMIT_IN_F))
	      {
		x = lw6mat_x_inv_32 (x_in[i]);
		if (abs (x - lw6mat_ftox (f)) <=
		    _TEST_X_EQUAL_INV_32_THRESHOLD)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("inv32 1/%f -> %f OK (fixed point) (%d ~= %d)"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x), x,
				lw6mat_ftox (f));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("inv32  1/%f -> %f problem (fixed point) was expecting %f (%d != %d)"),
				lw6mat_xtof (x_in[i]), lw6mat_xtof (x), f, x,
				lw6mat_ftox (f));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("inv32 not testing out-of-range 1/%f"),
			    lw6mat_xtof (x_in[i]));
	      }
	    x = lw6mat_x_inv_64 (x_in[i]);
	    if (abs (x - lw6mat_ftox (f)) <= _TEST_X_EQUAL_INV_64_THRESHOLD)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("inv64 1/%f -> %f OK (fixed point) (%d ~= %d)"),
			    lw6mat_xtof (x_in[i]), lw6mat_xtof (x), x,
			    lw6mat_ftox (f));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("inv64 1/%f -> %f problem (fixed point) was expecting %f (%d != %d)"),
			    lw6mat_xtof (x_in[i]), lw6mat_xtof (x), f, x,
			    lw6mat_ftox (f));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("inv32/64 not testing out-of-range 1/%f"),
			lw6mat_xtof (x_in[i]));
	  }
      }
    //#define _TEST_X_32_64_LIMIT 2000000001
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in fvec2.c
 */
static void
_test_fvec2 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_fvec2_t fvec2;
    lw6mat_fvec2_t fvec2_a;
    lw6mat_fvec2_t fvec2_b;
    float len_sq = 0.0f;
    float len = 0.0f;
    float dot = 0.0f;
    lw6mat_fvec3_t fvec3;	// needed for cross-product

    lw6mat_fvec2_zero (&fvec2);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should leave a trace in the log file"));
    if (lw6mat_fvec2_norm (&fvec2))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("norm did not return an error on vector zero"));
	ret = 1;
      }
    if (!lw6mat_fvec2_len (&fvec2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("zero fvec2 has zero size, this is fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("zero fvec2 has non-zero size"));
	ret = 0;
      }
    fvec2.p.x = _TEST_FVEC_X1;
    fvec2.p.y = _TEST_FVEC_Y1;
    len_sq = lw6mat_fvec2_len_sq (&fvec2);
    if (lw6mat_ftox (len_sq) == _TEST_FVEC2_LEN_SQ)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len_sq for fvec2 is %d -> OK"),
		    lw6mat_ftoi (len_sq));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len_sq for fvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len_sq), lw6mat_ftox (len_sq),
		    _TEST_FVEC2_LEN_SQ);
	ret = 0;
      }
    len = lw6mat_fvec2_len (&fvec2);
    if (lw6mat_ftox (len) == _TEST_FVEC2_LEN)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len for fvec2 is %d -> OK"),
		    lw6mat_ftoi (len));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for fvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len), lw6mat_ftox (len), _TEST_FVEC2_LEN);
	ret = 0;
      }
    lw6mat_fvec2_norm (&fvec2);
    if (!lw6mat_fvec2_norm (&fvec2))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("norm returned an error"));
	ret = 1;
      }
    len = lw6mat_fvec2_len (&fvec2);
    if (lw6mat_ftox (len) == LW6MAT_X_1)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("len for normalized fvec2 is 1 -> OK"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for normalized fvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len), lw6mat_ftox (len), LW6MAT_X_1);
	ret = 0;
      }

    fvec2_a = fvec2;
    /*
     * Here we do a per-member affectation on purpose, just in case
     * the struct/union would be two big and is_same would fail
     * because of extra data at its end.
     */
    memset (&fvec2_b, 0xff, sizeof (lw6mat_fvec2_t));
    fvec2_b.p.x = fvec2.p.x;
    fvec2_b.p.y = fvec2.p.y;
    if (lw6mat_fvec2_is_same (&fvec2_a, &fvec2_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("fvec2 comparison works when equal"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("fvec2 comparison broken when equal"));
	ret = 0;
      }
    lw6mat_fvec2_neg (&fvec2_b);
    if (!lw6mat_fvec2_is_same (&fvec2_a, &fvec2_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("fvec2 comparison works when different"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("fvec2 comparison broken when different"));
	ret = 0;
      }
    lw6mat_fvec2_add (&fvec2, &fvec2_a, &fvec2_b);
    if (!lw6mat_fvec2_len (&fvec2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("adding a fvec2 vector ands its neg gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("adding a fvec2 vector ands its neg does not give zero"));
	ret = 0;
      }
    lw6mat_fvec2_sub (&fvec2, &fvec2_a, &fvec2_a);
    if (!lw6mat_fvec2_len (&fvec2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("substracting a fvec2 vector to itself gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("substracting a fvec2 vector to itself does not give zero"));
	ret = 0;
      }
    // re-initializing for easier check of expected values
    fvec2_a.p.x = _TEST_FVEC_X1;
    fvec2_a.p.y = _TEST_FVEC_Y1;
    fvec2_b.p.x = _TEST_FVEC_X2;
    fvec2_b.p.y = _TEST_FVEC_Y2;
    dot = lw6mat_fvec2_dot (&fvec2_a, &fvec2_b);
    if (lw6mat_ftox (dot) == _TEST_FVEC2_DOT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dot for fvec2 is %d -> OK"),
		    lw6mat_ftoi (dot));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("dot for fvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (dot), lw6mat_ftox (dot), _TEST_FVEC2_DOT);
	ret = 0;
      }

    /*
     * Cross product test is specific to 3 dimensions vectors,
     * does not really make sense for dim 2 and 4.
     */
    lw6mat_fvec2_cross (&fvec3, &fvec2_a, &fvec2_b);
    if (lw6mat_ftox (fvec3.p.x) == _TEST_FVEC2_CROSS_X &&
	lw6mat_ftox (fvec3.p.y) == _TEST_FVEC2_CROSS_Y &&
	lw6mat_ftox (fvec3.p.z) == _TEST_FVEC2_CROSS_Z)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("cross product for fvec2 is (%d,%d,%d) -> OK"),
		    lw6mat_ftoi (fvec3.p.x), lw6mat_ftoi (fvec3.p.y),
		    lw6mat_ftoi (fvec3.p.z));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("cross product for fvec2 is (%d,%d,%d) -> bad, fixed point value is (%d,%d,%d) and shoud be (%d,%d,%d)"),
		    lw6mat_ftoi (fvec3.p.x), lw6mat_ftoi (fvec3.p.y),
		    lw6mat_ftoi (fvec3.p.z), lw6mat_ftox (fvec3.p.x),
		    lw6mat_ftox (fvec3.p.y), lw6mat_ftox (fvec3.p.z),
		    _TEST_FVEC2_CROSS_X, _TEST_FVEC2_CROSS_Y,
		    _TEST_FVEC2_CROSS_Z);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in fvec3.c
 */
static void
_test_fvec3 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_fvec3_t fvec3;
    lw6mat_fvec3_t fvec3_a;
    lw6mat_fvec3_t fvec3_b;
    float len_sq = 0.0f;
    float len = 0.0f;
    float dot = 0.0f;

    lw6mat_fvec3_zero (&fvec3);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should leave a trace in the log file"));
    if (lw6mat_fvec3_norm (&fvec3))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("norm did not return an error on vector zero"));
	ret = 1;
      }
    if (!lw6mat_fvec3_len (&fvec3))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("zero fvec3 has zero size, this is fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("zero fvec3 has non-zero size"));
	ret = 0;
      }
    fvec3.p.x = _TEST_FVEC_X1;
    fvec3.p.y = _TEST_FVEC_Y1;
    fvec3.p.z = _TEST_FVEC_Z1;
    len_sq = lw6mat_fvec3_len_sq (&fvec3);
    if (lw6mat_ftox (len_sq) == _TEST_FVEC3_LEN_SQ)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len_sq for fvec3 is %d -> OK"),
		    lw6mat_ftoi (len_sq));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len_sq for fvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len_sq), lw6mat_ftox (len_sq),
		    _TEST_FVEC3_LEN_SQ);
	ret = 0;
      }
    len = lw6mat_fvec3_len (&fvec3);
    if (lw6mat_ftox (len) == _TEST_FVEC3_LEN)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len for fvec3 is %d -> OK"),
		    lw6mat_ftoi (len));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for fvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len), lw6mat_ftox (len), _TEST_FVEC3_LEN);
	ret = 0;
      }
    if (!lw6mat_fvec3_norm (&fvec3))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("norm returned an error"));
	ret = 1;
      }
    len = lw6mat_fvec3_len (&fvec3);
    if (lw6mat_ftox (len) == LW6MAT_X_1)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("len for normalized fvec3 is 1 -> OK"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for normalized fvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len), lw6mat_ftox (len), LW6MAT_X_1);
	ret = 0;
      }

    fvec3_a = fvec3;
    /*
     * Here we do a per-member affectation on purpose, just in case
     * the struct/union would be two big and is_same would fail
     * because of extra data at its end.
     */
    memset (&fvec3_b, 0xff, sizeof (lw6mat_fvec3_t));
    fvec3_b.p.x = fvec3.p.x;
    fvec3_b.p.y = fvec3.p.y;
    fvec3_b.p.z = fvec3.p.z;
    if (lw6mat_fvec3_is_same (&fvec3_a, &fvec3_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("fvec3 comparison works when equal"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("fvec3 comparison broken when equal"));
	ret = 0;
      }
    lw6mat_fvec3_neg (&fvec3_b);
    if (!lw6mat_fvec3_is_same (&fvec3_a, &fvec3_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("fvec3 comparison works when different"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("fvec3 comparison broken when different"));
	ret = 0;
      }
    lw6mat_fvec3_add (&fvec3, &fvec3_a, &fvec3_b);
    if (!lw6mat_fvec3_len (&fvec3))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("adding a fvec3 vector ands its neg gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("adding a fvec3 vector ands its neg does not give zero"));
	ret = 0;
      }
    lw6mat_fvec3_sub (&fvec3, &fvec3_a, &fvec3_a);
    if (!lw6mat_fvec3_len (&fvec3))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("substracting a fvec3 vector to itself gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("substracting a fvec3 vector to itself does not give zero"));
	ret = 0;
      }
    // re-initializing for easier check of expected values
    fvec3_a.p.x = _TEST_FVEC_X1;
    fvec3_a.p.y = _TEST_FVEC_Y1;
    fvec3_a.p.z = _TEST_FVEC_Z1;
    fvec3_b.p.x = _TEST_FVEC_X2;
    fvec3_b.p.y = _TEST_FVEC_Y2;
    fvec3_b.p.z = _TEST_FVEC_Z2;
    dot = lw6mat_fvec3_dot (&fvec3_a, &fvec3_b);
    if (lw6mat_ftox (dot) == _TEST_FVEC3_DOT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dot for fvec3 is %d -> OK"),
		    lw6mat_ftoi (dot));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("dot for fvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (dot), lw6mat_ftox (dot), _TEST_FVEC3_DOT);
	ret = 0;
      }

    /*
     * Cross product test is specific to 3 dimensions vectors,
     * does not really make sense for dim 2 and 4.
     */
    lw6mat_fvec3_cross (&fvec3, &fvec3_a, &fvec3_b);
    if (lw6mat_ftox (fvec3.p.x) == _TEST_FVEC3_CROSS_X &&
	lw6mat_ftox (fvec3.p.y) == _TEST_FVEC3_CROSS_Y &&
	lw6mat_ftox (fvec3.p.z) == _TEST_FVEC3_CROSS_Z)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("cross product for fvec3 is (%d,%d,%d) -> OK"),
		    lw6mat_ftoi (fvec3.p.x), lw6mat_ftoi (fvec3.p.y),
		    lw6mat_ftoi (fvec3.p.z));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("cross product for fvec3 is (%d,%d,%d) -> bad, fixed point value is (%d,%d,%d) and shoud be (%d,%d,%d)"),
		    lw6mat_ftoi (fvec3.p.x), lw6mat_ftoi (fvec3.p.y),
		    lw6mat_ftoi (fvec3.p.z), lw6mat_ftox (fvec3.p.x),
		    lw6mat_ftox (fvec3.p.y), lw6mat_ftox (fvec3.p.z),
		    _TEST_FVEC3_CROSS_X, _TEST_FVEC3_CROSS_Y,
		    _TEST_FVEC3_CROSS_Z);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in fvec4.c
 */
static void
_test_fvec4 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_fvec4_t fvec4;
    lw6mat_fvec4_t fvec4_a;
    lw6mat_fvec4_t fvec4_b;
    float len_sq = 0.0f;
    float len = 0.0f;
    float dot = 0.0f;
    lw6mat_fvec3_t fvec3;	// needed for cross-product

    lw6mat_fvec4_zero (&fvec4);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should leave a trace in the log file"));
    if (lw6mat_fvec4_norm (&fvec4))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("norm did not return an error on vector zero"));
	ret = 1;
      }
    if (!lw6mat_fvec4_len (&fvec4))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("zero fvec4 has zero size, this is fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("zero fvec4 has non-zero size"));
	ret = 0;
      }
    fvec4.p.x = _TEST_FVEC_X1;
    fvec4.p.y = _TEST_FVEC_Y1;
    fvec4.p.z = _TEST_FVEC_Z1;
    fvec4.p.w = _TEST_FVEC_W1;
    len_sq = lw6mat_fvec4_len_sq (&fvec4);
    if (lw6mat_ftox (len_sq) == _TEST_FVEC4_LEN_SQ)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len_sq for fvec4 is %d -> OK"),
		    lw6mat_ftoi (len_sq));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len_sq for fvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len_sq), lw6mat_ftox (len_sq),
		    _TEST_FVEC4_LEN_SQ);
	ret = 0;
      }
    len = lw6mat_fvec4_len (&fvec4);
    if (lw6mat_ftox (len) == _TEST_FVEC4_LEN)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len for fvec4 is %d -> OK"),
		    lw6mat_ftoi (len));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for fvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len), lw6mat_ftox (len), _TEST_FVEC4_LEN);
	ret = 0;
      }
    if (!lw6mat_fvec4_norm (&fvec4))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("norm returned an error"));
	ret = 1;
      }
    len = lw6mat_fvec4_len (&fvec4);
    if (lw6mat_ftox (len) == LW6MAT_X_1)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("len for normalized fvec4 is 1 -> OK"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for normalized fvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (len), lw6mat_ftox (len), LW6MAT_X_1);
	ret = 0;
      }

    fvec4_a = fvec4;
    /*
     * Here we do a per-member affectation on purpose, just in case
     * the struct/union would be two big and is_same would fail
     * because of extra data at its end.
     */
    memset (&fvec4_b, 0xff, sizeof (lw6mat_fvec4_t));
    fvec4_b.p.x = fvec4.p.x;
    fvec4_b.p.y = fvec4.p.y;
    fvec4_b.p.z = fvec4.p.z;
    fvec4_b.p.w = fvec4.p.w;
    if (lw6mat_fvec4_is_same (&fvec4_a, &fvec4_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("fvec4 comparison works when equal"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("fvec4 comparison broken when equal"));
	ret = 0;
      }
    lw6mat_fvec4_neg (&fvec4_b);
    if (!lw6mat_fvec4_is_same (&fvec4_a, &fvec4_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("fvec4 comparison works when different"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("fvec4 comparison broken when different"));
	ret = 0;
      }
    lw6mat_fvec4_add (&fvec4, &fvec4_a, &fvec4_b);
    if (!lw6mat_fvec4_len (&fvec4))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("adding a fvec4 vector ands its neg gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("adding a fvec4 vector ands its neg does not give zero"));
	ret = 0;
      }
    lw6mat_fvec4_sub (&fvec4, &fvec4_a, &fvec4_a);
    if (!lw6mat_fvec4_len (&fvec4))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("substracting a fvec4 vector to itself gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("substracting a fvec4 vector to itself does not give zero"));
	ret = 0;
      }
    // re-initializing for easier check of expected values
    fvec4_a.p.x = _TEST_FVEC_X1;
    fvec4_a.p.y = _TEST_FVEC_Y1;
    fvec4_a.p.z = _TEST_FVEC_Z1;
    fvec4_a.p.w = _TEST_FVEC_W1;
    fvec4_b.p.x = _TEST_FVEC_X2;
    fvec4_b.p.y = _TEST_FVEC_Y2;
    fvec4_b.p.z = _TEST_FVEC_Z2;
    fvec4_b.p.w = _TEST_FVEC_W2;
    dot = lw6mat_fvec4_dot (&fvec4_a, &fvec4_b);
    if (lw6mat_ftox (dot) == _TEST_FVEC4_DOT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dot for fvec4 is %d -> OK"),
		    lw6mat_ftoi (dot));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("dot for fvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_ftoi (dot), lw6mat_ftox (dot), _TEST_FVEC4_DOT);
	ret = 0;
      }

    /*
     * Cross product test is specific to 3 dimensions vectors,
     * does not really make sense for dim 2 and 4.
     */
    lw6mat_fvec4_cross (&fvec3, &fvec4_a, &fvec4_b);
    if (lw6mat_ftox (fvec3.p.x) == _TEST_FVEC4_CROSS_X &&
	lw6mat_ftox (fvec3.p.y) == _TEST_FVEC4_CROSS_Y &&
	lw6mat_ftox (fvec3.p.z) == _TEST_FVEC4_CROSS_Z)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("cross product for fvec4 is (%d,%d,%d) -> OK"),
		    lw6mat_ftoi (fvec3.p.x), lw6mat_ftoi (fvec3.p.y),
		    lw6mat_ftoi (fvec3.p.z));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("cross product for fvec4 is (%d,%d,%d) -> bad, fixed point value is (%d,%d,%d) and shoud be (%d,%d,%d)"),
		    lw6mat_ftoi (fvec3.p.x), lw6mat_ftoi (fvec3.p.y),
		    lw6mat_ftoi (fvec3.p.z), lw6mat_ftox (fvec3.p.x),
		    lw6mat_ftox (fvec3.p.y), lw6mat_ftox (fvec3.p.z),
		    _TEST_FVEC4_CROSS_X, _TEST_FVEC4_CROSS_Y,
		    _TEST_FVEC4_CROSS_Z);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in dvec2.c
 */
static void
_test_dvec2 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_dvec2_t dvec2;
    lw6mat_dvec2_t dvec2_a;
    lw6mat_dvec2_t dvec2_b;
    double len_sq = 0.0f;
    double len = 0.0f;
    double dot = 0.0f;
    lw6mat_dvec3_t dvec3;	// needed for cross-product

    lw6mat_dvec2_zero (&dvec2);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should leave a trace in the log file"));
    if (lw6mat_dvec2_norm (&dvec2))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("norm did not return an error on vector zero"));
	ret = 1;
      }
    if (!lw6mat_dvec2_len (&dvec2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("zero dvec2 has zero size, this is fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("zero dvec2 has non-zero size"));
	ret = 0;
      }
    dvec2.p.x = _TEST_DVEC_X1;
    dvec2.p.y = _TEST_DVEC_Y1;
    len_sq = lw6mat_dvec2_len_sq (&dvec2);
    if (lw6mat_dtox (len_sq) == _TEST_DVEC2_LEN_SQ)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len_sq for dvec2 is %d -> OK"),
		    lw6mat_dtoi (len_sq));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len_sq for dvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len_sq), lw6mat_dtox (len_sq),
		    _TEST_DVEC2_LEN_SQ);
	ret = 0;
      }
    len = lw6mat_dvec2_len (&dvec2);
    if (lw6mat_dtox (len) == _TEST_DVEC2_LEN)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len for dvec2 is %d -> OK"),
		    lw6mat_dtoi (len));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for dvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len), lw6mat_dtox (len), _TEST_DVEC2_LEN);
	ret = 0;
      }
    lw6mat_dvec2_norm (&dvec2);
    if (!lw6mat_dvec2_norm (&dvec2))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("norm returned an error"));
	ret = 1;
      }
    len = lw6mat_dvec2_len (&dvec2);
    if (lw6mat_dtox (len) == LW6MAT_X_1)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("len for normalized dvec2 is 1 -> OK"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for normalized dvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len), lw6mat_dtox (len), LW6MAT_X_1);
	ret = 0;
      }

    dvec2_a = dvec2;
    /*
     * Here we do a per-member affectation on purpose, just in case
     * the struct/union would be two big and is_same would fail
     * because of extra data at its end.
     */
    memset (&dvec2_b, 0xff, sizeof (lw6mat_dvec2_t));
    dvec2_b.p.x = dvec2.p.x;
    dvec2_b.p.y = dvec2.p.y;
    if (lw6mat_dvec2_is_same (&dvec2_a, &dvec2_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dvec2 comparison works when equal"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("dvec2 comparison broken when equal"));
	ret = 0;
      }
    lw6mat_dvec2_neg (&dvec2_b);
    if (!lw6mat_dvec2_is_same (&dvec2_a, &dvec2_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dvec2 comparison works when different"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("dvec2 comparison broken when different"));
	ret = 0;
      }
    lw6mat_dvec2_add (&dvec2, &dvec2_a, &dvec2_b);
    if (!lw6mat_dvec2_len (&dvec2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("adding a dvec2 vector ands its neg gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("adding a dvec2 vector ands its neg does not give zero"));
	ret = 0;
      }
    lw6mat_dvec2_sub (&dvec2, &dvec2_a, &dvec2_a);
    if (!lw6mat_dvec2_len (&dvec2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("substracting a dvec2 vector to itself gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("substracting a dvec2 vector to itself does not give zero"));
	ret = 0;
      }
    // re-initializing for easier check of expected values
    dvec2_a.p.x = _TEST_DVEC_X1;
    dvec2_a.p.y = _TEST_DVEC_Y1;
    dvec2_b.p.x = _TEST_DVEC_X2;
    dvec2_b.p.y = _TEST_DVEC_Y2;
    dot = lw6mat_dvec2_dot (&dvec2_a, &dvec2_b);
    if (lw6mat_dtox (dot) == _TEST_DVEC2_DOT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dot for dvec2 is %d -> OK"),
		    lw6mat_dtoi (dot));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("dot for dvec2 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (dot), lw6mat_dtox (dot), _TEST_DVEC2_DOT);
	ret = 0;
      }

    /*
     * Cross product test is specific to 3 dimensions vectors,
     * does not really make sense for dim 2 and 4.
     */
    lw6mat_dvec2_cross (&dvec3, &dvec2_a, &dvec2_b);
    if (lw6mat_dtox (dvec3.p.x) == _TEST_DVEC2_CROSS_X &&
	lw6mat_dtox (dvec3.p.y) == _TEST_DVEC2_CROSS_Y &&
	lw6mat_dtox (dvec3.p.z) == _TEST_DVEC2_CROSS_Z)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("cross product for dvec2 is (%d,%d,%d) -> OK"),
		    lw6mat_dtoi (dvec3.p.x), lw6mat_dtoi (dvec3.p.y),
		    lw6mat_dtoi (dvec3.p.z));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("cross product for dvec2 is (%d,%d,%d) -> bad, fixed point value is (%d,%d,%d) and shoud be (%d,%d,%d)"),
		    lw6mat_dtoi (dvec3.p.x), lw6mat_dtoi (dvec3.p.y),
		    lw6mat_dtoi (dvec3.p.z), lw6mat_dtox (dvec3.p.x),
		    lw6mat_dtox (dvec3.p.y), lw6mat_dtox (dvec3.p.z),
		    _TEST_FVEC2_CROSS_X, _TEST_FVEC2_CROSS_Y,
		    _TEST_FVEC2_CROSS_Z);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in dvec3.c
 */
static void
_test_dvec3 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_dvec3_t dvec3;
    lw6mat_dvec3_t dvec3_a;
    lw6mat_dvec3_t dvec3_b;
    double len_sq = 0.0f;
    double len = 0.0f;
    double dot = 0.0f;

    lw6mat_dvec3_zero (&dvec3);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should leave a trace in the log file"));
    if (lw6mat_dvec3_norm (&dvec3))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("norm did not return an error on vector zero"));
	ret = 1;
      }
    if (!lw6mat_dvec3_len (&dvec3))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("zero dvec3 has zero size, this is fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("zero dvec3 has non-zero size"));
	ret = 0;
      }
    dvec3.p.x = _TEST_DVEC_X1;
    dvec3.p.y = _TEST_DVEC_Y1;
    dvec3.p.z = _TEST_DVEC_Z1;
    len_sq = lw6mat_dvec3_len_sq (&dvec3);
    if (lw6mat_dtox (len_sq) == _TEST_DVEC3_LEN_SQ)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len_sq for dvec3 is %d -> OK"),
		    lw6mat_dtoi (len_sq));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len_sq for dvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len_sq), lw6mat_dtox (len_sq),
		    _TEST_DVEC3_LEN_SQ);
	ret = 0;
      }
    len = lw6mat_dvec3_len (&dvec3);
    if (lw6mat_dtox (len) == _TEST_DVEC3_LEN)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len for dvec3 is %d -> OK"),
		    lw6mat_dtoi (len));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for dvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len), lw6mat_dtox (len), _TEST_DVEC3_LEN);
	ret = 0;
      }
    if (!lw6mat_dvec3_norm (&dvec3))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("norm returned an error"));
	ret = 1;
      }
    len = lw6mat_dvec3_len (&dvec3);
    if (lw6mat_dtox (len) == LW6MAT_X_1)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("len for normalized dvec3 is 1 -> OK"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for normalized dvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len), lw6mat_dtox (len), LW6MAT_X_1);
	ret = 0;
      }

    dvec3_a = dvec3;
    /*
     * Here we do a per-member affectation on purpose, just in case
     * the struct/union would be two big and is_same would fail
     * because of extra data at its end.
     */
    memset (&dvec3_b, 0xff, sizeof (lw6mat_dvec3_t));
    dvec3_b.p.x = dvec3.p.x;
    dvec3_b.p.y = dvec3.p.y;
    dvec3_b.p.z = dvec3.p.z;
    if (lw6mat_dvec3_is_same (&dvec3_a, &dvec3_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dvec3 comparison works when equal"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("dvec3 comparison broken when equal"));
	ret = 0;
      }
    lw6mat_dvec3_neg (&dvec3_b);
    if (!lw6mat_dvec3_is_same (&dvec3_a, &dvec3_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dvec3 comparison works when different"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("dvec3 comparison broken when different"));
	ret = 0;
      }
    lw6mat_dvec3_add (&dvec3, &dvec3_a, &dvec3_b);
    if (!lw6mat_dvec3_len (&dvec3))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("adding a dvec3 vector ands its neg gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("adding a dvec3 vector ands its neg does not give zero"));
	ret = 0;
      }
    lw6mat_dvec3_sub (&dvec3, &dvec3_a, &dvec3_a);
    if (!lw6mat_dvec3_len (&dvec3))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("substracting a dvec3 vector to itself gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("substracting a dvec3 vector to itself does not give zero"));
	ret = 0;
      }
    // re-initializing for easier check of expected values
    dvec3_a.p.x = _TEST_DVEC_X1;
    dvec3_a.p.y = _TEST_DVEC_Y1;
    dvec3_a.p.z = _TEST_DVEC_Z1;
    dvec3_b.p.x = _TEST_DVEC_X2;
    dvec3_b.p.y = _TEST_DVEC_Y2;
    dvec3_b.p.z = _TEST_DVEC_Z2;
    dot = lw6mat_dvec3_dot (&dvec3_a, &dvec3_b);
    if (lw6mat_dtox (dot) == _TEST_DVEC3_DOT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dot for dvec3 is %d -> OK"),
		    lw6mat_dtoi (dot));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("dot for dvec3 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (dot), lw6mat_dtox (dot), _TEST_DVEC3_DOT);
	ret = 0;
      }

    /*
     * Cross product test is specific to 3 dimensions vectors,
     * does not really make sense for dim 2 and 4.
     */
    lw6mat_dvec3_cross (&dvec3, &dvec3_a, &dvec3_b);
    if (lw6mat_dtox (dvec3.p.x) == _TEST_DVEC3_CROSS_X &&
	lw6mat_dtox (dvec3.p.y) == _TEST_DVEC3_CROSS_Y &&
	lw6mat_dtox (dvec3.p.z) == _TEST_DVEC3_CROSS_Z)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("cross product for dvec3 is (%d,%d,%d) -> OK"),
		    lw6mat_dtoi (dvec3.p.x), lw6mat_dtoi (dvec3.p.y),
		    lw6mat_dtoi (dvec3.p.z));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("cross product for dvec3 is (%d,%d,%d) -> bad, fixed point value is (%d,%d,%d) and shoud be (%d,%d,%d)"),
		    lw6mat_dtoi (dvec3.p.x), lw6mat_dtoi (dvec3.p.y),
		    lw6mat_dtoi (dvec3.p.z), lw6mat_dtox (dvec3.p.x),
		    lw6mat_dtox (dvec3.p.y), lw6mat_dtox (dvec3.p.z),
		    _TEST_DVEC3_CROSS_X, _TEST_DVEC3_CROSS_Y,
		    _TEST_DVEC3_CROSS_Z);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in dvec4.c
 */
static void
_test_dvec4 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_dvec4_t dvec4;
    lw6mat_dvec4_t dvec4_a;
    lw6mat_dvec4_t dvec4_b;
    double len_sq = 0.0f;
    double len = 0.0f;
    double dot = 0.0f;
    lw6mat_dvec3_t dvec3;	// needed for cross-product

    lw6mat_dvec4_zero (&dvec4);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should leave a trace in the log file"));
    if (lw6mat_dvec4_norm (&dvec4))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("norm did not return an error on vector zero"));
	ret = 1;
      }
    if (!lw6mat_dvec4_len (&dvec4))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("zero dvec4 has zero size, this is fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("zero dvec4 has non-zero size"));
	ret = 0;
      }
    dvec4.p.x = _TEST_DVEC_X1;
    dvec4.p.y = _TEST_DVEC_Y1;
    dvec4.p.z = _TEST_DVEC_Z1;
    dvec4.p.w = _TEST_DVEC_W1;
    len_sq = lw6mat_dvec4_len_sq (&dvec4);
    if (lw6mat_dtox (len_sq) == _TEST_DVEC4_LEN_SQ)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len_sq for dvec4 is %d -> OK"),
		    lw6mat_dtoi (len_sq));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len_sq for dvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len_sq), lw6mat_dtox (len_sq),
		    _TEST_DVEC4_LEN_SQ);
	ret = 0;
      }
    len = lw6mat_dvec4_len (&dvec4);
    if (lw6mat_dtox (len) == _TEST_DVEC4_LEN)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("len for dvec4 is %d -> OK"),
		    lw6mat_dtoi (len));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for dvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len), lw6mat_dtox (len), _TEST_DVEC4_LEN);
	ret = 0;
      }
    if (!lw6mat_dvec4_norm (&dvec4))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("norm returned an error"));
	ret = 1;
      }
    len = lw6mat_dvec4_len (&dvec4);
    if (lw6mat_dtox (len) == LW6MAT_X_1)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("len for normalized dvec4 is 1 -> OK"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("len for normalized dvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (len), lw6mat_dtox (len), LW6MAT_X_1);
	ret = 0;
      }

    dvec4_a = dvec4;
    /*
     * Here we do a per-member affectation on purpose, just in case
     * the struct/union would be two big and is_same would fail
     * because of extra data at its end.
     */
    memset (&dvec4_b, 0xff, sizeof (lw6mat_dvec4_t));
    dvec4_b.p.x = dvec4.p.x;
    dvec4_b.p.y = dvec4.p.y;
    dvec4_b.p.z = dvec4.p.z;
    dvec4_b.p.w = dvec4.p.w;
    if (lw6mat_dvec4_is_same (&dvec4_a, &dvec4_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dvec4 comparison works when equal"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("dvec4 comparison broken when equal"));
	ret = 0;
      }
    lw6mat_dvec4_neg (&dvec4_b);
    if (!lw6mat_dvec4_is_same (&dvec4_a, &dvec4_b))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("dvec4 comparison works when different"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_ ("dvec4 comparison broken when different"));
	ret = 0;
      }
    lw6mat_dvec4_add (&dvec4, &dvec4_a, &dvec4_b);
    if (!lw6mat_dvec4_len (&dvec4))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("adding a dvec4 vector ands its neg gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("adding a dvec4 vector ands its neg does not give zero"));
	ret = 0;
      }
    lw6mat_dvec4_sub (&dvec4, &dvec4_a, &dvec4_a);
    if (!lw6mat_dvec4_len (&dvec4))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("substracting a dvec4 vector to itself gives zero, fine"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("substracting a dvec4 vector to itself does not give zero"));
	ret = 0;
      }
    // re-initializing for easier check of expected values
    dvec4_a.p.x = _TEST_DVEC_X1;
    dvec4_a.p.y = _TEST_DVEC_Y1;
    dvec4_a.p.z = _TEST_DVEC_Z1;
    dvec4_a.p.w = _TEST_DVEC_W1;
    dvec4_b.p.x = _TEST_DVEC_X2;
    dvec4_b.p.y = _TEST_DVEC_Y2;
    dvec4_b.p.z = _TEST_DVEC_Z2;
    dvec4_b.p.w = _TEST_DVEC_W2;
    dot = lw6mat_dvec4_dot (&dvec4_a, &dvec4_b);
    if (lw6mat_dtox (dot) == _TEST_DVEC4_DOT)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dot for dvec4 is %d -> OK"),
		    lw6mat_dtoi (dot));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("dot for dvec4 is %d -> bad, fixed point value is %d and should be %d"),
		    lw6mat_dtoi (dot), lw6mat_dtox (dot), _TEST_DVEC4_DOT);
	ret = 0;
      }

    /*
     * Cross product test is specific to 3 dimensions vectors,
     * does not really make sense for dim 2 and 4.
     */
    lw6mat_dvec4_cross (&dvec3, &dvec4_a, &dvec4_b);
    if (lw6mat_dtox (dvec3.p.x) == _TEST_DVEC4_CROSS_X &&
	lw6mat_dtox (dvec3.p.y) == _TEST_DVEC4_CROSS_Y &&
	lw6mat_dtox (dvec3.p.z) == _TEST_DVEC4_CROSS_Z)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("cross product for dvec4 is (%d,%d,%d) -> OK"),
		    lw6mat_dtoi (dvec3.p.x), lw6mat_dtoi (dvec3.p.y),
		    lw6mat_dtoi (dvec3.p.z));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("cross product for dvec4 is (%d,%d,%d) -> bad, fixed point value is (%d,%d,%d) and shoud be (%d,%d,%d)"),
		    lw6mat_dtoi (dvec3.p.x), lw6mat_dtoi (dvec3.p.y),
		    lw6mat_dtoi (dvec3.p.z), lw6mat_dtox (dvec3.p.x),
		    lw6mat_dtox (dvec3.p.y), lw6mat_dtox (dvec3.p.z),
		    _TEST_DVEC4_CROSS_X, _TEST_DVEC4_CROSS_Y,
		    _TEST_DVEC4_CROSS_Z);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

// utility to print a fmat2
static int
_print_fmat2 (const lw6mat_fmat2_t * fmat2, const char *about)
{
  int ret = 1;
  char *repr;

  repr = lw6mat_fmat2_repr (fmat2);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("fmat2 %s repr=\"%s\""), about,
		  repr);
      LW6SYS_FREE (repr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to generate fmat2 %s repr"), about);
      ret = 0;
    }

  return ret;
}

/*
 * Testing functions in fmat2.c
 */
static void
_test_fmat2 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_fmat2_t fmat2;
    lw6mat_fmat2_t fmat2_inv;
    lw6mat_fmat2_t fmat2_mul;
    lw6mat_fmat2_t fmat2_id;
    lw6mat_fmat2_t fmat2_trans;
    int i = 0;
    int j = 0;
    int k = 0;
    float det = LW6MAT_F_0;
    float det_scaled = LW6MAT_F_0;
    float det_scaled_expected = LW6MAT_F_0;
    int n = _TEST_MAT_DET_0_NB_TRIES;
    int transposed = 0;

    while ((!_f_is_within_limit (det, _TEST_FMAT_DET_LIMIT)) && n > 0)
      {
	lw6mat_fmat2_zero (&fmat2);
	for (i = 0; i < LW6MAT_MAT2_M_SIZE_2; ++i)
	  {
	    for (j = 0; j < LW6MAT_MAT2_M_SIZE_2; ++j)
	      {
		fmat2.m[i][j] =
		  round (lw6sys_random_float
			 (-_TEST_FMAT_RANDOM_RANGE,
			  _TEST_FMAT_RANDOM_RANGE) /
			 _TEST_FMAT_RANDOM_ROUND) * _TEST_FMAT_RANDOM_ROUND;
		k = lw6mat_mat2_v_index (i, j);
		if (fmat2.m[i][j] == fmat2.v[k])
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("filled matrix fmat2 [column=%d][row=%d] (element %d) with %f"),
				i, j, k, fmat2.m[i][j]);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("problem filling matrix fmat2 [column=%d][row=%d]=%f but element %d is %f"),
				i, j, fmat2.m[i][j], k, fmat2.v[k]);
		    ret = 0;
		  }
	      }
	  }
	ret = _print_fmat2 (&fmat2, "init") && ret;
	det = lw6mat_fmat2_det (&fmat2);
	if (det)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("fmat2 det=%f"), det);
	  }
	n--;
      }

    if (_f_is_within_limit (det, _TEST_FMAT_DET_LIMIT))
      {
	memset (&fmat2_inv, 0xff, sizeof (lw6mat_fmat2_t));
	for (k = 0; k < LW6MAT_MAT2_V_SIZE_4; ++k)
	  {
	    fmat2_inv.v[k] = fmat2.v[k];
	  }

	if (lw6mat_fmat2_is_same (&fmat2, &fmat2_inv))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("fmat2 comparison works when equal"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("fmat2 comparison broken when equal"));
	    ret = 0;
	  }

	fmat2_inv = fmat2;
	if (lw6mat_fmat2_inv (&fmat2_inv))
	  {
	    ret = _print_fmat2 (&fmat2_inv, "inv") && ret;

	    if (!lw6mat_fmat2_is_same (&fmat2, &fmat2_inv))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("fmat2 comparison works when different"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("fmat2 comparison broken when different"));
		ret = 0;
	      }

	    lw6mat_fmat2_mul_fmat2 (&fmat2_mul, &fmat2_inv, &fmat2);
	    ret = _print_fmat2 (&fmat2_mul, "mul") && ret;
	    lw6mat_fmat2_id (&fmat2_id);
	    for (k = 0; k < LW6MAT_MAT2_V_SIZE_4; ++k)
	      {
		if (fabs (fmat2_mul.v[k] - fmat2_id.v[k]) <
		    _TEST_FMAT_INVMUL_TOLERANCE)
		  {
		    /*
		     * OK, similar enough, we don't require exactness here, there
		     * always rounding errors, and we're working on random tests sets
		     */
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("fmat2_mul failed, element %d is %f and should be %f"),
				k, fmat2_mul.v[k], fmat2_id.v[k]);
		    ret = 0;
		  }
	      }
	    lw6mat_fmat2_scale (&fmat2, _TEST_FMAT_SCALE);
	    ret = _print_fmat2 (&fmat2, "scale") && ret;
	    det_scaled = lw6mat_fmat2_det (&fmat2);
	    det_scaled_expected = det * _TEST_FMAT_SCALE * _TEST_FMAT_SCALE;
	    if (fabs (det_scaled - det_scaled_expected) <
		_TEST_FMAT_SCALE_TOLERANCE)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f"),
			    _TEST_FMAT_SCALE, det_scaled, det);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f, this is not consistent, was expecting %f"),
			    _TEST_FMAT_SCALE, det_scaled, det,
			    det_scaled_expected);
		ret = 0;
	      }
	    fmat2_trans = fmat2;
	    lw6mat_fmat2_trans (&fmat2_trans);
	    ret = _print_fmat2 (&fmat2_trans, "trans 1X") && ret;
	    transposed = 1;
	    for (i = 0; i < LW6MAT_MAT2_M_SIZE_2; ++i)
	      {
		for (j = 0; j < LW6MAT_MAT2_M_SIZE_2; ++j)
		  {
		    if (fmat2.m[i][j] == fmat2_trans.m[j][i])
		      {
			// OK
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("transposition mismatch for fmat2 orig:%d,%d=%f trans:%d,%d=%f"),
				    i, j, fmat2.m[i][j], j, i,
				    fmat2_trans.m[j][i]);
			transposed = 0;
			ret = 0;
		      }
		  }
	      }
	    if (transposed)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("orig and trans fmat2 correspond, OK"));
		lw6mat_fmat2_trans (&fmat2_trans);
		ret = _print_fmat2 (&fmat2_trans, "trans 2X") && ret;
		if (lw6mat_fmat2_is_same (&fmat2, &fmat2_trans))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("orig and double-trans fmat2 look the same, OK"));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("orig and double-trans fmat2 look different"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("transposed fmat2 is broken"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to inverse fmat2"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("unable to produce an fmat2 matrix with a non-zero det"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

// utility to print a fmat3
static int
_print_fmat3 (const lw6mat_fmat3_t * fmat3, const char *about)
{
  int ret = 1;
  char *repr;

  repr = lw6mat_fmat3_repr (fmat3);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("fmat3 %s repr=\"%s\""), about,
		  repr);
      LW6SYS_FREE (repr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to generate fmat3 %s repr"), about);
      ret = 0;
    }

  return ret;
}

/*
 * Testing functions in fmat3.c
 */
static void
_test_fmat3 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_fmat3_t fmat3;
    lw6mat_fmat3_t fmat3_inv;
    lw6mat_fmat3_t fmat3_mul;
    lw6mat_fmat3_t fmat3_id;
    lw6mat_fmat3_t fmat3_trans;
    int i = 0;
    int j = 0;
    int k = 0;
    float det = LW6MAT_F_0;
    float det_scaled = LW6MAT_F_0;
    float det_scaled_expected = LW6MAT_F_0;
    int n = _TEST_MAT_DET_0_NB_TRIES;
    int transposed = 0;

    while ((!_f_is_within_limit (det, _TEST_FMAT_DET_LIMIT)) && n > 0)
      {
	lw6mat_fmat3_zero (&fmat3);
	for (i = 0; i < LW6MAT_MAT3_M_SIZE_3; ++i)
	  {
	    for (j = 0; j < LW6MAT_MAT3_M_SIZE_3; ++j)
	      {
		fmat3.m[i][j] =
		  round (lw6sys_random_float
			 (-_TEST_FMAT_RANDOM_RANGE,
			  _TEST_FMAT_RANDOM_RANGE) /
			 _TEST_FMAT_RANDOM_ROUND) * _TEST_FMAT_RANDOM_ROUND;
		k = lw6mat_mat3_v_index (i, j);
		if (fmat3.m[i][j] == fmat3.v[k])
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("filled matrix fmat3 [column=%d][row=%d] (element %d) with %f"),
				i, j, k, fmat3.m[i][j]);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("problem filling matrix fmat3 [column=%d][row=%d]=%f but element %d is %f"),
				i, j, fmat3.m[i][j], k, fmat3.v[k]);
		    ret = 0;
		  }
	      }
	  }
	ret = _print_fmat3 (&fmat3, "init") && ret;
	det = lw6mat_fmat3_det (&fmat3);
	if (det)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("fmat3 det=%f"), det);
	  }
	n--;
      }

    if (_f_is_within_limit (det, _TEST_FMAT_DET_LIMIT))
      {
	memset (&fmat3_inv, 0xff, sizeof (lw6mat_fmat3_t));
	for (k = 0; k < LW6MAT_MAT3_V_SIZE_9; ++k)
	  {
	    fmat3_inv.v[k] = fmat3.v[k];
	  }

	if (lw6mat_fmat3_is_same (&fmat3, &fmat3_inv))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("fmat3 comparison works when equal"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("fmat3 comparison broken when equal"));
	    ret = 0;
	  }

	fmat3_inv = fmat3;
	if (lw6mat_fmat3_inv (&fmat3_inv))
	  {
	    ret = _print_fmat3 (&fmat3_inv, "inv") && ret;

	    if (!lw6mat_fmat3_is_same (&fmat3, &fmat3_inv))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("fmat3 comparison works when different"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("fmat3 comparison broken when different"));
		ret = 0;
	      }

	    lw6mat_fmat3_mul_fmat3 (&fmat3_mul, &fmat3_inv, &fmat3);
	    ret = _print_fmat3 (&fmat3_mul, "mul") && ret;
	    lw6mat_fmat3_id (&fmat3_id);
	    for (k = 0; k < LW6MAT_MAT3_V_SIZE_9; ++k)
	      {
		if (fabs (fmat3_mul.v[k] - fmat3_id.v[k]) <
		    _TEST_FMAT_INVMUL_TOLERANCE)
		  {
		    /*
		     * OK, similar enough, we don't require exactness here, there
		     * always rounding errors, and we're working on random tests sets
		     */
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("fmat3_mul failed, element %d is %f and should be %f"),
				k, fmat3_mul.v[k], fmat3_id.v[k]);
		    ret = 0;
		  }
	      }
	    lw6mat_fmat3_scale (&fmat3, _TEST_FMAT_SCALE);
	    ret = _print_fmat3 (&fmat3, "scale") && ret;
	    det_scaled = lw6mat_fmat3_det (&fmat3);
	    det_scaled_expected =
	      det * _TEST_FMAT_SCALE * _TEST_FMAT_SCALE * _TEST_FMAT_SCALE;
	    if (fabs (det_scaled - det_scaled_expected) <
		_TEST_FMAT_SCALE_TOLERANCE)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f"),
			    _TEST_FMAT_SCALE, det_scaled, det);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f, this is not consistent, was expecting %f"),
			    _TEST_FMAT_SCALE, det_scaled, det,
			    det_scaled_expected);
		ret = 0;
	      }
	    fmat3_trans = fmat3;
	    lw6mat_fmat3_trans (&fmat3_trans);
	    ret = _print_fmat3 (&fmat3_trans, "trans 1X") && ret;
	    transposed = 1;
	    for (i = 0; i < LW6MAT_MAT3_M_SIZE_3; ++i)
	      {
		for (j = 0; j < LW6MAT_MAT3_M_SIZE_3; ++j)
		  {
		    if (fmat3.m[i][j] == fmat3_trans.m[j][i])
		      {
			// OK
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("transposition mismatch for fmat3 orig:%d,%d=%f trans:%d,%d=%f"),
				    i, j, fmat3.m[i][j], j, i,
				    fmat3_trans.m[j][i]);
			transposed = 0;
			ret = 0;
		      }
		  }
	      }
	    if (transposed)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("orig and trans fmat3 correspond, OK"));
		lw6mat_fmat3_trans (&fmat3_trans);
		ret = _print_fmat3 (&fmat3_trans, "trans 2X") && ret;
		if (lw6mat_fmat3_is_same (&fmat3, &fmat3_trans))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("orig and double-trans fmat3 look the same, OK"));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("orig and double-trans fmat3 look different"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("transposed fmat3 is broken"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to inverse fmat3"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("unable to produce an fmat3 matrix with a non-zero det"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

// utility to print a fmat4
static int
_print_fmat4 (const lw6mat_fmat4_t * fmat4, const char *about)
{
  int ret = 1;
  char *repr;

  repr = lw6mat_fmat4_repr (fmat4);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("fmat4 %s repr=\"%s\""), about,
		  repr);
      LW6SYS_FREE (repr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to generate fmat4 %s repr"), about);
      ret = 0;
    }

  return ret;
}

/*
 * Testing functions in fmat4.c
 */
static void
_test_fmat4 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_fmat4_t fmat4;
    lw6mat_fmat4_t fmat4_inv;
    lw6mat_fmat4_t fmat4_mul;
    lw6mat_fmat4_t fmat4_id;
    lw6mat_fmat4_t fmat4_trans;
    int i = 0;
    int j = 0;
    int k = 0;
    float det = LW6MAT_F_0;
    float det_scaled = LW6MAT_F_0;
    float det_scaled_expected = LW6MAT_F_0;
    int n = _TEST_MAT_DET_0_NB_TRIES;
    int transposed = 0;

    while ((!_f_is_within_limit (det, _TEST_FMAT_DET_LIMIT)) && n > 0)
      {
	lw6mat_fmat4_zero (&fmat4);
	for (i = 0; i < LW6MAT_MAT4_M_SIZE_4; ++i)
	  {
	    for (j = 0; j < LW6MAT_MAT4_M_SIZE_4; ++j)
	      {
		fmat4.m[i][j] =
		  round (lw6sys_random_float
			 (-_TEST_FMAT_RANDOM_RANGE,
			  _TEST_FMAT_RANDOM_RANGE) /
			 _TEST_FMAT_RANDOM_ROUND) * _TEST_FMAT_RANDOM_ROUND;
		k = lw6mat_mat4_v_index (i, j);
		if (fmat4.m[i][j] == fmat4.v[k])
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("filled matrix fmat4 [column=%d][row=%d] (element %d) with %f"),
				i, j, k, fmat4.m[i][j]);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("problem filling matrix fmat4 [column=%d][row=%d]=%f but element %d is %f"),
				i, j, fmat4.m[i][j], k, fmat4.v[k]);
		    ret = 0;
		  }
	      }
	  }
	ret = _print_fmat4 (&fmat4, "init") && ret;
	det = lw6mat_fmat4_det (&fmat4);
	if (det)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("fmat4 det=%f"), det);
	  }
	n--;
      }

    if (_f_is_within_limit (det, _TEST_FMAT_DET_LIMIT))
      {
	memset (&fmat4_inv, 0xff, sizeof (lw6mat_fmat4_t));
	for (k = 0; k < LW6MAT_MAT4_V_SIZE_16; ++k)
	  {
	    fmat4_inv.v[k] = fmat4.v[k];
	  }

	if (lw6mat_fmat4_is_same (&fmat4, &fmat4_inv))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("fmat4 comparison works when equal"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("fmat4 comparison broken when equal"));
	    ret = 0;
	  }

	fmat4_inv = fmat4;
	if (lw6mat_fmat4_inv (&fmat4_inv))
	  {
	    ret = _print_fmat4 (&fmat4_inv, "inv") && ret;

	    if (!lw6mat_fmat4_is_same (&fmat4, &fmat4_inv))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("fmat4 comparison works when different"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("fmat4 comparison broken when different"));
		ret = 0;
	      }

	    lw6mat_fmat4_mul_fmat4 (&fmat4_mul, &fmat4_inv, &fmat4);
	    ret = _print_fmat4 (&fmat4_mul, "mul") && ret;
	    lw6mat_fmat4_id (&fmat4_id);
	    for (k = 0; k < LW6MAT_MAT4_V_SIZE_16; ++k)
	      {
		if (fabs (fmat4_mul.v[k] - fmat4_id.v[k]) <
		    _TEST_FMAT_INVMUL_TOLERANCE)
		  {
		    /*
		     * OK, similar enough, we don't require exactness here, there
		     * always rounding errors, and we're working on random tests sets
		     */
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("fmat4_mul failed, element %d is %f and should be %f"),
				k, fmat4_mul.v[k], fmat4_id.v[k]);
		    ret = 0;
		  }
	      }
	    lw6mat_fmat4_scale (&fmat4, _TEST_FMAT_SCALE);
	    ret = _print_fmat4 (&fmat4, "scale") && ret;
	    det_scaled = lw6mat_fmat4_det (&fmat4);
	    det_scaled_expected =
	      det * _TEST_FMAT_SCALE * _TEST_FMAT_SCALE * _TEST_FMAT_SCALE *
	      _TEST_FMAT_SCALE;
	    if (fabs (det_scaled - det_scaled_expected) <
		_TEST_FMAT_SCALE_TOLERANCE)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f"),
			    _TEST_FMAT_SCALE, det_scaled, det);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f, this is not consistent, was expecting %f"),
			    _TEST_FMAT_SCALE, det_scaled, det,
			    det_scaled_expected);
		ret = 0;
	      }
	    fmat4_trans = fmat4;
	    lw6mat_fmat4_trans (&fmat4_trans);
	    ret = _print_fmat4 (&fmat4_trans, "trans 1X") && ret;
	    transposed = 1;
	    for (i = 0; i < LW6MAT_MAT4_M_SIZE_4; ++i)
	      {
		for (j = 0; j < LW6MAT_MAT4_M_SIZE_4; ++j)
		  {
		    if (fmat4.m[i][j] == fmat4_trans.m[j][i])
		      {
			// OK
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("transposition mismatch for fmat4 orig:%d,%d=%f trans:%d,%d=%f"),
				    i, j, fmat4.m[i][j], j, i,
				    fmat4_trans.m[j][i]);
			transposed = 0;
			ret = 0;
		      }
		  }
	      }
	    if (transposed)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("orig and trans fmat4 correspond, OK"));
		lw6mat_fmat4_trans (&fmat4_trans);
		ret = _print_fmat4 (&fmat4_trans, "trans 2X") && ret;
		if (lw6mat_fmat4_is_same (&fmat4, &fmat4_trans))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("orig and double-trans fmat4 look the same, OK"));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("orig and double-trans fmat4 look different"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("transposed fmat4 is broken"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to inverse fmat4"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("unable to produce an fmat4 matrix with a non-zero det"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

// utility to print a dmat2
static int
_print_dmat2 (const lw6mat_dmat2_t * dmat2, const char *about)
{
  int ret = 1;
  char *repr;

  repr = lw6mat_dmat2_repr (dmat2);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dmat2 %s repr=\"%s\""), about,
		  repr);
      LW6SYS_FREE (repr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to generate dmat2 %s repr"), about);
      ret = 0;
    }

  return ret;
}

/*
 * Testing functions in dmat2.c
 */
static void
_test_dmat2 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_dmat2_t dmat2;
    lw6mat_dmat2_t dmat2_inv;
    lw6mat_dmat2_t dmat2_mul;
    lw6mat_dmat2_t dmat2_id;
    lw6mat_dmat2_t dmat2_trans;
    int i = 0;
    int j = 0;
    int k = 0;
    double det = LW6MAT_F_0;
    double det_scaled = LW6MAT_F_0;
    double det_scaled_expected = LW6MAT_F_0;
    int n = _TEST_MAT_DET_0_NB_TRIES;
    int transposed = 0;

    while ((!_f_is_within_limit (det, _TEST_DMAT_DET_LIMIT)) && n > 0)
      {
	lw6mat_dmat2_zero (&dmat2);
	for (i = 0; i < LW6MAT_MAT2_M_SIZE_2; ++i)
	  {
	    for (j = 0; j < LW6MAT_MAT2_M_SIZE_2; ++j)
	      {
		dmat2.m[i][j] =
		  round (lw6sys_random_float
			 (-_TEST_DMAT_RANDOM_RANGE,
			  _TEST_DMAT_RANDOM_RANGE) /
			 _TEST_DMAT_RANDOM_ROUND) * _TEST_DMAT_RANDOM_ROUND;
		k = lw6mat_mat2_v_index (i, j);
		if (dmat2.m[i][j] == dmat2.v[k])
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("filled matrix dmat2 [column=%d][row=%d] (element %d) with %f"),
				i, j, k, dmat2.m[i][j]);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("problem filling matrix dmat2 [column=%d][row=%d]=%f but element %d is %f"),
				i, j, dmat2.m[i][j], k, dmat2.v[k]);
		    ret = 0;
		  }
	      }
	  }
	ret = _print_dmat2 (&dmat2, "init") && ret;
	det = lw6mat_dmat2_det (&dmat2);
	if (det)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dmat2 det=%f"), det);
	  }
	n--;
      }

    if (_f_is_within_limit (det, _TEST_DMAT_DET_LIMIT))
      {
	memset (&dmat2_inv, 0xff, sizeof (lw6mat_dmat2_t));
	for (k = 0; k < LW6MAT_MAT2_V_SIZE_4; ++k)
	  {
	    dmat2_inv.v[k] = dmat2.v[k];
	  }

	if (lw6mat_dmat2_is_same (&dmat2, &dmat2_inv))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("dmat2 comparison works when equal"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("dmat2 comparison broken when equal"));
	    ret = 0;
	  }

	dmat2_inv = dmat2;
	if (lw6mat_dmat2_inv (&dmat2_inv))
	  {
	    ret = _print_dmat2 (&dmat2_inv, "inv") && ret;

	    if (!lw6mat_dmat2_is_same (&dmat2, &dmat2_inv))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("dmat2 comparison works when different"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("dmat2 comparison broken when different"));
		ret = 0;
	      }

	    lw6mat_dmat2_mul_dmat2 (&dmat2_mul, &dmat2_inv, &dmat2);
	    ret = _print_dmat2 (&dmat2_mul, "mul") && ret;
	    lw6mat_dmat2_id (&dmat2_id);
	    for (k = 0; k < LW6MAT_MAT2_V_SIZE_4; ++k)
	      {
		if (fabs (dmat2_mul.v[k] - dmat2_id.v[k]) <
		    _TEST_DMAT_INVMUL_TOLERANCE)
		  {
		    /*
		     * OK, similar enough, we don't require exactness here, there
		     * always rounding errors, and we're working on random tests sets
		     */
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("dmat2_mul failed, element %d is %f and should be %f"),
				k, dmat2_mul.v[k], dmat2_id.v[k]);
		    ret = 0;
		  }
	      }
	    lw6mat_dmat2_scale (&dmat2, _TEST_DMAT_SCALE);
	    ret = _print_dmat2 (&dmat2, "scale") && ret;
	    det_scaled = lw6mat_dmat2_det (&dmat2);
	    det_scaled_expected = det * _TEST_DMAT_SCALE * _TEST_DMAT_SCALE;
	    if (fabs (det_scaled - det_scaled_expected) <
		_TEST_DMAT_SCALE_TOLERANCE)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f"),
			    _TEST_DMAT_SCALE, det_scaled, det);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f, this is not consistent, was expecting %f"),
			    _TEST_DMAT_SCALE, det_scaled, det,
			    det_scaled_expected);
		ret = 0;
	      }
	    dmat2_trans = dmat2;
	    lw6mat_dmat2_trans (&dmat2_trans);
	    ret = _print_dmat2 (&dmat2_trans, "trans 1X") && ret;
	    transposed = 1;
	    for (i = 0; i < LW6MAT_MAT2_M_SIZE_2; ++i)
	      {
		for (j = 0; j < LW6MAT_MAT2_M_SIZE_2; ++j)
		  {
		    if (dmat2.m[i][j] == dmat2_trans.m[j][i])
		      {
			// OK
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("transposition mismatch for dmat2 orig:%d,%d=%f trans:%d,%d=%f"),
				    i, j, dmat2.m[i][j], j, i,
				    dmat2_trans.m[j][i]);
			transposed = 0;
			ret = 0;
		      }
		  }
	      }
	    if (transposed)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("orig and trans dmat2 correspond, OK"));
		lw6mat_dmat2_trans (&dmat2_trans);
		ret = _print_dmat2 (&dmat2_trans, "trans 2X") && ret;
		if (lw6mat_dmat2_is_same (&dmat2, &dmat2_trans))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("orig and double-trans dmat2 look the same, OK"));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("orig and double-trans dmat2 look different"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("transposed dmat2 is broken"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to inverse dmat2"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("unable to produce an dmat2 matrix with a non-zero det"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

// utility to print a dmat3
static int
_print_dmat3 (const lw6mat_dmat3_t * dmat3, const char *about)
{
  int ret = 1;
  char *repr;

  repr = lw6mat_dmat3_repr (dmat3);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dmat3 %s repr=\"%s\""), about,
		  repr);
      LW6SYS_FREE (repr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to generate dmat3 %s repr"), about);
      ret = 0;
    }

  return ret;
}

/*
 * Testing functions in dmat3.c
 */
static void
_test_dmat3 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_dmat3_t dmat3;
    lw6mat_dmat3_t dmat3_inv;
    lw6mat_dmat3_t dmat3_mul;
    lw6mat_dmat3_t dmat3_id;
    lw6mat_dmat3_t dmat3_trans;
    int i = 0;
    int j = 0;
    int k = 0;
    double det = LW6MAT_F_0;
    double det_scaled = LW6MAT_F_0;
    double det_scaled_expected = LW6MAT_F_0;
    int n = _TEST_MAT_DET_0_NB_TRIES;
    int transposed = 0;

    while ((!_f_is_within_limit (det, _TEST_DMAT_DET_LIMIT)) && n > 0)
      {
	lw6mat_dmat3_zero (&dmat3);
	for (i = 0; i < LW6MAT_MAT3_M_SIZE_3; ++i)
	  {
	    for (j = 0; j < LW6MAT_MAT3_M_SIZE_3; ++j)
	      {
		dmat3.m[i][j] =
		  round (lw6sys_random_float
			 (-_TEST_DMAT_RANDOM_RANGE,
			  _TEST_DMAT_RANDOM_RANGE) /
			 _TEST_DMAT_RANDOM_ROUND) * _TEST_DMAT_RANDOM_ROUND;
		k = lw6mat_mat3_v_index (i, j);
		if (dmat3.m[i][j] == dmat3.v[k])
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("filled matrix dmat3 [column=%d][row=%d] (element %d) with %f"),
				i, j, k, dmat3.m[i][j]);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("problem filling matrix dmat3 [column=%d][row=%d]=%f but element %d is %f"),
				i, j, dmat3.m[i][j], k, dmat3.v[k]);
		    ret = 0;
		  }
	      }
	  }
	ret = _print_dmat3 (&dmat3, "init") && ret;
	det = lw6mat_dmat3_det (&dmat3);
	if (det)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dmat3 det=%f"), det);
	  }
	n--;
      }

    if (_f_is_within_limit (det, _TEST_DMAT_DET_LIMIT))
      {
	memset (&dmat3_inv, 0xff, sizeof (lw6mat_dmat3_t));
	for (k = 0; k < LW6MAT_MAT3_V_SIZE_9; ++k)
	  {
	    dmat3_inv.v[k] = dmat3.v[k];
	  }

	if (lw6mat_dmat3_is_same (&dmat3, &dmat3_inv))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("dmat3 comparison works when equal"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("dmat3 comparison broken when equal"));
	    ret = 0;
	  }

	dmat3_inv = dmat3;
	if (lw6mat_dmat3_inv (&dmat3_inv))
	  {
	    ret = _print_dmat3 (&dmat3_inv, "inv") && ret;

	    if (!lw6mat_dmat3_is_same (&dmat3, &dmat3_inv))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("dmat3 comparison works when different"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("dmat3 comparison broken when different"));
		ret = 0;
	      }

	    lw6mat_dmat3_mul_dmat3 (&dmat3_mul, &dmat3_inv, &dmat3);
	    ret = _print_dmat3 (&dmat3_mul, "mul") && ret;
	    lw6mat_dmat3_id (&dmat3_id);
	    for (k = 0; k < LW6MAT_MAT3_V_SIZE_9; ++k)
	      {
		if (fabs (dmat3_mul.v[k] - dmat3_id.v[k]) <
		    _TEST_DMAT_INVMUL_TOLERANCE)
		  {
		    /*
		     * OK, similar enough, we don't require exactness here, there
		     * always rounding errors, and we're working on random tests sets
		     */
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("dmat3_mul failed, element %d is %f and should be %f"),
				k, dmat3_mul.v[k], dmat3_id.v[k]);
		    ret = 0;
		  }
	      }
	    lw6mat_dmat3_scale (&dmat3, _TEST_DMAT_SCALE);
	    ret = _print_dmat3 (&dmat3, "scale") && ret;
	    det_scaled = lw6mat_dmat3_det (&dmat3);
	    det_scaled_expected =
	      det * _TEST_DMAT_SCALE * _TEST_DMAT_SCALE * _TEST_DMAT_SCALE;
	    if (fabs (det_scaled - det_scaled_expected) <
		_TEST_DMAT_SCALE_TOLERANCE)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f"),
			    _TEST_DMAT_SCALE, det_scaled, det);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f, this is not consistent, was expecting %f"),
			    _TEST_DMAT_SCALE, det_scaled, det,
			    det_scaled_expected);
		ret = 0;
	      }
	    dmat3_trans = dmat3;
	    lw6mat_dmat3_trans (&dmat3_trans);
	    ret = _print_dmat3 (&dmat3_trans, "trans 1X") && ret;
	    transposed = 1;
	    for (i = 0; i < LW6MAT_MAT3_M_SIZE_3; ++i)
	      {
		for (j = 0; j < LW6MAT_MAT3_M_SIZE_3; ++j)
		  {
		    if (dmat3.m[i][j] == dmat3_trans.m[j][i])
		      {
			// OK
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("transposition mismatch for dmat3 orig:%d,%d=%f trans:%d,%d=%f"),
				    i, j, dmat3.m[i][j], j, i,
				    dmat3_trans.m[j][i]);
			transposed = 0;
			ret = 0;
		      }
		  }
	      }
	    if (transposed)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("orig and trans dmat3 correspond, OK"));
		lw6mat_dmat3_trans (&dmat3_trans);
		ret = _print_dmat3 (&dmat3_trans, "trans 2X") && ret;
		if (lw6mat_dmat3_is_same (&dmat3, &dmat3_trans))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("orig and double-trans dmat3 look the same, OK"));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("orig and double-trans dmat3 look different"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("transposed dmat3 is broken"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to inverse dmat3"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("unable to produce an dmat3 matrix with a non-zero det"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

// utility to print a dmat4
static int
_print_dmat4 (const lw6mat_dmat4_t * dmat4, const char *about)
{
  int ret = 1;
  char *repr;

  repr = lw6mat_dmat4_repr (dmat4);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dmat4 %s repr=\"%s\""), about,
		  repr);
      LW6SYS_FREE (repr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to generate dmat4 %s repr"), about);
      ret = 0;
    }

  return ret;
}

/*
 * Testing functions in dmat4.c
 */
static void
_test_dmat4 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6mat_dmat4_t dmat4;
    lw6mat_dmat4_t dmat4_inv;
    lw6mat_dmat4_t dmat4_mul;
    lw6mat_dmat4_t dmat4_id;
    lw6mat_dmat4_t dmat4_trans;
    int i = 0;
    int j = 0;
    int k = 0;
    double det = LW6MAT_F_0;
    double det_scaled = LW6MAT_F_0;
    double det_scaled_expected = LW6MAT_F_0;
    int n = _TEST_MAT_DET_0_NB_TRIES;
    int transposed = 0;

    while ((!_f_is_within_limit (det, _TEST_DMAT_DET_LIMIT)) && n > 0)
      {
	lw6mat_dmat4_zero (&dmat4);
	for (i = 0; i < LW6MAT_MAT4_M_SIZE_4; ++i)
	  {
	    for (j = 0; j < LW6MAT_MAT4_M_SIZE_4; ++j)
	      {
		dmat4.m[i][j] =
		  round (lw6sys_random_float
			 (-_TEST_DMAT_RANDOM_RANGE,
			  _TEST_DMAT_RANDOM_RANGE) /
			 _TEST_DMAT_RANDOM_ROUND) * _TEST_DMAT_RANDOM_ROUND;
		k = lw6mat_mat4_v_index (i, j);
		if (dmat4.m[i][j] == dmat4.v[k])
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("filled matrix dmat4 [column=%d][row=%d] (element %d) with %f"),
				i, j, k, dmat4.m[i][j]);
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("problem filling matrix dmat4 [column=%d][row=%d]=%f but element %d is %f"),
				i, j, dmat4.m[i][j], k, dmat4.v[k]);
		    ret = 0;
		  }
	      }
	  }
	ret = _print_dmat4 (&dmat4, "init") && ret;
	det = lw6mat_dmat4_det (&dmat4);
	if (det)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("dmat4 det=%f"), det);
	  }
	n--;
      }

    if (_f_is_within_limit (det, _TEST_DMAT_DET_LIMIT))
      {
	memset (&dmat4_inv, 0xff, sizeof (lw6mat_dmat4_t));
	for (k = 0; k < LW6MAT_MAT4_V_SIZE_16; ++k)
	  {
	    dmat4_inv.v[k] = dmat4.v[k];
	  }

	if (lw6mat_dmat4_is_same (&dmat4, &dmat4_inv))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("dmat4 comparison works when equal"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("dmat4 comparison broken when equal"));
	    ret = 0;
	  }

	dmat4_inv = dmat4;
	if (lw6mat_dmat4_inv (&dmat4_inv))
	  {
	    ret = _print_dmat4 (&dmat4_inv, "inv") && ret;

	    if (!lw6mat_dmat4_is_same (&dmat4, &dmat4_inv))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("dmat4 comparison works when different"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("dmat4 comparison broken when different"));
		ret = 0;
	      }

	    lw6mat_dmat4_mul_dmat4 (&dmat4_mul, &dmat4_inv, &dmat4);
	    ret = _print_dmat4 (&dmat4_mul, "mul") && ret;
	    lw6mat_dmat4_id (&dmat4_id);
	    for (k = 0; k < LW6MAT_MAT4_V_SIZE_16; ++k)
	      {
		if (fabs (dmat4_mul.v[k] - dmat4_id.v[k]) <
		    _TEST_DMAT_INVMUL_TOLERANCE)
		  {
		    /*
		     * OK, similar enough, we don't require exactness here, there
		     * always rounding errors, and we're working on random tests sets
		     */
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("dmat4_mul failed, element %d is %f and should be %f"),
				k, dmat4_mul.v[k], dmat4_id.v[k]);
		    ret = 0;
		  }
	      }
	    lw6mat_dmat4_scale (&dmat4, _TEST_DMAT_SCALE);
	    ret = _print_dmat4 (&dmat4, "scale") && ret;
	    det_scaled = lw6mat_dmat4_det (&dmat4);
	    det_scaled_expected =
	      det * _TEST_DMAT_SCALE * _TEST_DMAT_SCALE * _TEST_DMAT_SCALE *
	      _TEST_DMAT_SCALE;
	    if (fabs (det_scaled - det_scaled_expected) <
		_TEST_DMAT_SCALE_TOLERANCE)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f"),
			    _TEST_DMAT_SCALE, det_scaled, det);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("determinant of scaled matrix (scale %f) is %f, orig was %f, this is not consistent, was expecting %f"),
			    _TEST_DMAT_SCALE, det_scaled, det,
			    det_scaled_expected);
		ret = 0;
	      }
	    dmat4_trans = dmat4;
	    lw6mat_dmat4_trans (&dmat4_trans);
	    ret = _print_dmat4 (&dmat4_trans, "trans 1X") && ret;
	    transposed = 1;
	    for (i = 0; i < LW6MAT_MAT4_M_SIZE_4; ++i)
	      {
		for (j = 0; j < LW6MAT_MAT4_M_SIZE_4; ++j)
		  {
		    if (dmat4.m[i][j] == dmat4_trans.m[j][i])
		      {
			// OK
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_
				    ("transposition mismatch for dmat4 orig:%d,%d=%f trans:%d,%d=%f"),
				    i, j, dmat4.m[i][j], j, i,
				    dmat4_trans.m[j][i]);
			transposed = 0;
			ret = 0;
		      }
		  }
	      }
	    if (transposed)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("orig and trans dmat4 correspond, OK"));
		lw6mat_dmat4_trans (&dmat4_trans);
		ret = _print_dmat4 (&dmat4_trans, "trans 2X") && ret;
		if (lw6mat_dmat4_is_same (&dmat4, &dmat4_trans))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("orig and double-trans dmat4 look the same, OK"));
		  }
		else
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("orig and double-trans dmat4 look different"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("transposed dmat4 is broken"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to inverse dmat4"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("unable to produce an dmat4 matrix with a non-zero det"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("init libmat CUnit test suite"));
  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("quit libmat CUnit test suite"));
  return CUE_SUCCESS;
}

/**
 * lw6mat_test_register
 *
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libmat module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6mat_test_register (int mode)
{
  int ret = 1;
  CU_Suite *suite;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (mode);
    }

  suite = CU_add_suite ("lw6mat", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_sizeof);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_convert);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_x);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fvec2);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fvec3);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fvec4);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dvec2);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dvec3);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dvec4);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fmat2);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fmat3);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fmat4);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dmat2);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dmat3);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dmat4);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to add CUnit test suite, error msg is \"%s\""),
		  CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6mat_test_run
 *
 * @mode: test mode (bitmask)
 *
 * Runs the @mat module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6mat_test_run (int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  if (lw6sys_cunit_run_tests (mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
