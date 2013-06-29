/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6CNS_H
#define LIQUIDWAR6CNS_H

#include "../sys/sys.h"

typedef void (*lw6cns_callback_func_t) (char *line);

/* cns-handler.c */
extern void lw6cns_handler_install (lw6cns_callback_func_t callback);
extern void lw6cns_handler_poll ();
extern void lw6cns_handler_remove ();

/* cns-history.c */
extern void lw6cns_history_add_if_needed (char *line);

/* cns-support.c */
extern int lw6cns_support ();

/* cns-test.c */
extern int lw6cns_test_register (int mode);
extern int lw6cns_test_run (int mode);

#endif
