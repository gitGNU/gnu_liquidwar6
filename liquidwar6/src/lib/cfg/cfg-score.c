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

#include "cfg.h"
#include "cfg-internal.h"
#include "../hlp/hlp.h"

#define _SCORE_FILE "score.xml"
#define _SCORE_KEY "score"
#define _CHECKSUM_KEY "checksum"
#define _DEFAULT_SCORE 6
#define _MIN_SCORE 6
#define _MAX_SCORE 10

typedef struct _score_s
{
  int score;
  int checksum;
} _score_t;

static char *
_get_filename (char *user_dir)
{
  return lw6sys_path_concat (user_dir, _SCORE_FILE);
}

static u_int32_t
_calc_checksum (int score)
{
  char *username = NULL;
  char *hostname = NULL;
  u_int32_t checksum = 0;

  username = lw6sys_get_username ();
  if (username)
    {
      hostname = lw6sys_get_hostname ();
      if (hostname)
	{
	  lw6sys_checksum_update_str (&checksum, username);
	  lw6sys_checksum_update_str (&checksum, hostname);
	  lw6sys_checksum_update_int32 (&checksum, score);
	  LW6SYS_FREE (hostname);
	}
      LW6SYS_FREE (username);
    }

  return checksum;
}

static void
load_callback (void *callback_data, char *element, char *key, char *value)
{
  _score_t *score = (_score_t *) callback_data;

  score = (_score_t *) callback_data;

  if (!strcmp (key, _SCORE_KEY))
    {
      score->score = lw6sys_atoi (value);
    }
  else if (!strcmp (key, _CHECKSUM_KEY))
    {
      score->checksum = lw6sys_atoi (value);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("item \"%s\" ignored"), key);
    }
}

/**
 * lw6cfg_load_score
 *
 * @user_dir: the user directory
 * @score: the score (out param)
 * 
 * Gets score from file.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cfg_load_score (char *user_dir, int *score)
{
  int ret = 0;
  char *filename = NULL;
  int checksum = 0;
  _score_t score_t;

  score_t.score = _DEFAULT_SCORE;
  score_t.checksum = 0;

  filename = _get_filename (user_dir);
  if (filename)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("loading score from \"%s\""),
		  filename);

      if (lw6sys_file_exists (filename))
	{
	  ret =
	    lw6cfg_read_key_value_xml_file (filename, load_callback,
					    (void *) &score_t);
	  checksum = _calc_checksum (score_t.score);
	  if (checksum != score_t.checksum)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("bad score checksum"));
	      score_t.score = _MIN_SCORE;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("score file \"%s\" doesn't exist, using defaults"),
		      filename);
	}

      LW6SYS_FREE (filename);
    }

  score_t.score =
    lw6sys_min (_MAX_SCORE, lw6sys_max (_MIN_SCORE, score_t.score));
  (*score) = score_t.score;

  return ret;
}

/**
 * lw6cfg_save_score
 *
 * @user_dir: the user directory
 * @score: the score
 * 
 * Saves score to file.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cfg_save_score (char *user_dir, int score)
{
  int ret = 0;
  FILE *f;
  char *filename = NULL;
  int checksum = 0;

  filename = _get_filename (user_dir);
  if (filename)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("saving score to \"%s\""), filename);

      f = fopen (filename, "wb");
      if (f)
	{
	  lw6sys_print_xml_header (f,
				   _x_
				   ("This is where your score is kept. Please do not edit, this would be assimilated to cheating, while it's not that hard to fool the game and make it believe you're super strong when you are not, such practice is not encouraged. It's believed it's more fun to wait until this number increases naturally."));

	  score = lw6sys_min (_MAX_SCORE, lw6sys_max (_MIN_SCORE, score));
	  checksum = _calc_checksum (score);
	  fprintf (f, "<int key=\"%s\" value=\"%d\" />%s", _SCORE_KEY, score,
		   lw6sys_eol ());
	  fprintf (f, "<int key=\"%s\" value=\"%d\" />%s", _CHECKSUM_KEY,
		   checksum, lw6sys_eol ());
	  lw6sys_print_xml_footer (f);
	  fclose (f);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("could not open file \"%s\" in write mode"),
		      filename);
	}
      LW6SYS_FREE (filename);
    }

  return ret;
}
