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

#ifndef LIQUIDWAR6GEN_H
#define LIQUIDWAR6GEN_H

#include "../sys/sys.h"
#include "../map/map.h"

#define LW6GEN_SEED_CHARS "0123456789abcdefghijklmnopqrstuvwxyz"
#define LW6GEN_SEED_LENGTH 16

/* gen-create.c */
extern lw6map_level_t *lw6gen_create_from_seed (lw6sys_context_t * sys_context, const char *seed, int map_w, int map_h);

/* gen-seed.c */
extern char *lw6gen_seed_new (lw6sys_context_t * sys_context);
extern char *lw6gen_seed_normalize (lw6sys_context_t * sys_context, const char *seed);
extern char lw6gen_seed_char (lw6sys_context_t * sys_context);

/* gen-test.c */
extern int lw6gen_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6gen_test_run (lw6sys_context_t * sys_context, int mode);

#endif
