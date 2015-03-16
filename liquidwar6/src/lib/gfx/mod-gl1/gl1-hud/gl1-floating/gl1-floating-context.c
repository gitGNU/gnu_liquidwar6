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

#include "../../mod-gl1.h"
#include "gl1-floating-internal.h"

int
_mod_gl1_hud_floating_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				    _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;

  mod_gl1_utils_viewport_drawable_max (sys_context, utils_context);

  ret = 1;

  return ret;
}

int
_mod_gl1_hud_floating_context_begin_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					 _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 1;

  if (!floating_context->gauges.disk)
    {
      floating_context->gauges.disk = gluNewQuadric ();
      if (floating_context->gauges.disk)
	{
	  gluQuadricOrientation (floating_context->gauges.disk, GLU_OUTSIDE);
	  gluQuadricDrawStyle (floating_context->gauges.disk, GLU_FILL);
	  gluQuadricNormals (floating_context->gauges.disk, GLU_FLAT);
	}
      else
	{
	  ret = 0;
	}
    }

  return ret;
}

void
_mod_gl1_hud_floating_context_end_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				       _mod_gl1_hud_floating_context_t * floating_context)
{
  if (floating_context->gauges.disk)
    {
      gluDeleteQuadric (floating_context->gauges.disk);
      floating_context->gauges.disk = 0;
    }
}

static int
_update_clock (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;

  int time_left;
  int hours;
  int minutes;
  int seconds;
  char *text = NULL;

  time_left = lw6ker_game_state_get_time_left (sys_context, floating_context->game_state);
  hours = time_left / 3600;
  minutes = (time_left / 60) % 60;
  seconds = time_left % 60;
  text = lw6sys_new_sprintf (sys_context, _x_ ("%0d:%02d:%02d"), hours, minutes, seconds);
  if (text)
    {
      if (floating_context->clock.clock_text)
	{
	  mod_gl1_utils_shaded_text_update (sys_context, utils_context,
					    floating_context->clock.clock_text, text, &(floating_context->look->style.color_set.hud_color_text));
	}
      else
	{
	  floating_context->clock.clock_text =
	    mod_gl1_utils_shaded_text_new (sys_context, utils_context, utils_context->font_data.menu, text,
					   &(floating_context->look->style.color_set.hud_color_text));
	}
      LW6SYS_FREE (sys_context, text);
    }

  return ret;
}

static int
_update_gauges (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 1;
  int i;
  char *percent_text = NULL;
  char *frags_text = NULL;
  int team_color;
  int percent;
  int frags;
  lw6map_color_couple_t color_text;

  for (i = 0; i < floating_context->score_array.nb_scores; ++i)
    {
      percent = floating_context->score_array.scores[i].fighters_percent;
      frags = floating_context->score_array.scores[i].frags;
      team_color = floating_context->score_array.scores[i].team_color;

      color_text.bg = floating_context->look->style.color_set.team_color_dead;
      if (team_color >= 0)
	{
	  color_text.fg = floating_context->look->style.color_set.team_colors[team_color];
	}
      else
	{
	  color_text.fg = floating_context->look->style.color_set.hud_color_text.fg;
	}

      if (floating_context->game_state)
	{
	  percent_text = lw6sys_new_sprintf (sys_context, _x_ ("%d%%"), percent);
	  if (percent_text)
	    {
	      if (floating_context->gauges.percent_texts[i])
		{
		  mod_gl1_utils_shaded_text_update (sys_context, utils_context, floating_context->gauges.percent_texts[i], percent_text, &color_text);
		}
	      else
		{
		  floating_context->gauges.percent_texts[i] =
		    mod_gl1_utils_shaded_text_new (sys_context, utils_context, utils_context->font_data.hud, percent_text, &color_text);
		}
	      LW6SYS_FREE (sys_context, percent_text);
	    }

	  if (floating_context->game_state->game_struct->rules.respawn_team)
	    {
	      frags_text = lw6sys_new_sprintf (sys_context, _x_ ("%d"), frags);
	      if (frags_text)
		{
		  if (floating_context->gauges.frags_texts[i])
		    {
		      mod_gl1_utils_shaded_text_update (sys_context, utils_context, floating_context->gauges.frags_texts[i], frags_text, &color_text);
		    }
		  else
		    {
		      floating_context->gauges.frags_texts[i] =
			mod_gl1_utils_shaded_text_new (sys_context, utils_context, utils_context->font_data.hud, frags_text, &color_text);
		    }
		  LW6SYS_FREE (sys_context, frags_text);
		}
	    }
	}
    }

  return ret;
}

int
_mod_gl1_hud_floating_context_update_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					  utils_context,
					  _mod_gl1_hud_floating_context_t
					  * floating_context,
					  const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  int ret = 0;


  floating_context->look = look;
  floating_context->game_state = game_state;
  floating_context->local_cursors = local_cursors;
  lw6ker_score_array_update (sys_context, &floating_context->score_array, floating_context->game_state);

  ret = _update_clock (sys_context, utils_context, floating_context) && ret;
  ret = _update_gauges (sys_context, utils_context, floating_context) && ret;

  return ret;
}

int
_mod_gl1_hud_floating_context_begin_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					   _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 1;

  if (!floating_context->score_pie.disk)
    {
      floating_context->score_pie.disk = gluNewQuadric ();
      if (floating_context->score_pie.disk)
	{
	  gluQuadricOrientation (floating_context->score_pie.disk, GLU_OUTSIDE);
	  gluQuadricDrawStyle (floating_context->score_pie.disk, GLU_FILL);
	  gluQuadricNormals (floating_context->score_pie.disk, GLU_FLAT);
	}
      else
	{
	  ret = 0;
	}
    }

  return ret;
}

void
_mod_gl1_hud_floating_context_end_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					 _mod_gl1_hud_floating_context_t * floating_context)
{
  if (floating_context->score_pie.disk)
    {
      gluDeleteQuadric (floating_context->score_pie.disk);
      floating_context->score_pie.disk = 0;
    }
}

static int
_update_pie (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 1;
  // int percent, frags;
  int team_color;
  lw6map_color_couple_t color_text;
  char *score_text = NULL;
  int i;

  for (i = 0; i < floating_context->score_array.nb_scores; ++i)
    {
      // percent = floating_context->score_array.scores[i].fighters_percent;
      // frags = floating_context->score_array.scores[i].frags;
      team_color = floating_context->score_array.scores[i].team_color;

      color_text = floating_context->look->style.color_set.hud_color_text;

      if (floating_context->game_state)
	{
	  if (floating_context->game_state->game_struct->rules.respawn_team)
	    {
	      score_text =
		lw6sys_new_sprintf (sys_context, _x_ ("%d: %s, %d frags"),
				    i + 1, _(lw6map_team_color_index_to_key (sys_context, team_color)), floating_context->score_array.scores[i].frags);
	    }
	  else
	    {
	      score_text =
		lw6sys_new_sprintf (sys_context, _x_ ("%d: %s, %d%%"), i + 1,
				    _(lw6map_team_color_index_to_key (sys_context, team_color)), floating_context->score_array.scores[i].fighters_percent);
	    }
	  if (score_text)
	    {
	      if (floating_context->score_pie.score_texts[i])
		{
		  mod_gl1_utils_shaded_text_update (sys_context, utils_context, floating_context->gauges.percent_texts[i], score_text, &color_text);
		}
	      else
		{
		  floating_context->score_pie.score_texts[i] =
		    mod_gl1_utils_shaded_text_new (sys_context, utils_context, utils_context->font_data.hud, score_text, &color_text);
		}
	      LW6SYS_FREE (sys_context, score_text);
	    }
	}
    }

  return ret;
}

int
_mod_gl1_hud_floating_context_update_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					    utils_context,
					    _mod_gl1_hud_floating_context_t
					    * floating_context,
					    const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors)
{
  int ret = 0;

  floating_context->look = look;
  floating_context->game_state = game_state;
  floating_context->local_cursors = local_cursors;
  lw6ker_score_array_update (sys_context, &floating_context->score_array, floating_context->game_state);

  ret = _update_pie (sys_context, utils_context, floating_context);

  return ret;
}

int
_mod_gl1_hud_floating_context_clear_hud_clock (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					       _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;

  if (floating_context->clock.clock_text)
    {
      mod_gl1_utils_shaded_text_free (sys_context, utils_context, floating_context->clock.clock_text);
    }
  memset (&(floating_context->clock), 0, sizeof (_mod_gl1_hud_floating_clock_t));

  ret = 1;

  return ret;
}

int
_mod_gl1_hud_floating_context_clear_hud_gauges (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						_mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;
  int i;

  if (floating_context->gauges.gauge_frame)
    {
      mod_gl1_utils_bitmap_free (sys_context, utils_context, floating_context->gauges.gauge_frame);
      floating_context->gauges.gauge_frame = 0;
    }
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    {
      if (floating_context->gauges.percent_texts[i])
	{
	  mod_gl1_utils_shaded_text_free (sys_context, utils_context, floating_context->gauges.percent_texts[i]);
	}
      if (floating_context->gauges.frags_texts[i])
	{
	  mod_gl1_utils_shaded_text_free (sys_context, utils_context, floating_context->gauges.frags_texts[i]);
	}
    }
  memset (&(floating_context->gauges), 0, sizeof (_mod_gl1_hud_floating_gauges_t));

  ret = 1;

  return ret;
}

int
_mod_gl1_hud_floating_context_clear_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					 _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;

  ret =
    _mod_gl1_hud_floating_context_clear_hud_clock (sys_context, utils_context,
						   floating_context)
    && _mod_gl1_hud_floating_context_clear_hud_gauges (sys_context, utils_context, floating_context);

  return ret;
}

int
_mod_gl1_hud_floating_context_clear_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					   _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;
  int i = 0;

  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    {
      if (floating_context->score_pie.score_texts[i])
	{
	  mod_gl1_utils_shaded_text_free (sys_context, utils_context, floating_context->score_pie.score_texts[i]);
	}
    }

  return ret;
}

int
_mod_gl1_hud_floating_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				     _mod_gl1_hud_floating_context_t * floating_context)
{
  int ret = 0;

  _mod_gl1_hud_floating_context_clear_hud (sys_context, utils_context, floating_context);
  _mod_gl1_hud_floating_context_clear_score (sys_context, utils_context, floating_context);

  ret = 1;

  return ret;
}
