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

#ifndef LIQUIDWAR6GFX_MOD_GL1_SPLASH_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GL1_SPLASH_INTERNAL_H

#include "gl1-splash.h"

#define _MOD_GL1_SPLASH_GAME_NODE_ID 0x1234123412341234LL
#define _MOD_GL1_SPLASH_GAME_CURSOR1_ID 0x1234
#define _MOD_GL1_SPLASH_GAME_CURSOR2_ID 0x2345
#define _MOD_GL1_SPLASH_GAME_CURSOR3_ID 0x3456
#define _MOD_GL1_SPLASH_GAME_CURSOR4_ID 0x4567
#define _MOD_GL1_SPLASH_GAME_X_MOVE 1

typedef struct _mod_gl1_splash_const_data_s
{
  lw6sys_color_8_t root_bg;
  lw6map_color_couple_t map_color;
  lw6map_color_couple_t text_color;
  float text_center_x;
  float text_center_y;
  float text_size;
  float text_dw;
  float text_dh;
  int text_period;
  float rotate_x;
  float rotate_y;
  float rotate_z;
  float planet_x;
  float planet_y;
  float planet_z;
  float planet_radius;
  int slices;
  int stacks;
  int planet_day_period;
  float satellite_radius;
  float satellite_distance;
  int satellite_year_period;
  int satellite_day_period;
  float cursors_center_speed;
  int cursors_spin_radius;
  int cursors_spin_period;
} _mod_gl1_splash_const_data_t;

typedef struct _mod_gl1_splash_bitmap_data_s
{
  mod_gl1_utils_bitmap_t *satellite;
} _mod_gl1_splash_bitmap_data_t;

typedef struct _mod_gl1_splash_text_s
{
  char *str;
  mod_gl1_utils_shaded_text_t *shaded_text;
} _mod_gl1_splash_text_t;

typedef struct _mod_gl1_splash_game_s
{
  lw6map_level_t *level;
  lw6ker_game_struct_t *game_struct;
  lw6ker_game_state_t *game_state;
  GLUquadricObj *sphere;
  mod_gl1_utils_bitmap_t *bitmap;
} _mod_gl1_splash_game_t;

typedef struct _mod_gl1_splash_context_s
{
  _mod_gl1_splash_const_data_t const_data;
  _mod_gl1_splash_bitmap_data_t bitmap_data;
  _mod_gl1_splash_text_t text;
  _mod_gl1_splash_game_t game;
} _mod_gl1_splash_context_t;

/* gl1-splash-data.c */
extern int _mod_gl1_splash_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context);
extern void _mod_gl1_splash_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context);

/* gl1-splash-display.c */
extern void _mod_gl1_splash_display (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context);
extern void _mod_gl1_splash_patch_system_color (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context, _mod_gl1_splash_context_t * splash_context, lw6map_color_couple_t * system_color);

/* gl1-splash-setup.c */
extern _mod_gl1_splash_context_t *_mod_gl1_splash_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void _mod_gl1_splash_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context);

#endif // LIQUIDWAR6GFX_MOD_GL1_SPLASH_INTERNAL_H
