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

#ifndef LIQUIDWAR6GFX_MOD_GL1_BACKGROUND_BUBBLES_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GL1_BACKGROUND_BUBBLES_INTERNAL_H

#include "gl1-bubbles.h"

#define _MOD_GL1_BACKGROUND_BUBBLES_MAX_NB_BUBBLES 100

typedef struct _mod_gl1_background_bubbles_bubble_s
{
  int active;
  int64_t t0;
  float size;
  float x;
}
_mod_gl1_background_bubbles_bubble_t;

typedef struct _mod_gl1_background_bubbles_state_s
{
  _mod_gl1_background_bubbles_bubble_t bubbles[_MOD_GL1_BACKGROUND_BUBBLES_MAX_NB_BUBBLES];
}
_mod_gl1_background_bubbles_state_t;

typedef struct _mod_gl1_background_bubbles_bitmap_data_s
{
  mod_gl1_utils_bitmap_t *background;
  mod_gl1_utils_bitmap_t *bubble;
}
_mod_gl1_background_bubbles_bitmap_data_t;

typedef struct _mod_gl1_background_bubbles_lastest_colors_s
{
  int colorize;
  lw6map_color_couple_t root;
  lw6map_color_couple_t stuff;
}
_mod_gl1_background_bubbles_latest_colors_t;

typedef struct _mod_gl1_background_bubbles_const_data_s
{
  int nb_bubbles;
  float yspeed;
  float bubble_yspeed;
  float bubble_size_min;
  float bubble_size_max;
}
_mod_gl1_background_bubbles_const_data_t;

typedef struct _mod_gl1_background_bubbles_context_s
{
  _mod_gl1_background_bubbles_state_t state;
  _mod_gl1_background_bubbles_bitmap_data_t bitmap_data;
  _mod_gl1_background_bubbles_latest_colors_t latest_colors;
  _mod_gl1_background_bubbles_const_data_t const_data;
}
_mod_gl1_background_bubbles_context_t;

/* gl1-bubbles-display.c */
extern void
_mod_gl1_background_bubbles_display_background (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context, _mod_gl1_background_bubbles_context_t * bubbles_context, const lw6gui_look_t * look);

/* gl1-bubbles-data.c */
extern int _mod_gl1_background_bubbles_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						  _mod_gl1_background_bubbles_context_t * bubbles_context);
extern void _mod_gl1_background_bubbles_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						     _mod_gl1_background_bubbles_context_t * bubbles_context);

/* gl1-bubbles-setup.c */
extern _mod_gl1_background_bubbles_context_t *_mod_gl1_background_bubbles_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void _mod_gl1_background_bubbles_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					      _mod_gl1_background_bubbles_context_t * bubbles_context);

#endif
