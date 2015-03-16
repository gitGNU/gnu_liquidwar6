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
#endif

#include "gui.h"

/**
 * lw6gui_keypress_new
 *
 * @sys_context: global system context
 * @keysym: the keysym to use
 * @unicode: the unicode value for this keysym
 * @label: the label (optional, might be NULL)
 *
 * Creates a keypress structure, the only reason for needing a contructor
 * is that the label field needs be duplicated.
 *
 * Return value: a pointer to the newly allocated object.
 */
lw6gui_keypress_t *
lw6gui_keypress_new (lw6sys_context_t * sys_context, int keysym, int unicode, const char *label)
{
  lw6gui_keypress_t *ret = NULL;

  ret = (lw6gui_keypress_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6gui_keypress_t));
  if (ret)
    {
      ret->keysym = keysym;
      ret->unicode = unicode;
      if (label)
	{
	  ret->label = lw6sys_str_copy (sys_context, label);
	  /*
	   * we don't care if the copied label is NULL, a keypress
	   * struct with a NULL label is valid anyway
	   */
	}
    }

  return ret;
}

/**
 * lw6gui_keypress_free
 *
 * @sys_context: global system context
 * @keypress: the keypress object to free.
 *
 * Deletes a keypress structure.
 *
 * Return value: none.
 */
void
lw6gui_keypress_free (lw6sys_context_t * sys_context, lw6gui_keypress_t * keypress)
{
  if (keypress)
    {
      if (keypress->label)
	{
	  LW6SYS_FREE (sys_context, keypress->label);
	}
      LW6SYS_FREE (sys_context, keypress);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL keypress"));
    }
}

/**
 * lw6gui_keypress_repr
 *
 * @sys_context: global system context
 * @keypress: the keypress to work on
 *
 * Returns a human-readable representation of the keypress.
 *
 * Return value: a newly allocated string
 */
char *
lw6gui_keypress_repr (lw6sys_context_t * sys_context, const lw6gui_keypress_t * keypress)
{
  char *ret = NULL;

  if (keypress->label)
    {
      if (keypress->unicode >= 32 && keypress->unicode <= 127)
	{
	  ret =
	    lw6sys_new_sprintf (sys_context, _x_
				("keypress keysym=%d ascii=%d ('%c') label=\"%s\""), keypress->keysym, keypress->unicode, keypress->unicode, keypress->label);
	}
      else
	{
	  ret = lw6sys_new_sprintf (sys_context, _x_ ("keypress keysym=%d unicode=%d label=\"%s\""), keypress->keysym, keypress->unicode, keypress->label);
	}
    }
  else
    {
      ret = lw6sys_new_sprintf (sys_context, _x_ ("keypress keysym=%d unicode=%d label=NULL"), keypress->keysym, keypress->unicode);
    }

  return ret;
}
