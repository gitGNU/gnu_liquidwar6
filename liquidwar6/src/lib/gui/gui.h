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

#ifndef LIQUIDWAR6GUI_H
#define LIQUIDWAR6GUI_H

#include "../hlp/hlp.h"
#include "../cfg/cfg.h"
#include "../sys/sys.h"
#include "../map/map.h"
#include "../ldr/ldr.h"

#define LW6GUI_NB_KEYS 512
#define LW6GUI_NB_JOYSTICKS 2
#define LW6GUI_JOYSTICK1_ID 0
#define LW6GUI_JOYSTICK2_ID 1
#define LW6GUI_NB_JOYSTICK_BUTTONS 4

#define LW6GUI_DEFAULT_LOW_WIDTH 320
#define LW6GUI_DEFAULT_LOW_HEIGHT 200
#define LW6GUI_DEFAULT_STANDARD_WIDTH 800
#define LW6GUI_DEFAULT_STANDARD_HEIGHT 600
#define LW6GUI_DEFAULT_HIGH_WIDTH 1920
#define LW6GUI_DEFAULT_HIGH_HEIGHT 1080
#if LW6_MS_WINDOWS
// windowed mode still buggy on MS-Windows
#define LW6GUI_DEFAULT_FULLSCREEN 1
#else
#define LW6GUI_DEFAULT_FULLSCREEN 0
#endif

#define LW6GUI_DISPLAY_SPLASH     0x00000001
#define LW6GUI_DISPLAY_BACKGROUND 0x00000002
#define LW6GUI_DISPLAY_PREVIEW    0x00000004
#define LW6GUI_DISPLAY_MAP        0x00000008
#define LW6GUI_DISPLAY_FIGHTERS   0x00000010
#define LW6GUI_DISPLAY_CURSORS    0x00000020
#define LW6GUI_DISPLAY_HUD        0x00000040
#define LW6GUI_DISPLAY_SCORE      0x00000080
#define LW6GUI_DISPLAY_MENU       0x00000100
#define LW6GUI_DISPLAY_PROGRESS   0x00000200
#define LW6GUI_DISPLAY_LOG        0x00000400
#define LW6GUI_DISPLAY_FPS        0x00000800
#define LW6GUI_DISPLAY_MPS        0x00001000
#define LW6GUI_DISPLAY_URL        0x00002000
#define LW6GUI_DISPLAY_MOUSE      0x00004000

#define LW6GUI_DISPLAY_DEBUG_ZONES    0x00010000
#define LW6GUI_DISPLAY_DEBUG_GRADIENT 0x00020000

#define LW6GUI_GFX_QUALITY_LOW      0
#define LW6GUI_GFX_QUALITY_STANDARD 1
#define LW6GUI_GFX_QUALITY_HIGH     2

typedef struct lw6gui_video_mode_s
{
  int width;
  int height;
  int fullscreen;
} lw6gui_video_mode_t;

typedef struct lw6gui_fullscreen_modes_s
{
  lw6gui_video_mode_t low;
  lw6gui_video_mode_t standard;
  lw6gui_video_mode_t high;
}
lw6gui_fullscreen_modes_t;

typedef struct lw6gui_repeat_settings_s
{
  int delay;
  int interval;
} lw6gui_repeat_settings_t;

typedef void (*lw6gui_resize_callback_func_t) (lw6gui_video_mode_t *
					       video_mode);

typedef struct lw6gui_look_s
{
  u_int32_t id;
  float dynamic_zoom;
  int gfx_quality;
  lw6map_style_t style;
}
lw6gui_look_t;

typedef struct lw6gui_keypress_s
{
  int keysym;
  int unicode;
  char *label;
}
lw6gui_keypress_t;

typedef struct lw6gui_move_pad_s
{
  int up;
  int down;
  int left;
  int right;
}
lw6gui_move_pad_t;

typedef struct lw6gui_button_s
{
  int is_pressed;
  int press_queue;
  int64_t last_press;
  int64_t last_repeat;
}
lw6gui_button_t;

typedef struct lw6gui_keyboard_s
{
  lw6gui_button_t key_up;
  lw6gui_button_t key_down;
  lw6gui_button_t key_left;
  lw6gui_button_t key_right;
  lw6gui_button_t key_enter;
  lw6gui_button_t key_esc;
  lw6gui_button_t key_pgup;
  lw6gui_button_t key_pgdown;
  lw6sys_list_t *queue;
  lw6gui_button_t keys_state[LW6GUI_NB_KEYS];
}
lw6gui_keyboard_t;

typedef struct lw6gui_mouse_s
{
  int x;
  int y;
  int moved;
  int64_t last_moved;
  int map_x;
  int map_y;
  int menu_position;
  int menu_scroll;
  int menu_esc;
  lw6gui_button_t button_left;
  lw6gui_button_t button_right;
  lw6gui_button_t wheel_up;
  lw6gui_button_t wheel_down;
}
lw6gui_mouse_t;

typedef struct lw6gui_joystick_s
{
  lw6gui_button_t button_up;
  lw6gui_button_t button_down;
  lw6gui_button_t button_left;
  lw6gui_button_t button_right;
  lw6gui_button_t button_a;
  lw6gui_button_t button_b;
  lw6gui_button_t button_c;
  lw6gui_button_t button_d;
}
lw6gui_joystick_t;

typedef struct lw6gui_input_s
{
  int need_sync;
  lw6gui_keyboard_t keyboard;
  lw6gui_mouse_t mouse;
  lw6gui_joystick_t joysticks[LW6GUI_NB_JOYSTICKS];
}
lw6gui_input_t;

typedef struct lw6gui_menuitem_s
{
  u_int32_t id;
  char *label;
  int value;
  int enabled;
  int selected;
  int colored;
  int last_change;
  int last_select;
  int last_unselect;
}
lw6gui_menuitem_t;

typedef struct lw6gui_menu_s
{
  u_int32_t id;
  char *title;
  int nb_items;
  lw6gui_menuitem_t *esc_item;
  lw6gui_menuitem_t **items;
  int selected_item;
  int first_item_displayed;
  int nb_items_displayed;
  int order_of_selected_on_display;
  int allow_scroll;
}
lw6gui_menu_t;

typedef struct lw6gui_smoother_s
{
  float s1;
  float y1;
  float y2;
  int64_t t1;
  int duration;
} lw6gui_smoother_t;

typedef struct lw6gui_zone_s
{
  float x1;
  float y1;
  float x2;
  float y2;
  float w;
  float h;
} lw6gui_zone_t;

typedef struct lw6gui_viewport_s
{
  lw6sys_whd_t map_shape;
  lw6sys_whd_t screen_shape;
  float center_x;
  float center_y;
  int x_polarity;
  int y_polarity;
  int x_wrap;
  int y_wrap;
  lw6gui_zone_t drawable;
  lw6gui_zone_t map_main;
  lw6gui_zone_t map_main_clipped;
  lw6gui_zone_t map_visible;

} lw6gui_viewport_t;

/* gui-button.c */
extern void lw6gui_button_register_down (lw6gui_button_t * button,
					 int64_t timestamp);
extern void lw6gui_button_register_up (lw6gui_button_t * button);
extern int lw6gui_button_is_pressed (lw6gui_button_t * button);
extern int lw6gui_button_pop_press (lw6gui_button_t * button);
extern void lw6gui_button_update_repeat (lw6gui_button_t * button,
					 lw6gui_repeat_settings_t *
					 repeat_settings, int64_t timestamp);
extern int lw6gui_button_sync (lw6gui_button_t * dst, lw6gui_button_t * src);

/* gui-coord.c */
extern int lw6gui_coord_calc_xy (float *dst_x, float *dst_y, float dst_x0,
				 float dst_y0, float dst_w, float dst_h,
				 float src_x, float src_y, float src_x0,
				 float src_y0, float src_w, float src_h);
extern int lw6gui_coord_viewport_setup (lw6gui_viewport_t * viewport);

/* gui-input.c */
extern int lw6gui_input_init (lw6gui_input_t * input);
extern void lw6gui_input_quit (lw6gui_input_t * input);
extern lw6gui_input_t *lw6gui_input_new ();
extern void lw6gui_input_free (lw6gui_input_t * input);
extern int lw6gui_input_reset (lw6gui_input_t * input);
extern void lw6gui_input_update_repeat (lw6gui_input_t * input,
					lw6gui_repeat_settings_t *
					repeat_settings, int64_t timestamp);
extern void lw6gui_input_register_change (lw6gui_input_t * input);
extern int lw6gui_input_need_sync (lw6gui_input_t * input);
extern int lw6gui_input_sync (lw6gui_input_t * dst, lw6gui_input_t * src);

/* gui-joystick.c */
extern int lw6gui_joystick_check_index (int i);
extern void lw6gui_joystick_update_axis_x (lw6gui_joystick_t * joystick,
					   int x, int limit,
					   int64_t timestamp);
extern void lw6gui_joystick_update_axis_y (lw6gui_joystick_t * joystick,
					   int y, int limit,
					   int64_t timestamp);
extern void lw6gui_joystick_update_repeat (lw6gui_joystick_t * joystick,
					   lw6gui_repeat_settings_t *
					   repeat_settings,
					   int64_t timestamp);
extern int lw6gui_joystick_sync (lw6gui_joystick_t * dst,
				 lw6gui_joystick_t * src);
extern void lw6gui_joystick_get_move_pad (lw6gui_joystick_t * joystick,
					  lw6gui_move_pad_t * move_pad);

/* gui-keyboard.c */
extern int lw6gui_keyboard_check_keysym (int keysym);
extern lw6gui_keypress_t *lw6gui_keyboard_pop_keypress (lw6gui_keyboard_t *
							keyboard);
extern int lw6gui_keyboard_is_pressed (lw6gui_keyboard_t * keyboard,
				       int keysym);
extern int lw6gui_keyboard_register_key_down (lw6gui_keyboard_t * keyboard,
					      int keysym, int unicode,
					      char *label, int64_t timestamp);
extern int lw6gui_keyboard_register_key_up (lw6gui_keyboard_t * keyboard,
					    int keysym);
extern void lw6gui_keyboard_update_repeat (lw6gui_keyboard_t * keyboard,
					   lw6gui_repeat_settings_t *
					   repeat_settings,
					   int64_t timestamp);
extern int lw6gui_keyboard_sync (lw6gui_keyboard_t * dst,
				 lw6gui_keyboard_t * src);
extern void lw6gui_keyboard_get_move_pad (lw6gui_keyboard_t * keyboard,
					  lw6gui_move_pad_t * move_pad);

/* gui-keypress.c */
extern lw6gui_keypress_t *lw6gui_keypress_new (int keysym, int unicode,
					       char *label);
extern void lw6gui_keypress_free (lw6gui_keypress_t * keypress);
extern char *lw6gui_keypress_repr (lw6gui_keypress_t * keypress);

/*
 * In look.c
 */
extern lw6gui_look_t *lw6gui_look_new (lw6map_style_t * map_style);
extern void lw6gui_look_free (lw6gui_look_t * look);
extern int lw6gui_look_memory_footprint (lw6gui_look_t * look);
extern char *lw6gui_look_repr (lw6gui_look_t * look);
extern int lw6gui_look_set (lw6gui_look_t * look, char *key, char *value);
extern char *lw6gui_look_get (lw6gui_look_t * look, char *key);
extern int lw6gui_look_is_same (lw6gui_look_t * look_a,
				lw6gui_look_t * look_b);
extern lw6gui_look_t *lw6gui_look_dup (lw6gui_look_t * look);
extern void lw6gui_look_fix (lw6gui_look_t * look);
extern int lw6gui_look_zoom_in (lw6gui_look_t * look, float zoom_step);
extern int lw6gui_look_zoom_out (lw6gui_look_t * look, float zoom_step);

/* gui-menuitem.c */
extern lw6gui_menuitem_t *lw6gui_menuitem_new (char *label, int value,
					       int enabled, int selected,
					       int colored);
extern void lw6gui_menuitem_free (lw6gui_menuitem_t * menuitem);

extern int lw6gui_menuitem_memory_footprint (lw6gui_menuitem_t * menuitem);
extern char *lw6gui_menuitem_repr (lw6gui_menuitem_t * menuitem);
extern void lw6gui_menuitem_set_label (lw6gui_menuitem_t * menuitem,
				       char *label, int64_t now);
extern void lw6gui_menuitem_set_value (lw6gui_menuitem_t * menuitem,
				       int value, int64_t now);
extern void lw6gui_menuitem_select (lw6gui_menuitem_t * menuitem, int state,
				    int64_t now);
extern void lw6gui_menuitem_enable (lw6gui_menuitem_t * menuitem, int state,
				    int64_t now);
extern u_int32_t lw6gui_menuitem_checksum (lw6gui_menuitem_t * menuitem,
					   lw6gui_look_t * look);
extern int lw6gui_menuitem_is_same (lw6gui_menuitem_t * menuitem_a,
				    lw6gui_menuitem_t * menuitem_b);
extern lw6gui_menuitem_t *lw6gui_menuitem_dup (lw6gui_menuitem_t * menuitem);
extern int lw6gui_menuitem_sync (lw6gui_menuitem_t * dst,
				 lw6gui_menuitem_t * src);

/* gui-menu.c */
extern lw6gui_menu_t *lw6gui_menu_new (char *title, char *esc,
				       int enable_esc);
extern void lw6gui_menu_free (lw6gui_menu_t * menu);
extern int lw6gui_menu_memory_footprint (lw6gui_menu_t * menu);
extern char *lw6gui_menu_repr (lw6gui_menu_t * menu);
extern void lw6gui_menu_set_title (lw6gui_menu_t * menu, char *title);
extern lw6gui_menuitem_t *lw6gui_menu_get_item (lw6gui_menu_t * menu,
						int position);
extern int lw6gui_menu_select (lw6gui_menu_t * menu, int position,
			       int allow_scroll, int64_t now);
extern void lw6gui_menu_select_esc (lw6gui_menu_t * menu, int state,
				    int64_t now);
extern void lw6gui_menu_enable_esc (lw6gui_menu_t * menu, int state,
				    int64_t now);
extern int lw6gui_menu_scroll_up (lw6gui_menu_t * menu);
extern int lw6gui_menu_scroll_down (lw6gui_menu_t * menu);
extern void lw6gui_menu_center (lw6gui_menu_t * menu, int position,
				int max_displayed_items);
extern int lw6gui_menu_insert (lw6gui_menu_t * menu,
			       lw6gui_menuitem_t * menuitem, int position,
			       int64_t now);
extern int lw6gui_menu_append (lw6gui_menu_t * menu,
			       lw6gui_menuitem_t * menuitem, int64_t now);
extern int lw6gui_menu_remove (lw6gui_menu_t * menu, int position,
			       int64_t now);
extern void lw6gui_menu_update_display_range (lw6gui_menu_t * menu,
					      int max_displayed_items);
extern int lw6gui_menu_insert_for_id_use (lw6gui_menu_t * menu, char *label,
					  int value, int enabled,
					  int selected, int colored,
					  int position, int64_t now);
extern int lw6gui_menu_append_for_id_use (lw6gui_menu_t * menu, char *label,
					  int value, int enabled,
					  int selected, int colored,
					  int64_t now);
extern int lw6gui_menu_remove_using_id (lw6gui_menu_t * menu, int menuitem_id,
					int64_t now);
extern void lw6gui_menu_sync_using_id (lw6gui_menu_t * menu, int menuitem_id,
				       char *label, int value, int enabled,
				       int selected, int colored,
				       int64_t now);
extern int lw6gui_menu_is_same (lw6gui_menu_t * menu_a,
				lw6gui_menu_t * menu_b);
extern lw6gui_menu_t *lw6gui_menu_dup (lw6gui_menu_t * menu);
extern int lw6gui_menu_sync (lw6gui_menu_t * dst, lw6gui_menu_t * src);

/* gui-mouse.c */
extern void lw6gui_mouse_register_move (lw6gui_mouse_t * mouse, int x, int y,
					int64_t timestamp);
extern int lw6gui_mouse_poll_move (lw6gui_mouse_t * mouse, int *x, int *y);
extern void lw6gui_mouse_update_repeat (lw6gui_mouse_t * mouse,
					lw6gui_repeat_settings_t *
					repeat_settings, int64_t timestamp);
extern int lw6gui_mouse_sync (lw6gui_mouse_t * dst, lw6gui_mouse_t * src);

/* gui-smoother.c */
extern void lw6gui_smoother_init (lw6gui_smoother_t * smoother, float value,
				  int duration);
extern void lw6gui_smoother_immediate_force (lw6gui_smoother_t * smoother,
					     float value);
extern void lw6gui_smoother_set_target (lw6gui_smoother_t * smoother,
					float value, int64_t now);
extern float lw6gui_smoother_get_value (lw6gui_smoother_t * smoother,
					int64_t now);

/* gui-test.c */
extern int lw6gui_test (int mode);

/* gui-videomode.c */
extern int lw6gui_video_mode_find_closest (lw6gui_video_mode_t * closest,
					   lw6gui_video_mode_t * wished,
					   lw6sys_list_t * available);
extern int lw6gui_video_mode_is_same (lw6gui_video_mode_t * mode_a,
				      lw6gui_video_mode_t * mode_b);
extern int lw6gui_video_mode_sync_ratio (lw6gui_video_mode_t * dst,
					 lw6gui_video_mode_t * src);

/* gui-viewport.c */
extern int lw6gui_viewport_init (lw6gui_viewport_t * viewport, int screen_w,
				 int screen_h, float drawable_x1,
				 float drawable_y1, float drawable_x2,
				 float drawable_y2, float center_x,
				 float center_y, int map_w, int map_h,
				 int x_polarity, int y_polarity, int x_wrap,
				 int y_wrap, int keep_ratio,
				 float global_zoom);
extern void lw6gui_viewport_map_to_screen (lw6gui_viewport_t * viewport,
					   float *screen_x, float *screen_y,
					   float map_x, float map_y,
					   int clip);
extern void lw6gui_viewport_screen_to_map (lw6gui_viewport_t * viewport,
					   float *map_x, float *map_y,
					   float screen_x, float screen_y,
					   int wrap);

/* gui-zone.c */
extern void lw6gui_zone_init_x1y1x2y2 (lw6gui_zone_t * zone, float x1,
				       float y1, float x2, float y2);
extern void lw6gui_zone_init_xywh (lw6gui_zone_t * zone, float x, float y,
				   float w, float h);
extern void lw6gui_zone_clip (lw6gui_zone_t * dst, lw6gui_zone_t * src,
			      lw6gui_zone_t * clip);

#endif
