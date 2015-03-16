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

#ifndef LIQUIDWAR6LDR_INTERNAL_H
#define LIQUIDWAR6LDR_INTERNAL_H

#define _LW6LDR_FILE_METADATA_XML "metadata.xml"

#define _LW6LDR_FILE_MAP_PNG "map.png"
#define _LW6LDR_FILE_LAYER2_PNG "layer2.png"
#define _LW6LDR_FILE_LAYER3_PNG "layer3.png"
#define _LW6LDR_FILE_LAYER4_PNG "layer4.png"
#define _LW6LDR_FILE_LAYER5_PNG "layer5.png"
#define _LW6LDR_FILE_LAYER6_PNG "layer6.png"
#define _LW6LDR_FILE_LAYER7_PNG "layer7.png"
#define _LW6LDR_FILE_GLUE_PNG "glue.png"
#define _LW6LDR_FILE_BOOST_PNG "boost.png"
#define _LW6LDR_FILE_DANGER_PNG "danger.png"
#define _LW6LDR_FILE_MEDICINE_PNG "medicine.png"
#define _LW6LDR_FILE_ONE_WAY_NORTH_PNG "one-way-north.png"
#define _LW6LDR_FILE_ONE_WAY_EAST_PNG "one-way-east.png"
#define _LW6LDR_FILE_ONE_WAY_SOUTH_PNG "one-way-south.png"
#define _LW6LDR_FILE_ONE_WAY_WEST_PNG "one-way-west.png"
#define _LW6LDR_FILE_TEXTURE_PNG "texture.png"
#define _LW6LDR_FILE_TEXTURE_JPEG "texture.jpeg"
#define _LW6LDR_FILE_TEXTURE_JPG "texture.jpg"
#define _LW6LDR_FILE_TEXTURE_ALPHA_JPEG "texture-alpha.jpeg"
#define _LW6LDR_FILE_TEXTURE_ALPHA_JPG "texture-alpha.jpg"
#define _LW6LDR_FILE_CURSOR_PNG "cursor.png"
#define _LW6LDR_FILE_CURSOR_COLOR_PNG "cursor-color.png"

#define _LW6LDR_FILE_RULES_XML "rules.xml"
#define _LW6LDR_FILE_HINTS_XML "hints.xml"
#define _LW6LDR_FILE_STYLE_XML "style.xml"
#define _LW6LDR_FILE_TEAMS_XML "teams.xml"

typedef struct _lw6ldr_image_bw_s
{
  int w;
  int h;
  u_int8_t **data;
  int step;
}
_lw6ldr_image_bw_t;

typedef struct _lw6ldr_image_rgba_s
{
  int w;
  int h;
  u_int8_t **data;
}
_lw6ldr_image_rgba_t;


/* ldr-bw.c */
extern int _lw6ldr_bw_read (lw6sys_context_t * sys_context, _lw6ldr_image_bw_t * image, const char *png_file, lw6sys_progress_t * progress);
extern void _lw6ldr_bw_clear (lw6sys_context_t * sys_context, _lw6ldr_image_bw_t * image);
extern float _lw6ldr_bw_gray_level (lw6sys_context_t * sys_context, _lw6ldr_image_bw_t * image);

/* ldr-color.c */
extern int _lw6ldr_guess_colors (lw6sys_context_t * sys_context, lw6map_level_t * level, lw6sys_progress_t * progress);
extern void _lw6ldr_apply_guessed_colors (lw6sys_context_t * sys_context, lw6map_level_t * level);

/* ldr-cursortexturelayer.c */
extern int _lw6ldr_cursor_texture_layer_read_png (lw6sys_context_t * sys_context, lw6map_cursor_texture_layer_t * cursor_texture_layer, const char *png_file);

/* ldr-exp.c */
extern void _lw6ldr_exp_fix (lw6sys_context_t * sys_context, lw6map_rules_t * rules, int exp);

/* ldr-rgba.c */
extern int _lw6ldr_rgba_read_png (lw6sys_context_t * sys_context, _lw6ldr_image_rgba_t * image, const char *png_file, lw6sys_progress_t * progress);
extern int _lw6ldr_rgba_read_jpeg (lw6sys_context_t * sys_context, _lw6ldr_image_rgba_t * image, const char *jpeg_file, lw6sys_progress_t * progress);
extern void _lw6ldr_rgba_clear (lw6sys_context_t * sys_context, _lw6ldr_image_rgba_t * image);

#endif
