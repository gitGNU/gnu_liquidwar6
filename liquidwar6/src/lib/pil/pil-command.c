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

#include "pil.h"
#include "pil-internal.h"

#include <ctype.h>

static int
is_spc (char c)
{
  /*
   * Because c is of type char, comparing it to 32 is enough, it will
   * also trap chars > 127 (accents & the rest), in fact we just want
   * to ignore those, and use strict ASCII.
   */
  return (((signed char) c) <= 32);
}

static void
_command_normalize (lw6sys_context_t * sys_context, char *command_text)
{
  char *pos_src = NULL;
  char *pos_dst = NULL;

  pos_src = command_text;
  while (is_spc (*pos_src))
    {
      pos_src++;
    }
  for (pos_dst = command_text; (*pos_src) && (*pos_dst); ++pos_src, ++pos_dst)
    {
      if (is_spc (pos_src[0]))
	{
	  while (is_spc (pos_src[1]))
	    {
	      pos_src++;
	    }
	  pos_dst[0] = ' ';
	}
      else
	{
	  pos_dst[0] = toupper (pos_src[0]);
	}
    }

  pos_dst[0] = '\0';
}

static int
_command_add_parse (lw6sys_context_t * sys_context, lw6pil_command_t * command, char *command_args)
{
  int ret = 0;
  char *pos;
  char *seek;

  seek = command_args;
  pos = seek;
  while (!is_spc (*seek))
    {
      seek++;
    }
  if (*seek)
    {
      (*seek) = '\0';
      seek++;
      command->args.add.cursor_id = lw6sys_id_atol (sys_context, pos);
      pos = seek;
      if (lw6sys_check_id_16 (sys_context, command->args.add.cursor_id))
	{
	  command->args.add.team_color = lw6map_team_color_key_to_index (sys_context, pos);
	  if (command->args.add.team_color >= 0 && command->args.add.team_color < LW6MAP_MAX_NB_TEAMS)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_
			  ("%s command parsed cursor_id=%x, team_color=%d (%s)"),
			  LW6PIL_COMMAND_TEXT_ADD, (int) command->args.set.cursor_id, command->args.add.team_color, pos);
	      ret = 1;
	    }
	}
    }

  return ret;
}

static int
_command_remove_parse (lw6sys_context_t * sys_context, lw6pil_command_t * command, char *command_args)
{
  int ret = 0;

  command->args.remove.cursor_id = lw6sys_id_atol (sys_context, command_args);
  if (lw6sys_check_id_16 (sys_context, command->args.remove.cursor_id))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%s command parsed cursor_id=%x"), LW6PIL_COMMAND_TEXT_REMOVE, (int) command->args.set.cursor_id);
      ret = 1;
    }

  return ret;
}

static int
_command_set_parse (lw6sys_context_t * sys_context, lw6pil_command_t * command, char *command_args)
{
  int ret = 0;
  char *pos;
  char *seek;

  seek = command_args;
  pos = seek;
  while (!is_spc (*seek))
    {
      seek++;
    }
  if (*seek)
    {
      (*seek) = '\0';
      seek++;
      command->args.set.cursor_id = lw6sys_id_atol (sys_context, pos);
      pos = seek;
      if (lw6sys_check_id_16 (sys_context, command->args.set.cursor_id))
	{
	  while (!is_spc (*seek))
	    {
	      seek++;
	    }
	  if (*seek)
	    {
	      (*seek) = '\0';
	      seek++;
	      command->args.set.x = lw6sys_atoi (sys_context, pos);
	      pos = seek;
	      while (!is_spc (*seek))
		{
		  seek++;
		}
	      if (*seek)
		{
		  (*seek) = '\0';
		  command->args.set.y = lw6sys_atoi (sys_context, pos);
		  seek++;
		  pos = seek;
		  while (!is_spc (*seek))
		    {
		      seek++;
		    }
		  if (*seek)
		    {
		      (*seek) = '\0';
		      command->args.set.fire = lw6sys_atoi (sys_context, pos);
		      if (command->args.set.fire == 0 || command->args.set.fire == 1)
			{
			  seek++;
			  pos = seek;
			  command->args.set.fire2 = lw6sys_atoi (sys_context, pos);
			  if (command->args.set.fire2 == 0 || command->args.set.fire2 == 1)
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
					  _x_
					  ("%s command parsed cursor_id=%x, x=%d, y=%d, fire=%d, fire2=%d"),
					  LW6PIL_COMMAND_TEXT_SET,
					  (int) command->args.set.cursor_id,
					  command->args.set.x, command->args.set.y, command->args.set.fire, command->args.set.fire2);
			      ret = 1;
			    }
			}
		    }
		}
	    }
	}
    }

  return ret;
}

static int
_command_dump_parse (lw6sys_context_t * sys_context, lw6pil_command_t * command, char *command_args)
{
  int ret = 0;
  char *pos;
  char *seek;

  seek = command_args;
  pos = seek;
  while (!is_spc (*seek))
    {
      seek++;
    }
  if (*seek)
    {
      (*seek) = '\0';
      seek++;
      command->args.dump.level_hexa = lw6sys_str_copy (sys_context, pos);
      pos = seek;

      if (command->args.dump.level_hexa)
	{
	  while (!is_spc (*seek))
	    {
	      seek++;
	    }

	  if (*seek)
	    {
	      (*seek) = '\0';
	      seek++;
	      command->args.dump.game_struct_hexa = lw6sys_str_copy (sys_context, pos);
	      pos = seek;

	      if (command->args.dump.game_struct_hexa)
		{
		  command->args.dump.game_state_hexa = lw6sys_str_copy (sys_context, pos);
		  if (command->args.dump.game_state_hexa)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%s command parsed"), LW6PIL_COMMAND_TEXT_DUMP);
		      ret = 1;
		    }
		}
	    }
	}
    }

  return ret;
}

static int
_command_parse (lw6sys_context_t * sys_context, lw6pil_command_t * command, const char *command_text, int64_t seq_0, int round_0)
{
  int ret = 0;
  char *pos;
  char *seek;
  char *tmp;

  tmp = lw6sys_str_copy (sys_context, command_text);
  if (tmp)
    {
      _command_normalize (sys_context, tmp);
      command->text = lw6sys_str_copy (sys_context, tmp);
      if (command->text)
	{
	  seek = tmp;
	  pos = seek;
	  while (!is_spc (*seek))
	    {
	      seek++;
	    }
	  if (*seek)
	    {
	      (*seek) = '\0';
	      seek++;
	      command->seq = lw6sys_atoll (sys_context, pos);
	      if (command->seq >= _LW6PIL_MIN_SEQ_0 && command->seq >= seq_0)
		{
		  command->round = command->seq - seq_0 + round_0;
		  pos = seek;
		  while (!is_spc (*seek))
		    {
		      seek++;
		    }
		  if (*seek)
		    {
		      (*seek) = '\0';
		      seek++;
		      command->node_id = lw6sys_id_atol (sys_context, pos);
		      if (lw6sys_check_id_64 (sys_context, command->node_id))
			{
			  pos = seek;
			  while (!is_spc (*seek))
			    {
			      seek++;
			    }
			  if (*seek)
			    {
			      (*seek) = '\0';
			      seek++;
			      if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_ADD))
				{
				  command->code = LW6PIL_COMMAND_CODE_ADD;
				  ret = _command_add_parse (sys_context, command, seek);
				}
			      else if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_REMOVE))
				{
				  command->code = LW6PIL_COMMAND_CODE_REMOVE;
				  ret = _command_remove_parse (sys_context, command, seek);
				}
			      else if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_SET))
				{
				  command->code = LW6PIL_COMMAND_CODE_SET;
				  ret = _command_set_parse (sys_context, command, seek);
				}
			      else if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_DUMP))
				{
				  command->code = LW6PIL_COMMAND_CODE_DUMP;
				  ret = _command_dump_parse (sys_context, command, seek);
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad command text \"%s\""), pos);
				}
			    }
			  else
			    {
			      if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_NOP))
				{
				  command->code = LW6PIL_COMMAND_CODE_NOP;
				  ret = 1;
				}
			      else if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_REGISTER))
				{
				  command->code = LW6PIL_COMMAND_CODE_REGISTER;
				  ret = 1;
				}
			      else if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_UNREGISTER))
				{
				  command->code = LW6PIL_COMMAND_CODE_UNREGISTER;
				  ret = 1;
				}
			      else if (lw6sys_str_is_same (sys_context, pos, LW6PIL_COMMAND_TEXT_SEED))
				{
				  command->code = LW6PIL_COMMAND_CODE_SEED;
				  ret = 1;
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad command text \"%s\""), pos);
				}
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad id in command \"%s\""), command_text);
			}
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad seq in command \"%s\""), command_text);
		}
	    }
	}
      LW6SYS_FREE (sys_context, tmp);
    }

  if (!ret)
    {
      if (command->text)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid command \"%s\""), command->text);
	  LW6SYS_FREE (sys_context, command->text);
	}
      memset (command, 0, sizeof (lw6pil_command_t));
    }

  return ret;
}

/**
 * lw6pil_command_new
 *
 * @sys_context: global system context
 * @command_text: readable text of the command
 * @seq_0: sequence offset reference (to calculate difference between sequence and rounds)
 * @round_0: round offset reference (to calculate difference between sequence and rounds)
 *
 * Creates a new command from its text representation.
 *
 * Return value: newly allocated object
 */
lw6pil_command_t *
lw6pil_command_new (lw6sys_context_t * sys_context, const char *command_text, int64_t seq_0, int round_0)
{
  lw6pil_command_t *ret = NULL;

  ret = (lw6pil_command_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6pil_command_t));
  if (ret)
    {
      if (!_command_parse (sys_context, ret, command_text, seq_0, round_0))
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6pil_command_dup
 *
 * @sys_context: global system context
 * @command: object to duplicate
 *
 * Creates a copy of a command struct.
 *
 * Return value: newly allocated object.
 */
lw6pil_command_t *
lw6pil_command_dup (lw6sys_context_t * sys_context, lw6pil_command_t * command)
{
  lw6pil_command_t *ret = NULL;

  ret = (lw6pil_command_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6pil_command_t));
  if (ret)
    {
      memcpy (ret, command, sizeof (lw6pil_command_t));
      ret->text = lw6sys_str_copy (sys_context, command->text);
      if (!ret->text)
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
      if (ret && ret->code == LW6PIL_COMMAND_CODE_DUMP)
	{
	  ret->args.dump.level_hexa = lw6sys_str_copy (sys_context, command->args.dump.level_hexa);
	  ret->args.dump.game_struct_hexa = lw6sys_str_copy (sys_context, command->args.dump.game_struct_hexa);
	  ret->args.dump.game_state_hexa = lw6sys_str_copy (sys_context, command->args.dump.game_state_hexa);
	  if ((!ret->args.dump.level_hexa) || (!ret->args.dump.game_struct_hexa) || (!ret->args.dump.game_state_hexa))
	    {
	      lw6pil_command_free (sys_context, ret);
	      ret = NULL;
	    }
	}
    }

  return ret;
}

/**
 * lw6pil_command_free
 *
 * @sys_context: global system context
 * @command: command to free
 *
 * Frees a command struct, with all its members.
 *
 * Return value: none.
 */
void
lw6pil_command_free (lw6sys_context_t * sys_context, lw6pil_command_t * command)
{
  if (command)
    {
      if (command->text)
	{
	  LW6SYS_FREE (sys_context, command->text);
	}
      if (command->code == LW6PIL_COMMAND_CODE_DUMP)
	{
	  if (command->args.dump.level_hexa)
	    {
	      LW6SYS_FREE (sys_context, command->args.dump.level_hexa);
	    }
	  if (command->args.dump.game_struct_hexa)
	    {
	      LW6SYS_FREE (sys_context, command->args.dump.game_struct_hexa);
	    }
	  if (command->args.dump.game_state_hexa)
	    {
	      LW6SYS_FREE (sys_context, command->args.dump.game_state_hexa);
	    }
	}
      LW6SYS_FREE (sys_context, command);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL command"));
    }
}

int
_lw6pil_command_sort_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;
  const lw6pil_command_t *command_a = (const lw6pil_command_t *) ptr_a;
  const lw6pil_command_t *command_b = (const lw6pil_command_t *) ptr_b;

  if (command_a->seq < command_b->seq)
    {
      ret = -1;
    }
  else if (command_a->seq > command_b->seq)
    {
      ret = 1;
    }
  else if (command_a->code < command_b->code)
    {
      ret = -1;
    }
  else if (command_a->code > command_b->code)
    {
      ret = 1;
    }
  else if (command_a->node_id < command_b->node_id)
    {
      ret = -1;
    }
  else if (command_a->node_id > command_b->node_id)
    {
      ret = 1;
    }
  else
    {
      ret = strcmp (command_a->text, command_b->text);
    }

  return ret;
}

/**
 * lw6pil_command_repr
 *
 * @sys_context: global system context
 * @command: command to represent
 *
 * Gives a readable representation of a command.
 *
 * Return value: dynamically allocated string.
 */
char *
lw6pil_command_repr (lw6sys_context_t * sys_context, const lw6pil_command_t * command)
{
  char *ret = NULL;

  switch (command->code)
    {
    case LW6PIL_COMMAND_CODE_NOP:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s", (long long) command->seq, (long long) command->node_id, LW6PIL_COMMAND_TEXT_NOP);
      break;
    case LW6PIL_COMMAND_CODE_REGISTER:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s", (long long) command->seq, (long long) command->node_id, LW6PIL_COMMAND_TEXT_REGISTER);
      break;
    case LW6PIL_COMMAND_CODE_ADD:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s %x %s", (long long) command->seq,
			    (long long) command->node_id,
			    LW6PIL_COMMAND_TEXT_ADD,
			    (int) command->args.add.cursor_id, lw6map_team_color_index_to_key (sys_context, command->args.add.team_color));
      break;
    case LW6PIL_COMMAND_CODE_SET:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s %x %d %d %d %d", (long long) command->seq,
			    (long long) command->node_id,
			    LW6PIL_COMMAND_TEXT_SET,
			    (int) command->args.set.cursor_id, command->args.set.x, command->args.set.y, command->args.set.fire, command->args.set.fire2);
      break;
    case LW6PIL_COMMAND_CODE_REMOVE:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s %x", (long long) command->seq,
			    (long long) command->node_id, LW6PIL_COMMAND_TEXT_REMOVE, (int) command->args.remove.cursor_id);
      break;
    case LW6PIL_COMMAND_CODE_UNREGISTER:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s", (long long) command->seq, (long long) command->node_id, LW6PIL_COMMAND_TEXT_UNREGISTER);
      break;
    case LW6PIL_COMMAND_CODE_SEED:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s", (long long) command->seq, (long long) command->node_id, LW6PIL_COMMAND_TEXT_SEED);
      break;
    case LW6PIL_COMMAND_CODE_DUMP:
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL
			    "x %s %d %d %d", (long long) command->seq,
			    (long long) command->node_id,
			    LW6PIL_COMMAND_TEXT_DUMP,
			    (int) strlen (command->args.dump.game_state_hexa),
			    (int) strlen (command->args.dump.game_struct_hexa), (int) strlen (command->args.dump.level_hexa));
      break;
    default:
      ret = lw6sys_new_sprintf (sys_context, "%s %s", LW6PIL_COMMAND_TEXT_INVALID, command->text);
      break;
    }

  return ret;
}

/**
 * lw6pil_command_execute
 *
 * @sys_context: global system context
 * @dump: pointer on dump structure (out param, can be NULL)
 * @timestamp: current timestamp (can be 0 if dump is NULL)
 * @game_state: game state to work on, can be NULL (usefull for DUMP)
 * @command: command to process
 *
 * Interprets a command and runs it against game_state. If
 * dump and timestamp are set, then any DUMP command will fill
 * the dump structure with the right values. If not running
 * from a pilot context, this is useless, use NULL and 0LL.
 *
 * Return value: 1 if ok, 0 if failed.
 */
int
lw6pil_command_execute (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp, lw6ker_game_state_t * game_state, lw6pil_command_t * command)
{
  int ret = 0;
  lw6ker_cursor_t cursor;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("execute command \"%s\""), command->text);

  switch (command->code)
    {
    case LW6PIL_COMMAND_CODE_NOP:
      // do nothing!
      ret = 1;
      break;
    case LW6PIL_COMMAND_CODE_REGISTER:
      if (game_state)
	{
	  ret = lw6ker_game_state_register_node (sys_context, game_state, command->node_id);
	}
      break;
    case LW6PIL_COMMAND_CODE_ADD:
      if (game_state)
	{
	  ret = lw6ker_game_state_add_cursor (sys_context, game_state, command->node_id, command->args.add.cursor_id, command->args.add.team_color);
	}
      break;
    case LW6PIL_COMMAND_CODE_SET:
      if (game_state)
	{
	  lw6ker_cursor_reset (sys_context, &cursor);
	  cursor.node_id = command->node_id;
	  cursor.cursor_id = command->args.set.cursor_id;
	  cursor.pos.x = command->args.set.x;
	  cursor.pos.y = command->args.set.y;
	  cursor.fire = command->args.set.fire;
	  cursor.fire2 = command->args.set.fire2;
	  ret = lw6ker_game_state_set_cursor (sys_context, game_state, &cursor);
	}
      break;
    case LW6PIL_COMMAND_CODE_REMOVE:
      if (game_state)
	{
	  ret = lw6ker_game_state_remove_cursor (sys_context, game_state, command->node_id, command->args.remove.cursor_id);
	}
      break;
    case LW6PIL_COMMAND_CODE_UNREGISTER:
      if (game_state)
	{
	  ret = lw6ker_game_state_unregister_node (sys_context, game_state, command->node_id);
	}
      break;
    case LW6PIL_COMMAND_CODE_DUMP:
      /*
       * Here we don't test for game_state being not NULL, in fact,
       * the whole purpose of allowing game_state to be NULL is to
       * be able to execute DUMP, which does not require a game_state.
       * Note that DUMP can arrive both in the case game_state is not NULL
       * and in the case it is NULL.
       */
      ret = lw6pil_dump_command_execute (sys_context, dump, timestamp, command, NULL);
      break;
    case LW6PIL_COMMAND_CODE_SEED:
      /*
       * SEED is always fine, this is sort of a placeholder to make
       * sure the first message is not a hudge DUMP, but else, it
       * really does nothing serious.
       */
      ret = 1;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("incorrect command \"%s\""), command->text);
      break;
    }

  return ret;
}

/**
 * lw6pil_command_execute_text
 *
 * @sys_context: global system context
 * @dump: pointer on dump structure (out param, can be NULL)
 * @timestamp: current timestamp (can be 0 if dump is NULL)
 * @game_state: game state to work on, can be NULL (typically for DUMP)
 * @command_text: command text to process
 * @seq_0: reference seq_0, used to genereate consistent dumps if needed
 *
 * Interprets a command text and runs it against game_state. If
 * dump and timestamp are set, then any DUMP command will fill
 * the dump structure with the right values. If not running
 * from a pilot context, this is useless, use NULL and 0LL.
 *
 * Return value: 1 if ok, 0 if failed.
 */
int
lw6pil_command_execute_text (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp, lw6ker_game_state_t * game_state,
			     const char *command_text, int64_t seq_0)
{
  int ret = 0;
  lw6pil_command_t *command = NULL;

  command = lw6pil_command_new (sys_context, command_text, seq_0, _LW6PIL_MIN_ROUND_0);
  if (command)
    {
      ret = lw6pil_command_execute (sys_context, dump, timestamp, game_state, command);
      lw6pil_command_free (sys_context, command);
    }

  return ret;
}

/**
 * lw6pil_command_execute_local
 *
 * @sys_context: global system context
 * @local_cursors: local cursors information
 * @command: command to execute
 *
 * Executes a local command, typically a cursor move, on the
 * local_cursor struct, without changing any game state.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6pil_command_execute_local (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, lw6pil_command_t * command)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("execute command \"%s\""), command->text);
  switch (command->code)
    {
    case LW6PIL_COMMAND_CODE_NOP:
    case LW6PIL_COMMAND_CODE_REGISTER:
    case LW6PIL_COMMAND_CODE_ADD:
    case LW6PIL_COMMAND_CODE_REMOVE:
    case LW6PIL_COMMAND_CODE_UNREGISTER:
    case LW6PIL_COMMAND_CODE_DUMP:
      // do nothing!
      ret = 1;
      break;
    case LW6PIL_COMMAND_CODE_SET:
      ret = lw6pil_local_cursors_set_xy (sys_context, local_cursors, command->args.set.cursor_id, command->args.set.x, command->args.set.y);
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("incorrect command \"%s\""), command->text);
      break;
    }

  return ret;
}

/**
 * lw6pil_command_execute_local_text
 *
 * @sys_context: global system context
 * @local_cursors: local cursors information
 * @command_text: command text to execute
 *
 * Executes a local command text, typically a cursor move, on the
 * local_cursor struct, without changing any game state.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6pil_command_execute_local_text (lw6sys_context_t * sys_context, lw6pil_local_cursors_t * local_cursors, const char *command_text)
{
  int ret = 0;
  lw6pil_command_t *command = NULL;

  command = lw6pil_command_new (sys_context, command_text, _LW6PIL_MIN_SEQ_0, _LW6PIL_MIN_ROUND_0);
  if (command)
    {
      ret = lw6pil_command_execute_local (sys_context, local_cursors, command);
      lw6pil_command_free (sys_context, command);
    }

  return ret;
}
