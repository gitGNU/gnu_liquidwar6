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
#endif // HAVE_CONFIG_H

#include "pil.h"
#include "pil-internal.h"

/**
 * lw6pil_nopilot_poll_dump
 *
 * @sys_context: global system context
 * @dump: will contain the dump information if needed, can be NULL
 * @command_text: the command received from the network.
 * @timestamp: timestamp, used to initialize the pilot
 *
 * Normally, it's the commit function of the pilot which will automatically
 * return a dump if needed. But... when a client connects, at first, it has
 * absolutely no pilot/map whatsoever yet, so this is just to bootstrap the
 * process, this function will take network messages, any message, and if
 * it's about a dump -> it will create the dump.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6pil_nopilot_poll_dump (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, const char *command_text, int64_t timestamp)
{
  int ret = 0;

  /*
   * Yes, we need to set dump to zero, else we might return
   * false positives...
   */
  lw6pil_dump_zero (sys_context, dump);

  ret = lw6pil_command_execute_text (sys_context, dump, timestamp, NULL, command_text, _LW6PIL_MIN_SEQ_0);

  return ret;
}
