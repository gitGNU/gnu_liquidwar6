/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6TSK_H
#define LIQUIDWAR6TSK_H

#include "../sys/sys.h"
#include "../map/map.h"
#include "../ker/ker.h"
#include "../cfg/cfg.h"
#include "../hlp/hlp.h"
#include "../ldr/ldr.h"

typedef struct lw6tsk_loader_s
{
  u_int32_t id;
  void *thread;
  void *data;
}
lw6tsk_loader_t;

/* tsk-load.c */
extern void lw6tsk_loader_push (lw6tsk_loader_t * loader, char *map_path,
				char *relative_path,
				lw6sys_assoc_t * default_param,
				lw6sys_assoc_t * forced_param,
				int display_w, int display_h, int bench_value,
				int magic_number);
extern int lw6tsk_loader_pop (lw6map_level_t ** level,
			      lw6ker_game_struct_t ** game_struct,
			      lw6ker_game_state_t ** game_state,
			      lw6tsk_loader_t * loader);

/* tsk-loader.c */
extern lw6tsk_loader_t *lw6tsk_loader_new (float sleep, float *progress);
extern void lw6tsk_loader_free (lw6tsk_loader_t * loader);
extern char *lw6tsk_loader_repr (lw6tsk_loader_t * loader);
extern int lw6tsk_loader_get_stage (lw6tsk_loader_t * loader);

/* tsk-test.c */
extern int lw6tsk_test (int mode);

#endif
