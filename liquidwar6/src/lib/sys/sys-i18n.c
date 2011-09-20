/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include <iconv.h>
#ifndef LW6_MS_WINDOWS
#include <langinfo.h>
#endif

#include "sys.h"

/**
 * lw6sys_locale_to_utf8
 *
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
lw6sys_locale_to_utf8 (char *string)
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

  /*
   * Note: we could also test here if we are already
   * in UTF-8 mode to avoid translating from UTF-8 to UTF-8,
   * not sure it would make a big difference anyway.
   */
  if (codeset)
    {
      cd = iconv_open ("UTF-8", codeset);

      if (cd != (iconv_t) - 1)
	{
	  int ilen;
	  int max_olen;
#ifdef LW6_MS_WINDOWS
	  const char *iptr;
#else
	  char *iptr;
#endif
	  size_t ileft;
	  char *optr;
	  size_t oleft;

	  ilen = strlen (string);
	  max_olen = strlen (string) * 2;

	  utf8 = LW6SYS_CALLOC (max_olen + 1);
	  if (utf8)
	    {
	      iptr = string;
	      ileft = ilen;
	      optr = utf8;
	      oleft = max_olen;

	      if (iconv (cd, &iptr, &ileft, &optr, &oleft) == (size_t) - 1)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("iconv error \"%s\""),
			      string);
		}
	    }
	  iconv_close (cd);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to open iconv"));
	}
    }

  if (utf8 == NULL)
    {
      utf8 = lw6sys_str_copy (string);
    }

  if (utf8 == NULL)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to translate string to utf8"));
    }

  return utf8;
}
