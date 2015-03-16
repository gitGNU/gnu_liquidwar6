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

#ifndef LIQUIDWAR6HLP_H
#define LIQUIDWAR6HLP_H

#include "../sys/sys.h"

#define LW6HLP_MAIN_BEGIN(SYS_CONTEXT) { lw6hlp_reference_init ((SYS_CONTEXT)); }
#define LW6HLP_MAIN_END(SYS_CONTEXT) { lw6hlp_reference_quit ((SYS_CONTEXT)); }

// number of credits entries, how many different lines can be shown
#define LW6HLP_NB_CREDITS_ENTRIES 26

// used to create hash tables for help & config
#define LW6HLP_APPROX_NB_ENTRIES 421

typedef enum lw6hlp_type_e
{
  LW6HLP_TYPE_VOID = 0,
  LW6HLP_TYPE_STR = 1,
  LW6HLP_TYPE_INT = 2,
  LW6HLP_TYPE_FLOAT = 3,
  LW6HLP_TYPE_BOOL = 4,
  LW6HLP_TYPE_COLOR = 5
}
lw6hlp_type_t;

/* hlp-about.c */
extern int lw6hlp_is_documented (lw6sys_context_t * sys_context, const char *keyword);
extern const char *lw6hlp_about (lw6sys_context_t * sys_context, lw6hlp_type_t * type, const char **default_value, int *min_value, int *max_value,
				 const char *keyword);
extern lw6hlp_type_t lw6hlp_get_type (lw6sys_context_t * sys_context, const char *keyword);
extern const char *lw6hlp_get_default_value (lw6sys_context_t * sys_context, const char *keyword);
extern int lw6hlp_get_min_value (lw6sys_context_t * sys_context, const char *keyword);
extern int lw6hlp_get_max_value (lw6sys_context_t * sys_context, const char *keyword);

/* hlp-credits.c */
extern char *lw6hlp_get_credits (lw6sys_context_t * sys_context, int id);

/* hlp-options.c */
extern int lw6hlp_process_non_run_options (lw6sys_context_t * sys_context, int argc, const char *argv[], int *run_game);

/* hlp-keyword.c */
extern int lw6hlp_match (lw6sys_context_t * sys_context, const char *keyword1, const char *keyword2);

/* hlp-list.c */
extern lw6sys_list_t *lw6hlp_list_quick (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_doc (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_show (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_path (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_players (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_input (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_graphics (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_sound (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_network (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_map (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_map_rules (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_map_hints (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_map_style (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_map_teams (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_funcs (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_hooks (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_advanced (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_aliases (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_team_colors (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list_weapons (lw6sys_context_t * sys_context);
extern lw6sys_list_t *lw6hlp_list (lw6sys_context_t * sys_context);

/* hlp-print.c */
extern void lw6hlp_print_keyword (lw6sys_context_t * sys_context, lw6sys_list_t ** list, FILE * f);
extern void lw6hlp_print_content (lw6sys_context_t * sys_context, lw6sys_list_t ** list, FILE * f);
extern void lw6hlp_print_about (lw6sys_context_t * sys_context, const char *keyword, FILE * f);
extern void lw6hlp_print_help (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_version (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_short_copyright (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_long_copyright (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_bench (lw6sys_context_t * sys_context, float bench_result, FILE * f);
extern void lw6hlp_print_pedigree (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_host (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_audit (lw6sys_context_t * sys_context, int argc, const char *argv[], FILE * f);
extern void lw6hlp_print_modules (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_credits (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_quick (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_doc (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_show (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_path (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_players (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_input (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_graphics (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_sound (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_network (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_map (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_map_hints (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_map_rules (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_map_style (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_map_teams (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_funcs (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_hooks (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_advanced (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_aliases (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_team_colors (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list_weapons (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_list (lw6sys_context_t * sys_context, FILE * f);
extern void lw6hlp_print_hello (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern void lw6hlp_print_goodbye (lw6sys_context_t * sys_context);

/* hlp-ref.c */
extern int lw6hlp_reference_init (lw6sys_context_t * sys_context);
extern void lw6hlp_reference_quit (lw6sys_context_t * sys_context);

/* hlp-test.c */
extern int lw6hlp_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6hlp_test_run (lw6sys_context_t * sys_context, int mode);

#endif
