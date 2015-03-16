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

/*
 * Above this limit, there will be a "NOTICE" message
 * written that says there's a big message being handled.
 * This usually concerns dumps.
 */
#define _LONG_MSG_NOTICE 100000

/*
 * If order is below this, don't report in progress, just
 * ignore.
 */
#define _ORDER_N_PROGRESS_REPORT_MIN 10

void
_lw6dat_stack_zero (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  memset (stack, 0, sizeof (_lw6dat_stack_t));
}

static void
_prepare (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  int i = 0;

  stack->serial_n_1 = stack->serial_0_base + (_LW6DAT_MAX_NB_BLOCKS * _LW6DAT_NB_ATOMS_PER_BLOCK) - 1;
  stack->serial_min = INT_MAX;
  stack->serial_max = LW6DAT_SERIAL_INVALID;
  stack->serial_draft = stack->serial_max;
  stack->serial_reference = stack->serial_max;
  stack->serial_miss_min = stack->serial_n_1;
  stack->serial_miss_max = stack->serial_0_real;
  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
    {
      stack->serial_min_to_send[i] = stack->serial_0_real;
      stack->seq_0s[i] = LW6DAT_SEQ_INVALID;
    }
  stack->seq_0 = LW6DAT_SEQ_INVALID;
  for (i = 0; i < _LW6DAT_MAX_NB_BLOCKS; ++i)
    {
      if (stack->blocks[i])
	{
	  _lw6dat_block_free (sys_context, stack->blocks[i]);
	  stack->blocks[i] = NULL;
	}
    }
}

void
_lw6dat_stack_clear (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  stack->node_id = 0;
  if (stack->node_id_str)
    {
      LW6SYS_FREE (sys_context, stack->node_id_str);
      stack->node_id_str = NULL;
    }
  stack->serial_0_base = LW6DAT_SERIAL_START;
  stack->serial_0_real = LW6DAT_SERIAL_START;

  _prepare (sys_context, stack);
}

void
_lw6dat_stack_purge (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  stack->serial_0_base = stack->serial_n_1 + 1;
  stack->serial_0_real = stack->serial_n_1 + 1;

  _prepare (sys_context, stack);
}

int
_lw6dat_stack_init (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, u_int64_t node_id, int serial_0, int64_t seq_0)
{
  int ret = 0;
  int stack_index = 0;
  int serial_0_base = serial_0;
  int serial_0_real = serial_0;

  if (lw6sys_check_id (sys_context, node_id) && serial_0 >= LW6DAT_SERIAL_START)
    {
      /*
       * Now we "align" our serial_0 value so that LW6DAT_SERIAL_START
       * is always at index 0, this is to make sure we don't get in a
       * weird situation where we start, say at serial_0=5 so to store
       * serial_0=4 we need to have some block that goes from "-1995" to 4.
       * In theory it's not that bad but it's just as easy to align
       * everything and consider "minus something" is an error,
       * additionnally it provides an easy way to implement per block
       * checkpoints since blocks are aligned the same way on every node.
       */
      serial_0_base = (((serial_0_real - LW6DAT_SERIAL_START) / _LW6DAT_NB_ATOMS_PER_BLOCK) * _LW6DAT_NB_ATOMS_PER_BLOCK) + LW6DAT_SERIAL_START;

      _lw6dat_stack_clear (sys_context, stack);
      stack->node_id = node_id;
      stack->node_id_str = lw6sys_id_ltoa (sys_context, node_id);
      stack->serial_0_base = serial_0_base;
      stack->serial_0_real = serial_0_real;

      _prepare (sys_context, stack);

      /*
       * Fill up all other stacks information with our seq_0 number,
       * in practice this is not much of a problem, indeed we won't
       * send to another host/stack informations we do not even
       * care about...
       */
      for (stack_index = 0; stack_index < LW6DAT_MAX_NB_STACKS && ret < 0; ++stack_index)
	{
	  stack->seq_0s[stack_index] = seq_0;
	}
      stack->seq_0 = seq_0;
      if (stack->node_id_str != NULL)
	{
	  if (stack->serial_0_base > LW6DAT_SERIAL_INVALID && stack->serial_0_real > LW6DAT_SERIAL_INVALID && stack->serial_n_1 >= LW6DAT_SERIAL_INVALID)
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("bad range serial_0_base=%d serial_0_real=%d and serial_n_1=%d"), stack->serial_0_base, stack->serial_0_real, stack->serial_n_1);
	    }
	}
      if (!ret)
	{
	  _lw6dat_stack_clear (sys_context, stack);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad stack_init params node_id=%" LW6SYS_PRINTF_LL "x, serial_0=%d"), (long long) node_id, serial_0);
    }

  return ret;
}

int
_lw6dat_stack_get_serial (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  int ret = 0;

  ret = stack->serial_max;

  return ret;
}

int
_lw6dat_stack_put_atom (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int type,
			int serial, int order_i, int order_n, int64_t seq,
			const char *full_str, int seq_from_cmd_str_offset, int cmd_str_offset, int send_flag)
{
  int ret = 0;
  int block_index = 0;
  _lw6dat_block_t *block = NULL;
  int i, j, delta;

  if (seq >= stack->seq_0s[_LW6DAT_LOCAL_NODE_INDEX])
    {
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
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("shifting blocks up delta=%d"), delta);
	  if (delta < -_LW6DAT_MAX_NB_BLOCKS)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("very big serial leap, history will be cleared, delta=%d"), delta);
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
		      _lw6dat_block_free (sys_context, stack->blocks[i]);
		    }
		}
	      stack->blocks[i] = NULL;
	    }
	  stack->serial_0_base += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
	  stack->serial_0_real += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
	  stack->serial_n_1 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;

	  stack->serial_min = serial;
	  stack->serial_max = LW6DAT_SERIAL_INVALID;
	  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
	    {
	      stack->serial_min_to_send[i] = serial;
	    }
	  for (i = _LW6DAT_MAX_NB_BLOCKS - 1; i >= 0 && stack->serial_max == LW6DAT_SERIAL_INVALID; --i)
	    {
	      block = stack->blocks[i];
	      if (block)
		{
		  for (j = _LW6DAT_NB_ATOMS_PER_BLOCK - 1; j >= 0 && stack->serial_max == LW6DAT_SERIAL_INVALID; --j)
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
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("shifting blocks down delta=%d"), delta);
	  if (delta > _LW6DAT_MAX_NB_BLOCKS)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("very big serial leap, history will be cleared, delta=%d"), delta);
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
		      _lw6dat_block_free (sys_context, stack->blocks[i]);
		    }
		}
	      stack->blocks[i] = NULL;
	    }
	  stack->serial_0_base += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
	  stack->serial_0_real += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;
	  stack->serial_n_1 += delta * _LW6DAT_NB_ATOMS_PER_BLOCK;

	  stack->serial_max = serial;
	  stack->serial_min = INT_MAX;
	  for (i = 0; i < LW6DAT_MAX_NB_STACKS; ++i)
	    {
	      stack->serial_min_to_send[i] = lw6sys_imax (stack->serial_min_to_send[i], stack->serial_0_real);
	    }
	  for (i = 0; i < _LW6DAT_MAX_NB_BLOCKS && stack->serial_min == INT_MAX; ++i)
	    {
	      block = stack->blocks[i];
	      if (block)
		{
		  for (j = 0; j < _LW6DAT_NB_ATOMS_PER_BLOCK && stack->serial_min == INT_MAX; ++j)
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
	      stack->blocks[block_index] = _lw6dat_block_new (sys_context, stack->serial_0_base + block_index * _LW6DAT_NB_ATOMS_PER_BLOCK);
	      block = stack->blocks[block_index];
	    }
	  if (block)
	    {
	      ret =
		_lw6dat_block_put_atom (sys_context, block, type, serial, order_i, order_n, seq, full_str, seq_from_cmd_str_offset, cmd_str_offset, send_flag);
	      if (!ret)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to put atom in stack serial=%d"), serial);
		}
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("serial out of range serial=%d stack->serial_0_base=%d, stack->serial_0_real=%d, block_index=%d"),
		      serial, stack->serial_0_base, stack->serial_0_real, block_index);
	}

      if (ret)
	{
	  if (serial > LW6DAT_SERIAL_INVALID)
	    {
	      stack->serial_min = lw6sys_imin (stack->serial_min, serial);
	      stack->serial_max = lw6sys_imax (stack->serial_max, serial);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad serial %d in atom \"%s\""), serial, full_str);
	    }
	}

      if (stack->serial_0_base <= LW6DAT_SERIAL_INVALID || stack->serial_0_real <= LW6DAT_SERIAL_INVALID || stack->serial_n_1 < LW6DAT_SERIAL_INVALID)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("bad range serial_0_base=%d serial_0_real=%d and serial_n_1=%d (serial=%d)"),
		      stack->serial_0_base, stack->serial_0_real, stack->serial_n_1, serial);
	}

      /*
       * Record that we got a message, this can be used later, for instance
       * to be aware that we're receiving plenty of new messages and it's of
       * no use to send MISS messages...
       */
      if (ret)
	{
	  if (order_i > 0)
	    {
	      ++(stack->nb_atom_parts_since_last_poll);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("atom is too old seq=%" LW6SYS_PRINTF_LL "d seq_0=%"
		       LW6SYS_PRINTF_LL "d"), (long long) seq, (long long) stack->seq_0s[_LW6DAT_LOCAL_NODE_INDEX]);
    }

  return ret;
}

int
_lw6dat_stack_put_atom_str (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, const char *full_str, int send_flag)
{
  int ret = 0;
  int type = 0;
  int serial = 0;
  int order_i = 0;
  int order_n = 0;
  int64_t seq = 0LL;
  u_int64_t logical_from = 0L;
  int seq_from_cmd_str_offset = 0;
  int cmd_str_offset = 0;

  if (_lw6dat_atom_parse_from_cmd (sys_context, &type, &serial, &order_i, &order_n, &seq, &logical_from, &seq_from_cmd_str_offset, &cmd_str_offset, full_str))
    {
      ret = _lw6dat_stack_put_atom (sys_context, stack, type, serial, order_i, order_n, seq, full_str, seq_from_cmd_str_offset, cmd_str_offset, send_flag);
    }

  return ret;
}

_lw6dat_atom_t *
_lw6dat_stack_get_atom (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int serial)
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
	  atom = _lw6dat_block_get_atom (sys_context, block, serial);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no block defined at block_index=%d for serial %d"), block_index, serial);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("bad block_index=%d for serial %d"), block_index, serial);
    }

  return atom;
}

int
_lw6dat_stack_put_msg (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int64_t * local_seq_last, const char *msg, int send_flag)
{
  int ret = 0;
  int len = 0;
  int order_n = 0;
  int order_i = 0;
  int64_t seq = LW6DAT_SEQ_INVALID;
  u_int64_t logical_from = 0;
  int p = 0;
  int s = 0;
  int serial = 0;
  char full_str[_LW6DAT_HEADER_MAX_SIZE + _LW6DAT_ATOM_MAX_SIZE + 1];
  int seq_from_cmd_str_offset = 0;
  int cmd_str_offset = 0;
  char *next = NULL;
  char *z = NULL;

  next = (char *) msg;
  if (lw6msg_word_first_int_64_ge0 (sys_context, &seq, &next, next))
    {
      if (lw6msg_word_first_id_64 (sys_context, &logical_from, &next, next))
	{
	  full_str[_LW6DAT_HEADER_MAX_SIZE + _LW6DAT_ATOM_MAX_SIZE] = '\0';
	  z = lw6msg_z_encode (sys_context, next, _LW6DAT_ATOM_MAX_SIZE);
	  if (z)
	    {
	      len = strlen (z);
	      order_n = (lw6sys_imax ((len - 1), 0) / _LW6DAT_ATOM_MAX_SIZE) + 1;
	      if (order_n > 1)
		{
		  if (len >= _LONG_MSG_NOTICE)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("put long message in stack, %d bytes in %d atoms"), len, order_n);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("put message in stack, %d bytes in %d atoms"), len, order_n);
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("put message in stack, %d bytes in 1 atom"), len);
		}
	      for (order_i = 0; order_i < order_n; ++order_i)
		{
		  ret = 1;
		  p = order_i * _LW6DAT_ATOM_MAX_SIZE;
		  if (p < len)
		    {
		      s = lw6sys_imin (len - p, _LW6DAT_ATOM_MAX_SIZE);
		      if (s >= 0)
			{
			  serial = _lw6dat_stack_get_serial (sys_context, stack) + 1;
			  /*
			   * mmm, this is weird, ugly, we're not using the
			   * standard libmsg function lw6msg_cmd_generate_data,
			   * this is bad because it's a bug nest to duplicate
			   * code but OTOH we really must split the message
			   * and at the same time not having a test function
			   * in libmsg would really be a problem. On short
			   * messages it could fit, but not long ones...
			   */
			  snprintf (full_str, _LW6DAT_HEADER_MAX_SIZE, "%s %d %d %d ", LW6MSG_CMD_DATA, serial, order_i, order_n);
			  seq_from_cmd_str_offset = strlen (full_str);
			  if (seq_from_cmd_str_offset < _LW6DAT_HEADER_MAX_SIZE)
			    {
			      snprintf (full_str + seq_from_cmd_str_offset,
					_LW6DAT_HEADER_MAX_SIZE -
					seq_from_cmd_str_offset, "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL "x ", (long long) seq, (long long) logical_from);
			      cmd_str_offset = strlen (full_str);
			      if (cmd_str_offset < _LW6DAT_HEADER_MAX_SIZE)
				{
				  if (s > 0)
				    {
				      memcpy (full_str + cmd_str_offset, z + p, s);
				    }
				  else
				    {
				      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("0 sized atom"));
				    }
				  full_str[cmd_str_offset + s] = '\0';
				  ret = ret
				    && _lw6dat_stack_put_atom (sys_context, stack,
							       _LW6DAT_ATOM_TYPE_DATA,
							       serial, order_i, order_n, seq, full_str, seq_from_cmd_str_offset, cmd_str_offset, send_flag);
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("header is too long (cmd_str_offest=%d)"), cmd_str_offset);
				  ret = 0;
				}
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("header is too long (seq_from_cmd_str_offset=%d)"), seq_from_cmd_str_offset);
			      ret = 0;
			    }
			}
		    }
		}
	      LW6SYS_FREE (sys_context, z);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad logical_from in message \"%s\""), msg);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad seq in message \"%s\""), msg);
    }

  if (ret && local_seq_last && seq >= LW6DAT_SEQ_START)
    {
      /*
       * If operation was successful and local_seq_last is not NULL,
       * then update it, this is usefull to maintain a typical seq
       * that could use to put further dummy NOP operations for
       * keepalive's sake.
       */
      (*local_seq_last) = seq;
    }

  return ret;
}

int
_lw6dat_stack_shift (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int new_serial_0, int64_t new_seq_0)
{
  int ret = 0;
  int delta_serial = 0;
  int64_t delta_seq = 0LL;
  _lw6dat_stack_t *shifted_stack = NULL;
  _lw6dat_atom_t *first_atom = NULL;
  _lw6dat_atom_t *tmp_atom = NULL;
  int i = 0;
  int get_put_ok = 1;
  int guessed = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
	      _x_
	      ("BEGIN shift stack serial_0_base=%d serial_0_real=%d, seq_0=%"
	       LW6SYS_PRINTF_LL "d with new_serial_0=%d and new_seq_0=%"
	       LW6SYS_PRINTF_LL "d"), stack->serial_0_base, stack->serial_0_real, (long long) stack->seq_0, new_serial_0, (long long) new_seq_0);

  if (new_seq_0 == LW6DAT_SEQ_INVALID && ((first_atom = _lw6dat_stack_get_atom (sys_context, stack, new_serial_0)) != NULL))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		  _x_
		  ("shift stack, guessing new seq_0 from first atom stack->seq_0=%"
		   LW6SYS_PRINTF_LL "d first_atom->seq=%" LW6SYS_PRINTF_LL
		   "d first_atom->full_str=\"%s\""), (long long) stack->seq_0, (long long) first_atom->seq, _lw6dat_atom_get_full_str (sys_context,
																       first_atom));
      new_seq_0 = first_atom->seq;
      guessed = 1;
    }

  delta_serial = new_serial_0 - stack->serial_0_real;
  delta_seq = new_seq_0 - stack->seq_0;

  if (delta_serial == 0 && delta_seq == 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("shift stack, nothing to do"));
      ret = 1;
    }
  else if (delta_serial >= 0 && (delta_seq >= 0 || guessed))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("shift stack delta_serial=%d delta_seq=%" LW6SYS_PRINTF_LL "d"), delta_serial, (long long) delta_seq);
      /*
       * Now shifting a stack is fundamentally very long, but it should not
       * happen often, only at game creation/join and moreover, it should happen
       * on almost empty stacks.
       */
      shifted_stack = (_lw6dat_stack_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6dat_stack_t));
      if (shifted_stack)
	{
	  if (_lw6dat_stack_init (sys_context, shifted_stack, stack->node_id, new_serial_0, new_seq_0))
	    {
	      for (i = stack->serial_0_real + delta_serial; i <= stack->serial_max; ++i)
		{
		  if ((tmp_atom = _lw6dat_stack_get_atom (sys_context, stack, i)) != NULL)
		    {
		      get_put_ok = get_put_ok
			&& _lw6dat_stack_put_atom (sys_context, shifted_stack,
						   tmp_atom->type,
						   tmp_atom->serial,
						   tmp_atom->order_i,
						   tmp_atom->order_n,
						   tmp_atom->seq,
						   _lw6dat_atom_get_full_str
						   (sys_context, tmp_atom), tmp_atom->seq_from_cmd_str_offset, tmp_atom->cmd_str_offset, tmp_atom->send_flag);
		    }
		}
	      if (get_put_ok)
		{
		  first_atom = _lw6dat_stack_get_atom (sys_context, shifted_stack, new_serial_0);
		  if (!first_atom)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("after stack shift of node_id %"
				       LW6SYS_PRINTF_LL
				       "x unable to get atom for serial %d (which should be serial_0), while this is not formally an error, it's strange"),
				  (long long) stack->node_id, new_serial_0);
		      ret = 1;
		    }
		  else if ((new_seq_0 != LW6DAT_SEQ_INVALID) && (first_atom->seq > new_seq_0))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				  _x_
				  ("inconsistent stack shift for node_id %"
				   LW6SYS_PRINTF_LL "x first_atom->seq=%"
				   LW6SYS_PRINTF_LL "d shifted_stack->seq_0=%"
				   LW6SYS_PRINTF_LL
				   "d for serial %d full_str=\"%s\""),
				  (long long) stack->node_id,
				  (long long) first_atom->seq, (long long) shifted_stack->seq_0, new_serial_0, _lw6dat_atom_get_full_str (sys_context,
																	  first_atom));
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("shifted stack for node_id %" LW6SYS_PRINTF_LL "x serial_0=%d"), (long long) stack->node_id, new_serial_0);
		      _lw6dat_stack_clear (sys_context, stack);
		      memcpy (stack, shifted_stack, sizeof (_lw6dat_stack_t));
		      LW6SYS_FREE (sys_context, shifted_stack);
		      shifted_stack = NULL;
		      ret = 1;
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("error shifting stack for node_id %" LW6SYS_PRINTF_LL "x , get/put problem, i=%d"), (long long) stack->node_id, i);
		}
	    }
	  if (shifted_stack)
	    {
	      _lw6dat_stack_clear (sys_context, shifted_stack);
	      LW6SYS_FREE (sys_context, shifted_stack);
	      shifted_stack = NULL;
	    }
	}
      // todo ret=1
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("shift stack with negative delta delta_serial=%d delta_seq=%" LW6SYS_PRINTF_LL "d"), delta_serial, (long long) delta_seq);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
	      _x_ ("END shift stack serial_0_base=%d serial_0_real=%d seq_0=%"
		   LW6SYS_PRINTF_LL "d with new_serial_0=%d and new_seq_0=%"
		   LW6SYS_PRINTF_LL "d"), stack->serial_0_base, stack->serial_0_real, (long long) stack->seq_0, new_serial_0, (long long) new_seq_0);

  return ret;
}

int
_lw6dat_stack_calc_serial_draft_and_reference (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  int ret = 0;
  int64_t seq = 0LL;
  int serial = 0;
  _lw6dat_atom_t *atom = NULL;
  int atom_complete = 1;
  int atoms_no_hole = 1;
  int order_i = 0;
  int order_n = 0;

  serial = lw6sys_imax (stack->serial_min, stack->serial_reference);
  seq = _lw6dat_stack_get_seq_reference (sys_context, stack);
  while (serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, serial);
      if (atom)
	{
	  if (atom->order_i == 0)
	    {
	      atom_complete = 1;
	      seq = atom->seq;
	      order_n = atom->order_n;
	      for (order_i = 1; order_i < order_n && serial + order_i <= stack->serial_max; ++order_i)
		{
		  atom = _lw6dat_stack_get_atom (sys_context, stack, serial + order_i);
		  if (atom)
		    {
		      if (atom->seq != seq)
			{
			  atom_complete = 0;
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				      _x_
				      ("bad seq %" LW6SYS_PRINTF_LL
				       "d should be %" LW6SYS_PRINTF_LL
				       "d for atom \"%s\""), (long long) atom->seq, (long long) seq, _lw6dat_atom_get_full_str (sys_context, atom));
			}
		      if (atom->order_i != order_i)
			{
			  atom_complete = 0;
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				      _x_ ("bad order_i %d should be %d for atom \"%s\""), atom->order_i, order_i, _lw6dat_atom_get_full_str (sys_context,
																	      atom));
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				  _x_
				  ("incomplete msg from %" LW6SYS_PRINTF_LL
				   "x, missing atom serial=%d order=%d/%d"), (long long) stack->node_id, serial + order_i, order_i, order_n);
		      atom_complete = 0;
		    }
		}
	      if (order_i == order_n && atom_complete)
		{
		  /*
		   * We set ret to 1 here, else we leave it at 0, since
		   * we did not find any complete serie of atoms
		   */
		  ret = 1;
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
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("not starting with order=0 but %d full_str=\"%s\""), atom->order_i, _lw6dat_atom_get_full_str (sys_context, atom));
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

int64_t
_lw6dat_stack_get_seq_min (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  int64_t ret = LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_min >= stack->serial_0_real && stack->serial_min <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, stack->serial_min);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

int64_t
_lw6dat_stack_get_seq_max (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  int64_t ret = LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_max >= stack->serial_0_real && stack->serial_max <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, stack->serial_max);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

int64_t
_lw6dat_stack_get_seq_draft (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  int64_t ret = LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_draft >= stack->serial_0_real && stack->serial_draft <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, stack->serial_draft);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

int64_t
_lw6dat_stack_get_seq_reference (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack)
{
  int64_t ret = LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (stack->serial_reference >= stack->serial_0_real && stack->serial_reference <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, stack->serial_reference);
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
	  ret = atom->seq - 1LL;
	}
    }

  if (ret <= LW6DAT_SEQ_INVALID)
    {
      ret = LW6DAT_SEQ_INVALID;
    }

  return ret;
}

int
_lw6dat_stack_seq2serial (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int64_t seq)
{
  int ret = LW6DAT_SERIAL_INVALID;
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
  int64_t seq_min = LW6DAT_SEQ_INVALID;
  int64_t seq_max = LW6DAT_SEQ_INVALID;
  int i;

  if (stack->serial_max >= stack->serial_0_real && stack->serial_max <= stack->serial_n_1)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, stack->serial_min);
      if (atom)
	{
	  seq_min = atom->seq;
	}
      atom = _lw6dat_stack_get_atom (sys_context, stack, stack->serial_max);
      if (atom)
	{
	  seq_max = atom->seq;
	}
      if (seq_min != LW6DAT_SEQ_INVALID && seq_max != LW6DAT_SEQ_INVALID)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_ ("seq_min=%" LW6SYS_PRINTF_LL "d seq=%"
			   LW6SYS_PRINTF_LL "d seq_max=%" LW6SYS_PRINTF_LL
			   "d serial_min=%d serial_max=%d"), (long long) seq_min, (long long) seq, (long long) seq_max, stack->serial_min, stack->serial_max);
	  if (seq_min < seq_max && seq_min <= seq && seq <= seq_max && seq - seq_min < seq_max - seq)
	    {
	      /*
	       * Loop with i increasing since we suspect we'd
	       * better start from the bottom of the list
	       */
	      for (i = stack->serial_min; i <= stack->serial_max && ret == LW6DAT_SERIAL_INVALID; ++i)
		{
		  atom = _lw6dat_stack_get_atom (sys_context, stack, i);
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
	      for (i = stack->serial_max; i >= stack->serial_min && ret == LW6DAT_SERIAL_INVALID; --i)
		{
		  atom = _lw6dat_stack_get_atom (sys_context, stack, i);
		  if (atom)
		    {
		      if (atom->seq == seq)
			{
			  ret = i;
			}
		    }
		}
	    }
	  /*
	     if (seq_min == seq_max && seq_min == seq)
	     {
	     ret = stack->serial_min;
	     }
	   */
	  if (ret != LW6DAT_SERIAL_INVALID)
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
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistency, serial is %d and should be %d"), ret, atom->serial);
		    }
		}
	    }
	}
    }

  return ret;
}

int64_t
_lw6dat_stack_serial2seq (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int serial)
{
  int64_t ret = LW6DAT_SEQ_INVALID;
  _lw6dat_atom_t *atom = NULL;

  if (serial >= stack->serial_min && serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, serial);
      if (atom)
	{
	  ret = atom->seq;
	}
    }

  return ret;
}

lw6sys_list_t *
_lw6dat_stack_init_list (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *ret = NULL;

  ret = lw6sys_list_new (sys_context, lw6sys_free_callback);

  return ret;
}

static int
_update_msg_list_by_seq_with_search (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack,
				     lw6sys_list_t ** msg_list, int64_t seq,
				     int serial, int search_inc, int search_dec, int get_all, int *worst_msg_i, int *worst_msg_n)
{
  int ret = 0;
  _lw6dat_atom_t *atom = NULL;
  char *full_str = NULL;
  int atom_cmd_len = 0;
  char *msg = NULL;
  int cmd_len = 0;
  char *cmd = NULL;
  int i, n;
  int no_hole = 1;
  int keep = 1;
  int seq_from_len = 0;
  char seq_from[_LW6DAT_HEADER_MAX_SIZE + 1];
  char *unz = NULL;
  int local_worst_msg_i = 0;
  int local_worst_msg_n = 0;

  if (worst_msg_i)
    {
      local_worst_msg_i = (*worst_msg_i);
    }
  if (worst_msg_n)
    {
      local_worst_msg_n = (*worst_msg_n);
    }

  if (serial >= stack->serial_min && serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, serial);
      if (atom && atom->seq == seq)
	{
	  serial -= atom->order_i;
	  if (serial >= stack->serial_min && serial <= stack->serial_max)
	    {
	      n = atom->order_n;
	      cmd_len = _LW6DAT_ATOM_MAX_SIZE * n + 1;
	      cmd = (char *) LW6SYS_CALLOC (sys_context, cmd_len * sizeof (char));
	      if (cmd)
		{
		  full_str = _lw6dat_atom_get_full_str (sys_context, atom);
		  seq_from_len = atom->cmd_str_offset - atom->seq_from_cmd_str_offset;
		  if (seq_from_len > 0 && seq_from_len < _LW6DAT_HEADER_MAX_SIZE)
		    {
		      memset (seq_from, 0, _LW6DAT_HEADER_MAX_SIZE + 1);
		      memcpy (seq_from, full_str + atom->seq_from_cmd_str_offset, seq_from_len);
		      for (i = 0; i < n; ++i)
			{
			  atom = _lw6dat_stack_get_atom (sys_context, stack, serial + i);
			  if (atom)
			    {
			      if ((!get_all) && atom->update_msg_called)
				{
				  keep = 0;
				}
			      else
				{
				  full_str = _lw6dat_atom_get_full_str (sys_context, atom);
				  if (full_str)
				    {
				      /*
				       * Test no_hole and keep only here but still
				       * allow the loop to keep going, else we
				       * would fail to report missing atoms
				       */
				      if (no_hole && keep)
					{
					  atom_cmd_len = strlen (full_str + atom->cmd_str_offset);
					  memcpy (cmd + (i * _LW6DAT_ATOM_MAX_SIZE), full_str + atom->cmd_str_offset, atom_cmd_len);
					  if (i == (n - 1))
					    {
					      cmd_len = (i * _LW6DAT_ATOM_MAX_SIZE) + atom_cmd_len;
					    }
					}
				    }
				  else
				    {
				      /*
				       * Normally this is an almost theorical case,
				       * but well, we update range here as well as below
				       */
				      if (serial + i <= stack->serial_max + 1)
					{
					  stack->serial_miss_min = lw6sys_imin (stack->serial_miss_min, serial + i);
					  stack->serial_miss_max = lw6sys_imax (stack->serial_miss_max, serial + i);
					}

				      no_hole = 0;
				    }
				}
			    }
			  else
			    {
			      /*
			       * Update miss range, we might decide afterwards not to send
			       * all missing stuff to avoid sending too much, but we report
			       * it here as accurately as possible. Another good effect of doing
			       * it here is that we trap messages that were partially sent with
			       * remaining atoms in the queue. Note that we don't update the
			       * range if serial is really too big to avoid (again) sending
			       * data that is simply in the pipe. This is just a signal of the
			       * form "hey, there's more left", there's no point in saying
			       * there are many messages missing, just trigger the necessary calls.
			       */
			      if (serial <= stack->serial_max + 1)
				{
				  stack->serial_miss_min = lw6sys_imin (stack->serial_miss_min, serial + i);
				  stack->serial_miss_max = lw6sys_imax (stack->serial_miss_max, serial + i);
				}

			      no_hole = 0;

			      if (n >= _ORDER_N_PROGRESS_REPORT_MIN)
				{
				  if (i * local_worst_msg_n <= local_worst_msg_i * n)
				    {
				      local_worst_msg_i = i;
				      local_worst_msg_n = n;
				    }
				}
			    }
			}
		      // search messages before
		      if (search_dec)
			{
			  _update_msg_list_by_seq_with_search (sys_context, stack, msg_list, seq, serial - 1, 0, 1, get_all, &local_worst_msg_i,
							       &local_worst_msg_n);
			}
		      if (no_hole && keep)
			{
			  /*
			   * Set a flag to explicitly state that this has
			   * already been updated/got/popped and need not
			   * be retrieved again.
			   */
			  for (i = 0; i < n; ++i)
			    {
			      atom = _lw6dat_stack_get_atom (sys_context, stack, serial + i);
			      if (atom)
				{
				  atom->update_msg_called = 1;
				}
			    }
			  // put this message in the queue
			  unz = lw6msg_z_decode (sys_context, cmd);
			  if (unz)
			    {
			      if (atom && atom->type == _LW6DAT_ATOM_TYPE_DATA)
				{
				  msg = lw6sys_str_concat (sys_context, seq_from, unz);
				  if (msg)
				    {
				      if (n > 1)
					{
					  if (cmd_len >= _LONG_MSG_NOTICE)
					    {
					      lw6sys_log (sys_context,
							  LW6SYS_LOG_NOTICE, _x_ ("get long DATA message from stack, %d bytes in %d atoms"), cmd_len, n);
					    }
					  else
					    {
					      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("get DATA message from stack, %d bytes in %d atoms"), cmd_len, n);
					    }
					}
				      else
					{
					  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("get DATA message from stack, %d bytes in 1 atom"), cmd_len);
					}

				      lw6sys_list_push_back (sys_context, msg_list, msg);
				      ret = 1;
				    }
				}
			      else if (atom && atom->type == _LW6DAT_ATOM_TYPE_META)
				{
				  if (n == 1)
				    {
				      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("get META message from stack, ignoring"));
				    }
				  else
				    {
				      lw6sys_log (sys_context,
						  LW6SYS_LOG_WARNING, _x_ ("get long META message from stack, this should never happen, n=%d"), n);
				    }
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("get UNKNOWN message from stack, ignoring"));
				}
			      LW6SYS_FREE (sys_context, unz);
			    }
			}
		      // search messages after
		      if (search_inc)
			{
			  _update_msg_list_by_seq_with_search (sys_context, stack, msg_list, seq, serial + n, 1, 0, get_all, &local_worst_msg_i,
							       &local_worst_msg_n);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad seq_from_len=%d"), seq_from_len);
		    }
		  LW6SYS_FREE (sys_context, cmd);
		}
	    }
	}
    }

  if (local_worst_msg_n >= _ORDER_N_PROGRESS_REPORT_MIN)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("local_worst_msg_i=%d local_worst_msg_n=%d"), local_worst_msg_i, local_worst_msg_n);
      if (worst_msg_i)
	{
	  (*worst_msg_i) = local_worst_msg_i;
	}
      if (worst_msg_n)
	{
	  (*worst_msg_n) = local_worst_msg_n;
	}
    }

  return ret;
}

static void
_clear_recent (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int serial)
{
  int i;
  _lw6dat_atom_t *atom;

  for (i = serial; i <= stack->serial_max; ++i)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, serial);
      if (atom)
	{
	  atom->update_msg_called = 0;
	}
    }
}


int
_lw6dat_stack_update_msg_list_by_seq (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack,
				      lw6sys_list_t ** msg_list, int64_t seq, int get_all, int clear_recent, int *worst_msg_i, int *worst_msg_n)
{
  int ret = 0;
  int serial;

  if (seq != LW6DAT_SEQ_INVALID && seq >= _lw6dat_stack_get_seq_min (sys_context, stack))
    {
      serial = _lw6dat_stack_seq2serial (sys_context, stack, seq);

      if (serial != LW6DAT_SERIAL_INVALID)
	{
	  /*
	   * If clear_recent is set, then all subsequent messages are marked as popped
	   * We need to do this before the update is launched for the update does things
	   * nicely and really stops exactly when seq chances, when the clear function
	   * is raw and blindy clears stuff from given serial, which might be in the
	   * middle of a wide range of messages with the same seq.
	   */
	  if (clear_recent)
	    {
	      _clear_recent (sys_context, stack, serial);
	    }

	  ret = _update_msg_list_by_seq_with_search (sys_context, stack, msg_list, seq, serial, 1, 1, get_all, worst_msg_i, worst_msg_n);
	}
    }

  return ret;
}

int
_lw6dat_stack_update_atom_str_list_by_serial (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, lw6sys_list_t ** msg_list, int serial)
{
  int ret = 0;
  _lw6dat_atom_t *atom = NULL;
  char *atom_str = NULL;

  if (serial >= stack->serial_min && serial <= stack->serial_max)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, serial);
      if (atom)
	{
	  atom_str = _lw6dat_atom_get_full_str (sys_context, atom);
	  if (atom_str)
	    {
	      lw6sys_list_push_back (sys_context, msg_list, lw6sys_str_copy (sys_context, atom_str + atom->seq_from_cmd_str_offset));
	      ret = 1;
	    }
	}
    }

  return ret;
}

int
_lw6dat_stack_update_atom_str_list_not_sent (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, lw6sys_list_t ** msg_list, int target_index)
{
  int ret = 0;
  _lw6dat_atom_t *atom = NULL;
  char *atom_str = NULL;
  int serial = 0;
  int serial_min = 0;
  int serial_max = 0;
  int send_mask = 0;
  int no_hole = 1;
  int nb = 0;

  send_mask = _lw6dat_flag (target_index);
  /*
   * We keep a track of per-node "minimum atom to send",
   * this is just so that we avoid walking over the whole
   * stuff each time
   */
  serial_min = lw6sys_imax (stack->serial_min, stack->serial_min_to_send[target_index]);
  serial_max = stack->serial_max;
  for (serial = serial_min; serial <= serial_max; ++serial)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, serial);
      if (atom)
	{
	  if ((atom->send_flag & send_mask) && (!(atom->sent_status & send_mask)) && (atom->seq >= stack->seq_0s[target_index]))
	    {
	      atom_str = _lw6dat_atom_get_full_str (sys_context, atom);
	      if (atom_str)
		{
		  if (no_hole)
		    {
		      stack->serial_min_to_send[target_index] = serial;
		    }
		  atom->sent_status |= send_mask;
		  lw6sys_list_push_back (sys_context, msg_list, lw6sys_str_copy (sys_context, atom_str));
		  ++nb;
		  ret = 1;
		}
	    }
	}
      else
	{
	  no_hole = 0;
	}
    }

  if (nb > _LW6DAT_STACK_MANY_ATOMS)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("put %d messages in stack"), nb);
    }
  else if (nb > 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("put %d messages in stack"), nb);
    }

  return ret;
}


lw6dat_miss_t *
_lw6dat_stack_get_miss (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int max_range, int *worst_msg_i, int *worst_msg_n)
{
  lw6dat_miss_t *ret = NULL;
  int serial = 0;
  int serial_min = 0;
  int serial_max = 0;
  _lw6dat_atom_t *atom = NULL;
  int tmp_worst_msg_i = 0;
  int tmp_worst_msg_n = 0;
  int main_worst_msg_i = 0;
  int main_worst_msg_n = 0;

  /*
   * We need the serial_max limit to be set to something meaningfull,
   * else we would make useless requests for tons of messages that simply
   * were not sent yet.
   */
  if (stack->serial_max >= stack->serial_0_real && stack->serial_max <= stack->serial_n_1)
    {
      /*
       * Depending on wether some records were found last time, we perform
       * a full search or a smaller search. This avoids re-asking for the
       * same zone over and over, and also introduces some latency, old
       * messages are fectched first, and once they are complete, we ask
       * for the rest. Additionnally, it speeds up the search process by
       * doing it on smaller zones.
       */
      if (stack->serial_miss_min <= stack->serial_miss_max)
	{
	  serial_min = stack->serial_miss_min;
	  serial_max = stack->serial_miss_max;
	}
      else
	{
	  /*
	   * Here we use serial_0 and not serial_min for the missing
	   * might, indeed, be the very first one, and no previous
	   * message has ever been received.
	   */
	  serial_min = stack->serial_0_real;
	  serial_max = lw6sys_imax (stack->serial_max, stack->serial_miss_max);
	}

      stack->serial_miss_min = stack->serial_n_1;
      //stack->serial_miss_max = stack->serial_0_real;
      if (serial_min <= serial_max)
	{
	  for (serial = serial_min; serial <= serial_max && stack->serial_miss_min >= stack->serial_n_1; ++serial)
	    {
	      if (((atom = _lw6dat_stack_get_atom (sys_context, stack, serial)) == NULL) || (!atom->not_null))
		{
		  stack->serial_miss_min = serial;
		  //stack->serial_miss_max = serial;
		  if (tmp_worst_msg_i * main_worst_msg_n <= main_worst_msg_i * tmp_worst_msg_n)
		    {
		      main_worst_msg_i = tmp_worst_msg_i;
		      main_worst_msg_n = tmp_worst_msg_n;
		    }
		}
	      else
		{
		  if (atom->order_n >= _ORDER_N_PROGRESS_REPORT_MIN)
		    {
		      tmp_worst_msg_i = atom->order_i + 1;
		      tmp_worst_msg_n = atom->order_n;
		      if (tmp_worst_msg_i == tmp_worst_msg_n)
			{
			  tmp_worst_msg_i = tmp_worst_msg_n = 0;
			}
		    }
		}
	    }
	  if (stack->serial_miss_min < stack->serial_n_1)
	    {
	      serial_min = stack->serial_miss_min;
	      serial_max = lw6sys_imin (stack->serial_miss_min + max_range, serial_max);
	      for (serial = serial_min; serial <= serial_max; ++serial)
		{
		  if (((atom = _lw6dat_stack_get_atom (sys_context, stack, serial)) == NULL) || (!atom->not_null))
		    {
		      stack->serial_miss_max = serial;
		    }
		}
	    }
	}
      if (stack->serial_miss_min <= stack->serial_miss_max)
	{
	  ret = lw6dat_miss_new (sys_context, stack->node_id, stack->serial_miss_min, stack->serial_miss_max);
	}

      if (main_worst_msg_n >= _ORDER_N_PROGRESS_REPORT_MIN)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("main_worst_msg_i=%d main_worst_msg_n=%d"), main_worst_msg_i, main_worst_msg_n);
	  if (worst_msg_i)
	    {
	      (*worst_msg_i) = main_worst_msg_i;
	    }
	  if (worst_msg_n)
	    {
	      (*worst_msg_n) = main_worst_msg_n;
	    }
	}
    }

  return ret;
}

void
_lw6dat_stack_miss_invalidate (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int target_index, int serial_min, int serial_max)
{
  int loop_min;
  int loop_max;
  int i;
  _lw6dat_atom_t *atom;
  int send_not_mask = _lw6dat_not_flag (target_index);

  loop_min = lw6sys_imax (stack->serial_min, serial_min);
  loop_max = lw6sys_imin (stack->serial_max, serial_max);

  for (i = loop_min; i <= loop_max; ++i)
    {
      atom = _lw6dat_stack_get_atom (sys_context, stack, i);
      if (atom)
	{
	  atom->sent_status &= send_not_mask;
	}
    }

  if (loop_min <= loop_max)
    {
      stack->serial_min_to_send[target_index] = lw6sys_imin (stack->serial_min_to_send[target_index], loop_min);
    }
}

int
_lw6dat_stack_meta_update (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, lw6msg_meta_array_t * meta_array, int64_t seq)
{
  int ret = 0;
  int serial = 0;

  if (stack->node_id)
    {
      serial = _lw6dat_stack_seq2serial (sys_context, stack, seq);
      ret = lw6msg_meta_array_set (sys_context, meta_array, stack->node_id, serial, seq);
    }

  return ret;
}

int
_lw6dat_stack_meta_put (lw6sys_context_t * sys_context, _lw6dat_stack_t * stack, int64_t seq, lw6msg_meta_array_t * meta_array)
{
  int ret = 0;
  char *meta_msg;
  int serial = 0;
  /*
   * We always have i=0 and n=1 here as there's only
   * one message, spawning meta messages on several atoms
   * could probably be possible but just... we don't do it.
   */
  int i = 0;
  int n = 1;

  serial = _lw6dat_stack_get_serial (sys_context, stack) + 1;
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("meta_put seq=%" LW6SYS_PRINTF_LL "d serial=%d"), (long long) seq, serial);
  meta_msg = lw6msg_cmd_generate_meta (sys_context, serial, i, n, seq, meta_array);
  if (meta_msg)
    {
      ret = _lw6dat_stack_put_atom_str (sys_context, stack, meta_msg, _LW6DAT_FLAG_REMOTE);
      LW6SYS_FREE (sys_context, meta_msg);
    }

  return ret;
}
