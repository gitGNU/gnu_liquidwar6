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

#include <iconv.h>
#ifndef LW6_MS_WINDOWS
#include <langinfo.h>
#endif

#include "sys.h"

#define _UTF8 "UTF-8"

/**
 * lw6sys_locale_to_utf8
 *
 * @sys_context: global system context
 * @string: the string to convert
 *
 * Used to force strings into UTF-8 mode, this is basically
 * to match the TTF font settings used when displaying stuff
 * on OpenGL. Indeed, in this case, the standard _ gettext
 * function won't work, we need to force UTF-8 mode. If the
 * locale is UTF-8, then function does nothing, but at least
 * it's transparent usage won't hurt.
 *
 * Returned value: a newly allocated string, always in UTF-8
 *   no matter what the locale is.
 */
char *
lw6sys_locale_to_utf8 (lw6sys_context_t * sys_context, const char *string)
{
  char *utf8 = NULL;

  char *codeset = NULL;
  iconv_t cd;

#ifndef LW6_MS_WINDOWS
  /*
   * No support for ns_langinfo on mingw32. However it seems assuming
   * we're in ISO-8859-1" (latin1) works. At least it works for French
   * translations on my computer. There is certainly a way to get this
   * using GetLocaleInfo. Not implemented yet.
   */
  codeset = nl_langinfo (CODESET);
#endif
  if (!codeset)
    {
      codeset = "ISO-8859-1";
    }

  if (codeset)
    {
      if (lw6sys_str_is_same (sys_context, codeset, _UTF8))
	{
	  /*
	   * Note: we are in UTF-8 already, no need to change again...
	   */
	  utf8 = lw6sys_str_copy (sys_context, string);
	}
      else
	{
	  int ilen;
	  int max_olen;
#ifdef LW6_MS_WINDOWS
	  const char *iptr = NULL;
#else // LW6_MS_WINDOWS
	  char *iptr = NULL;
#endif // LW6_MS_WINDOWS
	  size_t ileft;
	  char *optr = NULL;
	  size_t oleft;

	  cd = iconv_open ("UTF-8", codeset);

	  if (cd != (iconv_t) - 1)
	    {
	      ilen = strlen (string);
	      max_olen = strlen (string) * 2;

	      utf8 = LW6SYS_CALLOC (sys_context, max_olen + 1);
	      if (utf8)
		{
		  /*
		   * OK, we do a cast from const to non-const,
		   * actually iconv does modify iptr when
		   * passed &iptr but it does not touch *iptr.
		   */
#ifdef LW6_MS_WINDOWS
		  iptr = (const char *) string;
#else // LW6_MS_WINDOWS
		  iptr = (char *) string;
#endif // LW6_MS_WINDOWS
		  ileft = ilen;
		  optr = utf8;
		  oleft = max_olen;

		  if (iconv (cd, &iptr, &ileft, &optr, &oleft) == (size_t) - 1)
		    {
		      /*
		       * The message below is not translated, not
		       * even _x_-ed because well, we certainly
		       * do not want this function to be called
		       * recursively in case of problem. If transation
		       * is not working, it makes to translate
		       * anyway...
		       */
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, "iconv error \"%s\"", string);
		      LW6SYS_FREE (sys_context, utf8);
		      utf8 = NULL;	// important to be sure it's handled later
		    }
		}
	      iconv_close (cd);
	    }
	  else
	    {
	      /*
	       * Message not translated, iconv doesn't work anyway
	       * if we're here
	       */
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, "unable to open iconv");
	    }
	}
    }

  if (utf8 == NULL)
    {
      utf8 = lw6sys_str_copy (sys_context, string);
    }

  if (utf8 == NULL)
    {
      /*
       * Message not translated, iconv doesn't work anyway
       * if we're here
       */
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, "unable to translate string to utf8");
    }

  return utf8;
}
