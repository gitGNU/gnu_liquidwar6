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

#include <errno.h>

#include "sys.h"
#include "sys-internal.h"

#if ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
static volatile u_int32_t seq_id = 0;
#else // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
#if LW6_X86 ||  LW6_AMD64
static volatile u_int32_t seq_id = 0;
#endif // LW6_X86 || AMD64
#endif // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)

/**
 * lw6sys_spinlock_create
 *
 * @sys_context: global system context
 *
 * Creates a spinlock object.
 *
 * Return value: newly allocated pointer.
 */
lw6sys_spinlock_t *
lw6sys_spinlock_create (lw6sys_context_t * sys_context)
{
  _lw6sys_spinlock_t *spinlock;
#if ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  spinlock = (_lw6sys_spinlock_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6sys_spinlock_t));
  if (spinlock)
    {
      spinlock->id = 0;
      while (!(spinlock->id))
	{
	  spinlock->id = ++seq_id;
	}

      if (!pthread_spin_init (&(spinlock->spinlock), 0))
	{
	  // OK
	}
      else
	{
	  // should never fail anyway...
	  LW6SYS_FREE (sys_context, spinlock);
	  spinlock = NULL;
	}
    }

  if (!spinlock)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create spinlock"));
    }
#else // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
#if LW6_X86 || LW6_AMD64
  spinlock = (_lw6sys_spinlock_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6sys_spinlock_t));
  if (spinlock)
    {
      spinlock->id = 0;
      while (!(spinlock->id))
	{
	  spinlock->id = ++seq_id;
	}
    }

  if (!spinlock)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create X86 spinlock"));
    }
#else // LW6_X86 || LW6_AMD64
  spinlock = (_lw6sys_spinlock_t *) LW6SYS_MUTEX_CREATE (sys_context);
#endif // LW6_X86 || LW6_AMD64
#endif // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  return (lw6sys_spinlock_t *) spinlock;
}

/**
 * lw6sys_spinlock_destroy
 *
 * @sys_context: global system context
 * @spinlock: the spinlock to destroy.
 *
 * Destroys a spinlock object.
 *
 * Return value: none.
 */
void
lw6sys_spinlock_destroy (lw6sys_context_t * sys_context, lw6sys_spinlock_t * spinlock)
{
#if ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  pthread_spin_destroy (&(((_lw6sys_spinlock_t *) spinlock)->spinlock));
  LW6SYS_FREE (sys_context, (void *) spinlock);
#else // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
#if LW6_X86 || LW6_AMD64
  LW6SYS_FREE (sys_context, (void *) spinlock);
#else // LW6_X86 || LW6_AMD64
  LW6SYS_MUTEX_DESTROY (sys_context, (lw6sys_mutex_t *) spinlock);
#endif // LW6_X86 || LW6_AMD64
#endif // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
}

/**
 * lw6sys_spinlock_lock
 *
 * @sys_context: global system context
 * @spinlock: the spinlock to use
 *
 * Locks the spinlock. Note that this should never fail unless
 * there's a serious initialization problem, instead, function
 * will wait forever until spinlock is released.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6sys_spinlock_lock (lw6sys_context_t * sys_context, lw6sys_spinlock_t * spinlock)
{
  int ret = 0;
#if ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  int pthread_ret;

  pthread_ret = pthread_spin_lock (&(((_lw6sys_spinlock_t *) spinlock)->spinlock));
  if (!pthread_ret)
    {
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("pthread_spin_lock failed with error code %d"), pthread_ret);
    }
#else // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
#if LW6_X86 || LW6_AMD64
  // http://www.cis.temple.edu/~ingargio/cis307/readings/spinsem.html
#ifdef LW6_OPTIMIZE
  while (lw6sys_test_and_set (&(((_lw6sys_spinlock_t *) spinlock)->test_and_set)))
#else // LW6_OPTIMIZE
  while (_lw6sys_test_and_set_asm (&(((_lw6sys_spinlock_t *) spinlock)->test_and_set)))
#endif // LW6_OPTIMIZE
    {
      // eat 100% CPU
    }
  ret = 1;
#else // LW6_X86 || LW6_AMD64
  ret = LW6SYS_MUTEX_LOCK (sys_context, (lw6sys_mutex_t *) spinlock);
#endif // LW6_X86 || LW6_AMD64
#endif // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  return ret;
}

/**
 * lw6sys_spinlock_trylock
 *
 * @sys_context: global system context
 * @spinlock: the spinlock to use
 *
 * Tries to locks the spinlock. That is, tells wether spinlock
 * can be locked immediately or not. Note that this does
 * not mean there's 100% chance next call to lock will
 * terminated immediately, since lock can still be acquired
 * by another thread.
 *
 * Return value: 1 if spinlock unlocked, 0 if locked or error.
 */
int
lw6sys_spinlock_trylock (lw6sys_context_t * sys_context, lw6sys_spinlock_t * spinlock)
{
  int ret = 0;
#if ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  int pthread_ret;

  pthread_ret = pthread_spin_trylock (&(((_lw6sys_spinlock_t *) spinlock)->spinlock));
  if (!pthread_ret)
    {
      ret = 1;
    }
  else
    {
      if (pthread_ret == EBUSY)
	{
	  // OK, we should just wait more
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("pthread_spin_trylock failed with error code %d"), pthread_ret);
	}
    }
#else // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
#if LW6_X86 || LW6_AMD64
  ret = !(((_lw6sys_spinlock_t *) spinlock)->test_and_set);
#else // LW6_X86 || LW6_AMD64
  ret = LW6SYS_MUTEX_TRYLOCK (sys_context, (lw6sys_mutex_t *) spinlock);
#endif // LW6_X86 || LW6_AMD64
#endif // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  return ret;
}

/**
 * lw6sys_spinlock_unlock
 *
 * @sys_context: global system context
 * @spinlock: the spinlock to use
 *
 * Unlocks a spinlock.
 *
 * Return value: 1 if sucess, 0 if error.
 */
int
lw6sys_spinlock_unlock (lw6sys_context_t * sys_context, lw6sys_spinlock_t * spinlock)
{
  int ret = 0;
#if ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  int pthread_ret;

  pthread_ret = pthread_spin_unlock (&(((_lw6sys_spinlock_t *) spinlock)->spinlock));
  if (!pthread_ret)
    {
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("pthread_spin_unlock failed with error code %d"), pthread_ret);
    }
#else // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
#if LW6_X86 || LW6_AMD64
  (((_lw6sys_spinlock_t *) spinlock)->test_and_set) = 0;
  ret = 1;
#else // LW6_X86 || LW6_AMD64
  ret = LW6SYS_MUTEX_UNLOCK (sys_context, (lw6sys_mutex_t *) spinlock);
#endif // LW6_X86 || LW6_AMD64
#endif // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
  return ret;
}
