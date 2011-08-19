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

#include "gui.h"

/**
 * lw6gui_polarity_fix_float
 *
 * @x: x coord (in/out param)
 * @y: y coord (in/out param)
 * @x_flipped: flipped on x (out param)
 * @y_flipped: flipped on y (out param)
 * @w: width
 * @h: height
 * @x_polarity: x polarity (-1, 0 or 1)
 * @y_polarity: y polarity (-1, 0 or 1)
 *
 * Same as @lw6map_fix_coords except it operates on floats.
 * Usefull for cursor and other rendering operations. Additionnally,
 * will keep track of inversions, that is to say if map is flipped
 * in one or another way.
 *
 * Return value: none
 */
void lw6gui_polarity_fix_float(float *x, float *y, int *x_flipped, int *y_flipped, float w, float h,int x_polarity, int y_polarity)
{
      /*
       * The algorithm is copied/pasted from map code, only here it 
       * operates with floats, not integers.
       */
  (*x_flipped)=0;
  (*y_flipped)=0;

      if ((*x) < 0.0f || (*y) < 0.0f || (*x) > w || (*y) > h)
	{
	  if (x_polarity>0)
	    {
	      while ((*x) > w)
		{
		  (*x) -= w;
		}
	      while ((*x) < 0)
		{
		  (*x) += w;
		}
	    }
	  if (y_polarity>0)
	    {
	      while ((*y) > h)
		{
		  (*y) -= h;
		}
	      while ((*y) < 0)
		{
		  (*y) += h;
		}
	    }
	  if (x_polarity<0)
	    {
	      while ((*x) > w)
		{
		  (*x) -= w;
		  (*y) = h - 1 - (*y);
		  (*x_flipped)=!(*x_flipped);
		}
	      while ((*x) < 0)
		{
		  (*x) += w;
		  (*y) = h - 1 - (*y);
		  (*x_flipped)=!(*x_flipped);
		}
	    }
	  if (y_polarity<0)
	    {
	      while ((*y) > h)
		{
		  (*x) = w - 1 - (*x);
		  (*y) -= h;
		  (*y_flipped)=!(*y_flipped);
		}
	      while ((*y) < 0)
		{
		  (*x) = w - 1 - (*x);
		  (*y) += h;
		  (*y_flipped)=!(*y_flipped);
		}
	    }

	  /*
	   * Last, we check for absolute limits, this will also
	   * handle the 0 polarit(*y) case.
	   */
	  if ((*x) < 0.0f)
	    {
	      (*x) = 0.0f;
	    }
	  if ((*x) > w)
	    {
	      (*x) = w;
	    }
	  if ((*y) < 0.0f)
	    {
	      (*y) = 0.0f;
	    }
	  if ((*y) > h)
	    {
	      (*y) = h;
	    }
	}
}

