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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "mod-gles2-internal.h"

void
_mod_gles2_timer_update (_mod_gles2_context_t * gles2_context)
{
  lw6sys_timer_update (&(gles2_context->timer.timestamp),
		       &(gles2_context->timer.uptime),
		       &(gles2_context->timer.cycle));
}

int64_t
_mod_gles2_timer_get_timestamp (_mod_gles2_context_t * gles2_context)
{
  return gles2_context->timer.timestamp;
}

int64_t
_mod_gles2_timer_get_uptime (_mod_gles2_context_t * gles2_context)
{
  return gles2_context->timer.uptime;
}

int32_t
_mod_gles2_timer_get_cycle (_mod_gles2_context_t * gles2_context)
{
  return gles2_context->timer.cycle;
}
