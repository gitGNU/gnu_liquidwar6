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

#include "dat-internal.h"

void
_lw6dat_atom_zero (_lw6dat_atom_t * atom)
{
  memset (atom, 0, sizeof (_lw6dat_atom_t));
}

void
_lw6dat_atom_clear (_lw6dat_atom_t * atom)
{
  if (atom->text_if_longer)
    {
      LW6SYS_FREE (atom->text_if_longer);
    }
  memset (atom, 0, sizeof (_lw6dat_atom_t));
}

int
_lw6dat_atom_set_text (_lw6dat_atom_t * atom, char *text)
{
  int ret = 0;
  int len = 0;

  len = strlen (text);
  if (len <= _LW6DAT_ATOM_STATIC_SIZE)
    {
      memcpy (atom->text_if_short, text, len);
      atom->text_if_short[len] = '\0';
      if (atom->text_if_longer)
	{
	  LW6SYS_FREE (atom->text_if_longer);
	  atom->text_if_longer = NULL;
	}
      ret = 1;
    }
  else
    {
      if (atom->text_if_longer)
	{
	  if (strlen (atom->text_if_longer) >= len)
	    {
	      memcpy (atom->text_if_longer, text, len);
	      atom->text_if_longer[len] = '\0';
	      ret = 1;
	    }
	  else
	    {
	      LW6SYS_FREE (atom->text_if_longer);
	      atom->text_if_longer = NULL;
	    }
	}
      if (!atom->text_if_longer)
	{
	  atom->text_if_longer = lw6sys_str_copy (text);
	}
      if (atom->text_if_longer)
	{
	  ret = 1;
	}
    }

  atom->not_null = ret;

  return ret;
}

char *
_lw6dat_atom_get_text (_lw6dat_atom_t * atom)
{
  char *ret = NULL;

  if (atom->not_null)
    {
      if (atom->text_if_longer)
	{
	  ret = atom->text_if_longer;
	}
      else
	{
	  ret = atom->text_if_short;
	}
    }

  return ret;
}
