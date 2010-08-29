/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include <time.h>
#include <math.h>

#include "liquidwar6.h"

/**
 * lw6_print_help
 *
 * Displays a short help message.
 *
 * Return value: none
 */
void
lw6_print_help ()
{
  lw6sys_list_t *list;

  printf ("\n");
  printf
    (_("Usage: %s [--option1[=value1]] ... [--optionN[=valueN]]\n"),
     lw6sys_build_get_package_tarname ());
  printf ("\n");

  list = lw6hlp_list_quick ();
  if (list)
    {
      lw6hlp_print_content (&list, stdout);
      lw6sys_list_free (list);
    }

  printf
    (_("Online manual: http://www.gnu.org/software/liquidwar6/manual/\n"));
  printf ("\n");
  printf (_("Report bugs to <bug-liquidwar6@gnu.org>\n"));
  printf ("\n");

  fflush (stdout);
}

/**
 * lw6_print_version
 *
 * Displays the version of the game.
 *
 * Return value: none
 */
void
lw6_print_version ()
{
  scm_init_guile ();

  printf ("%s (%s) %s\n", lw6sys_build_get_package_tarname (),
	  lw6sys_build_get_package_name (), lw6sys_build_get_version ());
  lw6_print_short_copyright ();
}

/**
 * lw6_print_short_copyright
 *
 * Displays the copyright of the game (short version).
 *
 * Return value: none
 */
void
lw6_print_short_copyright ()
{
  printf ("%s\n", lw6sys_build_get_copyright ());
  printf (_
	  ("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
	   "This is free software: you are free to change and redistribute it.\n"
	   "There is NO WARRANTY, to the extent permitted by law.\n"));
  fflush (stdout);
}

/**
 * lw6_print_long_copyright
 *
 * Displays the copyright of the game (long version).
 *
 * Return value: none
 */
void
lw6_print_long_copyright ()
{
  printf
    ("%s",
     "Liquid War 6 is a unique multiplayer wargame.\n"
     "Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>\n"
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
     "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
     "Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA\n");
  fflush (stdout);
}

/**
 * lw6_print_bench
 *
 * Displays the program bench value.
 *
 * Return value: none
 */
void
lw6_print_bench (float bench_result)
{
  printf ("%d\n", (int) ceil (bench_result));
  fflush (stdout);
}

/**
 * lw6_print_pedigree
 *
 * Displays the program pedigree, think of this as
 * version on steroids.
 *
 * Return value: none
 */
void
lw6_print_pedigree ()
{
  printf ("package-tarname: %s\n", lw6sys_build_get_package_tarname ());
  printf ("package-name: %s\n", lw6sys_build_get_package_name ());
  printf ("package-string: %s\n", lw6sys_build_get_package_string ());
  printf ("version: %s\n", lw6sys_build_get_version ());
  printf ("codename: %s\n", lw6sys_build_get_codename ());
  printf ("stamp: %s\n", lw6sys_build_get_stamp ());
  printf ("md5sum: %s\n", lw6sys_build_get_md5sum ());
  printf ("copyright: %s\n", lw6sys_build_get_copyright ());
  printf ("license: %s\n", lw6sys_build_get_license ());
  printf ("url: %s\n", lw6sys_build_get_url ());
  printf ("configure-args: %s\n", lw6sys_build_get_configure_args ());
  printf ("gcc-version: %s\n", lw6sys_build_get_gcc_version ());
  printf ("cflags: %s\n", lw6sys_build_get_cflags ());
  printf ("ldflags: %s\n", lw6sys_build_get_ldflags ());
  printf ("hostname: %s\n", lw6sys_build_get_hostname ());
  printf ("date: %s\n", lw6sys_build_get_date ());
  printf ("time: %s\n", lw6sys_build_get_time ());
  printf ("bin-id: %d\n", lw6sys_build_get_bin_id ());
  fflush (stdout);
}

/**
 * lw6_print_host
 *
 * Displays the host on which the program was compiled.
 *
 * Return value: none
 */
void
lw6_print_host ()
{
  printf ("host-cpu: %s\n", lw6sys_build_get_host_cpu ());
  printf ("endianness: %s\n", lw6sys_build_get_endianness ());
  printf ("pointer-size: %d\n", lw6sys_build_get_pointer_size ());
  printf ("x86: %d\n", lw6sys_build_is_x86 ());
  printf ("host-os: %s\n", lw6sys_build_get_host_os ());
  printf ("ms-windows: %d\n", lw6sys_build_is_ms_windows ());
  printf ("mac-os-x: %d\n", lw6sys_build_is_mac_os_x ());
  printf ("gp2x: %d\n", lw6sys_build_is_gp2x ());
  fflush (stdout);
}

/**
 * lw6_print_audit
 *
 * Displays various paths used by the game.
 *
 * Return value: none
 */
void
lw6_print_audit (int argc, char *argv[])
{
  char *default_path = NULL;
  char *path = NULL;

  /*
   * Those could be in pedigree, but there are more convenient here,
   * compared with other path values.
   */
  printf ("top-srcdir: %s\n", lw6sys_build_get_top_srcdir ());
  printf ("datadir: %s\n", lw6sys_build_get_datadir ());
  printf ("libdir: %s\n", lw6sys_build_get_libdir ());
  printf ("includedir: %s\n", lw6sys_build_get_includedir ());
  printf ("localedir: %s\n", lw6sys_build_get_localedir ());
  printf ("docdir: %s\n", lw6sys_build_get_docdir ());

  path = lw6sys_get_cwd (argc, argv);
  if (path)
    {
      printf ("cwd: %s\n", path);
      LW6SYS_FREE (path);
    }
  path = lw6sys_get_run_dir (argc, argv);
  if (path)
    {
      printf ("run-dir: %s\n", path);
      LW6SYS_FREE (path);
    }

  default_path = lw6sys_get_default_user_dir ();
  if (default_path)
    {
      path = lw6sys_get_user_dir (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-user-dir: %s\n", default_path);
	    }
	  printf ("user-dir: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_config_file ();
  if (default_path)
    {
      path = lw6sys_get_config_file (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-config-file: %s\n", default_path);
	    }
	  printf ("config-file: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_log_file ();
  if (default_path)
    {
      path = lw6sys_get_log_file (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-log-file: %s\n", default_path);
	    }
	  printf ("log-file: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_prefix ();
  if (default_path)
    {
      path = lw6sys_get_prefix (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-prefix: %s\n", default_path);
	    }
	  printf ("prefix: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_mod_dir ();
  if (default_path)
    {
      path = lw6sys_get_mod_dir (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-mod-dir: %s\n", default_path);
	    }
	  printf ("mod-dir: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_data_dir ();
  if (default_path)
    {
      path = lw6sys_get_data_dir (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-data-dir: %s\n", default_path);
	    }
	  printf ("data-dir: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_music_dir ();
  if (default_path)
    {
      path = lw6sys_get_music_dir (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-music-dir: %s\n", default_path);
	    }
	  printf ("music-dir: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_music_path ();
  if (default_path)
    {
      path = lw6sys_get_music_path (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-music-path: %s\n", default_path);
	    }
	  printf ("music-path: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_map_dir ();
  if (default_path)
    {
      path = lw6sys_get_map_dir (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-map-dir: %s\n", default_path);
	    }
	  printf ("map-dir: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_map_path ();
  if (default_path)
    {
      path = lw6sys_get_map_path (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-map-path: %s\n", default_path);
	    }
	  printf ("map-path: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  default_path = lw6sys_get_default_script_file ();
  if (default_path)
    {
      path = lw6sys_get_script_file (argc, argv);
      if (path)
	{
	  if (strcmp (default_path, path))
	    {
	      printf ("default-script-file: %s\n", default_path);
	    }
	  printf ("script-file: %s\n", path);
	  LW6SYS_FREE (path);
	}
      LW6SYS_FREE (default_path);
    }

  fflush (stdout);
}

/**
 * lw6_print_audit
 *
 * Displays the list of modules compiled with the game.
 *
 * Return value: none
 */
void
lw6_print_modules ()
{
  printf ("enable-console: %s\n", lw6sys_build_get_enable_console ());
  printf ("enable-gtk: %s\n", lw6sys_build_get_enable_gtk ());
  printf ("enable-mod-gl: %s\n", lw6sys_build_get_enable_mod_gl ());
  printf ("enable-mod-csound: %s\n", lw6sys_build_get_enable_mod_csound ());
  printf ("enable-mod-ogg: %s\n", lw6sys_build_get_enable_mod_ogg ());
  printf ("enable-mod-http: %s\n", lw6sys_build_get_enable_mod_http ());
  printf ("enable-optimize: %s\n", lw6sys_build_get_enable_optimize ());
  printf ("enable-allinone: %s\n", lw6sys_build_get_enable_allinone ());
  printf ("enable-fullstatic: %s\n", lw6sys_build_get_enable_fullstatic ());
  printf ("enable-paranoid: %s\n", lw6sys_build_get_enable_paranoid ());
  printf ("enable-gprof: %s\n", lw6sys_build_get_enable_gprof ());
  printf ("enable-instrument: %s\n", lw6sys_build_get_enable_instrument ());
  printf ("enable-profiler: %s\n", lw6sys_build_get_enable_profiler ());
  printf ("enable-gcov: %s\n", lw6sys_build_get_enable_gcov ());
  printf ("enable-valgrind: %s\n", lw6sys_build_get_enable_valgrind ());

  fflush (stdout);
}

/**
 * lw6_print_list_quick
 *
 * Displays the list of 'quick' options.
 *
 * Return value: none
 */
void
lw6_print_list_quick ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_quick ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_doc
 *
 * Displays the list of 'doc' options.
 *
 * Return value: none
 */
void
lw6_print_list_doc ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_doc ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_show
 *
 * Displays the list of 'show' options.
 *
 * Return value: none
 */
void
lw6_print_list_show ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_show ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_path
 *
 * Displays the list of 'path' options.
 *
 * Return value: none
 */
void
lw6_print_list_path ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_path ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_players
 *
 * Displays the list of 'players' options.
 *
 * Return value: none
 */
void
lw6_print_list_players ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_players ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_input
 *
 * Displays the list of 'input' options.
 *
 * Return value: none
 */
void
lw6_print_list_input ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_input ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_graphics
 *
 * Displays the list of 'graphics' options.
 *
 * Return value: none
 */
void
lw6_print_list_graphics ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_graphics ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_sound
 *
 * Displays the list of 'sound' options.
 *
 * Return value: none
 */
void
lw6_print_list_sound ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_sound ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_network
 *
 * Displays the list of 'network' options.
 *
 * Return value: none
 */
void
lw6_print_list_network ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_network ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_map
 *
 * Displays the list of 'map' options.
 *
 * Return value: none
 */
void
lw6_print_list_map ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_map_rules
 *
 * Displays the list of 'map rules' options.
 *
 * Return value: none
 */
void
lw6_print_list_map_rules ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map_rules ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_map_hints
 *
 * Displays the list of 'map hints' options.
 *
 * Return value: none
 */
void
lw6_print_list_map_hints ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map_hints ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_map_style
 *
 * Displays the list of 'amp style' options.
 *
 * Return value: none
 */
void
lw6_print_list_map_style ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_map_style ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_funcs
 *
 * Displays the list of 'funcs'.
 *
 * Return value: none
 */
void
lw6_print_list_funcs ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_funcs ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_hooks
 *
 * Displays the list of 'hooks'.
 *
 * Return value: none
 */
void
lw6_print_list_hooks ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_hooks ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_advanced
 *
 * Displays the list of 'advanced' options.
 *
 * Return value: none
 */
void
lw6_print_list_advanced ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_advanced ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list_aliases
 *
 * Displays the list of options aliases.
 *
 * Return value: none
 */
void
lw6_print_list_aliases ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list_aliases ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_list
 *
 * Displays the list of all known options.
 *
 * Return value: none
 */
void
lw6_print_list ()
{
  lw6sys_list_t *list;

  list = lw6hlp_list ();
  if (list)
    {
      lw6hlp_print_keyword (&list, stdout);
      lw6sys_list_free (list);
    }
}

/**
 * lw6_print_about
 *
 * Displays the about message for a keyword.
 *
 * Return value: none
 */
void
lw6_print_about (char *keyword)
{
  lw6hlp_print_about (keyword, stdout);
}

/**
 * lw6_print_hello
 *
 * Displays 'hello' at the beginning of the program.
 *
 * Return value: none
 */
void
lw6_print_hello (int argc, char *argv[])
{
  lw6sys_log (LW6SYS_LOG_INFO, _("hello"));

  lw6sys_build_log_all ();
  lw6sys_options_log_defaults ();
  lw6sys_options_log (argc, argv);

  lw6sys_check_types_size (argc, argv);
  lw6sys_log (LW6SYS_LOG_INFO, _("approx. %d megabytes available"),
	      lw6sys_megabytes_available ());
}

/**
 * lw6_print_goodbye
 *
 * Displays 'goodbye', typically use at end of program to
 * know it's over and everything went fine.
 *
 * Return value: none
 */
void
lw6_print_goodbye ()
{
  lw6sys_log (LW6SYS_LOG_INFO, _("goodbye"));
}
