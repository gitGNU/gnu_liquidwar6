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

/**
 * Content of hints.xml stored into a C struct.
 */
typedef struct lw6ldr_hints_s
{
  /// Wether to resample the map on the fly when loaded.
  int resample;
  /// Minimum map width.
  int min_map_width;
  /// Maximum map width.
  int max_map_width;
  /// Minimum map height.
  int min_map_height;
  /// Maximum map height.
  int max_map_height;
  /// Minimum map surface.
  int min_map_surface;
  /// Maximum map surface.
  int max_map_surface;
  /// Use greater or smaller fighters.
  float fighter_scale;
  /// Wether to downsize the map, if needed, using fighter scale.
  int downsize_using_fighter_scale;
  /// Wether to upsize the map, if needed, using fighter scale.
  int upsize_using_fighter_scale;
  /// Wether to downsize the map, if needed, using bench value.
  int downsize_using_bench_value;
  /// Wether to upsize the map, if needed, using bench value.
  int upsize_using_bench_value;
  /// Wether to guess colors from the map.
  int guess_colors;
  /// Wether to set up background colors automatically.
  int background_color_auto;
  /// Wether to set up hud colors automatically.
  int hud_color_auto;
  /// Wether to set up menu colors automatically.
  int menu_color_auto;
  /// Wether to set up view colors automatically.
  int view_color_auto;
  /// Wether to set up system colors automatically.
  int system_color_auto;
  /// Wall grease used when rescaling.
  int wall_grease;
  /// Guess the moves per sec value automatically.
  int guess_moves_per_sec;
  /// Global speed.
  float speed;
} lw6ldr_hints_t;

#define LW6LDR_USE_DEFAULT_USE_TEXTURE 1
#define LW6LDR_USE_DEFAULT_USE_CURSOR_TEXTURE 1
#define LW6LDR_USE_DEFAULT_USE_RULES_XML 1
#define LW6LDR_USE_DEFAULT_USE_HINTS_XML 1
#define LW6LDR_USE_DEFAULT_USE_STYLE_XML 1
#define LW6LDR_USE_DEFAULT_USE_TEAMS_XML 1
#define LW6LDR_USE_DEFAULT_USE_MUSIC_FILE 1

/**
 * What files to use when loading a map.
 */
typedef struct lw6ldr_use_s
{
  /// Wether to use texture.jpeg.
  int use_texture;
  /// Wether to use cursor-texture.jpeg.
  int use_cursor_texture;
  /// Wether to use rules.xml.
  int use_rules_xml;
  /// Wether to use hints.xml.
  int use_hints_xml;
  /// Wether to use style.xml.
  int use_style_xml;
  /// Wether to use teams.xml.
  int use_teams_xml;
  /// Wether to use the map specific music file.
  int use_music_file;
} lw6ldr_use_t;

/**
 * Almost internal struct use to handler the resampling
 * process. It has informations about the source, the
 * target, and the ratio between them. It basically
 * contains informations about how to scale.
 */
typedef struct lw6ldr_resampler_s
{
  /// Target width.
  int target_w;
  /// Target height.
  int target_h;
  /// Source width.
  int source_w;
  /// Source height.
  int source_h;
  /// Ratio for the X axis (target_w/source_w).
  float scale_x;
  /// Ratio for the Y axis (target_h/source_h).
  float scale_y;
}
lw6ldr_resampler_t;

/**
 * Contains informations about a map, but just the minimum
 * to, for instance, display it in a menu entry.
 */
typedef struct lw6ldr_entry_s
{
  /// The map metadata.
  lw6map_metadata_t metadata;
  /// The map absolute path, use this to load it.
  char *absolute_path;
  /// The map relative path, store this in config file.
  char *relative_path;
  /// Wether the entry has subdirs (and consequently, isn't a map)
  int has_subdirs;
  /// Number of sub mpas within this map.
  int nb_submaps;
  /// Wether it is forbidden (eg, not enough exp).
  int forbidden;
}
lw6ldr_entry_t;

/* ldr-body.c */
extern int lw6ldr_body_read (lw6sys_context_t * sys_context, lw6map_body_t * body, const char *dirname,
			     lw6map_param_t * param,
			     const lw6ldr_hints_t * hints, int display_w,
			     int display_h, float ratio, int bench_value, int magic_number, lw6sys_progress_t * progress);

/* ldr-color.c */
extern void lw6ldr_auto_colors (lw6sys_context_t * sys_context, lw6map_style_t * style, const lw6ldr_hints_t * hints);

/* ldr-cursortexture.c */
extern int lw6ldr_cursor_texture_read (lw6sys_context_t * sys_context, lw6map_cursor_texture_t * cursor_texture, const char *dirname);

/* ldr-dir.c */
extern void lw6ldr_free_entry (lw6sys_context_t * sys_context, lw6ldr_entry_t * entry);
extern lw6ldr_entry_t *lw6ldr_dup_entry (lw6sys_context_t * sys_context, const lw6ldr_entry_t * entry);
extern lw6sys_list_t *lw6ldr_get_entries (lw6sys_context_t * sys_context, char const *map_path, char const *relative_path, char const *user_dir);
extern void lw6ldr_for_all_entries (lw6sys_context_t * sys_context, const char *map_path,
				    const char *relative_path,
				    const char *user_dir, int recursive, lw6sys_list_callback_func_t callback_func, void *func_data);
extern lw6ldr_entry_t *lw6ldr_chain_entry (lw6sys_context_t * sys_context, const char *map_path, const char *relative_path, const char *user_dir);

/* ldr-exp.c */
extern int lw6ldr_exp_validate (lw6sys_context_t * sys_context, const lw6map_level_t * level, const char *user_dir);

/* ldr-grease.c */
extern int lw6ldr_grease_apply (lw6sys_context_t * sys_context, lw6map_layer_t * layer, const lw6map_rules_t * rules, const lw6ldr_hints_t * hints,
				lw6sys_progress_t * progress);

/* ldr-hints.c */
extern void lw6ldr_hints_defaults (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints);
extern void lw6ldr_hints_zero (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints);
extern void lw6ldr_hints_clear (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints);
extern int lw6ldr_hints_read (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints, const char *dirname);
extern int lw6ldr_hints_set (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints, const char *key, const char *value);
extern char *lw6ldr_hints_get (lw6sys_context_t * sys_context, const lw6ldr_hints_t * hints, const char *key);
extern char *lw6ldr_hints_get_default (lw6sys_context_t * sys_context, const char *key);
extern int lw6ldr_hints_update (lw6sys_context_t * sys_context, lw6ldr_hints_t * hints, lw6sys_assoc_t * values);

/* ldr-layer.c */
extern int lw6ldr_layer_read_first (lw6sys_context_t * sys_context, lw6map_layer_t * layer,
				    const char *filename,
				    lw6map_param_t * param,
				    const lw6ldr_hints_t * hints,
				    int display_w, int display_h,
				    float target_ratio, int bench_value, int magic_number, int expected_depth, lw6sys_progress_t * progress);
extern int lw6ldr_layer_read_next (lw6sys_context_t * sys_context, lw6map_layer_t * layer, const char *filename, int target_w, int target_h);

/* ldr-metadata.c */
extern int lw6ldr_metadata_read (lw6sys_context_t * sys_context, lw6map_metadata_t * metadata, const char *dirname);

/* ldr-metalayer.c */
extern int lw6ldr_meta_layer_read (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, const char *filename, int target_w, int target_h,
				   int analog);
extern int lw6ldr_meta_layer_read_if_exists (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, const char *dirname, const char *file_only,
					     int target_w, int target_h, int analog);

/* ldr-options.c */
extern int lw6ldr_process_non_run_options (lw6sys_context_t * sys_context, int argc, const char *argv[], int *run_game);

/* ldr-rules.c */
extern int lw6ldr_rules_read (lw6sys_context_t * sys_context, lw6map_rules_t * rules, const char *dirname);
extern int lw6ldr_rules_update (lw6sys_context_t * sys_context, lw6map_rules_t * rules, lw6sys_assoc_t * values);

/* ldr-param.c */
extern int lw6ldr_param_read (lw6sys_context_t * sys_context, lw6map_param_t * param, const char *dirname);
extern int lw6ldr_param_update (lw6sys_context_t * sys_context, lw6map_param_t * param, lw6sys_assoc_t * values);

/* ldr-print.c */
extern void lw6ldr_print_example_rules_xml (lw6sys_context_t * sys_context, FILE * f);
extern void lw6ldr_print_example_hints_xml (lw6sys_context_t * sys_context, FILE * f);
extern void lw6ldr_print_example_style_xml (lw6sys_context_t * sys_context, FILE * f);
extern void lw6ldr_print_example_teams_xml (lw6sys_context_t * sys_context, FILE * f);
extern int lw6ldr_print_examples (lw6sys_context_t * sys_context, char *user_dir);

/* ldr-read.c */
extern lw6map_level_t *lw6ldr_read (lw6sys_context_t * sys_context, const char *dirname,
				    lw6sys_assoc_t * default_param,
				    lw6sys_assoc_t * forced_param,
				    int display_w, int display_h, int bench_value, int magic_number, const char *user_dir, lw6sys_progress_t * progress);
extern lw6map_level_t *lw6ldr_read_relative (lw6sys_context_t * sys_context, const char *map_path,
					     const char *relative_path,
					     lw6sys_assoc_t * default_param,
					     lw6sys_assoc_t * forced_param,
					     int display_w, int display_h,
					     int bench_value, int magic_number, const char *user_dir, lw6sys_progress_t * progress);

/* ldr-resampler.c */
extern void lw6ldr_resampler_init (lw6sys_context_t * sys_context, lw6ldr_resampler_t * resampler,
				   lw6map_param_t * param,
				   const lw6ldr_hints_t * hints,
				   int source_w,
				   int source_h,
				   int display_w, int display_h, float target_ratio, int bench_value, int magic_number, int expected_depth, float gray_level);
extern void lw6ldr_resampler_use_for_gen (lw6sys_context_t * sys_context, int *map_w, int *map_h, int display_w, int display_h, int bench_value,
					  int magic_number);
extern void lw6ldr_resampler_force (lw6sys_context_t * sys_context, lw6ldr_resampler_t * resampler, int source_w, int source_h, int target_w, int target_h);
extern void lw6ldr_resampler_source2target (lw6sys_context_t * sys_context, const lw6ldr_resampler_t * resampler, int *target_x, int *target_y, int source_x,
					    int source_y);
extern void lw6ldr_resampler_target2source (lw6sys_context_t * sys_context, const lw6ldr_resampler_t * resampler, int *source_x, int *source_y, int target_x,
					    int target_y);

/* ldr-style.c */
extern int lw6ldr_style_read (lw6sys_context_t * sys_context, lw6map_style_t * style, const char *dirname);
extern int lw6ldr_style_update (lw6sys_context_t * sys_context, lw6map_style_t * style, lw6sys_assoc_t * values);

/* ldr-teams.c */
extern int lw6ldr_teams_read (lw6sys_context_t * sys_context, lw6map_teams_t * teams, const char *dirname);
extern int lw6ldr_teams_update (lw6sys_context_t * sys_context, lw6map_teams_t * teams, lw6sys_assoc_t * values);

/* ldr-test.c */
extern int lw6ldr_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6ldr_test_run (lw6sys_context_t * sys_context, int mode);

/* ldr-texture.c */
extern int lw6ldr_texture_read (lw6sys_context_t * sys_context, lw6map_texture_t * texture,
				const char *dirname,
				const lw6map_param_t * param,
				const lw6ldr_hints_t * hints, int use_texture,
				int display_w, int display_h, float *ratio, int *texture_exists, lw6sys_progress_t * progress);

/* ldr-use.c */
extern void lw6ldr_use_defaults (lw6sys_context_t * sys_context, lw6ldr_use_t * use);
extern void lw6ldr_use_clear (lw6sys_context_t * sys_context, lw6ldr_use_t * use);
extern int lw6ldr_use_set (lw6sys_context_t * sys_context, lw6ldr_use_t * use, const char *key, const char *value);
extern int lw6ldr_use_update (lw6sys_context_t * sys_context, lw6ldr_use_t * use, lw6sys_assoc_t * values);

#endif
