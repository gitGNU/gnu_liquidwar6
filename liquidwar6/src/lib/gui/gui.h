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

#ifndef LIQUIDWAR6GUI_H
#define LIQUIDWAR6GUI_H

#include "../hlp/hlp.h"
#include "../cfg/cfg.h"
#include "../sys/sys.h"
#include "../map/map.h"

#define LW6GUI_NB_KEYS 512
#define LW6GUI_NB_JOYSTICKS 2
#define LW6GUI_JOYSTICK1_ID 0
#define LW6GUI_JOYSTICK2_ID 1
#define LW6GUI_NB_JOYSTICK_BUTTONS 6
#define LW6GUI_JOYSTICK_BUTTON_ID_A 0
#define LW6GUI_JOYSTICK_BUTTON_ID_B 1
#define LW6GUI_JOYSTICK_BUTTON_ID_C 2
#define LW6GUI_JOYSTICK_BUTTON_ID_D 3
#define LW6GUI_JOYSTICK_BUTTON_ID_E 4
#define LW6GUI_JOYSTICK_BUTTON_ID_F 5

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
#define LW6GUI_DISPLAY_META       0x00000200
#define LW6GUI_DISPLAY_PROGRESS   0x00000400
#define LW6GUI_DISPLAY_LOG        0x00000800
#define LW6GUI_DISPLAY_FPS        0x00001000
#define LW6GUI_DISPLAY_MPS        0x00002000
#define LW6GUI_DISPLAY_URL        0x00004000
#define LW6GUI_DISPLAY_MOUSE      0x00008000

#define LW6GUI_DISPLAY_DEBUG_ZONES    0x00010000
#define LW6GUI_DISPLAY_DEBUG_GRADIENT 0x00020000

#define LW6GUI_GFX_QUALITY_LOW      0
#define LW6GUI_GFX_QUALITY_STANDARD 1
#define LW6GUI_GFX_QUALITY_HIGH     2

#define LW6GUI_MIN_TILE_SIZE 16
#define LW6GUI_MAX_TILE_SIZE 4096
#define LW6GUI_MIN_BORDER_SIZE 1
#define LW6GUI_MAX_BORDER_SIZE 256

/**
 * Contains the parameters for a video mode, regardless
 * of driver used.
 */
typedef struct lw6gui_video_mode_s
{
  /// Width, in pixels.
  int width;
  /// Height, in pixels.
  int height;
  /// 1 for fullscreen mode, 0 for windowed mode.
  int fullscreen;
} lw6gui_video_mode_t;

/**
 * Contains information about available fullscreen modes.
 */
typedef struct lw6gui_fullscreen_modes_s
{
  /// Low resolution mode.
  lw6gui_video_mode_t low;
  /// Standard resolution mode.
  lw6gui_video_mode_t standard;
  /// High resolution mode.
  lw6gui_video_mode_t high;
}
lw6gui_fullscreen_modes_t;

/**
 * Parameters used to handle repeat. This is used both
 * by keys and buttons (joystick buttons and mouse buttons).
 */
typedef struct lw6gui_repeat_settings_s
{
  /**
   * Delay, in milliseconds, after which a given key/button
   * enters repeat mode.
   */
  int delay;
  /**
   * Interval, in milliseconds, between two key/button press events
   * in repeat mode.
   */
  int interval;
  /**
   * If pressed twice within this delay (in milliseconds) then a
   * double-click event is generated.
   */
  int double_click_delay;
  /**
   * After this delay (milliseconds) any key will be considered be
   * unpressed, that is, it will be released automatically. This is
   * usefull when the input library (depends on the gfx backend) does
   * not send proper "key up" events. The workarround is to automatically
   * consider the key is released after some time. Usually, this
   * would typically be set just below the repeat delay.
   */
  int auto_release_delay;
} lw6gui_repeat_settings_t;

typedef void (*lw6gui_resize_callback_func_t) (lw6sys_context_t * sys_context, lw6gui_video_mode_t * video_mode);

/**
 * The look structure contains everything the renderer needs
 * to skin the display. This is where one specifies the color set,
 * dynamic zoom effect, and possibly other things.
 */
typedef struct lw6gui_look_s
{
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /**
   * Dynamic zoom, this is multiplicated by the map zoom, and gives
   * the global zoom, the one finally used.
   */
  float dynamic_zoom;
  /**
   * Overall graphics quality, the higher the better, will trigger
   * various parameters, depending on the renderer.
   */
  int gfx_quality;
  /**
   * A style structure which will override the one from the map,
   * depending on the local options (config file, environnement,
   * command-line options).
   */
  lw6map_style_t style;
}
lw6gui_look_t;

/**
 * Keypress information, contains more than just a keycode but
 * also meta/readable informations about it.
 */
typedef struct lw6gui_keypress_s
{
  /**
   * The keysym, note that this is implementation specific.
   * In practice, SDL uniformizes this, but there's no garantee
   * all graphics engine are SDL based, so don't rely on this
   * too much outside the graphics backend.
   */
  int keysym;
  /// Unicode code for this letter/key.
  int unicode;
  /**
   * Readable label for the key, typically usable in a
   * "choose keyboard settings" interface.
   */
  char *label;
}
lw6gui_keypress_t;

/**
 * Standard interface for joypad-like interfaces, can
 * also be used to map keyboard arrows.
 */
typedef struct lw6gui_move_pad_s
{
  /// Up button (boolean).
  int up;
  /// Down button (boolean).
  int down;
  /// Left button (boolean).
  int left;
  /// Right button (boolean).
  int right;
}
lw6gui_move_pad_t;

/**
 * Used to store a complete button state, along
 * with repeat informations, queues. It might be
 * overkill for basic cases, having different types
 * of buttons (union?) for different cases might
 * be a good idea.
 */
typedef struct lw6gui_button_s
{
  /// Wether button is pressed, 1 means pressed, 0 unpressed.
  int is_pressed;
  /**
   * Each time the button is pressed, this increases, each
   * times one "pops" a press from it, it's decreased. This
   * allows for button buffering, as events might take some
   * time to go through the pipeline given the heavily
   * multithreaded nature of the dsp/gfx couple.
   */
  int press_queue;
  /**
   * Simple-click counter, as opposed to double-click.
   * A "simple-click" is validated when one has pressed
   * the button, and then waiting long enough to discard
   * the possibility to double-click. This is not really buffered,
   * queue will ignore simple-clicks if one is already
   * buffered.
   */
  int simple_click_queue;
  /**
   * Double-click counter. This is not really buffered,
   * queue will ignore double-clicks if one is already
   * buffered.
   */
  int double_click_queue;
  /**
   * Triple-click counter. This is not really buffered,
   * queue will ignore triple-clicks if one is already
   * buffered.
   */
  int triple_click_queue;
  /// Timestamp of last key press.
  int64_t last_press;
  /// Timestamp of last key repeat.
  int64_t last_repeat;
  /**
   * Used to handle multiple-clicks, this is the timestamp
   * of the click "2 clicks ago".
   */
  int64_t double_click_t1;
  /**
   * Used to handle multiple-clicks, this is the timestamp
   * of the click just before the last click.
   */
  int64_t double_click_t2;
  /**
   * Used to handle multiple-clicks, this is the timestamp
   * of the last click.
   */
  int64_t double_click_t3;
}
lw6gui_button_t;

/**
 * Stores a complete keyboard state.
 */
typedef struct lw6gui_keyboard_s
{
  /// Wether auto_release mode is set.
  int auto_release_enabled;
  /**
   * State of keyboard up arrow.
   * This can be the combination of several keys,
   * for instance the numeric pad up arrow,
   * and the corresponding arrow pad key.
   */
  lw6gui_button_t arrow_up;
  /**
   * State of keyboard down arrow.
   * This can be the combination of several keys,
   * for instance the numeric pad down arrow,
   * and the corresponding arrow pad key.
   */
  lw6gui_button_t arrow_down;
  /**
   * State of keyboard left arrow.
   * This can be the combination of several keys,
   * for instance the numeric pad left arrow,
   * and the corresponding arrow pad key.
   */
  lw6gui_button_t arrow_left;
  /**
   * State of keyboard right arrow.
   * This can be the combination of several keys,
   * for instance the numeric pad right arrow,
   * and the corresponding arrow pad key.
   */
  lw6gui_button_t arrow_right;
  /**
   * State of keyboard ENTER key.
   * This can be the combination of several keys,
   * for instance both the numeric pad ENTER and the
   * standard, default one.
   */
  lw6gui_button_t key_enter;
  /**
   * State of keyboard ESC key.
   * This can be the combination of several keys,
   * for instance both the standard ESC key and another key.
   */
  lw6gui_button_t key_esc;
  /**
   * State of keyboard ESC key.
   * This can be the combination of several keys,
   * for instance both left and right CTRL keys.
   */
  lw6gui_button_t key_ctrl;
  /**
   * State of keyboard ESC key.
   * This can be the combination of several keys,
   * for instance both left and right ALT keys.
   */
  lw6gui_button_t key_alt;
  /**
   * State of keyboard PAGE UP key.
   * This can be the combination of several keys.
   */
  lw6gui_button_t key_pgup;
  /**
   * State of keyboard PAGE UP key.
   * This can be the combination of several keys.
   */
  lw6gui_button_t key_pgdown;
  /// List of events, contains keypress objects.
  lw6sys_list_t *queue;
  /// Array of button states, indexed by keycodes.
  lw6gui_button_t keys_state[LW6GUI_NB_KEYS];
}
lw6gui_keyboard_t;

typedef enum lw6gui_drag_mode_e
{
  LW6GUI_DRAG_MODE_OFF = 0,
  LW6GUI_DRAG_MODE_ON = 1,
  LW6GUI_DRAG_MODE_DONE = 2
}
lw6gui_drag_mode_t;

/**
 * Use to store mouse pointer information.
 */
typedef struct lw6gui_mouse_pointer_s
{
  /// Mouse X position (pixels).
  int pos_x;
  /// Mouse Y position (pixels).
  int pos_y;
  /**
   * Mouse X speed. The unit is pixels per second. This is
   * based on the last move, for instance if between two
   * moves 100 msec have elapsed, and mouse moved 13 pixels,
   * then speed is 130.
   */
  int speed_x;
  /**
   * Mouse Y speed. The unit is pixels per second. This is
   * based on the last move, for instance if between two
   * moves 100 msec have elapsed, and mouse moved 13 pixels,
   * then speed is 130.
   */
  int speed_y;
}
lw6gui_mouse_pointer_t;

/**
 * Mouse information, contains detailed mouse state,
 * including mouse position and button states but
 * also keeps track of mouse speed as well as its
 * corresponding map coordinates. That is, given
 * the current screen position, what does it mean
 * on the logical map/battlefield.
 */
typedef struct lw6gui_mouse_s
{
  /// Wether mouse was moved lately. 1 means yes, 0 no.
  int moved;
  /// Timestamp of last move.
  int64_t last_moved;
  /**
   * Information about the mouse pointer, using screen
   * coordinates, the unit being pixels.
   */
  lw6gui_mouse_pointer_t screen_pointer;
  /**
   * Information about the mouse pointer, using map
   * coordinates, the unit being the map slot.
   * This is possibly very different from screen coordinates,
   * they can be inverted, have a different scale, and globally
   * it's just something else, even if it refers to the
   * same physical move.
   */
  lw6gui_mouse_pointer_t map_pointer;
  /**
   * Information about the mouse pointer when drag
   * mode was entered. The unit is screen pixels.
   */
  lw6gui_mouse_pointer_t screen_drag_start;
  /// The current drag state.
  lw6gui_drag_mode_t drag_mode;
  /**
   * The index of the menu item the mouse is on.
   * This is the only was to know when to select an item,
   * one should not use mouse coords outside the gfx renderer
   * code for this purpose, it's the renderer which has
   * knowledge about where menu items are.
   */
  int menu_position;
  /**
   * Set to -1 if one needs to scroll up (decrease menu index)
   * to +1 if one needs to scroll down (increase menu index)
   * and 0 if one needs to do nothing as far as scrolling is
   * concerned.
   */
  int menu_scroll;
  /// Wether mouse pointer is over the ESC button.
  int menu_esc;
  /// Mouse left button state.
  lw6gui_button_t button_left;
  /// Mouse right button state.
  lw6gui_button_t button_right;
  /// Mouse middle button state.
  lw6gui_button_t button_middle;
  /// Mouse wheel up state.
  lw6gui_button_t wheel_up;
  /// Mouse wheel down state.
  lw6gui_button_t wheel_down;
}
lw6gui_mouse_t;

/**
 * Joystick information, contains detailed joystick state.
 * This structure uses a pad-like interface, there's no
 * knowledge of analog interfaces, it transforms everything
 * to a binary "up or down" and "left or right". This
 * interface only knows about 6 buttons, this is done on
 * purpose, the logic behind it is that more than 6 buttons
 * makes the control way too complicated. Actually, most
 * common functions are and should be available through
 * the 4 first (a,b,c,d) buttons. The e and f are here for
 * additionnal not-so-important features.
 */
typedef struct lw6gui_joystick_s
{
  /// Joystick up button state.
  lw6gui_button_t pad_up;
  /// Joystick down button state.
  lw6gui_button_t pad_down;
  /// Joystick left button state.
  lw6gui_button_t pad_left;
  /// Joystick right button state.
  lw6gui_button_t pad_right;
  /// Joystick a button state.
  lw6gui_button_t button_a;
  /// Joystick b button state.
  lw6gui_button_t button_b;
  /// Joystick c button state.
  lw6gui_button_t button_c;
  /// Joystick d button state.
  lw6gui_button_t button_d;
  /// Joystick e button state.
  lw6gui_button_t button_e;
  /// Joystick f button state.
  lw6gui_button_t button_f;
}
lw6gui_joystick_t;

/**
 * Global input state, contains informations about the
 * keyboard, mouse and joystick. This is the macro object
 * used to exchange data and transmit input information
 * from the rendering thread which gathers it to the
 * logical thread which computes the game state.
 */
typedef struct lw6gui_input_s
{
  /// Wether this input struct has changed and needs to be synchronized.
  int need_sync;
  /// Keyboard information.
  lw6gui_keyboard_t keyboard;
  /// Mouse information.
  lw6gui_mouse_t mouse;
  /// Joysticks information.
  lw6gui_joystick_t joysticks[LW6GUI_NB_JOYSTICKS];
}
lw6gui_input_t;

/**
 * Menu item object. Basically, a menu is an array of
 * these items, it's up to the gfx backend to render
 * this as accurately as possible. The most important
 * field is probably the label.
 */
typedef struct lw6gui_menuitem_s
{
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /// What is displayed in the menu item.
  char *label;
  /// An additionnal tooltip explaining what the item is about.
  char *tooltip;
  /**
   * The value for this item, can typically be used for booleans
   * and integer values, in addition to the information conveyed
   * by the label. One special case is colored items, in that
   * case the value will be used as a color index.
   */
  int value;
  /// Wether the item is valid and can be used.
  int enabled;
  /// Wether the item is the current selection.
  int selected;
  /**
   * Wether to colorize the item, and in that case,
   * use the value field to know which color to use.
   */
  int colored;
  /// Timestamp of last time the menu item was updated and changed.
  int last_change;
  /// Timestamp of last time the menu was selected.
  int last_select;
  /// Timestamp of last time the menu was unselected.
  int last_unselect;
}
lw6gui_menuitem_t;

/**
 * Menu item object. Basically, a menu is an array of
 * menu items, it's up to the gfx backend to render
 * this as accurately as possible. The most important
 * field is probably the items labels. The menu object
 * also stores state information such as what was the
 * first item displayed lately.
 */
typedef struct lw6gui_menu_s
{
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /// Title of the menu, used for breadcrumbs.
  char *title;
  /// Additionnal help text, explaining what the menu is about.
  char *help;
  /**
   * Popup text, will be displayed when the menu is first
   * displayed, and then disappear.
   */
  char *popup;
  /// Number of items.
  int nb_items;
  /// Special item describing the ESC button.
  lw6gui_menuitem_t *esc_item;
  /// Array of items, containing all the menu items.
  lw6gui_menuitem_t **items;
  /// The current selection.
  int selected_item;
  /**
   * The first item displayed, this is mandatory if we want
   * the menus to be displayable in different states, for
   * instance with first item being 2 and items displayed from
   * 2 to 10 or with first item being 5 and items displayed from
   * 2 to 10. In the first case the 1st item is selected, in
   * the second case it's the 4th.
   */
  int first_item_displayed;
  /// Number of items displayed.
  int nb_items_displayed;
  /**
   * Index, display-based (that is, 0 here means first displayed
   * and not necessarly first in the items array), of the selected
   * item.
   */
  int order_of_selected_on_display;
  /// Wether scrolling is allowed.
  int allow_scroll;
  /**
   * List of strings containing the breadcrumbs, that is to
   * say all the menu titles that one must use to get here.
   */
  lw6sys_list_t *breadcrumbs;
}
lw6gui_menu_t;

/**
 * Statefull object used to make transitions between 2 floats.
 * Basically, one needs to choose a target, which is y2, and give
 * a start, which is defined by s1 and y1 (speed and y value).
 * Then with t1 (start timestamp) and duration the object has
 * functions which enables interpolation between those two values,
 * knowing at the end the value will be y2 and the speed 0. To
 * some extent, this is a primitive bezier-like tool.
 */
typedef struct lw6gui_smoother_s
{
  /// Speed at startup.
  float s1;
  /// Y value at startup.
  float y1;
  /// Y target value.
  float y2;
  /// Timestamp at startup.
  int64_t t1;
  /// Duration (in milliseconds) of the transition.
  int duration;
} lw6gui_smoother_t;

/**
 * A basic rectangle data. The idea is to store both
 * corner positions and width and height to cache the
 * values and avoid always recalculating them.
 * Values are integer based, for a floating point
 * equivalent, see the zone struct.
 */
typedef struct lw6gui_rect_s
{
  /// Top-left corner X position.
  int x1;
  /// Top-left corner Y position.
  int y1;
  /// Bottom-right corner X position.
  int x2;
  /// Bottom-right corner Y position.
  int y2;
  /// Width.
  int w;
  /// Height.
  int h;
}
lw6gui_rect_t;

/**
 * Array of rectangles. This is typically used to make
 * tiles that overlap. It's mostly used to display fighters/maps
 * using multiple textures when the whole stuff does not fit
 * in one single OpenGL texture and needs to be splitted.
 * Technically, when one needs to split textures, performance
 * is poor, but still better than relying on software renderer
 * only.
 */
typedef struct lw6gui_rect_array_s
{
  /// Size of original source data.
  lw6sys_whd_t source;
  /**
   * Boundary limits of the rect array, this is typically
   * bigger that source size, it starts at negative values
   * and finishes outside the source. It's interesting to
   * cover that big an area to enable both the water effect
   * and proper wrapping/clamping.
   */
  lw6gui_rect_t limits;
  /**
   * Width and height of the tiles, this is typically a
   * power of two, as it's designed to match an OpenGL
   * low-level texture object.
   */
  int tile_size;
  /**
   * The border size one needs to cut from the tile_size
   * (on both sides, up and down or left and right) to
   * get the real usable size of the tile.
   */
  int border_size;
  /**
   * The tile spacing, difference of X or Y between two
   * tiles, this is typically smaller that tile_size.
   */
  int tile_spacing;
  /// Number of tiles on the X axis (width).
  int nb_tiles_w;
  /// Number of tiles on the Y axis (height).
  int nb_tiles_h;
  /// Overall number of tiles.
  int nb_tiles;
}
lw6gui_rect_array_t;

/**
 * Basic point type, 3 floating point coords.
 */
typedef struct lw6gui_point_s
{
  /// X position.
  float x;
  /// Y position.
  float y;
  /// Z position.
  float z;
} lw6gui_point_t;

/**
 * Basic segment type, composed of 2 points
 * (floating point values).
 */
typedef struct lw6gui_segment_s
{
  /// 1st point.
  lw6gui_point_t p1;
  /// 2nd point.
  lw6gui_point_t p2;
}
lw6gui_segment_t;

/**
 * Basic triangle type, composed of 3 points
 * (floating point values).
 */
typedef struct lw6gui_triangle_s
{
  /// 1st point.
  lw6gui_point_t p1;
  /// 2nd point.
  lw6gui_point_t p2;
  /// 3rd point.
  lw6gui_point_t p3;
} lw6gui_triangle_t;

/**
 * Basic quad type, composed of 4 points
 * (floating point values).
 */
typedef struct lw6gui_quad_s
{
  /// 1st point.
  lw6gui_point_t p1;
  /// 2nd point.
  lw6gui_point_t p2;
  /// 3rd point.
  lw6gui_point_t p3;
  /// 4th point.
  lw6gui_point_t p4;
} lw6gui_quad_t;

/**
 * A basic rectangle data. The idea is to store both
 * corner positions and width and height to cache the
 * values and avoid always recalculating them.
 * Values are float based, for an integer point
 * equivalent, see the rect struct.
 */
typedef struct lw6gui_zone_s
{
  /// Top-left corner X position.
  float x1;
  /// Top-left corner Y position.
  float y1;
  /// Bottom-right corner X position.
  float x2;
  /// Bottom-right corner Y position.
  float y2;
  /// Width.
  float w;
  /// Height.
  float h;
} lw6gui_zone_t;

/**
 * Macro object used to store viewport information.
 * Viewport here means "what part of the map should
 * we display, on which part of the screen, and with
 * which parameters".
 */
typedef struct lw6gui_viewport_s
{
  /// Shape of the map to display, unit is map slot.
  lw6sys_whd_t map_shape;
  /// Shape of the screen, unit is pixels.
  lw6sys_whd_t screen_shape;
  /**
   * X coord of the point we want to display at the
   * center of the screen. This is typically our
   * main cursor if we're using the keyboard to
   * move it. Unit is map slot.
   */
  float center_x;
  /**
   * Y coord of the point we want to display at the
   * center of the screen. This is typically our
   * main cursor if we're using the keyboard to
   * move it. Unit is map slot.
   */
  float center_y;
  /**
   * Previous X coord of the point we wanted to display at the
   * center of the screen. This is typically our
   * main cursor if we're using the keyboard to
   * move it. Unit is map slot.
   */
  float old_center_x;
  /**
   * Previous Y coord of the point we wanted to display at the
   * center of the screen. This is typically our
   * main cursor if we're using the keyboard to
   * move it. Unit is map slot.
   */
  float old_center_y;
  /**
   * Speed at which the viewport is moving on the X axis, unit
   * is map slot per second.
   */
  float speed_x;
  /**
   * Speed at which the viewport is moving on the Y axis, unit
   * is map slot per second.
   */
  float speed_y;
  /// X-polarity parameter (1=on, 0=off, -1=invert).
  int x_polarity;
  /// Y-polarity parameter (1=on, 0=off, -1=invert).
  int y_polarity;
  /// Wether to wrap map on the X axis.
  int x_wrap;
  /// Wether to wrap map on the Y axis.
  int y_wrap;
  /**
   * Drawable zone, this is the physical on-screen viewport.
   * Unit is pixels.
   */
  lw6gui_zone_t drawable;
  /**
   * Zone corresponding to the map, if it was to be drawn
   * as a whole, regardless of drawable size, wrapping and
   * polarity.
   */
  lw6gui_zone_t map_main;
  /**
   * Zone corresponding to the map, only the main map, ignoring
   * wrapping and polarity, but clipped with drawable zone.
   */
  lw6gui_zone_t map_main_clipped;
  /**
   * Actual visible zone of the map, including wrapping, polarity,
   * and drawable clip aware.
   */
  lw6gui_zone_t map_visible;
} lw6gui_viewport_t;

/* gui-button.c */
extern void lw6gui_button_register_down (lw6sys_context_t * sys_context, lw6gui_button_t * button, int64_t timestamp);
extern void lw6gui_button_register_up (lw6sys_context_t * sys_context, lw6gui_button_t * button);
extern int lw6gui_button_is_pressed (lw6sys_context_t * sys_context, const lw6gui_button_t * button);
extern int lw6gui_button_pop_press (lw6sys_context_t * sys_context, lw6gui_button_t * button);
extern int lw6gui_button_pop_simple_click (lw6sys_context_t * sys_context, lw6gui_button_t * button);
extern int lw6gui_button_pop_double_click (lw6sys_context_t * sys_context, lw6gui_button_t * button);
extern int lw6gui_button_pop_triple_click (lw6sys_context_t * sys_context, lw6gui_button_t * button);
extern void lw6gui_button_update_repeat (lw6sys_context_t * sys_context, lw6gui_button_t * button, lw6gui_repeat_settings_t * repeat_settings,
					 int64_t timestamp, int auto_release_enabled);
extern int lw6gui_button_sync (lw6sys_context_t * sys_context, lw6gui_button_t * dst, lw6gui_button_t * src);

/* gui-coord.c */
extern int lw6gui_coord_calc_xy (lw6sys_context_t * sys_context, float *dst_x, float *dst_y, float dst_x0,
				 float dst_y0, float dst_w, float dst_h, float src_x, float src_y, float src_x0, float src_y0, float src_w, float src_h);
extern int lw6gui_coord_viewport_setup (lw6sys_context_t * sys_context, lw6gui_viewport_t * viewport);
extern void lw6gui_coords_fix_xy_float (lw6sys_context_t * sys_context, float *x, float *y, int *x_flipped, int *y_flipped, float w, float h, int x_polarity,
					int y_polarity);

/* gui-input.c */
extern int lw6gui_input_init (lw6sys_context_t * sys_context, lw6gui_input_t * input);
extern void lw6gui_input_quit (lw6sys_context_t * sys_context, lw6gui_input_t * input);
extern lw6gui_input_t *lw6gui_input_new (lw6sys_context_t * sys_context);
extern void lw6gui_input_free (lw6sys_context_t * sys_context, lw6gui_input_t * input);
extern int lw6gui_input_reset (lw6sys_context_t * sys_context, lw6gui_input_t * input);
extern void lw6gui_input_update_repeat (lw6sys_context_t * sys_context, lw6gui_input_t * input, lw6gui_repeat_settings_t * repeat_settings, int64_t timestamp);
extern void lw6gui_input_register_change (lw6sys_context_t * sys_context, lw6gui_input_t * input);
extern int lw6gui_input_need_sync (lw6sys_context_t * sys_context, const lw6gui_input_t * input);
extern int lw6gui_input_sync (lw6sys_context_t * sys_context, lw6gui_input_t * dst, lw6gui_input_t * src);
extern void lw6gui_input_enable_auto_release (lw6sys_context_t * sys_context, lw6gui_input_t * input);

/* gui-joystick.c */
extern int lw6gui_joystick_check_index (lw6sys_context_t * sys_context, int i);
extern void lw6gui_joystick_update_axis_x (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, int x, int limit, int64_t timestamp);
extern void lw6gui_joystick_update_axis_y (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, int y, int limit, int64_t timestamp);
extern void lw6gui_joystick_update_repeat (lw6sys_context_t * sys_context, lw6gui_joystick_t * joystick, lw6gui_repeat_settings_t * repeat_settings,
					   int64_t timestamp);
extern int lw6gui_joystick_sync (lw6sys_context_t * sys_context, lw6gui_joystick_t * dst, lw6gui_joystick_t * src);
extern void lw6gui_joystick_get_move_pad (lw6sys_context_t * sys_context, const lw6gui_joystick_t * joystick, lw6gui_move_pad_t * move_pad);

/* gui-keyboard.c */
extern int lw6gui_keyboard_check_keysym (lw6sys_context_t * sys_context, int keysym);
extern lw6gui_keypress_t *lw6gui_keyboard_pop_keypress (lw6sys_context_t * sys_context, lw6gui_keyboard_t * keyboard);
extern int lw6gui_keyboard_is_pressed (lw6sys_context_t * sys_context, const lw6gui_keyboard_t * keyboard, int keysym);
extern int lw6gui_keyboard_register_key_down (lw6sys_context_t * sys_context, lw6gui_keyboard_t * keyboard, int keysym, int unicode, char *label,
					      int64_t timestamp);
extern int lw6gui_keyboard_register_key_up (lw6sys_context_t * sys_context, lw6gui_keyboard_t * keyboard, int keysym);
extern void lw6gui_keyboard_update_repeat (lw6sys_context_t * sys_context, lw6gui_keyboard_t * keyboard, lw6gui_repeat_settings_t * repeat_settings,
					   int64_t timestamp);
extern int lw6gui_keyboard_sync (lw6sys_context_t * sys_context, lw6gui_keyboard_t * dst, lw6gui_keyboard_t * src);
extern void lw6gui_keyboard_get_move_pad (lw6sys_context_t * sys_context, const lw6gui_keyboard_t * keyboard, lw6gui_move_pad_t * move_pad);

/* gui-keypress.c */
extern lw6gui_keypress_t *lw6gui_keypress_new (lw6sys_context_t * sys_context, int keysym, int unicode, const char *label);
extern void lw6gui_keypress_free (lw6sys_context_t * sys_context, lw6gui_keypress_t * keypress);
extern char *lw6gui_keypress_repr (lw6sys_context_t * sys_context, const lw6gui_keypress_t * keypress);

/* gui-look.c */
extern lw6gui_look_t *lw6gui_look_new (lw6sys_context_t * sys_context, const lw6map_style_t * map_style);
extern void lw6gui_look_free (lw6sys_context_t * sys_context, lw6gui_look_t * look);
extern int lw6gui_look_memory_footprint (lw6sys_context_t * sys_context, const lw6gui_look_t * look);
extern char *lw6gui_look_repr (lw6sys_context_t * sys_context, const lw6gui_look_t * look);
extern int lw6gui_look_set (lw6sys_context_t * sys_context, lw6gui_look_t * look, char *key, char *value);
extern char *lw6gui_look_get (lw6sys_context_t * sys_context, const lw6gui_look_t * look, char *key);
extern int lw6gui_look_is_same (lw6sys_context_t * sys_context, const lw6gui_look_t * look_a, const lw6gui_look_t * look_b);
extern lw6gui_look_t *lw6gui_look_dup (lw6sys_context_t * sys_context, const lw6gui_look_t * look);
extern void lw6gui_look_fix (lw6sys_context_t * sys_context, lw6gui_look_t * look);
extern int lw6gui_look_zoom_in (lw6sys_context_t * sys_context, lw6gui_look_t * look, float zoom_step);
extern int lw6gui_look_zoom_out (lw6sys_context_t * sys_context, lw6gui_look_t * look, float zoom_step);

/* gui-menuitem.c */
extern lw6gui_menuitem_t *lw6gui_menuitem_new (lw6sys_context_t * sys_context, const char *label, const char *tooltip, int value, int enabled, int selected,
					       int colored);
extern void lw6gui_menuitem_free (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem);

extern int lw6gui_menuitem_memory_footprint (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem);
extern char *lw6gui_menuitem_repr (lw6sys_context_t * sys_context, const lw6gui_menuitem_t * menuitem);
extern void lw6gui_menuitem_set_label (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, const char *label, int64_t now);
extern void lw6gui_menuitem_set_tooltip (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, const char *tooltip, int64_t now);
extern void lw6gui_menuitem_set_value (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, int value, int64_t now);
extern void lw6gui_menuitem_select (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, int state, int64_t now);
extern void lw6gui_menuitem_enable (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, int state, int64_t now);
extern u_int32_t lw6gui_menuitem_checksum (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, lw6gui_look_t * look);
extern int lw6gui_menuitem_is_same (lw6sys_context_t * sys_context, const lw6gui_menuitem_t * menuitem_a, const lw6gui_menuitem_t * menuitem_b);
extern lw6gui_menuitem_t *lw6gui_menuitem_dup (lw6sys_context_t * sys_context, const lw6gui_menuitem_t * menuitem);
extern int lw6gui_menuitem_sync (lw6sys_context_t * sys_context, lw6gui_menuitem_t * dst, lw6gui_menuitem_t * src);

/* gui-menu.c */
extern lw6gui_menu_t *lw6gui_menu_new (lw6sys_context_t * sys_context, const char *title, const char *help, const char *popup, const char *esc,
				       int enable_esc);
extern void lw6gui_menu_free (lw6sys_context_t * sys_context, lw6gui_menu_t * menu);
extern int lw6gui_menu_memory_footprint (lw6sys_context_t * sys_context, lw6gui_menu_t * menu);
extern char *lw6gui_menu_repr (lw6sys_context_t * sys_context, const lw6gui_menu_t * menu);
extern void lw6gui_menu_set_title (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, const char *title);
extern void lw6gui_menu_set_help (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, const char *help);
extern void lw6gui_menu_set_popup (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, const char *popup);
extern void lw6gui_menu_close_popup (lw6sys_context_t * sys_context, lw6gui_menu_t * menu);
extern int lw6gui_menu_has_popup (lw6sys_context_t * sys_context, lw6gui_menu_t * menu);
extern lw6gui_menuitem_t *lw6gui_menu_get_item (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position);
extern int lw6gui_menu_select (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position, int allow_scroll, int64_t now);
extern void lw6gui_menu_select_esc (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int state, int64_t now);
extern void lw6gui_menu_enable_esc (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int state, int64_t now);
extern int lw6gui_menu_scroll_up (lw6sys_context_t * sys_context, lw6gui_menu_t * menu);
extern int lw6gui_menu_scroll_down (lw6sys_context_t * sys_context, lw6gui_menu_t * menu);
extern int lw6gui_menu_set_breadcrumbs (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, lw6sys_list_t * breadcrumbs);
extern void lw6gui_menu_center (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position, int max_displayed_items);
extern int lw6gui_menu_insert (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, lw6gui_menuitem_t * menuitem, int position, int64_t now);
extern int lw6gui_menu_append (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, lw6gui_menuitem_t * menuitem, int64_t now);
extern int lw6gui_menu_remove (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position, int64_t now);
extern int lw6gui_menu_remove_all (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int64_t now);
extern void lw6gui_menu_update_display_range (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int max_displayed_items);
extern int lw6gui_menu_insert_for_id_use (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, char *label,
					  char *tooltip, int value, int enabled, int selected, int colored, int position, int64_t now);
extern int lw6gui_menu_append_for_id_use (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, char *label, char *tooltip, int value, int enabled,
					  int selected, int colored, int64_t now);
extern int lw6gui_menu_remove_using_id (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int menuitem_id, int64_t now);
extern void lw6gui_menu_sync_using_id (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int menuitem_id,
				       char *label, char *tooltip, int value, int enabled, int selected, int colored, int64_t now);
extern int lw6gui_menu_is_same (lw6sys_context_t * sys_context, const lw6gui_menu_t * menu_a, const lw6gui_menu_t * menu_b);
extern lw6gui_menu_t *lw6gui_menu_dup (lw6sys_context_t * sys_context, const lw6gui_menu_t * menu);
extern int lw6gui_menu_sync (lw6sys_context_t * sys_context, lw6gui_menu_t * dst, lw6gui_menu_t * src);

/* gui-mouse.c */
extern void lw6gui_mouse_register_move (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, int screen_pos_x, int screen_pos_y, int64_t timestamp);
extern int lw6gui_mouse_poll_move (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, int *screen_pos_x, int *screen_pos_y);
extern void lw6gui_mouse_update_repeat (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, lw6gui_repeat_settings_t * repeat_settings, int64_t timestamp);
extern int lw6gui_mouse_sync (lw6sys_context_t * sys_context, lw6gui_mouse_t * dst, lw6gui_mouse_t * src);
extern void lw6gui_mouse_drag_begin (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse);
extern void lw6gui_mouse_drag_end (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse);
extern int lw6gui_mouse_drag_pop (lw6sys_context_t * sys_context, lw6gui_mouse_t * mouse, int *delta_x, int *delta_y, int *pos_x, int *pos_y, int *speed_x,
				  int *speed_y);

/* gui-point.c */
extern int lw6gui_point_is_inside_rect (lw6sys_context_t * sys_context, lw6gui_point_t point, const lw6gui_rect_t * rect);

/* gui-poweroftwo.c */
extern int lw6gui_power_of_two_le (lw6sys_context_t * sys_context, int size);
extern int lw6gui_power_of_two_ge (lw6sys_context_t * sys_context, int size);

/* gui-quad.c */
extern int lw6gui_quad_is_inside_rect (lw6sys_context_t * sys_context, const lw6gui_quad_t * quad, const lw6gui_rect_t * rect);

/* gui-rect.c */
extern void lw6gui_rect_init_xywh (lw6sys_context_t * sys_context, lw6gui_rect_t * rect, int x, int y, int w, int h);
extern void lw6gui_rect_init_x1y1x2y2 (lw6sys_context_t * sys_context, lw6gui_rect_t * rect, int x1, int y1, int x2, int y2);
extern void lw6gui_rect_clip (lw6sys_context_t * sys_context, lw6gui_rect_t * dst, const lw6gui_rect_t * src, const lw6gui_rect_t * clip);

/* gui-rectarray.c */
extern int lw6gui_rect_array_init (lw6sys_context_t * sys_context, lw6gui_rect_array_t * rect_array, int w, int h, int tile_size, int border_size);
extern int lw6gui_rect_array_get_tile_by_source_xy (lw6sys_context_t * sys_context, const lw6gui_rect_array_t * rect_array, lw6gui_rect_t * rect, int *i,
						    int source_x, int source_y);
extern int lw6gui_rect_array_get_tile_by_i (lw6sys_context_t * sys_context, const lw6gui_rect_array_t * rect_array, lw6gui_rect_t * rect, int i);
extern int lw6gui_rect_array_get_tile_and_quad (lw6sys_context_t * sys_context, const lw6gui_rect_array_t *
						rect_array,
						lw6gui_rect_t * rect, int *i,
						lw6gui_quad_t * quad, const lw6gui_quad_t * source_quad, int x_polarity, int y_polarity);

/* gui-segment.c */
extern int lw6gui_segment_is_inside_rect (lw6sys_context_t * sys_context, const lw6gui_segment_t * segment, const lw6gui_rect_t * rect);

/* gui-smoother.c */
extern void lw6gui_smoother_init (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, float value, int duration);
extern void lw6gui_smoother_immediate_force (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, float value);
extern void lw6gui_smoother_set_target (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, float value, int64_t now);
extern float lw6gui_smoother_get_value (lw6sys_context_t * sys_context, const lw6gui_smoother_t * smoother, int64_t now);
extern void lw6gui_smoother_fix_overflow (lw6sys_context_t * sys_context, lw6gui_smoother_t * smoother, int step);

/* gui-test.c */
extern int lw6gui_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6gui_test_run (lw6sys_context_t * sys_context, int mode);

/* gui-triangle.c */
extern int lw6gui_triangle_is_inside_rect (lw6sys_context_t * sys_context, const lw6gui_triangle_t * triangle, const lw6gui_rect_t * rect);

/* gui-videomode.c */
extern int lw6gui_video_mode_find_closest (lw6sys_context_t * sys_context, lw6gui_video_mode_t * closest, const lw6gui_video_mode_t * wished,
					   lw6sys_list_t * available);
extern int lw6gui_video_mode_is_same (lw6sys_context_t * sys_context, const lw6gui_video_mode_t * mode_a, const lw6gui_video_mode_t * mode_b);
extern int lw6gui_video_mode_sync_ratio (lw6sys_context_t * sys_context, lw6gui_video_mode_t * dst, const lw6gui_video_mode_t * src);

/* gui-viewport.c */
extern int lw6gui_viewport_init (lw6sys_context_t * sys_context, lw6gui_viewport_t * viewport, int screen_w,
				 int screen_h, float drawable_x1,
				 float drawable_y1, float drawable_x2,
				 float drawable_y2, float center_x,
				 float center_y, int map_w, int map_h,
				 int x_polarity, int y_polarity, int x_wrap,
				 int y_wrap, int keep_ratio, float global_zoom, float scroll_limit, int use_old_center);
extern void lw6gui_viewport_map_to_screen (lw6sys_context_t * sys_context, lw6gui_viewport_t * viewport, float *screen_x, float *screen_y, float map_x,
					   float map_y, int clip);
extern void lw6gui_viewport_screen_to_map (lw6sys_context_t * sys_context, lw6gui_viewport_t * viewport, float *map_x, float *map_y, float screen_x,
					   float screen_y, int wrap);
extern void lw6gui_viewport_calc_drag (lw6sys_context_t * sys_context, lw6gui_viewport_t * viewport, float *map_dst_x, float *map_dst_y, float map_src_x,
				       float map_src_y, int screen_dx, int screen_dy);

/* gui-zone.c */
extern void lw6gui_zone_init_x1y1x2y2 (lw6sys_context_t * sys_context, lw6gui_zone_t * zone, float x1, float y1, float x2, float y2);
extern void lw6gui_zone_init_xywh (lw6sys_context_t * sys_context, lw6gui_zone_t * zone, float x, float y, float w, float h);
extern void lw6gui_zone_clip (lw6sys_context_t * sys_context, lw6gui_zone_t * dst, lw6gui_zone_t * src, lw6gui_zone_t * clip);

#endif
