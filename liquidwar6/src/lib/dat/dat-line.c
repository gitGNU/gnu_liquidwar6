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

#include "dat.h"

/**
 * lw6dat_line_zero
 *
 * @line: the line to fill with zero
 *
 * Fills a line object with 0 (@memset).
 *
 * Return value: none.
 */
void
lw6dat_line_zero (lw6dat_line_t * line)
{
  memset (line, 0, sizeof (lw6dat_line_t));
}

/**
 * lw6dat_line_clear
 *
 * @line: the line to clear
 *
 * Clears a line object, frees everything that needs to.
 *
 * Return value: none.
 */
void
lw6dat_line_clear (lw6dat_line_t * line)
{
  if (line->text_if_longer)
    {
      LW6SYS_FREE (line->text_if_longer);
    }
  memset (line, 0, sizeof (lw6dat_line_t));
}

/**
 * lw6dat_line_set_text
 *
 * @line: the line to update
 * @text: the text to put in the object
 *
 * Set the text of a line, will allocate memory if
 * needed and in any case copy it internally.
 *
 * Return value: none.
 */
int
lw6dat_line_set_text (lw6dat_line_t * line, char *text)
{
  int ret = 0;
  int len = 0;

  len = strlen (text);
  if (len <= LW6DAT_LINE_STATIC_SIZE)
    {
      memcpy (line->text_if_short, text, len);
      line->text_if_short[len] = '\0';
      if (line->text_if_longer)
	{
	  LW6SYS_FREE (line->text_if_longer);
	  line->text_if_longer = NULL;
	}
      ret = 1;
    }
  else
    {
      if (line->text_if_longer)
	{
	  if (strlen (line->text_if_longer) >= len)
	    {
	      memcpy (line->text_if_longer, text, len);
	      line->text_if_longer[len] = '\0';
	      ret = 1;
	    }
	  else
	    {
	      LW6SYS_FREE (line->text_if_longer);
	      line->text_if_longer = NULL;
	    }
	}
      if (!line->text_if_longer)
	{
	  line->text_if_longer = lw6sys_str_copy (text);
	}
      if (line->text_if_longer)
	{
	  ret = 1;
	}
    }

  line->not_null = ret;

  return ret;
}

/**
 * lw6dat_line_get_text
 *
 * @line: the line to query
 *
 * Get the text of a line.
 *
 * Return value: a pointer on the string (must *NOT* be freed), NULL if no text.
 */
char *
lw6dat_line_get_text (lw6dat_line_t * line)
{
  char *ret = NULL;

  if (line->not_null)
    {
      if (line->text_if_longer)
	{
	  ret = line->text_if_longer;
	}
      else
	{
	  ret = line->text_if_short;
	}
    }

  return ret;
}
