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

#include "net.h"
#include "net-internal.h"

#define _NET_DIR "net"
#define _TCP_RECV_FILE "tcp-recv.log"
#define _TCP_SEND_FILE "tcp-send.log"
#define _UDP_RECV_FILE "udp-recv.log"
#define _UDP_SEND_FILE "udp-send.log"

int
_lw6net_counters_init (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6net_counters_t * counters)
{
  int ret = 0;

  counters->spinlock = lw6sys_spinlock_create (sys_context);
  if (counters->spinlock)
    {
      ret = 1;
    }

  return ret;
}

void
_lw6net_counters_quit (lw6sys_context_t * sys_context, _lw6net_counters_t * counters)
{
  if (counters->open_counter < counters->close_counter)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("%d sockets opened, but %d closed, there's probably a bug"), counters->open_counter, counters->close_counter);
    }
  if (counters->open_counter > counters->close_counter)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("%d sockets opened, but only %d closed"), counters->open_counter, counters->close_counter);
    }
  if (counters->close_counter == counters->open_counter)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("%d sockets opened and closed"), counters->open_counter);
    }
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("%d kb sent"), lw6net_counters_get_sent_kbytes (sys_context));
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("%d kb received"), lw6net_counters_get_received_kbytes (sys_context));

  /*
   * Only free spinlock now as functions above might need it
   */
  if (counters->spinlock)
    {
      lw6sys_spinlock_destroy (sys_context, counters->spinlock);
    }

  memset (counters, 0, sizeof (_lw6net_counters_t));
}

void
_lw6net_counters_register_socket (lw6sys_context_t * sys_context, _lw6net_counters_t * counters)
{
  if (lw6sys_spinlock_lock (sys_context, counters->spinlock))
    {
      counters->open_counter++;
      lw6sys_spinlock_unlock (sys_context, counters->spinlock);
    }
}

void
_lw6net_counters_unregister_socket (lw6sys_context_t * sys_context, _lw6net_counters_t * counters)
{
  if (lw6sys_spinlock_lock (sys_context, counters->spinlock))
    {
      counters->close_counter++;
      lw6sys_spinlock_unlock (sys_context, counters->spinlock);
    }
}

void
_lw6net_counters_register_send (lw6sys_context_t * sys_context, _lw6net_counters_t * counters, int bytes)
{
  /*
   * It's interesting to use the lock since the counter field is 64 bits
   * so it's not an atomic update on 32-bit machines.
   */
  if (lw6sys_spinlock_lock (sys_context, counters->spinlock))
    {
      counters->sent_bytes += bytes;
      lw6sys_spinlock_unlock (sys_context, counters->spinlock);
    }
}

void
_lw6net_counters_register_recv (lw6sys_context_t * sys_context, _lw6net_counters_t * counters, int bytes)
{
  /*
   * It's interesting to use the lock since the counter field is 64 bits
   * so it's not an atomic update on 32-bit machines.
   */
  if (lw6sys_spinlock_lock (sys_context, counters->spinlock))
    {
      counters->received_bytes += bytes;
      lw6sys_spinlock_unlock (sys_context, counters->spinlock);
    }
}

int
lw6net_counters_get_sent_kbytes (lw6sys_context_t * sys_context)
{
  int ret = 0;
  _lw6net_counters_t *counters = &(_lw6net_global_context->counters);

  if (lw6sys_spinlock_lock (sys_context, counters->spinlock))
    {
      ret = counters->sent_bytes / 1024LL;
      lw6sys_spinlock_unlock (sys_context, counters->spinlock);
    }

  return ret;
}

int
lw6net_counters_get_received_kbytes (lw6sys_context_t * sys_context)
{
  int ret = 0;
  _lw6net_counters_t *counters = &(_lw6net_global_context->counters);

  if (lw6sys_spinlock_lock (sys_context, counters->spinlock))
    {
      ret = counters->received_bytes / 1024LL;
      lw6sys_spinlock_unlock (sys_context, counters->spinlock);
    }

  return ret;
}
