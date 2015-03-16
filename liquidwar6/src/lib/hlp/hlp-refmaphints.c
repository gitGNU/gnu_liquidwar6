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

#include "../ldr/ldr.h"

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_map_hints (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_RESAMPLE,
			 _x_
			 ("If set to true, maps will always be resampled to a size which depends on your screen resolution, zoom factor, and the rest. If false, maps will be set at the exact resolution of map.png."),
			 LW6LDR_HINTS_DEFAULT_RESAMPLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MIN_MAP_WIDTH,
			_x_
			("Allows you to give a minimum map width. When designing a map you might wonder: this is dumb I'm conceiving this map I know its width, why should I limit it? Now think of the player who decided to play with highly-defined maps because he has a super calculator and a hudge screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
			LW6LDR_HINTS_DEFAULT_MIN_MAP_WIDTH, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_MAP_WIDTH,
			_x_
			("Allows you to give a maximum map width. When designing a map you might wonder: this is dumb I'm conceiving this map I know its width, why should I limit it? Now think of the play who plays on a old slowish computer with a tiny screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
			LW6LDR_HINTS_DEFAULT_MAX_MAP_WIDTH, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MIN_MAP_HEIGHT,
			_x_
			("Allows you to give a minimum map height. When designing a map you might wonder: this is dumb I'm conceiving this map I know its height, why should I limit it? Now think of the player who decided to play with highly-defined maps because he has a super calculator and a hudge screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
			LW6LDR_HINTS_DEFAULT_MIN_MAP_HEIGHT, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_MAP_HEIGHT,
			_x_
			("Allows you to give a maximum map height. When designing a map you might wonder: this is dumb I'm conceiving this map I know its height, why should I limit it? Now think of the play who plays on a old slowish computer with a tiny screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
			LW6LDR_HINTS_DEFAULT_MAX_MAP_HEIGHT, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MIN_MAP_SURFACE,
			_x_
			("Allows you to give a minimum map surface. Map surface is simply (width * height). This parameter is just here to save you the hassle of defining both 'min-map-width' and 'min-map-height' in a consistent manner."),
			LW6LDR_HINTS_DEFAULT_MIN_MAP_SURFACE, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_MAP_SURFACE,
			_x_
			("Allows you to give a maximum map surface. Map surface is simply (width * height). This parameter is just here to save you the hassle of defining both 'max-map-width' and 'max-map-height' in a consistent manner."),
			LW6LDR_HINTS_DEFAULT_MAX_MAP_SURFACE, 0, 0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_FIGHTER_SCALE,
			  _x_
			  ("Defines how wide (in pixels) fighters must be. This parameter is very important and will largely condition the number of fighters on the map. It is used when loading the map. If it is, for instance, set to 1, there will be exactly a fighter per pixel on the screen. That is, if you play 640x480 on an empty map, the maximum fighters you could have is about 300000. The idea is that by changing the resolution, you also define the density of the map. In pratice, this is done in the hope that someone with a slow computer will pick up a low resolution and therefore play small levels. Conversely, someone with a brand new computer with powerfull CPU & GPU will use great resolutions and be happy with many fighters on the map. Still, changing the resolution after loading the map will not affet the number of fighters. Same for network games, the first player, who loads the map, defines its properties according to its own settings."),
			  LW6LDR_HINTS_DEFAULT_FIGHTER_SCALE, 0, 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DOWNSIZE_USING_FIGHTER_SCALE,
			 _x_
			 ("If set, then the game will automatically downsize a map according to the 'fighter-scale' parameter. Downsizing means: a 1600x1200 maps becomes 200x150, for instance. Downsizing causes fighters to be bigger because map resolution is lower. This can be usefull if you don't want fighters to be too small."),
			 LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_FIGHTER_SCALE);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_UPSIZE_USING_FIGHTER_SCALE,
			 _x_
			 ("If set, then the game will automatically upsize a map according to the 'fighter-scale' parameter. Upsizing means: a 160x120 maps becomes 400x300, for instance. Upsizing causes fighters to be smaller because map resolution is higher. This can be usefull if you don't want fighters to be too big."),
			 LW6LDR_HINTS_DEFAULT_UPSIZE_USING_FIGHTER_SCALE);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DOWNSIZE_USING_BENCH_VALUE,
			 _x_
			 ("If set, then the game will automatically downsize a map according to the 'bench-value' parameter. Downsizing means: a 1600x1200 maps becomes 200x150, for instance. Downsizing causes fighters to be bigger because map resolution is lower. This will avoid running the game on a too big map, with your computer not being able to handle it at the required speed."),
			 LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_BENCH_VALUE);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_UPSIZE_USING_BENCH_VALUE,
			 _x_
			 ("If set, then the game will automatically upsize a map according to the 'fighter-scale' parameter. Upsizing means: a 160x120 maps becomes 400x300, for instance. Upsizing causes fighters to be smaller because map resolution is higher. This will avoid useless pixelish 'jumbo fighters' look when your computer is powerfull enough to do better."),
			 LW6LDR_HINTS_DEFAULT_UPSIZE_USING_BENCH_VALUE);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_GUESS_COLORS,
			 _x_
			 ("Defines wether colors should be set automatically from texture colors. If set to true, then the program will try to pick up colors automatically from the texture, and will override the values of the color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg parameters. How these colors are picked up can't be garanteed, so if the map does not have strong contrast or if there can be any form of ambiguity, it's safe to set this to false and define one's own colors."),
			 LW6LDR_HINTS_DEFAULT_GUESS_COLORS);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_BACKGROUND_COLOR_AUTO,
			 _x_
			 ("Defines wether hud colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then hud_color_frame_bg, hud_color_frame_fg, hud_color_text_bg and hud_color_text_fg will be automatically set."),
			 LW6LDR_HINTS_DEFAULT_BACKGROUND_COLOR_AUTO);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_HUD_COLOR_AUTO,
			 _x_
			 ("Defines wether hud colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then hud_color_frame_bg, hud_color_frame_fg, hud_color_text_bg and hud_color_text_fg will be automatically set."),
			 LW6LDR_HINTS_DEFAULT_HUD_COLOR_AUTO);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_MENU_COLOR_AUTO,
			 _x_
			 ("Defines wether menu colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then menu_color_default_bg, menu_color_default_fg, menu_color_selected_bg, menu_color_selected_fg, menu_color_disabled_bg and menu_color_disabled_fg will be automatically set."),
			 LW6LDR_HINTS_DEFAULT_MENU_COLOR_AUTO);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_VIEW_COLOR_AUTO,
			 _x_
			 ("Defines wether view colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then view_color_cursor_bg, view_color_cursor_fg, view_color_map_bg and view_color_map_fg will be automatically set."),
			 LW6LDR_HINTS_DEFAULT_VIEW_COLOR_AUTO);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_SYSTEM_COLOR_AUTO,
			 _x_
			 ("Defines wether system colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then system_color_bg and system_color_fg will be automatically set."),
			 LW6LDR_HINTS_DEFAULT_SYSTEM_COLOR_AUTO);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_WALL_GREASE,
			_x_
			("This parameter allows you to make walls (AKA map foreground) thicker, or thiner, when map is loaded. Indeed, when map are resampled, and especially when they are downscaled, some walls may disappear, or some passages may be blocked. The loader can't automatically figure out wether it's more important to keep an existing wall or to keep an open passage for fighters. This parameter helps doing so, if you set it to a low value, level will be less greasy, and many passages might open themselves. On the contrary, if grease is at a high level, then a thin line of almost isolated pixels might become a thick wall. There's no real garantee your wall or passage will always be present, but it's a same bet to assume on a 'tunnel-like' level one needs to set grease to a low value, and on a 'wide open' level with few walls one needs to set grease to a high value."),
			LW6LDR_HINTS_DEFAULT_WALL_GREASE, LW6LDR_HINTS_MIN_WALL_GREASE, LW6LDR_HINTS_MAX_WALL_GREASE);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_GUESS_MOVES_PER_SEC,
			 _x_ ("If set, then loader will use 'time-to-cross-level' to guess the game speed parameters."),
			 LW6LDR_HINTS_DEFAULT_GUESS_MOVES_PER_SEC);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_SPEED,
			  _x_
			  ("This parameter is the main parameter on which game speed depends. The map loader will garantee, by downscaling the map, that to cross the level (by crossing the level we mean, for instance, going from top-left corner to bottom-right corner in a straight line) a fighter will take a constant amount of time. Under the hood, the loader might of course rescale the map but it will also change game speed so that, at the end, fighters take a constant time to cross the level. This is, indeed, the most important thing, players do not care much if internally there are X or Y moves per second, the global game experience depends on how fast fighter movement looks on the screen. The default settings corresponds roughly to one second to cross the level. If you set this to 2.0, it will go twice faster."),
			  LW6LDR_HINTS_DEFAULT_SPEED, 0, 0);

  ret = 1;

  return ret;
}
