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

#ifndef LIQUIDWAR6IMG_H
#define LIQUIDWAR6IMG_H

#include "../sys/sys.h"
#include "../cfg/cfg.h"
#include "../map/map.h"
#include "../ker/ker.h"

#define LW6IMG_JPEG_QUALITY_MIN 0
#define LW6IMG_JPEG_QUALITY_MAX 100
#define LW6IMG_JPEG_QUALITY_DEFAULT 85

/**
 * Contains informations about a (loaded) JPEG file.
 */
typedef struct lw6img_jpeg_s
{
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /// JPEG file shape, only w and h are relevant.
  lw6sys_whd_t shape;
  /// JPEG size (file size, same as data buffer size).
  int jpeg_size;
  /// JPEG raw data.
  void *jpeg_data;
}
lw6img_jpeg_t;

/* img-repr.c */
extern char *lw6img_repr (lw6sys_context_t * sys_context, const lw6img_jpeg_t * jpeg);

/* img-screenshot.c */
extern lw6img_jpeg_t *lw6img_screenshot_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, char *user_dir, int quality);
extern void lw6img_screenshot_free (lw6sys_context_t * sys_context, lw6img_jpeg_t * screenshot);

/* img-test.c */
extern int lw6img_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6img_test_run (lw6sys_context_t * sys_context, int mode);

#endif
