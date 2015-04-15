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

#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "sys.h"
#include "sys-internal.h"

#ifndef LW6_OPTIMIZE
#define BAZOOKA_ERASER_MALLOC 'M'
#define BAZOOKA_ERASER_REALLOC_1 'E'
#define BAZOOKA_ERASER_REALLOC_2 'R'
#define BAZOOKA_ERASER_FREE 'F'

#define BAZOOKA_MODULO_MUL 5
#define BAZOOKA_MODULO_DIV 7

void
_lw6sys_bazooka_context_init (lw6sys_context_t * sys_context, _lw6sys_bazooka_context_t * bazooka_context)
{
  memset (bazooka_context, 0, sizeof (_lw6sys_bazooka_context_t));
  bazooka_context->eraser = 1;
  bazooka_context->trustable = 1;
}

/*
 * The idea of this function is to provide a default index
 * to store pointers. It avoids always starting loops at index 0
 * and therefore wasting lots of time.
 */
static int
_hash_index (lw6sys_context_t * sys_context, void *p)
{
  int ret = 0;
  u_int32_t checksum = 0;
  u_int32_t modulo = 0;
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  modulo = (bazooka_context->size * BAZOOKA_MODULO_MUL) / BAZOOKA_MODULO_DIV;
  if (modulo > 0)
    {
      checksum = (lw6sys_checksum (sys_context, (unsigned char *) &p, sizeof (void *))) % modulo;
      ret = checksum;
      if (ret < 0)
	{
	  ret = 0;
	}
    }

  return ret;
}

static void
_bazooka_lock (lw6sys_context_t * sys_context)
{
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  if (bazooka_context->spinlock)
    {
      lw6sys_spinlock_lock (sys_context, bazooka_context->spinlock);
    }
}

static void
_bazooka_unlock (lw6sys_context_t * sys_context)
{
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  if (bazooka_context->spinlock)
    {
      lw6sys_spinlock_unlock (sys_context, bazooka_context->spinlock);
    }
}

static void
_bazooka_destroy_spinlock (lw6sys_context_t * sys_context)
{
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);
  lw6sys_spinlock_t *tmp_spinlock = NULL;

  tmp_spinlock = bazooka_context->spinlock;
  bazooka_context->spinlock = NULL;
  if (tmp_spinlock)
    {
      lw6sys_spinlock_destroy (sys_context, tmp_spinlock);
    }
}

static void
_bazooka_free_data (lw6sys_context_t * sys_context)
{
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  bazooka_context->size = 0;
  if (bazooka_context->data)
    {
      free (bazooka_context->data);
    }
  bazooka_context->data = NULL;
}

static void
_big_cleanup (lw6sys_context_t * sys_context)
{
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  if (bazooka_context->size > 0)
    {
      memset (bazooka_context->data, 0, bazooka_context->size * sizeof (_lw6sys_bazooka_line_t));
      bazooka_context->trustable = 0;
    }
}

static int
_bazooka_register_malloc (lw6sys_context_t * sys_context, char *ptr, int size, const char *file, int line, const char *func, int erase)
{
  int ret = 1;
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);
  _lw6sys_bazooka_line_t *bazooka_data = NULL;

  _bazooka_lock (sys_context);
  bazooka_context->malloc_count++;
  bazooka_context->malloc_current_count++;
  bazooka_context->malloc_max_count = lw6sys_imax (bazooka_context->malloc_max_count, bazooka_context->malloc_current_count);
  bazooka_context->malloc_bytes += size;
  bazooka_context->malloc_current_bytes += size;
  bazooka_context->malloc_max_bytes = lw6sys_imax (bazooka_context->malloc_max_bytes, bazooka_context->malloc_current_bytes);
  bazooka_data = bazooka_context->data;
  if (bazooka_data)
    {
      int i;

      if (erase)
	{
	  memset (ptr, BAZOOKA_ERASER_MALLOC, size);
	}

      for (i = _hash_index (sys_context, ptr); i < bazooka_context->size; ++i)
	{
	  if (bazooka_data[i].ptr == NULL)
	    {
	      memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_line_t));
	      bazooka_data[i].ptr = ptr;
	      bazooka_data[i].size = size;

	      _lw6sys_caller_info_set (&(bazooka_data[i].alloc_info), file, line, func);

	      bazooka_data[i].timestamp = lw6sys_get_timestamp (sys_context);

	      if (i >= bazooka_context->size - 1)
		{
		  /*
		   * we've reached the limit, it can't be considered
		   * as trustable any more as it is full...
		   */
		  bazooka_context->trustable = 0;
		}
	      break;		// important to leave loop, else serious perfomance problem
	    }
	}
      if (i == bazooka_context->size)
	{
	  _big_cleanup (sys_context);
	}
    }
  _bazooka_unlock (sys_context);

  return ret;
}

static void
_report_line (lw6sys_context_t * sys_context, _lw6sys_bazooka_line_t * bazooka)
{
  _lw6sys_bazooka_line_t local_bazooka;
  int sample_int = 0;
  char sample_str[_LW6SYS_BAZOOKA_SAMPLE_SIZE];
  void *sample_void = NULL;
  char *pos;
  int64_t i_when;
  time_t t_when;
  struct tm tm_when;
  struct tm *tm_ret = NULL;

  // make a local copy, log functions can modify our data...
  memcpy (&local_bazooka, bazooka, sizeof (_lw6sys_bazooka_line_t));

  memset (sample_str, 0, _LW6SYS_BAZOOKA_SAMPLE_SIZE);
  if (local_bazooka.size > 0)
    {
      memcpy (sample_str, local_bazooka.ptr, lw6sys_imin (local_bazooka.size - 1, _LW6SYS_BAZOOKA_SAMPLE_SIZE - 1));
    }
  sample_void = (void *) sample_str;
  sample_int = *((int *) sample_void);
  for (pos = sample_str; *pos; pos++)
    {
      // get rid of stange system characters
      if ((*pos) < 32)
	{
	  (*pos) = ' ';
	}
    }

  memset (&t_when, 0, sizeof (time_t));
  memset (&tm_when, 0, sizeof (struct tm));
  i_when = local_bazooka.timestamp;
  t_when = i_when / LW6SYS_TICKS_PER_SEC;;
  tm_ret = localtime_r (&t_when, &tm_when);

  if (tm_ret == &tm_when)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		  _x_
		  ("memory bazooka found unfreed data ptr=%p size=%d file:line=\"%s:%d\" func=\"%s()\" time=\"%04d-%02d-%02d %02d:%02d:%02d,%03d\" sample_int=%d sample_str=\"%s\""),
		  local_bazooka.ptr, local_bazooka.size, local_bazooka.alloc_info.file,
		  local_bazooka.alloc_info.line, local_bazooka.alloc_info.func, (int) tm_when.tm_year + 1900,
		  (int) tm_when.tm_mon + 1, (int) tm_when.tm_mday,
		  (int) tm_when.tm_hour, (int) tm_when.tm_min, (int) tm_when.tm_sec, (int) (i_when % LW6SYS_TICKS_PER_SEC), sample_int, sample_str);
    }
}
#endif // LW6_OPTIMIZE

/**
 * lw6sys_default_memory_bazooka
 *
 * @sys_context: global system context
 *
 * Will set up a default memory bazooka, a slow yet convenient
 * tool to track down and hopefully kill memory leaks.
 * Named bazooka after a night wasted to track down an unfoundable
 * leak... BAZOOOOOOKA!!!
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6sys_default_memory_bazooka (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifdef LW6_OPTIMIZE
  ret = 1;
#else // LW6_OPTIMIZE
  ret = lw6sys_set_memory_bazooka_size (sys_context, LW6SYS_BAZOOKA_DEFAULT_SIZE);
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_clear_memory_bazooka
 *
 * @sys_context: global system context
 *
 * Clears the memory bazooka.
 *
 * Return value: none.
 */
void
lw6sys_clear_memory_bazooka (lw6sys_context_t * sys_context)
{
#ifndef LW6_OPTIMIZE
  lw6sys_set_memory_bazooka_size (sys_context, 0);
#endif // LW6_OPTIMIZE
}

/**
 * lw6sys_set_memory_bazooka_size
 *
 * @sys_context: global system context
 * @size: number of items (calls to malloc) to keep
 *
 * Resizes, the memory bazooka. What's this? It's an inelegant yet
 * efficient tool to track down memory leak. Memory bazooka will keep
 * track of every call to malloc, keeping a trace of what has been
 * malloced, where it has been called (from which file, which line), how
 * much memory was allocated, it will even show you what's at the
 * address in a 0-terminated string-friendly fashion. Of course this
 * slows down the program, so in production, you might set this to 0,
 * but for debugging, a million bazooka is worth the megabytes and
 * CPU cycles it wastes.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6sys_set_memory_bazooka_size (lw6sys_context_t * sys_context, int size)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);
  _lw6sys_bazooka_line_t *bazooka_data = NULL;

  if (size == bazooka_context->size)
    {
      // nothing to do
      ret = 1;
    }
  else
    {
      if (size > 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("setting memory bazooka size, old_size=%d new_size=%d"), bazooka_context->size, size);

	  _bazooka_lock (sys_context);

	  bazooka_data = bazooka_context->data;
	  if (bazooka_data)
	    {
	      /*
	       * We can't realloc for the _hash_index() depends on
	       * size and we would be left with lost pointers
	       */
	      bazooka_context->trustable = 0;
	      free (bazooka_data);
	      bazooka_data = NULL;
	      bazooka_context->size = 0;
	    }

	  bazooka_data = (_lw6sys_bazooka_line_t *) calloc (size, sizeof (_lw6sys_bazooka_line_t));
	  if (bazooka_data)
	    {
	      bazooka_context->size = size;
	      ret = 1;
	    }
	  else
	    {
	      bazooka_context->size = 0;
	    }
	  bazooka_context->data = bazooka_data;

	  if (bazooka_context->spinlock)
	    {
	      _bazooka_unlock (sys_context);
	    }
	  else
	    {
	      /*
	       * It's important to create the spinlock now only, for spinlock
	       * creation calls memory allocation routines, and could
	       * therefore wreck tracking.
	       */
	      bazooka_context->spinlock = lw6sys_spinlock_create (sys_context);
	    }
	  if (!bazooka_data)
	    {
	      lw6sys_log_critical (sys_context, _x_ ("can't allocate bazooka memory (%d bytes)"), (int) (size * sizeof (_lw6sys_bazooka_line_t)));
	    }
	}
      else
	{
	  _bazooka_destroy_spinlock (sys_context);
	  _bazooka_free_data (sys_context);

	  ret = 1;
	}
    }
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_size
 *
 * @sys_context: global system context
 *
 * The companion of @lw6sys_set_memory_bazooka_size. This function
 * will return how many calls to malloc can be traced. A return
 * value of 0 indicates that feature is disabled.
 *
 * Return value: size of the bazooka array.
 */
int
lw6sys_get_memory_bazooka_size (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  if (bazooka_context->data)
    {
      ret = bazooka_context->size;
    }
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_set_memory_bazooka_eraser
 *
 * @sys_context: global system context
 * @state: the state of the eraser
 *
 * Sets the memory bazooka eraser state. Note that to really work,
 * it requires the memory bazooka to be "big enough".
 *
 * Return value: 1 if activated, 0 if not. Note that the main reason
 *   for it not to be activated is if the memory bazooka has zero size.
 */
int
lw6sys_set_memory_bazooka_eraser (lw6sys_context_t * sys_context, int state)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("setting memory bazooka eraser, old_state=%d new_state=%d"), bazooka_context->eraser, state);

  bazooka_context->eraser = state ? 1 : 0;
  ret = lw6sys_get_memory_bazooka_eraser (sys_context);
#endif

  return ret;
}

int
lw6sys_get_memory_bazooka_eraser (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = (bazooka_context->eraser && bazooka_context->data && bazooka_context->size > 0);
#endif // LW6_OPTIMIZE

  return ret;
}

int
_lw6sys_bazooka_register_malloc (lw6sys_context_t * sys_context, char *ptr, int size, const char *file, int line, const char *func)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = _bazooka_register_malloc (sys_context, ptr, size, file, line, func, bazooka_context->eraser);
#endif // LW6_OPTIMIZE

  return ret;
}

int
_lw6sys_bazooka_register_calloc (lw6sys_context_t * sys_context, char *ptr, int size, const char *file, int line, const char *func)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  ret = _bazooka_register_malloc (sys_context, ptr, size, file, line, func, 0);
#endif // LW6_OPTIMIZE

  return ret;
}

int
_lw6sys_bazooka_register_realloc_1 (lw6sys_context_t * sys_context, char *ptr, int size, const char *file, int line, const char *func)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);
  _lw6sys_bazooka_line_t *bazooka_data = NULL;

  _bazooka_lock (sys_context);
  bazooka_data = bazooka_context->data;
  if (bazooka_data)
    {
      int i0 = 0;
      int i = 0;

      i0 = _hash_index (sys_context, ptr);
      for (i = i0; i < bazooka_context->size; ++i)
	{
	  if (bazooka_data[i].ptr == ptr)
	    {
	      bazooka_context->malloc_bytes += size - bazooka_data[i].size;
	      bazooka_context->malloc_current_bytes += size - bazooka_data[i].size;
	      bazooka_context->malloc_max_bytes = lw6sys_imax (bazooka_context->malloc_max_bytes, bazooka_context->malloc_current_bytes);
	      if (bazooka_context->eraser && bazooka_data[i].size > size)
		{
		  memset (bazooka_data[i].ptr + size, BAZOOKA_ERASER_REALLOC_1, bazooka_data[i].size - size);
		}
	      break;		// important to leave loop, else serious perfomance problem
	    }
	}
      if (bazooka_context->trustable && i == bazooka_context->size)
	{
	  ret = 0;
	}
    }
  _bazooka_unlock (sys_context);
#endif // LW6_OPTIMIZE

  return ret;
}

int
_lw6sys_bazooka_register_realloc_2 (lw6sys_context_t * sys_context, char *ptr, char *ptr2, int size, const char *file, int line, const char *func)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);
  _lw6sys_bazooka_line_t *bazooka_data = NULL;

  _bazooka_lock (sys_context);
  bazooka_data = bazooka_context->data;
  if (bazooka_data)
    {
      int i;

      for (i = _hash_index (sys_context, ptr); i < bazooka_context->size; ++i)
	{
	  if (bazooka_data[i].ptr == ptr)
	    {
	      if (bazooka_context->eraser && bazooka_data[i].size < size)
		{
		  memset (ptr2 + bazooka_data[i].size, BAZOOKA_ERASER_REALLOC_2, size - bazooka_data[i].size);
		}
	      memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_line_t));
	      if (ptr == ptr2)
		{
		  bazooka_data[i].ptr = ptr2;	// was NULL because of memset
		  bazooka_data[i].size = size;

		  _lw6sys_caller_info_set (&(bazooka_data[i].alloc_info), file, line, func);

		  bazooka_data[i].timestamp = lw6sys_get_timestamp (sys_context);
		}
	      break;		// important to leave loop, else serious perfomance problem
	    }
	}
      if (bazooka_context->trustable && i == bazooka_context->size)
	{
	  ret = 0;
	}

      if (ptr != ptr2)
	{
	  /*
	   * It's really important to relocate
	   * entry if ptr!= ptr2 else _hash_index()
	   * will be useless
	   */
	  for (i = _hash_index (sys_context, ptr2); i < bazooka_context->size; ++i)
	    {
	      if (bazooka_data[i].ptr == NULL)
		{
		  memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_line_t));
		  bazooka_data[i].ptr = ptr2;
		  bazooka_data[i].size = size;

		  _lw6sys_caller_info_set (&(bazooka_data[i].alloc_info), file, line, func);

		  bazooka_data[i].timestamp = lw6sys_get_timestamp (sys_context);
		  if (i >= bazooka_context->size - 1)
		    {
		      /*
		       * we've reached the limit, it can't be considered
		       * as trustable any more as it is full...
		       */
		      bazooka_context->trustable = 0;
		    }
		  break;	// important to leave loop, else serious perfomance problem
		}
	    }
	  if (i == bazooka_context->size)
	    {
	      _big_cleanup (sys_context);
	    }
	}
    }
  _bazooka_unlock (sys_context);
#endif // LW6_OPTIMIZE

  return ret;
}

int
_lw6sys_bazooka_register_free (lw6sys_context_t * sys_context, char *ptr)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);
  _lw6sys_bazooka_line_t *bazooka_data = NULL;

  int i;

  _bazooka_lock (sys_context);
  bazooka_data = bazooka_context->data;
  bazooka_context->free_count++;
  bazooka_context->malloc_current_count--;
  if (bazooka_context->free_count > bazooka_context->malloc_count)
    {
      lw6sys_log_critical (sys_context, _x_ ("problem, more free (%d) than malloc (%d)"), bazooka_context->free_count, bazooka_context->malloc_count);
    }

#ifdef LW6_PARANOID
  {
    int same = 0;

    for (i = _hash_index (sys_context, ptr), same = 1; i < _LW6SYS_BAZOOKA_ALLOC_MIN; ++i)
      {
	if (ptr[i] != BAZOOKA_ERASER_FREE)
	  {
	    same = 0;
	    break;
	  }
      }

    if (same)
      {
	ret = 0;
      }
  }
#endif // LW6_PARANOID

  if (bazooka_data)
    {
      for (i = _hash_index (sys_context, ptr); i < bazooka_context->size; ++i)
	{
	  if (bazooka_data[i].ptr == ptr)
	    {
	      bazooka_context->free_bytes += bazooka_data[i].size;
	      bazooka_context->malloc_current_bytes -= bazooka_data[i].size;
	      if (bazooka_context->free_bytes > bazooka_context->malloc_bytes)
		{
		  lw6sys_log_critical (sys_context, _x_
				       ("problem, more bytes freed (%"
					LW6SYS_PRINTF_LL "d) than malloced (%"
					LW6SYS_PRINTF_LL "d)"), (long long) bazooka_context->free_bytes, (long long) bazooka_context->malloc_bytes);
		}
	      if (bazooka_context->eraser)
		{
		  memset (bazooka_data[i].ptr, BAZOOKA_ERASER_FREE, bazooka_data[i].size);
		}
	      memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_line_t));
	      break;		// important to leave loop, else serious perfomance problem
	    }
	}
      if (bazooka_context->trustable && i == bazooka_context->size)
	{
	  ret = 0;
	}
    }

  _bazooka_unlock (sys_context);
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_count
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to
 * allocate memory, this function will tell you how many times @malloc
 * has been called.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_count (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = bazooka_context->malloc_count;
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_free_count
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_FREE macro to free
 * memory, this function will tell you how many times @free has been called.
 *
 * Return value: the number of calls to @lw6sys_free since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_free_count (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = bazooka_context->free_count;
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_current_count
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to
 * allocate memory, this function will tell you the current number of
 * pointer returned by @LW6SYS_MALLOC an @LW6SYS_CALLOC, currently
 * alive on the heap.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_current_count (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = bazooka_context->malloc_current_count;
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_max_count
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to
 * allocate memory, this function will tell you the maximum of pointers
 * returned by @malloc that were present at the same time on the heap.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_max_count (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = bazooka_context->malloc_max_count;
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_megabytes
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to
 * allocate memory, this function will tell you how many bytes @malloc
 * has reserved.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_megabytes (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = (int64_t) (bazooka_context->malloc_bytes / 0x100000L);
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_free_megabytes
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_FREE macro to free
 * memory, this function will tell you how many bytes @free has freed.
 *
 * Return value: the number of calls to @lw6sys_free since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_free_megabytes (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = (int64_t) (bazooka_context->free_bytes / 0x100000L);
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_current_bytes
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to
 * allocate memory, this function will tell you the current number of
 * bytes returned by @LW6SYS_MALLOC an @LW6SYS_CALLOC, currently
 * alive on the heap.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_current_bytes (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = bazooka_context->malloc_current_bytes;
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_max_bytes
 *
 * @sys_context: global system context
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to
 * allocate memory, this function will tell you the maximum bytes
 * returned by @malloc that were present at the same time on the heap.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_max_bytes (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = bazooka_context->malloc_max_bytes;
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_is_memory_bazooka_trustable
 *
 * @sys_context: global system context
 *
 * Returns true if memory bazooka data are perfectly trustable, that is,
 * it has never been resetted or resized.
 *
 * Return value: 1 if trustable, 0 if not.
 */
int
lw6sys_is_memory_bazooka_trustable (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);

  ret = bazooka_context->trustable;
#endif // LW6_OPTIMIZE

  return ret;
}

/**
 * lw6sys_memory_bazooka_report
 *
 * @sys_context: global system context
 *
 * Reports memory bazooka diagnostics on the console. Carefull, this
 * one is not reentrant, call at the end of your program when all threads
 * are joined.
 *
 * Return value: 1 if no allocated stuff left, 0 if there are still malloc'ed stuff
 */
int
lw6sys_memory_bazooka_report (lw6sys_context_t * sys_context)
{
  int ret = 1;

#ifdef LW6_OPTIMIZE
  /*
   * Do not display this systematically (NOTICE) for it
   * could interfere with standard output when processing
   * commands like --version or such things.
   */
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("no memory check"));
#else // LW6_OPTIMIZE
  _lw6sys_bazooka_context_t *bazooka_context = &(((_lw6sys_context_t *) sys_context)->bazooka_context);
  _lw6sys_bazooka_line_t *bazooka_data = NULL;
  int recreate_spinlock = 0;
  int i;
  int malloc_count;
  int free_count;
  int malloc_current_count;
  int malloc_max_count;
  int malloc_megabytes;
  int free_megabytes;
  int malloc_current_bytes;
  int malloc_max_bytes;
  int malloc_max_megabytes;
  int reported_lines = 0;

  if (bazooka_context->spinlock)
    {
      _bazooka_destroy_spinlock (sys_context);
      recreate_spinlock = 1;
    }

  bazooka_data = bazooka_context->data;
  malloc_count = lw6sys_get_memory_bazooka_malloc_count (sys_context);
  free_count = lw6sys_get_memory_bazooka_free_count (sys_context);
  malloc_current_count = lw6sys_get_memory_bazooka_malloc_current_count (sys_context);
  malloc_max_count = lw6sys_get_memory_bazooka_malloc_max_count (sys_context);
  malloc_megabytes = lw6sys_get_memory_bazooka_malloc_megabytes (sys_context);
  free_megabytes = lw6sys_get_memory_bazooka_free_megabytes (sys_context);
  malloc_current_bytes = lw6sys_get_memory_bazooka_malloc_current_bytes (sys_context);
  malloc_max_bytes = lw6sys_get_memory_bazooka_malloc_max_bytes (sys_context);
  malloc_max_megabytes = (malloc_max_bytes >> 20) + 1;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("malloc_count=%d"), malloc_count);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("free_count=%d"), free_count);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("malloc_current_count=%d"), malloc_current_count);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("malloc_max_count=%d"), malloc_max_count);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("malloc_megabytes=%d"), malloc_megabytes);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("free_megabytest=%d"), free_megabytes);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("malloc_current_bytes=%d"), malloc_current_bytes);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("malloc_max_bytes=%d"), malloc_max_bytes);

  if (malloc_count == free_count)
    {
      if (bazooka_data)
	{
	  if (bazooka_context->trustable)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d malloc calls, %d megabytes system heap"), malloc_count, malloc_max_megabytes);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("%d malloc calls, %d megabytes system heap (untrusted)"), malloc_count, malloc_max_megabytes);
	    }
	}
    }
  else
    {
      /*
       * Display the error message only if bazooka is active
       * or if there have been no threads arround, the idea
       * is that if we're threaded and there's no bazooka, we
       * could report false positives.
       */
      if (bazooka_data || lw6sys_get_thread_create_count (sys_context) == 0)
	{
	  ret = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("possible memory leak, %d calls to malloc and %d calls to free, note that if the program exited abnormally because of an unexpected error, this difference might be \"normal\""),
		      malloc_count, free_count);
	}
    }

  if (bazooka_data)
    {
      for (i = 0, reported_lines = 0; i < bazooka_context->size; ++i)
	{
	  if (bazooka_data[i].ptr)
	    {
	      ret = 0;
	      _report_line (sys_context, &(bazooka_data[i]));
	      reported_lines++;
	    }
	}
      if (!ret)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bazooka size is %d, %d lines reported"), bazooka_context->size, reported_lines);
	}
    }

  if (recreate_spinlock)
    {
      bazooka_context->spinlock = lw6sys_spinlock_create (sys_context);
    }
#endif // LW6_OPTIMIZE

  return ret;
}
