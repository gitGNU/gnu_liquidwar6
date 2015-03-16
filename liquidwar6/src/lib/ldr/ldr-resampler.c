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
#define _RESAMPLER_05 0.49f

#define _RESAMPLER_USE_FOR_GEN_EXPECTED_DEPTH 2
#define _RESAMPLER_USE_FOR_GEN_GRAY_LEVEL 0.5f

static void
_check_limits (lw6sys_context_t * sys_context, const lw6ldr_hints_t * hints, int *w, int *h)
{
  int surface = 0;
  float coeff = 1.0f;

  surface = (*w) * (*h);

  if (surface > 0)
    {
      if (hints->max_map_surface > 0)
	{
	  if (surface > hints->max_map_surface)
	    {
	      coeff = sqrt (((float) hints->max_map_surface) / ((float) surface));
	      *w = ((float) (*w)) * coeff;
	      *h = ((float) (*h)) * coeff;
	      surface = (*w) * (*h);
	    }
	}
      if (hints->min_map_surface > 0)
	{
	  if (surface < hints->min_map_surface)
	    {
	      coeff = sqrt (((float) hints->min_map_surface) / ((float) surface));
	      *w = ((float) (*w)) * coeff;
	      *h = ((float) (*h)) * coeff;
	      surface = (*w) * (*h);
	    }
	}
      if (surface > LW6MAP_MAX_BODY_SURFACE)
	{
	  coeff = sqrt (((float) LW6MAP_MAX_BODY_SURFACE) / ((float) surface));
	  *w = ((float) (*w)) * coeff;
	  *h = ((float) (*h)) * coeff;
	  surface = (*w) * (*h);
	}
      if (surface < LW6MAP_MIN_BODY_SURFACE)
	{
	  coeff = sqrt (((float) LW6MAP_MIN_BODY_SURFACE) / ((float) surface));
	  *w = ((float) (*w)) * coeff;
	  *h = ((float) (*h)) * coeff;
	  surface = (*w) * (*h);
	}
    }

  *w = lw6sys_imin (*w, hints->max_map_width);
  *h = lw6sys_imin (*h, hints->max_map_height);

  *w = lw6sys_imax (*w, hints->min_map_width);
  *h = lw6sys_imax (*h, hints->min_map_height);

  *w = lw6sys_imin (*w, LW6MAP_MAX_BODY_WIDTH);
  *h = lw6sys_imin (*h, LW6MAP_MAX_BODY_HEIGHT);

  *w = lw6sys_imax (*w, LW6MAP_MIN_BODY_WIDTH);
  *h = lw6sys_imax (*h, LW6MAP_MIN_BODY_HEIGHT);
}

static float
_estimate_capacity (lw6sys_context_t * sys_context, const lw6map_rules_t * rules, const lw6ldr_hints_t * hints, int w, int h, int expected_depth,
		    float gray_level)
{
  float ret = 0.0f;
  int surface;
  /*
   * Some values defined float to avoid overflows in some calculus
   */
  float estimated_nb_fighters;
  float moves_per_sec;

  surface = w * h;
  estimated_nb_fighters = ((float) expected_depth) * ((float) surface) * ((float) rules->total_armies_size) * ((float) rules->total_armies_size) / 10000LL;
  estimated_nb_fighters *= gray_level;
  if (hints->guess_moves_per_sec && hints->speed > 0.0f)
    {
      moves_per_sec = sqrt (w * w + h * h) * hints->speed / _RESAMPLER_R2;
    }
  else
    {
      moves_per_sec = rules->rounds_per_sec * rules->moves_per_round;
    }
  ret = estimated_nb_fighters * moves_per_sec + ((float) surface) * ((float) _RESAMPLER_BOGUS_FPS);

  return ret;
}

static void
_guess_moves_per_sec (lw6sys_context_t * sys_context, lw6map_rules_t * rules, const lw6ldr_hints_t * hints, int w, int h)
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
      for (i = rules->moves_per_round + 1; i <= LW6MAP_RULES_MAX_MOVES_PER_ROUND; ++i)
	{
	  if (i * rules->rounds_per_sec <= moves_per_sec)
	    {
	      rules->moves_per_round = i;
	    }
	}
      while (rules->moves_per_round * rules->rounds_per_sec > moves_per_sec && rules->rounds_per_sec > LW6MAP_RULES_MIN_ROUNDS_PER_SEC)
	{
	  rules->rounds_per_sec--;
	}
      while (rules->moves_per_round * rules->rounds_per_sec < moves_per_sec && rules->rounds_per_sec < LW6MAP_RULES_MAX_ROUNDS_PER_SEC)
	{
	  rules->rounds_per_sec++;
	}
      if (old_moves_per_round != rules->moves_per_round || old_rounds_per_sec != rules->rounds_per_sec)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("changing speed from %dx%d to %dx%d"), old_rounds_per_sec, old_moves_per_round, rules->rounds_per_sec, rules->moves_per_round);
	}
    }
  else
    {
      // nothing to do
    }
}

/**
 * lw6ldr_resampler_init
 *
 * @sys_context: global system context
 * @resampler: resampler object to init
 * @param: map parameters to use
 * @hints: loading hints
 * @source_w: width of source map
 * @source_h: height of source map
 * @display_w: width of source display
 * @display_h: height of source display
 * @target_ratio: ratio, that is width/height of the target
 * @bench_value: rough estimation of this computer power
 * @magic_number: arbitrary constant, needed to calibrate speed
 * @expected_depth: how thick the map could be (in practice, looks like d in whd)
 * @gray_level: used to estimate capacity, 1.0f is white and means many slots
 *
 * Initializes a resampler. There is wizardry based on the bench, magic number
 * map size, gray level. This is bot bullet proof, but has been experience
 * driven and is the result of many tries / failures and hopefully successes.
 * Might need tuning as the algorithm evolves. This is the very function that
 * chooses the actual logical map size.
 *
 * Return value: none.
 */
void
lw6ldr_resampler_init (lw6sys_context_t * sys_context, lw6ldr_resampler_t * resampler,
		       lw6map_param_t * param, const lw6ldr_hints_t * hints,
		       int source_w,
		       int source_h, int display_w, int display_h, float target_ratio, int bench_value, int magic_number, int expected_depth, float gray_level)
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
  int lin_bench_value;

  memset (resampler, 0, sizeof (lw6ldr_resampler_t));

  target_w = source_w;
  target_h = source_h;

  if (source_w > 0 && source_h > 0)
    {
      if (hints->resample)
	{
	  _check_limits (sys_context, hints, &target_w, &target_h);

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
	  if (hints->downsize_using_fighter_scale && tmp_w * tmp_h < target_w * target_h)
	    {
	      target_w = tmp_w;
	      target_h = tmp_h;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_
			  ("fighter_scale=%f on display %dx%d -> downsizing from %dx%d to %dx%d"),
			  hints->fighter_scale, display_w, display_h, source_w, source_h, target_w, target_h);
	    }
	  else if (hints->upsize_using_fighter_scale && tmp_w * tmp_h > target_w * target_h)
	    {
	      target_w = tmp_w;
	      target_h = tmp_h;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_
			  ("fighter_scale=%f on display %dx%d -> upsizing from %dx%d to %dx%d"),
			  hints->fighter_scale, display_w, display_h, source_w, source_h, target_w, target_h);
	    }

	  _check_limits (sys_context, hints, &target_w, &target_h);

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

	  _check_limits (sys_context, hints, &target_w, &target_h);

	  /*
	   * Step 3, take bench in account
	   */
	  lin_bench_value = lw6sys_math_log2lin (sys_context, bench_value, LW6MAP_LOG2LIN_BASE);
	  capacity_orig = ((float) lin_bench_value) * ((float) magic_number);
	  capacity = capacity_orig;
	  required = _estimate_capacity (sys_context, &param->rules, hints, target_w, target_h, expected_depth, gray_level);
	  if (required > 0.0f)
	    {
	      f = 1.0f;
	      if (hints->downsize_using_bench_value && capacity < required)
		{
		  while (capacity < required && tmp_w > LW6MAP_MIN_BODY_WIDTH && tmp_h > LW6MAP_MIN_BODY_HEIGHT)
		    {
		      f *= _RESAMPLER_DOWNSIZE;
		      tmp_w = target_w * f;
		      tmp_h = target_h * f;
		      required = _estimate_capacity (sys_context, &param->rules, hints, tmp_w, tmp_h, expected_depth, gray_level);
		    }
		  if (tmp_w != target_w || tmp_h != target_h)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_
				  ("required=%f capacity=%f bench_value=%d lin_bench_value=%d -> downsizing from %dx%d to %dx%d"),
				  required, capacity_orig, bench_value, lin_bench_value, target_w, target_h, tmp_w, tmp_h);
		      target_w = tmp_w;
		      target_h = tmp_h;
		    }
		}
	      else if (hints->upsize_using_bench_value && capacity > required)
		{
		  while (capacity < required && tmp_w < LW6MAP_MAX_BODY_WIDTH && tmp_h < LW6MAP_MAX_BODY_HEIGHT)
		    {
		      f *= _RESAMPLER_UPSIZE;
		      tmp_w = target_w * f;
		      tmp_h = target_h * f;
		      required = _estimate_capacity (sys_context, &param->rules, hints, tmp_w, tmp_h, expected_depth, gray_level);
		    }
		  if (tmp_w != target_w || tmp_h != target_h)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_
				  ("required=%f capacity=%f bench_value=%d -> upsizing from %dx%d to %dx%d"),
				  required, capacity_orig, bench_value, target_w, target_h, tmp_w, tmp_h);
		      target_w = tmp_w;
		      target_h = tmp_h;
		    }
		}
	    }

	  _check_limits (sys_context, hints, &target_w, &target_h);

	  /*
	   * Step 4, guess speed if needed
	   */
	  _guess_moves_per_sec (sys_context, &param->rules, hints, target_w, target_h);

	  /*
	   * Step 4, we check against map surface limits
	   */
	  if (target_w * target_h > hints->max_map_surface)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("target_w=%d target_h=%d hints->max_map_surface=%d -> downsizing"), target_w, target_h, hints->max_map_surface);
	      f = sqrt ((((float) target_w) * ((float) target_h)) / ((float) hints->max_map_surface));
	      target_w /= f;
	      target_h /= f;
	    }

	  if (target_w * target_h < hints->min_map_surface)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("target_w=%d target_h=%d hints->min_map_surface=%d -> upsizing"), target_w, target_h, hints->min_map_surface);
	      f = sqrt (((float) hints->min_map_surface) / (((float) target_w) * ((float) target_h)));
	      target_w *= f;
	      target_h *= f;
	    }

	  _check_limits (sys_context, hints, &target_w, &target_h);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to handle map with shape %dx%d"), source_w, source_h);
    }

  resampler->target_w = target_w;
  resampler->target_h = target_h;
  resampler->source_w = source_w;
  resampler->source_h = source_h;
  resampler->scale_x = ((float) (resampler->target_w)) / ((float) (resampler->source_w));
  resampler->scale_y = ((float) (resampler->target_h)) / ((float) (resampler->source_h));
}

/**
 * lw6ld_resample_use_for_gen
 *
 * @sys_context: global system context
 * @map_w: target map width (out param)
 * @map_h: target map height (out param)
 * @display_w: screen width (pixels)
 * @display_h: screen height (pixels)
 * @bench_value: rough estimation of this computer power
 * @magic_number: arbitrary constant, needed to calibrate speed
 *
 * Builds a resampler and does all the calculus so that one gets the
 * correct map width and height for the gen module. The idea is that
 * when generating a pseudo-random map, one can not really know what
 * size to give it, so this function gives a hint, relying on bench
 * and magic values, which are computer/runtime dependant.
 *
 * Return value: none
 */
void
lw6ldr_resampler_use_for_gen (lw6sys_context_t * sys_context, int *map_w, int *map_h, int display_w, int display_h, int bench_value, int magic_number)
{
  lw6map_param_t param;
  lw6ldr_hints_t hints;
  float target_ratio = 0.0f;
  lw6ldr_resampler_t resampler;

  display_w = lw6sys_imax (display_w, 1);
  display_h = lw6sys_imax (display_h, 1);

  lw6map_param_zero (sys_context, &param);
  lw6map_param_defaults (sys_context, &param);
  lw6ldr_hints_zero (sys_context, &hints);
  lw6ldr_hints_defaults (sys_context, &hints);

  target_ratio = ((float) display_w) / ((float) display_h);

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("using resampler for gen, display=%dx%d bench_value=%d"), display_w, display_h, bench_value);
  lw6ldr_resampler_init (sys_context, &resampler, &param, &hints, display_w, display_h,
			 display_w, display_h, target_ratio, bench_value,
			 magic_number, _RESAMPLER_USE_FOR_GEN_EXPECTED_DEPTH, _RESAMPLER_USE_FOR_GEN_GRAY_LEVEL);

  (*map_w) = resampler.target_w;
  (*map_h) = resampler.target_h;

  lw6map_param_clear (sys_context, &param);
  lw6ldr_hints_clear (sys_context, &hints);
}

static void
check_bounds (int *x, int *y, int w, int h)
{
  (*x) = lw6sys_imin ((*x), w - 1);
  (*y) = lw6sys_imin ((*y), h - 1);
  (*x) = lw6sys_imax ((*x), 0);
  (*y) = lw6sys_imax ((*y), 0);
}

/**
 * lw6ldr_resampler_force
 *
 * @sys_context: global system context
 * @resampler: resampler to set
 * @source_w: source map width
 * @source_h: source map height
 * @target_w: target map width
 * @target_h: target map height
 *
 * Initializes a resampler with hardcoded values, does not
 * calibrate according to context, simply set it to rescale
 * the size you want.
 *
 * Return value: none.
 */
void
lw6ldr_resampler_force (lw6sys_context_t * sys_context, lw6ldr_resampler_t * resampler, int source_w, int source_h, int target_w, int target_h)
{
  resampler->target_w = target_w;
  resampler->target_h = target_h;
  resampler->source_w = source_w;
  resampler->source_h = source_h;
  resampler->scale_x = ((float) (resampler->target_w)) / ((float) (resampler->source_w));
  resampler->scale_y = ((float) (resampler->target_h)) / ((float) (resampler->source_h));
}

/**
 * lw6ldr_resampler_source2target
 *
 * @sys_context: global system context
 * @resample: resampler to use
 * @target_x: target x coordinate (out param)
 * @target_y: target y coordinate (out param)
 * @source_x: source x coordinate (in param)
 * @source_y: source y coordinate (in param)
 *
 * Transforms from source coordinate to target coordinates.
 * Does rounding fine-tuning, it's not a simple integer division.
 *
 * Return value: none.
 */
void
lw6ldr_resampler_source2target (lw6sys_context_t * sys_context, const lw6ldr_resampler_t * resampler, int *target_x, int *target_y, int source_x, int source_y)
{
  (*target_x) = (int) (floor ((((float) source_x) + _RESAMPLER_05) * resampler->scale_x));
  (*target_y) = (int) (floor ((((float) source_y) + _RESAMPLER_05) * resampler->scale_y));
  check_bounds (target_x, target_y, resampler->target_w, resampler->target_h);
}

/**
 * lw6ldr_resampler_target2source
 *
 * @sys_context: global system context
 * @resample: resampler to use
 * @source_x: source x coordinate (out param)
 * @source_y: source y coordinate (out param)
 * @target_x: target x coordinate (in param)
 * @target_y: target y coordinate (in param)
 *
 * Transforms from target coordinate to source coordinates.
 * Yes, target to source. Target is our final logical map,
 * source is what we loaded from disk, here we want to know,
 * given a point in the target, where to fetch its data from source.
 * Does rounding fine-tuning, it's not a simple integer division.
 *
 * Return value: none.
 */
void
lw6ldr_resampler_target2source (lw6sys_context_t * sys_context, const lw6ldr_resampler_t * resampler, int *source_x, int *source_y, int target_x, int target_y)
{
  (*source_x) = (int) (floor ((((float) target_x) + _RESAMPLER_05) / resampler->scale_x));
  (*source_y) = (int) (floor ((((float) target_y) + _RESAMPLER_05) / resampler->scale_y));
  check_bounds (source_x, source_y, resampler->source_w, resampler->source_h);
}
