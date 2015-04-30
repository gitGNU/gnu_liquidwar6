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
#include "gl1-cylinder-internal.h"

#define TEST_MENU_SIZE 8
#define _DESC_MENU "menu"
#define _PASS_THROUGH_SELECTED 1
#define _BREADCRUMBS_GLUE " > "

/*
 * OpenGL wizardry, to prepare view parameters.
 */
static void
_prepare_view (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context)
{
  glEnable (GL_TEXTURE_2D);
  glColor3f (1.0f, 1.0f, 1.0f);
}

static void
_draw_cylinder (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context, int i, int n,
		float relative_text_width)
{
  _mod_gl1_menu_cylinder_draw_cylinder (sys_context, utils_context, cylinder_context, GL_RENDER, i, n, relative_text_width);
}

static void
_draw_sphere (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context, int i, int n,
	      int sphere_i, int nb_spheres)
{
  _mod_gl1_menu_cylinder_draw_sphere (sys_context, utils_context, cylinder_context, GL_RENDER, i, n, sphere_i, nb_spheres);
}

static SDL_Color
_get_fg_color (lw6sys_context_t * sys_context, const lw6gui_look_t * look, const lw6gui_menuitem_t * menuitem)
{
  SDL_Color color;

  if (menuitem->selected)
    {
      color = mod_gl1_utils_color_8_to_sdl (sys_context, look->style.color_set.menu_color_selected.fg);
    }
  else
    {
      if (menuitem->enabled)
	{
	  color = mod_gl1_utils_color_8_to_sdl (sys_context, look->style.color_set.menu_color_default.fg);
	}
      else
	{
	  color = mod_gl1_utils_color_8_to_sdl (sys_context, look->style.color_set.menu_color_disabled.fg);
	}
    }

  return color;
}

static SDL_Color
_get_bg_color (lw6sys_context_t * sys_context, const lw6gui_look_t * look, const lw6gui_menuitem_t * menuitem)
{
  SDL_Color color;

  if (menuitem->colored)
    {
      if (menuitem->value >= 0 && menuitem->value < LW6MAP_NB_TEAM_COLORS)
	{
	  // todo, get it from config...
	  color = mod_gl1_utils_color_8_to_sdl (sys_context, look->style.color_set.team_colors[menuitem->value]);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid value %d for colored menu item"), menuitem->value);
	  color = mod_gl1_utils_color_8_to_sdl (sys_context, LW6SYS_COLOR_8_WHITE);
	}
    }
  else
    {
      if (menuitem->selected)
	{
	  color = mod_gl1_utils_color_8_to_sdl (sys_context, look->style.color_set.menu_color_selected.bg);
	}
      else
	{
	  if (menuitem->enabled)
	    {
	      color = mod_gl1_utils_color_8_to_sdl (sys_context, look->style.color_set.menu_color_default.bg);
	    }
	  else
	    {
	      color = mod_gl1_utils_color_8_to_sdl (sys_context, look->style.color_set.menu_color_disabled.bg);
	    }
	}
    }

  return color;
}

/*
 * Draw a basic text menu item (button).
 */
static void
_draw_button (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
	      _mod_gl1_menu_cylinder_context_t * cylinder_context, const lw6gui_look_t * look, const lw6gui_menuitem_t * menuitem, int i, int n)
{
  SDL_Surface *text_surface = NULL;
  mod_gl1_utils_bitmap_t *bitmap = NULL;
  char *utf8 = NULL;
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

  bitmap = mod_gl1_utils_get_button_from_menucache (sys_context, utils_context, look, menuitem);

  if (bitmap == NULL)
    {
      utf8 = lw6sys_locale_to_utf8 (sys_context, menuitem->label);
      if (utf8 != NULL)
	{
	  fg_color = _get_fg_color (sys_context, look, menuitem);
	  bg_color = _get_bg_color (sys_context, look, menuitem);
	  text_surface = TTF_RenderUTF8_Shaded (utils_context->font_data.menu, utf8, fg_color, bg_color);
	  if (text_surface != NULL)
	    {
	      utils_context->surface_counter.new_counter++;	// TTF_Render...
	      bitmap_w = text_surface->w + 2 * utils_context->const_data.menu_font_size;
	      bitmap = mod_gl1_utils_bitmap_new (sys_context, utils_context, bitmap_w, MOD_GL1_UTILS_MENU_TEXTURE_H, _DESC_MENU);
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

		  mod_gl1_utils_clear_surface_with_color (sys_context, bitmap->surface, bg_color);
		  SDL_BlitSurface (text_surface, &text_rect, bitmap->surface, &blit_rect);

		  if (!mod_gl1_utils_store_button_in_menucache (sys_context, utils_context, look, menuitem, bitmap))
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
		      mod_gl1_utils_bitmap_free (sys_context, utils_context, bitmap);
		      bitmap = NULL;
		    }
		}
	      mod_gl1_utils_delete_surface (sys_context, utils_context, text_surface);
	    }
	  LW6SYS_FREE (sys_context, utf8);
	}
    }

  if (bitmap != NULL)
    {
      relative_text_width = ((float) bitmap->surface->w) / ((float) MOD_GL1_UTILS_MENU_TEXTURE_W);
      screen_ratio = ((float) utils_context->sdl_context.video_mode.width) / ((float) utils_context->sdl_context.video_mode.height);
      texture_scale = mod_gl1_utils_texture_scale (sys_context, bitmap->surface->w);

      mod_gl1_utils_bitmap_bind (sys_context, utils_context, bitmap);

      glMatrixMode (GL_TEXTURE);
      glPushMatrix ();
      glLoadIdentity ();
      glScalef (texture_scale, -1.0f, 1.0f);
      glTranslatef (0.0f, -0.25f + cylinder_context->const_data.texture_offset, 0.0f);
      glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);

      if (i >= 0)
	{
	  _draw_cylinder (sys_context, utils_context, cylinder_context, i, n, relative_text_width);
	}
      else
	{
	  draw_esc_offset = cylinder_context->const_data.esc_offset;
	  draw_esc_radius = cylinder_context->const_data.esc_radius;
	  draw_esc_cyl_height = relative_text_width * cylinder_context->const_data.esc_cyl_height * screen_ratio;
	  draw_esc_cyl_height_offset = cylinder_context->const_data.esc_cyl_height_offset * screen_ratio;
	  draw_esc_rotate = cylinder_context->const_data.esc_rotate;
	  _mod_gl1_menu_cylinder_draw_fixed_cylinder (sys_context, utils_context,
						      cylinder_context,
						      GL_RENDER,
						      draw_esc_offset, draw_esc_radius, draw_esc_cyl_height, draw_esc_cyl_height_offset, draw_esc_rotate);
	}
      glMatrixMode (GL_TEXTURE);
      glPopMatrix ();

      ret = 1;

      // _mod_gl1_schedule_delete_texture(context,text_texture);
    }

  /*
   * Note: it's important *NOT* to delete bitmap.
   * Indeed it's kept in the menucache structure. It will
   * be free when needed, wizardry inside menucache.c 8-)
   */

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to draw button \"%s\""), menuitem->label);
    }
}

static void
_draw_spheres (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context, int i, int n,
	       int blink_state, int nb_spheres)
{
  int sphere_i;

  if (blink_state)
    {
      mod_gl1_utils_bitmap_bind (sys_context, utils_context, utils_context->textures_1x1.menu_color_default_fg);
    }
  else
    {
      mod_gl1_utils_bitmap_bind (sys_context, utils_context, utils_context->textures_1x1.menu_color_default_bg);
    }

  for (sphere_i = 0; sphere_i < nb_spheres; ++sphere_i)
    {
      _draw_sphere (sys_context, utils_context, cylinder_context, i, n, sphere_i, nb_spheres);
    }
}

/*
 * Display a menu.
 */
void
_mod_gl1_menu_cylinder_display_menu (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				     _mod_gl1_menu_cylinder_context_t * cylinder_context, const lw6gui_look_t * look, lw6gui_menu_t * menu)
{
  int i, j, n;
  int blink_state;
  lw6gui_menuitem_t *menuitem;
  float popup_x, popup_y, popup_w, popup_h;
  char *popup = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display menu, step 1: setup"));
  mod_gl1_utils_set_render_mode_3d_menu (sys_context, utils_context);

  _prepare_view (sys_context, utils_context, cylinder_context);

  lw6gui_menu_update_display_range (sys_context, menu, cylinder_context->const_data.max_displayed_items);

  blink_state = _lw6gfx_sdl_timer_get_uptime (sys_context, &(utils_context->sdl_context)) / (cylinder_context->const_data.sphere_blink_period / 2) % 2;
  n = menu->nb_items_displayed + 2;
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display menu, step 2: spheres"));
  if (menu->first_item_displayed > 0)
    {
      _draw_spheres (sys_context, utils_context, cylinder_context, 0, n, blink_state, cylinder_context->const_data.nb_spheres);
    }
  if (menu->first_item_displayed + menu->nb_items_displayed < menu->nb_items)
    {
      _draw_spheres (sys_context, utils_context, cylinder_context, n - 1, n, blink_state ? 0 : 1, cylinder_context->const_data.nb_spheres);
    }
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display menu, step 3: items"));
  for (i = 0; i < menu->nb_items_displayed; ++i)
    {
      j = i + menu->first_item_displayed;
      menuitem = menu->items[j];
      if (menuitem->selected)
	{
	  cylinder_context->i_right_point = i;
	  cylinder_context->j_tooltip = j;
	}
      _draw_button (sys_context, utils_context, cylinder_context, look, menuitem, i + 1, n);
    }
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display menu, step 4: esc"));
  if (menu->esc_item->enabled)
    {
      _draw_button (sys_context, utils_context, cylinder_context, look, menu->esc_item, -1, n);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display menu, step 5: popup"));
  if (lw6gui_menu_has_popup (sys_context, menu))
    {
      popup = menu->popup;
      if (!lw6sys_str_is_null_or_empty (sys_context, popup))
	{
	  if ((!lw6sys_str_is_same (sys_context, popup, utils_context->menucache_array.popup_str)) || (!utils_context->menucache_array.popup_bitmap))
	    {
	      if (utils_context->menucache_array.popup_str)
		{
		  LW6SYS_FREE (sys_context, utils_context->menucache_array.popup_str);
		  utils_context->menucache_array.popup_str = NULL;
		}
	      if (utils_context->menucache_array.popup_bitmap)
		{
		  mod_gl1_utils_bitmap_free (sys_context, utils_context, utils_context->menucache_array.popup_bitmap);
		  utils_context->menucache_array.popup_bitmap = NULL;
		}
	      utils_context->menucache_array.popup_str = lw6sys_str_copy (sys_context, popup);
	      utils_context->menucache_array.popup_bitmap =
		mod_gl1_utils_multiline_text_write (sys_context, utils_context,
						    utils_context->font_data.menu, popup,
						    &(look->style.color_set.menu_color_default),
						    cylinder_context->const_data.popup_alpha_bg,
						    cylinder_context->const_data.popup_max_width,
						    cylinder_context->const_data.popup_max_height,
						    cylinder_context->const_data.popup_border_size,
						    cylinder_context->const_data.popup_margin_size, cylinder_context->const_data.popup_reformat_width);
	    }

	  if (utils_context->menucache_array.popup_bitmap)
	    {
	      mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

	      popup_w =
		(utils_context->sdl_context.video_mode.width *
		 cylinder_context->const_data.popup_relative_size *
		 utils_context->menucache_array.popup_bitmap->surface->w) / cylinder_context->const_data.popup_max_width;
	      popup_h = (popup_w * utils_context->menucache_array.popup_bitmap->surface->h) / utils_context->menucache_array.popup_bitmap->surface->w;
	      popup_x = (utils_context->sdl_context.video_mode.width - popup_w) / 2;
	      popup_y = (utils_context->sdl_context.video_mode.height - popup_h) / 2;
	      mod_gl1_utils_bitmap_display (sys_context, utils_context,
					    utils_context->menucache_array.popup_bitmap, popup_x, popup_y, popup_x + popup_w, popup_y + popup_h);
	    }
	}
    }
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display menu, step 6: done"));
}

void
mod_gl1_menu_cylinder_display_menu (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *cylinder_context,
				    const lw6gui_look_t * look, lw6gui_menu_t * menu)
{
  _mod_gl1_menu_cylinder_display_menu (sys_context, utils_context, (_mod_gl1_menu_cylinder_context_t *) cylinder_context, look, menu);
  /*
   * It's really better to pick *after* drawing for the first
   * step will generate bitmaps in cache, and those bitmaps we
   * need later when picking to scale things
   */
  _mod_gl1_menu_cylinder_pick_item (sys_context, utils_context,
				    (_mod_gl1_menu_cylinder_context_t *)
				    cylinder_context,
				    look,
				    &(utils_context->sdl_context.input.mouse.menu_position),
				    &(utils_context->sdl_context.input.mouse.menu_scroll),
				    &(utils_context->sdl_context.input.mouse.menu_esc), menu,
				    utils_context->sdl_context.input.mouse.screen_pointer.pos_x, utils_context->sdl_context.input.mouse.screen_pointer.pos_y);
}

void
_mod_gl1_menu_cylinder_display_progress (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					 utils_context, _mod_gl1_menu_cylinder_context_t * cylinder_context, const lw6gui_look_t * look, float progress)
{
  mod_gl1_utils_set_render_mode_3d_menu (sys_context, utils_context);

  _prepare_view (sys_context, utils_context, cylinder_context);

  if (progress > 0.0f && progress < 1.0f)	// strict, to disable it when complete
    {
      _mod_gl1_menu_cylinder_draw_progress (sys_context, utils_context, cylinder_context, look, progress);
    }
}

void
mod_gl1_menu_cylinder_display_progress (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *cylinder_context,
					const lw6gui_look_t * look, float progress)
{
  _mod_gl1_menu_cylinder_display_progress (sys_context, utils_context, (_mod_gl1_menu_cylinder_context_t *) cylinder_context, look, progress);
}

/*
 * Display a menu.
 */
void
_mod_gl1_menu_cylinder_display_meta (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				     _mod_gl1_menu_cylinder_context_t * cylinder_context, const lw6gui_look_t * look, lw6gui_menu_t * menu)
{
  int n;
  int right_point_x = 0;
  int right_point_y = 0;
  float tooltip_x, tooltip_y, tooltip_w, tooltip_h;
  char *tooltip = NULL;
  float help_x, help_y, help_w, help_h;
  char *help = NULL;
  float breadcrumbs_x, breadcrumbs_y, breadcrumbs_w, breadcrumbs_h;
  char *breadcrumbs = NULL;

  mod_gl1_utils_set_render_mode_3d_menu (sys_context, utils_context);

  if (!lw6gui_menu_has_popup (sys_context, menu))
    {
      n = menu->nb_items_displayed + 2;
      if (cylinder_context->j_tooltip >= 0
	  && cylinder_context->j_tooltip < menu->nb_items
	  && cylinder_context->i_right_point >= 0 && cylinder_context->i_right_point < menu->nb_items_displayed)
	{
	  tooltip = menu->items[cylinder_context->j_tooltip]->tooltip;
	}
      if (!lw6sys_str_is_null_or_empty (sys_context, tooltip))
	{
	  _mod_gl1_menu_cylinder_get_cylinder_right_point (sys_context, utils_context,
							   cylinder_context, cylinder_context->i_right_point + 1, n, 1.0f, &right_point_x, &right_point_y);
	  if ((!lw6sys_str_is_same (sys_context, tooltip, utils_context->menucache_array.tooltip_str)) || (!utils_context->menucache_array.tooltip_bitmap))
	    {
	      if (utils_context->menucache_array.tooltip_str)
		{
		  LW6SYS_FREE (sys_context, utils_context->menucache_array.tooltip_str);
		  utils_context->menucache_array.tooltip_str = NULL;
		}
	      if (utils_context->menucache_array.tooltip_bitmap)
		{
		  mod_gl1_utils_bitmap_free (sys_context, utils_context, utils_context->menucache_array.tooltip_bitmap);
		  utils_context->menucache_array.tooltip_bitmap = NULL;
		}
	      utils_context->menucache_array.tooltip_str = lw6sys_str_copy (sys_context, tooltip);
	      utils_context->menucache_array.tooltip_bitmap =
		mod_gl1_utils_multiline_text_write (sys_context, utils_context,
						    utils_context->font_data.hud, tooltip,
						    &(look->style.color_set.menu_color_default),
						    cylinder_context->const_data.tooltip_alpha_bg,
						    cylinder_context->const_data.tooltip_max_width,
						    cylinder_context->const_data.tooltip_max_height,
						    cylinder_context->const_data.tooltip_border_size,
						    cylinder_context->const_data.tooltip_margin_size, cylinder_context->const_data.tooltip_reformat_width);
	    }

	  if (utils_context->menucache_array.tooltip_bitmap)
	    {
	      mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

	      tooltip_w =
		(utils_context->sdl_context.video_mode.width *
		 cylinder_context->const_data.tooltip_relative_size *
		 utils_context->menucache_array.tooltip_bitmap->surface->w) / cylinder_context->const_data.tooltip_max_width;
	      tooltip_h = (tooltip_w * utils_context->menucache_array.tooltip_bitmap->surface->h) / utils_context->menucache_array.tooltip_bitmap->surface->w;
	      tooltip_x = utils_context->sdl_context.video_mode.width - tooltip_w;
	      tooltip_y = ((float) right_point_y) - tooltip_h / 2;
	      mod_gl1_utils_bitmap_display (sys_context, utils_context,
					    utils_context->menucache_array.tooltip_bitmap, tooltip_x, tooltip_y, tooltip_x + tooltip_w, tooltip_y + tooltip_h);
	    }
	}

      help = menu->help;
      if (!lw6sys_str_is_null_or_empty (sys_context, help))
	{
	  if ((!lw6sys_str_is_same (sys_context, help, utils_context->menucache_array.help_str)) || (!utils_context->menucache_array.help_bitmap))
	    {
	      if (utils_context->menucache_array.help_str)
		{
		  LW6SYS_FREE (sys_context, utils_context->menucache_array.help_str);
		  utils_context->menucache_array.help_str = NULL;
		}
	      if (utils_context->menucache_array.help_bitmap)
		{
		  mod_gl1_utils_bitmap_free (sys_context, utils_context, utils_context->menucache_array.help_bitmap);
		  utils_context->menucache_array.help_bitmap = NULL;
		}
	      utils_context->menucache_array.help_str = lw6sys_str_copy (sys_context, help);
	      utils_context->menucache_array.help_bitmap =
		mod_gl1_utils_multiline_text_write (sys_context, utils_context,
						    utils_context->font_data.hud, help,
						    &(look->style.color_set.menu_color_default),
						    cylinder_context->const_data.help_alpha_bg,
						    cylinder_context->const_data.help_max_width,
						    cylinder_context->const_data.help_max_height,
						    cylinder_context->const_data.help_border_size,
						    cylinder_context->const_data.help_margin_size, cylinder_context->const_data.help_reformat_width);
	    }

	  if (utils_context->menucache_array.help_bitmap)
	    {
	      mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

	      help_w =
		(utils_context->sdl_context.video_mode.width *
		 cylinder_context->const_data.help_relative_size *
		 utils_context->menucache_array.help_bitmap->surface->w) / cylinder_context->const_data.help_max_width;
	      help_h = (help_w * utils_context->menucache_array.help_bitmap->surface->h) / utils_context->menucache_array.help_bitmap->surface->w;
	      help_x = (utils_context->sdl_context.video_mode.width - help_w) / 2;
	      help_y = utils_context->sdl_context.video_mode.height - help_h;
	      mod_gl1_utils_bitmap_display (sys_context, utils_context, utils_context->menucache_array.help_bitmap, help_x, help_y, help_x + help_w,
					    help_y + help_h);
	    }
	}
    }

  if (menu->breadcrumbs)
    {
      breadcrumbs = lw6sys_str_join (sys_context, menu->breadcrumbs, _BREADCRUMBS_GLUE);
    }

  if (!lw6sys_str_is_null_or_empty (sys_context, breadcrumbs))
    {
      if ((!lw6sys_str_is_same (sys_context, breadcrumbs, utils_context->menucache_array.breadcrumbs_str))
	  || (!utils_context->menucache_array.breadcrumbs_bitmap))
	{
	  if (utils_context->menucache_array.breadcrumbs_str)
	    {
	      LW6SYS_FREE (sys_context, utils_context->menucache_array.breadcrumbs_str);
	      utils_context->menucache_array.breadcrumbs_str = NULL;
	    }
	  if (utils_context->menucache_array.breadcrumbs_bitmap)
	    {
	      mod_gl1_utils_bitmap_free (sys_context, utils_context, utils_context->menucache_array.breadcrumbs_bitmap);
	      utils_context->menucache_array.breadcrumbs_bitmap = NULL;
	    }
	  utils_context->menucache_array.breadcrumbs_str = lw6sys_str_copy (sys_context, breadcrumbs);
	  utils_context->menucache_array.breadcrumbs_bitmap =
	    mod_gl1_utils_multiline_text_write (sys_context, utils_context,
						utils_context->font_data.hud,
						breadcrumbs,
						&(look->style.color_set.menu_color_default),
						cylinder_context->const_data.breadcrumbs_alpha_bg,
						cylinder_context->const_data.breadcrumbs_max_width,
						cylinder_context->const_data.breadcrumbs_max_height,
						cylinder_context->const_data.breadcrumbs_border_size,
						cylinder_context->const_data.breadcrumbs_margin_size, cylinder_context->const_data.breadcrumbs_reformat_width);
	}

      if (utils_context->menucache_array.breadcrumbs_bitmap)
	{
	  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

	  breadcrumbs_w =
	    (utils_context->sdl_context.video_mode.width *
	     cylinder_context->const_data.breadcrumbs_relative_size *
	     utils_context->menucache_array.breadcrumbs_bitmap->surface->w) / cylinder_context->const_data.breadcrumbs_max_width;
	  breadcrumbs_h =
	    (breadcrumbs_w * utils_context->menucache_array.breadcrumbs_bitmap->surface->h) / utils_context->menucache_array.breadcrumbs_bitmap->surface->w;
	  breadcrumbs_x = utils_context->sdl_context.video_mode.width * (1.0f - cylinder_context->const_data.breadcrumbs_relative_size) / 2.0f;
	  breadcrumbs_y = 0.0f;
	  mod_gl1_utils_bitmap_display (sys_context, utils_context,
					utils_context->menucache_array.breadcrumbs_bitmap,
					breadcrumbs_x, breadcrumbs_y, breadcrumbs_x + breadcrumbs_w, breadcrumbs_y + breadcrumbs_h);
	}
    }
  if (breadcrumbs)
    {
      LW6SYS_FREE (sys_context, breadcrumbs);
    }
}

void
mod_gl1_menu_cylinder_display_meta (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *cylinder_context,
				    const lw6gui_look_t * look, lw6gui_menu_t * menu)
{
  _mod_gl1_menu_cylinder_display_meta (sys_context, utils_context, (_mod_gl1_menu_cylinder_context_t *) cylinder_context, look, menu);
}
