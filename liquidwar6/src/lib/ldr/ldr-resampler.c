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

#include <string.h>
#include <math.h>

#include "ldr.h"

/*
 * The _RESAMPLER_BOGUS_FPS is an arbritrary frames per sec
 * constant which is used in capacity planning, for some operations
 * (pure fighter moving, on the map) are directly linked to the number
 * of fighters and/or surface multiplicated by moves per second, but
 * some other, such as displaying stuff, are directly linked to the
 * surface but have no link with game speed. A typicall such task is:
 * displaying fighters. Therefore we include this constant which is
 * somewhat arbitrary and corresponds to "what we could expect as a
 * reasonnable fps setting, assuming displaying a fighter and moving
 * it have the same cost.
 */
#define _RESAMPLER_BOGUS_FPS 18
#define _RESAMPLER_UPSIZE 1.01f
#define _RESAMPLER_DOWNSIZE 0.99f
#define _RESAMPLER_R2 1.414f

static void
_check_limits (lw6ldr_hints_t * hints, int *w, int *h)
{
  *w = lw6sys_min (*w, hints->max_map_width);
  *h = lw6sys_min (*h, hints->max_map_height);

  *w = lw6sys_max (*w, hints->min_map_width);
  *h = lw6sys_max (*h, hints->min_map_height);

  *w = lw6sys_min (*w, LW6MAP_MAX_WIDTH);
  *h = lw6sys_min (*h, LW6MAP_MAX_HEIGHT);

  *w = lw6sys_max (*w, LW6MAP_MIN_WIDTH);
  *h = lw6sys_max (*h, LW6MAP_MIN_HEIGHT);
}

static float
_estimate_capacity (lw6map_rules_t * rules, lw6ldr_hints_t * hints, int w,
		    int h, int bench_value, int magic_number,
		    int expected_depth, float gray_level)
{
  float ret = 0.0f;
  int surface;
  /*
   * Some values defined float to avoid overflows in some calculus
   */
  float estimated_nb_fighters;
  float moves_per_sec;

  surface = w * h;
  estimated_nb_fighters =
    ((float) expected_depth) * ((float) surface) *
    ((float) rules->total_armies_size) *
    ((float) rules->total_armies_size) / 10000LL;
  estimated_nb_fighters *= gray_level;
  if (hints->guess_moves_per_sec && hints->speed > 0.0f)
    {
      moves_per_sec = sqrt (w * w + h * h) * hints->speed / _RESAMPLER_R2;
    }
  else
    {
      moves_per_sec = rules->rounds_per_sec * rules->moves_per_round;
    }
  ret =
    estimated_nb_fighters * moves_per_sec +
    ((float) surface) * ((float) _RESAMPLER_BOGUS_FPS);

  return ret;
}

static void
_guess_moves_per_sec (lw6map_rules_t * rules, lw6ldr_hints_t * hints, int w,
		      int h)
{
  int moves_per_sec;
  int i;
  int old_moves_per_round = 0;
  int old_rounds_per_sec = 0;

  if (hints->guess_moves_per_sec && hints->speed > 0.0f)
    {
      old_moves_per_round = rules->moves_per_round;
      old_rounds_per_sec = rules->rounds_per_sec;
      moves_per_sec = sqrt (w * w + h * h) * hints->speed / _RESAMPLER_R2;
      rules->moves_per_round = LW6MAP_RULES_MIN_MOVES_PER_ROUND;
      for (i = rules->moves_per_round + 1;
	   i <= LW6MAP_RULES_MAX_MOVES_PER_ROUND; ++i)
	{
	  if (i * rules->rounds_per_sec <= moves_per_sec)
	    {
	      rules->moves_per_round = i;
	    }
	}
      while (rules->moves_per_round * rules->rounds_per_sec > moves_per_sec
	     && rules->rounds_per_sec > LW6MAP_RULES_MIN_ROUNDS_PER_SEC)
	{
	  rules->rounds_per_sec--;
	}
      while (rules->moves_per_round * rules->rounds_per_sec < moves_per_sec
	     && rules->rounds_per_sec < LW6MAP_RULES_MAX_ROUNDS_PER_SEC)
	{
	  rules->rounds_per_sec++;
	}
      if (old_moves_per_round != rules->moves_per_round
	  || old_rounds_per_sec != rules->rounds_per_sec)
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _x_ ("changing speed from %dx%d to %dx%d"),
		      old_rounds_per_sec, old_moves_per_round,
		      rules->rounds_per_sec, rules->moves_per_round);
	}
    }
  else
    {
      // nothing to do
    }
}

void
lw6ldr_resampler_init (lw6ldr_resampler_t * resampler,
		       lw6map_param_t * param, lw6ldr_hints_t * hints,
		       int source_w,
		       int source_h,
		       int display_w, int display_h, float target_ratio,
		       int bench_value, int magic_number, int expected_depth,
		       float gray_level)
{
  int target_w = 0;
  int target_h = 0;
  int tmp_w = 0;
  int tmp_h = 0;
  float f;
  float fighter_scale;
  float required;
  float capacity;
  float capacity_orig;

  memset (resampler, 0, sizeof (lw6ldr_resampler_t));

  target_w = source_w;
  target_h = source_h;

  if (source_w > 0 && source_h > 0)
    {
      if (hints->resample)
	{
	  _check_limits (hints, &target_w, &target_h);

	  /*
	   * Step 1, we use fighter scale to get our target size
	   * This also includes zoom.
	   */
	  // 0*0=0 so size must be non-zero
	  if (hints->fighter_scale > 0.0f)
	    {
	      fighter_scale = hints->fighter_scale;
	    }
	  else
	    {
	      fighter_scale = LW6LDR_HINTS_DEFAULT_FIGHTER_SCALE;
	    }
	  tmp_w = display_w * param->style.zoom / fighter_scale;
	  tmp_h = display_h * param->style.zoom / fighter_scale;
	  if (hints->downsize_using_fighter_scale
	      && tmp_w * tmp_h < target_w * target_h)
	    {
	      target_w = tmp_w;
	      target_h = tmp_h;
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("fighter_scale=%f on display %dx%d -> downsizing from %dx%d to %dx%d"),
			  hints->fighter_scale, display_w, display_h,
			  source_w, source_h, target_w, target_h);
	    }
	  else if (hints->upsize_using_fighter_scale
		   && tmp_w * tmp_h > target_w * target_h)
	    {
	      target_w = tmp_w;
	      target_h = tmp_h;
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_
			  ("fighter_scale=%f on display %dx%d -> upsizing from %dx%d to %dx%d"),
			  hints->fighter_scale, display_w, display_h,
			  source_w, source_h, target_w, target_h);
	    }

	  _check_limits (hints, &target_w, &target_h);

	  /*
	   * Step 2, we correct the ratio
	   */
	  if (target_w > 0 && target_h > 0)
	    {
	      f = target_ratio / (((float) target_w) / ((float) target_h));
	      if (f > 1.0f)
		{
		  target_w *= f;
		}
	      else if (f < 1.0f)
		{
		  target_h /= f;
		}
	    }

	  _check_limits (hints, &target_w, &target_h);

	  /*
	   * Step 3, take bench in account
	   */
	  capacity_orig = ((float) bench_value) * ((float) magic_number);
	  capacity = capacity_orig;
	  required =
	    _estimate_capacity (&param->rules, hints, target_w, target_h,
				bench_value, magic_number, expected_depth,
				gray_level);
	  if (required > 0.0f)
	    {
	      f = 1.0f;
	      if (hints->downsize_using_bench_value && capacity < required)
		{
		  while (capacity < required && tmp_w > LW6MAP_MIN_WIDTH
			 && tmp_h > LW6MAP_MIN_HEIGHT)
		    {
		      f *= _RESAMPLER_DOWNSIZE;
		      tmp_w = target_w * f;
		      tmp_h = target_h * f;
		      required =
			_estimate_capacity (&param->rules, hints, tmp_w,
					    tmp_h, bench_value, magic_number,
					    expected_depth, gray_level);
		    }
		  if (tmp_w != target_w || tmp_h != target_h)
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _x_
				  ("required=%f capacity=%f bench_value=%d -> downsizing from %dx%d to %dx%d"),
				  required, capacity_orig, bench_value,
				  target_w, target_h, tmp_w, tmp_h);
		      target_w = tmp_w;
		      target_h = tmp_h;
		    }
		}
	      else if (hints->upsize_using_bench_value && capacity > required)
		{
		  while (capacity < required && tmp_w < LW6MAP_MAX_WIDTH
			 && tmp_h < LW6MAP_MAX_HEIGHT)
		    {
		      f *= _RESAMPLER_UPSIZE;
		      tmp_w = target_w * f;
		      tmp_h = target_h * f;
		      required =
			_estimate_capacity (&param->rules, hints, tmp_w,
					    tmp_h, bench_value, magic_number,
					    expected_depth, gray_level);
		    }
		  if (tmp_w != target_w || tmp_h != target_h)
		    {
		      lw6sys_log (LW6SYS_LOG_INFO,
				  _x_
				  ("required=%f capacity=%f bench_value=%d -> upsizing from %dx%d to %dx%d"),
				  required, capacity_orig, bench_value,
				  target_w, target_h, tmp_w, tmp_h);
		      target_w = tmp_w;
		      target_h = tmp_h;
		    }
		}
	    }

	  _check_limits (hints, &target_w, &target_h);

	  /*
	   * Step 4, guess speed if needed
	   */
	  _guess_moves_per_sec (&param->rules, hints, target_w, target_h);

	  /*
	   * Step 4, we check against map surface limits
	   */
	  if (target_w * target_h > hints->max_map_surface)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_
			  ("target_w=%d target_h=%d hints->max_map_surface=%d -> downsizing"),
			  target_w, target_h, hints->max_map_surface);
	      f =
		sqrt ((((float) target_w) * ((float) target_h)) /
		      ((float) hints->max_map_surface));
	      target_w /= f;
	      target_h /= f;
	    }

	  if (target_w * target_h < hints->min_map_surface)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_
			  ("target_w=%d target_h=%d hints->min_map_surface=%d -> upsizing"),
			  target_w, target_h, hints->min_map_surface);
	      f =
		sqrt (((float) hints->min_map_surface) /
		      (((float) target_w) * ((float) target_h)));
	      target_w *= f;
	      target_h *= f;
	    }

	  _check_limits (hints, &target_w, &target_h);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to handle map with shape %dx%d"), source_w,
		  source_h);
    }

  resampler->target_w = target_w;
  resampler->target_h = target_h;
  resampler->source_w = source_w;
  resampler->source_h = source_h;
  resampler->scale_x =
    ((float) (resampler->target_w)) / ((float) (resampler->source_w));
  resampler->scale_y =
    ((float) (resampler->target_h)) / ((float) (resampler->source_h));
}

static void
check_bounds (int *x, int *y, int w, int h)
{
  (*x) = lw6sys_min ((*x), w - 1);
  (*y) = lw6sys_min ((*y), h - 1);
  (*x) = lw6sys_max ((*x), 0);
  (*y) = lw6sys_max ((*y), 0);
}

void
lw6ldr_resampler_force (lw6ldr_resampler_t * resampler,
			int source_w,
			int source_h, int target_w, int target_h)
{
  resampler->target_w = target_w;
  resampler->target_h = target_h;
  resampler->source_w = source_w;
  resampler->source_h = source_h;
  resampler->scale_x =
    ((float) (resampler->target_w)) / ((float) (resampler->source_w));
  resampler->scale_y =
    ((float) (resampler->target_h)) / ((float) (resampler->source_h));
}

void
lw6ldr_resampler_source2target (lw6ldr_resampler_t * resampler, int *target_x,
				int *target_y, int source_x, int source_y)
{
  (*target_x) = (int) (source_x * resampler->scale_x);
  (*target_y) = (int) (source_y * resampler->scale_y);
  check_bounds (target_x, target_y, resampler->target_w, resampler->target_h);
}

void
lw6ldr_resampler_target2source (lw6ldr_resampler_t * resampler, int *source_x,
				int *source_y, int target_x, int target_y)
{
  (*source_x) = (int) (target_x / resampler->scale_x);
  (*source_y) = (int) (target_y / resampler->scale_y);
  check_bounds (source_x, source_y, resampler->source_w, resampler->source_h);
}
