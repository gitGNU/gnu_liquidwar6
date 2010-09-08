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

#include <math.h>

#include "../../mod-gl.h"
#include "gl-cylinder-internal.h"

#define TEST_MENU_SIZE 8

/*
 * OpenGL wizardry, to prepare view parameters.
 */
static void
prepare_view (mod_gl_utils_context_t * utils_context,
	      _mod_gl_menu_cylinder_context_t * cylinder_context)
{
  glEnable (GL_TEXTURE_2D);
  glColor3f (1.0f, 1.0f, 1.0f);
}

static void
draw_cylinder (mod_gl_utils_context_t * utils_context,
	       _mod_gl_menu_cylinder_context_t * cylinder_context,
	       int i, int n, float relative_text_width)
{
  _mod_gl_menu_cylinder_draw_cylinder (utils_context, cylinder_context,
				       GL_RENDER, i, n, relative_text_width);
}

static void
draw_sphere (mod_gl_utils_context_t * utils_context,
	     _mod_gl_menu_cylinder_context_t * cylinder_context, int i, int n,
	     int sphere_i, int nb_spheres)
{
  _mod_gl_menu_cylinder_draw_sphere (utils_context, cylinder_context,
				     GL_RENDER, i, n, sphere_i, nb_spheres);
}

static SDL_Color
get_fg_color (lw6gui_look_t * look, lw6gui_menuitem_t * menuitem)
{
  SDL_Color color;

  if (!menuitem->enabled)
    {
      color =
	mod_gl_utils_color_8_to_sdl (look->style.color_set.
				     menu_color_disabled.fg);
    }
  else if (menuitem->selected)
    {
      color =
	mod_gl_utils_color_8_to_sdl (look->style.color_set.
				     menu_color_selected.fg);
    }
  else
    {
      color =
	mod_gl_utils_color_8_to_sdl (look->style.color_set.menu_color_default.
				     fg);
    }

  return color;
}

static SDL_Color
get_bg_color (lw6gui_look_t * look, lw6gui_menuitem_t * menuitem)
{
  SDL_Color color;

  if (!menuitem->enabled)
    {
      color =
	mod_gl_utils_color_8_to_sdl (look->style.color_set.
				     menu_color_disabled.bg);
    }
  else if (menuitem->colored)
    {
      if (menuitem->value >= 0 && menuitem->value < LW6MAP_NB_TEAM_COLORS)
	{
	  // todo, get it from config...
	  color =
	    mod_gl_utils_color_8_to_sdl (look->style.color_set.
					 team_colors[menuitem->value]);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("invalid value %d for colored menu item"),
		      menuitem->value);
	  color = mod_gl_utils_color_8_to_sdl (LW6SYS_COLOR_8_WHITE);
	}
    }
  else if (menuitem->selected)
    {
      color =
	mod_gl_utils_color_8_to_sdl (look->style.color_set.
				     menu_color_selected.bg);
    }
  else
    {
      color =
	mod_gl_utils_color_8_to_sdl (look->style.color_set.menu_color_default.
				     bg);
    }

  return color;
}

/*
 * Draw a basic text menu item (button).
 */
static void
draw_button (mod_gl_utils_context_t * utils_context,
	     _mod_gl_menu_cylinder_context_t * cylinder_context,
	     lw6gui_look_t * look, lw6gui_menuitem_t * menuitem, int i, int n)
{
  SDL_Surface *text_surface = NULL;
  mod_gl_utils_bitmap_t *bitmap = NULL;
  char *utf8;
  SDL_Color bg_color;
  SDL_Color fg_color;
  int ret = 0;
  SDL_Rect text_rect;
  SDL_Rect blit_rect;
  int bitmap_w;
  float texture_scale;
  float draw_esc_offset;
  float draw_esc_radius;
  float draw_esc_cyl_height;
  float draw_esc_cyl_height_offset;
  float draw_esc_rotate;
  float relative_text_width;
  float screen_ratio;

  bitmap =
    mod_gl_utils_get_button_from_menucache (utils_context, look, menuitem);

  if (bitmap == NULL)
    {
      utf8 = lw6sys_locale_to_utf8 (menuitem->label);
      if (utf8 != NULL)
	{
	  fg_color = get_fg_color (look, menuitem);
	  bg_color = get_bg_color (look, menuitem);
	  text_surface =
	    TTF_RenderUTF8_Shaded (utils_context->font_data.menu, utf8,
				   fg_color, bg_color);
	  if (text_surface != NULL)
	    {
	      utils_context->surface_counter.new_counter++;	// TTF_Render...
	      bitmap_w = text_surface->w +
		2 * utils_context->const_data.menu_font_size;
	      bitmap =
		mod_gl_utils_bitmap_new (utils_context, bitmap_w,
					 MOD_GL_UTILS_MENU_TEXTURE_H);
	      if (bitmap != NULL)
		{
		  text_rect.x = 0;
		  text_rect.y = 0;
		  text_rect.w = text_surface->w;
		  text_rect.h = text_surface->h;
		  blit_rect.x = (bitmap->surface->w - text_surface->w) / 2;
		  blit_rect.y = (bitmap->surface->h - text_surface->h) / 2;
		  blit_rect.w = text_surface->w;
		  blit_rect.h = text_surface->h;

		  mod_gl_utils_clear_surface_with_color (bitmap->surface,
							 bg_color);
		  SDL_BlitSurface (text_surface, &text_rect, bitmap->surface,
				   &blit_rect);

		  if (!mod_gl_utils_store_button_in_menucache
		      (utils_context, look, menuitem, bitmap))
		    {
		      /*
		       * Storing stuff in menucache should always work, but in
		       * case it wouldn't work, we just cancel everything,
		       * it's better not to display something just one time
		       * than risk a memory leak. Besides doing this will
		       * show the user that "something" is going wrong, which
		       * is an order of magnitude better than an obscure invisible
		       * memory leak...
		       */
		      mod_gl_utils_bitmap_free (utils_context, bitmap);
		      bitmap = NULL;
		    }
		}
	      mod_gl_utils_delete_surface (utils_context, text_surface);
	    }
	  LW6SYS_FREE (utf8);
	}
    }

  if (bitmap != NULL)
    {
      relative_text_width =
	((float) bitmap->surface->w) / ((float) MOD_GL_UTILS_MENU_TEXTURE_W);
      screen_ratio =
	((float) utils_context->video_mode.width) /
	((float) utils_context->video_mode.height);
      texture_scale = mod_gl_utils_texture_scale (bitmap->surface->w);

      mod_gl_utils_bitmap_bind (utils_context, bitmap);

      glMatrixMode (GL_TEXTURE);
      glPushMatrix ();
      glLoadIdentity ();
      glScalef (texture_scale, -1.0f, 1.0f);
      glTranslatef (0.0f,
		    -0.25f + cylinder_context->const_data.texture_offset,
		    0.0f);
      glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);

      if (i >= 0)
	{
	  draw_cylinder (utils_context, cylinder_context, i, n,
			 relative_text_width);
	}
      else
	{
	  draw_esc_offset = cylinder_context->const_data.esc_offset;
	  draw_esc_radius = cylinder_context->const_data.esc_radius;
	  draw_esc_cyl_height =
	    relative_text_width *
	    cylinder_context->const_data.esc_cyl_height * screen_ratio;
	  draw_esc_cyl_height_offset =
	    cylinder_context->const_data.esc_cyl_height_offset * screen_ratio;
	  draw_esc_rotate = cylinder_context->const_data.esc_rotate;
	  _mod_gl_menu_cylinder_draw_fixed_cylinder (utils_context,
						     cylinder_context,
						     GL_RENDER,
						     draw_esc_offset,
						     draw_esc_radius,
						     draw_esc_cyl_height,
						     draw_esc_cyl_height_offset,
						     draw_esc_rotate);
	}
      glMatrixMode (GL_TEXTURE);
      glPopMatrix ();

      ret = 1;

      // _mod_gl_schedule_delete_texture(context,text_texture);
    }

  /*
   * Note: it's important *NOT* to delete bitmap.
   * Indeed it's kept in the menucache structure. It will
   * be free when needed, wizardry inside menucache.c 8-)
   */

  if (!ret)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to draw button \"%s\""), menuitem->label);
    }
}

static void
draw_spheres (mod_gl_utils_context_t * utils_context,
	      _mod_gl_menu_cylinder_context_t * cylinder_context, int i,
	      int n, int blink_state, int nb_spheres)
{
  int sphere_i;

  if (blink_state)
    {
      mod_gl_utils_bitmap_bind (utils_context,
				utils_context->textures_1x1.
				menu_color_default_fg);
    }
  else
    {
      mod_gl_utils_bitmap_bind (utils_context,
				utils_context->textures_1x1.
				menu_color_default_bg);
    }

  for (sphere_i = 0; sphere_i < nb_spheres; ++sphere_i)
    {
      draw_sphere (utils_context, cylinder_context, i, n, sphere_i,
		   nb_spheres);
    }
}

/*
 * Display a menu.
 */
void
_mod_gl_menu_cylinder_display_menu (mod_gl_utils_context_t * utils_context,
				    _mod_gl_menu_cylinder_context_t *
				    cylinder_context, lw6gui_look_t * look,
				    lw6gui_menu_t * menu)
{
  int i, j, n;
  int blink_state;
  lw6gui_menuitem_t *menuitem;

  mod_gl_utils_set_render_mode_3d_menu (utils_context);
  mod_gl_utils_texture_1x1_update (utils_context, look);

  prepare_view (utils_context, cylinder_context);

  lw6gui_menu_update_display_range (menu,
				    cylinder_context->
				    const_data.max_displayed_items);

  blink_state =
    mod_gl_utils_timer_get_uptime (utils_context) /
    (cylinder_context->const_data.sphere_blink_period / 2) % 2;
  n = menu->nb_items_displayed + 2;
  if (menu->first_item_displayed > 0)
    {
      draw_spheres (utils_context, cylinder_context, 0, n,
		    blink_state, cylinder_context->const_data.nb_spheres);
    }
  if (menu->first_item_displayed + menu->nb_items_displayed < menu->nb_items)
    {
      draw_spheres (utils_context, cylinder_context, n - 1, n,
		    blink_state ? 0 : 1,
		    cylinder_context->const_data.nb_spheres);
    }
  for (i = 0; i < menu->nb_items_displayed; ++i)
    {
      j = i + menu->first_item_displayed;
      menuitem = menu->items[j];
      draw_button (utils_context, cylinder_context, look, menuitem, i + 1, n);
    }
  if (menu->esc_item->enabled)
    {
      draw_button (utils_context, cylinder_context, look, menu->esc_item, -1,
		   n);
    }
}

void
mod_gl_menu_cylinder_display_menu (mod_gl_utils_context_t * utils_context,
				   void *cylinder_context,
				   lw6gui_look_t * look, lw6gui_menu_t * menu)
{
  _mod_gl_menu_cylinder_display_menu (utils_context,
				      (_mod_gl_menu_cylinder_context_t *)
				      cylinder_context, look, menu);
  /*
   * It's really better to pick *after* drawing for the first
   * step will generate bitmaps in cache, and those bitmaps we
   * need later when picking to scale things
   */
  _mod_gl_menu_cylinder_pick_item (utils_context,
				   (_mod_gl_menu_cylinder_context_t *)
				   cylinder_context,
				   look,
				   &(utils_context->input.mouse.
				     menu_position),
				   &(utils_context->input.mouse.menu_scroll),
				   &(utils_context->input.mouse.menu_esc),
				   menu, utils_context->input.mouse.x,
				   utils_context->input.mouse.y);
}

void
_mod_gl_menu_cylinder_display_progress (mod_gl_utils_context_t *
					utils_context,
					_mod_gl_menu_cylinder_context_t *
					cylinder_context,
					lw6gui_look_t * look, float progress)
{
  mod_gl_utils_set_render_mode_3d_menu (utils_context);
  mod_gl_utils_texture_1x1_update (utils_context, look);

  prepare_view (utils_context, cylinder_context);

  if (progress > 0.0f && progress < 1.0f)	// strict, to disable it when complete
    {
      _mod_gl_menu_cylinder_draw_progress (utils_context, cylinder_context,
					   look, progress);
    }
}

void
mod_gl_menu_cylinder_display_progress (mod_gl_utils_context_t * utils_context,
				       void *cylinder_context,
				       lw6gui_look_t * look, float progress)
{
  _mod_gl_menu_cylinder_display_progress (utils_context,
					  (_mod_gl_menu_cylinder_context_t *)
					  cylinder_context, look, progress);
}
