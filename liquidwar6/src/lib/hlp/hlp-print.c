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

#include <math.h>

#include "hlp.h"

static void
_print_keyword_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  FILE *f;
  char *entry = NULL;

  f = (FILE *) func_data;
  entry = (char *) data;
  if (entry)
    {
      fprintf (f, "%s\n", entry);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to print NULL help keyword"));
    }
}

static void
_print_content_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  FILE *f;
  char *entry = NULL;
  char *desc = NULL;
  const char *about;
  lw6hlp_type_t type;

  f = (FILE *) func_data;
  entry = (char *) data;
  if (entry)
    {
      about = lw6hlp_about (sys_context, &type, NULL, NULL, NULL, entry);
      if (lw6hlp_match (sys_context, entry, LW6DEF_ABOUT))
	{
	  /*
	   * OK this is ugly, but so much less complicated than handling
	   * a "does this have a parameter?" flag, especially when this
	   * is the only exception. On the other side that's typically
	   * the option that requires a *precise* help entry, and
	   * moreover this is displayed by '--help', so, I hacked it.
	   * Note that ./doc/selfdoc.pl does the same.
	   */
	  fprintf (f, "--about=<keyword>\n");
	}
      else
	{
	  fprintf (f, "--%s\n", entry);
	}
      desc = lw6sys_str_reformat (sys_context, about, LW6SYS_REFORMAT_INDENT_PREFIX, LW6SYS_REFORMAT_INDENT_NB_COLUMNS);
      if (desc)
	{
	  fprintf (f, "%s", desc);
	  LW6SYS_FREE (sys_context, desc);
	}
      fprintf (f, "\n");
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to print NULL help keyword"));
    }
}

/**
 * lw6hlp_print_keyword
 *
 * @sys_context: global system context
 * @list: a pointer to a list of keywords
 * @f: the file to print the content to
 *
 * Prints all the keywords from the list. One keyword per line.
 *
 * Return value: none.
 */
void
lw6hlp_print_keyword (lw6sys_context_t * sys_context, lw6sys_list_t ** list, FILE * f)
{
  lw6sys_sort (sys_context, list, lw6sys_sort_str_callback, NULL);
  lw6sys_list_map (sys_context, *list, _print_keyword_callback, (void *) f);
}

/**
 * lw6hlp_print_content
 *
 * @sys_context: global system context
 * @list: a pointer to a list of keywords
 * @f: the file to print the content to
 *
 * Prints all the keywords from the list, with the associated
 * keyword help, to the given file. Output is formatted to fit
 * on the standard terminal/console.
 *
 * Return value: none.
 */
void
lw6hlp_print_content (lw6sys_context_t * sys_context, lw6sys_list_t ** list, FILE * f)
{
  lw6sys_sort (sys_context, list, lw6sys_sort_str_callback, NULL);
  lw6sys_list_map (sys_context, *list, _print_content_callback, (void *) f);
}

/**
 * lw6hlp_print_about
 *
 * @sys_context: global system context
 * @keyword: the keyword to print help about
 * @f: the file to print the content to
 *
 * Displays the help about a keyword, to a file, directly.
 * It's formatted for the purpose of the --about=<value> option.
 *
 * Return value: none
 */
void
lw6hlp_print_about (lw6sys_context_t * sys_context, const char *keyword, FILE * f)
{
  char *doc_str = NULL;
  lw6hlp_type_t type = LW6HLP_TYPE_VOID;
  const char *default_value = NULL;
  int min_value = 0;
  int max_value = 0;
  char *type_str = NULL;
  char *tmp = NULL;
  int values_set = 0;

  doc_str =
    lw6sys_str_reformat (sys_context, lw6hlp_about
			 (sys_context, &type, &default_value, &min_value, &max_value, keyword), LW6SYS_REFORMAT_DEFAULT_PREFIX,
			 LW6SYS_REFORMAT_DEFAULT_NB_COLUMNS);
  if (doc_str)
    {
      switch (type)
	{
	case LW6HLP_TYPE_STR:
	  type_str = _x_ ("string");
	  break;
	case LW6HLP_TYPE_INT:
	  type_str = _x_ ("integer");
	  break;
	case LW6HLP_TYPE_FLOAT:
	  type_str = _x_ ("float");
	  break;
	case LW6HLP_TYPE_BOOL:
	  type_str = _x_ ("boolean");
	  break;
	case LW6HLP_TYPE_COLOR:
	  type_str = _x_ ("color");
	  break;
	default:
	  // stay NULL
	  break;
	}
      if (type_str && type != LW6HLP_TYPE_VOID)
	{
	  tmp = lw6sys_new_sprintf (sys_context, _x_ ("Type: %s"), type_str);
	  if (tmp)
	    {
	      fprintf (f, "%s\n\n", tmp);
	      LW6SYS_FREE (sys_context, tmp);
	    }
	}
      if (default_value)
	{
	  /*
	   * Previous versions of the game used to print a dot "."
	   * after the default value but this is confusing when
	   * the default is "." or "" ...
	   */
	  tmp = lw6sys_new_sprintf (sys_context, _x_ ("Default value: %s"), default_value);
	  if (tmp)
	    {
	      values_set = 1;
	      fprintf (f, "%s\n", tmp);
	      LW6SYS_FREE (sys_context, tmp);
	    }
	}
      if (min_value != 0 || max_value != 0)
	{
	  tmp = lw6sys_new_sprintf (sys_context, _x_ ("Min value: %d"), min_value);
	  if (tmp)
	    {
	      values_set = 1;
	      fprintf (f, "%s\n", tmp);
	      LW6SYS_FREE (sys_context, tmp);
	    }
	  tmp = lw6sys_new_sprintf (sys_context, _x_ ("Max value: %d"), max_value);
	  if (tmp)
	    {
	      values_set = 1;
	      fprintf (f, "%s\n", tmp);
	      LW6SYS_FREE (sys_context, tmp);
	    }
	}
      if (values_set)
	{
	  fprintf (f, "\n");
	}
      fprintf (f, "%s", doc_str);
      fflush (f);
      LW6SYS_FREE (sys_context, doc_str);
    }
}

/**
 * lw6hlp_print_help
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays a short help message.
 *
 * Return value: none
 */
void
lw6hlp_print_help (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  fprintf (f, "\n");
  printf (_("Usage: %s [--option1[=value1]] ... [--optionN[=valueN]]\n"), lw6sys_build_get_package_tarname ());
  fprintf (f, "\n");

  list = lw6hlp_list_quick (sys_context);
  if (list)
    {
      lw6hlp_print_content (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }

  fprintf (f, "%s: %s\n\n", _("Home page"), lw6sys_build_get_home_url ());
  fprintf (f, "%s: %s\n\n", _("Report bugs"), lw6sys_build_get_bugs_url ());

  fflush (f);
}

/**
 * lw6hlp_print_version
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the version of the game.
 *
 * Return value: none
 */
void
lw6hlp_print_version (lw6sys_context_t * sys_context, FILE * f)
{
  fprintf (f, "%s (%s) %s\n", lw6sys_build_get_package_tarname (), lw6sys_build_get_package_name (sys_context), lw6sys_build_get_version ());
  lw6hlp_print_short_copyright (sys_context, f);
}

/**
 * lw6hlp_print_short_copyright
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the copyright of the game (short version).
 *
 * Return value: none
 */
void
lw6hlp_print_short_copyright (lw6sys_context_t * sys_context, FILE * f)
{
  fprintf (f, "%s\n", lw6sys_build_get_copyright ());
  fprintf (f, _
	   ("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
	    "This is free software: you are free to change and redistribute it.\n" "There is NO WARRANTY, to the extent permitted by law.\n"));
  fflush (f);
}

/**
 * lw6hlp_print_long_copyright
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the copyright of the game (long version).
 *
 * Return value: none
 */
void
lw6hlp_print_long_copyright (lw6sys_context_t * sys_context, FILE * f)
{
  printf
    ("%s",
     "Liquid War 6 is a unique multiplayer wargame.\n"
     "Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>\n"
     "\n"
     "This program is free software; you can redistribute it and/or modify\n"
     "it under the terms of the GNU General Public License as published by\n"
     "the Free Software Foundation, either version 3 of the License, or\n"
     "(at your option) any later version.\n"
     "\n"
     "This program is distributed in the hope that it will be useful,\n"
     "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
     "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
     "GNU General Public License for more details.\n"
     "\n"
     "You should have received a copy of the GNU General Public License\n"
     "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n" "Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA\n");
  fflush (f);
}

/**
 * lw6hlp_print_bench
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the program bench value.
 *
 * Return value: none
 */
void
lw6hlp_print_bench (lw6sys_context_t * sys_context, float bench_result, FILE * f)
{
  fprintf (f, "%d\n", (int) ceil (bench_result));
  fflush (f);
}

/**
 * lw6hlp_print_pedigree
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the program pedigree, think of this as
 * version on steroids.
 *
 * Return value: none
 */
void
lw6hlp_print_pedigree (lw6sys_context_t * sys_context, FILE * f)
{
  fprintf (f, "package-tarname: %s\n", lw6sys_build_get_package_tarname ());
  fprintf (f, "package-name: %s\n", lw6sys_build_get_package_name ());
  fprintf (f, "version: %s\n", lw6sys_build_get_version ());
  fprintf (f, "codename: %s\n", lw6sys_build_get_codename ());
  fprintf (f, "md5sum: %s\n", lw6sys_build_get_md5sum ());
  fprintf (f, "copyright: %s\n", lw6sys_build_get_copyright ());
  fprintf (f, "license: %s\n", lw6sys_build_get_license ());
  fprintf (f, "home url: %s\n", lw6sys_build_get_home_url ());
  fprintf (f, "bugs url: %s\n", lw6sys_build_get_bugs_url ());
  fprintf (f, "configure-args: %s\n", lw6sys_build_get_configure_args ());
  fprintf (f, "gcc-version: %s\n", lw6sys_build_get_gcc_version ());
  fprintf (f, "cflags: %s\n", lw6sys_build_get_cflags ());
  fprintf (f, "ldflags: %s\n", lw6sys_build_get_ldflags ());
  fprintf (f, "hostname: %s\n", lw6sys_build_get_hostname ());
  fprintf (f, "date: %s\n", lw6sys_build_get_date ());
  fprintf (f, "time: %s\n", lw6sys_build_get_time ());
  fprintf (f, "bin-id: %d\n", lw6sys_build_get_bin_id (sys_context));
  fflush (f);
}

/**
 * lw6hlp_print_host
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the host on which the program was compiled.
 *
 * Return value: none
 */
void
lw6hlp_print_host (lw6sys_context_t * sys_context, FILE * f)
{
  fprintf (f, "host-cpu: %s\n", lw6sys_build_get_host_cpu ());
  fprintf (f, "endianness: %s\n", lw6sys_build_get_endianness (sys_context));
  fprintf (f, "pointer-size: %d\n", lw6sys_build_get_pointer_size ());
  fprintf (f, "x86: %d\n", lw6sys_build_is_x86 ());
  fprintf (f, "amd64: %d\n", lw6sys_build_is_amd64 ());
  fprintf (f, "host-os: %s\n", lw6sys_build_get_host_os ());
  fprintf (f, "gnu: %d\n", lw6sys_build_is_gnu ());
  fprintf (f, "unix: %d\n", lw6sys_build_is_unix ());
  fprintf (f, "ms-windows: %d\n", lw6sys_build_is_ms_windows ());
  fprintf (f, "mac-os-x: %d\n", lw6sys_build_is_mac_os_x ());
  fprintf (f, "gp2x: %d\n", lw6sys_build_is_gp2x ());
  fflush (f);
}

/**
 * lw6hlp_print_audit
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays various paths used by the game.
 *
 * Return value: none
 */
void
lw6hlp_print_audit (lw6sys_context_t * sys_context, int argc, const char *argv[], FILE * f)
{
  char *default_path = NULL;
  char *path = NULL;

  /*
   * Those could be in pedigree, but there are more convenient here,
   * compared with other path values.
   */
  fprintf (f, "top-srcdir: %s\n", lw6sys_build_get_top_srcdir (sys_context));
  fprintf (f, "abs-srcdir: %s\n", lw6sys_build_get_abs_srcdir ());
  fprintf (f, "datadir: %s\n", lw6sys_build_get_datadir ());
  fprintf (f, "libdir: %s\n", lw6sys_build_get_libdir ());
  fprintf (f, "includedir: %s\n", lw6sys_build_get_includedir ());
  fprintf (f, "localedir: %s\n", lw6sys_build_get_localedir ());
  fprintf (f, "docdir: %s\n", lw6sys_build_get_docdir ());

  path = lw6sys_get_cwd (sys_context);
  if (path)
    {
      fprintf (f, "cwd: %s\n", path);
      LW6SYS_FREE (sys_context, path);
    }
  path = lw6sys_get_run_dir (sys_context, argc, argv);
  if (path)
    {
      fprintf (f, "run-dir: %s\n", path);
      LW6SYS_FREE (sys_context, path);
    }

  default_path = lw6sys_get_default_user_dir (sys_context);
  if (default_path)
    {
      path = lw6sys_get_user_dir (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-user-dir: %s\n", default_path);
	    }
	  fprintf (f, "user-dir: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_config_file (sys_context);
  if (default_path)
    {
      path = lw6sys_get_config_file (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-config-file: %s\n", default_path);
	    }
	  fprintf (f, "config-file: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_log_file (sys_context);
  if (default_path)
    {
      path = lw6sys_get_log_file (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-log-file: %s\n", default_path);
	    }
	  fprintf (f, "log-file: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_prefix (sys_context);
  if (default_path)
    {
      path = lw6sys_get_prefix (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-prefix: %s\n", default_path);
	    }
	  fprintf (f, "prefix: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_mod_dir (sys_context);
  if (default_path)
    {
      path = lw6sys_get_mod_dir (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-mod-dir: %s\n", default_path);
	    }
	  fprintf (f, "mod-dir: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_data_dir (sys_context);
  if (default_path)
    {
      path = lw6sys_get_data_dir (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-data-dir: %s\n", default_path);
	    }
	  fprintf (f, "data-dir: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_music_dir (sys_context);
  if (default_path)
    {
      path = lw6sys_get_music_dir (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-music-dir: %s\n", default_path);
	    }
	  fprintf (f, "music-dir: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_music_path (sys_context);
  if (default_path)
    {
      path = lw6sys_get_music_path (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-music-path: %s\n", default_path);
	    }
	  fprintf (f, "music-path: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_map_dir (sys_context);
  if (default_path)
    {
      path = lw6sys_get_map_dir (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-map-dir: %s\n", default_path);
	    }
	  fprintf (f, "map-dir: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_map_path (sys_context);
  if (default_path)
    {
      path = lw6sys_get_map_path (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-map-path: %s\n", default_path);
	    }
	  fprintf (f, "map-path: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  default_path = lw6sys_get_default_script_file (sys_context);
  if (default_path)
    {
      path = lw6sys_get_script_file (sys_context, argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      fprintf (f, "default-script-file: %s\n", default_path);
	    }
	  fprintf (f, "script-file: %s\n", path);
	  LW6SYS_FREE (sys_context, path);
	}
      LW6SYS_FREE (sys_context, default_path);
    }

  fflush (f);
}

/**
 * lw6hlp_print_modules
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of modules compiled with the game.
 *
 * Return value: none
 */
void
lw6hlp_print_modules (lw6sys_context_t * sys_context, FILE * f)
{
  fprintf (f, "enable-console: %s\n", lw6sys_build_get_enable_console ());
  fprintf (f, "enable-gtk: %s\n", lw6sys_build_get_enable_gtk ());
  fprintf (f, "enable-mod-gl1: %s\n", lw6sys_build_get_enable_mod_gl1 ());
  fprintf (f, "enable-mod-gles2: %s\n", lw6sys_build_get_enable_mod_gles2 ());
  fprintf (f, "enable-mod-soft: %s\n", lw6sys_build_get_enable_mod_soft ());
  fprintf (f, "enable-mod-caca: %s\n", lw6sys_build_get_enable_mod_caca ());
  fprintf (f, "enable-mod-csound: %s\n", lw6sys_build_get_enable_mod_csound ());
  fprintf (f, "enable-mod-ogg: %s\n", lw6sys_build_get_enable_mod_ogg ());
  fprintf (f, "enable-mod-http: %s\n", lw6sys_build_get_enable_mod_http ());
  fprintf (f, "enable-openmp: %s\n", lw6sys_build_get_enable_openmp ());
  fprintf (f, "enable-optimize: %s\n", lw6sys_build_get_enable_optimize ());
  fprintf (f, "enable-allinone: %s\n", lw6sys_build_get_enable_allinone ());
  fprintf (f, "enable-fullstatic: %s\n", lw6sys_build_get_enable_fullstatic ());
  fprintf (f, "enable-paranoid: %s\n", lw6sys_build_get_enable_paranoid ());
  fprintf (f, "enable-gprof: %s\n", lw6sys_build_get_enable_gprof ());
  fprintf (f, "enable-instrument: %s\n", lw6sys_build_get_enable_instrument ());
  fprintf (f, "enable-profiler: %s\n", lw6sys_build_get_enable_profiler ());
  fprintf (f, "enable-gcov: %s\n", lw6sys_build_get_enable_gcov ());
  fprintf (f, "enable-valgrind: %s\n", lw6sys_build_get_enable_valgrind ());

  fflush (f);
}

/**
 * lw6hlp_print_credits
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays all credits on f, those should be available elsewhere
 * within the game (typically on splash screen) but it's good to be able
 * to show them "standalone".
 *
 * Return value: none
 */
void
lw6hlp_print_credits (lw6sys_context_t * sys_context, FILE * f)
{
  int i = 0;
  char *credits;

  for (i = 0; i < LW6HLP_NB_CREDITS_ENTRIES; ++i)
    {
      credits = lw6hlp_get_credits (sys_context, i);
      if (credits)
	{
	  fprintf (f, "%s\n", credits);
	  LW6SYS_FREE (sys_context, credits);
	}
    }

  fflush (f);
}

/**
 * lw6hlp_print_list_quick
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'quick' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_quick (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_quick (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_doc
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'doc' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_doc (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_doc (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_show
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'show' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_show (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_show (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_path
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'path' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_path (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_path (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_players
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'players' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_players (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_players (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_input
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'input' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_input (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_input (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_graphics
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'graphics' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_graphics (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_graphics (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_sound
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'sound' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_sound (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_sound (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_network
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'network' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_network (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_network (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_map
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'map' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_map (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_map_rules
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'map rules' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_map_rules (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map_rules (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_map_hints
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'map hints' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_map_hints (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map_hints (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_map_style
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'map style' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_map_style (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map_style (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_map_teams
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'map teams' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_map_teams (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map_teams (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_funcs
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'funcs'.
 *
 * Return value: none
 */
void
lw6hlp_print_list_funcs (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_funcs (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_hooks
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'hooks'.
 *
 * Return value: none
 */
void
lw6hlp_print_list_hooks (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_hooks (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_advanced
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of 'advanced' options.
 *
 * Return value: none
 */
void
lw6hlp_print_list_advanced (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_advanced (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_aliases
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of options aliases.
 *
 * Return value: none
 */
void
lw6hlp_print_list_aliases (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_aliases (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_team_colors
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of team colors.
 *
 * Return value: none
 */
void
lw6hlp_print_list_team_colors (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_team_colors (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list_weapons
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of weapons.
 *
 * Return value: none
 */
void
lw6hlp_print_list_weapons (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list_weapons (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_list
 *
 * @sys_context: global system context
 * @f: the file to print the content to
 *
 * Displays the list of all known options.
 *
 * Return value: none
 */
void
lw6hlp_print_list (lw6sys_context_t * sys_context, FILE * f)
{
  lw6sys_list_t *list;

  list = lw6hlp_list (sys_context);
  if (list)
    {
      lw6hlp_print_keyword (sys_context, &list, f);
      lw6sys_list_free (sys_context, list);
    }
}

/**
 * lw6hlp_print_hello
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of args as passed to main
 *
 * Displays 'hello' at the beginning of the program.
 *
 * Return value: none
 */
void
lw6hlp_print_hello (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *username = NULL;
  char *hostname = NULL;
  char *date_rfc1123 = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _("hello"));

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, "v%s \"%s\" (%s) %s/%s",
	      lw6sys_build_get_version (),
	      lw6sys_build_get_codename (), lw6sys_build_get_date (), lw6sys_build_get_host_os (sys_context), lw6sys_build_get_host_cpu ());

  date_rfc1123 = lw6sys_date_rfc1123 (sys_context, 0);
  if (date_rfc1123)
    {
      username = lw6sys_get_username (sys_context);
      if (username)
	{
	  hostname = lw6sys_get_hostname (sys_context);
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_INFO, "%s \"%s@%s\"", date_rfc1123, username, hostname);
	    LW6SYS_FREE (sys_context, hostname);
	  }
	  LW6SYS_FREE (sys_context, username);
	}
      LW6SYS_FREE (sys_context, date_rfc1123);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, "%d %s - %d %s", lw6sys_megabytes_available (sys_context), _x_ ("Mb"), lw6sys_openmp_get_num_procs (sys_context),
	      _x_ ("procs"));

  lw6sys_build_log_all (sys_context);
  lw6sys_options_log_defaults (sys_context);
  lw6sys_options_log (sys_context, argc, argv);

  if (!lw6sys_check_types_size (sys_context))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("run-time types size problem"));
    }
}

/**
 * lw6hlp_print_goodbye
 *
 * @sys_context: global system context
 *
 * Displays 'goodbye', typically use at end of program to
 * know it's over and everything went fine.
 *
 * Return value: none
 */
void
lw6hlp_print_goodbye (lw6sys_context_t * sys_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _("goodbye"));
}
