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

#ifndef LIQUIDWAR6SYS_INTERNAL_H
#define LIQUIDWAR6SYS_INTERNAL_H

#include <pthread.h>
#include <stdarg.h>
#include <sys/time.h>

#include "sys.h"

/*
 * By default memory bazooka size is just hudge, will eat up to 7 or 8 megs.
 * The game will set it up to a much lower value through its config, still
 * we keep this very high value for the purpose of debugging. All test
 * programs will use this.
 */
#ifdef LW6_PARANOID
#define _LW6SYS_BAZOOKA_SAMPLE_SIZE 1000
#define _LW6SYS_BAZOOKA_ALLOC_MIN 16
#define _LW6SYS_CALLER_INFO_FILE_SIZE 80
#define _LW6SYS_CALLER_INFO_FUNC_SIZE 50
#else // LW6_PARANOID
#define _LW6SYS_BAZOOKA_SAMPLE_SIZE 100
#define _LW6SYS_CALLER_INFO_FILE_SIZE 50
#define _LW6SYS_CALLER_INFO_FUNC_SIZE 30
// ALLOC_MIN not defined in non-paranoid mode
#endif // LW6_PARANOID

#define _LW6SYS_DEBUG_DEFAULT 0
#define _LW6SYS_QUIT_DEFAULT 0

// ticks limit set to 15 minutes
#define _LW6SYS_TICKS_LIMIT 900000

#define _LW6SYS_PTHREAD_COND_TIMEDWAIT_SEC 0
#define _LW6SYS_PTHREAD_COND_TIMEDWAIT_NSEC 10000000
#define _LW6SYS_VTHREAD_COND_TIMEDWAIT_SEC 0
#define _LW6SYS_VTHREAD_COND_TIMEDWAIT_NSEC 100000000
#define _LW6SYS_VTHREAD_ID -1

typedef struct _lw6sys_caller_info_s
{
  char file[_LW6SYS_CALLER_INFO_FILE_SIZE];
  int line;
  char func[_LW6SYS_CALLER_INFO_FUNC_SIZE];
} _lw6sys_caller_info_t;

typedef struct _lw6sys_bazooka_line_s
{
  void *ptr;
  int size;
  _lw6sys_caller_info_t alloc_info;
  int64_t timestamp;
} _lw6sys_bazooka_line_t;

typedef struct _lw6sys_bazooka_context_s
{
  int size;
  _lw6sys_bazooka_line_t *data;
  lw6sys_spinlock_t *spinlock;
  int malloc_count;
  int free_count;
  int malloc_current_count;
  int malloc_max_count;
  int64_t malloc_bytes;
  int64_t free_bytes;
  int malloc_current_bytes;
  int malloc_max_bytes;
  int eraser;
  int trustable;
} _lw6sys_bazooka_context_t;

typedef struct _lw6sys_mutex_s
{
  u_int32_t id;
  pthread_mutex_t mutex;
  _lw6sys_caller_info_t create_info;
  _lw6sys_caller_info_t lock_info;
} _lw6sys_mutex_t;

#if ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
typedef struct _lw6sys_spinlock_s
{
  u_int32_t id;
  pthread_spinlock_t spinlock;
} _lw6sys_spinlock_t;
#else // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)
#ifdef LW6_X86
typedef struct _lw6sys_spinlock_s
{
  u_int32_t id;
  volatile int32_t test_and_set;
} _lw6sys_spinlock_t;
#else // LW6_X86
#ifdef LW6_AMD64
typedef struct _lw6sys_spinlock_s
{
  u_int32_t id;
  volatile int64_t test_and_set;
} _lw6sys_spinlock_t;
#else // LW6_AMD64
#define _lw6sys_spinlock_s _lw6sys_mutex_s
#define _lw6sys_spinlock_t _lw6sys_mutex_t
#endif // LW6_AMD64
#endif // LW6_X86
#endif // ((_POSIX_SPIN_LOCKS - 200112L) >= 0L)

typedef struct _lw6sys_thread_handler_s
{
  u_int32_t id;
  lw6sys_context_t *sys_context;
  pthread_t thread;
  pthread_mutex_t mutex;
  pthread_cond_t cond_callback_done;
  volatile int flag_callback_done;
  pthread_cond_t cond_can_join;
  volatile int flag_can_join;
  lw6sys_thread_callback_func_t callback_func;
  lw6sys_thread_callback_func_t callback_join;
  void *callback_data;
#ifndef LW6_MS_WINDOWS
  struct itimerval itimer;
#endif				// LW6_MS_WINDOWS
} _lw6sys_thread_handler_t;

#define _LW6SYS_LOG_FILENAME_SIZE 65535

typedef struct _lw6sys_global_s
{
  volatile int debug;
  volatile int log_level;
  volatile int log_backtrace_mode;
  volatile int quit;
  char log_filename[_LW6SYS_LOG_FILENAME_SIZE + 1];
  volatile int dialog_timeout_msec;
  volatile int console_enable_state;
  volatile int mutex_lock_counter;
  volatile int mutex_unlock_counter;
  volatile int sdl_registered_use;
  lw6sys_mutex_t *cunit_mutex;
  volatile int thread_create_counter;
  volatile int thread_join_counter;
  lw6sys_thread_handler_t *vthread_run_handler;
  _lw6sys_thread_handler_t *vthread_main_handler;
} _lw6sys_global_t;

typedef struct _lw6sys_context_s
{
  u_int32_t id;
  _lw6sys_global_t global;
  _lw6sys_bazooka_context_t bazooka_context;
}
_lw6sys_context_t;

/* sys-bazooka.c */
extern void _lw6sys_bazooka_context_init (lw6sys_context_t * sys_context, _lw6sys_bazooka_context_t * bazooka_context);
extern int _lw6sys_bazooka_register_malloc (lw6sys_context_t * sys_context, char *ptr, int size, const char *file, int line, const char *func);
extern int _lw6sys_bazooka_register_calloc (lw6sys_context_t * sys_context, char *ptr, int size, const char *file, int line, const char *func);
extern int _lw6sys_bazooka_register_realloc_1 (lw6sys_context_t * sys_context, char *ptr, int size, const char *file, int line, const char *func);
extern int _lw6sys_bazooka_register_realloc_2 (lw6sys_context_t * sys_context, char *ptr, char *ptr2, int size, const char *file, int line, const char *func);
extern int _lw6sys_bazooka_register_free (lw6sys_context_t * sys_context, char *ptr);

/* sys-callerinfo.c */
extern void _lw6sys_caller_info_set (_lw6sys_caller_info_t * caller_info, const char *file, int line, const char *func);
extern void _lw6sys_caller_info_clear (_lw6sys_caller_info_t * caller_info);

/* sys-path.c */
extern const char *_lw6sys_path_file_only_raw (const char *file);

/* sys-str.c */
extern char *_lw6sys_new_vsnprintf (lw6sys_context_t * sys_context, int n, const char *fmt, va_list ap);
extern int _lw6sys_buf_vsnprintf (lw6sys_context_t * sys_context, char *buf, int len, const char *fmt, va_list ap);

/* sys-testandset.s */
#ifdef LW6_X86
extern int32_t _lw6sys_test_and_set_asm (volatile int32_t * test_and_set);
#endif // LW6_X86
#ifdef LW6_AMD64
extern int64_t _lw6sys_test_and_set_asm (volatile int64_t * test_and_set);
#endif // LW6_AMD64

/* sys_thread.c */
extern void _lw6sys_thread_callback (void *thread_handler);

#ifdef LW6_MAC_OS_X
extern void _lw6sys_macosx_alert (lw6sys_context_t * sys_context, const char *title, const char *msg);
#endif

#endif
