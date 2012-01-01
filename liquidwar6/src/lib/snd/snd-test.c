/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "snd.h"

#define TEST_FX_VOLUME 0.6f
#define TEST_WATER_VOLUME 0.5f
#define TEST_MUSIC_VOLUME 0.4f
#define TEST_FX_SLEEP 1.0f
#define TEST_WATER_NB 10
#define TEST_WATER_SLEEP 1.0f
#define TEST_MUSIC_SLEEP 10.0f
#define TEST_MUSIC_FILTER ""
#define TEST_MUSIC_EXCLUDE ""
#define TEST_ARGC 1
#define TEST_ARGV0 "prog"

static int
test_init (lw6snd_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *repr = NULL;

    ret = ret
      && lw6snd_init (backend, TEST_FX_VOLUME, TEST_WATER_VOLUME,
		      TEST_MUSIC_VOLUME);
    if (ret)
      {
	repr = lw6snd_repr (backend);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("snd repr is \"%s\""), repr);
	    LW6SYS_FREE (repr);
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_play_fx (lw6snd_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;

    for (i = 0; i < LW6SND_NB_FX; ++i)
      {
	if (lw6snd_play_fx (backend, i))
	  {
	    lw6sys_sleep (TEST_FX_SLEEP);
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_play_water (lw6snd_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;

    for (i = 0; i < TEST_WATER_NB; ++i)
      {
	lw6snd_poll (backend);
	lw6sys_sleep (TEST_WATER_SLEEP);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_play_music (lw6snd_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *map_path = NULL;
    const int argc = TEST_ARGC;
    const char *argv[TEST_ARGC] = { TEST_ARGV0 };

    map_path = lw6cfg_unified_get_music_path (argc, argv);
    if (map_path)
      {
	if (lw6snd_play_music_random
	    (backend, map_path, TEST_MUSIC_FILTER, TEST_MUSIC_EXCLUDE))
	  {
	    lw6sys_sleep (TEST_MUSIC_SLEEP);
	    if (lw6snd_play_music_random
		(backend, map_path, TEST_MUSIC_FILTER, TEST_MUSIC_EXCLUDE))
	      {
		lw6sys_sleep (TEST_MUSIC_SLEEP);
	      }
	    else
	      {
		ret = 0;
	      }
	    lw6snd_stop_music (backend);
	  }
	else
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (map_path);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_quit (lw6snd_backend_t * backend)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  lw6snd_quit (backend);

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6snd_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @snd module test suite. If run in check mode (0),
 * won't really perform the test, since it could fail because
 * of hardware problems, context, permissions...
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6snd_test (int mode)
{
  int ret = 1;
  lw6snd_backend_t *backend;
  const int argc = TEST_ARGC;
  const char *argv[TEST_ARGC] = { TEST_ARGV0 };

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6cfg_test (mode);
      /*
       * No lw6dyn_test, see https://savannah.gnu.org/bugs/index.php?35017
       * this function is available only in non-allinone mode.
       */
      // lw6dyn_test (mode);
    }

  if (mode)
    {
#ifdef MOD_OGG
      backend = lw6snd_create_backend (argc, argv, "ogg");
      if (backend)
	{
	  ret = test_init (backend) && test_play_fx (backend)
	    && test_play_water (backend) && test_play_music (backend)
	    && test_quit (backend) && ret;
	  lw6snd_destroy_backend (backend);
	}
#endif

#ifdef MOD_CSOUND
      backend = lw6snd_create_backend (argc, argv, "csound");
      if (backend)
	{
	  ret = test_init (backend) && test_play_fx (backend)
	    && test_quit (backend) && ret;
	  lw6snd_destroy_backend (backend);
	}
#endif
    }
  else
    {
      ret = 1;
    }

  return ret;
}
