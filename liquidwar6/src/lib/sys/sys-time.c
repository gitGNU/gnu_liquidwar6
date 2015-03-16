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

#include <time.h>
#ifndef LW6_MS_WINDOWS
#include <sys/select.h>
#endif // LW6_MS_WINDOWS
#include <sys/time.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#include "sys.h"
#include "sys-internal.h"

#define _TIMER_CYCLE_MASK 0xFFFFFL
#define _RFC1123_SIZE 33
#define _CLF_SIZE 40

static int64_t timestamp_0 = 0;

/**
 * lw6sys_get_timestamp
 *
 * @sys_context: global system context
 *
 * Returns a 64-bit timestamp, for general purpose. The unit is milliseconds,
 * should return the number of milliseconds since EPOCH. Don't use this
 * for accurate date handling, but rather to technical stamp events.
 *
 * Return value: the timestamp.
 */
int64_t
lw6sys_get_timestamp (lw6sys_context_t * sys_context)
{
  int64_t ret = 0L;

  ret = lw6sys_get_uptime (sys_context) + timestamp_0;

  return ret;
}

/**
 * lw6sys_get_uptime
 *
 * @sys_context: global system context
 *
 * Returns the number of milliseconds since program was started. Milliseconds
 * are often referred to as 'ticks'.
 *
 * Return value: the number of milliseconds (64-bit)
 */
int64_t
lw6sys_get_uptime (lw6sys_context_t * sys_context)
{
  int64_t ret = 0;
  static int64_t last_ret = 0;
  /*
   * Code inspired from SDL/src/timer/unix/SDL_systimer.c.
   * On win32 SDL/src/timer/win32/SDL_systimer.c relies on GetTickCount
   * but with the help of Mingw32 we can systematically rely
   * on POSIX libs. A call to clock_gettime with CLOCK_MONOTONIC
   * should return better results, in pratice, as it is not portable,
   * it might be better to have the game to cope with semi-bogus
   * output from these functions than target perfection.
   */
  struct timeval now;

  memset (&now, 0, sizeof (struct timeval));
  gettimeofday (&now, NULL);
  ret = ((int64_t) (now.tv_sec)) * LW6SYS_TICKS_PER_SEC + ((int64_t) (now.tv_usec)) / (1000000 / LW6SYS_TICKS_PER_SEC) - timestamp_0;

  if (ret < last_ret)
    {
      /*
       * If, for some reason, time seems to go back (user changed his
       * local settings for instance) then we decide to update
       * our reference value so that, from the program's point of
       * view, time is always running the right way.
       */
      timestamp_0 -= last_ret - ret;
      ret = last_ret;
    }
  last_ret = ret;

  return ret;
}

/**
 * lw6sys_get_cycle
 *
 * @sys_context: global system context
 *
 * Returns a 32-bit timestamp, which is likely to "loop" and have twice
 * the same value during a single program execution. The idea here is
 * just to provide a 32-bit value, not too big, for animation purposes.
 * The idea is that with 64-bit values, numbers are too big and if the
 * goal is just to animate a cursor or spin a sphere, one does not care
 * if every ten hours there's a display glitch because value became
 * zero again. Besides, those values are often used for their "rest"
 * in a module operation, to translate textures for instance, and
 * having too big numbers causes floating point imprecisions. In fact
 * those values or even only 20-bit. The function is based
 * on @lw6sys_get_uptime so it will return 0 at game startup.
 *
 * Return value: the cycle value, a 20-bit integer.
 */
int32_t
lw6sys_get_cycle (lw6sys_context_t * sys_context)
{
  int64_t ret = 0L;

  ret = lw6sys_get_uptime (sys_context) & _TIMER_CYCLE_MASK;

  return ret;
}

/**
 * lw6sys_timer_update
 *
 * @sys_context: global system context
 * @timestamp: the timestamp in msec since EPOCH (output), can be NULL
 * @uptime: the uptime in msec since startup (output), can be NULL
 * @cycle: a 20-bit value for animation purpose.
 *
 * Returns timestamp & uptime with only one system call.
 *
 * Return value: none (parameters modified).
 */
void
lw6sys_timer_update (lw6sys_context_t * sys_context, int64_t * timestamp, int64_t * uptime, int32_t * cycle)
{
  int64_t tmp = 0;

  tmp = lw6sys_get_uptime (sys_context);

  if (uptime)
    {
      (*uptime) = tmp;
    }
  if (timestamp)
    {
      (*timestamp) = tmp + timestamp_0;
    }
  if (cycle)
    {
      (*cycle) = tmp & _TIMER_CYCLE_MASK;
    }
}

/**
 * lw6sys_sleep
 *
 * @sys_context: global system context
 * @seconds: the number of seconds to wait, fractions allowed
 *
 * Will sleep for the given amount of seconds. Same as @lw6sys_delay
 * only input is provided as a floating number of seconds instead
 * of ticks.
 */
void
lw6sys_sleep (lw6sys_context_t * sys_context, float seconds)
{
  lw6sys_delay (sys_context, (int) (seconds * ((float) LW6SYS_TICKS_PER_SEC)));
}

/**
 * lw6sys_delay
 *
 * @sys_context: global system context
 * @msec: the number of milliseconds (ticks) to wait
 *
 * Will sleep for the given amount of seconds. Provides accurate timing
 * and has "about-millisecond" precision, since it
 * uses @usleep or @select internally.
 * Might however be interrupted in some cases, so consider function can
 * always return quicker than specified. A common usage of this function
 * is polling loops, where you don't care if 2 polls are very close,
 * but simply want to avoid polling continuously, therefore consumming
 * 100% of the CPU for nothing.
 */
void
lw6sys_delay (lw6sys_context_t * sys_context, int msec)
{
  if (msec > 0)
    {
#ifdef LW6_MS_WINDOWS
      Sleep (msec);
#else // LW6_MS_WINDOWS
#ifdef HAVE_UNISTD_H
      usleep (msec * 1000);
#else // HAVE_UNISTD_H
      /*
       * Alternate, fallback way of sleeping. Normally, this
       * is never needed since usleep is here, but well, just
       * in case usleep is not here, this (hopefully portable)
       * code will do the job.
       */
      struct timeval tv;

      tv.tv_sec = msec / 1000;
      tv.tv_usec = (msec % 1000) * 1000;

      select (0, NULL, NULL, NULL, &tv);
#endif // HAVE_UNISTD_H
#endif // LW6_MS_WINDOWS
    }
}

/**
 * lw6sys_idle
 *
 * @sys_context: global system context
 *
 * Will sleep for a minimal amount of time, just giving the OS a chance
 * to let other threads/processes execute themselves. This can make
 * a big difference in polling loops between a process that eats 100% CPU
 * and a process that has a very moderate load.
 * of ticks.
 */
void
lw6sys_idle (lw6sys_context_t * sys_context)
{
  lw6sys_delay (sys_context, LW6SYS_SLEEP_DELAY);
}

/**
 * lw6sys_snooze
 *
 * @sys_context: global system context
 *
 * Will sleep for some time, like @lw6sys_idle, except it's a "longer"
 * time, use this when you don't really care about reactivity but are
 * more concerned about saving CPU, not running uselessly the same
 * polling code.
 */
void
lw6sys_snooze (lw6sys_context_t * sys_context)
{
  lw6sys_delay (sys_context, LW6SYS_SNOOZE_DELAY);
}

/**
 * lw6sys_time_init
 *
 * @sys_context: global system context
 *
 * Global initializations required to handle time properly.
 */
void
lw6sys_time_init (lw6sys_context_t * sys_context)
{
  struct timeval now;

  memset (&now, 0, sizeof (struct timeval));
  gettimeofday (&now, NULL);
  timestamp_0 = ((int64_t) (now.tv_sec)) * 1000L + ((int64_t) (now.tv_usec)) / 1000L;
}

/**
 * lw6sys_date_rfc1123
 *
 * @sys_context: global system context
 * @seconds_from_now: an offset to add to current time
 *
 * Gives the date according to RFC1123, this is typically
 * usefull for HTTP protocol.
 *
 * Return value: newly allocated string.
 */
char *
lw6sys_date_rfc1123 (lw6sys_context_t * sys_context, int seconds_from_now)
{
  char *ret = NULL;
  time_t now;
  time_t when;
  char *old_locale;
  char *locale;
  char *old_tz;
  struct tm tm;
  struct tm *tm_ptr;
  int strflen;

  ret = (char *) LW6SYS_CALLOC (sys_context, _RFC1123_SIZE + 1);	// 32 should even be enough

  locale = setlocale (LC_TIME, NULL);
  if (locale)
    {
      /*
       * We do need to make a copy in a separate buffer,
       * otherwise the content pointed by *locale
       * might change dynamically when calling setlocale
       */
      old_locale = lw6sys_str_copy (sys_context, locale);

      setlocale (LC_TIME, "POSIX");

      old_tz = lw6sys_getenv (sys_context, "TZ");
      lw6sys_setenv (sys_context, "TZ", "GMT");
      tzset ();

      time (&now);
      when = now + seconds_from_now;
      memset (&tm, 0, sizeof (struct tm));
#ifdef LW6_MS_WINDOWS
      /*
       * Seems mingw might not have gmtime_r but ms
       * gmtime function could be thread safe anyway
       */
      tm_ptr = gmtime (&when);
#else // LW6_MS_WINDOWS
      gmtime_r (&when, &tm);
      tm_ptr = &tm;
#endif // LW6_MS_WINDOWS
      // http://www.gta.igs.net/~hwt/rfcdate.html
      strflen = strftime (ret, _RFC1123_SIZE, "%a, %d %b %Y %H:%M:%S +0000", tm_ptr);
      if (strflen >= _RFC1123_SIZE)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("buffer exceeded %d>=%d"), strflen, _RFC1123_SIZE);
	}
      // called with old_tz=NULL, will unset
      lw6sys_setenv (sys_context, "TZ", old_tz);
      if (old_locale)
	{
	  setlocale (LC_TIME, old_locale);
	  LW6SYS_FREE (sys_context, old_locale);
	}
      if (old_tz)
	{
	  LW6SYS_FREE (sys_context, old_tz);
	}
    }

  return ret;
}

/**
 * lw6sys_date_clf
 *
 * @sys_context: global system context
 *
 * Gives the date in a format which is compatible with Apache CLF
 * Common Log Format.
 *
 * Return value: newly allocated string.
 */
char *
lw6sys_date_clf (lw6sys_context_t * sys_context)
{
  char *ret = NULL;
  time_t now;
  char *old_locale;
  char *locale;
  struct tm tm;
  struct tm *tm_ptr;
  int strflen;

  ret = (char *) LW6SYS_CALLOC (sys_context, _CLF_SIZE + 1);

  locale = setlocale (LC_TIME, NULL);
  if (locale)
    {
      /*
       * We do need to make a copy in a separate buffer,
       * otherwise the content pointed by *locale
       * might change dynamically when calling setlocale
       */
      old_locale = lw6sys_str_copy (sys_context, locale);

      setlocale (LC_TIME, "C");

      time (&now);
      memset (&tm, 0, sizeof (struct tm));
#ifdef LW6_MS_WINDOWS
      /*
       * Seems mingw might not have gmtime_r but ms
       * gmtime function could be thread safe anyway
       */
      tm_ptr = localtime (&now);
#else // LW6_MS_WINDOWS
      localtime_r (&now, &tm);
      tm_ptr = &tm;
#endif // LW6_MS_WINDOWS
      // http://www.gta.igs.net/~hwt/rfcdate.html
      strflen = strftime (ret, _CLF_SIZE, "%d/%b/%Y:%H:%M:%S %z", tm_ptr);
      if (strflen >= _CLF_SIZE)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("buffer exceeded %d>=%d"), strflen, _CLF_SIZE);
	}
      if (old_locale)
	{
	  setlocale (LC_TIME, old_locale);
	  LW6SYS_FREE (sys_context, old_locale);
	}
    }

  return ret;
}

/**
 * lw6sys_readable_uptimew
 *
 * @sys_context: global system context
 * @timestamp_delta: the duration to show, in msec
 *
 * Returns a readable form of an uptime, typically 1d 12:34:06
 * for one day, 12 hours, 34 min, 6 sec or 7:03:45 for 7 hours,
 * 3 minutes 45 sec.
 *
 * Return value: newly allocated string
 */
char *
lw6sys_readable_uptime (lw6sys_context_t * sys_context, int64_t timestamp_delta)
{
  char *ret = NULL;
  int delta;
  int days;
  int hours;
  int minutes;
  int seconds;

  delta = timestamp_delta / 1000;
  seconds = delta % 60;
  minutes = (delta / 60) % 60;
  hours = (delta / 3600) % 24;
  days = (delta / (3600 * 24));
  if (days > 0)
    {
      ret = lw6sys_new_sprintf (sys_context, "%dd %d:%02d:%02d", days, hours, minutes, seconds);
    }
  else
    {
      ret = lw6sys_new_sprintf (sys_context, "%d:%02d:%02d", hours, minutes, seconds);
    }

  return ret;
}
