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

#ifndef LIQUIDWAR6IMG_H
#define LIQUIDWAR6IMG_H

#include "../sys/sys.h"
#include "../cfg/cfg.h"
#include "../map/map.h"
#include "../ker/ker.h"

#define LW6IMG_JPEG_QUALITY_MIN 0
#define LW6IMG_JPEG_QUALITY_MAX 100
#define LW6IMG_JPEG_QUALITY_DEFAULT 85

typedef struct lw6img_jpeg_s
{
  int jpeg_size;
  void *jpeg_data;
}
lw6img_jpeg_t;

/* img-screenshot.c */
extern lw6img_jpeg_t *lw6img_screenshot_new (lw6ker_game_state_t * game_state,
					     char *user_dir, int quality);
extern void lw6img_screenshot_free (lw6img_jpeg_t * screenshot);

/* img-test.c */
extern int lw6img_test (int mode);

#endif
