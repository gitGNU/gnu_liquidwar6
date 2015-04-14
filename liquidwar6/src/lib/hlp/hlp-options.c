
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

#include "hlp.h"

static int
_check_arg (lw6sys_context_t * sys_context, const char *arg)
{
  char *keyword = NULL;
  char *equal = NULL;
  int ret = 0;

  keyword = lw6sys_str_copy (sys_context, arg);
  if (keyword)
    {
      equal = strchr (keyword, '=');
      if (equal)
	{
	  (*equal) = '\0';
	}
      ret = lw6hlp_is_documented (sys_context, keyword);
      LW6SYS_FREE (sys_context, keyword);
    }

  return ret;
}

/**
 * lw6hlp_process_non_run_options:
 *
 * @sys_context: global system context
 * @argc: the number of command-line args, as passed to main
 * @argv: an array of strings containing command-line args, as passed to main
 * @run_game: a pointer to a boolean which will contain true (1) if the
 *   game must be launched, or false (0) if the option is such that game
 *   must be skipped. Example: --copyright, --help, ...
 *
 * Will interpret the command-line arguments, and trap those who are
 * related to docs, this is usefull when building the game,
 * we want to have an extra binary to do this without being linked
 * to SDL, for instance.
 *
 * Return value: non-zero if success, 0 if error. The error can be, for
 *   instance, the test suite returning "no, tests were not OK".
 */
int
lw6hlp_process_non_run_options (lw6sys_context_t * sys_context, int argc, const char *argv[], int *run_game)
{
  int i;
  int ret = 1;
  char *path = NULL;
  char *value = NULL;
  char *pid_file = NULL;

  (*run_game) = 1;
  for (i = 1; i < argc; ++i)
    {
      /*
       * Syntax check, done now to let "/?" pass
       */
      if (!_check_arg (sys_context, argv[i]))
	{
	  lw6sys_log_clear (sys_context, NULL);
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("unknown option \"%s\""), argv[i]);
	  (*run_game) = 0;
	  ret = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_DAEMON, argv[i]))
	{
	  /*
	   * DAEMON mode is one of the first check we make for we want to switch
	   * into this mode as soon as possible.
	   */
	  pid_file = lw6sys_daemon_pid_file (sys_context, argc, argv);
	  if (pid_file)
	    {
	      if (lw6sys_daemon_start (sys_context, pid_file))
		{
		  // OK
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("unable to start daemon"));
		  (*run_game) = 0;
		}
	      LW6SYS_FREE (sys_context, pid_file);
	    }
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_HELP, argv[i]) ||
	       lw6sys_arg_match (sys_context, "h", argv[i]) || lw6sys_arg_match (sys_context, "?", argv[i]))
	{
	  lw6hlp_print_help (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_VERSION, argv[i]) || lw6sys_arg_match (sys_context, "v", argv[i]))
	{
	  lw6hlp_print_version (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_COPYRIGHT, argv[i]) ||
	       lw6sys_arg_match (sys_context, LW6DEF_COPYLEFT, argv[i]) ||
	       lw6sys_arg_match (sys_context, LW6DEF_COPY, argv[i]) || lw6sys_arg_match (sys_context, "c", argv[i]))
	{
	  lw6hlp_print_long_copyright (sys_context, stdout);
	  (*run_game) = 0;
	}
      /*
       * Some other low-level options
       */
      else if (lw6sys_arg_match (sys_context, LW6DEF_PEDIGREE, argv[i]))
	{
	  lw6hlp_print_pedigree (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_HOST, argv[i]))
	{
	  lw6hlp_print_host (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_AUDIT, argv[i]))
	{
	  lw6hlp_print_audit (sys_context, argc, argv, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_MODULES, argv[i]))
	{
	  lw6hlp_print_modules (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_CREDITS, argv[i]))
	{
	  lw6hlp_print_credits (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST, argv[i]))
	{
	  lw6hlp_print_list (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_ABOUT, argv[i]))
	{
	  value = lw6sys_arg_get_value (sys_context, argc, argv, LW6DEF_ABOUT);
	  if (value)
	    {
	      lw6hlp_print_about (sys_context, value, stdout);
	      LW6SYS_FREE (sys_context, value);
	    }
	  (*run_game) = 0;
	}
      /*
       * Specific list
       */
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_QUICK, argv[i]))
	{
	  lw6hlp_print_list_quick (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_DOC, argv[i]))
	{
	  lw6hlp_print_list_doc (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_SHOW, argv[i]))
	{
	  lw6hlp_print_list_show (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_PATH, argv[i]))
	{
	  lw6hlp_print_list_path (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_PLAYERS, argv[i]))
	{
	  lw6hlp_print_list_players (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_INPUT, argv[i]))
	{
	  lw6hlp_print_list_input (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_GRAPHICS, argv[i]))
	{
	  lw6hlp_print_list_graphics (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_SOUND, argv[i]))
	{
	  lw6hlp_print_list_sound (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_NETWORK, argv[i]))
	{
	  lw6hlp_print_list_network (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_MAP, argv[i]))
	{
	  lw6hlp_print_list_map (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_MAP_RULES, argv[i]))
	{
	  lw6hlp_print_list_map_rules (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_MAP_HINTS, argv[i]))
	{
	  lw6hlp_print_list_map_hints (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_MAP_STYLE, argv[i]))
	{
	  lw6hlp_print_list_map_style (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_MAP_TEAMS, argv[i]))
	{
	  lw6hlp_print_list_map_teams (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_FUNCS, argv[i]))
	{
	  lw6hlp_print_list_funcs (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_HOOKS, argv[i]))
	{
	  lw6hlp_print_list_hooks (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_ADVANCED, argv[i]))
	{
	  lw6hlp_print_list_advanced (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_ALIASES, argv[i]))
	{
	  lw6hlp_print_list_aliases (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_TEAM_COLORS, argv[i]))
	{
	  lw6hlp_print_list_team_colors (sys_context, stdout);
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_LIST_WEAPONS, argv[i]))
	{
	  lw6hlp_print_list_weapons (sys_context, stdout);
	  (*run_game) = 0;
	}
      /*
       * Show any build_XXX LW specific value
       */
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_PACKAGE_TARNAME, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_package_tarname ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_PACKAGE_NAME, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_package_name ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_PACKAGE_STRING, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_package_string ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_PACKAGE_ID, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_package_id ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_VERSION, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_version ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_CODENAME, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_codename ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_VERSION_BASE, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_version_base ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_VERSION_MAJOR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_version_major ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_VERSION_MINOR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_version_minor ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_STAMP, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_stamp ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_MD5SUM, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_md5sum ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_COPYRIGHT, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_copyright ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_LICENSE, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_license ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_HOME_URL, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_home_url ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_BUGS_URL, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_bugs_url ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_CONFIGURE_ARGS, argv[i]) || lw6sys_arg_match (sys_context, LW6DEF_CONFIGURE_ARGS, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_configure_args ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_GCC_VERSION, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_gcc_version ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_CFLAGS, argv[i]) || lw6sys_arg_match (sys_context, LW6DEF_CFLAGS, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_cflags ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_LDFLAGS, argv[i]) || lw6sys_arg_match (sys_context, LW6DEF_LDFLAGS, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_ldflags ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_HOSTNAME, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_hostname ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_DATE, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_date ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_TIME, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_time ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_HOST_CPU, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_host_cpu ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENDIANNESS, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_endianness (sys_context));
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_POINTER_SIZE, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_get_pointer_size ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_X86, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_is_x86 ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_AMD64, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_is_amd64 ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_HOST_OS, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_host_os ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_GNU, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_is_gnu ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_UNIX, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_is_unix ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_MS_WINDOWS, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_is_ms_windows ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_MAC_OS_X, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_is_mac_os_x ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_GP2X, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_is_gp2x ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_TOP_SRCDIR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_top_srcdir (sys_context));
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ABS_SRCDIR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_abs_srcdir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_PREFIX, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_prefix ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_DATADIR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_datadir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_LIBDIR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_libdir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_INCLUDEDIR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_includedir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_LOCALEDIR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_localedir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_DOCDIR, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_docdir ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_CONSOLE, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_console ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_GTK, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_gtk ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_MOD_GL1, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_mod_gl1 ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_MOD_GLES2, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_mod_gles2 ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_MOD_SOFT, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_mod_soft ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_MOD_CACA, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_mod_caca ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_MOD_CSOUND, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_mod_csound ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_MOD_OGG, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_mod_ogg ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_MOD_HTTP, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_mod_http ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_OPENMP, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_openmp ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_OPTIMIZE, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_optimize ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_ALLINONE, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_allinone ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_FULLSTATIC, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_fullstatic ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_PARANOID, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_paranoid ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_GPROF, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_gprof ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_INSTRUMENT, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_instrument ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_PROFILER, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_profiler ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_GCOV, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_gcov ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_ENABLE_VALGRIND, argv[i]))
	{
	  fprintf (stdout, "%s\n", lw6sys_build_get_enable_valgrind ());
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_BUILD_BIN_ID, argv[i]))
	{
	  fprintf (stdout, "%d\n", lw6sys_build_get_bin_id (sys_context));
	  (*run_game) = 0;
	}
      /*
       * Show any interesting default path
       */
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_USER_DIR, argv[i]))
	{
	  path = lw6sys_get_default_user_dir (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_CONFIG_FILE, argv[i]))
	{
	  path = lw6sys_get_default_config_file (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_LOG_FILE, argv[i]))
	{
	  path = lw6sys_get_default_log_file (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_PREFIX, argv[i]))
	{
	  path = lw6sys_get_default_prefix (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_MOD_DIR, argv[i]))
	{
	  path = lw6sys_get_default_mod_dir (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_DATA_DIR, argv[i]))
	{
	  path = lw6sys_get_default_data_dir (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_MUSIC_DIR, argv[i]))
	{
	  path = lw6sys_get_default_music_dir (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_MUSIC_PATH, argv[i]))
	{
	  path = lw6sys_get_default_music_path (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_MAP_DIR, argv[i]))
	{
	  path = lw6sys_get_default_map_dir (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_MAP_PATH, argv[i]))
	{
	  path = lw6sys_get_default_map_path (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DEFAULT_SCRIPT_FILE, argv[i]))
	{
	  path = lw6sys_get_default_script_file (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      /*
       * Show any interesting path
       */
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_CWD, argv[i]))
	{
	  path = lw6sys_get_cwd (sys_context);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_RUN_DIR, argv[i]))
	{
	  path = lw6sys_get_run_dir (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_CONFIG_FILE, argv[i]))
	{
	  path = lw6sys_get_config_file (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_PREFIX, argv[i]))
	{
	  path = lw6sys_get_prefix (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_MOD_DIR, argv[i]))
	{
	  path = lw6sys_get_mod_dir (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_DATA_DIR, argv[i]))
	{
	  path = lw6sys_get_data_dir (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_MUSIC_DIR, argv[i]))
	{
	  path = lw6sys_get_music_dir (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_MAP_DIR, argv[i]))
	{
	  path = lw6sys_get_map_dir (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
      else if (lw6sys_arg_match (sys_context, LW6DEF_SHOW_SCRIPT_FILE, argv[i]))
	{
	  path = lw6sys_get_script_file (sys_context, argc, argv);
	  if (path)
	    {
	      fprintf (stdout, "%s\n", path);
	      LW6SYS_FREE (sys_context, path);
	      path = NULL;
	    }
	  (*run_game) = 0;
	}
    }

  return ret;
}
