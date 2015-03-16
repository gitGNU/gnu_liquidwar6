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

_lw6dat_block_t *
_lw6dat_block_new (lw6sys_context_t * sys_context, int serial_0)
{
  _lw6dat_block_t *block = NULL;

  block = (_lw6dat_block_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6dat_block_t));
  if (block)
    {
      block->serial_0 = serial_0;
      block->serial_n_1 = serial_0 + _LW6DAT_NB_ATOMS_PER_BLOCK - 1;
    }

  return block;
}

void
_lw6dat_block_free (lw6sys_context_t * sys_context, _lw6dat_block_t * block)
{
  int i;

  for (i = 0; i < _LW6DAT_NB_ATOMS_PER_BLOCK; ++i)
    {
      _lw6dat_atom_clear (sys_context, &(block->atoms[i]));
    }
  LW6SYS_FREE (sys_context, block);
}

int
_lw6dat_block_put_atom (lw6sys_context_t * sys_context, _lw6dat_block_t * block, int type,
			int serial, int order_i, int order_n, int64_t seq,
			const char *full_str, int seq_from_cmd_str_offset, int cmd_str_offset, int send_flag)
{
  int ret = 0;
  _lw6dat_atom_t *atom;
  char *old_full_str = NULL;

  if (serial >= block->serial_0 && serial <= block->serial_n_1)
    {
      atom = &(block->atoms[serial - block->serial_0]);
      old_full_str = _lw6dat_atom_get_full_str (sys_context, atom);
      if (old_full_str)
	{
	  if (strcmp (old_full_str, full_str) || order_i != atom->order_i || order_n != atom->order_n || serial != atom->serial)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("inconsistency, atom set twice atom->serial=%d atom->order_i=%d atom->order_n=%d atom->full_str=\"%s\" but serial=%d order_i=%d order_n=%d full_str=\"%s\""),
			  atom->serial, atom->order_i, atom->order_n, old_full_str, serial, order_i, order_n, full_str);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("receiving same atom serial=%d order_i=%d order_n=%d full_str=\"%s\""), serial, order_i, order_n, full_str);
	      ret = 1;
	    }
	}
      else
	{
	  ret = _lw6dat_atom_set_full_str (sys_context, atom, full_str);
	  if (ret)
	    {
	      atom->not_null = 1;
	      atom->type = type;
	      atom->serial = serial;
	      atom->order_i = order_i;
	      atom->order_n = order_n;
	      atom->seq = seq;
	      atom->seq_from_cmd_str_offset = seq_from_cmd_str_offset;
	      atom->cmd_str_offset = cmd_str_offset;
	      atom->send_flag = send_flag;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("couldn't set atom full_str"));
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("serial out of range serial=%d block->serial_0=%d block->serial_n_1=%d"), serial, block->serial_0, block->serial_n_1);
    }

  return ret;
}

_lw6dat_atom_t *
_lw6dat_block_get_atom (lw6sys_context_t * sys_context, _lw6dat_block_t * block, int serial)
{
  int atom_index;
  _lw6dat_atom_t *atom = NULL;

  atom_index = _lw6dat_block_get_atom_index (block, serial);
  if (atom_index >= 0 && atom_index < _LW6DAT_NB_ATOMS_PER_BLOCK)
    {
      if (block->atoms[atom_index].not_null)
	{
	  atom = &(block->atoms[atom_index]);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no atom defined at atom_index=%d for serial %d"), atom_index, serial);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad atom_index=%d for serial %d"), atom_index, serial);
    }

  return atom;
}
