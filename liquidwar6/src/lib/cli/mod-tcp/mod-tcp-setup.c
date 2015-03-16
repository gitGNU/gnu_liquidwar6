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

#include "../cli.h"
#include "mod-tcp-internal.h"

_mod_tcp_context_t *
_mod_tcp_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6cnx_properties_t * properties)
{
  _mod_tcp_context_t *tcp_context = NULL;
  char *data_dir = NULL;
  int ok = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("tcp init"));

  tcp_context = (_mod_tcp_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_tcp_context_t));
  if (tcp_context)
    {
      data_dir = lw6sys_get_data_dir (sys_context, argc, argv);
      if (data_dir)
	{
	  if (_mod_tcp_load_data (sys_context, &(tcp_context->data), data_dir))
	    {
	      properties->hint_timeout = tcp_context->data.consts.global_timeout;
	      properties->ping_alter_base = tcp_context->data.consts.ping_alter_base;
	      properties->ping_alter_percent = tcp_context->data.consts.ping_alter_percent;
	      properties->reliable = 1;
	      properties->backend_id = MOD_TCP_BACKEND_ID;
	      ok = 1;
	    }
	  LW6SYS_FREE (sys_context, data_dir);
	}
      if (!ok)
	{
	  _mod_tcp_quit (sys_context, tcp_context);
	  tcp_context = NULL;
	}
    }

  if (!tcp_context)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("can't initialize mod_tcp"));
    }

  return tcp_context;
}

void
_mod_tcp_quit (lw6sys_context_t * sys_context, _mod_tcp_context_t * tcp_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("tcp quit"));
  _mod_tcp_unload_data (sys_context, &(tcp_context->data));
  LW6SYS_FREE (sys_context, tcp_context);
}
