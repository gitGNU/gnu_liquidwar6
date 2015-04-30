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

#ifndef LIQUIDWAR6GFX_MOD_GL1_UTILS_H
#define LIQUIDWAR6GFX_MOD_GL1_UTILS_H

#include <math.h>

#include "../../gfx-internal.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <SDL_opengl.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE GL_CLAMP
#endif

#define MOD_GL1_UTILS_MENU_TEXTURE_W 512
#define MOD_GL1_UTILS_MENU_TEXTURE_H 128
#define MOD_GL1_UTILS_TRANSPARENCY_SCALE 100

/*
 * We keep 64 menu bitmaps in memory, to avoid redrawing them
 * at each display frame.
 */
#define MOD_GL1_UTILS_MENUCACHE_ARRAY_SIZE 64

/*
 * We keep 32 textures in memory, this is to avoid transfering
 * bitmaps from SDL structures to the OpenGL world at each frame
 * display.
 */
#define MOD_GL1_UTILS_TEXTURECACHE_ARRAY_SIZE 32


/*
 * OpenGL should handle at least this texture size. Theorically
 * we should support any MAX_TEXTURE_SIZE value, but in practise,
 * most recent drivers support 512x512, and the "liquidwarish"
 * answer to this problem is -> use another backend.
 */
#define MOD_GL1_UTILS_REQUIRED_TEXTURE_SIZE 512

/*
 * How many different colors for armies.
 */
#define MOD_GL1_SHADES_FOR_FIGHTERS_SCALE 64

typedef struct mod_gl1_utils_bitmap_s
{
  u_int32_t id;
  char *desc;
  SDL_Surface *surface;
  int has_alpha;
  SDL_Surface *colorized_surface;
  int colorize;
  lw6map_color_couple_t colorize_color;
  int mipmap;
  GLint wrap;
  GLint filter;
  GLuint texture;
  int texture_w;
  int texture_h;
  float s1;
  float t1;
  float s2;
  float t2;
  int64_t last_refresh;
  int need_another_refresh;
} mod_gl1_utils_bitmap_t;

typedef struct mod_gl1_utils_shaded_text_s
{
  TTF_Font *font;
  char *text;
  lw6map_color_couple_t color;
  mod_gl1_utils_bitmap_t *bg;
  mod_gl1_utils_bitmap_t *fg;
  int texture_w;
  int texture_h;
} mod_gl1_utils_shaded_text_t;

typedef struct mod_gl1_utils_path_s
{
  char *data_dir;
  char *capture_dir;
  char *bitmap_dir;
  char *bitmap_frame_dir;
}
mod_gl1_utils_path_t;

typedef struct mod_gl1_utils_mem_counter_s
{
  int new_counter;
  int delete_counter;
}
mod_gl1_utils_mem_counter_t;

typedef struct mod_gl1_utils_caps_s
{
  GLint max_texture_size;
  int bpp;
}
mod_gl1_utils_caps_t;

typedef enum mod_gl1_utils_render_mode_e
{
  MOD_GL1_UTILS_RENDER_NONE = 0,
  MOD_GL1_UTILS_RENDER_2D = 1,
  MOD_GL1_UTILS_RENDER_2D_BLEND = 2,
  MOD_GL1_UTILS_RENDER_3D_MAP = 3,
  MOD_GL1_UTILS_RENDER_3D_MENU = 4,
  MOD_GL1_UTILS_RENDER_3D_SELECT = 5,
  MOD_GL1_UTILS_RENDER_3D_FEEDBACK = 6
}
mod_gl1_utils_render_mode_t;

typedef struct mod_gl1_utils_render_param_s
{
  mod_gl1_utils_render_mode_t mode;
  int gfx_quality;
} mod_gl1_utils_render_param_t;

typedef struct mod_gl1_utils_rect_array_s
{
  int tile_size;
  int border_size;
  int tile_spacing;
  int nb_tiles_w;
  int nb_tiles_h;
  int nb_tiles;
  int source_w;
  int source_h;
}
mod_gl1_utils_rect_array_t;

typedef struct mod_gl1_utils_bitmap_array_s
{
  lw6gui_rect_array_t layout;
  mod_gl1_utils_bitmap_t **bitmaps;
}
mod_gl1_utils_bitmap_array_t;

typedef struct mod_gl1_utils_texture_data_s
{
  lw6sys_list_t *to_delete;
}
mod_gl1_utils_texture_data_t;

typedef struct mod_gl1_utils_font_data_s
{
  TTF_Font *menu;
  TTF_Font *hud;
  TTF_Font *cursor;
  TTF_Font *system;
}
mod_gl1_utils_font_data_t;

typedef struct mod_gl1_utils_menucache_item_s
{
  char *key;
  mod_gl1_utils_bitmap_t *bitmap;
}
mod_gl1_utils_menucache_item_t;

typedef struct mod_gl1_utils_menucache_array_s
{
  lw6map_color_set_t color_set;
  int last_stored_item;
  mod_gl1_utils_menucache_item_t item_array[MOD_GL1_UTILS_MENUCACHE_ARRAY_SIZE];
  char *tooltip_str;
  mod_gl1_utils_bitmap_t *tooltip_bitmap;
  char *help_str;
  mod_gl1_utils_bitmap_t *help_bitmap;
  char *breadcrumbs_str;
  mod_gl1_utils_bitmap_t *breadcrumbs_bitmap;
  char *popup_str;
  mod_gl1_utils_bitmap_t *popup_bitmap;
}
mod_gl1_utils_menucache_array_t;

typedef struct mod_gl1_utils_const_data_s
{
  /*
   * Set by the utils module itself
   */
  float persp_fovy;
  float persp_znear;
  float persp_zfar;
  int menu_font_size;
  int hud_font_size;
  int cursor_font_size;
  int system_font_size;
  int system_font_dh;
  int system_font_dw;
  float system_font_hcoef;
  float gradient_opacity;
  float zones_opacity;
  float mainlight_r;
  float mainlight_g;
  float mainlight_b;
  float mainlight_a;
  float mainlight_x;
  float mainlight_y;
  float mainlight_z;
  int smoother_global_zoom_duration;
  int smoother_center_duration;
  int smoother_drawable_duration;
}
mod_gl1_utils_const_data_t;

typedef struct mod_gl1_utils_texture_1x1_s
{
  mod_gl1_utils_bitmap_t *color_base_fg;
  mod_gl1_utils_bitmap_t *color_base_bg;
  mod_gl1_utils_bitmap_t *color_alternate_fg;
  mod_gl1_utils_bitmap_t *color_alternate_bg;
  mod_gl1_utils_bitmap_t *background_color_root_fg;
  mod_gl1_utils_bitmap_t *background_color_root_bg;
  mod_gl1_utils_bitmap_t *background_color_stuff_fg;
  mod_gl1_utils_bitmap_t *background_color_stuff_bg;
  mod_gl1_utils_bitmap_t *hud_color_frame_fg;
  mod_gl1_utils_bitmap_t *hud_color_frame_bg;
  mod_gl1_utils_bitmap_t *hud_color_text_fg;
  mod_gl1_utils_bitmap_t *hud_color_text_bg;
  mod_gl1_utils_bitmap_t *menu_color_default_fg;
  mod_gl1_utils_bitmap_t *menu_color_default_bg;
  mod_gl1_utils_bitmap_t *menu_color_selected_fg;
  mod_gl1_utils_bitmap_t *menu_color_selected_bg;
  mod_gl1_utils_bitmap_t *menu_color_disabled_fg;
  mod_gl1_utils_bitmap_t *menu_color_disabled_bg;
  mod_gl1_utils_bitmap_t *view_color_cursor_fg;
  mod_gl1_utils_bitmap_t *view_color_cursor_bg;
  mod_gl1_utils_bitmap_t *view_color_map_fg;
  mod_gl1_utils_bitmap_t *view_color_map_bg;
  mod_gl1_utils_bitmap_t *system_color_fg;
  mod_gl1_utils_bitmap_t *system_color_bg;
  mod_gl1_utils_bitmap_t *team_color_dead;
  mod_gl1_utils_bitmap_t *team_colors[LW6MAP_NB_TEAM_COLORS];
  mod_gl1_utils_bitmap_t *team_colors_transparency[LW6MAP_NB_TEAM_COLORS][MOD_GL1_UTILS_TRANSPARENCY_SCALE + 1];
}
mod_gl1_utils_texture_1x1_t;

typedef struct mod_gl1_utils_team_color_map_s
{
  lw6sys_color_f_t team_colors_f[LW6MAP_MAX_NB_TEAMS];
  Uint32 team_colors[LW6MAP_MAX_NB_TEAMS][MOD_GL1_SHADES_FOR_FIGHTERS_SCALE + 1];
}
mod_gl1_utils_team_color_map_t;

typedef struct mod_gl1_utils_last_action_s
{
  int game_bitmap_array_update_id;
  int game_bitmap_array_update_rounds;
}
mod_gl1_utils_last_action_t;

typedef struct mod_gl1_utils_joysticks_info_s
{
  int supported;
  int driver_nb_joysticks;
  SDL_Joystick *sdl_joysticks[LW6GUI_NB_JOYSTICKS];
}
mod_gl1_utils_joysticks_info_t;

typedef struct mod_gl1_utils_mouse_state_s
{
  int shown;
}
mod_gl1_utils_mouse_state_t;

typedef struct mod_gl1_utils_timer_s
{
  int64_t bitmap_refresh;
}
mod_gl1_utils_timer_t;

typedef struct _mod_gl1_utils_smoothers_s
{
  lw6gui_smoother_t global_zoom;
  lw6gui_smoother_t map_center_x;
  lw6gui_smoother_t map_center_y;
  //lw6gui_smoother_t drawable_x1;
  //lw6gui_smoother_t drawable_y1;
  //lw6gui_smoother_t drawable_x2;
  //lw6gui_smoother_t drawable_y2;
}
_mod_gl1_utils_smoothers_t;

typedef struct _mod_gl1_utils_smoothed_s
{
  float global_zoom;
  float map_center_x;
  float map_center_y;
  lw6gui_zone_t drawable;
}
_mod_gl1_utils_smoothed_t;

typedef struct mod_gl1_utils_icon_s
{
  mod_gl1_utils_bitmap_t *bitmap;
}
mod_gl1_utils_icon_t;

typedef struct mod_gl1_utils_counter_s
{
  int nb_frames;
}
mod_gl1_utils_counter_t;

typedef struct mod_gl1_utils_cache_s
{
  lw6map_color_set_t color_set;
}
mod_gl1_utils_cache_t;

typedef struct mod_gl1_utils_context_s
{
  _lw6gfx_sdl_context_t sdl_context;
  mod_gl1_utils_path_t path;
  mod_gl1_utils_mem_counter_t surface_counter;
  mod_gl1_utils_mem_counter_t texture_counter;
  mod_gl1_utils_caps_t caps;
  mod_gl1_utils_render_param_t render_param;
  mod_gl1_utils_texture_data_t texture_data;
  mod_gl1_utils_font_data_t font_data;
  mod_gl1_utils_const_data_t const_data;
  mod_gl1_utils_texture_1x1_t textures_1x1;
  mod_gl1_utils_menucache_array_t menucache_array;
  mod_gl1_utils_team_color_map_t team_color_map;
  mod_gl1_utils_last_action_t last_action;
  mod_gl1_utils_joysticks_info_t joysticks_info;
  mod_gl1_utils_mouse_state_t mouse_state;
  _mod_gl1_utils_smoothers_t smoothers;
  _mod_gl1_utils_smoothed_t smoothed;
  mod_gl1_utils_icon_t icon;
  mod_gl1_utils_counter_t counter;
  lw6sys_hash_t *bitmap_hash;
  mod_gl1_utils_cache_t cache;
  mod_gl1_utils_timer_t timer;
}
mod_gl1_utils_context_t;

typedef struct mod_gl1_utils_multiline_text_callback_data_s
{
  mod_gl1_utils_context_t *utils_context;
  TTF_Font *font;
  SDL_Color sdl_color_bg;
  SDL_Color sdl_color_fg;
  lw6sys_whd_t shape;
  lw6sys_xyz_t pos;
  SDL_Surface *target;
} mod_gl1_utils_multiline_text_callback_data_t;

/* gl1-utils-bitmap.c */
extern mod_gl1_utils_bitmap_t *mod_gl1_utils_bitmap_new (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int width, int height,
							 const char *desc);
extern mod_gl1_utils_bitmap_t *mod_gl1_utils_bitmap_load (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *filename);
extern mod_gl1_utils_bitmap_t *mod_gl1_utils_surface2bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface,
							     const char *desc);
extern void mod_gl1_utils_bitmap_free (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern int mod_gl1_utils_bitmap_colorize (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					  utils_context, mod_gl1_utils_bitmap_t * bitmap, int colorize, const lw6map_color_couple_t * color);
extern int mod_gl1_utils_bitmap_set_wrap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap,
					  GLint wrap);
extern int mod_gl1_utils_bitmap_set_filter (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap,
					    GLint filter);
extern int mod_gl1_utils_bitmap_set_texture_coords (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap,
						    float s1, float t1, float s2, float t2);
extern void mod_gl1_utils_bitmap_clear_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern void mod_gl1_utils_bitmap_clear_texture_now (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern int mod_gl1_utils_bitmap_refresh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern int mod_gl1_utils_bitmap_refresh_force (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern void mod_gl1_utils_bitmap_bind_no_gen (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern int mod_gl1_utils_bitmap_bind (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern int mod_gl1_utils_bitmap_display (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap, float x1,
					 float y1, float x2, float y2);
extern int mod_gl1_utils_bitmap_update_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);

/* gl1-utils-bitmaparray.c */
extern int mod_gl1_utils_bitmap_array_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					    utils_context, mod_gl1_utils_bitmap_array_t * bitmap_array, int w, int h, int tile_size, int border_size);
extern int
mod_gl1_utils_bitmap_array_init_from_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					      utils_context,
					      mod_gl1_utils_bitmap_array_t *
					      bitmap_array, SDL_Surface * surface, int tile_size, int border_size, int x_polarity, int y_polarity);
extern int mod_gl1_utils_bitmap_array_init_from_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						     utils_context,
						     mod_gl1_utils_bitmap_array_t
						     * bitmap_array,
						     const lw6map_level_t * level, int tile_size, int border_size, int x_polarity, int y_polarity);
extern int mod_gl1_utils_bitmap_array_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					      mod_gl1_utils_bitmap_array_t * bitmap_array, GLint wrap, GLint filter);
extern void mod_gl1_utils_bitmap_array_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					      mod_gl1_utils_bitmap_array_t * bitmap_array);
extern void mod_gl1_utils_schedule_delete_bitmap_array (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
							mod_gl1_utils_bitmap_array_t * bitmap_array);
extern int mod_gl1_utils_bitmap_array_set (lw6sys_context_t * sys_context, mod_gl1_utils_bitmap_array_t * bitmap_array, int i,
					   mod_gl1_utils_bitmap_t * bitmap);
extern mod_gl1_utils_bitmap_t *mod_gl1_utils_bitmap_array_get (lw6sys_context_t * sys_context, mod_gl1_utils_bitmap_array_t * bitmap_array, int i);

/* gl1-utils-bitmaphash.c */
extern lw6sys_hash_t *mod_gl1_utils_bitmap_hash_init (lw6sys_context_t * sys_context);
extern void mod_gl1_utils_bitmap_hash_quit (lw6sys_context_t * sys_context, lw6sys_hash_t * bitmap_hash);
extern int mod_gl1_utils_bitmap_hash_register (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern int mod_gl1_utils_bitmap_hash_unregister (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap);
extern int mod_gl1_utils_bitmap_hash_refresh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern int mod_gl1_utils_bitmap_hash_dump2disk (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int force);

/* gl1-utils-cache.c */
int mod_gl1_utils_cache_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look);

/* gl1-utils-capture.c */
extern SDL_Surface *mod_gl1_utils_capture2surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern int mod_gl1_utils_capture2disk (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-color.c */
extern SDL_Color mod_gl1_utils_color_8_to_sdl (lw6sys_context_t * sys_context, lw6sys_color_8_t color_8);
extern SDL_Color mod_gl1_utils_color_f_to_sdl (lw6sys_context_t * sys_context, const lw6sys_color_f_t * color_f);
extern void mod_gl1_utils_update_team_color_map (lw6sys_context_t * sys_context, mod_gl1_utils_team_color_map_t * team_color_map,
						 const lw6map_style_t * map_style);
extern int mod_gl1_utils_team_color_map_is_same (lw6sys_context_t * sys_context, const mod_gl1_utils_team_color_map_t * team_color_map_a,
						 const mod_gl1_utils_team_color_map_t * team_color_map_b);
extern void mod_gl1_utils_force_color32_alpha (lw6sys_context_t * sys_context, Uint32 * color, float alpha);
extern Uint32 mod_gl1_utils_get_shaded_color_for_fighter_f (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int team_id,
							    float health);
inline static Uint32
mod_gl1_utils_get_shaded_color_for_fighter (mod_gl1_utils_context_t * utils_context, int team_id, int health)
{
  return utils_context->team_color_map.team_colors[team_id][(health * MOD_GL1_SHADES_FOR_FIGHTERS_SCALE) / LW6MAP_MAX_FIGHTER_HEALTH];
}

// gl1-utils-colorize.c
extern SDL_Surface *mod_gl1_utils_colorize_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						    utils_context, SDL_Surface * surface, const lw6map_color_couple_t * color, int has_alpha);

/* gl1-utils-consts.c */
extern int mod_gl1_utils_load_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context);
extern void mod_gl1_utils_unload_consts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context);

/* gl1-utils-cursor.c */
extern mod_gl1_utils_bitmap_t
  * mod_gl1_utils_cursor_create_fg_bg (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
				       const lw6map_level_t * level);
extern mod_gl1_utils_bitmap_t *mod_gl1_utils_cursor_create_color (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
								  const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_cursor_t * cursor);

/* gl1-utils-data.c */
extern int mod_gl1_utils_load_fonts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context);
extern void mod_gl1_utils_unload_fonts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context);

extern SDL_Surface *mod_gl1_utils_load_image (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *filename);
extern void mod_gl1_utils_unload_image (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * image);
extern int mod_gl1_utils_load_fonts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_unload_fonts (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern int mod_gl1_utils_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-debug.c */
extern void mod_gl1_utils_display_zones (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
					 const lw6ker_game_struct_t * game_struct);
extern void mod_gl1_utils_display_gradient (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
					    const lw6ker_game_state_t * game_state, int team_id, int layer_id);

/* gl1-utils-display.c */
extern void mod_gl1_utils_display_quad_begin (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_display_quad_end (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_display_quad_do (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					   utils_context, GLuint texture,
					   float x1, float y1, float x2, float y2, float texture_x1, float texture_y1, float texture_x2, float texture_y2);
extern void mod_gl1_utils_display_texture_full (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context, GLuint texture, float x1, float y1, float x2, float y2, int texture_w, int texture_h);
extern void mod_gl1_utils_display_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					   utils_context, GLuint texture,
					   float x1, float y1, float x2, float y2, float texture_x1, float texture_y1, float texture_x2, float texture_y2);
extern void mod_gl1_utils_display_texture_with_filter (lw6sys_context_t * sys_context, mod_gl1_utils_context_t
						       * utils_context,
						       GLuint texture,
						       float x1, float y1,
						       float x2, float y2,
						       float texture_x1, float texture_y1, float texture_x2, float texture_y2, int pixelize);

/* gl1-utils-draw.c */
extern void mod_gl1_utils_draw_rectfill (lw6sys_context_t * sys_context, SDL_Surface * surface, int x1, int y1, int x2, int y2, Uint32 color);
extern void mod_gl1_utils_draw_set_alpha_for_color (lw6sys_context_t * sys_context, SDL_Surface * surface, float alpha, Uint32 color);

/* gl1-utils-event.c */
extern void mod_gl1_utils_pump_event_callback (lw6sys_context_t * sys_context, void *data, void *event);

/* gl1-utils-font.c */
extern int mod_gl1_utils_get_system_text_width (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *text);
extern int mod_gl1_utils_get_system_text_height (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *text);
extern mod_gl1_utils_bitmap_t
  * mod_gl1_utils_multiline_text_write (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					utils_context, TTF_Font * font,
					const char *text,
					const lw6map_color_couple_t * color,
					float alpha_bg, int max_width, int max_height, int border_size, int margin_size, int reformat_width);
extern SDL_Surface *mod_gl1_utils_blended_text_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context, TTF_Font * font, SDL_Color color,
							const char *text);
extern void mod_gl1_utils_draw_system_text (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
					    const char *text, int x, int y);
extern void mod_gl1_utils_draw_system_text_top_left (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
						     const char **text_list);
extern void mod_gl1_utils_draw_system_text_top_right (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
						      const char **text_list);
extern void mod_gl1_utils_draw_system_text_bottom_left (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
							const char **text_list);
extern void mod_gl1_utils_draw_system_text_bottom_right (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
							 const char **text_list);


/* gl1-utils-game.c */
extern void mod_gl1_utils_update_game_bitmap_raw (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						  utils_context,
						  mod_gl1_utils_bitmap_t *
						  bitmap, const lw6ker_game_state_t * game_state, const lw6map_color_couple_t * map_color, int invert_y);
extern void mod_gl1_utils_update_game_bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					      utils_context,
					      mod_gl1_utils_bitmap_t * bitmap, const lw6ker_game_state_t * game_state, const lw6gui_look_t * look);
extern void mod_gl1_utils_update_game_bitmap_array (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						    utils_context,
						    mod_gl1_utils_bitmap_array_t
						    * bitmap_array, const lw6ker_game_state_t * game_state, const lw6gui_look_t * look);

/* gl1-utils-gradient.c */
extern SDL_Surface
  * mod_gl1_utils_create_gradient_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6ker_game_state_t * game_state,
					   int team_id, int layer_id);

/* gl1-utils-icon.c */
extern int mod_gl1_utils_icon_set (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_icon_unset (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-info.c */
extern void mod_gl1_utils_display_log (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
				       const char **log_list);
extern void mod_gl1_utils_display_fps (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look, float fps);
extern void mod_gl1_utils_display_mps (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look, float mps,
				       int target_mps);
extern void mod_gl1_utils_display_url (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look, const char *url);

/* gl1-utils-joystick.c */
extern int mod_gl1_utils_joystick_init (lw6sys_context_t * sys_context, mod_gl1_utils_joysticks_info_t * joysticks_info);
extern void mod_gl1_utils_joystick_quit (lw6sys_context_t * sys_context, mod_gl1_utils_joysticks_info_t * joysticks_info);

/* gl1-utils-light.c */
extern void mod_gl1_utils_main_light (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-menucache.c */
extern int mod_gl1_utils_store_button_in_menucache (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						    utils_context,
						    const lw6gui_look_t * look, const lw6gui_menuitem_t * menuitem, mod_gl1_utils_bitmap_t * bitmap);
extern mod_gl1_utils_bitmap_t
  * mod_gl1_utils_get_button_from_menucache (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * context, const lw6gui_look_t * look,
					     const lw6gui_menuitem_t * menuitem);
extern void mod_gl1_utils_clear_menucache (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-mode.c */
extern int mod_gl1_utils_set_video_mode (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, lw6gui_video_mode_t * video_mode);
extern int mod_gl1_utils_resize_video_mode (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, lw6gui_video_mode_t * video_mode);
extern int mod_gl1_utils_get_video_mode (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, lw6gui_video_mode_t * video_mode);
extern int mod_gl1_utils_sync_viewport (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern int mod_gl1_utils_sync_mode (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int force);
extern int mod_gl1_utils_set_resize_callback (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					      lw6gui_resize_callback_func_t resize_callback);
extern void mod_gl1_utils_call_resize_callback (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-mouse.c */
extern void mod_gl1_utils_show_mouse (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int state, int force);

/* gl1-utils-path.c */
extern int mod_gl1_utils_path_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int argc, const char *argv[]);
extern int mod_gl1_utils_path_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_path_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-pixel.c */
extern Uint32 mod_gl1_utils_getpixel (lw6sys_context_t * sys_context, SDL_Surface * surface, int x, int y);
extern void mod_gl1_utils_putpixel (lw6sys_context_t * sys_context, SDL_Surface * surface, int x, int y, Uint32 pixel);
static inline void
mod_gl1_utils_putpixel_4_bytes_per_pixel (SDL_Surface * surface, int x, int y, Uint32 pixel)
{
  *((Uint32 *) ((Uint8 *) surface->pixels + (y * surface->pitch) + (x << 2))) = pixel;
}

/* gl1-utils-render.c */
extern void mod_gl1_utils_prepare_buffer (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look);
extern void mod_gl1_utils_swap_buffers (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_set_render_mode_2d (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_set_render_mode_2d_blend (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_set_render_mode_3d_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_set_render_mode_3d_menu (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_set_render_mode_3d_select (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int x, int y);
extern void mod_gl1_utils_set_render_mode_3d_feedback (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-resolution.c */
extern int mod_gl1_utils_get_fullscreen_modes (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, lw6gui_fullscreen_modes_t * modes);
extern void mod_gl1_utils_find_closest_resolution (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						   utils_context, int *closest_width, int *closest_height, int wished_width, int wished_height);

/* gl1-utils-shadedtext.c */
extern mod_gl1_utils_shaded_text_t
  * mod_gl1_utils_shaded_text_new (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, TTF_Font * font, const char *text,
				   const lw6map_color_couple_t * color);
extern int mod_gl1_utils_shaded_text_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					     mod_gl1_utils_shaded_text_t * shaded_text, const char *text, const lw6map_color_couple_t * color);
extern int mod_gl1_utils_shaded_text_display (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					      mod_gl1_utils_shaded_text_t * shaded_text, float x1, float y1, float x2, float y2, float dw, float dh);
extern void mod_gl1_utils_shaded_text_free (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					    mod_gl1_utils_shaded_text_t * shaded_text);

/* gl1-utils-smoothers.c */
extern void mod_gl1_utils_smoothers_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_smoothers_reset_drawable (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_smoothers_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-surface.c */
extern SDL_Surface *mod_gl1_utils_create_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int w, int h);
extern void mod_gl1_utils_delete_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface);
extern void mod_gl1_utils_clear_surface_with_color (lw6sys_context_t * sys_context, SDL_Surface * surface, SDL_Color color);
extern void mod_gl1_utils_clear_surface (lw6sys_context_t * sys_context, SDL_Surface * surface);
extern SDL_Surface *mod_gl1_utils_map2surface_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level,
						    int x, int y, int w, int h);
extern SDL_Surface *mod_gl1_utils_map2surface_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int w,
						  int h);
extern SDL_Surface *mod_gl1_utils_map2surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level);

/* gl1-utils-texture.c */
extern float mod_gl1_utils_texture_scale (lw6sys_context_t * sys_context, int size);
extern GLuint mod_gl1_utils_surface2texture_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int x, int y,
						  int w, int h, int mipmap);
extern GLuint mod_gl1_utils_surface2texture_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int w, int h,
						int mipmap);
extern GLuint mod_gl1_utils_surface2texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, int mipmap);
extern GLuint mod_gl1_utils_map2texture_xywh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int x,
					      int y, int w, int h);
extern GLuint mod_gl1_utils_map2texture_wh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level, int w,
					    int h);
extern GLuint mod_gl1_utils_map2texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6map_level_t * level);
extern void mod_gl1_utils_delete_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, GLuint texture);
extern void mod_gl1_utils_schedule_delete_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, GLuint texture);
extern void mod_gl1_utils_delete_scheduled_textures (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_texture_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, GLuint texture, SDL_Surface * surface);

/* gl1-utils-texture1x1.c */
extern mod_gl1_utils_bitmap_t *mod_gl1_utils_texture_1x1_color2bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
								       lw6sys_color_8_t color_8);
extern int mod_gl1_utils_texture_1x1_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look);
extern void mod_gl1_utils_texture_1x1_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-timer.c */
extern int64_t mod_gl1_utils_timer_get_bitmap_refresh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_timer_set_bitmap_refresh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-utils-viewport.c */
extern void mod_gl1_utils_viewport_drawable_max (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_utils_viewport_screen_to_gl (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, float *gl_x, float *gl_y,
						 int screen_x, int screen_y);
extern void mod_gl1_utils_viewport_gl_to_screen (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int *screen_x, int *screen_y,
						 float gl_x, float gl_y);

/* gl1-utils-zones.c */
extern SDL_Surface *mod_gl1_utils_create_zones_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
							const lw6ker_game_struct_t * map_struct);

#endif
