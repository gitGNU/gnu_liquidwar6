/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#define THREAD_JOIN_SLEEP 0.1f

static u_int32_t seq_id = 0;
static int mutex_lock_counter = 0;
static int mutex_unlock_counter = 0;

/**
 * lw6sys_mutex_create
 *
 * Creates a mutex object.
 *
 * Return value: newly allocated pointer.
 */
lw6sys_mutex_t *
lw6sys_mutex_create ()
{
  _lw6sys_mutex_t *mutex;

  mutex = (_lw6sys_mutex_t *) LW6SYS_CALLOC (sizeof (_lw6sys_mutex_t));
  if (mutex)
    {
      mutex->id = 0;
      while (mutex->id)
	{
	  mutex->id = ++seq_id;
	}

      if (!pthread_mutex_init (&(mutex->mutex), NULL))
	{
	  // OK
	}
      else
	{
	  // should never fail anyway...
	  LW6SYS_FREE (mutex);
	  mutex = NULL;
	}
    }

  if (!mutex)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create mutex"));
    }

  return (lw6sys_mutex_t *) mutex;
}

/**
 * lw6sys_mutex_destroy
 *
 * @mutex: the mutex to destroy.
 *
 * Destroys a mutex object.
 *
 * Return value: none.
 */
void
lw6sys_mutex_destroy (lw6sys_mutex_t * mutex)
{
  pthread_mutex_destroy (&(((_lw6sys_mutex_t *) mutex)->mutex));
  LW6SYS_FREE (mutex);
}

/**
 * lw6sys_mutex_lock
 *
 * @mutex: the mutex to use
 *
 * Locks the mutex. Note that this should never fail unless
 * there's a serious initialization problem, instead, function
 * will wait forever until mutex is released.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6sys_mutex_lock (lw6sys_mutex_t * mutex)
{
  int ret = 0;
  int pthread_ret;

  pthread_ret = pthread_mutex_lock (&(((_lw6sys_mutex_t *) mutex)->mutex));
  if (!pthread_ret)
    {
      mutex_lock_counter++;
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("pthread_mutex_lock failed with error code %d"),
		  pthread_ret);
    }

  return ret;
}

/**
 * lw6sys_mutex_trylock
 *
 * @mutex: the mutex to use
 *
 * Tries to locks the mutex. That is, tells wether mutex
 * can be locked immediately or not. Note that this does
 * not mean there's 100% chance next call to lock will
 * terminated immediately, since lock can still be acquired
 * by another thread.
 *
 * Return value: 1 if mutex unlocked, 0 if locked or error.
 */
int
lw6sys_mutex_trylock (lw6sys_mutex_t * mutex)
{
  int ret = 0;
  int pthread_ret;

  pthread_ret = pthread_mutex_trylock (&(((_lw6sys_mutex_t *) mutex)->mutex));
  if (!pthread_ret)
    {
      mutex_lock_counter++;
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
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("pthread_mutex_trylock failed with error code %d"),
		      pthread_ret);
	}
    }

  return ret;
}

/**
 * lw6sys_mutex_unlock
 *
 * @mutex: the mutex to use
 *
 * Unlocks a mutex.
 *
 * Return value: 1 if sucess, 0 if error.
 */
int
lw6sys_mutex_unlock (lw6sys_mutex_t * mutex)
{
  int ret = 0;
  int pthread_ret;

  pthread_ret = pthread_mutex_unlock (&(((_lw6sys_mutex_t *) mutex)->mutex));
  if (!pthread_ret)
    {
      mutex_lock_counter++;
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("pthread_mutex_unlock failed with error code %d"),
		  pthread_ret);
    }

  return ret;
}

/**
 * lw6sys_get_mutex_lock_count
 *
 * Returns how many mutexes have been locked since program start.
 * Usefull for sanity checking when debugging.
 *
 * Return value: number of calls to lock
 */
int
lw6sys_get_mutex_lock_count ()
{
  return mutex_lock_counter;
}

/**
 * lw6sys_get_mutex_unlock_count
 *
 * Returns how many mutexes have been unlocked since program start.
 * Usefull for sanity checking when debugging.
 *
 * Return value: number of calls to unlock
 */
int
lw6sys_get_mutex_unlock_count ()
{
  return mutex_unlock_counter;
}

/**
 * lw6sys_check_mutex_count
 *
 * Checks wether unlock has been called as many times as lock.
 * Usefull for sanity checking when debugging.
 *
 * Return value: 1 if OK, 0 if inconsistency.
 */
int
lw6sys_check_mutex_count ()
{
  int ret = 1;

  if (mutex_lock_counter != mutex_unlock_counter)
    {
      ret = 0;
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("possible mutex problem, %d mutexes have been locked, but only %d mutexes have been unlocked"),
		  mutex_lock_counter, mutex_unlock_counter);
    }

  return ret;
}
