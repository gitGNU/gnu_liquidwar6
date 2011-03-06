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

#ifndef LIQUIDWAR6LDR_INTERNAL_H
#define LIQUIDWAR6LDR_INTERNAL_H

#define _LW6LDR_FILE_README "README"
#define _LW6LDR_FILE_README_TXT "readme.txt"
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

#define _LW6LDR_FILE_RULES_XML "rules.xml"
#define _LW6LDR_FILE_HINTS_XML "hints.xml"
#define _LW6LDR_FILE_STYLE_XML "style.xml"

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


/*
 * In bw.c
 */
extern int _lw6ldr_bw_read (_lw6ldr_image_bw_t * image, char *png_file,
			    lw6sys_progress_t * progress);
extern void _lw6ldr_bw_clear (_lw6ldr_image_bw_t * image);
extern float _lw6ldr_bw_gray_level (_lw6ldr_image_bw_t * image);

/*
 * In color.c
 */
extern int _lw6ldr_guess_colors (lw6map_level_t * level,
				 lw6sys_progress_t * progress);
extern void _lw6ldr_apply_guessed_colors (lw6map_level_t * level);

/*
 * In rgba.c
 */
extern int _lw6ldr_rgba_read_png (_lw6ldr_image_rgba_t * image,
				  char *png_file,
				  lw6sys_progress_t * progress);
extern int _lw6ldr_rgba_read_jpeg (_lw6ldr_image_rgba_t * image,
				   char *jpeg_file,
				   lw6sys_progress_t * progress);
extern void _lw6ldr_rgba_clear (_lw6ldr_image_rgba_t * image);

#endif
