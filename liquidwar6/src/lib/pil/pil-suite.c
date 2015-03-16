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

#define _STAGE_1_GAME_STATE_CHECKSUM 0x3ce791b2
#define _STAGE_2_GAME_STATE_CHECKSUM 0x39db8636
#define _STAGE_3_GAME_STATE_CHECKSUM 0x94fae0bd
#define _STAGE_4_GAME_STATE_CHECKSUM 0x0e0efc50
#define _STAGE_5_GAME_STATE_CHECKSUM 0x6363d4eb
#define _STAGE_6_GAME_STATE_CHECKSUM 0xebfd020b

/*
 * There are 6 numbered stages, but they correspond, basically,
 * to only 3 logical stages, the idea is that stage_2 only contains
 * dummy nop messages to bump the queue and validate stage_1 messages
 */
#define _STAGE_1_ROUND 10005
#define _STAGE_2_ROUND 10006
#define _STAGE_3_ROUND 25005
#define _STAGE_4_ROUND 25006
#define _STAGE_5_ROUND 40005
#define _STAGE_6_ROUND 40006

#define _SEQ_0 1000000000000LL

#define _NODE_A_ID 0x1001100110011001LL
#define _NODE_B_ID 0x2002200220022002LL
#define _NODE_C_ID 0x3003300330033003LL

#define _MAX_MESSAGES_PER_NODE_AND_STAGE 20
#define _INIT_SCALE_PERCENT 33

/*
 * Note: it's important that last message round is less than
 * (LW6MAP_RULES_DEFAULT_TOTAL_TIME *
 * LW6MAP_RULES_DEFAULT_ROUNDS_PER_SEC *
 * LW6MAP_RULES_DEFAULT_MOVES_PER_ROUND)
 * which is something like 900 * 50 * 2 = 90000.
 */
static const char *_commands[LW6PIL_SUITE_NB_NODES][LW6PIL_SUITE_NB_STAGES][_MAX_MESSAGES_PER_NODE_AND_STAGE] = {
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
    "1000000005005 1001100110011001 SET 1001 1 1 1 0",
    "1000000005005 1001100110011001 SET 1002 2 2 1 0",
    "1000000005005 1001100110011001 SET 1003 3 3 1 0",
    "1000000005005 1001100110011001 SET 1004 4 4 1 0",
    "1000000010005 1001100110011001 SET 1001 1 1 0 1",
    "1000000010005 1001100110011001 SET 1002 2 2 0 1",
    "1000000010005 1001100110011001 SET 1003 3 3 0 1",
    "1000000010005 1001100110011001 SET 1004 4 4 0 1",
    NULL},
   /*
    * STAGE_2 messages
    */
   {"1000000010006 1001100110011001 NOP", NULL},
   /*
    * STAGE_3 messages
    */
   {
    "1000000010055 1001100110011001 SET 1001 5 5 0 0",
    "1000000010055 1001100110011001 SET 1002 6 6 0 0",
    "1000000010055 1001100110011001 SET 1003 7 7 0 0",
    "1000000010055 1001100110011001 SET 1003 8 8 0 0",
    "1000000015055 1001100110011001 SET 1001 5 5 1 0",
    "1000000015055 1001100110011001 SET 1002 6 6 1 0",
    "1000000015055 1001100110011001 SET 1003 7 7 1 0",
    "1000000015055 1001100110011001 SET 1003 8 8 1 0",
    "1000000025005 1001100110011001 NOP",
    NULL},
   /*
    * STAGE_4 messages
    */
   {"1000000025006 1001100110011001 NOP", NULL},
   /*
    * STAGE_5 messages
    */
   {
    "1000000025055 1001100110011001 SET 1001 1 1 0 0",
    "1000000025055 1001100110011001 SET 1002 2 2 0 0",
    "1000000025055 1001100110011001 SET 1003 3 3 0 0",
    "1000000025055 1001100110011001 SET 1003 4 4 0 0",
    "1000000030055 1001100110011001 SET 1001 1 1 0 1",
    "1000000030055 1001100110011001 SET 1002 2 2 0 1",
    "1000000030055 1001100110011001 SET 1003 3 3 0 1",
    "1000000030055 1001100110011001 SET 1003 3 3 0 1",
    "1000000040005 1001100110011001 NOP",
    NULL},
   /*
    * STAGE_6 messages
    */
   {"1000000040006 1001100110011001 NOP", NULL}
   },
  /*
   * NODE_B messages
   */
  {
   /*
    * STAGE_1 messages
    */
   {NULL},
   /*
    * STAGE_2 messages
    */
   {"1000000010006 2002200220022002 NOP",
    NULL},
   /*
    * STAGE_3 messages
    */
   {
    "1000000015005 2002200220022002 REGISTER",
    "1000000015005 2002200220022002 ADD 2001 CYAN",
    "1000000015005 2002200220022002 ADD 2002 MAGENTA",
    "1000000015005 2002200220022002 ADD 2003 ORANGE",
    "1000000020005 2002200220022002 SET 2001 11 11 1 0",
    "1000000020005 2002200220022002 SET 2002 22 22 1 0",
    "1000000020005 2002200220022002 SET 2003 33 33 1 0",
    "1000000025005 2002200220022002 SET 2001 11 11 0 1",
    "1000000025005 2002200220022002 SET 2002 22 22 0 1",
    "1000000025005 2002200220022002 SET 2003 33 33 0 1",
    NULL},
   /*
    * STAGE_4 messages
    */
   {"1000000025006 2002200220022002 NOP", NULL},
   /*
    * STAGE_5 messages
    */
   {
    "1000000025555 2002200220022002 SET 2001 44 44 1 0",
    "1000000025555 2002200220022002 SET 2002 55 55 1 0",
    "1000000025555 2002200220022002 SET 2003 66 66 1 0",
    "1000000030555 2002200220022002 SET 2001 11 11 0 1",
    "1000000030555 2002200220022002 SET 2002 22 22 0 1",
    "1000000030555 2002200220022002 SET 2003 33 33 0 1",
    "1000000040005 2002200220022002 NOP",
    NULL},
   /*
    * STAGE_6 messages
    */
   {"1000000040006 2002200220022002 NOP", NULL}
   },
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
   {NULL},
   /*
    * STAGE_4 messages
    */
   {"1000000025006 3003300330033003 NOP", NULL},
   /*
    * STAGE_5 messages
    */
   {
    "1000000030005 3003300330033003 REGISTER",
    "1000000030005 3003300330033003 ADD 3001 LIGHTBLUE",
    "1000000030005 3003300330033003 ADD 3002 PURPLE",
    "1000000030005 3003300330033003 ADD 3003 PINK",
    "1000000035005 3003300330033003 SET 3001 11 1 1 0",
    "1000000035005 3003300330033003 SET 3002 22 2 1 0",
    "1000000035005 3003300330033003 SET 3003 33 3 1 0",
    "1000000040005 3003300330033003 SET 3001 1 11 0 1",
    "1000000040005 3003300330033003 SET 3002 2 22 0 1",
    "1000000040005 3003300330033003 SET 3003 3 33 0 1",
    NULL},
   /*
    * STAGE_6 messages
    */
   {"1000000040006 3003300330033003 NOP", NULL}
   }
};

/**
 * lw6pil_suite_init
 *
 * @sys_context: global system context
 * @dump: dump to use as a base to init the suite (out param)
 * @timestamp: timestamp used for pilot creation (should be "now")
 *
 * Build the objects used by the test suite. The idea is to wrap all this
 * in a single function since it requires to be exactly the same every time
 * as the test suite is very pedantic about checksums.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6pil_suite_init (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, int64_t timestamp)
{
  int ret = 0;

  lw6pil_dump_zero (sys_context, dump);

  dump->level = lw6map_builtin_scale (sys_context, _INIT_SCALE_PERCENT);
  if (dump->level)
    {
      dump->game_struct = lw6ker_game_struct_new (sys_context, dump->level, NULL);
      if (dump->game_struct)
	{
	  dump->game_state = lw6ker_game_state_new (sys_context, dump->game_struct, NULL);
	  if (dump->game_state)
	    {
	      dump->pilot = lw6pil_pilot_new (sys_context, dump->game_state, _SEQ_0, timestamp, NULL);
	      if (dump->pilot)
		{
		  ret = 1;
		}
	    }
	}
    }

  if (!ret)
    {
      lw6pil_dump_clear (sys_context, dump);
    }
  return ret;
}

/**
 * lw6pil_suite_get_seq_0
 *
 * @sys_context: global system context
 *
 * Get the base seq_0 for the reference test suite.
 *
 * Return value: 64-bit integer.
 */
int64_t
lw6pil_suite_get_seq_0 (lw6sys_context_t * sys_context)
{
  return _SEQ_0;
}

/**
 * lw6pil_suite_get_node_id
 *
 * @sys_context: global system context
 * @node_index: index of the node we want informations about
 *
 * Get the node_id associated to an index, typically a 64-bit
 * unique. The index is just a simple integer which is 0 for node A,
 * 1 for node B, etc.
 *
 * Return value: the node id, as an unsigned 64-bit integer
 */
u_int64_t
lw6pil_suite_get_node_id (lw6sys_context_t * sys_context, int node_index)
{
  u_int64_t ret = 0LL;

  switch (node_index)
    {
    case LW6PIL_SUITE_NODE_A:
      ret = _NODE_A_ID;
      break;
    case LW6PIL_SUITE_NODE_B:
      ret = _NODE_B_ID;
      break;
    case LW6PIL_SUITE_NODE_C:
      ret = _NODE_C_ID;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid node_index %d"), node_index);
    }

  return ret;
}

/**
 * lw6pil_suite_get_command_by_node_index
 *
 * @sys_context: global system context
 * @node_index: index of the node (not its id)
 * @stage: major stage of the test suite
 * @step: minor step of the test suite
 *
 * Get the reference test suite message by node_index, stage and step.
 *
 * Return value: static string, must not be freed.
 */
const char *
lw6pil_suite_get_command_by_node_index (lw6sys_context_t * sys_context, int node_index, int stage, int step)
{
  const char *ret = NULL;
  const char **commands = NULL;
  int i = 0;

  if (node_index >= 0 && node_index < LW6PIL_SUITE_NB_NODES && stage >= 0 && stage < LW6PIL_SUITE_NB_STAGES)
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

static int
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
 * @sys_context: global system context
 * @stage: major stage of the test suite
 * @step: minor step of the test suite
 *
 * Get the reference test suite message by stage and step.
 * Messages from various nodes are mixed and sorted.
 *
 * Return value: static string, must not be freed.
 */
const char *
lw6pil_suite_get_command_by_stage (lw6sys_context_t * sys_context, int stage, int step)
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
	  sorted_commands = (const char **) LW6SYS_MALLOC (sys_context, (nb_commands + 1) * sizeof (char *));
	  if (sorted_commands)
	    {
	      j = 0;
	      for (node_index = 0; node_index < LW6PIL_SUITE_NB_NODES; ++node_index)
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
		  qsort (sorted_commands, nb_commands, sizeof (char *), _command_sort);
		}
	      ret = sorted_commands[step];
	      LW6SYS_FREE (sys_context, sorted_commands);
	    }
	}
    }

  return ret;
}

/**
 * lw6pil_suite_get_command_by_step
 *
 * @sys_context: global system context
 * @step: minor step of the test suite
 *
 * Get the reference test suite message by step.
 * Messages from various nodes and stages are mixed and sorted.
 *
 * Return value: static string, must not be freed.
 */
const char *
lw6pil_suite_get_command_by_step (lw6sys_context_t * sys_context, int step)
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
      sorted_commands = (const char **) LW6SYS_MALLOC (sys_context, (nb_commands + 1) * sizeof (char *));
      if (sorted_commands)
	{
	  j = 0;
	  for (node_index = 0; node_index < LW6PIL_SUITE_NB_NODES; ++node_index)
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
	      qsort (sorted_commands, nb_commands, sizeof (char *), _command_sort);
	    }
	  ret = sorted_commands[step];
	  LW6SYS_FREE (sys_context, sorted_commands);
	}
    }

  return ret;
}

/**
 * lw6pil_suite_get_checkpoint
 *
 * @sys_context: global system context
 * @game_state_checksum: expected checksum for the given checkpoint (out param)
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
lw6pil_suite_get_checkpoint (lw6sys_context_t * sys_context, u_int32_t * game_state_checksum, int64_t * seq, int *round, int stage)
{
  switch (stage)
    {
    case LW6PIL_SUITE_STAGE_1:
      (*game_state_checksum) = _STAGE_1_GAME_STATE_CHECKSUM;
      (*seq) = _STAGE_1_ROUND + _SEQ_0;
      (*round) = _STAGE_1_ROUND;
      break;
    case LW6PIL_SUITE_STAGE_2:
      (*game_state_checksum) = _STAGE_2_GAME_STATE_CHECKSUM;
      (*seq) = _STAGE_2_ROUND + _SEQ_0;
      (*round) = _STAGE_2_ROUND;
      break;
    case LW6PIL_SUITE_STAGE_3:
      (*game_state_checksum) = _STAGE_3_GAME_STATE_CHECKSUM;
      (*seq) = _STAGE_3_ROUND + _SEQ_0;
      (*round) = _STAGE_3_ROUND;
      break;
    case LW6PIL_SUITE_STAGE_4:
      (*game_state_checksum) = _STAGE_4_GAME_STATE_CHECKSUM;
      (*seq) = _STAGE_4_ROUND + _SEQ_0;
      (*round) = _STAGE_4_ROUND;
      break;
    case LW6PIL_SUITE_STAGE_5:
      (*game_state_checksum) = _STAGE_5_GAME_STATE_CHECKSUM;
      (*seq) = _STAGE_5_ROUND + _SEQ_0;
      (*round) = _STAGE_5_ROUND;
      break;
    case LW6PIL_SUITE_STAGE_6:
      (*game_state_checksum) = _STAGE_6_GAME_STATE_CHECKSUM;
      (*seq) = _STAGE_6_ROUND + _SEQ_0;
      (*round) = _STAGE_6_ROUND;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid stage %d"), stage);
      (*game_state_checksum) = 0;
      (*seq) = 0LL;
      (*round) = 0;
    }
}
