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

#ifndef LIQUIDWAR6TSK_INTERNAL_H
#define LIQUIDWAR6TSK_INTERNAL_H

#include "tsk.h"

#define _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN 0.01f
#define _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN_MAP 0.02f
#define _LW6TSK_LOADER_PROGRESS_STAGE1_END_MAP 0.55f
#define _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN_DUP 0.56f
#define _LW6TSK_LOADER_PROGRESS_STAGE1_END_DUP 0.65f
#define _LW6TSK_LOADER_PROGRESS_STAGE1_END 0.66f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN 0.67f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN_MAP 0.70f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_END_MAP 0.74f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN_STRUCT 0.75f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_END_STRUCT 0.89f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN_STATE 0.90f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_END_STATE 0.99f
#define _LW6TSK_LOADER_PROGRESS_STAGE2_END 1.0f

typedef struct _lw6tsk_loader_stage1_s
{
  char *map_path;
  char *relative_path;
  lw6sys_assoc_t *default_param;
  lw6sys_assoc_t *forced_param;
  char *seed;
  int display_w;
  int display_h;
  int bench_value;
  int magic_number;
  lw6map_level_t *level;
}
_lw6tsk_loader_stage1_t;

typedef struct _lw6tsk_loader_stage2_s
{
  lw6map_level_t *src;
  lw6map_level_t *level;
  lw6ker_game_struct_t *game_struct;
  lw6ker_game_state_t *game_state;
}
_lw6tsk_loader_stage2_t;

typedef struct _lw6tsk_loader_data_s
{
  volatile int stop;
  float sleep;
  char *user_dir;
  lw6sys_mutex_t *mutex;
  volatile int stage;
  volatile int request_number;
  volatile float *progress;
  _lw6tsk_loader_stage1_t stage1;
  _lw6tsk_loader_stage2_t stage2;
}
_lw6tsk_loader_data_t;

#endif
