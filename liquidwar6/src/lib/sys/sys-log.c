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

#include "sys.h"
#include "sys-internal.h"

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <math.h>

#ifdef LW6_GTK
#include <gtk/gtk.h>
#endif

#define _HISTORY_LENGTH 256
#define _LEVEL_LENGTH 80
#define _MSGBOX_BUF_LENGTH 8192
#define _MSGBOX_BT_LENGTH 640
#define _MSGBOX_BT_MIDDLE " ... "
/*
 * For some reason seams on GNU/Linux (gtk2/xorg) systems
 * the message box is trimmed at 64 chars width. 128 makes
 * it transparent. For the "\n" is already here on one over
 * two lines.
 */
#define _MSGBOX_WIDTH 128

#define _CRITICAL_FILE "sys-log.c"
#define _CRITICAL_LINE __LINE__
#define _CRITICAL_FUNC "?"

#define _GTK_NB_IDLE_ITERATIONS 10

static char *
_get_log_file (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  char *log_file = NULL;

  if (global->log_filename[0] != 0)
    {
      log_file = global->log_filename;
    }
  else
    {
      log_file = lw6sys_get_default_log_file (sys_context);
      if (log_file)
	{
	  lw6sys_buf_sprintf (sys_context, global->log_filename, _LW6SYS_LOG_FILENAME_SIZE, "%s", log_file);
	  LW6SYS_FREE (sys_context, log_file);
	  log_file = global->log_filename;
	}
      else
	{
	  lw6sys_log_critical (sys_context, _("log_file is NULL"));
	}
    }

  return log_file;
}

/**
 * lw6sys_log_errno_str
 *
 * @sys_context: global system context
 * @errno_int: the error code, typically errno
 *
 * Convenience fonction which returns the "macro" corresponding
 * to an errno code. I find it easier to use this than bothering
 * declaring a buffer for strerror_r... Besides LW6b has its own
 * error messages. Wil never return NULL, if error does not exists
 * just returns "?".
 *
 * Return value: static string, must not be freed
 */
const char *
lw6sys_log_errno_str (lw6sys_context_t * sys_context, int errno_int)
{
  const char *ret = "?";

  switch (errno_int)
    {
#ifdef E2BIG
    case E2BIG:
      ret = "E2BIG";
      break;
#endif
#ifdef EACCES
    case EACCES:
      ret = "EACCES";
      break;
#endif
#ifdef EADDRINUSE
    case EADDRINUSE:
      ret = "EADDRINUSE";
      break;
#endif
#ifdef EADDRNOTAVAIL
    case EADDRNOTAVAIL:
      ret = "EADDRNOTAVAIL";
      break;
#endif
#ifdef EAFNOSUPPORT
    case EAFNOSUPPORT:
      ret = "EAFNOSUPPORT";
      break;
#endif
#ifdef EAGAIN
    case EAGAIN:		// == EWOULDBLOCK
      ret = "EAGAIN";
      break;
#endif
#ifdef EALREADY
    case EALREADY:
      ret = "EALREADY";
      break;
#endif
#ifdef EBADF
    case EBADF:
      ret = "EBADF";
      break;
#endif
#ifdef EBADMSG
    case EBADMSG:
      ret = "EBADMSG";
      break;
#endif
#ifdef EBUSY
    case EBUSY:
      ret = "EBUSY";
      break;
#endif
#ifdef ECANCELED
    case ECANCELED:
      ret = "ECANCELED";
      break;
#endif
#ifdef ECHILD
    case ECHILD:
      ret = "ECHILD";
      break;
#endif
#ifdef ECONNABORTED
    case ECONNABORTED:
      ret = "ECONNABORTED";
      break;
#endif
#ifdef ECONNREFUSED
    case ECONNREFUSED:
      ret = "ECONNREFUSED";
      break;
#endif
#ifdef ECONNRESET
    case ECONNRESET:
      ret = "ECONNRESET";
      break;
#endif
#ifdef EDEADLK
    case EDEADLK:
      ret = "EDEADLK";
      break;
#endif
#ifdef EDESTADDRREQ
    case EDESTADDRREQ:
      ret = "EDESTADDRREQ";
      break;
#endif
#ifdef EDOM
    case EDOM:
      ret = "EDOM";
      break;
#endif
#ifdef EDQUOT
    case EDQUOT:
      ret = "EDQUOT";
      break;
#endif
#ifdef EEXIST
    case EEXIST:
      ret = "EEXIST";
      break;
#endif
#ifdef EFAULT
    case EFAULT:
      ret = "EFAULT";
      break;
#endif
#ifdef EFBIG
    case EFBIG:
      ret = "EFBIG";
      break;
#endif
#ifdef EHOSTUNREACH
    case EHOSTUNREACH:
      ret = "EHOSTUNREACH";
      break;
#endif
#ifdef EIDRM
    case EIDRM:
      ret = "EIDRM";
      break;
#endif
#ifdef EILSEQ
    case EILSEQ:
      ret = "EILSEQ";
      break;
#endif
#ifdef EINPROGRESS
    case EINPROGRESS:
      ret = "EINPROGRESS";
      break;
#endif
#ifdef EINTR
    case EINTR:
      ret = "EINTR";
      break;
#endif
#ifdef EINVAL
    case EINVAL:
      ret = "EINVAL";
      break;
#endif
#ifdef EIO
    case EIO:
      ret = "EIO";
      break;
#endif
#ifdef EISCONN
    case EISCONN:
      ret = "EISCONN";
      break;
#endif
#ifdef EISDIR
    case EISDIR:
      ret = "EISDIR";
      break;
#endif
#ifdef ELOOP
    case ELOOP:
      ret = "ELOOP";
      break;
#endif
#ifdef EMFILE
    case EMFILE:
      ret = "EMFILE";
      break;
#endif
#ifdef EMLINK
    case EMLINK:
      ret = "EMLINK";
      break;
#endif
#ifdef EMSGSIZE
    case EMSGSIZE:
      ret = "EMSGSIZE";
      break;
#endif
#ifdef EMULTIHOP
    case EMULTIHOP:
      ret = "EMULTIHOP";
      break;
#endif
#ifdef ENAMETOOLONG
    case ENAMETOOLONG:
      ret = "ENAMETOOLONG";
      break;
#endif
#ifdef ENETDOWN
    case ENETDOWN:
      ret = "ENETDOWN";
      break;
#endif
#ifdef ENETRESET
    case ENETRESET:
      ret = "ENETRESET";
      break;
#endif
#ifdef ENETUNREACH
    case ENETUNREACH:
      ret = "ENETUNREACH";
      break;
#endif
#ifdef ENFILE
    case ENFILE:
      ret = "ENFILE";
      break;
#endif
#ifdef ENOBUFS
    case ENOBUFS:
      ret = "ENOBUFS";
      break;
#endif
#ifdef ENODATA
    case ENODATA:
      ret = "ENODATA";
      break;
#endif
#ifdef ENODEV
    case ENODEV:
      ret = "ENODEV";
      break;
#endif
#ifdef ENOENT
    case ENOENT:
      ret = "ENOENT";
      break;
#endif
#ifdef ENOEXEC
    case ENOEXEC:
      ret = "ENOEXEC";
      break;
#endif
#ifdef ENOLCK
    case ENOLCK:
      ret = "ENOLCK";
      break;
#endif
#ifdef ENOLINK
    case ENOLINK:
      ret = "ENOLINK";
      break;
#endif
#ifdef ENOMEM
    case ENOMEM:
      ret = "ENOMEM";
      break;
#endif
#ifdef ENOMSG
    case ENOMSG:
      ret = "ENOMSG";
      break;
#endif
#ifdef ENOPROTOOPT
    case ENOPROTOOPT:
      ret = "ENOPROTOOPT";
      break;
#endif
#ifdef ENOSPC
    case ENOSPC:
      ret = "ENOSPC";
      break;
#endif
#ifdef ENOSR
    case ENOSR:
      ret = "ENOSR";
      break;
#endif
#ifdef ENOSTR
    case ENOSTR:
      ret = "ENOSTR";
      break;
#endif
#ifdef ENOSYS
    case ENOSYS:
      ret = "ENOSYS";
      break;
#endif
#ifdef ENOTCONN
    case ENOTCONN:
      ret = "ENOTCONN";
      break;
#endif
#ifdef ENOTDIR
    case ENOTDIR:
      ret = "ENOTDIR";
      break;
#endif
#ifdef ENOTEMPTY
    case ENOTEMPTY:
      ret = "ENOTEMPTY";
      break;
#endif
#ifdef ENOTSOCK
    case ENOTSOCK:
      ret = "ENOTSOCK";
      break;
#endif
#ifdef ENOTSUP
    case ENOTSUP:		// == EOPNOTSUPP
      ret = "ENOTSUP";
      break;
#endif
#ifdef ENOTTY
    case ENOTTY:
      ret = "ENOTTY";
      break;
#endif
#ifdef ENXIO
    case ENXIO:
      ret = "ENXIO";
      break;
#endif
#ifdef EOVERFLOW
    case EOVERFLOW:
      ret = "EOVERFLOW";
      break;
#endif
#ifdef EPERM
    case EPERM:
      ret = "EPERM";
      break;
#endif
#ifdef EPIPE
    case EPIPE:
      ret = "EPIPE";
      break;
#endif
#ifdef EPROTO
    case EPROTO:
      ret = "EPROTO";
      break;
#endif
#ifdef EPROTONOSUPPORT
    case EPROTONOSUPPORT:
      ret = "EPROTONOSUPPORT";
      break;
#endif
#ifdef EPROTOTYPE
    case EPROTOTYPE:
      ret = "EPROTOTYPE";
      break;
#endif
#ifdef ERANGE
    case ERANGE:
      ret = "ERANGE";
      break;
#endif
#ifdef EROFS
    case EROFS:
      ret = "EROFS";
      break;
#endif
#ifdef ESPIPE
    case ESPIPE:
      ret = "ESPIPE";
      break;
#endif
#ifdef ESRCH
    case ESRCH:
      ret = "ESRCH";
      break;
#endif
#ifdef ESTALE
    case ESTALE:
      ret = "ESTALE";
      break;
#endif
#ifdef ETIME
    case ETIME:
      ret = "ETIME";
      break;
#endif
#ifdef ETIMEDOUT
    case ETIMEDOUT:
      ret = "ETIMEDOUT";
      break;
#endif
#ifdef ETXTBSY
    case ETXTBSY:
      ret = "ETXTBSY";
      break;
#endif
#ifdef EXDEV
    case EXDEV:
      ret = "EXDEV";
      break;
#endif
    }

  return ret;
}

/**
 * lw6sys_log_set_file:
 *
 * @sys_context: global system context
 * @filename: the name of the log file.
 *
 * Sets up the log file. Until you call this function, messages
 * all logged to the default log file, as returned by
 * the @lw6sys_get_default_log_file function.
 *
 * Return value: void
 */
void
lw6sys_log_set_file (lw6sys_context_t * sys_context, const char *filename)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  memset (global->log_filename, 0, sizeof (global->log_filename));
  memcpy (global->log_filename, filename, lw6sys_imin (sizeof (global->log_filename) - 1, strlen (filename)));
}

/**
 * lw6sys_log_clear:
 *
 * @sys_context: global system context
 * @filename: the name of the log file.
 *
 * Clears the log file, this function would typically be called
 * at the beginning of the program. If filename is NULL, then the
 * default log file is cleared.
 *
 * Return value: void
 */
void
lw6sys_log_clear (lw6sys_context_t * sys_context, const char *filename)
{
  char *default_log_file = NULL;

  if (filename)
    {
      lw6sys_log_set_file (sys_context, filename);
    }
  else
    {
      default_log_file = lw6sys_get_default_log_file (sys_context);
      filename = default_log_file;
    }

  if (filename)
    {
      lw6sys_clear_file (sys_context, filename);
      if (!lw6sys_file_exists (sys_context, filename))
	{
	  lw6sys_log_critical (sys_context, _("can't open log file \"%s\""), filename);
	}
    }

  if (default_log_file)
    {
      LW6SYS_FREE (sys_context, default_log_file);
    }
}

/**
 * lw6sys_log_set_dialog_timeout
 *
 * @sys_context: global system context
 * @timeout_sec: number of seconds to wait before alert dialogs disappear
 *
 * By default, alert boxes will stay out forever unless one clicks on them,
 * however, this parameter will force the dialog shutdown after some time.
 * Mostly used for testing, to allow tests being blocked on a dialog.
 *
 * Return value: 1 if timeout is supported on platform, 0 if not
 */
int
lw6sys_log_set_dialog_timeout (lw6sys_context_t * sys_context, int timeout_sec)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  global->dialog_timeout_msec = LW6SYS_TICKS_PER_SEC * lw6sys_imax (0,
								    lw6sys_imin (LW6SYS_DIALOG_TIMEOUT_MAX,
										 timeout_sec));

#ifdef LW6_GTK
  return 1;
#else // LW6_GTK
  return 0;
#endif //LW6_GTK
}

static FILE *
_open_log_file (lw6sys_context_t * sys_context)
{
  FILE *ret = NULL;
  char *name;

  name = _get_log_file (sys_context);
  ret = fopen (name, "ab");

  return ret;
}

static void
_log_to_file (lw6sys_context_t * sys_context, FILE * f, int level_id,
	      const char *level_str, const char *file, int line, const char *func, const char *fmt, va_list ap)
{
  time_t t_now;
  struct tm tm_now;
  struct tm *tm_ret = NULL;
  struct timeval timeval_now;
  char *bt = NULL;
  int free_bt = 0;
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  switch (global->log_backtrace_mode)
    {
    case LW6SYS_LOG_BACKTRACE_MODE_FUNC:
      bt = (char *) func;
      break;
    case LW6SYS_LOG_BACKTRACE_MODE_FULL:
      bt = lw6sys_backtrace (sys_context, 1, 0);
      if (bt)
	{
	  free_bt = 1;
	}
      break;
    default:
      bt = _CRITICAL_FUNC;
      break;
    }

  memset (&t_now, 0, sizeof (time_t));
  memset (&tm_now, 0, sizeof (struct tm));
  memset (&timeval_now, 0, sizeof (struct timeval));
  gettimeofday (&timeval_now, NULL);
  t_now = timeval_now.tv_sec;
  tm_ret = localtime_r (&t_now, &tm_now);
  if (tm_ret == &tm_now)
    {
      fprintf (f, "%04d-%02d-%02d %02d:%02d:%02d.%03d\t%s:%d/%s\t%d\t%s\t",
	       (int) tm_now.tm_year + 1900, (int) tm_now.tm_mon + 1,
	       (int) tm_now.tm_mday, (int) tm_now.tm_hour,
	       (int) tm_now.tm_min, (int) tm_now.tm_sec,
	       (int) timeval_now.tv_usec / (int) (1000000 /
						  LW6SYS_TICKS_PER_SEC), file, line, lw6sys_str_empty_if_null (sys_context, bt), level_id, level_str);
    }
  vfprintf (f, fmt, ap);

  if (free_bt)
    {
      LW6SYS_FREE (sys_context, bt);
    }

  fprintf (f, "\n");
  fflush (f);
}

static void
_log_to_console (lw6sys_context_t * sys_context, FILE * f, const char *level_str, const char *fmt, va_list ap)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  if (global->console_enable_state)
    {
      time_t t_now;
      struct tm tm_now;
      struct tm *tm_ret = NULL;
      struct timeval timeval_now;

      memset (&t_now, 0, sizeof (time_t));
      memset (&tm_now, 0, sizeof (struct tm));
      memset (&timeval_now, 0, sizeof (struct timeval));
      gettimeofday (&timeval_now, NULL);
      t_now = timeval_now.tv_sec;
      tm_ret = localtime_r (&t_now, &tm_now);
      if (tm_ret == &tm_now)
	{
	  fprintf (f, "%s: (%02d:%02d:%02d) %s",
		   lw6sys_build_get_package_tarname (), (int) tm_now.tm_hour, (int) tm_now.tm_min, (int) tm_now.tm_sec, level_str);
	  vfprintf (f, fmt, ap);
	  fprintf (f, "\n");
	  fflush (f);
	}
    }
}

static void
_log_to_history (lw6sys_context_t * sys_context, const char *level_str, const char *fmt, va_list ap)
{
  char msg[_HISTORY_LENGTH + 1];
  char full_msg[_HISTORY_LENGTH + 1];

  _lw6sys_buf_vsnprintf (sys_context, msg, _HISTORY_LENGTH, fmt, ap);
  lw6sys_buf_sprintf (sys_context, full_msg, _HISTORY_LENGTH, "%s%s", level_str, msg);
  lw6sys_history_register (sys_context, full_msg);
}

#ifdef LW6_GTK
static void
_gtk_dlg_timeout_callback (lw6sys_context_t * sys_context, void *data)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  GtkDialog **dlg_timeout_ptr = (GtkDialog **) data;
  int64_t start_time = lw6sys_get_timestamp (sys_context);
  int64_t end_time = start_time + global->dialog_timeout_msec;

  while (((*dlg_timeout_ptr) != NULL) && lw6sys_get_timestamp (sys_context) < end_time)
    {
      lw6sys_idle (sys_context);
    }
  if (*dlg_timeout_ptr)
    {
      gtk_dialog_response (*dlg_timeout_ptr, GTK_RESPONSE_OK);
    }
}
#endif // LW6_GTK

/*
 * This function not declared static so that we're sure it's never inlined
 * because that could wreck the backtrace construction. It's not exported
 * in the sys/sys.h header anyway, we don't want others to use it, it still
 * does have the _lw6sys_ prefix to avoid name conflicts.
 */
void
_lw6sys_msgbox_alert (lw6sys_context_t * sys_context, const char *level_str, const char *file, int line, const char *func, const char *fmt, va_list ap)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);
  char message_raw[_MSGBOX_BUF_LENGTH + 1];
  char message_full[_MSGBOX_BUF_LENGTH + 1];
  char *bt = NULL;
  int free_bt = 0;

  _lw6sys_buf_vsnprintf (sys_context, message_raw, _MSGBOX_BUF_LENGTH, fmt, ap);
  lw6sys_str_reformat_this (sys_context, message_raw, _MSGBOX_WIDTH);

  switch (global->log_backtrace_mode)
    {
    case LW6SYS_LOG_BACKTRACE_MODE_FUNC:
      bt = (char *) func;
      break;
    case LW6SYS_LOG_BACKTRACE_MODE_FULL:
      bt = lw6sys_backtrace (sys_context, 2, 1);	// skip this function & caller
      if (bt)
	{
	  lw6sys_str_truncate_middle (sys_context, bt, _MSGBOX_BT_LENGTH, _MSGBOX_BT_MIDDLE);
	  lw6sys_str_reformat_this (sys_context, bt, _MSGBOX_WIDTH);
	  free_bt = 1;
	}
      break;
    default:
      bt = _CRITICAL_FUNC;
      break;
    }

  lw6sys_buf_sprintf (sys_context, message_full, _MSGBOX_BUF_LENGTH,
		      "%s (%s:%d)\n\n%s\n\n%s: %s\n\n%s: %s", level_str, file,
		      line, message_raw, _("Backtrace"), lw6sys_str_empty_if_null (sys_context, bt), _("Report bugs"), lw6sys_build_get_bugs_url ());
  if (free_bt)
    {
      LW6SYS_FREE (sys_context, bt);
      bt = NULL;
    }
#ifdef LW6_MS_WINDOWS
  if (message_full)
    {
      MessageBox (NULL, message_full, lw6sys_build_get_package_name (), MB_OK | MB_ICONERROR | MB_APPLMODAL);
    }
#else
#ifdef LW6_MAC_OS_X
  _lw6sys_macosx_alert (sys_context, lw6sys_build_get_package_name (), message_full);
#else
/*
 * Not implemented yet, below sample code, which would require to
 * link against gtk2, with:
 * CFLAGS=`pkg-config --cflags gtk+-2.0`
 * LDFLAGS=`pkg-config --libs gtk+-2.0`
 */
#ifdef LW6_GTK
  static int gtk_init_done = 0;
  /*
   * We fake argc & argv, OK this way GTK won't
   * be aware of *real* argc & argv values, but
   * passing them everywhere just for this is a pain.
   * calloc 2 pointers, last will be NULL, not
   * sure this is mandatory, but just in case.
   * Additionnally, argv must be mutable, this
   * justifies the argv_data hack. And... content is
   * static, never know if gtk needs them to stay
   * here, it might do as argc & argv usually
   * do not disappear within a program.
   */
  static char empty_str[1];
  static char *argv_data[2];
  int argc = 1;
  char **argv = NULL;
  GtkWidget *dlg = NULL;
  GtkWidget *dlg_timeout = NULL;
  lw6sys_thread_handler_t *timeout_thread = NULL;

  empty_str[0] = '\0';
  argv_data[0] = empty_str;
  argv_data[1] = NULL;
  argv = argv_data;
  if (!gtk_init_done)
    {
      if (gtk_init_check (&argc, &argv))
	{
	  gtk_init_done = 1;
	}
      // no "else" here, we won't log inside a log function
    }
  if (gtk_init_done)
    {
      dlg = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_NONE, "%s", message_full);
      if (dlg)
	{
	  gtk_window_set_title (GTK_WINDOW (dlg), lw6sys_build_get_package_name ());
	  gtk_dialog_add_button (GTK_DIALOG (dlg), GTK_STOCK_OK, GTK_RESPONSE_OK);
	  dlg_timeout = dlg;
	  if (global->dialog_timeout_msec > 0)
	    {
	      timeout_thread = lw6sys_thread_create (sys_context, _gtk_dlg_timeout_callback, NULL, (void *) &dlg_timeout);
	    }
	  /*
	   * Here we don't log on error, it's not a really good idea to call
	   * ourselves, side effects could be worse than remedy.
	   */
	  gtk_dialog_run (GTK_DIALOG (dlg));
	  /*
	   * It's important to set dlg_timeout to NULL so that
	   * timeout thread does finish when we click on OK.
	   */
	  dlg_timeout = NULL;
	  if (timeout_thread)
	    {
	      lw6sys_thread_join (sys_context, timeout_thread);
	      timeout_thread = NULL;
	    }
	  gtk_widget_hide_all (dlg);
	  gtk_widget_destroy (dlg);
	  dlg = NULL;

	  while (gtk_events_pending ())
	    {
	      gtk_main_iteration ();
	      lw6sys_idle (sys_context);
	    }
	}
    }
#endif
#endif
#endif
}

/**
 * lw6sys_log:
 *
 * @sys_context: global system context
 * @level_id: the log level to use. Possible values are, by order,
 *   LW6SYS_LOG_ERROR_ID (0),
 *   LW6SYS_LOG_WARNING_ID (1),
 *   LW6SYS_LOG_NOTICE_ID (2),
 *   LW6SYS_LOG_INFO_ID (3),
 *   LW6SYS_LOG_DEBUG_ID (4) and
 *   LW6SYS_LOG_TMP_ID (5).
 * @file: the name of the source file where the function is called,
 *   one can use __FILE__
 * @line: the line in the source file where the function is called,
 *   one can use __LINE__
 * @func: the name of the function where this log line was called,
 *   on can use __FUNCTION__
 * @fmt: a printf-like format string
 * @...: printf-like arguments, corresponding to @fmt.
 *
 * This function is usually called with the first three arguments
 * packed into a single macro. For instance the
 * @LW6SYS_LOG_WARNING macro expands and fills the first 3
 * args, so there's no need to type __FILE__ and __LINE__ again
 * and again. Note that this function will reset errno. The
 * idea is to call it whenever there's something to do with errno
 * (if you deal with errno, it's a good habit to log it) then
 * errno is cleared so that it won't interfere with next log messages.
 *
 * @Return value: void
 */
void
lw6sys_log (lw6sys_context_t * sys_context, int level_id, const char *file, int line, const char *func, const char *fmt, ...)
{
#ifdef LW6_OPTIMIZE
  if ((level_id <= LW6SYS_LOG_NOTICE_ID && level_id <= lw6sys_log_get_level (sys_context)) || level_id == LW6SYS_LOG_TMP_ID || lw6sys_debug_get (sys_context))
#else
  if (level_id <= lw6sys_log_get_level (sys_context) || level_id == LW6SYS_LOG_TMP_ID || lw6sys_debug_get (sys_context))
#endif
    {
      char level_str[_LEVEL_LENGTH + 1];
#ifdef HAVE_SYSLOG_H
      int syslog_priority = 0;
#endif
      FILE *f;
      int errno_int;
      va_list ap;
      va_list ap2;
      const char *file_only = NULL;

      errno_int = errno;
      level_str[0] = '\0';

      file_only = strrchr (file, '/');
      if (file_only && *file_only)
	{
	  file_only++;
	}
      else
	{
	  file_only = file;
	}

      va_start (ap, fmt);

      switch (level_id)
	{
	case LW6SYS_LOG_ERROR_ID:
	  if (errno_int)
	    {
	      lw6sys_buf_sprintf (sys_context, level_str, _LEVEL_LENGTH, _("ERROR! (errno=%d:%s) "), errno_int, lw6sys_log_errno_str (sys_context, errno_int));
	    }
	  else
	    {
	      lw6sys_buf_sprintf (sys_context, level_str, _LEVEL_LENGTH, _("ERROR! "));
	    }
#ifdef HAVE_SYSLOG_H
	  syslog_priority = LOG_ERR;
#endif
	  break;
	case LW6SYS_LOG_WARNING_ID:
	  if (errno_int)
	    {
	      lw6sys_buf_sprintf (sys_context, level_str, _LEVEL_LENGTH,
				  _("WARNING! (errno=%d:%s) "), errno_int, lw6sys_log_errno_str (sys_context, errno_int));
	    }
	  else
	    {
	      lw6sys_buf_sprintf (sys_context, level_str, _LEVEL_LENGTH, _("WARNING! "));
	    }
#ifdef HAVE_SYSLOG_H
	  syslog_priority = LOG_WARNING;
#endif
	  break;
	case LW6SYS_LOG_NOTICE_ID:
#ifdef HAVE_SYSLOG_H
	  syslog_priority = LOG_NOTICE;
#endif
	  break;
	case LW6SYS_LOG_INFO_ID:
#ifdef HAVE_SYSLOG_H
	  syslog_priority = LOG_INFO;
#endif
	  break;
	case LW6SYS_LOG_TMP_ID:
	  lw6sys_buf_sprintf (sys_context, level_str, _LEVEL_LENGTH, _("[tmp] "));
#ifdef HAVE_SYSLOG_H
	  syslog_priority = LOG_DEBUG;
#endif
	  break;
	default:		// LW6SYS_LOG_DEBUG_ID
	  lw6sys_buf_sprintf (sys_context, level_str, _LEVEL_LENGTH, _("[debug] "));
#ifdef HAVE_SYSLOG_H
	  syslog_priority = LOG_DEBUG;
#endif
	  break;
	}

      if (fmt != NULL)
	{
	  // exclude INFO & DEBUG from console in non-debug mode
	  if (level_id <= LW6SYS_LOG_NOTICE_ID || level_id == LW6SYS_LOG_TMP_ID || lw6sys_debug_get (sys_context))
	    {
	      va_copy (ap2, ap);
	      _log_to_console (sys_context, stderr, level_str, fmt, ap2);
	      va_end (ap2);
	    }
	  if (level_id <= LW6SYS_LOG_WARNING_ID || level_id == LW6SYS_LOG_TMP_ID)
	    {
	      va_copy (ap2, ap);
	      _log_to_history (sys_context, level_str, fmt, ap2);
	      va_end (ap2);
	    }
#ifdef HAVE_SYSLOG_H
	  if (level_id <= LW6SYS_LOG_WARNING_ID || level_id == LW6SYS_LOG_TMP_ID)
	    {
	      va_copy (ap2, ap);
	      vsyslog (syslog_priority, fmt, ap2);
	      va_end (ap2);
	    }
#endif
	  f = _open_log_file (sys_context);
	  if (f)
	    {
	      va_copy (ap2, ap);
	      _log_to_file (sys_context, f, level_id, level_str, file_only, line, func, fmt, ap2);
	      va_end (ap2);
	      fclose (f);
	    }
	  if (level_id <= LW6SYS_LOG_ERROR_ID)
	    {
	      va_copy (ap2, ap);
	      _lw6sys_msgbox_alert (sys_context, level_str, file_only, line, func, fmt, ap2);
	      va_end (ap2);
	    }
	}

      va_end (ap);
    }

  /*
   * We reset errno to avoid getting pointless errno in logs,
   * the idea is to print errno when needed...
   */
  errno = 0;
}

/**
 * lw6sys_log_critical:
 *
 * @sys_context: global system context
 * @fmt: a printf-like format string
 * @...: printf-like arguments, corresponding to @fmt.
 *
 * This function is a special log function which will dump informations
 * on the console only, without opening any log file whatsoever. The idea
 * is that it's a "never fail" function. Additionnally, it will never
 * return but quit the program. This can be used as an ultimate emergency
 * function, use it when the program won't run for sure, and displaying
 * an immediate error message is the only issue.
 *
 * @Return value: void
 */
void
lw6sys_log_critical (lw6sys_context_t * sys_context, const char *fmt, ...)
{
  va_list ap;
  va_list ap2;

  va_start (ap, fmt);

  va_copy (ap2, ap);
  _log_to_console (sys_context, stderr, _("CRITICAL! "), fmt, ap2);
  va_end (ap2);

  va_copy (ap2, ap);
  _lw6sys_msgbox_alert (sys_context, _("CRITICAL!"), _CRITICAL_FILE, _CRITICAL_LINE, _CRITICAL_FUNC, fmt, ap2);
  va_end (ap2);

  exit (LW6SYS_EXIT_CRITICAL);

  va_end (ap);
}

/**
 * lw6sys_log_get_level
 *
 * @sys_context: global system context
 *
 * Get the current log level. 
 *
 * @Return value: the log level, integer between 0 & 4. 4 is very verbose
 * (debug), 0 displays errors only.
 */
int
lw6sys_log_get_level (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  return global->log_level;
}

/**
 * lw6sys_log_set_level
 *
 * @sys_context: global system context
 * @level: the log level, integer between 0 & 4. 4 is very verbose (debug),
 * 0 displays errors only.
 *
 * Set the current log level.
 *
 * @Return valaue: none.
 */
void
lw6sys_log_set_level (lw6sys_context_t * sys_context, int level)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  level = lw6sys_imin (level, LW6SYS_LOG_DEBUG_ID);
  level = lw6sys_imax (level, LW6SYS_LOG_ERROR_ID);

  global->log_level = level;
}

/**
 * lw6sys_log_get_backtrace_mode
 *
 * @sys_context: global system context
 *
 * Gets the current backtrace mode.
 *
 * @Return value: the backtrace mode, LW6SYS_LOG_BACKTRACE_MODE_FULL or
 * LW6SYS_LOG_BACKTRACE_MODE_FUNC.
 */
int
lw6sys_log_get_backtrace_mode (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  return global->log_backtrace_mode;
}

/**
 * lw6sys_log_set_backtrace_mode
 *
 * Sets the current backtrace mode.
 *
 * @sys_context: global system context
 * @backtrace_mode: the backtrace mode, LW6SYS_LOG_BACKTRACE_MODE_FULL or
 * LW6SYS_LOG_BACKTRACE_MODE_FUNC.
 *
 * Return value : none
 */
void
lw6sys_log_set_backtrace_mode (lw6sys_context_t * sys_context, int backtrace_mode)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  backtrace_mode = backtrace_mode ? LW6SYS_LOG_BACKTRACE_MODE_FULL : LW6SYS_LOG_BACKTRACE_MODE_FUNC;

  global->log_backtrace_mode = backtrace_mode;
}

/**
 * lw6sys_log_get_console_state
 *
 * @sys_context: global system context
 *
 * Get the console output state. This is important, for instance to
 * set the console "back in the state it was" after setting it on or off.
 *
 * Return value: 1 if enabled, 0 if not.
 */
int
lw6sys_log_get_console_state (lw6sys_context_t * sys_context)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  return global->console_enable_state ? 1 : 0;
}

/**
 * lw6sys_log_set_console_state
 *
 * @sys_context: global system context
 * @state: 1 to activate console output, 0 to disable it.
 *
 * Enables or disables console output. By console output, we basically
 * mean stderr (and possibly stdout). If console output is enabled (the default)
 * all output is copied to stderr. If it's disabled, only the log file
 * will contain the information.
 *
 * Return value: none.
 */
void
lw6sys_log_set_console_state (lw6sys_context_t * sys_context, int state)
{
  _lw6sys_global_t *global = &(((_lw6sys_context_t *) sys_context)->global);

  global->console_enable_state = state ? 1 : 0;
}
