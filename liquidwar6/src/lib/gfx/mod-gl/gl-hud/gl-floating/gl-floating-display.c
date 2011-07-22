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

#include <math.h>

#include "../../mod-gl.h"
#include "gl-floating-internal.h"

void
display_clock (mod_gl_utils_context_t * utils_context,
	       _mod_gl_hud_floating_context_t * floating_context)
{
  float x1, x2, y1, y2, dw, dh;
  float size_factor =
    sqrt (utils_context->video_mode.width * utils_context->video_mode.height);

  if (floating_context->clock.clock_text
      && floating_context->clock.clock_text->texture_h > 0)
    {
      x2 =
	utils_context->video_mode.width *
	floating_context->const_data.clock_x2;
      x1 =
	x2 -
	size_factor * floating_context->const_data.clock_size *
	((float) floating_context->clock.clock_text->texture_w) /
	((float) floating_context->clock.clock_text->texture_h);
      ;
      y1 = size_factor * floating_context->const_data.clock_y1;
      y2 = y1 + (size_factor * floating_context->const_data.clock_size);
      dw = size_factor * floating_context->const_data.clock_dw;
      dh = size_factor * floating_context->const_data.clock_dh;
      mod_gl_utils_shaded_text_display (utils_context,
					floating_context->clock.clock_text,
					x1, y1, x2, y2, dw, dh);
    }
}

void
display_gauges (mod_gl_utils_context_t * utils_context,
		_mod_gl_hud_floating_context_t * floating_context)
{
  float x1, x2, y1, y2, w, h;
  float text_w, text_h, text_x1, text_x2, text_y1, text_y2, text_dw, text_dh;
  float size_factor_screen =
    sqrt (utils_context->video_mode.width * utils_context->video_mode.height);
  float size_factor_score;
  float size_factor;
  float ratio;
  float inner, outer;
  int slices, loops;
  int i;
  float start;
  float sweep1;
  float sweep2;
  int team_color;

  if (floating_context->game_state && floating_context->gauges.gauge_frame
      && floating_context->gauges.gauge_frame->texture_h > 0)
    {
      x2 = utils_context->video_mode.width;
      y2 = utils_context->video_mode.height;
      start = floating_context->const_data.gauge_start;
      for (i = floating_context->score_array.nb_scores - 1; i >= 0; --i)
	{
	  ratio = floating_context->score_array.scores[i].fighters_ratio;
	  /*
	   * sqrt because what's important is what happens near zero
	   */
	  ratio = sqrt (ratio);
	  team_color = floating_context->score_array.scores[i].team_color;
	  if (team_color >= 0)
	    {
	      size_factor_score =
		ratio * floating_context->const_data.gauge_max_size + (1.0f -
								       ratio)
		* floating_context->const_data.gauge_min_size;
	      size_factor = size_factor_screen * size_factor_score;
	      w = size_factor;
	      h = size_factor;

	      x1 = x2 - w;
	      y1 = y2 - h;

	      slices = floating_context->const_data.gauge_slices;
	      loops = floating_context->const_data.gauge_loops;
	      sweep1 = ratio * floating_context->const_data.gauge_sweep;
	      sweep2 = floating_context->const_data.gauge_sweep - sweep1;

	      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	      glEnable (GL_BLEND);

	      glMatrixMode (GL_MODELVIEW);
	      glPushMatrix ();
	      glLoadIdentity ();
	      glTranslatef ((x1 + x2) / 2.0f, (y1 + y2) / 2.0f, 0.0f);
	      gluQuadricTexture (floating_context->gauges.disk, GL_TRUE);

	      inner =
		size_factor *
		floating_context->const_data.gauge_relative_bg_inner;
	      outer =
		size_factor *
		floating_context->const_data.gauge_relative_bg_outer;

	      if (floating_context->gauges.gauge_frame)
		{
		  mod_gl_utils_bitmap_colorize (utils_context,
						floating_context->gauges.
						gauge_frame,
						floating_context->look->style.
						colorize,
						&(floating_context->look->
						  style.color_set.
						  hud_color_frame));
		  mod_gl_utils_bitmap_bind (utils_context,
					    floating_context->gauges.
					    gauge_frame);
		  gluDisk (floating_context->gauges.disk, inner, outer,
			   slices, loops);
		}

	      glDisable (GL_BLEND);

	      inner =
		size_factor *
		floating_context->const_data.gauge_relative_fg_inner;
	      outer =
		size_factor *
		floating_context->const_data.gauge_relative_fg_outer;

	      if (utils_context->textures_1x1.team_colors[team_color])
		{
		  mod_gl_utils_bitmap_bind (utils_context,
					    utils_context->
					    textures_1x1.team_colors
					    [team_color]);
		  gluPartialDisk (floating_context->gauges.disk, inner, outer,
				  slices, loops, start, sweep1);
		}

	      if (utils_context->textures_1x1.team_color_dead)
		{
		  mod_gl_utils_bitmap_bind (utils_context,
					    utils_context->
					    textures_1x1.team_color_dead);
		  gluPartialDisk (floating_context->gauges.disk, inner, outer,
				  slices, loops, start + sweep1, sweep2);
		}
	      glMatrixMode (GL_MODELVIEW);
	      glPopMatrix ();

	      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	      glEnable (GL_BLEND);

	      text_h =
		size_factor *
		floating_context->const_data.gauge_relative_text_size;
	      text_dw = size_factor *
		floating_context->const_data.gauge_relative_text_dw;
	      text_dh = size_factor *
		floating_context->const_data.gauge_relative_text_dh;

	      if (floating_context->game_state->game_struct->
		  rules.respawn_team)
		{
		  if (floating_context->gauges.percent_texts[i]
		      && floating_context->gauges.
		      percent_texts[i]->texture_h > 0)
		    {
		      text_w =
			text_h *
			((float) floating_context->gauges.percent_texts[i]->
			 texture_w) /
			((float) floating_context->gauges.
			 percent_texts[i]->texture_h);
		      text_x1 = (x1 + x2 - text_w) / 2.0f;
		      text_y1 =
			(y1 + y2) / 2.0f +
			size_factor *
			floating_context->
			const_data.gauge_relative_percent_y1;
		      text_x2 = text_x1 + text_w;
		      text_y2 = text_y1 + text_h;
		      mod_gl_utils_shaded_text_display (utils_context,
							floating_context->
							gauges.
							percent_texts[i],
							text_x1, text_y1,
							text_x2, text_y2,
							text_dw, text_dh);
		    }
		  if (floating_context->gauges.frags_texts[i]
		      && floating_context->gauges.frags_texts[i]->texture_h >
		      0)
		    {
		      text_w =
			text_h *
			((float) floating_context->gauges.frags_texts[i]->
			 texture_w) /
			((float) floating_context->gauges.
			 frags_texts[i]->texture_h);
		      text_x1 = (x1 + x2 - text_w) / 2.0f;
		      text_y1 =
			(y1 + y2) / 2.0f +
			size_factor *
			floating_context->const_data.gauge_relative_frags_y1;
		      text_x2 = text_x1 + text_w;
		      text_y2 = text_y1 + text_h;
		      mod_gl_utils_shaded_text_display (utils_context,
							floating_context->
							gauges.frags_texts[i],
							text_x1, text_y1,
							text_x2, text_y2,
							text_dw, text_dh);
		    }
		}
	      else
		{
		  if (floating_context->gauges.percent_texts[i]
		      && floating_context->gauges.
		      percent_texts[i]->texture_h > 0)
		    {
		      text_w =
			text_h *
			((float) floating_context->gauges.percent_texts[i]->
			 texture_w) /
			((float) floating_context->gauges.
			 percent_texts[i]->texture_h);
		      text_x1 = (x1 + x2 - text_w) / 2.0f;
		      text_y1 =
			(y1 + y2) / 2.0f +
			size_factor *
			floating_context->
			const_data.gauge_relative_percent_only_y1;
		      text_x2 = text_x1 + text_w;
		      text_y2 = text_y1 + text_h;
		      mod_gl_utils_shaded_text_display (utils_context,
							floating_context->
							gauges.
							percent_texts[i],
							text_x1, text_y1,
							text_x2, text_y2,
							text_dw, text_dh);
		    }
		}
	      x2 = x1;
	    }
	}
    }
}

static void
display_hud (mod_gl_utils_context_t * utils_context,
	     _mod_gl_hud_floating_context_t * floating_context)
{
  glMatrixMode (GL_TEXTURE);
  glPushMatrix ();
  glLoadIdentity ();
  display_clock (utils_context, floating_context);
  display_gauges (utils_context, floating_context);
  glMatrixMode (GL_TEXTURE);
  glPopMatrix ();
}

void
_mod_gl_hud_floating_display_hud (mod_gl_utils_context_t * utils_context,
				  _mod_gl_hud_floating_context_t *
				  floating_context,
				  lw6gui_look_t * look,
				  lw6ker_game_state_t * game_state,
				  lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl_hud_floating_context_update_hud (utils_context, floating_context,
					   look, game_state, local_cursors);

  mod_gl_utils_set_render_mode_2d_blend (utils_context);
  display_hud (utils_context, floating_context);
}

void
mod_gl_hud_floating_display_hud (mod_gl_utils_context_t * utils_context,
				 void *hud_context,
				 lw6gui_look_t * look,
				 lw6ker_game_state_t * game_state,
				 lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl_hud_floating_display_hud (utils_context,
				    (_mod_gl_hud_floating_context_t *)
				    hud_context, look, game_state,
				    local_cursors);
}

void
display_pie (mod_gl_utils_context_t * utils_context,
	     _mod_gl_hud_floating_context_t * floating_context)
{
  int i = 0;
  float x0, y0, dx, dy, angle1, angle2, angle_offset;
  int percent = 0;
  float size_factor_screen =
    sqrt (utils_context->video_mode.width * utils_context->video_mode.height);
  float inner, outer;
  float heartbeat_factor = 1.0f;
  int slices, loops;
  int team_color;
  int cycle = 0;

  if (floating_context->game_state)
    {
      x0 = utils_context->video_mode.width / 2;
      y0 = utils_context->video_mode.height / 2;
      cycle = lw6sys_get_cycle ();
      angle1 = angle2 =
	-(cycle * 360.0f) /
	floating_context->const_data.score_pie_rotation_period;

      inner =
	floating_context->const_data.score_pie_inner *
	size_factor_screen / 2.0f;
      outer =
	floating_context->const_data.score_pie_outer *
	size_factor_screen / 2.0f;
      slices = floating_context->const_data.score_pie_slices;
      loops = floating_context->const_data.score_pie_loops;

      for (i = 0; i < floating_context->score_array.nb_scores;
	   ++i, angle1 = angle2)
	{
	  percent =
	    floating_context->score_array.scores[i].consolidated_percent;
	  angle2 = angle1 + percent * 3.6f;
	  angle_offset =
	    M_PI / 2.0f - ((angle1 + angle2) / 2.0f) * M_PI / 180.0f;
	  dx =
	    cos (angle_offset) * size_factor_screen *
	    floating_context->const_data.score_pie_offset / 2.0f;
	  dy =
	    sin (angle_offset) * size_factor_screen *
	    floating_context->const_data.score_pie_offset / 2.0f;

	  team_color = floating_context->score_array.scores[i].team_color;

	  if (i == 0)
	    {
	      heartbeat_factor =
		lw6sys_math_heartbeat (cycle,
				       floating_context->
				       const_data.score_pie_heartbeat_period,
				       1.0f,
				       floating_context->
				       const_data.score_pie_heartbeat_factor);
	    }
	  else
	    {
	      heartbeat_factor = 1.0f;
	    }

	  if (team_color >= 0)
	    {
	      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	      glEnable (GL_BLEND);

	      glMatrixMode (GL_MODELVIEW);
	      glPushMatrix ();
	      glLoadIdentity ();
	      glTranslatef (x0 + dx, y0 + dy, 0.0f);
	      mod_gl_utils_bitmap_bind (utils_context,
					utils_context->
					textures_1x1.team_colors[team_color]);
	      gluQuadricTexture (floating_context->score_pie.disk, GL_TRUE);
	      gluPartialDisk (floating_context->gauges.disk,
			      inner * heartbeat_factor,
			      outer * heartbeat_factor, slices, loops, angle1,
			      angle2 - angle1);
	      glPopMatrix ();
	    }
	}
    }
}

static void
display_score (mod_gl_utils_context_t * utils_context,
	       _mod_gl_hud_floating_context_t * floating_context)
{
  glMatrixMode (GL_TEXTURE);
  glPushMatrix ();
  glLoadIdentity ();
  display_pie (utils_context, floating_context);
  glMatrixMode (GL_TEXTURE);
  glPopMatrix ();
}

void
_mod_gl_hud_floating_display_score (mod_gl_utils_context_t * utils_context,
				    _mod_gl_hud_floating_context_t *
				    floating_context,
				    lw6gui_look_t * look,
				    lw6ker_game_state_t * game_state,
				    lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl_hud_floating_context_update_score (utils_context, floating_context,
					     look, game_state, local_cursors);

  mod_gl_utils_set_render_mode_2d_blend (utils_context);
  display_score (utils_context, floating_context);
}

void
mod_gl_hud_floating_display_score (mod_gl_utils_context_t * utils_context,
				   void *hud_context,
				   lw6gui_look_t * look,
				   lw6ker_game_state_t * game_state,
				   lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl_hud_floating_display_score (utils_context,
				      (_mod_gl_hud_floating_context_t *)
				      hud_context, look, game_state,
				      local_cursors);
}
