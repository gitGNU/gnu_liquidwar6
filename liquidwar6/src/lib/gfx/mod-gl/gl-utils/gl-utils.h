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

#ifndef LIQUIDWAR6GFX_MOD_GL_UTILS_H
#define LIQUIDWAR6GFX_MOD_GL_UTILS_H

#include "../../gfx.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE GL_CLAMP
#endif

#define MOD_GL_UTILS_MENU_TEXTURE_W 512
#define MOD_GL_UTILS_MENU_TEXTURE_H 128

/*
 * We keep 64 menu bitmaps in memory, to avoid redrawing them
 * at each display frame.
 */
#define MOD_GL_UTILS_MENUCACHE_ARRAY_SIZE 64

/*
 * We keep 32 textures in memory, this is to avoid transfering
 * bitmaps from SDL structures to the OpenGL world at each frame
 * display.
 */
#define MOD_GL_UTILS_TEXTURECACHE_ARRAY_SIZE 32


/*
 * OpenGL should handle at least this texture size. Theorically
 * we should support any MAX_TEXTURE_SIZE value, but in practise,
 * most recent drivers support 512x512, and the "liquidwarish" 
 * answer to this problem is -> use another backend.
 */
#define MOD_GL_UTILS_REQUIRED_TEXTURE_SIZE 512

/*
 * How many different colors for armies.
 */
#define MOD_GL_SHADES_FOR_FIGHTERS_SCALE 64

typedef struct mod_gl_utils_bitmap_s
{
  u_int32_t id;
  char *filename;
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
  u_int64_t last_refresh;
} mod_gl_utils_bitmap_t;

typedef struct mod_gl_utils_shaded_text_s
{
  TTF_Font *font;
  char *text;
  lw6map_color_couple_t color;
  mod_gl_utils_bitmap_t *bg;
  mod_gl_utils_bitmap_t *fg;
  int texture_w;
  int texture_h;
} mod_gl_utils_shaded_text_t;

typedef struct mod_gl_utils_path_s
{
  char *data_dir;
  char *capture_dir;
}
mod_gl_utils_path_t;

typedef struct mod_gl_utils_mem_counter_s
{
  int new_counter;
  int delete_counter;
}
mod_gl_utils_mem_counter_t;

typedef struct mod_gl_utils_caps_s
{
  GLint max_texture_size;
  int bpp;
}
mod_gl_utils_caps_t;

typedef enum mod_gl_utils_render_mode_e
{
  MOD_GL_UTILS_RENDER_NONE = 0,
  MOD_GL_UTILS_RENDER_2D = 1,
  MOD_GL_UTILS_RENDER_2D_BLEND = 2,
  MOD_GL_UTILS_RENDER_3D_MAP = 3,
  MOD_GL_UTILS_RENDER_3D_MENU = 4,
  MOD_GL_UTILS_RENDER_3D_PICK = 5
}
mod_gl_utils_render_mode_t;

typedef struct mod_gl_utils_render_param_s
{
  mod_gl_utils_render_mode_t mode;
  int gfx_quality;
} mod_gl_utils_render_param_t;

typedef struct mod_gl_utils_rect_array_s
{
  int n_w;
  int n_h;
  int *x0;
  int *y0;
  int *w;
  int *h;
  float *screen_x;
  float *screen_y;
}
mod_gl_utils_rect_array_t;

typedef struct mod_gl_utils_bitmap_array_s
{
  mod_gl_utils_rect_array_t layout;
  mod_gl_utils_bitmap_t **bitmaps;
}
mod_gl_utils_bitmap_array_t;

typedef struct mod_gl_utils_texture_data_s
{
  lw6sys_list_t *to_delete;
}
mod_gl_utils_texture_data_t;

typedef struct mod_gl_utils_font_data_s
{
  TTF_Font *menu;
  TTF_Font *hud;
  TTF_Font *cursor;
  TTF_Font *system;
}
mod_gl_utils_font_data_t;

typedef struct mod_gl_utils_menucache_item_s
{
  char *key;
  mod_gl_utils_bitmap_t *bitmap;
}
mod_gl_utils_menucache_item_t;

typedef struct mod_gl_utils_menucache_array_s
{
  int last_stored_item;
    mod_gl_utils_menucache_item_t
    item_array[MOD_GL_UTILS_MENUCACHE_ARRAY_SIZE];
}
mod_gl_utils_menucache_array_t;

typedef struct mod_gl_utils_const_data_s
{
  /*
   * Set by the utils module itself
   */
  float warp_x;
  float warp_y;
  float mode_sleep;
  float quit_sleep;
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
  int keysym1_up;
  int keysym2_up;
  int keysym3_up;
  int keysym4_up;
  int keysym1_down;
  int keysym2_down;
  int keysym3_down;
  int keysym4_down;
  int keysym1_left;
  int keysym2_left;
  int keysym3_left;
  int keysym4_left;
  int keysym1_right;
  int keysym2_right;
  int keysym3_right;
  int keysym4_right;
  int keysym1_enter;
  int keysym2_enter;
  int keysym1_esc;
  int keysym2_esc;
  int keysym1_pgup;
  int keysym2_pgup;
  int keysym1_pgdown;
  int keysym2_pgdown;
  int keysym_quit;
  int joystick_limit;
  int joystick1_index;
  int joystick1_button_a_index;
  int joystick1_button_b_index;
  int joystick1_button_c_index;
  int joystick1_button_d_index;
  int joystick2_index;
  int joystick2_button_a_index;
  int joystick2_button_b_index;
  int joystick2_button_c_index;
  int joystick2_button_d_index;
  int joystick_all_buttons;
  int smoother_global_zoom_duration;
  int smoother_center_duration;
  int smoother_drawable_duration;
}
mod_gl_utils_const_data_t;

typedef struct mod_gl_utils_texture_1x1_s
{
  lw6map_color_set_t color_set;
  mod_gl_utils_bitmap_t *color_base_fg;
  mod_gl_utils_bitmap_t *color_base_bg;
  mod_gl_utils_bitmap_t *color_alternate_fg;
  mod_gl_utils_bitmap_t *color_alternate_bg;
  mod_gl_utils_bitmap_t *background_color_root_fg;
  mod_gl_utils_bitmap_t *background_color_root_bg;
  mod_gl_utils_bitmap_t *background_color_stuff_fg;
  mod_gl_utils_bitmap_t *background_color_stuff_bg;
  mod_gl_utils_bitmap_t *hud_color_frame_fg;
  mod_gl_utils_bitmap_t *hud_color_frame_bg;
  mod_gl_utils_bitmap_t *hud_color_text_fg;
  mod_gl_utils_bitmap_t *hud_color_text_bg;
  mod_gl_utils_bitmap_t *menu_color_default_fg;
  mod_gl_utils_bitmap_t *menu_color_default_bg;
  mod_gl_utils_bitmap_t *menu_color_selected_fg;
  mod_gl_utils_bitmap_t *menu_color_selected_bg;
  mod_gl_utils_bitmap_t *menu_color_disabled_fg;
  mod_gl_utils_bitmap_t *menu_color_disabled_bg;
  mod_gl_utils_bitmap_t *view_color_cursor_fg;
  mod_gl_utils_bitmap_t *view_color_cursor_bg;
  mod_gl_utils_bitmap_t *view_color_map_fg;
  mod_gl_utils_bitmap_t *view_color_map_bg;
  mod_gl_utils_bitmap_t *system_color_fg;
  mod_gl_utils_bitmap_t *system_color_bg;
  mod_gl_utils_bitmap_t *team_color_dead;
  mod_gl_utils_bitmap_t *team_colors[LW6MAP_NB_TEAM_COLORS];
}
mod_gl_utils_texture_1x1_t;

typedef struct mod_gl_utils_team_color_map_s
{
  lw6sys_color_f_t team_colors_f[LW6MAP_MAX_NB_TEAMS];
  Uint32 team_colors[LW6MAP_MAX_NB_TEAMS][MOD_GL_SHADES_FOR_FIGHTERS_SCALE +
					  1];
}
mod_gl_utils_team_color_map_t;

typedef struct mod_gl_utils_last_action_s
{
  int game_bitmap_array_update_id;
  int game_bitmap_array_update_rounds;
}
mod_gl_utils_last_action_t;

typedef struct mod_gl_utils_joysticks_info_s
{
  int supported;
  int driver_nb_joysticks;
  SDL_Joystick *sdl_joysticks[LW6GUI_NB_JOYSTICKS];
}
mod_gl_utils_joysticks_info_t;

typedef struct mod_gl_utils_mouse_state_s
{
  int shown;
}
mod_gl_utils_mouse_state_t;

typedef struct mod_gl_utils_timer_s
{
  int64_t timestamp;
  int64_t uptime;
  int32_t cycle;
  int64_t bitmap_refresh;
}
mod_gl_utils_timer_t;

typedef struct _mod_gl_utils_smoothers_s
{
  lw6gui_smoother_t global_zoom;
  lw6gui_smoother_t center_x;
  lw6gui_smoother_t center_y;
  lw6gui_smoother_t drawable_x1;
  lw6gui_smoother_t drawable_y1;
  lw6gui_smoother_t drawable_x2;
  lw6gui_smoother_t drawable_y2;
}
_mod_gl_utils_smoothers_t;

typedef struct _mod_gl_utils_smoothed_s
{
  float global_zoom;
  float center_x;
  float center_y;
  lw6gui_zone_t drawable;
}
_mod_gl_utils_smoothed_t;

typedef struct mod_gl_utils_icon_s
{
  mod_gl_utils_bitmap_t *bitmap;
}
mod_gl_utils_icon_t;

typedef struct mod_gl_utils_counter_s
{
  int nb_frames;
}
mod_gl_utils_counter_t;

typedef struct mod_gl_utils_context_s
{
  lw6gui_resize_callback_func_t resize_callback;
  mod_gl_utils_path_t path;
  mod_gl_utils_mem_counter_t surface_counter;
  mod_gl_utils_mem_counter_t texture_counter;
  mod_gl_utils_caps_t caps;
  lw6gui_video_mode_t video_mode;
  mod_gl_utils_render_param_t render_param;
  mod_gl_utils_texture_data_t texture_data;
  mod_gl_utils_font_data_t font_data;
  mod_gl_utils_const_data_t const_data;
  mod_gl_utils_texture_1x1_t textures_1x1;
  mod_gl_utils_menucache_array_t menucache_array;
  mod_gl_utils_team_color_map_t team_color_map;
  mod_gl_utils_last_action_t last_action;
  mod_gl_utils_joysticks_info_t joysticks_info;
  mod_gl_utils_mouse_state_t mouse_state;
  mod_gl_utils_timer_t timer;
  lw6gui_input_t input;
  _mod_gl_utils_smoothers_t smoothers;
  _mod_gl_utils_smoothed_t smoothed;
  mod_gl_utils_icon_t icon;
  mod_gl_utils_counter_t counter;
}
mod_gl_utils_context_t;

/*
 * In bitmap.c
 */
extern mod_gl_utils_bitmap_t *mod_gl_utils_bitmap_new (mod_gl_utils_context_t
						       * utils_context,
						       int width, int height);
extern mod_gl_utils_bitmap_t *mod_gl_utils_bitmap_load (mod_gl_utils_context_t
							* utils_context,
							char *filename);
extern mod_gl_utils_bitmap_t
  * mod_gl_utils_surface2bitmap (mod_gl_utils_context_t * utils_context,
				 SDL_Surface * surface);
extern void mod_gl_utils_bitmap_free (mod_gl_utils_context_t * utils_context,
				      mod_gl_utils_bitmap_t * bitmap);
extern int mod_gl_utils_bitmap_colorize (mod_gl_utils_context_t *
					 utils_context,
					 mod_gl_utils_bitmap_t * bitmap,
					 int colorize,
					 lw6map_color_couple_t * color);
extern int mod_gl_utils_bitmap_set_wrap (mod_gl_utils_context_t *
					 utils_context,
					 mod_gl_utils_bitmap_t * bitmap,
					 GLint wrap);
extern int mod_gl_utils_bitmap_set_filter (mod_gl_utils_context_t *
					   utils_context,
					   mod_gl_utils_bitmap_t * bitmap,
					   GLint filter);
extern int mod_gl_utils_bitmap_set_texture_coords (mod_gl_utils_context_t *
						   utils_context,
						   mod_gl_utils_bitmap_t *
						   bitmap, float s1, float t1,
						   float s2, float t2);
extern void mod_gl_utils_bitmap_clear_texture (mod_gl_utils_context_t *
					       utils_context,
					       mod_gl_utils_bitmap_t *
					       bitmap);
extern void mod_gl_utils_bitmap_clear_texture_now (mod_gl_utils_context_t *
						   utils_context,
						   mod_gl_utils_bitmap_t *
						   bitmap);
extern int mod_gl_utils_bitmap_refresh (mod_gl_utils_context_t *
					utils_context,
					mod_gl_utils_bitmap_t * bitmap);
extern int mod_gl_utils_bitmap_force_refresh (mod_gl_utils_context_t *
					      utils_context,
					      mod_gl_utils_bitmap_t * bitmap);
extern int mod_gl_utils_bitmap_bind (mod_gl_utils_context_t * utils_context,
				     mod_gl_utils_bitmap_t * bitmap);
extern int mod_gl_utils_bitmap_display (mod_gl_utils_context_t *
					utils_context,
					mod_gl_utils_bitmap_t * bitmap,
					float x1, float y1, float x2,
					float y2);
extern int mod_gl_utils_bitmap_update_texture (mod_gl_utils_context_t *
					       utils_context,
					       mod_gl_utils_bitmap_t *
					       bitmap);

/*
 * In bitmaparray.c
 */
extern int mod_gl_utils_bitmap_array_init (mod_gl_utils_context_t *
					   utils_context,
					   mod_gl_utils_bitmap_array_t *
					   bitmap_array, int w, int h,
					   int tile_size);
extern int mod_gl_utils_bitmap_array_init_from_surface (mod_gl_utils_context_t
							* utils_context,
							mod_gl_utils_bitmap_array_t
							* bitmap_array,
							SDL_Surface * surface,
							int tile_size);
extern int mod_gl_utils_bitmap_array_init_from_map (mod_gl_utils_context_t *
						    utils_context,
						    mod_gl_utils_bitmap_array_t
						    * bitmap_array,
						    lw6map_level_t * level,
						    int tile_size);
extern int mod_gl_utils_bitmap_array_update (mod_gl_utils_context_t *
					     utils_context,
					     mod_gl_utils_bitmap_array_t *
					     bitmap_array, GLint wrap,
					     GLint filter);
extern void mod_gl_utils_bitmap_array_clear (mod_gl_utils_context_t *
					     utils_context,
					     mod_gl_utils_bitmap_array_t *
					     bitmap_array);
extern int mod_gl_utils_bitmap_array_set (mod_gl_utils_bitmap_array_t *
					  bitmap_array, int n_x, int n_y,
					  mod_gl_utils_bitmap_t * bitmap);
extern mod_gl_utils_bitmap_t
  * mod_gl_utils_bitmap_array_get (mod_gl_utils_bitmap_array_t * bitmap_array,
				   int n_x, int n_y);

/*
 * In capture.c
 */
extern SDL_Surface *mod_gl_utils_capture2surface (mod_gl_utils_context_t *
						  utils_context);
extern int mod_gl_utils_capture2disk (mod_gl_utils_context_t * utils_context);

/*
 * In color.c
 */
extern SDL_Color mod_gl_utils_color_8_to_sdl (lw6sys_color_8_t color_8);
extern SDL_Color mod_gl_utils_color_f_to_sdl (lw6sys_color_f_t * color_f);
extern void mod_gl_utils_update_team_color_map (mod_gl_utils_team_color_map_t
						* team_color_map,
						lw6map_style_t * map_style);
extern int mod_gl_utils_team_color_map_is_same (mod_gl_utils_team_color_map_t
						* team_color_map_a,
						mod_gl_utils_team_color_map_t
						* team_color_map_b);
extern void mod_gl_utils_force_color32_alpha (Uint32 * color, float alpha);
extern Uint32
mod_gl_utils_get_shaded_color_for_fighter_f (mod_gl_utils_context_t *
					     utils_context, int team_id,
					     float health);
inline static Uint32
mod_gl_utils_get_shaded_color_for_fighter (mod_gl_utils_context_t *
					   utils_context, int team_id,
					   int health)
{
  return utils_context->team_color_map.team_colors[team_id][(health *
							     MOD_GL_SHADES_FOR_FIGHTERS_SCALE)
							    /
							    LW6MAP_MAX_FIGHTER_HEALTH];
}

// gl-utils-colorize.c
extern SDL_Surface *mod_gl_utils_colorize_surface (mod_gl_utils_context_t *
						   utils_context,
						   SDL_Surface * surface,
						   lw6map_color_couple_t *
						   color, int has_alpha);

/*
 * In consts.c
 */
extern int mod_gl_utils_load_consts (mod_gl_utils_context_t * context);
extern void mod_gl_utils_unload_consts (mod_gl_utils_context_t * context);

/*
 * In cursor.c
 */
extern mod_gl_utils_bitmap_t
  * mod_gl_utils_cursor_create_fg_bg (mod_gl_utils_context_t * utils_context,
				      lw6gui_look_t * look,
				      lw6map_level_t * level);
extern mod_gl_utils_bitmap_t
  * mod_gl_utils_cursor_create_color (mod_gl_utils_context_t * utils_context,
				      lw6gui_look_t * look,
				      lw6map_level_t * level,
				      lw6ker_cursor_t * cursor);

/*
 * In data.c
 */
extern int mod_gl_utils_load_fonts (mod_gl_utils_context_t * context);
extern void mod_gl_utils_unload_fonts (mod_gl_utils_context_t * context);

extern SDL_Surface *mod_gl_utils_load_image (mod_gl_utils_context_t *
					     utils_context, char *filename);
extern void mod_gl_utils_unload_image (mod_gl_utils_context_t * utils_context,
				       SDL_Surface * image);
extern int mod_gl_utils_load_fonts (mod_gl_utils_context_t * utils_context);
extern void mod_gl_utils_unload_fonts (mod_gl_utils_context_t *
				       utils_context);
extern int mod_gl_utils_load_data (mod_gl_utils_context_t * utils_context);
extern void mod_gl_utils_unload_data (mod_gl_utils_context_t * utils_context);

/*
 * In debug.c
 */
extern void mod_gl_utils_display_zones (mod_gl_utils_context_t *
					utils_context, lw6gui_look_t * look,
					lw6ker_game_struct_t * game_struct);
extern void mod_gl_utils_display_gradient (mod_gl_utils_context_t *
					   utils_context,
					   lw6gui_look_t * look,
					   lw6ker_game_state_t * game_state,
					   int team_id, int layer_id);

/*
 * In display.c
 */
extern void mod_gl_utils_display_texture_full (mod_gl_utils_context_t *
					       utils_context, GLuint texture,
					       float x1, float y1, float x2,
					       float y2, int texture_w,
					       int texture_h);
extern void mod_gl_utils_display_texture (mod_gl_utils_context_t *
					  utils_context, GLuint texture,
					  float x1, float y1, float x2,
					  float y2, float texture_x1,
					  float texture_y1, float texture_x2,
					  float texture_y2);
extern void mod_gl_utils_display_texture_with_filter (mod_gl_utils_context_t *
						      utils_context,
						      GLuint texture,
						      float x1, float y1,
						      float x2, float y2,
						      float texture_x1,
						      float texture_y1,
						      float texture_x2,
						      float texture_y2,
						      int pixelize);

/*
 * In draw.c
 */
extern void mod_gl_utils_draw_rectfill (SDL_Surface * surface, int x1, int y1,
					int x2, int y2, Uint32 color);

/*
 * In event.c
 */
extern lw6gui_input_t *mod_gl_utils_pump_events (mod_gl_utils_context_t *
						 utils_context);

/*
 * In font.c
 */
extern int mod_gl_utils_get_system_text_width (mod_gl_utils_context_t *
					       utils_context, char *text);
extern int mod_gl_utils_get_system_text_height (mod_gl_utils_context_t *
						utils_context, char *text);
extern SDL_Surface *mod_gl_utils_blended_text_surface (mod_gl_utils_context_t
						       * context,
						       TTF_Font * font,
						       SDL_Color color,
						       char *text);
extern void mod_gl_utils_draw_system_text (mod_gl_utils_context_t *
					   utils_context,
					   lw6gui_look_t * look, char *text,
					   int x, int y);
extern void mod_gl_utils_draw_system_text_top_left (mod_gl_utils_context_t *
						    utils_context,
						    lw6gui_look_t * look,
						    char **text_list);
extern void mod_gl_utils_draw_system_text_top_right (mod_gl_utils_context_t *
						     utils_context,
						     lw6gui_look_t * look,
						     char **text_list);
extern void mod_gl_utils_draw_system_text_bottom_left (mod_gl_utils_context_t
						       * utils_context,
						       lw6gui_look_t * look,
						       char **text_list);
extern void mod_gl_utils_draw_system_text_bottom_right (mod_gl_utils_context_t
							* utils_context,
							lw6gui_look_t * look,
							char **text_list);


/*
 * In game.c
 */
extern void mod_gl_utils_update_game_bitmap_raw (mod_gl_utils_context_t *
						 utils_context,
						 mod_gl_utils_bitmap_t *
						 bitmap,
						 lw6ker_game_state_t *
						 game_state,
						 lw6map_color_couple_t *
						 map_color, int invert_y);
extern void mod_gl_utils_update_game_bitmap (mod_gl_utils_context_t *
					     utils_context,
					     mod_gl_utils_bitmap_t * bitmap,
					     lw6ker_game_state_t * game_state,
					     lw6gui_look_t * look);
extern void mod_gl_utils_update_game_bitmap_array (mod_gl_utils_context_t *
						   utils_context,
						   mod_gl_utils_bitmap_array_t
						   * bitmap_array,
						   lw6ker_game_state_t *
						   game_state,
						   lw6gui_look_t * look);

/*
 * In gradient.c
 */
extern SDL_Surface
  * mod_gl_utils_create_gradient_surface (mod_gl_utils_context_t *
					  utils_context,
					  lw6ker_game_state_t * game_state,
					  int team_id, int layer_id);

/*
 * In icon.c
 */
extern int mod_gl_utils_icon_set (mod_gl_utils_context_t * utils_context);
extern void mod_gl_utils_icon_unset (mod_gl_utils_context_t * utils_context);

/*
 * In info.c
 */
extern void mod_gl_utils_display_log (mod_gl_utils_context_t * utils_context,
				      lw6gui_look_t * look, char **log_list);
extern void mod_gl_utils_display_fps (mod_gl_utils_context_t * utils_context,
				      lw6gui_look_t * look, float fps);
extern void mod_gl_utils_display_mps (mod_gl_utils_context_t * utils_context,
				      lw6gui_look_t * look, float mps,
				      int target_mps);
extern void mod_gl_utils_display_url (mod_gl_utils_context_t * utils_context,
				      lw6gui_look_t * look, char *url);

/*
 * In joystick.c
 */
extern int mod_gl_utils_joystick_init (mod_gl_utils_joysticks_info_t *
				       joysticks_info);
extern void mod_gl_utils_joystick_quit (mod_gl_utils_joysticks_info_t *
					joysticks_info);

/*
 * In light.c
 */
extern void mod_gl_utils_main_light (mod_gl_utils_context_t * utils_context);

/*
 * In menucache.c
 */
extern int mod_gl_utils_store_button_in_menucache (mod_gl_utils_context_t *
						   utils_context,
						   lw6gui_look_t * look,
						   lw6gui_menuitem_t *
						   menuitem,
						   mod_gl_utils_bitmap_t *
						   bitmap);
extern mod_gl_utils_bitmap_t
  * mod_gl_utils_get_button_from_menucache (mod_gl_utils_context_t * context,
					    lw6gui_look_t * look,
					    lw6gui_menuitem_t * menuitem);
extern void mod_gl_utils_clear_menucache (mod_gl_utils_context_t *
					  utils_context);

/*
 * In mode.c
 */
extern int mod_gl_utils_set_video_mode (mod_gl_utils_context_t *
					utils_context,
					lw6gui_video_mode_t * video_mode);
extern int mod_gl_utils_resize_video_mode (mod_gl_utils_context_t *
					   utils_context,
					   lw6gui_video_mode_t * video_mode);
extern int mod_gl_utils_get_video_mode (mod_gl_utils_context_t *
					utils_context,
					lw6gui_video_mode_t * video_mode);
extern int mod_gl_utils_sync_viewport (mod_gl_utils_context_t *
				       utils_context);
extern int mod_gl_utils_sync_mode (mod_gl_utils_context_t * utils_context,
				   int force);
extern int mod_gl_utils_set_resize_callback (mod_gl_utils_context_t *
					     utils_context,
					     lw6gui_resize_callback_func_t
					     resize_callback);
extern void mod_gl_utils_call_resize_callback (mod_gl_utils_context_t *
					       utils_context);

/*
 * In mouse.c
 */
extern void mod_gl_utils_show_mouse (mod_gl_utils_context_t * utils_context,
				     int state, int force);

/*
 * In path.c
 */
extern int mod_gl_utils_path_init (mod_gl_utils_context_t *
				   utils_context, int argc, char *argv[]);
extern void mod_gl_utils_path_quit (mod_gl_utils_context_t * utils_context);

/*
 * In pixel.c
 */
extern Uint32 mod_gl_utils_getpixel (SDL_Surface * surface, int x, int y);
extern void mod_gl_utils_putpixel (SDL_Surface * surface, int x, int y,
				   Uint32 pixel);
static inline void
mod_gl_utils_putpixel_4_bytes_per_pixel (SDL_Surface * surface, int x, int y,
					 Uint32 pixel)
{
  *((Uint32 *) ((Uint8 *) surface->pixels + (y * surface->pitch) +
		(x << 2))) = pixel;
}

/*
 * In rectarray.c
 */
extern int mod_gl_utils_rect_array_init (mod_gl_utils_context_t *
					 utils_context,
					 mod_gl_utils_rect_array_t *
					 rect_array, int w, int h,
					 int tile_size);
extern void mod_gl_utils_rect_array_clear (mod_gl_utils_context_t *
					   utils_context,
					   mod_gl_utils_rect_array_t *
					   rect_array);

/*
 * In render.c
 */
extern void mod_gl_utils_prepare_buffer (mod_gl_utils_context_t *
					 utils_context, lw6gui_look_t * look);
extern void mod_gl_utils_swap_buffers (mod_gl_utils_context_t *
				       utils_context);
extern void mod_gl_utils_set_render_mode_2d (mod_gl_utils_context_t *
					     utils_context);
extern void mod_gl_utils_set_render_mode_2d_blend (mod_gl_utils_context_t *
						   utils_context);
extern void mod_gl_utils_set_render_mode_3d_map (mod_gl_utils_context_t *
						 utils_context);
extern void mod_gl_utils_set_render_mode_3d_menu (mod_gl_utils_context_t *
						  utils_context);
extern void mod_gl_utils_set_render_mode_3d_pick (mod_gl_utils_context_t *
						  utils_context, int x,
						  int y);

/*
 * In resolution.c
 */
extern int mod_gl_utils_get_fullscreen_modes (mod_gl_utils_context_t *
					      utils_context,
					      lw6gui_fullscreen_modes_t *
					      modes);
extern void mod_gl_utils_find_closest_resolution (mod_gl_utils_context_t *
						  utils_context,
						  int *closest_width,
						  int *closest_height,
						  int wished_width,
						  int wished_height);

/*
 * In shaded_text.c
 */
extern mod_gl_utils_shaded_text_t
  * mod_gl_utils_shaded_text_new (mod_gl_utils_context_t * utils_context,
				  TTF_Font * font, char *text,
				  lw6map_color_couple_t * color);
extern int mod_gl_utils_shaded_text_update (mod_gl_utils_context_t *
					    utils_context,
					    mod_gl_utils_shaded_text_t *
					    shaded_text, char *text,
					    lw6map_color_couple_t * color);
extern int mod_gl_utils_shaded_text_display (mod_gl_utils_context_t *
					     utils_context,
					     mod_gl_utils_shaded_text_t *
					     shaded_text, float x1, float y1,
					     float x2, float y2, float dw,
					     float dh);
extern void mod_gl_utils_shaded_text_free (mod_gl_utils_context_t *
					   utils_context,
					   mod_gl_utils_shaded_text_t *
					   shaded_text);

/*
 * In smoothers.c
 */
extern void mod_gl_utils_smoothers_init (mod_gl_utils_context_t *
					 utils_context);
extern void mod_gl_utils_smoothers_reset_drawable (mod_gl_utils_context_t *
						   utils_context);
extern void mod_gl_utils_smoothers_update (mod_gl_utils_context_t *
					   utils_context);

/*
 * In surface.c
 */
extern SDL_Surface *mod_gl_utils_create_surface (mod_gl_utils_context_t *
						 utils_context, int w, int h);
extern void mod_gl_utils_delete_surface (mod_gl_utils_context_t *
					 utils_context,
					 SDL_Surface * surface);
extern void mod_gl_utils_clear_surface_with_color (SDL_Surface * surface,
						   SDL_Color color);
extern void mod_gl_utils_clear_surface (SDL_Surface * surface);
extern SDL_Surface *mod_gl_utils_map2surface_xywh (mod_gl_utils_context_t *
						   utils_context,
						   lw6map_level_t * level,
						   int x, int y, int w,
						   int h);
extern SDL_Surface *mod_gl_utils_map2surface_wh (mod_gl_utils_context_t *
						 utils_context,
						 lw6map_level_t * level,
						 int w, int h);
extern SDL_Surface *mod_gl_utils_map2surface (mod_gl_utils_context_t *
					      utils_context,
					      lw6map_level_t * level);

/*
 * In texture.c
 */
extern int mod_gl_utils_power_of_two_le (int size);
extern int mod_gl_utils_power_of_two_ge (int size);
extern float mod_gl_utils_texture_scale (int size);
extern GLuint mod_gl_utils_surface2texture_xywh (mod_gl_utils_context_t *
						 utils_context,
						 SDL_Surface * surface, int x,
						 int y, int w, int h,
						 int mipmap);
extern GLuint mod_gl_utils_surface2texture_wh (mod_gl_utils_context_t *
					       utils_context,
					       SDL_Surface * surface, int w,
					       int h, int mipmap);
extern GLuint mod_gl_utils_surface2texture (mod_gl_utils_context_t *
					    utils_context,
					    SDL_Surface * surface,
					    int mipmap);
extern GLuint mod_gl_utils_map2texture_xywh (mod_gl_utils_context_t *
					     utils_context,
					     lw6map_level_t * level, int x,
					     int y, int w, int h);
extern GLuint mod_gl_utils_map2texture_wh (mod_gl_utils_context_t *
					   utils_context,
					   lw6map_level_t * level, int w,
					   int h);
extern GLuint mod_gl_utils_map2texture (mod_gl_utils_context_t *
					utils_context,
					lw6map_level_t * level);
extern void mod_gl_utils_delete_texture (mod_gl_utils_context_t *
					 utils_context, GLuint texture);
extern void mod_gl_utils_schedule_delete_texture (mod_gl_utils_context_t *
						  utils_context,
						  GLuint texture);
extern void mod_gl_utils_delete_scheduled_textures (mod_gl_utils_context_t *
						    utils_context);
extern void mod_gl_utils_texture_update (mod_gl_utils_context_t *
					 utils_context, GLuint texture,
					 SDL_Surface * surface);

/*
 * In texture1x1.c
 */
extern mod_gl_utils_bitmap_t
  * mod_gl_utils_texture_1x1_color2bitmap (mod_gl_utils_context_t *
					   utils_context,
					   lw6sys_color_8_t color_8);
extern int mod_gl_utils_texture_1x1_update (mod_gl_utils_context_t *
					    utils_context,
					    lw6gui_look_t * look);
extern void mod_gl_utils_texture_1x1_clear (mod_gl_utils_context_t *
					    utils_context);

/*
 * In timer.c
 */
extern void mod_gl_utils_timer_update (mod_gl_utils_context_t *
				       utils_context);
extern int64_t mod_gl_utils_timer_get_timestamp (mod_gl_utils_context_t *
						 utils_context);
extern int64_t mod_gl_utils_timer_get_uptime (mod_gl_utils_context_t *
					      utils_context);
extern int32_t mod_gl_utils_timer_get_cycle (mod_gl_utils_context_t *
					     utils_context);
extern int64_t mod_gl_utils_timer_get_bitmap_refresh (mod_gl_utils_context_t *
						      utils_context);
extern void mod_gl_utils_timer_set_bitmap_refresh (mod_gl_utils_context_t *
						   utils_context);

/*
 * In viewport.c
 */
extern void mod_gl_utils_viewport_drawable_max (mod_gl_utils_context_t *
						utils_context);

/*
 * In zones.c
 */
extern SDL_Surface *mod_gl_utils_create_zones_surface (mod_gl_utils_context_t
						       * utils_context,
						       lw6ker_game_struct_t *
						       map_struct);

#endif
