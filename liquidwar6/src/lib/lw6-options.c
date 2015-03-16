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

#ifdef LW6_CUNIT
#include <CUnit/CUnit.h>
#endif // LW6_CUNIT

#include "liquidwar6.h"

#ifdef LW6_ALLINONE
#define DYN_TEST_REGISTER(SYS_CONTEXT,MODE) 1
#else // LW6_ALLINONE
#define DYN_TEST_REGISTER(SYS_CONTEXT,MODE) lw6dyn_test_register((SYS_CONTEXT),(MODE))
#endif // LW6_ALLINONE

static int
_register_and_run_tests (lw6sys_context_t * sys_context, int argc, const char **argv, int mode)
{
  int ret = 1;

#ifdef LW6_CUNIT
  ret = lw6sys_test_exec (sys_context, argc, argv, mode) && ret;

  if (ret)
    {
      ret = 0;
      if (CU_initialize_registry () == CUE_SUCCESS)
	{
	  ret =
	    lw6sys_test_register (sys_context, mode)
	    && lw6glb_test_register (sys_context, mode)
	    && lw6map_test_register (sys_context, mode)
	    && lw6ker_test_register (sys_context, mode)
	    && lw6pil_test_register (sys_context, mode) && DYN_TEST_REGISTER (sys_context, mode)
	    && lw6bot_test_register (sys_context, mode) && lw6gen_test_register (sys_context, mode)
	    && lw6sim_test_register (sys_context, mode) && lw6cns_test_register (sys_context, mode)
	    && lw6hlp_test_register (sys_context, mode) && lw6cfg_test_register (sys_context, mode)
	    && lw6ldr_test_register (sys_context, mode) && lw6tsk_test_register (sys_context, mode)
	    && lw6mat_test_register (sys_context, mode) && lw6gui_test_register (sys_context, mode)
	    && lw6vox_test_register (sys_context, mode) && lw6gfx_test_register (sys_context, mode)
	    && lw6dsp_test_register (sys_context, mode) && lw6snd_test_register (sys_context, mode)
	    && lw6img_test_register (sys_context, mode) && lw6net_test_register (sys_context, mode)
	    && lw6nod_test_register (sys_context, mode) && lw6cnx_test_register (sys_context, mode)
	    && lw6msg_test_register (sys_context, mode) && lw6cli_test_register (sys_context, mode)
	    && lw6srv_test_register (sys_context, mode) && lw6dat_test_register (sys_context, mode)
	    && lw6p2p_test_register (sys_context, mode) && lw6scm_test_register (sys_context, mode) && lw6_test_register (sys_context, mode);

	  if (ret)
	    {
	      ret = lw6sys_cunit_run_tests (sys_context, mode);
	    }
	  CU_cleanup_registry ();
	}
    }

#else // LW6_CUNIT
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no test support, CUnit not linked"));
  ret = 0;
#endif // LW6_CUNIT

  return ret;
}

/**
 * lw6_process_non_run_options:
 *
 * Interprets command line arguments, and if some need to be processed,
 * typically those args that are used to display stuff on the console,
 * then perform the corresponding actions.
 *
 * @sys_context: global system context
 * @argc: the number of command-line args, as passed to main
 * @argv: an array of strings containing command-line args, as passed to main
 * @run_game: a pointer to a boolean which will contain true (1) if the
 *   game must be launched, or false (0) if the option is such that game
 *   must be skipped. Example: --copyright, --help, ...
 *
 * Return value: non-zero if success, 0 if error. The error can be, for
 *   instance, the test suite returning "no, tests were not OK".
 */
int
lw6_process_non_run_options (lw6sys_context_t * sys_context, int argc, const char *argv[], int *run_game)
{
  int i;
  int ret = 1;
  char *path = NULL;
  char *log_file = NULL;
  char *input = NULL;
  char *output = NULL;

  /*
   * The following will call lw6hlp_process_non_run_options under
   * the hood, handling basic syntax and a load of doc-related options.
   */
  ret = lw6ldr_process_non_run_options (sys_context, argc, argv, run_game);
  if (ret && (*run_game))
    {
      for (i = 1; i < argc; ++i)
	{
	  /*
	   * Some other low-level options
	   */
	  if (lw6sys_arg_match (sys_context, LW6DEF_TEST, argv[i]))
	    {
	      log_file = lw6cfg_unified_get_log_file (sys_context, argc, argv);
	      if (log_file)
		{
		  lw6sys_log_clear (sys_context, log_file);

		  ret = _register_and_run_tests (sys_context, argc, argv, LW6SYS_TEST_MODE_FULL_TEST);
		  if (ret)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _("all tests SUCCESSFULL!"));
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("test FAILED, see log file \"%s\" for details"), log_file);
		    }
		  LW6SYS_FREE (sys_context, log_file);
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_CHECK, argv[i]))
	    {
	      log_file = lw6cfg_unified_get_log_file (sys_context, argc, argv);
	      if (log_file)
		{
		  lw6sys_log_clear (sys_context, log_file);

		  ret = _register_and_run_tests (sys_context, argc, argv, 0);
		  if (ret)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("all tests SUCCESSFULL! (check-only mode)"));
		    }
		  else
		    {
		      /*
		       * Here only a warning, not an error which can require
		       * interactive operation to finish
		       */
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("check FAILED, see log file \"%s\" for details"), log_file);
		    }
		  LW6SYS_FREE (sys_context, log_file);
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_CUNIT, argv[i]))
	    {
	      log_file = lw6cfg_unified_get_log_file (sys_context, argc, argv);
	      if (log_file)
		{
		  lw6sys_log_clear (sys_context, log_file);

		  ret = _register_and_run_tests (sys_context, argc, argv, LW6SYS_TEST_MODE_FULL_TEST | LW6SYS_TEST_MODE_INTERACTIVE);
		  if (ret)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _("all tests SUCCESSFULL!"));
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("test FAILED, see log file \"%s\" for details"), log_file);
		    }
		  LW6SYS_FREE (sys_context, log_file);
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_BENCH, argv[i]))
	    {
	      float bench_result = 0.0f;

	      lw6sys_log_clear (sys_context, NULL);
	      ret = lw6pil_bench (sys_context, argc, argv, &bench_result, NULL);
	      lw6hlp_print_bench (sys_context, bench_result, stdout);

	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_RESET, argv[i]))
	    {
	      lw6cfg_reset (sys_context, argc, argv);
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_DEFAULTS, argv[i]))
	    {
	      lw6cfg_reset (sys_context, argc, argv);
	    }
	  /*
	   * Show any interesting path
	   */
	  else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_USER_DIR, argv[i]))
	    {
	      path = lw6cfg_unified_get_user_dir (sys_context, argc, argv);
	      if (path)
		{
		  printf ("%s\n", path);
		  LW6SYS_FREE (sys_context, path);
		  path = NULL;
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_LOG_FILE, argv[i]))
	    {
	      path = lw6cfg_unified_get_log_file (sys_context, argc, argv);
	      if (path)
		{
		  printf ("%s\n", path);
		  LW6SYS_FREE (sys_context, path);
		  path = NULL;
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_MUSIC_PATH, argv[i]))
	    {
	      path = lw6cfg_unified_get_music_path (sys_context, argc, argv);
	      if (path)
		{
		  printf ("%s\n", path);
		  LW6SYS_FREE (sys_context, path);
		  path = NULL;
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_MAP_PATH, argv[i]))
	    {
	      path = lw6cfg_unified_get_map_path (sys_context, argc, argv);
	      if (path)
		{
		  printf ("%s\n", path);
		  LW6SYS_FREE (sys_context, path);
		  path = NULL;
		}
	      (*run_game) = 0;
	    }
	  /*
	   * Decode/encode debugging routines
	   */
	  else if (lw6sys_arg_match (sys_context, LW6DEF_BASE64_ENCODE, argv[i]))
	    {
	      input = lw6sys_stream_file_to_str (sys_context, stdin);
	      if (input)
		{
		  output = lw6glb_base64_encode_str (sys_context, input);
		  if (output)
		    {
		      lw6sys_stream_str_to_file (sys_context, stdout, output);
		      LW6SYS_FREE (sys_context, output);
		      printf ("\n");
		    }
		  LW6SYS_FREE (sys_context, input);
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_BASE64_DECODE, argv[i]))
	    {
	      input = lw6sys_stream_file_to_str (sys_context, stdin);
	      if (input)
		{
		  output = lw6glb_base64_decode_str (sys_context, input);
		  if (output)
		    {
		      lw6sys_stream_str_to_file (sys_context, stdout, output);
		      LW6SYS_FREE (sys_context, output);
		    }
		  LW6SYS_FREE (sys_context, input);
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_Z_ENCODE, argv[i]))
	    {
	      input = lw6sys_stream_file_to_str (sys_context, stdin);
	      if (input)
		{
		  output = lw6msg_z_encode (sys_context, input, 0);
		  if (output)
		    {
		      lw6sys_stream_str_to_file (sys_context, stdout, output);
		      LW6SYS_FREE (sys_context, output);
		      printf ("\n");
		    }
		  LW6SYS_FREE (sys_context, input);
		}
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_Z_DECODE, argv[i]))
	    {
	      input = lw6sys_stream_file_to_str (sys_context, stdin);
	      if (input)
		{
		  output = lw6msg_z_decode (sys_context, input);
		  if (output)
		    {
		      lw6sys_stream_str_to_file (sys_context, stdout, output);
		      LW6SYS_FREE (sys_context, output);
		    }
		  LW6SYS_FREE (sys_context, input);
		}
	      (*run_game) = 0;
	    }
	  /*
	   * Simulate fights for statistics
	   */
	  else if (lw6sys_arg_match (sys_context, LW6DEF_SIMULATE_BASIC, argv[i]))
	    {
	      lw6sim_results_t results;

	      lw6sim_simulate_basic (sys_context, argc, argv, &results);
	      lw6sim_print (sys_context, &results, stdout);
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_SIMULATE_FULL, argv[i]))
	    {
	      lw6sim_results_t results;

	      lw6sim_simulate_full (sys_context, argc, argv, &results);
	      lw6sim_print (sys_context, &results, stdout);
	      (*run_game) = 0;
	    }
	}
    }

  return ret;
}
