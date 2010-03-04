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
static u_int32_t seq_id = 0;

/**
 * lw6pil_pilot_new
 *
 * @game_state: the game state we're going to work on
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
lw6pil_pilot_new (lw6ker_game_state_t * game_state, int64_t timestamp,
		  lw6sys_progress_t * progress)
{
  lw6pil_pilot_t *ret = NULL;
  int ok = 0;
  lw6sys_progress_t progress_level;
  lw6sys_progress_t progress_game_struct;
  lw6sys_progress_t progress_game_state;
  lw6sys_progress_t progress_reference;
  lw6sys_progress_t progress_draft;

  lw6sys_progress_split5 (&progress_level, &progress_game_struct,
			  &progress_game_state, &progress_reference,
			  &progress_draft, progress);

  ret = (lw6pil_pilot_t *) LW6SYS_CALLOC (sizeof (lw6pil_pilot_t));
  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}
      ret->last_commit_round = -1;
      ret->last_sync_draft_from_reference = -1;
      lw6pil_pilot_calibrate (ret, timestamp,
			      lw6ker_game_state_get_rounds (game_state));

      ret->replay =
	lw6sys_list_new ((lw6sys_free_func_t) lw6pil_command_free);
      if (ret->replay)
	{
	  ret->verified_queue = lw6sys_list_new (lw6sys_free_callback);
	  if (ret->verified_queue)
	    {
	      ret->unverified_queue = lw6sys_list_new (lw6sys_free_callback);
	      if (ret->unverified_queue)
		{
		  ret->level =
		    lw6map_dup (game_state->game_struct->level,
				&progress_level);
		  if (ret->level)
		    {
		      ret->game_struct =
			lw6ker_game_struct_dup (game_state->game_struct,
						&progress_game_struct);
		      if (ret->game_struct)
			{
			  if (ret->level)
			    {
			      ret->backup =
				lw6ker_game_state_dup (game_state,
						       &progress_game_state);
			      if (ret->backup)
				{
				  /*
				   * These are absolutely mandatory, the whole point
				   * in duping game_struct and level is to be 100% sure
				   * the spawned threads will never point on freed objects.
				   * So, we need to dup *and* manually tell "OK, now you're
				   * pointing on this new safe object, not the old one".
				   */
				  lw6ker_game_struct_point_to
				    (ret->game_struct, ret->level);
				  lw6ker_game_state_point_to (ret->backup,
							      ret->
							      game_struct);
				  if (_lw6pil_worker_init
				      (&(ret->reference), ret->backup,
				       &progress_reference))
				    {
				      if (_lw6pil_worker_init
					  (&(ret->draft), ret->backup,
					   &progress_draft))
					{
					  ok = 1;
					}
				    }
				  else
				    {
				      _lw6pil_worker_quit (&(ret->reference));
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
	      lw6ker_game_state_free (ret->backup);
	    }
	  if (ret->game_struct)
	    {
	      lw6ker_game_struct_free (ret->game_struct);
	    }
	  if (ret->level)
	    {
	      lw6map_free (ret->level);
	    }
	  if (ret->unverified_queue)
	    {
	      lw6sys_list_free (ret->unverified_queue);
	    }
	  if (ret->verified_queue)
	    {
	      lw6sys_list_free (ret->verified_queue);
	    }
	  if (ret->replay)
	    {
	      lw6sys_list_free (ret->replay);
	    }
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6pil_pilot_free
 *
 * @pilot: the object to free.
 *
 * Frees a 'pilot' object, note that this might involve joining
 * some threads, so it can 'take some time'.
 *
 * Return value: none.
 */
void
lw6pil_pilot_free (lw6pil_pilot_t * pilot)
{
  if (pilot)
    {
      _lw6pil_worker_quit (&(pilot->draft));
      _lw6pil_worker_quit (&(pilot->reference));
      lw6map_free (pilot->level);
      lw6ker_game_struct_free (pilot->game_struct);
      lw6ker_game_state_free (pilot->backup);
      lw6sys_list_free (pilot->unverified_queue);
      lw6sys_list_free (pilot->verified_queue);
      lw6sys_list_free (pilot->replay);
      LW6SYS_FREE (pilot);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("trying to free NULL pilot"));
    }
}

/**
 * lw6pil_pilot_send_command
 *
 * @pilot: the object to send commands to.
 * @command_text: the text of the command, as received form network
 * @verified: wether we're sure this message is valid.
 *
 * Sends a command and handles it internally.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6pil_pilot_send_command (lw6pil_pilot_t * pilot, char *command_text,
			   int verified)
{
  int ret = 1;
  char *command_text_dup;

  command_text_dup = lw6sys_str_copy (command_text);
  if (command_text_dup)
    {
      if (verified)
	{
	  lw6sys_list_push_front (&(pilot->verified_queue), command_text_dup);
	}
      else
	{
	  lw6sys_list_push_front (&(pilot->unverified_queue),
				  command_text_dup);
	}
    }

  return ret;
}

/**
 * lw6pil_pilot_local_command
 *
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
lw6pil_pilot_local_command (lw6pil_pilot_t * pilot, char *command_text)
{
  int ret = 0;

  ret =
    lw6pil_command_execute_local_text (&(pilot->local_cursors), command_text);

  return ret;
}

static void
sync_draft_from_reference (lw6pil_pilot_t * pilot)
{
  if (lw6pil_pilot_get_reference_current_round (pilot) >
      pilot->last_sync_draft_from_reference)
    {
      lw6sys_mutex_lock (pilot->draft.compute_mutex);
      lw6sys_mutex_lock (pilot->reference.compute_mutex);

      if (lw6pil_pilot_get_reference_current_round (pilot) >
	  pilot->last_sync_draft_from_reference)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("pilot sync round=%d"),
		      lw6ker_game_state_get_rounds (pilot->
						    reference.game_state));
	  lw6ker_game_state_sync (pilot->draft.game_state,
				  pilot->reference.game_state);
	  pilot->draft.target_round = pilot->reference.target_round;
	  pilot->draft.current_round = pilot->reference.current_round;

	  pilot->last_sync_draft_from_reference =
	    lw6pil_pilot_get_reference_current_round (pilot);
	}

      lw6sys_mutex_unlock (pilot->reference.compute_mutex);
      lw6sys_mutex_unlock (pilot->draft.compute_mutex);
    }
}

static int
commit_reference (lw6pil_pilot_t * pilot)
{
  int ret = 1;
  char *command_text = NULL;
  lw6pil_command_t *command = NULL;
  int min_round = -1;
  int max_round = 0;

  /*
   * Process the commands for the reference threads.
   */
  if (!lw6sys_list_is_empty (pilot->verified_queue))
    {
      lw6sys_spinlock_lock (pilot->reference.commands_spinlock);
      while (pilot->verified_queue
	     && (command_text =
		 lw6sys_list_pop_front (&(pilot->verified_queue))) != NULL)
	{
	  command = lw6pil_command_new (command_text);
	  if (command)
	    {
	      if (min_round < 0)
		{
		  min_round = command->round;
		}
	      else
		{
		  min_round = lw6sys_max (command->round, min_round);
		}
	      max_round = lw6sys_max (command->round, max_round);
	      lw6sys_list_push_front (&(pilot->reference.commands), command);
	    }
	  LW6SYS_FREE (command_text);
	}
      if (pilot->verified_queue)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("verified_queue should be NULL, but isn't"));
	}
      pilot->verified_queue = lw6sys_list_new (lw6sys_free_callback);
      lw6sys_spinlock_unlock (pilot->reference.commands_spinlock);
      if (min_round >= 0)
	{
	  if (min_round <= pilot->last_commit_round)
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _
			  ("possible game inconsistency, min_round=%d last_commit_round=%d"),
			  min_round, pilot->last_commit_round);
	    }
	}
      if (max_round > 0)
	{
	  pilot->last_commit_round = max_round;
	}
    }

  return ret;
}

static int
commit_draft (lw6pil_pilot_t * pilot)
{
  int ret = 1;
  char *command_text = NULL;
  lw6pil_command_t *command = NULL;
  lw6pil_command_t *command_dup = NULL;
  lw6sys_list_t *replay = NULL;

  /*
   * Process the commands for the draft threads.
   */
  if (!lw6sys_list_is_empty (pilot->unverified_queue))
    {
      /*
       * Add unverified events to replay list
       */
      while (pilot->unverified_queue
	     && (command_text =
		 lw6sys_list_pop_front (&(pilot->unverified_queue))) != NULL)
	{
	  command = lw6pil_command_new (command_text);
	  if (command)
	    {
	      if (command->round > pilot->last_sync_draft_from_reference
		  && pilot->replay)
		{
		  lw6sys_list_push_front (&(pilot->replay), command);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _("ignoring unverified command \"%s\""),
			      command->text);
		  lw6pil_command_free (command);
		}
	    }
	  LW6SYS_FREE (command_text);
	}
      if (pilot->unverified_queue)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unverified_queue should be NULL, but isn't"));
	}
      pilot->unverified_queue = lw6sys_list_new (lw6sys_free_callback);

      /*
       * Now filter replay events, push them, and keep them for next time
       */
      if (!lw6sys_list_is_empty (pilot->replay))
	{
	  sync_draft_from_reference (pilot);

	  lw6sys_spinlock_lock (pilot->draft.commands_spinlock);
	  replay = lw6sys_list_new ((lw6sys_free_func_t) lw6pil_command_free);
	  if (replay)
	    {
	      while (pilot->replay
		     && (command =
			 (lw6pil_command_t *)
			 lw6sys_list_pop_front (&(pilot->replay))))
		{
		  if (command->round > pilot->last_sync_draft_from_reference)
		    {
		      command_dup = lw6pil_command_dup (command);
		      if (command_dup)
			{
			  lw6sys_list_push_front (&replay, command_dup);
			}
		      lw6sys_list_push_front (&(pilot->draft.commands),
					      command);
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _("draft anticipation of \"%s\""),
				  command->text);
		    }
		  else
		    {
		      lw6pil_command_free (command);
		    }
		}
	      if (pilot->replay)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("replay should be NULL, but isn't"));
		}
	      pilot->replay = replay;
	      replay = NULL;
	    }
	  lw6sys_spinlock_unlock (pilot->draft.commands_spinlock);
	}
    }

  return ret;
}

/**
 * lw6pil_pilot_commit
 *
 * @pilot: the object to commit.
 *
 * Commits all commands sent and actually send them to the
 * corresponding threads. This commit system allows better performance
 * by sending, for instance, all the commands for a given round together.
 *
 * Return value: none.
 */
int
lw6pil_pilot_commit (lw6pil_pilot_t * pilot)
{
  int ret = 1;

  ret = commit_reference (pilot) && ret;
  ret = commit_draft (pilot) && ret;

  return ret;
}

/**
 * lw6pil_pilot_make_backup
 *
 * @pilot: the object to perform the backup on
 *
 * Makes a new backup in the pilot, that is, copy 'reference'
 * to 'backup'.
 *
 * Return value: 1 if OK, 0 if not.
 */
int
lw6pil_pilot_make_backup (lw6pil_pilot_t * pilot)
{
  int ret = 0;

  ret = lw6pil_pilot_sync_from_reference (pilot->backup, pilot);

  return ret;
}

/**
 * lw6pil_pilot_can_sync
 *
 * @target: the target game_state we would sync on
 * @pilot: the object to perform the backup on
 *
 * Tests wether sync functions are callable with a given game state.
 * It verifies if the internal game_state and the target look the same.
 *
 * Return value: 1 if sync functions can be called, 0 if not.
 */
int
lw6pil_pilot_can_sync (lw6ker_game_state_t * target, lw6pil_pilot_t * pilot)
{
  int ret = 1;

  ret = ret && lw6ker_game_state_can_sync (target, pilot->backup);
  ret = ret
    && lw6ker_game_state_can_sync (target, pilot->reference.game_state);
  ret = ret && lw6ker_game_state_can_sync (target, pilot->draft.game_state);

  return ret;
}

/**
 * lw6pil_pilot_sync_from_backup
 *
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
lw6pil_pilot_sync_from_backup (lw6ker_game_state_t * target,
			       lw6pil_pilot_t * pilot)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("sync from backup round=%d"),
	      lw6ker_game_state_get_rounds (pilot->backup));
  ret = lw6ker_game_state_sync (target, pilot->backup);

  return ret;
}

/**
 * lw6pil_pilot_sync_from_reference
 *
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
lw6pil_pilot_sync_from_reference (lw6ker_game_state_t * target,
				  lw6pil_pilot_t * pilot)
{
  int ret = 0;

  lw6sys_mutex_lock (pilot->reference.global_mutex);

  lw6sys_log (LW6SYS_LOG_DEBUG, _("sync from reference round=%d"),
	      lw6ker_game_state_get_rounds (pilot->reference.game_state));
  ret = lw6ker_game_state_sync (target, pilot->reference.game_state);

  lw6sys_mutex_unlock (pilot->reference.global_mutex);

  return ret;
}

/**
 * lw6pil_pilot_sync_from_draft
 *
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
lw6pil_pilot_sync_from_draft (lw6ker_game_state_t * target,
			      lw6pil_pilot_t * pilot, int dirty_read)
{
  int ret = 0;
  int draft_rounds;
  int reference_rounds;
  int current_round;

  current_round = lw6ker_game_state_get_rounds (target);

  if (lw6pil_pilot_get_max_round (pilot) > current_round)
    {
      if (dirty_read == LW6PIL_DIRTY_READ_NEVER)
	{
	  lw6sys_mutex_lock (pilot->draft.compute_mutex);
	  lw6sys_mutex_lock (pilot->reference.compute_mutex);
	}
      if (lw6pil_pilot_get_max_round (pilot) > current_round)
	{
	  draft_rounds =
	    lw6ker_game_state_get_rounds (pilot->draft.game_state);
	  reference_rounds =
	    lw6ker_game_state_get_rounds (pilot->reference.game_state);
	  if (draft_rounds > reference_rounds)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("sync from draft round=%d/%d"),
			  draft_rounds, reference_rounds);
	      ret = lw6ker_game_state_sync (target, pilot->draft.game_state);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("sync from draft using reference round=%d/%d"),
			  reference_rounds, draft_rounds);
	      ret =
		lw6ker_game_state_sync (target, pilot->reference.game_state);
	    }
	}
      else
	{
	  ret = 1;
	}
      if (dirty_read == LW6PIL_DIRTY_READ_NEVER)
	{
	  lw6sys_mutex_unlock (pilot->reference.compute_mutex);
	  lw6sys_mutex_unlock (pilot->draft.compute_mutex);
	}
    }
  else
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6pil_pilot_dirty_read
 *
 * @pilot: the object to get informations from.
 *
 * Returns a direct access to the most up-to-date game_state, without
 * locking anything whatsoever. This is clearly to implement a dirty read
 * mode as the name of the function suggests.
 *
 * Return value: 1 if OK, 0 if not.
 */
lw6ker_game_state_t *
lw6pil_pilot_dirty_read (lw6pil_pilot_t * pilot)
{
  lw6ker_game_state_t *ret = NULL;
  int draft_rounds = 0;
  int reference_rounds = 0;

  draft_rounds = lw6ker_game_state_get_rounds (pilot->draft.game_state);
  reference_rounds =
    lw6ker_game_state_get_rounds (pilot->reference.game_state);
  if (draft_rounds > reference_rounds)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("dirty read using draft round=%d/%d"),
		  draft_rounds, reference_rounds);
      ret = pilot->draft.game_state;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _("dirty read using reference round=%d/%d"),
		  reference_rounds, draft_rounds);
      ret = pilot->reference.game_state;
    }

  return ret;
}

/**
 * lw6pil_pilot_repr
 *
 * @pil: the pilot to describe
 *
 * Returns a string describing the pilot. This is a very short description,
 * use it for logs, and to debug stuff. By no means it's a complete exhaustive
 * description. Still, the string returned should be unique.
 *
 * Return value: a dynamically allocated string.
 */
char *
lw6pil_pilot_repr (lw6pil_pilot_t * pilot)
{
  char *ret = NULL;

  if (pilot)
    {
      ret =
	lw6sys_new_sprintf
	("%u (%dx%d commmit_round=%d, current_round=%d)",
	 pilot->id, pilot->backup->map_state.shape.w,
	 pilot->backup->map_state.shape.h,
	 lw6pil_pilot_get_last_commit_round (pilot),
	 lw6pil_pilot_get_reference_current_round (pilot));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("can't generate string id for NULL pilot"));
    }

  return ret;
}

/**
 * lw6pil_pilot_calibrate
 *
 * @pilot: the object to calibrate
 * @timestamp: the current ticks setting (1000 ticks per second)
 * @round: the round expected to be returned with this ticks value
 *
 * Calibrates the pilot, that is, initializes it so that subsequent calls
 * to @lw6pil_pilot_get_round return consistent values.
 *
 * Return value: none.
 */
void
lw6pil_pilot_calibrate (lw6pil_pilot_t * pilot, int64_t timestamp, int round)
{
  pilot->calibrate_ticks = timestamp;
  pilot->calibrate_round = round;
}

/**
 * lw6pil_pilot_speed_up
 *
 * @pilot: the pilot to speed up
 * @round_inc: the number of rounds
 *
 * Re-calibrates the pilot so that it speeds up a bit.
 * This will basically increase next_round by round_inc.
 *
 * Return value: none.
 */
void
lw6pil_pilot_speed_up (lw6pil_pilot_t * pilot, int round_inc)
{
  pilot->calibrate_round += round_inc;
}

/**
 * lw6pil_pilot_slow_down
 *
 * @pilot: the pilot to speed up
 * @round_dec: the number of rounds
 *
 * Re-calibrates the pilot so that it slows down a bit.
 * This will basically decrease next_round by round_inc.
 *
 * Return value: none.
 */
void
lw6pil_pilot_slow_down (lw6pil_pilot_t * pilot, int round_dec)
{
  pilot->calibrate_round -= round_dec;
}

/**
 * lw6pil_pilot_get_next_round
 *
 * @pilot: the object to query
 * @timestamp: the current ticks setting (1000 ticks per second)
 *
 * Returns the round one should use to generate new events/commands
 * at a given time (given in ticks).
 *
 * Return value: none.
 */
int
lw6pil_pilot_get_next_round (lw6pil_pilot_t * pilot, int64_t timestamp)
{
  int ret = 0;
  int64_t delta;

  delta = timestamp - pilot->calibrate_ticks;
  delta *= (int64_t) pilot->backup->game_struct->rules.rounds_per_sec;
  delta /= LW6SYS_TICKS_PER_SEC;
  ret = pilot->calibrate_round + (int) delta;

  return ret;
}

/**
 * lw6pil_pilot_get_last_commit_round
 *
 * @pilot: the object to query
 *
 * Returns the round of the last commit (reference game_state) for
 * this object.
 *
 * Return value: the commit round (reference object)
 */
int
lw6pil_pilot_get_last_commit_round (lw6pil_pilot_t * pilot)
{
  int ret = 0;

  ret = pilot->last_commit_round;

  return ret;
}

/**
 * lw6pil_pilot_get_reference_target_round
 *
 * @pilot: the object to query
 *
 * Returns the round which is targetted in the reference game_state,
 * this is 'how far computation will go in the reference game_state
 * if no new commands are issued'. Note that there can always
 * be some commands which are not yet processed, so you should not
 * rely on this too heavily, however it gives a good idea of how
 * things are going.
 *
 * Return value: the target round (reference object)
 */
int
lw6pil_pilot_get_reference_target_round (lw6pil_pilot_t * pilot)
{
  int ret = 0;

  ret = pilot->reference.target_round;

  return ret;
}

/**
 * lw6pil_pilot_get_reference_current_round
 *
 * @pilot: the object to query
 *
 * Returns the current round in the reference game_state. There's no
 * lock on this call so don't rely on this too heavily, it just
 * gives you an idea of wether the pilot is very late on its
 * objectives or just on time.
 *
 * Return value: the current round (reference object)
 */
int
lw6pil_pilot_get_reference_current_round (lw6pil_pilot_t * pilot)
{
  int ret = 0;

  ret = pilot->reference.current_round;

  return ret;
}

/**
 * lw6pil_pilot_get_max_round
 *
 * @pilot: the object to query
 *
 * Returns the max current round in the reference or draft
 * game states. No lock on this call so don't rely on this
 * too heavily, it just gives you an idea of computation state.
 *
 * Return value: the current round (reference object)
 */
int
lw6pil_pilot_get_max_round (lw6pil_pilot_t * pilot)
{
  int ret = 0;

  ret =
    lw6sys_max (pilot->reference.current_round, pilot->draft.current_round);

  return ret;
}

/**
 * lw6pil_pilot_is_over
 *
 * @pilot: the object to query
 *
 * Tells wether the game is over or not.
 *
 * Return value: 1 if over, 0 if not
 */
int
lw6pil_pilot_is_over (lw6pil_pilot_t * pilot)
{
  int ret = 0;

  ret = pilot->reference.over ? 1 : 0;

  return ret;
}
