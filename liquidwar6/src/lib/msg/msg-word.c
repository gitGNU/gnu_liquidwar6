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

#include "msg.h"

/**
 * lw6msg_word_first
 *
 * @word: will contain the parsed word
 * @next: if NOT NULL, will contain a (non freeable) pointer on remaining message
 * @msg: the message to parse
 *
 * Analyses a message and gets the first word. This word is put in @buf member
 * with its length. @next is usefull if you want to parse the rest of the message,
 * it points at the beginning of it.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6msg_word_first (lw6msg_word_t * word, char **next, char *msg)
{
  int ret = 0;
  int i = 0, j = 0;
  int len = 0;

  word->len = 0;
  word->buf[0] = '\0';
  if (next)
    {
      (*next) = NULL;
    }

  while (lw6sys_chr_is_space (msg[i]))
    {
      i++;
    }
  j = i;
  /*
   * Note: (len=(j-i)<=...) must really be the first test else len is false
   */
  while ((len = (j - i)) <= LW6MSG_MAX_WORD_SIZE && msg[j]
	 && !lw6sys_chr_is_space (msg[j]) && !lw6sys_chr_is_eol (msg[j]))
    {
      j++;
    }
  if (i < j)
    {
      if (len <= LW6MSG_MAX_WORD_SIZE)
	{
	  word->len = len;
	  memcpy (word->buf, msg + i, len);
	  word->buf[len] = '\0';
	  if (next)
	    {
	      while (lw6sys_chr_is_space (msg[j]))
		{
		  j++;
		}
	      (*next) = msg + j;
	    }
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("word too long"));
	}
    }

  return ret;
}

/**
 * lw6msg_word_first_int
 *
 * @parsed_value: will contain the parsed value
 * @next: if NOT NULL, will contain a (non freeable) pointer on remaining message
 * @msg: the message to parse
 *
 * Analyses a message, gets the first word and interpret it as an int.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6msg_word_first_int (int *parsed_value, char **next, char *msg)
{
  int ret = 0;
  lw6msg_word_t word;

  (*parsed_value) = 0;
  if (lw6msg_word_first (&word, next, msg))
    {
      (*parsed_value) = lw6sys_atoi (word.buf);
      ret = 1;
    }

  return ret;
}

/**
 * lw6msg_word_first_int_gt0
 *
 * @parsed_value: will contain the parsed value
 * @next: if NOT NULL, will contain a (non freeable) pointer on remaining message
 * @msg: the message to parse
 *
 * Analyses a message, gets the first word and interpret it as an int.
 * The value must be strictly greater than 0.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6msg_word_first_int_gt0 (int *parsed_value, char **next, char *msg)
{
  int ret = 0;
  int tmp_value = 0;

  (*parsed_value) = 0;
  if (lw6msg_word_first_int (&tmp_value, next, msg))
    {
      if (tmp_value > 0)
	{
	  (*parsed_value) = tmp_value;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("bad int value %d, not strictly greater than 0"),
		      tmp_value);
	}
    }

  return ret;
}

/**
 * lw6msg_word_first_id_16
 *
 * @parsed_value: will contain the parsed value
 * @next: if NOT NULL, will contain a (non freeable) pointer on remaining message
 * @msg: the message to parse
 *
 * Analyses a message, gets the first word and interpret it as an 16-bit id.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6msg_word_first_id_16 (u_int16_t * parsed_value, char **next, char *msg)
{
  int ret = 0;
  lw6msg_word_t word;
  u_int16_t tmp_value;

  (*parsed_value) = 0;
  if (lw6msg_word_first (&word, next, msg))
    {
      tmp_value = lw6sys_id_atol (word.buf);
      if (tmp_value && lw6sys_check_id_16 (tmp_value))
	{
	  (*parsed_value) = tmp_value;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("bad id 16 \"%s\""), word.buf);
	}
    }

  return ret;
}

/**
 * lw6msg_word_first_id_32
 *
 * @parsed_value: will contain the parsed value
 * @next: if NOT NULL, will contain a (non freeable) pointer on remaining message
 * @msg: the message to parse
 *
 * Analyses a message, gets the first word and interpret it as an 32-bit id.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6msg_word_first_id_32 (u_int32_t * parsed_value, char **next, char *msg)
{
  int ret = 0;
  lw6msg_word_t word;
  u_int32_t tmp_value;

  (*parsed_value) = 0;
  if (lw6msg_word_first (&word, next, msg))
    {
      tmp_value = lw6sys_id_atol (word.buf);
      if (tmp_value && lw6sys_check_id_32 (tmp_value))
	{
	  (*parsed_value) = tmp_value;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("bad id 32 \"%s\""), word.buf);
	}
    }

  return ret;
}

/**
 * lw6msg_word_first_id_64
 *
 * @parsed_value: will contain the parsed value
 * @next: if NOT NULL, will contain a (non freeable) pointer on remaining message
 * @msg: the message to parse
 *
 * Analyses a message, gets the first word and interpret it as an 64-bit id.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6msg_word_first_id_64 (u_int64_t * parsed_value, char **next, char *msg)
{
  int ret = 0;
  lw6msg_word_t word;
  u_int64_t tmp_value;

  (*parsed_value) = 0;
  if (lw6msg_word_first (&word, next, msg))
    {
      tmp_value = lw6sys_id_atol (word.buf);
      if (tmp_value && lw6sys_check_id_64 (tmp_value))
	{
	  (*parsed_value) = tmp_value;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("bad id 64 \"%s\""), word.buf);
	}
    }

  return ret;
}
