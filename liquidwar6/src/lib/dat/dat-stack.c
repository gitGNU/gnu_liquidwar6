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
  if (stack->node_id_str)
    {
      LW6SYS_FREE (stack->node_id_str);
      stack->node_id_str = NULL;
    }
  stack->serial_0 = _LW6DAT_SERIAL_START;
  stack->serial_n_1 =
    stack->serial_0 + (_LW6DAT_MAX_NB_BLOCKS * _LW6DAT_NB_ATOMS_PER_BLOCK) -
    1;
  stack->serial_min = INT_MAX;
  stack->serial_max = _LW6DAT_SERIAL_INVALID;
  stack->serial_draft = stack->serial_max;
  stack->serial_reference = stack->serial_max;
  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      stack->serial_min_to_send[i] = stack->serial_0;
    }
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
  stack->serial_n_1 =
    stack->serial_0 + (_LW6DAT_MAX_NB_BLOCKS * _LW6DAT_NB_ATOMS_PER_BLOCK) -
    1;
  stack->serial_min = INT_MAX;
  stack->serial_max = _LW6DAT_SERIAL_INVALID;
  stack->serial_draft = stack->serial_max;
  stack->serial_reference = stack->serial_max;
  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      stack->serial_min_to_send[i] = stack->serial_0;
    }
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
  int i;

  if (lw6sys_check_id (node_id) && serial_0 >= _LW6DAT_SERIAL_START)
    {
      /*
       * Now we "align" our serial_0 value so that _LW6DAT_SERIAL_START
       * is always at index 0, this is to make sure we don't get in a
       * weird situation where we start, say at serial_0=5 so to store
       * serial_0=4 we need to have some block that goes from "-1995" to 4.
       * In theory it's not that bad but it's just as easy to align
       * everything and consider "minus something" is an error,
       * additionnally it provides an easy way to implement per block
       * checkpoints since blocks are aligned the same way on every node.
       */
      serial_0 =
	(((serial_0 -
	   _LW6DAT_SERIAL_START) / _LW6DAT_NB_ATOMS_PER_BLOCK) *
	 _LW6DAT_NB_ATOMS_PER_BLOCK) + _LW6DAT_SERIAL_START;

      _lw6dat_stack_clear (stack);
      stack->node_id = node_id;
      if (stack->node_id_str)
	{
	  LW6SYS_FREE (stack->node_id_str);
	  stack->node_id_str = NULL;
	}

      stack->node_id_str = lw6sys_id_ltoa (node_id);
      stack->serial_0 = serial_0;
      stack->serial_n_1 =
	stack->serial_0 +
	(_LW6DAT_MAX_NB_BLOCKS * _LW6DAT_NB_ATOMS_PER_BLOCK) - 1;
      //stack->serial_min=INT_MAX;
      //stack->serial_max = _LW6DAT_SERIAL_INVALID;
  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      stack->serial_min_to_send[i] = stack->serial_0;
    }
      if (stack->node_id_str != NULL)
	{
	  if (stack->serial_0 > _LW6DAT_SERIAL_INVALID
	      && stack->serial_n_1 >= _LW6DAT_SERIAL_INVALID)
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("bad range serial_0=%d and serial_n_1=%d"),
			  stack->serial_0, stack->serial_n_1);
	    }
	}
      if (!ret)
	{
	  _lw6dat_stack_clear (stack);
	}
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
			int serial, int order_i, int order_n, int seq,
			char *full_str, int seq_from_cmd_str_offset,
			int cmd_str_offset, int send_flag)
{
  int ret = 0;
  int block_index = 0;
  _lw6dat_block_t *block = NULL;
  int i, j, delta;

  block_index = _lw6dat_stack_get_block_index (stack, serial);

  /*
   * Now this block delta handling should be bulletproof tested,
   * it seems it gets wrong when delta is really too high,
   * this is (at least) related to get_block_index having limitations
   * when delta is high (or very negative, to be more precise)
   */

  delta = block_index;
  if (delta > -_LW6DAT_BLOCK_DELTA_MAX && delta < 0)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("shifting blocks up delta=%d"),
		  delta);
      if (delta < -_LW6DAT_MAX_NB_BLOCKS)
	{

	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("very big serial leap, history will be cleared, delta=%d"),
		      delta);
	}
      for (i = _LW6DAT_MAX_NB_BLOCKS - 1; i >= 0; --i)
	{
	  j = i - delta;
	  if (j < _LW6DAT_MAX_NB_BLOCKS)
	    {
	      stack->blocks[j] = stack->blocks[i];
	    }
	  else
	    {
	      if (stack->blocks[i])
		{
		  _lw6dat_block_free (stack->blocks[i]);
		}
	    }
	  stack->blocks[i] = NULL;
	}
      stack->serial_0 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
      stack->serial_n_1 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;

      stack->serial_min = serial;
      stack->serial_max = _LW6DAT_SERIAL_INVALID;
      for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
	{
	  stack->serial_min_to_send[i] = serial;
	}
      for (i = _LW6DAT_MAX_NB_BLOCKS - 1;
	   i >= 0 && stack->serial_max == _LW6DAT_SERIAL_INVALID; --i)
	{
	  block = stack->blocks[i];
	  if (block)
	    {
	      for (j = _LW6DAT_NB_ATOMS_PER_BLOCK - 1;
		   j >= 0 && stack->serial_max == _LW6DAT_SERIAL_INVALID; --j)
		{
		  if (block->atoms[j].not_null)
		    {
		      stack->serial_max = block->atoms[j].serial;
		    }
		}
	    }
	}
    }

  delta = block_index + 1 - _LW6DAT_MAX_NB_BLOCKS;
  if (delta < _LW6DAT_BLOCK_DELTA_MAX && delta > 0)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("shifting blocks down delta=%d"),
		  delta);
      if (delta > _LW6DAT_MAX_NB_BLOCKS)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("very big serial leap, history will be cleared, delta=%d"),
		      delta);
	}
      for (i = 0; i < _LW6DAT_MAX_NB_BLOCKS; ++i)
	{
	  j = i - delta;
	  if (j >= 0)
	    {
	      stack->blocks[j] = stack->blocks[i];
	    }
	  else
	    {
	      if (stack->blocks[i])
		{
		  _lw6dat_block_free (stack->blocks[i]);
		}
	    }
	  stack->blocks[i] = NULL;
	}
      stack->serial_0 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
      stack->serial_n_1 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;

      stack->serial_max = serial;
      stack->serial_min = INT_MAX;
      for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
	{
	  stack->serial_min_to_send[i] = lw6sys_max(stack->serial_min_to_send[i] ,stack->serial_0);
	}
      for (i = 0; i < _LW6DAT_MAX_NB_BLOCKS && stack->serial_min == INT_MAX;
	   ++i)
	{
	  block = stack->blocks[i];
	  if (block)
	    {
	      for (j = 0;
		   j < _LW6DAT_NB_ATOMS_PER_BLOCK
		   && stack->serial_min == INT_MAX; ++j)
		{
		  if (block->atoms[j].not_null)
		    {
		      stack->serial_min = block->atoms[j].serial;
		    }
		}
	    }
	}
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
	  /*
	     stack->serial_n_1 =
	     lw6sys_max (stack->serial_n_1, block->serial_n_1);
	     stack->serial_0 =
	     lw6sys_min (stack->serial_0, block->serial_0);
	   */
	  ret =
	    _lw6dat_block_put_atom (block, serial, order_i, order_n, seq,
				    full_str, seq_from_cmd_str_offset,
				    cmd_str_offset, send_flag);
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
      if (serial > _LW6DAT_SERIAL_INVALID)
	{
	  stack->serial_min = lw6sys_min (stack->serial_min, serial);
	  stack->serial_max = lw6sys_max (stack->serial_max, serial);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("bad serial %d in atom \"%d\""), serial);
	}
    }

  if (stack->serial_0 <= _LW6DAT_SERIAL_INVALID
      || stack->serial_n_1 < _LW6DAT_SERIAL_INVALID)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("bad range serial_0=%d and serial_n_1=%d (serial=%d)"),
		  stack->serial_0, stack->serial_n_1, serial);
    }

  return ret;
}

int
_lw6dat_stack_put_atom_str (_lw6dat_stack_t * stack,
			    char *full_str, int send_flag)
{
  int ret = 0;
  int serial = 0;
  int order_i = 0;
  int order_n = 0;
  int seq = 0;
  u_int64_t logical_from = 0L;
  int seq_from_cmd_str_offset = 0;
  int cmd_str_offset = 0;

  if (_lw6dat_atom_parse_serial_i_n_seq_from_cmd
      (&serial, &order_i, &order_n, &seq, &logical_from,
       &seq_from_cmd_str_offset, &cmd_str_offset, full_str))
    {
      ret =
	_lw6dat_stack_put_atom (stack,
				serial, order_i, order_n, seq, full_str,
				seq_from_cmd_str_offset, cmd_str_offset,
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
      lw6sys_log (LW6SYS_LOG_DEBUG,
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
  int seq = 0;
  u_int64_t logical_from = 0;
  int p = 0;
  int s = 0;
  int serial = 0;
  char full_str[_LW6DAT_HEADER_MAX_SIZE + _LW6DAT_ATOM_MAX_SIZE + 1];
  int seq_from_cmd_str_offset = 0;
  int cmd_str_offset = 0;
  char *next = NULL;

  next = msg;
  if (lw6msg_word_first_int_ge0 (&seq, &next, next))
    {
      if (lw6msg_word_first_id_64 (&logical_from, &next, next))
	{
	  full_str[_LW6DAT_HEADER_MAX_SIZE + _LW6DAT_ATOM_MAX_SIZE] = '\0';
	  len = strlen (next);
	  order_n = (lw6sys_max ((len - 1), 0) / _LW6DAT_ATOM_MAX_SIZE) + 1;
	  for (order_i = 0; order_i < order_n; ++order_i)
	    {
	      ret = 1;
	      p = order_i * _LW6DAT_ATOM_MAX_SIZE;
	      if (p < len)
		{
		  s = lw6sys_min (len - p, _LW6DAT_ATOM_MAX_SIZE);
		  if (s >= 0)
		    {
		      serial = _lw6dat_stack_get_serial (stack) + 1;
		      snprintf (full_str, _LW6DAT_HEADER_MAX_SIZE,
				"%d %d %d ", serial, order_i, order_n);
		      seq_from_cmd_str_offset = strlen (full_str);
		      if (seq_from_cmd_str_offset < _LW6DAT_HEADER_MAX_SIZE)
			{
			  /*
			   * stupid cast as long long unsigned int as GCC can't figure
			   * it's the same as u_int64_t
			   */
			  snprintf (full_str + seq_from_cmd_str_offset,
				    _LW6DAT_HEADER_MAX_SIZE -
				    seq_from_cmd_str_offset,
				    "%d %" LW6SYS_PRINTF_LL "x ", seq,
				    (long long unsigned int) logical_from);
			  cmd_str_offset = strlen (full_str);
			  if (cmd_str_offset < _LW6DAT_HEADER_MAX_SIZE)
			    {
			      if (s > 0)
				{
				  memcpy (full_str + cmd_str_offset, next + p,
					  s);
				}
			      else
				{
				  lw6sys_log (LW6SYS_LOG_WARNING,
					      _x_ ("0 sized atom"));
				}
			      full_str[cmd_str_offset + s] = '\0';
			      ret = ret
				&& _lw6dat_stack_put_atom (stack,
							   serial, order_i,
							   order_n, seq,
							   full_str,
							   seq_from_cmd_str_offset,
							   cmd_str_offset,
							   send_flag);
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_WARNING,
					  _x_
					  ("header is too long (cmd_str_offest=%d)"),
					  cmd_str_offset);
			      ret = 0;
			    }
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("header is too long (seq_from_cmd_str_offset=%d)"),
				      seq_from_cmd_str_offset);
			  ret = 0;
			}
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
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("bad seq in message \"%s\""), msg);
    }
  return ret;
}

int
_lw6dat_stack_calc_serial_draft_and_reference (_lw6dat_stack_t * stack)
{
  int ret = 0;
  int seq = 0;
  int serial = 0;
  _lw6dat_atom_t *atom = NULL;
  int atom_complete = 1;
  int atoms_no_hole = 1;
  int order_i = 0;
  int order_n = 0;

  serial = lw6sys_max (stack->serial_min, stack->serial_reference + 1);
  seq = _lw6dat_stack_get_seq_reference (stack);
  while (serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (stack, serial);
      if (atom)
	{
	  if (atom->order_i == 0)
	    {
	      atom_complete = 1;
	      seq = atom->seq;
	      order_n = atom->order_n;
	      for (order_i = 1;
		   order_i < order_n && serial + order_i <= stack->serial_max;
		   ++order_i)
		{
		  atom = _lw6dat_stack_get_atom (stack, serial + order_i);
		  if (atom)
		    {
		      if (atom->seq != seq)
			{
			  atom_complete = 0;
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("bad seq %d should be %d for atom \"%s\""),
				      atom->seq, seq,
				      _lw6dat_atom_get_full_str (atom));
			}
		      if (atom->order_i != order_i)
			{
			  atom_complete = 0;
			  lw6sys_log (LW6SYS_LOG_WARNING,
				      _x_
				      ("bad order_i %d should be %d for atom \"%s\""),
				      atom->order_i, order_i,
				      _lw6dat_atom_get_full_str (atom));
			}
		    }
		  else
		    {
		      atom_complete = 0;
		    }
		}
	      if (order_i == order_n && atom_complete)
		{
		  stack->serial_draft = serial;
		  if (atoms_no_hole)
		    {
		      stack->serial_reference = serial;
		    }
		}
	      else
		{
		  atoms_no_hole = 0;
		}
	      serial += order_i;
	    }
	  else
	    {
	      atoms_no_hole = 0;
	      serial++;
	    }
	}
      else
	{
	  atoms_no_hole = 0;
	  serial++;
	}
    }

  return ret;
}

int
_lw6dat_stack_get_seq_min (_lw6dat_stack_t * stack)
{
  int ret = _LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_min >= stack->serial_0
      && stack->serial_min <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (stack, stack->serial_min);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

int
_lw6dat_stack_get_seq_max (_lw6dat_stack_t * stack)
{
  int ret = _LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_max >= stack->serial_0
      && stack->serial_max <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (stack, stack->serial_max);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

int
_lw6dat_stack_get_seq_draft (_lw6dat_stack_t * stack)
{
  int ret = _LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_draft >= stack->serial_0
      && stack->serial_draft <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (stack, stack->serial_draft);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

int
_lw6dat_stack_get_seq_reference (_lw6dat_stack_t * stack)
{
  int ret = _LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_reference >= stack->serial_0
      && stack->serial_reference <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (stack, stack->serial_reference);
      if (atom)
	{
	  /*
	   * Now here the "-1" is very (very!) important,
	   * it means that if we are sure we've received complete
	   * messages until, say round 1200, then we're 100% sure we
	   * have the complete stuff for rounf 1199, while there's
	   * still a chance we receive messages for round 1200, there's
	   * no way we can still have informations about round 1199
	   * again.
	   */
	  ret = atom->seq - 1;
	}
    }

  return ret;
}

int
_lw6dat_stack_seq2serial (_lw6dat_stack_t * stack, int seq)
{
  int ret = _LW6DAT_SERIAL_INVALID;
  _lw6dat_atom_t *atom = NULL;
  /*
   * Carefull, those values won't hold the real
   * min and max values for seq, as seq ain't
   * garanteed to be ordered. Still, it should
   * give a good idea of the result in most cases,
   * the most common case in which seq isn't
   * ever increasing is when a map is resetted
   * and round counting is back to "0".
   */
  int seq_min = _LW6DAT_SEQ_INVALID;
  int seq_max = _LW6DAT_SEQ_INVALID;
  int i;

  if (stack->serial_max >= stack->serial_0
      && stack->serial_max <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (stack, stack->serial_min);
      if (atom)
	{
	  seq_min = atom->seq;
	}
      atom = _lw6dat_stack_get_atom (stack, stack->serial_max);
      if (atom)
	{
	  seq_max = atom->seq;
	}
      if (seq_min != _LW6DAT_SEQ_INVALID && seq_max != _LW6DAT_SEQ_INVALID)
	{
	  if (seq_min < seq_max && seq_min <= seq && seq <= seq_max &&
	      seq - seq_min < seq_max - seq)
	    {
	      /*
	       * Loop with i increasing since we suspect we'd
	       * better start from the bottom of the lif
	       */
	      for (i = stack->serial_min;
		   i <= stack->serial_max
		   && ret == _LW6DAT_SERIAL_INVALID; ++i)
		{
		  atom = _lw6dat_stack_get_atom (stack, i);
		  if (atom)
		    {
		      if (atom->seq == seq)
			{
			  ret = i;
			}
		    }
		}
	    }
	  else
	    {
	      /*
	       * By default, walk from top, most
	       * interesting messages are here, they
	       * are the latest sent.
	       */
	      for (i = stack->serial_max;
		   i >= stack->serial_min
		   && ret == _LW6DAT_SERIAL_INVALID; --i)
		{
		  atom = _lw6dat_stack_get_atom (stack, i);
		  if (atom)
		    {
		      if (atom->seq == seq)
			{
			  ret = i;
			}
		    }
		}
	    }
	  if (seq_min == seq_max && seq_min == seq)
	    {
	      ret = stack->serial_min;
	    }
	  if (ret != _LW6DAT_SERIAL_INVALID)
	    {
	      /*
	       * Now this might be a little risky, be carefull
	       * when hacking algorithm above, that atom
	       * does indeed contain the correct element
	       */
	      if (atom)
		{
		  if (atom->serial == ret)
		    {
		      /*
		       * We return the *first* element
		       */
		      ret -= atom->order_i;
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_
				  ("inconsistency, serial is %d and should be %d"),
				  ret, atom->serial);
		    }
		}
	    }
	}
    }

  return ret;
}

int
_lw6dat_stack_serial2seq (_lw6dat_stack_t * stack, int serial)
{
  int ret = _LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (serial >= stack->serial_min && serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (stack, serial);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

lw6sys_list_t *
_lw6dat_stack_init_list ()
{
  lw6sys_list_t *ret = NULL;

  ret = lw6sys_list_new (lw6sys_free_callback);

  return ret;
}

static int
_update_msg_list_by_seq_with_search (_lw6dat_stack_t * stack,
				     lw6sys_list_t ** msg_list, int seq,
				     int serial, int search_inc,
				     int search_dec)
{
  int ret = 0;
  _lw6dat_atom_t *atom = NULL;
  char *full_str = NULL;
  char *msg = NULL;
  char *cmd = NULL;
  int seq_from_len = 0;
  int i, n;
  int no_hole = 1;

  if (serial >= stack->serial_min && serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (stack, serial);
      if (atom && atom->seq == seq)
	{
	  serial -= atom->order_i;
	  if (serial >= stack->serial_min && serial <= stack->serial_max)
	    {
	      n = atom->order_n;
	      msg =
		(char *)
		LW6SYS_CALLOC (((_LW6DAT_HEADER_MAX_SIZE +
				 (_LW6DAT_ATOM_MAX_SIZE * n)) +
				1) * sizeof (char));
	      if (msg)
		{
		  full_str = _lw6dat_atom_get_full_str (atom);
		  seq_from_len =
		    atom->cmd_str_offset - atom->seq_from_cmd_str_offset;
		  if (seq_from_len > 0
		      && seq_from_len < _LW6DAT_HEADER_MAX_SIZE)
		    {
		      memcpy (msg, full_str + atom->seq_from_cmd_str_offset,
			      seq_from_len);
		      cmd = msg + seq_from_len;
		      for (i = 0; i < n && no_hole; ++i)
			{
			  atom = _lw6dat_stack_get_atom (stack, serial + i);
			  if (atom)
			    {
			      full_str = _lw6dat_atom_get_full_str (atom);
			      if (full_str)
				{
				  memcpy (cmd + (i * _LW6DAT_ATOM_MAX_SIZE),
					  full_str + atom->cmd_str_offset,
					  strlen (full_str +
						  atom->cmd_str_offset));
				}
			    }
			  else
			    {
			      no_hole = 0;
			    }
			}
		      if (no_hole)
			{
			  if (search_dec)
			    {
			      _update_msg_list_by_seq_with_search (stack,
								   msg_list,
								   seq,
								   serial - 1,
								   0, 1);
			    }
			  lw6sys_list_push_front (msg_list, msg);
			  if (search_inc)
			    {
			      _update_msg_list_by_seq_with_search (stack,
								   msg_list,
								   seq,
								   serial + n,
								   1, 0);
			    }
			  ret = 1;
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_ ("bad seq_from_len=%d"), seq_from_len);
		    }
		}
	    }
	}
    }

  return ret;
}

int
_lw6dat_stack_update_msg_list_by_seq (_lw6dat_stack_t * stack,
				      lw6sys_list_t ** msg_list, int seq)
{
  int ret = 0;
  int serial;

  serial = _lw6dat_stack_seq2serial (stack, seq);
  ret =
    _update_msg_list_by_seq_with_search (stack, msg_list, seq, serial, 1, 1);

  return ret;
}

int
_lw6dat_stack_update_atom_str_list_by_serial (_lw6dat_stack_t * stack,
					      lw6sys_list_t ** msg_list,
					      int serial)
{
  int ret = 0;
  _lw6dat_atom_t *atom = NULL;
  char *atom_str = NULL;

  if (serial >= stack->serial_min && serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (stack, serial);
      if (atom)
	{
	  atom_str = _lw6dat_atom_get_full_str (atom);
	  if (atom_str)
	    {
	      lw6sys_list_push_front (msg_list,
				      lw6sys_str_copy (atom_str +
						       atom->seq_from_cmd_str_offset));
	      ret = 1;
	    }
	}
    }

  return ret;
}

int
_lw6dat_stack_update_atom_str_list_not_sent (_lw6dat_stack_t * stack,
					     lw6sys_list_t ** msg_list,
					     int target_index)
{
  int ret = 0;
  _lw6dat_atom_t *atom = NULL;
  char *atom_str = NULL;
  int serial = 0;
  int serial_min = 0;
  int serial_max = 0;
  int send_mask = 0;
  int no_hole = 1;

  send_mask = _lw6dat_flag (target_index);
  /*
   * We keep a track of per-node "minimum atom to send",
   * this is just so that we avoid walking over the whole
   * stuff each time
   */
  serial_min =
    lw6sys_max (stack->serial_min, stack->serial_min_to_send[target_index]);
  serial_max = stack->serial_max;
  for (serial = serial_min; serial <= serial_max; ++serial)
    {
      atom = _lw6dat_stack_get_atom (stack, serial);
      if (atom)
	{
	  if ((atom->send_flag & send_mask)
	      && !(atom->sent_status & send_mask))
	    {
	      atom_str = _lw6dat_atom_get_full_str (atom);
	      if (atom_str)
		{
		  if (no_hole)
		    {
		      stack->serial_min_to_send[target_index] = serial;
		    }
		  atom->sent_status |= send_mask;
		  lw6sys_list_push_front (msg_list,
					  lw6sys_str_copy (atom_str));
		  ret = 1;
		}
	    }
	}
      else
	{
	  no_hole = 0;
	}
    }

  return ret;
}
