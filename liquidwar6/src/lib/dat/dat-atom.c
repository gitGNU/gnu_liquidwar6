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

#include "dat-internal.h"

void
_lw6dat_atom_zero (lw6sys_context_t * sys_context, _lw6dat_atom_t * atom)
{
  memset (atom, 0, sizeof (_lw6dat_atom_t));
}

void
_lw6dat_atom_clear (lw6sys_context_t * sys_context, _lw6dat_atom_t * atom)
{
  if (atom->full_str_if_longer)
    {
      LW6SYS_FREE (sys_context, atom->full_str_if_longer);
    }
  memset (atom, 0, sizeof (_lw6dat_atom_t));
}

int
_lw6dat_atom_set_full_str (lw6sys_context_t * sys_context, _lw6dat_atom_t * atom, const char *full_str)
{
  int ret = 0;
  int len = 0;

  len = strlen (full_str);
  if (len <= _LW6DAT_ATOM_STATIC_SIZE)
    {
      memcpy (atom->full_str_if_short, full_str, len);
      atom->full_str_if_short[len] = '\0';
      if (atom->full_str_if_longer)
	{
	  LW6SYS_FREE (sys_context, atom->full_str_if_longer);
	  atom->full_str_if_longer = NULL;
	}
      ret = 1;
    }
  else
    {
      if (atom->full_str_if_longer)
	{
	  if (strlen (atom->full_str_if_longer) >= len)
	    {
	      memcpy (atom->full_str_if_longer, full_str, len);
	      atom->full_str_if_longer[len] = '\0';
	      ret = 1;
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, atom->full_str_if_longer);
	      atom->full_str_if_longer = NULL;
	    }
	}
      if (!atom->full_str_if_longer)
	{
	  atom->full_str_if_longer = lw6sys_str_copy (sys_context, full_str);
	}
      if (atom->full_str_if_longer)
	{
	  ret = 1;
	}
    }

  atom->not_null = ret;

  return ret;
}

char *
_lw6dat_atom_get_full_str (lw6sys_context_t * sys_context, _lw6dat_atom_t * atom)
{
  char *ret = NULL;

  if (atom->not_null)
    {
      if (atom->full_str_if_longer)
	{
	  ret = atom->full_str_if_longer;
	}
      else
	{
	  ret = atom->full_str_if_short;
	}
    }

  return ret;
}

int
_lw6dat_atom_parse_from_cmd (lw6sys_context_t * sys_context, int *type, int *serial, int *order_i,
			     int *order_n, int64_t * seq, u_int64_t * logical_from, int *seq_from_cmd_str_offset, int *cmd_str_offset, const char *full_str)
{
  int ret = 0;
  char *next = (char *) full_str;
  lw6msg_word_t word;

  if (lw6msg_word_first (sys_context, &word, &next, next))
    {
      ret = 1;

      if (lw6sys_str_is_same_no_case (sys_context, word.buf, LW6MSG_CMD_DATA))
	{
	  (*type) = _LW6DAT_ATOM_TYPE_DATA;
	}
      else if (lw6sys_str_is_same_no_case (sys_context, word.buf, LW6MSG_CMD_META))
	{
	  (*type) = _LW6DAT_ATOM_TYPE_META;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value for type in atom \"%s\""), full_str);
	  ret = 0;
	}
      if (ret)
	{
	  if (lw6msg_word_first_int_32_gt0 (sys_context, serial, &next, next))
	    {
	      if (lw6msg_word_first_int_32_ge0 (sys_context, order_i, &next, next))
		{
		  if (lw6msg_word_first_int_32_gt0 (sys_context, order_n, &next, next))
		    {
		      /*
		       * Now this is a "trick", we set cmd to the part that is just
		       * after order_n and before the seq, this is so that the stored
		       * string contains seq and logical_from. OK, it's redundant, but
		       * the advantage is that it allows us to cache the whole stuff
		       * and avoid rewritting it each time we need to read it.
		       */
		      (*seq_from_cmd_str_offset) = next - full_str;
		      if (lw6msg_word_first_int_64_ge0 (sys_context, seq, &next, next))
			{
			  if (lw6msg_word_first_id_64 (sys_context, logical_from, &next, next))
			    {
			      (*cmd_str_offset) = next - full_str;
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("successfully parsed \"%s\""), full_str);
			      ret = 1;
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value for logical_from in atom \"%s\""), full_str);
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value for seq in atom \"%s\""), full_str);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value for order_n in atom \"%s\""), full_str);
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value for order_i in atom \"%s\""), full_str);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value for serial in atom \"%s\""), full_str);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad value for META/DATA in atom \"%s\""), full_str);
    }

  return ret;
}
