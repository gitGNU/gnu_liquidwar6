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

#define THREAD_JOIN_SLEEP 0.1f

static volatile u_int32_t seq_id = 0;

/**
 * lw6sys_mutex_create
 *
 * @sys_context: global system context
 * @file: the name of the source file where the function is called,
 *   one can use __FILE__
 * @line: the line in the source file where the function is called,
 *   one can use __LINE__
 * @func: the name of the function where this log line was called,
 *   on can use __FUNCTION__
 *
 * Creates a mutex object.
 *
 * Return value: newly allocated pointer.
 */
lw6sys_mutex_t *
lw6sys_mutex_create (lw6sys_context_t * sys_context, const char *file, int line, const char *func)
{
  _lw6sys_mutex_t *_mutex;

  _mutex = (_lw6sys_mutex_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6sys_mutex_t));
  if (_mutex)
    {
      _mutex->id = 0;
      while (!(_mutex->id))
	{
	  _mutex->id = ++seq_id;
	}

      if (!pthread_mutex_init (&(_mutex->mutex), NULL))
	{
#ifndef LW6_OPTIMIZE
	  _lw6sys_caller_info_set (&(_mutex->create_info), _lw6sys_path_file_only_raw (file), line, func);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mutex %u created from %s:%d %s()"), _mutex->id, _mutex->create_info.file, _mutex->create_info.line,
		      _mutex->create_info.func);
#endif // LW6_OPTIMIZE
	}
      else
	{
	  // should never fail anyway...
	  LW6SYS_FREE (sys_context, _mutex);
	  _mutex = NULL;
	}
    }

  if (!_mutex)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create mutex"));
    }

  return (lw6sys_mutex_t *) _mutex;
}

/**
 * lw6sys_mutex_destroy
 *
 * @sys_context: global system context
 * @mutex: the mutex to destroy.
 * @file: the name of the source file where the function is called,
 *   one can use __FILE__
 * @line: the line in the source file where the function is called,
 *   one can use __LINE__
 * @func: the name of the function where this log line was called,
 *   on can use __FUNCTION__
 *
 * Destroys a mutex object.
 *
 * Return value: none.
 */
void
lw6sys_mutex_destroy (lw6sys_context_t * sys_context, lw6sys_mutex_t * mutex, const char *file, int line, const char *func)
{
  _lw6sys_mutex_t *_mutex = (_lw6sys_mutex_t *) mutex;

#ifndef LW6_OPTIMIZE
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("destroying mutex %u from %s:%d %s(), was created from %s:%d %s()"), _mutex->id,
	      _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func);
#endif // LW6_OPTIMIZE
  pthread_mutex_destroy (&(_mutex->mutex));
  LW6SYS_FREE (sys_context, mutex);
}

/**
 * lw6sys_mutex_lock
 *
 * @sys_context: global system context
 * @mutex: the mutex to use
 * @file: the name of the source file where the function is called,
 *   one can use __FILE__
 * @line: the line in the source file where the function is called,
 *   one can use __LINE__
 * @func: the name of the function where this log line was called,
 *   on can use __FUNCTION__
 *
 * Locks the mutex. Note that this should never fail unless
 * there's a serious initialization problem, instead, function
 * will wait forever until mutex is released.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6sys_mutex_lock (lw6sys_context_t * sys_context, lw6sys_mutex_t * mutex, const char *file, int line, const char *func)
{
  int ret = 0;
  int pthread_ret;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  _lw6sys_mutex_t *_mutex = (_lw6sys_mutex_t *) mutex;

#ifndef LW6_OPTIMIZE
  if (_mutex->lock_info.line)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("locking mutex %u from %s:%d %s(), was created from %s:%d %s(), could be locked from %s:%d %s()"),
		  _mutex->id, _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func,
		  _mutex->lock_info.file, _mutex->lock_info.line, _mutex->lock_info.func);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("locking mutex %u from %s:%d %s(), was created from %s:%d %s(), could be unlocked"), _mutex->id, file,
		  line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func);
    }
#endif // LW6_OPTIMIZE

  pthread_ret = pthread_mutex_lock (&(_mutex->mutex));
  if (!pthread_ret)
    {
#ifndef LW6_OPTIMIZE
      if (_mutex->lock_info.line)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("locked mutex %u from %s:%d %s(), was created from %s:%d %s(), was probably locked from %s:%d %s()"),
		      _mutex->id, _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func,
		      _mutex->lock_info.file, _mutex->lock_info.line, _mutex->lock_info.func);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("locked mutex %u from %s:%d %s(), was created from %s:%d %s(), was probably unlocked"), _mutex->id,
		      _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func);
	}

      _lw6sys_caller_info_set (&(_mutex->lock_info), _lw6sys_path_file_only_raw (file), line, func);
#endif // LW6_OPTIMIZE

      global->mutex_lock_counter++;
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("pthread_mutex_lock failed with error code %d"), pthread_ret);
    }

  return ret;
}

/**
 * lw6sys_mutex_trylock
 *
 * @sys_context: global system context
 * @mutex: the mutex to use
 * @file: the name of the source file where the function is called,
 *   one can use __FILE__
 * @line: the line in the source file where the function is called,
 *   one can use __LINE__
 * @func: the name of the function where this log line was called,
 *   on can use __FUNCTION__
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
lw6sys_mutex_trylock (lw6sys_context_t * sys_context, lw6sys_mutex_t * mutex, const char *file, int line, const char *func)
{
  int ret = 0;
  int pthread_ret;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  _lw6sys_mutex_t *_mutex = (_lw6sys_mutex_t *) mutex;

#ifndef LW6_OPTIMIZE
  if (_mutex->lock_info.line)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("trying to lock mutex %u from %s:%d %s(), was created from %s:%d %s(), could be locked from %s:%d %s()"),
		  _mutex->id, _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func,
		  _mutex->lock_info.file, _mutex->lock_info.line, _mutex->lock_info.func);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("trying to lock mutex %u from %s:%d %s(), was created from %s:%d %s(), could be unlocked"), _mutex->id,
		  _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func);
    }
#endif // LW6_OPTIMIZE

  pthread_ret = pthread_mutex_trylock (&(_mutex->mutex));
  if (!pthread_ret)
    {
#ifndef LW6_OPTIMIZE
      if (_mutex->lock_info.line)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("locked mutex %u from %s:%d %s(), was created from %s:%d %s(), was probably locked from %s:%d %s()"),
		      _mutex->id, _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func,
		      _mutex->lock_info.file, _mutex->lock_info.line, _mutex->lock_info.func);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("locked mutex %u from %s:%d %s(), was created from %s:%d %s(), was probably unlocked"), _mutex->id,
		      _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func);
	}

      _lw6sys_caller_info_set (&(_mutex->lock_info), _lw6sys_path_file_only_raw (file), line, func);
#endif // LW6_OPTIMIZE

      global->mutex_lock_counter++;
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
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("pthread_mutex_trylock failed with error code %d"), pthread_ret);
	}
    }

  return ret;
}

/**
 * lw6sys_mutex_unlock
 *
 * @sys_context: global system context
 * @mutex: the mutex to use
 * @file: the name of the source file where the function is called,
 *   one can use __FILE__
 * @line: the line in the source file where the function is called,
 *   one can use __LINE__
 * @func: the name of the function where this log line was called,
 *   on can use __FUNCTION__
 *
 * Unlocks a mutex.
 *
 * Return value: 1 if sucess, 0 if error.
 */
int
lw6sys_mutex_unlock (lw6sys_context_t * sys_context, lw6sys_mutex_t * mutex, const char *file, int line, const char *func)
{
  int ret = 0;
  int pthread_ret;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  _lw6sys_mutex_t *_mutex = (_lw6sys_mutex_t *) mutex;

#ifndef LW6_OPTIMIZE
  if (_mutex->lock_info.line)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unlocking mutex %u from %s:%d %s(), was created from %s:%d %s(), could be locked from %s:%d %s()"),
		  _mutex->id, _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func,
		  _mutex->lock_info.file, _mutex->lock_info.line, _mutex->lock_info.func);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unlocking mutex %u from %s:%d %s(), was created from %s:%d %s()"), _mutex->id,
		  _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func);
    }
#endif // LW6_OPTIMIZE

  pthread_ret = pthread_mutex_unlock (&(_mutex->mutex));
  if (!pthread_ret)
    {
#ifndef LW6_OPTIMIZE
      if (_mutex->lock_info.line)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_ ("unlocked mutex %u from %s:%d %s(), was created from %s:%d %s(), was probably locked from %s:%d %s()"), _mutex->id,
		      _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func,
		      _mutex->lock_info.file, _mutex->lock_info.line, _mutex->lock_info.func);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unlocked mutex %u from %s:%d %s(), was created from %s:%d %s(), was probably unlocked"), _mutex->id,
		      _lw6sys_path_file_only_raw (file), line, func, _mutex->create_info.file, _mutex->create_info.line, _mutex->create_info.func);
	}

      _lw6sys_caller_info_clear (&(_mutex->lock_info));
#endif // LW6_OPTIMIZE

      global->mutex_lock_counter++;
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("pthread_mutex_unlock failed with error code %d"), pthread_ret);
    }

  return ret;
}

/**
 * lw6sys_get_mutex_lock_count
 *
 * @sys_context: global system context
 * @file: the name of the source file where the function is called,
 *   one can use __FILE__
 * @line: the line in the source file where the function is called,
 *   one can use __LINE__
 * @func: the name of the function where this log line was called,
 *   on can use __FUNCTION__
 *
 * Returns how many mutexes have been locked since program start.
 * Usefull for sanity checking when debugging.
 *
 * Return value: number of calls to lock
 */
int
lw6sys_get_mutex_lock_count (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  return global->mutex_lock_counter;
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
lw6sys_get_mutex_unlock_count (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  return global->mutex_unlock_counter;
}

/**
 * lw6sys_check_mutex_count
 *
 * @sys_context: global system context
 *
 * Checks wether unlock has been called as many times as lock.
 * Usefull for sanity checking when debugging.
 *
 * Return value: 1 if OK, 0 if inconsistency.
 */
int
lw6sys_check_mutex_count (lw6sys_context_t * sys_context)
{
  int ret = 1;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (global->mutex_lock_counter != global->mutex_unlock_counter)
    {
      ret = 0;
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("possible mutex problem, %d mutexes have been locked, but only %d mutexes have been unlocked"),
		  global->mutex_lock_counter, global->mutex_unlock_counter);
    }

  return ret;
}
