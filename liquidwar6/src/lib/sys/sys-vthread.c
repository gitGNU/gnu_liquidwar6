/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "sys.h"
#include "sys-internal.h"

static void *_run_handler = NULL;
static _lw6sys_vthread_handler_t *_main_vhandler = NULL;

static void
vthread_callback (_lw6sys_vthread_handler_t * vthread_handler)
{
  _lw6sys_thread_handler_t *thread_handler = &(vthread_handler->thread);

  if (thread_handler->callback_join)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("begin vthread"));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("begin vthread (fast mode, no join)"));
    }
  if (thread_handler->callback_func)
    {
      thread_handler->callback_func (thread_handler->callback_data);
    }
  /* 
   * callback is over, we signal it to the caller, if needed
   */
  thread_handler->callback_done = 1;
  /*
   * If callback_join is defined, we wait until the caller
   * has called "join" before freeing the ressources. If it's
   * not defined, we just end now. The idea is that callback_join
   * would free ressources that might be usefull in the thread,
   * and queried by the caller. If we're running a stateless
   * thread we just end quickly.
   */
  if (thread_handler->callback_join)
    {
      while (!thread_handler->can_join)
	{
	  /*
	   * Now the caller is supposed to set "can_join"
	   * to allow this thread to actually finish.
	   */
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("waiting for can_join to be 1 (vhtread)"));
	  lw6sys_idle ();
	}
      thread_handler->callback_join (thread_handler->callback_data);
    }
  if (thread_handler->callback_join)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("end vthread"));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("end vthread (fast mode, no join)"));
    }
  /* 
   * callback is over, we signal it to the caller, if needed
   */
  vthread_handler->join_done = 1;
}

/**
 * lw6sys_vthread_run 
 *
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
lw6sys_vthread_run (lw6sys_thread_callback_func_t callback_func,
		    lw6sys_thread_callback_func_t callback_join,
		    void *callback_data)
{
  int ret = 0;

  if (!_main_vhandler)
    {
      _run_handler =
	lw6sys_thread_create (callback_func, callback_join, callback_data);
      if (_run_handler)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("run vthread begin"));

	  while (!_main_vhandler)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _
			  ("waiting for main handler to be defined by spawned thread"));
	      lw6sys_idle ();
	    }

	  lw6sys_log (LW6SYS_LOG_DEBUG, _("vthread main handler"));
	  vthread_callback (_main_vhandler);

	  while (!lw6sys_thread_is_callback_done (_run_handler))
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("waiting for callback to be done (vhtread)"));
	      lw6sys_idle ();
	    }

	  lw6sys_log (LW6SYS_LOG_INFO, _("run vthread end"));

	  lw6sys_thread_join (_run_handler);
	  _run_handler = NULL;

	  /*
	   * Only free _main_handler now.
	   * See https://savannah.gnu.org/bugs/?27819 for details.
	   * The idea is that we need join_done to join properly.
	   */
	  if (_main_vhandler)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("freeing _main_vhandler"));
	      LW6SYS_FREE (_main_vhandler);
	      _main_vhandler = NULL;
	    }

	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("unable to create run vthread"));
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("can't run vthread, already initialized"));
    }

  return ret;
}

/**
 * lw6sys_vthread_is_running
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
lw6sys_vthread_is_running ()
{
  int ret = 0;

  ret = (_run_handler != NULL);

  return ret;
}

/**
 * lw6sys_vthread_create
 *
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
lw6sys_vthread_create (lw6sys_thread_callback_func_t callback_func,
		       lw6sys_thread_callback_func_t callback_join,
		       void *callback_data)
{
  int ret = 0;
  _lw6sys_vthread_handler_t *vhandler = NULL;
  _lw6sys_thread_handler_t *handler = NULL;

  if (!_main_vhandler)
    {
      vhandler = (_lw6sys_vthread_handler_t *)
	LW6SYS_CALLOC (sizeof (_lw6sys_vthread_handler_t));
      if (vhandler)
	{
	  handler = &(vhandler->thread);
	  // handler->thread member should stay O/NULL
	  handler->id = _LW6SYS_VTHREAD_ID;
	  handler->callback_func = callback_func;
	  handler->callback_join = callback_join;
	  handler->callback_data = callback_data;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("vhtread create successfull"));
	  ret = 1;
	  _main_vhandler = vhandler;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("can't create vthread"));
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("can't start vthread, there's already a running vthread"));
    }

  return ret;
}

/**
 * lw6sys_vthread_join
 *
 * The equivalent of @lw6sys_thread_join but for the vthread infrastructure.
 * The idea is to pretend firing a spawned thread, but in fact it's the main
 * thread that runs the code. This function must imperatively be called
 * within the @lw6sys_vthread_run function, else it will fail or be buggy.
 *
 * Return value: none.
 */
void
lw6sys_vthread_join ()
{
  _lw6sys_vthread_handler_t *vhandler = NULL;
  _lw6sys_thread_handler_t *handler = NULL;

  vhandler = _main_vhandler;
  if (vhandler)
    {
      handler = &(vhandler->thread);
      if (handler->callback_join)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("joining vthread"));
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("joining vthread (fast mode, no join)"));
	}

      handler->can_join = 1;

      while (!vhandler->join_done)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("waiting for join to be done (vhtread)"));
	  lw6sys_idle ();
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("can't call join vthread, vhandler is NULL"));
    }
}
