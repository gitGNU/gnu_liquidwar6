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

#include <CUnit/CUnit.h>

#include "snd.h"

#define _TEST_FX_VOLUME 0.6f
#define _TEST_WATER_VOLUME 0.5f
#define _TEST_MUSIC_VOLUME 0.4f
#define _TEST_FX_SLEEP 1.0f
#define _TEST_WATER_NB 10
#define _TEST_WATER_SLEEP 1.0f
#define _TEST_MUSIC_SLEEP 10.0f
#define _TEST_MUSIC_FILTER ""
#define _TEST_MUSIC_EXCLUDE ""
#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"

typedef struct _lw6snd_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
  lw6snd_backend_t *backend;
} _lw6snd_test_data_t;

static _lw6snd_test_data_t _test_data = { 0, NULL, NULL };

#if MOD_OGG || MOD_CSOUND
static int
_call_init (lw6sys_context_t * sys_context, lw6snd_backend_t * backend)
{
  int ret = 1;
  char *repr = NULL;

  ret = ret && lw6snd_init (sys_context, backend, _TEST_FX_VOLUME, _TEST_WATER_VOLUME, _TEST_MUSIC_VOLUME);
  if (ret)
    {
      repr = lw6snd_repr (sys_context, backend);
      if (repr)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("snd repr is \"%s\""), repr);
	  LW6SYS_FREE (sys_context, repr);
	}
    }

  return ret;
}

static void
_test_play_fx ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6snd_backend_t *backend = _test_data.backend;
    int i;

    for (i = 0; i < LW6SND_NB_FX; ++i)
      {
	if (lw6snd_play_fx (sys_context, backend, i))
	  {
	    lw6sys_sleep (sys_context, _TEST_FX_SLEEP);
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_play_water ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6snd_backend_t *backend = _test_data.backend;
    int i;

    for (i = 0; i < _TEST_WATER_NB; ++i)
      {
	lw6snd_poll (sys_context, backend);
	lw6sys_sleep (sys_context, _TEST_WATER_SLEEP);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_play_music ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6snd_backend_t *backend = _test_data.backend;
    char *map_path = NULL;
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

    map_path = lw6cfg_unified_get_music_path (sys_context, argc, argv);
    if (map_path)
      {
	if (lw6snd_play_music_random (sys_context, backend, map_path, _TEST_MUSIC_FILTER, _TEST_MUSIC_EXCLUDE))
	  {
	    lw6sys_sleep (sys_context, _TEST_MUSIC_SLEEP);
	    if (lw6snd_play_music_random (sys_context, backend, map_path, _TEST_MUSIC_FILTER, _TEST_MUSIC_EXCLUDE))
	      {
		lw6sys_sleep (sys_context, _TEST_MUSIC_SLEEP);
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6snd_stop_music (sys_context, backend);
	  }
	else
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, map_path);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}
#endif // MOD_OGG || MOD_CSOUND

static void
_call_quit (lw6sys_context_t * sys_context, lw6snd_backend_t * backend)
{
  lw6snd_quit (sys_context, backend);
}

#ifdef MOD_OGG
static int
_setup_init_ogg ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libsnd-ogg CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6snd_create_backend (sys_context, argc, argv, "ogg");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6snd_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_ogg ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libsnd-ogg CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6snd_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}
#endif // MOD_OGG

#ifdef MOD_CSOUND
static int
_setup_init_csound ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;
  int argc = _TEST_ARGC;
  const char *argv[_TEST_ARGC] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libsnd-csound CUnit test suite"));
  if (_test_data.backend == NULL)
    {
      _test_data.backend = lw6snd_create_backend (sys_context, argc, argv, "csound");
      if (_test_data.backend)
	{
	  if (_call_init (sys_context, _test_data.backend))
	    {
	      ret = CUE_SUCCESS;
	    }
	  else
	    {
	      lw6snd_destroy_backend (sys_context, _test_data.backend);
	      _test_data.backend = NULL;
	    }
	}
    }

  return ret;
}

static int
_setup_quit_csound ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libsnd-csound CUnit test suite"));

  if (_test_data.backend)
    {
      _call_quit (sys_context, _test_data.backend);
      lw6snd_destroy_backend (sys_context, _test_data.backend);
      _test_data.backend = NULL;
      ret = CUE_SUCCESS;
    }

  return ret;
}
#endif // MOD_CSOUND

/**
 * lw6snd_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libsnd module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6snd_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
#if MOD_OGG || MOD_CSOUND || MOD_SOFT || MOD_CACA
  CU_Suite *suite = NULL;
#endif // MOD_OGG || MOD_CSOUND || MOD_SOFT || MOD_CACA

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6cfg_test_register (sys_context, mode);
      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
    }

#ifdef MOD_OGG
  suite = CU_add_suite ("lw6snd-ogg", _setup_init_ogg, _setup_quit_ogg);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_play_fx);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_play_water);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_play_music);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }
#endif // MOD_OGG

#ifdef MOD_CSOUND
  suite = CU_add_suite ("lw6snd-csound", _setup_init_csound, _setup_quit_csound);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_play_fx);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_play_water);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_play_music);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }
#endif // MOD_CSOUND

  return ret;
}

/**
 * lw6snd_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @snd module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6snd_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
