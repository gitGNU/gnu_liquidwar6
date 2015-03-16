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

static void
_vthread_callback (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  _lw6sys_thread_handler_t *tmp_handler = NULL;
  struct timespec ts;
  int yield_timeslice = 0;

  ts.tv_sec = _LW6SYS_VTHREAD_COND_TIMEDWAIT_SEC;
  ts.tv_nsec = _LW6SYS_VTHREAD_COND_TIMEDWAIT_NSEC;

  if (global->vthread_main_handler->callback_join)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("begin vthread"));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("begin vthread (fast mode, no join)"));
    }
  if (global->vthread_main_handler->callback_func)
    {
      global->vthread_main_handler->callback_func (sys_context, global->vthread_main_handler->callback_data);
    }
  /*
   * callback is over, we signal it to the caller, if needed
   */
  if (!pthread_mutex_lock (&(global->vthread_main_handler->mutex)))
    {
      global->vthread_main_handler->flag_callback_done = 1;
      pthread_cond_signal (&(global->vthread_main_handler->cond_callback_done));
      pthread_mutex_unlock (&(global->vthread_main_handler->mutex));
    }
  else
    {
      global->vthread_main_handler->flag_callback_done = 1;
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread (vthread)"));
    }
  /*
   * If callback_join is defined, we wait until the caller
   * has called "join" before freeing the ressources. If it's
   * not defined, we just end now. The idea is that callback_join
   * would free ressources that might be usefull in the thread,
   * and queried by the caller. If we're running a stateless
   * thread we just end quickly.
   */
  if (global->vthread_main_handler->callback_join)
    {
      while (!global->vthread_main_handler->flag_can_join)
	{
	  /*
	   * Now the caller is supposed to set "can_join"
	   * to allow this thread to actually finish.
	   */
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("waiting for can_join to be 1 (vhtread)"));
	  if (!pthread_mutex_lock (&(global->vthread_main_handler->mutex)))
	    {
	      yield_timeslice =
		(pthread_cond_timedwait (&(global->vthread_main_handler->cond_can_join), &(global->vthread_main_handler->mutex), &ts) == ETIMEDOUT);
	      pthread_mutex_unlock (&(global->vthread_main_handler->mutex));
	      if (yield_timeslice)
		{
		  lw6sys_snooze (sys_context);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread (vthread)"));
	    }
	}
      /*
       * In the peculiar case of vthread, join must imperatively
       * be called in this thread, not in the other one...
       */
      global->vthread_main_handler->callback_join (sys_context, global->vthread_main_handler->callback_data);
    }
  if (global->vthread_main_handler->callback_join)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("end vthread"));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("end vthread (fast mode, no join)"));
    }
  /*
   * callback is over, we signal it to the caller, if needed
   */
  tmp_handler = global->vthread_main_handler;
  global->vthread_main_handler = NULL;

  if (!pthread_mutex_lock (&(tmp_handler->mutex)))
    {
      pthread_cond_destroy (&(tmp_handler->cond_callback_done));
      pthread_cond_destroy (&(tmp_handler->cond_can_join));
      pthread_mutex_unlock (&(tmp_handler->mutex));
      pthread_mutex_destroy (&(tmp_handler->mutex));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread (vthread)"));
    }

  LW6SYS_FREE (sys_context, tmp_handler);
}

/**
 * lw6sys_vthread_run
 *
 * @sys_context: global system context
 * @callback_func: the main callback, the function that will run the thread
 * @callback_join: function which will be called when joining, at the end
 * @callback_data: data which will be passed to the callback
 *
 * This function is similar to @lw6sys_thread_create, but it's dedicated to
 * creating a unique (one per process only) thread, which, in turn, will
 * be able to run commands in the main thread itself. This is a hack to
 * allow apparently spawned child threads to be actually handled by main.
 * This is because some libraries, which LW6 uses in threads, need to be
 * actually called in the main thread. SDL, for instance. Note that after
 * running this you loose control on the main thread, this one will only
 * wait for possible commands from the spawned thread, typically sent
 * with the @lw6sys_vthread_create function.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sys_vthread_run (lw6sys_context_t * sys_context,
		    lw6sys_thread_callback_func_t callback_func, lw6sys_thread_callback_func_t callback_join, void *callback_data)
{
  int ret = 0;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (!global->vthread_run_handler && !global->vthread_main_handler)
    {
      global->vthread_run_handler = lw6sys_thread_create (sys_context, callback_func, callback_join,
							  callback_data);
      if (global->vthread_run_handler)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("run vthread begin"));

	  /*
	   * More than one vthread could be started (not at once!)
	   * successively so we loop until spawned is done.
	   */
	  while (!lw6sys_thread_is_callback_done (sys_context, global->vthread_run_handler))
	    {
	      while (global->vthread_main_handler == NULL && !lw6sys_thread_is_callback_done (sys_context, global->vthread_run_handler))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("waiting for main handler to be defined by spawned thread"));
		  lw6sys_snooze (sys_context);
		}
	      /*
	       * global->vthread_main_handler could be NULL if we're here because
	       * the callback is done
	       */
	      if (global->vthread_main_handler)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("vthread main handler"));
		  _vthread_callback (sys_context);
		}
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("waiting for callback to be done (vhtread)"));
	      lw6sys_snooze (sys_context);
	    }

	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("run vthread end"));

	  lw6sys_thread_join (sys_context, global->vthread_run_handler);
	  global->vthread_run_handler = NULL;
#if 0
	  /*
	   * Only free global->vthread_main_handler now.
	   * See https://savannah.gnu.org/bugs/?27819 for details.
	   * The idea is that we need join_done to join properly.
	   */
	  if (global->vthread_main_handler)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing _main_vhandler"));
	      LW6SYS_FREE (sys_context, _main_vhandler);
	      _main_vhandler = NULL;
	    }
#endif
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create run vthread"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't run vthread, already initialized"));
    }

  return ret;
}

/**
 * lw6sys_vthread_is_running
 *
 * @sys_context: global system context
 *
 * Returns true if @lw6sys_vthread_run has been called. Note that this
 * is not bullet proof, it will return true in a correct manner only
 * if you call it from the vthread itself. In practise this shouldn't
 * be a problem, the idea is just to write portable code for the main
 * control thread and be able to decide on the fly wether to create
 * a thread we should prefer the @lw6sys_thread_create or its
 * equivalent the @lw6sys_vthread_create function.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sys_vthread_is_running (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  int ret = 0;

  ret = (global->vthread_run_handler != NULL);

  return ret;
}

/**
 * lw6sys_vthread_create
 *
 * @sys_context: global system context
 * @callback_func: the main callback, the function that will run the thread
 * @callback_join: function which will be called when joining, at the end
 * @callback_data: data which will be passed to the callback
 *
 * The equivalent of @lw6sys_thread_create but for the vthread infrastructure.
 * The idea is to pretend firing a spawned thread, but in fact it's the main
 * thread that runs the code. This function must imperatively be called
 * within the @lw6sys_vthread_run function, else it will fail or be buggy.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sys_vthread_create (lw6sys_context_t * sys_context,
		       lw6sys_thread_callback_func_t callback_func, lw6sys_thread_callback_func_t callback_join, void *callback_data)
{
  int ret = 0;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  _lw6sys_thread_handler_t *tmp_handler = NULL;
  int thread_ok = 0;
  int mutex_ok = 0;
  int cond_callback_done_ok = 0;
  int cond_can_join_ok = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("vthread_create"));
  if (!global->vthread_main_handler)
    {
      tmp_handler = (_lw6sys_thread_handler_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6sys_thread_handler_t));
      if (tmp_handler)
	{
	  if (!pthread_mutex_init (&(tmp_handler->mutex), NULL))
	    {
	      mutex_ok = 1;
	      if (!pthread_cond_init (&(tmp_handler->cond_callback_done), NULL))
		{
		  cond_callback_done_ok = 1;
		  if (!pthread_cond_init (&(tmp_handler->cond_can_join), NULL))
		    {
		      cond_can_join_ok = 1;
		      thread_ok = 1;
		    }
		}
	    }
	}
      if (thread_ok)
	{
	  // handler->thread member should stay O/NULL
	  tmp_handler->id = _LW6SYS_VTHREAD_ID;
	  tmp_handler->callback_func = callback_func;
	  tmp_handler->callback_join = callback_join;
	  tmp_handler->callback_data = callback_data;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("vhtread create successfull"));
	  ret = 1;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("defining global->vthread_main_handler"));


	  /*
	   * It's important to only affect in now that
	   * all fields are correctly filled.
	   */
	  global->vthread_main_handler = tmp_handler;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create vthread"));
	  if (tmp_handler)
	    {
	      if (mutex_ok)
		{
		  pthread_mutex_destroy (&(tmp_handler->mutex));
		}
	      if (cond_callback_done_ok)
		{
		  pthread_cond_destroy (&(tmp_handler->cond_callback_done));
		}
	      if (cond_can_join_ok)
		{
		  pthread_cond_destroy (&(tmp_handler->cond_can_join));
		}
	      LW6SYS_FREE (sys_context, tmp_handler);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't start vthread, there's already a running vthread"));
    }

  return ret;
}

/**
 * lw6sys_vthread_join
 *
 * @sys_context: global system context
 *
 * The equivalent of @lw6sys_thread_join but for the vthread infrastructure.
 * The idea is to pretend firing a spawned thread, but in fact it's the main
 * thread that runs the code. This function must imperatively be called
 * within the @lw6sys_vthread_run function, else it will fail or be buggy.
 *
 * Return value: none.
 */
void
lw6sys_vthread_join (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (global->vthread_main_handler)
    {
      if (global->vthread_main_handler->callback_join)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joining vthread"));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("joining vthread (fast mode, no join)"));
	}

      if (!pthread_mutex_lock (&(global->vthread_main_handler->mutex)))
	{
	  global->vthread_main_handler->flag_can_join = 1;
	  pthread_cond_signal (&(global->vthread_main_handler->cond_can_join));
	  pthread_mutex_unlock (&(global->vthread_main_handler->mutex));
	}
      else
	{
	  global->vthread_main_handler->flag_can_join = 1;
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to lock internal thread mutex thread (vthread)"));
	}

      while (global->vthread_main_handler)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("waiting for main handler to be NULL"));
	  lw6sys_snooze (sys_context);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't call join vthread, vhandler is NULL"));
    }
}
