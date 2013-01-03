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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <errno.h>

#include "mod-caca-internal.h"

lw6gui_input_t *
_mod_caca_pump_events (_mod_caca_context_t * caca_context)
{
  lw6gui_input_t *ret = NULL;
  caca_event_t event;

  if (caca_context)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("pumping libcaca events timeout %d microseconds"),
		  caca_context->const_data.event_timeout_microseconds);

      // todo : pump them for good!

      caca_get_event (caca_context->display, CACA_EVENT_KEY_PRESS, &event,
		      caca_context->const_data.event_timeout_microseconds);
      ret = &(caca_context->input);
    }

  return ret;
}
