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
#include "sys-internal.h"

#define _HISTORY_NB_MESSAGES 50
#define _HISTORY_MESSAGE_LENGTH 160
#define _HISTORY_MESSAGE_DOTS 3

typedef struct _history_msg_s
{
  int64_t timestamp;
  char content[_HISTORY_MESSAGE_LENGTH + 1];
} _history_msg_t;

typedef struct _history_s
{
  int current;
  _history_msg_t messages[_HISTORY_NB_MESSAGES];
} _history_t;

static _history_t _history;

/**
 * lw6sys_history_init
 *
 * @sys_context: global system context
 *
 * Initializes the history system. Not initializing won't cause
 * any segfault, but data will be inconsistent.
 *
 * Return value: none.
 */
void
lw6sys_history_init (lw6sys_context_t * sys_context)
{
  memset (&_history, 0, sizeof (_history_t));
}

/**
 * lw6sys_history_register
 *
 * @sys_context: global system context
 * @msg: the message to register.
 *
 * Registers a message in the history log, that is, adds it.
 *
 * Return value: none.
 */
void
lw6sys_history_register (lw6sys_context_t * sys_context, char *msg)
{
  int64_t timestamp;
  int current;
  int i;

  timestamp = lw6sys_get_timestamp (sys_context);
  current = _history.current;

  if (current < 0 || current >= _HISTORY_NB_MESSAGES)
    {
      current = 0;
    }
  _history.messages[current].timestamp = timestamp;
  strncpy (_history.messages[current].content, msg, _HISTORY_MESSAGE_LENGTH);
  if (_history.messages[current].content[_HISTORY_MESSAGE_LENGTH - 1] != '\0')
    {
      for (i = 0; i < _HISTORY_MESSAGE_DOTS; ++i)
	{
	  _history.messages[current].content[_HISTORY_MESSAGE_LENGTH - 1 - i] = '.';
	}
    }
  current++;
  if (current < 0 || current >= _HISTORY_NB_MESSAGES)
    {
      current = 0;
    }
  _history.current = current;
}

/**
 * lw6sys_history_get
 *
 * @sys_context: global system context
 * @timeout: the message age limit.
 *
 * Get all the messages that are younger than timeout (in seconds).
 *
 * Return value: a pointer on string pointers. May be NULL. Last
 *   pointer is NULL too, that's how you know the array is over.
 */
char_ptr_t *
lw6sys_history_get (lw6sys_context_t * sys_context, int64_t timeout)
{
  int64_t timestamp;
  int current;
  int i, j, k;
  char **ret = NULL;

  timestamp = lw6sys_get_timestamp (sys_context);
  current = _history.current;

  if (current < 0 || current >= _HISTORY_NB_MESSAGES)
    {
      current = 0;
    }
  ret = (char **) LW6SYS_CALLOC (sys_context, (_HISTORY_NB_MESSAGES + 1) * sizeof (char *));
  if (ret)
    {
      for (i = 0, j = current, k = 0; i < _HISTORY_NB_MESSAGES; ++i, ++j)
	{
	  if (j >= _HISTORY_NB_MESSAGES)
	    {
	      j = 0;
	    }
	  if (_history.messages[j].timestamp + timeout > timestamp && _history.messages[j].content[0] != '\0')
	    {
	      _history.messages[j].content[_HISTORY_MESSAGE_LENGTH] = '\0';
	      ret[k] = lw6sys_str_copy (sys_context, _history.messages[j].content);
	      k++;
	    }
	}
    }
  return ret;
}

/**
 * lw6sys_history_free
 *
 * @sys_context: global system context
 * @history: the data to free
 *
 * Frees a pointer returned by @lw6sys_history_get.
 *
 * Return value: none.
 */
void
lw6sys_history_free (lw6sys_context_t * sys_context, char **history)
{
  char **i;

  if (history)
    {
      for (i = history; i[0]; i++)
	{
	  LW6SYS_FREE (sys_context, i[0]);
	}
      LW6SYS_FREE (sys_context, history);
    }
}
