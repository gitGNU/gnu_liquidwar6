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

#ifdef LW6_ALLINONE
#define DYN_TEST 1
#else
#define DYN_TEST lw6dyn_test(1)
#endif

int
check_arg (char *arg)
{
  char *keyword = NULL;
  char *equal = NULL;
  int ret = 0;

  keyword = lw6sys_str_copy (arg);
  if (keyword)
    {
      equal = strchr (keyword, '=');
      if (equal)
	{
	  (*equal) = '\0';
	}
      ret = lw6hlp_is_documented (keyword);
      LW6SYS_FREE (keyword);
    }

  return ret;
}

/**
 * lw6_process_non_run_options:
 *
 * @argc: the number of command-line args, as passed to main()
 * @argv: an array of strings containing command-line args, as passed to main()
 * @run_game: a pointer to a boolean which will contain true (1) if the
 *   game must be launched, or false (0) if the option is such that game 
 *   must be skipped. Example: --copyright, --help, ...
 *
 * Return value: non-zero if success, 0 if error. The error can be, for
 *   instance, the test suite returning "no, tests were not OK".
 */
int
lw6_process_non_run_options (int argc, char *argv[], int *run_game)
{
  int i;
  int ret = 1;
  char *path = NULL;
  char *value = NULL;
  char *log_file = NULL;
  char *pid_file = NULL;
  char *input = NULL;
  char *output = NULL;

  (*run_game) = 1;
  for (i = 1; i < argc; ++i)
    {
      /*
       * Syntax check, done now to let "/?" pass
       */
      if (!check_arg (argv[i]))
	{
	  lw6sys_log_clear (NULL);
	  lw6sys_log (LW6SYS_LOG_ERROR, _("unknown option \"%s\""), argv[i]);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_DAEMON, argv[i]))
	{
	  /*
	   * DAEMON mode is one of the first check we make for we want to switch
	   * into this mode as soon as possible.
	   */
	  pid_file = lw6sys_daemon_pid_file (argc, argv);
	  if (pid_file)
	    {
	      if (lw6sys_daemon_start (pid_file))
		{
		  // OK
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_ERROR, _("unable to start daemon"));
		  (*run_game) = 0;
		}
	      LW6SYS_FREE (pid_file);
	    }
	}
      else if (lw6sys_arg_match (LW6DEF_HELP, argv[i]) ||
	       lw6sys_arg_match ("h", argv[i])
	       || lw6sys_arg_match ("?", argv[i]))
	{
	  lw6_print_help ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_VERSION, argv[i]) ||
	       lw6sys_arg_match ("v", argv[i]))
	{
	  lw6_print_version ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_COPYRIGHT, argv[i]) ||
	       lw6sys_arg_match (LW6DEF_COPYLEFT, argv[i]) ||
	       lw6sys_arg_match (LW6DEF_COPY, argv[i]) ||
	       lw6sys_arg_match ("c", argv[i]))
	{
	  lw6_print_long_copyright ();
	  (*run_game) = 0;
	}
      /*
       * Some other low-level options
       */
      else if (lw6sys_arg_match (LW6DEF_TEST, argv[i]))
	{
	  log_file = lw6cfg_unified_get_log_file (argc, argv);
	  if (log_file)
	    {
	      lw6sys_log_clear (log_file);

	      ret = lw6sys_test_exec (argc, argv, 1) && lw6sys_test (1)
		&& lw6glb_test (1)
		&& lw6map_test (1)
		&& lw6ker_test (1) && DYN_TEST && lw6bot_test (1)
		&& lw6cns_test (1)
		&& lw6hlp_test (1) && lw6cfg_test (1) && lw6ldr_test (1)
		&& lw6tsk_test (1) && lw6gui_test (1) && lw6vox_test (1)
		&& lw6gfx_test (1) && lw6dsp_test (1) && lw6snd_test (1)
		&& lw6img_test (1) && lw6pil_test (1) && lw6net_test (1)
		&& lw6nod_test (1) && lw6cnx_test (1) && lw6msg_test (1)
		&& lw6cli_test (1) && lw6srv_test (1) && lw6dat_test (1)
		&& lw6p2p_test (1) && lw6scm_test (1) && lw6_test (1);
	      if (ret)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE, _("all tests SUCCESSFULL!"));
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_ERROR,
			      _
			      ("test FAILED, see log file \"%s\" for details"),
			      log_file);
		}
	      LW6SYS_FREE (log_file);
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_CHECK, argv[i]))
	{
	  log_file = lw6cfg_unified_get_log_file (argc, argv);
	  if (log_file)
	    {
	      lw6sys_log_clear (log_file);

	      ret = lw6sys_test_exec (argc, argv, 0) && lw6sys_test (0)
		&& lw6glb_test (0)
		&& lw6map_test (0)
		&& lw6ker_test (0) && DYN_TEST && lw6bot_test (0)
		&& lw6cns_test (0)
		&& lw6hlp_test (0) && lw6cfg_test (0) && lw6ldr_test (0)
		&& lw6tsk_test (0) && lw6gui_test (0) && lw6vox_test (0)
		&& lw6gfx_test (0) && lw6dsp_test (0) && lw6snd_test (0)
		&& lw6img_test (0) && lw6pil_test (0) && lw6net_test (0)
		&& lw6nod_test (0) && lw6cnx_test (0) && lw6msg_test (0)
		&& lw6cli_test (0) && lw6srv_test (0) && lw6dat_test (0)
		&& lw6p2p_test (0) && lw6scm_test (0) && lw6_test (0);
	      if (ret)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_
			      ("all tests SUCCESSFULL! (check-only mode)"));
		}
	      else
		{
		  /*
		   * Here only a warning, not an erro which can require
		   * interactive operation to finish
		   */
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("check FAILED, see log file \"%s\" for details"),
			      log_file);
		}
	      LW6SYS_FREE (log_file);
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_BENCH, argv[i]))
	{
	  float bench_result = 0.0f;

	  lw6sys_log_clear (NULL);
	  ret = lw6pil_bench (&bench_result, NULL);
	  lw6_print_bench (bench_result);

	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_RESET, argv[i]))
	{
	  lw6cfg_reset (argc, argv);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_DEFAULTS, argv[i]))
	{
	  lw6cfg_reset (argc, argv);
	}
      else if (lw6sys_arg_match (LW6DEF_PEDIGREE, argv[i]))
	{
	  lw6_print_pedigree ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_HOST, argv[i]))
	{
	  lw6_print_host ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_AUDIT, argv[i]))
	{
	  lw6_print_audit (argc, argv);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_MODULES, argv[i]))
	{
	  lw6_print_modules ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST, argv[i]))
	{
	  lw6_print_list ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_ABOUT, argv[i]))
	{
	  value = lw6sys_arg_get_value (argc, argv, LW6DEF_ABOUT);
	  if (value)
	    {
	      lw6_print_about (value);
	      LW6SYS_FREE (value);
	    }
	  (*run_game) = 0;
	}
      /*
       * Specific list
       */
      else if (lw6sys_arg_match (LW6DEF_LIST_QUICK, argv[i]))
	{
	  lw6_print_list_quick ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_DOC, argv[i]))
	{
	  lw6_print_list_doc ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_SHOW, argv[i]))
	{
	  lw6_print_list_show ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_PATH, argv[i]))
	{
	  lw6_print_list_path ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_PLAYERS, argv[i]))
	{
	  lw6_print_list_players ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_INPUT, argv[i]))
	{
	  lw6_print_list_input ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_GRAPHICS, argv[i]))
	{
	  lw6_print_list_graphics ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_SOUND, argv[i]))
	{
	  lw6_print_list_sound ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_NETWORK, argv[i]))
	{
	  lw6_print_list_network ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_MAP, argv[i]))
	{
	  lw6_print_list_map ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_MAP_RULES, argv[i]))
	{
	  lw6_print_list_map_rules ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_MAP_HINTS, argv[i]))
	{
	  lw6_print_list_map_hints ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_MAP_STYLE, argv[i]))
	{
	  lw6_print_list_map_style ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_MAP_TEAMS, argv[i]))
	{
	  lw6_print_list_map_teams ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_FUNCS, argv[i]))
	{
	  lw6_print_list_funcs ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_HOOKS, argv[i]))
	{
	  lw6_print_list_hooks ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_ADVANCED, argv[i]))
	{
	  lw6_print_list_advanced ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_LIST_ALIASES, argv[i]))
	{
	  lw6_print_list_aliases ();
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_EXAMPLE_RULES_XML, argv[i]))
	{
	  lw6ldr_print_example_rules_xml (stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_EXAMPLE_HINTS_XML, argv[i]))
	{
	  lw6ldr_print_example_hints_xml (stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_EXAMPLE_STYLE_XML, argv[i]))
	{
	  lw6ldr_print_example_style_xml (stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_EXAMPLE_TEAMS_XML, argv[i]))
	{
	  lw6ldr_print_example_teams_xml (stdout);
	  (*run_game) = 0;
	}
      /*
       * Show any build_XXX LW specific value
       */
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_PACKAGE_TARNAME, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_package_tarname ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_PACKAGE_NAME, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_package_name ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_PACKAGE_STRING, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_package_string ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_VERSION, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_version ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_CODENAME, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_codename ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_STAMP, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_stamp ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_MD5SUM, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_md5sum ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_COPYRIGHT, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_copyright ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_LICENSE, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_license ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_HOME_URL, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_home_url ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_BUGS_URL, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_bugs_url ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_CONFIGURE_ARGS, argv[i])
	       || lw6sys_arg_match (LW6DEF_CONFIGURE_ARGS, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_configure_args ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_GCC_VERSION, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_gcc_version ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_CFLAGS, argv[i])
	       || lw6sys_arg_match (LW6DEF_CFLAGS, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_cflags ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_LDFLAGS, argv[i])
	       || lw6sys_arg_match (LW6DEF_LDFLAGS, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_ldflags ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_HOSTNAME, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_hostname ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_DATE, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_date ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_TIME, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_time ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_HOST_CPU, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_host_cpu ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENDIANNESS, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_endianness ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_POINTER_SIZE, argv[i]))
	{
	  printf ("%d\n", lw6sys_build_get_pointer_size ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_X86, argv[i]))
	{
	  printf ("%d\n", lw6sys_build_is_x86 ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_HOST_OS, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_host_os ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_MS_WINDOWS, argv[i]))
	{
	  printf ("%d\n", lw6sys_build_is_ms_windows ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_GP2X, argv[i]))
	{
	  printf ("%d\n", lw6sys_build_is_gp2x ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_MAC_OS_X, argv[i]))
	{
	  printf ("%d\n", lw6sys_build_is_mac_os_x ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_TOP_SRCDIR, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_top_srcdir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_PREFIX, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_prefix ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_DATADIR, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_datadir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_LIBDIR, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_libdir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_INCLUDEDIR, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_includedir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_LOCALEDIR, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_localedir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_DOCDIR, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_docdir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_CONSOLE, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_console ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_GTK, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_gtk ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_MOD_GL, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_mod_gl ());
	  (*run_game) = 0;
	}
      else
	if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_MOD_CSOUND, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_mod_csound ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_MOD_OGG, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_mod_ogg ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_MOD_HTTP, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_mod_http ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_OPENMP, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_openmp ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_OPTIMIZE, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_optimize ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_ALLINONE, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_allinone ());
	  (*run_game) = 0;
	}
      else
	if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_FULLSTATIC, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_fullstatic ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_PARANOID, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_paranoid ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_GPROF, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_gprof ());
	  (*run_game) = 0;
	}
      else
	if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_INSTRUMENT, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_instrument ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_PROFILER, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_profiler ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_GCOV, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_gcov ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_ENABLE_VALGRIND, argv[i]))
	{
	  printf ("%s\n", lw6sys_build_get_enable_valgrind ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_BUILD_BIN_ID, argv[i]))
	{
	  printf ("%d\n", lw6sys_build_get_bin_id ());
	  (*run_game) = 0;
	}
      /*
       * Show any interesting default path
       */
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_USER_DIR, argv[i]))
	{
	  path = lw6sys_get_default_user_dir ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_CONFIG_FILE, argv[i]))
	{
	  path = lw6sys_get_default_config_file ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_LOG_FILE, argv[i]))
	{
	  path = lw6sys_get_default_log_file ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_PREFIX, argv[i]))
	{
	  path = lw6sys_get_default_prefix ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_MOD_DIR, argv[i]))
	{
	  path = lw6sys_get_default_mod_dir ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_DATA_DIR, argv[i]))
	{
	  path = lw6sys_get_default_data_dir ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_MUSIC_DIR, argv[i]))
	{
	  path = lw6sys_get_default_music_dir ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_MUSIC_PATH, argv[i]))
	{
	  path = lw6sys_get_default_music_path ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_MAP_DIR, argv[i]))
	{
	  path = lw6sys_get_default_map_dir ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_MAP_PATH, argv[i]))
	{
	  path = lw6sys_get_default_map_path ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DEFAULT_SCRIPT_FILE, argv[i]))
	{
	  path = lw6sys_get_default_script_file ();
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      /*
       * Show any interesting path
       */
      else if (lw6sys_arg_match (LW6DEF_SHOW_CWD, argv[i]))
	{
	  path = lw6sys_get_cwd (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_RUN_DIR, argv[i]))
	{
	  path = lw6sys_get_run_dir (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_USER_DIR, argv[i]))
	{
	  path = lw6cfg_unified_get_user_dir (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_CONFIG_FILE, argv[i]))
	{
	  path = lw6sys_get_config_file (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_LOG_FILE, argv[i]))
	{
	  path = lw6cfg_unified_get_log_file (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_PREFIX, argv[i]))
	{
	  path = lw6sys_get_prefix (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_MOD_DIR, argv[i]))
	{
	  path = lw6sys_get_mod_dir (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_DATA_DIR, argv[i]))
	{
	  path = lw6sys_get_data_dir (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_MUSIC_DIR, argv[i]))
	{
	  path = lw6sys_get_music_dir (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_MUSIC_PATH, argv[i]))
	{
	  path = lw6cfg_unified_get_music_path (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_MAP_DIR, argv[i]))
	{
	  path = lw6sys_get_map_dir (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_MAP_PATH, argv[i]))
	{
	  path = lw6cfg_unified_get_map_path (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_SHOW_SCRIPT_FILE, argv[i]))
	{
	  path = lw6sys_get_script_file (argc, argv);
	  if (path)
	    {
	      printf ("%s\n", path);
	      LW6SYS_FREE (path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      /*
       * Decode/encode debugging routines
       */
      else if (lw6sys_arg_match (LW6DEF_BASE64_ENCODE, argv[i]))
	{
	  input = lw6sys_stream_file_to_str (stdin);
	  if (input)
	    {
	      output = lw6glb_base64_encode_str (input);
	      if (output)
		{
		  lw6sys_stream_str_to_file (stdout, output);
		  LW6SYS_FREE (output);
		  printf ("\n");
		}
	      LW6SYS_FREE (input);
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_BASE64_DECODE, argv[i]))
	{
	  input = lw6sys_stream_file_to_str (stdin);
	  if (input)
	    {
	      output = lw6glb_base64_decode_str (input);
	      if (output)
		{
		  lw6sys_stream_str_to_file (stdout, output);
		  LW6SYS_FREE (output);
		}
	      LW6SYS_FREE (input);
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_Z_ENCODE, argv[i]))
	{
	  input = lw6sys_stream_file_to_str (stdin);
	  if (input)
	    {
	      output = lw6msg_z_encode (input, 0);
	      if (output)
		{
		  lw6sys_stream_str_to_file (stdout, output);
		  LW6SYS_FREE (output);
		  printf ("\n");
		}
	      LW6SYS_FREE (input);
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (LW6DEF_Z_DECODE, argv[i]))
	{
	  input = lw6sys_stream_file_to_str (stdin);
	  if (input)
	    {
	      output = lw6msg_z_decode (input);
	      if (output)
		{
		  lw6sys_stream_str_to_file (stdout, output);
		  LW6SYS_FREE (output);
		}
	      LW6SYS_FREE (input);
	    }
	  (*run_game) = 0;
	}
    }

  return ret;
}
