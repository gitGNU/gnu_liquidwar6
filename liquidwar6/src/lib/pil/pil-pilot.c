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

/*
 * This is used to stamp pilots as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

_lw6pil_pilot_t *
_lw6pil_pilot_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int64_t seq_0, int64_t timestamp, lw6sys_progress_t * progress)
{
  _lw6pil_pilot_t *ret = NULL;
  int ok = 0;
  lw6sys_progress_t progress_level;
  lw6sys_progress_t progress_game_struct;
  lw6sys_progress_t progress_game_state;
  lw6sys_progress_t progress_reference;
  lw6sys_progress_t progress_draft;

  lw6sys_progress_split5 (sys_context, &progress_level, &progress_game_struct, &progress_game_state, &progress_reference, &progress_draft, progress);

  ret = (_lw6pil_pilot_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6pil_pilot_t));
  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}
      ret->last_commit_seq = -1L;
      ret->last_sync_draft_from_reference_seq = -1L;
      ret->round_0 = lw6ker_game_state_get_rounds (sys_context, game_state);
      ret->seq_0 = seq_0;
      _lw6pil_pilot_calibrate (sys_context, ret, timestamp, seq_0 + lw6ker_game_state_get_rounds (sys_context, game_state));
      ret->replay = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6pil_command_free);
      if (ret->replay)
	{
	  ret->verified_queue = lw6sys_list_new (sys_context, lw6sys_free_callback);
	  if (ret->verified_queue)
	    {
	      ret->unverified_queue = lw6sys_list_new (sys_context, lw6sys_free_callback);
	      if (ret->unverified_queue)
		{
		  ret->level = lw6map_dup (sys_context, game_state->game_struct->level, &progress_level);
		  if (ret->level)
		    {
		      ret->game_struct = lw6ker_game_struct_dup (sys_context, game_state->game_struct, &progress_game_struct);
		      if (ret->game_struct)
			{
			  if (ret->level)
			    {
			      ret->backup = lw6ker_game_state_dup (sys_context, game_state, &progress_game_state);
			      if (ret->backup)
				{
				  /*
				   * These are absolutely mandatory, the whole point
				   * in duping game_struct and level is to be 100% sure
				   * the spawned threads will never point on freed objects.
				   * So, we need to dup *and* manually tell "OK, now you're
				   * pointing on this new safe object, not the old one".
				   */
				  lw6ker_game_struct_point_to (sys_context, ret->game_struct, ret->level);
				  lw6ker_game_state_point_to (sys_context, ret->backup, ret->game_struct);
				  if (_lw6pil_worker_init (sys_context, &(ret->reference), ret->backup, 1, &progress_reference))
				    {
				      if (_lw6pil_worker_init (sys_context, &(ret->draft), ret->backup, 0, &progress_draft))
					{
					  ok = 1;
					}
				    }
				  else
				    {
				      _lw6pil_worker_quit (sys_context, &(ret->reference));
				    }
				}
			    }
			}
		    }
		}
	    }
	}
      if (!ok)
	{
	  if (ret->backup)
	    {
	      lw6ker_game_state_free (sys_context, ret->backup);
	    }
	  if (ret->game_struct)
	    {
	      lw6ker_game_struct_free (sys_context, ret->game_struct);
	    }
	  if (ret->level)
	    {
	      lw6map_free (sys_context, ret->level);
	    }
	  if (ret->unverified_queue)
	    {
	      lw6sys_list_free (sys_context, ret->unverified_queue);
	    }
	  if (ret->verified_queue)
	    {
	      lw6sys_list_free (sys_context, ret->verified_queue);
	    }
	  if (ret->replay)
	    {
	      lw6sys_list_free (sys_context, ret->replay);
	    }
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6pil_pilot_new
 *
 * @sys_context: global system context
 * @game_state: the game state we're going to work on
 * @seq_0: the start sequence to use, that is, the seq at round=0
 * @timestamp: the current ticks (1000 ticks per sec, used to calibrate)
 * @progress: object used to show the advancement of the process
 *
 * Initializes a 'pilot' object, this object is responsible for
 * interpreting messages, transform them into low-level 'ker' module
 * function calls, and handle all the thread-spooky stuff.
 *
 * Return value: a working pilot object. May be NULL on memory failure.
 */
lw6pil_pilot_t *
lw6pil_pilot_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int64_t seq_0, int64_t timestamp, lw6sys_progress_t * progress)
{
  return (lw6pil_pilot_t *) _lw6pil_pilot_new (sys_context, game_state, seq_0, timestamp, progress);
}

void
_lw6pil_pilot_free (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot)
{
  if (pilot)
    {
      _lw6pil_worker_quit (sys_context, &(pilot->draft));
      _lw6pil_worker_quit (sys_context, &(pilot->reference));
      lw6map_free (sys_context, pilot->level);
      lw6ker_game_struct_free (sys_context, pilot->game_struct);
      lw6ker_game_state_free (sys_context, pilot->backup);
      lw6sys_list_free (sys_context, pilot->unverified_queue);
      lw6sys_list_free (sys_context, pilot->verified_queue);
      lw6sys_list_free (sys_context, pilot->replay);
      LW6SYS_FREE (sys_context, pilot);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL pilot"));
    }
}

/**
 * lw6pil_pilot_free
 *
 * @sys_context: global system context
 * @pilot: the object to free.
 *
 * Frees a 'pilot' object, note that this might involve joining
 * some threads, so it can 'take some time'.
 *
 * Return value: none.
 */
void
lw6pil_pilot_free (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot)
{
  _lw6pil_pilot_free (sys_context, (_lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_send_command (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, const char *command_text, int verified)
{
  int ret = 1;
  char *command_text_dup;

  command_text_dup = lw6sys_str_copy (sys_context, command_text);
  if (command_text_dup)
    {
      if (verified)
	{
	  lw6sys_list_push_front (sys_context, &(pilot->verified_queue), command_text_dup);
	}
      else
	{
	  lw6sys_list_push_front (sys_context, &(pilot->unverified_queue), command_text_dup);
	}
    }

  return ret;
}

/**
 * lw6pil_pilot_send_command
 *
 * @sys_context: global system context
 * @pilot: the object to send commands to.
 * @command_text: the text of the command, as received form network
 * @verified: wether we're sure this message is valid.
 *
 * Sends a command and handles it internally.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6pil_pilot_send_command (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, const char *command_text, int verified)
{
  return _lw6pil_pilot_send_command (sys_context, (_lw6pil_pilot_t *) pilot, command_text, verified);
}

int
_lw6pil_pilot_local_command (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, const char *command_text)
{
  int ret = 0;

  ret = lw6pil_command_execute_local_text (sys_context, &(pilot->local_cursors), command_text);

  return ret;
}

/**
 * lw6pil_pilot_local_command
 *
 * @sys_context: global system context
 * @pilot: the object to apply the local command on
 * @command_text: the command text
 *
 * This function is used to fix the annoying fact that by only sending
 * commands a limited number of times per sec to the game state, the
 * display always reflect an outdated position for cursors. But players
 * do not want to see this, they want to see the cursor in the right place.
 * So what we do is that the pilot can process "local" commands which have
 * absolutely no effect on the game but simply update a local cursor state,
 * only used for display. It's here in the pil module for it's where the
 * command interpreting code is, and the fact that there's this lag is
 * directly linked with the pilot way of doing things.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6pil_pilot_local_command (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, const char *command_text)
{
  return _lw6pil_pilot_local_command (sys_context, (_lw6pil_pilot_t *) pilot, command_text);
}

static void
_sync_draft_from_reference (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot)
{
  if (_lw6pil_pilot_get_reference_current_seq (sys_context, pilot) > pilot->last_sync_draft_from_reference_seq)
    {
      LW6SYS_MUTEX_LOCK (sys_context, pilot->draft.compute_mutex);
      LW6SYS_MUTEX_LOCK (sys_context, pilot->reference.compute_mutex);

      if (_lw6pil_pilot_get_reference_current_seq (sys_context, pilot) > pilot->last_sync_draft_from_reference_seq)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("pilot sync round=%d"), lw6ker_game_state_get_rounds (sys_context, pilot->reference.game_state));
	  lw6ker_game_state_sync (sys_context, pilot->draft.game_state, pilot->reference.game_state);
	  pilot->draft.target_round = pilot->reference.target_round;
	  pilot->draft.current_round = pilot->reference.current_round;

	  pilot->last_sync_draft_from_reference_seq = _lw6pil_pilot_get_reference_current_seq (sys_context, pilot);
	}

      LW6SYS_MUTEX_UNLOCK (sys_context, pilot->reference.compute_mutex);
      LW6SYS_MUTEX_UNLOCK (sys_context, pilot->draft.compute_mutex);
    }
}

static int
_commit_reference (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, _lw6pil_pilot_t * pilot)
{
  int ret = 1;
  char *command_text = NULL;
  lw6pil_command_t *command = NULL;
  int min_round = -1;
  int max_round = 0;
  int last_commit_round = 0;
  lw6sys_list_t *bulk_list = NULL;

  /*
   * Process the commands for the reference threads.
   */
  if (!lw6sys_list_is_empty (sys_context, pilot->verified_queue))
    {
      bulk_list = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6pil_command_free);
      while (pilot->verified_queue && ((command_text = lw6sys_list_pop_front (sys_context, &(pilot->verified_queue))) != NULL) && (bulk_list != NULL))
	{
	  command = lw6pil_command_new (sys_context, command_text, pilot->seq_0, pilot->round_0);
	  if (command)
	    {
	      if (min_round < 0)
		{
		  min_round = command->round;
		}
	      else
		{
		  min_round = lw6sys_imax (command->round, min_round);
		}
	      max_round = lw6sys_imax (command->round, max_round);
	      lw6sys_list_push_front (sys_context, &bulk_list, command);
	    }
	  LW6SYS_FREE (sys_context, command_text);
	}
      if (bulk_list)
	{
	  lw6sys_list_r_transfer_from (sys_context, pilot->reference.commands, &bulk_list);
	}
      if (bulk_list)
	{
	  LW6SYS_FREE (sys_context, bulk_list);
	  bulk_list = NULL;
	}
      if (pilot->verified_queue)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("verified_queue should be NULL, but isn't"));
	}
      pilot->verified_queue = lw6sys_list_new (sys_context, lw6sys_free_callback);

      if (min_round >= 0)
	{
	  last_commit_round = _lw6pil_pilot_seq2round (sys_context, pilot, pilot->last_commit_seq);
	  if (min_round <= last_commit_round)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("possible game inconsistency, min_round=%d last_commit_round=%d, seq_0=%"
			   LW6SYS_PRINTF_LL "d last_commmit_seq=%"
			   LW6SYS_PRINTF_LL "d"), min_round, last_commit_round, (long long) pilot->seq_0, (long long) pilot->last_commit_seq);
	    }
	}
      if (max_round > 0)
	{
	  pilot->last_commit_seq = _lw6pil_pilot_round2seq (sys_context, pilot, max_round);
	}
    }

  /*
   * Pump dump back to caller if needed, we only do it if dump
   * if set and test this before locking to avoid locking all the
   * time, after all, the operation that requires atomicity is
   * the exchange of data, and we won't die if we miss one turn,
   * actually, this is asynchronous anyway.
   */
  if (dump && lw6pil_dump_exists (sys_context, &(pilot->reference.dump)))
    {
      LW6SYS_MUTEX_LOCK (sys_context, pilot->reference.global_mutex);
      LW6SYS_MUTEX_LOCK (sys_context, pilot->reference.compute_mutex);
      /*
       * Yes, test again, now that we're locked
       */
      if (lw6pil_dump_exists (sys_context, &(pilot->reference.dump)))
	{
	  (*dump) = pilot->reference.dump;
	  lw6pil_dump_zero (sys_context, &(pilot->reference.dump));
	}
      LW6SYS_MUTEX_UNLOCK (sys_context, pilot->reference.compute_mutex);
      LW6SYS_MUTEX_UNLOCK (sys_context, pilot->reference.global_mutex);

      /*
       * Now pump any message left in queue, those already committed should
       * have been processed but as this is asynchronous the dump might
       * be from a *previous* commit so messages in this commit aren't in
       * the queue...
       */
      if (lw6pil_dump_exists (sys_context, dump))
	{
	  lw6sys_list_r_map (sys_context, pilot->reference.commands, _lw6pil_compute_pump_command_callback, dump->pilot);
	}
    }

  return ret;
}

static int
_commit_draft (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot)
{
  int ret = 1;
  char *command_text = NULL;
  lw6pil_command_t *command = NULL;
  lw6pil_command_t *command_dup = NULL;
  lw6sys_list_t *replay = NULL;
  int last_sync_draft_from_reference = 0;

  /*
   * Process the commands for the draft threads.
   */
  if (!lw6sys_list_is_empty (sys_context, pilot->unverified_queue))
    {
      /*
       * Add unverified events to replay list
       */
      while (pilot->unverified_queue && (command_text = lw6sys_list_pop_front (sys_context, &(pilot->unverified_queue))) != NULL)
	{
	  command = lw6pil_command_new (sys_context, command_text, pilot->seq_0, pilot->round_0);
	  if (command)
	    {
	      last_sync_draft_from_reference = _lw6pil_pilot_seq2round (sys_context, pilot, pilot->last_sync_draft_from_reference_seq);
	      if (command->round > last_sync_draft_from_reference && pilot->replay)
		{
		  lw6sys_list_push_front (sys_context, &(pilot->replay), command);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("ignoring unverified command \"%s\""), command->text);
		  lw6pil_command_free (sys_context, command);
		}
	    }
	  LW6SYS_FREE (sys_context, command_text);
	}
      if (pilot->unverified_queue)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unverified_queue should be NULL, but isn't"));
	}
      pilot->unverified_queue = lw6sys_list_new (sys_context, lw6sys_free_callback);

      /*
       * Now filter replay events, push them, and keep them for next time
       */
      if (!lw6sys_list_is_empty (sys_context, pilot->replay))
	{
	  _sync_draft_from_reference (sys_context, pilot);

	  replay = lw6sys_list_new (sys_context, (lw6sys_free_func_t) lw6pil_command_free);
	  if (replay)
	    {
	      while (pilot->replay && (command = (lw6pil_command_t *) lw6sys_list_pop_front (sys_context, &(pilot->replay))))
		{
		  last_sync_draft_from_reference = _lw6pil_pilot_seq2round (sys_context, pilot, pilot->last_sync_draft_from_reference_seq);
		  if (command->round > last_sync_draft_from_reference)
		    {
		      command_dup = lw6pil_command_dup (sys_context, command);
		      if (command_dup)
			{
			  lw6sys_list_push_front (sys_context, &replay, command_dup);
			}
		      lw6sys_list_r_push_front (sys_context, pilot->draft.commands, command);
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("draft anticipation of \"%s\""), command->text);
		    }
		  else
		    {
		      lw6pil_command_free (sys_context, command);
		    }
		}
	      if (pilot->replay)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("replay should be NULL, but isn't"));
		}
	      pilot->replay = replay;
	      replay = NULL;
	    }
	}
    }

  return ret;
}

int
_lw6pil_pilot_commit (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, _lw6pil_pilot_t * pilot)
{
  int ret = 1;

  lw6pil_dump_zero (sys_context, dump);
  ret = _commit_reference (sys_context, dump, pilot) && ret;
  ret = _commit_draft (sys_context, pilot) && ret;

  return ret;
}

/**
 * lw6pil_pilot_commit
 *
 * @sys_context: global system context
 * @dump: will contain the dump information if needed, can be NULL
 * @pilot: the object to commit.
 *
 * Commits all commands sent and actually send them to the
 * corresponding threads. This commit system allows better performance
 * by sending, for instance, all the commands for a given round together.
 *
 * Return value: none.
 */
int
lw6pil_pilot_commit (lw6sys_context_t * sys_context, lw6pil_dump_t * dump, lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_commit (sys_context, dump, (_lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_make_backup (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot)
{
  int ret = 0;

  /*
   * Before we start the backup, we want to make sure
   * the backup really corresponds to the right seq, so
   * we basically wait until reference is up-to-date.
   *
   * It's not as crucial as for a dump, but it can avoid
   * mysterious bugs, why on earth my backup was done
   * in the past, and why can't I find the backup I want?
   */
  while (_lw6pil_pilot_get_reference_current_seq (sys_context, pilot) < _lw6pil_pilot_get_last_commit_seq (sys_context, pilot))
    {
      lw6sys_idle (sys_context);
    }

  ret = _lw6pil_pilot_sync_from_reference (sys_context, pilot->backup, pilot);

  return ret;
}

/**
 * lw6pil_pilot_make_backup
 *
 * @sys_context: global system context
 * @pilot: the object to perform the backup on
 *
 * Makes a new backup in the pilot, that is, copy 'reference'
 * to 'backup'.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6pil_pilot_make_backup (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_make_backup (sys_context, (_lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_can_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot)
{
  int ret = 1;

  ret = ret && lw6ker_game_state_can_sync (sys_context, target, pilot->backup);
  ret = ret && lw6ker_game_state_can_sync (sys_context, target, pilot->reference.game_state);
  ret = ret && lw6ker_game_state_can_sync (sys_context, target, pilot->draft.game_state);

  return ret;
}

/**
 * lw6pil_pilot_can_sync
 *
 * @sys_context: global system context
 * @target: the target game_state we would sync on
 * @pilot: the object to perform the backup on
 *
 * Tests wether sync functions are callable with a given game state.
 * It verifies if the internal game_state and the target look the same.
 *
 * Return value: 1 if sync functions can be called, 0 if not.
 */
int
lw6pil_pilot_can_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_can_sync (sys_context, target, (_lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_sync_from_backup (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sync from backup round=%d"), lw6ker_game_state_get_rounds (sys_context, pilot->backup));
  ret = lw6ker_game_state_sync (sys_context, target, pilot->backup);

  return ret;
}

/**
 * lw6pil_pilot_sync_from_backup
 *
 * @sys_context: global system context
 * @target: the game_state structure which will get the informations.
 * @pilot: the object to get informations from.
 *
 * Gets the backup from the pilot object. This is the last snapshot
 * taken by @make_backup or, by default, the game_state the pilot
 * was constructed with.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6pil_pilot_sync_from_backup (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_sync_from_backup (sys_context, target, (_lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_sync_from_reference (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot)
{
  int ret = 0;

  LW6SYS_MUTEX_LOCK (sys_context, pilot->reference.global_mutex);

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sync from reference round=%d"), lw6ker_game_state_get_rounds (sys_context, pilot->reference.game_state));
  ret = lw6ker_game_state_sync (sys_context, target, pilot->reference.game_state);

  LW6SYS_MUTEX_UNLOCK (sys_context, pilot->reference.global_mutex);

  return ret;
}

/**
 * lw6pil_pilot_sync_from_reference
 *
 * @sys_context: global system context
 * @target: the game_state structure which will get the informations.
 * @pilot: the object to get informations from.
 *
 * Gets the latest reference game_state, that is, a stable snapshot
 * of the game, with no inconsistency, a game position that exists
 * and that we can rely on. Note that getting this can take time
 * since a global mutex is required, and computations must end
 * before you get the data.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6pil_pilot_sync_from_reference (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_sync_from_reference (sys_context, target, (_lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_sync_from_draft (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, _lw6pil_pilot_t * pilot, int dirty_read)
{
  int ret = 0;
  int max_round;
  int draft_rounds;
  int reference_rounds;
  int current_round;

  current_round = lw6ker_game_state_get_rounds (sys_context, target);

  max_round = _lw6pil_pilot_seq2round (sys_context, pilot, _lw6pil_pilot_get_max_seq (sys_context, pilot));
  if (max_round > current_round)
    {
      if (dirty_read == LW6PIL_DIRTY_READ_NEVER)
	{
	  LW6SYS_MUTEX_LOCK (sys_context, pilot->draft.compute_mutex);
	  LW6SYS_MUTEX_LOCK (sys_context, pilot->reference.compute_mutex);
	}
      max_round = _lw6pil_pilot_seq2round (sys_context, pilot, _lw6pil_pilot_get_max_seq (sys_context, pilot));
      if (max_round > current_round)
	{
	  draft_rounds = lw6ker_game_state_get_rounds (sys_context, pilot->draft.game_state);
	  reference_rounds = lw6ker_game_state_get_rounds (sys_context, pilot->reference.game_state);
	  if (draft_rounds > reference_rounds)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sync from draft round=%d/%d"), draft_rounds, reference_rounds);
	      ret = lw6ker_game_state_sync (sys_context, target, pilot->draft.game_state);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("sync from draft using reference round=%d/%d"), reference_rounds, draft_rounds);
	      ret = lw6ker_game_state_sync (sys_context, target, pilot->reference.game_state);
	    }
	}
      else
	{
	  ret = 1;
	}
      if (dirty_read == LW6PIL_DIRTY_READ_NEVER)
	{
	  LW6SYS_MUTEX_UNLOCK (sys_context, pilot->reference.compute_mutex);
	  LW6SYS_MUTEX_UNLOCK (sys_context, pilot->draft.compute_mutex);
	}
    }
  else
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6pil_pilot_sync_from_draft
 *
 * @sys_context: global system context
 * @target: the game_state structure which will get the informations.
 * @pilot: the object to get informations from.
 * @dirty_read: wether to allow dirty read or not
 *
 * Gets the informations from the pilot object, not being worried about
 * game consistency, this one will just return the latest version available.
 * It might even be in an inconsistent state, the position could reflect
 * a position which will never exist. Still, the data returned will not
 * correspond to a half-spread or half-moved game_state if dirty_read
 * is set to 0. In this case the data has at least some basic consistency
 * and getting this does require some mutex lock, however wait time should
 * be fairly small (max. a round). But, in a general manner, this function
 * is only used for display, and we do not care much if there's a small
 * glitch, we prefer fast & smooth display.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6pil_pilot_sync_from_draft (lw6sys_context_t * sys_context, lw6ker_game_state_t * target, lw6pil_pilot_t * pilot, int dirty_read)
{
  return _lw6pil_pilot_sync_from_draft (sys_context, target, (_lw6pil_pilot_t *) pilot, dirty_read);
}

lw6ker_game_state_t *
_lw6pil_pilot_dirty_read (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot)
{
  lw6ker_game_state_t *ret = NULL;
  int draft_rounds = 0;
  int reference_rounds = 0;

  draft_rounds = lw6ker_game_state_get_rounds (sys_context, pilot->draft.game_state);
  reference_rounds = lw6ker_game_state_get_rounds (sys_context, pilot->reference.game_state);
  if (draft_rounds > reference_rounds)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("dirty read using draft round=%d/%d"), draft_rounds, reference_rounds);
      ret = pilot->draft.game_state;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("dirty read using reference round=%d/%d"), reference_rounds, draft_rounds);
      ret = pilot->reference.game_state;
    }

  return ret;
}

/**
 * lw6pil_pilot_dirty_read
 *
 * @sys_context: global system context
 * @pilot: the object to get informations from.
 *
 * Returns a direct access to the most up-to-date game_state, without
 * locking anything whatsoever. This is clearly to implement a dirty read
 * mode as the name of the function suggests.
 *
 * Return value: 1 if OK, 0 if not.
 */
lw6ker_game_state_t *
lw6pil_pilot_dirty_read (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_dirty_read (sys_context, (_lw6pil_pilot_t *) pilot);
}

char *
_lw6pil_pilot_repr (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  char *ret = NULL;

  if (pilot)
    {
      ret =
	lw6sys_new_sprintf
	(sys_context, "%u (%dx%dx%d commmit_seq=%" LW6SYS_PRINTF_LL "d, current_seq=%"
	 LW6SYS_PRINTF_LL "d)", pilot->id,
	 lw6ker_game_state_get_w (sys_context, pilot->backup),
	 lw6ker_game_state_get_h (sys_context, pilot->backup),
	 lw6ker_game_state_get_d (sys_context, pilot->backup),
	 (long long) _lw6pil_pilot_get_last_commit_seq (sys_context, pilot), (long long) _lw6pil_pilot_get_reference_current_seq (sys_context, pilot));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't generate string id for NULL pilot"));
    }

  return ret;
}

/**
 * lw6pil_pilot_repr
 *
 * @sys_context: global system context
 * @pil: the pilot to describe
 *
 * Returns a string describing the pilot. This is a very short description,
 * use it for logs, and to debug stuff. By no means it's a complete exhaustive
 * description. Still, the string returned should be unique.
 *
 * Return value: a dynamically allocated string.
 */
char *
lw6pil_pilot_repr (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_repr (sys_context, (_lw6pil_pilot_t *) pilot);
}

void
_lw6pil_pilot_calibrate (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int64_t timestamp, int64_t seq)
{
  pilot->calibrate_timestamp = timestamp;
  pilot->calibrate_seq = seq;
}

/**
 * lw6pil_pilot_calibrate
 *
 * @sys_context: global system context
 * @pilot: the object to calibrate
 * @timestamp: the current ticks setting (1000 ticks per second)
 * @seq: the round expected to be returned with this ticks value
 *
 * Calibrates the pilot, that is, initializes it so that subsequent calls
 * to @lw6pil_pilot_get_round return consistent values.
 *
 * Return value: none.
 */
void
lw6pil_pilot_calibrate (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int64_t timestamp, int64_t seq)
{
  return _lw6pil_pilot_calibrate (sys_context, (_lw6pil_pilot_t *) pilot, timestamp, seq);
}

void
_lw6pil_pilot_speed_up (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int seq_inc)
{
  pilot->calibrate_seq += seq_inc;
}

/**
 * lw6pil_pilot_speed_up
 *
 * @sys_context: global system context
 * @pilot: the pilot to speed up
 * @seq_inc: the number of seqs
 *
 * Re-calibrates the pilot so that it speeds up a bit.
 * This will basically increase next_seq by seq_inc.
 *
 * Return value: none.
 */
void
lw6pil_pilot_speed_up (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int seq_inc)
{
  _lw6pil_pilot_speed_up (sys_context, (_lw6pil_pilot_t *) pilot, seq_inc);
}

void
_lw6pil_pilot_slow_down (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int seq_dec)
{
  pilot->calibrate_seq -= seq_dec;
}

/**
 * lw6pil_pilot_slow_down
 *
 * @sys_context: global system context
 * @pilot: the pilot to speed up
 * @seq_dec: the number of seqs
 *
 * Re-calibrates the pilot so that it slows down a bit.
 * This will basically decrease next_seq by seq_inc.
 *
 * Return value: none.
 */
void
lw6pil_pilot_slow_down (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int seq_dec)
{
  _lw6pil_pilot_slow_down (sys_context, (_lw6pil_pilot_t *) pilot, seq_dec);
}

int
_lw6pil_pilot_get_round_0 (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  return pilot->round_0;
}

/**
 * lw6pil_pilot_get_round_0
 *
 * @sys_context: global system context
 * @pilot: pilot object to query
 *
 * Get the initial round (the one passed at object construction) which
 * says what the round was at object creation, it's just an offset.
 *
 * Return value: 64-bit integer
 */
int
lw6pil_pilot_get_round_0 (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_round_0 (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int64_t
_lw6pil_pilot_get_seq_0 (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  return pilot->seq_0;
}

/**
 * lw6pil_pilot_get_seq_0
 *
 * @sys_context: global system context
 * @pilot: pilot object to query
 *
 * Get the initial seq (the one passed at object construction) which
 * says what the seq was at round=0, it's just an offset.
 *
 * Return value: 64-bit integer
 */
int64_t
lw6pil_pilot_get_seq_0 (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_seq_0 (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_seq2round (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, int64_t seq)
{
  return (seq >= _LW6PIL_MIN_SEQ_0 ? ((seq - _lw6pil_pilot_get_seq_0 (sys_context, pilot)) + _lw6pil_pilot_get_round_0 (sys_context, pilot)) : -1);
}

/**
 * lw6pil_pilot_seq2round
 *
 * @sys_context: global system context
 * @pilot: pilot object to work on
 * @seq: the seq to convert
 *
 * Converts a seq (64-bit) to a round (32-bit). 64-bit seqs are used
 * to avoid out-of-range errors on very long games, OTOH a round is 32-bit
 * to garantee the atomicity of its affection, even on platforms which
 * are not native 64-bit.
 *
 * Return value: the round (32-bit integer)
 */
int
lw6pil_pilot_seq2round (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, int64_t seq)
{
  return _lw6pil_pilot_seq2round (sys_context, (const _lw6pil_pilot_t *) pilot, seq);
}

int64_t
_lw6pil_pilot_round2seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, int round)
{
  return round + _lw6pil_pilot_get_seq_0 (sys_context, pilot) - _lw6pil_pilot_get_round_0 (sys_context, pilot);
}

/**
 * lw6pil_pilot_round2seq
 *
 * @sys_context: global system context
 * @pilot: pilot object to work on
 * @round: the round to convert
 *
 * Converts a round (32-bit) to a seq (64-bit). 64-bit seqs are used
 * to avoid out-of-range errors on very long games, OTOH a round is 32-bit
 * to garantee the atomicity of its affection, even on platforms which
 * are not native 64-bit.
 *
 * Return value: the seq (64-bit integer)
 */
int64_t
lw6pil_pilot_round2seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, int round)
{
  return _lw6pil_pilot_round2seq (sys_context, (_lw6pil_pilot_t *) pilot, round);
}

int64_t
_lw6pil_pilot_get_next_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, int64_t timestamp)
{
  int64_t next_seq = 0LL;
  int64_t last_commit_seq = 0LL;
  int64_t delta;

  delta = timestamp - pilot->calibrate_timestamp;
  delta *= (int64_t) pilot->backup->game_struct->rules.rounds_per_sec;
  delta /= LW6SYS_TICKS_PER_SEC;
  next_seq = pilot->calibrate_seq + (int) delta;

  /*
   * Now, most of the time, the value at this stage is OK,
   * but when processing long operations and/or when the command
   * queue is really filled, the pilot might fall behind, in that
   * case, we use at least last_commit_seq + 1
   */
  last_commit_seq = _lw6pil_pilot_get_last_commit_seq (sys_context, pilot);
  if (next_seq <= last_commit_seq)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("ideal next_seq=%" LW6SYS_PRINTF_LL
		       "d but last_commit_seq=%" LW6SYS_PRINTF_LL "d, forcing greater value"), (long long) next_seq, (long long) last_commit_seq);
      next_seq = last_commit_seq + 1LL;
    }

  return next_seq;
}

/**
 * lw6pil_pilot_get_next_seq
 *
 * @sys_context: global system context
 * @pilot: the object to query
 * @timestamp: the current ticks setting (1000 ticks per second)
 *
 * Returns the seq one should use to generate new events/commands
 * at a given time (given in ticks).
 *
 * Return value: none.
 */
int64_t
lw6pil_pilot_get_next_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, int64_t timestamp)
{
  return _lw6pil_pilot_get_next_seq (sys_context, (const _lw6pil_pilot_t *) pilot, timestamp);
}

int64_t
_lw6pil_pilot_get_last_commit_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  int64_t ret = 0;

  ret = lw6sys_llmax (pilot->last_commit_seq, _LW6PIL_MIN_SEQ_0);

  return ret;
}

/**
 * lw6pil_pilot_get_last_commit_seq
 *
 * @sys_context: global system context
 * @pilot: the object to query
 *
 * Returns the seq of the last commit (reference game_state) for
 * this object.
 *
 * Return value: the commit seq (reference object)
 */
int64_t
lw6pil_pilot_get_last_commit_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_last_commit_seq (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int64_t
_lw6pil_pilot_get_reference_target_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  int64_t ret = 0;

  ret = _lw6pil_pilot_round2seq (sys_context, pilot, pilot->reference.target_round);

  return ret;
}

/**
 * lw6pil_pilot_get_reference_target_seq
 *
 * @sys_context: global system context
 * @pilot: the object to query
 *
 * Returns the seq which is targetted in the reference game_state,
 * this is 'how far computation will go in the reference game_state
 * if no new commands are issued'. Note that there can always
 * be some commands which are not yet processed, so you should not
 * rely on this too heavily, however it gives a good idea of how
 * things are going.
 *
 * Return value: the target seq (reference object)
 */
int64_t
lw6pil_pilot_get_reference_target_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_reference_target_seq (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int64_t
_lw6pil_pilot_get_reference_current_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  int64_t ret = 0;

  ret = _lw6pil_pilot_round2seq (sys_context, pilot, pilot->reference.current_round);

  return ret;
}

/**
 * lw6pil_pilot_get_reference_current_seq
 *
 * @sys_context: global system context
 * @pilot: the object to query
 *
 * Returns the current seq in the reference game_state. There's no
 * lock on this call so don't rely on this too heavily, it just
 * gives you an idea of wether the pilot is very late on its
 * objectives or just on time.
 *
 * Return value: the current seq (reference object)
 */
int64_t
lw6pil_pilot_get_reference_current_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_reference_current_seq (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int64_t
_lw6pil_pilot_get_max_seq (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  int64_t ret = 0;

  ret = _lw6pil_pilot_round2seq (sys_context, pilot, lw6sys_imax (pilot->reference.current_round, pilot->draft.current_round));

  return ret;
}

/**
 * lw6pil_pilot_get_max_seq
 *
 * @sys_context: global system context
 * @pilot: the object to query
 *
 * Returns the max current seq in the reference or draft
 * game states. No lock on this call so don't rely on this
 * too heavily, it just gives you an idea of computation state.
 *
 * Return value: the current seq (reference object)
 */
int64_t
lw6pil_pilot_get_max_seq (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_max_seq (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_is_over (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  int ret = 0;

  ret = pilot->reference.over ? 1 : 0;

  return ret;
}

/**
 * lw6pil_pilot_is_over
 *
 * @sys_context: global system context
 * @pilot: the object to query
 *
 * Tells wether the game is over or not.
 *
 * Return value: 1 if over, 0 if not
 */
int
lw6pil_pilot_is_over (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_is_over (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_did_cursor_win (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot, u_int16_t cursor_id)
{
  int ret = 0;

  if (pilot->reference.game_state)
    {
      ret = lw6ker_game_state_did_cursor_win (sys_context, pilot->reference.game_state, cursor_id);
    }

  return ret;
}

/**
 * lw6pil_pilot_did_cursor_win
 *
 * @sys_context: global system context
 * @pilot: the object to query
 * @cursor_id: the cursor_id concerned
 *
 * Tells wether a given cursor was winner or not.
 *
 * Return value: 1 if over, 0 if not
 */
int
lw6pil_pilot_did_cursor_win (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot, u_int16_t cursor_id)
{
  return _lw6pil_pilot_did_cursor_win (sys_context, (const _lw6pil_pilot_t *) pilot, cursor_id);
}

int
_lw6pil_pilot_get_winner (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  int ret = 0;

  if (pilot->reference.game_state)
    {
      ret = lw6ker_game_state_get_winner (sys_context, pilot->reference.game_state, LW6MAP_TEAM_COLOR_INVALID);
    }

  return ret;
}

/**
 * lw6pil_pilot_get_winner
 *
 * @sys_context: global system context
 * @pilot: the object to query
 *
 * Gets the winner color.
 *
 * Return value: a team color, -1 if no winner and/or error.
 */
int
lw6pil_pilot_get_winner (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_winner (sys_context, (const _lw6pil_pilot_t *) pilot);
}

int
_lw6pil_pilot_get_looser (lw6sys_context_t * sys_context, const _lw6pil_pilot_t * pilot)
{
  int ret = 0;

  if (pilot->reference.game_state)
    {
      ret = lw6ker_game_state_get_looser (sys_context, pilot->reference.game_state, LW6MAP_TEAM_COLOR_INVALID);
    }

  return ret;
}

/**
 * lw6pil_pilot_get_looser
 *
 * @sys_context: global system context
 * @pilot: the object to query
 *
 * Gets the looser color.
 *
 * Return value: a team color, -1 if no looser and/or error.
 */
int
lw6pil_pilot_get_looser (lw6sys_context_t * sys_context, const lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_looser (sys_context, (const _lw6pil_pilot_t *) pilot);
}

lw6pil_local_cursors_t *
_lw6pil_pilot_get_local_cursors (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot)
{
  return &(pilot->local_cursors);
}

/**
 * lw6pil_pilot_get_local_cursors
 *
 * @sys_context: global system context
 * @pilot: object to query
 *
 * Returns a pointer on the local_cursors struct used within
 * the object. Beware, this is the *real* pointer, not a copy...
 *
 * Return value: pointer on internal object
 */
lw6pil_local_cursors_t *
lw6pil_pilot_get_local_cursors (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot)
{
  return _lw6pil_pilot_get_local_cursors (sys_context, (_lw6pil_pilot_t *) pilot);
}

void
_lw6pil_pilot_checksum_log_set_interval (lw6sys_context_t * sys_context, _lw6pil_pilot_t * pilot, int checksum_log_interval)
{
  /*
   * It's not necessary to lock here, it's only a simple integer
   * affectation so should be fairly atomic
   */
  lw6ker_game_state_checksum_log_set_interval (sys_context, pilot->reference.game_state, checksum_log_interval);
  lw6ker_game_state_checksum_log_set_interval (sys_context, pilot->draft.game_state, checksum_log_interval);
}

/**
 * lw6pil_pilot_checksum_log_set_interval
 *
 * Debugging function used to set automatically an interval at which engine
 * will log a checksum automatically. This is typically to track down where
 * and when there starts to be a difference between two game_states that have
 * evolved separately. This function will propagate the parameter to all
 * the game_states handled by the pilot, each will log its informations
 * separately.
 *
 * @sys_context: global system context
 * @pilot: the pilot to track
 * @checksum_log_interval: dump interval, if 0, feature is disabled
 *
 * Return value: none
 */
void
lw6pil_pilot_checksum_log_set_interval (lw6sys_context_t * sys_context, lw6pil_pilot_t * pilot, int checksum_log_interval)
{
  _lw6pil_pilot_checksum_log_set_interval (sys_context, (_lw6pil_pilot_t *) pilot, checksum_log_interval);
}
