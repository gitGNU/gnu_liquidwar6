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

#include "../../../../mat/mat.h"

#include "../../mod-gl1.h"
#include "gl1-cylinder-internal.h"

#define FEEDBACK_BUFFER_SIZE 4096

#define _PASS_THROUGH_DEFAULT -1
#define _PASS_THROUGH_PREV -2
#define _PASS_THROUGH_NEXT -3
#define _PASS_THROUGH_ESC -4
#define _PASS_THROUGH_SELECTED -5

typedef struct _mod_gl1_cylinder_pick_item_s
{
  lw6gui_zone_t zone;
  int position;
  int scroll;
  int esc;
} _mod_gl1_cylinder_pick_item_t;

static void
_mat_all (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
	  _mod_gl1_menu_cylinder_context_t * cylinder_context, lw6mat_fmat4_t * mat_all, int i, int n, float relative_text_width, float radius,
	  float cyl_height, float cyl_height_offset, float y, float dy, float rotate)
{
  lw6mat_fmat4_t mat_perspective;
  lw6mat_fvec3_t vec_translate1;
  lw6mat_fmat4_t mat_translate1;
  lw6mat_fmat4_t mat_rotate1;
  lw6mat_fmat4_t mat_rotate2;
  lw6mat_fvec3_t vec_translate2;
  lw6mat_fmat4_t mat_translate2;
  lw6mat_fvec3_t vec_translate3;
  lw6mat_fmat4_t mat_translate3;
  lw6mat_fvec3_t vec_translate4;
  lw6mat_fmat4_t mat_translate4;

  lw6mat_fmat4_identity (&mat_translate3);
  lw6mat_fmat4_identity (&mat_translate2);
  lw6mat_fmat4_identity (&mat_rotate1);
  lw6mat_fmat4_identity (&mat_translate1);
  lw6mat_fmat4_identity (&mat_perspective);
  lw6mat_fvec3_zero (&vec_translate1);
  lw6mat_fvec3_zero (&vec_translate2);
  lw6mat_fvec3_zero (&vec_translate3);

  lw6mat_fmat4_perspective (&mat_perspective, utils_context->const_data.persp_fovy,
			    ((float) utils_context->sdl_context.video_mode.width) /
			    ((float) utils_context->sdl_context.video_mode.height), utils_context->const_data.persp_znear,
			    utils_context->const_data.persp_zfar);

  vec_translate1.p.x = cyl_height_offset - cyl_height / 2.0f;
  vec_translate1.p.y = 0.0f;
  vec_translate1.p.z = -1.0f;
  lw6mat_fmat4_translation (&mat_translate1, &vec_translate1);
  lw6mat_fmat4_rot_y (&mat_rotate1, lw6sys_math_deg2rad (90));
  vec_translate2.p.x = 0.0f;
  vec_translate2.p.y = y + dy;
  vec_translate2.p.z = 0.0f;
  lw6mat_fmat4_translation (&mat_translate2, &vec_translate2);
  vec_translate3.p.x = 0.0f;
  vec_translate3.p.y = 0.0f;
  vec_translate3.p.z = cyl_height / 2.0f;
  lw6mat_fmat4_translation (&mat_translate3, &vec_translate3);
  lw6mat_fmat4_rot_x (&mat_rotate2, rotate);
  vec_translate4.p.x = 0.0f;
  vec_translate4.p.y = 0.0f;
  vec_translate4.p.z = -cyl_height / 2.0f;
  lw6mat_fmat4_translation (&mat_translate4, &vec_translate4);

  lw6mat_fmat4_mul_fmat4 (mat_all, mat_all, &mat_perspective);
  lw6mat_fmat4_mul_fmat4 (mat_all, mat_all, &mat_translate1);
  lw6mat_fmat4_mul_fmat4 (mat_all, mat_all, &mat_rotate1);
  lw6mat_fmat4_mul_fmat4 (mat_all, mat_all, &mat_translate2);
  lw6mat_fmat4_mul_fmat4 (mat_all, mat_all, &mat_translate3);
  lw6mat_fmat4_mul_fmat4 (mat_all, mat_all, &mat_rotate2);
  lw6mat_fmat4_mul_fmat4 (mat_all, mat_all, &mat_translate4);
}

static void
_find_cylinder_limits (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
		       _mod_gl1_menu_cylinder_context_t * cylinder_context, lw6gui_zone_t * zone, int i, int n, float relative_text_width, int esc)
{
  memset (zone, 0, sizeof (lw6gui_zone_t));
  if (n > 0 && utils_context->sdl_context.video_mode.width > 0 && utils_context->sdl_context.video_mode.height > 9)
    {
      lw6mat_fmat4_t mat_all;
      lw6mat_fvec4_t p1;
      lw6mat_fvec4_t p2;
      lw6mat_fvec4_t p3;
      lw6mat_fvec4_t p4;
      float ratio = 0.0f;
      float radius = 0.0f;
      float cyl_height = 0.0f;
      float cyl_height_offset = 0.0f;
      float y = 0.0f;
      float dy = 0.0f;
      float rotate = 0.0f;
      int32_t dt = 0;

      lw6mat_fmat4_identity (&mat_all);

      lw6mat_fvec4_zero (&p1);
      lw6mat_fvec4_zero (&p2);
      lw6mat_fvec4_zero (&p3);
      lw6mat_fvec4_zero (&p4);

      ratio = ((float) utils_context->sdl_context.video_mode.width) / ((float) utils_context->sdl_context.video_mode.height);
      radius = cylinder_context->const_data.radius1 / n;
      cyl_height = relative_text_width * (esc ? cylinder_context->const_data.esc_cyl_height : cylinder_context->const_data.cyl_height) * ratio;
      cyl_height_offset = relative_text_width * (esc ? cylinder_context->const_data.esc_cyl_height_offset : 0.0f) * ratio;
      y = _mod_gl1_menu_cylinder_get_cylinder_y (sys_context, utils_context, cylinder_context, i, n);
      dt = _lw6gfx_sdl_timer_get_cycle (sys_context, &(utils_context->sdl_context)) % cylinder_context->const_data.oscil_period;
      dy =
	(cylinder_context->const_data.oscil_range1 / n) * sin (2.0f * M_PI *
							       ((dt / ((float) cylinder_context->const_data.oscil_period)) + (((float) i) / ((float) n))));
      rotate = esc ? cylinder_context->const_data.esc_rotate : 0.0f;

      _mat_all (sys_context, utils_context, cylinder_context, &mat_all, i, n, relative_text_width, radius, cyl_height, cyl_height_offset, y, dy, rotate);

      p1.p.y = p4.p.y = -radius;
      p2.p.y = p3.p.y = radius;
      p1.p.x = p2.p.x = p3.p.x = p4.p.x = 0.0f;
      p1.p.z = p2.p.z = 0.0f;
      p3.p.z = p4.p.z = cyl_height;
      p1.p.w = p2.p.w = p3.p.w = p4.p.w = LW6MAT_F_1;

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("before [%0.3f,%0.3f,%0.3f] [%0.3f,%0.3f,%0.3f] [%0.3f,%0.3f,%0.3f] [%0.3f,%0.3f,%0.3f]"), p1.p.x,
		  p1.p.y, p1.p.z, p2.p.x, p2.p.y, p2.p.z, p3.p.x, p3.p.y, p3.p.z, p4.p.x, p4.p.y, p4.p.z);

      lw6mat_fmat4_mul_fvec4 (&p1, &mat_all, &p1);
      lw6mat_fvec4_homogeneous (sys_context, &p1);
      lw6mat_fmat4_mul_fvec4 (&p2, &mat_all, &p2);
      lw6mat_fvec4_homogeneous (sys_context, &p2);
      lw6mat_fmat4_mul_fvec4 (&p3, &mat_all, &p3);
      lw6mat_fvec4_homogeneous (sys_context, &p3);
      lw6mat_fmat4_mul_fvec4 (&p4, &mat_all, &p4);
      lw6mat_fvec4_homogeneous (sys_context, &p4);

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("after [%0.3f,%0.3f,%0.3f] [%0.3f,%0.3f,%0.3f] [%0.3f,%0.3f,%0.3f] [%0.3f,%0.3f,%0.3f]"), p1.p.x, p1.p.y,
		  p1.p.z, p2.p.x, p2.p.y, p2.p.z, p3.p.x, p3.p.y, p3.p.z, p4.p.x, p4.p.y, p4.p.z);

      zone->x1 = utils_context->sdl_context.video_mode.width + 1;
      zone->x2 = -1;
      zone->y1 = utils_context->sdl_context.video_mode.height + 1;
      zone->y2 = -1;

      zone->x1 = zone->x1 < p1.p.x ? zone->x1 : p1.p.x;
      zone->x1 = zone->x1 < p2.p.x ? zone->x1 : p2.p.x;
      zone->x1 = zone->x1 < p3.p.x ? zone->x1 : p3.p.x;
      zone->x1 = zone->x1 < p4.p.x ? zone->x1 : p4.p.x;
      zone->y1 = zone->y1 < p1.p.y ? zone->y1 : p1.p.y;
      zone->y1 = zone->y1 < p2.p.y ? zone->y1 : p2.p.y;
      zone->y1 = zone->y1 < p3.p.y ? zone->y1 : p3.p.y;
      zone->y1 = zone->y1 < p4.p.y ? zone->y1 : p4.p.y;
      zone->x2 = zone->x2 > p1.p.x ? zone->x2 : p1.p.x;
      zone->x2 = zone->x2 > p2.p.x ? zone->x2 : p2.p.x;
      zone->x2 = zone->x2 > p3.p.x ? zone->x2 : p3.p.x;
      zone->x2 = zone->x2 > p4.p.x ? zone->x2 : p4.p.x;
      zone->y2 = zone->y2 > p1.p.y ? zone->y2 : p1.p.y;
      zone->y2 = zone->y2 > p2.p.y ? zone->y2 : p2.p.y;
      zone->y2 = zone->y2 > p3.p.y ? zone->y2 : p3.p.y;
      zone->y2 = zone->y2 > p4.p.y ? zone->y2 : p4.p.y;

      zone->w = zone->x1 - zone->x2;
      zone->h = zone->y1 - zone->y2;
    }
}

void
_mod_gl1_menu_cylinder_pick_item (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				  _mod_gl1_menu_cylinder_context_t *
				  cylinder_context,
				  const lw6gui_look_t * look, int *position, int *scroll, int *esc, lw6gui_menu_t * menu, int screen_x, int screen_y)
{
  int i = 0, n = 0;
  _mod_gl1_cylinder_pick_item_t *pick_items = NULL;
  _mod_gl1_cylinder_pick_item_t *pick_item = NULL;
  float gl_x = -1.0f;
  float gl_y = -1.0f;

  *position = -1;
  *scroll = 0;
  *esc = 0;

  n = menu->nb_items_displayed + 2;
  if (n > 0)
    {
      pick_items = (_mod_gl1_cylinder_pick_item_t *) LW6SYS_CALLOC (sys_context, (n + 1) * sizeof (_mod_gl1_cylinder_pick_item_t));
      if (pick_items != NULL)
	{
	  for (i = 0; i < n + 1; ++i)
	    {
	      pick_items[i].position = -1;
	    }
	  if (menu->first_item_displayed > 0)
	    {
	      _find_cylinder_limits (sys_context, utils_context, cylinder_context, &(pick_items[0].zone), 0, n, 1.0f, 0);
	      pick_items[0].scroll = -1;
	    }
	  if (menu->first_item_displayed + menu->nb_items_displayed < menu->nb_items)
	    {
	      _find_cylinder_limits (sys_context, utils_context, cylinder_context, &(pick_items[n - 1].zone), n - 1, n, 1.0f, 0);
	      pick_items[n - 1].scroll = 1;
	    }
	  for (i = 0; i < menu->nb_items_displayed; ++i)
	    {
	      _find_cylinder_limits (sys_context, utils_context, cylinder_context, &(pick_items[i + 1].zone), i + 1, n, 1.0f, 0);
	      pick_items[i + 1].position = i;
	    }
	  if (menu->esc_item->enabled)
	    {
	      _find_cylinder_limits (sys_context, utils_context, cylinder_context, &(pick_items[n].zone), 0, n, 1.0f, 1);

	      pick_items[n].esc = 1;
	    }

	  mod_gl1_utils_viewport_screen_to_gl (sys_context, utils_context, &gl_x, &gl_y, screen_x, screen_y);
	  for (i = 0; i < n + 1; ++i)
	    {
	      pick_item = &(pick_items[i]);
	      if (pick_item->zone.x1 <= gl_x && pick_item->zone.x2 >= gl_x && pick_item->zone.y1 <= gl_y && pick_item->zone.y2 >= gl_y)
		{
		  *position = pick_item->position;
		  *scroll = pick_item->scroll;
		  *esc = pick_item->esc;
		}
	    }

	  LW6SYS_FREE (sys_context, pick_items);
	}
    }
}

void
_mod_gl1_menu_cylinder_get_cylinder_right_point (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						 utils_context,
						 _mod_gl1_menu_cylinder_context_t
						 * cylinder_context, int i, int n, float relative_text_width, int *right_point_x, int *right_point_y)
{
  lw6gui_zone_t zone;

  memset (&zone, 0, sizeof (lw6gui_zone_t));
  if (n > 0)
    {
      _find_cylinder_limits (sys_context, utils_context, cylinder_context, &zone, i, n, relative_text_width, 0);
    }

  mod_gl1_utils_viewport_gl_to_screen (sys_context, utils_context, right_point_x, right_point_y, zone.x2, (zone.y1 + zone.y2) / 2.0f);
}
