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
#endif // HAVE_CONFIG_H

#include "sys.h"
#include "sys-internal.h"

/**
 * lw6sys_progress_bind
 *
 * @sys_context: global system context
 * @progress: the progress struct to initialize
 * @value: the value to point to
 *
 * Sets a progress struct to default values, that is, ranging from 0.0f to 1.0f,
 * does not touch the value.
 *
 * Return value: none.
 */
void
lw6sys_progress_bind (lw6sys_context_t * sys_context, lw6sys_progress_t * progress, float *value)
{
  if (progress)
    {
      progress->value = value;
    }
}

/**
 * lw6sys_progress_default
 *
 * @sys_context: global system context
 * @progress: the progress struct to initialize
 * @value: the value to point to
 *
 * Sets a progress struct to default values, that is, ranging from 0.0f to 1.0f.
 * This function won't touch the value, one needs to call begin or update
 * or end to do that.
 *
 * Return value: none.
 */
void
lw6sys_progress_default (lw6sys_context_t * sys_context, lw6sys_progress_t * progress, float *value)
{
  if (progress)
    {
      progress->min = 0.0f;
      progress->max = 1.0f;
      lw6sys_progress_bind (sys_context, progress, value);
    }
}

/**
 * lw6sys_progress_update
 *
 * @sys_context: global system context
 * @progress: the progress struct to update
 * @min: the min value
 * @max: the max value
 * @value: the current value
 *
 * Updates a progress struct. This is typically the function used by a
 * callback to show the progress of a process. Note that this is note an
 * initializer. Rather, the progress struct was initialized before, and
 * this call is done in a loop with min being 0, max being the last value
 * in the loop, and value the current index in the loop. NULL pointers
 * correctly handled internally, so call this with any parameters, it's safe.
 *
 * Return value: none.
 */
void
lw6sys_progress_update (lw6sys_context_t * sys_context, lw6sys_progress_t * progress, int min, int max, int value)
{
  if (progress && progress->value && (max > min))
    {
      value = lw6sys_imax (min, value);
      value = lw6sys_imin (max, value);
      (*(progress->value)) = ((((float) value) - ((float) min)) / (((float) max) - ((float) min))) * (progress->max - progress->min) + progress->min;
      if ((*(progress->value)) < progress->min)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, "progress value too low min=%f max=%f value=%f", progress->min, progress->max, *(progress->value));
	  (*(progress->value)) = progress->min;
	}
      if ((*(progress->value)) > progress->max)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, "progress value too high min=%f max=%f value=%f", progress->min, progress->max, *(progress->value));
	  (*(progress->value)) = progress->max;
	}
    }
}

/**
 * lw6sys_progress_split
 *
 * @sys_context: global system context
 * @progress1: the first part of the splitted progress
 * @progress2: the second part of the splitted progress
 * @progress_src: the progress to split
 *
 * Utility function to split a progress struct, that is, if a progress was
 * ranging from a to b, make 2 progress structs, ranging from a to c and
 * from c to b, c being between a and b.
 *
 * Return value: none
 */
void
lw6sys_progress_split (lw6sys_context_t * sys_context, lw6sys_progress_t * progress1, lw6sys_progress_t * progress2, lw6sys_progress_t * progress_src)
{
  float middle;

  if (progress_src)
    {
      middle = (progress_src->min + progress_src->max) / 2.0f;
      if (progress1)
	{
	  progress1->min = progress_src->min;
	  progress1->max = middle;
	  progress1->value = progress_src->value;
	}
      if (progress2)
	{
	  progress2->min = middle;
	  progress2->max = progress_src->max;
	  progress2->value = progress_src->value;
	}
    }
  else
    {
      if (progress1)
	{
	  memset (progress1, 0, sizeof (lw6sys_progress_t));
	}
      if (progress2)
	{
	  memset (progress2, 0, sizeof (lw6sys_progress_t));
	}
    }
}

/**
 * lw6sys_progress_split_here
 *
 * @sys_context: global system context
 * @progress1: the first part of the splitted progress
 * @progress2: the second part of the splitted progress
 * @progress_src: the progress to split
 * @here: where to split
 *
 * Utility function to split a progress struct, that is, if a progress was
 * ranging from a to b, make 2 progress structs, ranging from a to c and
 * from c to b, c being between a and b. The here value controls what c
 * is. If here=0, then c=a. If here=1, then c=b.
 *
 * Return value: none
 */
void
lw6sys_progress_split_here (lw6sys_context_t * sys_context,
			    lw6sys_progress_t * progress1, lw6sys_progress_t * progress2, lw6sys_progress_t * progress_src, float here)
{
  float middle;

  if (progress_src)
    {
      middle = progress_src->min * (1.0f - here) + progress_src->max * here;
      if (middle < progress_src->min)
	{
	  middle = progress_src->min;
	}
      if (middle > progress_src->max)
	{
	  middle = progress_src->max;
	}
      if (progress1)
	{
	  progress1->min = progress_src->min;
	  progress1->max = middle;
	  progress1->value = progress_src->value;
	}
      if (progress2)
	{
	  progress2->min = middle;
	  progress2->max = progress_src->max;
	  progress2->value = progress_src->value;
	}
    }
  else
    {
      if (progress1)
	{
	  memset (progress1, 0, sizeof (lw6sys_progress_t));
	}
      if (progress2)
	{
	  memset (progress2, 0, sizeof (lw6sys_progress_t));
	}
    }
}

/**
 * lw6sys_progress_split3
 *
 * @sys_context: global system context
 * @progress1: the first part of the splitted progress
 * @progress2: the second part of the splitted progress
 * @progress3: the third part of the splitted progress
 * @progress_src: the progress to split
 *
 * Utility function to split a progress struct, this one will split it
 * into 3 equal parts.
 *
 * Return value: none
 */
void
lw6sys_progress_split3 (lw6sys_context_t * sys_context,
			lw6sys_progress_t * progress1, lw6sys_progress_t * progress2, lw6sys_progress_t * progress3, lw6sys_progress_t * progress_src)
{
  float limit_a, limit_b;

  if (progress_src)
    {
      limit_a = (2.0f * progress_src->min + progress_src->max) / 3.0f;
      limit_b = (progress_src->min + 2.0f * progress_src->max) / 3.0f;
      if (progress1)
	{
	  progress1->min = progress_src->min;
	  progress1->max = limit_a;
	  progress1->value = progress_src->value;
	}
      if (progress2)
	{
	  progress2->min = limit_a;
	  progress2->max = limit_b;
	  progress2->value = progress_src->value;
	}
      if (progress3)
	{
	  progress3->min = limit_b;
	  progress3->max = progress_src->max;
	  progress3->value = progress_src->value;
	}
    }
  else
    {
      if (progress1)
	{
	  memset (progress1, 0, sizeof (lw6sys_progress_t));
	}
      if (progress2)
	{
	  memset (progress2, 0, sizeof (lw6sys_progress_t));
	}
      if (progress3)
	{
	  memset (progress3, 0, sizeof (lw6sys_progress_t));
	}
    }
}

/**
 * lw6sys_progress_split4
 *
 * @sys_context: global system context
 * @progress1: the first part of the splitted progress
 * @progress2: the second part of the splitted progress
 * @progress3: the third part of the splitted progress
 * @progress4: the fourth part of the splitted progress
 * @progress_src: the progress to split
 *
 * Utility function to split a progress struct, this one will split it
 * into 4 equal parts.
 *
 * Return value: none
 */
void
lw6sys_progress_split4 (lw6sys_context_t * sys_context,
			lw6sys_progress_t * progress1,
			lw6sys_progress_t * progress2, lw6sys_progress_t * progress3, lw6sys_progress_t * progress4, lw6sys_progress_t * progress_src)
{
  float limit_a, limit_b, limit_c;

  if (progress_src)
    {
      limit_a = (3.0f * progress_src->min + progress_src->max) / 4.0f;
      limit_b = (2.0f * progress_src->min + 2.0f * progress_src->max) / 4.0f;
      limit_c = (progress_src->min + 3.0f * progress_src->max) / 4.0f;
      if (progress1)
	{
	  progress1->min = progress_src->min;
	  progress1->max = limit_a;
	  progress1->value = progress_src->value;
	}
      if (progress2)
	{
	  progress2->min = limit_a;
	  progress2->max = limit_b;
	  progress2->value = progress_src->value;
	}
      if (progress3)
	{
	  progress3->min = limit_b;
	  progress3->max = limit_c;
	  progress3->value = progress_src->value;
	}
      if (progress4)
	{
	  progress4->min = limit_c;
	  progress4->max = progress_src->max;
	  progress4->value = progress_src->value;
	}
    }
  else
    {
      if (progress1)
	{
	  memset (progress1, 0, sizeof (lw6sys_progress_t));
	}
      if (progress2)
	{
	  memset (progress2, 0, sizeof (lw6sys_progress_t));
	}
      if (progress3)
	{
	  memset (progress3, 0, sizeof (lw6sys_progress_t));
	}
      if (progress4)
	{
	  memset (progress4, 0, sizeof (lw6sys_progress_t));
	}
    }
}

/**
 * lw6sys_progress_split5
 *
 * @sys_context: global system context
 * @progress1: the first part of the splitted progress
 * @progress2: the second part of the splitted progress
 * @progress3: the third part of the splitted progress
 * @progress4: the fourth part of the splitted progress
 * @progress5: the fourth part of the splitted progress
 * @progress_src: the progress to split
 *
 * Utility function to split a progress struct, this one will split it
 * into 5 equal parts.
 *
 * Return value: none
 */
void
lw6sys_progress_split5 (lw6sys_context_t * sys_context,
			lw6sys_progress_t * progress1,
			lw6sys_progress_t * progress2,
			lw6sys_progress_t * progress3, lw6sys_progress_t * progress4, lw6sys_progress_t * progress5, lw6sys_progress_t * progress_src)
{
  float limit_a, limit_b, limit_c, limit_d;

  if (progress_src)
    {
      limit_a = (4.0f * progress_src->min + progress_src->max) / 5.0f;
      limit_b = (3.0f * progress_src->min + 2.0f * progress_src->max) / 5.0f;
      limit_c = (2.0f * progress_src->min + 3.0f * progress_src->max) / 5.0f;
      limit_d = (progress_src->min + 4.0f * progress_src->max) / 5.0f;
      if (progress1)
	{
	  progress1->min = progress_src->min;
	  progress1->max = limit_a;
	  progress1->value = progress_src->value;
	}
      if (progress2)
	{
	  progress2->min = limit_a;
	  progress2->max = limit_b;
	  progress2->value = progress_src->value;
	}
      if (progress3)
	{
	  progress3->min = limit_b;
	  progress3->max = limit_c;
	  progress3->value = progress_src->value;
	}
      if (progress4)
	{
	  progress4->min = limit_c;
	  progress4->max = limit_d;
	  progress4->value = progress_src->value;
	}
      if (progress5)
	{
	  progress5->min = limit_d;
	  progress5->max = progress_src->max;
	  progress5->value = progress_src->value;
	}
    }
  else
    {
      if (progress1)
	{
	  memset (progress1, 0, sizeof (lw6sys_progress_t));
	}
      if (progress2)
	{
	  memset (progress2, 0, sizeof (lw6sys_progress_t));
	}
      if (progress3)
	{
	  memset (progress3, 0, sizeof (lw6sys_progress_t));
	}
      if (progress4)
	{
	  memset (progress4, 0, sizeof (lw6sys_progress_t));
	}
      if (progress5)
	{
	  memset (progress5, 0, sizeof (lw6sys_progress_t));
	}
    }
}

/**
 * lw6sys_progress_begin
 *
 * @sys_context: global system context
 * @progress: the progress to update
 *
 * Sets the progress to its min value, NULL values correctly handled.
 *
 * Return value: none
 */
void
lw6sys_progress_begin (lw6sys_context_t * sys_context, lw6sys_progress_t * progress)
{
  if (progress && progress->value)
    {
      (*(progress->value)) = progress->min;
    }
}

/**
 * lw6sys_progress_half
 *
 * @sys_context: global system context
 * @progress: the progress to update
 *
 * Sets the progress to the average between min and max,
 * NULL values correctly handled.
 *
 * Return value: none
 */
void
lw6sys_progress_half (lw6sys_context_t * sys_context, lw6sys_progress_t * progress)
{
  if (progress && progress->value)
    {
      (*(progress->value)) = (progress->min + progress->max) / 2.0f;
    }
}

/**
 * lw6sys_progress_end
 *
 * @sys_context: global system context
 * @progress: the progress to update
 *
 * Sets the progress to its max value, NULL values correctly handled.
 *
 * Return value: none
 */
void
lw6sys_progress_end (lw6sys_context_t * sys_context, lw6sys_progress_t * progress)
{
  if (progress && progress->value)
    {
      (*(progress->value)) = progress->max;
    }
}
