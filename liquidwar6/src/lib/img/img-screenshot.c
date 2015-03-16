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

#undef HAVE_STDDEF_H		// redefined by jpeglib.h
#undef HAVE_STDLIB_H		// redefined by jpeglib.h

#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>

#define LW6_SKIP_WINDOWS_H

#include "img.h"

#define _JPEG_3 3
#define _SCREENSHOT_JPEG_FILE "screenshot.jpeg"

/*
 * This is used to stamp jpegs as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

/**
 * lw6img_screenshot_new
 *
 * @sys_context: global system context
 * @game_state: game_state to create a screenshot from
 * @user_dir: user directory
 * @quality: quality, from 0 to 100
 *
 * Creates a JPEG screenshot from a game state. The @user_dir parameter
 * is used to build a file name and then use it
 * to write data on disk, it is then read and kept in memory.
 * Globally it's not that bad to store it for we do not generate
 * screenshots that often, and it's nice for debugging to have it so
 * developping a RAM-only writer wouldn't make it a blast anyway.
 *
 * Return value: dynamically allocated object.
 */
lw6img_jpeg_t *
lw6img_screenshot_new (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, char *user_dir, int quality)
{
  lw6img_jpeg_t *ret = NULL;
  lw6ker_game_struct_t *game_struct = game_state->game_struct;
  lw6map_level_t *level = game_struct->level;
  lw6map_style_t *style = &(game_struct->level->param.style);
  lw6sys_whd_t shape = { 0, 0, 0 };
  int x, y, z;
  int fighter_id = -1;
  const lw6ker_fighter_t *fighter = NULL;
  int team_color = LW6MAP_TEAM_COLOR_INVALID;
  lw6sys_color_8_t pixel_color;
  int surface = 0;
  JSAMPLE *tmp_buffer = NULL;
  /*
   * Immortalized because this is probably my (ufoot@ufoot.org) worst
   * bug ever https://savannah.gnu.org/bugs/?35349
   * Look, I mispelled int i=0 into int i=9, just because the 0 and the 9
   * keys are so close. Then compiler does not raise warning about value
   * not being initialized. But it's nonetheless wrong. And at end
   * of array, kaboum!
   */
  // int i = 9;
  int i = 0;
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *outfile;
  JSAMPROW row_pointer[1];
  int row_stride;
  char *filename = NULL;

  lw6ker_game_state_get_shape (sys_context, game_state, &shape);

  filename = lw6sys_path_concat (sys_context, user_dir, _SCREENSHOT_JPEG_FILE);
  if (filename)
    {
      ret = (lw6img_jpeg_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6img_jpeg_t));
      if (ret)
	{
	  ret->id = 0;
	  while (!ret->id)
	    {
	      ret->id = ++seq_id;
	    }

	  surface = shape.w * shape.h;
	  tmp_buffer = (JSAMPLE *) LW6SYS_CALLOC (sys_context, surface * _JPEG_3 * sizeof (JSAMPLE));
	  if (tmp_buffer)
	    {
	      for (y = 0; y < shape.h; ++y)
		{
		  for (x = 0; x < shape.w; ++x)
		    {
		      fighter_id = -1;
		      for (z = 0; z < shape.d && fighter_id < 0; ++z)
			{
			  fighter_id = lw6ker_game_state_get_fighter_id (sys_context, game_state, x, y, z);
			}
		      if (fighter_id >= 0)
			{
			  fighter = lw6ker_game_state_get_fighter_ro_by_id (sys_context, game_state, fighter_id);
			  team_color = fighter->team_color;
			  if (lw6map_team_color_is_valid (team_color))
			    {
			      pixel_color = style->color_set.team_colors[team_color];
			    }
			  else
			    {
			      /*
			       * Normally we should never get there, but well,
			       * just in case, we use black/dead color
			       */
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad team_color=%d at %d,%d"), team_color, x, y);
			      pixel_color = style->color_set.team_color_dead;
			    }
			}
		      else
			{
			  pixel_color = lw6map_texture_get_with_body_coord (sys_context, level, x, y);
			}
		      tmp_buffer[i++] = pixel_color.r;
		      tmp_buffer[i++] = pixel_color.g;
		      tmp_buffer[i++] = pixel_color.b;
		    }
		}

	      memset (&cinfo, 0, sizeof (struct jpeg_compress_struct));
	      memset (&jerr, 0, sizeof (struct jpeg_error_mgr));
	      cinfo.err = jpeg_std_error (&jerr);
	      jpeg_create_compress (&cinfo);

	      if ((outfile = fopen (filename, "wb")) == NULL)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't open jpeg file \"%s\""), filename);
		}
	      else
		{
		  jpeg_stdio_dest (&cinfo, outfile);

		  cinfo.image_width = shape.w;
		  cinfo.image_height = shape.h;
		  cinfo.input_components = _JPEG_3;
		  cinfo.in_color_space = JCS_RGB;

		  jpeg_set_defaults (&cinfo);
		  jpeg_set_quality (&cinfo, quality, TRUE);

		  jpeg_start_compress (&cinfo, TRUE);

		  row_stride = shape.w * _JPEG_3;

		  while (cinfo.next_scanline < cinfo.image_height)
		    {
		      row_pointer[0] = &tmp_buffer[cinfo.next_scanline * row_stride];
		      (void) jpeg_write_scanlines (&cinfo, row_pointer, 1);
		    }

		  jpeg_finish_compress (&cinfo);

		  fclose (outfile);

		  ret->jpeg_data = lw6sys_read_file_content_bin (sys_context, &ret->jpeg_size, filename);
		  if ((ret->jpeg_data != NULL) && (ret->jpeg_size > 0))
		    {
		      ret->shape = shape;
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("wrote screenshot in \"%s\""), filename);
		    }
		}

	      jpeg_destroy_compress (&cinfo);
	      LW6SYS_FREE (sys_context, tmp_buffer);
	    }
	}
      LW6SYS_FREE (sys_context, filename);
    }

  if (ret)
    {
      if ((!(ret->jpeg_data)) || (ret->jpeg_size <= 0) || (ret->shape.w <= 0) || (ret->shape.h <= 0))
	{
	  lw6img_screenshot_free (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6img_screenshot_free
 *
 * @sys_context: global system context
 * @screenhost: screenshot object to free
 *
 * Frees a JPEG screenshot.
 *
 * Return value: none.
 */
void
lw6img_screenshot_free (lw6sys_context_t * sys_context, lw6img_jpeg_t * screenshot)
{
  if (screenshot)
    {
      if (screenshot->jpeg_data)
	{
	  LW6SYS_FREE (sys_context, screenshot->jpeg_data);
	}
      LW6SYS_FREE (sys_context, screenshot);
    }
}
