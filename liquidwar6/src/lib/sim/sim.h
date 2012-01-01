/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6SIM_H
#define LIQUIDWAR6SIM_H

#include "../sys/sys.h"
#include "../map/map.h"
#include "../ker/ker.h"
#include "../pil/pil.h"
#include "../dyn/dyn.h"
#include "../bot/bot.h"

#define LW6SIM_SIMULATE_BASIC_NB_TEAMS 4
#define LW6SIM_SIMULATE_BASIC_BOT_BACKEND "idiot"
#define LW6SIM_SIMULATE_FULL_NB_TEAMS 10
#define LW6SIM_SIMULATE_FULL_BOT_BACKEND "idiot"

typedef struct lw6sim_results_s
{
  int nb_teams;
  int absolute[LW6MAP_MAX_NB_TEAMS];
  float percent[LW6MAP_MAX_NB_TEAMS];
} lw6sim_results_t;

/* sim-print.c */
extern void lw6sim_print (lw6sim_results_t * results, FILE * f);

/* sim-results.c */
extern void lw6sim_results_zero (lw6sim_results_t * results);
extern int lw6sim_results_update_percents (lw6sim_results_t * results);

/* sim-simulate.c */
extern int lw6sim_simulate (int argc, const char *argv[],
			    lw6sim_results_t * results, int nb_teams,
			    char *bot_backend);
extern int lw6sim_simulate_basic (int argc, const char *argv[],
				  lw6sim_results_t * results);
extern int lw6sim_simulate_full (int argc, const char *argv[],
				 lw6sim_results_t * results);

/* sim-test.c */
extern int lw6sim_test (int mode);

#endif
