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

#include "sys.h"
#include "sys-internal.h"

static int thread_create_counter = 0;
static int thread_join_counter = 0;
static u_int32_t seq_id = 0;

static void
thread_callback (void *thread_handler)
{
  _lw6sys_thread_handler_t *th;
  th = (_lw6sys_thread_handler_t *) thread_handler;

  if (th)
    {
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
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("setitimer failed"));
	}
#endif
#endif

      if (th->callback_join)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("begin thread id=%u"), th->id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("begin thread id=%u (fast mode, no join)"),
		      th->id);
	}
      if (th->callback_func)
	{
	  th->callback_func (th->callback_data);
	}
      /* 
       * callback is over, we signal it to the caller, if needed
       */
      th->callback_done = 1;
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
	  while (!th->can_join)
	    {
	      /*
	       * Now the caller is supposed to set "can_join"
	       * to allow this thread to actually finish.
	       */
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_ ("waiting for can_join to be 1, thread id=%u"),
			  th->id);
	      lw6sys_idle ();
	    }
	  th->callback_join (th->callback_data);
	}
      if (th->callback_join)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("end thread id=%u"), th->id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("end thread id=%u (fast mode, no join)"), th->id);
	}
      pthread_exit (NULL);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't call thread_callback on NULL thread_handler"));
    }
}

/**
 * lw6sys_thread_create 
 *
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
lw6sys_thread_create (lw6sys_thread_callback_func_t callback_func,
		      lw6sys_thread_callback_func_t callback_join,
		      void *callback_data)
{
  _lw6sys_thread_handler_t *thread_handler = NULL;

  thread_handler =
    (_lw6sys_thread_handler_t *)
    LW6SYS_CALLOC (sizeof (_lw6sys_thread_handler_t));
  if (thread_handler)
    {
      // callback_done & the rest already set to 0
      ++thread_create_counter;
      thread_handler->id = 0;
      while (thread_handler->id)
	{
	  thread_handler->id = ++seq_id;
	}
      thread_handler->callback_func = callback_func;
      thread_handler->callback_join = callback_join;
      thread_handler->callback_data = callback_data;

      if (thread_handler->callback_join)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("creating thread id=%u"),
		      thread_handler->id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("creating thread id=%u (fast mode, no join)"),
		      thread_handler->id);
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
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("getitimer failed"));
	}
#endif
#endif

      if (!pthread_create (&(thread_handler->thread), NULL,
			   (void *) thread_callback, (void *) thread_handler))
	{
	  // OK, thread created
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("can't start thread"));
	  /*
	   * Better do a thread_create_counter-- here, this way we
	   * can use a "very probably" atomic operation to increase
	   * it *before* thread creation, ensuring the id is properly
	   * set. Not perfect, but better and simpler than many other
	   * "solutions".
	   */
	  thread_create_counter--;
	  LW6SYS_FREE (thread_handler);
	  thread_handler = NULL;
	}
    }

  return (lw6sys_thread_handler_t *) thread_handler;
}

/**
 * lw6sys_thread_is_callback_done
 *
 * @thread_handler: thread to work on
 *
 * Tells wether the callback is done, that is to say, wether the
 * results are available, and we can join.
 *
 * Return value: 1 if done, else 0.
 */
int
lw6sys_thread_is_callback_done (lw6sys_thread_handler_t * thread_handler)
{
  int ret = 0;

  if (thread_handler)
    {
      _lw6sys_thread_handler_t *th;
      th = (_lw6sys_thread_handler_t *) thread_handler;

      ret = th->callback_done;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't call is_callback_done on NULL thread_handler"));
    }

  return ret;
}

/**
 * lw6sys_thread_get_id
 *
 * @thread_handler: thread to query
 *
 * Returns the id of the thread, this is an internal value,
 * unique for each process, which can help identifying the thread.
 *
 * Return value: the id, should be >0.
 */
int
lw6sys_thread_get_id (lw6sys_thread_handler_t * thread_handler)
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
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't call get_id on NULL thread_handler"));
    }

  return ret;
}

/**
 * lw6sys_thread_get_data
 *
 * @thread_handler: thread to query
 *
 * Returns the data associated to the thread, that is, the pointer
 * which was passed to the callback function.
 *
 * Return value: a pointer.
 */
void *
lw6sys_thread_get_data (lw6sys_thread_handler_t * thread_handler)
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
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't call get_data on NULL thread_handler"));
    }

  return ret;
}

/**
 * lw6sys_thread_join
 *
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
lw6sys_thread_join (lw6sys_thread_handler_t * thread_handler)
{
  if (thread_handler)
    {
      _lw6sys_thread_handler_t *th;
      th = (_lw6sys_thread_handler_t *) thread_handler;

      if (th->callback_join)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("joining thread id=%u"), th->id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("joining thread id=%u (fast mode, no join)"),
		      th->id);
	}

      th->can_join = 1;

      if (!pthread_join (th->thread, NULL))
	{
	  LW6SYS_FREE (th);
	  thread_join_counter++;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("can't join thread id=%u"),
		      th->id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't call join on NULL thread_handler"));
    }
}

/**
 * lw6sys_get_thread_create_count
 *
 * Utility function used to check how many threads where created and joined.
 *
 * Return value: how many threads were created.
 */
int
lw6sys_get_thread_create_count ()
{
  return thread_create_counter;
}

/**
 * lw6sys_get_thread_join_count
 *
 * Utility function used to check how many threads where created and joined.
 *
 * Return value: how many threads were joined.
 */
int
lw6sys_get_thread_join_count ()
{
  return thread_join_counter;
}

/**
 * lw6sys_check_thread_count
 *
 * Utility function used to check how many threads where created and joined.
 * This one will compare the results of @lw6sys_get_thread_create_count
 * and @lw6sys_get_thread_join_count.
 *
 * Return value: 1 if both are equals, 0 if not (error...).
 */
int
lw6sys_check_thread_count ()
{
  int ret = 1;

  if (thread_create_counter != thread_join_counter)
    {
      ret = 0;
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("possible thread problem, %d threads have been started, but only %d threads have been joined"),
		  thread_create_counter, thread_join_counter);
    }

  return ret;
}
