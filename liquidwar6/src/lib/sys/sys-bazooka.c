/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#define BAZOOKA_MODULO_MUL 9
#define BAZOOKA_MODULO_DIV 10

static int bazooka_size = 0;
static _lw6sys_bazooka_t *bazooka_data = NULL;
static void *bazooka_spinlock = NULL;
static int bazooka_malloc_count = 0;
static int bazooka_free_count = 0;
static int bazooka_malloc_current_count = 0;
static int bazooka_malloc_max_count = 0;
static int64_t bazooka_malloc_bytes = 0;
static int64_t bazooka_free_bytes = 0;
static int bazooka_malloc_current_bytes = 0;
static int bazooka_malloc_max_bytes = 0;
static int bazooka_eraser = 1;
static int bazooka_trustable = 1;

/*
 * The idea of this function is to provide a default index
 * to store pointers. It avoids always starting loops at index 0
 * and therefore wasting lots of time.
 */
static int
_hash_index (void *p)
{
  int ret = 0;
  u_int32_t checksum = 0;
  u_int32_t modulo = 0;

  modulo = (bazooka_size * BAZOOKA_MODULO_MUL) / BAZOOKA_MODULO_DIV;
  if (modulo > 0)
    {
      checksum =
	(lw6sys_checksum ((unsigned char *) &p, sizeof (void *))) % modulo;
      ret = checksum;
      if (ret < 0)
	{
	  ret = 0;
	}
    }

  return ret;
}

static void
bazooka_lock ()
{
  if (bazooka_spinlock)
    {
      lw6sys_spinlock_lock (bazooka_spinlock);
    }
}

static void
bazooka_unlock ()
{
  if (bazooka_spinlock)
    {
      lw6sys_spinlock_unlock (bazooka_spinlock);
    }
}

static void
bazooka_destroy_spinlock ()
{
  void *tmp_spinlock;

  tmp_spinlock = bazooka_spinlock;
  bazooka_spinlock = NULL;
  if (tmp_spinlock)
    {
      lw6sys_spinlock_destroy (tmp_spinlock);
    }
}

static void
bazooka_free_data ()
{
  bazooka_size = 0;
  if (bazooka_data)
    {
      free (bazooka_data);
    }
  bazooka_data = NULL;
}

static void
_big_cleanup ()
{
  if (bazooka_size > 0)
    {
      memset (bazooka_data, 0, bazooka_size * sizeof (_lw6sys_bazooka_t));
      bazooka_trustable = 0;
    }
}

static int
bazooka_register_malloc (char *ptr, int size, char *file, int line, int erase)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  bazooka_lock ();
  bazooka_malloc_count++;
  bazooka_malloc_current_count++;
  bazooka_malloc_max_count =
    lw6sys_max (bazooka_malloc_max_count, bazooka_malloc_current_count);
  bazooka_malloc_bytes += size;
  bazooka_malloc_current_bytes += size;
  bazooka_malloc_max_bytes =
    lw6sys_max (bazooka_malloc_max_bytes, bazooka_malloc_current_bytes);
  if (bazooka_data)
    {
      int i;
      char *file_only;

      if (erase)
	{
	  memset (ptr, BAZOOKA_ERASER_MALLOC, size);
	}

      for (i = _hash_index (ptr); i < bazooka_size; ++i)
	{
	  if (bazooka_data[i].ptr == NULL)
	    {
	      memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_t));
	      bazooka_data[i].ptr = ptr;
	      bazooka_data[i].size = size;
	      file_only = strrchr (file, '/');
	      if (file_only && *file_only)
		{
		  file_only++;
		}
	      else
		{
		  file_only = file;
		}
	      strncpy (bazooka_data[i].file, file_only,
		       _LW6SYS_BAZOOKA_FILE_SIZE - 1);
	      bazooka_data[i].line = line;
	      bazooka_data[i].timestamp = lw6sys_get_timestamp ();
	      break;		// important to leave loop, else serious perfomance problem
	    }
	}
      if (i == bazooka_size)
	{
	  _big_cleanup ();
	}
    }
  bazooka_unlock ();
#endif

  return ret;
}

static void
report_line (_lw6sys_bazooka_t * bazooka)
{
  _lw6sys_bazooka_t local_bazooka;
  int sample_int = 0;
  char sample_str[_LW6SYS_BAZOOKA_SAMPLE_SIZE];
  void *sample_void = NULL;
  char *pos;
  int64_t i_when;
  time_t t_when;
  struct tm tm_when;
  struct tm *tm_ret = NULL;

  // make a local copy, log functions can modify our data...
  memcpy (&local_bazooka, bazooka, sizeof (_lw6sys_bazooka_t));

  memset (sample_str, 0, _LW6SYS_BAZOOKA_SAMPLE_SIZE);
  if (local_bazooka.size > 0)
    {
      memcpy (sample_str, local_bazooka.ptr,
	      lw6sys_min (local_bazooka.size - 1,
			  _LW6SYS_BAZOOKA_SAMPLE_SIZE - 1));
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
      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_
		  ("memory bazooka found unfreed data ptr=%p size=%d file:line=\"%s:%d\" time=\"%04d-%02d-%02d %02d:%02d:%02d,%03d\" sample_int=%d sample_str=\"%s\""),
		  local_bazooka.ptr, local_bazooka.size, local_bazooka.file,
		  local_bazooka.line, (int) tm_when.tm_year + 1900,
		  (int) tm_when.tm_mon + 1, (int) tm_when.tm_mday,
		  (int) tm_when.tm_hour, (int) tm_when.tm_min,
		  (int) tm_when.tm_sec, (int) (i_when % LW6SYS_TICKS_PER_SEC),
		  sample_int, sample_str);
    }
}
#endif

/**
 * lw6sys_default_memory_bazooka
 *
 * Will set up a default memory bazooka, a slow yet convenient
 * tool to track down and hopefully kill memory leaks.
 * Named bazooka after a night wasted to track down an unfoundable
 * leak... BAZOOOOOOKA!!!
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6sys_default_memory_bazooka ()
{
  int ret = 0;

#ifdef LW6_OPTIMIZE
  ret = 1;
#else
  ret = lw6sys_set_memory_bazooka_size (LW6SYS_BAZOOKA_DEFAULT_SIZE);
#endif

  return ret;
}

/**
 * lw6sys_clear_memory_bazooka
 *
 * Clears the memory bazooka.
 *
 * Return value: none.
 */
void
lw6sys_clear_memory_bazooka ()
{
#ifndef LW6_OPTIMIZE
  lw6sys_set_memory_bazooka_size (0);
#endif
}

/**
 * lw6sys_set_memory_bazooka_size
 *
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
lw6sys_set_memory_bazooka_size (int size)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  if (size == bazooka_size)
    {
      // nothing to do
      ret = 1;
    }
  else
    {
      if (size > 0)
	{
	  bazooka_lock ();

	  if (bazooka_data)
	    {
	      /*
	       * We can't realloc for the _hash_index() depends on
	       * size and we would be left with lost pointers
	       */
	      bazooka_trustable = 0;
	      free (bazooka_data);
	      bazooka_data = NULL;
	      bazooka_size = 0;
	    }

	  bazooka_data =
	    (_lw6sys_bazooka_t *) calloc (size, sizeof (_lw6sys_bazooka_t));
	  if (bazooka_data)
	    {
	      bazooka_size = size;
	      ret = 1;
	    }
	  else
	    {
	      bazooka_size = 0;
	    }

	  if (bazooka_spinlock)
	    {
	      bazooka_unlock ();
	    }
	  else
	    {
	      /*
	       * It's important to create the spinlock now only, for spinlock
	       * creation calls memory allocation routines, and could
	       * therefore wreck tracking.
	       */
	      bazooka_spinlock = lw6sys_spinlock_create ();
	    }
	  if (!bazooka_data)
	    {
	      lw6sys_log_critical (_x_
				   ("can't allocate bazooka memory (%d bytes)"),
				   size * sizeof (_lw6sys_bazooka_t));
	    }
	}
      else
	{
	  bazooka_destroy_spinlock ();
	  bazooka_free_data ();

	  ret = 1;
	}
    }
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_size
 *
 * The companion of @lw6sys_set_memory_bazooka_size. This function
 * will return how many calls to malloc can be traced. A return
 * value of 0 indicates that feature is disabled.
 *
 * Return value: size of the bazooka array.
 */
int
lw6sys_get_memory_bazooka_size ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  if (bazooka_data)
    {
      ret = bazooka_size;
    }
#endif

  return ret;
}

/**
 * lw6sys_set_memory_bazooka_eraser
 *
 * @state: the state of the eraser
 *
 * Sets the memory bazooka eraser state. Note that to really work,
 * it requires the memory bazooka to be "big enough".
 *
 * Return value: 1 if activated, 0 if not. Note that the main reason
 *   for it not to be activated is if the memory bazooka has zero size.
 */
int
lw6sys_set_memory_bazooka_eraser (int state)
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  bazooka_eraser = state ? 1 : 0;
  ret = lw6sys_get_memory_bazooka_eraser ();
#endif

  return ret;
}

int
lw6sys_get_memory_bazooka_eraser ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = (bazooka_eraser && bazooka_data && bazooka_size > 0);
#endif

  return ret;
}

int
_lw6sys_bazooka_register_malloc (char *ptr, int size, char *file, int line)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  ret = bazooka_register_malloc (ptr, size, file, line, bazooka_eraser);
#endif

  return ret;
}

int
_lw6sys_bazooka_register_calloc (char *ptr, int size, char *file, int line)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  ret = bazooka_register_malloc (ptr, size, file, line, 0);
#endif

  return ret;
}

int
_lw6sys_bazooka_register_realloc_1 (char *ptr, int size, char *file, int line)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  bazooka_lock ();
  if (bazooka_data)
    {
      int i;

      for (i = _hash_index (ptr); i < bazooka_size; ++i)
	{
	  if (bazooka_data[i].ptr == ptr)
	    {
	      bazooka_malloc_bytes += size - bazooka_data[i].size;
	      bazooka_malloc_current_bytes += size - bazooka_data[i].size;
	      bazooka_malloc_max_bytes =
		lw6sys_max (bazooka_malloc_max_bytes,
			    bazooka_malloc_current_bytes);
	      if (bazooka_eraser && bazooka_data[i].size > size)
		{
		  memset (bazooka_data[i].ptr + size,
			  BAZOOKA_ERASER_REALLOC_1,
			  bazooka_data[i].size - size);
		}
	      break;		// important to leave loop, else serious perfomance problem
	    }
	}
      if (bazooka_trustable && i == bazooka_size)
	{
	  ret = 0;
	}
    }
  bazooka_unlock ();
#endif

  return ret;
}

int
_lw6sys_bazooka_register_realloc_2 (char *ptr, char *ptr2, int size,
				    char *file, int line)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  bazooka_lock ();
  if (bazooka_data)
    {
      int i;
      char *file_only;

      for (i = _hash_index (ptr); i < bazooka_size; ++i)
	{
	  if (bazooka_data[i].ptr == ptr)
	    {
	      if (bazooka_eraser && bazooka_data[i].size < size)
		{
		  memset (ptr2 + bazooka_data[i].size,
			  BAZOOKA_ERASER_REALLOC_2,
			  size - bazooka_data[i].size);
		}
	      memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_t));
	      if (ptr == ptr2)
		{
		  bazooka_data[i].ptr = ptr2;	// was NULL because of memset
		  bazooka_data[i].size = size;
		  file_only = strrchr (file, '/');
		  if (file_only && *file_only)
		    {
		      file_only++;
		    }
		  else
		    {
		      file_only = file;
		    }
		  strncpy (bazooka_data[i].file, file_only,
			   _LW6SYS_BAZOOKA_FILE_SIZE - 1);
		  bazooka_data[i].line = line;
		  bazooka_data[i].timestamp = lw6sys_get_timestamp ();
		}
	      break;		// important to leave loop, else serious perfomance problem           
	    }
	}
      if (bazooka_trustable && i == bazooka_size)
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
	  for (i = _hash_index (ptr2); i < bazooka_size; ++i)
	    {
	      if (bazooka_data[i].ptr == NULL)
		{
		  memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_t));
		  bazooka_data[i].ptr = ptr2;
		  bazooka_data[i].size = size;
		  file_only = strrchr (file, '/');
		  if (file_only && *file_only)
		    {
		      file_only++;
		    }
		  else
		    {
		      file_only = file;
		    }
		  strncpy (bazooka_data[i].file, file_only,
			   _LW6SYS_BAZOOKA_FILE_SIZE - 1);
		  bazooka_data[i].line = line;
		  bazooka_data[i].timestamp = lw6sys_get_timestamp ();
		  break;	// important to leave loop, else serious perfomance problem
		}
	    }
	  if (i == bazooka_size)
	    {
	      _big_cleanup ();
	    }
	}
    }
  bazooka_unlock ();
#endif

  return ret;
}

int
_lw6sys_bazooka_register_free (char *ptr)
{
  int ret = 1;

#ifndef LW6_OPTIMIZE
  int i;

  bazooka_lock ();
  bazooka_free_count++;
  bazooka_malloc_current_count--;
  if (bazooka_free_count > bazooka_malloc_count)
    {
      lw6sys_log_critical (_x_ ("problem, more free (%d) than malloc (%d)"),
			   bazooka_free_count, bazooka_malloc_count);
    }

#ifdef LW6_PARANOID
  {
    int same = 0;

    for (i = _hash_index (ptr), same = 1; i < _LW6SYS_BAZOOKA_ALLOC_MIN; ++i)
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
#endif

  if (bazooka_data)
    {
      for (i = _hash_index (ptr); i < bazooka_size; ++i)
	{
	  if (bazooka_data[i].ptr == ptr)
	    {
	      bazooka_free_bytes += bazooka_data[i].size;
	      bazooka_malloc_current_bytes -= bazooka_data[i].size;
	      if (bazooka_free_bytes > bazooka_malloc_bytes)
		{
		  lw6sys_log_critical (_x_
				       ("problem, more bytes freed (%d) than malloced (%d)"),
				       bazooka_free_bytes,
				       bazooka_malloc_bytes);
		}
	      if (bazooka_eraser)
		{
		  memset (bazooka_data[i].ptr, BAZOOKA_ERASER_FREE,
			  bazooka_data[i].size);
		}
	      memset (&(bazooka_data[i]), 0, sizeof (_lw6sys_bazooka_t));
	      break;		// important to leave loop, else serious perfomance problem
	    }
	}
      if (bazooka_trustable && i == bazooka_size)
	{
	  ret = 0;
	}
    }

  bazooka_unlock ();
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_count
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to 
 * allocate memory, this function will tell you how many times @malloc 
 * has been called.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_count ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = bazooka_malloc_count;
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_free_count
 *
 * Provided you have always called the @LW6SYS_FREE macro to free
 * memory, this function will tell you how many times @free has been called.
 *
 * Return value: the number of calls to @lw6sys_free since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_free_count ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  return bazooka_free_count;
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_current_count
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
lw6sys_get_memory_bazooka_malloc_current_count ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = bazooka_malloc_current_count;
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_max_count
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to 
 * allocate memory, this function will tell you the maximum of pointers
 * returned by @malloc that were present at the same time on the heap.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_max_count ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = bazooka_malloc_max_count;
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_megabytes
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to 
 * allocate memory, this function will tell you how many bytes @malloc 
 * has reserved.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_megabytes ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = (int64_t) (bazooka_malloc_bytes / 0x100000L);
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_free_megabytes
 *
 * Provided you have always called the @LW6SYS_FREE macro to free
 * memory, this function will tell you how many bytes @free has freed.
 *
 * Return value: the number of calls to @lw6sys_free since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_free_megabytes ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = (int64_t) (bazooka_free_bytes / 0x100000L);
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_current_bytes
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
lw6sys_get_memory_bazooka_malloc_current_bytes ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = bazooka_malloc_current_bytes;
#endif

  return ret;
}

/**
 * lw6sys_get_memory_bazooka_malloc_max_bytes
 *
 * Provided you have always called the @LW6SYS_MALLOC an @LW6SYS_CALLOC to 
 * allocate memory, this function will tell you the maximum bytes
 * returned by @malloc that were present at the same time on the heap.
 *
 * Return value: the number of calls to @lw6sys_malloc or @lw6sys_calloc since
 *   program was started.
 */
int
lw6sys_get_memory_bazooka_malloc_max_bytes ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = bazooka_malloc_max_bytes;
#endif

  return ret;
}

/**
 * lw6sys_is_memory_bazooka_trustable
 *
 * Returns true if memory bazooka data are perfectly trustable, that is,
 * it has never been resetted or resized.
 *
 * Return value: 1 if trustable, 0 if not.
 */
int
lw6sys_is_memory_bazooka_trustable ()
{
  int ret = 0;

#ifndef LW6_OPTIMIZE
  ret = bazooka_trustable;
#endif

  return ret;
}

/**
 * lw6sys_memory_bazooka_report
 *
 * Reports memory bazooka diagnostics on the console. Carefull, this
 * one is not reentrant, call at the end of your program when all threads
 * are joined.
 *
 * Return value: 1 if no allocated stuff left, 0 if there are still malloc'ed stuff
 */
int
lw6sys_memory_bazooka_report ()
{
  int ret = 1;

#ifdef LW6_OPTIMIZE
  /*
   * Do not display this systematically (NOTICE) for it
   * could interfere with standard output when processing
   * commands like --version or such things.
   */
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("no memory check"));
#else
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

  if (bazooka_spinlock)
    {
      bazooka_destroy_spinlock ();
      recreate_spinlock = 1;
    }

  malloc_count = lw6sys_get_memory_bazooka_malloc_count ();
  free_count = lw6sys_get_memory_bazooka_free_count ();
  malloc_current_count = lw6sys_get_memory_bazooka_malloc_current_count ();
  malloc_max_count = lw6sys_get_memory_bazooka_malloc_max_count ();
  malloc_megabytes = lw6sys_get_memory_bazooka_malloc_megabytes ();
  free_megabytes = lw6sys_get_memory_bazooka_free_megabytes ();
  malloc_current_bytes = lw6sys_get_memory_bazooka_malloc_current_bytes ();
  malloc_max_bytes = lw6sys_get_memory_bazooka_malloc_max_bytes ();
  malloc_max_megabytes = (malloc_max_bytes >> 20) + 1;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("malloc_count=%d"), malloc_count);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("free_count=%d"), free_count);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("malloc_current_count=%d"),
	      malloc_current_count);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("malloc_max_count=%d"), malloc_max_count);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("malloc_megabytes=%d"), malloc_megabytes);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("free_megabytest=%d"), free_megabytes);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("malloc_current_bytes=%d"),
	      malloc_current_bytes);
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("malloc_max_bytes=%d"), malloc_max_bytes);

  if (malloc_count == free_count)
    {
      if (bazooka_data)
	{
	  if (bazooka_trustable)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _x_
			  ("%d malloc calls, %d megabytes system heap"),
			  malloc_count, malloc_max_megabytes);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("%d malloc calls, %d megabytes system heap (untrusted)"),
			  malloc_count, malloc_max_megabytes);
	    }
	}
    }
  else
    {
      ret = 0;
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("possible memory leak, %d calls to malloc and %d calls to free, note that if the program exited abnormally because of an unexpected error, this difference might be \"normal\""),
		  malloc_count, free_count);
    }

  if (bazooka_data)
    {
      for (i = 0; i < bazooka_size; ++i)
	{
	  if (bazooka_data[i].ptr)
	    {
	      ret = 0;
	      report_line (&(bazooka_data[i]));
	    }
	}
    }

  if (recreate_spinlock)
    {
      bazooka_spinlock = lw6sys_spinlock_create ();
    }
#endif

  return ret;
}
