/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#define _STAGE_1_CHECKSUM 0x0
#define _STAGE_2_CHECKSUM 0x0
#define _STAGE_3_CHECKSUM 0x0

#define _STAGE_1_ROUND 1000
#define _STAGE_2_ROUND 2000
#define _STAGE_3_ROUND 3000

#define _SEQ_0 1000000000000

#define _MAX_MESSAGES_PER_NODE_AND_STAGE 20
#define _INIT_SCALE_PERCENT 200

/*
 * Note: it's important that last message round is less than
 * (LW6MAP_RULES_DEFAULT_TOTAL_TIME * 
 * LW6MAP_RULES_DEFAULT_ROUNDS_PER_SEC *
 * LW6MAP_RULES_DEFAULT_MOVES_PER_ROUND)
 * which is something like 900 * 50 * 2 = 90000.
 */
static const char
  *_commands[LW6PIL_SUITE_NB_NODES][LW6PIL_SUITE_NB_STAGES]
  [_MAX_MESSAGES_PER_NODE_AND_STAGE] = {
  /*
   * NODE_A messages
   */
  {
   /*
    * STAGE_1 messages
    */
   {
    "1000000000005 1001100110011001 REGISTER",
    "1000000000005 1001100110011001 ADD 1001 RED",
    "1000000000005 1001100110011001 ADD 1002 GREEN",
    "1000000000005 1001100110011001 ADD 1003 BLUE",
    "1000000000005 1001100110011001 ADD 1004 YELLOW",
    "1000000010005 1001100110011001 SET 1001 1 1 1 0",
    "1000000010005 1001100110011001 SET 1002 2 2 1 0",
    "1000000010005 1001100110011001 SET 1003 3 3 1 0",
    "1000000010005 1001100110011001 SET 1004 4 4 1 0",
    "1000000020005 1001100110011001 SET 1001 1 1 0 1",
    "1000000020005 1001100110011001 SET 1002 2 2 0 1",
    "1000000020005 1001100110011001 SET 1003 3 3 0 1",
    "1000000020005 1001100110011001 SET 1004 4 4 0 1",
    NULL},
   /*
    * STAGE_2 messages
    */
   {NULL},
   /*
    * STAGE_3 messages
    */
   {NULL}},
  /*
   * NODE_B messages
   */
  {
   {NULL},
   {
    "1000000030005 2002200220022002 REGISTER",
    "1000000030005 2002200220022002 ADD 2001 CYAN",
    "1000000030005 2002200220022002 ADD 2002 MAGENTA",
    "1000000030005 2002200220022002 ADD 2003 ORANGE",
    "1000000040005 2002200220022002 SET 2001 11 11 1 0",
    "1000000040005 2002200220022002 SET 2002 22 22 1 0",
    "1000000040005 2002200220022002 SET 2003 33 33 1 0",
    "1000000050005 2002200220022002 SET 2001 11 11 0 1",
    "1000000050005 2002200220022002 SET 2002 22 22 0 1",
    "1000000050005 2002200220022002 SET 2003 33 33 0 1",
    NULL},
   {NULL}},
  /*
   * NODE_C messages
   */
  {
   /*
    * STAGE_1 messages
    */
   {NULL},
   /*
    * STAGE_2 messages
    */
   {NULL},
   /*
    * STAGE_3 messages
    */
   {
    "1000000060005 3003300330033003 REGISTER",
    "1000000060005 3003300330033003 ADD 3001 LIGHTBLUE",
    "1000000060005 3003300330033003 ADD 3002 PURPLE",
    "1000000060005 3003300330033003 ADD 3003 PINK",
    "1000000070005 3003300330033003 SET 3001 11 1 1 0",
    "1000000070005 3003300330033003 SET 3002 22 2 1 0",
    "1000000070005 3003300330033003 SET 3003 33 3 1 0",
    "1000000080005 3003300330033003 SET 3001 1 11 0 1",
    "1000000080005 3003300330033003 SET 3002 2 22 0 1",
    "1000000080005 3003300330033003 SET 3003 3 33 0 1",
    NULL}}
};

/**
 * lw6pil_suite_init
 *
 * @pilot: pilot to run test suite (out param)
 * @game_state: game state to run test suite (out param)
 * @game_struct: game struct to run test suite (out param)
 * @level: level to run test suite (out param)
 * @timestamp: timestamp used for pilot creation (should be "now")
 * 
 * Build the objects used by the test suite. The idea is to wrap all this
 * in a single function since it requires to be exactly the same every time
 * as the test suite is very pedantic about checksums.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6pil_suite_init (lw6pil_pilot_t ** pilot, lw6ker_game_state_t ** game_state,
		   lw6ker_game_struct_t ** game_struct,
		   lw6map_level_t ** level, int64_t timestamp)
{
  int ret = 0;

  (*level) = NULL;
  (*game_struct) = NULL;
  (*game_state) = NULL;
  (*pilot) = NULL;

  (*level) = lw6map_builtin_scale (_INIT_SCALE_PERCENT);
  if (*level)
    {
      (*game_struct) = lw6ker_game_struct_new (*level, NULL);
      if (*game_struct)
	{
	  (*game_state) = lw6ker_game_state_new (*game_struct, NULL);
	  if (*game_state)
	    {
	      (*pilot) =
		lw6pil_pilot_new (*game_state, _SEQ_0, timestamp, NULL);
	      if (*pilot)
		{
		  ret = 1;
		}
	    }
	}
    }

  if (!ret)
    {
      if (*pilot)
	{
	  lw6pil_pilot_free (*pilot);
	  (*pilot) = NULL;
	}
      if (*game_state)
	{
	  lw6ker_game_state_free (*game_state);
	  (*game_state) = NULL;
	}
      if (*game_struct)
	{
	  lw6ker_game_struct_free (*game_struct);
	  (*game_struct) = NULL;
	}
      if (*level)
	{
	  lw6map_free (*level);
	  (*level) = NULL;
	}
    }
  return ret;
}

/**
 * lw6pil_suite_get_seq_0
 *
 * Get the base seq_0 for the reference test suite.
 *
 * Return value: 64-bit integer.
 */
int64_t
lw6pil_suite_get_seq_0 ()
{
  return _SEQ_0;
}

/**
 * lw6pil_suite_get_command_by_node_index
 *
 * @node_index: index of the node (not its id)
 * @stage: major stage of the test suite
 * @step: minor step of the test suite
 *
 * Get the reference test suite message by node_index, stage and step.
 *
 * Return value: static string, must not be freed.
 */
const char *
lw6pil_suite_get_command_by_node_index (int node_index, int stage, int step)
{
  const char *ret = NULL;
  const char **commands = NULL;
  int i = 0;

  if (node_index >= 0 && node_index < LW6PIL_SUITE_NB_NODES && stage >= 0
      && stage < LW6PIL_SUITE_NB_STAGES)
    {
      commands = _commands[node_index][stage];
      while (i <= step && commands[i])
	{
	  ret = commands[i];
	  ++i;
	}
      if (i <= step)
	{
	  ret = NULL;
	}
    }

  return ret;
}

int
_command_sort (const void *arg_a, const void *arg_b)
{
  int ret = 0;
  const char *command_a = (const char *) (*((const char **) arg_a));
  const char *command_b = (const char *) (*((const char **) arg_b));

  ret = strcmp (command_a, command_b);

  return ret;
}

/**
 * lw6pil_suite_get_command_by_stage
 *
 * @stage: major stage of the test suite
 * @step: minor step of the test suite
 *
 * Get the reference test suite message by stage and step.
 * Messages from various nodes are mixed and sorted.
 *
 * Return value: static string, must not be freed.
 */
const char *
lw6pil_suite_get_command_by_stage (int stage, int step)
{
  const char *ret = NULL;
  const char **commands = NULL;
  int node_index = 0;
  int i = 0, j = 0;
  int nb_commands = 0;
  const char **sorted_commands = NULL;

  if (stage >= 0 && stage < LW6PIL_SUITE_NB_STAGES)
    {
      for (node_index = 0; node_index < LW6PIL_SUITE_NB_NODES; ++node_index)
	{
	  i = 0;
	  commands = _commands[node_index][stage];
	  while (commands[i])
	    {
	      ++i;
	      ++nb_commands;
	    }
	}
      if (step < nb_commands)
	{
	  sorted_commands =
	    (const char **) LW6SYS_MALLOC ((nb_commands + 1) *
					   sizeof (char *));
	  if (sorted_commands)
	    {
	      j = 0;
	      for (node_index = 0; node_index < LW6PIL_SUITE_NB_NODES;
		   ++node_index)
		{
		  i = 0;
		  commands = _commands[node_index][stage];
		  while (commands[i])
		    {
		      sorted_commands[j] = commands[i];
		      ++j;
		      ++i;
		    }
		}
	      if (j == nb_commands)
		{
		  sorted_commands[j] = NULL;
		  /*
		   * Yes, we sort the array *each* time a call is done to this
		   * function. But it's not a real problem, this is just for
		   * testing, never used within real games.
		   */
		  qsort (sorted_commands, nb_commands, sizeof (char *),
			 _command_sort);
		}
	      ret = sorted_commands[step];
	      LW6SYS_FREE (sorted_commands);
	    }
	}
    }

  return ret;
}

/**
 * lw6pil_suite_get_command_by_step
 *
 * @step: minor step of the test suite
 *
 * Get the reference test suite message by step.
 * Messages from various nodes and stages are mixed and sorted.
 *
 * Return value: static string, must not be freed.
 */
const char *
lw6pil_suite_get_command_by_step (int step)
{
  const char *ret = NULL;
  const char **commands = NULL;
  int node_index = 0;
  int stage = 0;
  int i = 0, j = 0;
  int nb_commands = 0;
  const char **sorted_commands = NULL;

  for (node_index = 0; node_index < LW6PIL_SUITE_NB_NODES; ++node_index)
    {
      for (stage = 0; stage < LW6PIL_SUITE_NB_STAGES; ++stage)
	{
	  i = 0;
	  commands = _commands[node_index][stage];
	  while (commands[i])
	    {
	      ++i;
	      ++nb_commands;
	    }
	}
    }
  if (step < nb_commands)
    {
      sorted_commands =
	(const char **) LW6SYS_MALLOC ((nb_commands + 1) * sizeof (char *));
      if (sorted_commands)
	{
	  j = 0;
	  for (node_index = 0; node_index < LW6PIL_SUITE_NB_NODES;
	       ++node_index)
	    {
	      for (stage = 0; stage < LW6PIL_SUITE_NB_STAGES; ++stage)
		{
		  i = 0;
		  commands = _commands[node_index][stage];
		  while (commands[i])
		    {
		      sorted_commands[j] = commands[i];
		      ++i;
		      ++j;
		    }
		}
	    }
	  if (j == nb_commands)
	    {
	      sorted_commands[j] = NULL;
	      /*
	       * Yes, we sort the array *each* time a call is done to this
	       * function. But it's not a real problem, this is just for
	       * testing, never used within real games.
	       */
	      qsort (sorted_commands, nb_commands, sizeof (char *),
		     _command_sort);
	    }
	  ret = sorted_commands[step];
	  LW6SYS_FREE (sorted_commands);
	}
    }

  return ret;
}

/**
 * lw6pil_suite_get_checkpoint
 *
 * @checksum: expected checksum for the given checkpoint (out param)
 * @seq: expected seq for the given checkpoint (out param)
 * @round: expected round for the given checkpoint (out param)
 * @stage: stage to query checksum and other info about
 *
 * Gives the values which are expected for a given checkpoint.
 * If the right messages have been feeded, then these values
 * are expected.
 *
 * Return value: none, everything in out params
 */
void
lw6pil_suite_get_checkpoint (u_int32_t * checksum, int64_t * seq, int *round,
			     int stage)
{
  switch (stage)
    {
    case LW6PIL_SUITE_STAGE_1:
      (*checksum) = _STAGE_1_CHECKSUM;
      (*seq) = _STAGE_1_ROUND + _SEQ_0;
      (*round) = _STAGE_1_ROUND;
      break;
    case LW6PIL_SUITE_STAGE_2:
      (*checksum) = _STAGE_1_CHECKSUM;
      (*seq) = _STAGE_1_ROUND + _SEQ_0;
      (*round) = _STAGE_1_ROUND;
      break;
    case LW6PIL_SUITE_STAGE_3:
      (*checksum) = _STAGE_1_CHECKSUM;
      (*seq) = _STAGE_1_ROUND + _SEQ_0;
      (*round) = _STAGE_1_ROUND;
      break;
    default:
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("invalid stage %d"), stage);
      (*checksum) = 0;
      (*seq) = 0LL;
      (*round) = 0;
    }
}
