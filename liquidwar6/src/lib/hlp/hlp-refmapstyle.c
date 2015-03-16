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

#include "../map/map.h"

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_map_style (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_KEEP_RATIO,
			 _x_
			 ("Defines wether the map should keep its ratio, or if it should be stretched to fill the shape of your screen."),
			 LW6MAP_STYLE_DEFAULT_KEEP_RATIO);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_ZOOM,
			  _x_
			  ("Defines the map zoom. If lower than 1.0, map will occupy only a fraction of the screen, if greater than 1.0, some areas will be outside the screen, and the player will need to scroll through it."),
			  LW6MAP_STYLE_DEFAULT_ZOOM, 0, 0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_ZOOM_MIN,
			  _x_
			  ("Defines the min map zoom. If set to a low value, you'll be able to dynamically view a very small, reduced map."),
			  LW6MAP_STYLE_DEFAULT_ZOOM_MIN, 0, 0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_ZOOM_MAX,
			  _x_
			  ("Defines the max map zoom. If set to a high value, you'll be able to dynamically view the map with hudge fighters, seeing only a fraction of the level."),
			  LW6MAP_STYLE_DEFAULT_ZOOM_MAX, 0, 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_X_WRAP,
			 _x_
			 ("Defines wether the map should be wrapped on the x axis. This is the companion of 'x-polarity', if no polarity is defined, map can't be wrapped, but in some cases, one might wish to have a map with polarity but without wrapping if, for instance, textures do not tile nicely."),
			 LW6MAP_STYLE_DEFAULT_X_WRAP);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_Y_WRAP,
			 _x_
			 ("Defines wether the map should be wrapped on the y axis. This is the companion of 'y-polarity', if no polarity is defined, map can't be wrapped, but in some cases, one might wish to have a map with polarity but without wrapping if, for instance, textures do not tile nicely."),
			 LW6MAP_STYLE_DEFAULT_Y_WRAP);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BACKGROUND_STYLE,
			_x_
			("The background defines, of course, what is displayed at the background, but it also conditions the colors used for other items, such as the menus for instance. The possible values are 'void' and 'bubbles'."),
			LW6MAP_STYLE_DEFAULT_BACKGROUND_STYLE);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_HUD_STYLE,
			_x_
			("The hud is where informations about the game are displayed. This means, who is winning, are other status-like informations. Possible values include 'floating' and 'tactical'."),
			LW6MAP_STYLE_DEFAULT_HUD_STYLE);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MENU_STYLE,
			_x_
			("The menu style is simply the name of the engine used to power the menu system. The only possible value, for now, is 'cylinder'."),
			LW6MAP_STYLE_DEFAULT_MENU_STYLE);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_VIEW_STYLE,
			_x_
			("The view style conditions which renderer is used for the map, the area where fighters are displayed. This is not the graphics backend. Indeed, the graphics backend defines which technical tool one uses (which library) one runs, wether this parameter says what kind of rendering one wants."),
			LW6MAP_STYLE_DEFAULT_VIEW_STYLE);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_ANIMATION_DENSITY,
			  _x_
			  ("Density of the background animation, that is, for instance, if the background animation is about displaying bubbles, using a high value will display many bubbles. A value of 1.0 corresponds to the default setting."),
			  LW6MAP_STYLE_DEFAULT_ANIMATION_DENSITY, 0, 10);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_ANIMATION_SPEED,
			  _x_
			  ("Speed of the background animation, that is, for instance, if the background animation is about displaying bubbles, using a high value will cause bubbles to move very fast. A value of 1.0 corresponds to the default setting."),
			  LW6MAP_STYLE_DEFAULT_ANIMATION_SPEED, 0, 10);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_CURSOR_SIZE,
			  _x_
			  ("Size of the cursors on the map. 1 is the default, setting it to a higher value will make cursors bigger, a lower value will make them smaller."),
			  LW6MAP_STYLE_DEFAULT_CURSOR_SIZE, 0, 10);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_COLORIZE_CURSOR,
			 _x_
			 ("If set, then all cursors will use the automatic guessed colors, or the specified colors, but basically they won't be displayed using their native colors. This can be usefull for you can wish to use a generic non-colored texture for your cursor and let it be colorized automatically so that it's accorded to the level."),
			 LW6MAP_STYLE_DEFAULT_COLORIZE_CURSOR);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_BLINK_CURSOR,
			 _x_
			 ("If set, then cursor will blink, allowing you to see what's under the cursor. It's just a matter of taste, you might to always have your cursor displayed, or prefer to have it disappear from time to time so that you can see the action below"),
			 LW6MAP_STYLE_DEFAULT_BLINK_CURSOR);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_HIDDEN_LAYER_ALPHA,
			  _x_
			  ("Whenever players are supposed to be hidden behind a wall, for instance if they are in layer 2 and layer 1 is filled with walls, it's still possible to see them, but with a low alpha value (almost transparent). This parameter allows you to trick this value, 0 will make these players absolutely invisible, 1 will make them totally opaque, like if they were on layer 1."),
			  LW6MAP_STYLE_DEFAULT_HIDDEN_LAYER_ALPHA, 0, 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_COLORIZE,
			 _x_
			 ("If set, then all background drawings including textures will use the background colors. This means, for instance, that if background colors are set automatically by color-auto from the map texture, then the background will adopt the same range of colors than the map itself. In short, the background will mimic the map."),
			 LW6MAP_STYLE_DEFAULT_COLORIZE);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_PIXELIZE,
			 _x_
			 ("Depending on the renderer capabilities, will try to pixelize some parts of the game. This can be used to emulate the old LW5 appearance."),
			 LW6MAP_STYLE_DEFAULT_PIXELIZE);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_COLOR_BASE_BG,
			  _x_
			  ("Defines the base color, more precisely, its bg (background) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
			  LW6MAP_STYLE_DEFAULT_COLOR_BASE_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_COLOR_BASE_FG,
			  _x_
			  ("Defines the base color, more precisely, its fg (foreground) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
			  LW6MAP_STYLE_DEFAULT_COLOR_BASE_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_COLOR_ALTERNATE_BG,
			  _x_
			  ("Defines the alternate color, more precisely, its bg (background) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
			  LW6MAP_STYLE_DEFAULT_COLOR_ALTERNATE_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_COLOR_ALTERNATE_FG,
			  _x_
			  ("Defines the alternate color, more precisely, its fg (foreground) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
			  LW6MAP_STYLE_DEFAULT_COLOR_ALTERNATE_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_BACKGROUND_COLOR_ROOT_BG,
			  _x_
			  ("Defines the main background color. This is, for instance, the color which will be used to clear the screen before drawing thing. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_ROOT_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_BACKGROUND_COLOR_ROOT_FG,
			  _x_
			  ("Defines a color which will be used together with color-base-bg to compose the background. It can be wise to have a minimum contrast between this color and color-base-bg, but it is not mandatory, especially if other colors are manually redefined. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_ROOT_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_BACKGROUND_COLOR_STUFF_BG,
			  _x_
			  ("Defines a color which will be used together with color-alternate-fg to draw things (animations, sprites, text, whatever) in the background. It should be different enough from color-alternate-fg so that one can really distinguish these colors. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_STUFF_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_BACKGROUND_COLOR_STUFF_FG,
			  _x_
			  ("Defines a color which will be used to draw things (animations, sprites, text, whatever) in the background. It should be different enough from color-alternate-bg so that one can really distinguish these colors. Think of this as the sprite, the text, the whatever-needs-to-be-seen-uses-this color. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_STUFF_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_HUD_COLOR_FRAME_BG,
			  _x_
			  ("Defines the background color for the hud frame. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_HUD_COLOR_FRAME_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_HUD_COLOR_FRAME_FG,
			  _x_
			  ("Defines the foreground color for the hud frame. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_HUD_COLOR_FRAME_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_HUD_COLOR_TEXT_BG,
			  _x_
			  ("Defines the background color for hud text. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_HUD_COLOR_TEXT_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_HUD_COLOR_TEXT_FG,
			  _x_
			  ("Defines the foreground color for hud text. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_HUD_COLOR_TEXT_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_MENU_COLOR_DEFAULT_BG,
			  _x_
			  ("Defines the default background color for menus. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DEFAULT_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_MENU_COLOR_DEFAULT_FG,
			  _x_
			  ("Defines the default foreground color for menus. In fact, this is the main color for menu text, the color used to draw letters in menus. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DEFAULT_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_MENU_COLOR_SELECTED_BG,
			  _x_
			  ("Defines the background color for a selected menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_MENU_COLOR_SELECTED_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_MENU_COLOR_SELECTED_FG,
			  _x_
			  ("Defines the foreground color for a selected menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_MENU_COLOR_SELECTED_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_MENU_COLOR_DISABLED_BG,
			  _x_
			  ("Defines the background color for a disabled menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DISABLED_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_MENU_COLOR_DISABLED_FG,
			  _x_
			  ("Defines the foreground color for a disabled menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DISABLED_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_VIEW_COLOR_CURSOR_BG,
			  _x_
			  ("Defines the background cursor color. Will typically be used to draw the shape of the cursor. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_CURSOR_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_VIEW_COLOR_CURSOR_FG,
			  _x_
			  ("Defines the foreground cursor color. Will typically be used to draw text in the cursor. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_CURSOR_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_VIEW_COLOR_MAP_BG,
			  _x_
			  ("Defines the background map color. If there's no map texture defined or if use-texture is false, this is the color of the places where armies will go. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_MAP_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_VIEW_COLOR_MAP_FG,
			  _x_
			  ("Defines the foreground map color. If there's no map texture defined or if use-texture is false, this is the color of walls, what armies can't go through. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_MAP_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_SYSTEM_COLOR_BG,
			  _x_
			  ("Defines the system background color, used when displaying system info, such as the number of frames per second. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_SYSTEM_COLOR_BG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_SYSTEM_COLOR_FG,
			  _x_
			  ("Defines the system foreground color, used when displaying system info, such as the number of frames per second. This will typically be text color. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
			  LW6MAP_STYLE_DEFAULT_SYSTEM_COLOR_FG);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_DEAD,
			  _x_
			  ("Defines the color for the teams when they are dead. By default it is black, this means when a team is weak it becomes black. Syntax is HTML-like, #RGB or #RRGGBB."),
			  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_DEAD);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_RED,
			  _x_ ("Defines the color for the red team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_RED);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_GREEN,
			  _x_ ("Defines the color for the green team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_GREEN);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_BLUE,
			  _x_ ("Defines the color for the blue team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_BLUE);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_YELLOW,
			  _x_ ("Defines the color for the yellow team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_YELLOW);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_CYAN,
			  _x_ ("Defines the color for the cyan team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_CYAN);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_MAGENTA,
			  _x_ ("Defines the color for the magenta team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_MAGENTA);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_ORANGE,
			  _x_ ("Defines the color for the orange team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_ORANGE);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_LIGHTBLUE,
			  _x_ ("Defines the color for the light blue team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_LIGHTBLUE);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_PURPLE,
			  _x_ ("Defines the color for the purple team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_PURPLE);
  _LW6HLP_POPULATE_COLOR (sys_context, references, LW6DEF_TEAM_COLOR_PINK,
			  _x_ ("Defines the color for the pink team. Syntax is HTML-like, #RGB or #RRGGBB."), LW6MAP_STYLE_DEFAULT_TEAM_COLOR_PINK);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MUSIC_FILE,
			_x_
			("Allows you to play a custom music file (typically your own ogg music) and override default game music. If file does not exist, game will use its internal music. The file will be searched for in the current 'music-path' but also in the current map directory. No absolute or even relative path are allowed, only a plain filename with no slash or backslash. Avoid special characters at all cost."),
			LW6MAP_STYLE_DEFAULT_MUSIC_FILE);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MUSIC_FILTER,
			_x_
			("A music filter, used when files are played randomly. This is not a complex regex-enabled filter, just a plain string search. Even the '*' wildcard won't work. If you want precise control on what music file to play, please consider reorganizing your files and/or use the 'music-file' parameter."),
			LW6MAP_STYLE_DEFAULT_MUSIC_FILTER);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MUSIC_EXCLUDE,
			_x_
			("If this string is found in a music file name, it will be excluded from the list when playing in random mode."),
			LW6MAP_STYLE_DEFAULT_MUSIC_EXCLUDE);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_WAVES, _x_ ("Activates the wave effect, that's to say level appears to be under water when playing."),
			 LW6MAP_STYLE_DEFAULT_WAVES);

  ret = 1;

  return ret;
}
