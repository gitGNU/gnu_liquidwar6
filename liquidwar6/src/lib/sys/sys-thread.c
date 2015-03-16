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

static volatile u_int32_t seq_id = 0;

/*
 * Could have been declared as static as it is used
 * nowhere but in thread-dedicated code, but it's nice
 * to have this appear in call stacks when backtracing
 * so we let it be a real symbol, even if it fundamentally
 * remains private.
 */
void
_lw6sys_thread_callback (void *thread_handler)
{
  _lw6sys_thread_handler_t *th = (_lw6sys_thread_handler_t *) thread_handler;

  if (th)
    {
      lw6sys_context_t *sys_context = th->sys_context;
      struct timespec ts;
      int yield_timeslice = 0;

      ts.tv_sec = _LW6SYS_PTHREAD_COND_TIMEDWAIT_SEC;
      ts.tv_nsec = _LW6SYS_PTHREAD_COND_TIMEDWAIT_NSEC;

#ifndef LW6_MS_WINDOWS
#ifdef LW6_GPROF
      /*
       * We need this for good interoperation with gprof, read
       * http://sam.zoy.org/writings/programming/gprof.html
       * which explains why one needs to set up an itimer in each
       * thread.
       */
      if (setitimer (ITIMER_PROF, &th->itimer, NULL))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("setitimer failed"));
	}
#endif // LW6_GPROF
#endif // LW6_MS_WINDOWS

      if (th->callback_join)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("begin thread id=%u"), th->id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("begin thread id=%u (fast mode, no join)"), th->id);
	}
      if (th->callback_func)
	{
	  th->callback_func (sys_context, th->callback_data);
	}
      /*
       * callback is over, we signal it to the caller, if needed
       */
      if (!pthread_mutex_lock (&(th->mutex)))
	{
	  th->flag_callback_done = 1;
	  pthread_cond_signal (&(th->cond_callback_done));
	  pthread_mutex_unlock (&(th->mutex));
	}
      else
	{
	  th->flag_callback_done = 1;
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread id=%u"), th->id);
	}
      /*
       * If callback_join is defined, we wait until the caller
       * has called "join" before freeing the ressources. If it's
       * not defined, we just end now. The idea is that callback_join
       * would free ressources that might be usefull in the thread,
       * and queried by the caller. If we're running a stateless
       * thread we just end quickly.
       */
      if (th->callback_join)
	{
	  while (!th->flag_can_join)
	    {
	      /*
	       * Now the caller is supposed to set "can_join"
	       * to allow this thread to actually finish.
	       */
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("waiting for can_join to be 1, thread id=%u"), th->id);
	      if (!pthread_mutex_lock (&(th->mutex)))
		{
		  yield_timeslice = (pthread_cond_timedwait (&(th->cond_can_join), &(th->mutex), &ts) == ETIMEDOUT);
		  pthread_mutex_unlock (&(th->mutex));
		  if (yield_timeslice)
		    {
		      lw6sys_idle (sys_context);
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread id=%u"), th->id);
		}
	    }
	  th->callback_join (sys_context, th->callback_data);
	}
      if (th->callback_join)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("end thread id=%u"), th->id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("end thread id=%u (fast mode, no join)"), th->id);
	}
      pthread_exit (NULL);
    }
}

/**
 * lw6sys_thread_create
 *
 * @sys_context: global system context
 * @callback_func: the main callback, the function that will run the thread
 * @callback_join: function which will be called when joining, at the end
 * @callback_data: data which will be passed to the callback
 *
 * Creates a thread. All threads must be joined. This is because we
 * really do not want the game to leak, and detached threads are typically
 * the kind of thing that leaves stuff in the heap. Note that callback_func
 * is just something which will be called when joining it can be NULL.
 * The idea is to put in it free & delete functions, which you can't
 * call before joining when you want the main thread to get the results
 * of the callback_func.
 *
 * Return value: an opaque pointer on the thread. Can be NULL if failed.
 */
lw6sys_thread_handler_t *
lw6sys_thread_create (lw6sys_context_t * sys_context,
		      lw6sys_thread_callback_func_t callback_func, lw6sys_thread_callback_func_t callback_join, void *callback_data)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  _lw6sys_thread_handler_t *thread_handler = NULL;
  int thread_ok = 0;
  int mutex_ok = 0;
  int cond_callback_done_ok = 0;
  int cond_can_join_ok = 0;

  thread_handler = (_lw6sys_thread_handler_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6sys_thread_handler_t));
  if (thread_handler)
    {
      // callback_done & the rest already set to 0, CALLOC is important
      ++(global->thread_create_counter);
      thread_handler->id = 0;
      while (!(thread_handler->id))
	{
	  thread_handler->id = ++seq_id;
	}
      thread_handler->sys_context = sys_context;
      thread_handler->callback_func = callback_func;
      thread_handler->callback_join = callback_join;
      thread_handler->callback_data = callback_data;

      if (thread_handler->callback_join)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating thread id=%u"), thread_handler->id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("creating thread id=%u (fast mode, no join)"), thread_handler->id);
	}

#ifndef LW6_MS_WINDOWS
#ifdef LW6_GPROF
      /*
       * We need this for good interoperation with gprof, read
       * http://sam.zoy.org/writings/programming/gprof.html
       * which explains why one needs to set up an itimer in each
       * thread.
       */
      if (getitimer (ITIMER_PROF, &thread_handler->itimer))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("getitimer failed"));
	}
#endif
#endif

      if (!pthread_mutex_init (&(thread_handler->mutex), NULL))
	{
	  mutex_ok = 1;
	  if (!pthread_cond_init (&(thread_handler->cond_callback_done), NULL))
	    {
	      cond_callback_done_ok = 1;
	      if (!pthread_cond_init (&(thread_handler->cond_can_join), NULL))
		{
		  cond_can_join_ok = 1;
		  if (!pthread_create (&(thread_handler->thread), NULL, (void *) _lw6sys_thread_callback, (void *) thread_handler))
		    {
		      thread_ok = 1;
		    }
		}
	    }
	}

      if (!thread_ok)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't start thread"));
	  if (mutex_ok)
	    {
	      pthread_mutex_destroy (&(thread_handler->mutex));
	    }
	  if (cond_callback_done_ok)
	    {
	      pthread_cond_destroy (&(thread_handler->cond_callback_done));
	    }
	  if (cond_can_join_ok)
	    {
	      pthread_cond_destroy (&(thread_handler->cond_can_join));
	    }
	  /*
	   * Better do a global->thread_create_counter-- here, this way we
	   * can use a "very probably" atomic operation to increase
	   * it *before* thread creation, ensuring the id is properly
	   * set. Not perfect, but better and simpler than many other
	   * "solutions".
	   */
	  --(global->thread_create_counter);
	  LW6SYS_FREE (sys_context, thread_handler);
	  thread_handler = NULL;
	}
    }

  return (lw6sys_thread_handler_t *) thread_handler;
}

/**
 * lw6sys_thread_is_callback_done
 *
 * @sys_context: global system context
 * @thread_handler: thread to work on
 *
 * Tells wether the callback is done, that is to say, wether the
 * results are available, and we can join.
 *
 * Return value: 1 if done, else 0.
 */
int
lw6sys_thread_is_callback_done (lw6sys_context_t * sys_context, lw6sys_thread_handler_t * thread_handler)
{
  int ret = 0;

  if (thread_handler)
    {
      _lw6sys_thread_handler_t *th;
      th = (_lw6sys_thread_handler_t *) thread_handler;

      ret = th->flag_callback_done;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't call is_callback_done on NULL thread_handler"));
    }

  return ret;
}

/**
 * lw6sys_thread_wait_callback_done
 *
 * @sys_context: global system context
 * @thread_handler: thread to work on
 *
 * Waits until the callback of the thread is done, this does not
 * necessarly mean it's freed, in fact it's not at this stage,
 * the join callback can still be yet to call, but at least the
 * main stuff is done.
 *
 * Return value: 1 if done, 0 on error
 */
int
lw6sys_thread_wait_callback_done (lw6sys_context_t * sys_context, lw6sys_thread_handler_t * thread_handler)
{
  int ret = 0;
  struct timespec ts;

  ts.tv_sec = _LW6SYS_PTHREAD_COND_TIMEDWAIT_SEC;
  ts.tv_nsec = _LW6SYS_PTHREAD_COND_TIMEDWAIT_NSEC;

  if (thread_handler)
    {
      _lw6sys_thread_handler_t *th;
      th = (_lw6sys_thread_handler_t *) thread_handler;

      while (!th->flag_callback_done)
	{
	  if (!pthread_mutex_lock (&(th->mutex)))
	    {
	      pthread_cond_timedwait (&(th->cond_callback_done), &(th->mutex), &ts);
	      pthread_mutex_unlock (&(th->mutex));
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread id=%u"), th->id);
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't call wait_callback_done on NULL thread_handler"));
    }

  return ret;
}

/**
 * lw6sys_thread_get_id
 *
 * @sys_context: global system context
 * @thread_handler: thread to query
 *
 * Returns the id of the thread, this is an internal value,
 * unique for each process, which can help identifying the thread.
 *
 * Return value: the id, should be >0.
 */
int
lw6sys_thread_get_id (lw6sys_context_t * sys_context, lw6sys_thread_handler_t * thread_handler)
{
  int ret = 0;

  if (thread_handler)
    {
      _lw6sys_thread_handler_t *th;
      th = (_lw6sys_thread_handler_t *) thread_handler;

      ret = th->id;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't call get_id on NULL thread_handler"));
    }

  return ret;
}

/**
 * lw6sys_thread_get_data
 *
 * @sys_context: global system context
 * @thread_handler: thread to query
 *
 * Returns the data associated to the thread, that is, the pointer
 * which was passed to the callback function.
 *
 * Return value: a pointer.
 */
void *
lw6sys_thread_get_data (lw6sys_context_t * sys_context, lw6sys_thread_handler_t * thread_handler)
{
  void *ret = NULL;

  if (thread_handler)
    {
      _lw6sys_thread_handler_t *th;
      th = (_lw6sys_thread_handler_t *) thread_handler;

      ret = th->callback_data;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't call get_data on NULL thread_handler"));
    }

  return ret;
}

/**
 * lw6sys_thread_join
 *
 * @sys_context: global system context
 * @thread_handler: thread to end
 *
 * Joins the thread, that's to say wait until the thread is over,
 * and destroys the ressources associated to it. Note that if the thread
 * is looping forever, this function will just wait forever.
 * This is the only way to end a thread.
 *
 * Return value: none.
 */
void
lw6sys_thread_join (lw6sys_context_t * sys_context, lw6sys_thread_handler_t * thread_handler)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (thread_handler)
    {
      _lw6sys_thread_handler_t *th;
      th = (_lw6sys_thread_handler_t *) thread_handler;

      if (th->callback_join)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joining thread id=%u"), th->id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("joining thread id=%u (fast mode, no join)"), th->id);
	}

      if (!pthread_mutex_lock (&(th->mutex)))
	{
	  th->flag_can_join = 1;
	  pthread_cond_signal (&(th->cond_can_join));
	  pthread_mutex_unlock (&(th->mutex));
	}
      else
	{
	  th->flag_can_join = 1;
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread id=%u"), th->id);
	}

      if (!pthread_join (th->thread, NULL))
	{
	  if (!pthread_mutex_lock (&(th->mutex)))
	    {
	      pthread_cond_destroy (&(th->cond_callback_done));
	      pthread_cond_destroy (&(th->cond_can_join));
	      pthread_mutex_unlock (&(th->mutex));
	      pthread_mutex_destroy (&(th->mutex));
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread id=%u"), th->id);
	    }
	  LW6SYS_FREE (sys_context, th);
	  ++(global->thread_join_counter);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't join thread id=%u"), th->id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't call join on NULL thread_handler"));
    }
}

/**
 * lw6sys_get_thread_create_count
 *
 * @sys_context: global system context
 *
 * Utility function used to check how many threads where created and joined.
 *
 * Return value: how many threads were created.
 */
int
lw6sys_get_thread_create_count (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  return global->thread_create_counter;
}

/**
 * lw6sys_get_thread_join_count
 *
 * @sys_context: global system context
 *
 * Utility function used to check how many threads where created and joined.
 *
 * Return value: how many threads were joined.
 */
int
lw6sys_get_thread_join_count (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  return global->thread_join_counter;
}

/**
 * lw6sys_check_thread_count
 *
 * @sys_context: global system context
 *
 * Utility function used to check how many threads where created and joined.
 * This one will compare the results of @lw6sys_get_thread_create_count
 * and @lw6sys_get_thread_join_count.
 *
 * Return value: 1 if both are equals, 0 if not (error...).
 */
int
lw6sys_check_thread_count (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  int ret = 1;

  if (global->thread_create_counter != global->thread_join_counter)
    {
      ret = 0;
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("possible thread problem, %d threads have been started, but only %d threads have been joined"),
		  global->thread_create_counter, global->thread_join_counter);
    }

  return ret;
}
