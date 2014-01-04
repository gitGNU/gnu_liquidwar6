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

#define _TEST_FVEC_X 5.0f
#define _TEST_FVEC_Y -10.0f
#define _TEST_FVEC_Z 15.0f
#define _TEST_FVEC_W -20.0f

#define _TEST_FVEC2_LEN_SQ 8192000
#define _TEST_FVEC2_LEN 732715
#define _TEST_FVEC3_LEN_SQ 22937600
#define _TEST_FVEC3_LEN 1226066
#define _TEST_FVEC4_LEN_SQ 49152000
#define _TEST_FVEC4_LEN 1794777

typedef struct _lw6mat_test_data_s
{
  int ret;
} _lw6mat_test_data_t;

static _lw6mat_test_data_t _test_data = { 0 };

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
_setup_init ()
{
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("init libmat CUnit test suite"));
  return CUE_SUCCESS;
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
    float len_sq = 0.0f;
    float len = 0.0f;

    lw6mat_fvec2_zero (&fvec2);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should be a warning"));
    lw6mat_fvec2_norm (&fvec2);
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
    fvec2.p.x = _TEST_FVEC_X;
    fvec2.p.y = _TEST_FVEC_Y;
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
    float len_sq = 0.0f;
    float len = 0.0f;

    lw6mat_fvec3_zero (&fvec3);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should be a warning"));
    lw6mat_fvec3_norm (&fvec3);
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
    fvec3.p.x = _TEST_FVEC_X;
    fvec3.p.y = _TEST_FVEC_Y;
    fvec3.p.z = _TEST_FVEC_Z;
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
    lw6mat_fvec3_norm (&fvec3);
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
    float len_sq = 0.0f;
    float len = 0.0f;

    lw6mat_fvec4_zero (&fvec4);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("will try to normalize vector zero, following line should be a warning"));
    lw6mat_fvec4_norm (&fvec4);
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
    fvec4.p.x = _TEST_FVEC_X;
    fvec4.p.y = _TEST_FVEC_Y;
    fvec4.p.z = _TEST_FVEC_Z;
    fvec4.p.w = _TEST_FVEC_W;
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
    lw6mat_fvec4_norm (&fvec4);
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
  }

  LW6SYS_TEST_FUNCTION_END;
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
      LW6SYS_CUNIT_ADD_TEST (suite, _test_convert);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fvec2);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fvec3);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_fvec4);
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
