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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef LW6_CONSOLE
/*
 * Need stdio.h for some old readline headers, see bug #24315 on Savannah
 */
#include <stdio.h>
#include <readline/history.h>
#endif

#include "cns.h"

/**
 * lw6cns_history_add_if_needed
 *
 * @sys_context: global system context
 * @line: line to add
 *
 * Adds a line to the console history, won't add it if
 * it's NULL or empty.
 *
 * Return value: none.
 */
void
lw6cns_history_add_if_needed (lw6sys_context_t * sys_context, char *line)
{
#ifdef LW6_CONSOLE
  if (line && line[0])
    {
      add_history (line);
    }
#endif
}
