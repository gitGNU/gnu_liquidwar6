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
_lw6dat_stack_zero (_lw6dat_stack_t * stack)
{
  memset (stack, 0, sizeof (_lw6dat_stack_t));
}

void
_lw6dat_stack_clear (_lw6dat_stack_t * stack)
{
  int i = 0;

  stack->node_id = 0;
  stack->serial_0 = 0;
  stack->serial_n_1 = -1;
  stack->serial_max = -1;
  for (i = 0; i < _LW6DAT_MAX_NB_BLOCKS; ++i)
    {
      if (stack->blocks[i])
	{
	  _lw6dat_block_free (stack->blocks[i]);
	  stack->blocks[i] = NULL;
	}
    }
}

void
_lw6dat_stack_purge (_lw6dat_stack_t * stack)
{
  int i = 0;

  stack->serial_0 = stack->serial_n_1 + 1;
  stack->serial_max = stack->serial_0 - 1;
  for (i = 0; i < _LW6DAT_MAX_NB_BLOCKS; ++i)
    {
      if (stack->blocks[i])
	{
	  _lw6dat_block_free (stack->blocks[i]);
	  stack->blocks[i] = NULL;
	}
    }
}

int
_lw6dat_stack_init (_lw6dat_stack_t * stack, u_int64_t node_id, int serial_0)
{
  int ret = 0;

  if (lw6sys_check_id (node_id) && serial_0 > 0)
    {
      _lw6dat_stack_clear (stack);
      stack->node_id = node_id;
      stack->serial_0 = serial_0;
      stack->serial_n_1 = serial_0 - 1;
      stack->serial_max = serial_0 - 1;
      ret = 1;
    }

  return ret;
}

int
_lw6dat_stack_get_serial (_lw6dat_stack_t * stack)
{
  int ret = 0;

  ret = stack->serial_max;

  return ret;
}

int
_lw6dat_stack_put_atom (_lw6dat_stack_t * stack,
			int serial, int order_i, int order_n, int round,
			char *text, int send_flag)
{
  int ret = 0;
  int block_index = 0;
  _lw6dat_block_t *block = NULL;
  int i, delta, delta_min, delta_max;

  block_index = _lw6dat_stack_get_block_index (stack, serial);

  delta = block_index;
  if (delta < 0)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("shifting blocks up delta=%d"),
		  delta);
      if (delta > -_LW6DAT_MAX_NB_BLOCKS)
	{
	  delta_min = delta;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("very big serial leap, history will be cleared, delta=%d"),
		      delta);
	  delta_min = -_LW6DAT_MAX_NB_BLOCKS;
	}
      for (i = _LW6DAT_MAX_NB_BLOCKS - 1;
	   i >= _LW6DAT_MAX_NB_BLOCKS + delta_min; --i)
	{
	  _lw6dat_block_free (stack->blocks[i]);
	  stack->blocks[i] = NULL;
	}
      for (i = _LW6DAT_MAX_NB_BLOCKS + delta - 1; i >= 0; --i)
	{
	  stack->blocks[i - delta] = stack->blocks[i];
	  stack->blocks[i] = NULL;
	}
      stack->serial_0 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
      stack->serial_n_1 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
    }

  delta = block_index + 1 - _LW6DAT_MAX_NB_BLOCKS;
  if (delta > 0)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("shifting blocks down delta=%d"),
		  delta);
      if (delta < _LW6DAT_MAX_NB_BLOCKS)
	{
	  delta_max = delta;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("very big serial leap, history will be cleared, delta=%d"),
		      delta);
	  delta_max = _LW6DAT_MAX_NB_BLOCKS;
	}
      for (i = 0; i < delta_max; ++i)
	{
	  _lw6dat_block_free (stack->blocks[i]);
	  stack->blocks[i] = NULL;
	}
      for (i = delta; i < _LW6DAT_MAX_NB_BLOCKS; ++i)
	{
	  stack->blocks[i - delta] = stack->blocks[i];
	  stack->blocks[i] = NULL;
	}
      stack->serial_0 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
      stack->serial_n_1 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
    }

  block_index = _lw6dat_stack_get_block_index (stack, serial);
  if (block_index >= 0 && block_index < _LW6DAT_MAX_NB_BLOCKS)
    {
      block = stack->blocks[block_index];
      if (!block)
	{
	  stack->blocks[block_index] =
	    _lw6dat_block_new (stack->serial_0 +
			       block_index * _LW6DAT_NB_ATOMS_PER_BLOCK);
	  block = stack->blocks[block_index];
	}
      if (block)
	{
	  stack->serial_n_1 =
	    lw6sys_max (stack->serial_n_1, block->serial_n_1);
	  ret =
	    _lw6dat_block_put_atom (block, serial, order_i, order_n, round,
				    text, send_flag);
	  if (!ret)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_ ("unable to put atom in stack serial=%d"),
			  serial);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("serial out of range serial=%d stack->serial_0=%d, block_index=%d"),
		  serial, stack->serial_0, block_index);
    }

  if (ret)
    {
      stack->serial_max = lw6sys_max (stack->serial_max, serial);
    }

  return ret;
}

int
_lw6dat_stack_put_atom_str (_lw6dat_stack_t * stack,
			    char *atom_str_serial_i_n_round_from_cmd,
			    int send_flag)
{
  int ret = 0;
  int serial = 0;
  int order_i = 0;
  int order_n = 0;
  int round = 0;
  u_int64_t logical_from = 0L;
  char *cmd = NULL;

  if (_lw6dat_atom_parse_serial_i_n_round_from_cmd
      (&serial, &order_i, &order_n, &round, &logical_from, &cmd,
       atom_str_serial_i_n_round_from_cmd))
    {
      ret =
	_lw6dat_stack_put_atom (stack,
				serial, order_i, order_n, round, cmd,
				send_flag);
    }

  return ret;
}

_lw6dat_atom_t *
_lw6dat_stack_get_atom (_lw6dat_stack_t * stack, int serial)
{
  int block_index;
  _lw6dat_block_t *block = NULL;
  _lw6dat_atom_t *atom = NULL;

  block_index = _lw6dat_stack_get_block_index (stack, serial);
  if (block_index >= 0 && block_index < _LW6DAT_MAX_NB_BLOCKS)
    {
      block = stack->blocks[block_index];
      if (block)
	{
	  atom = _lw6dat_block_get_atom (block, serial);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("no block defined at block_index=%d for serial %d"),
		      block_index, serial);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("bad block_index=%d for serial %d"), block_index,
		  serial);
    }

  return atom;
}

int
_lw6dat_stack_put_msg (_lw6dat_stack_t * stack, char *msg, int send_flag)
{
  int ret = 0;
  int len = 0;
  int order_n = 0;
  int order_i = 0;
  int round = 0;
  u_int64_t logical_from = 0;
  int p = 0;
  int s = 0;
  int serial = 0;
  char atom_str[_LW6DAT_ATOM_MAX_SIZE + 1];
  char *next = NULL;

  next = msg;
  if (lw6msg_word_first_int_ge0 (&round, &next, next))
    {
      if (lw6msg_word_first_id_64 (&logical_from, &next, next))
	{
	  atom_str[_LW6DAT_ATOM_MAX_SIZE] = '\0';
	  len = strlen (msg);
	  order_n = (lw6sys_max ((len - 1), 0) / _LW6DAT_ATOM_MAX_SIZE) + 1;
	  for (order_i = 0; order_i < order_n; ++order_i)
	    {
	      ret = 1;
	      p = order_i * _LW6DAT_ATOM_MAX_SIZE;
	      if (p < len)
		{
		  s = lw6sys_min (len - p, _LW6DAT_ATOM_MAX_SIZE);
		  if (s > 0)
		    {
		      memcpy (atom_str, msg + p, s);
		      atom_str[s] = '\0';
		      serial = _lw6dat_stack_get_serial (stack) + 1;
		      ret = ret
			&& _lw6dat_stack_put_atom (stack,
						   serial, order_i, order_n,
						   round, atom_str,
						   send_flag);
		    }
		}
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad logical_from in message \"%s\""), msg);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bad round in message \"%s\""),
		  msg);
    }
  return ret;
}
