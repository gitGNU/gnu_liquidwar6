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

#include <math.h>

#include "gl1-splash.h"
#include "gl1-splash-internal.h"

void
_display_root (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context)
{
  lw6sys_color_f_t bg_color;

  /*
   * Not very performant since we've already cleared with prepare_buffer,
   * but well, seems anyway cleaner than forcing look to match splash
   * settings.
   */
  lw6sys_color_8_to_f (&bg_color, splash_context->const_data.root_bg);
  glClearColor (bg_color.r, bg_color.g, bg_color.b, bg_color.a);
  glClear (GL_COLOR_BUFFER_BIT);
}

void
_update_system (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context)
{
  int x_center = 0;
  int y_center = 0;
  float angle = 0.0f;
  lw6ker_cursor_t cursor;

  if (splash_context->game.bitmap && splash_context->game.game_state)
    {
      x_center =
	((int)
	 (_lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) *
	  splash_context->const_data.cursors_center_speed)) % lw6ker_game_state_get_w (sys_context, splash_context->game.game_state);
      y_center = lw6ker_game_state_get_h (sys_context, splash_context->game.game_state) / 2;
      angle = ((float) _lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context))) / ((float) splash_context->const_data.cursors_spin_period);
      if (lw6ker_game_state_get_cursor (sys_context, splash_context->game.game_state, &cursor, _MOD_GL1_SPLASH_GAME_CURSOR1_ID))
	{
	  cursor.pos.x = x_center + splash_context->const_data.cursors_spin_radius * cos (angle);
	  cursor.pos.y = y_center + splash_context->const_data.cursors_spin_radius * sin (angle);
	  lw6ker_game_state_set_cursor (sys_context, splash_context->game.game_state, &cursor);
	}
      if (lw6ker_game_state_get_cursor (sys_context, splash_context->game.game_state, &cursor, _MOD_GL1_SPLASH_GAME_CURSOR2_ID))
	{
	  cursor.pos.x = x_center - splash_context->const_data.cursors_spin_radius * sin (angle);
	  cursor.pos.y = y_center + splash_context->const_data.cursors_spin_radius * cos (angle);
	  lw6ker_game_state_set_cursor (sys_context, splash_context->game.game_state, &cursor);
	}
      if (lw6ker_game_state_get_cursor (sys_context, splash_context->game.game_state, &cursor, _MOD_GL1_SPLASH_GAME_CURSOR3_ID))
	{
	  cursor.pos.x = x_center - splash_context->const_data.cursors_spin_radius * cos (angle);
	  cursor.pos.y = y_center - splash_context->const_data.cursors_spin_radius * sin (angle);
	  lw6ker_game_state_set_cursor (sys_context, splash_context->game.game_state, &cursor);
	}
      if (lw6ker_game_state_get_cursor (sys_context, splash_context->game.game_state, &cursor, _MOD_GL1_SPLASH_GAME_CURSOR4_ID))
	{
	  cursor.pos.x = x_center + splash_context->const_data.cursors_spin_radius * sin (angle);
	  cursor.pos.y = y_center - splash_context->const_data.cursors_spin_radius * cos (angle);
	  lw6ker_game_state_set_cursor (sys_context, splash_context->game.game_state, &cursor);
	}
      lw6ker_game_state_do_round (sys_context, splash_context->game.game_state);
      mod_gl1_utils_update_game_bitmap_raw (sys_context, utils_context,
					    splash_context->game.bitmap, splash_context->game.game_state, &splash_context->const_data.map_color, 1);
    }
}

void
_display_system (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context)
{
  int slices;
  int stacks;
  float planet_radius;
  float satellite_radius;
  float planet_day_angle = 0.0f;
  float satellite_year_angle = 0.0f;
  float satellite_day_angle = 0.0f;

  slices = splash_context->const_data.slices;
  stacks = splash_context->const_data.stacks;

  planet_radius = splash_context->const_data.planet_radius;
  satellite_radius = splash_context->const_data.satellite_radius;
  if (splash_context->const_data.planet_day_period > 0)
    {
      planet_day_angle =
	(_lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) %
	 splash_context->const_data.planet_day_period) * 360.0f / splash_context->const_data.planet_day_period;
    }
  if (splash_context->const_data.satellite_year_period > 0)
    {
      satellite_year_angle =
	(_lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) %
	 splash_context->const_data.satellite_year_period) * 360.0f / splash_context->const_data.satellite_year_period;
    }
  if (splash_context->const_data.satellite_day_period > 0)
    {
      satellite_day_angle =
	(_lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) %
	 splash_context->const_data.satellite_day_period) * 360.0f / splash_context->const_data.satellite_day_period;
    }

  mod_gl1_utils_set_render_mode_3d_menu (sys_context, utils_context);

  glDisable (GL_BLEND);

  /*
   * Dunno why but had to add these after some scroll hack
   * the splash screen got pixelized???
   */
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  mod_gl1_utils_bitmap_bind (sys_context, utils_context, splash_context->game.bitmap);
  // glMatrixMode (GL_TEXTURE);
  // glPushMatrix ();
  // glLoadIdentity ();
  // glScalef(-1.0f,1.0f,1.0f);
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();
  gluQuadricTexture (splash_context->game.sphere, GL_TRUE);
  glTranslatef (splash_context->const_data.planet_x, splash_context->const_data.planet_y, splash_context->const_data.planet_z);
  glRotatef (splash_context->const_data.rotate_x, 1.0f, 0.0f, 0.0f);
  glRotatef (splash_context->const_data.rotate_y, 0.0f, 1.0f, 0.0f);
  glRotatef (splash_context->const_data.rotate_z, 0.0f, 0.0f, 1.0f);
  glRotatef (planet_day_angle, 0.0f, 1.0f, 0.0f);
  glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
  gluSphere (splash_context->game.sphere, planet_radius, slices, stacks);
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
  // glMatrixMode (GL_TEXTURE);
  // glPopMatrix ();

  mod_gl1_utils_bitmap_bind (sys_context, utils_context, splash_context->bitmap_data.satellite);
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();
  gluQuadricTexture (splash_context->game.sphere, GL_TRUE);
  glTranslatef (splash_context->const_data.planet_x, splash_context->const_data.planet_y, splash_context->const_data.planet_z);
  glRotatef (splash_context->const_data.rotate_x, 1.0f, 0.0f, 0.0f);
  glRotatef (splash_context->const_data.rotate_y, 0.0f, 1.0f, 0.0f);
  glRotatef (splash_context->const_data.rotate_z, 0.0f, 0.0f, 1.0f);
  glRotatef (satellite_year_angle, 0.0f, 1.0f, 0.0f);
  glTranslatef (splash_context->const_data.satellite_distance, 0.0f, 0.0f);
  glRotatef (satellite_day_angle, 0.0f, 1.0f, 0.0f);
  glRotatef (+90.0f, 1.0f, 0.0f, 0.0f);
  gluSphere (splash_context->game.sphere, satellite_radius, slices, stacks);
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
}

void
_update_text (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context)
{
  int i;
  char *credits;

  if (splash_context->const_data.text_period > 0)
    {
      i = _lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) / splash_context->const_data.text_period;
      credits = lw6hlp_get_credits (sys_context, i);
      if (credits)
	{
	  if ((!(splash_context->text.shaded_text)) || (strcmp (credits, splash_context->text.shaded_text->text) != 0))
	    {
	      if (splash_context->text.shaded_text)
		{
		  mod_gl1_utils_shaded_text_free (sys_context, utils_context, splash_context->text.shaded_text);
		}
	      splash_context->text.shaded_text =
		mod_gl1_utils_shaded_text_new (sys_context, utils_context, utils_context->font_data.hud, credits, &splash_context->const_data.text_color);
	    }
	  LW6SYS_FREE (sys_context, credits);
	}
    }
}

void
_display_text (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context)
{
  float x1, y1, x2, y2;
  float center_x, center_y, w, h, dw, dh;
  float cos_input;
  float scale;
  float size;

  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

  if (splash_context->const_data.text_period > 0 && splash_context->text.shaded_text)
    {
      cos_input = 2.0f * M_PI * _lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) / splash_context->const_data.text_period;
      scale = 1.0f - cos (cos_input);
      size = scale * splash_context->const_data.text_size;
      h = size * utils_context->sdl_context.video_mode.height;
      w = h * splash_context->text.shaded_text->texture_w / (float) splash_context->text.shaded_text->texture_h;
      center_x = utils_context->sdl_context.video_mode.width * splash_context->const_data.text_center_x;
      center_y = utils_context->sdl_context.video_mode.height * splash_context->const_data.text_center_y;
      x1 = center_x - w / 2.0f;
      x2 = center_x + w / 2.0f;
      y1 = center_y - h / 2.0f;
      y2 = center_y + h / 2.0f;
      dw = scale * utils_context->sdl_context.video_mode.width * splash_context->const_data.text_dw;
      dh = scale * utils_context->sdl_context.video_mode.height * splash_context->const_data.text_dh;

      mod_gl1_utils_shaded_text_display (sys_context, utils_context, splash_context->text.shaded_text, x1, y1, x2, y2, dw, dh);
    }
}

void
_mod_gl1_splash_display (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context)
{
  _display_root (sys_context, utils_context, splash_context);
  _update_system (sys_context, utils_context, splash_context);
  _display_system (sys_context, utils_context, splash_context);
  _update_text (sys_context, utils_context, splash_context);
  _display_text (sys_context, utils_context, splash_context);
}

void
mod_gl1_splash_display (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *splash_context)
{
  if (utils_context && splash_context)
    {
      _mod_gl1_splash_display (sys_context, utils_context, (_mod_gl1_splash_context_t *) splash_context);
    }
}

void
_mod_gl1_splash_patch_system_color (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context,
				    lw6map_color_couple_t * system_color)
{
  *system_color = splash_context->const_data.text_color;
}

void
mod_gl1_splash_patch_system_color (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *splash_context,
				   lw6map_color_couple_t * system_color)
{
  if (utils_context && splash_context)
    {
      _mod_gl1_splash_patch_system_color (sys_context, utils_context, (_mod_gl1_splash_context_t *) splash_context, system_color);
    }
}
