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

/**
 * lw6sys_print_xml_header
 *
 * @sys_context: global system context
 * @f: file to output content to
 *
 * Prints a standard Liquid War compliant XML header in the given file.
 *
 * Return value: none.
 */
void
lw6sys_print_xml_header (lw6sys_context_t * sys_context, FILE * f, char *comment)
{
  char *reformatted_comment = NULL;

  fprintf (f, "<?xml version=\"1.0\"?>%s", lw6sys_eol ());
  fprintf (f, "<!DOCTYPE %s SYSTEM \"%s/dtd/%s.dtd\">%s",
	   lw6sys_build_get_package_tarname (), lw6sys_build_get_docdir (), lw6sys_build_get_package_tarname (), lw6sys_eol ());
  fprintf (f, "<%s>%s", lw6sys_build_get_package_tarname (), lw6sys_eol ());
  reformatted_comment = lw6sys_str_reformat (sys_context, comment, LW6SYS_REFORMAT_XML_PREFIX, LW6SYS_REFORMAT_XML_NB_COLUMNS);
  if (reformatted_comment)
    {
      fprintf (f, "  <!--%s", lw6sys_eol ());
      fprintf (f, "%s", reformatted_comment);
      fprintf (f, "  -->%s", lw6sys_eol ());
      LW6SYS_FREE (sys_context, reformatted_comment);
    }
}

/**
 * lw6sys_print_xml_header
 *
 * @sys_context: global system context
 * @f: file to output content to
 *
 * Prints a standard Liquid War 6 compliant XML footer in the given file.
 *
 * Return value: none.
 */
void
lw6sys_print_xml_footer (lw6sys_context_t * sys_context, FILE * f)
{
  fprintf (f, "</%s>%s", lw6sys_build_get_package_tarname (), lw6sys_eol ());
}
