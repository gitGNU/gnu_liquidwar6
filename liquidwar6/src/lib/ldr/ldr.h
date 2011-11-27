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

#ifndef LIQUIDWAR6LDR_H
#define LIQUIDWAR6LDR_H

#include "../sys/sys.h"
#include "../map/map.h"
#include "../cfg/cfg.h"
#include "../hlp/hlp.h"

#define LW6LDR_DEFAULT_BENCH_VALUE 20
#define LW6LDR_DEFAULT_MAX_LOCAL_BENCH_VALUE 800
#define LW6LDR_DEFAULT_MAX_NETWORK_BENCH_VALUE 200

/*
 * This is traditionnally a prime number, just for fun
 * http://en.wikipedia.org/wiki/List_of_prime_numbers
 * 
 * The higher the number is, the more optimistic we
 * are about core algorithm performance. 43261 has
 * been used for some time but proves too high 
 * on too fast maps.
 */
#define LW6LDR_DEFAULT_MAGIC_NUMBER 14741

#define LW6LDR_HINTS_DEFAULT_RESAMPLE 1
#define LW6LDR_HINTS_DEFAULT_MIN_MAP_WIDTH 40
#define LW6LDR_HINTS_DEFAULT_MAX_MAP_WIDTH 1500
#define LW6LDR_HINTS_DEFAULT_MIN_MAP_HEIGHT 30
#define LW6LDR_HINTS_DEFAULT_MAX_MAP_HEIGHT 1000
// 80x45, a 16/9 resolution close to LW3 legacy 80x50
#define LW6LDR_HINTS_DEFAULT_MIN_MAP_SURFACE 3600
// 1000x1000, greater than HD 720p (921600)
#define LW6LDR_HINTS_DEFAULT_MAX_MAP_SURFACE 1000000
#define LW6LDR_HINTS_DEFAULT_FIGHTER_SCALE 1.0
#define LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_FIGHTER_SCALE 0
#define LW6LDR_HINTS_DEFAULT_UPSIZE_USING_FIGHTER_SCALE 1
#define LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_BENCH_VALUE 1
#define LW6LDR_HINTS_DEFAULT_UPSIZE_USING_BENCH_VALUE 0
#define LW6LDR_HINTS_DEFAULT_GUESS_COLORS 1
#define LW6LDR_HINTS_DEFAULT_BACKGROUND_COLOR_AUTO 1
#define LW6LDR_HINTS_DEFAULT_HUD_COLOR_AUTO 1
#define LW6LDR_HINTS_DEFAULT_MENU_COLOR_AUTO 1
#define LW6LDR_HINTS_DEFAULT_VIEW_COLOR_AUTO 1
#define LW6LDR_HINTS_DEFAULT_SYSTEM_COLOR_AUTO 1
#define LW6LDR_HINTS_DEFAULT_WALL_GREASE 0
#define LW6LDR_HINTS_DEFAULT_GUESS_MOVES_PER_SEC 1
#define LW6LDR_HINTS_DEFAULT_SPEED 1.0

#define LW6LDR_HINTS_MIN_WALL_GREASE -5
#define LW6LDR_HINTS_MAX_WALL_GREASE 5

typedef struct lw6ldr_hints_s
{
  int resample;
  int min_map_width;
  int max_map_width;
  int min_map_height;
  int max_map_height;
  int min_map_surface;
  int max_map_surface;
  float fighter_scale;
  int downsize_using_fighter_scale;
  int upsize_using_fighter_scale;
  int downsize_using_bench_value;
  int upsize_using_bench_value;
  int guess_colors;
  int background_color_auto;
  int hud_color_auto;
  int menu_color_auto;
  int view_color_auto;
  int system_color_auto;
  int wall_grease;
  int guess_moves_per_sec;
  float speed;
} lw6ldr_hints_t;

#define LW6LDR_USE_DEFAULT_USE_TEXTURE 1
#define LW6LDR_USE_DEFAULT_USE_CURSOR_TEXTURE 1
#define LW6LDR_USE_DEFAULT_USE_RULES_XML 1
#define LW6LDR_USE_DEFAULT_USE_HINTS_XML 1
#define LW6LDR_USE_DEFAULT_USE_STYLE_XML 1
#define LW6LDR_USE_DEFAULT_USE_TEAMS_XML 1
#define LW6LDR_USE_DEFAULT_USE_MUSIC_FILE 1

typedef struct lw6ldr_use_s
{
  int use_texture;
  int use_cursor_texture;
  int use_rules_xml;
  int use_hints_xml;
  int use_style_xml;
  int use_teams_xml;
  int use_music_file;
} lw6ldr_use_t;

typedef struct lw6ldr_resampler_s
{
  int target_w;
  int target_h;
  int source_w;
  int source_h;
  float scale_x;
  float scale_y;
}
lw6ldr_resampler_t;

typedef struct lw6ldr_entry_s
{
  char *title;
  char *absolute_path;
  char *relative_path;
  int has_subdirs;
  int exp;
  int forbidden;
}
lw6ldr_entry_t;

/* ldr-body.c */
extern int lw6ldr_body_read (lw6map_body_t * body, char *dirname,
			     lw6map_param_t * param, lw6ldr_hints_t * hints,
			     int display_w, int display_h, float ratio,
			     int bench_value, int magic_number,
			     lw6sys_progress_t * progress);

/* ldr-color.c */
extern void lw6ldr_auto_colors (lw6map_style_t * style,
				lw6ldr_hints_t * hints);

/* ldr-cursortexture.c */
extern int lw6ldr_cursor_texture_read (lw6map_cursor_texture_t *
				       cursor_texture, char *dirname);

/* ldr-dir.c */
extern void lw6ldr_free_entry (lw6ldr_entry_t * entry);
extern lw6ldr_entry_t *lw6ldr_dup_entry (lw6ldr_entry_t * entry);
extern lw6sys_list_t *lw6ldr_get_entries (char *map_path,
					  char *relative_path,
					  char *user_dir);
extern void lw6ldr_for_all_entries (char *map_path, char *relative_path,
				    char *user_dir, int recursive,
				    lw6sys_list_callback_func_t callback_func,
				    void *func_data);
extern lw6ldr_entry_t *lw6ldr_chain_entry (char *map_path,
					   char *relative_path,
					   char *user_dir);

/* ldr-exp.c */
extern int lw6ldr_exp_validate (lw6map_level_t * level, char *user_dir);

/* ldr-grease.c */
extern int lw6ldr_grease_apply (lw6map_layer_t * layer,
				lw6map_rules_t * rules,
				lw6ldr_hints_t * hints,
				lw6sys_progress_t * progress);

/* ldr-hints.c */
extern void lw6ldr_hints_defaults (lw6ldr_hints_t * hints);
extern void lw6ldr_hints_zero (lw6ldr_hints_t * hints);
extern void lw6ldr_hints_clear (lw6ldr_hints_t * hints);
extern int lw6ldr_hints_read (lw6ldr_hints_t * hints, char *dirname);
extern int lw6ldr_hints_set (lw6ldr_hints_t * hints, char *key, char *value);
extern char *lw6ldr_hints_get (lw6ldr_hints_t * hints, char *key);
extern char *lw6ldr_hints_get_default (char *key);
extern int lw6ldr_hints_update (lw6ldr_hints_t * hints,
				lw6sys_assoc_t * values);

/* ldr-layer.c */
extern int lw6ldr_layer_read_first (lw6map_layer_t * layer, char *filename,
				    lw6map_param_t * param,
				    lw6ldr_hints_t * hints,
				    int display_w,
				    int display_h,
				    float target_ratio,
				    int bench_value,
				    int magic_number,
				    int expected_depth,
				    lw6sys_progress_t * progress);
extern int lw6ldr_layer_read_next (lw6map_layer_t * layer, char *filename,
				   int target_w, int target_h);

/*
 * In metadata.c
 */
extern int lw6ldr_metadata_read (lw6map_metadata_t * metadata, char *dirname);

/* ldr-metalayer.c */
extern int lw6ldr_meta_layer_read (lw6map_meta_layer_t * meta_layer,
				   char *filename, int target_w,
				   int target_h, int analog);
extern int lw6ldr_meta_layer_read_if_exists (lw6map_meta_layer_t * meta_layer,
					     char *dirname, char *file_only,
					     int target_w, int target_h,
					     int analog);

/* ldr-rules.c */
extern int lw6ldr_rules_read (lw6map_rules_t * rules, char *dirname);
extern int lw6ldr_rules_update (lw6map_rules_t * rules,
				lw6sys_assoc_t * values);

/* ldr-param.c */
extern int lw6ldr_param_read (lw6map_param_t * param, char *dirname);
extern int lw6ldr_param_update (lw6map_param_t * param,
				lw6sys_assoc_t * values);

/* ldr-print.c */
extern void lw6ldr_print_example_rules_xml (FILE * f);
extern void lw6ldr_print_example_hints_xml (FILE * f);
extern void lw6ldr_print_example_style_xml (FILE * f);
extern void lw6ldr_print_example_teams_xml (FILE * f);
extern int lw6ldr_print_examples (char *user_dir);

/* ldr-read.c */
extern lw6map_level_t *lw6ldr_read (char *dirname,
				    lw6sys_assoc_t * default_param,
				    lw6sys_assoc_t * forced_param,
				    int display_w, int display_h,
				    int bench_value, int magic_number,
				    char *user_dir,
				    lw6sys_progress_t * progress);
extern lw6map_level_t *lw6ldr_read_relative (char *map_path,
					     char *relative_path,
					     lw6sys_assoc_t * default_param,
					     lw6sys_assoc_t * forced_param,
					     int display_w, int display_h,
					     int bench_value,
					     int magic_number, char *user_dir,
					     lw6sys_progress_t * progress);

/*
 * In resampler.c
 */
extern void lw6ldr_resampler_init (lw6ldr_resampler_t * resampler,
				   lw6map_param_t * param,
				   lw6ldr_hints_t * hints,
				   int source_w,
				   int source_h,
				   int display_w,
				   int display_h, float target_ratio,
				   int bench_value, int magic_number,
				   int expected_depth, float gray_level);
extern void lw6ldr_resampler_force (lw6ldr_resampler_t * resampler,
				    int source_w, int source_h, int target_w,
				    int target_h);
extern void lw6ldr_resampler_source2target (lw6ldr_resampler_t * resampler,
					    int *target_x, int *target_y,
					    int source_x, int source_y);
extern void lw6ldr_resampler_target2source (lw6ldr_resampler_t * resampler,
					    int *source_x, int *source_y,
					    int target_x, int target_y);

/* ldr-style.c */
extern int lw6ldr_style_read (lw6map_style_t * style, char *dirname);
extern int lw6ldr_style_update (lw6map_style_t * style,
				lw6sys_assoc_t * values);

/* ldr-teams.c */
extern int lw6ldr_teams_read (lw6map_teams_t * teams, char *dirname);
extern int lw6ldr_teams_update (lw6map_teams_t * teams,
				lw6sys_assoc_t * values);

/* ldr-test.c */
extern int lw6ldr_test (int mode);

/*
 * In texture.c
 */
extern int lw6ldr_texture_read (lw6map_texture_t * texture, char *dirname,
				lw6map_param_t * param,
				lw6ldr_hints_t * hints,
				int use_texture,
				int display_w, int display_h, float *ratio,
				int *texture_exists,
				lw6sys_progress_t * progress);

/*
 * use.c
 */
extern void lw6ldr_use_defaults (lw6ldr_use_t * use);
extern void lw6ldr_use_clear (lw6ldr_use_t * use);
extern int lw6ldr_use_set (lw6ldr_use_t * use, char *key, char *value);
extern int lw6ldr_use_update (lw6ldr_use_t * use, lw6sys_assoc_t * values);

#endif
