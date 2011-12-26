/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "liquidwar6.h"

#define TEST_FILE "test.scm"
#define TEST_ARGC 1
#define TEST_ARGV0 "prog"

typedef struct _lw6_test_param_s
{
  int ret;
}
_lw6_test_param_t;

static char *
get_test_file ()
{
  const int argc = TEST_ARGC;
  const char *argv[] = { TEST_ARGV0 };
  char *script_file = NULL;
  char *script_dir = NULL;
  char *ret = NULL;

  script_file = lw6sys_get_script_file (argc, argv);
  if (script_file)
    {
      script_dir = lw6sys_path_parent (script_file);
      if (script_dir)
	{
	  ret = lw6sys_path_concat (script_dir, TEST_FILE);
	  LW6SYS_FREE (script_dir);
	}
      LW6SYS_FREE (script_file);
    }

  if (ret && !lw6sys_file_exists (ret))
    {
      LW6SYS_FREE (ret);
      ret = NULL;
    }

  return ret;
}

static void
test_callback (_lw6_test_param_t * param)
{
  int ret = 1;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *test_file = NULL;

    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("registering Guile smobs"));
    if (lw6_register_smobs ())
      {

	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("registering Guile functions"));
	if (lw6_register_funcs ())
	  {
	    test_file = get_test_file ();
	    if (test_file)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("loading \"%s\""),
			    test_file);
		lw6scm_c_primitive_load (test_file);
		LW6SYS_FREE (test_file);
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to register funcs"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to register smobs"));
	ret = 0;
      }

    scm_gc ();

    lw6_quit_global ();
  }

  LW6SYS_TEST_FUNCTION_END;

  param->ret = ret;
}

static void *
guile_test (void *data)
{
  _lw6_test_param_t *param = (_lw6_test_param_t *) data;

  test_callback (param);

  return NULL;
}

/**
 * lw6_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the liquidwar6 core module test suite, this will mostly
 * test how Guile script integration works, loading a sample
 * script and running it. It does not launch all the other sub
 * modules tests.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6_test (int mode)
{
  _lw6_test_param_t param;
  const int argc = TEST_ARGC;
  const char *argv[] = { TEST_ARGV0 };

  memset (&param, 0, sizeof (_lw6_test_param_t));

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6glb_test (mode);
      lw6map_test (mode);
      lw6ker_test (mode);
      lw6pil_test (mode);
      lw6bot_test (mode);
      lw6sim_test (mode);
      lw6cns_test (mode);
      lw6hlp_test (mode);
      lw6cfg_test (mode);
      lw6ldr_test (mode);
      lw6tsk_test (mode);
      lw6gui_test (mode);
      lw6vox_test (mode);
      lw6gfx_test (mode);
      lw6dsp_test (mode);
      lw6snd_test (mode);
      lw6img_test (mode);
      lw6net_test (mode);
      lw6nod_test (mode);
      lw6cnx_test (mode);
      lw6msg_test (mode);
      lw6cli_test (mode);
      lw6srv_test (mode);
      lw6dat_test (mode);
      lw6p2p_test (mode);
      lw6scm_test (mode);
    }

  if (lw6_init_global (argc, argv))
    {
      scm_with_guile (guile_test, &param);
    }

  return param.ret;
}
